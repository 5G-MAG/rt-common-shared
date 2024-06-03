/**************************************************************************
 * AnyType.hh : AnyType model base class/interface
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

#ifndef _OPENAPI_ANY_TYPE_HH_
#define _OPENAPI_ANY_TYPE_HH_

#include "CJson.hh"

namespace fiveg_mag_reftools {

class AnyType {
public:
    AnyType() : m_val(nullptr) {};
    AnyType(const AnyType &other) : m_val(nullptr) {
        if (other.m_val) m_val = new CJson(*other.m_val);
    };
    AnyType(AnyType &&other) : m_val(other.m_val) {
        other.m_val = nullptr;
    };
    AnyType(const CJson &json, bool as_request = false) : m_val(nullptr) {
        this->fromJSON(json, as_request);
    };

    virtual ~AnyType() { if (m_val) delete m_val; };

    virtual CJson toJSON(bool as_request = false) const {
        if (m_val) return *m_val;
        return CJson::Null;
    };

    virtual void fromJSON(const CJson &json, bool as_request = false) {
        if (m_val) {
            delete m_val;
        }
        m_val = new CJson(json);
    };

private:
    CJson *m_val;
};

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _OPENAPI_ANY_TYPE_HH_ */
