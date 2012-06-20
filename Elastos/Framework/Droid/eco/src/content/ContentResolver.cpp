
#include "content/ContentResolver.h"
#include "content/CCursorWrapperInner.h"
#include "app/ActivityManagerNative.h"

const String ContentResolver::SCHEME_CONTENT  = "content";
const String ContentResolver::SCHEME_ELASTOS_RESOURCE  = "elastos.resource";
const String ContentResolver::SCHEME_FILE  = "file";

ContentResolver::ContentResolver(
    /* [in] */ IContext* context) :
    mContext(context)
{}

/**
 * Returns the content provider for the given content URI..
 *
 * @param uri The URI to a content provider
 * @return The ContentProvider for the given URI, or null if no content provider is found.
 * @hide
 */
ECode ContentResolver::AcquireProvider(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProvider** provider)
{
    if (provider == NULL) return E_INVALID_ARGUMENT;

    String scheme;
    uri->GetScheme(&scheme);
    if (String(ContentResolver_SCHEME_CONTENT).Compare(scheme)) {
        String::Free(scheme);
        *provider = NULL;
        return E_DOES_NOT_EXIST;
    }
    String::Free(scheme);

    String auth;
    uri->GetAuthority(&auth);
    if (!auth.IsNull()) {
        ECode ec = AcquireProvider(mContext, auth, provider);
        String::Free(auth);
        return ec;
    }
    String::Free(auth);
    *provider = NULL;
    return E_DOES_NOT_EXIST;
}

/**
 * Returns the content provider for the given content URI if the process
 * already has a reference on it.
 *
 * @param uri The URI to a content provider
 * @return The ContentProvider for the given URI, or null if no content provider is found.
 * @hide
 */
ECode ContentResolver::AcquireExistingProvider(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProvider** provider)
{
    if (provider == NULL) return E_INVALID_ARGUMENT;

    String scheme;
    uri->GetScheme(&scheme);
    if (String(ContentResolver_SCHEME_CONTENT).Compare(scheme)) {
        String::Free(scheme);
        *provider = NULL;
        return E_DOES_NOT_EXIST;
    }
    String::Free(scheme);

    String auth;
    uri->GetAuthority(&auth);
    if (!auth.IsNull()) {
        ECode ec = AcquireExistingProvider(mContext, auth, provider);
        String::Free(auth);
        return ec;
    }
    String::Free(auth);
    *provider = NULL;
    return E_DOES_NOT_EXIST;
}

/**
 * Open a stream on to the content associated with a content URI.  If there
 * is no data associated with the URI, FileNotFoundException is thrown.
 *
 * <h5>Accepts the following URI schemes:</h5>
 * <ul>
 * <li>content ({@link #SCHEME_CONTENT})</li>
 * <li>android.resource ({@link #SCHEME_ANDROID_RESOURCE})</li>
 * <li>file ({@link #SCHEME_FILE})</li>
 * </ul>
 *
 * <p>See {@link #openAssetFileDescriptor(Uri, String)} for more information
 * on these schemes.
 *
 * @param uri The desired URI.
 * @return InputStream
 * @throws FileNotFoundException if the provided URI could not be opened.
 * @see #openAssetFileDescriptor(Uri, String)
 */
ECode ContentResolver::OpenInputStream(
    /* [in] */ IUri* uri,
    /* [out] */ IInputStream** istream)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Resolves an android.resource URI to a {@link Resources} and a resource id.
 *
 * @hide
 */
ECode ContentResolver::GetResourceId(
    /* [in] */ IUri* uri,
    /* [out] */ IOpenResourceIdResult** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentResolver::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ String selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Return the MIME type of the given content URL.
 *
 * @param url A Uri identifying content (either a list or specific type),
 * using the content:// scheme.
 * @return A MIME type for the content, or null if the URL is invalid or the type is unknown
 */
ECode ContentResolver::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    if (type == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IContentProvider> provider;
    AcquireExistingProvider(uri, (IContentProvider**)&provider);
    if (provider != NULL) {
//        try {
        ECode ec = provider->GetType(uri, type);

//        } catch (RemoteException e) {
//            return null;
//        } catch (java.lang.Exception e) {
//            return null;
//        } finally {
//            releaseProvider(provider);
//        }
        return ec;
    }

    String scheme;
    uri->GetScheme(&scheme);
    if (String(ContentResolver_SCHEME_CONTENT).Compare(scheme)) {
        String::Free(scheme);
        *type = NULL;
        return E_DOES_NOT_EXIST;
    }
    String::Free(scheme);

//    try {
    AutoPtr<IActivityManager> am;
    ActivityManagerNative::GetDefault((IActivityManager**)&am);
    return am->GetProviderMimeType(uri, type);
//    } catch (RemoteException e) {
//        *type = NULL;
//        return E_DOES_NOT_EXIST;
//    }
}

ECode ContentResolver::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [out] */ IUri** insertedItemUri)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Query the given URI, returning a {@link Cursor} over the result set.
 *
 * @param uri The URI, using the content:// scheme, for the content to
 *         retrieve.
 * @param projection A list of which columns to return. Passing null will
 *         return all columns, which is inefficient.
 * @param selection A filter declaring which rows to return, formatted as an
 *         SQL WHERE clause (excluding the WHERE itself). Passing null will
 *         return all rows for the given URI.
 * @param selectionArgs You may include ?s in selection, which will be
 *         replaced by the values from selectionArgs, in the order that they
 *         appear in the selection. The values will be bound as Strings.
 * @param sortOrder How to order the rows, formatted as an SQL ORDER BY
 *         clause (excluding the ORDER BY itself). Passing null will use the
 *         default sort order, which may be unordered.
 * @return A Cursor object, which is positioned before the first entry, or null
 * @see Cursor
 */
ECode ContentResolver::Query(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String>& projection,
    /* [in] */ String selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ String sortOrder,
    /* [out] */ ICursor** cursor)
{
    if (cursor == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IContentProvider> provider;
    ECode ec = AcquireProvider(uri, (IContentProvider**)&provider);
    if (FAILED(ec) || provider == NULL) {
        *cursor = NULL;
        return ec;
    }

//    long startTime = SystemClock.uptimeMillis();
    AutoPtr<ICursor> qCursor;
    ec = provider->Query(uri, projection, selection,
                    selectionArgs, sortOrder, (ICursor**)&qCursor);
    if (FAILED(ec) || qCursor == NULL) {
        ReleaseProvider(provider);
        *cursor = NULL;
        return ec;
    }
//    // force query execution
//    qCursor.getCount();
//    long durationMillis = SystemClock.uptimeMillis() - startTime;
//    maybeLogQueryToEventLog(durationMillis, uri, projection, selection, sortOrder);
    // Wrap the cursor object into CursorWrapperInner object
    return CCursorWrapperInner::New(qCursor, provider, cursor);
}

ECode ContentResolver::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ String selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    return E_NOT_IMPLEMENTED;
}
