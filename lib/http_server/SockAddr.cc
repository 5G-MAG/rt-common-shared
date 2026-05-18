/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: SockAddr class
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

#include <inttypes.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <cstring>
#include <format>
#include <memory>
#include <stdexcept>

#include "common.hh"

#include "SockAddr.hh"

HTTPXPP_NAMESPACE_START

static bool compare_addr_and_port(const struct sockaddr_in &a, const struct sockaddr_in &b);
static bool compare_addr_and_port(const struct sockaddr_in6 &a, const struct sockaddr_in6 &b);

SockAddr::SockAddr()
    :m_sockaddr()
    ,m_addrlen(0)
{
}

SockAddr::SockAddr(int family, const std::string &hostname, in_port_t port)
    :m_sockaddr()
    ,m_addrlen(0)
{
    struct addrinfo *ai = nullptr;
    auto port_name = std::format("{}", port);
    getaddrinfo(hostname.c_str(), port_name.c_str(), nullptr, &ai);

    for (auto *it = ai; it; it = it->ai_next) {
        if (it->ai_family == AF_INET || it->ai_family == AF_INET6) {
            m_sockaddr.reset(new sockaddr_storage);
            m_addrlen = it->ai_addrlen;
            memcpy(m_sockaddr.get(), it->ai_addr, it->ai_addrlen);
            break;
        }
    } 

    if (ai) freeaddrinfo(ai);
}

SockAddr::SockAddr(int sock_fd)
    :m_sockaddr()
    ,m_addrlen(0)
{
    fromSockName(sock_fd);
}

SockAddr::SockAddr(const SockAddr &other)
    :m_sockaddr()
    ,m_addrlen(other.m_addrlen)
{
    if (other.m_sockaddr) {
        m_sockaddr.reset(new sockaddr_storage);
        memcpy(m_sockaddr.get(), other.m_sockaddr.get(), other.m_addrlen);
    }
}

SockAddr::SockAddr(const struct sockaddr &sa)
    :m_sockaddr()
    ,m_addrlen(0)
{
    if (sa.sa_family == AF_INET) {
        m_sockaddr.reset(new sockaddr_storage);
        m_addrlen = sizeof(sockaddr_in);
        memcpy(m_sockaddr.get(), &sa, sizeof(sockaddr_in));
    } else if (sa.sa_family == AF_INET6) {
        m_sockaddr.reset(new sockaddr_storage);
        m_addrlen = sizeof(sockaddr_in6);
        memcpy(m_sockaddr.get(), &sa, sizeof(sockaddr_in6));
    } else {
        throw std::out_of_range("Address type not handled");
    }
}

SockAddr::SockAddr(const struct sockaddr_in &sin)
    :m_sockaddr(new sockaddr_storage)
    ,m_addrlen(sizeof(sockaddr_in))
{
    memcpy(m_sockaddr.get(), &sin, sizeof(sockaddr_in));
}

SockAddr::SockAddr(const struct sockaddr_in6 &sin6)
    :m_sockaddr(new sockaddr_storage)
    ,m_addrlen(sizeof(sockaddr_in6))
{
    memcpy(m_sockaddr.get(), &sin6, sizeof(sockaddr_in6));
}

SockAddr::SockAddr(SockAddr &&other)
    :m_sockaddr(std::move(other.m_sockaddr))
    ,m_addrlen(other.m_addrlen)
{
    other.m_addrlen = 0;
}

SockAddr::SockAddr(struct sockaddr *sa, socklen_t sa_len)
    :m_sockaddr()
    ,m_addrlen(sa_len)
{
    if (sa_len == sizeof(sockaddr_storage)) {
        m_sockaddr.reset(reinterpret_cast<struct sockaddr_storage*>(sa));
    } else {
        m_sockaddr.reset(new sockaddr_storage);
        memcpy(m_sockaddr.get(), sa, m_addrlen);
        free(sa);
    }
}

SockAddr::SockAddr(struct sockaddr_in *sin)
    :m_sockaddr(new sockaddr_storage)
    ,m_addrlen(sizeof(*sin))
{
    memcpy(m_sockaddr.get(), sin, m_addrlen);
    free(sin);
}

SockAddr::SockAddr(struct sockaddr_in6 *sin6)
    :m_sockaddr(new sockaddr_storage)
    ,m_addrlen(sizeof(*sin6))
{
    memcpy(m_sockaddr.get(), sin6, m_addrlen);
    free(sin6);
}

SockAddr &SockAddr::operator=(const SockAddr &other)
{
    m_addrlen = other.m_addrlen;
    if (!other.m_sockaddr) {
        m_sockaddr.reset();
    } else {
        m_sockaddr.reset(new sockaddr_storage);
        memcpy(m_sockaddr.get(), other.m_sockaddr.get(), m_addrlen);
    }
    return *this;
}

SockAddr &SockAddr::operator=(SockAddr &&other)
{
    m_sockaddr = std::move(other.m_sockaddr);
    m_addrlen = other.m_addrlen;
    other.m_addrlen = 0;
    return *this;
}

SockAddr &SockAddr::operator=(const struct sockaddr &sa)
{
    if (sa.sa_family == AF_INET || sa.sa_family == AF_INET6) {
        m_sockaddr.reset(new sockaddr_storage);
        m_addrlen = (sa.sa_family == AF_INET)?sizeof(sockaddr_in):sizeof(sockaddr_in6);
        memcpy(m_sockaddr.get(), &sa, m_addrlen);
    } else {
        throw std::out_of_range("Address type not handled");
    }
    return *this;
}

SockAddr &SockAddr::operator=(const struct sockaddr_in &sin)
{
    m_sockaddr.reset(new sockaddr_storage);
    m_addrlen = sizeof(sockaddr_in);
    memcpy(m_sockaddr.get(), &sin, sizeof(sockaddr_in));
    return *this;
}

SockAddr &SockAddr::operator=(const struct sockaddr_in6 &sin6)
{
    m_sockaddr.reset(new sockaddr_storage);
    m_addrlen = sizeof(sockaddr_in6);
    memcpy(m_sockaddr.get(), &sin6, sizeof(sockaddr_in6));
    return *this;
}

bool SockAddr::operator==(const SockAddr &other) const
{
    if (m_addrlen != other.m_addrlen) return false;
    if (m_sockaddr == other.m_sockaddr) return true;
    if (!m_sockaddr || !other.m_sockaddr) return false;
    if (m_sockaddr->ss_family != other.m_sockaddr->ss_family) return false;
    if (m_sockaddr->ss_family == AF_INET) {
        return compare_addr_and_port(*std::reinterpret_pointer_cast<const struct sockaddr_in>(m_sockaddr),
                                     *std::reinterpret_pointer_cast<const struct sockaddr_in>(other.m_sockaddr));
    } else if (m_sockaddr->ss_family == AF_INET6) {
        return compare_addr_and_port(*std::reinterpret_pointer_cast<const struct sockaddr_in6>(m_sockaddr),
                                     *std::reinterpret_pointer_cast<const struct sockaddr_in6>(other.m_sockaddr));
    }
    return (memcmp(m_sockaddr.get(), other.m_sockaddr.get(), m_addrlen) == 0);
}

bool SockAddr::operator==(const struct sockaddr &other) const
{
    if (!m_sockaddr) return false;
    if (m_sockaddr->ss_family != other.sa_family) return false;
    if (m_sockaddr->ss_family == AF_INET) {
        return compare_addr_and_port(*std::reinterpret_pointer_cast<const struct sockaddr_in>(m_sockaddr),
                                     reinterpret_cast<const struct sockaddr_in&>(other));
    } else if (m_sockaddr->ss_family == AF_INET6) {
        return compare_addr_and_port(*std::reinterpret_pointer_cast<const struct sockaddr_in6>(m_sockaddr),
                                     reinterpret_cast<const struct sockaddr_in6&>(other));
    }
    return memcmp(m_sockaddr.get(), &other, m_addrlen) == 0;
}

bool SockAddr::operator==(const struct sockaddr_in &other) const
{
    if (!m_sockaddr) return false;
    if (m_sockaddr->ss_family != AF_INET) return false;
    return compare_addr_and_port(*std::reinterpret_pointer_cast<const struct sockaddr_in>(m_sockaddr), other);
}

bool SockAddr::operator==(const struct sockaddr_in6 &other) const
{
    if (!m_sockaddr) return false;
    if (m_sockaddr->ss_family != AF_INET6) return false;
    return compare_addr_and_port(*std::reinterpret_pointer_cast<const struct sockaddr_in6>(m_sockaddr), other);
}

SockAddr::operator const struct sockaddr*() const
{
    return reinterpret_cast<const struct sockaddr*>(m_sockaddr.get());
}

SockAddr::operator const struct sockaddr_in*() const
{
    if (!m_sockaddr || m_sockaddr->ss_family != AF_INET) {
        throw std::out_of_range("Cast to sockaddr_in failed, wrong address family");
    }
    return reinterpret_cast<const struct sockaddr_in*>(m_sockaddr.get());
}

SockAddr::operator const struct sockaddr_in6*() const
{
    if (!m_sockaddr || m_sockaddr->ss_family != AF_INET6) {
        throw std::out_of_range("Cast to sockaddr_in6 failed, wrong address family");
    }
    return reinterpret_cast<const struct sockaddr_in6*>(m_sockaddr.get());
}

uint16_t SockAddr::port() const
{
    if (!m_sockaddr) {
        throw std::out_of_range("Unable to get port number from empty address");
    } else if (m_sockaddr->ss_family == AF_INET) {
        return std::reinterpret_pointer_cast<sockaddr_in>(m_sockaddr)->sin_port;
    } else if (m_sockaddr->ss_family == AF_INET6) {
        return std::reinterpret_pointer_cast<sockaddr_in6>(m_sockaddr)->sin6_port;
    }
    throw std::out_of_range("Unable to get port number from unhandled address type");
}

SockAddr &SockAddr::port(uint16_t num)
{
    if (!m_sockaddr) {
        throw std::out_of_range("Unable to set port number on an empty address");
    } else if (m_sockaddr->ss_family == AF_INET) {
        std::reinterpret_pointer_cast<sockaddr_in>(m_sockaddr)->sin_port = num;
    } else if (m_sockaddr->ss_family == AF_INET6) {
        std::reinterpret_pointer_cast<sockaddr_in6>(m_sockaddr)->sin6_port = num;
    } else {
        throw std::out_of_range("Unable to set port number on an unhandled address type");
    }
    return *this;
}

const struct in_addr &SockAddr::ipv4Address() const
{
    if (m_sockaddr && m_sockaddr->ss_family == AF_INET) {
        return std::reinterpret_pointer_cast<sockaddr_in>(m_sockaddr)->sin_addr;
    }
    throw std::out_of_range("Address is not IPv4");
}

const struct in6_addr &SockAddr::ipv6Address() const
{
    if (m_sockaddr && m_sockaddr->ss_family == AF_INET6) {
        return std::reinterpret_pointer_cast<sockaddr_in6>(m_sockaddr)->sin6_addr;
    }
    throw std::out_of_range("Address is not IPv6");
}

SockAddr &SockAddr::fromSockName(int fd)
{
    if (fd >= 0) {
        reserve();
        ::getsockname(fd, reinterpret_cast<struct sockaddr*>(m_sockaddr.get()), &m_addrlen);
    }
    return *this;
}

SockAddr::operator std::string() const
{
    char ipaddr[INET6_ADDRSTRLEN];
    in_port_t port;
    if (family() == AF_INET) {
        auto *ipv4 = reinterpret_cast<const struct sockaddr_in*>(m_sockaddr.get());
        inet_ntop(AF_INET, &ipv4->sin_addr, ipaddr, sizeof(ipaddr));
        port = ntohs(ipv4->sin_port);
        return std::format("{}:{}", ipaddr, port);
    } else if (family() == AF_INET6) {
        auto *ipv6 = reinterpret_cast<const struct sockaddr_in6*>(m_sockaddr.get());
        inet_ntop(AF_INET6, &ipv6->sin6_addr, ipaddr, sizeof(ipaddr));
        port = ntohs(ipv6->sin6_port);
        return std::format("[{}]:{}", ipaddr, port);
    }
    throw std::out_of_range("SockAddr can only convert IPv4 and IPv6 addresses");
}

std::size_t SockAddr::hash() const
{
    const char *ptr = reinterpret_cast<const char*>(m_sockaddr.get());
    return std::hash<std::string_view>{}(std::string_view(ptr, ptr+m_addrlen));
}

SockAddr SockAddr::applyNetmask(const SockAddr &netmask) const
{
    if (netmask.family() != family()) {
        throw std::out_of_range("SockAddr::applyNetmask failed: Netmask address type is not the same address family");
    }
    SockAddr result(*this);
    if (family() == AF_INET) {
        auto *new_ipv4 = reinterpret_cast<struct sockaddr_in*>(result.m_sockaddr.get());
        auto *mask_ipv4 = reinterpret_cast<const struct sockaddr_in*>(netmask.m_sockaddr.get());
        new_ipv4->sin_addr.s_addr &= mask_ipv4->sin_addr.s_addr;
    } else if (family() == AF_INET6) {
        auto *new_ipv6 = reinterpret_cast<struct sockaddr_in6*>(result.m_sockaddr.get());
        auto *mask_ipv6 = reinterpret_cast<const struct sockaddr_in6*>(netmask.m_sockaddr.get());
        new_ipv6->sin6_addr.s6_addr32[0] &= mask_ipv6->sin6_addr.s6_addr32[0];
        new_ipv6->sin6_addr.s6_addr32[1] &= mask_ipv6->sin6_addr.s6_addr32[1];
        new_ipv6->sin6_addr.s6_addr32[2] &= mask_ipv6->sin6_addr.s6_addr32[2];
        new_ipv6->sin6_addr.s6_addr32[3] &= mask_ipv6->sin6_addr.s6_addr32[3];
    } else {
        throw std::out_of_range("SockAddr::applyNetmask failed: Address type not handled");
    }
    return result;
}

static bool compare_addr_and_port(const struct sockaddr_in &a, const struct sockaddr_in &b)
{
    if (a.sin_family != AF_INET || b.sin_family != AF_INET) return false;
    if (a.sin_addr.s_addr != b.sin_addr.s_addr) return false;
    if (a.sin_port == 0 || b.sin_port == 0) return true;
    return a.sin_port == b.sin_port;
}

static bool compare_addr_and_port(const struct sockaddr_in6 &a, const struct sockaddr_in6 &b)
{
    if (a.sin6_family != AF_INET6 || b.sin6_family != AF_INET6) return false;
    if (memcmp(&a.sin6_addr, &b.sin6_addr, sizeof(a.sin6_addr)) != 0) return false;
    if (a.sin6_port == 0 || b.sin6_port == 0) return true;
    return a.sin6_port == b.sin6_port;
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
