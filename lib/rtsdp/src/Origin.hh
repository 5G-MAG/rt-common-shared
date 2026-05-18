#ifndef _LIBSDP_ORIGIN_HH_
#define _LIBSDP_ORIGIN_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: Origin class
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
#include <string>

#include <rtsdp/common.hh>
#include "Address.hh"

LIBRTSDP_NAMESPACE_START

class Origin {
public:
    Origin();
    Origin(const Address &origin_address, const std::string &user = std::string());
    Origin(const Origin &other);
    Origin(Origin &&other);

    virtual ~Origin() {};

    Origin &operator=(const Origin &other);
    Origin &operator=(Origin &&other);

    bool operator==(const Origin &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const { return isValid(); };

    bool isValid() const;

    void updateVersion();

    const std::string &username() const { return m_username; };
    Origin &username(const std::string &val) { m_username = val; return *this; };
    Origin &username(std::string &&val) { m_username = std::move(val); return *this; };
    Origin &usernameClear() { m_username.clear(); return *this; };

    uint64_t sessionId() const { return m_sessionId; };
    Origin &sessionId(uint64_t val) { m_sessionId = val; return *this; };

    uint64_t sessionVersion() const { return m_sessionVersion; };
    Origin &sessionVersion(uint64_t val) { m_sessionVersion = val; return *this; };

    const Address &originAddress() const { return m_originAddress; };
    Origin &originAddress(const Address &val) { m_originAddress = val; return *this; };
    Origin &originAddress(Address &&val) { m_originAddress = std::move(val); return *this; };

private:
    std::string m_username;
    uint64_t m_sessionId;
    uint64_t m_sessionVersion;
    Address m_originAddress;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(Origin), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(Origin) &origin, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(origin));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(Origin)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(Origin) &origin) const { return origin.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(Origin) &origin);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_ORIGIN_HH_ */
