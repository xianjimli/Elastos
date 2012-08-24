
#ifndef __CONTENTRESOLVER_H__
#define __CONTENTRESOLVER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class ContentResolver
{
public:
    static const char* SCHEME_CONTENT;
    static const char* SCHEME_ELASTOS_RESOURCE;
    static const char* SCHEME_FILE;

public:
    ContentResolver(
        /* [in] */ IContext* context = NULL);

    virtual CARAPI AcquireProvider(
        /* [in] */ IContext* context,
        /* [in] */ const String& name,
        /* [out] */ IContentProvider** provider) = 0;

    virtual CARAPI AcquireExistingProvider(
        /* [in] */ IContext* context,
        /* [in] */ const String& name,
        /* [out] */ IContentProvider** provider) = 0;

    CARAPI AcquireProvider(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProvider** provider);

    CARAPI AcquireExistingProvider(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProvider** provider);

    virtual CARAPI ReleaseProvider(
        /* [in] */ IContentProvider* provider) = 0;

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

    /**
     * Return the MIME type of the given content URL.
     *
     * @param url A Uri identifying content (either a list or specific type),
     * using the content:// scheme.
     * @return A MIME type for the content, or null if the URL is invalid or the type is unknown
     */
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

protected:
    AutoPtr<IContext> mContext;
};

#endif //__CONTENTRESOLVER_H__
