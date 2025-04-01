/**************************************************************************
 * OgsAllocator.cc : OgsAllocator global overrides
 **************************************************************************
 * This provides overrides for the global new and delete functions so that
 * Open5GS memory allocation functions are used.
 **************************************************************************
 * License: 5G-MAG Public License (v1.0)
 * Author: David Waring
 * Copyright: (C)2024 British Broadcasting Corporation
 *
 * For full license terms please see the LICENSE file distributed with this
 * program. If this file is missing then the license can be retrieved from
 * https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view
 */

//#include "core/ogs-core.h"
#define OGS_CORE_INSIDE
#define OGS_USE_TALLOC 1
#include "core/ogs-memory.h"
#undef OGS_CORE_INSIDE

void *operator new[](size_t size)
{
    return ogs_malloc(size);
}

void *operator new(size_t size)
{
    return ogs_malloc(size);
}

void operator delete(void *ptr)
{
    ogs_free(ptr);
}

void operator delete(void *ptr, unsigned long __size)
{
    ogs_free(ptr);
}

void operator delete[](void *ptr)
{
    ogs_free(ptr);
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
