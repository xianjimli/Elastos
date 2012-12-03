
#include "database/sqlite/CSQLiteQuery.h"
ECode CSQLiteQuery::OnAllReferencesReleased()
{
    return SQLiteQuery::OnAllReferencesReleased();
}

ECode CSQLiteQuery::OnAllReferencesReleasedFromContainer()
{
    return SQLiteQuery::OnAllReferencesReleasedFromContainer();
}

ECode CSQLiteQuery::AcquireReference()
{
    return SQLiteQuery::AcquireReference();
}

ECode CSQLiteQuery::ReleaseReference()
{
    return SQLiteQuery::ReleaseReference();
}

ECode CSQLiteQuery::ReleaseReferenceFromContainer()
{
    return SQLiteQuery::ReleaseReferenceFromContainer();
}

ECode CSQLiteQuery::GetUniqueId(
    /* [out] */ Int32 *value)
{
    return SQLiteQuery::GetUniqueId(value);
}

ECode CSQLiteQuery::GetSqlString(
    /* [out] */ String *sqlString)
{
    return SQLiteQuery::GetSqlString(sqlString);
}

ECode CSQLiteQuery::Compile(
    /* [in] */ const String& sql,
    /* [in] */ Boolean forceCompilation)
{
    return SQLiteQuery::Compile(sql,forceCompilation);
}

ECode CSQLiteQuery::BindNull(
    /* [in] */ Int32 index)
{
    return SQLiteQuery::BindNull(index);
}

ECode CSQLiteQuery::BindInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return SQLiteQuery::BindInt64(index,value);
}

ECode CSQLiteQuery::BindDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return SQLiteQuery::BindDouble(index,value);
}

ECode CSQLiteQuery::BindString(
    /* [in] */ Int32 index,
    /* [in] */ const String& value)
{
    return SQLiteQuery::BindString(index,value);
}

ECode CSQLiteQuery::BindBlob(
    /* [in] */ Int32 index,
    /* [in] */ const ArrayOf<Byte>& value)
{
    return SQLiteQuery::BindBlob(index,value);
}

ECode CSQLiteQuery::ClearBindings()
{
    return SQLiteQuery::ClearBindings();
}

ECode CSQLiteQuery::Close()
{
    return SQLiteQuery::Close();
}

ECode CSQLiteQuery::FillWindow(
    /* [in] */ ICursorWindow *window,
    /* [in] */ Int32 maxRead,
    /* [in] */ Int32 lastPos,
    /* [out] */ Int32 *value)
{
    return SQLiteQuery::FillWindow(window,maxRead,lastPos,value);
}

ECode CSQLiteQuery::ColumnCountLocked(
    /* [out] */ Int32 *value)
{
    return SQLiteQuery::ColumnCountLocked(value);
}

ECode CSQLiteQuery::ColumnNameLocked(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String *value)
{
    return SQLiteQuery::ColumnNameLocked(columnIndex,value);
}

ECode CSQLiteQuery::Requery()
{
    return SQLiteQuery::Requery();
}

ECode CSQLiteQuery::constructor(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ const String& query,
    /* [in] */ Int32 offsetIndex,
    /* [in] */ ArrayOf<String>* bindArgs)
{
    return SQLiteQuery::Init(db,query,offsetIndex,bindArgs);
}

