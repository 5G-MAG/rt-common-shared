#ifndef _LIBSDP_ADDRESS_HH_
#define _LIBSDP_ADDRESS_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: Address class
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
#include <sys/socket.h>

#include <format>
#include <iostream>
#include <memory>
#include <string>

#include <rtsdp/common.hh>

LIBRTSDP_NAMESPACE_START

class Address {
public:
    Address();
    Address(const std::string &hostname, int family);
    Address(std::string &&hostname, int family);
    Address(const struct sockaddr &sock_addr, socklen_t sock_addr_len);
    Address(const struct sockaddr_in &sin_addr);
    Address(const struct sockaddr_in6 &sin6_addr);
    Address(const struct in_addr &sin_addr);
    Address(const struct in6_addr &sin6_addr);
    Address(const Address &other);
    Address(Address &&other);

    virtual ~Address() {};

    Address &operator=(const Address &other);
    Address &operator=(Address &&other);

    bool operator==(const Address &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const { return isValid(); };

    bool isValid() const;

    const std::string &hostname() const { return m_hostname; };
    Address &hostname(const std::string &host, int family);
    Address &hostname(std::string &&host, int family);

    socklen_t sockaddrlen() const { return m_sockAddrLen; };

    const struct sockaddr *sockaddr() const { return reinterpret_cast<const struct sockaddr*>(m_sockAddr.get()); };

private:
    void setSockAddrFromHostname(int family);

    std::unique_ptr<struct sockaddr_storage> m_sockAddr;
    socklen_t m_sockAddrLen;
    std::string m_hostname;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(Address), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(Address) &address, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(address));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(Address)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(Address) &address) const { return address.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(Address) &address);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_ADDRESS_HH_ */
