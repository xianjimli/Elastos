
#ifndef __CAPPLICATIONCONTENTRESOLVER_H__
#define __CAPPLICATIONCONTENTRESOLVER_H__

#include "_CApplicationContentResolver.h"
#include "content/ContentResolver.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CApplicationContentResolver), public ContentResolver
{
public:

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IApplicationApartment* apartment);

    CARAPI AcquireProvider(
        /* [in] */ IContext* context,
        /* [in] */ const String& name,
        /* [out] */ IContentProvider** provider);

    CARAPI AcquireExistingProvider(
        /* [in] */ IContext* context,
        /* [in] */ const String& name,
        /* [out] */ IContentProvider** provider);

    CARAPI ReleaseProvider(
        /* [in] */ IContentProvider* provider);

    CARAPI OpenInputStream(
        /* [in] */ IUri* uri,
        /* [out] */ IInputStream** istream);

    CARAPI GetResourceId(
        /* [in] */ IUri* uri,
        /* [out] */ IOpenResourceIdResult** result);

    CARAPI Delete(
        /* [in] */ IUri* uri,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [out] */ Int32* rowsAffected);

    CARAPI GetType(
        /* [in] */ IUri* uri,
        /* [out] */ String* type);

    CARAPI Insert(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [out] */ IUri** insertedItemUri);

    CARAPI Query(
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<String>& projection,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    CARAPI Update(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [out] */ Int32* rowsAffected);

private:
    AutoPtr<IApplicationApartment> mApartment;
};

#endif //__CAPPLICATIONCONTENTRESOLVER_H__
