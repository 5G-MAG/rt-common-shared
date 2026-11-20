#ifndef _LIBRTSDP_PUB_MEDIA_DESCRIPTION_HH_
#define _LIBRTSDP_PUB_MEDIA_DESCRIPTION_HH_
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

#include <netinet/in.h>

#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "common.hh"
#include "Attribute.hh"
#include "ConnectionInformation.hh"

LIBRTSDP_NAMESPACE_START

class MediaDescription {
public:
    using bandwidth_value = unsigned long int;
    using bandwidth_type = std::pair<std::string, bandwidth_value>;

    static std::shared_ptr<MediaDescription> makeMediaDescription();
    static std::shared_ptr<MediaDescription> makeMediaDescription(const std::string &media_type, uint16_t port, const std::string &proto, const std::string &fmt, uint16_t number_of_ports = 1);

    virtual ~MediaDescription() {};

    bool operator==(const MediaDescription &other) const;

    virtual std::size_t hash() const = 0;
    virtual operator std::string() const = 0;
    operator bool() const { return isValid(); };

    virtual bool isValid() const = 0;

    virtual const std::string &mediaTitle() const = 0;
    virtual MediaDescription &mediaTitle(const std::string &media_title) = 0;
    virtual MediaDescription &mediaTitleRemove() = 0;

    virtual std::list<std::shared_ptr<ConnectionInformation>> connectionInformations() const = 0;
    virtual MediaDescription &connectionInformationAdd(const ConnectionInformation &connection_information) = 0;
    virtual MediaDescription &connectionInformationAdd(const std::shared_ptr<ConnectionInformation> &connection_information) = 0;
    virtual MediaDescription &connectionInformationAdd(ConnectionInformation &&connection_information) = 0;
    virtual MediaDescription &connectionInformationAdd(std::shared_ptr<ConnectionInformation> &&connection_information) = 0;
    virtual MediaDescription &connectionInformationRemove(const std::shared_ptr<ConnectionInformation> &connection_information) = 0;
    virtual MediaDescription &connectionInformationClear() = 0;

    virtual std::list<bandwidth_type> bandwidthInformations() const = 0;
    virtual MediaDescription &bandwidthInformationAdd(bandwidth_value bitrate, const std::string &bw_type = std::string()) = 0;
    virtual MediaDescription &bandwidthInformationAdd(const bandwidth_type &bandwidth) = 0;
    virtual MediaDescription &bandwidthInformationAdd(bandwidth_type &&bandwidth) = 0;
    virtual MediaDescription &bandwidthInformationRemove(const bandwidth_type &bandwidth) = 0;

    virtual const std::string &encryptionKey() const = 0;
    virtual MediaDescription &encryptionKey(const std::string &encryption_key) = 0;
    virtual MediaDescription &encryptionKeyRemove() = 0;

    virtual const std::list<Attribute> &mediaAttributes() const = 0;
    MediaDescription &mediaAttributeAdd(const std::string &field, const std::optional<std::string> &value = std::nullopt) {
        return mediaAttributeAdd(Attribute(field, value));
    };
    virtual MediaDescription &mediaAttributeAdd(const Attribute &attribute) = 0;
    virtual MediaDescription &mediaAttributeAdd(Attribute &&attribute) = 0;
    virtual MediaDescription &mediaAttributeRemove(const Attribute &attribute) = 0;

protected:
    MediaDescription();
    MediaDescription(const MediaDescription &other);
    MediaDescription(MediaDescription &&other);

    MediaDescription &operator=(const MediaDescription &other);
    MediaDescription &operator=(MediaDescription &&other);
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(MediaDescription), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(MediaDescription) &media_description, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(media_description));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(MediaDescription)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(MediaDescription) &media_description) const {
        return media_description.hash();
    };
};

}

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(MediaDescription) &media_description);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_MEDIA_DESCRIPTION_HH_ */
