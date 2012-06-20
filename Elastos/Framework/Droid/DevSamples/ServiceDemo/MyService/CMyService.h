
#ifndef __CMYSERVICE_H__
#define __CMYSERVICE_H__

#include "CService.h"
#include "CMyServiceBinder.h"
#include "_CMyService.h"

using namespace Elastos;

class CMyService : public CService//, public IBinder
{
public:
//    CARAPI_(UInt32) AddRef();
//
//    CARAPI_(UInt32) Release();
//
//    CARAPI_(PInterface) Probe(
//        /* [in] */ REIID riid);
//
//    CARAPI GetInterfaceID(
//        /* [in] */ IInterface *pObject,
//        /* [out] */ InterfaceID *pIID);

    CARAPI OnCreate();

    CARAPI OnDestroy();

    CARAPI OnStart(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 startId);

    CARAPI OnStartCommand(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId,
        /* [out] */ Int32* result);

    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    CARAPI OnUnbind(
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* succeeded);

    CARAPI OnRebind(
        /* [in] */ IIntent* intent);
};

#endif //__CMYSERVICE_H__
