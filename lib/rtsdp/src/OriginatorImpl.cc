/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: OriginatorImpl class
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
#include "Origin.hh"
#include <rtsdp/Originator.hh>
#include <rtsdp/SDP.hh>
#include "SDPImpl.hh"

#include "OriginatorImpl.hh"

LIBRTSDP_NAMESPACE_START

OriginatorImpl::OriginatorImpl()
    :Originator()
    ,m_parent()
    ,m_origin()
{
}

OriginatorImpl::OriginatorImpl(const std::string &hostname, int family, const std::string &user)
    :Originator()
    ,m_parent()
    ,m_origin(new Origin (Address(hostname, family), user))
{
}

OriginatorImpl::OriginatorImpl(const struct sockaddr_in &sin, const std::string &user)
    :Originator()
    ,m_parent()
    ,m_origin(new Origin (Address(sin), user))
{
}

OriginatorImpl::OriginatorImpl(const struct sockaddr_in6 &sin6, const std::string &user)
    :Originator()
    ,m_parent()
    ,m_origin(new Origin (Address(sin6), user))
{
}

OriginatorImpl::OriginatorImpl(const struct in_addr &ina, const std::string &user)
    :Originator()
    ,m_parent()
    ,m_origin(new Origin (Address(ina), user))
{
}

OriginatorImpl::OriginatorImpl(const struct in6_addr &in6a, const std::string &user)
    :Originator()
    ,m_parent()
    ,m_origin(new Origin (Address(in6a), user))
{
}

OriginatorImpl::OriginatorImpl(const OriginatorImpl &other)
    :Originator()
    ,m_parent(other.m_parent)
    ,m_origin(other.m_origin)
{
}

OriginatorImpl::OriginatorImpl(OriginatorImpl &&other)
    :Originator()
    ,m_parent(std::move(other.m_parent))
    ,m_origin(std::move(other.m_origin))
{
}

OriginatorImpl::OriginatorImpl(const std::shared_ptr<Origin> &origin)
    :Originator()
    ,m_parent()
    ,m_origin(origin)
{
}

OriginatorImpl &OriginatorImpl::operator=(const OriginatorImpl &other)
{
    m_parent = other.m_parent;
    m_origin = other.m_origin;
    return *this;
}

OriginatorImpl &OriginatorImpl::operator=(OriginatorImpl &&other)
{
    m_parent = std::move(other.m_parent);
    m_origin = std::move(other.m_origin);
    return *this;
}

bool OriginatorImpl::operator==(const OriginatorImpl &other) const
{
    return m_origin == other.m_origin || *m_origin == *other.m_origin;
}

std::size_t OriginatorImpl::hash() const
{
    return m_origin->hash();
}

OriginatorImpl::operator std::string() const
{
    return static_cast<std::string>(*m_origin);
}

bool OriginatorImpl::isValid() const
{
    return m_origin && m_origin->isValid();
}

const std::string &OriginatorImpl::hostname() const
{
    static const std::string empty_str;
    return m_origin?m_origin->originAddress().hostname():empty_str;
}

Originator &OriginatorImpl::hostname(const std::string &host, int family)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->originAddress(Address(host, family));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

Originator &OriginatorImpl::hostname(std::string &&host, int family)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->originAddress(Address(std::move(host), family));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

std::pair<const struct sockaddr&, socklen_t> OriginatorImpl::address() const
{
    static const struct sockaddr empty_sa{ .sa_family = AF_UNSPEC };
    if (!m_origin) return std::pair<const struct sockaddr&, socklen_t>(empty_sa, sizeof(empty_sa));
    auto &addr = m_origin->originAddress();
    return std::pair<const struct sockaddr&, socklen_t>(*addr.sockaddr(), addr.sockaddrlen());
}

Originator &OriginatorImpl::address(const struct sockaddr &sa, socklen_t sa_len)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->originAddress(Address(sa, sa_len));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

Originator &OriginatorImpl::address(const struct sockaddr_in &sin)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->originAddress(Address(sin));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

Originator &OriginatorImpl::address(const struct sockaddr_in6 &sin6)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->originAddress(Address(sin6));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

Originator &OriginatorImpl::address(const struct in_addr &ina)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->originAddress(Address(ina));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

Originator &OriginatorImpl::address(const struct in6_addr &in6a)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->originAddress(Address(in6a));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

const std::string &OriginatorImpl::username() const
{
    static const std::string empty_str;
    if (!m_origin) return empty_str;
    return m_origin->username();
}

Originator &OriginatorImpl::username(const std::string &user_name)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->username(user_name);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

Originator &OriginatorImpl::username(std::string &&user_name)
{
    if (!m_origin) m_origin.reset(new Origin);
    m_origin->username(std::move(user_name));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(OriginatorImpl) &origin)
{
    os << static_cast<std::string>(origin);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
