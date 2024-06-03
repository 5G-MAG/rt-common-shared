/**************************************************************************
 * ModelException.hh : ModelException model base class/interface
 **************************************************************************
 * This is the base type for all model files generated from the OpenAPI
 * YAML.
 **************************************************************************
 * License: 5G-MAG Public License (v1.0)
 * Author: David Waring
 * Copyright: (C)2024 British Broadcasting Corporation
 *
 * For full license terms please see the LICENSE file distributed with this
 * program. If this file is missing then the license can be retrieved from
 * https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view
 */

#ifndef _OPENAPI_MODEL_EXCEPTION_HH_
#define _OPENAPI_MODEL_EXCEPTION_HH_

#include <stdexcept>
#include <string>

namespace fiveg_mag_reftools {

class ModelException : public std::runtime_error {
public:
    ModelException(const std::string &ex_reason, const std::string &ex_classname, const std::string &ex_parameter = std::string())
        :std::runtime_error(ex_reason)
        ,classname(ex_classname)
        ,parameter(ex_parameter)
        {};
    ModelException(const ModelException &other) noexcept
        :std::runtime_error(other)
        ,classname(other.classname)
        ,parameter(other.parameter)
        {};

    virtual ~ModelException() {};

    std::string classname;
    std::string parameter;
private:
    ModelException() = delete;
};

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _OPENAPI_MODEL_EXCEPTION_HH_ */
