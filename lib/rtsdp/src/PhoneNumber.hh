#ifndef _LIBSDP_PHONE_NUMBER_HH_
#define _LIBSDP_PHONE_NUMBER_HH_
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
#include <list>
#include <string>

#include <rtsdp/common.hh>

LIBRTSDP_NAMESPACE_START

class PhoneNumber {
public:
    PhoneNumber() :m_phoneNumber() {};
    PhoneNumber(const std::string &phone_number) :m_phoneNumber(phone_number) { validate(phone_number); };
    PhoneNumber(std::string &&phone_number) :m_phoneNumber() { validate(phone_number); m_phoneNumber = std::move(phone_number); };
    PhoneNumber(const PhoneNumber &other) :m_phoneNumber(other.m_phoneNumber) {};
    PhoneNumber(PhoneNumber &&other) :m_phoneNumber(std::move(other.m_phoneNumber)) {};

    virtual ~PhoneNumber() {};

    PhoneNumber &operator=(const std::string &other) { validate(other); m_phoneNumber = other; return *this; };
    PhoneNumber &operator=(std::string &&other) { validate(other); m_phoneNumber = std::move(other); return *this; };
    PhoneNumber &operator=(const PhoneNumber &other) { m_phoneNumber = other.m_phoneNumber; return *this; };
    PhoneNumber &operator=(PhoneNumber &&other) { m_phoneNumber = std::move(other.m_phoneNumber); return *this; };

    bool operator==(const PhoneNumber &other) const { return m_phoneNumber == other.m_phoneNumber; };

    std::size_t hash() const { return std::hash<std::string>{}(m_phoneNumber); };
    operator std::string() const { return m_phoneNumber.empty()?std::string():std::format("p={}\r\n", m_phoneNumber); };

    const std::string &str() const { return m_phoneNumber; };

    PhoneNumber &clear() { m_phoneNumber.clear(); return *this; };

private:
    static void validate(const std::string &phone_number);

    std::string m_phoneNumber;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(PhoneNumber), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(PhoneNumber) &phone_number, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(phone_number));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(PhoneNumber)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(PhoneNumber) &phone_number) const { return phone_number.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(PhoneNumber) &phone_number);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_PHONE_NUMBER_HH_ */
