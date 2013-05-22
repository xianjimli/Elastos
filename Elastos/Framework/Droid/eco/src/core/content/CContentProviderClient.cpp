
#include "content/CContentProviderClient.h"
#include "ext/frameworkext.h"

CContentProviderClient::CContentProviderClient()
{}

CContentProviderClient::~CContentProviderClient()
{}

ECode CContentProviderClient::Query(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String>& projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(cursor);
    FAIL_RETURN(mContentProvider->Query(uri, projection, selection, selectionArgs, sortOrder, cursor));
    return NOERROR;
}

ECode CContentProviderClient::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(type);
    FAIL_RETURN(mContentProvider->GetType(uri, type));
    return NOERROR;
}

ECode CContentProviderClient::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ IUri** insertedItemUri)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(initialValues);
    VALIDATE_NOT_NULL(insertedItemUri);
    FAIL_RETURN(mContentProvider->Insert(uri, initialValues, insertedItemUri));
    return NOERROR;
}

ECode CContentProviderClient::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<IContentValues *>& initialValues,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(number);
    FAIL_RETURN(mContentProvider->BulkInsert(uri, initialValues, number));
    return NOERROR;
}

ECode CContentProviderClient::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(rowsAffected);
    FAIL_RETURN(mContentProvider->Delete(uri, selection, selectionArgs, rowsAffected));
    return NOERROR;
}

ECode CContentProviderClient::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(values);
    VALIDATE_NOT_NULL(rowsAffected);
    FAIL_RETURN(mContentProvider->Update(uri, values, selection, selectionArgs, rowsAffected));
    return NOERROR;
}

ECode CContentProviderClient::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(fileDescriptor);
    FAIL_RETURN(mContentProvider->OpenFile(uri, mode, fileDescriptor));
    return NOERROR;
}

ECode CContentProviderClient::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(fileDescriptor);
    FAIL_RETURN(mContentProvider->OpenAssetFile(uri, mode, fileDescriptor));
    return NOERROR;
}

ECode CContentProviderClient::ReleaseProvider(
    /* [out] */ Boolean* isRelease)
{
    VALIDATE_NOT_NULL(isRelease);
    FAIL_RETURN(mContentResolver->ReleaseProvider(mContentProvider, isRelease));
    return NOERROR;
}

ECode CContentProviderClient::GetLocalContentProvider(
    /* [out] */ IContentProvider** contentProvider)
{
    //ContentProvider.coerceToLocalContentProvider(mContentProvider);
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderClient::constructor(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IContentProvider* provider)
{
    VALIDATE_NOT_NULL(resolver);
    VALIDATE_NOT_NULL(provider);
    mContentProvider = provider;
    mContentResolver = resolver;
    return NOERROR;
}

