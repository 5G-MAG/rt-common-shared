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

#include <rtsdp/common.hh>
#include <rtsdp/Attribute.hh>
#include <rtsdp/ConnectionInformation.hh>
#include "MediaDescriptionImpl.hh"

#include <rtsdp/MediaDescription.hh>

LIBRTSDP_NAMESPACE_START

std::shared_ptr<MediaDescription> MediaDescription::makeMediaDescription()
{
    auto impl = std::shared_ptr<MediaDescriptionImpl>(new MediaDescriptionImpl);
    return std::static_pointer_cast<MediaDescription>(impl);
}

std::shared_ptr<MediaDescription> MediaDescription::makeMediaDescription(const std::string &media_type, uint16_t port, const std::string &proto, const std::string &fmt, uint16_t number_of_ports)
{
    auto impl = std::shared_ptr<MediaDescriptionImpl>(new MediaDescriptionImpl(media_type, port, proto, fmt, number_of_ports));
    return std::static_pointer_cast<MediaDescription>(impl);
}

bool MediaDescription::operator==(const MediaDescription &other) const
{
    auto *other_impl = dynamic_cast<const MediaDescriptionImpl*>(&other);
    if (!other_impl) return false;
    auto *this_impl = dynamic_cast<const MediaDescriptionImpl*>(this);
    if (!this_impl) return false;
    return *this_impl == *other_impl;
}

//protected:
MediaDescription::MediaDescription()
{
}

MediaDescription::MediaDescription(const MediaDescription &other)
{
}

MediaDescription::MediaDescription(MediaDescription &&other)
{
}

MediaDescription &MediaDescription::operator=(const MediaDescription &other)
{
    return *this;
}

MediaDescription &MediaDescription::operator=(MediaDescription &&other)
{
    return *this;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(MediaDescription) &media_description)
{
    os << static_cast<std::string>(media_description);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
