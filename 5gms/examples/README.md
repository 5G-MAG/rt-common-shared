# 5G-MAG Reference Tools 5GMS Examples

This directory contains examples of 5GMS API outputs and configuration files that are shared between parts of the 5G-MAG Reference Tools 5GMS software suite.

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

## `ServiceAccessInformation_Big-Buck-Bunny_pull-ingest.json`

This file is used to provide the ServiceAccessInformation for the 5GMS-Aware
application (MVP#1). This file is based on the content hosting provisioned as
described in `ContentHostingConfiguration_Big-Buck-Bunny_pull-ingest.json`.
