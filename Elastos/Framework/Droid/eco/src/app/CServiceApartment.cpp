
#include "app/CServiceApartment.h"
#include "app/CServiceResolver.h"

#include <new>
#include "app/marshal.h"
#include "utils/log.h"

EXTERN_C int invoke(void* pThis, void* pFunc, void* pParam, int nSize);

CServiceApartment::CServiceApartment()
{
    CApartment::New((IApartment**)&mApartment);
    mApartment->Start(ApartmentAttr_New);
}

CServiceApartment::~CServiceApartment()
{}

ECode CServiceApartment::ScheduleInvokeService(
    /* [in] */ IInterface* service,
    /* [in] */ Handle32 data,
    /* [in] */ IParcel* params)
{
    ECode ec = NOERROR;
    PVoid pCoalesceFunc = NULL;
    IParcel *pParams = NULL;
    PCallbackEvent pCallbackEvent = NULL;
    IInterface* pCallbackContext = NULL;
    ECode (STDCALL CServiceApartment::*pHandlerFunc)(
        IInterface* service, Handle32 data, IParcel* params);

    pHandlerFunc = &CServiceApartment::PerformInvokeService;

    EventHandler delegate(EventHandler::Make((PVoid)this, *(PVoid*)&pHandlerFunc, CallbackType_CPP));

    CCallbackParcel::New(&pParams);
    pParams->WriteInterfacePtr(service);
    pParams->WriteInt32((Int32)data);
    pParams->WriteInterfacePtr(params);

    pCallbackEvent = _Impl_CallbackSink_AllocCallbackEvent(sizeof(_EzCallbackEvent));
    pCallbackEvent = new(pCallbackEvent) _EzCallbackEvent(
            0,
            CallbackEventFlag_DirectCall | CallbackEventFlag_SyncCall | CallbackPriority_Normal,
            NULL,
            delegate.m_pCarObjClient,
            pCoalesceFunc,
            delegate.GetThisPtr(), delegate.GetFuncPtr(),
            delegate.GetFuncType(), pParams);

    mApartment->GetCallbackContext(&pCallbackContext);
    ec = _Impl_CallbackSink_SendCallbackEvent(pCallbackContext, pCallbackEvent, INFINITE);

    pCallbackContext->Release();
    pParams->Release();

    return ec;     
}

ECode CServiceApartment::PerformInvokeService(
    /* [in] */ IInterface* svr,
    /* [in] */ Handle32 data,
    /* [in] */ IParcel* params)
{
    ECode ec;
    MarshalHeader* header = (MarshalHeader*)data;
    CServiceResolver* service = (CServiceResolver*)svr;
    CServiceResolver::CInterfaceStub* curInterface;
    const CIMethodInfo* methodInfo;
    UInt32 methodIndex;
    void* methodAddr;
   
    if (header->m_hMethodIndex >= 4) {
        curInterface = &(service->mInterfaceStubs[header->m_hInterfaceIndex]);

        methodIndex = header->m_hMethodIndex - 4;
        if (methodIndex >= curInterface->mInfo->methodNumMinus4) {
            LOGE("Stub: method index out of range - %d:%d\n",
                    methodIndex,
                    curInterface->mInfo->methodNumMinus4);
            ec = E_INVALID_ARGUMENT;        
            goto ErrorExit;
        }

        methodAddr = (void*)curInterface->mInterface->mVtptr->mVtable[methodIndex + 4];

        methodInfo = &((curInterface->mInfo)->methods[methodIndex]);
        
        Int32 size = 0;
        Byte* buffer = NULL;
        if (params != NULL) {
            params->GetElementPayload((Handle32*)&buffer);
            params->GetElementSize(&size);
        }
        
        ec = invoke(service->mService, methodAddr, buffer, size);
    }
    else if (header->m_hMethodIndex == 0) {
//        printf("Stub error: Remote Probe(), oid = 0x%08x\n", pInHeader->m_oid);
        ec = E_INVALID_OPERATION;    // Do not allow remote Probe().
        goto ErrorExit;
    }
    else if (header->m_hMethodIndex == 1) {
        // use "ec" to transport the reference
        ec = this->AddRef();

        LOGD("Stub: remote AddRef(), cRefs(%d)\n", ec);
        goto Exit;
    }
    else if (header->m_hMethodIndex == 2) {
        // use "ec" to transport the reference
        ec = this->Release();

        LOGD("Stub: remote Release(), cRefs(%d)\n", ec);
        goto Exit;
    }
    else {
        //GetInterfaceID();
        ec = E_NOT_IMPLEMENTED;
    }  

Exit:
ErrorExit:
    return ec;          
}
