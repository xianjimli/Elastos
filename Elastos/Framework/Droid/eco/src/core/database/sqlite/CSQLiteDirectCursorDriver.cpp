
#include "database/sqlite/CSQLiteDirectCursorDriver.h"
ECode CSQLiteDirectCursorDriver::Query(
    /* [in] */ ICursorFactory *factory,
    /* [in] */ ArrayOf<String>* bindArgs,
    /* [out] */ ICursor **cursor)
{
    return SQLiteDirectCursorDriver::Query(factory,bindArgs,cursor);
}

ECode CSQLiteDirectCursorDriver::CursorDeactivated()
{
    return SQLiteDirectCursorDriver::CursorDeactivated();
}

ECode CSQLiteDirectCursorDriver::CursorRequeried(
    /* [in] */ ICursor *cursor)
{
    return SQLiteDirectCursorDriver::CursorRequeried(cursor);
}

ECode CSQLiteDirectCursorDriver::CursorClosed()
{
    return SQLiteDirectCursorDriver::CursorClosed();
}

ECode CSQLiteDirectCursorDriver::SetBindArguments(
    /* [in] */ ArrayOf<String>* bindArgs)
{
    return SQLiteDirectCursorDriver::SetBindArguments(bindArgs);
}

ECode CSQLiteDirectCursorDriver::constructor(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ const String &sql,
    /* [in] */ const String &editTable)
{
    return SQLiteDirectCursorDriver::Init(db,sql,editTable);
}

