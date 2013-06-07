
#include "content/ContentResolver.h"
#ifdef _FRAMEWORK_CORE
#include "content/CCursorWrapperInner.h"
#include "os/CParcelFileDescriptor.h"
#include "os/CParcelFileDescriptorHelper.h"
#include "content/CAssetFileDescriptor.h"
#include "content/COpenResourceIdResult.h"
#elif defined(_FRAMEWORK_SERVER)
#include "Elastos.Framework.Core.h"
#endif
#include "app/ActivityManagerNative.h"
#include "os/SystemClock.h"
#include "text/TextUtils.h"

const char* ContentResolver::SCHEME_CONTENT  = "content";
const char* ContentResolver::SCHEME_ELASTOS_RESOURCE  = "elastos.resource";
const char* ContentResolver::SCHEME_FILE  = "file";

const CString ContentResolver::TAG = "ContentResolver";

// Always log queries which take 500ms+; shorter queries are
// sampled accordingly.
const Int32 ContentResolver_SLOW_THRESHOLD_MILLIS = 500;

/** @hide */
ECode ContentResolver::ModeToMode(IUri* uri, String mode, Int32* result)
{
    assert(result);

    Int32 modeBits;

    if (mode.Equals("r")) {
        modeBits = ParcelFileDescriptor_MODE_READ_ONLY;
    } else if (mode.Equals("w") || mode.Equals("wt")) {
        modeBits = ParcelFileDescriptor_MODE_WRITE_ONLY
                | ParcelFileDescriptor_MODE_CREATE
                | ParcelFileDescriptor_MODE_TRUNCATE;
    } else if (mode.Equals("wa")) {
        modeBits = ParcelFileDescriptor_MODE_WRITE_ONLY
                | ParcelFileDescriptor_MODE_CREATE
                | ParcelFileDescriptor_MODE_APPEND;
    } else if (mode.Equals("rw")) {
        modeBits = ParcelFileDescriptor_MODE_READ_WRITE
                | ParcelFileDescriptor_MODE_CREATE;
    } else if (mode.Equals("rwt")) {
        modeBits = ParcelFileDescriptor_MODE_READ_WRITE
                | ParcelFileDescriptor_MODE_CREATE
                | ParcelFileDescriptor_MODE_TRUNCATE;
    } else {
        // throw new FileNotFoundException("Bad mode for " + uri + ": " + mode);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    *result = modeBits;

    return NOERROR;
}

#if 0
class CParcelFileDescriptorInner : public CParcelFileDescriptor
{
private:
    static const CString TAG;

public:

    CParcelFileDescriptorInner(
        IContentResolver* contentResolver,
        IParcelFileDescriptor* pfd,
        IContentProvider* icp);

    virtual ~CParcelFileDescriptorInner();

public:
    CARAPI Close();

private:
    AutoPtr<IContentResolver> mContentResolver;
    AutoPtr<IContentProvider> mContentProvider;
    Boolean mReleaseProviderFlag;
};

const CString CParcelFileDescriptorInner::TAG = "ParcelFileDescriptorInner";

CParcelFileDescriptorInner::CParcelFileDescriptorInner(
    IContentResolver* contentResolver,
    IParcelFileDescriptor* pfd,
    IContentProvider* icp)
    : mReleaseProviderFlag(FALSE)
{
    mContentResolver = contentResolver;
    constructor(pfd);
    mContentProvider = icp;
}

CParcelFileDescriptorInner::~CParcelFileDescriptorInner()
{
    if (!mReleaseProviderFlag) {
        Close();
    }
}

ECode CParcelFileDescriptorInner::Close()
{
    if (!mReleaseProviderFlag) {
        CParcelFileDescriptor::Close();
        Boolean result = FALSE;
        FAIL_RETURN(mContentResolver->ReleaseProvider(
            mContentProvider, &result));
        mReleaseProviderFlag = TRUE;
    }

    return NOERROR;
}
#endif

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
ECode ContentResolver::AcquireProvider2(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProvider** provider)
{
    if (provider == NULL) return E_INVALID_ARGUMENT;

    String scheme;
    uri->GetScheme(&scheme);
    if (String(ContentResolver_SCHEME_CONTENT).Compare(scheme)) {
        *provider = NULL;
        return E_DOES_NOT_EXIST;
    }

    String auth;
    uri->GetAuthority(&auth);
    if (!auth.IsNull()) {
        ECode ec = AcquireProvider(mContext, auth, provider);
        return ec;
    }
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
ECode ContentResolver::AcquireExistingProvider2(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProvider** provider)
{
    if (provider == NULL) return E_INVALID_ARGUMENT;

    String scheme;
    uri->GetScheme(&scheme);
    if (String(ContentResolver_SCHEME_CONTENT).Compare(scheme)) {
        *provider = NULL;
        return E_DOES_NOT_EXIST;
    }

    String auth;
    uri->GetAuthority(&auth);
    if (!auth.IsNull()) {
        ECode ec = AcquireExistingProvider(mContext, auth, provider);
        return ec;
    }
    *provider = NULL;
    return E_DOES_NOT_EXIST;
}

/**
 * @hide
 */
ECode ContentResolver::AcquireProvider3(
    /* [in] */ String name,
    /* [out] */ IContentProvider** result)
{
    VALIDATE_NOT_NULL(result);

    if (name.IsNull()) {
        *result = NULL;
        return NOERROR;
    }

    return AcquireProvider(mContext, name, result);
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
    VALIDATE_NOT_NULL(istream);

    String scheme;
    FAIL_RETURN(uri->GetScheme(&scheme));

    if (scheme.Equals(SCHEME_ELASTOS_RESOURCE)) {
        // Note: left here to avoid breaking compatibility.  May be removed
        // with sufficient testing.
        AutoPtr<IOpenResourceIdResult> r;
        FAIL_RETURN(GetResourceId(uri, (IOpenResourceIdResult**)&r));

        // try {
            AutoPtr<IResources> resources;
            FAIL_RETURN(r->GetResources((IResources**)&resources));

            Int32 id;
            FAIL_RETURN(r->GetResourceId(&id));

            return resources->OpenRawResource(id, istream);
        // } catch (Resources.NotFoundException ex) {
        //     throw new FileNotFoundException("Resource does not exist: " + uri);
        // }
    } else if (scheme.Equals(SCHEME_FILE)) {
        // Note: left here to avoid breaking compatibility.  May be removed
        // with sufficient testing.
        String path;
        FAIL_RETURN(uri->GetPath(&path));

        return CFileInputStream::New(path, (IFileInputStream**)&istream);
    } else {
        AutoPtr<IAssetFileDescriptor> fd;
        FAIL_RETURN(OpenAssetFileDescriptor(
            uri, String("r"), (IAssetFileDescriptor**)&fd));
        // try {
            if (fd == NULL) {
                *istream = NULL;
                return NOERROR;
            }
            else {
                // TODO: ALEX
                // return fd->CreateInputStream(istream);
                return E_NOT_IMPLEMENTED;
            }
        // } catch (IOException e) {
        //     throw new FileNotFoundException("Unable to create stream");
        // }
    }
}

ECode ContentResolver::OpenOutputStream(
    /* [in] */ IUri* uri,
    /* [out] */ IOutputStream** ostream)
{
    return OpenOutputStream2(uri, String("w"), ostream);
}

ECode ContentResolver::OpenOutputStream2(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IOutputStream** ostream)
{
    // TODO: ALEX need IAssetFileDescriptor::CreateOutputStream
    return E_NOT_IMPLEMENTED;
#if 0
    VALIDATE_NOT_NULL(ostream);

    AutoPtr<IAssetFileDescriptor> fd;
    FAIL_RETURN(OpenAssetFileDescriptor(
        uri, mode, (IAssetFileDescriptor**)&fd));
    // try {
    return (fd != NULL) ? fd->CreateOutputStream() : NULL;
    // } catch (IOException e) {
    //     throw new FileNotFoundException("Unable to create stream");
    // }
#endif
}

ECode ContentResolver::OpenFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** result)
{
    // TODO: ALEX need IAssetFileDescriptor::GetDeclaredLength
    return E_NOT_IMPLEMENTED;
#if 0
    VALIDATE_NOT_NULL(result);

    AutoPtr<IAssetFileDescriptor> afd;
    FAIL_RETURN(OpenAssetFileDescriptor(
        uri, mode, (IAssetFileDescriptor**)&afd));

    if (afd == NULL) {
        *result = NULL;
        return NOERROR;
    }

    if (afd->GetDeclaredLength() < 0) {
        // This is a full file!
        return afd->GetParcelFileDescriptor(&result);
    }

    // Client can't handle a sub-section of a file, so close what
    // we got and bail with an exception.
    // try {
    afd->Close();
    // } catch (IOException e) {
    // }

    // throw new FileNotFoundException("Not a whole file");
    return E_FILE_NOT_FOUND_EXCEPTION;
#endif
}

ECode ContentResolver::OpenAssetFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** result)
{
    VALIDATE_NOT_NULL(result);

    String scheme;
    FAIL_RETURN(uri->GetScheme(&scheme));

    if (scheme.Equals(SCHEME_ELASTOS_RESOURCE)) {
        if (!mode.Equals("r")) {
            // throw new FileNotFoundException("Can't write resources: " + uri);
            return E_FILE_NOT_FOUND_EXCEPTION;
        }

        AutoPtr<IOpenResourceIdResult> r;
        FAIL_RETURN(GetResourceId(uri, (IOpenResourceIdResult**)&r));

        // try {
            AutoPtr<IResources> resources;
            FAIL_RETURN(r->GetResources((IResources**)&resources));

            Int32 id;
            FAIL_RETURN(r->GetResourceId(&id));

            return resources->OpenRawResourceFd(id, result);
        // } catch (Resources.NotFoundException ex) {
        //     throw new FileNotFoundException("Resource does not exist: " + uri);
        // }
    } else if (scheme.Equals(SCHEME_FILE)) {
        AutoPtr<IParcelFileDescriptorHelper> helper;
        AutoPtr<IFile> file;
        AutoPtr<IParcelFileDescriptor> pfd;

        FAIL_RETURN(CParcelFileDescriptorHelper::AcquireSingleton(
            (IParcelFileDescriptorHelper**)&helper));

        String path;
        FAIL_RETURN(uri->GetPath(&path));

        FAIL_RETURN(CFile::New(path, (IFile**)&file));

        Int32 nMode;
        FAIL_RETURN(ModeToMode(uri, mode, &nMode));

        FAIL_RETURN(helper->Open(file, nMode, (IParcelFileDescriptor**)&pfd));

        return CAssetFileDescriptor::New(pfd, 0, -1, result);
    } else {
        // TODO: ALEX we need IAssetFileDescriptor::GetParcelFileDescriptor
        return E_NOT_IMPLEMENTED;
#if 0
        AutoPtr<IContentProvider> provider;
        FAIL_RETURN(AcquireProvider2(uri, (IContentProvider**)&provider));
        if (provider == NULL) {
            // throw new FileNotFoundException("No content provider: " + uri);
            return E_FILE_NOT_FOUND_EXCEPTION;
        }
        // try {
#define FAIL_RETURN_RELEASE_PROVIDER(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) { \
            ReleaseProvider(provider); \
            return ec; \
        } \
    } while(0);

            AutoPtr<IAssetFileDescriptor> fd;
            FAIL_RETURN_RELEASE_PROVIDER(provider->OpenAssetFile(uri, mode,
                (IAssetFileDescriptor**)&fd));
            if (fd == NULL) {
                ReleaseProvider(provider);
                *result = NULL;
                return NOERROR;
            }

            AutoPtr<IParcelFileDescriptor> pfd2;
            FAIL_RETURN_RELEASE_PROVIDER(fd=>GetParcelFileDescriptor(
                (IParcelFileDescriptor**)&pfd2));

            AutoPtr<IParcelFileDescriptor> pfd;
            pfd = new CParcelFileDescriptorInner(
                ProbeIContentProvider(), pfd2, provider);
            if (pfd == NULL) {
                ReleaseProvider(provider);
                return E_OUT_OF_MEMORY;
            }

            Int64 startOffset, length;
            FAIL_RETURN_RELEASE_PROVIDER(fd->GetStartOffset(&startOffset));
            FAIL_RETURN_RELEASE_PROVIDER(fd->GetDeclaredLength(&length));

            FAIL_RETURN_RELEASE_PROVIDER(CAssetFileDescriptor::New(
                pfd, startOffset, length, result));

            return NOERROR;
        // } catch (RemoteException e) {
        //     releaseProvider(provider);
        //     throw new FileNotFoundException("Dead content provider: " + uri);
        // } catch (FileNotFoundException e) {
        //     releaseProvider(provider);
        //     throw e;
        // } catch (RuntimeException e) {
        //     releaseProvider(provider);
        //     throw e;
        // }
#endif
    }
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
    VALIDATE_NOT_NULL(result);

    String authority;
    FAIL_RETURN(uri->GetAuthority(&authority));

    AutoPtr<IResources> r;
    AutoPtr<ICharSequence> authority2;
    FAIL_RETURN(CStringWrapper::New(authority, (ICharSequence**)&authority2));

    if (TextUtils::IsEmpty(authority2)) {
        // throw new FileNotFoundException("No authority: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    } else {
        // try {
            // TODO: ALEX need getResourcesForApplication
            // r = mContext.getPackageManager().getResourcesForApplication(authority);
            return E_NOT_IMPLEMENTED;
        // } catch (NameNotFoundException ex) {
        //     throw new FileNotFoundException("No package found for authority: " + uri);
        // }
    }

    ArrayOf<String>* path;
    FAIL_RETURN(uri->GetPathSegments(&path));
    if (path == NULL) {
        // throw new FileNotFoundException("No path: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    Int32 len = path->GetLength();
    Int32 id;
    if (len == 1) {
        // try {
            id = (*path)[0].ToInt32(10);
        // } catch (NumberFormatException e) {
        //     throw new FileNotFoundException("Single path segment is not a resource ID: " + uri);
        // }
    } else if (len == 2) {
        FAIL_RETURN(r->GetIdentifier((*path)[1], (*path)[0], authority, &id));
    } else {
        // throw new FileNotFoundException("More than two path segments: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    if (id == 0) {
        // throw new FileNotFoundException("No resource found for: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    AutoPtr<IOpenResourceIdResult> res;
    FAIL_RETURN(COpenResourceIdResult::New((IOpenResourceIdResult**)&res));

    res->SetResources(r);
    res->SetResourceId(id);

    *result = res;
    res->AddRef();

    return NOERROR;
}

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
ECode ContentResolver::BulkInsert(
    /* [in] */ IUri* url,
    /* [in] */ const ArrayOf<IContentValues*>& values,
    /* [out] */ Int32* result)
{
#if 0
    VALIDATE_NOT_NULL(result);

    AutoPtr<IContentProvider> provider;
    FAIL_RETURN(AcquireProvider2(uri, (IContentProvider**)&provider));

    if (provider == NULL) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int32 rowsCreated;
    FAIL_RETURN(provider->BulkInsert(url, values, &rowsCreated));
    Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
    MaybeLogUpdateToEventLog(durationMillis, url, String("bulkinsert"), NULL /* where */);
    *result = rowsCreated;
    ReleaseProvider(provider, NULL);

    return NOERROR;
    // } catch (RemoteException e) {
    //     return 0;
    // } finally {
    //     releaseProvider(provider);
    // }
#endif
    // TODO: ALEX need IContentProvider::BulkInsert
    return E_NOT_IMPLEMENTED;
}

ECode ContentResolver::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);

    AutoPtr<IContentProvider> provider;
    FAIL_RETURN(AcquireProvider2(uri, (IContentProvider**)&provider));

    if (provider == NULL) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int32 rowsDeleted;
    FAIL_RETURN(provider->Delete(uri, selection, selectionArgs, &rowsDeleted));
    Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
    MaybeLogUpdateToEventLog(durationMillis, uri, String("delete"), selection);
    *rowsAffected = rowsDeleted;
    ReleaseProvider(provider, NULL);

    return NOERROR;
    // } catch (RemoteException e) {
    //     return -1;
    // } finally {
    //     releaseProvider(provider);
    // }
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
    AcquireExistingProvider2(uri, (IContentProvider**)&provider);
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
        *type = NULL;
        return E_DOES_NOT_EXIST;
    }

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
    VALIDATE_NOT_NULL(insertedItemUri);

    AutoPtr<IContentProvider> provider;
    FAIL_RETURN(AcquireProvider2(uri, (IContentProvider**)&provider));

    if (provider == NULL) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    AutoPtr<IUri> createdRow;
    FAIL_RETURN(provider->Insert(uri, values, (IUri**)&createdRow));
    Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
    MaybeLogUpdateToEventLog(durationMillis, uri, String("insert"), String(NULL) /* where */);
    *insertedItemUri = createdRow;
    createdRow->AddRef();
    ReleaseProvider(provider, NULL);

    return NOERROR;
    // } catch (RemoteException e) {
    //     return null;
    // } finally {
    //     releaseProvider(provider);
    // }
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
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    if (cursor == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IContentProvider> provider;
    ECode ec = AcquireProvider2(uri, (IContentProvider**)&provider);
    if (FAILED(ec) || provider == NULL) {
        *cursor = NULL;
        return ec;
    }

//    long startTime = SystemClock.uptimeMillis();
    AutoPtr<ICursor> qCursor;
    ec = provider->Query(uri, projection, selection,
                    selectionArgs, sortOrder, (ICursor**)&qCursor);
    if (FAILED(ec) || qCursor == NULL) {
        ReleaseProvider(provider, NULL);
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
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);

    AutoPtr<IContentProvider> provider;
    FAIL_RETURN(AcquireProvider2(uri, (IContentProvider**)&provider));

    if (provider == NULL) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int32 rowsUpdated;
    FAIL_RETURN(provider->Update(
        uri, values, selection, selectionArgs, &rowsUpdated));
    Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
    MaybeLogUpdateToEventLog(durationMillis, uri, String("update"), selection);
    *rowsAffected = rowsUpdated;
    ReleaseProvider(provider, NULL);

    return NOERROR;
    // } catch (RemoteException e) {
    //     return -1;
    // } finally {
    //     releaseProvider(provider);
    // }
}

ECode ContentResolver::RegisterContentObserverEx(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendents,
    /* [in] */ ILocalContentObserver* observer)
{
    // try {
    if(observer == NULL){
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IContentObserver> contentObserver;
    FAIL_RETURN(observer->GetContentObserver((IContentObserver**)&contentObserver));

    ECode ec = GetContentService()->RegisterContentObserver(uri, notifyForDescendents, contentObserver.Get());
    // } catch (RemoteException e) {
    // }
    return ec;
}

ECode ContentResolver::RegisterContentObserver(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendents,
    /* [in] */ IContentObserver* observer)
{
    // try {
    if(observer == NULL){
        return E_INVALID_ARGUMENT;
    }

    ECode ec = GetContentService()->RegisterContentObserver(uri, notifyForDescendents, observer);
    // } catch (RemoteException e) {
    // }
    return ec;
}

/**
 *
 */
ECode ContentResolver::UnregisterContentObserverEx(
    /* [in] */ ILocalContentObserver* observer)
{
    // try {
    if(observer == NULL){
        return E_INVALID_ARGUMENT;
    }

     AutoPtr<IContentObserver> contentObserver;
     FAIL_RETURN(observer->ReleaseContentObserver((IContentObserver**)&contentObserver));

     if (contentObserver != NULL) {
         return GetContentService()->UnregisterContentObserver(contentObserver);
     }
    // } catch (RemoteException e) {
    // }

    return NOERROR;
}

ECode ContentResolver::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    // try {
    if(observer == NULL){
        return E_INVALID_ARGUMENT;
    }

     return GetContentService()->UnregisterContentObserver(observer);
}

ECode ContentResolver::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ ILocalContentObserver* observer)
{
    return NotifyChange2(uri, observer, TRUE /* sync to network */);
}

ECode ContentResolver::NotifyChange2(
    /* [in] */ IUri* uri,
    /* [in] */ ILocalContentObserver* observer,
    /* [in] */ Boolean syncToNetwork)
{
    // try {
    AutoPtr<IContentObserver> contentObserver = NULL;
    Boolean deliverSelfNotification = FALSE;

    if(observer != NULL){
        FAIL_RETURN(observer->GetContentObserver((IContentObserver**)&contentObserver));
        FAIL_RETURN(observer->DeliverSelfNotifications(&deliverSelfNotification));
    }

    return GetContentService()->NotifyChange(
        uri, contentObserver,
        (observer != NULL && deliverSelfNotification),
        syncToNetwork);
    // } catch (RemoteException e) {
    // }
}

AutoPtr<IContentService> ContentResolver::sContentService;

IContentService* ContentResolver::GetContentService()
{
    if (sContentService != NULL) {
        return sContentService;
    }

    AutoPtr<IServiceManager> serviceManager;

    if (FAILED(Elastos::GetServiceManager(
        (IServiceManager**)&serviceManager))) {
        return NULL;
    }

    if (FAILED(serviceManager->GetService(
        String(ContentResolver_CONTENT_SERVICE_NAME),
	    (IInterface**)(IContentService**)&sContentService))) {
	    return NULL;
    }

    return (IContentService*)sContentService;
}

Void MaybeLogQueryToEventLog(
    Int64 durationMillis,
    IUri* uri,
    const ArrayOf<String>& projection,
    String selection,
    String sortOrder)
{
    // TODO: ALEX
}

Void ContentResolver::MaybeLogUpdateToEventLog(
    Int64 durationMillis,
    IUri* uri,
    String operation,
    String selection)
{
    // TODO: ALEX
}

