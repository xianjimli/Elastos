
#ifndef __CSQLITESTATEMENT_H__
#define __CSQLITESTATEMENT_H__

#include "_CSQLiteStatement.h"
#include "SQLiteStatement.h"

CarClass(CSQLiteStatement),SQLiteStatement
{
public:
    CARAPI OnAllReferencesReleased();

    CARAPI OnAllReferencesReleasedFromContainer();

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI GetUniqueId(
        /* [out] */ Int32 *value);

    CARAPI GetSqlString(
        /* [out] */ String *sqlString);

    CARAPI Compile(
        /* [in] */ const String& sql,
        /* [in] */ Boolean forceCompilation);

    CARAPI BindNull(
        /* [in] */ Int32 index);

    CARAPI BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    CARAPI BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value);

    CARAPI ClearBindings();

    CARAPI Close();

    CARAPI Execute();

    CARAPI ExecuteInsert(
        /* [out] */ Int64 *value);

    CARAPI SimpleQueryForLong(
        /* [out] */ Int64 *value);

    CARAPI SimpleQueryForString(
        /* [out] */ String *value);

    CARAPI constructor(
        /* [in] */ ISQLiteDatabase *db,
        /* [in] */ const String& sql);
};

#endif // __CSQLITESTATEMENT_H__
