/**
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
namespace modsecurity {
namespace utils {
namespace crypto {
#endif

/**
 * \brief           Take a raw element and hex encode it
 *
 * \param obuf      The output hex encoded value
 *
 * \param ibuf      The raw input buffer
 *
 * \param len       The length of the raw input
 */
std::string md5( std::string input );


#ifdef __cplusplus
}  // namespace crypto
}  // namespace utils
}  // namespace modsecurity
#endif

#ifdef __cplusplus
}
#endif

