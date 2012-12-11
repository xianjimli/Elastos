
#include "database/sqlite/SQLiteProgram.h"
#include "database/sqlite/Sqlite3Exception.h"


const CString SQLiteProgram::TAG = "SQLiteProgram";

SQLiteProgram::SQLiteProgram(
    /* [in] */ SQLiteDatabase* db,
    /* [in] */ const String& sql)
    : mDatabase(db)
    , mNativeStatement(NULL)
{
    assert(db != NULL);
    mSql = sql.Trim();
    db->AcquireReference();
    db->AddSQLiteClosable(((ISQLiteClosable*)this->Probe(EIID_ISQLiteClosable)));
    mNativeHandle = db->mNativeHandle;

    //Only cache CRUD statements
    String prefixSql = mSql.Substring(0,6);
    if (!prefixSql.EqualsIgnoreCase("INSERT") &&
        !prefixSql.EqualsIgnoreCase("UPDATE") &&
        !prefixSql.EqualsIgnoreCase("REPLAC") &&
        !prefixSql.EqualsIgnoreCase("DELETE") &&
        !prefixSql.EqualsIgnoreCase("SELECT")) {
        mCompiledSql = new SQLiteCompiledSql(db, sql);
        mNativeStatement = mCompiledSql->mNativeStatement;
        // since it is not in the cache, no need to acquire() it.
        return;
    }

    // it is not pragma
    mCompiledSql = db->GetCompiledStatementForSql(sql);
    if (mCompiledSql == NULL) {
        //create a new compiled-sql obj
        mCompiledSql = new SQLiteCompiledSql(db,sql);

        // add it to the cache of compiled-sqls
        // but before adding it and thus making it available for anyone else to use it,
        // make sure it is acquired by me.
        mCompiledSql->Acquire();
        db->AddToCompiledQueries(sql, mCompiledSql);
//        if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//            Log.v(TAG, "Created DbObj (id#" + mCompiledSql.nStatement +
//                    ") for sql: " + sql);
//        }
    }
    else {
        //it is already in compiled-sql cache
        //try to acquire the object
        if (!mCompiledSql->Acquire()) {
//            Int32 last = mCompiledSql.nStatement;
                // the SQLiteCompiledSql in cache is in use by some other SQLiteProgram object.
                // we can't have two different SQLiteProgam objects can't share the same
                // CompiledSql object. create a new one.
                // finalize it when I am done with it in "this" object.
            mCompiledSql = new SQLiteCompiledSql(db, sql);
//                if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//                    Log.v(TAG, "** possible bug ** Created NEW DbObj (id#" +
//                            mCompiledSql.nStatement +
//                            ") because the previously created DbObj (id#" + last +
//                            ") was not released for sql:" + sql);
//                }
            // since it is not in the cache, no need to acquire() it.
        }
    }
    mNativeStatement = mCompiledSql->mNativeStatement;
}

SQLiteProgram::~SQLiteProgram()
{}

//@Override
ECode SQLiteProgram::OnAllReferencesReleased()
{
    ReleaseCompiledSqlIfNotInCache();
    mDatabase->ReleaseReference();
    mDatabase->RemoveSQLiteClosable((ISQLiteClosable*)this->Probe(EIID_ISQLiteClosable));
    return NOERROR;
}

//@Override
ECode SQLiteProgram::OnAllReferencesReleasedFromContainer()
{
    ReleaseCompiledSqlIfNotInCache();
    mDatabase->ReleaseReference();
    return NOERROR;
}

void SQLiteProgram::ReleaseCompiledSqlIfNotInCache()
{
    if (mCompiledSql == NULL) {
        return;
    }
    Mutex::Autolock lock(mDatabase->mCompiledQueriesLock);

    Boolean found = FALSE;
    HashMap<String, AutoPtr<SQLiteCompiledSql> >::Iterator it;
    for (it = mDatabase->mCompiledQueries.Begin(); it != mDatabase->mCompiledQueries.End(); ++it) {
        if (it->mSecond == mCompiledSql) {
            found = TRUE;
            break;
        }
    }
    if (!found) {
        // it is NOT in compiled-sql cache. i.e., responsibility of
        // releasing this statement is on me.
        mCompiledSql->ReleaseSqlStatement();
        mCompiledSql = NULL;
        mNativeStatement = NULL;
    }
    else {
        // it is in compiled-sql cache. reset its CompiledSql#mInUse flag
        mCompiledSql->Dismiss();
    }
}

/**
 * Returns a unique identifier for this program.
 *
 * @return a unique identifier for this program
 */
Int32 SQLiteProgram::GetUniqueId()
{
    return (Int32)mNativeStatement;
}

String SQLiteProgram::GetSqlString()
{
    return mSql;
}

 /**
 * @deprecated This method is deprecated and must not be used.
 *
 * @param sql the SQL string to compile
 * @param forceCompilation forces the SQL to be recompiled in the event that there is an
 *  existing compiled SQL program already around
 */
//@Deprecated
void SQLiteProgram::Compile(
    /* [in] */ const String& sql,
    /* [in] */ Boolean forceCompilation)
{
    // TODO is there a need for this?
}

/**
 * Bind a NULL value to this statement. The value remains bound until
 * {@link #clearBindings} is called.
 *
 * @param index The 1-based index to the parameter to bind null to
 */
ECode SQLiteProgram::BindNull(
        /* [in] */ Int32 index)
{
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
    ECode ec = NativeBindNull(index);
//    } finally {
    ReleaseReference();
//    }
    return ec;
}

ECode SQLiteProgram::BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value)
{
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
    ECode ec = NativeBindInt64(index, value);
//    } finally {
    ReleaseReference();
//    }
    return ec;
}

ECode SQLiteProgram::BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value)
{
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
    ECode ec = NativeBindDouble(index, value);
//    } finally {
    ReleaseReference();
//    }
    return ec;
}

ECode SQLiteProgram::BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value)
{
    if (value.IsNull()) {
        // throw new IllegalArgumentException("the bind value at index " + index + " is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
    ECode ec = NativeBindString(index, value);
//    } finally {
    ReleaseReference();
//    }
    return ec;
}

ECode SQLiteProgram::BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value)
{
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
    ECode ec = NativeBindBlob(index, value);
//    } finally {
    ReleaseReference();
//    }
    return ec;
}

ECode SQLiteProgram::ClearBindings()
{
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
    ECode ec = NativeClearBindings();
//    } finally {
    ReleaseReference();
//    }
    return ec;
}

ECode SQLiteProgram::Close()
{
    Boolean isOpen;
    if (mDatabase->IsOpen(&isOpen), !isOpen) {
        return NOERROR;
    }
    mDatabase->Lock();
//    try {
    ECode ec = ReleaseReference();
//    } finally {
    mDatabase->Unlock();
//    }
    return ec;
}

ECode SQLiteProgram::NativeCompile(
        /* [in] */ const String& sql)
{
    // char buf[65];
    // strcpy(buf, "android_database_SQLiteProgram->native_compile() not implemented");
    // throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
    // return;
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteProgram::NativeBindNull(
        /* [in] */ Int32 index)
{
    Int32 err;

    err = sqlite3_bind_null(mNativeStatement, index);
    if (err != SQLITE_OK) {
        // char buf[32];
        // sprintf(buf, "handle %p", statement);
        // throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
        return throw_sqlite3_exception(mNativeHandle);
    }
    return NOERROR;
}

ECode SQLiteProgram::NativeBindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value)
{
    Int32 err;

    err = sqlite3_bind_int64(mNativeStatement, index, value);
    if (err != SQLITE_OK) {
        // char buf[32];
        // sprintf(buf, "handle %p", statement);
        // throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
        return throw_sqlite3_exception(mNativeHandle);
    }
    return NOERROR;
}

ECode SQLiteProgram::NativeBindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value)
{
    Int32 err;

    err = sqlite3_bind_double(mNativeStatement, index, value);
    if (err != SQLITE_OK) {
        // char buf[32];
        // sprintf(buf, "handle %p", statement);
        // throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
        return throw_sqlite3_exception(mNativeHandle);
    }
    return NOERROR;
}

ECode SQLiteProgram::NativeBindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value)
{
    Int32 err;
    char const * sql;
    Int32 sqlLen;

    sql = (const char *)value;
    sqlLen = value.GetLength();
    err = sqlite3_bind_text(mNativeStatement, index, sql, sqlLen, SQLITE_TRANSIENT);
    if (err != SQLITE_OK) {
        // char buf[32];
        // sprintf(buf, "handle %p", statement);
        // throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
        return throw_sqlite3_exception(mNativeHandle);
    }
    return NOERROR;
}

ECode SQLiteProgram::NativeBindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value)
{
    Int32 err;

    Int32 len = value.GetLength();
    Byte* bytes = value.GetPayload();

    err = sqlite3_bind_blob(mNativeStatement, index, bytes, len, SQLITE_TRANSIENT);
    if (err != SQLITE_OK) {
        // char buf[32];
        // sprintf(buf, "statement %p", statement);
        // throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
        return throw_sqlite3_exception(mNativeHandle);
    }
    return NOERROR;
}

ECode SQLiteProgram::NativeClearBindings()
{
    Int32 err;

    err = sqlite3_clear_bindings(mNativeStatement);
    if (err != SQLITE_OK) {
        return throw_sqlite3_exception(mNativeHandle);
    }
    return NOERROR;
}
