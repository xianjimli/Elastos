
#ifndef __CONTENTPROVIDERNATIVE_H__
#define __CONTENTPROVIDERNATIVE_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "os/Binder.h"

using namespace Elastos;

class ContentProviderNative
    : public ElRefBase
    , public IObject
    , public Binder
    , public IContentProvider
{
public:
    class ContentProviderProxy: public ElRefBase, public IContentProvider
    {
    public:
        ContentProviderProxy(IBinder* binder);

        ~ContentProviderProxy();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI AsBinder(
            /* [out] */ IBinder** binder);     

        CARAPI AttachInfo(
            /* [in] */ IContext* context,
            /* [in] */ IContentProviderInfo* providerInfo);

        CARAPI BulkQuery(
            /* [in] */ IUri* uri,
            /* [in] */ const ArrayOf<String>& projection,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String>& selectionArgs,
            /* [in] */ const String& sortOrder,
            /* [in] */ IContentObserver* observer,
            /* [in] */ ICursorWindow* window,
            /* [out] */ IBulkCursor** bulkCursor);

        CARAPI Query(
            /* [in] */ IUri* uri,
            /* [in] */ const ArrayOf<String>& projection,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String>& selectionArgs,
            /* [in] */ const String& sortOrder,
            /* [out] */ ICursor** cursor);

        CARAPI GetType(
            /* [in] */ IUri* uri,
            /* [out] */ String* str);

        CARAPI Insert(
            /* [in] */ IUri* uri,
            /* [in] */ IContentValues* initialValues,
            /* [out] */ IUri** insertUri);

        CARAPI BulkInsert(
            /* [in] */ IUri* uri,
            /* [in] */ const ArrayOf<IContentValues*>& initialValues,
            /* [out] */ Int32* number);

        // public ContentProviderResult[] applyBatch(ArrayList<ContentProviderOperation> operations);
        CARAPI ApplyBatch(
            /* [in] */ IObjectContainer* operations,
            /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResult);

        CARAPI Delete(
            /* [in] */ IUri* uri,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String>& selectionArgs,
            /* [out] */ Int32* number);

        CARAPI Update(
            /* [in] */ IUri* uri,
            /* [in] */ IContentValues* values,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String>& selectionArgs,
            /* [out] */ Int32* number);

        CARAPI OpenFile(
            /* [in] */ IUri* uri,
            /* [in] */ const String& mode,
            /* [out] */ IParcelFileDescriptor** fileDescriptor);

        CARAPI OpenAssetFile(
            /* [in] */ IUri* uri,
            /* [in] */ const String& mode,
            /* [out] */ IAssetFileDescriptor** fileDescriptor);

        CARAPI Call(
            /* [in] */ const String& method,
            /* [in] */ const String& request,
            /* [in] */ IBundle* args,
            /* [out] */ IBundle** bundle);

    private:
        // Like bulkQuery() but sets up provided 'adaptor' if not null.
        CARAPI BulkQueryInternal(
            /* [in] */ IUri* uri,
            /* [in] */ const ArrayOf<String>& projection,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String>& selectionArgs,
            /* [in] */ const String& sortOrder,
            /* [in] */ IContentObserver* observer,
            /* [in] */ ICursorWindow* window,
            /* [in] */ IBulkCursorToCursorAdaptor* adaptor,
            /* [out] */ IBulkCursor** bulkCursor);

    private:
        AutoPtr<IBinder> mRemote;

    };

public:

    ContentProviderNative();

    virtual ~ContentProviderNative();

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

    /**
     * Cast a Binder object into a content resolver interface, generating
     * a proxy if needed.
     */
    static CARAPI AsInterface(
        /* [in] */ IBinder* obj,
        /* [out] */ IContentProvider** contentProvider);

    CARAPI OnTransact(
        /* [in] */ Int32 code,
        /* [in] */ IParcel* data,
        /* [in] */ IParcel* reply,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* result);

    CARAPI AsBinder(
        /* [out] */ IBinder** binder);   

private:
    static CString TAG;
};

#endif //__CONTENTPROVIDERNATIVE_H__
