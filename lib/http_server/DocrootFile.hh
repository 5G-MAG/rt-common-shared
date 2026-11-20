#ifndef _HTTPXPP_DOCROOT_FILE_HH_
#define _HTTPXPP_DOCROOT_FILE_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: DocrootFile class
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
#include <vector>

#include "common.hh"
#include "MimeTypeMap.hh"

HTTPXPP_NAMESPACE_START

class DocrootFile {
public:
    DocrootFile() = delete;
    DocrootFile(const std::filesystem::path &filename, const std::shared_ptr<const MimeTypeMap> &mime_type_map = MimeTypeMap::defaultMap);
    DocrootFile(const DocrootFile &other) = delete;
    DocrootFile(DocrootFile &&other) = delete;

    virtual ~DocrootFile() {};

    DocrootFile &operator=(const DocrootFile &other) = delete;
    DocrootFile &operator=(DocrootFile &&other) = delete;

    const std::vector<char> &body() const { return m_body; };
    const std::map<std::string, std::string> &headers() const { return m_headers; };

    bool isModified(const std::optional<std::string> &if_none_match = std::nullopt, const std::optional<std::string> &if_modified_since= std::nullopt) const;

private:
    std::vector<char> m_body;
    std::map<std::string, std::string> m_headers;
    std::shared_ptr<const MimeTypeMap> m_mimeTypeMap;
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_DOCROOT_FILE_HH_ */
