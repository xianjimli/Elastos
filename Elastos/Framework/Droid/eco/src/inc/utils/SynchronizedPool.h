
#ifndef __SYNCHRONIZEDPOOL_H__
#define __SYNCHRONIZEDPOOL_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core::Threading;

class SynchronizedPool : public ElRefBase, public IPool
{
public:
    SynchronizedPool(
        /* [in] */ IPool* pool);

    SynchronizedPool(
        /* [in] */ IPool* pool,
        /* [in] */ Mutex& lock);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Acquire(
        /* [out] */ IPoolable** element);

    CARAPI ReleaseElement(
        /* [in] */ IPoolable* element);

private:
    AutoPtr<IPool> mPool;
    Mutex mLock;
    Mutex& mLockRef;
};

#endif //__SYNCHRONIZEDPOOL_H__