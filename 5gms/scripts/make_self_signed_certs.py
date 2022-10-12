#!/usr/bin/python3
#
# 5G-MAG Reference Tools: 5GMS Self-signed Certificate Creator
# ============================================================
#
# File: make_self_signed_certs.py
# License: 5G-MAG Public License (v1.0)
# Author: David Waring
# Copyright: (C) 2022 British Broadcasting Corporation
#
# For full license terms please see the LICENSE file distributed with this
# program. If this file is missing then the license can be retrieved from
# https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view
#
# This is the 5G-MAG Reference Tools 5GMS utility to create self-signed
# certificates with private keys from a ContentHostingConfiguration and a 
# Certificates JSON file.
#
# This utility cross references the used certificate Ids from the
# ContentHostingConfiguration with the filename that will be used for the
# certificate in the Certificates file. It uses the canonicalDomainName from
# the ContentHostingConfiguration as the common name for the certificate and
# lists the canonicalDomainName and domainNameAlias as the subjectAltNames.
#
'''5G-MAG Reference Tools: Self-signed certificate creator

This script will take a content hosting configuration JSON file and a
certificate JSON file as input and will generate a self-signed certificate for
each file given.

Syntax: make_self_signed_certs.py <CHC-JSON-file> <Certificates-JSON-file>
'''

import json
import os.path
import subprocess
import sys

class ConfigError(RuntimeError):
    pass

def get_chc_certificate_id_map(chc):
    found_cert_ids = {}
    for dc in chc['distributionConfigurations']:
        if 'canonicalDomainName' not in dc:
            raise ConfigError('ContentHostingConfiguration.distributionConfigurations must include a canonicalDomainName')
        hostnames = {dc['canonicalDomainName']}
        if 'domainNameAlias' in dc and len(dc['domainNameAlias']) > 0:
            hostnames.add(dc['domainNameAlias'])
        if 'certificateId' in dc:
            if dc['certificateId'] not in found_cert_ids:
                found_cert_ids[dc['certificateId']] = {'hostnames':set(), 'canonicalname': dc['canonicalDomainName']}
            found_cert_ids[dc['certificateId']]['hostnames'].update(hostnames)
    return found_cert_ids

def join_paths(base, relpath):
    if relpath[0] == os.path.sep:
        return relpath
    if base[-1] != os.path.sep:
        base = os.path.dirname(base)
    return os.path.realpath(os.path.join(base, relpath))

def read_chc(chc_file):
    with open(chc_file) as chc_in:
        chc = json.load(chc_in)

    if not isinstance(chc, dict):
        raise ConfigError('Expected an object in ContentHostingConfiguration file')

    if 'distributionConfigurations' not in chc:
        raise ConfigError('ContentHostingConfiguration must have distributionConfigurations for certificate association.')

    found_cert_ids = get_chc_certificate_id_map(chc)

    if len(found_cert_ids) < 1:
        raise ConfigError('At least one ContentHostingConfiguration.distributionConfigurations must reference a certificate.')

    return chc

def read_certs(certs_file):
    with open(certs_file) as certs_in:
        certs = json.load(certs_in)
    
    if not isinstance(certs, dict):
        raise ConfigError('Certificates JSON file must be a JSON object')

    for cert_id, filename in certs.items():
        if not isinstance(cert_id,str):
            raise ConfigError('Certificate IDs in the Certificates JSON file must be strings.')
        if not isinstance(filename,str):
            raise ConfigError('Filenames in Certificates JSON file must be strings.')
    certs = {cert_id: join_paths(certs_file, filename) for cert_id, filename in certs.items()}

    return certs

def main():
    if len(sys.argv) != 3:
        sys.stderr.write('Syntax: %s <CHC-JSON-file> <Certificates-JSON-file>\n'%os.path.basename(sys.argv[0]))
        return 1

    chc_file = sys.argv[1]
    certs_file = sys.argv[2]

    try:
        chc = read_chc(chc_file)
        certs = read_certs(certs_file)
    except ConfigError as e:
        sys.stderr.write(str(e) + '\n')
        return 1

    cert_id_map = get_chc_certificate_id_map(chc)

    # Check we have all the certificate configuration first
    for cert_id, hosts in cert_id_map.items():
        if cert_id not in certs:
            raise ConfigError('Certificate ID %s in ContentHostingConfiguration but does not appear in the certificates map'%cert_id)

    # Generate the certificates
    for cert_id, hosts in cert_id_map.items():
        cert_filename = certs[cert_id]
        cmd = ['openssl','req','-new','-newkey','rsa:2048','-keyout',cert_filename,'-out',cert_filename,'-x509','-batch','-outform','PEM','-noenc','-set_serial','1','-subj','/CN='+hosts['canonicalname'],'-days','30']
        for h in hosts['hostnames']:
            cmd += ['-addext','subjectAltName=DNS:'+h]
        print('Running: '+' '.join(cmd))
        subprocess.run(cmd)

    return 0

if __name__ == '__main__':
    sys.exit(main())
