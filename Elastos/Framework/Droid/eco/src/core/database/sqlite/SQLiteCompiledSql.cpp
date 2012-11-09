#include "database/sqlite/SQLiteCompiledSql.h"

const String SQLiteCompiledSql::TAG = String("SQLiteCompiledSql");

void SQLiteCompiledSql::Init()
{
    nHandle = 0;
    nStatement = 0;
    mSqlStmt = NULL;
    mInUse = FALSE;
}

SQLiteCompiledSql::SQLiteCompiledSql()
{
    Init();
}

SQLiteCompiledSql::SQLiteCompiledSql(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String sql)
{
    Init();
    Boolean result;
    db->IsOpen(&result);
    if (!result) {
//        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mDatabase = db;
    mSqlStmt = sql;
//    mStackTrace = new DatabaseObjectNotClosedException().fillInStackTrace();
//    this->nHandle = db->mNativeHandle;
    Compile(sql, TRUE);
}

ECode SQLiteCompiledSql::Compile(
        /* [in] */ String sql,
        /* [in] */ Boolean forceCompilation)
{
    Boolean result;
    mDatabase->IsOpen(&result);
    if (!result) {
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
            mDatabase->UnLock();
//        }
    }
    return NOERROR;
}

ECode SQLiteCompiledSql::NativeCompile(
        /* [in] */ String sqlString)
{
    Int32 err;
    char const * sql;
    Int32 sqlLen;
//  sqlite3_stmt * statement = GET_STATEMENT(env, object);
    sqlite3_stmt * statement = NULL;
    sqlite3 * handle = NULL;

    // Make sure not to leak the statement if it already exists
    if (statement != NULL) {
        sqlite3_finalize(statement);
 //       env->SetIntField(object, gStatementField, 0);
    }

    // Compile the SQL
//    sql = env->GetStringChars(sqlString, NULL);
//    sqlLen = env->GetStringLength(sqlString);
    sql = (char *)&sqlString;
    sqlLen = sqlString.GetLength();
    err = sqlite3_prepare16_v2(handle, sql, sqlLen * 2, &statement, NULL);
//    env->ReleaseStringChars(sqlString, sql);

    if (err == SQLITE_OK) {
        // Store the statement in the Java object for future calls
//        LOGV("Prepared statement %p on %p", statement, handle);
//        env->SetIntField(object, gStatementField, (int)statement);
        return NOERROR;
    } else {
        // Error messages like 'near ")": syntax error' are not
        // always helpful enough, so construct an error string that
        // includes the query itself.
/*        const char *query = env->GetStringUTFChars(sqlString, NULL);
        char *message = (char*) malloc(strlen(query) + 50);
        if (message) {
            strcpy(message, ", while compiling: "); // less than 50 chars
            strcat(message, query);
        }
        env->ReleaseStringUTFChars(sqlString, query);
        throw_sqlite3_exception(env, handle, message);
        free(message);   */
        return !NOERROR;
    }
    return NOERROR;
}

ECode SQLiteCompiledSql::ReleaseSqlStatement()
{
    // Note that native_finalize() checks to make sure that nStatement is
    // non-null before destroying it.
    if (nStatement != 0) {
//        if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//            Log.v(TAG, "closed and deallocated DbObj (id#" + nStatement +")");
//        }
//        try {
            mDatabase->Lock();
            NativeFinalize();
            nStatement = 0;
//        } finally {
            mDatabase->UnLock();
//        }
    }
    return NOERROR;
}

ECode SQLiteCompiledSql::NativeFinalize()
{
//    sqlite3_stmt * statement = GET_STATEMENT(env, object);
    sqlite3_stmt * statement = NULL;

    if (statement != NULL) {
        sqlite3_finalize(statement);
//        env->SetIntField(object, gStatementField, 0);
    }
    return NOERROR;
}

ECode SQLiteCompiledSql::Acquire(
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if (mInUse) {
        *result = FALSE;
        return NOERROR;
    }
    mInUse = TRUE;
//    if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//        Log.v(TAG, "Acquired DbObj (id#" + nStatement + ") from DB cache");
//    }
    *result = TRUE;
    return NOERROR;
}

ECode SQLiteCompiledSql::Release()
{
//    if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//        Log.v(TAG, "Released DbObj (id#" + nStatement + ") back to DB cache");
//    }
    mInUse = FALSE;
    return NOERROR;
}

SQLiteCompiledSql::~SQLiteCompiledSql()
{

}