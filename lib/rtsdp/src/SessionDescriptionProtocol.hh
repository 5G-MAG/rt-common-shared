#ifndef _LIBSDP_SESSION_DESCRIPTION_PROTOCOL_HH_
#define _LIBSDP_SESSION_DESCRIPTION_PROTOCOL_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: SessionDescriptionProtocol class
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
#include <optional>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/Attribute.hh>
#include "BandwidthInfo.hh"
#include "ConnectionInfo.hh"
#include "EmailAddress.hh"
#include "MediaDesc.hh"
#include "Origin.hh"
#include "PhoneNumber.hh"
#include "TimingInfo.hh"

LIBRTSDP_NAMESPACE_START

class SessionDescriptionProtocol {
public:
    SessionDescriptionProtocol();
    SessionDescriptionProtocol(const std::shared_ptr<Origin> &originator, const std::string &session_name,
                               const std::shared_ptr<TimingInfo> &timing_info,
                                const std::string &session_info = std::string(),
                                const std::string &description_uri = std::string(),
                                const EmailAddress &email_address = EmailAddress(),
                                const PhoneNumber &phone_number = PhoneNumber(),
                                const std::shared_ptr<ConnectionInfo> &connection_info = nullptr,
                                const std::string &encryption_key = std::string());
    SessionDescriptionProtocol(const SessionDescriptionProtocol &other);
    SessionDescriptionProtocol(SessionDescriptionProtocol &&other);

    virtual ~SessionDescriptionProtocol() {};

    SessionDescriptionProtocol &operator=(const SessionDescriptionProtocol &other);
    SessionDescriptionProtocol &operator=(SessionDescriptionProtocol &&other);

    bool operator==(const SessionDescriptionProtocol &other) const;

    std::size_t hash() const;
    operator std::string() const;
    bool isValid() const;

    const std::shared_ptr<Origin> &originator() const { return m_originator; };
    SessionDescriptionProtocol &originator(const std::shared_ptr<Origin> &origin) { m_originator = origin; return *this; };
    SessionDescriptionProtocol &originator(std::shared_ptr<Origin> &&origin) { m_originator = std::move(origin); return *this; };

    const std::string &sessionName() const { return m_sessionName; };
    SessionDescriptionProtocol &sessionName(const std::string &val) { m_sessionName = val; return *this; };
    SessionDescriptionProtocol &sessionName(std::string &&val) { m_sessionName = std::move(val); return *this; };

    const std::string &sessionInfo() const { return m_sessionInfo; };
    SessionDescriptionProtocol &sessionInfo(const std::string &val) { m_sessionInfo = val; return *this; };
    SessionDescriptionProtocol &sessionInfo(std::string &&val) { m_sessionInfo = std::move(val); return *this; };
    SessionDescriptionProtocol &sessionInfoClear() { m_sessionInfo.clear(); return *this; };

    const std::string &descriptionURI() const { return m_descriptionURI; };
    SessionDescriptionProtocol &descriptionURI(const std::string &val) { m_descriptionURI = val; return *this; };
    SessionDescriptionProtocol &descriptionURI(std::string &&val) { m_descriptionURI = std::move(val); return *this; };
    SessionDescriptionProtocol &descriptionURIClear() { m_descriptionURI.clear(); return *this; };

    const EmailAddress &emailAddress() const { return m_emailAddress; };
    SessionDescriptionProtocol &emailAddress(const std::string &val) { m_emailAddress = val; return *this; };
    SessionDescriptionProtocol &emailAddress(std::string &&val) { m_emailAddress = std::move(val); return *this; };
    SessionDescriptionProtocol &emailAddress(const EmailAddress &val) { m_emailAddress = val; return *this; };
    SessionDescriptionProtocol &emailAddress(EmailAddress &&val) { m_emailAddress = std::move(val); return *this; };
    SessionDescriptionProtocol &emailAddressClear() { m_emailAddress.clear(); return *this; };

    const PhoneNumber &phoneNumber() const { return m_phoneNumber; };
    SessionDescriptionProtocol &phoneNumber(const std::string &val) { m_phoneNumber = val; return *this; };
    SessionDescriptionProtocol &phoneNumber(std::string &&val) { m_phoneNumber = std::move(val); return *this; };
    SessionDescriptionProtocol &phoneNumber(const PhoneNumber &val) { m_phoneNumber = val; return *this; };
    SessionDescriptionProtocol &phoneNumber(PhoneNumber &&val) { m_phoneNumber = std::move(val); return *this; };
    SessionDescriptionProtocol &phoneNumberClear() { m_phoneNumber.clear(); return *this; };

    const std::shared_ptr<ConnectionInfo> &connectionInfo() const { return m_connectionInfo; };
    SessionDescriptionProtocol &connectionInfo(const std::shared_ptr<ConnectionInfo> &val) {
        m_connectionInfo = val;
        return *this;
    };
    SessionDescriptionProtocol &connectionInfo(std::shared_ptr<ConnectionInfo> &&val) {
        m_connectionInfo = std::move(val);
        return *this;
    };
    SessionDescriptionProtocol &connectionInfoClear() { m_connectionInfo.reset(); return *this; };

    std::list<BandwidthInfo> &bandwidthInfos() { return m_bandwidthInfos; };
    const std::list<BandwidthInfo> &bandwidthInfos() const { return m_bandwidthInfos; };
    SessionDescriptionProtocol &bandwidthInfos(const std::list<BandwidthInfo> &bw_infos) { m_bandwidthInfos = bw_infos; return *this; };
    SessionDescriptionProtocol &bandwidthInfos(std::list<BandwidthInfo> &&bw_infos) { m_bandwidthInfos = std::move(bw_infos); return *this; };

    std::list<std::shared_ptr<TimingInfo>> &timeDescriptions() { return m_timeDescriptions; };
    const std::list<std::shared_ptr<TimingInfo>> &timeDescriptions() const { return m_timeDescriptions; };
    SessionDescriptionProtocol &timeDescriptions(const std::list<std::shared_ptr<TimingInfo>> &timings) { m_timeDescriptions = timings; return *this; };
    SessionDescriptionProtocol &timeDescriptions(std::list<std::shared_ptr<TimingInfo>> &&timings) { m_timeDescriptions = std::move(timings); return *this; };

    const std::string &encryptionKey() const { return m_encryptionKey; };
    SessionDescriptionProtocol &encryptionKey(const std::string &key) { m_encryptionKey = key; return *this; };
    SessionDescriptionProtocol &encryptionKey(std::string &&key) { m_encryptionKey = std::move(key); return *this; };
    SessionDescriptionProtocol &encryptionKeyClear() { m_encryptionKey.clear(); return *this; };

    std::list<Attribute> &sessionAttributes() { return m_sessionAttributes; };
    const std::list<Attribute> &sessionAttributes() const { return m_sessionAttributes; };
    SessionDescriptionProtocol &sessionAttributes(const std::list<Attribute> &attrs) { m_sessionAttributes = attrs; return *this; };
    SessionDescriptionProtocol &sessionAttributes(std::list<Attribute> &&attrs) { m_sessionAttributes = std::move(attrs); return *this; };

    std::list<std::shared_ptr<MediaDesc>> &mediaDescriptions() { return m_mediaDescriptions; };
    const std::list<std::shared_ptr<MediaDesc>> &mediaDescriptions() const { return m_mediaDescriptions; };
    SessionDescriptionProtocol &mediaDescriptions(const std::list<std::shared_ptr<MediaDesc>> &medias) {
        m_mediaDescriptions = medias;
        return *this;
    };
    SessionDescriptionProtocol &mediaDescriptions(std::list<std::shared_ptr<MediaDesc>> &&medias) {
        m_mediaDescriptions = std::move(medias);
        return *this;
    };

private:

    std::shared_ptr<Origin> m_originator; // mandatory
    std::string m_sessionName;            // mandatory
    std::string m_sessionInfo;            // optional
    std::string m_descriptionURI;         // optional
    EmailAddress m_emailAddress;          // optional
    PhoneNumber m_phoneNumber;            // optional
    std::shared_ptr<ConnectionInfo> m_connectionInfo; // only empty when set in all media
    std::list<BandwidthInfo> m_bandwidthInfos; // zero or more
    std::list<std::shared_ptr<TimingInfo>> m_timeDescriptions; // one or more
    std::string m_encryptionKey;          // optional
    std::list<Attribute> m_sessionAttributes; // zero or more
    std::list<std::shared_ptr<MediaDesc>> m_mediaDescriptions; // zero or more
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(SessionDescriptionProtocol), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(SessionDescriptionProtocol) &sdp, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(sdp));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(SessionDescriptionProtocol)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(SessionDescriptionProtocol) &sdp) const { return sdp.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(SessionDescriptionProtocol) &sdp);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_SESSION_DESCRIPTION_PROTOCOL_HH_ */
