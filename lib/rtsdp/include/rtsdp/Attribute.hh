#ifndef _LIBRTSDP_PUB_ATTRIBUTE_HH_
#define _LIBRTSDP_PUB_ATTRIBUTE_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: Attribute class
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

#include "common.hh"

LIBRTSDP_NAMESPACE_START

class Attribute {
public:
    Attribute();
    Attribute(const std::string &field, const std::optional<std::string> &value = std::nullopt);
    Attribute(const Attribute &other);
    Attribute(Attribute &&other);

    virtual ~Attribute() {};

    Attribute &operator=(const Attribute &other);
    Attribute &operator=(Attribute &&other);

    bool operator==(const Attribute &other) const;

    std::size_t hash() const;
    operator std::string() const;

    const std::string &field() const { return m_field; };
    Attribute &field(const std::string &field_name) { m_field = field_name; return *this; };
    Attribute &field(std::string &&field_name) { m_field = std::move(field_name); return *this; };

    const std::optional<std::string> &value() const { return m_value; };
    const std::string &value_or(const std::string &default_value) const { return m_value?m_value.value():default_value; };
    Attribute &value(const std::nullopt_t &) { m_value.reset(); return *this; };
    Attribute &value(const std::string &val) { m_value = val; return *this; };
    Attribute &value(std::string &&val) { m_value = std::move(val); return *this; };
    Attribute &value(const std::optional<std::string> &val) { m_value = val; return *this; };
    Attribute &value(std::optional<std::string> &&val) { m_value = std::move(val); return *this; };

private:
    std::string m_field;
    std::optional<std::string> m_value;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(Attribute), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(Attribute) &attribute, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(attribute));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(Attribute)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(Attribute) &attribute) const { return attribute.hash(); };
};

}

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(Attribute) &attribute);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_ATTRIBUTE_HH_ */
