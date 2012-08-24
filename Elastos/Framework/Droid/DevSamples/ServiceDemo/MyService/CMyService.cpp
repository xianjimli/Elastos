
#include "CMyService.h"
#include <stdio.h>

ECode CMyService::OnCreate()
{
    printf("------------CMyService:OnCreate!\n");
    return NOERROR;
}

ECode CMyService::OnDestroy()
{
    return NOERROR;
}

ECode CMyService::OnStart(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return NOERROR;
}

ECode CMyService::OnStartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    return CService::OnStartCommand(intent, flags, startId, result);
}

ECode CMyService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    printf("------------CMyService:OnBind!\n");
    CMyServiceBinder::New(binder);

    return NOERROR;
}

ECode CMyService::OnUnbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return NOERROR;
}

ECode CMyService::OnRebind(
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

//PInterface CMyService::Probe(
//    REIID riid)
//{
//    if (EIID_IInterface == riid) {
//        return (IInterface *)(IObjectEnumerator *)this;
//    }
//    else if (riid == EIID_IObject) {
//        return (IObject *)this;
//    }
//    else if (EIID_IBinder == riid) {
//        return (IBinder *)this;
//    }
//    else if (EIID_IService == riid) {
//        return (IService *)this;
//    }
//    else {
//        return NULL;
//    }
//}
//
//ECode CMyService::GetInterfaceID(
//    /* [in] */ IInterface *pObject,
//    /* [out] */ InterfaceID *pIID)
//{
//    if (NULL == pIID) return E_INVALID_ARGUMENT;
//
//    if (pObject == (IInterface *)(IObject *)this) {
//        *pIID = EIID_IObject;
//    }
//    else if (pObject == (IInterface *)(IBinder *)this) {
//        *pIID = EIID_IBinder;
//    }
//    else if (pObject == (IInterface *)(IService *)this) {
//        *pIID = EIID_IService;
//    }
//    else {
//        return E_INVALID_ARGUMENT;
//    }
//    return NOERROR;
//}
//
//UInt32 CMyService::AddRef()
//{
//    Int32 nRef = atomic_inc(&mRef);
//    return (UInt32)nRef;
//}
//
//UInt32 CMyService::Release()
//{
//    Int32 nRef;
//
//    nRef = atomic_dec(&mRef);
//    if (nRef == 0) {
//        delete this;
//    }
//    return (UInt32)nRef;
//}
