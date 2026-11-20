/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: PhoneNumber class
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
#include <regex>
#include <string>

#include <rtsdp/common.hh>

#include "PhoneNumber.hh"

LIBRTSDP_NAMESPACE_START

void PhoneNumber::validate(const std::string &phone_number)
{
    if (phone_number.empty()) return;
    static const std::regex phone_number_re("\\+\\s*[1-9][0-9]*(\\s+[0-9]+)*");
    if (!std::regex_match(phone_number, phone_number_re))
        throw std::out_of_range(std::format("{} does not match the format of a phone number (+<country-code><phone-number>)", phone_number));
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(PhoneNumber) &phone_number)
{
    os << static_cast<std::string>(phone_number);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
