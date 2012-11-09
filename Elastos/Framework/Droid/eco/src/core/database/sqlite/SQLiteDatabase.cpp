#include "database/sqlite/SQLiteDatabase.h"


const String SQLiteDatabase::TAG = String("Database");
const String SQLiteDatabase::COMMIT_SQL = String("COMMIT;");
const String SQLiteDatabase::LOG_SLOW_QUERIES_PROPERTY = String("db.log.slow_query_threshold");
const String SQLiteDatabase::GET_LOCK_LOG_PREFIX = String("GETLOCK:");

void SQLiteDatabase::Init()
{
    mLockAcquiredWallTime = 0L;
    mLockAcquiredThreadTime = 0L;
    mLastLockMessageTime = 0L;
    mLastSqlStatement = String(NULL);
    mPathForLogs = String(NULL);
    mTimeOpened = String(NULL);
    mTimeClosed = String(NULL);
    mMaxSqlCacheSize = MAX_SQL_CACHE_SIZE;
    mNativeHandle = 0;
    mTempTableSequence = 0;
    mLockingEnabled = TRUE;
    mSyncUpdateInfo = new HashMap<String, SyncUpdateInfo*>;
}

ECode SQLiteDatabase::AddSQLiteClosable(
        /* [in] */ ISQLiteClosable* closable)
{
    Lock();
//    mPrograms->Insert(closable, NULL);
    Unlock();
    return NOERROR;
}

ECode SQLiteDatabase::RemoveSQLiteClosable(
        /* [in] */ ISQLiteClosable* closable)
{
    Lock();
//    mPrograms->Erase(closable);
    Unlock();
    return NOERROR;
}

ECode SQLiteDatabase::OnAllReferencesReleased()
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::ReleaseMemory(
        /* [out] */ Int32* value)
{
    *value = sqlite3_release_memory(SQLITE_SOFT_HEAP_LIMIT);
    return NOERROR;
}

ECode SQLiteDatabase::SetLockingEnabled(
        /* [in] */ Boolean lockingEnabled)
{
    mLockingEnabled = lockingEnabled;
    return NOERROR;
}

ECode SQLiteDatabase::OnCorruption()
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
            IFile* file;
            CFile::New(mPath, &file);
            Boolean isSucceeded;
            file->Delete(&isSucceeded);
        }
//    }
    return NOERROR;
}

ECode SQLiteDatabase::Lock()
{
    if (!mLockingEnabled) {
        return NOERROR;
    }
/*
    mLock.lock();
    if (SQLiteDebug.DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.getHoldCount() == 1) {
            // Use elapsed real-time since the CPU may sleep when waiting for IO
            mLockAcquiredWallTime = SystemClock.elapsedRealtime();
            mLockAcquiredThreadTime = Debug.threadCpuTimeNanos();
        }
    }
*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::LockForced()
{
/*
    mLock.lock();
    if (SQLiteDebug.DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.getHoldCount() == 1) {
            // Use elapsed real-time since the CPU may sleep when waiting for IO
            mLockAcquiredWallTime = SystemClock.elapsedRealtime();
            mLockAcquiredThreadTime = Debug.threadCpuTimeNanos();
        }
    }
*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::Unlock()
{
    if (!mLockingEnabled)
    {
        return NOERROR;
    }
/*
    if (SQLiteDebug.DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.getHoldCount() == 1) {
            checkLockHoldTime();
        }
    }
    mLock.unlock();
*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::UnlockForced()
{
/*
    if (SQLiteDebug.DEBUG_LOCK_TIME_TRACKING) {
        if (mLock.getHoldCount() == 1) {
            checkLockHoldTime();
        }
    }
    mLock.unlock();
*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::CheckLockHoldTime() {
    // Use elapsed real-time since the CPU may sleep when waiting for IO
    Int64 elapsedTime = SystemClock::GetElapsedRealtime();
    Int64 lockedTime = elapsedTime - mLockAcquiredWallTime;
    Boolean pIsLoggable;
    Logger::IsLoggable(TAG, Logger::VERBOSE, &pIsLoggable);
    if (lockedTime < LOCK_ACQUIRED_WARNING_TIME_IN_MS_ALWAYS_PRINT && (!pIsLoggable) &&
            (elapsedTime - mLastLockMessageTime) < LOCK_WARNING_WINDOW_IN_MS) {
        return NOERROR;
    }

/*    if (lockedTime > LOCK_ACQUIRED_WARNING_TIME_IN_MS) {
        Int32 threadTime = (int)
                ((Debug.threadCpuTimeNanos() - mLockAcquiredThreadTime) / 1000000);
        if (threadTime > LOCK_ACQUIRED_WARNING_THREAD_TIME_IN_MS ||
                lockedTime > LOCK_ACQUIRED_WARNING_TIME_IN_MS_ALWAYS_PRINT) {
            mLastLockMessageTime = elapsedTime;
            String msg = "lock held on " + mPath + " for " + lockedTime + "ms. Thread time was "
                    + threadTime + "ms";
            if (SQLiteDebug.DEBUG_LOCK_TIME_TRACKING_STACK_TRACE) {
                Logger::D(TAG, msg, new Exception());
            } else {
                Logger::D(TAG, msg);
            }
        }
    }
*/
    return NOERROR;
}

ECode SQLiteDatabase::BeginTransaction()
{
    BeginTransactionWithListener(NULL);
    return NOERROR;
}

ECode SQLiteDatabase::BeginTransactionWithListener(
        /* [in] */ ISQLiteTransactionListener* transactionListener)
{
   LockForced();
   Boolean isOpen;
   this->IsOpen(&isOpen);
    if (!isOpen) {
//        throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
/*    Boolean ok = FALSE;
    try {
        // If this thread already had the lock then get out
        if (mLock.getHoldCount() > 1) {
            if (mInnerTransactionIsSuccessful) {
                String msg = "Cannot call beginTransaction between "
                        + "calling setTransactionSuccessful and endTransaction";
                IllegalStateException e = new IllegalStateException(msg);
                Log.e(TAG, "beginTransaction() failed", e);
                throw e;
            }
            ok = true;
            return;
        }

        // This thread didn't already have the lock, so begin a database
        // transaction now.
        execSQL("BEGIN EXCLUSIVE;");
        mTransactionListener = transactionListener;
        mTransactionIsSuccessful = TRUE;
        mInnerTransactionIsSuccessful = FALSE;
        if (transactionListener != NULL) {
            try {
                transactionListener->OnBegin();
            } catch (RuntimeException e) {
                execSQL("ROLLBACK;");
                throw e;
            }
        }
        ok = true;
    } finally {
        if (!ok) {
            // beginTransaction is called before the try block so we must release the lock in
            // the case of failure.
            unlockForced();
        }
    }
 */
    return NOERROR;
}

ECode SQLiteDatabase::EndTransaction()
{
    Boolean isOpen;
    this->IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    if (!mLock.isHeldByCurrentThread()) {
//        throw new IllegalStateException("no transaction pending");
//    }
//    try {
        if (mInnerTransactionIsSuccessful) {
            mInnerTransactionIsSuccessful = FALSE;
        } else {
            mTransactionIsSuccessful = FALSE;
        }
//        if (mLock.getHoldCount() != 1) {
//            return;
//        }
//        RuntimeException savedException = null;
        if (mTransactionListener != NULL) {
//            try {
                if (mTransactionIsSuccessful) {
                    mTransactionListener->OnCommit();
                } else {
                    mTransactionListener->OnRollback();
                }
//            } catch (RuntimeException e) {
//                savedException = e;
//                mTransactionIsSuccessful = false;
//            }
        }
        if (mTransactionIsSuccessful) {
            ExecSQL(COMMIT_SQL);
        } else {
//            try {
                String sql = String("ROLLBACK;");
                ExecSQL(sql);
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
    return NOERROR;
}

ECode SQLiteDatabase::SetTransactionSuccessful()
{
    Boolean isOpen;
    this->IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    if (!mLock.isHeldByCurrentThread()) {
//        throw new IllegalStateException("no transaction pending");
//    }
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
//    return mLock.getHoldCount() > 0;
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::IsDbLockedByCurrentThread(
        /* [out] */ Boolean* result)
{
//    return mLock.isHeldByCurrentThread();
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::IsDbLockedByOtherThreads(
        /* [out] */ Boolean* result)
{
//    return !mLock.isHeldByCurrentThread() && mLock.isLocked();
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::YieldIfContended(
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
    this->YieldIfContendedHelper(FALSE /* do not check yielding */,
                -1 /* sleepAfterYieldDelay */, result);
    return NOERROR;
}

ECode SQLiteDatabase::YieldIfContendedSafely(
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
    this->YieldIfContendedHelper(TRUE /* check yielding */, -1 /* sleepAfterYieldDelay*/, result);
    return NOERROR;
}

ECode SQLiteDatabase::YieldIfContendedSafelyEx(
        /* [in] */ Int64 sleepAfterYieldDelay,
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
    this->YieldIfContendedHelper(TRUE /* check yielding */, sleepAfterYieldDelay, result);
    return NOERROR;
}

ECode SQLiteDatabase::YieldIfContendedHelper(
        /* [in] */ Boolean checkFullyYielded,
        /* [in] */ Int64 sleepAfterYieldDelay,
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
/*    if (mLock.getQueueLength() == 0) {
        // Reset the lock acquire time since we know that the thread was willing to yield
        // the lock at this time.
        mLockAcquiredWallTime = SystemClock.elapsedRealtime();
        mLockAcquiredThreadTime = Debug.threadCpuTimeNanos();
        return false;
    }*/
    SetTransactionSuccessful();
    ISQLiteTransactionListener* transactionListener = mTransactionListener;
    EndTransaction();
    if (checkFullyYielded) {
        Boolean result;
        this->IsDbLockedByCurrentThread(&result);
        if (result) {
//            throw new IllegalStateException(
//                    "Db locked more than once. yielfIfContended cannot yield");
            return E_ILLEGAL_STATE_EXCEPTION;
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
    *result = TRUE;
    return NOERROR;
}

SQLiteDatabase::SyncUpdateInfo::SyncUpdateInfo(
        /* [in] */ String masterTable,
        /* [in] */ String deletedTable,
        /* [in] */ String foreignKey)
{
    this->masterTable = masterTable;
    this->deletedTable = deletedTable;
    this->foreignKey = foreignKey;
}

ECode SQLiteDatabase::GetSyncedTables(
        /* [out] */ HashMap<String,String>* tables)
{
    assert(tables != NULL);
//    synchronized(mSyncUpdateInfo) {
        /*HashMap<String, String>* tables = new HashMap<String, String>;
        for (String table : mSyncUpdateInfo.keySet()) {
            SyncUpdateInfo info = mSyncUpdateInfo.get(table);
            if (info.deletedTable != null) {
                tables.put(table, info.deletedTable);
            }
        }*/
        HashMap<String, String>::Iterator it = tables->Begin();
        for(;it != tables->End(); ++it) {
            String str = it->mFirst;
        //    HashMap<String, SyncUpdateInfo>::Iterator it2 = mSyncUpdateInfo->Find(str);
        //    SyncUpdateInfo info = it2->mSecond;
            
        //    if (!((info.deletedTable).IsNull() ) ) {
        //        tables->Insert(str, info.deletedTable);
        //    }
        }
//    }
    return NOERROR;
}

ECode SQLiteDatabase::OpenDatabase(
        /* [in] */ String path,
        /* [in] */ ICursorFactory* factory,
        /* [in] */ Int32 flags,
        /* [out] */ ISQLiteDatabase** sdb)
{
/*
    SQLiteDatabase sqliteDatabase = null;
    try {
        // Open the database.
        sqliteDatabase = new SQLiteDatabase(path, factory, flags);
        if (SQLiteDebug.DEBUG_SQL_STATEMENTS) {
            sqliteDatabase.enableSqlTracing(path);
        }
        if (SQLiteDebug.DEBUG_SQL_TIME) {
            sqliteDatabase.enableSqlProfiling(path);
        }
    } catch (SQLiteDatabaseCorruptException e) {
        // Try to recover from this, if we can.
        // TODO: should we do this for other open failures?
        Log.e(TAG, "Deleting and re-creating corrupt database " + path, e);
        EventLog.writeEvent(EVENT_DB_CORRUPT, path);
        if (!path.equalsIgnoreCase(":memory")) {
            // delete is only for non-memory database files
            new File(path).delete();
        }
        sqliteDatabase = new SQLiteDatabase(path, factory, flags);
    }
    ActiveDatabases.getInstance().mActiveDatabases.add(
            new WeakReference<SQLiteDatabase>(sqliteDatabase));
    return sqliteDatabase;
*/
    SQLiteDatabase* sqliteDatabase = NULL;

    // open the database
    sqliteDatabase = new SQLiteDatabase(path, factory, flags);
//    if (SQLiteDebug.DEBUG_SQL_STATEMENTS) {
        sqliteDatabase->EnableSqlTracing(path);
//    }
//    if (SQLiteDebug.DEBUG_SQL_TIME) {
        sqliteDatabase->EnableSqlProfiling(path);
//    }    
//    ActiveDatabases.getInstance().mActiveDatabases.add(
//            new WeakReference<SQLiteDatabase>(sqliteDatabase));
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::OpenOrCreateDatabaseEx(
        /* [in] */ String path,
        /* [in] */ ICursorFactory* factory,
        /* [out] */ ISQLiteDatabase** sdb)
{
    OpenDatabase(path, factory, CREATE_IF_NECESSARY, sdb);
    return NOERROR;
}

ECode SQLiteDatabase::OpenOrCreateDatabase(
        /* [in] */ IFile* file,
        /* [in] */ ICursorFactory* factory,
        /* [out] */ ISQLiteDatabase** sdb)
{
    String path;
    file->GetPath(&path);
    OpenOrCreateDatabaseEx(path, factory, sdb);
    return NOERROR;
}

ECode SQLiteDatabase::Create(
        /* [in] */ ICursorFactory* factory,
        /* [out] */ ISQLiteDatabase** sdb)
{
    // This is a magic string with special meaning for SQLite.
    String path = String(":memory:");
    OpenDatabase(path, factory, CREATE_IF_NECESSARY, sdb);
    return NOERROR;
}

ECode SQLiteDatabase::Close()
{
    Boolean isOpen;
    IsOpen(&isOpen);
    if (!isOpen) {
        return NOERROR; // already closed
    }
    Lock();
//    try {
        CloseClosable();
        // close this database instance - regardless of its reference count value
        OnAllReferencesReleased();
//    } finally {
        Unlock();
//    }
    return NOERROR;
}

ECode SQLiteDatabase::CloseClosable() {
    /* deallocate all compiled sql statement objects from mCompiledQueries cache.
     * this should be done before de-referencing all {@link SQLiteClosable} objects
     * from this database object because calling
     * {@link SQLiteClosable#onAllReferencesReleasedFromContainer()} could cause the database
     * to be closed. sqlite doesn't let a database close if there are
     * any unfinalized statements - such as the compiled-sql objects in mCompiledQueries.
     */
    DeallocCachedSqlStatements();

    HashMap<AutoPtr<ISQLiteClosable>, AutoPtr<IInterface> >::Iterator iter = mPrograms->Begin();
    while (iter != mPrograms->End()) {
        AutoPtr<ISQLiteClosable> program = iter->mFirst;
        if (program != NULL) {
            program->OnAllReferencesReleasedFromContainer();
        }
    }
    return NOERROR;
}

static void SqlTrace(void *databaseName, const char *sql) {
 //   LOGI("sql_statement|%s|%s\n", (char *)databaseName, sql);
}

static void SqlProfile(void *databaseName, const char *sql, sqlite3_uint64 tm) {
 //   Int64 d = tm/1000000.0;
//    LOGI("elapsedTime4Sql|%s|%.3f ms|%s\n", (char *)databaseName, d, sql);
}

ECode SQLiteDatabase::Dbclose()
{
//  sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;

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
//      LOGV("Closing database: handle=%p\n", handle);
        Int32 result = sqlite3_close(handle);
        if (result == SQLITE_OK) {
//          LOGV("Closed %p\n", handle);
//          env->SetIntField(object, offset_db_handle, 0);
        } else {
            // This can happen if sub-objects aren't closed first.  Make sure the caller knows.
//          throw_sqlite3_exception(env, handle);
//          LOGE("sqlite3_close(%p) failed: %d\n", handle, result);
            return !NOERROR;
        }
    }
    return NOERROR;
}

static char *CreateStr(const char *path) {
    int len = strlen(path);
    char *str = (char *)malloc(len + 1);
    strncpy(str, path, len);
    str[len] = NULL;
    return str;
}

static void SqlLogger(void *databaseName, int iErrCode, const char *zMsg) {
    // skip printing this message if it is due to certain types of errors
    if (iErrCode == SQLITE_CONSTRAINT) return;
//    LOGI("sqlite returned: error code = %d, msg = %s\n", iErrCode, zMsg);
}

static ECode RegisterLoggingFunc(const char *path) {
    static bool loggingFuncSet = false;
    if (loggingFuncSet) {
        return NOERROR;
    }

//    LOGV("Registering sqlite logging func \n");
    Int32 err = sqlite3_config(SQLITE_CONFIG_LOG, &SqlLogger, (void *)CreateStr(path));
    if (err != SQLITE_OK) {
//        LOGE("sqlite_config failed error_code = %d. THIS SHOULD NEVER occur.\n", err);
        return !NOERROR;
    }
    loggingFuncSet = true;
    return NOERROR;
}


ECode SQLiteDatabase::Dbopen(
        /* [in] */ String path,
        /* [in] */ Int32 flags)
{
    Int32 err;
    sqlite3 * handle = NULL;
    sqlite3_stmt * statement = NULL;
//  char const * path8 = env->GetStringUTFChars(pathString, NULL);
    char const * path8 = (char *)&path;
    Int32 sqliteFlags;

    // register the logging func on sqlite. needs to be done BEFORE any sqlite3 func is called.
    if(RegisterLoggingFunc(path8) != NOERROR) return !NOERROR;

    // convert our flags into the sqlite flags
    if (flags & CREATE_IF_NECESSARY) {
        sqliteFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    } else if (flags & OPEN_READONLY) {
        sqliteFlags = SQLITE_OPEN_READONLY;
    } else {
        sqliteFlags = SQLITE_OPEN_READWRITE;
    }

    err = sqlite3_open_v2(path8, &handle, sqliteFlags, NULL);
    if (err != SQLITE_OK) {
//        LOGE("sqlite3_open_v2(\"%s\", &handle, %d, NULL) failed\n", path8, sqliteFlags);
//        throw_sqlite3_exception(env, handle);
        goto done;
    }

    // The soft heap limit prevents the page cache allocations from growing
    // beyond the given limit, no matter what the max page cache sizes are
    // set to. The limit does not, as of 3.5.0, affect any other allocations.
    sqlite3_soft_heap_limit(SQLITE_SOFT_HEAP_LIMIT);

    // Set the default busy handler to retry for 1000ms and then return SQLITE_BUSY
    err = sqlite3_busy_timeout(handle, 1000 /* ms */);
    if (err != SQLITE_OK) {
//        LOGE("sqlite3_busy_timeout(handle, 1000) failed for \"%s\"\n", path8);
//        throw_sqlite3_exception(env, handle);
        goto done;
    }

#ifdef DB_INTEGRITY_CHECK
    static const char* integritySql = "pragma integrity_check(1);";
    err = sqlite3_prepare_v2(handle, integritySql, -1, &statement, NULL);
    if (err != SQLITE_OK) {
//        LOGE("sqlite_prepare_v2(handle, \"%s\") failed for \"%s\"\n", integritySql, path8);
//        throw_sqlite3_exception(env, handle);
        goto done;
    }

    // first is OK or error message
    err = sqlite3_step(statement);
    if (err != SQLITE_ROW) {
//        LOGE("integrity check failed for \"%s\"\n", integritySql, path8);
//        throw_sqlite3_exception(env, handle);
        goto done;
    } else {
        const char *text = (const char*)sqlite3_column_text(statement, 0);
        if (strcmp(text, "ok") != 0) {
//            LOGE("integrity check failed for \"%s\": %s\n", integritySql, path8, text);
//            jniThrowException(env, "android/database/sqlite/SQLiteDatabaseCorruptException", text);
            goto done;
        }
    }
#endif

//    err = register_android_functions(handle, UTF16_STORAGE);
//    if (err) {
//        throw_sqlite3_exception(env, handle);
//        goto done;
//    }

 //   LOGV("Opened '%s' - %p\n", path8, handle);
 //   env->SetIntField(object, offset_db_handle, (int) handle);
    handle = NULL;  // The caller owns the handle now.

done:
    // Release allocated resources
//    if (path8 != NULL) env->ReleaseStringUTFChars(pathString, path8);
    if (statement != NULL) sqlite3_finalize(statement);
    if (handle != NULL) sqlite3_close(handle);
    return NOERROR;
}

ECode SQLiteDatabase::EnableSqlTracing(
        /* [in] */ String path)
{
//    sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;

    if (path.IsNull()) {
//        LOGE("Failure in getDatabaseName(). VM ran out of memory?\n");
        return !NOERROR; // VM would have thrown OutOfMemoryError
    }
    char *dbNameStr = CreateStr((char *)&path);

    sqlite3_trace(handle, &SqlTrace, (void *)dbNameStr);
    return NOERROR;
}

ECode SQLiteDatabase::EnableSqlProfiling(
        /* [in] */ String path)
{
//    sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;
    if (path.IsNull()) {
//        LOGE("Failure in getDatabaseName(). VM ran out of memory?\n");
        return !NOERROR; // VM would have thrown OutOfMemoryError
    }
    char *dbNameStr = CreateStr((char *)&path);
    sqlite3_profile(handle, &SqlProfile, (void *)dbNameStr);
    return NOERROR;
}

ECode SQLiteDatabase::Native_ExecSQL(
        /* [in] */ String sqlS)
{
    Int32 err;
    Int32 stepErr;
    sqlite3_stmt * statement = NULL;
//    sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;
    char const * sql = (char *)&sqlS;
    Int32 sqlLen = sqlS.GetLength();

    if (sql == NULL || sqlLen == 0) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", "You must supply an SQL string");
        return !NOERROR;
    }

    err = sqlite3_prepare16_v2(handle, sql, sqlLen * 2, &statement, NULL);

 //   env->ReleaseStringChars(sqlString, sql);

    if (err != SQLITE_OK) {
 //       char const * sql8 = env->GetStringUTFChars(sqlString, NULL);
//        LOGE("Failure %d (%s) on %p when preparing '%s'.\n", err, sqlite3_errmsg(handle), handle, sql8);
//        throw_sqlite3_exception(env, handle, sql8);
//        env->ReleaseStringUTFChars(sqlString, sql8);
        return !NOERROR;
    }

    stepErr = sqlite3_step(statement);
    err = sqlite3_finalize(statement);

    if (stepErr != SQLITE_DONE) {
        if (stepErr == SQLITE_ROW) {
 //           throw_sqlite3_exception(env, "Queries cannot be performed using execSQL(), use query() instead.");
        } else {
 //           char const * sql8 = env->GetStringUTFChars(sqlString, NULL);
 //           LOGE("Failure %d (%s) on %p when executing '%s'\n", err, sqlite3_errmsg(handle), handle, sql8);
 //           throw_sqlite3_exception(env, handle, sql8);
 //           env->ReleaseStringUTFChars(sqlString, sql8);

        }
    } else
#ifndef DB_LOG_STATEMENTS
//   IF_LOGV()
#endif
    {
//       char const * sql8 = env->GetStringUTFChars(sqlString, NULL);
//        LOGV("Success on %p when executing '%s'\n", handle, sql8);
//        env->ReleaseStringUTFChars(sqlString, sql8);
    }
    return NOERROR;
}

ECode SQLiteDatabase::Native_SetLocale(
        /* [in] */ String loc,
        /* [in] */ Int32 flags)
{
    if ((flags & NO_LOCALIZED_COLLATORS)) return NOERROR;

    Int32 err;
    char const* locale8 = (char *)&loc;
//    sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;
    sqlite3_stmt* stmt = NULL;
    char** meta = NULL;
    Int32 rowCount, colCount;
    char* dbLocale = NULL;

    // create the table, if necessary and possible
    if (!(flags & OPEN_READONLY)) {
        static const char *createSql ="CREATE TABLE IF NOT EXISTS " ANDROID_TABLE " (locale TEXT)";
        err = sqlite3_exec(handle, createSql, NULL, NULL, NULL);
        if (err != SQLITE_OK) {
//            LOGE("CREATE TABLE " ANDROID_TABLE " failed\n");
//            throw_sqlite3_exception(env, handle);
            goto done;
        }
    }

    // try to read from the table
    static const char *selectSql = "SELECT locale FROM " ANDROID_TABLE " LIMIT 1";
    err = sqlite3_get_table(handle, selectSql, &meta, &rowCount, &colCount, NULL);
    if (err != SQLITE_OK) {
//        LOGE("SELECT locale FROM " ANDROID_TABLE " failed\n");
//        throw_sqlite3_exception(env, handle);
        goto done;
    }

    dbLocale = (rowCount >= 1) ? meta[colCount] : NULL;
/*
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
*/
    // need to update android_metadata and indexes atomically, so use a transaction...
    err = sqlite3_exec(handle, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (err != SQLITE_OK) {
//        LOGE("BEGIN TRANSACTION failed setting locale\n");
//        throw_sqlite3_exception(env, handle);
        goto done;
    }
/*
    err = register_localized_collators(handle, locale8, UTF16_STORAGE);
    if (err != SQLITE_OK) {
//        LOGE("register_localized_collators() failed setting locale\n");
//        throw_sqlite3_exception(env, handle);
        goto rollback;
    }
*/
    err = sqlite3_exec(handle, "DELETE FROM " ANDROID_TABLE, NULL, NULL, NULL);
    if (err != SQLITE_OK) {
//        LOGE("DELETE failed setting locale\n");
//        throw_sqlite3_exception(env, handle);
        goto rollback;
    }

    static const char *sql = "INSERT INTO " ANDROID_TABLE " (locale) VALUES(?);";
    err = sqlite3_prepare_v2(handle, sql, -1, &stmt, NULL);
    if (err != SQLITE_OK) {
//        LOGE("sqlite3_prepare_v2(\"%s\") failed\n", sql);
//        throw_sqlite3_exception(env, handle);
        goto rollback;
    }

    err = sqlite3_bind_text(stmt, 1, locale8, -1, SQLITE_TRANSIENT);
    if (err != SQLITE_OK) {
//        LOGE("sqlite3_bind_text() failed setting locale\n");
//        throw_sqlite3_exception(env, handle);
        goto rollback;
    }

    err = sqlite3_step(stmt);
    if (err != SQLITE_OK && err != SQLITE_DONE) {
//        LOGE("sqlite3_step(\"%s\") failed setting locale\n", sql);
//        throw_sqlite3_exception(env, handle);
        goto rollback;
    }

    err = sqlite3_exec(handle, "REINDEX LOCALIZED", NULL, NULL, NULL);
    if (err != SQLITE_OK) {
//        LOGE("REINDEX LOCALIZED failed\n");
//        throw_sqlite3_exception(env, handle);
        goto rollback;
    }

    // all done, yay!
    err = sqlite3_exec(handle, "COMMIT TRANSACTION", NULL, NULL, NULL);
    if (err != SQLITE_OK) {
//        LOGE("COMMIT TRANSACTION failed setting locale\n");
//        throw_sqlite3_exception(env, handle);
        goto done;
    }

rollback:
    if (err != SQLITE_OK) {
        sqlite3_exec(handle, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
    }

done:
//    if (locale8 != NULL) env->ReleaseStringUTFChars(localeString, locale8);
    if (stmt != NULL) sqlite3_finalize(stmt);
    if (meta != NULL) sqlite3_free_table(meta);
    return NOERROR;
}

Int64 SQLiteDatabase::LastInsertRow()
{
 //   sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;
    return (Int64)sqlite3_last_insert_rowid(handle);
}

Int32 SQLiteDatabase::LastChangeCount()
{
//    sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;
    return sqlite3_changes(handle);
}

Int32 SQLiteDatabase::Native_GetDbLookaside()
{
//    sqlite3 * handle = (sqlite3 *)env->GetIntField(object, offset_db_handle);
    sqlite3 * handle = NULL;
    Int32 pCur = -1;
    Int32 unused;
    sqlite3_db_status(handle, SQLITE_DBSTATUS_LOOKASIDE_USED, &pCur, &unused, 0);
    return pCur;
}


ECode SQLiteDatabase::GetVersion(
        /* [out] */ Int32* version)
{
    assert(version != NULL);
    SQLiteStatement* prog = NULL;
    Lock();
    Boolean isOpen;
    this->IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
        const String stmt = String("PRAGMA user_version;");
        prog = new SQLiteStatement((ISQLiteDatabase*)this, stmt);
        Int64 ver;
        prog->SimpleQueryForLong(&ver);
        *version = (Int32)ver;
        //return NOERROR;
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
    ExecSQL(sql);
    return NOERROR;
}

ECode SQLiteDatabase::GetMaximumSize(
        /* [out] */ Int64* maxSize)
{
    assert(maxSize != NULL);
    SQLiteStatement* prog = NULL;
    Lock();
    Boolean isOpen;
    this->IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
        const String stmt = String("PRAGMA max_page_count;");
        prog = new SQLiteStatement((ISQLiteDatabase*)this, stmt);
        Int64 pageCount, pageSize;
        prog->SimpleQueryForLong(&pageCount);
        this->GetPageSize(&pageSize);
        *maxSize = pageCount * pageSize;
        //return NOERROR;
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
    SQLiteStatement* prog = NULL;
    Lock();
    Boolean isOpen;
    this->IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
        Int64 pageSize;
        this->GetPageSize(&pageSize);
        Int64 numPages = numBytes / pageSize;
        // If numBytes isn't a multiple of pageSize, bump up a page
        if ((numBytes % pageSize) != 0) {
            numPages++;
        }
        String sql = String("PRAGMA max_page_count = ") + String::FromInt64(numPages);
        prog = new SQLiteStatement((ISQLiteDatabase*)this, sql);
        Int64 newPageCount;
        prog->SimpleQueryForLong(&newPageCount);
        *maxSize = newPageCount * pageSize;
        //return NOERROR;
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
    assert(pageSize != NULL);
    SQLiteStatement* prog = NULL;
    Lock();
    Boolean isOpen;
    this->IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
        String sql = String("PRAGMA page_size;");
        prog = new SQLiteStatement((ISQLiteDatabase*)this, sql);
        Int64 size;
        prog->SimpleQueryForLong(&size);
        *pageSize = size;
        //return NOERROR;
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
    ExecSQL(sql);
    return NOERROR;
}

ECode SQLiteDatabase::MarkTableSyncable(
        /* [in] */ String table,
        /* [in] */ String deletedTable)
{
    MarkTableSyncableEx2(table, String("_id"), table, deletedTable);
    return NOERROR;
}

ECode SQLiteDatabase::MarkTableSyncableEx(
        /* [in] */ String table,
        /* [in] */ String foreignKey,
        /* [in] */ String updateTable)
{
    MarkTableSyncableEx2(table, foreignKey, updateTable, String(NULL) );
    return NOERROR;
}

ECode SQLiteDatabase::MarkTableSyncableEx2(
        /* [in] */ String table,
        /* [in] */ String foreignKey,
        /* [in] */ String updateTable,
        /* [in] */ String deletedTable)
{
    Lock();
    //try {
        Native_ExecSQL(String("SELECT _sync_dirty FROM ") + updateTable
                + String(" LIMIT 0") );
        Native_ExecSQL(String("SELECT ") + foreignKey + String(" FROM ") + table
                + String(" LIMIT 0") );
    //} finally {
        Unlock();
    //}

//    SyncUpdateInfo* info = new SyncUpdateInfo(updateTable, deletedTable, foreignKey);
    //synchronized (mSyncUpdateInfo) {
     //   mSyncUpdateInfo->Insert(table, *info);
    //}
    return NOERROR;
}

ECode SQLiteDatabase::RowUpdated(
        /* [in] */ String table,
        /* [in] */ Int32 rowId)
{
    SyncUpdateInfo* info;
    //synchronized (mSyncUpdateInfo) {
        HashMap<String, SyncUpdateInfo*>::Iterator find = mSyncUpdateInfo->Find(table);
        if(find != mSyncUpdateInfo->End()) {
            info = find->mSecond;
        }
    //}
    if (info != NULL) {
        ExecSQL(String("UPDATE ") + info->masterTable
                + String(" SET _sync_dirty=1 WHERE _id=(SELECT ") + info->foreignKey
                + String(" FROM ") + table + String(" WHERE _id=") + String::FromInt32(rowId) + String(")") );
    }
    return NOERROR;
}

ECode SQLiteDatabase::FindEditTable(
        /* [in] */ String tables,
        /* [out] */ String* editable)
{
/*    if (!TextUtils.isEmpty(tables)) {
        // find the first word terminated by either a space or a comma
        int spacepos = tables.indexOf(' ');
        int commapos = tables.indexOf(',');

        if (spacepos > 0 && (spacepos < commapos || commapos < 0)) {
            return tables.substring(0, spacepos);
        } else if (commapos > 0 && (commapos < spacepos || spacepos < 0) ) {
            return tables.substring(0, commapos);
        }
        return tables;
    } else {
        throw new IllegalStateException("Invalid tables");
    }*/
    return NOERROR;
}

ECode SQLiteDatabase::CompileStatement(
        /* [in] */ String sql,
        /* [out] */ ISQLiteStatement** stmt)
{
    Lock();
    Boolean isOpen;
    IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
    //    CSQLiteStatement::New((ISQLiteDatabase*)this, sql, stmt);
    //} finally {
        Unlock();
    //}
    return NOERROR;
}

ECode SQLiteDatabase::Query(
        /* [in] */ Boolean distinct,
        /* [in] */ String table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ String selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ String groupBy,
        /* [in] */ String having,
        /* [in] */ String orderBy,
        /* [in] */ String limit,
        /* [out] */ ICursor** cursor)
{
    QueryWithFactory(NULL, distinct, table, columns, selection, selectionArgs,
            groupBy, having, orderBy, limit, cursor);
    return NOERROR;
}

ECode SQLiteDatabase::QueryWithFactory(
        /* [in] */ ICursorFactory* cursorFactory,
        /* [in] */ Boolean distinct,
        /* [in] */ String table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ String selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ String groupBy,
        /* [in] */ String having,
        /* [in] */ String orderBy,
        /* [in] */ String limit,
        /* [out] */ ICursor** cursor)
{
    Boolean isOpen;
    IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    String sql, editTable;
//    SQLiteQueryBuilder::BuildQueryString(
//            distinct, table, columns, selection, groupBy, having, orderBy, limit, &sql);
    FindEditTable(table, &editTable);
    RawQueryWithFactory(
            cursorFactory, sql, selectionArgs, editTable, cursor);
    return NOERROR;
}

ECode SQLiteDatabase::QueryEx(
        /* [in] */ String table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ String selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ String groupBy,
        /* [in] */ String having,
        /* [in] */ String orderBy,
        /* [out] */ ICursor** cursor)
{
    Query(FALSE, table, columns, selection, selectionArgs, groupBy,
                having, orderBy, String(NULL) /* limit */, cursor);
    return NOERROR; 
}

ECode SQLiteDatabase::QueryEx2(
        /* [in] */ String table, 
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ String selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ String groupBy,
        /* [in] */ String having,
        /* [in] */ String orderBy,
        /* [in] */ String limit,
        /* [out] */ ICursor** cursor)
{
    Query(FALSE, table, columns, selection, selectionArgs, groupBy,
            having, orderBy, limit, cursor);
    return NOERROR;
}

ECode SQLiteDatabase::RawQuery(
        /* [in] */ String sql,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ ICursor** cursor)
{
    RawQueryWithFactory(NULL, sql, selectionArgs, String(NULL), cursor);
    return NOERROR;
}

ECode SQLiteDatabase::RawQueryWithFactory(
        /* [in] */ ICursorFactory* cursorFactory,
        /* [in] */ String sql,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ String editTable,
        /* [out] */ ICursor** cursor)
{
    /*if (!isOpen()) {
        throw new IllegalStateException("database not open");
    }
    BlockGuard.getThreadPolicy().onReadFromDisk();
    long timeStart = 0;

    if (Config.LOGV || mSlowQueryThreshold != -1) {
        timeStart = System.currentTimeMillis();
    }

    SQLiteCursorDriver driver = new SQLiteDirectCursorDriver(this, sql, editTable);

    Cursor cursor = null;
    try {
        cursor = driver.query(
                cursorFactory != null ? cursorFactory : mFactory,
                selectionArgs);
    } finally {
        if (Config.LOGV || mSlowQueryThreshold != -1) {

            // Force query execution
            int count = -1;
            if (cursor != null) {
                count = cursor.getCount();
            }

            long duration = System.currentTimeMillis() - timeStart;

            if (Config.LOGV || duration >= mSlowQueryThreshold) {
                Log.v(SQLiteCursor.TAG,
                      "query (" + duration + " ms): " + driver.toString() + ", args are "
                              + (selectionArgs != null
                              ? TextUtils.join(",", selectionArgs)
                              : "<null>")  + ", count is " + count);
            }
        }
    }
    return cursor;*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::RawQueryEx(
        /* [in] */ String sql,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ Int32 initialRead,
        /* [in] */ Int32 maxRead,
        /* [out] */ ICursor** cursor)
{
/*    SQLiteCursor c = (SQLiteCursor)rawQueryWithFactory(
            null, sql, selectionArgs, null);
    c.setLoadStyle(initialRead, maxRead);
    return c;*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::Insert(
        /* [in] */ String table,
        /* [in] */ String nullColumnHack,
        /* [in] */ IContentValues* values,
        /* [out] */ Int64* value)
{
//    try {
        InsertWithOnConflict(table, nullColumnHack, values, CONFLICT_NONE, value);
        return NOERROR;
//    } catch (SQLException e) {
//        Log.e(TAG, "Error inserting " + values, e);
//        return -1;
//    }
}

ECode SQLiteDatabase::InsertOrThrow(
        /* [in] */ String table,
        /* [in] */ String nullColumnHack,
        /* [in] */ IContentValues* values,
        /* [out] */ Int64* value)
{
    InsertWithOnConflict(table, nullColumnHack, values, CONFLICT_NONE, value);
    return NOERROR;
}

ECode SQLiteDatabase::Replace(
        /* [in] */ String table,
        /* [in] */ String nullColumnHack,
        /* [in] */ IContentValues* initialValues,
        /* [out] */ Int64* value)
{
//    try {
        InsertWithOnConflict(table, nullColumnHack, initialValues, CONFLICT_REPLACE, value);
        return NOERROR;
//    } catch (SQLException e) {
//        Log.e(TAG, "Error inserting " + initialValues, e);
//        return -1;
//    }
}

ECode SQLiteDatabase::ReplaceOrThrow(
        /* [in] */ String table,
        /* [in] */ String nullColumnHack,
        /* [in] */ IContentValues* initialValues,
        /* [out] */ Int64* value)
{
    InsertWithOnConflict(table, nullColumnHack, initialValues, CONFLICT_REPLACE, value);
    return NOERROR;
}

ECode SQLiteDatabase::InsertWithOnConflict(
        /* [in] */ String table,
        /* [in] */ String nullColumnHack,
        /* [in] */ IContentValues* initialValues,
        /* [in] */ Int32 conflictAlgorithm,
        /* [out] */ Int64* value)
{
    //BlockGuard.getThreadPolicy().onWriteToDisk();
    Boolean isOpen;
    IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // Measurements show most sql lengths <= 152
    //StringBuilder sql = new StringBuilder(152);
    StringBuffer sql = StringBuffer("INSERT");
    //sql += CONFLICT_VALUES[conflictAlgorithm];
    sql += " INFO ";
    sql += table;

    // Measurements show most values lengths < 40
    //StringBuilder values = new StringBuilder(40);
    StringBuffer values;

    /*Set<Map.Entry<String, Object>> entrySet = null;
    if (initialValues != null && initialValues.size() > 0) {
        entrySet = initialValues.valueSet();
        Iterator<Map.Entry<String, Object>> entriesIter = entrySet.iterator();
        sql.append('(');

        boolean needSeparator = false;
        while (entriesIter.hasNext()) {
            if (needSeparator) {
                sql.append(", ");
                values.append(", ");
            }
            needSeparator = true;
            Map.Entry<String, Object> entry = entriesIter.next();
            sql.append(entry.getKey());
            values.append('?');
        }

        sql.append(')');
    } else {
        sql.append("(" + nullColumnHack + ") ");
        values.append("NULL");
    }

    sql.append(" VALUES(");
    sql.append(values);
    sql.append(");");

    lock();
    SQLiteStatement statement = null;
    try {
        statement = compileStatement(sql.toString());

        // Bind the values
        if (entrySet != null) {
            int size = entrySet.size();
            Iterator<Map.Entry<String, Object>> entriesIter = entrySet.iterator();
            for (int i = 0; i < size; i++) {
                Map.Entry<String, Object> entry = entriesIter.next();
                DatabaseUtils.bindObjectToProgram(statement, i + 1, entry.getValue());
            }
        }

        // Run the program and then cleanup
        statement.execute();

        long insertedRowId = lastInsertRow();
        if (insertedRowId == -1) {
            Log.e(TAG, "Error inserting " + initialValues + " using " + sql);
        } else {
            if (Config.LOGD && Log.isLoggable(TAG, Log.VERBOSE)) {
                Log.v(TAG, "Inserting row " + insertedRowId + " from "
                        + initialValues + " using " + sql);
            }
        }
        return insertedRowId;
    } catch (SQLiteDatabaseCorruptException e) {
        onCorruption();
        throw e;
    } finally {
        if (statement != null) {
            statement.close();
        }
        unlock();
    }*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::Delete(
        /* [in] */ String table,
        /* [in] */ String whereClause,
        /* [in] */ ArrayOf<String>* whereArgs,
        /* [out] */ Int32* value)
{
    //BlockGuard.getThreadPolicy().onWriteToDisk();
    Lock();
    Boolean isOpen;
    IsOpen(&isOpen);
    if (!isOpen) {
        //throw new IllegalStateException("database not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<ISQLiteStatement> statement = NULL;
//    try {
        //CompileStatement("DELETE FROM " + table
        //        + (!TextUtils::IsEmpty(&whereClause)
        //        ? " WHERE " + whereClause : ""), &statement);
        if (whereArgs != NULL) {
            Int32 numArgs = whereArgs->GetLength();
            for (Int32 i = 0; i < numArgs; i++) {
                //DatabaseUtils.bindObjectToProgram(statement, i + 1, whereArgs[i]);
            }
        }
        statement->Execute();
        *value = LastChangeCount();
        return NOERROR;
//    } catch (SQLiteDatabaseCorruptException e) {
//        onCorruption();
//        throw e;
//    } finally {
        if (statement != NULL) {
            statement->Close();
        }
        Unlock();
//    }
    return NOERROR;
}

ECode SQLiteDatabase::Update(
        /* [in] */ String table,
        /* [in] */ IContentValues* values,
        /* [in] */ String whereClause,
        /* [in] */ ArrayOf<String>* whereArgs,
        /* [out] */ Int32* value)
{
    UpdateWithOnConflict(table, values, whereClause, whereArgs, CONFLICT_NONE, value);
    return NOERROR;
}

ECode SQLiteDatabase::UpdateWithOnConflict(
        /* [in] */ String table,
        /* [in] */ IContentValues* values,
        /* [in] */ String whereClause,
        /* [in] */ ArrayOf<String>* whereArgs,
        /* [in] */ Int32 conflictAlgorithm,
        /* [out] */ Int32* value)
{
/*    BlockGuard.getThreadPolicy().onWriteToDisk();
    if (values == null || values.size() == 0) {
        throw new IllegalArgumentException("Empty values");
    }

    StringBuilder sql = new StringBuilder(120);
    sql.append("UPDATE ");
    sql.append(CONFLICT_VALUES[conflictAlgorithm]);
    sql.append(table);
    sql.append(" SET ");

    Set<Map.Entry<String, Object>> entrySet = values.valueSet();
    Iterator<Map.Entry<String, Object>> entriesIter = entrySet.iterator();

    while (entriesIter.hasNext()) {
        Map.Entry<String, Object> entry = entriesIter.next();
        sql.append(entry.getKey());
        sql.append("=?");
        if (entriesIter.hasNext()) {
            sql.append(", ");
        }
    }

    if (!TextUtils.isEmpty(whereClause)) {
        sql.append(" WHERE ");
        sql.append(whereClause);
    }

    lock();
    if (!isOpen()) {
        throw new IllegalStateException("database not open");
    }
    SQLiteStatement statement = null;
    try {
        statement = compileStatement(sql.toString());

        // Bind the values
        int size = entrySet.size();
        entriesIter = entrySet.iterator();
        int bindArg = 1;
        for (int i = 0; i < size; i++) {
            Map.Entry<String, Object> entry = entriesIter.next();
            DatabaseUtils.bindObjectToProgram(statement, bindArg, entry.getValue());
            bindArg++;
        }

        if (whereArgs != null) {
            size = whereArgs.length;
            for (int i = 0; i < size; i++) {
                statement.bindString(bindArg, whereArgs[i]);
                bindArg++;
            }
        }

        // Run the program and then cleanup
        statement.execute();
        int numChangedRows = lastChangeCount();
        if (Config.LOGD && Log.isLoggable(TAG, Log.VERBOSE)) {
            Log.v(TAG, "Updated " + numChangedRows + " using " + values + " and " + sql);
        }
        return numChangedRows;
    } catch (SQLiteDatabaseCorruptException e) {
        onCorruption();
        throw e;
    } catch (SQLException e) {
        Log.e(TAG, "Error updating " + values + " using " + sql);
        throw e;
    } finally {
        if (statement != null) {
            statement.close();
        }
        unlock();
    }*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::ExecSQL(
        /* [in] */ String sql)
{
/*    BlockGuard.getThreadPolicy().onWriteToDisk();
    long timeStart = SystemClock.uptimeMillis();
    lock();
    if (!isOpen()) {
        throw new IllegalStateException("database not open");
    }
    logTimeStat(mLastSqlStatement, timeStart, GET_LOCK_LOG_PREFIX);
    try {
        native_execSQL(sql);
    } catch (SQLiteDatabaseCorruptException e) {
        onCorruption();
        throw e;
    } finally {
        unlock();
    }

    // Log commit statements along with the most recently executed
    // SQL statement for disambiguation.  Note that instance
    // equality to COMMIT_SQL is safe here.
    if (sql == COMMIT_SQL) {
        logTimeStat(mLastSqlStatement, timeStart, COMMIT_SQL);
    } else {
        logTimeStat(sql, timeStart, null);
    }*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::ExecSQLEx(
        /* [in] */ String sql,
        /* [in] */ ArrayOf<IInterface*>* bindArgs)
{
/*    BlockGuard.getThreadPolicy().onWriteToDisk();
    if (bindArgs == null) {
        throw new IllegalArgumentException("Empty bindArgs");
    }
    long timeStart = SystemClock.uptimeMillis();
    lock();
    if (!isOpen()) {
        throw new IllegalStateException("database not open");
    }
    SQLiteStatement statement = null;
    try {
        statement = compileStatement(sql);
        if (bindArgs != null) {
            int numArgs = bindArgs.length;
            for (int i = 0; i < numArgs; i++) {
                DatabaseUtils.bindObjectToProgram(statement, i + 1, bindArgs[i]);
            }
        }
        statement.execute();
    } catch (SQLiteDatabaseCorruptException e) {
        onCorruption();
        throw e;
    } finally {
        if (statement != null) {
            statement.close();
        }
        unlock();
    }
    logTimeStat(sql, timeStart);*/
    return E_NOT_IMPLEMENTED;
}

SQLiteDatabase::~SQLiteDatabase()
{
    Boolean isOpen;
    IsOpen(&isOpen);
    if (isOpen) {
//        Log.e(TAG, "close() was never explicitly called on database '" +
//                mPath + "' ", mStackTrace);
        CloseClosable();
        OnAllReferencesReleased();
    }
}

SQLiteDatabase::SQLiteDatabase(
    /* [in] */ String path,
    /* [in] */ ICursorFactory* factory,
    /* [in] */ Int32 flags)
{
    if (path.IsNull()) {
        //throw new IllegalArgumentException("path should not be null");
    }
    mFlags = flags;
    mPath = path;
//    mSlowQueryThreshold = SystemProperties.getInt(LOG_SLOW_QUERIES_PROPERTY, -1);
//    mStackTrace = new DatabaseObjectNotClosedException().fillInStackTrace();
    mFactory = factory;
    Dbopen(mPath, mFlags);
//    if (SQLiteDebug.DEBUG_SQL_CACHE) {
//        mTimeOpened = getTime();
//    }
/*    mPrograms = new WeakHashMap<SQLiteClosable,Object>();
    try {
        setLocale(Locale.getDefault());
    } catch (RuntimeException e) {
        Log.e(TAG, "Failed to setLocale() when constructing, closing the database", e);
        dbclose();
        if (SQLiteDebug.DEBUG_SQL_CACHE) {
            mTimeClosed = getTime();
        }
        throw e;
    }*/
}

ECode SQLiteDatabase::GetTime(
        /* [out] */ String* time)
{
    //return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS ").format(System.currentTimeMillis());
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::IsReadOnly(
        /* [out] */ Boolean* isReadOnly)
{
    assert(isReadOnly != NULL);
    *isReadOnly = (mFlags & OPEN_READ_MASK) == OPEN_READONLY;
    return NOERROR;
}

ECode SQLiteDatabase::IsOpen(
        /* [out] */ Boolean* isOpen)
{
    assert(isOpen != NULL);
    *isOpen = mNativeHandle != 0;
    return NOERROR;
}

ECode SQLiteDatabase::NeedUpgrade(
        /* [in] */ Int32 newVersion,
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
    Int32 version;
    GetVersion(&version);
    *result = newVersion > version;
    return NOERROR;
}

ECode SQLiteDatabase::GetPath(
        /* [out] */ String* path)
{
    assert(path != NULL);
    *path = mPath;
    return NOERROR;
}

ECode SQLiteDatabase::LogTimeStat(
        /* [in] */ String sql,
        /* [in] */ Int64 beginMillis)
{
    LogTimeStatEx(sql, beginMillis, String(NULL) );
    return NOERROR;
}

ECode SQLiteDatabase::LogTimeStatEx(
        /* [in] */ String sql,
        /* [in] */ Int64 beginMillis,
        /* [in] */ String prefix)
{
/*    // Keep track of the last statement executed here, as this is
    // the common funnel through which all methods of hitting
    // libsqlite eventually flow.
    mLastSqlStatement = sql;

    // Sample fast queries in proportion to the time taken.
    // Quantize the % first, so the logged sampling probability
    // exactly equals the actual sampling rate for this query.

    int samplePercent;
    long durationMillis = SystemClock.uptimeMillis() - beginMillis;
    if (durationMillis == 0 && prefix == GET_LOCK_LOG_PREFIX) {
        // The common case is locks being uncontended.  Don't log those,
        // even at 1%, which is our default below.
        return;
    }
    if (sQueryLogTimeInMillis == 0) {
        sQueryLogTimeInMillis = SystemProperties.getInt("db.db_operation.threshold_ms", 500);
    }
    if (durationMillis >= sQueryLogTimeInMillis) {
        samplePercent = 100;
    } else {;
        samplePercent = (int) (100 * durationMillis / sQueryLogTimeInMillis) + 1;
        if (mRandom.nextInt(100) >= samplePercent) return;
    }

    // Note: the prefix will be "COMMIT;" or "GETLOCK:" when non-null.  We wait to do
    // it here so we avoid allocating in the common case.
    if (prefix != null) {
        sql = prefix + sql;
    }

    if (sql.length() > QUERY_LOG_SQL_LENGTH) sql = sql.substring(0, QUERY_LOG_SQL_LENGTH);

    // ActivityThread.currentPackageName() only returns non-null if the
    // current thread is an application main thread.  This parameter tells
    // us whether an event loop is blocked, and if so, which app it is.
    //
    // Sadly, there's no fast way to determine app name if this is *not* a
    // main thread, or when we are invoked via Binder (e.g. ContentProvider).
    // Hopefully the full path to the database will be informative enough.

    String blockingPackage = AppGlobals.getInitialPackage();
    if (blockingPackage == null) blockingPackage = "";

    EventLog.writeEvent(
        EVENT_DB_OPERATION,
        getPathForLogs(),
        sql,
        durationMillis,
        blockingPackage,
        samplePercent);*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::GetPathForLogs(
        /* [out] */ String* pathForLogs)
{
    assert(pathForLogs != NULL);
    if (!mPathForLogs.IsNull()) {
        *pathForLogs = mPathForLogs;
        return NOERROR;
    }
    if (mPath.IsNull()) {
        *pathForLogs = String(NULL);
        return NOERROR;
    }
    if (mPath.IndexOf('@') == -1) {
        mPathForLogs = mPath;
    } else {
        //mPathForLogs = EMAIL_IN_DB_PATTERN.matcher(mPath).replaceAll("XX@YY");
    }
    *pathForLogs = mPathForLogs;
    return NOERROR;
}

ECode SQLiteDatabase::SetLocale(
        /* [in] */ ILocale* locale)
{
    Lock();
//    try {
//        Native_SetLocale(locale.toString(), mFlags);
//    } finally {
        Unlock();
//    }
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::AddToCompiledQueries(
        /* [in] */ String sql,
        /* [in] */ ISQLiteCompiledSql* compiledStatement)
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::DeallocCachedSqlStatements()
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteDatabase::GetCompiledStatementForSql(
        /* [in] */ String sql,
        /* [in] */ ISQLiteCompiledSql* compiledStatement)
{
    return E_NOT_IMPLEMENTED;   
}

ECode SQLiteDatabase::IsInCompiledSqlCache(
        /* [in] */ String sql,
        /* [out] */ Boolean* isInCompiledSqlCache)
{
    assert(isInCompiledSqlCache != NULL);
//    synchronized(mCompiledQueries) {
        Map<String, AutoPtr<ISQLiteCompiledSql> >::Iterator it = mCompiledQueries->Find(sql);
        if (it != mCompiledQueries->End()) {
            *isInCompiledSqlCache = TRUE;
        }
        return NOERROR;
//    }
}

ECode SQLiteDatabase::PurgeFromCompiledSqlCache(
        /* [in] */ String sql)
{
//    synchronized(mCompiledQueries) {
        Map<String, AutoPtr<ISQLiteCompiledSql> >::Iterator it = mCompiledQueries->Find(sql);
        if (it != mCompiledQueries->End()) {
            mCompiledQueries->Erase(it);
        }
        return NOERROR;
//    }
}

ECode SQLiteDatabase::ResetCompiledSqlCache()
{
//    synchronized(mCompiledQueries) {
        mCompiledQueries->Clear();
        return NOERROR;
//    }
}

ECode SQLiteDatabase::GetMaxSqlCacheSize(
        /* [out] */ Int32* maxSqlCacheSize)
{
    assert(maxSqlCacheSize != NULL);
    *maxSqlCacheSize = mMaxSqlCacheSize;
    return NOERROR;
}

ECode SQLiteDatabase::SetMaxSqlCacheSize(
        /* [in] */ Int32 cacheSize)
{
    if (cacheSize > MAX_SQL_CACHE_SIZE || cacheSize < 0) {
        //throw new IllegalStateException("expected value between 0 and " + MAX_SQL_CACHE_SIZE);
        return E_ILLEGAL_STATE_EXCEPTION;
    } else if (cacheSize < mMaxSqlCacheSize) {
        //throw new IllegalStateException("cannot set cacheSize to a value less than the value " +
                //"set with previous setMaxSqlCacheSize() call.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mMaxSqlCacheSize = cacheSize;
    return NOERROR;
}

ECode SQLiteDatabase::GetPragmaVal(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String pragma,
        /* [out] */ Int64* pragmaVal)
{
    assert(pragmaVal != NULL);
    Boolean isOpen;
    db->IsOpen(&isOpen);
    if (!isOpen) {
        *pragmaVal = 0L;
        return NOERROR;
    }
    SQLiteStatement *prog = NULL;
    //try {
        prog = new SQLiteStatement(db, String("PRAGMA ") + pragma);
        Int64 val;
        prog->SimpleQueryForLong(&val);
        *pragmaVal = val;
    //} finally {
        if (prog != NULL) prog->Close();
    //}
    return NOERROR;
}