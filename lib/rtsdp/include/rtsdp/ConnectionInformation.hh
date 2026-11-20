#ifndef _LIBRTSDP_PUB_CONNECTION_INFORMATION_HH_
#define _LIBRTSDP_PUB_CONNECTION_INFORMATION_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: ConnectionInformation class
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

#include <netinet/in.h>

#include <format>
#include <iostream>
#include <memory>
#include <string>

#include "common.hh"

LIBRTSDP_NAMESPACE_START

class ConnectionInformation {
public:
    static std::shared_ptr<ConnectionInformation> makeConnectionInformation();
    static std::shared_ptr<ConnectionInformation> makeConnectionInformation(const std::string &hostname, int family = AF_UNSPEC, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    static std::shared_ptr<ConnectionInformation> makeConnectionInformation(const struct sockaddr_in &sin, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    static std::shared_ptr<ConnectionInformation> makeConnectionInformation(const struct sockaddr_in6 &sin6, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    static std::shared_ptr<ConnectionInformation> makeConnectionInformation(const struct in_addr &ina, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    static std::shared_ptr<ConnectionInformation> makeConnectionInformation(const struct in6_addr &in6a, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    static std::shared_ptr<ConnectionInformation> makeConnectionInformation(const std::shared_ptr<ConnectionInformation> &other); // Copies the underlying ConnectionInformation object

    virtual ~ConnectionInformation() {};

    bool operator==(const ConnectionInformation &other) const;

    virtual std::size_t hash() const = 0;
    virtual operator std::string() const = 0;

protected:
    ConnectionInformation();
    ConnectionInformation(const ConnectionInformation &other);
    ConnectionInformation(ConnectionInformation &&other);

    ConnectionInformation &operator=(const ConnectionInformation &other);
    ConnectionInformation &operator=(ConnectionInformation &&other);
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(ConnectionInformation), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(ConnectionInformation) &connection_info, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(connection_info));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(ConnectionInformation)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(ConnectionInformation) &connection_info) const { return connection_info.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(ConnectionInformation) &connection_info);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_CONNECTION_INFORMATION_HH_ */
