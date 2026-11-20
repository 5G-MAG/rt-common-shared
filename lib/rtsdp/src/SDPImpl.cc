/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: SDP implementation class
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

#include <algorithm>
#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/ConnectionInformation.hh>
#include "ConnectionInformationImpl.hh"
#include <rtsdp/MediaDescription.hh>
#include "MediaDescriptionImpl.hh"
#include <rtsdp/Originator.hh>
#include "OriginatorImpl.hh"
#include <rtsdp/SDP.hh>
#include "SessionDescriptionProtocol.hh"
#include "TimingInfo.hh"
#include <rtsdp/TimingInformation.hh>
#include "TimingInformationImpl.hh"

#include "SDPImpl.hh"

LIBRTSDP_NAMESPACE_START

static const std::string empty_string{};

static std::list<SDPImpl::bandwidth_type> BandwidthInfo_list_to_pair_list(const std::list<BandwidthInfo> &from);
static std::list<BandwidthInfo> pair_list_to_BandwidthInfo_list(const std::list<SDPImpl::bandwidth_type> &from);
static std::list<BandwidthInfo> pair_list_to_BandwidthInfo_list(std::list<SDPImpl::bandwidth_type> &&from);

SDPImpl::SDPImpl(const std::shared_ptr<Originator> &originator, const std::string &session_name,
            const std::shared_ptr<TimingInformation> &timing_info,
            const std::string &session_info, const std::string &description_uri,
            const std::string &email_address, const std::string &phone_number,
            const std::shared_ptr<ConnectionInformation> &connection_info,
            const std::string &encryption_key)
    :SDP()
    ,std::enable_shared_from_this<SDPImpl>()
    ,m_sdp()
{
    auto origin_impl = std::dynamic_pointer_cast<OriginatorImpl>(originator);
    auto conn_info_impl = std::dynamic_pointer_cast<ConnectionInformationImpl>(connection_info);
    auto timing_info_impl = std::dynamic_pointer_cast<TimingInformationImpl>(timing_info);
    m_sdp = SessionDescriptionProtocol(origin_impl?origin_impl->origin():std::shared_ptr<Origin>(), session_name, timing_info_impl?timing_info_impl->timingInfo():std::shared_ptr<TimingInfo>(), session_info, description_uri, email_address, phone_number, conn_info_impl?conn_info_impl->connectionInfo():std::shared_ptr<ConnectionInfo>(), encryption_key);
}

SDPImpl::SDPImpl(const SDPImpl &other)
    :SDP()
    ,std::enable_shared_from_this<SDPImpl>()
    ,m_sdp(other.m_sdp)
{
}

SDPImpl::SDPImpl(SDPImpl &&other)
    :SDP()
    ,std::enable_shared_from_this<SDPImpl>()
    ,m_sdp(std::move(other.m_sdp))
{
}

SDPImpl &SDPImpl::operator=(const SDPImpl &other)
{
    m_sdp = other.m_sdp;
    return *this;
}

SDPImpl &SDPImpl::operator=(SDPImpl &&other)
{
    m_sdp = std::move(other.m_sdp);
    return *this;
}

bool SDPImpl::operator==(const SDPImpl &other) const
{
    return m_sdp == other.m_sdp;
}

std::size_t SDPImpl::hash() const
{
    return m_sdp.hash();
}

SDPImpl::operator std::string() const
{
    return static_cast<std::string>(m_sdp);
}

bool SDPImpl::isValid() const
{
    return m_sdp.isValid();
}

uint64_t SDPImpl::sessionId() const
{
    auto &origin = m_sdp.originator();
    return origin?origin->sessionId():0;
}

uint64_t SDPImpl::sessionVersion() const
{
    auto &origin = m_sdp.originator();
    return origin?origin->sessionVersion():0;
}

void SDPImpl::sessionModified()
{
    auto &origin = m_sdp.originator();
    if (origin) origin->updateVersion();
}

std::shared_ptr<Originator> SDPImpl::originator() const
{
    auto impl = std::shared_ptr<OriginatorImpl>(new OriginatorImpl(m_sdp.originator()));
    impl->setParent(weak_sdp_from_this());
    return std::static_pointer_cast<Originator>(impl);
}

const std::string &SDPImpl::originHostname() const
{
    auto &origin = m_sdp.originator();
    if (!origin) return empty_string;
    return origin->originAddress().hostname();
}

SDP &SDPImpl::originHostname(const std::string &host, int family)
{
    if (!m_sdp.originator()) m_sdp.originator(std::shared_ptr<Origin>(new Origin));
    m_sdp.originator()->originAddress(Address(host, family));
    sessionModified();
    return *this;
}

SDP &SDPImpl::originHostname(std::string &&host, int family)
{
    if (!m_sdp.originator()) m_sdp.originator(std::shared_ptr<Origin>(new Origin));
    m_sdp.originator()->originAddress(Address(std::move(host), family));
    sessionModified();
    return *this;
}

std::pair<const struct sockaddr&, socklen_t> SDPImpl::originAddress() const
{
    static const struct sockaddr empty_sa{ .sa_family = AF_UNSPEC };
    const auto &origin = m_sdp.originator();
    if (!origin) {
        return std::pair<const struct sockaddr&, socklen_t>(empty_sa, 0);
    }
    const auto &addr = origin->originAddress();
    if (!addr.sockaddr()) {
        return std::pair<const struct sockaddr&, socklen_t>(empty_sa, 0);
    }
    return std::pair<const struct sockaddr&, socklen_t>(*addr.sockaddr(), addr.sockaddrlen());
}

const std::string &SDPImpl::originUser() const
{
    auto &origin = m_sdp.originator();
    if (!origin) return empty_string;
    return origin->username();
}

const std::string &SDPImpl::sessionName() const
{
    return m_sdp.sessionName();
}

SDP &SDPImpl::sessionName(const std::string &name)
{
    m_sdp.sessionName(name);
    sessionModified();
    return *this;
}

SDP &SDPImpl::sessionName(std::string &&name)
{
    m_sdp.sessionName(std::move(name));
    sessionModified();
    return *this;
}

const std::string &SDPImpl::sessionInfo() const
{
    return m_sdp.sessionInfo();
}

SDP &SDPImpl::sessionInfo(const std::string &info){
    m_sdp.sessionInfo(info);
    sessionModified();
    return *this;
}

SDP &SDPImpl::sessionInfo(std::string &&info){
    m_sdp.sessionInfo(std::move(info));
    sessionModified();
    return *this;
}

const std::string &SDPImpl::descriptionURI() const
{
    return m_sdp.descriptionURI();
}

SDP &SDPImpl::descriptionURI(const std::string &uri)
{
    m_sdp.descriptionURI(uri);
    sessionModified();
    return *this;
}

SDP &SDPImpl::descriptionURI(std::string &&uri)
{
    m_sdp.descriptionURI(std::move(uri));
    sessionModified();
    return *this;
}

const std::string &SDPImpl::emailAddress() const
{
    return m_sdp.emailAddress().str();
}

SDP &SDPImpl::emailAddress(const std::string &email)
{
    m_sdp.emailAddress(email);
    sessionModified();
    return *this;
}

SDP &SDPImpl::emailAddress(std::string &&email)
{
    m_sdp.emailAddress(std::move(email));
    sessionModified();
    return *this;
}

const std::string &SDPImpl::phoneNumber() const
{
    return m_sdp.phoneNumber().str();
}

SDP &SDPImpl::phoneNumber(const std::string &phone)
{
    m_sdp.phoneNumber(phone);
    sessionModified();
    return *this;
}

SDP &SDPImpl::phoneNumber(std::string &&phone)
{
    m_sdp.phoneNumber(std::move(phone));
    sessionModified();
    return *this;
}

std::shared_ptr<ConnectionInformation> SDPImpl::connectionInformation() const
{
    auto impl = std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(m_sdp.connectionInfo()));
    impl->setParent(weak_sdp_from_this());
    return std::static_pointer_cast<ConnectionInformation>(impl);
}

SDP &SDPImpl::connectionInformation(const std::shared_ptr<ConnectionInformation> &conn_info)
{
    if (conn_info) {
        auto impl = std::dynamic_pointer_cast<ConnectionInformationImpl>(conn_info);
        m_sdp.connectionInfo(impl->connectionInfo());
        impl->setParent(weak_sdp_from_this());
    } else {
        m_sdp.connectionInfoClear();
    }
    sessionModified();
    return *this;
}

SDP &SDPImpl::connectionInformation(std::shared_ptr<ConnectionInformation> &&conn_info)
{
    if (conn_info) {
        auto impl = std::dynamic_pointer_cast<ConnectionInformationImpl>(conn_info);
        m_sdp.connectionInfo(std::move(impl->connectionInfo()));
        impl->setParent(weak_sdp_from_this());
    } else {
        m_sdp.connectionInfoClear();
    }
    sessionModified();
    return *this;
}

const std::string &SDPImpl::encryptionKey() const
{
    return m_sdp.encryptionKey();
}

SDP &SDPImpl::encryptionKey(const std::string &key)
{
    m_sdp.encryptionKey(key);
    sessionModified();
    return *this;
}

SDP &SDPImpl::encryptionKey(std::string &&key)
{
    m_sdp.encryptionKey(std::move(key));
    sessionModified();
    return *this;
}

std::list<SDPImpl::bandwidth_type> SDPImpl::bandwidthInfos() const
{
    return BandwidthInfo_list_to_pair_list(m_sdp.bandwidthInfos());
}

SDP &SDPImpl::bandwidthInfos(const std::list<SDPImpl::bandwidth_type> &bandwidth_infos)
{
    m_sdp.bandwidthInfos(pair_list_to_BandwidthInfo_list(bandwidth_infos));
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfos(std::list<SDPImpl::bandwidth_type> &&bandwidth_infos)
{
    m_sdp.bandwidthInfos(pair_list_to_BandwidthInfo_list(std::move(bandwidth_infos)));
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfoAdd(const SDPImpl::bandwidth_type::first_type &bw_type, SDPImpl::bandwidth_value bw_value)
{
    m_sdp.bandwidthInfos().emplace_back(bw_type, bw_value);
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfoAdd(SDPImpl::bandwidth_type::first_type &&bw_type, SDPImpl::bandwidth_value bw_value)
{
    m_sdp.bandwidthInfos().emplace_back(std::move(bw_type), bw_value);
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfoAdd(const SDPImpl::bandwidth_type &bandwidth_info)
{
    m_sdp.bandwidthInfos().emplace_back(bandwidth_info.first, bandwidth_info.second);
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfoAdd(SDPImpl::bandwidth_type &&bandwidth_info)
{
    m_sdp.bandwidthInfos().emplace_back(std::move(bandwidth_info.first), bandwidth_info.second);
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfoRemove(const SDPImpl::bandwidth_type::first_type &bw_type, SDPImpl::bandwidth_value bw_value)
{
    m_sdp.bandwidthInfos().remove(BandwidthInfo(bw_type, bw_value));
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfoRemove(const SDPImpl::bandwidth_type &bandwidth_info)
{
    m_sdp.bandwidthInfos().remove(BandwidthInfo(bandwidth_info.first, bandwidth_info.second));
    sessionModified();
    return *this;
}

SDP &SDPImpl::bandwidthInfosClear()
{
    m_sdp.bandwidthInfos().clear();
    sessionModified();
    return *this;
}

std::list<std::shared_ptr<TimingInformation>> SDPImpl::timingInformations() const
{
    std::list<std::shared_ptr<TimingInformation>> result;

    auto &time_infos = m_sdp.timeDescriptions();
    std::transform(time_infos.begin(), time_infos.end(), std::back_inserter(result), [this](const std::shared_ptr<TimingInfo> &timing_info) -> std::shared_ptr<TimingInformation> {
        auto impl = std::shared_ptr<TimingInformationImpl>(new TimingInformationImpl(timing_info));
        impl->setParent(weak_sdp_from_this());
        return std::static_pointer_cast<TimingInformation>(impl);
    });

    return result;
}

SDP &SDPImpl::timingInformationAdd(const std::shared_ptr<TimingInformation> &timing_information)
{
    if (timing_information) {
        auto impl = std::dynamic_pointer_cast<TimingInformationImpl>(timing_information);
        impl->setParent(weak_sdp_from_this());
        m_sdp.timeDescriptions().push_back(impl->timingInfo());
        sessionModified();
    }
    return *this;
}

SDP &SDPImpl::timingInformationAdd(std::shared_ptr<TimingInformation> &&timing_information)
{
    if (timing_information) {
        auto impl = std::dynamic_pointer_cast<TimingInformationImpl>(std::move(timing_information));
        m_sdp.timeDescriptions().push_back(std::move(impl->timingInfo()));
        sessionModified();
    }
    return *this;
}

SDP &SDPImpl::timingInformationRemove(const std::shared_ptr<TimingInformation> &timing_information)
{
    if (timing_information) {
        auto impl = std::dynamic_pointer_cast<TimingInformationImpl>(timing_information);
        impl->setParent(std::weak_ptr<SDP>());
        m_sdp.timeDescriptions().remove(impl->timingInfo());
        sessionModified();
    }
    return *this;
}

SDP &SDPImpl::timingInformationsClear()
{
    auto &timings = m_sdp.timeDescriptions();
    if (timings.size() > 0) {
        timings.clear();
        sessionModified();
    }
    return *this;
}

const std::list<Attribute> &SDPImpl::sessionAttributes() const
{
    return m_sdp.sessionAttributes();
}

SDP &SDPImpl::sessionAttributes(const std::list<Attribute> &attributes)
{
    m_sdp.sessionAttributes(attributes);
    sessionModified();
    return *this;
}

SDP &SDPImpl::sessionAttributes(std::list<Attribute> &&attributes)
{
    m_sdp.sessionAttributes(std::move(attributes));
    sessionModified();
    return *this;
}

SDP &SDPImpl::sessionAttributeAdd(const Attribute &attribute)
{
    m_sdp.sessionAttributes().push_back(attribute);
    sessionModified();
    return *this;
}

SDP &SDPImpl::sessionAttributeAdd(Attribute &&attribute)
{
    m_sdp.sessionAttributes().push_back(std::move(attribute));
    sessionModified();
    return *this;
}

SDP &SDPImpl::sessionAttributeRemove(const Attribute &attribute)
{
    m_sdp.sessionAttributes().remove(attribute);
    sessionModified();
    return *this;
}

SDP &SDPImpl::sessionAttributesClear()
{
    m_sdp.sessionAttributes().clear();
    sessionModified();
    return *this;
}

std::list<std::shared_ptr<MediaDescription>> SDPImpl::mediaDescriptions() const
{
    std::list<std::shared_ptr<MediaDescription>> result;

    std::transform(m_sdp.mediaDescriptions().begin(), m_sdp.mediaDescriptions().end(), std::back_inserter(result), [this](const std::shared_ptr<MediaDesc> &media_desc) -> std::shared_ptr<MediaDescription> {
        auto impl = std::shared_ptr<MediaDescriptionImpl>(new MediaDescriptionImpl(media_desc));
        impl->setParent(weak_sdp_from_this());
        return std::static_pointer_cast<MediaDescription>(impl);
    });

    return result;
}

SDP &SDPImpl::mediaDescriptionAdd(const std::shared_ptr<MediaDescription> &media_description)
{
    if (media_description) {
        auto impl = std::dynamic_pointer_cast<MediaDescriptionImpl>(media_description);
        impl->setParent(weak_sdp_from_this());
        m_sdp.mediaDescriptions().push_back(impl->mediaDesc());
        sessionModified();
    }
    return *this;
}

SDP &SDPImpl::mediaDescriptionAdd(std::shared_ptr<MediaDescription> &&media_description)
{
    if (media_description) {
        auto impl = std::dynamic_pointer_cast<MediaDescriptionImpl>(std::move(media_description));
        m_sdp.mediaDescriptions().push_back(std::move(impl->mediaDesc()));
        sessionModified();
    }
    return *this;
}

SDP &SDPImpl::mediaDescriptionRemove(const std::shared_ptr<MediaDescription> &media_description)
{
    if (media_description) {
        auto impl = std::dynamic_pointer_cast<MediaDescriptionImpl>(media_description);
        impl->setParent(std::weak_ptr<SDP>());
        m_sdp.mediaDescriptions().remove(impl->mediaDesc());
        sessionModified();
    }
    return *this;
}

SDP &SDPImpl::mediaDescriptionsClear()
{
    auto &medias = m_sdp.mediaDescriptions();
    if (medias.size() > 0) {
        medias.clear();
        sessionModified();
    }
    return *this;
}

// private:

std::weak_ptr<SDP> SDPImpl::weak_sdp_from_this() const
{
    return std::weak_ptr<SDP>(std::static_pointer_cast<SDP>(std::const_pointer_cast<SDPImpl>(shared_from_this())));
}

// local functions

static std::list<SDPImpl::bandwidth_type> BandwidthInfo_list_to_pair_list(const std::list<BandwidthInfo> &from)
{
    std::list<SDPImpl::bandwidth_type> result;
    std::transform(from.begin(), from.end(), std::back_inserter(result),
                   [](const BandwidthInfo &bw_info) -> SDPImpl::bandwidth_type {
        return SDPImpl::bandwidth_type(bw_info.type(), bw_info.value());
    });
    return result;
}

static std::list<BandwidthInfo> pair_list_to_BandwidthInfo_list(const std::list<SDPImpl::bandwidth_type> &from)
{
    std::list<BandwidthInfo> result;
    std::transform(from.begin(), from.end(), std::back_inserter(result),
                   [](const SDPImpl::bandwidth_type &bw_info) -> BandwidthInfo {
        return BandwidthInfo(bw_info.first, bw_info.second);
    });
    return result;
}

static std::list<BandwidthInfo> pair_list_to_BandwidthInfo_list(std::list<SDPImpl::bandwidth_type> &&from)
{
    std::list<BandwidthInfo> result;
    std::transform(from.begin(), from.end(), std::back_inserter(result),
                   [](SDPImpl::bandwidth_type &bw_info) -> BandwidthInfo {
        return BandwidthInfo(std::move(bw_info.first), bw_info.second);
    });
    return result;
}

LIBRTSDP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
