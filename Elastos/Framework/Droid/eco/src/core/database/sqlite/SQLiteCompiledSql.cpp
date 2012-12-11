
#include "database/sqlite/SQLiteCompiledSql.h"
#include "database/sqlite/Sqlite3Exception.h"

const CString SQLiteCompiledSql::TAG = "SQLiteCompiledSql";

SQLiteCompiledSql::SQLiteCompiledSql(
        /* [in] */ SQLiteDatabase* db,
        /* [in] */ const String& sql)
    : mNativeHandle(NULL)
    , mNativeStatement(NULL)
    , mInUse(FALSE)
{
    Boolean isOpen;
    if (db->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + db.getPath() + " already closed");
        assert(0);
    }
    mDatabase = db;
    mSqlStmt = sql;
//    mStackTrace = new DatabaseObjectNotClosedException().fillInStackTrace();
    mNativeHandle = db->mNativeHandle;
    Compile(sql, TRUE);
}

SQLiteCompiledSql::~SQLiteCompiledSql()
{
    // try {
    if (mNativeStatement == NULL) return;
    // finalizer should NEVER get called
    // if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
    //     Log.v(TAG, "** warning ** Finalized DbObj (id#" + nStatement + ")");
    // }
    // if (StrictMode.vmSqliteObjectLeaksEnabled()) {
    //     int len = mSqlStmt.length();
    //     StrictMode.onSqliteObjectLeaked(
    //         "Releasing statement in a finalizer. Please ensure " +
    //         "that you explicitly call close() on your cursor: " +
    //         mSqlStmt.substring(0, (len > 100) ? 100 : len),
    //         mStackTrace);
    // }
    ReleaseSqlStatement();
    // } finally {
    //     super.finalize();
    // }
}

ECode SQLiteCompiledSql::Compile(
    /* [in] */ const String& sql,
    /* [in] */ Boolean forceCompilation)
{
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // Only compile if we don't have a valid statement already or the caller has
    // explicitly requested a recompile.
    if (forceCompilation) {
        mDatabase->Lock();
//        try {
            // Note that the native_compile() takes care of destroying any previously
            // existing programs before it compiles.
        NativeCompile(sql);
//        } finally {
        mDatabase->Unlock();
//        }
    }
    return NOERROR;
}

void SQLiteCompiledSql::ReleaseSqlStatement()
{
    // Note that native_finalize() checks to make sure that nStatement is
    // non-null before destroying it.
    if (mNativeStatement != NULL) {
//        if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//            Log.v(TAG, "closed and deallocated DbObj (id#" + nStatement +")");
//        }
//        try {
        mDatabase->Lock();
        NativeFinalize();
        mNativeStatement = NULL;
//        } finally {
        mDatabase->Unlock();
//        }
    }
}

Boolean SQLiteCompiledSql::Acquire()
{
    Mutex::Autolock lock(mLock);

    if (mInUse) {
        // someone already has acquired it.
        return FALSE;
    }
    mInUse = TRUE;
//    if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//        Log.v(TAG, "Acquired DbObj (id#" + nStatement + ") from DB cache");
//    }
    return TRUE;
}

void SQLiteCompiledSql::Dismiss()
{
    Mutex::Autolock lock(mLock);

//    if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//        Log.v(TAG, "Released DbObj (id#" + nStatement + ") back to DB cache");
//    }
    mInUse = FALSE;
}

ECode SQLiteCompiledSql::NativeCompile(
    /* [in] */ const String& sqlString)
{
    Int32 err;
    char const * sql;
    Int32 sqlLen;
    sqlite3_stmt* statement = mNativeStatement;

    // Make sure not to leak the statement if it already exists
    if (statement != NULL) {
        sqlite3_finalize(statement);
        mNativeStatement = NULL;
    }

    // Compile the SQL
    sql = (const char*)sqlString;
    sqlLen = sqlString.GetLength();
    err = sqlite3_prepare_v2(mNativeHandle, sql, sqlLen, &statement, NULL);

    if (err == SQLITE_OK) {
        // Store the statement in the Java object for future calls
        // LOGV("Prepared statement %p on %p", statement, mNativeHandle);
        mNativeStatement = statement;
        return NOERROR;
    }
    else {
        // Error messages like 'near ")": syntax error' are not
        // always helpful enough, so construct an error string that
        // includes the query itself.
//        const char *query = env->GetStringUTFChars(sqlString, NULL);
//        char *message = (char*) malloc(strlen(query) + 50);
//        if (message) {
//            strcpy(message, ", while compiling: "); // less than 50 chars
//            strcat(message, query);
//        }
//        env->ReleaseStringUTFChars(sqlString, query);
//        throw_sqlite3_exception(env, handle, message);
//        free(message);
        return throw_sqlite3_exception(mNativeHandle);
    }
}

void SQLiteCompiledSql::NativeFinalize()
{
    sqlite3_stmt* statement = mNativeStatement;

    if (statement != NULL) {
        sqlite3_finalize(statement);
        mNativeStatement = NULL;
    }
}



