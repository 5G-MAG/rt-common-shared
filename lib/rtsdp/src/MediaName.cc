/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: MediaName class
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

#include "MediaName.hh"

LIBRTSDP_NAMESPACE_START

MediaName::MediaName()
    :m_mediaType()
    ,m_port(0)
    ,m_numberOfPorts(0)
    ,m_proto()
    ,m_fmt()
{
}

MediaName::MediaName(const std::string &media_type, uint16_t port, const std::string &proto, const std::string &fmt, uint16_t number_of_ports)
    :m_mediaType(media_type)
    ,m_port(port)
    ,m_numberOfPorts(number_of_ports)
    ,m_proto(proto)
    ,m_fmt(fmt)
{
}

MediaName::MediaName(const MediaName &other)
    :m_mediaType(other.m_mediaType)
    ,m_port(other.m_port)
    ,m_numberOfPorts(other.m_numberOfPorts)
    ,m_proto(other.m_proto)
    ,m_fmt(other.m_fmt)
{
}

MediaName::MediaName(MediaName &&other)
    :m_mediaType(std::move(other.m_mediaType))
    ,m_port(other.m_port)
    ,m_numberOfPorts(other.m_numberOfPorts)
    ,m_proto(std::move(other.m_proto))
    ,m_fmt(std::move(other.m_fmt))
{
    other.m_port = 0;
    other.m_numberOfPorts = 1;
}

MediaName &MediaName::operator=(const MediaName &other)
{
    m_mediaType = other.m_mediaType;
    m_port = other.m_port;
    m_numberOfPorts = other.m_numberOfPorts;
    m_proto = other.m_proto;
    m_fmt = other.m_fmt;
    return *this;
}

MediaName &MediaName::operator=(MediaName &&other)
{
    m_mediaType = std::move(other.m_mediaType);
    m_port = other.m_port;
    m_numberOfPorts = other.m_numberOfPorts;
    m_proto = std::move(other.m_proto);
    m_fmt = std::move(other.m_fmt);
    other.m_port = 0;
    other.m_numberOfPorts = 1;
    return *this;
}

bool MediaName::operator==(const MediaName &other) const
{
    return m_port == other.m_port && m_numberOfPorts == other.m_numberOfPorts && m_mediaType == other.m_mediaType &&
            m_proto == other.m_proto && m_fmt == other.m_fmt;
}

std::size_t MediaName::hash() const
{
    std::hash<std::string> hs;
    std::hash<uint16_t> hui;

    return hs(m_mediaType) + hui(m_port) * 2 + hui(m_numberOfPorts) * 3 + hs(m_proto) * 5 + hs(m_fmt) * 7;
}

MediaName::operator std::string() const
{
    if (m_mediaType.empty()) return std::string();

    std::string result = std::format("m={} {}", m_mediaType, m_port);
    if (m_numberOfPorts > 1) {
        result += std::format("/{}", m_numberOfPorts);
    }
    result += std::format(" {} {}\r\n", m_proto, m_fmt);

    return result;
}

MediaName::operator bool() const
{
    return !m_mediaType.empty();
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(MediaName) &media_name)
{
    os << static_cast<std::string>(media_name);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
