
#include "CContentProvider.h"

CContentProvider::~CContentProvider()
{}

ECode CContentProvider::Initialize()
{
    return NOERROR;
}

PInterface CContentProvider::Probe(
    /* [in] */ REIID riid)
{
   if (riid == EIID_IContentProvider) {
        return (IContentProvider *)this;
   }
   else {
       return CBaseObject::Probe(riid);
   }
}

UInt32 CContentProvider::AddRef()
{
    Int32 nRef = atomic_inc(&mRef);
    return (UInt32)nRef;
}

UInt32 CContentProvider::Release()
{
    Int32 nRef;

    nRef = atomic_dec(&mRef);
    if (nRef == 0) {
        delete this;
    }
    return (UInt32)nRef;
}

ECode CContentProvider::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IContentProvider *)this) {
        *pIID = EIID_IContentProvider;
        return NOERROR;
    }
    else {
        return CBaseObject::GetInterfaceID(pObject, pIID);
    }
}

ECode CContentProvider::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IContentProviderInfo* providerInfo)
{  
    /*
     * Only allow it to be set once, so after the content service gives
     * this to us clients can't change it.
     */
    if (mContext == NULL) {
        mContext = context;
//        mMyUid = Process.myUid();
        if (providerInfo != NULL) {
//            setReadPermission(info.readPermission);
//            setWritePermission(info.writePermission);
//            setPathPermissions(info.pathPermissions);
        }
        OnCreate();
    }
    return NOERROR;
}

ECode CContentProvider::OnCreate()
{
    return NOERROR;
}
