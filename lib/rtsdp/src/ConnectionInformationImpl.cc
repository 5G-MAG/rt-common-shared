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
#include "Address.hh"
#include <rtsdp/ConnectionInformation.hh>
#include "ConnectionInfo.hh"
#include <rtsdp/SDP.hh>

#include "ConnectionInformationImpl.hh"

LIBRTSDP_NAMESPACE_START

ConnectionInformationImpl::ConnectionInformationImpl()
    :ConnectionInformation()
    ,m_connectionInfo()
{
}

ConnectionInformationImpl::ConnectionInformationImpl(const std::string &hostname, int family, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
    :ConnectionInformation()
    ,m_connectionInfo(new ConnectionInfo(Address(hostname, family), ttl, consecutive_addresses))
{
}

ConnectionInformationImpl::ConnectionInformationImpl(const struct sockaddr_in &sin, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
    :ConnectionInformation()
    ,m_connectionInfo(new ConnectionInfo(Address(sin), ttl, consecutive_addresses))
{
}

ConnectionInformationImpl::ConnectionInformationImpl(const struct sockaddr_in6 &sin6, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
    :ConnectionInformation()
    ,m_connectionInfo(new ConnectionInfo(Address(sin6), ttl, consecutive_addresses))
{
}

ConnectionInformationImpl::ConnectionInformationImpl(const struct in_addr &ina, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
    :ConnectionInformation()
    ,m_connectionInfo(new ConnectionInfo(Address(ina), ttl, consecutive_addresses))
{
}

ConnectionInformationImpl::ConnectionInformationImpl(const struct in6_addr &in6a, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
    :ConnectionInformation()
    ,m_connectionInfo(new ConnectionInfo(Address(in6a), ttl, consecutive_addresses))
{
}

ConnectionInformationImpl::ConnectionInformationImpl(const ConnectionInformationImpl &other)
    :ConnectionInformation()
    ,m_connectionInfo(other.m_connectionInfo)
{
}

ConnectionInformationImpl::ConnectionInformationImpl(ConnectionInformationImpl &&other)
    :ConnectionInformation()
    ,m_connectionInfo(std::move(other.m_connectionInfo))
{
}

ConnectionInformationImpl &ConnectionInformationImpl::operator=(const ConnectionInformationImpl &other)
{
    m_connectionInfo = other.m_connectionInfo;
    return *this;
}

ConnectionInformationImpl &ConnectionInformationImpl::operator=(ConnectionInformationImpl &&other)
{
    m_connectionInfo = std::move(other.m_connectionInfo);
    return *this;
}

bool ConnectionInformationImpl::operator==(const ConnectionInformationImpl &other) const
{
    return m_connectionInfo == other.m_connectionInfo;
}

std::size_t ConnectionInformationImpl::hash() const
{
    return m_connectionInfo->hash();
}

ConnectionInformationImpl::operator std::string() const
{
    return static_cast<std::string>(*m_connectionInfo);
}

bool ConnectionInformationImpl::isValid() const
{
    return !m_connectionInfo || m_connectionInfo->isValid();
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(ConnectionInformationImpl) &connection_info)
{
    os << static_cast<std::string>(connection_info);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
