#include "database/sqlite/SQLiteProgram.h"

const String SQLiteProgram::TAG = String("SQLiteProgram");

void SQLiteProgram::Init()
{
    nHandle = 0;
    nStatement = 0;
}

ECode SQLiteProgram::GetUniqueId(
        /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = nStatement;
    return NOERROR;
}

ECode SQLiteProgram::GetSqlString(
        /* [out] */ String* value)
{
    assert(value != NULL);
    *value = mSql;
    return NOERROR;
}

ECode SQLiteProgram::Compile(
        /* [in] */ String sql,
        /* [in] */ Boolean forceCompilation)
{
    return E_NOT_IMPLEMENTED;
}

