
#ifndef _Container_H_
#define _Container_H_

#include <elastos.h>
#include <container.h>

#include "_CParcelableObjectContainer.h"
#include "CObjectEnumerator.h"

CarClass(CParcelableObjectContainer), public SimpleContainer
{
public:
    CParcelableObjectContainer();

    virtual ~CParcelableObjectContainer();

    CARAPI Add(
        /* [in] */ IInterface* pObject);

    CARAPI Remove(
        /* [in] */ IInterface* pObject);

    CARAPI GetObjectEnumerator(
        /* [out] */ IObjectEnumerator** ppEnumerator);

    CARAPI GetObjectCount(
        /* [out] */ Int32* pCount);

    CARAPI Dispose();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    Int32               mCount;
    Int32               mState;
};

#endif

