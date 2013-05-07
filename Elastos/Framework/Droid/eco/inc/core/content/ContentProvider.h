
#ifndef __CONTENTPROVIDER_H__
#define __CONTENTPROVIDER_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class ContentProvider
    : public ElRefBase
    , public IObject
    , public IContentProvider
{
public:
    ContentProvider();

    ContentProvider(
        /* [in] */ IContext* context,
        /* [in] */ String readPermission,
        /* [in] */ String writePermission,
        /* [in] */ ArrayOf<IPathPermission*>* pathPermissions);

    virtual ~ContentProvider();

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

    CARAPI AttachInfo(
        /* [in] */ IContext* context,
        /* [in] */ IContentProviderInfo* providerInfo);


    static CARAPI CoerceToLocalContentProvider(
        /* [in] */ IContentProvider* abstractInterface,
        /* [out] */ ContentProvider** provider);

public:
    class Transport: public IContentProviderTransport
    {
        friend class ContentProvider;
    public:
        Transport(ContentProvider* provider);


        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI AttachInfo(
            /* [in] */ IContext* context,
            /* [in] */ IContentProviderInfo* providerInfo);

        CARAPI GetContentProvider(
            /* [out] */ ContentProvider** provider);

        virtual CARAPI BulkQuery(
            /* [in] */ IUri* uri,
            /* [in] */ const ArrayOf<String> & projection,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String> & selectionArgs,
            /* [in] */ const String& sortOrder,
            /* [in] */ IContentObserver* observer,
            /* [in] */ ICursorWindow* window,
            /* [out] */ IBulkCursor** bulkCursor);

        virtual CARAPI Query(
            /* [in] */ IUri* uri,
            /* [in] */ const ArrayOf<String> & projection,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String> & selectionArgs,
            /* [in] */ const String& sortOrder,
            /* [out] */ ICursor** cursor);

        virtual CARAPI GetType(
            /* [in] */ IUri* uri,
            /* [out] */ String* str);

        virtual CARAPI Insert(
            /* [in] */ IUri* uri,
            /* [in] */ IContentValues* initialValues,
            /* [out] */ IUri** insertUri);

        virtual CARAPI BulkInsert(
            /* [in] */ IUri* uri,
            /* [in] */ const ArrayOf<IContentValues*> & initialValues,
            /* [out] */ Int32* number);

        virtual CARAPI ApplyBatch(
            /* [in] */ IObjectContainer* operations,
            /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults);

        virtual CARAPI Delete(
            /* [in] */ IUri* uri,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String> & selectionArgs,
            /* [out] */ Int32* number);

        virtual CARAPI Update(
            /* [in] */ IUri* uri,
            /* [in] */ IContentValues* values,
            /* [in] */ const String& selection,
            /* [in] */ const ArrayOf<String> & selectionArgs,
            /* [out] */ Int32* number);

        virtual CARAPI OpenFile(
            /* [in] */ IUri* uri,
            /* [in] */ const String& mode,
            /* [out] */ IParcelFileDescriptor** fileDescriptor);

        virtual CARAPI OpenAssetFile(
            /* [in] */ IUri* uri,
            /* [in] */ const String& mode,
            /* [out] */ IAssetFileDescriptor** fileDescriptor);

        virtual CARAPI Call(
            /* [in] */ const String& method,
            /* [in] */ const String& request,
            /* [in] */ IBundle* args,
            /* [out] */ IBundle** bundle);
    private:
        CARAPI EnforceReadPermission(
            /* [in] */ IUri* uri);

        CARAPI HasWritePermission(
            /* [in] */ IUri* uri,
            /* [out] */ Boolean* result);

        CARAPI EnforceWritePermission(
            /* [in] */ IUri* uri);
    private:
        ContentProvider* mContentProvider;
    };

public:
    CARAPI GetContext(
        /* [out] */ IContext** content);

    CARAPI GetReadPermission(
        /* [out] */ String* str);

    CARAPI GetWritePermission(
        /* [out] */ String* permission);

    CARAPI GetPathPermissions(
        /* [out] */ ArrayOf<IPathPermission*>** permissions);

    virtual CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI OnLowMemory();


    virtual CARAPI_(Boolean) onCreate() = 0;

    virtual CARAPI Query(
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<String> & projection,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor) = 0;

    virtual CARAPI GetType(
        /* [in] */ IUri* uri,
        /* [out] */ String* str) = 0;

    virtual CARAPI Insert(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* initialValues,
        /* [out] */ IUri** insertUri) = 0;

    virtual CARAPI BulkInsert(
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<IContentValues*> & values,
        /* [out] */ Int32* number);

    virtual CARAPI Delete(
        /* [in] */ IUri* uri,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [out] */ Int32* number) = 0;

    virtual CARAPI Update(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [out] */ Int32* number) = 0;

    virtual CARAPI OpenFile(
        /* [in] */ IUri* uri,
        /* [in] */ String mode,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    virtual CARAPI OpenAssetFile(
        /* [in] */ IUri* uri,
        /* [in] */ String mode,
        /* [out] */ IAssetFileDescriptor** fileDescriptor);

    virtual CARAPI GetIContentProvider(
        /* [out] */ IContentProvider** provider);

    virtual CARAPI ApplyBatch(
        /* [in] */ IObjectContainer* operations,
        /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults);

    virtual CARAPI Call(
        /* [in] */ String method,
        /* [in] */ String request,
        /* [in] */ IBundle* args,
        /* [out] */ IBundle** bundle);

protected:
    CARAPI SetReadPermission(
        /* [in] */ String permission);

    CARAPI SetWritePermission(
        /* [in] */ String permission);

    CARAPI SetPathPermissions(
        /* [in] */ ArrayOf<IPathPermission*>* permissions);

    CARAPI OpenFileHelper(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    CARAPI IsTemporary(
        /* [out] */ Boolean* result);

protected:
    virtual CARAPI OnCreate();

private:
    AutoPtr<IContext> mContext;
    Int32 mMyUid;
    String mReadPermission;
    String mWritePermission;
    ArrayOf<IPathPermission*>* mPathPermissions;
    Boolean mExported;
    AutoPtr<ContentProvider::Transport> mTransport;
};

#endif //__CCONTENTPROVIDER_H__
