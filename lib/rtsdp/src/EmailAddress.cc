/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: EmailAddress class
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

#include <format>
#include <iostream>
#include <list>
#include <regex>
#include <stdexcept>
#include <string>

#include <rtsdp/common.hh>

#include "EmailAddress.hh"

LIBRTSDP_NAMESPACE_START

#define dot_atom "[-A-Za-z0-9!#$%&'*+/=?^_`{|}~]{1,}(\\.[-A-Za-z0-9!#$%&'*+/=?^_`{|}~]{1,})*"
#define domain "\\[[-!\"#$%&'()*+,./0-9:;<=>?@A-Z^_`a-z{|}]*\\]"
#define quoted_string "\"([-!#$%&'()*+,./0-9:;<=>?@A-Z[^_`a-z{|}~]|\\]|\\\\([-!\"#$%&'()*+,./0-9:;<=>?@A-Z[\\^_`a-z{|}~]|\\]))*\""
void EmailAddress::validate(const std::string &email_address)
{
    if (email_address.empty()) return;
    static const std::regex email_validate_re("(" dot_atom "|" quoted_string ")@(" dot_atom "|" domain ")");
    if (!std::regex_match(email_address, email_validate_re))
        throw std::out_of_range(std::format("{} is not a valid email address", email_address));
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(EmailAddress) &email_address)
{
    os << static_cast<std::string>(email_address);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
