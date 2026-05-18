/******************************************************************************
 * 5G-MAG Reference Tools: ConnectionInformation Library: ConnectionInformation class
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
#include "ConnectionInformationImpl.hh"

#include <rtsdp/ConnectionInformation.hh>

LIBRTSDP_NAMESPACE_START

std::shared_ptr<ConnectionInformation> ConnectionInformation::makeConnectionInformation()
{
    return std::static_pointer_cast<ConnectionInformation>(std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl));
}

std::shared_ptr<ConnectionInformation> ConnectionInformation::makeConnectionInformation(const std::string &hostname, int family, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
{
    return std::static_pointer_cast<ConnectionInformation>(std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(
                    hostname, family, ttl, consecutive_addresses)));
}

std::shared_ptr<ConnectionInformation> ConnectionInformation::makeConnectionInformation(const struct sockaddr_in &sin, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
{
    return std::static_pointer_cast<ConnectionInformation>(std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(
                    sin, ttl, consecutive_addresses)));
}

std::shared_ptr<ConnectionInformation> ConnectionInformation::makeConnectionInformation(const struct sockaddr_in6 &sin6, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
{
    return std::static_pointer_cast<ConnectionInformation>(std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(
                    sin6, ttl, consecutive_addresses)));
}

std::shared_ptr<ConnectionInformation> ConnectionInformation::makeConnectionInformation(const struct in_addr &ina, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
{
    return std::static_pointer_cast<ConnectionInformation>(std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(
                    ina, ttl, consecutive_addresses)));
}

std::shared_ptr<ConnectionInformation> ConnectionInformation::makeConnectionInformation(const struct in6_addr &in6a, const std::optional<uint8_t> &ttl, size_t consecutive_addresses)
{
    return std::static_pointer_cast<ConnectionInformation>(std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(
                    in6a, ttl, consecutive_addresses)));
}

std::shared_ptr<ConnectionInformation> ConnectionInformation::makeConnectionInformation(const std::shared_ptr<ConnectionInformation> &other)
{
    auto other_impl = std::dynamic_pointer_cast<ConnectionInformationImpl>(other);
    return std::static_pointer_cast<ConnectionInformation>(std::shared_ptr<ConnectionInformationImpl>(new ConnectionInformationImpl(        *other_impl)));
}

bool ConnectionInformation::operator==(const ConnectionInformation &other) const
{
    auto *impl = dynamic_cast<const ConnectionInformationImpl*>(this);
    auto *other_impl = dynamic_cast<const ConnectionInformationImpl*>(&other);
    return *impl == *other_impl;
}

std::size_t ConnectionInformation::hash() const
{
    auto *impl = dynamic_cast<const ConnectionInformationImpl*>(this);
    return impl->hash();
}

ConnectionInformation::operator std::string() const
{
    auto *impl = dynamic_cast<const ConnectionInformationImpl*>(this);
    return static_cast<std::string>(*impl);
}

//protected:

ConnectionInformation::ConnectionInformation()
{
}

ConnectionInformation::ConnectionInformation(const ConnectionInformation &other)
{
}

ConnectionInformation::ConnectionInformation(ConnectionInformation &&other)
{
}

ConnectionInformation &ConnectionInformation::operator=(const ConnectionInformation &other)
{
    return *this;
}

ConnectionInformation &ConnectionInformation::operator=(ConnectionInformation &&other)
{
    return *this;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(ConnectionInformation) &connection_info)
{
    os << static_cast<std::string>(connection_info);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
