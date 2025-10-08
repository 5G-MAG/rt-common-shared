#ifndef _OPENAPI_PROBLEM_CAUSE_HH_
#define _OPENAPI_PROBLEM_CAUSE_HH_
/**************************************************************************
 * ProblemCause.hh : This holds a mapping of values for ProblemDetail.cause
 **************************************************************************
 * This is the base type for all model files generated from the OpenAPI
 * YAML.
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
#include <memory>
#include <string>

namespace fiveg_mag_reftools {

class ProblemCause : public std::enable_shared_from_this<ProblemCause> {
public:
    static const ProblemCause INVALID_API;
    static const ProblemCause INVALID_MSG_FORMAT;
    static const ProblemCause INVALID_QUERY_PARAM;
    static const ProblemCause MANDATORY_QUERY_PARAM_INCORRECT;
    static const ProblemCause OPTIONAL_QUERY_PARAM_INCORRECT;
    static const ProblemCause MANDATORY_QUERY_PARAM_MISSING;
    static const ProblemCause MANDATORY_IE_INCORRECT;
    static const ProblemCause OPTIONAL_IE_INCORRECT;
    static const ProblemCause MANDATORY_IE_MISSING;
    static const ProblemCause UNSPECIFIED_MSG_FAILURE;
    static const ProblemCause RESOURCE_CONTEXT_NOT_FOUND;
    static const ProblemCause CCA_VERIFICATION_FAILURE;
    static const ProblemCause TOKEN_CCA_MISMATCH;
    static const ProblemCause MODIFICATION_NOT_ALLOWED;
    static const ProblemCause SUBSCRIPTION_NOT_FOUND;
    static const ProblemCause RESOURCE_URI_STRUCTURE_NOT_FOUND;
    static const ProblemCause INCORRECT_LENGTH;
    static const ProblemCause NF_CONGESTION_RISK;
    static const ProblemCause NF_SERVICE_CONGESTION_RISK;
    static const ProblemCause INSUFFICIENT_RESOURCES;
    static const ProblemCause UNSPECIFIED_NF_FAILURE;
    static const ProblemCause SYSTEM_FAILURE;
    static const ProblemCause NF_FAILOVER;
    static const ProblemCause NF_SERVICE_FAILOVER;
    static const ProblemCause INBOUND_SERVER_ERROR;
    static const ProblemCause NF_CONGESTION;
    static const ProblemCause NF_SERVICE_CONGESTION;
    static const ProblemCause TARGET_NF_NOT_REACHABLE;
    static const ProblemCause TIMED_OUT_REQUEST;

    ProblemCause() = delete;
    ProblemCause(const ProblemCause &other);
    ProblemCause(ProblemCause &&other);
    
    virtual ~ProblemCause() {};

    ProblemCause &operator=(const ProblemCause &other);
    ProblemCause &operator=(ProblemCause &&other);

    bool operator==(const ProblemCause &other) const;
    bool operator!=(const ProblemCause &other) const;

    const std::string &cause() const;
    const std::string &reason() const;
    const std::string &description() const;
    int statusCode() const;
    long int causeEnum() const;

    static const ProblemCause &registerCause(const std::string &cause, int status_code, const std::string &status_reason, const std::string &description);

    std::shared_ptr<ProblemCause> make_shared();
    std::shared_ptr<const ProblemCause> make_shared() const;

private:
    ProblemCause(const std::string &cause, int status_code, const std::string &status_reason, const std::string &description);

    std::string m_cause;
    int         m_statusCode;
    std::string m_reason;
    std::string m_description;
};

}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif //_OPENAPI_PROBLEM_CAUSE_HH_
