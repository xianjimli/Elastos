
#include "database/sqlite/SQLiteDirectCursorDriver.h"
#include "database/sqlite/SQLiteQuery.h"
#include "database/sqlite/CSQLiteCursor.h"


SQLiteDirectCursorDriver::SQLiteDirectCursorDriver(
    /*[in]*/ SQLiteDatabase* db,
    /*[in]*/ const String& sql,
    /*[in]*/ const String& editTable)
    : mEditTable(editTable)
    , mDatabase(db)
    , mSql(sql)
{}

ECode SQLiteDirectCursorDriver::Query(
    /*[in]*/ ICursorFactory* factory,
    /*[in]*/ ArrayOf<String>* selectionArgs,
    /*[out]*/ ICursor** cs)
{
    // Compile the query
    AutoPtr<ISQLiteQuery> query = new SQLiteQuery(mDatabase, mSql, 0, selectionArgs);

//    try {
        // Arg binding
    Int32 numArgs = selectionArgs == NULL ? 0 : selectionArgs->GetLength();
    for (Int32 i = 0; i < numArgs; i++) {
        query->BindString(i + 1, (*selectionArgs)[i]);
    }

    // Create the cursor
    if (factory == NULL) {
        CSQLiteCursor::New((ISQLiteDatabase*)mDatabase.Get(), this, mEditTable, query, (ISQLiteCursor**)&mCursor);
    }
    else {
        factory->NewCursor((ISQLiteDatabase*)mDatabase.Get(), this, mEditTable, query, (ICursor**)&mCursor);
    }

    mQuery = query;
    *cs = mCursor;
    if (*cs != NULL) (*cs)->AddRef();
//    } finally {
    // Make sure this object is cleaned up if something happens
    if (query != NULL) query->Close();
//    }
    return NOERROR;
}

ECode SQLiteDirectCursorDriver::CursorClosed()
{
    mCursor = NULL;
    return NOERROR;
}

ECode SQLiteDirectCursorDriver::SetBindArguments(
    /*[in]*/ ArrayOf<String>* bindArgs)
{
    assert(bindArgs != NULL);

    Int32 numArgs = bindArgs->GetLength();
    for (Int32 i = 0; i < numArgs; i++) {
        mQuery->BindString(i + 1, (*bindArgs)[i]);
    }
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
