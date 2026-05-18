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

#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/Originator.hh>
#include <rtsdp/ConnectionInformation.hh>
#include "SDPImpl.hh"
#include "version.h"

#include <rtsdp/SDP.hh>

LIBRTSDP_NAMESPACE_START

const std::string SDP::library_version(LIBRTSDP_VERSION);

std::shared_ptr<SDP> SDP::makeSDP()
{
    std::shared_ptr<SDPImpl> impl(new SDPImpl);
    return std::static_pointer_cast<SDP>(impl);
}

std::shared_ptr<SDP> SDP::makeSDP(const std::shared_ptr<Originator> &originator, const std::string &session_name,
                                  const std::shared_ptr<TimingInformation> &timing_info,
                                  const std::string &session_info, const std::string &description_uri,
                                  const std::string &email_address, const std::string &phone_number,
                                  const std::shared_ptr<ConnectionInformation> &connection_info,
                                  const std::string &encryption_key)
{
    std::shared_ptr<SDPImpl> impl(new SDPImpl(originator, session_name, timing_info, session_info, description_uri, email_address,
                                              phone_number, connection_info, encryption_key));
    return std::static_pointer_cast<SDP>(impl);
}

std::shared_ptr<SDP> SDP::makeSDP(const std::shared_ptr<SDP> &other)
{
    std::shared_ptr<SDPImpl> other_impl = std::dynamic_pointer_cast<SDPImpl>(other);
    std::shared_ptr<SDPImpl> impl;
    if (other_impl) {
        impl.reset(new SDPImpl(*other_impl));
    } else {
        impl.reset(new SDPImpl());
    }
    return std::static_pointer_cast<SDP>(impl);
}

bool SDP::operator==(const SDP &other) const
{
    auto *this_impl = dynamic_cast<const SDPImpl*>(this);
    if (!this_impl) return false;
    auto *other_impl = dynamic_cast<const SDPImpl*>(&other);
    if (!other_impl) return false;

    return *this_impl == *other_impl;
}

std::size_t SDP::hash() const
{
    auto *this_impl = dynamic_cast<const SDPImpl*>(this);
    return this_impl->hash();
}

SDP::operator std::string() const
{
    auto *this_impl = dynamic_cast<const SDPImpl*>(this);
    return static_cast<std::string>(*this_impl);
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(SDP) &sdp)
{
    os << static_cast<std::string>(sdp);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
