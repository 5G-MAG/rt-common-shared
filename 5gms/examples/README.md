# 5G-MAG Reference Tools 5GMS Examples

This directory contains examples of 5GMS API outputs and configuration files that are shared between parts of the 5G-MAG Reference Tools 5GMS software suite.

## `Certificates.json`

This file contains a mapping from certificate ID to certificate filename. The
certificate IDs in this file are used to find the matching certificate file
(containing a public certificate, private key and any intermediate CA
certificates) when referenced from a ContentHostingConfiguration file.

The `5gms/scripts/make_self_signed_certs.py` script can be used, passing a
ContentHostingConfiguration and this `Certificates.json` file as parameters, to
create suitable self-signed certificate files for testing purposes.

## `ContentHostingConfiguration_Big-Buck-Bunny_pull-ingest.json`

This file is used as the configuration for both the rt-5gms-application-server
(MVP#1 and MVP#2) and the rt-5gms-application-function (MVP#2).

It contains a ContentHostingConfiguration, based on 3GPP TS 26.512 Release 17,
which points to a media origin host, suitable for use with pull-ingest, which
holds the Big Buck Bunny short animated film.

The distribution side of the configurations tells the rt-5gms-application-server
to present an M4d interface, which will use a URL path prefix of
`/m4d/provisioning-session-<identifier>/`, on its localhost (127.0.0.1)
loopback interface.

When the rt-5gms-application-server (for MVP#1 or MVP#2) is started with this
configuration, the media should then be accessible, via the M4d interface, using
{http://localhost/m4d/provisioning-session-d54a1fcc-d411-4e32-807b-2c60dbaeaf5f/BigBuckBunny_4s_onDemand_2014_05_09.mpd}
as the media entry URL.

## `ContentHostingConfiguration_Big-Buck-Bunny_pull-ingest_http_and_https.json`

This file is an alternative to
`ContentHostingConfiguration_Big-Buck-Bunny_pull-ingest.json` (see above) and
can be used along with the `Certificates.json` file to run a
rt-5gms-application-server instance which will provide both HTTP and HTTPS
distribution points.

## `ContentHostingConfiguration_Big-Buck-Bunny_pull-ingest_https.json`

This file is an alternative to
`ContentHostingConfiguration_Big-Buck-Bunny_pull-ingest.json` (see above) and
can be used along with the `Certificates.json` file to run a 
rt-5gms-application-server instance which will provide a HTTPS distribution
point instead of an HTTP distribution point.

## `ServiceAccessInformation_Big-Buck-Bunny_pull-ingest.json`

This file is used to provide the ServiceAccessInformation for the 5GMS-Aware
application (MVP#1). This file is based on the content hosting provisioned as
described in `ContentHostingConfiguration_Big-Buck-Bunny_pull-ingest.json`.
