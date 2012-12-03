
#include "database/sqlite/CSQLiteOpenHelper.h"

ECode CSQLiteOpenHelper::GetWritableDatabase(
    /* [out] */ ISQLiteDatabase **database)
{
    return SQLiteOpenHelper::GetWritableDatabase(database);
}

ECode CSQLiteOpenHelper::GetReadableDatabase(
    /* [out] */ ISQLiteDatabase **database)
{
    return SQLiteOpenHelper::GetReadableDatabase(database);
}

ECode CSQLiteOpenHelper::Close()
{
    return SQLiteOpenHelper::Close();
}

ECode CSQLiteOpenHelper::OnCreate(
    /* [in] */ ISQLiteDatabase *database)
{
    return NOERROR;
}

ECode CSQLiteOpenHelper::OnUpgrade(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ Int32 oldVersion,
    /* [in] */ Int32 newVersion)
{
    return NOERROR;
}

ECode CSQLiteOpenHelper::OnOpen(
    /* [in] */ ISQLiteDatabase *db)
{
    return SQLiteOpenHelper::OnOpen(db);
}

ECode CSQLiteOpenHelper::constructor(
    /* [in] */ IContext *dontext,
    /* [in] */ const String& name,
    /* [in] */ ICursorFactory *factory,
    /* [in] */ Int32 number)
{
    return SQLiteOpenHelper::Init(dontext,name,factory,number);
}

