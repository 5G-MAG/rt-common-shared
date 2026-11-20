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

#include "SessionDescriptionProtocol.hh"

LIBRTSDP_NAMESPACE_START

SessionDescriptionProtocol::SessionDescriptionProtocol()
    :m_originator()
    ,m_sessionName()
    ,m_sessionInfo()
    ,m_descriptionURI()
    ,m_emailAddress()
    ,m_phoneNumber()
    ,m_connectionInfo()
    ,m_bandwidthInfos()
    ,m_timeDescriptions()
    ,m_encryptionKey()
    ,m_sessionAttributes()
    ,m_mediaDescriptions()
{
}

SessionDescriptionProtocol::SessionDescriptionProtocol(const std::shared_ptr<Origin> &originator, const std::string &session_name,
                                                       const std::shared_ptr<TimingInfo> &timing_info,
                                                       const std::string &session_info, const std::string &description_uri,
                                                       const EmailAddress &email_address, const PhoneNumber &phone_number,
                                                       const std::shared_ptr<ConnectionInfo> &connection_info,
                                                       const std::string &encryption_key)
    :m_originator(originator)
    ,m_sessionName(session_name)
    ,m_sessionInfo(session_info)
    ,m_descriptionURI(description_uri)
    ,m_emailAddress(email_address)
    ,m_phoneNumber(phone_number)
    ,m_connectionInfo(connection_info)
    ,m_bandwidthInfos()
    ,m_timeDescriptions({timing_info})
    ,m_encryptionKey(encryption_key)
    ,m_sessionAttributes()
    ,m_mediaDescriptions()
{
}

SessionDescriptionProtocol::SessionDescriptionProtocol(const SessionDescriptionProtocol &other)
    :m_originator(other.m_originator)
    ,m_sessionName(other.m_sessionName)
    ,m_sessionInfo(other.m_sessionInfo)
    ,m_descriptionURI(other.m_descriptionURI)
    ,m_emailAddress(other.m_emailAddress)
    ,m_phoneNumber(other.m_phoneNumber)
    ,m_connectionInfo(other.m_connectionInfo)
    ,m_bandwidthInfos(other.m_bandwidthInfos)
    ,m_timeDescriptions(other.m_timeDescriptions)
    ,m_encryptionKey(other.m_encryptionKey)
    ,m_sessionAttributes(other.m_sessionAttributes)
    ,m_mediaDescriptions(other.m_mediaDescriptions)
{
}

SessionDescriptionProtocol::SessionDescriptionProtocol(SessionDescriptionProtocol &&other)
    :m_originator(std::move(other.m_originator))
    ,m_sessionName(std::move(other.m_sessionName))
    ,m_sessionInfo(std::move(other.m_sessionInfo))
    ,m_descriptionURI(std::move(other.m_descriptionURI))
    ,m_emailAddress(std::move(other.m_emailAddress))
    ,m_phoneNumber(std::move(other.m_phoneNumber))
    ,m_connectionInfo(std::move(other.m_connectionInfo))
    ,m_bandwidthInfos(std::move(other.m_bandwidthInfos))
    ,m_timeDescriptions(std::move(other.m_timeDescriptions))
    ,m_encryptionKey(std::move(other.m_encryptionKey))
    ,m_sessionAttributes(std::move(other.m_sessionAttributes))
    ,m_mediaDescriptions(std::move(other.m_mediaDescriptions))
{
}

SessionDescriptionProtocol &SessionDescriptionProtocol::operator=(const SessionDescriptionProtocol &other)
{
    m_originator = other.m_originator;
    m_sessionName = other.m_sessionName;
    m_sessionInfo = other.m_sessionInfo;
    m_descriptionURI = other.m_descriptionURI;
    m_emailAddress = other.m_emailAddress;
    m_phoneNumber = other.m_phoneNumber;
    m_connectionInfo = other.m_connectionInfo;
    m_bandwidthInfos = other.m_bandwidthInfos;
    m_timeDescriptions = other.m_timeDescriptions;
    m_encryptionKey = other.m_encryptionKey;
    m_sessionAttributes = other.m_sessionAttributes;
    m_mediaDescriptions = other.m_mediaDescriptions;
    return *this;
}

SessionDescriptionProtocol &SessionDescriptionProtocol::operator=(SessionDescriptionProtocol &&other)
{
    m_originator = std::move(other.m_originator);
    m_sessionName = std::move(other.m_sessionName);
    m_sessionInfo = std::move(other.m_sessionInfo);
    m_descriptionURI = std::move(other.m_descriptionURI);
    m_emailAddress = std::move(other.m_emailAddress);
    m_phoneNumber = std::move(other.m_phoneNumber);
    m_connectionInfo = std::move(other.m_connectionInfo);
    m_bandwidthInfos = std::move(other.m_bandwidthInfos);
    m_timeDescriptions = std::move(other.m_timeDescriptions);
    m_encryptionKey = std::move(other.m_encryptionKey);
    m_sessionAttributes = std::move(other.m_sessionAttributes);
    m_mediaDescriptions = std::move(other.m_mediaDescriptions);
    return *this;
}

bool SessionDescriptionProtocol::operator==(const SessionDescriptionProtocol &other) const
{
    return m_originator == other.m_originator &&
            m_sessionName == other.m_sessionName &&
            m_sessionInfo == other.m_sessionInfo &&
            m_descriptionURI == other.m_descriptionURI &&
            m_emailAddress == other.m_emailAddress &&
            m_phoneNumber == other.m_phoneNumber &&
            m_connectionInfo == other.m_connectionInfo &&
            m_bandwidthInfos == other.m_bandwidthInfos &&
            m_timeDescriptions == other.m_timeDescriptions &&
            m_encryptionKey == other.m_encryptionKey &&
            m_sessionAttributes == other.m_sessionAttributes &&
            m_mediaDescriptions == other.m_mediaDescriptions;
}

std::size_t SessionDescriptionProtocol::hash() const
{
    std::hash<std::string> hstr;
    std::hash<size_t> hsz;

    std::size_t result = (m_originator?m_originator->hash():0) + hstr(m_sessionName) * 2 + hstr(m_sessionInfo) * 3 +
                         hstr(m_descriptionURI) * 5 + m_emailAddress.hash() * 7 + m_phoneNumber.hash() * 11 +
                         hstr(m_encryptionKey) * 13;
    if (m_connectionInfo) result += m_connectionInfo->hash() * 17;
    size_t idx = 0;
    for (auto &bw : m_bandwidthInfos) {
        result += hsz(++idx) ^ bw.hash();
    }
    for (auto &timew : m_timeDescriptions) {
        result += hsz(++idx) ^ timew->hash();
    }
    for (auto &attr : m_sessionAttributes) {
        result += hsz(++idx) ^ attr.hash();
    }
    for (auto &media : m_mediaDescriptions) {
        if (media) result += hsz(++idx) ^ media->hash();
    }

    return result;
}

SessionDescriptionProtocol::operator std::string() const
{
    if (!isValid()) {
        std::string reason="";
        const char *sep = "";
        if (!m_originator || !m_originator->isValid()) {
            reason += "No or bad origin";
            sep = "; ";
        }
        if (m_timeDescriptions.empty()) {
            reason += sep;
            reason += "Empty timings information";
            sep = "; ";
        }
        if (!m_connectionInfo) {
            if (m_mediaDescriptions.empty()) {
                reason += sep;
                reason += "No session connection information and no media descriptions";
                sep = "; ";
            } else {
                for (auto &media : m_mediaDescriptions) {
                    if (media->connectionInfos().empty()) {
                        reason += sep;
                        reason += "No session connection information and not all media descriptions have connection information";
                        sep = "; ";
                        break;
                    }
                }
            }
        } else if (!m_connectionInfo->isValid()) {
            reason += sep;
            reason += "Session connection information is not valid";
            sep = "; ";
        }
        throw std::out_of_range("This is not a valid SDP: " + reason);
    }
    std::string result = std::format("v=0\r\n{}s={}\r\n", *m_originator, m_sessionName);
    if (!m_sessionInfo.empty()) result += std::format("i={}\r\n", m_sessionInfo);
    if (!m_descriptionURI.empty()) result += std::format("u={}\r\n", m_descriptionURI);
    result += std::format("{}{}", m_emailAddress, m_phoneNumber);
    if (m_connectionInfo) result += std::format("{}", *m_connectionInfo);
    for (auto &bw : m_bandwidthInfos) {
        result += std::format("{}", bw);
    }
    for (auto &timew : m_timeDescriptions) {
        if (timew) result += std::format("{}", *timew);
    }
    if (!m_encryptionKey.empty()) result += std::format("k={}\r\n", m_encryptionKey);
    for (auto &attr : m_sessionAttributes) {
        result += std::format("{}", attr);
    }
    for (auto &media : m_mediaDescriptions) {
        if (media) result += std::format("{}", *media);
    }

    return result;
}

bool SessionDescriptionProtocol::isValid() const
{
    if (m_originator && m_originator->isValid() && !m_timeDescriptions.empty())
    {
        if (m_connectionInfo && m_connectionInfo->isValid()) return true;
        if (!m_mediaDescriptions.empty()) {
            bool result = true;
            for (const auto &media : m_mediaDescriptions) {
                if (media->connectionInfos().empty()) {
                    result = false;
                    break;
                }
            }
            return result;
        }
    }
    return false;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(SessionDescriptionProtocol) &sdp)
{
    os << static_cast<std::string>(sdp);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
