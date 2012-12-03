#include "database/sqlite/SQLiteDirectCursorDriver.h"

ECode SQLiteDirectCursorDriver::SQLiteDirectCursorDriver::Init(
    /*[in]*/ ISQLiteDatabase* db,
    /*[in]*/ String sql, 
    /*[in]*/ String editTable)
{
    mDatabase = db;
    mEditTable = editTable;
    mSql = sql;
    return NOERROR;
}

SQLiteDirectCursorDriver::SQLiteDirectCursorDriver()
{
}

ECode SQLiteDirectCursorDriver::Query(
    /*[in]*/ ICursorFactory* factory,
    /*[in]*/ ArrayOf<String>* selectionArgs,
    /*[out]*/ ICursor** cs)
{/*
    // Compile the query
    AutoPtr<ISQLiteQuery> query = new SQLiteQuery(mDatabase, mSql, 0, selectionArgs);

//    try {
        // Arg binding
        Int32 numArgs = selectionArgs == NULL ? 0 : selectionArgs.GetLength();
        for (int i = 0; i < numArgs; i++) {
            query->BindString(i + 1, selectionArgs[i]);
        }

        // Create the cursor
        if (factory == NULL) {
            mCursor = new SQLiteCursor(mDatabase, this, mEditTable, query);
        } else {
            mCursor = factory->NewCursor(mDatabase, this, mEditTable, query);
        }

        mQuery = query;
        query = null;
        cs = &mCursor;
        return mCursor;
//    } finally {
        // Make sure this object is cleaned up if something happens
        if (query != NULL) query.close();
//    }
*/
    return NOERROR;
}

ECode SQLiteDirectCursorDriver::CursorClosed()
{
    mCursor = NULL;

    return NOERROR;
}

ECode SQLiteDirectCursorDriver::SetBindArguments(
        /*[in]*/ ArrayOf<String>* bindArgs)
{/*
    Int32 numArgs = bindArgs.GetLength();
    for (Int32 i = 0; i < numArgs; i++) {
        mQuery.BindString(i + 1, bindArgs[i]);
    }
*/
    return NOERROR;
}

ECode SQLiteDirectCursorDriver::CursorDeactivated()
{
	return NOERROR;
}

ECode SQLiteDirectCursorDriver::CursorRequeried(
        /*[in]*/ ICursor* cursor)
{
	return NOERROR;
}