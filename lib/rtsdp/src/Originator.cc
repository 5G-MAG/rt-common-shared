/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: Originator class
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
#include "OriginatorImpl.hh"

#include <rtsdp/Originator.hh>

LIBRTSDP_NAMESPACE_START

std::shared_ptr<Originator> Originator::makeOriginator()
{
    std::shared_ptr<OriginatorImpl> impl{new OriginatorImpl};
    return std::static_pointer_cast<Originator>(impl);
}

std::shared_ptr<Originator> Originator::makeOriginator(const std::string &hostname, int family, const std::string &user)
{
    std::shared_ptr<OriginatorImpl> impl{new OriginatorImpl(hostname, family, user)};
    return std::static_pointer_cast<Originator>(impl);
}

std::shared_ptr<Originator> Originator::makeOriginator(const struct sockaddr_in &sin, const std::string &user)
{
    std::shared_ptr<OriginatorImpl> impl{new OriginatorImpl(sin, user)};
    return std::static_pointer_cast<Originator>(impl);
}

std::shared_ptr<Originator> Originator::makeOriginator(const struct sockaddr_in6 &sin6, const std::string &user)
{
    std::shared_ptr<OriginatorImpl> impl{new OriginatorImpl(sin6, user)};
    return std::static_pointer_cast<Originator>(impl);
}

std::shared_ptr<Originator> Originator::makeOriginator(const struct in_addr &ina, const std::string &user)
{
    std::shared_ptr<OriginatorImpl> impl{new OriginatorImpl(ina, user)};
    return std::static_pointer_cast<Originator>(impl);
}

std::shared_ptr<Originator> Originator::makeOriginator(const struct in6_addr &in6a, const std::string &user)
{
    std::shared_ptr<OriginatorImpl> impl{new OriginatorImpl(in6a, user)};
    return std::static_pointer_cast<Originator>(impl);
}

std::shared_ptr<Originator> Originator::makeOriginator(const std::shared_ptr<Originator> &other)
{
    auto other_impl = std::dynamic_pointer_cast<OriginatorImpl>(other);
    std::shared_ptr<OriginatorImpl> impl{new OriginatorImpl(*other_impl)};
    return std::static_pointer_cast<Originator>(impl);
}

bool Originator::operator==(const Originator &other) const
{
    auto *other_impl = dynamic_cast<const OriginatorImpl*>(&other);
    auto *this_impl = dynamic_cast<const OriginatorImpl*>(this);
    if (this_impl && other_impl) return *this_impl == *other_impl;
    return false;
}

//protected:

Originator::Originator()
{
}

Originator::Originator(const Originator &other)
{
}

Originator::Originator(Originator &&other)
{
}

Originator &Originator::operator=(const Originator &other)
{
    return *this;
}

Originator &Originator::operator=(Originator &&other)
{
    return *this;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(Originator) &origin)
{
    os << static_cast<std::string>(origin);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
