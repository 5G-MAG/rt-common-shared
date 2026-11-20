/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: MimeTypeMap class
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

#include <filesystem>
#include <map>
#include <string>

#include "common.hh"

#include "MimeTypeMap.hh"

HTTPXPP_NAMESPACE_START

const std::shared_ptr<const MimeTypeMap> MimeTypeMap::defaultMap(new MimeTypeMap{
    {"txt", "text/plain"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"xhtml", "text/xml+html"},
    {"json", "application/json"},
    {"mpg", "video/mpeg"},
    {"mpeg", "video/mpeg"},
    {"mp3", "audio/mp3"},
});

MimeTypeMap::MimeTypeMap()
    :std::map<std::string, std::string>()
{
}

MimeTypeMap::MimeTypeMap(std::initializer_list<std::map<std::string, std::string>::value_type> init)
    :std::map<std::string, std::string>(init)
{
}

MimeTypeMap::MimeTypeMap(const MimeTypeMap &other)
    :std::map<std::string, std::string>(other)
{
}

MimeTypeMap::MimeTypeMap(MimeTypeMap &&other)
    :std::map<std::string, std::string>(std::move(other))
{
}

MimeTypeMap &MimeTypeMap::operator=(const MimeTypeMap &other)
{
    std::map<std::string, std::string>::operator=(other);
    return *this;
}

MimeTypeMap &MimeTypeMap::operator=(MimeTypeMap &&other)
{
    std::map<std::string, std::string>::operator=(std::move(other));
    return *this;
}

void MimeTypeMap::setMimeType(const std::string &ext, const std::string &mime_type)
{
    insert(std::make_pair(ext, mime_type));
}

static const std::string g_default_mime_type{"application/octet-stream"};

const std::string &MimeTypeMap::typeForFilename(const std::filesystem::path &filename) const
{
    auto ext = filename.extension().string();
    if (!ext.empty()) {
        ext.erase(0,1);
        return typeForExtension(ext);
    }
    return g_default_mime_type;
}

const std::string &MimeTypeMap::typeForExtension(const std::string &ext) const
{
    auto it = find(ext);
    if (it != end()) return it->second;
    return g_default_mime_type;
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
