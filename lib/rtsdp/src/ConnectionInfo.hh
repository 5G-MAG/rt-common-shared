#ifndef _LIBSDP_CONNECTION_INFO_HH_
#define _LIBSDP_CONNECTION_INFO_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: ConnectionInfo class
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

#include <chrono>
#include <format>
#include <iostream>
#include <optional>
#include <string>

#include <rtsdp/common.hh>
#include "Address.hh"

LIBRTSDP_NAMESPACE_START

class ConnectionInfo {
public:
    ConnectionInfo();
    ConnectionInfo(const Address &address, const std::optional<uint8_t> &ttl = std::nullopt,
                   unsigned int consecutive_addresses = 1);
    ConnectionInfo(const ConnectionInfo &other);
    ConnectionInfo(ConnectionInfo &&other);

    virtual ~ConnectionInfo() {};

    ConnectionInfo &operator=(const ConnectionInfo &other);
    ConnectionInfo &operator=(ConnectionInfo &&other);

    bool operator==(const ConnectionInfo &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const { return isValid(); };    

    bool isValid() const;

private:
    Address m_address;
    std::optional<uint8_t> m_ttl;
    unsigned int m_consecutiveAddresses;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(ConnectionInfo), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(ConnectionInfo) &connection_info, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(connection_info));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(ConnectionInfo)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(ConnectionInfo) &connection_info) const { return connection_info.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(ConnectionInfo) &connection_info);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_CONNECTION_INFO_HH_ */
