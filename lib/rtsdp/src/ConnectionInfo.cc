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

#include "ConnectionInfo.hh"

LIBRTSDP_NAMESPACE_START

ConnectionInfo::ConnectionInfo()
    :m_address()
    ,m_ttl()
    ,m_consecutiveAddresses(1)
{
}

ConnectionInfo::ConnectionInfo(const Address &address, const std::optional<uint8_t> &ttl, unsigned int consecutive_addresses)
    :m_address(address)
    ,m_ttl(ttl)
    ,m_consecutiveAddresses(consecutive_addresses)
{
}

ConnectionInfo::ConnectionInfo(const ConnectionInfo &other)
    :m_address(other.m_address)
    ,m_ttl(other.m_ttl)
    ,m_consecutiveAddresses(other.m_consecutiveAddresses)
{
}

ConnectionInfo::ConnectionInfo(ConnectionInfo &&other)
    :m_address(std::move(other.m_address))
    ,m_ttl(std::move(other.m_ttl))
    ,m_consecutiveAddresses(other.m_consecutiveAddresses)
{
}

ConnectionInfo &ConnectionInfo::operator=(const ConnectionInfo &other)
{
    m_address = other.m_address;
    m_ttl = other.m_ttl;
    m_consecutiveAddresses = other.m_consecutiveAddresses;
    return *this;
}

ConnectionInfo &ConnectionInfo::operator=(ConnectionInfo &&other)
{
    m_address = std::move(other.m_address);
    m_ttl = std::move(other.m_ttl);
    m_consecutiveAddresses = other.m_consecutiveAddresses;
    return *this;
}

bool ConnectionInfo::operator==(const ConnectionInfo &other) const
{
    return m_consecutiveAddresses == other.m_consecutiveAddresses && m_ttl == other.m_ttl && m_address == other.m_address;
}

std::size_t ConnectionInfo::hash() const
{
    std::hash<Address> ha;
    std::hash<std::optional<uint8_t> > houi;
    std::hash<unsigned int> hui;

    return ha(m_address) + (houi(m_ttl) * 2) + (hui(m_consecutiveAddresses) * 3);
}

ConnectionInfo::operator std::string() const
{
    std::string result;
    if (m_address) {
        result = std::format("c={}", m_address);
        if (m_ttl) {
            result += std::format("/{}", m_ttl.value());
        }
        if (m_consecutiveAddresses > 1) {
            result += std::format("/{}", m_consecutiveAddresses);
        }
        result += "\r\n";
    }
    return result;
}

bool ConnectionInfo::isValid() const
{
    return !!m_address;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(ConnectionInfo) &connection_info)
{
    os << static_cast<std::string>(connection_info);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
