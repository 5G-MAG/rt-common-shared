/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: BandwidthInfo class
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

#include <format>
#include <iostream>
#include <list>
#include <string>

#include <rtsdp/common.hh>

#include "BandwidthInfo.hh"

LIBRTSDP_NAMESPACE_START

BandwidthInfo::BandwidthInfo()
    :m_type()
    ,m_value(0)
{
}

BandwidthInfo::BandwidthInfo(const std::string &bandwidth_type, BandwidthInfo::value_type value)
    :m_type(bandwidth_type)
    ,m_value(value)
{
}

BandwidthInfo::BandwidthInfo(const BandwidthInfo &other)
    :m_type(other.m_type)
    ,m_value(other.m_value)
{
}

BandwidthInfo::BandwidthInfo(BandwidthInfo &&other)
    :m_type(std::move(other.m_type))
    ,m_value(other.m_value)
{
}

BandwidthInfo &BandwidthInfo::operator=(const BandwidthInfo &other)
{
    m_type = other.m_type;
    m_value = other.m_value;
    return *this;
}

BandwidthInfo &BandwidthInfo::operator=(BandwidthInfo &&other)
{
    m_type = std::move(other.m_type);
    m_value = other.m_value;
    return *this;
}

bool BandwidthInfo::operator==(const BandwidthInfo &other) const
{
    return m_value == other.m_value && m_type == other.m_type;
}

std::size_t BandwidthInfo::hash() const
{
    std::hash<std::string> hs;
    std::hash<value_type> hui;

    return hs(m_type) + (hui(m_value) << 1);
}

BandwidthInfo::operator std::string() const
{
    if (m_type.empty() && m_value == 0) return std::string();
    if (m_type.empty()) return std::format("b={}\r\n", m_value);
    return std::format("b={}:{}\r\n", m_type, m_value);
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(BandwidthInfo) &bandwidth_info)
{
    os << static_cast<std::string>(bandwidth_info);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
