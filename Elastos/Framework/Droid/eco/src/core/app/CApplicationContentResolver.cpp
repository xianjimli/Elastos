
#include "app/CApplicationContentResolver.h"

IContentProvider* CApplicationContentResolver::ProbeIContentProvider()
{
    return (IContentProvider*)this;
}

ECode CApplicationContentResolver::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IApplicationApartment* apartment)
{
    mContext = context;
    mApartment = apartment;

    return NOERROR;
}

ECode CApplicationContentResolver::ReleaseProvider(
	/* [in] */ IContentProvider* icp,
	/* [out] */ Boolean* result)
{
    // TODO: ALEX fix IApplicationApartment::ReleaseProvider
    if (result) *result = TRUE;

    return mApartment->ReleaseProvider(icp);
}

ECode CApplicationContentResolver::AcquireProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& name,
    /* [out] */ IContentProvider** provider)
{
    return mApartment->AcquireProvider(context, name, provider);
}

ECode CApplicationContentResolver::AcquireExistingProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& name,
    /* [out] */ IContentProvider** provider)
{
    return mApartment->AcquireExistingProvider(context, name, provider);
}

ECode CApplicationContentResolver::AcquireExistingProvider2(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProvider** result)
{
    return ContentResolver::AcquireExistingProvider2(uri, result);
}

ECode CApplicationContentResolver::AcquireProvider3(
    /* [in] */ const String& name,
    /* [out] */ IContentProvider** provider)
{
    return ContentResolver::AcquireProvider3(name, provider);
}

ECode CApplicationContentResolver::AcquireProvider2(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProvider** result)
{
    return ContentResolver::AcquireProvider2(uri, result);
}

ECode CApplicationContentResolver::GetResourceId(
    /* [in] */ IUri* uri,
    /* [out] */ IOpenResourceIdResult** result)
{
    return ContentResolver::GetResourceId(uri, result);
}

ECode CApplicationContentResolver::BulkInsert(
    /* [in] */ IUri* url,
    /* [in] */ const ArrayOf<IContentValues*>& values,
    /* [out] */ Int32* result)
{
    return ContentResolver::BulkInsert(
        url, values, result);
}

ECode CApplicationContentResolver::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    return ContentResolver::Delete(uri,
            selection, selectionArgs, rowsAffected);
}

ECode CApplicationContentResolver::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    return ContentResolver::GetType(uri, type);
}

ECode CApplicationContentResolver::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [out] */ IUri** insertedItemUri)
{
    return ContentResolver::Insert(uri, values, insertedItemUri);
}

ECode CApplicationContentResolver::Query(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String>& projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    return ContentResolver::Query(uri,
            projection, selection, selectionArgs,
            sortOrder, cursor);
}

ECode CApplicationContentResolver::OpenInputStream(
    /* [in] */ IUri* uri,
    /* [out] */ IInputStream** istream)
{
    return ContentResolver::OpenInputStream(uri, istream);
}

ECode CApplicationContentResolver::OpenOutputStream(
    /* [in] */ IUri* uri,
    /* [out] */ IOutputStream** ostream)
{
    return ContentResolver::OpenOutputStream(uri, ostream);
}

ECode CApplicationContentResolver::OpenOutputStream2(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IOutputStream** ostream)
{
    return ContentResolver::OpenOutputStream2(uri, mode, ostream);
}

ECode CApplicationContentResolver::OpenFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** result)
{
    return ContentResolver::OpenFileDescriptor(
        uri, mode, result);
}

ECode CApplicationContentResolver::OpenAssetFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** result)
{
    return ContentResolver::OpenAssetFileDescriptor(
        uri, mode, result);
}

ECode CApplicationContentResolver::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    return ContentResolver::Update(uri,
            values, selection, selectionArgs, rowsAffected);
}

ECode CApplicationContentResolver::RegisterContentObserver(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendents,
    /* [in] */ IContentObserver* observer)
{
    return ContentResolver::RegisterContentObserver(
        uri, notifyForDescendents, observer);
}

ECode CApplicationContentResolver::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return ContentResolver::UnregisterContentObserver(observer);
}

ECode CApplicationContentResolver::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ IContentObserver* observer)
{
    return ContentResolver::NotifyChange(uri, observer);
}

ECode CApplicationContentResolver::NotifyChange2(
    /* [in] */ IUri* uri,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean syncToNetwork)
{
    return ContentResolver::NotifyChange2(
        uri, observer, syncToNetwork);
}