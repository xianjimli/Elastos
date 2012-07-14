
#ifndef __CBROADCASTRECEIVER_H__
#define __CBROADCASTRECEIVER_H__

#include <elastos.h>
#include <Elastos.Framework.h>
#include "CBaseObject.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class CBroadcastReceiver : public CBaseObject, public IBroadcastReceiver
{
public:
    CBroadcastReceiver();

    virtual ~CBroadcastReceiver();

    virtual CARAPI Initialize();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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

    CARAPI SetOrderedHint(
        /* [in] */ Boolean isOrdered);

    CARAPI SetInitialStickyHint(
        /* [in] */ Boolean isInitialSticky);

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

#endif //__CBROADCASTRECEIVER_H__
