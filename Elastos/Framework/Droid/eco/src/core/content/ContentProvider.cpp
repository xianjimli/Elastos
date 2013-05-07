
#ifdef _FRAMEWORK_CORE
#include "content/ContentProvider.h"
#include "content/CContentValues.h"
#include "content/CAssetFileDescriptor.h"
#include "content/ContentResolver.h"
#include "database/CCursorToBulkCursorAdaptor.h"
#include "os/CParcelFileDescriptor.h"
#else
#include "ContentProvider.h"
#endif
//#include "os/Binder.h"

ContentProvider::ContentProvider()
{
}

ContentProvider::ContentProvider(
    /* [in] */ IContext* context,
    /* [in] */ String readPermission,
    /* [in] */ String writePermission,
    /* [in] */ ArrayOf<IPathPermission*>* pathPermissions)
{
    mContext = context;
    mReadPermission = readPermission;
    mWritePermission = writePermission;
    mPathPermissions = pathPermissions;
    mTransport = new ContentProvider::Transport(this);
}

ContentProvider::~ContentProvider()
{}

ECode ContentProvider::Initialize()
{
    return NOERROR;
}

PInterface ContentProvider::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IContentProvider*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    if (riid == EIID_IContentProvider) {
        return (IContentProvider *)this;
    }

    return NULL;
}

UInt32 ContentProvider::AddRef()
{
    Int32 nRef = atomic_inc(&mRef);
    return (UInt32)nRef;
}

UInt32 ContentProvider::Release()
{
    Int32 nRef;

    nRef = atomic_dec(&mRef);
    if (nRef == 0) {
        delete this;
    }
    return (UInt32)nRef;
}

ECode ContentProvider::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IContentProvider *)this) {
        *pIID = EIID_IContentProvider;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode ContentProvider::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * After being instantiated, this is called to tell the content provider
 * about itself.
 *
 * @param context The context this provider is running in
 * @param info Registered information about this content provider
 */
ECode ContentProvider::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IContentProviderInfo* providerInfo)
{
    /*
     * Only allow it to be set once, so after the content service gives
     * this to us clients can't change it.
     */
    if (mContext == NULL) {
        mContext = context;
//        mMyUid = Process.myUid();
        if (providerInfo != NULL) {
//            setReadPermission(info.readPermission);
//            setWritePermission(info.writePermission);
//            setPathPermissions(info.pathPermissions);
        }
        OnCreate();
    }
    return NOERROR;
}
/**
 * Given an IContentProvider, try to coerce it back to the real
 * ContentProvider object if it is running in the local process.  This can
 * be used if you know you are running in the same process as a provider,
 * and want to get direct access to its implementation details.  Most
 * clients should not nor have a reason to use it.
 *
 * @param abstractInterface The ContentProvider interface that is to be
 *              coerced.
 * @return If the IContentProvider is non-null and local, returns its actual
 * ContentProvider instance.  Otherwise returns null.
 * @hide
 */
ECode ContentProvider::CoerceToLocalContentProvider(
    /* [in] */ IContentProvider* abstractInterface,
    /* [out] */ ContentProvider** localProvider)
{
    if (abstractInterface->Probe(abstractInterface) != NULL) {
        return ((ContentProvider::Transport*)abstractInterface)->GetContentProvider(localProvider);
    }

    *localProvider = NULL;
    return NOERROR;
}

ECode ContentProvider::OnCreate()
{
    return NOERROR;
}

ContentProvider::Transport::Transport(ContentProvider* provider)
{
    mContentProvider = provider;
}

PInterface ContentProvider::Transport::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IContentProvider*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    if (riid == EIID_IContentProvider) {
        return (IContentProvider *)this;
    }

    return NULL;
}

UInt32 ContentProvider::Transport::AddRef()
{
    // Int32 nRef = atomic_inc(&mRef);
    // return (UInt32)nRef;

    return E_NOT_IMPLEMENTED;
}

UInt32 ContentProvider::Transport::Release()
{
    // Int32 nRef;

    // nRef = atomic_dec(&mRef);
    // if (nRef == 0) {
    //     delete this;
    // }
    // return (UInt32)nRef;

    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::Transport::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IContentProvider *)this) {
        *pIID = EIID_IContentProvider;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

/**
 * After being instantiated, this is called to tell the content provider
 * about itself.
 *
 * @param context The context this provider is running in
 * @param info Registered information about this content provider
 */
ECode ContentProvider::Transport::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IContentProviderInfo* providerInfo)
{
    /*
     * Only allow it to be set once, so after the content service gives
     * this to us clients can't change it.
     */
//     if (mContext == NULL) {
//         mContext = context;
// //        mMyUid = Process.myUid();
//         if (providerInfo != NULL) {
// //            setReadPermission(info.readPermission);
// //            setWritePermission(info.writePermission);
// //            setPathPermissions(info.pathPermissions);
//         }
//         OnCreate();
//     }
    return E_NOT_IMPLEMENTED;
}

/**
 * Binder object that deals with remoting.
 *
 * @hide
 */

ECode ContentProvider::Transport::GetContentProvider(
    /* [out] */ ContentProvider** provider)
{
     *provider = mContentProvider;
    return NOERROR;
}

    /**
     * Remote version of a query, which returns an IBulkCursor. The bulk
     * cursor should be wrapped with BulkCursorToCursorAdaptor before use.
     */
ECode ContentProvider::Transport::BulkQuery(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String> & projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ IContentObserver* observer,
    /* [in] */ ICursorWindow* window,
    /* [out] */ IBulkCursor** bulkCursor)
{
    EnforceReadPermission(uri);

    ICursor* cursor;
    mContentProvider->Query(uri,
                        projection,
                        selection,
                        selectionArgs,
                        sortOrder,
                        &cursor);

    if (cursor == NULL) {
        *bulkCursor = NULL;
        return NOERROR;
    }

    Boolean hasWritePermission;
    HasWritePermission(uri, &hasWritePermission);

    return CCursorToBulkCursorAdaptor::New(
            cursor,
            observer,
            String("ContentProvider"),
            hasWritePermission,
            window,
            (ICursorToBulkCursorAdaptor**)bulkCursor);
}

ECode ContentProvider::Transport::Query(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String> & projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    EnforceReadPermission(uri);
    return mContentProvider->Query(
            uri,
            projection,
            selection,
            selectionArgs,
            sortOrder,
            cursor);
}

ECode ContentProvider::Transport::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* str)
{
    return mContentProvider->GetType(uri, str);
}


ECode ContentProvider::Transport::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ IUri** insertUri)
{
    EnforceWritePermission(uri);
    return mContentProvider->Insert(uri, initialValues, insertUri);
}

ECode ContentProvider::Transport::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<IContentValues*> & initialValues,
    /* [out] */ Int32* number)
{
    EnforceWritePermission(uri);
    return mContentProvider->BulkInsert(uri, initialValues, number);
}

ECode ContentProvider::Transport::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults)
{
    Boolean hasNext;
    Boolean isRead;
    Boolean isWrite;
    AutoPtr<IUri> uri;
    AutoPtr<IContentProviderOperation> operation;
    AutoPtr<IObjectEnumerator> ObjEnumerator;

    operations->GetObjectEnumerator((IObjectEnumerator**) &ObjEnumerator);

    while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
        ObjEnumerator->Current((IInterface**) &operation);
        operation->IsReadOperation(&isRead);
        operation->IsWriteOperation(&isWrite);
        operation->GetUri((IUri**) &uri);

        if (isRead) {
            EnforceReadPermission(uri);
        }

        if (isWrite) {
            EnforceWritePermission(uri);
        }
    }

    return mContentProvider->ApplyBatch(operations, providerResults);
}

ECode ContentProvider::Transport::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [out] */ Int32* number)
{
    EnforceWritePermission(uri);
    return mContentProvider->Delete(uri,
            selection,
            selectionArgs,
            number);
}

ECode ContentProvider::Transport::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [out] */ Int32* number)
{
    EnforceWritePermission(uri);
    return mContentProvider->Update(
            uri,
            values,
            selection,
            selectionArgs,
            number);
}

ECode ContentProvider::Transport::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    if (mode != NULL && mode.StartWith("rw", StringCase_Sensitive)) {
        EnforceWritePermission(uri);
    }
    else {
        EnforceReadPermission(uri);
    }
    return mContentProvider->OpenFile(uri, mode, fileDescriptor);
}

ECode ContentProvider::Transport::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    if (mode != NULL && mode.StartWith("rw", StringCase_Sensitive)) {
        EnforceWritePermission(uri);
    }
    else {
        EnforceReadPermission(uri);
    }

    return mContentProvider->OpenAssetFile(uri, mode, fileDescriptor);
}

/**
 * @hide
 */
ECode ContentProvider::Transport::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& request,
    /* [in] */ IBundle* args,
    /* [out] */ IBundle** bundle)
{
    return mContentProvider->Call(method, request, args, bundle);
}

ECode ContentProvider::Transport::EnforceReadPermission(
    /* [in] */ IUri* uri)
{
    const Int32 uid = 0;//Binder::GetCallingUid();
    if (uid == mContentProvider->mMyUid) {
        return NOERROR;
    }

    const Int32 pid = 0;//Binder::GetCallingPid();;
    IContext* context;
    String rperm;
    mContentProvider->GetContext(&context);
    mContentProvider->GetReadPermission(&rperm);

    Int32 permission;
    context->CheckPermission(rperm, pid, uid, &permission);

    if (mContentProvider->mExported && (rperm == NULL
            || permission == 0/*PackageManager.PERMISSION_GRANTED*/)) {
        return NOERROR;
    }

    ArrayOf<IPathPermission*>* pps;
    mContentProvider->GetPathPermissions(&pps);

    if (pps != NULL) {
        String path;
        uri->GetPath(&path);
        Int32 i = pps->GetLength();
        while (i > 0) {
            i--;
            IPathPermission* pp = pps->GetPayload()[i];
            String pprperm;
            pp->GetReadPermission(&pprperm);
            Boolean isMatch;
            pp->Match(path, &isMatch);
            if (pprperm != NULL && isMatch) {
                Int32 checkPermission;
                context->CheckPermission(pprperm, pid, uid, &checkPermission);
                if (checkPermission == 0/*PackageManager.PERMISSION_GRANTED*/) {
                    return NOERROR;
                }
            }
        }
    }

    Int32 result;
    context->CheckUriPermissionEx(uri, pid, uid,
            Intent_FLAG_GRANT_READ_URI_PERMISSION, &result);
    if (result == 0/*PackageManager.PERMISSION_GRANTED*/) {
        return NOERROR;
    }

    // String msg = "Permission Denial: reading "
    //         + String("ContentProvider")ContentProvider.this.getClass().getName()
    //         + " uri " + uri + " from pid=" + Binder.getCallingPid()
    //         + ", uid=" + Binder.getCallingUid()
    //         + " requires " + rperm;
    //throw new SecurityException(msg);
    return E_SECURITY_EXCEPTION;
}

ECode ContentProvider::Transport::HasWritePermission(
    /* [in] */ IUri* uri,
    /* [out] */ Boolean* result)
{
    const Int32 uid = 0;//Binder::GetCallingUid();
    if (uid == mContentProvider->mMyUid) {
        *result = TRUE;
        return NOERROR;
    }

    IContext* context;
    String wperm;
    mContentProvider->GetContext(&context);
    mContentProvider->GetWritePermission(&wperm);

    const Int32 pid = 0;//Binder::GetCallingPid();
    Int32 permission;
    context->CheckPermission(wperm, pid, uid, &permission);
    if (mContentProvider->mExported && (wperm == NULL
            || permission == 0/*PackageManager.PERMISSION_GRANTED*/)) {
        *result = TRUE;
        return NOERROR;
    }

    ArrayOf<IPathPermission*>* pps;
    mContentProvider->GetPathPermissions(&pps);
    if (pps != NULL) {
        String path;
        uri->GetPath(&path);
        Int32 i = pps->GetLength();
        while (i > 0) {
            i--;
            IPathPermission* pp = pps->GetPayload()[i];
            String ppwperm;
            pp->GetWritePermission(&ppwperm);
            Boolean isMatch;
            pp->Match(path, &isMatch);
            if (ppwperm != NULL && isMatch) {
                Int32 permission;
                context->CheckPermission(ppwperm, pid, uid, &permission);
                if (permission == 0/*PackageManager.PERMISSION_GRANTED*/) {
                    *result = TRUE;
                    return NOERROR;
                }
            }
        }
    }

    context->CheckUriPermissionEx(uri, pid, uid,
            Intent_FLAG_GRANT_WRITE_URI_PERMISSION, &permission);
    if (permission == 0/*PackageManager.PERMISSION_GRANTED*/) {
        *result = TRUE;
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

ECode ContentProvider::Transport::EnforceWritePermission(
    /* [in] */ IUri* uri)
{
    Boolean hasPermission;

    HasWritePermission(uri, &hasPermission);
    if (hasPermission) {
        return NOERROR;
    }

    // String msg = "Permission Denial: writing "
    //         + ContentProvider.this.getClass().getName()
    //         + " uri " + uri + " from pid=" + Binder.getCallingPid()
    //         + ", uid=" + Binder.getCallingUid()
    //         + " requires " + getWritePermission();
    // throw new SecurityException(msg);
    return E_SECURITY_EXCEPTION;
}


/**
 * Retrieves the Context this provider is running in.  Only available once
 * {@link #onCreate} has been called -- this will return null in the
 * constructor.
 */
ECode ContentProvider::GetContext(
    /* [out] */ IContext** context)
{
    *context = mContext;
    return NOERROR;
}

/**
 * Change the permission required to read data from the content
 * provider.  This is normally set for you from its manifest information
 * when the provider is first created.
 *
 * @param permission Name of the permission required for read-only access.
 */
ECode ContentProvider::SetReadPermission(
    /* [in] */ String permission)
{
    mReadPermission = permission;
    return NOERROR;
}

/**
 * Return the name of the permission required for read-only access to
 * this content provider.  This method can be called from multiple
 * threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 */
ECode ContentProvider::GetReadPermission(
    /* [out] */ String* str)
{
    *str = mReadPermission;
    return NOERROR;
}

/**
 * Change the permission required to read and write data in the content
 * provider.  This is normally set for you from its manifest information
 * when the provider is first created.
 *
 * @param permission Name of the permission required for read/write access.
 */
ECode ContentProvider::SetWritePermission(
    /* [in] */ String permission)
{
    mWritePermission = permission;
    return NOERROR;
}

/**
 * Return the name of the permission required for read/write access to
 * this content provider.  This method can be called from multiple
 * threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 */
ECode ContentProvider::GetWritePermission(
    /* [out] */ String* permission)
{
    *permission = mWritePermission;
    return NOERROR;
}

/**
 * Change the path-based permission required to read and/or write data in
 * the content provider.  This is normally set for you from its manifest
 * information when the provider is first created.
 *
 * @param permissions Array of path permission descriptions.
 */
ECode ContentProvider::SetPathPermissions(
    /* [in] */ ArrayOf<IPathPermission*>* permissions)
{
    mPathPermissions = permissions;
    return NOERROR;
}

/**
 * Return the path-based permissions required for read and/or write access to
 * this content provider.  This method can be called from multiple
 * threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 */
ECode ContentProvider::GetPathPermissions(
    /* [out] */ ArrayOf<IPathPermission*>** permissions)
{
    *permissions = mPathPermissions;
    return NOERROR;
}

/**
 * Implement this to initialize your content provider on startup.
 * This method is called for all registered content providers on the
 * application main thread at application launch time.  It must not perform
 * lengthy operations, or application startup will be delayed.
 *
 * <p>You should defer nontrivial initialization (such as opening,
 * upgrading, and scanning databases) until the content provider is used
 * (via {@link #query}, {@link #insert}, etc).  Deferred initialization
 * keeps application startup fast, avoids unnecessary work if the provider
 * turns out not to be needed, and stops database errors (such as a full
 * disk) from halting application launch.
 *
 * <p>If you use SQLite, {@link android.database.sqlite.SQLiteOpenHelper}
 * is a helpful utility class that makes it easy to manage databases,
 * and will automatically defer opening until first use.  If you do use
 * SQLiteOpenHelper, make sure to avoid calling
 * {@link android.database.sqlite.SQLiteOpenHelper#getReadableDatabase} or
 * {@link android.database.sqlite.SQLiteOpenHelper#getWritableDatabase}
 * from this method.  (Instead, override
 * {@link android.database.sqlite.SQLiteOpenHelper#onOpen} to initialize the
 * database when it is first opened.)
 *
 * @return true if the provider was successfully loaded, false otherwise
 */
// public abstract boolean onCreate();

/**
 * {@inheritDoc}
 * This method is always called on the application main thread, and must
 * not perform lengthy operations.
 *
 * <p>The default content provider implementation does nothing.
 * Override this method to take appropriate action.
 * (Content providers do not usually care about things like screen
 * orientation, but may want to know about locale changes.)
 */
ECode ContentProvider::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * {@inheritDoc}
 * This method is always called on the application main thread, and must
 * not perform lengthy operations.
 *
 * <p>The default content provider implementation does nothing.
 * Subclasses may override this method to take appropriate action.
 */
ECode ContentProvider::OnLowMemory()
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Implement this to handle query requests from clients.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 * <p>
 * Example client call:<p>
 * <pre>// Request a specific record.
 * Cursor managedCursor = managedQuery(
            ContentUris.withAppendedId(Contacts.People.CONTENT_URI, 2),
            projection,    // Which columns to return.
            null,          // WHERE clause.
            null,          // WHERE clause value substitution
            People.NAME + " ASC");   // Sort order.</pre>
 * Example implementation:<p>
 * <pre>// SQLiteQueryBuilder is a helper class that creates the
    // proper SQL syntax for us.
    SQLiteQueryBuilder qBuilder = new SQLiteQueryBuilder();

    // Set the table we're querying.
    qBuilder.setTables(DATABASE_TABLE_NAME);

    // If the query ends in a specific record number, we're
    // being asked for a specific record, so set the
    // WHERE clause in our query.
    if((URI_MATCHER.match(uri)) == SPECIFIC_MESSAGE){
        qBuilder.appendWhere("_id=" + uri.getPathLeafId());
    }

    // Make the query.
    Cursor c = qBuilder.query(mDb,
            projection,
            selection,
            selectionArgs,
            groupBy,
            having,
            sortOrder);
    c.setNotificationUri(getContext().getContentResolver(), uri);
    return c;</pre>
 *
 * @param uri The URI to query. This will be the full URI sent by the client;
 *      if the client is requesting a specific record, the URI will end in a record number
 *      that the implementation should parse and add to a WHERE or HAVING clause, specifying
 *      that _id value.
 * @param projection The list of columns to put into the cursor. If
 *      null all columns are included.
 * @param selection A selection criteria to apply when filtering rows.
 *      If null then all rows are included.
 * @param selectionArgs You may include ?s in selection, which will be replaced by
 *      the values from selectionArgs, in order that they appear in the selection.
 *      The values will be bound as Strings.
 * @param sortOrder How the rows in the cursor should be sorted.
 *      If null then the provider is free to define the sort order.
 * @return a Cursor or null.
 */
// public abstract Cursor query(Uri uri, String[] projection,
//         String selection, String[] selectionArgs, String sortOrder);

/**
 * Implement this to handle requests for the MIME type of the data at the
 * given URI.  The returned MIME type should start with
 * <code>vnd.android.cursor.item</code> for a single record,
 * or <code>vnd.android.cursor.dir/</code> for multiple items.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 *
 * <p>Note that there are no permissions needed for an application to
 * access this information; if your content provider requires read and/or
 * write permissions, or is not exported, all applications can still call
 * this method regardless of their access permissions.  This allows them
 * to retrieve the MIME type for a URI when dispatching intents.
 *
 * @param uri the URI to query.
 * @return a MIME type string, or null if there is no type.
 */
// public abstract String getType(Uri uri);

/**
 * Implement this to handle requests to insert a new row.
 * As a courtesy, call {@link ContentResolver#notifyChange(android.net.Uri ,android.database.ContentObserver) notifyChange()}
 * after inserting.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 * @param uri The content:// URI of the insertion request.
 * @param values A set of column_name/value pairs to add to the database.
 * @return The URI for the newly inserted item.
 */
// public abstract Uri insert(Uri uri, ContentValues values);

/**
 * Override this to handle requests to insert a set of new rows, or the
 * default implementation will iterate over the values and call
 * {@link #insert} on each of them.
 * As a courtesy, call {@link ContentResolver#notifyChange(android.net.Uri ,android.database.ContentObserver) notifyChange()}
 * after inserting.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 *
 * @param uri The content:// URI of the insertion request.
 * @param values An array of sets of column_name/value pairs to add to the database.
 * @return The number of values that were inserted.
 */
ECode ContentProvider::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<IContentValues*> & values,
    /* [out] */ Int32* number)
{
    Int32 numValues = values.GetLength();
    for (Int32 i = 0; i < numValues; i++) {
        IUri* insertUri;
        Insert(uri, values[i], &insertUri);
    }
    *number = numValues;
    return NOERROR;
}

/**
 * Implement this to handle requests to delete one or more rows.
 * The implementation should apply the selection clause when performing
 * deletion, allowing the operation to affect multiple rows in a directory.
 * As a courtesy, call {@link ContentResolver#notifyChange(android.net.Uri ,android.database.ContentObserver) notifyDelete()}
 * after deleting.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 *
 * <p>The implementation is responsible for parsing out a row ID at the end
 * of the URI, if a specific row is being deleted. That is, the client would
 * pass in <code>content://contacts/people/22</code> and the implementation is
 * responsible for parsing the record number (22) when creating a SQL statement.
 *
 * @param uri The full URI to query, including a row ID (if a specific record is requested).
 * @param selection An optional restriction to apply to rows when deleting.
 * @return The number of rows affected.
 * @throws SQLException
 */
// public abstract int delete(Uri uri, String selection, String[] selectionArgs);

/**
 * Implement this to handle requests to update one or more rows.
 * The implementation should update all rows matching the selection
 * to set the columns according to the provided values map.
 * As a courtesy, call {@link ContentResolver#notifyChange(android.net.Uri ,android.database.ContentObserver) notifyChange()}
 * after updating.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 *
 * @param uri The URI to query. This can potentially have a record ID if this
 * is an update request for a specific record.
 * @param values A Bundle mapping from column names to new column values (NULL is a
 *               valid value).
 * @param selection An optional filter to match rows to update.
 * @return the number of rows affected.
 */
// public abstract int update(Uri uri, ContentValues values, String selection,
//         String[] selectionArgs);

/**
 * Override this to handle requests to open a file blob.
 * The default implementation always throws {@link FileNotFoundException}.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 *
 * <p>This method returns a ParcelFileDescriptor, which is returned directly
 * to the caller.  This way large data (such as images and documents) can be
 * returned without copying the content.
 *
 * <p>The returned ParcelFileDescriptor is owned by the caller, so it is
 * their responsibility to close it when done.  That is, the implementation
 * of this method should create a new ParcelFileDescriptor for each call.
 *
 * @param uri The URI whose file is to be opened.
 * @param mode Access mode for the file.  May be "r" for read-only access,
 * "rw" for read and write access, or "rwt" for read and write access
 * that truncates any existing file.
 *
 * @return Returns a new ParcelFileDescriptor which you can use to access
 * the file.
 *
 * @throws FileNotFoundException Throws FileNotFoundException if there is
 * no file associated with the given URI or the mode is invalid.
 * @throws SecurityException Throws SecurityException if the caller does
 * not have permission to access the file.
 *
 * @see #openAssetFile(Uri, String)
 * @see #openFileHelper(Uri, String)
 */
ECode ContentProvider::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ String mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    // throw new FileNotFoundException("No files supported by provider at "
    //         + uri);
    return E_FILE_NOT_FOUND_EXCEPTION;
}

/**
 * This is like {@link #openFile}, but can be implemented by providers
 * that need to be able to return sub-sections of files, often assets
 * inside of their .apk.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 *
 * <p>If you implement this, your clients must be able to deal with such
 * file slices, either directly with
 * {@link ContentResolver#openAssetFileDescriptor}, or by using the higher-level
 * {@link ContentResolver#openInputStream ContentResolver.openInputStream}
 * or {@link ContentResolver#openOutputStream ContentResolver.openOutputStream}
 * methods.
 *
 * <p class="note">If you are implementing this to return a full file, you
 * should create the AssetFileDescriptor with
 * {@link AssetFileDescriptor#UNKNOWN_LENGTH} to be compatible with
 * applications that can not handle sub-sections of files.</p>
 *
 * @param uri The URI whose file is to be opened.
 * @param mode Access mode for the file.  May be "r" for read-only access,
 * "w" for write-only access (erasing whatever data is currently in
 * the file), "wa" for write-only access to append to any existing data,
 * "rw" for read and write access on any existing data, and "rwt" for read
 * and write access that truncates any existing file.
 *
 * @return Returns a new AssetFileDescriptor which you can use to access
 * the file.
 *
 * @throws FileNotFoundException Throws FileNotFoundException if there is
 * no file associated with the given URI or the mode is invalid.
 * @throws SecurityException Throws SecurityException if the caller does
 * not have permission to access the file.
 *
 * @see #openFile(Uri, String)
 * @see #openFileHelper(Uri, String)
 */
ECode ContentProvider::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ String mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    IParcelFileDescriptor* fd;
    OpenFile(uri, mode, &fd);

    if (fd) {
        return CAssetFileDescriptor::New(fd, 0, -1, fileDescriptor);
    }
    else {
        *fileDescriptor = NULL;
        return NOERROR;
    }
}

/**
 * Convenience for subclasses that wish to implement {@link #openFile}
 * by looking up a column named "_data" at the given URI.
 *
 * @param uri The URI to be opened.
 * @param mode The file mode.  May be "r" for read-only access,
 * "w" for write-only access (erasing whatever data is currently in
 * the file), "wa" for write-only access to append to any existing data,
 * "rw" for read and write access on any existing data, and "rwt" for read
 * and write access that truncates any existing file.
 *
 * @return Returns a new ParcelFileDescriptor that can be used by the
 * client to access the file.
 */
ECode ContentProvider::OpenFileHelper(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    String* str = new String("_data");
    const ArrayOf<String>* strArray = ArrayOf<String>::Alloc(str, 1);
    ICursor* c;
    const ArrayOf<String>* arrayNull = NULL;
    const String* strNull = NULL;
    Query(uri, *strArray, *strNull, *arrayNull, *strNull, &c);
    Int32 count = 0;

    if (c != NULL) {
       c->GetCount(&count);
    }

    if (count != 1) {
        // If there is not exactly one result, throw an appropriate
        // exception.
        if (c != NULL) {
            c->Close();
        }
        if (count == 0) {
            //throw new FileNotFoundException("No entry for " + uri);
            return E_FILE_NOT_FOUND_EXCEPTION;
        }
        //throw new FileNotFoundException("Multiple items at " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    Boolean bSucceeded;
    c->MoveToFirst(&bSucceeded);

    Int32 i;
    c->GetColumnIndex(String("_data"), &i);

    String path;
    if (i >= 0) {
        c->GetString(i, &path);
    }
    c->Close();

    if (path == NULL) {
        //throw new FileNotFoundException("Column _data not found.");
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
#if 0
    Int32 modeBits;
    ContentResolver::ModeToMode(uri, mode, &modeBits);

    IFile* file;
    CFile::New(path, &file);
    return CParcelFileDescriptor::Open(file, modeBits, fileDescriptor);
#else
	return 0;
#endif
}

/**
 * Returns true if this instance is a temporary content provider.
 * @return true if this instance is a temporary content provider
 */
ECode ContentProvider::IsTemporary(
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    return NOERROR;
}

/**
 * Returns the Binder object for this provider.
 *
 * @return the Binder object for this provider
 * @hide
 */
ECode ContentProvider::GetIContentProvider(
    /* [out] */ IContentProvider** provider)
{
    *provider = mTransport;
    return NOERROR;
}

/**
 * Override this to handle requests to perform a batch of operations, or the
 * default implementation will iterate over the operations and call
 * {@link ContentProviderOperation#apply} on each of them.
 * If all calls to {@link ContentProviderOperation#apply} succeed
 * then a {@link ContentProviderResult} array with as many
 * elements as there were operations will be returned.  If any of the calls
 * fail, it is up to the implementation how many of the others take effect.
 * This method can be called from multiple threads, as described in
 * <a href="{@docRoot}guide/topics/fundamentals.html#procthread">Application Fundamentals:
 * Processes and Threads</a>.
 *
 * @param operations the operations to apply
 * @return the results of the applications
 * @throws OperationApplicationException thrown if any operation fails.
 * @see ContentProviderOperation#apply
 */
 ECode ContentProvider::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults)
 {
    Boolean hasNext;
    Int32 i = 0;
    AutoPtr<IContentProvider> provider;
    AutoPtr<IContentProviderOperation> operation;
    AutoPtr<IObjectEnumerator> ObjEnumerator;

    GetIContentProvider((IContentProvider**) &provider);
    operations->GetObjectEnumerator((IObjectEnumerator**) &ObjEnumerator);

    while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
        ObjEnumerator->Current((IInterface**) &operation);
        operation->Apply(provider, *providerResults, i, &(**providerResults)[i]);
        i++;
    }

    return NOERROR;
 }

/**
 * @hide -- until interface has proven itself
 *
 * Call an provider-defined method.  This can be used to implement
 * interfaces that are cheaper than using a Cursor.
 *
 * @param method Method name to call.  Opaque to framework.
 * @param request Nullable String argument passed to method.
 * @param args Nullable Bundle argument passed to method.
 */
ECode ContentProvider::Call(
    String method,
    String request,
    IBundle* args,
    IBundle** bundle)
{
    return NOERROR;
}
