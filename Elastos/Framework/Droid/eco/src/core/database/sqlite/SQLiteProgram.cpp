#include "database/sqlite/SQLiteProgram.h"

const String SQLiteProgram::TAG = String("SQLiteProgram");

void SQLiteProgram::Init()
{
    nHandle = 0;
    nStatement = 0;
}

SQLiteProgram::SQLiteProgram()
{
}

SQLiteProgram::~SQLiteProgram()
{
}

SQLiteProgram::SQLiteProgram(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String sql)
{
    mDatabase = db;
    mSql = sql.Trim();
    db->AcquireReference();
    db->AddSQLiteClosable(((ISQLiteClosable*)this));
//    this->nHandle = db.mNativeHandle;

    //Only cache CRUD statements
    String prefixSql = mSql.Substring(0,6);
    if (!prefixSql.EqualsIgnoreCase("INSERT") && 
        !prefixSql.EqualsIgnoreCase("UPDATE") &&
        !prefixSql.EqualsIgnoreCase("REPLAC") &&
        !prefixSql.EqualsIgnoreCase("DELETE") &&
        !prefixSql.EqualsIgnoreCase("SELECT")) {
//        mCompiledSql = new SQLiteCompiledSql(db,sql);
//        nStatement = mCompiledSql.nStatement;  
    }

    db->GetCompiledStatementForSql(sql,(ISQLiteCompiledSql**)&mCompiledSql);
    if (mCompiledSql == NULL) {
        //create a new compiled-sql obj
//       mCompiledSql = new SQLiteCompiledSql(db,sql);

        // add it to the cache of compiled-sqls
        // but before adding it and thus making it available for anyone else to use it,
        // make sure it is acquired by me.
        Boolean result;
        mCompiledSql->Acquire(&result);
        db->AddToCompiledQueries(sql,(ISQLiteCompiledSql*)mCompiledSql);
//        if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//            Log.v(TAG, "Created DbObj (id#" + mCompiledSql.nStatement +
//                    ") for sql: " + sql);
        } else {
        //it is already in compiled-sql cache
        //try to acquire the object
        Boolean result;
        mCompiledSql->Acquire(&result);
        if (!result) {
//            Int32 last = mCompiledSql.nStatement;
                // the SQLiteCompiledSql in cache is in use by some other SQLiteProgram object.
                // we can't have two different SQLiteProgam objects can't share the same
                // CompiledSql object. create a new one.
                // finalize it when I am done with it in "this" object.
//                mCompiledSql = new SQLiteCompiledSql(db, sql);
//                if (SQLiteDebug.DEBUG_ACTIVE_CURSOR_FINALIZATION) {
//                    Log.v(TAG, "** possible bug ** Created NEW DbObj (id#" +
//                            mCompiledSql.nStatement +
//                            ") because the previously created DbObj (id#" + last +
//                            ") was not released for sql:" + sql);
//                }
                // since it is not in the cache, no need to acquire() it.
        }
//        nStatement = mCompiledSql.nStatement;
    }
}

ECode SQLiteProgram::OnAllReferencesReleased()
{
    ReleaseCompiledSqlIfNotInCache();
    mDatabase->ReleaseReference();
    mDatabase->RemoveSQLiteClosable((ISQLiteClosable*)this);
    return NOERROR;
}

ECode SQLiteProgram::OnAllReferencesReleasedFromContainer()
{
    ReleaseCompiledSqlIfNotInCache();
    mDatabase->ReleaseReference();
    return NOERROR;
}

ECode SQLiteProgram::ReleaseCompiledSqlIfNotInCache()
{
    if (mCompiledSql == NULL) {
        return NOERROR;
    }
//    synchronized(mDatabase.mCompiledQueries) {
//    if (!mDatabase.mCompiledQueries.containsValue(mCompiledSql)) {
        // it is NOT in compiled-sql cache. i.e., responsibility of
        // releasing this statement is on me.
        mCompiledSql->ReleaseSqlStatement();
        mCompiledSql = NULL;
        nStatement = 0;
//    } else {
        // it is in compiled-sql cache. reset its CompiledSql#mInUse flag
        mCompiledSql->Release();
//    }
//}
    return NOERROR;
}

ECode SQLiteProgram::GetUniqueId(
        /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = nStatement;
    return NOERROR;
}

ECode SQLiteProgram::GetSqlString(
        /* [out] */ String* value)
{
    assert(value != NULL);
    *value = mSql;
    return NOERROR;
}

ECode SQLiteProgram::Compile(
        /* [in] */ String sql,
        /* [in] */ Boolean forceCompilation)
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteProgram::BindNull(
        /* [in] */ Int32 index)
{
    Boolean isOpen;
    mDatabase->IsOpen(&isOpen);
    if (!isOpen) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
//        native_bind_null(index);
//    } finally {
//        releaseReference();
//    }
    ReleaseReference();
    return NOERROR;
}

ECode SQLiteProgram::BindLong(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value)
{
    Boolean isOpen;
    mDatabase->IsOpen(&isOpen);
    if (!isOpen) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    try {
//        native_bind_long(index, value);
//    } finally {
//        releaseReference();
//    }
    ReleaseReference();
    return NOERROR;
}

ECode SQLiteProgram::BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value)
{
    Boolean isOpen;
    mDatabase->IsOpen(&isOpen);
    if (!isOpen) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    try {
//        native_bind_double(index, value);
//    } finally {
//        releaseReference();
//    }
    ReleaseReference();
    return NOERROR;
}

ECode SQLiteProgram::BindString(
        /* [in] */ Int32 index,
        /* [in] */ String value)
{
    if (value.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean isOpen;
    mDatabase->IsOpen(&isOpen);
    if (!isOpen) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    try {
//        native_bind_string(index, value);
//    } finally {
//        releaseReference();
//    }
    ReleaseReference();
    return NOERROR;
}

ECode SQLiteProgram::BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ ArrayOf<Byte>* value)
{
    if (value == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean isOpen;
    mDatabase->IsOpen(&isOpen);
    if (!isOpen) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    try {
//        native_bind_blob(index, value);
//    } finally {
//        releaseReference();
//    }
    ReleaseReference();
    return NOERROR;
}

ECode SQLiteProgram::ClearBindings()
{
    Boolean isOpen;
    mDatabase->IsOpen(&isOpen);
    if (!isOpen) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AcquireReference();
//    try {
//        native_clear_bindings();
//    } finally {
//        releaseReference();
//    }
    ReleaseReference();
    return NOERROR;
}

ECode SQLiteProgram::Close()
{
    Boolean isOpen;
    mDatabase->IsOpen(&isOpen);
    if (!isOpen) {
        return NOERROR;
    }
    mDatabase->Lock();
//    try {
//        releaseReference();;
//    } finally {
//        mDatabase.unLock();
//    }
    mDatabase->UnLock();
    return NOERROR; 
}

ECode SQLiteProgram::Native_Compile(
        /* [in] */ String sql)
{/*
    char buf[65];
    strcpy(buf, "android_database_SQLiteProgram->native_compile() not implemented");
    throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
    return;  */
    return E_NOT_IMPLEMENTED; 
}

ECode SQLiteProgram::Native_Finalize()
{/*
    char buf[66];
    strcpy(buf, "android_database_SQLiteProgram->native_finalize() not implemented");
    throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);
    return;*/
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteProgram::Native_Bind_Null(
        /* [in] */ Int32 index)
{
    Int32 err;
//    sqlite3_stmt * statement = GET_STATEMENT(env, object);
    sqlite3_stmt * statement = NULL;

    err = sqlite3_bind_null(statement, index);
    if (err != SQLITE_OK) {
/*        char buf[32];
        sprintf(buf, "handle %p", statement);
        throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);  */
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR; 
}
    
ECode SQLiteProgram::Native_Bind_Long(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value)
{
    Int32 err;
//    sqlite3_stmt * statement = GET_STATEMENT(env, object);
    sqlite3_stmt * statement = NULL;

    err = sqlite3_bind_int64(statement, index, value);
    if (err != SQLITE_OK) {
/*       char buf[32];
        sprintf(buf, "handle %p", statement);
        throw_sqlite3_exception(env, GET_HANDLE(env, object), buf); */
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR; 
}

ECode SQLiteProgram::Native_Bind_Double(
        /* [in] */ Int32 index,
        /* [in] */ Double value)
{
    Int32 err;
//    sqlite3_stmt * statement = GET_STATEMENT(env, object);
    sqlite3_stmt * statement = NULL;

    err = sqlite3_bind_double(statement, index, value);
    if (err != SQLITE_OK) {
/*        char buf[32];
        sprintf(buf, "handle %p", statement);
        throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);  */
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR; 
}

ECode SQLiteProgram::Native_Bind_String(
        /* [in] */ Int32 index,
        /* [in] */ String value)
{
    Int32 err;
    char const * sql;
    Int32 sqlLen;
    sqlite3_stmt * statement = NULL;
//  sqlite3_stmt * statement= GET_STATEMENT(env, object);

//  sql = env->GetStringChars(sqlString, NULL);
//  sqlLen = env->GetStringLength(sqlString);

    sql = (char *)&value;
    sqlLen = value.GetLength();
    err = sqlite3_bind_text16(statement, index, sql, sqlLen * 2, SQLITE_TRANSIENT);
//  env->ReleaseStringChars(sqlString, sql);
    if (err != SQLITE_OK) {
/*      char buf[32];
        sprintf(buf, "handle %p", statement);
        throw_sqlite3_exception(env, GET_HANDLE(env, object), buf);  */
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode SQLiteProgram::Native_Bind_Blob(
        /* [in] */ Int32 index,
        /* [in] */ ArrayOf<Byte> value)
{
    Int32 err;
//  sqlite3_stmt * statement= GET_STATEMENT(env, object);
    sqlite3_stmt * statement = NULL;

//  jint len = env->GetArrayLength(value);
//  jbyte * bytes = env->GetByteArrayElements(value, NULL);
    Int32 len = value.GetLength();
    Byte *bytes = value.GetPayload();


    err = sqlite3_bind_blob(statement, index, bytes, len, SQLITE_TRANSIENT);
//  env->ReleaseByteArrayElements(value, bytes, JNI_ABORT);

    if (err != SQLITE_OK) {
/*      char buf[32];
        sprintf(buf, "statement %p", statement);
        throw_sqlite3_exception(env, GET_HANDLE(env, object), buf); */
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode SQLiteProgram::Native_Clear_Bindings()
{
    Int32 err;
//  sqlite3_stmt * statement = GET_STATEMENT(env, object);
    sqlite3_stmt * statement = NULL;

    err = sqlite3_clear_bindings(statement);
    if (err != SQLITE_OK) {
//      throw_sqlite3_exception(env, GET_HANDLE(env, object));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}