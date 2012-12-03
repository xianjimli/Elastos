
#include "database/sqlite/CSQLiteStatement.h"
ECode CSQLiteStatement::OnAllReferencesReleased()
{
    return SQLiteStatement::OnAllReferencesReleased();
}

ECode CSQLiteStatement::OnAllReferencesReleasedFromContainer()
{
    return SQLiteStatement::OnAllReferencesReleasedFromContainer();
}

ECode CSQLiteStatement::AcquireReference()
{
    return SQLiteStatement::AcquireReference();
}

ECode CSQLiteStatement::ReleaseReference()
{
    return SQLiteStatement::ReleaseReference();
}

ECode CSQLiteStatement::ReleaseReferenceFromContainer()
{
    return SQLiteStatement::ReleaseReferenceFromContainer();
}

ECode CSQLiteStatement::GetUniqueId(
    /* [out] */ Int32 *value)
{
    return SQLiteStatement::GetUniqueId(value);
}

ECode CSQLiteStatement::GetSqlString(
    /* [out] */ String *sqlString)
{
    return SQLiteStatement::GetSqlString(sqlString);
}

ECode CSQLiteStatement::Compile(
    /* [in] */ const String& sql,
    /* [in] */ Boolean forceCompilation)
{
    return SQLiteStatement::Compile(sql,forceCompilation);
}

ECode CSQLiteStatement::BindNull(
    /* [in] */ Int32 index)
{
    return SQLiteStatement::BindNull(index);
}

ECode CSQLiteStatement::BindInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return SQLiteStatement::BindInt64(index,value);
}

ECode CSQLiteStatement::BindDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return SQLiteStatement::BindDouble(index,value);
}

ECode CSQLiteStatement::BindString(
    /* [in] */ Int32 index,
    /* [in] */ const String& value)
{
    return SQLiteStatement::BindString(index,value);
}

ECode CSQLiteStatement::BindBlob(
    /* [in] */ Int32 index,
    /* [in] */ const ArrayOf<Byte>& value)
{
    return SQLiteStatement::BindBlob(index,value);
}

ECode CSQLiteStatement::ClearBindings()
{
    return SQLiteStatement::ClearBindings();
}

ECode CSQLiteStatement::Close()
{
    return SQLiteStatement::Close();
}

ECode CSQLiteStatement::Execute()
{
    return SQLiteStatement::Execute();
}

ECode CSQLiteStatement::ExecuteInsert(
    /* [out] */ Int64 *value)
{
    return SQLiteStatement::ExecuteInsert(value);
}

ECode CSQLiteStatement::SimpleQueryForLong(
    /* [out] */ Int64 *value)
{
    return SQLiteStatement::SimpleQueryForLong(value);
}

ECode CSQLiteStatement::SimpleQueryForString(
    /* [out] */ String *value)
{
    return SQLiteStatement::SimpleQueryForString(value);
}

ECode CSQLiteStatement::constructor(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ const String& sql)
{
    return SQLiteStatement::Init(db,sql);
}

