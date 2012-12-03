
#ifndef __CSQLITECOMPILEDSQL_H__
#define __CSQLITECOMPILEDSQL_H__

#include "_CSQLiteCompiledSql.h"
#include "SQLiteCompiledSql.h"

CarClass(CSQLiteCompiledSql),public SQLiteCompiledSql
{
public:
    CARAPI ReleaseSqlStatement();

    CARAPI Acquire(
        /* [out] */ Boolean *result);

    CARAPI constructor(
        /* [in] */ ISQLiteDatabase *db,
        /* [in] */ const String& sql);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSQLITECOMPILEDSQL_H__
