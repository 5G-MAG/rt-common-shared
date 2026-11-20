#ifndef _LIBRTSDP_PUB_TIMING_INFORMATION_HH_
#define _LIBRTSDP_PUB_TIMING_INFORMATION_HH_
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

#include "common.hh"
#include "RepeatField.hh"

LIBRTSDP_NAMESPACE_START

class TimingInformation {
public:
    using timestamp_type = std::chrono::system_clock::time_point;

    static const timestamp_type NO_TIMESTAMP;

    static std::shared_ptr<TimingInformation> makeTimingInformation(const timestamp_type &start = NO_TIMESTAMP, const timestamp_type &end = NO_TIMESTAMP);
    static std::shared_ptr<TimingInformation> makeTimingInformation(const TimingInformation &other);

    virtual ~TimingInformation() {};

    bool operator==(const TimingInformation &other) const;

    virtual std::size_t hash() const = 0;
    virtual operator std::string() const = 0;
    operator bool() const { return isValid(); };

    virtual bool isValid() const = 0;

    virtual timestamp_type startTime() const = 0;
    virtual TimingInformation &startTime(const timestamp_type &start_time) = 0;
    virtual TimingInformation &startTime(timestamp_type &&start_time) = 0;

    virtual timestamp_type endTime() const = 0;
    virtual TimingInformation &endTime(const timestamp_type &end_time) = 0;
    virtual TimingInformation &endTime(timestamp_type &&end_time) = 0;

    virtual const std::list<RepeatField> &repeatFields() const = 0;
    virtual TimingInformation &repeatFields(const std::list<RepeatField> &repeat_fields) = 0;
    virtual TimingInformation &repeatFieldAdd(const RepeatField &repeat_field) = 0;
    virtual TimingInformation &repeatFieldAdd(RepeatField &&repeat_field) = 0;
    virtual TimingInformation &repeatFieldRemove(const RepeatField &repeat_field) = 0;

protected:
    TimingInformation();
    TimingInformation(const TimingInformation &other);
    TimingInformation(TimingInformation &&other);

    TimingInformation &operator=(const TimingInformation &other);
    TimingInformation &operator=(TimingInformation &&other);
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(TimingInformation), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(TimingInformation) &timing_info, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(timing_info));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(TimingInformation)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(TimingInformation) &timing_info) const { return timing_info.hash(); };
};

}

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimingInformation) &timing_info);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_TIMING_INFORMATION_HH_ */
