/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: TimingInformation class
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

#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/RepeatField.hh>
#include "TimeActive.hh"
#include "TimingInfo.hh"
#include "TimingInformationImpl.hh"

#include <rtsdp/TimingInformation.hh>

LIBRTSDP_NAMESPACE_START

const TimingInformation::timestamp_type TimingInformation::NO_TIMESTAMP = TimeActive::SDP_EPOCH;

std::shared_ptr<TimingInformation> TimingInformation::makeTimingInformation(const timestamp_type &start, const timestamp_type &end)
{
    auto impl = std::shared_ptr<TimingInformationImpl>(new TimingInformationImpl(start, end));
    return std::static_pointer_cast<TimingInformation>(impl);
}

std::shared_ptr<TimingInformation> TimingInformation::makeTimingInformation(const TimingInformation &other)
{
    const auto &other_impl = dynamic_cast<const TimingInformationImpl&>(other);
    std::shared_ptr<TimingInfo> new_timing_info(new TimingInfo(*other_impl.timingInfo()));
    auto impl = std::shared_ptr<TimingInformationImpl>(new TimingInformationImpl(new_timing_info));
    return std::static_pointer_cast<TimingInformation>(impl);
}

bool TimingInformation::operator==(const TimingInformation &other) const
{
    const auto &other_impl = dynamic_cast<const TimingInformationImpl&>(other);
    auto *this_impl = dynamic_cast<const TimingInformationImpl*>(this);
    return *this_impl == other_impl;
}

//protected:
TimingInformation::TimingInformation()
{
}

TimingInformation::TimingInformation(const TimingInformation &other)
{
}

TimingInformation::TimingInformation(TimingInformation &&other)
{
}

TimingInformation &TimingInformation::operator=(const TimingInformation &other)
{
    return *this;
}

TimingInformation &TimingInformation::operator=(TimingInformation &&other)
{
    return *this;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimingInformation) &timing_info)
{
    os << static_cast<std::string>(timing_info);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
