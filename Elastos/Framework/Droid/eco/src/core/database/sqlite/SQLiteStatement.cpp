
#include "database/sqlite/SQLiteStatement.h"
#include "database/sqlite/Sqlite3Exception.h"
#include <sqlite3.h>


SQLiteStatement::SQLiteStatement(
    /* [in] */ SQLiteDatabase* db,
    /* [in] */ const String& sql)
    : SQLiteProgram(db, sql)
{}

ECode SQLiteStatement::Execute()
{
//    BlockGuard.getThreadPolicy().onWriteToDisk();
    Boolean result;
    if (mDatabase->IsOpen(&result), !result) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
    ECode ec = NativeExecute();
//        mDatabase->LogTimeStat(mSql, timeStart);
//    } finally {
    ReleaseReference();
    mDatabase->Unlock();
//    }
    return ec;
}

ECode SQLiteStatement::ExecuteInsert(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

//    BlockGuard.getThreadPolicy().onWriteToDisk();
    Boolean result;
    if (mDatabase->IsOpen(&result), !result) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
    ECode ec = NativeExecute();
 //       mDatabase->LogTimeStat(mSql, timeStart);
    *value =  (mDatabase->LastChangeCount() > 0) ? mDatabase->LastInsertRow() : -1;
//    } finally {
    ReleaseReference();
    mDatabase->Unlock();
//    }
    return ec;
}

ECode SQLiteStatement::SimpleQueryForInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

//    BlockGuard.getThreadPolicy().onReadFromDisk();
    Boolean result;
    if (mDatabase->IsOpen(&result), !result) {
        // throw new IllegalStateException("database " + mDatabase.getPath() + " already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
    ECode ec = Native1x1Int64(value);
//        mDatabase->LogTimeStat(mSql, timeStart);
//    } finally {
    ReleaseReference();
    mDatabase->Unlock();
//    }
    return ec;
}

ECode SQLiteStatement::SimpleQueryForString(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

//    BlockGuard.getThreadPolicy().onReadFromDisk();
    Boolean result;
    if (mDatabase->IsOpen(&result), !result) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    long timeStart = SystemClock.uptimeMillis();
    mDatabase->Lock();

    AcquireReference();
//    try {
    ECode ec = Native1x1String(value);
//        mDatabase->LogTimeStat(mSql, timeStart);
//    } finally {
    ReleaseReference();
    mDatabase->Unlock();
//    }
    return ec;
}

ECode SQLiteStatement::NativeExecute()
{
    Int32 err;
    sqlite3* handle = mNativeHandle;
    sqlite3_stmt* statement = mNativeStatement;
    ECode ec = NOERROR;

    // Execute the statement
    err = sqlite3_step(statement);

    // Throw an exception if an error occured
    if (err != SQLITE_DONE) {
        ec = throw_sqlite3_exception_errcode(err);
    }

    // Reset the statment so it's ready to use again
    sqlite3_reset(statement);
    return ec;
}

ECode SQLiteStatement::Native1x1Int64(
    /* [out] */ Int64* value)
{
    Int32 err;
    sqlite3* handle = mNativeHandle;
    sqlite3_stmt* statement = mNativeStatement;
    ECode ec = NOERROR;
    *value = -1;

    // Execute the statement
    err = sqlite3_step(statement);

    // Handle the result
    if (err == SQLITE_ROW) {
        // No errors, read the data and return it
        *value = sqlite3_column_int64(statement, 0);
    }
    else {
        ec = throw_sqlite3_exception_errcode(err);
    }

    // Reset the statment so it's ready to use again
    sqlite3_reset(statement);

    return ec;
}

ECode SQLiteStatement::Native1x1String(
    /* [out] */ String* value)
{
    Int32 err;
    sqlite3* handle = mNativeHandle;
    sqlite3_stmt* statement = mNativeStatement;
    ECode ec = NOERROR;
    *value = NULL;

    // Execute the statement
    err = sqlite3_step(statement);

    // Handle the result
    if (err == SQLITE_ROW) {
        // No errors, read the data and return it
        char const * text = (char const *)sqlite3_column_text(statement, 0);
        *value = text;
    }
    else {
        ec = throw_sqlite3_exception_errcode(err);
    }

    // Reset the statment so it's ready to use again
    sqlite3_reset(statement);

    return ec;
}