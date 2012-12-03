
#include "database/sqlite/CSQLiteProgram.h"
ECode CSQLiteProgram::OnAllReferencesReleased()
{
    return SQLiteProgram::OnAllReferencesReleased();
}

ECode CSQLiteProgram::OnAllReferencesReleasedFromContainer()
{
    return SQLiteProgram::OnAllReferencesReleasedFromContainer();
}

ECode CSQLiteProgram::AcquireReference()
{
    return SQLiteProgram::AcquireReference();
}

ECode CSQLiteProgram::ReleaseReference()
{
    return SQLiteProgram::ReleaseReference();
}

ECode CSQLiteProgram::ReleaseReferenceFromContainer()
{
    return SQLiteProgram::ReleaseReferenceFromContainer();
}

ECode CSQLiteProgram::GetUniqueId(
    /* [out] */ Int32 *value)
{
    return SQLiteProgram::GetUniqueId(value);
}

ECode CSQLiteProgram::GetSqlString(
    /* [out] */ String *sqlString)
{
    return SQLiteProgram::GetSqlString(sqlString);
}

ECode CSQLiteProgram::Compile(
    /* [in] */ const String& sql,
    /* [in] */ Boolean forceCompilation)
{
    return SQLiteProgram::Compile(sql, forceCompilation);
}

ECode CSQLiteProgram::BindNull(
    /* [in] */ Int32 index)
{
    return SQLiteProgram::BindNull(index);
}

ECode CSQLiteProgram::BindInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return SQLiteProgram::BindInt64(index,value);
}

ECode CSQLiteProgram::BindDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return SQLiteProgram::BindDouble(index,value);
}

ECode CSQLiteProgram::BindString(
    /* [in] */ Int32 index,
    /* [in] */ const String& value)
{
    return SQLiteProgram::BindString(index,value);
}

ECode CSQLiteProgram::BindBlob(
    /* [in] */ Int32 index,
    /* [in] */ const ArrayOf<Byte>& value)
{
    return SQLiteProgram::BindBlob(index,value);
}

ECode CSQLiteProgram::ClearBindings()
{
    return SQLiteProgram::ClearBindings();
}

ECode CSQLiteProgram::Close()
{
    return SQLiteProgram::Close();
}

ECode CSQLiteProgram::constructor(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ const String& sql)
{
//    return SQLiteProgram::Init(db,sql);
    return -1;
}

