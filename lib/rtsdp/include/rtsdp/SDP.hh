#ifndef _LIBRTSDP_PUB_SESSION_DESCRIPTION_PROTOCOL_HH_
#define _LIBRTSDP_PUB_SESSION_DESCRIPTION_PROTOCOL_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: SDP class
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
#include <sys/socket.h>

#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "common.hh"
#include "Attribute.hh"
#include "ConnectionInformation.hh"
#include "MediaDescription.hh"
#include "Originator.hh"
#include "TimingInformation.hh"

LIBRTSDP_NAMESPACE_START

class SDP {
public:
    using bandwidth_value = unsigned long int;
    using bandwidth_type = std::pair<std::string, bandwidth_value>;

    static const std::string library_version;

    static std::shared_ptr<SDP> makeSDP();
    static std::shared_ptr<SDP> makeSDP(const std::shared_ptr<Originator> &originator, const std::string &session_name,
                                        const std::shared_ptr<TimingInformation> &timing_info,
                                        const std::string &session_info = std::string(),
                                        const std::string &description_uri = std::string(),
                                        const std::string &email_address = std::string(),
                                        const std::string &phone_number = std::string(),
                                        const std::shared_ptr<ConnectionInformation> &connection_info = nullptr,
                                        const std::string &encryption_key = std::string());
    static std::shared_ptr<SDP> makeSDP(const std::shared_ptr<SDP> &other); // Copies the underlying SDP object

    virtual ~SDP() {};

    bool operator==(const SDP &other) const;

    virtual std::size_t hash() const = 0;
    virtual operator std::string() const = 0;
    operator bool() const { return isValid(); };

    virtual bool isValid() const = 0;

    virtual uint64_t sessionId() const = 0;
    virtual uint64_t sessionVersion() const = 0;
    virtual void sessionModified() = 0;

    virtual std::shared_ptr<Originator> originator() const = 0;
    virtual const std::string &originHostname() const = 0;
    virtual SDP &originHostname(const std::string &host, int family = AF_UNSPEC) = 0;
    virtual SDP &originHostname(std::string &&host, int family = AF_UNSPEC) = 0;
    virtual std::pair<const struct sockaddr&, socklen_t> originAddress() const = 0;
    virtual const std::string &originUser() const = 0;

    virtual const std::string &sessionName() const = 0;
    virtual SDP &sessionName(const std::string &name) = 0;
    virtual SDP &sessionName(std::string &&name) = 0;

    virtual const std::string &sessionInfo() const = 0;
    virtual SDP &sessionInfo(const std::string &info) = 0;
    virtual SDP &sessionInfo(std::string &&info) = 0;

    virtual const std::string &descriptionURI() const = 0;
    virtual SDP &descriptionURI(const std::string &uri) = 0;
    virtual SDP &descriptionURI(std::string &&uri) = 0;

    virtual const std::string &emailAddress() const = 0;
    virtual SDP &emailAddress(const std::string &email) = 0;
    virtual SDP &emailAddress(std::string &&email) = 0;

    virtual const std::string &phoneNumber() const = 0;
    virtual SDP &phoneNumber(const std::string &phone) = 0;
    virtual SDP &phoneNumber(std::string &&phone) = 0;

    virtual std::shared_ptr<ConnectionInformation> connectionInformation() const = 0;
    virtual SDP &connectionInformation(const std::shared_ptr<ConnectionInformation> &conn_info) = 0;
    virtual SDP &connectionInformation(std::shared_ptr<ConnectionInformation> &&conn_info) = 0;

    virtual const std::string &encryptionKey() const = 0;
    virtual SDP &encryptionKey(const std::string &key) = 0;
    virtual SDP &encryptionKey(std::string &&key) = 0;

    virtual std::list<bandwidth_type> bandwidthInfos() const = 0;
    virtual SDP &bandwidthInfos(const std::list<bandwidth_type> &bandwidth_infos) = 0;
    virtual SDP &bandwidthInfos(std::list<bandwidth_type> &&bandwidth_infos) = 0;
    virtual SDP &bandwidthInfoAdd(const bandwidth_type::first_type &bw_type, bandwidth_value bw_value) = 0;
    virtual SDP &bandwidthInfoAdd(bandwidth_type::first_type &&bw_type, bandwidth_value bw_value) = 0;
    virtual SDP &bandwidthInfoAdd(const bandwidth_type &bandwidth_info) = 0;
    virtual SDP &bandwidthInfoAdd(bandwidth_type &&bandwidth_info) = 0;
    virtual SDP &bandwidthInfoRemove(const bandwidth_type::first_type &bw_type, bandwidth_value bw_value) = 0;
    virtual SDP &bandwidthInfoRemove(const bandwidth_type &bandwidth_info) = 0;
    virtual SDP &bandwidthInfosClear() = 0;

    virtual std::list<std::shared_ptr<TimingInformation>> timingInformations() const = 0;
    virtual SDP &timingInformationAdd(const std::shared_ptr<TimingInformation> &timing_information) = 0;
    virtual SDP &timingInformationAdd(std::shared_ptr<TimingInformation> &&timing_information) = 0;
    virtual SDP &timingInformationRemove(const std::shared_ptr<TimingInformation> &timing_information) = 0;
    virtual SDP &timingInformationsClear() = 0;

    virtual const std::list<Attribute> &sessionAttributes() const = 0;
    virtual SDP &sessionAttributes(const std::list<Attribute> &attributes) = 0;
    virtual SDP &sessionAttributes(std::list<Attribute> &&attributes) = 0;
    SDP &sessionAttributeAdd(const std::string &field, const std::optional<std::string> &value = std::nullopt) {
        return sessionAttributeAdd(Attribute(field, value));
    };
    virtual SDP &sessionAttributeAdd(const Attribute &attribute) = 0;
    virtual SDP &sessionAttributeAdd(Attribute &&attribute) = 0;
    virtual SDP &sessionAttributeRemove(const Attribute &attribute) = 0;
    virtual SDP &sessionAttributesClear() = 0;

    virtual std::list<std::shared_ptr<MediaDescription>> mediaDescriptions() const = 0;
    virtual SDP &mediaDescriptionAdd(const std::shared_ptr<MediaDescription> &media_description) = 0;
    virtual SDP &mediaDescriptionAdd(std::shared_ptr<MediaDescription> &&media_description) = 0;
    virtual SDP &mediaDescriptionRemove(const std::shared_ptr<MediaDescription> &media_description) = 0;
    virtual SDP &mediaDescriptionsClear() = 0;

protected:
    SDP() = default;
    SDP(const SDP &other) = default;
    SDP(SDP &&other) = default;

    SDP &operator=(const SDP &other) = default;
    SDP &operator=(SDP &&other) = default;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(SDP), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(SDP) &sdp, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(sdp));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(SDP)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(SDP) &sdp) const { return sdp.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(SDP) &sdp);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_SESSION_DESCRIPTION_PROTOCOL_HH_ */
