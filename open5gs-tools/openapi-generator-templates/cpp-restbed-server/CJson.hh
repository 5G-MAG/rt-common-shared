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

#ifndef _OPENAPI_C_JSON_HH_
#define _OPENAPI_C_JSON_HH_

#include <cmath>
#include <iterator>
#include <memory>

#include "sbi/openapi/external/cJSON.h"
#include "ModelException.hh"

namespace fiveg_mag_reftools {

class ModelObject;

class CJson {
public:
    static const CJson Null;

    template <class T>
    class Iterator {
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;
        typedef std::forward_iterator_tag iterator_type;

        Iterator(T *node, std::size_t index = 0) : m_node(node), m_index(index) {};

        bool operator==(const Iterator<T> &other) const { return other.m_node == m_node && other.m_index == m_index; };
        bool operator!=(const Iterator<T> &other) const { return other.m_node != m_node || other.m_index != m_index; };
        bool operator<(const Iterator<T> &other) const { return other.m_node == m_node && m_index < other.m_index; };
        bool operator>(const Iterator<T> &other) const { return other.m_node == m_node && m_index > other.m_index; };

        value_type operator*() const
        {
            return m_node->index(m_index);
        };

        Iterator<T> &operator++() {
            if (m_index < m_node->arraySize()) m_index++;
            return *this;
        };

        Iterator<T> operator+(std::size_t n) {
            std::size_t new_index = m_index + n;
            if (new_index > m_node->arraySize()) new_index = m_node->arraySize();
            return Iterator<T>(m_node, new_index);
        };
    private:
        T *m_node;
        std::size_t m_index;
    };
    typedef Iterator<CJson> iterator;
    typedef Iterator<const CJson> const_iterator;

    CJson(cJSON *c_json, bool owner = true) : m_owner(owner), m_node(c_json) {};
    CJson(const CJson &other) : m_owner(true), m_node(nullptr) {if (other.m_node) m_node = cJSON_Duplicate(other.m_node, true);};
    CJson(CJson &&other) : m_owner(other.m_owner), m_node(other.m_node) {other.m_node = nullptr;};

    static CJson newObject() {return CJson(cJSON_CreateObject()); };
    static CJson newArray() {return CJson(cJSON_CreateArray()); };
    static CJson newString(const std::string &str) {return CJson(cJSON_CreateString(str.c_str())); };
    static CJson newString(const char *value) {return CJson(cJSON_CreateString(value)); };
    static CJson newNumber(double value) {return CJson(cJSON_CreateNumber(value)); };
    static CJson newBool(bool value) {return CJson(cJSON_CreateBool(value?1:0)); };
    static CJson newNull() { return Null; };

    static CJson parse(const std::string &json_string) {
        cJSON *json = cJSON_Parse(json_string.c_str());
        if (!json) {
            throw ModelException("Unable to parse JSON", "CJson");
        }
        return CJson(json);
    }

    static CJson wrap(int val, bool as_request = false) { return CJson(cJSON_CreateNumber(val)); };
    static CJson wrap(long int val, bool as_request = false) { return CJson(cJSON_CreateNumber(val)); };
    static CJson wrap(long long int val, bool as_request = false) { return CJson(cJSON_CreateNumber(val)); };
    static CJson wrap(float val, bool as_request = false) { return CJson(cJSON_CreateNumber(val)); };
    static CJson wrap(double val, bool as_request = false) { return CJson(cJSON_CreateNumber(val)); };
    static CJson wrap(const std::string &val, bool as_request = false) { return CJson(cJSON_CreateString(val.c_str())); };
    static CJson wrap(const char *val, bool as_request = false) { return CJson(cJSON_CreateString(val)); };
    static CJson wrap(bool val, bool as_request = false) { return CJson(cJSON_CreateBool(val)); };
    static CJson wrap(const ModelObject &obj, bool as_request = false);
    static CJson wrap(const ModelObject *ptr, bool as_request = false);
    template <class T>
    static CJson wrap(const std::shared_ptr<T> &ptr, bool as_request = false) {
        return wrap(dynamic_cast<const ModelObject&>(*ptr), as_request);
    };

    virtual ~CJson() {if (m_owner && m_node) cJSON_Delete(m_node);};

    CJson &operator=(const CJson &other) {
        if (m_node == other.m_node) return *this;
        if (m_owner && m_node) cJSON_Delete(m_node);
        m_node = cJSON_Duplicate(other.m_node, 1);
        m_owner = true;
    }

    CJson &operator=(CJson &&other) {
        if (m_node == other.m_node) {
            m_owner = m_owner || other.m_owner;
            other.m_owner = false;
            return *this;
        }
        if (m_owner && m_node) cJSON_Delete(m_node);
        m_node = other.m_node;
        m_owner = other.m_owner;
        other.m_owner = false;
        return *this;
    }

    iterator begin() {return iterator(this, 0);};
    const_iterator begin() const {return const_iterator(this, 0);};
    const_iterator cbegin() const {return const_iterator(this, 0);};

    iterator end() {return iterator(this, arraySize());};
    const_iterator end() const {return const_iterator(this, arraySize());};
    const_iterator cend() const {return const_iterator(this, arraySize());};

    bool isArray() const {return m_node?cJSON_IsArray(m_node):false;};
    bool isObject() const {return m_node?cJSON_IsObject(m_node):false;};
    bool isString() const {return m_node?cJSON_IsString(m_node):false;};
    bool isNumber() const {return m_node?cJSON_IsNumber(m_node):false;};
    bool isBool() const {return m_node?cJSON_IsBool(m_node):false;};
    bool isNull() const {return m_node == nullptr || cJSON_IsNull(m_node);};

    std::size_t arraySize() const {return (isArray() || isObject())?cJSON_GetArraySize(m_node):0;};
    CJson index(std::size_t idx) const { return (idx < arraySize())?CJson(cJSON_GetArrayItem(m_node, idx), false):Null; };

    CJson &set(const std::string &key, const CJson &node) {
        return set(key, std::move(CJson(node)));
    };
    CJson &set(const std::string &key, CJson &&node) {
        if (!isObject()) throw ModelException("Attempt to set object parameter on non-object", "CJson");
        if (node.m_node) {
            cJSON *cjson = node.m_node;
            if (!node.m_owner) {
                cjson = cJSON_Duplicate(cjson, 1);
            } else {
                node.m_owner = false;
            }
            cJSON_AddItemToObject(m_node, key.c_str(), cjson);
        } else {
            cJSON_AddItemToObject(m_node, key.c_str(), cJSON_CreateNull());
        }
        return *this;
    };

    CJson &append(const CJson &node) {
        return append(std::move(CJson(node)));
    };
    CJson &append(CJson &&node) {
        if (!isArray()) throw ModelException("Attempt to append to non-array object", "CJson");
        if (node.m_node) {
            cJSON *cjson = node.m_node;
            if (!node.m_owner) {
                cjson = cJSON_Duplicate(cjson, 1);
            } else {
                node.m_owner = false;
            }
            cJSON_AddItemToArray(m_node, cjson);
        } else {
            cJSON_AddItemToArray(m_node, cJSON_CreateNull());
        }
        return *this;
    };
    CJson &append(const std::string &str) {
        return append(std::move(std::string(str)));
    };
    CJson &append(std::string &&str) {
        if (!isArray()) throw ModelException("Attempt to append to non-array object", "CJson");
        cJSON_AddItemToArray(m_node, cJSON_CreateString(str.c_str()));
        return *this;
    };
        

    const char *stringValue() const { return isString()?cJSON_GetStringValue(m_node):nullptr; };

    double numberValue() const { return isNumber()?cJSON_GetNumberValue(m_node):NAN; };

    bool boolValue() const { return isBool()?cJSON_IsTrue(m_node):false; };

    operator int() const { return static_cast<int>(numberValue()); };
    operator long int() const { return static_cast<long int>(numberValue()); };
    operator long long int() const { return static_cast<long long int>(numberValue()); };
    operator float() const { return static_cast<float>(numberValue()); };
    operator double() const { return numberValue(); };
    operator std::string() const { return std::string(stringValue()); };
    operator const char*() const { return stringValue(); };
    operator bool() const { return boolValue(); };

    CJson getObjectItemCaseSensitive(const std::string &key) const {
        if (isObject()) {
            return CJson(cJSON_GetObjectItemCaseSensitive(m_node, key.c_str()), false);
        }
        return Null;
    };

    cJSON *exportCJSON() {
        m_owner = false;
        return m_node;
    };

private:
    CJson();

    bool m_owner;
    cJSON *m_node;
};

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _OPENAPI_C_JSON_HH_ */
