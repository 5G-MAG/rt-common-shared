/**************************************************************************
 * AnyType.cc : AnyType model base class implementation
 **************************************************************************
 * This is a type that can carry any JSON representation.
 **************************************************************************
 * License: 5G-MAG Public License (v1.0)
 * Author: David Waring <david.waring2@bbc.co.uk>
 * Copyright: (C)2025 British Broadcasting Corporation
 *
 * For full license terms please see the LICENSE file distributed with this
 * program. If this file is missing then the license can be retrieved from
 * https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view
 */
#include "CJson.hh"
#include "ProblemCause.hh"

#include "AnyType.hh"

namespace fiveg_mag_reftools {

void AnyType::applyJSONPatch(const CJson &json)
{
    if (!json.isObject()) throw ModelException(std::string("Runtime Error: JSON Patch not recognised: ") + json.serialise(),  "AnyType", std::string(), ProblemCause::INVALID_MSG_FORMAT);

    auto op_json = json.getObjectItemCaseSensitive("op");
    if (op_json.isNull()) throw ModelException(std::string("Runtime Error: JSON Patch not recognised: ") + json.serialise(),  "AnyType", "op", ProblemCause::INVALID_MSG_FORMAT);
    std::string op(op_json.stringValue());

    auto path_json = json.getObjectItemCaseSensitive("path");
    if (path_json.isNull()) throw ModelException(std::string("Runtime Error: JSON Patch not recognised: ") + json.serialise(),  "AnyType", "path", ProblemCause::INVALID_MSG_FORMAT);
    std::string path(path_json.stringValue());

    if (path.empty()) {
        if (op == "add" || op == "replace") {
            auto value_json = json.getObjectItemCaseSensitive("value");
            if (value_json.isNull()) throw ModelException(std::string("Runtime Error: JSON Patch not recognised: ") + json.serialise(),  "AnyType", "value", ProblemCause::INVALID_MSG_FORMAT);
            fromJSON(value_json);
        } else {
            throw ModelException("Runtime Error: JSON Patch operation not implemented on this node", "AnyType", std::string(), ProblemCause::SYSTEM_FAILURE);
        }
    } else {
        throw ModelException("Runtime Error: Attempt to reference beyond leaf node in JSON Patch path", "AnyType", "path", ProblemCause::INVALID_MSG_FORMAT);
    }
}

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
