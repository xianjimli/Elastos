#include "database/sqlite/SQLiteStatement.h"
#include <sqlite3.h>

SQLiteStatement::SQLiteStatement()
{
}

SQLiteStatement::~SQLiteStatement()
{
}

SQLiteStatement::SQLiteStatement(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String sql)
{
    SQLiteProgram::SQLiteProgram(db, sql);
}

ECode SQLiteStatement::Execute()
{
//    BlockGuard.getThreadPolicy().onWriteToDisk();
    Boolean result;
    mDatabase->IsOpen(&result);
    if (!result) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
        Native_Execute();
//        mDatabase->LogTimeStat(mSql, timeStart);
//    } finally {
        ReleaseReference();
        mDatabase->UnLock();
//    }
    return NOERROR;
}

ECode SQLiteStatement::ExecuteInsert(
        /* [out] */ Int64* value)
{
//    BlockGuard.getThreadPolicy().onWriteToDisk();
    Boolean result;
    mDatabase->IsOpen(&result);
    if (!result) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
        Native_Execute();
 //       mDatabase->LogTimeStat(mSql, timeStart);
 //       return (mDatabase.lastChangeCount() > 0) ? mDatabase.lastInsertRow() : -1;
//    } finally {
        ReleaseReference();
        mDatabase->UnLock();
//    }
    return NOERROR;
}

ECode SQLiteStatement::SimpleQueryForLong(
        /* [out] */ Int64* value)
{
//    BlockGuard.getThreadPolicy().onReadFromDisk();
    Boolean result;
    mDatabase->IsOpen(&result);
    if (!result) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
//        long retValue = native_1x1_long();
//        mDatabase->LogTimeStat(mSql, timeStart);
//        *value = retValue;
//    } finally {
        ReleaseReference();
        mDatabase->UnLock();
//    }
    return NOERROR;
}

ECode SQLiteStatement::SimpleQueryForString(
        /* [out] */ String* value)
{
//    BlockGuard.getThreadPolicy().onReadFromDisk();
    Boolean result;
    mDatabase->IsOpen(&result);
    if (!result) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
//        String retValue = native_1x1_string();
//        mDatabase->LogTimeStat(mSql, timeStart);
//        return retValue;
//    } finally {
        ReleaseReference();
        mDatabase->UnLock();
//    }
    return NOERROR;
}

ECode Native_Execute()
{
    Int32 err;
/*
    sqlite3 * handle = GET_HANDLE(env, object);
    sqlite3_stmt * statement = GET_STATEMENT(env, object);
*/
 //   sqlite3 * handle = NULL;
    sqlite3_stmt * statement = NULL;

    // Execute the statement
    err = sqlite3_step(statement);

    // Throw an exception if an error occured
    if (err != SQLITE_DONE) {
//        throw_sqlite3_exception_errcode(env, err, sqlite3_errmsg(handle));
        return !NOERROR;
    }

    // Reset the statment so it's ready to use again
    sqlite3_reset(statement);
    return NOERROR;
}

Int64 Native_1x1_Long()
{
    Int32 err;
/*
    sqlite3 * handle = GET_HANDLE(env, object);
    sqlite3_stmt * statement = GET_STATEMENT(env, object);
    */
//    sqlite3 * handle = NULL;
    sqlite3_stmt * statement = NULL;
    Int64 value = -1;

    // Execute the statement
    err = sqlite3_step(statement);

    // Handle the result
    if (err == SQLITE_ROW) {
        // No errors, read the data and return it
        value = sqlite3_column_int64(statement, 0);
    } else {
//        throw_sqlite3_exception_errcode(env, err, sqlite3_errmsg(handle));
        return !NOERROR;
    }

    // Reset the statment so it's ready to use again
    sqlite3_reset(statement);

    return NOERROR;
}
    
String Native_1x1_String()
{
    Int32 err;
/*
    sqlite3 * handle = GET_HANDLE(env, object);
    sqlite3_stmt * statement = GET_STATEMENT(env, object);
    */
//    sqlite3 * handle = NULL;
    sqlite3_stmt * statement = NULL;
    String value;

    // Execute the statement
    err = sqlite3_step(statement);

    // Handle the result
    if (err == SQLITE_ROW) {
        // No errors, read the data and return it
        char const * text = (char const *)sqlite3_column_text(statement, 0);
 //       value = env->NewStringUTF(text);
        value = String(text);
    } else {
 //       throw_sqlite3_exception_errcode(env, err, sqlite3_errmsg(handle));
        return String("error");
    }

    // Reset the statment so it's ready to use again
    sqlite3_reset(statement);  

    return value;
}