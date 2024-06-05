/**************************************************************************
 * Boundary.hh : Boundary model base class/interface
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

#ifndef _OPENAPI_BOUNDARY_HH_
#define _OPENAPI_BOUNDARY_HH_

namespace fiveg_mag_reftools {

template <class T>
class Boundary {
public:
    typedef T value_type;

    Boundary(const T value = 0, bool include = true)
        :m_value(value)
        ,m_include(include)
    {};
    Boundary(const Boundary &other)
        :m_value(other.m_value)
        ,m_include(other.m_include)
    {};
    Boundary(Boundary &&other)
        :m_value(other.m_value)
        ,m_include(other.m_include)
    {};

    virtual ~Boundary() {};

    bool isLessThan(const T value) {
        if (m_include) {
            return (m_value <= value);
        }
        return (m_value < value);
    };

    bool isGreaterThan(const T value) {
        if (m_include) {
            return (m_value >= value);
        }
        return (m_value > value);
    };

private:
    value_type m_value;
    bool m_include;
};

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _OPENAPI_BOUNDARY_HH_ */
