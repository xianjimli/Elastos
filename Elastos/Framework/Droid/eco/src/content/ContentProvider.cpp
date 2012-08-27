
#ifdef _FRAMEWORK
#include "content/ContentProvider.h"
#else
#include "ContentProvider.h"
#endif


ContentProvider::~ContentProvider()
{}

ECode ContentProvider::Initialize()
{
    return NOERROR;
}

PInterface ContentProvider::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IContentProvider*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    if (riid == EIID_IContentProvider) {
        return (IContentProvider *)this;
    }

    return NULL;
}

UInt32 ContentProvider::AddRef()
{
    Int32 nRef = atomic_inc(&mRef);
    return (UInt32)nRef;
}

UInt32 ContentProvider::Release()
{
    Int32 nRef;

    nRef = atomic_dec(&mRef);
    if (nRef == 0) {
        delete this;
    }
    return (UInt32)nRef;
}

ECode ContentProvider::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IContentProvider *)this) {
        *pIID = EIID_IContentProvider;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode ContentProvider::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::AttachInfo(
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

ECode ContentProvider::OnCreate()
{
    return NOERROR;
}
