/**************************************************************************
 * Validator.hh : Validator model base class/interface
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

#ifndef _OPENAPI_VALIDATOR_HH_
#define _OPENAPI_VALIDATOR_HH_

//#include "core/ogs-core.h"
#define OGS_CORE_INSIDE
#define OGS_USE_TALLOC 1
#include "core/ogs-memory.h"
#undef OGS_CORE_INSIDE

#include <optional>
#include <regex>
#include <type_traits>

#include "Boundary.hh"
#include "CJson.hh"
#include "ModelException.hh"

namespace fiveg_mag_reftools {

template <class T>
class Validator {
public:
    typedef T value_type;

    Validator(const char *classname = nullptr, const char *fieldname = nullptr)
        :m_classname(classname)
        ,m_fieldname(fieldname)
        {};
    Validator(const Validator &other)
        :m_classname(other.m_classname)
        ,m_fieldname(other.m_fieldname)
        {};
    Validator(Validator &&other)
        :m_classname(other.m_classname)
        ,m_fieldname(other.m_fieldname)
        {};

    Validator &operator=(const Validator &other) {
        m_classname = other.m_classname;
        m_fieldname = other.m_fieldname;
        return *this;
    };
    Validator &operator=(Validator &&other) {
        m_classname = other.m_classname;
        m_fieldname = other.m_fieldname;
        return *this;
    };

    virtual ~Validator() {};

    virtual bool validate(const T& value) const = 0;

protected:
    const char *m_classname;
    const char *m_fieldname;
};

template <class T>
struct is_std_optional : std::false_type {};

template <class U>
struct is_std_optional<std::optional<U> > : std::true_type {};

template <class T>
class NumberValidator : public Validator<T> {
public:
    typedef Boundary<T> boundary_type;
    typedef typename Validator<T>::value_type value_type;

    NumberValidator(const char *classname = nullptr, const char *fieldname = nullptr, boundary_type *minimum = nullptr, boundary_type *maximum = nullptr)
        :Validator<T>(classname, fieldname)
        ,m_minimum(minimum)
        ,m_maximum(maximum)
    {};
    NumberValidator(const NumberValidator &other)
        :Validator<T>(other)
        ,m_minimum(nullptr)
        ,m_maximum(nullptr)
    {
        if (other.m_minimum) m_minimum = new boundary_type(*other.m_minimum);
        if (other.m_maximum) m_maximum = new boundary_type(*other.m_maximum);
    };
    NumberValidator(NumberValidator &&other)
        :Validator<T>(std::move(other))
        ,m_minimum(other.m_minimum)
        ,m_maximum(other.m_maximum)
    {
        other.m_minimum = nullptr;
        other.m_maximum = nullptr;
    };

    NumberValidator &operator=(const NumberValidator &other) {
        this->Validator<T>::operator=(other);

        if (m_minimum) {
            delete m_minimum;
            m_minimum = nullptr;
        }
        if (other.m_minimum) m_minimum = new boundary_type(*other.m_minimum);

        if (m_maximum) {
            delete m_maximum;
            m_maximum = nullptr;
        }
        if (other.m_maximum) m_maximum = new boundary_type(*other.m_maximum);

        return *this;
    };
    NumberValidator &operator=(NumberValidator &&other) {
        this->Validator<T>::operator=(std::move(other));
        m_minimum = other.m_minimum;
        m_maximum = other.m_maximum;
        other.m_minimum = nullptr;
        other.m_maximum = nullptr;
        return *this;
    };

    virtual ~NumberValidator() {
        if (m_minimum) delete m_minimum;
        if (m_maximum) delete m_maximum;
    };

    virtual bool validate(const value_type &value) const {
        return _validate(value);
    }

private:
    template <typename U, typename std::enable_if<is_std_optional<U>::value, bool>::type = true>
    bool _validate(const U &value) const {
        if (value.has_value()) {
            if (m_minimum && m_minimum->isGreaterThan(value.value())) {
                throw ModelException("Given value is less than the allowed minimum", this->m_classname, this->m_fieldname);
            }
            if (m_maximum && m_maximum->isLessThan(value.value())) {
                throw ModelException("Given value is greater than the allowed maximum", this->m_classname, this->m_fieldname);
            }
        }

        return true;
    };

    template <typename U, typename std::enable_if<!is_std_optional<U>::value, bool>::type = true>
    bool _validate(const U &value) const {
        if (m_minimum && m_minimum->isGreaterThan(value)) {
            throw ModelException("Given value is less than the allowed minimum", this->m_classname, this->m_fieldname);
        }
        if (m_maximum && m_maximum->isLessThan(value)) {
            throw ModelException("Given value is greater than the allowed maximum", this->m_classname, this->m_fieldname);
        }

        return true;
    };

    boundary_type *m_minimum;
    boundary_type *m_maximum;
};

class StringValidator : public Validator<std::string> {
public:
    StringValidator(const char *classname = nullptr, const char *fieldname = nullptr, const char *pattern = nullptr)
        :Validator<std::string>(classname, fieldname)
        ,m_pattern(pattern)
        ,m_regex(nullptr)
    {
        if (m_pattern) {
            std::string pattern(m_pattern);
            pattern = pattern.substr(1,pattern.size()-2);
            /* TODO: get regex from a pool */
            m_regex = new std::regex(pattern, std::regex_constants::ECMAScript);
        }
    }

    StringValidator(const StringValidator &other)
        :Validator<std::string>(other)
        ,m_pattern(other.m_pattern)
        ,m_regex(nullptr)
    {
        if (m_pattern) {
            std::string pattern(m_pattern);
            pattern = pattern.substr(1,pattern.size()-2);
            /* TODO: use pool regex from "other" */
            m_regex = new std::regex(pattern, std::regex_constants::ECMAScript);
        }
    };

    StringValidator(StringValidator &&other)
        :Validator<std::string>(std::move(other))
        ,m_pattern(other.m_pattern)
        ,m_regex(other.m_regex)
    {
        other.m_regex = nullptr;
    };

    StringValidator &operator=(const StringValidator &other)
    {
        this->Validator::operator=(other);
        m_pattern = other.m_pattern;
        if (m_regex) {
            delete m_regex;
            m_regex = nullptr;
        }
        if (m_pattern) {
            std::string pattern(m_pattern);
            pattern = pattern.substr(1,pattern.size()-2);
            /* TODO: use pool regex from "other" */
            m_regex = new std::regex(pattern, std::regex_constants::ECMAScript);
        }
        return *this;
    };
    StringValidator &operator=(StringValidator &&other)
    {
        this->Validator::operator=(std::move(other));
        m_pattern = other.m_pattern;
        if (m_regex) delete m_regex;
        m_regex = other.m_regex;
        other.m_regex = nullptr;
        return *this;
    };

    virtual ~StringValidator() {
        if (m_regex) delete m_regex;
    };

    virtual bool validate(const std::string &value) const {
        if (m_regex && !std::regex_match(value, *m_regex)) {
            throw ModelException("String did not match the correct format", m_classname, m_fieldname);
        }
        return true;
    };
    bool validate(const std::optional<std::string> &opt_val) const {
        if (!opt_val.has_value()) return true;
        return this->validate(opt_val.value());
    };

private:
    const char *m_pattern;
    std::regex *m_regex;
};

template<class T>
class ModelValidator : public Validator<std::shared_ptr<T> > {
public:
    typedef T model_type;
    ModelValidator(const char *classname = nullptr, const char *fieldname = nullptr)
        :Validator<std::shared_ptr<T> >(classname, fieldname)
    {};

    virtual ~ModelValidator() {};
    bool validate(const std::optional<std::shared_ptr<T> > &opt_ptr) const {
        if (!opt_ptr.has_value()) return true;
        if (!opt_ptr.value()) return true;
        return this->validate(*(opt_ptr.value()));
    };
    virtual bool validate(const std::shared_ptr<T> &ptr) const {
        if (!ptr) return false;
        return this->validate(*ptr);
    };
    bool validate(const std::optional<T> &opt_value) const {
        return (opt_value.has_value() && this->validate(opt_value.value()));
    };
    bool validate(const T &value) const {
        return value.validate();
    };
};

template <class T>
class NullValidator : public Validator<T> {
public:
    NullValidator(const char *classname = nullptr, const char *fieldname = nullptr)
        :Validator<T>(classname, fieldname)
    {};
    
    virtual ~NullValidator() {};
    virtual bool validate(const T &value) const { return true; };
    bool validate(const std::optional<T> &value) const { return true; };
};

template <class C, class V>
class ContainerValidator : public Validator<C> {
public:
    typedef C container_type;
    typedef V item_validator;
    typedef typename V::value_type item_type;

    ContainerValidator(const char *classname = nullptr, const char *fieldname = nullptr, V* item_validator = nullptr)
        :Validator<container_type>(classname, fieldname)
        ,m_itemValidator(item_validator)
    {};

    virtual ~ContainerValidator() {
        if (m_itemValidator) delete m_itemValidator;
    };

    virtual bool validate(const container_type &value) const {
        if (m_itemValidator) {
            for (auto &var : value) {
                m_itemValidator->validate(var);
            }
        }
        return true;
    };

    bool validate(const std::optional<container_type> &opt_value) const {
        if (!opt_value.has_value()) return true;
        return this->validate(opt_value.value());
    };

private:
    item_validator *m_itemValidator;
};

template <class V>
class MapValidator : public Validator<std::map<std::string, typename V::value_type, std::less<std::string>, OgsAllocator<std::pair<const std::string, typename V::value_type> > > > {
public:
    typedef std::map<std::string, typename V::value_type, std::less<std::string>, OgsAllocator<std::pair<const std::string, typename V::value_type> > > container_type;
    typedef V item_validator;
    typedef typename V::value_type item_type;

    MapValidator(const char *classname = nullptr, const char *fieldname = nullptr, V* item_validator = nullptr)
        :Validator<container_type>(classname, fieldname)
        ,m_itemValidator(item_validator)
    {};

    virtual ~MapValidator() {
        if (m_itemValidator) delete m_itemValidator;
    };

    virtual bool validate(const container_type &value) const {
        if (m_itemValidator) {
            for (auto &var : value) {
                m_itemValidator->validate(var.second);
            }
        }
        return true;
    };

    bool validate(const std::optional<container_type> &opt_value) const {
        if (!opt_value.has_value()) return true;
        return this->validate(opt_value.value());
    };

private:
    item_validator *m_itemValidator;
};

template <class V>
using ListValidator = ContainerValidator<std::list<typename V::value_type, OgsAllocator<typename V::value_type> >, V>;

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _OPENAPI_VALIDATOR_HH_ */
