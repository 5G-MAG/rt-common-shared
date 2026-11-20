#ifndef _LIBRTSDP_PUB_REPEAT_FIELD_HH_
#define _LIBRTSDP_PUB_REPEAT_FIELD_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: RepeatField class
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
#include <list>
#include <string>

#include <rtsdp/common.hh>
#include "RepeatTime.hh"
#include "TimeZoneAdjustment.hh"

LIBRTSDP_NAMESPACE_START

class RepeatField {
public:
    RepeatField();
    RepeatField(const RepeatTime &repeat_time);
    RepeatField(const RepeatTime &repeat_time, std::initializer_list<TimeZoneAdjustment> init_zones);
    RepeatField(const RepeatField &other);
    RepeatField(RepeatField &&other);

    virtual ~RepeatField() {};

    RepeatField &operator=(const RepeatField &other);
    RepeatField &operator=(RepeatField &&other);

    bool operator==(const RepeatField &other) const;

    std::size_t hash() const;
    operator std::string() const;

private:
    RepeatTime m_repeatTime;
    std::list<TimeZoneAdjustment> m_timeZoneAdjustments;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(RepeatField), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(RepeatField) &repeat_field, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(repeat_field));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(RepeatField)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(RepeatField) &repeat_field) const { return repeat_field.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(RepeatField) &repeat_field);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_REPEAT_FIELD_HH_ */
