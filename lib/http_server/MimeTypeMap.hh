#ifndef _HTTPXPP_MIME_TYPE_MAP_HH_
#define _HTTPXPP_MIME_TYPE_MAP_HH_
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

HTTPXPP_NAMESPACE_START

class MimeTypeMap : public std::map<std::string, std::string> {
public:
    static const std::shared_ptr<const MimeTypeMap> defaultMap;

    MimeTypeMap();
    MimeTypeMap(std::initializer_list<std::map<std::string, std::string>::value_type> init);
    MimeTypeMap(const MimeTypeMap &other);
    MimeTypeMap(MimeTypeMap &&other);

    virtual ~MimeTypeMap() {};

    MimeTypeMap &operator=(const MimeTypeMap &other);
    MimeTypeMap &operator=(MimeTypeMap &&other);

    void setMimeType(const std::string &ext, const std::string &mime_type);
    const std::string &typeForFilename(const std::filesystem::path &filename) const;
    const std::string &typeForExtension(const std::string &ext) const;
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_MIME_TYPE_MAP_HH_ */
