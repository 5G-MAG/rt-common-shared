#ifndef _LIBRTSDP_PUB_SESSION_DESCRIPTION_PROTOCOL_IMPL_HH_
#define _LIBRTSDP_PUB_SESSION_DESCRIPTION_PROTOCOL_IMPL_HH_
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

#include <netinet/in.h>
#include <sys/socket.h>

#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/ConnectionInformation.hh>
#include <rtsdp/Originator.hh>
#include <rtsdp/SDP.hh>
#include <rtsdp/TimingInformation.hh>
#include "SessionDescriptionProtocol.hh"

LIBRTSDP_NAMESPACE_START

class SDPImpl : public SDP, public std::enable_shared_from_this<SDPImpl> {
public:
    using bandwidth_value = SDP::bandwidth_value;
    using bandwidth_type = SDP::bandwidth_type;

    SDPImpl() :SDP(), std::enable_shared_from_this<SDPImpl>(), m_sdp() {};
    SDPImpl(const std::shared_ptr<Originator> &originator, const std::string &session_name,
            const std::shared_ptr<TimingInformation> &timing_info,
            const std::string &session_info = std::string(), const std::string &description_uri = std::string(),
            const std::string &email_address = std::string(), const std::string &phone_number = std::string(),
            const std::shared_ptr<ConnectionInformation> &connection_info = nullptr,
            const std::string &encryption_key = std::string());
    SDPImpl(const SDPImpl &other);
    SDPImpl(SDPImpl &&other);

    virtual ~SDPImpl() {};

    SDPImpl &operator=(const SDPImpl &other);
    SDPImpl &operator=(SDPImpl &&other);

    bool operator==(const SDPImpl &other) const;

    virtual std::size_t hash() const;
    virtual operator std::string() const;

    virtual bool isValid() const;

    virtual uint64_t sessionId() const;
    virtual uint64_t sessionVersion() const;
    virtual void sessionModified();

    virtual std::shared_ptr<Originator> originator() const;
    virtual const std::string &originHostname() const;
    virtual SDP &originHostname(const std::string &host, int family = AF_UNSPEC);
    virtual SDP &originHostname(std::string &&host, int family = AF_UNSPEC);
    virtual std::pair<const struct sockaddr&, socklen_t> originAddress() const;
    virtual const std::string &originUser() const;

    virtual const std::string &sessionName() const;
    virtual SDP &sessionName(const std::string &name);
    virtual SDP &sessionName(std::string &&name);

    virtual const std::string &sessionInfo() const;
    virtual SDP &sessionInfo(const std::string &info);
    virtual SDP &sessionInfo(std::string &&info);

    virtual const std::string &descriptionURI() const;
    virtual SDP &descriptionURI(const std::string &uri);
    virtual SDP &descriptionURI(std::string &&uri);

    virtual const std::string &emailAddress() const;
    virtual SDP &emailAddress(const std::string &email);
    virtual SDP &emailAddress(std::string &&email);

    virtual const std::string &phoneNumber() const;
    virtual SDP &phoneNumber(const std::string &phone);
    virtual SDP &phoneNumber(std::string &&phone);
    
    virtual std::shared_ptr<ConnectionInformation> connectionInformation() const;
    virtual SDP& connectionInformation(const std::shared_ptr<ConnectionInformation> &conn_info);
    virtual SDP &connectionInformation(std::shared_ptr<ConnectionInformation> &&conn_info);

    virtual const std::string &encryptionKey() const;
    virtual SDP &encryptionKey(const std::string &key);
    virtual SDP &encryptionKey(std::string &&key);

    virtual std::list<bandwidth_type> bandwidthInfos() const;
    virtual SDP &bandwidthInfos(const std::list<bandwidth_type> &bandwidth_infos);
    virtual SDP &bandwidthInfos(std::list<bandwidth_type> &&bandwidth_infos);
    virtual SDP &bandwidthInfoAdd(const bandwidth_type::first_type &bw_type, bandwidth_value bw_value);
    virtual SDP &bandwidthInfoAdd(bandwidth_type::first_type &&bw_type, bandwidth_value bw_value);
    virtual SDP &bandwidthInfoAdd(const bandwidth_type &bandwidth_info);
    virtual SDP &bandwidthInfoAdd(bandwidth_type &&bandwidth_info);
    virtual SDP &bandwidthInfoRemove(const bandwidth_type::first_type &bw_type, bandwidth_value bw_value);
    virtual SDP &bandwidthInfoRemove(const bandwidth_type &bandwidth_info);
    virtual SDP &bandwidthInfosClear();

    virtual std::list<std::shared_ptr<TimingInformation>> timingInformations() const;
    virtual SDP &timingInformationAdd(const std::shared_ptr<TimingInformation> &timing_information);
    virtual SDP &timingInformationAdd(std::shared_ptr<TimingInformation> &&timing_information);
    virtual SDP &timingInformationRemove(const std::shared_ptr<TimingInformation> &timing_information);
    virtual SDP &timingInformationsClear();

    virtual const std::list<Attribute> &sessionAttributes() const;
    virtual SDP &sessionAttributes(const std::list<Attribute> &attributes);
    virtual SDP &sessionAttributes(std::list<Attribute> &&attributes);
    virtual SDP &sessionAttributeAdd(const Attribute &attribute);
    virtual SDP &sessionAttributeAdd(Attribute &&attribute);
    virtual SDP &sessionAttributeRemove(const Attribute &attribute);
    virtual SDP &sessionAttributesClear();

    virtual std::list<std::shared_ptr<MediaDescription>> mediaDescriptions() const;
    virtual SDP &mediaDescriptionAdd(const std::shared_ptr<MediaDescription> &media_description);
    virtual SDP &mediaDescriptionAdd(std::shared_ptr<MediaDescription> &&media_description);
    virtual SDP &mediaDescriptionRemove(const std::shared_ptr<MediaDescription> &media_description);
    virtual SDP &mediaDescriptionsClear();

private:
    std::weak_ptr<SDP> weak_sdp_from_this() const;

    SessionDescriptionProtocol m_sdp;
};

LIBRTSDP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_SESSION_DESCRIPTION_PROTOCOL_IMPL_HH_ */
