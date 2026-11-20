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

#include <chrono>
#include <format>
#include <iostream>
#include <string>

#include <rtsdp/common.hh>
#include "Address.hh"

#include "Origin.hh"

LIBRTSDP_NAMESPACE_START

Origin::Origin()
    :m_username()
    ,m_sessionId(0)
    ,m_sessionVersion(0)
    ,m_originAddress()
{
}

Origin::Origin(const Address &origin_address, const std::string &user)
    :m_username(user)
    ,m_sessionId(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count())
    ,m_sessionVersion(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count())
    ,m_originAddress(origin_address)
{
}

Origin::Origin(const Origin &other)
    :m_username(other.m_username)
    ,m_sessionId(other.m_sessionId)
    ,m_sessionVersion(other.m_sessionVersion)
    ,m_originAddress(other.m_originAddress)
{
}

Origin::Origin(Origin &&other)
    :m_username(std::move(other.m_username))
    ,m_sessionId(other.m_sessionId)
    ,m_sessionVersion(other.m_sessionVersion)
    ,m_originAddress(std::move(other.m_originAddress))
{
    other.m_sessionId = 0;
    other.m_sessionVersion = 0;
}

Origin &Origin::operator=(const Origin &other)
{
    m_username = other.m_username;
    m_sessionId = other.m_sessionId;
    m_sessionVersion = other.m_sessionVersion;
    m_originAddress = other.m_originAddress;
    return *this;
}

Origin &Origin::operator=(Origin &&other)
{
    m_username = std::move(other.m_username);
    m_sessionId = other.m_sessionId;
    m_sessionVersion = other.m_sessionVersion;
    m_originAddress = std::move(other.m_originAddress);
    other.m_sessionId = 0;
    other.m_sessionVersion = 0;
    return *this;
}

bool Origin::operator==(const Origin &other) const
{
    return m_sessionId == other.m_sessionId && m_sessionVersion == other.m_sessionVersion && m_username == other.m_username &&
           m_originAddress == other.m_originAddress;
}

std::size_t Origin::hash() const
{
    std::hash<std::string> hs;
    std::hash<uint64_t> hui;
    std::hash<Address> ha;

    return hs(m_username) + hui(m_sessionId) * 2 + hui(m_sessionVersion) * 3 + ha(m_originAddress) * 5;
}

Origin::operator std::string() const
{
    if (!m_originAddress) return std::string();

    if (m_username.empty()) {
        return std::format("o=- {} {} {}\r\n", m_sessionId, m_sessionVersion, m_originAddress);
    }
    return std::format("o={} {} {} {}\r\n", m_username, m_sessionId, m_sessionVersion, m_originAddress);
}

bool Origin::isValid() const
{
    return !!m_originAddress;
}

void Origin::updateVersion()
{
    m_sessionVersion = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(Origin) &origin)
{
    os << static_cast<std::string>(origin);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
