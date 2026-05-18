#ifndef _LIBRTSDP_PUB_CONNECTION_INFORMATION_IMPL_HH_
#define _LIBRTSDP_PUB_CONNECTION_INFORMATION_IMPL_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: ConnectionInformationImpl class
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

#include <rtsdp/common.hh>
#include <rtsdp/ConnectionInformation.hh>
#include "ConnectionInfo.hh"

LIBRTSDP_NAMESPACE_START

class SDP;

class ConnectionInformationImpl : public ConnectionInformation {
public:
    ConnectionInformationImpl();
    ConnectionInformationImpl(const std::string &hostname, int family = AF_UNSPEC, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    ConnectionInformationImpl(const struct sockaddr_in &sin, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    ConnectionInformationImpl(const struct sockaddr_in6 &sin6, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    ConnectionInformationImpl(const struct in_addr &ina, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    ConnectionInformationImpl(const struct in6_addr &in6a, const std::optional<uint8_t> &ttl = std::nullopt, size_t consecutive_addresses = 1);
    ConnectionInformationImpl(const ConnectionInformationImpl &other);
    ConnectionInformationImpl(ConnectionInformationImpl &&other);

    ConnectionInformationImpl(const std::shared_ptr<ConnectionInfo> &conn_info) :ConnectionInformation(), m_parent(), m_connectionInfo(conn_info) {};
    ConnectionInformationImpl(std::shared_ptr<ConnectionInfo> &&conn_info) :ConnectionInformation(), m_parent(), m_connectionInfo(std::move(conn_info)) {};

    virtual ~ConnectionInformationImpl() {};

    ConnectionInformationImpl &operator=(const ConnectionInformationImpl &other);
    ConnectionInformationImpl &operator=(ConnectionInformationImpl &&other);

    bool operator==(const ConnectionInformationImpl &other) const;

    void setParent(const std::weak_ptr<SDP> &parent) { m_parent = parent; };
    const std::shared_ptr<ConnectionInfo> &connectionInfo() const { return m_connectionInfo; };

    virtual std::size_t hash() const;
    virtual operator std::string() const;
    virtual bool isValid() const;

private:
    std::weak_ptr<SDP> m_parent;
    std::shared_ptr<ConnectionInfo> m_connectionInfo;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(ConnectionInformationImpl), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(ConnectionInformationImpl) &connection_info, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(connection_info));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(ConnectionInformationImpl)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(ConnectionInformationImpl) &connection_info) const { return connection_info.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(ConnectionInformationImpl) &connection_info);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_CONNECTION_INFORMATION_IMPL_HH_ */
