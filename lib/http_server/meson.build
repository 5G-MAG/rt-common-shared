# License: 5G-MAG Public License (v1.0)
# Author(s): David Waring <david.waring2@bbc.co.uk>
# Copyright: (C) 2026 British Broadcasting Corporation
#
# Licensed under the License terms and conditions for use, reproduction, and
# distribution of 5G-MAG software (the “License”).  You may not use this file
# except in compliance with the License.  You may obtain a copy of the License
# at https://www.5g-mag.com/reference-tools.  Unless required by applicable
# law or agreed to in writing, software distributed under the License is
# distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.
#
# See the License for the specific language governing permissions and
# limitations under the License.

microhttpd_dep = dependency('libmicrohttpd', version: '>=1.0.0', required: true)
gnutls_dep = dependency('gnutls', version: '>=3.0.0', required: true)

libhttpxpp_sources = files('''
    common.hh
    CaseInsensitiveTraits.hh
    DirectoryIndexHandler.cc
    DirectoryIndexHandler.hh
    DocrootFile.cc
    DocrootFile.hh
    DocrootHTTPRequestHandler.cc
    DocrootHTTPRequestHandler.hh
    hash.hh
    HTTPRequest.cc
    HTTPRequest.hh
    HTTPRequestHandler.hh
    HTTPResponse.cc
    HTTPResponse.hh
    HTTPServer.cc
    HTTPServer.hh
    MimeTypeMap.cc
    MimeTypeMap.hh
    PathDelegatorHTTPRequestHandler.cc
    PathDelegatorHTTPRequestHandler.hh
    SockAddr.cc
    SockAddr.hh
'''.split())
libhttpxpp_version_major = '0'
libhttpxpp_version_minor = '1'
libhttpxpp_version_micro = '0'
libhttpxpp_version = libhttpxpp_version_major + '.' + libhttpxpp_version_minor + '.' + libhttpxpp_version_micro

version_conf = configuration_data()
version_conf.set_quoted('HTTPX_SERVER_VERSION', libhttpxpp_version)
version_conf.set('HTTPX_SERVER_MAJOR', libhttpxpp_version_major)
version_conf.set('HTTPX_SERVER_MINOR', libhttpxpp_version_minor)
version_conf.set('HTTPX_SERVER_MICRO', libhttpxpp_version_micro)

libhttpxpp_gen_sources = [configure_file(output : 'version.h', input: 'version.h.in', configuration : version_conf)]

libhttpxpp_libinc = include_directories('.')

libhttpxpp = static_library('httpx++',
    sources : libhttpxpp_sources + libhttpxpp_gen_sources,
    include_directories : [libhttpxpp_libinc],
    dependencies : [microhttpd_dep, gnutls_dep],
    install : false)

libhttpxpp_dep = declare_dependency(
    link_with : libhttpxpp,
    include_directories : [libhttpxpp_libinc],
    dependencies : [microhttpd_dep, gnutls_dep])
