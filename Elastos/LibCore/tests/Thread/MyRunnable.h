
#ifndef __MyRunnable_H__
#define __MyRunnable_H__

#include <Elastos.Core.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class MyRunnable
    : public ElRefBase
    , public IRunnable
{
public:
    MyRunnable();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Run();

public:
    Boolean mStop;

private:
    CARAPI_(Mutex*) GetSelfLock();

private:
    Mutex mLock;
};

#endif //__MyRunnable_H__
