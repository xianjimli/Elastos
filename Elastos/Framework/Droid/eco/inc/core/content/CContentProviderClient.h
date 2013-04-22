
#ifndef __CCONTENTPROVIDERCLIENT_H__
#define __CCONTENTPROVIDERCLIENT_H__

#include "_CContentProviderClient.h"
#include <elastos/AutoPtr.h>


CarClass(CContentProviderClient)
{
public:
    CContentProviderClient();

    ~CContentProviderClient();

    /** see {@link ContentProvider#query} */
    CARAPI Query(
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<String>& projection,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    /** see {@link ContentProvider#getType} */
    CARAPI GetType(
        /* [in] */ IUri* uri,
        /* [out] */ String* type);

    /** see {@link ContentProvider#insert} */
    CARAPI Insert(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* initialValues,
        /* [out] */ IUri** insertedItemUri);

    /** see {@link ContentProvider#bulkInsert} */
    CARAPI BulkInsert(
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<IContentValues *>& initialValues,
        /* [out] */ Int32* number);

    /** see {@link ContentProvider#delete} */
    CARAPI Delete(
        /* [in] */ IUri* uri,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [out] */ Int32* rowsAffected);

    /** see {@link ContentProvider#update} */
    CARAPI Update(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* pValues,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [out] */ Int32* rowsAffected);

    /** see {@link ContentProvider#openFile} */
    CARAPI OpenFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    /** see {@link ContentProvider#openAssetFile} */
    CARAPI OpenAssetFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IAssetFileDescriptor** fileDescriptor);

    /**
     * Call this to indicate to the system that the associated {@link ContentProvider} is no
     * longer needed by this {@link ContentProviderClient}.
     * @return true if this was release, false if it was already released
     */
    CARAPI ReleaseProvider(
        /* [out] */ Boolean* isRelease);

    /**
     * Get a reference to the {@link ContentProvider} that is associated with this
     * client. If the {@link ContentProvider} is running in a different process then
     * null will be returned. This can be used if you know you are running in the same
     * process as a provider, and want to get direct access to its implementation details.
     *
     * @return If the associated {@link ContentProvider} is local, returns it.
     * Otherwise returns null.
     */
    CARAPI GetLocalContentProvider(
        /* [out] */ IContentProvider** contentProvider);

    CARAPI constructor(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IContentProvider* provider);

private:
    AutoPtr<IContentProvider> mContentProvider;
    AutoPtr<IContentResolver> mContentResolver;

};

#endif // __CCONTENTPROVIDERCLIENT_H__
