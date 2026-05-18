#ifndef _LIBSDP_MEDIA_DESC_HH_
#define _LIBSDP_MEDIA_DESC_HH_
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

LIBRTSDP_NAMESPACE_START

class MediaDesc {
public:
    MediaDesc();
    MediaDesc(const MediaName &media_name, const std::optional<std::string> &media_title = std::nullopt,
                     const std::optional<ConnectionInfo> &connection_info = std::nullopt,
                     const std::optional<BandwidthInfo> &bandwidth_info = std::nullopt,
                     const std::optional<std::string> &encryption_key = std::nullopt,
                     const std::optional<Attribute> &attribute = std::nullopt);
    MediaDesc(const MediaDesc &other);
    MediaDesc(MediaDesc &&other);

    virtual ~MediaDesc() {};

    MediaDesc &operator=(const MediaDesc &other);
    MediaDesc &operator=(MediaDesc &&other);

    bool operator==(const MediaDesc &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const { return !!m_mediaName; };

    MediaName &mediaName() { return m_mediaName; };
    const MediaName &mediaName() const { return m_mediaName; };
    MediaDesc &mediaName(const MediaName &media_name) { m_mediaName = media_name; return *this; };
    MediaDesc &mediaName(MediaName &&media_name) { m_mediaName = std::move(media_name); return *this; };

    std::string &mediaTitle() { return m_mediaTitle; };
    const std::string &mediaTitle() const { return m_mediaTitle; };
    MediaDesc &mediaTitle(const std::string &media_title) { m_mediaTitle = media_title; return *this; };
    MediaDesc &mediaTitle(std::string &&media_title) { m_mediaTitle = std::move(media_title); return *this; };
    MediaDesc &mediaTitleClear() { m_mediaTitle.clear(); return *this; };

    std::list<std::shared_ptr<ConnectionInfo>> &connectionInfos() { return m_connectionInfos; };
    const std::list<std::shared_ptr<ConnectionInfo>> &connectionInfos() const { return m_connectionInfos; };
    MediaDesc &connectionInfos(const std::list<std::shared_ptr<ConnectionInfo>> &connection_infos) { m_connectionInfos = connection_infos; return *this; };
    MediaDesc &connectionInfos(std::list<std::shared_ptr<ConnectionInfo>> &&connection_infos) { m_connectionInfos = std::move(connection_infos); return *this; };
    MediaDesc &connectionInfoAdd(const std::shared_ptr<ConnectionInfo> &connection_info) { m_connectionInfos.push_back(connection_info); return *this; };
    MediaDesc &connectionInfoAdd(std::shared_ptr<ConnectionInfo> &&connection_info) { m_connectionInfos.push_back(std::move(connection_info)); return *this; };
    MediaDesc &connectionInfoRemove(const std::shared_ptr<ConnectionInfo> &connection_info) { m_connectionInfos.remove(connection_info); return *this; };

    std::list<BandwidthInfo> &bandwidthInfos() { return m_bandwidthInfos; };
    const std::list<BandwidthInfo> &bandwidthInfos() const { return m_bandwidthInfos; };
    MediaDesc &bandwidthInfos(const std::list<BandwidthInfo> &bw_infos) { m_bandwidthInfos = bw_infos; return *this; };
    MediaDesc &bandwidthInfos(std::list<BandwidthInfo> &&bw_infos) { m_bandwidthInfos = std::move(bw_infos); return *this; };
    MediaDesc &bandwidthInfoAdd(const BandwidthInfo &bw_info) { m_bandwidthInfos.push_back(bw_info); return *this; };
    MediaDesc &bandwidthInfoAdd(BandwidthInfo &&bw_info) { m_bandwidthInfos.push_back(std::move(bw_info)); return *this; };
    MediaDesc &bandwidthInfoRemove(const BandwidthInfo &bw_info) { m_bandwidthInfos.remove(bw_info); return *this; };

    std::string &encryptionKey() { return m_encryptionKey; };
    const std::string &encryptionKey() const { return m_encryptionKey; };
    MediaDesc &encryptionKey(const std::string &key) { m_encryptionKey = key; return *this; };
    MediaDesc &encryptionKey(std::string &&key) { m_encryptionKey = std::move(key); return *this; };
    MediaDesc &encryptionKeyClear() { m_encryptionKey.clear(); return *this; };

    std::list<Attribute> &mediaAttributes() { return m_mediaAttributes; };
    const std::list<Attribute> &mediaAttributes() const { return m_mediaAttributes; };
    MediaDesc &mediaAttributes(const std::list<Attribute> &attrs) { m_mediaAttributes = attrs; return *this; };
    MediaDesc &mediaAttributes(std::list<Attribute> &&attrs) { m_mediaAttributes = std::move(attrs); return *this; };
    MediaDesc &mediaAttributeAdd(const Attribute &attr) { m_mediaAttributes.push_back(attr); return *this; };
    MediaDesc &mediaAttributeAdd(Attribute &&attr) { m_mediaAttributes.push_back(std::move(attr)); return *this; };
    MediaDesc &mediaAttributeRemove(const Attribute &attr) { m_mediaAttributes.remove(attr); return *this; };

private:
    MediaName m_mediaName;                       // mandatory
    std::string m_mediaTitle;                    // optional
    std::list<std::shared_ptr<ConnectionInfo>> m_connectionInfos; // zero only if available at session level, or one or more
    std::list<BandwidthInfo> m_bandwidthInfos;   // zero or more
    std::string m_encryptionKey;                 // optional (deprecated)
    std::list<Attribute> m_mediaAttributes;      // zero or more
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(MediaDesc), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(MediaDesc) &media_description, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(media_description));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(MediaDesc)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(MediaDesc) &media_description) const {
        return media_description.hash();
    };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(MediaDesc) &media_description);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_MEDIA_DESC_HH_ */
