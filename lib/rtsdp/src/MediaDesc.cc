/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: MediaDesc class
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
#include <rtsdp/Attribute.hh>
#include "BandwidthInfo.hh"
#include "ConnectionInfo.hh"
#include "MediaName.hh"

#include "MediaDesc.hh"

namespace std {
template<class T>
struct hash<std::list<T>> {
    std::size_t operator()(const std::list<T> &t_list) const {
        std::hash<T> ht;
        std::hash<size_t> hs;
        std::size_t result = 0x1a2b3c4d5e6f7980;
        size_t idx = 0;
        for (auto it = t_list.cbegin(); it != t_list.cend(); ++it) {
            result += hs(idx++) ^ ht(*it);
        }
        return result;
    };
};
}

LIBRTSDP_NAMESPACE_START

MediaDesc::MediaDesc()
    :m_mediaName()
    ,m_mediaTitle()
    ,m_connectionInfos()
    ,m_bandwidthInfos()
    ,m_encryptionKey()
    ,m_mediaAttributes()
{
}

MediaDesc::MediaDesc(const MediaName &media_name, const std::optional<std::string> &media_title,
                                   const std::optional<ConnectionInfo> &connection_info,
                                   const std::optional<BandwidthInfo> &bandwidth_info,
                                   const std::optional<std::string> &encryption_key,
                                   const std::optional<Attribute> &attribute)
    :m_mediaName(media_name)
    ,m_mediaTitle()
    ,m_connectionInfos()
    ,m_bandwidthInfos()
    ,m_encryptionKey()
    ,m_mediaAttributes()
{
    if (media_title) {
        m_mediaTitle = media_title.value();
    }
    if (connection_info) {
        m_connectionInfos.emplace_back(new ConnectionInfo(connection_info.value()));
    }
    if (bandwidth_info) {
        m_bandwidthInfos.emplace_back(bandwidth_info.value());
    }
    if (encryption_key) {
        m_encryptionKey = encryption_key.value();
    }
    if (attribute) {
        m_mediaAttributes.emplace_back(attribute.value());
    }
}

MediaDesc::MediaDesc(const MediaDesc &other)
    :m_mediaName(other.m_mediaName)
    ,m_mediaTitle(other.m_mediaTitle)
    ,m_connectionInfos(other.m_connectionInfos)
    ,m_bandwidthInfos(other.m_bandwidthInfos)
    ,m_encryptionKey(other.m_encryptionKey)
    ,m_mediaAttributes(other.m_mediaAttributes)
{
}

MediaDesc::MediaDesc(MediaDesc &&other)
    :m_mediaName(std::move(other.m_mediaName))
    ,m_mediaTitle(std::move(other.m_mediaTitle))
    ,m_connectionInfos(std::move(other.m_connectionInfos))
    ,m_bandwidthInfos(std::move(other.m_bandwidthInfos))
    ,m_encryptionKey(std::move(other.m_encryptionKey))
    ,m_mediaAttributes(std::move(other.m_mediaAttributes))
{
}

MediaDesc &MediaDesc::operator=(const MediaDesc &other)
{
    m_mediaName = other.m_mediaName;
    m_mediaTitle = other.m_mediaTitle;
    m_connectionInfos = other.m_connectionInfos;
    m_bandwidthInfos = other.m_bandwidthInfos;
    m_encryptionKey = other.m_encryptionKey;
    m_mediaAttributes = other.m_mediaAttributes;
    return *this;
}

MediaDesc &MediaDesc::operator=(MediaDesc &&other)
{
    m_mediaName = std::move(other.m_mediaName);
    m_mediaTitle = std::move(other.m_mediaTitle);
    m_connectionInfos = std::move(other.m_connectionInfos);
    m_bandwidthInfos = std::move(other.m_bandwidthInfos);
    m_encryptionKey = std::move(other.m_encryptionKey);
    m_mediaAttributes = std::move(other.m_mediaAttributes);
    return *this;
}

bool MediaDesc::operator==(const MediaDesc &other) const
{
    if (m_mediaTitle == other.m_mediaTitle && m_encryptionKey == other.m_encryptionKey && m_mediaName == other.m_mediaName &&
        m_bandwidthInfos == other.m_bandwidthInfos && m_mediaAttributes == other.m_mediaAttributes) {
        if (m_connectionInfos.size() != other.m_connectionInfos.size()) return false;
        for (auto it_this = m_connectionInfos.begin(), it_other = other.m_connectionInfos.begin();
             it_this != m_connectionInfos.end(); ++it_this,++it_other) {
            if (*it_this != *it_other && **it_this != **it_other) return false;
        }
        return true;
    }
    return false;
}

std::size_t MediaDesc::hash() const
{
    std::hash<std::string> hs;
    std::hash<MediaName> hmn;
    std::hash<std::list<std::shared_ptr<ConnectionInfo>>> hlci;
    std::hash<std::list<BandwidthInfo>> hlbi;
    std::hash<std::list<Attribute>> hla;

    return hmn(m_mediaName) + hs(m_mediaTitle) * 2 + hlci(m_connectionInfos) * 3 + hlbi(m_bandwidthInfos) * 5 +
           hs(m_encryptionKey) * 7 + hla(m_mediaAttributes) * 11;
}

MediaDesc::operator std::string() const
{
    if (!m_mediaName) return std::string();

    std::string result = std::format("{}", m_mediaName);
    if (!m_mediaTitle.empty()) {
        result += std::format("i={}\r\n", m_mediaTitle);
    }
    for (auto &conn : m_connectionInfos) {
        if (conn) result += std::format("{}", *conn);
    }
    for (auto &bw : m_bandwidthInfos) {
        result += std::format("{}", bw);
    }
    if (!m_encryptionKey.empty()) {
        result += std::format("k={}\r\n", m_encryptionKey);
    }
    for (auto &attr : m_mediaAttributes) {
        result += std::format("{}", attr);
    }

    return result;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(MediaDesc) &media_description)
{
    os << static_cast<std::string>(media_description);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
