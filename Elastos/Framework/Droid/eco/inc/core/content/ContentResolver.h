
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

    static ModeToMode(
        /* [in] */ IUri* uri,
        /* [in] */ String mode,
        /* [in] */ Int32* result);

    virtual IContentProvider* ProbeIContentProvider() = 0;

    virtual CARAPI ReleaseProvider(
        /* [in] */ IContentProvider* icp,
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI AcquireProvider(
        /* [in] */ IContext* context,
        /* [in] */ const String& name,
        /* [out] */ IContentProvider** provider) = 0;

    virtual CARAPI AcquireExistingProvider(
        /* [in] */ IContext* context,
        /* [in] */ const String& name,
        /* [out] */ IContentProvider** provider) = 0;

    CARAPI OpenInputStream(
        /* [in] */ IUri* uri,
        /* [out] */ IInputStream** istream);

    /**
     * Synonym for {@link #openOutputStream(Uri, String)
     * openOutputStream(uri, "w")}.
     * @throws FileNotFoundException if the provided URI could not be opened.
     */
    CARAPI OpenOutputStream(
        /* [in] */ IUri* uri,
        /* [out] */ IOutputStream** ostream);

    /**
     * Open a stream on to the content associated with a content URI.  If there
     * is no data associated with the URI, FileNotFoundException is thrown.
     *
     * <h5>Accepts the following URI schemes:</h5>
     * <ul>
     * <li>content ({@link #SCHEME_CONTENT})</li>
     * <li>file ({@link #SCHEME_FILE})</li>
     * </ul>
     *
     * <p>See {@link #openAssetFileDescriptor(Uri, String)} for more information
     * on these schemes.
     *
     * @param uri The desired URI.
     * @param mode May be "w", "wa", "rw", or "rwt".
     * @return OutputStream
     * @throws FileNotFoundException if the provided URI could not be opened.
     * @see #openAssetFileDescriptor(Uri, String)
     */
    CARAPI OpenOutputStream2(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IOutputStream** ostream);

    /**
     * Open a raw file descriptor to access data under a "content:" URI.  This
     * is like {@link #openAssetFileDescriptor(Uri, String)}, but uses the
     * underlying {@link ContentProvider#openFile}
     * ContentProvider.openFile()} method, so will <em>not</em> work with
     * providers that return sub-sections of files.  If at all possible,
     * you should use {@link #openAssetFileDescriptor(Uri, String)}.  You
     * will receive a FileNotFoundException exception if the provider returns a
     * sub-section of a file.
     *
     * <h5>Accepts the following URI schemes:</h5>
     * <ul>
     * <li>content ({@link #SCHEME_CONTENT})</li>
     * <li>file ({@link #SCHEME_FILE})</li>
     * </ul>
     *
     * <p>See {@link #openAssetFileDescriptor(Uri, String)} for more information
     * on these schemes.
     *
     * @param uri The desired URI to open.
     * @param mode The file mode to use, as per {@link ContentProvider#openFile
     * ContentProvider.openFile}.
     * @return Returns a new ParcelFileDescriptor pointing to the file.  You
     * own this descriptor and are responsible for closing it when done.
     * @throws FileNotFoundException Throws FileNotFoundException of no
     * file exists under the URI or the mode is invalid.
     * @see #openAssetFileDescriptor(Uri, String)
     */
    CARAPI OpenFileDescriptor(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IParcelFileDescriptor** result);

    CARAPI OpenAssetFileDescriptor(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IAssetFileDescriptor** result);

    CARAPI GetResourceId(
        /* [in] */ IUri* uri,
        /* [out] */ IOpenResourceIdResult** result);

    /**
     * Inserts multiple rows into a table at the given URL.
     *
     * This function make no guarantees about the atomicity of the insertions.
     *
     * @param url The URL of the table to insert into.
     * @param values The initial values for the newly inserted rows. The key is the column name for
     *               the field. Passing null will create an empty row.
     * @return the number of newly created rows.
     */
    CARAPI BulkInsert(
        /* [in] */ IUri* url,
        /* [in] */ const ArrayOf<IContentValues*>& values,
        /* [out] */ Int32* result);

    CARAPI Delete(
        /* [in] */ IUri* uri,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
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
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ Int32* rowsAffected);

    /**
     * Returns the content provider for the given content URI.
     *
     * @param uri The URI to a content provider
     * @return The ContentProvider for the given URI, or null if no content provider is found.
     * @hide
     */
    CARAPI AcquireProvider2(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProvider** result);

    /**
     * Returns the content provider for the given content URI if the process
     * already has a reference on it.
     *
     * @param uri The URI to a content provider
     * @return The ContentProvider for the given URI, or null if no content provider is found.
     * @hide
     */
    CARAPI AcquireExistingProvider2(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProvider** result);

    /**
     * @hide
     */
    CARAPI AcquireProvider3(
        /* [in] */ String name,
        /* [out] */ IContentProvider** result);

    /**
     * Register an observer class that gets callbacks when data identified by a
     * given content URI changes.
     *
     * @param uri The URI to watch for changes. This can be a specific row URI, or a base URI
     * for a whole class of content.
     * @param notifyForDescendents If <code>true</code> changes to URIs beginning with <code>uri</code>
     * will also cause notifications to be sent. If <code>false</code> only changes to the exact URI
     * specified by <em>uri</em> will cause notifications to be sent. If true, than any URI values
     * at or below the specified URI will also trigger a match.
     * @param observer The object that receives callbacks when changes occur.
     * @see #unregisterContentObserver
     */
    CARAPI RegisterContentObserver(
        /* [in] */ IUri* uri,
        /* [in] */ Boolean notifyForDescendents,
        /* [in] */ IContentObserver* observer);

    /**
     * Unregisters a change observer.
     *
     * @param observer The previously registered observer that is no longer needed.
     * @see #registerContentObserver
     */
    CARAPI UnregisterContentObserver(
        /* [in] */ IContentObserver* observer);

    /**
     * Notify registered observers that a row was updated.
     * To register, call {@link #registerContentObserver(android.net.Uri , boolean, android.database.ContentObserver) registerContentObserver()}.
     * By default, CursorAdapter objects will get this notification.
     *
     * @param uri
     * @param observer The observer that originated the change, may be <code>null</null>
     */
    CARAPI NotifyChange(
        /* [in] */ IUri* uri,
        /* [in] */ IContentObserver* observer);

    /**
     * Notify registered observers that a row was updated.
     * To register, call {@link #registerContentObserver(android.net.Uri , boolean, android.database.ContentObserver) registerContentObserver()}.
     * By default, CursorAdapter objects will get this notification.
     *
     * @param uri
     * @param observer The observer that originated the change, may be <code>null</null>
     * @param syncToNetwork If true, attempt to sync the change to the network.
     */
    CARAPI NotifyChange2(
        /* [in] */ IUri* uri,
        /* [in] */ IContentObserver* observer,
        /* [in] */ Boolean syncToNetwork);

private:

    Void MaybeLogQueryToEventLog(
        Int64 durationMillis,
        IUri* uri,
        const ArrayOf<String>& projection,
        String selection,
        String sortOrder);

    Void MaybeLogUpdateToEventLog(
        Int64 durationMillis,
        IUri* uri,
        String operation,
        String selection);

protected:
    AutoPtr<IContext> mContext;
};

#endif //__CONTENTRESOLVER_H__
