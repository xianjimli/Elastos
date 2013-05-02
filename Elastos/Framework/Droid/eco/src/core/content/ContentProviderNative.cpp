
#include "content/ContentProviderNative.h"
#include "content/CContentValues.h"
#include "content/CAssetFileDescriptor.h"
#include "content/ContentResolver.h"
#include "database/CCursorToBulkCursorAdaptor.h"
#include "os/CParcelFileDescriptor.h"
#include "ext/frameworkext.h"

CString ContentProviderNative::TAG = "ContentProviderNative";

ContentProviderNative::ContentProviderProxy::ContentProviderProxy(
    IBinder* binder)
{
    mRemote = binder;
}

ContentProviderNative::ContentProviderProxy::~ContentProviderProxy()
{}

PInterface ContentProviderNative::ContentProviderProxy::Probe(
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

UInt32 ContentProviderNative::ContentProviderProxy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ContentProviderNative::ContentProviderProxy::Release()
{
    return ElRefBase::Release();
}

ECode ContentProviderNative::ContentProviderProxy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface *)(IContentProvider *)this) {
        *pIID = EIID_IContentProvider;
        return NOERROR;
    } else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode ContentProviderNative::ContentProviderProxy::AsBinder(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    *binder = mRemote;
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IContentProviderInfo* providerInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::BulkQuery(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String>& projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ IContentObserver* observer,
    /* [in] */ ICursorWindow* window,
    /* [out] */ IBulkCursor** bulkCursor)
{
    VALIDATE_NOT_NULL(bulkCursor);
    FAIL_RETURN(BulkQueryInternal(uri, projection, selection, selectionArgs, sortOrder, observer, window, NULL /* BulkCursorToCursorAdaptor */, bulkCursor));
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::Query(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String>& projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ IUri** insertUri)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<IContentValues*>& initialValues,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResult)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& request,
    /* [in] */ IBundle* args,
    /* [out] */ IBundle** bundle)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::ContentProviderProxy::BulkQueryInternal(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String>& projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ IContentObserver* observer,
    /* [in] */ ICursorWindow* window,
    /* [in] */ IBulkCursorToCursorAdaptor* adaptor,
    /* [out] */ IBulkCursor** bulkCursor)
{
    return E_NOT_IMPLEMENTED;
}

ContentProviderNative::ContentProviderNative()
{
    //attachInterface(this, descriptor);
}

ContentProviderNative::~ContentProviderNative()
{}

PInterface ContentProviderNative::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface) (IContentProvider*) this;
    } else if (riid == EIID_IObject) {
        return (IObject*) this;
    }

    if (riid == EIID_IContentProvider) {
        return (IContentProvider *) this;
    }

    return NULL;
}

UInt32 ContentProviderNative::AddRef()
{
    Int32 nRef = atomic_inc(&mRef);
    return (UInt32)nRef;
}

UInt32 ContentProviderNative::Release()
{
    Int32 nRef;
    nRef = atomic_dec(&mRef);

    if (nRef == 0) {
        delete this;
    }

    return (UInt32)nRef;
}

ECode ContentProviderNative::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID){
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface *)(IContentProvider *)this) {
        *pIID = EIID_IContentProvider;
        return NOERROR;
    } else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode ContentProviderNative::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::AsInterface(
    /* [in] */ IBinder* obj,
    /* [out] */ IContentProvider** contentProvider)
{
    VALIDATE_NOT_NULL(contentProvider);

    if (NULL == obj) {
        *contentProvider = NULL;
        return NOERROR;
    }
    //TODO
    //IContentProvider in = (IContentProvider)obj.queryLocalInterface(descriptor);
    AutoPtr<IContentProvider> provider;

    if (NULL != provider) {
        *contentProvider = provider;
        return NOERROR;
    }

    AutoPtr<ContentProviderNative::ContentProviderProxy> proxy = new ContentProviderNative::ContentProviderProxy(obj);
    provider = (IContentProvider*) proxy->Probe(EIID_IContentProvider);
    *contentProvider = provider;
    return NOERROR;
}

ECode ContentProviderNative::OnTransact(
    /* [in] */ Int32 code,
    /* [in] */ IParcel* data,
    /* [in] */ IParcel* reply,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::AsBinder(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    *binder = (IBinder *) this;
    return NOERROR;
}
