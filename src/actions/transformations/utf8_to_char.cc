/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2019 OZON, Inc. (https://www.ozon.io)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include "src/actions/transformations/utf8_to_char.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <cstring>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"
#include "src/utils/string.h"

namespace modsecurity {
namespace actions {
namespace transformations {

/* Return 0 if the buffer is too short or the expected byte is not utf8 part.
 * Otherwise return 1. Concatenate the 6 bits extracted form the partial utf8 byte
 * at the end of the "value"
 */
static inline char get_simple_char(const unsigned char *in, size_t *i, unsigned int l,
                                   unsigned long long int *value) {
	if (*i >= l)
		return 0;
	if ((in[*i] & 0xc0) != 0x80)
		return 0;
	*value = (*value << 6) | (in[*i] & 0x3f);
	(*i)++;
	return 1;
}

std::string Utf8ToChar::evaluate(std::string value,
                                 Transaction *transaction) {
	unsigned char *new_string;
	int len;
	const unsigned char *input;
	unsigned long long int character;
	size_t i;
	size_t o;
    std::string ret;

	/* Get a lot of memory in order to copy text, removing UTF8.
	 * each valid utf8 character (including forbidden ranges and
	 * overlong encoding) are replaced by corresponding byte if
	 * the value is < 0xff, or neutral char like 'a' if the value
	 * is > 0xff. The produced string is never longer than the
	 * input.
	 */
	len = value.length();
	new_string = (unsigned char *)malloc(sizeof(unsigned char) * len + 1);
	if (new_string == NULL)
		return value;

	input = (const unsigned char *)value.c_str();
	i = 0;
	o = 0;
	while (1) {

		if (i >= len)
			break;

		if (!(input[i] & 0x80)) {

			/* Special case, if we encounter a 0x00, break the transformation */
			if (input[i] == 0x00) {
				free(new_string);
				return value;
			}
			character = input[i];
			i++;

		} else if ((input[i] & 0xe0) == 0xc0) {

			/* Expect onr more bytes */
			if (i + 1 >= len) {
				free(new_string);
				return value;
			}
			character = input[i] & 0x1f;
			i++;
			if (!get_simple_char(input, &i, len, &character)) {
				free(new_string);
				return value;
			}

		} else if ((input[i] & 0xf0) == 0xe0) {

			/* Expect two more bytes */
			if (i + 2 >= len) {
				free(new_string);
				return value;
			}
			character = input[i] & 0x0f;
			i++;
			if (!get_simple_char(input, &i, len, &character)) {
				free(new_string);
				return value;
			}
			if (!get_simple_char(input, &i, len, &character)) {
				free(new_string);
				return value;
			}

		} else if ((input[i] & 0xf8) == 0xf0) {

			/* Expect three more bytes */
			if (i + 3 >= len) {
				free(new_string);
				return value;
			}
			character = input[i] & 0x07;
			i++;
			if (!get_simple_char(input, &i, len, &character)) {
				free(new_string);
				return value;
			}
			if (!get_simple_char(input, &i, len, &character)) {
				free(new_string);
				return value;
			}
			if (!get_simple_char(input, &i, len, &character)) {
				free(new_string);
				return value;
			}

		} else {

			/* Malformed message */
			free(new_string);
			return value;

		}

		/* Copy character. If < 0xff, copy it as is, otherwise
		 * we considers valid character and copy 'a'
		 */
		if (o >= len) {
			free(new_string);
			return value;
		}
		if (character == 0x00) {
			/* Special case, if we encounter a 0x00, break the transformation */
			free(new_string);
			return value;
		} else if (character <= 0xff) {
			/* copy character as is decoded */
			new_string[o] = character;
		} else {
			/* replace utf8 character by "a" in order to analyse 8-bit string. */
			new_string[o] = 'a';
		}
		o++;
	}

	/* Set final null char and duplicate value */
	if (o > len) {
		free(new_string);
		return value;
	}
	new_string[o] = '\0';


	/* assign new string to return value */
	ret.assign((const char *)new_string, o);
	free(new_string);
    return ret;
}

}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
