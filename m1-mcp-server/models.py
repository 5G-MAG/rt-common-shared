"""
Request bodies of the Maf_Provisioning API of TS 26.510 V19.2.0 as Pydantic models. FastMCP derives the input schema of a tool from them,
so the tools carry the schemas of the Release 19 resources (property names, required properties, enumerations, limits).
Properties that the Media AF assigns (read-only: identifiers, canonicalDomainName, state, stateReason) are not part of a request and are
not modelled. Edge resources, content publishing, RTC and supplementary (eMBMS/MBS) distribution are outside this server.
"""

from typing import Literal, Optional

from pydantic import BaseModel, ConfigDict, Field


class _Body(BaseModel):
    model_config = ConfigDict(extra="forbid")


class Snssai(_Body):
    """Single Network Slice Selection Assistance Information (TS 29.571)."""
    sst: int = Field(ge=0, le=255, description="Slice/Service Type")
    sd: Optional[str] = Field(None, pattern=r"^[A-Fa-f0-9]{6}$", description="Slice Differentiator, six hexadecimal digits")


# ---- Content Hosting Configuration (clause 8.8.3.1, table 8.8.3.1-1) ----

class IngestConfiguration(_Body):
    """Parameters for ingesting media content into the Media AS at reference point M2 or M10."""
    mode: Literal["PULL", "PUSH"] = Field(description="ContentTransferMode: the Media AS pulls the content (PULL) or it is pushed to it (PUSH)")
    protocol: str = Field(description=("Content ingest protocol term of TS 26.512 table 8.1-1: urn:3gpp:5gms:content-protocol:http-pull, "
                                       "http://dashif.org/ingest/v1.2/interface-1, http://dashif.org/ingest/v1.2/interface-2 or "
                                       "urn:3gpp:5gms:content-protocol:http-ll-pull"))
    baseURL: Optional[str] = Field(None, description="PULL: the origin base URL, nominated by the provider. PUSH: assigned by the Media AF, omit it")


class RelativeMediaEntryPoint(_Body):
    relativePath: str = Field(description="Path of the Media Entry Point document, relative, without a leading slash")
    contentType: str = Field(description="MIME content type of the Media Entry Point, e.g. application/dash+xml")
    profiles: Optional[list[str]] = Field(None, min_length=1, description="Conformance profile URIs")


class PathRewriteRule(_Body):
    requestPathPattern: str = Field(description="Regular expression matched against the request path")
    mappedPath: str = Field(description="Replacement for the matching portion of the path")


class CachingDirectives(_Body):
    statusCodeFilters: Optional[list[int]] = Field(None, min_length=1, description="Origin status codes the directives apply to")
    noCache: Optional[bool] = Field(None, description="Mark the resources as not to be cached (default false)")
    maxAge: Optional[int] = Field(None, ge=0, description="Caching time-to-live in seconds")


class CachingConfiguration(_Body):
    urlPatternFilter: str = Field(description="Regular expression matching the URLs at M2 or M10")
    cachingDirectives: CachingDirectives


class GeoFencing(_Body):
    locatorType: str = Field(description="Term identifier URI of the locator type (TS 26.510 annex B.1)")
    locators: list[str] = Field(min_length=1, description="Locators from which access is allowed")


class UrlSignature(_Body):
    urlPattern: str
    tokenName: str
    passphraseName: str
    passphrase: str = Field(min_length=6, max_length=50)
    tokenExpiryName: str
    useIPAddress: bool
    ipAddressName: Optional[str] = None


class DistributionConfiguration(_Body):
    """A content distribution configuration. baseURL and canonicalDomainName are assigned by the Media AF for a PULL distribution."""
    distributionId: str = Field(description="Label, unique within the Content Hosting Configuration, nominated by the provider")
    mode: Optional[Literal["PULL", "PUSH"]] = Field(None, description="ContentTransferMode of the distribution, PULL when omitted")
    affinityGroup: Optional[str] = None
    contentPreparationTemplateId: Optional[str] = Field(None, description="Id of a Content Preparation Template of the session")
    certificateId: Optional[str] = Field(None, description="Id of a Server Certificate of the session")
    domainNameAlias: Optional[str] = Field(None, description="Alternative FQDN, only when creating; needs certificateId with a matching alias")
    baseURL: Optional[str] = Field(None, description="Only for a PUSH distribution: nominated by the provider")
    entryPoint: Optional[RelativeMediaEntryPoint] = Field(None, description="Omit for a PUSH distribution")
    pathRewriteRules: Optional[list[PathRewriteRule]] = Field(None, min_length=1)
    cachingConfigurations: Optional[list[CachingConfiguration]] = Field(None, min_length=1)
    geoFencing: Optional[GeoFencing] = None
    urlSignature: Optional[UrlSignature] = None


class ContentHostingConfiguration(_Body):
    name: str = Field(description="A name for this Content Hosting Configuration")
    ingestConfiguration: IngestConfiguration
    distributionConfigurations: list[DistributionConfiguration] = Field(min_length=1)


# ---- Consumption Reporting Configuration (table 8.12.3.1-1) ----

class ConsumptionReportingConfiguration(_Body):
    reportingInterval: Optional[int] = Field(None, gt=0, description="Seconds between consumption reports; a single final report when omitted")
    samplePercentage: Optional[float] = Field(None, ge=0.0, le=100.0, description="Proportion of clients that report (default 100.0)")
    accessReporting: Optional[bool] = Field(None, description="Report access network changes (default false)")


# ---- Metrics Reporting Configuration (table 8.11.3-1) ----

class MetricsReportingConfiguration(_Body):
    samplingPeriod: int = Field(gt=0, description="Seconds between the samplings of the QoE metrics")
    scheme: Optional[str] = Field(None, description="QoE metrics scheme URI, e.g. urn:3GPP:ns:PSS:DASH:QM10")
    metrics: Optional[list[str]] = Field(None, min_length=1, description="Metric term URIs; the default set of the scheme when omitted")
    reportingInterval: Optional[int] = Field(None, gt=0)
    samplePercentage: Optional[float] = Field(None, ge=0.0, le=100.0)
    reportingStartOffset: Optional[int] = Field(None, ge=0, description="Seconds from the start of the media delivery session")
    reportingDuration: Optional[int] = Field(None, ge=0, description="Seconds after the start offset; 0 is a single report")
    urlFilters: Optional[list[str]] = Field(None, min_length=1)
    dataNetworkName: Optional[str] = None
    sliceScope: Optional[list[Snssai]] = Field(None, min_length=1)
    positiveCrossingThresholds: Optional[dict[str, list[float]]] = Field(None, description="Metric URI to threshold values (report when exceeded)")
    negativeCrossingThresholds: Optional[dict[str, list[float]]] = Field(None, description="Metric URI to threshold values (report when below)")
    locationFilter: Optional[list[dict]] = Field(None, min_length=1, description="LocationArea5G objects (TS 29.122)")


# ---- Policy Template (table 8.7.3.1-1) ----

class UnidirectionalQosSpecification(_Body):
    maximumAuthorisedBitRate: Optional[str] = Field(None, description='BitRate string, e.g. "20 Mbps"')
    minimumPacketLossRate: Optional[int] = Field(None, ge=0, description="Tenths of a percent")
    pduSetQosLimits: Optional[dict] = Field(None, description="PDUSetQosPara of TS 29.571")


class PolicyConstraints(_Body):
    """One of qosReference, downlinkQoSSpecification and uplinkQoSSpecification is required."""
    componentReference: str = Field(description="Identifier of this QoS specification, unique in the array")
    qosReference: Optional[str] = None
    downlinkQoSSpecification: Optional[UnidirectionalQosSpecification] = None
    uplinkQoSSpecification: Optional[UnidirectionalQosSpecification] = None
    pduSetMarking: Optional[bool] = None
    downlinkDataBurstSizeMarkingRequired: Optional[bool] = None
    downlinkTimeToNextBurstMarkingRequired: Optional[bool] = None
    downlinkExpeditedTransferIndicationMarkingRequired: Optional[bool] = None


class ApplicationSessionContext(_Body):
    """At least one of sliceInfo and dnn."""
    sliceInfo: Optional[Snssai] = None
    dnn: Optional[str] = None


class ChargingSpecification(_Body):
    sponsorId: Optional[str] = None
    sponsoringStatus: Optional[Literal["SPONSOR_ENABLED", "SPONSOR_DISABLED"]] = None
    permittedUes: Optional[list[str]] = Field(None, min_length=1, description="GPSIs of the UEs that may instantiate the template")


class PolicyTemplate(_Body):
    """At least one of qoSSpecifications, chargingSpecification, bdtPolicyId and bdtSpecification is required."""
    externalReference: str = Field(description="Identifier, unique in the Provisioning Session, e.g. HD_Premium")
    applicationSessionContexts: Optional[list[ApplicationSessionContext]] = Field(None, min_length=1)
    qoSSpecifications: Optional[list[PolicyConstraints]] = Field(None, min_length=1)
    chargingSpecification: Optional[ChargingSpecification] = None
    bdtPolicyId: Optional[str] = Field(None, description="Exclusive with bdtSpecification")
    bdtSpecification: Optional[dict] = Field(None, description="BdtPolicySchedule (table 8.7.3.2-1): windows (at least one), startDate, endDate")
    l4SEnablementPreference: Optional[bool] = None
    qoSMonitoringConfiguration: Optional[dict] = Field(None, description="QosMonitoringInformation of TS 29.122")


# ---- Event Data Processing Configuration (table 8.13.3-1; DataAccessProfile of TS 26.532 clause 6.3.2.3) ----

class DataAccessProfile(_Body):
    dataAccessProfileId: str = Field(description="Unique within the configuration")
    targetEventConsumerTypes: list[str] = Field(description="Event consumers the profile is for, e.g. NWDAF, NEF")
    parameters: list[str] = Field(description="Data parameters exposed to the consumers")
    timeAccessRestrictions: Optional[dict] = Field(None, description="duration (s) and aggregationFunctions")
    userAccessRestrictions: Optional[dict] = Field(None, description="groupIds, userIds and aggregationFunctions")
    locationAccessRestrictions: Optional[dict] = Field(None, description="locationAreas and aggregationFunctions")


class EventDataProcessingConfiguration(_Body):
    eventId: str = Field(description="AfEvent of TS 29.517 clause 5.6.3.3 relating to media delivery, e.g. MS_QOE_METRICS, MS_CONSUMPTION")
    dataAccessProfiles: list[DataAccessProfile] = Field(min_length=1)
    authorizationUrl: Optional[str] = Field(None, description="Absolute URL for authorising the event consumer")


def body(model: BaseModel) -> dict:
    """The model as the JSON object to send: only the properties that were set"""
    return model.model_dump(mode="json", exclude_none=True)
