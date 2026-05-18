#ifndef _LIBSDP_EMAIL_ADDRESS_HH_
#define _LIBSDP_EMAIL_ADDRESS_HH_
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
#include <string>

#include <rtsdp/common.hh>

LIBRTSDP_NAMESPACE_START

class EmailAddress {
public:
    EmailAddress() :m_emailAddress() {};
    EmailAddress(const std::string &email_address) :m_emailAddress(email_address) { validate(email_address); };
    EmailAddress(std::string &&email_address) :m_emailAddress() { validate(email_address); m_emailAddress = std::move(email_address); };
    EmailAddress(const EmailAddress &other) :m_emailAddress(other.m_emailAddress) {};
    EmailAddress(EmailAddress &&other) :m_emailAddress(std::move(other.m_emailAddress)) {};

    virtual ~EmailAddress() {};

    EmailAddress &operator=(const std::string &other) { validate(other); m_emailAddress = other; return *this; };
    EmailAddress &operator=(std::string &&other) { validate(other); m_emailAddress = std::move(other); return *this; };
    EmailAddress &operator=(const EmailAddress &other) { m_emailAddress = other.m_emailAddress; return *this; };
    EmailAddress &operator=(EmailAddress &&other) { m_emailAddress = std::move(other.m_emailAddress); return *this; };

    bool operator==(const EmailAddress &other) const { return m_emailAddress == other.m_emailAddress; };

    std::size_t hash() const { return std::hash<std::string>{}(m_emailAddress); };
    operator std::string() const { return m_emailAddress.empty()?std::string():std::format("e={}\r\n", m_emailAddress); };

    const std::string &str() const { return m_emailAddress; };

    EmailAddress &clear() { m_emailAddress.clear(); return *this; };

private:
    static void validate(const std::string &email_address);

    std::string m_emailAddress;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(EmailAddress), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(EmailAddress) &email_address, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(email_address));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(EmailAddress)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(EmailAddress) &email_address) const { return email_address.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(EmailAddress) &email_address);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_EMAIL_ADDRESS_HH_ */
