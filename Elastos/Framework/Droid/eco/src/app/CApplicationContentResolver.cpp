
#include "app/CApplicationContentResolver.h"

ECode CApplicationContentResolver::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IApplicationApartment* apartment)
{
    mContext = context;
    mApartment = apartment;

    return NOERROR;
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

ECode CApplicationContentResolver::ReleaseProvider(
    /* [in] */ IContentProvider* provider)
{
    return mApartment->ReleaseProvider(provider);
}

ECode CApplicationContentResolver::OpenInputStream(
    /* [in] */ IUri* uri,
    /* [out] */ IInputStream** istream)
{
    return ContentResolver::OpenInputStream(uri, istream);
}

ECode CApplicationContentResolver::GetResourceId(
    /* [in] */ IUri* uri,
    /* [out] */ IOpenResourceIdResult** result)
{
    return ContentResolver::GetResourceId(uri, result);
}

ECode CApplicationContentResolver::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
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

ECode CApplicationContentResolver::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    return ContentResolver::Update(uri,
            values, selection, selectionArgs, rowsAffected);
}
