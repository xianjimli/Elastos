
#ifndef __COBJECTCONTAINER_H__
#define __COBJECTCONTAINER_H__

#include <elastos.h>
#include <container.h>

#include "_CObjectContainer.h"
#include "CObjectEnumerator.h"

CarClass(CObjectContainer), public SimpleContainer
{
public:
    CObjectContainer();

    virtual ~CObjectContainer();

    CARAPI Add(
        /* [in] */ IInterface* pObject);

    CARAPI Remove(
        /* [in] */ IInterface* pObject);

    CARAPI GetObjectEnumerator(
        /* [out] */ IObjectEnumerator** ppEnumerator);

    CARAPI GetObjectCount(
        /* [out] */ Int32* pCount);

    CARAPI Dispose();

private:
    Int32               mCount;
    Int32               mState;
};

#endif //__COBJECTCONTAINER_H__

