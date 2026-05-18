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

librtsdp_sources = files('''
    Address.cc
    Address.hh
    Attribute.cc
    BandwidthInfo.cc
    BandwidthInfo.hh
    ConnectionInfo.cc
    ConnectionInfo.hh
    ConnectionInformation.cc
    ConnectionInformationImpl.cc
    ConnectionInformationImpl.hh
    EmailAddress.cc
    EmailAddress.hh
    MediaDesc.cc
    MediaDesc.hh
    MediaDescription.cc
    MediaDescriptionImpl.cc
    MediaDescriptionImpl.hh
    MediaName.cc
    MediaName.hh
    Origin.cc
    Origin.hh
    Originator.cc
    OriginatorImpl.cc
    OriginatorImpl.hh
    PhoneNumber.cc
    PhoneNumber.hh
    RepeatField.cc
    RepeatTime.cc
    SDP.cc
    SDPImpl.cc
    SDPImpl.hh
    SessionDescriptionProtocol.cc
    SessionDescriptionProtocol.hh
    TimeActive.cc
    TimeActive.hh
    TimeZoneAdjustment.cc
    TimingInfo.cc
    TimingInfo.hh
    TimingInformation.cc
    TimingInformationImpl.cc
    TimingInformationImpl.hh
'''.split())
librtsdp_version_major = '0'
librtsdp_version_minor = '1'
librtsdp_version_micro = '0'
librtsdp_version = librtsdp_version_major + '.' + librtsdp_version_minor + '.' + librtsdp_version_micro

version_conf = configuration_data()
version_conf.set_quoted('LIBRTSDP_VERSION', librtsdp_version)
version_conf.set('LIBRTSDP_MAJOR', librtsdp_version_major)
version_conf.set('LIBRTSDP_MINOR', librtsdp_version_minor)
version_conf.set('LIBRTSDP_MICRO', librtsdp_version_micro)

librtsdp_gen_sources = [configure_file(output : 'version.h', input: 'version.h.in', configuration : version_conf)]

librtsdp_priv_libinc = include_directories('.')

librtsdp = library('rtsdp',
    sources : librtsdp_sources + librtsdp_gen_sources,
    include_directories : [librtsdp_inc, librtsdp_priv_libinc],
    version : librtsdp_version,
    install : true)

librtsdp_dep = declare_dependency(
    link_with : librtsdp,
    include_directories : [librtsdp_inc])
