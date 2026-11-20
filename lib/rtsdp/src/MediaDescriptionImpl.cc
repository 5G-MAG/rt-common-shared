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

#include <algorithm>
#include <format>
#include <iostream>
#include <list>
#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/Attribute.hh>
#include "BandwidthInfo.hh"
#include <rtsdp/ConnectionInformation.hh>
#include "ConnectionInformationImpl.hh"
#include "MediaDesc.hh"
#include <rtsdp/MediaDescription.hh>
#include "MediaName.hh"

#include "MediaDescriptionImpl.hh"

LIBRTSDP_NAMESPACE_START

static const std::string empty_string{};

MediaDescriptionImpl::MediaDescriptionImpl()
    :MediaDescription()
    ,m_parent()
    ,m_mediaDesc()
{
}

MediaDescriptionImpl::MediaDescriptionImpl(const std::string &media_type, uint16_t port, const std::string &proto, const std::string &fmt, uint16_t number_of_ports)
    :MediaDescription()
    ,m_parent()
    ,m_mediaDesc(new MediaDesc(MediaName(media_type, port, proto, fmt, number_of_ports)))
{
}

MediaDescriptionImpl::MediaDescriptionImpl(const std::shared_ptr<MediaDesc> &media_desc)
    :MediaDescription()
    ,m_parent()
    ,m_mediaDesc(media_desc)
{
}

MediaDescriptionImpl::MediaDescriptionImpl(std::shared_ptr<MediaDesc> &&media_desc)
    :MediaDescription()
    ,m_parent()
    ,m_mediaDesc(std::move(media_desc))
{
}

MediaDescriptionImpl::MediaDescriptionImpl(const MediaDescriptionImpl &other)
    :MediaDescription(other)
    ,m_parent(other.m_parent)
    ,m_mediaDesc(other.m_mediaDesc)
{
}

MediaDescriptionImpl::MediaDescriptionImpl(MediaDescriptionImpl &&other)
    :MediaDescription(other)
    ,m_parent(std::move(other.m_parent))
    ,m_mediaDesc(std::move(other.m_mediaDesc))
{
}

MediaDescriptionImpl &MediaDescriptionImpl::operator=(const MediaDescriptionImpl &other)
{
    //MediaDescription::operator=(other);
    m_parent = other.m_parent;
    m_mediaDesc = other.m_mediaDesc;
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescriptionImpl &MediaDescriptionImpl::operator=(MediaDescriptionImpl &&other)
{
    //MediaDescription::operator=(std::move(other));
    m_parent = std::move(other.m_parent);
    m_mediaDesc = std::move(other.m_mediaDesc);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

bool MediaDescriptionImpl::operator==(const MediaDescriptionImpl &other) const
{
    if (!m_mediaDesc != !other.m_mediaDesc) return false;
    if (m_mediaDesc == other.m_mediaDesc) return true;
    if (m_mediaDesc && *m_mediaDesc == *other.m_mediaDesc) return true;
    return false;
}

std::size_t MediaDescriptionImpl::hash() const
{
    return m_mediaDesc?m_mediaDesc->hash():0;
}

MediaDescriptionImpl::operator std::string() const
{
    if (!m_mediaDesc) return std::string();
    return static_cast<std::string>(*m_mediaDesc);
}

bool MediaDescriptionImpl::isValid() const
{
    return !!m_mediaDesc;
}

const std::string &MediaDescriptionImpl::mediaTitle() const
{
    if (m_mediaDesc) return m_mediaDesc->mediaTitle();
    return empty_string;
}

MediaDescription &MediaDescriptionImpl::mediaTitle(const std::string &media_title)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->mediaTitle(media_title);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::mediaTitleRemove()
{
    if (m_mediaDesc) {
        m_mediaDesc->mediaTitle("");
        if (!m_parent.expired()) m_parent.lock()->sessionModified();
    }
    return *this;
}

std::list<std::shared_ptr<ConnectionInformation>> MediaDescriptionImpl::connectionInformations() const
{
    std::list<std::shared_ptr<ConnectionInformation>> result;
    if (m_mediaDesc) {
        auto &conn_list = m_mediaDesc->connectionInfos();
        std::transform(conn_list.begin(), conn_list.end(), std::back_inserter(result), [](const std::shared_ptr<ConnectionInfo> &conn_info) -> std::shared_ptr<ConnectionInformation> {
            auto conn_impl = std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(conn_info));
            return std::static_pointer_cast<ConnectionInformation>(conn_impl);
        });
    }
    return result;
}

MediaDescription &MediaDescriptionImpl::connectionInformationAdd(const ConnectionInformation &connection_information)
{
    const auto &conn_info_impl = dynamic_cast<const ConnectionInformationImpl&>(connection_information);
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->connectionInfoAdd(conn_info_impl.connectionInfo());
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::connectionInformationAdd(const std::shared_ptr<ConnectionInformation> &connection_information)
{
    auto conn_info_impl = std::dynamic_pointer_cast<ConnectionInformationImpl>(connection_information);
    conn_info_impl->setParent(m_parent);
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->connectionInfoAdd(conn_info_impl->connectionInfo());
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::connectionInformationAdd(ConnectionInformation &&connection_information)
{
    auto &&conn_info_impl = dynamic_cast<ConnectionInformationImpl&&>(connection_information);
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->connectionInfoAdd(std::move(conn_info_impl.connectionInfo()));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::connectionInformationAdd(std::shared_ptr<ConnectionInformation> &&connection_information)
{
    auto conn_info_impl = std::dynamic_pointer_cast<ConnectionInformationImpl>(std::move(connection_information));
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->connectionInfoAdd(std::move(conn_info_impl->connectionInfo()));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::connectionInformationRemove(const std::shared_ptr<ConnectionInformation> &connection_information)
{
    if (m_mediaDesc) {
        auto conn_info_impl = std::dynamic_pointer_cast<ConnectionInformationImpl>(connection_information);
        conn_info_impl->setParent(std::weak_ptr<SDP>());
        m_mediaDesc->connectionInfoRemove(conn_info_impl->connectionInfo());
        if (!m_parent.expired()) m_parent.lock()->sessionModified();
    }
    return *this;
}

MediaDescription &MediaDescriptionImpl::connectionInformationClear()
{
    if (m_mediaDesc) {
        m_mediaDesc->connectionInfos().clear();
        if (!m_parent.expired()) m_parent.lock()->sessionModified();
    }
    return *this;
}

std::list<MediaDescriptionImpl::bandwidth_type> MediaDescriptionImpl::bandwidthInformations() const
{
    std::list<bandwidth_type> result;
    if (m_mediaDesc) {
        auto &bw_infos = m_mediaDesc->bandwidthInfos();
        std::transform(bw_infos.cbegin(), bw_infos.cend(), std::back_inserter(result), [](const BandwidthInfo &bw_info) -> bandwidth_type {
            return bandwidth_type(bw_info.type(), bw_info.value());
        });
    }
    return result;
}

MediaDescription &MediaDescriptionImpl::bandwidthInformationAdd(bandwidth_value bitrate, const std::string &bw_type)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->bandwidthInfoAdd(BandwidthInfo(bw_type, bitrate));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::bandwidthInformationAdd(const bandwidth_type &bandwidth)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->bandwidthInfoAdd(BandwidthInfo(bandwidth.first, bandwidth.second));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::bandwidthInformationAdd(bandwidth_type &&bandwidth)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->bandwidthInfoAdd(BandwidthInfo(std::move(bandwidth.first), std::move(bandwidth.second)));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::bandwidthInformationRemove(const bandwidth_type &bandwidth)
{
    if (m_mediaDesc) {
        BandwidthInfo bwi{bandwidth.first, bandwidth.second};
        m_mediaDesc->bandwidthInfoRemove(bwi);
        if (!m_parent.expired()) m_parent.lock()->sessionModified();
    }
    return *this;
}

const std::string &MediaDescriptionImpl::encryptionKey() const
{
    if (m_mediaDesc) return m_mediaDesc->encryptionKey();
    return empty_string;
}

MediaDescription &MediaDescriptionImpl::encryptionKey(const std::string &encryption_key)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->encryptionKey(encryption_key);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::encryptionKeyRemove()
{
    if (m_mediaDesc) {
        m_mediaDesc->encryptionKey(std::string());
        if (!m_parent.expired()) m_parent.lock()->sessionModified();
    }
    return *this;
}

const std::list<Attribute> &MediaDescriptionImpl::mediaAttributes() const
{
    if (m_mediaDesc) return m_mediaDesc->mediaAttributes();
    static const std::list<Attribute> empty_conn_infos;
    return empty_conn_infos;
}

MediaDescription &MediaDescriptionImpl::mediaAttributeAdd(const Attribute &attribute)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->mediaAttributeAdd(attribute);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::mediaAttributeAdd(Attribute &&attribute)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->mediaAttributeAdd(std::move(attribute));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

MediaDescription &MediaDescriptionImpl::mediaAttributeRemove(const Attribute &attribute)
{
    if (!m_mediaDesc) m_mediaDesc.reset(new MediaDesc);
    m_mediaDesc->mediaAttributeRemove(attribute);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(MediaDescriptionImpl) &media_description)
{
    os << static_cast<std::string>(media_description);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
