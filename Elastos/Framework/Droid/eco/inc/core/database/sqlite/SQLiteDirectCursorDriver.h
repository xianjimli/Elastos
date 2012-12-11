#ifndef __SQLITEDIRECTCURSORDRIVER_H__
#define __SQLITEDIRECTCURSORDRIVER_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteDatabase.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * A cursor driver that uses the given query directly.
 *
 * @hide
 */
class SQLiteDirectCursorDriver
    : public ElRefBase
    , public ISQLiteCursorDriver
{
public:
    SQLiteDirectCursorDriver(
        /*[in]*/ SQLiteDatabase* db,
        /*[in]*/ const String& sql,
        /*[in]*/ const String& editTable);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Query(
        /*[in]*/ ICursorFactory* factory,
        /*[in]*/ ArrayOf<String>* selectionArgs,
        /*[out]*/ ICursor** cursor);

    CARAPI CursorClosed();

    CARAPI SetBindArguments(
        /*[in]*/ ArrayOf<String>* bindArgs);

    CARAPI CursorDeactivated();

    CARAPI CursorRequeried(
        /*[in]*/ ICursor* cursor);

private:
    String mEditTable;
    AutoPtr<SQLiteDatabase> mDatabase;
    AutoPtr<ICursor> mCursor;
    String mSql;
    AutoPtr<ISQLiteQuery> mQuery;
};


#endif //__SQLITEDIRECTCURSORDRIVER_H__