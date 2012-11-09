#include "database/sqlite/SQLiteCursor.h"

const String SQLiteCursor::TAG = String("Cursor");

const Int32 SQLiteCursor::NO_COUNT = -1;

SQLiteCursor::SQLiteCursor(
    /*[in]*/ ISQLiteDatabase* db,
    /*[in]*/ ISQLiteCursorDriver* driver,
    /*[in]*/ String editTable, 
    /*[in]*/ ISQLiteQuery* query)
{
    mCount =  NO_COUNT;
    mCursorState = 0;
    mPendingData = false;
}

SQLiteCursor::~SQLiteCursor()
{
    
}