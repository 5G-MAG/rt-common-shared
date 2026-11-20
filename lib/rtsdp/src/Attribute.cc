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

#include <rtsdp/common.hh>

#include <rtsdp/Attribute.hh>

LIBRTSDP_NAMESPACE_START

Attribute::Attribute()
    :m_field()
    ,m_value()
{
}

Attribute::Attribute(const std::string &field, const std::optional<std::string> &value)
    :m_field(field)
    ,m_value(value)
{
}

Attribute::Attribute(const Attribute &other)
    :m_field(other.m_field)
    ,m_value(other.m_value)
{
}

Attribute::Attribute(Attribute &&other)
    :m_field(std::move(other.m_field))
    ,m_value(std::move(other.m_value))
{
}

Attribute &Attribute::operator=(const Attribute &other)
{
    m_field = other.m_field;
    m_value = other.m_value;
    return *this;
}

Attribute &Attribute::operator=(Attribute &&other)
{
    m_field = std::move(other.m_field);
    m_value = std::move(other.m_value);
    return *this;
}

bool Attribute::operator==(const Attribute &other) const
{
    return m_field == other.m_field && m_value == other.m_value;
}

std::size_t Attribute::hash() const
{
    std::hash<std::string> hs;
    std::hash<std::optional<std::string> > hos;
    return hs(m_field) + (hos(m_value) << 1);
}

Attribute::operator std::string() const
{
    if (m_field.empty()) return std::string();
    if (m_value) {
        return std::format("a={}:{}\r\n", m_field, m_value.value());
    }
    return std::format("a={}\r\n", m_field);
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(Attribute) &attribute)
{
    os << static_cast<std::string>(attribute);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
