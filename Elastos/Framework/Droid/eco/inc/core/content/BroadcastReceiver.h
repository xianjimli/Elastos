
#ifndef __BROADCASTRECEIVER_H__
#define __BROADCASTRECEIVER_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class BroadcastReceiver
    : public ElRefBase
    , public IObject
    , public IBroadcastReceiver
{
public:
    BroadcastReceiver();

    virtual ~BroadcastReceiver();

    virtual CARAPI Initialize();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI ReceiveBroadcast(
        /* [in] */ IContext *pContext,
        /* [in] */ IIntent *pIntent);

    CARAPI SetResultCode(
        /* [in] */ Int32 code);

    CARAPI GetResultCode(
        /* [out] */ Int32* code);

    CARAPI SetResultData(
        /* [in] */ const String& data);

    CARAPI GetResultData(
        /* [out] */ String* data);

    CARAPI SetResultExtras(
        /* [in] */ IBundle* extras);

    CARAPI GetResultExtras(
        /* [in] */ Boolean makeMap,
        /* [out] */ IBundle** extras);

    CARAPI SetResult(
        /* [in] */ Int32 code,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras);

    CARAPI GetAbortBroadcast(
        /* [out] */ Boolean* aborted);

    CARAPI AbortBroadcast();

    CARAPI ClearAbortBroadcast();

    CARAPI IsOrderedBroadcast(
        /* [out] */ Boolean* isOrdered);

    CARAPI IsInitialStickyBroadcast(
        /* [out] */ Boolean* isInitialSticky);

    CARAPI SetOrderedHint(
        /* [in] */ Boolean isOrdered);

    CARAPI SetInitialStickyHint(
        /* [in] */ Boolean isInitialSticky);

    CARAPI SetDebugUnregister(
        /* [in] */ Boolean debug);

    CARAPI GetDebugUnregister(
        /* [out] */ Boolean* debugUnregister);

protected:
    virtual CARAPI OnReceive(
        /* [in] */ IContext *pContext,
        /* [in] */ IIntent *pIntent);

private:
    CARAPI_(void) CheckSynchronousHint();

private:
    Int32 mResultCode;
    String mResultData;
    AutoPtr<IBundle> mResultExtras;
    Boolean mAbortBroadcast;
    Boolean mDebugUnregister;
    Boolean mOrderedHint;
    Boolean mInitialStickyHint;
};

#endif //__BROADCASTRECEIVER_H__
