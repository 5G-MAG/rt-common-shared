/**************************************************************************
 * ProblemCause.cc : This holds a mapping of values for ProblemDetail.cause
 **************************************************************************
 * This holds information about ProblemDetail causes as given in TS 29.500
 * Table 5.2.7.2-1.
 **************************************************************************
 * License: 5G-MAG Public License (v1.0)
 * Author: David Waring
 * Copyright: (C)2025 British Broadcasting Corporation
 *
 * For full license terms please see the LICENSE file distributed with this
 * program. If this file is missing then the license can be retrieved from
 * https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view
 */
#include <cstdint>
#include <stdexcept>
#include <list>
#include <memory>
#include <string>

#include "ProblemCause.hh"

namespace fiveg_mag_reftools {

namespace {

static std::list<std::shared_ptr<ProblemCause> > &__registered_causes()
{
    static std::list<std::shared_ptr<ProblemCause> > _registered_causes;
    return _registered_causes;
}

} // end anonymous namespace

const ProblemCause ProblemCause::INVALID_API(ProblemCause::registerCause(
        "INVALID_API", 400, "Bad Request", "The HTTP request contains an unsupported API name or API version in the URI."));
const ProblemCause ProblemCause::INVALID_MSG_FORMAT(ProblemCause::registerCause(
        "INVALID_MSG_FORMAT", 400, "Bad Request", "The HTTP request has an invalid format."));
const ProblemCause ProblemCause::INVALID_QUERY_PARAM(ProblemCause::registerCause(
        "INVALID_QUERY_PARAM", 400, "Bad Request", "The HTTP request contains an unsupported query parameter in the URI. "));
const ProblemCause ProblemCause::MANDATORY_QUERY_PARAM_INCORRECT(ProblemCause::registerCause(
        "MANDATORY_QUERY_PARAM_INCORRECT", 400, "Bad Request", "A mandatory query parameter, or a conditional query parameter but mandatory required, for an HTTP method was received in the URI with semantically incorrect value."));
const ProblemCause ProblemCause::OPTIONAL_QUERY_PARAM_INCORRECT(ProblemCause::registerCause(
        "OPTIONAL_QUERY_PARAM_INCORRECT", 400, "Bad Request", "An optional query parameter for an HTTP method was received in the URI with a semantically incorrect value that prevents successful processing of the service request."));
const ProblemCause ProblemCause::MANDATORY_QUERY_PARAM_MISSING(ProblemCause::registerCause(
        "MANDATORY_QUERY_PARAM_MISSING", 400, "Bad Request", "Query parameter which is defined as mandatory, or as conditional but mandatory required, for an HTTP method is not included in the URI of the request."));
const ProblemCause ProblemCause::MANDATORY_IE_INCORRECT(ProblemCause::registerCause(
        "MANDATORY_IE_INCORRECT", 400, "Bad Request", "A mandatory IE (within the JSON body or within a variable part of an \"apiSpecificResourceUriPart\" or within an HTTP header), or conditional IE but mandatory required, for an HTTP method was received with a semantically incorrect value."));
const ProblemCause ProblemCause::OPTIONAL_IE_INCORRECT(ProblemCause::registerCause(
        "OPTIONAL_IE_INCORRECT", 400, "Bad Request", "An optional IE (within the JSON body or within an HTTP header) for an HTTP method was received with a semantically incorrect value that prevents successful processing of the service request."));
const ProblemCause ProblemCause::MANDATORY_IE_MISSING(ProblemCause::registerCause(
        "MANDATORY_IE_MISSING", 400, "Bad Request", "A mandatory IE (within the JSON body or within the variable part of an \"apiSpecificResourceUriPart\" or within an HTTP header), or conditional IE but mandatory required, for an HTTP method is not included in the request."));
const ProblemCause ProblemCause::UNSPECIFIED_MSG_FAILURE(ProblemCause::registerCause(
        "UNSPECIFIED_MSG_FAILURE", 400, "Bad Request", "The request is rejected due to unspecified client error."));
const ProblemCause ProblemCause::RESOURCE_CONTEXT_NOT_FOUND(ProblemCause::registerCause(
        "RESOURCE_CONTEXT_NOT_FOUND", 400, "Bad Request", "The notification request is rejected because the callback URI still exists in the receiver of the notification, but the specific resource context identified within the notification payloadis not found in the NF service consumer."));
const ProblemCause ProblemCause::CCA_VERIFICATION_FAILURE(ProblemCause::registerCause(
        "CCA_VERIFICATION_FAILURE", 403, "Forbidden", "The request is rejected due to a failure to verify the CCA at the receiving entity (e.g. NRF or NF service producer)."));
const ProblemCause ProblemCause::TOKEN_CCA_MISMATCH(ProblemCause::registerCause(
        "TOKEN_CCA_MISMATCH", 403, "Forbidden", "The request is rejected due to a mismatch between the subject claim in the access token and subject claim in the CCA."));
const ProblemCause ProblemCause::MODIFICATION_NOT_ALLOWED(ProblemCause::registerCause(
        "MODIFICATION_NOT_ALLOWED", 403, "Forbidden", "The request is rejected because the contained modification instructions attempt to modify IE which is not allowed to be modified."));
const ProblemCause ProblemCause::SUBSCRIPTION_NOT_FOUND(ProblemCause::registerCause(
        "SUBSCRIPTION_NOT_FOUND", 404, "Not Found", "The request for modification or deletion of subscription is rejected because the subscription is not found in the NF."));
const ProblemCause ProblemCause::RESOURCE_URI_STRUCTURE_NOT_FOUND(ProblemCause::registerCause(
        "RESOURCE_URI_STRUCTURE_NOT_FOUND", 404, "Not Found", "The request is rejected because a fixed part after the first variable part of an \"apiSpecificResourceUriPart\" (as defined in clause 4.4.1 of 3GPP TS 29.501) is not found in the NF.\n\nThis fixed part of the URI may represent a sub-resource collection (e.g. contexts, subscriptions, policies) or a custom operation."));
const ProblemCause ProblemCause::INCORRECT_LENGTH(ProblemCause::registerCause(
        "INCORRECT_LENGTH", 411, "Length Required", "The request is rejected due to incorrect value of a Content-length header field."));
const ProblemCause ProblemCause::NF_CONGESTION_RISK(ProblemCause::registerCause(
        "NF_CONGESTION_RISK", 429, "Too Many Requests", "The request is rejected due to excessive traffic which, if continued over time, may lead to (or may increase) an overload situation of the NF instance."));
const ProblemCause ProblemCause::NF_SERVICE_CONGESTION_RISK(ProblemCause::registerCause(
        "NF_SERVICE_CONGESTION_RISK", 429, "Too Many Requests", "The request is rejected due to excessive traffic which, if continued over time, may lead to (or may increase) an overload situation of the NF service instance."));
const ProblemCause ProblemCause::INSUFFICIENT_RESOURCES(ProblemCause::registerCause(
        "INSUFFICIENT_RESOURCES", 500, "Internal Server Error", "The request is rejected due to insufficient resources."));
const ProblemCause ProblemCause::UNSPECIFIED_NF_FAILURE(ProblemCause::registerCause(
        "UNSPECIFIED_NF_FAILURE", 500, "Internal Server Error", "The request is rejected due to unspecified reason at the NF."));
const ProblemCause ProblemCause::SYSTEM_FAILURE(ProblemCause::registerCause(
        "SYSTEM_FAILURE", 500, "Internal Server Error", "The request is rejected due to generic error condition in the NF."));
const ProblemCause ProblemCause::NF_FAILOVER(ProblemCause::registerCause(
        "NF_FAILOVER", 500, "Internal Server Error", "The request is rejected due to the unavailability of the NF, and the requester may trigger an immediate re-selection of an alternative NF based on this information."));
const ProblemCause ProblemCause::NF_SERVICE_FAILOVER(ProblemCause::registerCause(
        "NF_SERVICE_FAILOVER", 500, "Internal Server Error", "The request is rejected due to the unavailability of the NF service, and the requester may trigger an immediate re-selection of an alternative NF service based on this information."));
const ProblemCause ProblemCause::INBOUND_SERVER_ERROR(ProblemCause::registerCause(
        "INBOUND_SERVER_ERROR", 502, "Bad Gateway", "The request is rejected due to the receipt of an 5xx error from an inbound server that the NF Service Producer accessed while attempting to fulfil the request (see clause 6.4.2.1 of 3GPP TS 29.500)."));
const ProblemCause ProblemCause::NF_CONGESTION(ProblemCause::registerCause(
        "NF_CONGESTION", 503, "Service Unavailable", "The NF instance experiences congestion and performs overload control, which does not allow the request to be processed."));
const ProblemCause ProblemCause::NF_SERVICE_CONGESTION(ProblemCause::registerCause(
        "NF_SERVICE_CONGESTION", 503, "Service Unavailable", "The NF service instance experiences congestion and performs overload control, which does not allow the request to be processed."));
const ProblemCause ProblemCause::TARGET_NF_NOT_REACHABLE(ProblemCause::registerCause(
        "TARGET_NF_NOT_REACHABLE", 504, "Gateway Timeout", "The request is not served as the target NF is not reachable."));
const ProblemCause ProblemCause::TIMED_OUT_REQUEST(ProblemCause::registerCause(
        "TIMED_OUT_REQUEST", 504, "Gateway Timeout", "The request is rejected due a request that has timed out at the HTTP client (see clause 6.11.2 of 3GPP TS 29.500)."));

ProblemCause::ProblemCause(const ProblemCause &other)
    :m_cause(other.m_cause)
    ,m_statusCode(other.m_statusCode)
    ,m_reason(other.m_reason)
    ,m_description(other.m_description)
{
}

ProblemCause::ProblemCause(ProblemCause &&other)
    :m_cause(std::move(other.m_cause))
    ,m_statusCode(std::move(other.m_statusCode))
    ,m_reason(std::move(other.m_reason))
    ,m_description(std::move(other.m_description))
{
}
    
ProblemCause &ProblemCause::operator=(const ProblemCause &other)
{
    m_cause = other.m_cause;
    m_statusCode = other.m_statusCode;
    m_reason = other.m_reason;
    m_description = other.m_description;

    return *this;
}

ProblemCause &ProblemCause::operator=(ProblemCause &&other)
{
    m_cause = std::move(other.m_cause);
    m_statusCode = std::move(other.m_statusCode);
    m_reason = std::move(other.m_reason);
    m_description = std::move(other.m_description);

    return *this;
}

bool ProblemCause::operator==(const ProblemCause &other) const
{
    // If these are the same object then they are equal
    if (this == &other) return true;

    // Check member variables for equality
    if (m_statusCode != other.m_statusCode) return false;
    if (m_cause != other.m_cause) return false;
    if (m_reason != other.m_reason) return false;    

    return m_description == other.m_description;
}

bool ProblemCause::operator!=(const ProblemCause &other) const
{
    return !(*this == other);
}

const std::string &ProblemCause::cause() const
{
    return m_cause;
}

const std::string &ProblemCause::reason() const
{
    return m_reason;
}

const std::string &ProblemCause::description() const
{
    return m_description;
}

int ProblemCause::statusCode() const
{
    return m_statusCode;
}

long int ProblemCause::causeEnum() const
{
    return reinterpret_cast<long int>(this);
}

const ProblemCause &ProblemCause::registerCause(const std::string &cause, int status_code, const std::string &status_reason,
                                                const std::string &description)
{
    auto &reg_causes = __registered_causes();
    for (const auto &cause_ptr : reg_causes) {
        if (cause_ptr->m_cause == cause) throw std::invalid_argument(cause + " can only be registered as a ProblemCause once");
    }
    ProblemCause *problem_cause = new ProblemCause(cause, status_code, status_reason, description);
    reg_causes.emplace_back(problem_cause);
    return *problem_cause;
}

std::shared_ptr<ProblemCause> ProblemCause::make_shared()
{
    return shared_from_this();
}

std::shared_ptr<const ProblemCause> ProblemCause::make_shared() const
{
    return shared_from_this();
}

ProblemCause::ProblemCause(const std::string &cause, int status_code, const std::string &status_reason,
                           const std::string &description)
    :m_cause(cause)
    ,m_statusCode(status_code)
    ,m_reason(status_reason)
    ,m_description(description)
{
}

}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
