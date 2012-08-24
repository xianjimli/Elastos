
#ifndef __CMYSERVICEBINDER_H__
#define __CMYSERVICEBINDER_H__

#include "_CMyServiceBinder.h"

using namespace Elastos;

CarClass(CMyServiceBinder)
{
public:
    CARAPI Transact(
        /* [in] */ Int32 code,
        /* [in] */ IParcelable* data,
        /* [in] */ Int32 flags,
        /* [out] */ IParcelable** reply,
        /* [out] */ Boolean* succeeded)
    {
        return E_NOT_IMPLEMENTED;
    }

    CARAPI GetDescription(
        /* [out] */ String* description)
    {
        return E_NOT_IMPLEMENTED;
    }
};

#endif //__CMYSERVICEBINDER_H__
