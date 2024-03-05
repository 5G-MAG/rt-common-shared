# Data Reporting 5G_APIs overrides

The contents of this directory are modified versions of the OpenAPI YAML files
from the [5G_APIs](https://forge.3gpp.org/rep/all/5G_APIs) repository specific
to Data Reporting for use with a Data Collection Application Function.

This directory will be empty in the *main* branch of this repository.

Overrides for 3GPP Release 17 specifications will appear in the *3GPP-Rel17*
branch and overrides for 3GPP Release 18 specifications will appear in the
*3GPP-Rel18* branch.

This directory can be passed in the overrides path list to the
`open5gs-tools/scripts/generate-openapi` script. This will override files from
the [5G_APIs](https://forge.3gpp.org/rep/all/5G_APIs) repository with versions
from this directory after the 5G_APIs repository is cloned by the script.
