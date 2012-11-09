#ifndef __SQLITEDIRECTCURSORDRIVER_H__
#define __SQLITEDIRECTCURSORDRIVER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "database/sqlite/SQLiteCursorDriver.h"
#include "database/sqlite/SQLiteQuery.h"



class SQLiteDirectCursorDriver : public SQLiteCursorDriver
{
    SQLiteDirectCursorDriver(
        /*[in]*/ ISQLiteDatabase* db,
        /*[in]*/ String sql, 
        /*[in]*/ String editTable);

    CARAPI Query(
        /*[in]*/ ICursorFactory* factory,
        /*[in]*/ ArrayOf<String> selectionArgs,
        /*[out]*/ ICursor** cs);

    CARAPI CursorClosed();

    CARAPI SetBindArguments(
        /*[in]*/ ArrayOf<String> bindArgs);

    CARAPI CursorDeactivated();

    CARAPI CursorRequeried(
        /*[in]*/ ICursor* cursor);
private:
    String mEditTable; 

    AutoPtr<ISQLiteDatabase> mDatabase;

    AutoPtr<ICursor> mCursor;

    String mSql;

    AutoPtr<ISQLiteQuery> mQuery;
};


#endif //__SQLITEDIRECTCURSORDRIVER_H__