#ifndef _HTTPXPP_CASE_INSENSITIVE_TRAITS_HH_
#define _HTTPXPP_CASE_INSENSITIVE_TRAITS_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: CaseInsensitiveTraits class
 ******************************************************************************
 * Copyright: (C)2026 British Broadcasting Corporation
 * Author(s): David Waring <david.waring2@bbc.co.uk>
 * License: 5G-MAG Public License v1
 *
 * Licensed under the License terms and conditions for use, reproduction, and
 * distribution of 5G-MAG software (the “License”).  You may not use this file
 * except in compliance with the License.  You may obtain a copy of the License at
 * https://www.5g-mag.com/reference-tools.  Unless required by applicable law or
 * agreed to in writing, software distributed under the License is distributed on
 * an “AS IS” BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied.
 *
 * See the License for the specific language governing permissions and limitations
 * under the License.
 */

#include <cctype>
#include <string>

#include "common.hh"

HTTPXPP_NAMESPACE_START

template <class CharT>
class CaseInsensitiveTraits : public std::char_traits<CharT>
{
public:
    static bool eq(const CharT &a, const CharT &b) {
        //ogs_debug("%c == %c? = %s", a, b, (std::tolower(a) == std::tolower(b))?"true":"false");
        return std::tolower(a) == std::tolower(b);
    };
    static bool lt(const CharT &a, const CharT &b) {
        //ogs_debug("%c < %c? = %s", a, b, (std::tolower(a) < std::tolower(b))?"true":"false");
        return std::tolower(a) < std::tolower(b);
    };
    static int compare(const CharT* s1, const CharT* s2, std::size_t n)
    {
        //ogs_debug("%s <=> %s [len=%zi]", s1, s2, n);
        for (std::size_t i = 0; i < n; ++i)
            if (lt(s1[i], s2[i]))
                return -1;
            else if (lt(s2[i], s1[i]))
                return 1;
        return 0;
    };
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif // _HTTPXPP_CASE_INSENSITIVE_TRAITS_HH_
