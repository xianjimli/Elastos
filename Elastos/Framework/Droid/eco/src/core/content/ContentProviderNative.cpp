
#include "content/ContentProviderNative.h"
#include "content/CContentProviderOperation.h"
#include "content/CContentProviderResult.h"
#include "content/CContentValues.h"
#include "content/CAssetFileDescriptor.h"
#include "content/ContentResolver.h"
#include "database/BulkCursorToCursorAdaptor.h"
#include "database/CBulkCursorToCursorAdaptor.h"
#include "database/CCursorToBulkCursorAdaptor.h"
#include "database/CCursorWindow.h"
#include "database/DatabaseUtils.h"
#include "os/CParcelFileDescriptor.h"
#include "net/CStringUri.h"
#include "net/COpaqueUri.h"
#include "net/CHierarchicalUri.h"
#include "net/CPart.h"
#include "net/CPathPart.h"
#include "ext/frameworkext.h"
#include "utils/CParcelableObjectContainer.h"
#include <elastos/AutoFree.h>
#include <elastos/List.h>

#define FAILED_WITH_RETURN(ec, reply, result) do { \
                                if (FAILED(ec)) {   \
                                    DatabaseUtils::WriteExceptionToParcel(reply, ec);  \
                                    *result = TRUE; \
                                    return ec; \
                                }                   \
                            } while (0)

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
    VALIDATE_NOT_NULL(cursor);
    //TODO make a pool of windows so we can reuse memory dealers
    AutoPtr<ICursorWindow> window;
    FAIL_RETURN(CCursorWindow::New(FALSE /* window will be used remotely */, (ICursorWindow**)&window));
    AutoPtr<IBulkCursorToCursorAdaptor> adaptor;
    FAIL_RETURN(CBulkCursorToCursorAdaptor::New((IBulkCursorToCursorAdaptor**)&adaptor));
    AutoPtr<IBulkCursor> bulkCursor;
    AutoPtr<IContentObserver> observer;
    FAIL_RETURN(adaptor->GetObserver((IContentObserver**)&observer));
    FAIL_RETURN(BulkQueryInternal(uri, projection, selection, selectionArgs, sortOrder, observer, window, adaptor, (IBulkCursor**)&bulkCursor));

    if (NULL == bulkCursor) {
        *cursor = NULL;
        return NOERROR;
    }
    
    *cursor = adaptor;
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(str);
    AutoPtr<IParcel> data; //Parcel.obtain();
    AutoPtr<IParcel> reply; //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> parcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->WriteToParcel(reply));
    //mRemote.transact(IContentProvider.GET_TYPE_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionFromParcel(reply));
    FAIL_RETURN(reply->ReadString(str));
    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ IUri** insertUri)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(initialValues);
    VALIDATE_NOT_NULL(insertUri);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> uriParcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(uriParcelable->WriteToParcel(reply));
    AutoPtr<IParcelable> valuesParcelable = (IParcelable*) initialValues->Probe(EIID_IParcelable);
    FAIL_RETURN(valuesParcelable->WriteToParcel(reply));
    //mRemote.transact(IContentProvider.INSERT_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionFromParcel(reply));
    Int32 type;
    FAIL_RETURN(reply->ReadInt32(&type));
    String str1;
    
    if (1 == type) {
        FAIL_RETURN(reply->ReadString(&str1));
        FAIL_RETURN(CStringUri::New(str1, insertUri));
    } else if (2 == type) {
        FAIL_RETURN(reply->ReadString(&str1));
        AutoPtr<IPart> part1;
        FAIL_RETURN(Part::ReadFrom(reply, (IPart**)&part1));
        AutoPtr<IPart> part2;
        FAIL_RETURN(Part::ReadFrom(reply, (IPart**)&part2));
        FAIL_RETURN(COpaqueUri::New(str1, part1, part2, insertUri));
    } else if (3 == type) {
        FAIL_RETURN(reply->ReadString(&str1));
        AutoPtr<IPart> part1;
        FAIL_RETURN(Part::ReadFrom(reply, (IPart**)&part1));
        AutoPtr<IPathPart> pathPart1;
        FAIL_RETURN(PathPart::ReadFrom(reply, (IPathPart**)&pathPart1));
        AutoPtr<IPart> part2;
        FAIL_RETURN(Part::ReadFrom(reply, (IPart**)&part2));
        AutoPtr<IPart> part3;
        FAIL_RETURN(Part::ReadFrom(reply, (IPart**)&part3));
        FAIL_RETURN(CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, insertUri));
    }

    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<IContentValues*>& values,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(number);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> uriParcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(uriParcelable->WriteToParcel(reply));
    
    if (NULL != (&values)) {
        Int32 N = values.GetLength();
        FAIL_RETURN(data->WriteInt32(N));

        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IContentValues> item = values[i];
            if (NULL != item) {
                FAIL_RETURN(data->WriteInt32(1));
                AutoPtr<IParcelable> itemParcelable = (IParcelable*) item->Probe(EIID_IParcelable);
                FAIL_RETURN(itemParcelable->WriteToParcel(data));
            } else {
                FAIL_RETURN(data->WriteInt32(0));
            }
        }

    } else {
        FAIL_RETURN(data->WriteInt32(-1));
    }

    //mRemote.transact(IContentProvider.BULK_INSERT_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionFromParcel(reply));
    FAIL_RETURN(reply->ReadInt32(number));
    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults)
{
    VALIDATE_NOT_NULL(operations);
    VALIDATE_NOT_NULL(providerResults);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    Int32 count;
    FAIL_RETURN(operations->GetObjectCount(&count));
    FAIL_RETURN(data->WriteInt32(count));
    Boolean hasNext;
    AutoPtr<IContentProviderOperation> operation;
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    FAIL_RETURN(operations->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator));

    while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
        FAIL_RETURN(ObjEnumerator->Current((IInterface**) &operation));
        AutoPtr<IParcelable> operationParcelable = (IParcelable*) operation->Probe(EIID_IParcelable);
        FAIL_RETURN(operationParcelable->WriteToParcel(data));
    }

    //mRemote.transact(IContentProvider.APPLY_BATCH_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionWithOperationApplicationExceptionFromParcel(reply));
    Int32 N;
    Int32 value;
    FAIL_RETURN(reply->ReadInt32(&N));

    if (N < 0) {
        *providerResults = NULL;
        return NOERROR;
    }

    AutoPtr<IContentProviderResult> tmpProviderResult;

    for (Int32 i = 0; i < N; i++) {
        FAIL_RETURN(reply->ReadInt32(&value));
        if (0 != value) {
            FAIL_RETURN(CContentProviderResult::New(reply, (IContentProviderResult**)&tmpProviderResult));
            (**providerResults)[i] = tmpProviderResult;
        }
    }

    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(number);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> uriParcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(uriParcelable->WriteToParcel(reply));
    FAIL_RETURN(data->WriteString(selection));
    FAIL_RETURN(data->WriteArrayOfString(selectionArgs));
    //mRemote.transact(IContentProvider.DELETE_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionFromParcel(reply));
    FAIL_RETURN(reply->ReadInt32(number));
    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(values);
    VALIDATE_NOT_NULL(number);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> uriParcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(uriParcelable->WriteToParcel(reply));
    AutoPtr<IParcelable> valuesParcelable = (IParcelable*) values->Probe(EIID_IParcelable);
    FAIL_RETURN(valuesParcelable->WriteToParcel(reply));
    FAIL_RETURN(data->WriteString(selection));
    FAIL_RETURN(data->WriteArrayOfString(selectionArgs));
    //mRemote.transact(IContentProvider.UPDATE_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionFromParcel(reply));
    FAIL_RETURN(reply->ReadInt32(number));
    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(fileDescriptor);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> uriParcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(uriParcelable->WriteToParcel(reply));
    FAIL_RETURN(data->WriteString(mode));
    //mRemote.transact(IContentProvider.OPEN_FILE_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionWithFileNotFoundExceptionFromParcel(reply));
    Int32 has;
    FAIL_RETURN(reply->ReadInt32(&has));
    //ParcelFileDescriptor fd = has != 0 ? reply.readFileDescriptor() : null;

    if (has != 0) {
        Int32 fd;
        FAIL_RETURN(reply->ReadFileDescriptor(&fd));
        //TODO
    } else {
        *fileDescriptor = NULL;
    }

    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(fileDescriptor);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> uriParcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(uriParcelable->WriteToParcel(reply));
    FAIL_RETURN(data->WriteString(mode));
    //mRemote.transact(IContentProvider.OPEN_ASSET_FILE_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionWithFileNotFoundExceptionFromParcel(reply));
    Int32 has;
    FAIL_RETURN(reply->ReadInt32(&has));

    if (has != 0) {
        FAIL_RETURN(CAssetFileDescriptor::New(reply, fileDescriptor));
    } else {
        *fileDescriptor = NULL;
    }

    //data.recycle();
    //reply.recycle();
    return NOERROR;
}

ECode ContentProviderNative::ContentProviderProxy::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& request,
    /* [in] */ IBundle* args,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    FAIL_RETURN(data->WriteString(method));
    FAIL_RETURN(data->WriteString(request));

    if (NULL == args) {
        FAIL_RETURN(data->WriteInt32(-1));
    } else {
        FAIL_RETURN(data->WriteInterfacePtr(args));
    }

    //mRemote.transact(IContentProvider.CALL_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionFromParcel(reply));
    AutoPtr<IInterface> obj;
    reply->ReadInterfacePtrPtr((Handle32*) &obj);
    *bundle = obj != NULL ? (IBundle*) obj->Probe(EIID_IBundle) : NULL;
    //data.recycle();
    //reply.recycle();
    return NOERROR;
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
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(observer);
    VALIDATE_NOT_NULL(window);
    VALIDATE_NOT_NULL(bulkCursor);
    AutoPtr<IParcel> data;    //Parcel.obtain();
    AutoPtr<IParcel> reply;    //Parcel.obtain();
    //data.writeInterfaceToken(IContentProvider.descriptor);
    AutoPtr<IParcelable> parcelable = (IParcelable*) uri->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->WriteToParcel(reply));
    Int32 length = 0;

    if (NULL != (&projection)) {
        length = projection.GetLength();
    }

    FAIL_RETURN(data->WriteInt32(length));

    for (Int32 i = 0; i < length; i++) {
        FAIL_RETURN(data->WriteString(projection[i]));
    }

    FAIL_RETURN(data->WriteString(selection));

    if (NULL != (&selectionArgs)) {
        length = selectionArgs.GetLength();
    } else {
        length = 0;
    }

    FAIL_RETURN(data->WriteInt32(length));

    for (Int32 i = 0; i < length; i++) {
        FAIL_RETURN(data->WriteString(selectionArgs[i]));
    }

    FAIL_RETURN(data->WriteString(sortOrder));
    //data->writeStrongBinder(observer.asBinder());
    AutoPtr<IParcelable> windowParcelable = (IParcelable*) window->Probe(EIID_IParcelable);
    FAIL_RETURN(windowParcelable->WriteToParcel(data));

    // Flag for whether or not we want the number of rows in the
    // cursor and the position of the "_id" column index (or -1 if
    // non-existent).  Only to be returned if binder != null.
    Boolean wantsCursorMetadata = (NULL != adaptor);
    FAIL_RETURN(data->WriteInt32(wantsCursorMetadata ? 1 : 0));
    //mRemote.transact(IContentProvider.QUERY_TRANSACTION, data, reply, 0);
    FAIL_RETURN(DatabaseUtils::ReadExceptionFromParcel(reply));
    AutoPtr<IBinder> bulkCursorBinder;
    //IBinder bulkCursorBinder = reply.readStrongBinder();

    if (NULL != bulkCursorBinder) {
        //bulkCursor = BulkCursorNative.asInterface(bulkCursorBinder);

        if (wantsCursorMetadata) {
            Int32 rowCount;
            FAIL_RETURN(reply->ReadInt32(&rowCount));
            Int32 idColumnPosition;
            FAIL_RETURN(reply->ReadInt32(&idColumnPosition));

            if (NULL != (*bulkCursor)) {
                FAIL_RETURN(adaptor->SetEx(*bulkCursor, rowCount, idColumnPosition));
            }

        }

    }

    //data.recycle();
    //reply.recycle();
    return NOERROR;
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
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(reply);
    VALIDATE_NOT_NULL(result);
    ECode ec;

    switch (code) {
        case contentprovider_QUERY_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            AutoPtr<IUri> url;
            Int32 type;
            ec = data->ReadInt32(&type);
            FAILED_WITH_RETURN(ec, reply, result);
            String str1;
            
            if (1 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CStringUri::New(str1, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (2 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = COpaqueUri::New(str1, part1, part2, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (3 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPathPart> pathPart1;
                ec = PathPart::ReadFrom(data, (IPathPart**)&pathPart1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part3;
                ec = Part::ReadFrom(data, (IPart**)&part3);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            // String[] projection
            Int32 num;
            ec = data->ReadInt32(&num);
            FAILED_WITH_RETURN(ec, reply, result);
            ArrayOf<String>* projection;

            if (num > 0) {
                projection = ArrayOf<String>::Alloc(num);
                for (Int32 i = 0; i < num; i++) {
                    String str;
                    ec = data->ReadString(&str);
                    FAILED_WITH_RETURN(ec, reply, result);
                    (*projection)[i] = str;
                }
            }

            // String selection, String[] selectionArgs...
            String selection;
            ec = data->ReadString(&selection);
            FAILED_WITH_RETURN(ec, reply, result);
            ec = data->ReadInt32(&num);
            FAILED_WITH_RETURN(ec, reply, result);
            ArrayOf<String>* selectionArgs;

            if (num > 0) {
                selectionArgs = ArrayOf<String>::Alloc(num);
                for (Int32 i = 0; i < num; i++) {
                    String str;
                    ec = data->ReadString(&str);
                    FAILED_WITH_RETURN(ec, reply, result);
                    (*selectionArgs)[i] = str;
                }
            }

            String sortOrder;
            ec = data->ReadString(&sortOrder);
            FAILED_WITH_RETURN(ec, reply, result);
            AutoPtr<IContentObserver> observer;
            //IContentObserver.Stub.asInterface(data.readStrongBinder());
            AutoPtr<ICursorWindow> window;
            //CursorWindow.CREATOR.createFromParcel(data);

            // Flag for whether caller wants the number of
            // rows in the cursor and the position of the
            // "_id" column index (or -1 if non-existent)
            // Only to be returned if binder != null.
            ec = data->ReadInt32(&num);
            FAILED_WITH_RETURN(ec, reply, result);
            Boolean wantsCursorMetadata = num != 0;
            AutoPtr<IBulkCursor> bulkCursor;
            ec = BulkQuery(url, *projection, selection, *selectionArgs, sortOrder, observer, window, (IBulkCursor**) &bulkCursor);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();

            if (NULL != bulkCursor) {
                //reply.writeStrongBinder(bulkCursor.asBinder());

                if (wantsCursorMetadata) {
                    ec = bulkCursor->GetCount(&num);
                    FAILED_WITH_RETURN(ec, reply, result);
                    ec = reply->WriteInt32(num);
                    FAILED_WITH_RETURN(ec, reply, result);
                    AutoFree<ArrayOf<String> > names;
                    ec = bulkCursor->GetColumnNames((ArrayOf<String>**)&names);
                    FAILED_WITH_RETURN(ec, reply, result);
                    ec = reply->WriteInt32(BulkCursorToCursorAdaptor::FindRowIdColumnIndex(names));
                    FAILED_WITH_RETURN(ec, reply, result);
                }

            } else {
                //reply.writeStrongBinder(null);
            }

            ArrayOf<String>::Free(projection);
            ArrayOf<String>::Free(selectionArgs);
            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_GET_TYPE_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            AutoPtr<IUri> url;
            Int32 type;
            ec = data->ReadInt32(&type);
            FAILED_WITH_RETURN(ec, reply, result);
            String str1;
            
            if (1 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CStringUri::New(str1, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (2 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = COpaqueUri::New(str1, part1, part2, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (3 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPathPart> pathPart1;
                ec = PathPart::ReadFrom(data, (IPathPart**)&pathPart1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part3;
                ec = Part::ReadFrom(data, (IPart**)&part3);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            String typeStr;
            ec = GetType(url, &typeStr);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();
            ec = reply->WriteString(typeStr);
            FAILED_WITH_RETURN(ec, reply, result);
            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_INSERT_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            AutoPtr<IUri> url;
            Int32 type;
            ec = data->ReadInt32(&type);
            FAILED_WITH_RETURN(ec, reply, result);
            String str1;
            
            if (1 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CStringUri::New(str1, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (2 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = COpaqueUri::New(str1, part1, part2, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (3 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPathPart> pathPart1;
                ec = PathPart::ReadFrom(data, (IPathPart**)&pathPart1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part3;
                ec = Part::ReadFrom(data, (IPart**)&part3);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            AutoPtr<IContentValues> values;
            //ContentValues.CREATOR.createFromParcel(data);
            AutoPtr<IUri> out;
            ec = Insert(url, values, (IUri**)&out);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();

            if (NULL == out) {
                ec = reply->WriteInt32(0);
                FAILED_WITH_RETURN(ec, reply, result);
            } else {
                AutoPtr<IParcelable> parcelable = (IParcelable*) out->Probe(EIID_IParcelable);
                ec = parcelable->WriteToParcel(reply);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_APPLY_BATCH_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            Int32 numOperations;
            ec = data->ReadInt32(&numOperations);
            FAILED_WITH_RETURN(ec, reply, result);

            AutoPtr<IObjectContainer> operations;
            FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&operations));

            for (Int32 i = 0; i < numOperations; i++) {
                AutoPtr<IContentProviderOperation> oper;
                ec = CContentProviderOperation::New(data, (IContentProviderOperation**)&oper);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = operations->Add(oper.Get());
                FAILED_WITH_RETURN(ec, reply, result);
            }

            AutoFree<ArrayOf<IContentProviderResult*> > results;
            ec = ApplyBatch(operations, (ArrayOf<IContentProviderResult*>**)&results);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();
            
            if (NULL != results) {
                Int32 N = results->GetLength();
                ec = reply->WriteInt32(N);
                FAILED_WITH_RETURN(ec, reply, result);

                for (Int32 i=0; i<N; i++) {
                    AutoPtr<IContentProviderResult> item = (*results)[i];

                    if (NULL != item) {
                        ec = reply->WriteInt32(1);
                        FAILED_WITH_RETURN(ec, reply, result);
                        AutoPtr<IParcelable> itemParcelable = (IParcelable*) item->Probe(EIID_IParcelable);
                        ec = itemParcelable->WriteToParcel(reply);
                        FAILED_WITH_RETURN(ec, reply, result);
                    } else {
                        ec = reply->WriteInt32(0);
                        FAILED_WITH_RETURN(ec, reply, result);
                    }

                }

            } else {
                ec = reply->WriteInt32(-1);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_DELETE_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            AutoPtr<IUri> url;
            Int32 type;
            ec = data->ReadInt32(&type);
            FAILED_WITH_RETURN(ec, reply, result);
            String str1;
            
            if (1 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CStringUri::New(str1, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (2 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = COpaqueUri::New(str1, part1, part2, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (3 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPathPart> pathPart1;
                ec = PathPart::ReadFrom(data, (IPathPart**)&pathPart1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part3;
                ec = Part::ReadFrom(data, (IPart**)&part3);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            }
            
            String selection;
            ec = data->ReadString(&selection);
            FAILED_WITH_RETURN(ec, reply, result);
            AutoFree<ArrayOf<String> > selectionArgs;
            ec = data->ReadArrayOfString((Handle32*) &selectionArgs);
            FAILED_WITH_RETURN(ec, reply, result);
            Int32 count;
            ec = Delete(url, selection, *selectionArgs, &count);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();
            ec = reply->WriteInt32(count);
            FAILED_WITH_RETURN(ec, reply, result);
            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_UPDATE_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            AutoPtr<IUri> url;
            Int32 type;
            ec = data->ReadInt32(&type);
            FAILED_WITH_RETURN(ec, reply, result);
            String str1;
            
            if (1 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CStringUri::New(str1, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (2 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = COpaqueUri::New(str1, part1, part2, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (3 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPathPart> pathPart1;
                ec = PathPart::ReadFrom(data, (IPathPart**)&pathPart1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part3;
                ec = Part::ReadFrom(data, (IPart**)&part3);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            AutoPtr<IContentValues> values;
            //ContentValues.CREATOR.createFromParcel(data);
            String selection;
            ec = data->ReadString(&selection);
            FAILED_WITH_RETURN(ec, reply, result);
            AutoFree<ArrayOf<String> > selectionArgs;
            ec = data->ReadArrayOfString((Handle32*) &selectionArgs);
            FAILED_WITH_RETURN(ec, reply, result);
            Int32 count;
            ec = Update(url, values, selection, *selectionArgs, &count);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();
            ec = reply->WriteInt32(count);
            FAILED_WITH_RETURN(ec, reply, result);
            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_OPEN_FILE_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            AutoPtr<IUri> url;
            Int32 type;
            ec = data->ReadInt32(&type);
            FAILED_WITH_RETURN(ec, reply, result);
            String str1;
            
            if (1 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CStringUri::New(str1, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (2 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = COpaqueUri::New(str1, part1, part2, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (3 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPathPart> pathPart1;
                ec = PathPart::ReadFrom(data, (IPathPart**)&pathPart1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part3;
                ec = Part::ReadFrom(data, (IPart**)&part3);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            String mode;
            ec = data->ReadString(&mode);
            FAILED_WITH_RETURN(ec, reply, result);
            AutoPtr<IParcelFileDescriptor> fd;
            ec = OpenFile(url, mode, (IParcelFileDescriptor**)&fd);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();

            if (NULL != fd) {
                ec = reply->WriteInt32(1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IParcelable> parcelable = (IParcelable*) fd->Probe(EIID_IParcelable);
                ec = parcelable->WriteToParcel(reply);//fd.writeToParcel(reply,Parcelable.PARCELABLE_WRITE_RETURN_VALUE)
                FAILED_WITH_RETURN(ec, reply, result);
            } else {
                ec = reply->WriteInt32(0);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_OPEN_ASSET_FILE_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            AutoPtr<IUri> url;
            Int32 type;
            ec = data->ReadInt32(&type);
            FAILED_WITH_RETURN(ec, reply, result);
            String str1;
            
            if (1 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CStringUri::New(str1, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (2 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = COpaqueUri::New(str1, part1, part2, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            } else if (3 == type) {
                ec = data->ReadString(&str1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part1;
                ec = Part::ReadFrom(data, (IPart**)&part1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPathPart> pathPart1;
                ec = PathPart::ReadFrom(data, (IPathPart**)&pathPart1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part2;
                ec = Part::ReadFrom(data, (IPart**)&part2);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IPart> part3;
                ec = Part::ReadFrom(data, (IPart**)&part3);
                FAILED_WITH_RETURN(ec, reply, result);
                ec = CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&url);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            String mode;
            ec = data->ReadString(&mode);
            FAILED_WITH_RETURN(ec, reply, result);
            AutoPtr<IAssetFileDescriptor> fd;
            ec = OpenAssetFile(url, mode, (IAssetFileDescriptor**)&fd);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();

            if (NULL != fd) {
                ec = reply->WriteInt32(1);
                FAILED_WITH_RETURN(ec, reply, result);
                AutoPtr<IParcelable> parcelable = (IParcelable*) fd->Probe(EIID_IParcelable);
                ec = parcelable->WriteToParcel(reply);//fd.writeToParcel(reply,Parcelable.PARCELABLE_WRITE_RETURN_VALUE)
                FAILED_WITH_RETURN(ec, reply, result);
            } else {
                ec = reply->WriteInt32(0);
                FAILED_WITH_RETURN(ec, reply, result);
            }

            *result = TRUE;
            return NOERROR;
        }
        case contentprovider_CALL_TRANSACTION:
        {
            //data.enforceInterface(IContentProvider.descriptor);
            String method;
            ec = data->ReadString(&method);
            FAILED_WITH_RETURN(ec, reply, result);
            String stringArg;
            ec = data->ReadString(&stringArg);
            FAILED_WITH_RETURN(ec, reply, result);
            AutoPtr<IBundle> args;
            ec = data->ReadInterfacePtr((Handle32*) &args);
            FAILED_WITH_RETURN(ec, reply, result);
            AutoPtr<IBundle> responseBundle;
            ec = Call(method, stringArg, args, (IBundle**)&responseBundle);
            FAILED_WITH_RETURN(ec, reply, result);
            //reply.writeNoException();
            ec = reply->WriteInterfacePtr((IInterface*)responseBundle);
            FAILED_WITH_RETURN(ec, reply, result);
            *result = TRUE;
            return NOERROR;
        }

    }

    //super.onTransact(code, data, reply, flags);
    //*result = Binder::onTransact(code, data, reply, flags);
    return E_NOT_IMPLEMENTED;
}

ECode ContentProviderNative::AsBinder(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    *binder = (IBinder *) this;
    return NOERROR;
}


