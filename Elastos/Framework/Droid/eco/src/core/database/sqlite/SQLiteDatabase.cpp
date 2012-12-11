
#include "database/sqlite/Sqlite3Exception.h"
#include "database/sqlite/SQLiteDatabase.h"
#include "database/sqlite/SQLiteDebug.h"
#include "database/sqlite/SQLiteStatement.h"
#include "database/sqlite/SQLiteQueryBuilder.h"
#include "database/sqlite/SQLiteDirectCursorDriver.h"
#include "text/TextUtils.h"
#include "os/SystemProperties.h"
#include "os/SystemClock.h"
#include <StringBuffer.h>
#include <Logger.h>
#include <sqlite3_android.h>


using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define UTF16_STORAGE 0
#define SQLITE_SOFT_HEAP_LIMIT (4 * 1024 * 1024)
#define ANDROID_TABLE "android_metadata"

enum {
    OPEN_READWRITE          = 0x00000000,
    OPEN_READONLY           = 0x00000001,
    OPEN_READ_MASK          = 0x00000001,
    NO_LOCALIZED_COLLATORS  = 0x00000010,
    CREATE_IF_NECESSARY     = 0x10000000
};

SQLiteDatabase::ActiveDatabases* SQLiteDatabase::ActiveDatabases::sActiveDatabases = new ActiveDatabases();


SQLiteDatabase::SyncUpdateInfo::SyncUpdateInfo(
    /* [in] */ const String& masterTable,
    /* [in] */ const String& deletedTable,
    /* [in] */ const String& foreignKey)
    : mMasterTable(masterTable)
    , mDeletedTable(deletedTable)
    , mForeignKey(foreignKey)
{}

const CString SQLiteDatabase::TAG = "Database";
const Int32 SQLiteDatabase::EVENT_DB_OPERATION;
const Int32 SQLiteDatabase::EVENT_DB_CORRUPT;
const CString SQLiteDatabase::CONFLICT_VALUES[6] = {
        "", " OR ROLLBACK ", " OR ABORT ", " OR FAIL ", " OR IGNORE ", " OR REPLACE "};
const Int32 SQLiteDatabase::OPEN_READ_MASK;
const Int32 SQLiteDatabase::LOCK_WARNING_WINDOW_IN_MS;
const Int32 SQLiteDatabase::LOCK_ACQUIRED_WARNING_TIME_IN_MS;
const Int32 SQLiteDatabase::LOCK_ACQUIRED_WARNING_THREAD_TIME_IN_MS;
const Int32 SQLiteDatabase::LOCK_ACQUIRED_WARNING_TIME_IN_MS_ALWAYS_PRINT;
const Int32 SQLiteDatabase::SLEEP_AFTER_YIELD_QUANTUM;
const Int32 SQLiteDatabase::sQueryLogTimeInMillis;  // lazily initialized
const Int32 SQLiteDatabase::QUERY_LOG_SQL_LENGTH;
const CString SQLiteDatabase::COMMIT_SQL = "COMMIT;";
const CString SQLiteDatabase::GET_LOCK_LOG_PREFIX = "GETLOCK:";
const Int32 SQLiteDatabase::MAX_WARNINGS_ON_CACHESIZE_CONDITION;
const CString SQLiteDatabase::LOG_SLOW_QUERIES_PROPERTY = "db.log.slow_query_threshold";


SQLiteDatabase::SQLiteDatabase(
    /* [in] */ const String& path,
    /* [in] */ ICursorFactory* factory,
    /* [in] */ Int32 flags)
    : mNativeHandle(NULL)
    , mTempTableSequence(0)
    , mInnerTransactionIsSuccessful(FALSE)
    , mTransactionIsSuccessful(FALSE)
    , mLockAcquiredWallTime(0)
    , mLockAcquiredThreadTime(0)
    , mLastLockMessageTime(0)
    , mPrograms(13)
    , mMaxSqlCacheSize(SQLiteDatabase_MAX_SQL_CACHE_SIZE)
    , mLockingEnabled(TRUE)
    , mSyncUpdateInfo(13)
{
    if (path.IsNull()) {
        // throw new IllegalArgumentException("path should not be null");
        assert(0);
    }
    mFlags = flags;
    mPath = path;
    mSlowQueryThreshold = SystemProperties::GetInt32(LOG_SLOW_QUERIES_PROPERTY, -1);
    // mStackTrace = new DatabaseObjectNotClosedException().fillInStackTrace();
    mFactory = factory;
    Dbopen(mPath, mFlags);
    if (SQLiteDebug::DEBUG_SQL_CACHE) {
        mTimeOpened = GetTime();
    }
    // try {
    //     setLocale(Locale.getDefault());
    // } catch (RuntimeException e) {
    //     Log.e(TAG, "Failed to setLocale() when constructing, closing the database", e);
    //     dbclose();
    //     if (SQLiteDebug.DEBUG_SQL_CACHE) {
    //         mTimeClosed = getTime();
    //     }
    //     throw e;
    // }
}

SQLiteDatabase::~SQLiteDatabase()
{
    Boolean isOpen;
    if (IsOpen(&isOpen), isOpen) {
//        Log.e(TAG, "close() was never explicitly called on database '" +
//                mPath + "' ", mStackTrace);
        CloseClosable();
        OnAllReferencesReleased();
    }
}

PInterface SQLiteDatabase::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ISQLiteDatabase*)this;
    }
    else if (riid == EIID_ISQLiteDatabase) {
        return (ISQLiteDatabase*)this;
    }
    else if (riid == EIID_ISQLiteClosable) {
        return (ISQLiteClosable*)this;
    }
    else if (riid == EIID_SQLiteClosable) {
        return reinterpret_cast<PInterface>((SQLiteClosable*)this);
    }

    return NULL;
}

void SQLiteDatabase::AddSQLiteClosable(
    /* [in] */ ISQLiteClosable* closable)
{
    Lock();
    mPrograms[closable] = NULL;
    Unlock();
}

void SQLiteDatabase::RemoveSQLiteClosable(
        /* [in] */ ISQLiteClosable* closable)
{
    Lock();
    mPrograms.Erase(closable);
    Unlock();
}

//@Override
ECode SQLiteDatabase::OnAllReferencesReleased()
{
    ECode ec = NOERROR;
    Boolean isOpen;
    if (IsOpen(&isOpen), isOpen) {
        if (SQLiteDebug::DEBUG_SQL_CACHE) {
            mTimeClosed = GetTime();
        }
        ec = Dbclose();
    }
    return ec;
}

Int32 SQLiteDatabase::ReleaseMemory()
{
    // Attempt to release as much memory from the
    return sqlite3_release_memory(SQLITE_SOFT_HEAP_LIMIT);
}

ECode SQLiteDatabase::SetLockingEnabled(
        /* [in] */ Boolean lockingEnabled)
{
    mLockingEnabled = lockingEnabled;
    return NOERROR;
}

void SQLiteDatabase::OnCorruption()
{
    Logger::E(TAG,"Removing corrupt database: ");
//    EventLog.writeEvent(EVENT_DB_CORRUPT, mPath);
//    try {
    // Close the database (if we can), which will cause subsequent operations to fail.
    Close();
//    } finally {
    // Delete the corrupt file.  Don't re-create it now -- that would just confuse people
    // -- but the next time someone tries to open it, they can set it up from scratch.
    if (!mPath.EqualsIgnoreCase(":memory")) {
        // delete is only for non-memory database files
//            new File(mPath).delete();
        AutoPtr<IFile> file;
        CFile::New(mPath, (IFile**)&file);
        Boolean isSucceeded;
        file->Delete(&isSucceeded);
    }
//    }
}

void SQLiteDatabase::Lock()
{
    if (!mLockingEnabled) return;
    mLock.Lock();
    if (SQLiteDebug::DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.GetHoldCount() == 1) {
            // Use elapsed real-time since the CPU may sleep when waiting for IO
            mLockAcquiredWallTime = SystemClock::GetElapsedRealtime();
//            mLockAcquiredThreadTime = Debug.threadCpuTimeNanos();
        }
    }
}

void SQLiteDatabase::LockForced()
{
    mLock.Lock();
    if (SQLiteDebug::DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.GetHoldCount() == 1) {
            // Use elapsed real-time since the CPU may sleep when waiting for IO
            mLockAcquiredWallTime = SystemClock::GetElapsedRealtime();
//            mLockAcquiredThreadTime = Debug.threadCpuTimeNanos();
        }
    }
}

void SQLiteDatabase::Unlock()
{
    if (!mLockingEnabled) return;
    if (SQLiteDebug::DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.GetHoldCount() == 1) {
            CheckLockHoldTime();
        }
    }
    mLock.Unlock();
}

void SQLiteDatabase::UnlockForced()
{
    if (SQLiteDebug::DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.GetHoldCount() == 1) {
            CheckLockHoldTime();
        }
    }
    mLock.Unlock();
}

void SQLiteDatabase::CheckLockHoldTime()
{
    // Use elapsed real-time since the CPU may sleep when waiting for IO
    Int64 elapsedTime = SystemClock::GetElapsedRealtime();
    Int64 lockedTime = elapsedTime - mLockAcquiredWallTime;
    if (lockedTime < LOCK_ACQUIRED_WARNING_TIME_IN_MS_ALWAYS_PRINT &&
            !Logger::IsLoggable(TAG, Logger::VERBOSE) &&
            (elapsedTime - mLastLockMessageTime) < LOCK_WARNING_WINDOW_IN_MS) {
        return;
    }
//    if (lockedTime > LOCK_ACQUIRED_WARNING_TIME_IN_MS) {
//        Int32 threadTime = (int)
//                ((Debug.threadCpuTimeNanos() - mLockAcquiredThreadTime) / 1000000);
//        if (threadTime > LOCK_ACQUIRED_WARNING_THREAD_TIME_IN_MS ||
//                lockedTime > LOCK_ACQUIRED_WARNING_TIME_IN_MS_ALWAYS_PRINT) {
//            mLastLockMessageTime = elapsedTime;
//            String msg = "lock held on " + mPath + " for " + lockedTime + "ms. Thread time was "
//                    + threadTime + "ms";
//            if (SQLiteDebug.DEBUG_LOCK_TIME_TRACKING_STACK_TRACE) {
//                Logger::D(TAG, msg, new Exception());
//            } else {
//                Logger::D(TAG, msg);
//            }
//        }
//    }
}

ECode SQLiteDatabase::BeginTransaction()
{
    return BeginTransactionWithListener(NULL /* transactionStatusCallback */);
}

ECode SQLiteDatabase::BeginTransactionWithListener(
        /* [in] */ ISQLiteTransactionListener* transactionListener)
{
    ECode ec;

    LockForced();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
//        throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Boolean ok = FALSE;
    // try {
        // If this thread already had the lock then get out
        if (mLock.GetHoldCount() > 1) {
            if (mInnerTransactionIsSuccessful) {
                // String msg = "Cannot call beginTransaction between "
                //         + "calling setTransactionSuccessful and endTransaction";
                // IllegalStateException e = new IllegalStateException(msg);
                // Log.e(TAG, "beginTransaction() failed", e);
                // throw e;
                if (!ok) {
                    // beginTransaction is called before the try block so we must release the lock in
                    // the case of failure.
                    UnlockForced();
                }
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            ok = TRUE;
            return NOERROR;
        }

        // This thread didn't already have the lock, so begin a database
        // transaction now.
        ExecSQL("BEGIN EXCLUSIVE;");
        mTransactionListener = transactionListener;
        mTransactionIsSuccessful = TRUE;
        mInnerTransactionIsSuccessful = FALSE;
        if (transactionListener != NULL) {
            // try {
            ec = transactionListener->OnBegin();
            // } catch (RuntimeException e) {
            //     execSQL("ROLLBACK;");
            //     throw e;
            // }
            if (ec == (ECode)E_RUNTIME_EXCEPTION) {
                ExecSQL("ROLLBACK;");
                if (!ok) {
                    // beginTransaction is called before the try block so we must release the lock in
                    // the case of failure.
                    UnlockForced();
                }
                return ec;
            }
        }
        ok = TRUE;
    // } finally {
        if (!ok) {
            // beginTransaction is called before the try block so we must release the lock in
            // the case of failure.
            UnlockForced();
        }
    // }

    return NOERROR;
}

ECode SQLiteDatabase::EndTransaction()
{
    ECode ec;
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!mLock.IsHeldByCurrentThread()) {
        // throw new IllegalStateException("no transaction pending");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    try {
        if (mInnerTransactionIsSuccessful) {
            mInnerTransactionIsSuccessful = FALSE;
        }
        else {
            mTransactionIsSuccessful = FALSE;
        }
        if (mLock.GetHoldCount() != 1) {
            mTransactionListener = NULL;
            UnlockForced();
            return NOERROR;
        }
//        RuntimeException savedException = null;
        if (mTransactionListener != NULL) {
//            try {
            if (mTransactionIsSuccessful) {
                ec = mTransactionListener->OnCommit();
            }
            else {
                ec = mTransactionListener->OnRollback();
            }
//            } catch (RuntimeException e) {
//                savedException = e;
//                mTransactionIsSuccessful = false;
//            }
            if (ec == (ECode)E_RUNTIME_EXCEPTION) {
                mTransactionIsSuccessful = FALSE;
            }
        }
        if (mTransactionIsSuccessful) {
            ExecSQL(COMMIT_SQL);
        }
        else {
//            try {
            ExecSQL("ROLLBACK;");
//                if (savedException != null) {
//                    throw savedException;
//                }
//            } catch (SQLException e) {
//                if (Config.LOGD) {
//                    Log.d(TAG, "exception during rollback, maybe the DB previously "
//                            + "performed an auto-rollback");
//                }
        }

//    } finally {
        mTransactionListener = NULL;
        UnlockForced();
//        if (Config::LOGV) {
//            Logger.V(TAG, "unlocked " + Thread.currentThread()
//                    + ", holdCount is " + mLock.getHoldCount());
//        }
//    }
    return ec;
}

ECode SQLiteDatabase::SetTransactionSuccessful()
{
    Boolean isOpen;
    if (this->IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
   if (!mLock.IsHeldByCurrentThread()) {
        // throw new IllegalStateException("no transaction pending");
        return E_ILLEGAL_STATE_EXCEPTION;
   }
    if (mInnerTransactionIsSuccessful) {
//        throw new IllegalStateException(
//                "setTransactionSuccessful may only be called once per call to beginTransaction");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mInnerTransactionIsSuccessful = TRUE;
    return NOERROR;
}

ECode SQLiteDatabase::InTransaction(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mLock.GetHoldCount() > 0;
    return NOERROR;
}

ECode SQLiteDatabase::IsDbLockedByCurrentThread(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mLock.IsHeldByCurrentThread();
    return NOERROR;
}

ECode SQLiteDatabase::IsDbLockedByOtherThreads(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = !mLock.IsHeldByCurrentThread() && mLock.IsLocked();
    return NOERROR;
}

ECode SQLiteDatabase::YieldIfContended(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = YieldIfContendedHelper(FALSE /* do not check yielding */,
                -1 /* sleepAfterYieldDelay */);
    return NOERROR;
}

ECode SQLiteDatabase::YieldIfContendedSafely(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = YieldIfContendedHelper(TRUE /* check yielding */, -1 /* sleepAfterYieldDelay*/);
    return NOERROR;
}

ECode SQLiteDatabase::YieldIfContendedSafelyEx(
    /* [in] */ Int64 sleepAfterYieldDelay,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = YieldIfContendedHelper(TRUE /* check yielding */, sleepAfterYieldDelay);
    return NOERROR;
}

Boolean SQLiteDatabase::YieldIfContendedHelper(
    /* [in] */ Boolean checkFullyYielded,
    /* [in] */ Int64 sleepAfterYieldDelay)
{
    if (mLock.GetQueueLength() == 0) {
        // Reset the lock acquire time since we know that the thread was willing to yield
        // the lock at this time.
        mLockAcquiredWallTime = SystemClock::GetElapsedRealtime();
        // mLockAcquiredThreadTime = Debug.threadCpuTimeNanos();
        return FALSE;
    }
    SetTransactionSuccessful();
    ISQLiteTransactionListener* transactionListener = mTransactionListener;
    EndTransaction();
    if (checkFullyYielded) {
        Boolean result;
        if (IsDbLockedByCurrentThread(&result), result) {
//            throw new IllegalStateException(
//                    "Db locked more than once. yielfIfContended cannot yield");
            assert(0);
        }
    }
    if (sleepAfterYieldDelay > 0) {
        // Sleep for up to sleepAfterYieldDelay milliseconds, waking up periodically to
        // check if anyone is using the database.  If the database is not contended,
        // retake the lock and return.
        Int64 remainingDelay = sleepAfterYieldDelay;
        while (remainingDelay > 0) {
//            try {
//                Thread.sleep(remainingDelay < SLEEP_AFTER_YIELD_QUANTUM ?
//                        remainingDelay : SLEEP_AFTER_YIELD_QUANTUM);
//            } catch (InterruptedException e) {
//                Thread.interrupted();
//            }
            remainingDelay -= SLEEP_AFTER_YIELD_QUANTUM;
//            if (mLock.getQueueLength() == 0) {
//                break;
//            }
        }
    }
    this->BeginTransactionWithListener(transactionListener);
    return TRUE;
}

ECode SQLiteDatabase::GetSyncedTables(
    /* [out] */ IObjectStringMap** tables)
{
    VALIDATE_NOT_NULL(tables);

    // synchronized(mSyncUpdateInfo) {
    //     HashMap<String, String> tables = new HashMap<String, String>();
    //     for (String table : mSyncUpdateInfo.keySet()) {
    //         SyncUpdateInfo info = mSyncUpdateInfo.get(table);
    //         if (info.deletedTable != null) {
    //             tables.put(table, info.deletedTable);
    //         }
    //     }
    //     return tables;
    // }
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::OpenDatabase(
    /* [in] */ const String& path,
    /* [in] */ ICursorFactory* factory,
    /* [in] */ Int32 flags,
    /* [out] */ ISQLiteDatabase** db)
{
    VALIDATE_NOT_NULL(db);

    AutoPtr<SQLiteDatabase> sqliteDatabase;
    // try {
        // Open the database.
        sqliteDatabase = new SQLiteDatabase(path, factory, flags);
        if (SQLiteDebug::DEBUG_SQL_STATEMENTS) {
            sqliteDatabase->EnableSqlTracing(path);
        }
        if (SQLiteDebug::DEBUG_SQL_TIME) {
            sqliteDatabase->EnableSqlProfiling(path);
        }
    // } catch (SQLiteDatabaseCorruptException e) {
    //     // Try to recover from this, if we can.
    //     // TODO: should we do this for other open failures?
    //     Log.e(TAG, "Deleting and re-creating corrupt database " + path, e);
    //     EventLog.writeEvent(EVENT_DB_CORRUPT, path);
    //     if (!path.equalsIgnoreCase(":memory")) {
    //         // delete is only for non-memory database files
    //         new File(path).delete();
    //     }
    //     sqliteDatabase = new SQLiteDatabase(path, factory, flags);
    // }
    // ActiveDatabases.getInstance().mActiveDatabases.add(
    //         new WeakReference<SQLiteDatabase>(sqliteDatabase));
    ActiveDatabases::GetInstance()->mActiveDatabases.Insert(sqliteDatabase);
    *db = (ISQLiteDatabase*)sqliteDatabase.Get();
    (*db)->AddRef();
    return NOERROR;
}

ECode SQLiteDatabase::OpenOrCreateDatabase(
    /* [in] */ IFile* file,
    /* [in] */ ICursorFactory* factory,
    /* [out] */ ISQLiteDatabase** db)
{
    VALIDATE_NOT_NULL(db);

    String path;
    file->GetPath(&path);
    return OpenOrCreateDatabase(path, factory, db);
}

ECode SQLiteDatabase::OpenOrCreateDatabase(
    /* [in] */ const String& path,
    /* [in] */ ICursorFactory* factory,
    /* [out] */ ISQLiteDatabase** db)
{
    VALIDATE_NOT_NULL(db);

    return OpenDatabase(path, factory, SQLiteDatabase_CREATE_IF_NECESSARY, db);
}

ECode SQLiteDatabase::Create(
    /* [in] */ ICursorFactory* factory,
    /* [out] */ ISQLiteDatabase** db)
{
    VALIDATE_NOT_NULL(db);

    // This is a magic string with special meaning for SQLite.
    return OpenDatabase(String(":memory:"), factory, SQLiteDatabase_CREATE_IF_NECESSARY, db);
}

ECode SQLiteDatabase::Close()
{
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        return NOERROR; // already closed
    }
    Lock();
//    try {
    CloseClosable();
        // close this database instance - regardless of its reference count value
    ECode ec = OnAllReferencesReleased();
//    } finally {
    Unlock();
//    }
    return ec;
}

ECode SQLiteDatabase::CloseClosable()
{
    /* deallocate all compiled sql statement objects from mCompiledQueries cache.
     * this should be done before de-referencing all {@link SQLiteClosable} objects
     * from this database object because calling
     * {@link SQLiteClosable#onAllReferencesReleasedFromContainer()} could cause the database
     * to be closed. sqlite doesn't let a database close if there are
     * any unfinalized statements - such as the compiled-sql objects in mCompiledQueries.
     */
    DeallocCachedSqlStatements();

    HashMap<AutoPtr<ISQLiteClosable>, AutoPtr<IInterface> >::Iterator iter = mPrograms.Begin();
    while (iter != mPrograms.End()) {
        SQLiteClosable* program = (SQLiteClosable*)iter->mFirst->Probe(EIID_SQLiteClosable);
        if (program != NULL) {
            FAIL_RETURN(program->OnAllReferencesReleasedFromContainer());
        }
    }
    return NOERROR;
}

static void SqlTrace(void *databaseName, const char *sql)
{
    StringBuffer sb("sql_statement|");
    sb += (char*)databaseName;
    sb += "|";
    sb += sql;
    Logger::I("Database", (const char*)sb);
}

static void SqlProfile(void *databaseName, const char *sql, sqlite3_uint64 tm)
{
    Int64 d = tm/1000000.0;
    StringBuffer sb("elapsedTime4Sql|");
    sb += (char*)databaseName;
    sb += "|";
    sb += d;
    sb += " ms|";
    sb += sql;
    Logger::I("Database", (const char*)sb);
    // LOGI("elapsedTime4Sql|%s|%.3f ms|%s\n", (char *)databaseName, d, sql);
}

ECode SQLiteDatabase::Dbclose()
{
    sqlite3 * handle = mNativeHandle;

    if (handle != NULL) {
        // release the memory associated with the traceFuncArg in enableSqlTracing function
        void *traceFuncArg = sqlite3_trace(handle, &SqlTrace, NULL);
        if (traceFuncArg != NULL) {
            free(traceFuncArg);
        }
        // release the memory associated with the traceFuncArg in enableSqlProfiling function
        traceFuncArg = sqlite3_profile(handle, &SqlProfile, NULL);
        if (traceFuncArg != NULL) {
            free(traceFuncArg);
        }
        // LOGV("Closing database: handle=%p\n", handle);
        Int32 result = sqlite3_close(handle);
        if (result == SQLITE_OK) {
            // LOGV("Closed %p\n", handle);
            mNativeHandle = NULL;
        }
        else {
            // This can happen if sub-objects aren't closed first.  Make sure the caller knows.
            // LOGE("sqlite3_close(%p) failed: %d\n", handle, result);
            return throw_sqlite3_exception(handle);
        }
    }
    return NOERROR;
}

ECode SQLiteDatabase::GetVersion(
    /* [out] */ Int32* version)
{
    VALIDATE_NOT_NULL(version);

    AutoPtr<SQLiteStatement> prog;
    Lock();
    Boolean isOpen;
    if (this->IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
    prog = new SQLiteStatement(this, String("PRAGMA user_version;"));
    Int64 ver;
    prog->SimpleQueryForInt64(&ver);
    *version = (Int32)ver;
    //} finally {
    if (prog != NULL) {
        prog->Close();
    }
    Unlock();
    //}
    return NOERROR;
}

ECode SQLiteDatabase::SetVersion(
    /* [in] */ Int32 version)
{
    String sql = String("PRAGMA user_version = ") + String::FromInt32(version);
    return ExecSQL(sql);
}

ECode SQLiteDatabase::GetMaximumSize(
    /* [out] */ Int64* maxSize)
{
    VALIDATE_NOT_NULL(maxSize);

    AutoPtr<SQLiteStatement> prog;
    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
    prog = new SQLiteStatement(this, String("PRAGMA max_page_count;"));
    Int64 pageCount, pageSize;
    prog->SimpleQueryForInt64(&pageCount);
    GetPageSize(&pageSize);
    *maxSize = pageCount * pageSize;
    //} finally {
    if (prog != NULL) {
        prog->Close();
    }
    Unlock();
    //}
    return NOERROR;
}

ECode SQLiteDatabase::SetMaximumSize(
    /* [in] */ Int64 numBytes,
    /* [out] */ Int64* maxSize)
{
    VALIDATE_NOT_NULL(maxSize);

    AutoPtr<SQLiteStatement> prog;
    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
    Int64 pageSize;
    GetPageSize(&pageSize);
    Int64 numPages = numBytes / pageSize;
    // If numBytes isn't a multiple of pageSize, bump up a page
    if ((numBytes % pageSize) != 0) {
        numPages++;
    }
    String sql = String("PRAGMA max_page_count = ") + String::FromInt64(numPages);
    prog = new SQLiteStatement(this, sql);
    Int64 newPageCount;
    prog->SimpleQueryForInt64(&newPageCount);
    *maxSize = newPageCount * pageSize;
    //} finally {
    if (prog != NULL) {
        prog->Close();
    }
    Unlock();
    //}
    return NOERROR;
}

ECode SQLiteDatabase::GetPageSize(
    /* [out] */ Int64* pageSize)
{
    VALIDATE_NOT_NULL(pageSize);

    AutoPtr<SQLiteStatement> prog;
    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
    prog = new SQLiteStatement(this, String("PRAGMA page_size;"));
    prog->SimpleQueryForInt64(pageSize);
    //} finally {
    if (prog != NULL) {
        prog->Close();
    }
    Unlock();
    //}
    return NOERROR;
}

ECode SQLiteDatabase::SetPageSize(
    /* [in] */ Int64 numBytes)
{
    String sql = String("PRAGMA page_size = ") + String::FromInt64(numBytes);
    return ExecSQL(sql);
}

ECode SQLiteDatabase::MarkTableSyncable(
    /* [in] */ const String& table,
    /* [in] */ const String& deletedTable)
{
    return MarkTableSyncable(table, String("_id"), table, deletedTable);
}

ECode SQLiteDatabase::MarkTableSyncableEx(
    /* [in] */ const String& table,
    /* [in] */ const String& foreignKey,
    /* [in] */ const String& updateTable)
{
    return MarkTableSyncable(table, foreignKey, updateTable, String(NULL));
}

ECode SQLiteDatabase::MarkTableSyncable(
    /* [in] */ const String& table,
    /* [in] */ const String& foreignKey,
    /* [in] */ const String& updateTable,
    /* [in] */ const String& deletedTable)
{
    ECode ec;

    Lock();
    //try {
    ec = NativeExecSQL(String("SELECT _sync_dirty FROM ") + updateTable
            + String(" LIMIT 0") );
    if (SUCCEEDED(ec)) {
        ec = NativeExecSQL(String("SELECT ") + foreignKey + String(" FROM ") + table
                + String(" LIMIT 0") );
    }
    //} finally {
    Unlock();
    //}

    SyncUpdateInfo* info = new SyncUpdateInfo(updateTable, deletedTable, foreignKey);
    {
        Mutex::Autolock lock(mSyncUpdateInfoLock);
        mSyncUpdateInfo[table] = info;
    }

    return ec;
}

void SQLiteDatabase::RowUpdated(
    /* [in] */ const String& table,
    /* [in] */ Int32 rowId)
{
    SyncUpdateInfo* info;
    {
        Mutex::Autolock lock(mSyncUpdateInfoLock);

        HashMap<String, SyncUpdateInfo*>::Iterator find = mSyncUpdateInfo.Find(table);
        if(find != mSyncUpdateInfo.End()) {
            info = find->mSecond;
        }
    }
    if (info != NULL) {
        StringBuffer sb("UPDATE ");
        sb += info->mMasterTable;
        sb += " SET _sync_dirty=1 WHERE _id=(SELECT ";
        sb += info->mForeignKey;
        sb += " FROM ";
        sb += table;
        sb += " WHERE _id=";
        sb += rowId;
        sb += ")";
        ExecSQL(sb);
    }
}

ECode SQLiteDatabase::FindEditTable(
    /* [in] */ const String& tables,
    /* [out] */ String* editable)
{
    VALIDATE_NOT_NULL(editable);

    if (!tables.IsNullOrEmpty()) {
        // find the first word terminated by either a space or a comma
        Int32 spacepos = tables.IndexOf(' ');
        Int32 commapos = tables.IndexOf(',');

        if (spacepos > 0 && (spacepos < commapos || commapos < 0)) {
            *editable = tables.Substring(0, spacepos);
            return NOERROR;
        }
        else if (commapos > 0 && (commapos < spacepos || spacepos < 0) ) {
            *editable = tables.Substring(0, commapos);
            return NOERROR;
        }
        *editable = tables;
        return NOERROR;
    }
    else {
        // throw new IllegalStateException("Invalid tables");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode SQLiteDatabase::CompileStatement(
    /* [in] */ const String& sql,
    /* [out] */ ISQLiteStatement** stmt)
{
    VALIDATE_NOT_NULL(stmt);

    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
    *stmt = new SQLiteStatement(this, sql);
    (*stmt)->AddRef();
    //} finally {
    Unlock();
    //}
    return NOERROR;
}

ECode SQLiteDatabase::Query(
    /* [in] */ Boolean distinct,
    /* [in] */ const String& table,
    /* [in] */ ArrayOf<String>* columns,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [in] */ const String& limit,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    return QueryWithFactory(NULL, distinct, table, columns, selection, selectionArgs,
            groupBy, having, orderBy, limit, cursor);
}

ECode SQLiteDatabase::QueryWithFactory(
    /* [in] */ ICursorFactory* cursorFactory,
    /* [in] */ Boolean distinct,
    /* [in] */ const String& table,
    /* [in] */ ArrayOf<String>* columns,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [in] */ const String& limit,
    /* [out] */ ICursor** cursor)
{
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    String sql, editTable;
    SQLiteQueryBuilder::BuildQueryString(
            distinct, table, columns, selection, groupBy, having, orderBy, limit, &sql);
    FindEditTable(table, &editTable);
    return RawQueryWithFactory(
            cursorFactory, sql, selectionArgs, editTable, cursor);
}

ECode SQLiteDatabase::QueryEx(
    /* [in] */ const String& table,
    /* [in] */ ArrayOf<String>* columns,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [out] */ ICursor** cursor)
{
    return Query(FALSE, table, columns, selection, selectionArgs, groupBy,
            having, orderBy, String(NULL) /* limit */, cursor);
}

ECode SQLiteDatabase::QueryEx2(
    /* [in] */ const String& table,
    /* [in] */ ArrayOf<String>* columns,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [in] */ const String& limit,
    /* [out] */ ICursor** cursor)
{
    return Query(FALSE, table, columns, selection, selectionArgs, groupBy,
            having, orderBy, limit, cursor);
}

ECode SQLiteDatabase::RawQuery(
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ ICursor** cursor)
{
    return RawQueryWithFactory(NULL, sql, selectionArgs, String(NULL), cursor);
}

ECode SQLiteDatabase::RawQueryWithFactory(
    /* [in] */ ICursorFactory* cursorFactory,
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& editTable,
    /* [out] */ ICursor** cursor)
{
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // BlockGuard.getThreadPolicy().onReadFromDisk();
    // Int64 timeStart = 0;

    // if (Config.LOGV || mSlowQueryThreshold != -1) {
    //     timeStart = System::GetCurrentTimeMillis();
    // }

    AutoPtr<ISQLiteCursorDriver> driver = new SQLiteDirectCursorDriver(this, sql, editTable);

    // try {
    ECode ec = driver->Query(
            cursorFactory != NULL ? cursorFactory : mFactory.Get(),
            selectionArgs, cursor);
    // } finally {
    // if (Config.LOGV || mSlowQueryThreshold != -1) {

    //     // Force query execution
    //     int count = -1;
    //     if (cursor != null) {
    //         count = cursor.getCount();
    //     }

    //     long duration = System.currentTimeMillis() - timeStart;

    //     if (Config.LOGV || duration >= mSlowQueryThreshold) {
    //         Log.v(SQLiteCursor.TAG,
    //               "query (" + duration + " ms): " + driver.toString() + ", args are "
    //                       + (selectionArgs != null
    //                       ? TextUtils.join(",", selectionArgs)
    //                       : "<null>")  + ", count is " + count);
    //     }
    // }
    // }
    return ec;
}

ECode SQLiteDatabase::RawQueryEx(
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ Int32 initialRead,
    /* [in] */ Int32 maxRead,
    /* [out] */ ICursor** cursor)
{
    FAIL_RETURN(RawQueryWithFactory(
            NULL, sql, selectionArgs, String(NULL), cursor));
    ISQLiteCursor* c = ISQLiteCursor::Probe(*cursor);
    return c->SetLoadStyle(initialRead, maxRead);
}

ECode SQLiteDatabase::Insert(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues* values,
    /* [out] */ Int64* rowId)
{
    VALIDATE_NOT_NULL(rowId);

//    try {
    ECode ec = InsertWithOnConflict(table, nullColumnHack,
            values, SQLiteDatabase_CONFLICT_NONE, rowId);
//    } catch (SQLException e) {
//        Log.e(TAG, "Error inserting " + values, e);
//        return -1;
//    }
    if (ec == (ECode)E_SQL_EXCEPTION) {
        *rowId = -1;
    }
    return NOERROR;
}

ECode SQLiteDatabase::InsertOrThrow(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues* values,
    /* [out] */ Int64* rowId)
{
    VALIDATE_NOT_NULL(rowId);

    return InsertWithOnConflict(table, nullColumnHack,
            values, SQLiteDatabase_CONFLICT_NONE, rowId);
}

ECode SQLiteDatabase::Replace(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ Int64* rowId)
{
    VALIDATE_NOT_NULL(rowId);

//    try {
    ECode ec = InsertWithOnConflict(table, nullColumnHack,
            initialValues, SQLiteDatabase_CONFLICT_REPLACE, rowId);
//    } catch (SQLException e) {
//        Log.e(TAG, "Error inserting " + initialValues, e);
//        return -1;
//    }
    if (ec == (ECode)E_SQL_EXCEPTION) {
        *rowId = -1;
    }
    return NOERROR;
}

ECode SQLiteDatabase::ReplaceOrThrow(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ Int64* rowId)
{
    VALIDATE_NOT_NULL(rowId);

    return InsertWithOnConflict(table, nullColumnHack,
            initialValues, SQLiteDatabase_CONFLICT_REPLACE, rowId);
}

ECode SQLiteDatabase::InsertWithOnConflict(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues* initialValues,
    /* [in] */ Int32 conflictAlgorithm,
    /* [out] */ Int64* rowId)
{
    VALIDATE_NOT_NULL(rowId);

    //BlockGuard.getThreadPolicy().onWriteToDisk();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // Measurements show most sql lengths <= 152
    StringBuffer sql(152);
    sql += "INSERT";
    sql += CONFLICT_VALUES[conflictAlgorithm];
    sql += " INFO ";
    sql += table;
    // Measurements show most values lengths < 40
    StringBuffer values(40);

    Int32 size;
    AutoPtr<IObjectStringMap> entrySet;
    if (initialValues != NULL && (initialValues->GetSize(&size), size > 0)) {
        initialValues->ValueSet((IObjectStringMap**)&entrySet);
        // Iterator<Map.Entry<String, Object>> entriesIter = entrySet.iterator();
        // sql.append('(');

        // boolean needSeparator = false;
        // while (entriesIter.hasNext()) {
        //     if (needSeparator) {
        //         sql.append(", ");
        //         values.append(", ");
        //     }
        //     needSeparator = true;
        //     Map.Entry<String, Object> entry = entriesIter.next();
        //     sql.append(entry.getKey());
        //     values.append('?');
        // }

        // sql.append(')');
    }
    else {
        sql += "(" + nullColumnHack + ")";
        values += "NULL";
    }

    sql += " VALUES(";
    sql += values;
    sql += ");";

    Lock();
    AutoPtr<ISQLiteStatement> statement;
    // try {
        CompileStatement(String(sql), (ISQLiteStatement**)&statement);

        // Bind the values
        if (entrySet != NULL) {
            // int size = entrySet.size();
            // Iterator<Map.Entry<String, Object>> entriesIter = entrySet.iterator();
            // for (int i = 0; i < size; i++) {
            //     Map.Entry<String, Object> entry = entriesIter.next();
            //     DatabaseUtils.bindObjectToProgram(statement, i + 1, entry.getValue());
            // }
        }

        // Run the program and then cleanup
        statement->Execute();

        Int64 insertedRowId = LastInsertRow();
        // if (insertedRowId == -1) {
        //     Log.e(TAG, "Error inserting " + initialValues + " using " + sql);
        // } else {
        //     if (Config.LOGD && Log.isLoggable(TAG, Log.VERBOSE)) {
        //         Log.v(TAG, "Inserting row " + insertedRowId + " from "
        //                 + initialValues + " using " + sql);
        //     }
        // }
        *rowId = insertedRowId;
    // } catch (SQLiteDatabaseCorruptException e) {
    //     onCorruption();
    //     throw e;
    // } finally {
        if (statement != NULL) {
            statement->Close();
        }
        Unlock();
    // }
    return NOERROR;
}

ECode SQLiteDatabase::Delete(
    /* [in] */ const String& table,
    /* [in] */ const String& whereClause,
    /* [in] */ ArrayOf<String>* whereArgs,
    /* [out] */ Int32* value)
{
    //BlockGuard.getThreadPolicy().onWriteToDisk();
    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<ISQLiteStatement> statement;
//    try {
        StringBuffer sql("DELETE FROM ");
        sql += table;
        sql += !whereClause.IsNullOrEmpty() ? " WHERE " + whereClause : "";
        CompileStatement(String(sql), (ISQLiteStatement**)&statement);
        if (whereArgs != NULL) {
            Int32 numArgs = whereArgs->GetLength();
            for (Int32 i = 0; i < numArgs; i++) {
                // DatabaseUtils.bindObjectToProgram(statement, i + 1, whereArgs[i]);
            }
        }
        statement->Execute();
        *value = LastChangeCount();
//    } catch (SQLiteDatabaseCorruptException e) {
       // onCorruption();
       // throw e;
//    } finally {
        if (statement != NULL) {
            statement->Close();
        }
        Unlock();
//    }
    return NOERROR;
}

ECode SQLiteDatabase::Update(
    /* [in] */ const String& table,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& whereClause,
    /* [in] */ ArrayOf<String>* whereArgs,
    /* [out] */ Int32* value)
{
    return UpdateWithOnConflict(table, values, whereClause, whereArgs, SQLiteDatabase_CONFLICT_NONE, value);
}

ECode SQLiteDatabase::UpdateWithOnConflict(
    /* [in] */ const String& table,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& whereClause,
    /* [in] */ ArrayOf<String>* whereArgs,
    /* [in] */ Int32 conflictAlgorithm,
    /* [out] */ Int32* value)
{
    // BlockGuard.getThreadPolicy().onWriteToDisk();
    Int32 size;
    if (values == NULL || (values->GetSize(&size), size == 0)) {
        // throw new IllegalArgumentException("Empty values");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuffer sql(120);
    sql += "UPDATE ";
    sql += CONFLICT_VALUES[conflictAlgorithm];
    sql += table;
    sql += " SET ";

    AutoPtr<IObjectStringMap> entrySet;
    values->ValueSet((IObjectStringMap**)&entrySet);
    // Iterator<Map.Entry<String, Object>> entriesIter = entrySet.iterator();

    // while (entriesIter.hasNext()) {
    //     Map.Entry<String, Object> entry = entriesIter.next();
    //     sql.append(entry.getKey());
    //     sql.append("=?");
    //     if (entriesIter.hasNext()) {
    //         sql.append(", ");
    //     }
    // }

    if (!whereClause.IsNullOrEmpty()) {
        sql += " WHERE ";
        sql += whereClause;
    }

    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<ISQLiteStatement> statement;
    // try {
        CompileStatement(String(sql), (ISQLiteStatement**)&statement);

        // Bind the values
        // int size = entrySet.size();
        // entriesIter = entrySet.iterator();
        // int bindArg = 1;
        // for (int i = 0; i < size; i++) {
        //     Map.Entry<String, Object> entry = entriesIter.next();
        //     DatabaseUtils.bindObjectToProgram(statement, bindArg, entry.getValue());
        //     bindArg++;
        // }

        // if (whereArgs != null) {
        //     size = whereArgs.length;
        //     for (int i = 0; i < size; i++) {
        //         statement.bindString(bindArg, whereArgs[i]);
        //         bindArg++;
        //     }
        // }

        // Run the program and then cleanup
        statement->Execute();
        Int32 numChangedRows = LastChangeCount();
        // if (Config.LOGD && Log.isLoggable(TAG, Log.VERBOSE)) {
        //     Log.v(TAG, "Updated " + numChangedRows + " using " + values + " and " + sql);
        // }
        *value = numChangedRows;
    // } catch (SQLiteDatabaseCorruptException e) {
    //     onCorruption();
    //     throw e;
    // } catch (SQLException e) {
    //     Log.e(TAG, "Error updating " + values + " using " + sql);
    //     throw e;
    // } finally {
        if (statement != NULL) {
            statement->Close();
        }
        Unlock();
    // }
    return NOERROR;
}

ECode SQLiteDatabase::ExecSQL(
    /* [in] */ CString sql)
{
    // BlockGuard.getThreadPolicy().onWriteToDisk();
    Int64 timeStart = SystemClock::GetUptimeMillis();
    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    LogTimeStat(mLastSqlStatement, timeStart, GET_LOCK_LOG_PREFIX);
    // try {
        NativeExecSQL(sql);
    // } catch (SQLiteDatabaseCorruptException e) {
    //     onCorruption();
    //     throw e;
    // } finally {
        Unlock();
    // }

    // Log commit statements along with the most recently executed
    // SQL statement for disambiguation.  Note that instance
    // equality to COMMIT_SQL is safe here.
    if (sql.Equals(COMMIT_SQL)) {
        LogTimeStat(mLastSqlStatement, timeStart, COMMIT_SQL);
    }
    else {
        LogTimeStat(sql, timeStart, NULL);
    }
    return NOERROR;
}

ECode SQLiteDatabase::ExecSQLEx(
    /* [in] */ CString sql,
    /* [in] */ ArrayOf<IInterface*>* bindArgs)
{
    // BlockGuard.getThreadPolicy().onWriteToDisk();
    if (bindArgs == NULL) {
        // throw new IllegalArgumentException("Empty bindArgs");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 timeStart = SystemClock::GetUptimeMillis();
    Lock();
    Boolean isOpen;
    if (IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<ISQLiteStatement> statement;
    // try {
        CompileStatement(String(sql), (ISQLiteStatement**)&statement);
        if (bindArgs != NULL) {
            Int32 numArgs = bindArgs->GetLength();
            for (Int32 i = 0; i < numArgs; i++) {
                // DatabaseUtils.bindObjectToProgram(statement, i + 1, bindArgs[i]);
            }
        }
        statement->Execute();
    // } catch (SQLiteDatabaseCorruptException e) {
    //     onCorruption();
    //     throw e;
    // } finally {
        if (statement != NULL) {
            statement->Close();
        }
        Unlock();
    // }
    LogTimeStat(sql, timeStart);
    return NOERROR;
}

String SQLiteDatabase::GetTime()
{
    // return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS ").format(System.currentTimeMillis());
    return String(NULL);
}

ECode SQLiteDatabase::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);

    *isReadOnly = (mFlags & OPEN_READ_MASK) == SQLiteDatabase_OPEN_READONLY;
    return NOERROR;
}

ECode SQLiteDatabase::IsOpen(
    /* [out] */ Boolean* isOpen)
{
    VALIDATE_NOT_NULL(isOpen);

    *isOpen = mNativeHandle != NULL;
    return NOERROR;
}

ECode SQLiteDatabase::NeedUpgrade(
    /* [in] */ Int32 newVersion,
    /* [out] */ Boolean* needed)
{
    VALIDATE_NOT_NULL(needed);

    Int32 version;
    GetVersion(&version);
    *needed = newVersion > version;
    return NOERROR;
}

ECode SQLiteDatabase::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);

    *path = mPath;
    return NOERROR;
}

void SQLiteDatabase::LogTimeStat(
    /* [in] */ CString sql,
    /* [in] */ Int64 beginMillis)
{
    LogTimeStat(sql, beginMillis, NULL);
}

void SQLiteDatabase::LogTimeStat(
    /* [in] */ CString sql,
    /* [in] */ Int64 beginMillis,
    /* [in] */ CString prefix)
{
    // // Keep track of the last statement executed here, as this is
    // // the common funnel through which all methods of hitting
    // // libsqlite eventually flow.
    // mLastSqlStatement = sql;

    // // Sample fast queries in proportion to the time taken.
    // // Quantize the % first, so the logged sampling probability
    // // exactly equals the actual sampling rate for this query.

    // int samplePercent;
    // long durationMillis = SystemClock.uptimeMillis() - beginMillis;
    // if (durationMillis == 0 && prefix == GET_LOCK_LOG_PREFIX) {
    //     // The common case is locks being uncontended.  Don't log those,
    //     // even at 1%, which is our default below.
    //     return;
    // }
    // if (sQueryLogTimeInMillis == 0) {
    //     sQueryLogTimeInMillis = SystemProperties.getInt("db.db_operation.threshold_ms", 500);
    // }
    // if (durationMillis >= sQueryLogTimeInMillis) {
    //     samplePercent = 100;
    // } else {;
    //     samplePercent = (int) (100 * durationMillis / sQueryLogTimeInMillis) + 1;
    //     if (mRandom.nextInt(100) >= samplePercent) return;
    // }

    // // Note: the prefix will be "COMMIT;" or "GETLOCK:" when non-null.  We wait to do
    // // it here so we avoid allocating in the common case.
    // if (prefix != null) {
    //     sql = prefix + sql;
    // }

    // if (sql.length() > QUERY_LOG_SQL_LENGTH) sql = sql.substring(0, QUERY_LOG_SQL_LENGTH);

    // // ActivityThread.currentPackageName() only returns non-null if the
    // // current thread is an application main thread.  This parameter tells
    // // us whether an event loop is blocked, and if so, which app it is.
    // //
    // // Sadly, there's no fast way to determine app name if this is *not* a
    // // main thread, or when we are invoked via Binder (e.g. ContentProvider).
    // // Hopefully the full path to the database will be informative enough.

    // String blockingPackage = AppGlobals.getInitialPackage();
    // if (blockingPackage == null) blockingPackage = "";

    // EventLog.writeEvent(
    //     EVENT_DB_OPERATION,
    //     getPathForLogs(),
    //     sql,
    //     durationMillis,
    //     blockingPackage,
    //     samplePercent);
}

String SQLiteDatabase::GetPathForLogs()
{
    if (!mPathForLogs.IsNull()) {
        return mPathForLogs;
    }
    if (mPath.IsNull()) {
        return String(NULL);
    }
    if (mPath.IndexOf('@') == -1) {
        mPathForLogs = mPath;
    }
    else {
        // mPathForLogs = EMAIL_IN_DB_PATTERN.matcher(mPath).replaceAll("XX@YY");
    }
    return mPathForLogs;
}

ECode SQLiteDatabase::SetLocale(
    /* [in] */ ILocale* locale)
{
    Lock();
//    try {
    ECode ec = NativeSetLocale(NULL/*locale.toString()*/, mFlags);
//    } finally {
    Unlock();
//    }
    return ec;
}

void SQLiteDatabase::AddToCompiledQueries(
    /* [in] */ const String& sql,
    /* [in] */ SQLiteCompiledSql* compiledStatement)
{
    if (mMaxSqlCacheSize == 0) {
        // for this database, there is no cache of compiled sql.
        if (SQLiteDebug::DEBUG_SQL_CACHE) {
            // Log.v(TAG, "|NOT adding_sql_to_cache|" + getPath() + "|" + sql);
        }
        return;
    }

    AutoPtr<SQLiteCompiledSql> compiledSql;
    {
        Mutex::Autolock lock(mCompiledQueriesLock);

        // don't insert the new mapping if a mapping already exists
        HashMap<String, AutoPtr<SQLiteCompiledSql> >::Iterator it = mCompiledQueries.Find(sql);
        if (it != mCompiledQueries.End()) {
            return;
        }
        // add this <sql, compiledStatement> to the cache
        if (mCompiledQueries.GetSize() == mMaxSqlCacheSize) {
            /*
             * cache size of {@link #mMaxSqlCacheSize} is not enough for this app.
             * log a warning MAX_WARNINGS_ON_CACHESIZE_CONDITION times
             * chances are it is NOT using ? for bindargs - so caching is useless.
             * TODO: either let the callers set max cchesize for their app, or intelligently
             * figure out what should be cached for a given app.
             */
            if (++mCacheFullWarnings == MAX_WARNINGS_ON_CACHESIZE_CONDITION) {
                // Log.w(TAG, "Reached MAX size for compiled-sql statement cache for database " +
                //         getPath() + "; i.e., NO space for this sql statement in cache: " +
                //         sql + ". Please change your sql statements to use '?' for " +
                //         "bindargs, instead of using actual values");
            }
            // don't add this entry to cache
        } else {
            // cache is NOT full. add this to cache.
            mCompiledQueries[sql] = compiledStatement;
            // if (SQLiteDebug.DEBUG_SQL_CACHE) {
            //     Log.v(TAG, "|adding_sql_to_cache|" + getPath() + "|" +
            //             mCompiledQueries.size() + "|" + sql);
            // }
        }
    }
    return;
}

void SQLiteDatabase::DeallocCachedSqlStatements()
{
    Mutex::Autolock lock(mCompiledQueriesLock);

    HashMap<String, AutoPtr<SQLiteCompiledSql> >::Iterator it;
    for (it = mCompiledQueries.Begin(); it != mCompiledQueries.End(); ++it) {
            it->mSecond->ReleaseSqlStatement();
    }
    mCompiledQueries.Clear();
}

AutoPtr<SQLiteCompiledSql> SQLiteDatabase::GetCompiledStatementForSql(
    /* [in] */ const String& sql)
{
    AutoPtr<SQLiteCompiledSql> compiledStatement;
    Boolean cacheHit;
    {
        Mutex::Autolock lock(mCompiledQueriesLock);

        if (mMaxSqlCacheSize == 0) {
            // for this database, there is no cache of compiled sql.
            // if (SQLiteDebug.DEBUG_SQL_CACHE) {
            //     Log.v(TAG, "|cache NOT found|" + getPath());
            // }
            return NULL;
        }
        HashMap<String, AutoPtr<SQLiteCompiledSql> >::Iterator it = mCompiledQueries.Find(sql);
        cacheHit = it != mCompiledQueries.End();
    }
    if (cacheHit) {
        mNumCacheHits++;
    }
    else {
        mNumCacheMisses++;
    }

    if (SQLiteDebug::DEBUG_SQL_CACHE) {
        // Log.v(TAG, "|cache_stats|" +
        //         getPath() + "|" + mCompiledQueries.size() +
        //         "|" + mNumCacheHits + "|" + mNumCacheMisses +
        //         "|" + cacheHit + "|" + mTimeOpened + "|" + mTimeClosed + "|" + sql);
    }
    return compiledStatement;
}

ECode SQLiteDatabase::IsInCompiledSqlCache(
    /* [in] */ const String& sql,
    /* [out] */ Boolean* existed)
{
    VALIDATE_NOT_NULL(existed);

    {
        Mutex::Autolock lock(mCompiledQueriesLock);

        HashMap<String, AutoPtr<SQLiteCompiledSql> >::Iterator it = mCompiledQueries.Find(sql);
        *existed = it != mCompiledQueries.End() ? TRUE : FALSE;
        return NOERROR;
    }
}

ECode SQLiteDatabase::PurgeFromCompiledSqlCache(
    /* [in] */ const String& sql)
{
   {
        Mutex::Autolock lock(mCompiledQueriesLock);

        HashMap<String, AutoPtr<SQLiteCompiledSql> >::Iterator it = mCompiledQueries.Find(sql);
        if (it != mCompiledQueries.End()) {
            mCompiledQueries.Erase(it);
        }
        return NOERROR;
   }
}

ECode SQLiteDatabase::ResetCompiledSqlCache()
{
    Mutex::Autolock lock(mCompiledQueriesLock);

    mCompiledQueries.Clear();
    return NOERROR;
}

ECode SQLiteDatabase::GetMaxSqlCacheSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    Mutex::Autolock lock(mSyncLock);

    *size = mMaxSqlCacheSize;
    return NOERROR;
}

ECode SQLiteDatabase::SetMaxSqlCacheSize(
    /* [in] */ Int32 cacheSize)
{
    Mutex::Autolock lock(mSyncLock);

    if (cacheSize > SQLiteDatabase_MAX_SQL_CACHE_SIZE || cacheSize < 0) {
        //throw new IllegalStateException("expected value between 0 and " + MAX_SQL_CACHE_SIZE);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else if (cacheSize < mMaxSqlCacheSize) {
        //throw new IllegalStateException("cannot set cacheSize to a value less than the value " +
                //"set with previous setMaxSqlCacheSize() call.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mMaxSqlCacheSize = cacheSize;
    return NOERROR;
}

// /* package */ static ArrayList<DbStats> getDbStats()
// {
//     ArrayList<DbStats> dbStatsList = new ArrayList<DbStats>();
//     for (WeakReference<SQLiteDatabase> w : ActiveDatabases.getInstance().mActiveDatabases) {
//         SQLiteDatabase db = w.get();
//         if (db == null || !db.isOpen()) {
//             continue;
//         }
//         // get SQLITE_DBSTATUS_LOOKASIDE_USED for the db
//         int lookasideUsed = db.native_getDbLookaside();

//         // get the lastnode of the dbname
//         String path = db.getPath();
//         int indx = path.lastIndexOf("/");
//         String lastnode = path.substring((indx != -1) ? ++indx : 0);

//         // get list of attached dbs and for each db, get its size and pagesize
//         ArrayList<Pair<String, String>> attachedDbs = getAttachedDbs(db);
//         if (attachedDbs == null) {
//             continue;
//         }
//         for (int i = 0; i < attachedDbs.size(); i++) {
//             Pair<String, String> p = attachedDbs.get(i);
//             long pageCount = getPragmaVal(db, p.first + ".page_count;");

//             // first entry in the attached db list is always the main database
//             // don't worry about prefixing the dbname with "main"
//             String dbName;
//             if (i == 0) {
//                 dbName = lastnode;
//             } else {
//                 // lookaside is only relevant for the main db
//                 lookasideUsed = 0;
//                 dbName = "  (attached) " + p.first;
//                 // if the attached db has a path, attach the lastnode from the path to above
//                 if (p.second.trim().length() > 0) {
//                     int idx = p.second.lastIndexOf("/");
//                     dbName += " : " + p.second.substring((idx != -1) ? ++idx : 0);
//                 }
//             }
//             if (pageCount > 0) {
//                 dbStatsList.add(new DbStats(dbName, pageCount, db.getPageSize(),
//                         lookasideUsed));
//             }
//         }
//     }
//     return dbStatsList;
// }

Int64 SQLiteDatabase::GetPragmaVal(
    /* [in] */ SQLiteDatabase* db,
    /* [in] */ const String& pragma)
{
    Boolean isOpen;
    if (db->IsOpen(&isOpen), !isOpen) {
        return 0;
    }
    AutoPtr<SQLiteStatement> prog;
    //try {
    prog = new SQLiteStatement(db, String("PRAGMA ") + pragma);
    Int64 val;
    prog->SimpleQueryForInt64(&val);
    //} finally {
    if (prog != NULL) prog->Close();
    //}
    return val;
}

// private static ArrayList<Pair<String, String>> getAttachedDbs(SQLiteDatabase dbObj) {
//     if (!dbObj.isOpen()) {
//         return null;
//     }
//     ArrayList<Pair<String, String>> attachedDbs = new ArrayList<Pair<String, String>>();
//     Cursor c = dbObj.rawQuery("pragma database_list;", null);
//     while (c.moveToNext()) {
//          attachedDbs.add(new Pair<String, String>(c.getString(1), c.getString(2)));
//     }
//     c.close();
//     return attachedDbs;
// }

static char* CreateStr(const char* path)
{
    int len = strlen(path);
    char* str = (char*)malloc(len + 1);
    strncpy(str, path, len);
    str[len] = NULL;
    return str;
}

static void SqlLogger(void* databaseName, int iErrCode, const char* zMsg)
{
    // skip printing this message if it is due to certain types of errors
    if (iErrCode == SQLITE_CONSTRAINT) return;
    // LOGI("sqlite returned: error code = %d, msg = %s\n", iErrCode, zMsg);
}

static void RegisterLoggingFunc(const char *path)
{
    static bool loggingFuncSet = false;
    if (loggingFuncSet) {
        return;
    }

    // LOGV("Registering sqlite logging func \n");
    Int32 err = sqlite3_config(SQLITE_CONFIG_LOG, &SqlLogger, (void *)CreateStr(path));
    if (err != SQLITE_OK) {
        // LOGE("sqlite_config failed error_code = %d. THIS SHOULD NEVER occur.\n", err);
        return;
    }
    loggingFuncSet = true;
}

ECode SQLiteDatabase::Dbopen(
    /* [in] */ CString pathString,
    /* [in] */ Int32 flags)
{
    Int32 err;
    sqlite3* handle = NULL;
    sqlite3_stmt* statement = NULL;
    char const * path8 = (const char *)pathString;
    Int32 sqliteFlags;
    ECode ec;

    // register the logging func on sqlite. needs to be done BEFORE any sqlite3 func is called.
    RegisterLoggingFunc(path8);

    // convert our flags into the sqlite flags
    if (flags & CREATE_IF_NECESSARY) {
        sqliteFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }
    else if (flags & OPEN_READONLY) {
        sqliteFlags = SQLITE_OPEN_READONLY;
    }
    else {
        sqliteFlags = SQLITE_OPEN_READWRITE;
    }

    err = sqlite3_open_v2(path8, &handle, sqliteFlags, NULL);
    if (err != SQLITE_OK) {
        // LOGE("sqlite3_open_v2(\"%s\", &handle, %d, NULL) failed\n", path8, sqliteFlags);
        ec = throw_sqlite3_exception(handle);
        goto done;
    }

    // The soft heap limit prevents the page cache allocations from growing
    // beyond the given limit, no matter what the max page cache sizes are
    // set to. The limit does not, as of 3.5.0, affect any other allocations.
    sqlite3_soft_heap_limit(SQLITE_SOFT_HEAP_LIMIT);

    // Set the default busy handler to retry for 1000ms and then return SQLITE_BUSY
    err = sqlite3_busy_timeout(handle, 1000 /* ms */);
    if (err != SQLITE_OK) {
        // LOGE("sqlite3_busy_timeout(handle, 1000) failed for \"%s\"\n", path8);
        ec = throw_sqlite3_exception(handle);
        goto done;
    }

#ifdef DB_INTEGRITY_CHECK
    static const char* integritySql = "pragma integrity_check(1);";
    err = sqlite3_prepare_v2(handle, integritySql, -1, &statement, NULL);
    if (err != SQLITE_OK) {
        // LOGE("sqlite_prepare_v2(handle, \"%s\") failed for \"%s\"\n", integritySql, path8);
        ec = throw_sqlite3_exception(handle);
        goto done;
    }

    // first is OK or error message
    err = sqlite3_step(statement);
    if (err != SQLITE_ROW) {
        // LOGE("integrity check failed for \"%s\"\n", integritySql, path8);
        ec = throw_sqlite3_exception(handle);
        goto done;
    }
    else {
        const char *text = (const char*)sqlite3_column_text(statement, 0);
        if (strcmp(text, "ok") != 0) {
            // LOGE("integrity check failed for \"%s\": %s\n", integritySql, path8, text);
            // jniThrowException(env, "android/database/sqlite/SQLiteDatabaseCorruptException", text);
            ec = E_SQLITEDATABASE_CORRUPT_EXCEPTION;
            goto done;
        }
    }
#endif

    err = register_android_functions(handle, UTF16_STORAGE);
    if (err) {
       ec = throw_sqlite3_exception(handle);
       goto done;
    }

    // LOGV("Opened '%s' - %p\n", path8, handle);
    mNativeHandle = handle;
    handle = NULL;  // The caller owns the handle now.

done:
    // Release allocated resources
    if (statement != NULL) sqlite3_finalize(statement);
    if (handle != NULL) sqlite3_close(handle);
    return ec;
}

void SQLiteDatabase::EnableSqlTracing(
    /* [in] */ CString path)
{
    sqlite3 * handle = mNativeHandle;

    if (path.IsNull()) {
        // LOGE("Failure in getDatabaseName(). VM ran out of memory?\n");
        assert(0);
        return; // VM would have thrown OutOfMemoryError
    }
    char *dbNameStr = CreateStr((const char *)path);
    sqlite3_trace(handle, &SqlTrace, (void *)dbNameStr);
}

void SQLiteDatabase::EnableSqlProfiling(
    /* [in] */ CString path)
{
    sqlite3 * handle = mNativeHandle;

    if (path.IsNull()) {
        // LOGE("Failure in getDatabaseName(). VM ran out of memory?\n");
        assert(0);
        return; // VM would have thrown OutOfMemoryError
    }
    char *dbNameStr = CreateStr((const char *)path);
    sqlite3_profile(handle, &SqlProfile, (void *)dbNameStr);
}

ECode SQLiteDatabase::NativeExecSQL(
    /* [in] */ CString sqlString)
{
    Int32 err;
    Int32 stepErr;
    sqlite3_stmt * statement = NULL;
    sqlite3 * handle = mNativeHandle;
    char const * sql = (const char *)sqlString;
    Int32 sqlLen = sqlString.GetLength();

    if (sql == NULL || sqlLen == 0) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", "You must supply an SQL string");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    err = sqlite3_prepare_v2(handle, sql, sqlLen, &statement, NULL);

    if (err != SQLITE_OK) {
        // LOGE("Failure %d (%s) on %p when preparing '%s'.\n", err, sqlite3_errmsg(handle), handle, sql);
        return throw_sqlite3_exception(handle);
    }

    stepErr = sqlite3_step(statement);
    err = sqlite3_finalize(statement);

    if (stepErr != SQLITE_DONE) {
        if (stepErr == SQLITE_ROW) {
            // throw_sqlite3_exception(env, "Queries cannot be performed using execSQL(), use query() instead.");
            return throw_sqlite3_exception(NULL);
        }
        else {
            // LOGE("Failure %d (%s) on %p when executing '%s'\n", err, sqlite3_errmsg(handle), handle, sql);
            return throw_sqlite3_exception(handle);
        }
    }
    else {
// #ifndef DB_LOG_STATEMENTS
//         IF_LOGV()
// #endif
//         {
//             LOGV("Success on %p when executing '%s'\n", handle, sql);
//         }
        return NOERROR;
    }
}

ECode SQLiteDatabase::NativeSetLocale(
    /* [in] */ CString localeString,
    /* [in] */ Int32 flags)
{
    if ((flags & NO_LOCALIZED_COLLATORS)) return NOERROR;

    Int32 err;
    char const* locale8 = (const char *)localeString;
    sqlite3 * handle = mNativeHandle;
    sqlite3_stmt* stmt = NULL;
    char** meta = NULL;
    Int32 rowCount, colCount;
    char* dbLocale = NULL;
    ECode ec = NOERROR;

    // create the table, if necessary and possible
    if (!(flags & OPEN_READONLY)) {
        static const char *createSql ="CREATE TABLE IF NOT EXISTS " ANDROID_TABLE " (locale TEXT)";
        err = sqlite3_exec(handle, createSql, NULL, NULL, NULL);
        if (err != SQLITE_OK) {
            // LOGE("CREATE TABLE " ANDROID_TABLE " failed\n");
            ec = throw_sqlite3_exception(handle);
            goto done;
        }
    }

    // try to read from the table
    static const char *selectSql = "SELECT locale FROM " ANDROID_TABLE " LIMIT 1";
    err = sqlite3_get_table(handle, selectSql, &meta, &rowCount, &colCount, NULL);
    if (err != SQLITE_OK) {
        // LOGE("SELECT locale FROM " ANDROID_TABLE " failed\n");
        ec = throw_sqlite3_exception(handle);
        goto done;
    }

    dbLocale = (rowCount >= 1) ? meta[colCount] : NULL;

    if (dbLocale != NULL && !strcmp(dbLocale, locale8)) {
        // database locale is the same as the desired locale; set up the collators and go
        err = register_localized_collators(handle, locale8, UTF16_STORAGE);
        if (err != SQLITE_OK)     //    throw_sqlite3_exception(env, handle);
        goto done;   // no database changes needed
    }

    if ((flags & OPEN_READONLY)) {
        // read-only database, so we're going to have to put up with whatever we got
        // For registering new index. Not for modifing the read-only database.
        err = register_localized_collators(handle, locale8, UTF16_STORAGE);
        if (err != SQLITE_OK)    //   throw_sqlite3_exception(env, handle);
        goto done;
    }

    // need to update android_metadata and indexes atomically, so use a transaction...
    err = sqlite3_exec(handle, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (err != SQLITE_OK) {
        // LOGE("BEGIN TRANSACTION failed setting locale\n");
        ec = throw_sqlite3_exception(handle);
        goto done;
    }

    err = register_localized_collators(handle, locale8, UTF16_STORAGE);
    if (err != SQLITE_OK) {
        // LOGE("register_localized_collators() failed setting locale\n");
        ec = throw_sqlite3_exception(handle);
        goto rollback;
    }

    err = sqlite3_exec(handle, "DELETE FROM " ANDROID_TABLE, NULL, NULL, NULL);
    if (err != SQLITE_OK) {
        // LOGE("DELETE failed setting locale\n");
        ec = throw_sqlite3_exception(handle);
        goto rollback;
    }

    static const char *sql = "INSERT INTO " ANDROID_TABLE " (locale) VALUES(?);";
    err = sqlite3_prepare_v2(handle, sql, -1, &stmt, NULL);
    if (err != SQLITE_OK) {
        // LOGE("sqlite3_prepare_v2(\"%s\") failed\n", sql);
        ec = throw_sqlite3_exception(handle);
        goto rollback;
    }

    err = sqlite3_bind_text(stmt, 1, locale8, -1, SQLITE_TRANSIENT);
    if (err != SQLITE_OK) {
        // LOGE("sqlite3_bind_text() failed setting locale\n");
        ec = throw_sqlite3_exception(handle);
        goto rollback;
    }

    err = sqlite3_step(stmt);
    if (err != SQLITE_OK && err != SQLITE_DONE) {
        // LOGE("sqlite3_step(\"%s\") failed setting locale\n", sql);
        ec = throw_sqlite3_exception(handle);
        goto rollback;
    }

    err = sqlite3_exec(handle, "REINDEX LOCALIZED", NULL, NULL, NULL);
    if (err != SQLITE_OK) {
        // LOGE("REINDEX LOCALIZED failed\n");
        ec = throw_sqlite3_exception(handle);
        goto rollback;
    }

    // all done, yay!
    err = sqlite3_exec(handle, "COMMIT TRANSACTION", NULL, NULL, NULL);
    if (err != SQLITE_OK) {
        // LOGE("COMMIT TRANSACTION failed setting locale\n");
        ec = throw_sqlite3_exception(handle);
        goto done;
    }

rollback:
    if (err != SQLITE_OK) {
        sqlite3_exec(handle, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
    }

done:
    if (stmt != NULL) sqlite3_finalize(stmt);
    if (meta != NULL) sqlite3_free_table(meta);
    return ec;
}

Int64 SQLiteDatabase::LastInsertRow()
{
    return sqlite3_last_insert_rowid(mNativeHandle);
}

Int32 SQLiteDatabase::LastChangeCount()
{
    return sqlite3_changes(mNativeHandle);
}

Int32 SQLiteDatabase::NativeGetDbLookaside()
{
    Int32 pCur = -1;
    Int32 unused;
    sqlite3_db_status(mNativeHandle, SQLITE_DBSTATUS_LOOKASIDE_USED, &pCur, &unused, 0);
    return pCur;
}
