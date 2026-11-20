#ifndef _HTTPXPP_HASH_HH_
#define _HTTPXPP_HASH_HH_
/*****************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: SHA256 hashing function
 *****************************************************************************
 * Copyright: (C)2024-2026 British Broadcasting Corporation
 * Author(s): Dev Audsin <dev.audsin@bbc.co.uk>
 *            David Waring <david.waring2@bbc.co.uk>
 * License: 5G-MAG Public License (v1.0)
 *
 * Licensed under the License terms and conditions for use, reproduction, and
 * distribution of 5G-MAG software (the “License”).  You may not use this file
 * except in compliance with the License.  The license may be found in the
 * LICENSE file accompanying this software or you may obtain a copy of the
 * License at https://www.5g-mag.com/reference-tools.  Unless required by
 * applicable law or agreed to in writing, software distributed under the
 * License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <string>
#include <vector>

#include <gnutls/gnutls.h>
#include <gnutls/crypto.h>

#include "common.hh"

HTTPXPP_NAMESPACE_START

template <class T, class A>
std::string calculate_hash(const std::vector<T,A> &buf)
{
    size_t result_len = gnutls_hash_get_len(GNUTLS_DIG_SHA256);
    unsigned char result[result_len];
    gnutls_datum_t data = {
        .data = reinterpret_cast<unsigned char*>(const_cast<T*>(buf.data())),
        .size = static_cast<unsigned int>(sizeof(T) * buf.size())
    };
    char hash[result_len*2 + 1];

    gnutls_fingerprint(GNUTLS_DIG_SHA256, &data, result, &result_len);
    for (size_t i = 0; i < result_len; i++)
    {
        sprintf(hash+i*2, "%02x", result[i]);
    }

    return std::string(hash);
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _HTTPXPP_HASH_HH_ */
