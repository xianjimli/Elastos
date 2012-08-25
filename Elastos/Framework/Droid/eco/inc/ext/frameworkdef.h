
#ifndef __FRAMEWORKDEF_H__
#define __FRAMEWORKDEF_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <elastos.h>
#include <Elastos.Core.h>
#include <elastos/etl_hash_fun.h>
#include <elastos/etl_function.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct EqualTo<String>
{
    Boolean operator()(const String & x,
                       const String & y) const
    {
        return x.Compare(y) == 0;
    }
};

template<> struct EqualTo<Int32>
{
    Boolean operator()(const Int32 x,
                       const Int32 y) const
    {
        return x == y;
    }
};

template<> struct Hash<Int64>
{
    size_t operator()(Int64 x) const { return x; }
};

template<> struct EqualTo<Int64>
{
    Boolean operator()(const Int64 x,
        const Int64 y) const
    {
        return x == y;
    }
};

_ELASTOS_NAMESPACE_END

#define MAX_PATH    256

#define XML_NAMESPACE   "http://schemas.elastos.com/capsule/res/elastos"

#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }
#define VALIDATE_STRING_NOT_NULL(x) if (x.IsNull()) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }

#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));
#define INTERFACE_ADDREF(i) if (i) { (i)->AddRef(); }
#define INTERFACE_RELEASE(i) if (i) { (i)->Release(); }

#define FAIL_RETURN(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return ec; \
    } while(0);

#define FAIL_RETURN_NULL(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return NULL; \
    } while(0);

#define DUMP_CLSID(intf) \
    do { \
        ClassID clsid; \
        IObject* obj = IObject::Probe(intf); \
        assert(obj != NULL); \
        obj->GetClassID(&clsid); \
        printf("======== DUMP_CLSID ========\n"); \
        printf("{%p, %p, %p, {%p, %p, %p, %p, %p, %p, %p, %p} }\n", \
                clsid.clsid.Data1, clsid.clsid.Data2, clsid.clsid.Data3, \
                clsid.clsid.Data4[0], clsid.clsid.Data4[1], \
                clsid.clsid.Data4[2], clsid.clsid.Data4[3], \
                clsid.clsid.Data4[4], clsid.clsid.Data4[5], \
                clsid.clsid.Data4[6], clsid.clsid.Data4[7]); \
        printf("============================\n"); \
    } while(0);

using namespace Elastos;

#endif //__FRAMEWORKDEF_H__
