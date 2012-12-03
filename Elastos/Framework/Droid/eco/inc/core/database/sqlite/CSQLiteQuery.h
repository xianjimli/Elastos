
#ifndef __CSQLITEQUERY_H__
#define __CSQLITEQUERY_H__

#include "_CSQLiteQuery.h"
#include "SQLiteQuery.h"

CarClass(CSQLiteQuery),public SQLiteQuery
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

    CARAPI FillWindow(
        /* [in] */ ICursorWindow *sindow,
        /* [in] */ Int32 maxRead,
        /* [in] */ Int32 lastPos,
        /* [out] */ Int32 *value);

    CARAPI ColumnCountLocked(
        /* [out] */ Int32 *value);

    CARAPI ColumnNameLocked(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String *value);

    CARAPI Requery();

    CARAPI constructor(
        /* [in] */ ISQLiteDatabase *db,
        /* [in] */ const String& query,
        /* [in] */ Int32 offsetIndex,
        /* [in] */ ArrayOf<String>* bindArgs);
};

#endif // __CSQLITEQUERY_H__
