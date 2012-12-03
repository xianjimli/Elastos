
#include "database/sqlite/CSQLiteCompiledSql.h"
ECode CSQLiteCompiledSql::ReleaseSqlStatement()
{
    // TODO: Add your code here
    return SQLiteCompiledSql::ReleaseSqlStatement();
}

ECode CSQLiteCompiledSql::Acquire(
    /* [out] */ Boolean *result)
{
    // TODO: Add your code here
    return SQLiteCompiledSql::Acquire(result);
}

ECode CSQLiteCompiledSql::constructor(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ const String& sql)
{
    // TODO: Add your code here
    return SQLiteCompiledSql::Init(db,sql);
}

