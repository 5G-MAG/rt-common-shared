/**************************************************************************
 * OgsAllocator.hh : OgsAllocator class
 **************************************************************************
 * This is a std::allocator which uses the Open5GS memmory allocation
 * functions.
 **************************************************************************
 * License: 5G-MAG Public License (v1.0)
 * Author: David Waring
 * Copyright: (C)2024 British Broadcasting Corporation
 *
 * For full license terms please see the LICENSE file distributed with this
 * program. If this file is missing then the license can be retrieved from
 * https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view
 */

#ifndef _OPENAPI_OGS_ALLOCATOR_HH_
#define _OPENAPI_OGS_ALLOCATOR_HH_

//#include "core/ogs-core.h"
#define OGS_CORE_INSIDE
#define OGS_USE_TALLOC 1
#include "core/ogs-memory.h"
#undef OGS_CORE_INSIDE

#include <memory>
#include <limits>

namespace fiveg_mag_reftools {

template <class T>
class OgsAllocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template <class U>
    struct rebind {
        typedef OgsAllocator<U> other;
    };

    OgsAllocator() noexcept {};

    OgsAllocator(const OgsAllocator &other) noexcept {};

    template <class U>
    OgsAllocator(const OgsAllocator<U> &other) noexcept {};

    virtual ~OgsAllocator() noexcept {};

    pointer address(reference value) const {
        return &value;
    };

    const_pointer address(const_reference value) const {
        return &value;
    };

    size_type max_size () const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    };

    pointer allocate(size_type n, const void *hint = nullptr)
    {
        return (pointer)ogs_malloc(n * sizeof(T));
    };

    void deallocate(pointer p, size_type n)
    {
        ogs_free(p);
    };

    void construct (pointer p, const_reference value) {
        new((void*)p)T(value);
    };

    void destroy (pointer p) {
        p->~T();
    };

    template <class U>
    bool operator==(const OgsAllocator<U> &) const noexcept { return true; };

    template <class U>
    bool operator!=(const OgsAllocator<U> &) const noexcept { return false; };
};

} /* end namespace */

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif /* _OPENAPI_OGS_ALLOCATOR_HH_ */
