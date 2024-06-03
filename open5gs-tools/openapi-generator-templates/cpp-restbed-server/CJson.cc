/**************************************************************************
 * CJson.hh : CJson class interface to cJSON C library
 **************************************************************************
 * This provides a C++ interface to the cJSON functions.
 **************************************************************************
 * License: 5G-MAG Public License (v1.0)
 * Author: David Waring
 * Copyright: (C)2024 British Broadcasting Corporation
 *
 * For full license terms please see the LICENSE file distributed with this
 * program. If this file is missing then the license can be retrieved from
 * https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view
 */

#include <cmath>
#include <iterator>

#include "cJSON.h"
#include "ModelException.hh"
#include "ModelObject.hh"
#include "CJson.hh"

namespace fiveg_mag_reftools {

const CJson CJson::Null = CJson(nullptr);

CJson CJson::wrap(const ModelObject &obj, bool as_request)
{
    return obj.toJSON(as_request);
}

CJson CJson::wrap(const ModelObject *ptr, bool as_request)
{
    return ptr->toJSON(as_request);
}

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
