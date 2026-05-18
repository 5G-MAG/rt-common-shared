#ifndef _LIBRTSDP_MEDIA_DESCRIPTION_IMPL_HH_
#define _LIBRTSDP_MEDIA_DESCRIPTION_IMPL_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: MediaDescription class
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
#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/Attribute.hh>
#include <rtsdp/ConnectionInformation.hh>
#include "MediaDesc.hh"
#include <rtsdp/MediaDescription.hh>
#include <rtsdp/SDP.hh>

LIBRTSDP_NAMESPACE_START

class MediaDescriptionImpl : public MediaDescription {
public:
    using bandwidth_value = MediaDescription::bandwidth_value;
    using bandwidth_type = MediaDescription::bandwidth_type;

    MediaDescriptionImpl();
    MediaDescriptionImpl(const std::string &media_type, uint16_t port, const std::string &proto, const std::string &fmt, uint16_t number_of_ports = 1);
    MediaDescriptionImpl(const std::shared_ptr<MediaDesc> &media_desc);
    MediaDescriptionImpl(std::shared_ptr<MediaDesc> &&media_desc);
    MediaDescriptionImpl(const MediaDescriptionImpl &other);
    MediaDescriptionImpl(MediaDescriptionImpl &&other);

    virtual ~MediaDescriptionImpl() {};

    MediaDescriptionImpl &operator=(const MediaDescriptionImpl &other);
    MediaDescriptionImpl &operator=(MediaDescriptionImpl &&other);

    const std::shared_ptr<MediaDesc> &mediaDesc() const { return m_mediaDesc; };

    bool operator==(const MediaDescriptionImpl &other) const;

    virtual std::size_t hash() const;
    virtual operator std::string() const;
    operator bool() const { return isValid(); };

    virtual bool isValid() const;

    void setParent(const std::weak_ptr<SDP> &parent) { m_parent = parent; };

    virtual const std::string &mediaTitle() const;
    virtual MediaDescription &mediaTitle(const std::string &media_title);
    virtual MediaDescription &mediaTitleRemove();

    virtual std::list<std::shared_ptr<ConnectionInformation>> connectionInformations() const;
    virtual MediaDescription &connectionInformationAdd(const ConnectionInformation &connection_information);
    virtual MediaDescription &connectionInformationAdd(const std::shared_ptr<ConnectionInformation> &connection_information);
    virtual MediaDescription &connectionInformationAdd(ConnectionInformation &&connection_information);
    virtual MediaDescription &connectionInformationAdd(std::shared_ptr<ConnectionInformation> &&connection_information);
    virtual MediaDescription &connectionInformationRemove(const std::shared_ptr<ConnectionInformation> &connection_information);
    virtual MediaDescription &connectionInformationClear();

    virtual std::list<bandwidth_type> bandwidthInformations() const;
    virtual MediaDescription &bandwidthInformationAdd(bandwidth_value bitrate, const std::string &bw_type = std::string());
    virtual MediaDescription &bandwidthInformationAdd(const bandwidth_type &bandwidth);
    virtual MediaDescription &bandwidthInformationAdd(bandwidth_type &&bandwidth);
    virtual MediaDescription &bandwidthInformationRemove(const bandwidth_type &bandwidth);

    virtual const std::string &encryptionKey() const;
    virtual MediaDescription &encryptionKey(const std::string &encryption_key);
    virtual MediaDescription &encryptionKeyRemove();

    virtual const std::list<Attribute> &mediaAttributes() const;
    virtual MediaDescription &mediaAttributeAdd(const Attribute &attribute);
    virtual MediaDescription &mediaAttributeAdd(Attribute &&attribute);
    virtual MediaDescription &mediaAttributeRemove(const Attribute &attribute);

private:
    std::weak_ptr<SDP> m_parent;
    std::shared_ptr<MediaDesc> m_mediaDesc;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(MediaDescriptionImpl), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(MediaDescriptionImpl) &media_description, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(media_description));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(MediaDescriptionImpl)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(MediaDescriptionImpl) &media_description) const {
        return media_description.hash();
    };
};

}

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(MediaDescriptionImpl) &media_description);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_MEDIA_DESCRIPTION_IMPL_HH_ */
