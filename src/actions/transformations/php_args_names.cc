/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/actions/transformations/php_args_names.h"

#include <algorithm>
#include <string>
#include <string.h>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"
#include "modsecurity/actions/action.h"

namespace modsecurity {
namespace actions {
namespace transformations {


PhpArgsNames::PhpArgsNames(const std::string &a)
    : Transformation(a) {
}

std::string PhpArgsNames::evaluate(const std::string &val,
    Transaction *transaction) {
    //Took the logic from php src code:
    //https://github.com/php/php-src/blob/master/main/php_variables.c
    //Function call PHPAPI void php_register_variable_ex(const char *var_name, zval *val, zval *track_vars_array)
    std::string value(val);
    std::string ret = "";
    if(value[0] == '[' || value[0] == '=') {
	return ret;
    }
    std::string::size_type i = 0;
    while(value[i] == ' ') {
	    i++;
    }
    std::string::size_type val_size = value.length();
    bool is_array = false;
    bool is_open_sq_bracket = false;
    for (; i < val_size; ++i) {
	    if(value[i] == '[' && !is_open_sq_bracket) {
		    if(strchr(&value[i], ']') != NULL) {
			    is_array = true;
			    break;
		    }

		    ret +=  '_';
		    is_open_sq_bracket = true;
	    }
	    else if( !is_open_sq_bracket && (value[i] == ' ' || value[i] == '.') ) {
		    ret += '_';
	    }
	    else {
		ret += value[i];
	    }
    }

    if(is_array) {
	    char* start = &value[0];
	    while(true) {
		char *tmp = &value[i];
		char *close_bra = strchr(tmp, ']');
		if(close_bra == NULL) {
			return ret;
		}
		int array_size = (int)(close_bra - start) + 1;
		if(array_size - i == 3 && value[i+1] == ' ') {
			ret += '[';
			i+=2;
		}
		for(;i < array_size; ++i) {
			ret += value[i];
		}
		if(i >= val_size || value[i] != '[') {
			return ret;
		}
	    }
    }
    return ret;

}

}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
