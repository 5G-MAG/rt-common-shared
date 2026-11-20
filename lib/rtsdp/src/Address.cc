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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <cstring>
#include <format>
#include <iostream>
#include <memory>
#include <string>

#include <rtsdp/common.hh>

#include "Address.hh"

LIBRTSDP_NAMESPACE_START

Address::Address()
    :m_sockAddr()
    ,m_sockAddrLen(0)
    ,m_hostname()
{
}

Address::Address(const std::string &hostname, int family)
    :m_sockAddr()
    ,m_sockAddrLen(0)
    ,m_hostname(hostname)
{
    setSockAddrFromHostname(family);
}

Address::Address(std::string &&hostname, int family)
    :m_sockAddr()
    ,m_sockAddrLen(0)
    ,m_hostname(std::move(hostname))
{
    setSockAddrFromHostname(family);
}

Address::Address(const struct sockaddr &sock_addr, socklen_t sock_addr_len)
    :m_sockAddr(new struct sockaddr_storage)
    ,m_sockAddrLen(sock_addr_len)
    ,m_hostname()
{
    memcpy(m_sockAddr.get(), &sock_addr, sock_addr_len);
}

Address::Address(const struct sockaddr_in &sin_addr)
    :m_sockAddr(new struct sockaddr_storage)
    ,m_sockAddrLen(sizeof(struct sockaddr_in))
    ,m_hostname()
{
    memcpy(m_sockAddr.get(), &sin_addr, m_sockAddrLen);
}

Address::Address(const struct sockaddr_in6 &sin6_addr)
    :m_sockAddr(new struct sockaddr_storage)
    ,m_sockAddrLen(sizeof(struct sockaddr_in6))
    ,m_hostname()
{
    memcpy(m_sockAddr.get(), &sin6_addr, m_sockAddrLen);
}

Address::Address(const struct in_addr &sin_addr)
    :m_sockAddr(new struct sockaddr_storage)
    ,m_sockAddrLen(sizeof(struct sockaddr_in))
    ,m_hostname()
{
    m_sockAddr->ss_family = AF_INET;
    auto *sin = reinterpret_cast<struct sockaddr_in*>(m_sockAddr.get());
    sin->sin_addr = sin_addr;
}

Address::Address(const struct in6_addr &sin6_addr)
    :m_sockAddr(new struct sockaddr_storage)
    ,m_sockAddrLen(sizeof(struct sockaddr_in6))
    ,m_hostname()
{
    m_sockAddr->ss_family = AF_INET6;
    auto *sin6 = reinterpret_cast<struct sockaddr_in6*>(m_sockAddr.get());
    sin6->sin6_addr = sin6_addr;
}

Address::Address(const Address &other)
    :m_sockAddr()
    ,m_sockAddrLen(other.m_sockAddrLen)
    ,m_hostname(other.m_hostname)
{
    if (other.m_sockAddr) {
        m_sockAddr.reset(new struct sockaddr_storage);
        memcpy(m_sockAddr.get(), other.m_sockAddr.get(), m_sockAddrLen);
    }
}

Address::Address(Address &&other)
    :m_sockAddr(std::move(other.m_sockAddr))
    ,m_sockAddrLen(other.m_sockAddrLen)
    ,m_hostname(std::move(other.m_hostname))
{
    other.m_sockAddrLen = 0;
}

Address &Address::operator=(const Address &other)
{
    m_sockAddrLen = other.m_sockAddrLen;
    m_hostname = other.m_hostname;
    if (other.m_sockAddr) {
        if (!m_sockAddr) m_sockAddr.reset(new struct sockaddr_storage);
        memcpy(m_sockAddr.get(), other.m_sockAddr.get(), m_sockAddrLen);
    } else {
        m_sockAddr.reset();
    }
    return *this;
}

Address &Address::operator=(Address &&other)
{
    m_sockAddr = std::move(other.m_sockAddr);
    m_sockAddrLen = other.m_sockAddrLen;
    other.m_sockAddrLen = 0;
    m_hostname = std::move(other.m_hostname);
    return *this;
}

bool Address::operator==(const Address &other) const
{
    if (m_sockAddrLen != other.m_sockAddrLen) return false;
    if (m_hostname != other.m_hostname) return false;
    if (!m_sockAddr != !other.m_sockAddr) return false;
    if (m_sockAddr) return memcmp(m_sockAddr.get(), other.m_sockAddr.get(), m_sockAddrLen) == 0;
    return true;
}

std::size_t Address::hash() const
{
    std::hash<std::string> hs;
    std::hash<socklen_t> hui;

    auto result = hs(m_hostname) + hui(m_sockAddrLen) * 2;

    if (m_sockAddr) {
        std::hash<std::string_view> hsv;
        std::string_view sa_strv(reinterpret_cast<char*>(m_sockAddr.get()), m_sockAddrLen);
        result += hsv(sa_strv) * 3;
    }

    return result;
}

Address::operator std::string() const
{
    if (!m_sockAddr) return std::string();
    switch (m_sockAddr->ss_family) {
    case AF_INET:
        if (m_hostname.empty()) {
            char addr[INET_ADDRSTRLEN];
            auto *sin = reinterpret_cast<const struct sockaddr_in*>(m_sockAddr.get());
            inet_ntop(AF_INET, &sin->sin_addr, addr, sizeof(addr));
            return std::format("IN IP4 {}", addr);
        }
        return std::format("IN IP4 {}", m_hostname);
    case AF_INET6:
        if (m_hostname.empty()) {
            char addr[INET6_ADDRSTRLEN];
            auto *sin6 = reinterpret_cast<const struct sockaddr_in6*>(m_sockAddr.get());
            inet_ntop(AF_INET6, &sin6->sin6_addr, addr, sizeof(addr));
            return std::format("IN IP6 {}", addr);
        }
        return std::format("IN IP6 {}", m_hostname);
    default:
        throw std::out_of_range("Unable to format address that is not IPv4 or IPv6");
    }
}

bool Address::isValid() const
{
    return !!m_sockAddr && (m_sockAddr->ss_family == AF_INET || m_sockAddr->ss_family == AF_INET6);
}

Address &Address::hostname(const std::string &host, int family)
{
    m_hostname = host;
    setSockAddrFromHostname(family);
    return *this;
}

Address &Address::hostname(std::string &&host, int family)
{
    m_hostname = std::move(host);
    setSockAddrFromHostname(family);
    return *this;
}

// private:

void Address::setSockAddrFromHostname(int family)
{
    struct addrinfo *ai = nullptr;
    getaddrinfo(m_hostname.c_str(), "0", nullptr, &ai);
    for (auto *it = ai; it; it = it->ai_next) {
        if ((family == AF_UNSPEC && (it->ai_family == AF_INET || it->ai_family == AF_INET6)) || it->ai_family == family) {
            m_sockAddr.reset(new struct sockaddr_storage);
            memcpy(m_sockAddr.get(), it->ai_addr, it->ai_addrlen);
            m_sockAddrLen = it->ai_addrlen;
            break;
        }
    }

    if (ai) freeaddrinfo(ai);
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(Address) &address)
{
    os << static_cast<std::string>(address);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
