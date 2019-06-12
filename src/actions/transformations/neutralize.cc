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

#include "src/actions/transformations/neutralize.h"

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

#define IS_B64       0x01
#define IS_B64I      0x02
#define IS_HEX_UP    0x04
#define IS_HEX_DOWN  0x08

namespace modsecurity {
namespace actions {
namespace transformations {

static const unsigned char char_enc[256] = {
	[0x00] = 0,
	[0x01] = 0,
	[0x02] = 0,
	[0x03] = 0,
	[0x04] = 0,
	[0x05] = 0,
	[0x06] = 0,
	[0x07] = 0,
	[0x08] = 0,
	[0x09] = 0,
	[0x0a] = 0,
	[0x0b] = 0,
	[0x0c] = 0,
	[0x0d] = 0,
	[0x0e] = 0,
	[0x0f] = 0,
	[0x10] = 0,
	[0x11] = 0,
	[0x12] = 0,
	[0x13] = 0,
	[0x14] = 0,
	[0x15] = 0,
	[0x16] = 0,
	[0x17] = 0,
	[0x18] = 0,
	[0x19] = 0,
	[0x1a] = 0,
	[0x1b] = 0,
	[0x1c] = 0,
	[0x1d] = 0,
	[0x1e] = 0,
	[0x1f] = 0,
	[0x20] = 0,
	[0x21] = 0,
	[0x22] = 0,
	[0x23] = 0,
	[0x24] = 0,
	[0x25] = 0,
	[0x26] = 0,
	[0x27] = 0,
	[0x28] = 0,
	[0x29] = 0,
	[0x2a] = 0,
	['+']  = IS_B64,
	[0x2c] = 0,
	['-']  = IS_B64I,
	[0x2e] = 0,
	['/']  = IS_B64,
	['0']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['1']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['2']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['3']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['4']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['5']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['6']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['7']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['8']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	['9']  = IS_B64 | IS_B64I | IS_HEX_UP | IS_HEX_DOWN,
	[0x3a] = 0,
	[0x3b] = 0,
	[0x3c] = 0,
	['=']  = IS_B64,
	[0x3e] = 0,
	[0x3f] = 0,
	[0x40] = 0,
	['A']  = IS_B64 | IS_B64I | IS_HEX_UP,
	['B']  = IS_B64 | IS_B64I | IS_HEX_UP,
	['C']  = IS_B64 | IS_B64I | IS_HEX_UP,
	['D']  = IS_B64 | IS_B64I | IS_HEX_UP,
	['E']  = IS_B64 | IS_B64I | IS_HEX_UP,
	['F']  = IS_B64 | IS_B64I | IS_HEX_UP,
	['G']  = IS_B64 | IS_B64I,
	['H']  = IS_B64 | IS_B64I,
	['I']  = IS_B64 | IS_B64I,
	['J']  = IS_B64 | IS_B64I,
	['K']  = IS_B64 | IS_B64I,
	['L']  = IS_B64 | IS_B64I,
	['M']  = IS_B64 | IS_B64I,
	['N']  = IS_B64 | IS_B64I,
	['O']  = IS_B64 | IS_B64I,
	['P']  = IS_B64 | IS_B64I,
	['Q']  = IS_B64 | IS_B64I,
	['R']  = IS_B64 | IS_B64I,
	['S']  = IS_B64 | IS_B64I,
	['T']  = IS_B64 | IS_B64I,
	['U']  = IS_B64 | IS_B64I,
	['V']  = IS_B64 | IS_B64I,
	['W']  = IS_B64 | IS_B64I,
	['X']  = IS_B64 | IS_B64I,
	['Y']  = IS_B64 | IS_B64I,
	['Z']  = IS_B64 | IS_B64I,
	[0x5b] = 0,
	[0x5c] = 0,
	[0x5d] = 0,
	[0x5e] = 0,
	['_']  = IS_B64I,
	[0x60] = 0,
	['a']  = IS_B64 | IS_B64I | IS_HEX_DOWN,
	['b']  = IS_B64 | IS_B64I | IS_HEX_DOWN,
	['c']  = IS_B64 | IS_B64I | IS_HEX_DOWN,
	['d']  = IS_B64 | IS_B64I | IS_HEX_DOWN,
	['e']  = IS_B64 | IS_B64I | IS_HEX_DOWN,
	['f']  = IS_B64 | IS_B64I | IS_HEX_DOWN,
	['g']  = IS_B64 | IS_B64I,
	['h']  = IS_B64 | IS_B64I,
	['i']  = IS_B64 | IS_B64I,
	['j']  = IS_B64 | IS_B64I,
	['k']  = IS_B64 | IS_B64I,
	['l']  = IS_B64 | IS_B64I,
	['m']  = IS_B64 | IS_B64I,
	['n']  = IS_B64 | IS_B64I,
	['o']  = IS_B64 | IS_B64I,
	['p']  = IS_B64 | IS_B64I,
	['q']  = IS_B64 | IS_B64I,
	['r']  = IS_B64 | IS_B64I,
	['s']  = IS_B64 | IS_B64I,
	['t']  = IS_B64 | IS_B64I,
	['u']  = IS_B64 | IS_B64I,
	['v']  = IS_B64 | IS_B64I,
	['w']  = IS_B64 | IS_B64I,
	['x']  = IS_B64 | IS_B64I,
	['y']  = IS_B64 | IS_B64I,
	['z']  = IS_B64 | IS_B64I,
	/* all other initilized to 0 */
};

std::string NeutralizeSafeEnc::evaluate(std::string value,
                                        Transaction *transaction) {
	int is_b64 = 1;
	int is_b64i = 1;
	int is_hex_up = 1;
	int is_hex_down = 1;
	int is_final = 0;
	unsigned int flags;
	const unsigned char *string;
	char *new_string;
	int len;
    std::string ret;
	int i;

	len = value.length();
	string = (const unsigned char *)value.c_str();
	for (i = 0; i < len; i++) {
		flags = char_enc[string[i]];
		is_b64      = is_b64      && (flags & IS_B64);
		is_b64i     = is_b64i     && (flags & IS_B64I);
		is_hex_up   = is_hex_up   && (flags & IS_HEX_UP);
		is_hex_down = is_hex_down && (flags & IS_HEX_DOWN);

		/* special base64 case for '='. '=' is only final char. if
		 * it is encountered in other place than end, base64 is
		 * not valid.
		 */
		if (is_b64) {
			if (string[i] == '=')
				is_final = 1;
			else if (is_final)
				is_b64 = 0;
		}
	}

	/* Control that base64 encoding was 4 bytes based */
	if (is_b64 && len % 4 != 0)
		is_b64 = 0;

	/* Control that hexa encoding was pair value */
	if ((is_hex_up || is_hex_down) && len % 2 != 0) {
		is_hex_up = 0;
		is_hex_down = 0;
	}

	/* If we not detect known valid encoding, return original string */
	if (!is_b64 && !is_b64i && !is_hex_up && !is_hex_down)
		return value;

	/* We detect valid safe encoding, replace the string by safe character.
	 * If memory allocation fails, try to continue processing returning same
	 * string than input.
	 */
	new_string = (char *)malloc(sizeof(char) * len + 1);
	if (new_string == NULL)
		return value;

	/* fill new string with safe char */
	memset(new_string, 'a', len);
	new_string[len] = '\0';

	/* set escape signature for debug log */
	if (is_hex_up)
		memcpy(new_string, "hexup", len >= 5 ? 5 : len);
	else if (is_hex_down)
		memcpy(new_string, "hexdw", len >= 5 ? 5 : len);
	else if (is_b64i)
		memcpy(new_string, "b64i", len >= 4 ? 4 : len);
	else if (is_b64)
		memcpy(new_string, "b64", len >= 3 ? 3 : len);

	/* assign new string to return value */
	ret.assign(new_string, len);
	free(new_string);
    return ret;
}

}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
