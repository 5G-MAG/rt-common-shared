/**************************************************************************
 * ModelObject.hh : ModelObject model base class/interface
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

#ifndef _OPENAPI_MODEL_OBJECT_HH_
#define _OPENAPI_MODEL_OBJECT_HH_

#include <string>
#include "CJson.hh"
#include "ModelObject.hh"
#include "ModelException.hh"
#include "OgsAllocator.hh"

namespace fiveg_mag_reftools {

class ModelObject {
public:
    ModelObject() {};
    ModelObject(const ModelObject &other) {};
    ModelObject(ModelObject &&other) {};

    virtual ~ModelObject() {};

    virtual CJson toJSON(bool as_request = false) const = 0;
    virtual void fromJSON(const CJson &json, bool as_request = false) = 0;
};

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _OPENAPI_MODEL_OBJECT_HH_ */
