
#ifndef __CSQLITEDATABASE_H__
#define __CSQLITEDATABASE_H__

#include "_CSQLiteDatabase.h"

CarClass(CSQLiteDatabase)
{
public:
    CARAPI OnAllReferencesReleased();

    CARAPI OnAllReferencesReleasedFromContainer();

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI AddSQLiteClosable(
        /* [in] */ ISQLiteClosable * pClosable);

    CARAPI RemoveSQLiteClosable(
        /* [in] */ ISQLiteClosable * pClosable);

    CARAPI SetLockingEnabled(
        /* [in] */ Boolean lockingEnabled);

    CARAPI OnCorruption();

    CARAPI Lock();

    CARAPI UnLock();

    CARAPI BeginTransaction();

    CARAPI BeginTransactionWithListener(
        /* [in] */ ISQLiteTransactionListener * pTransactionListener);

    CARAPI EndTransaction();

    CARAPI SetTransactionSuccessful();

    CARAPI InTransaction(
        /* [out] */ Boolean * pResult);

    CARAPI IsDbLockedByCurrentThreads(
        /* [out] */ Boolean * pIsDbLockedByCurrentThreads);

    CARAPI YieldIfContended(
        /* [out] */ Boolean * pIsYielded);

    CARAPI YieldIfContendedSafely(
        /* [out] */ Boolean * pIsYielded);

    CARAPI YieldIfContendedSafelyEx(
        /* [in] */ Int64 sleepAfterYieldDelay,
        /* [out] */ Boolean * pIsYielded);

    CARAPI YieldIfContendedHelper(
        /* [in] */ Boolean checkFullyYielded,
        /* [in] */ Int64 sleepAfterYieldDelay,
        /* [out] */ Boolean * pIsYielded);

    CARAPI OpenDatabase(
        /* [in] */ const String& path,
        /* [in] */ ICursorFactory * pFactory,
        /* [in] */ Int32 flags,
        /* [out] */ ISQLiteDatabase ** ppDb);

    CARAPI Close();

    CARAPI GetVersion(
        /* [out] */ Int32 * pVersions);

    CARAPI SetVersion(
        /* [in] */ Int32 versions);

    CARAPI GetMaximumSize(
        /* [out] */ Int64 * pMaximumSize);

    CARAPI SetMaximumSize(
        /* [in] */ Int64 numBytes);

    CARAPI GetPageSize(
        /* [out] */ Int64 * pPageSize);

    CARAPI SetPageSize(
        /* [in] */ Int64 pageSize);

    CARAPI MarkTableSyncable(
        /* [in] */ const String& table,
        /* [in] */ const String& deletedTable);

    CARAPI MarkTableSyncableEx(
        /* [in] */ const String& table,
        /* [in] */ const String& foreignKey,
        /* [in] */ const String& updateTable);

    CARAPI RowUpdated(
        /* [in] */ const String& table,
        /* [in] */ Int64 rowId);

    CARAPI FindEditTable(
        /* [in] */ const String& tables,
        /* [out] */ String * pResultTables);

    CARAPI CompileStatement(
        /* [in] */ const String& sql,
        /* [out] */ ISQLiteStatement ** ppNewObj);

    CARAPI Query(
        /* [in] */ Boolean distinct,
        /* [in] */ const String& table,
        /* [in] */ const ArrayOf<String> & columns,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ ICursor ** ppCursor);

    CARAPI QueryWithFactory(
        /* [in] */ ICursorFactory * pCursorFactory,
        /* [in] */ Boolean distinct,
        /* [in] */ const String& table,
        /* [in] */ const ArrayOf<String> & columns,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ ICursor ** ppCursor);

    CARAPI QueryEx(
        /* [in] */ const String& table,
        /* [in] */ const ArrayOf<String> & columns,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [out] */ ICursor ** ppCursor);

    CARAPI QueryEx2(
        /* [in] */ const String& table,
        /* [in] */ const ArrayOf<String> & columns,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ ICursor ** ppCursor);

    CARAPI RawQuery(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [out] */ ICursor ** ppCursor);

    CARAPI RawQueryWithFactory(
        /* [in] */ ICursorFactory * pCursorFactory,
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [in] */ const String& editTable,
        /* [out] */ ICursor ** ppCursor);

    CARAPI RawQueryEx(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String> & selectionArgs,
        /* [in] */ Int32 initialRead,
        /* [in] */ Int32 maxRead,
        /* [out] */ ICursor ** ppCursor);

    CARAPI Insert(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues * pValues,
        /* [out] */ Int64 * pValue);

    CARAPI InsertOrThrow(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues * pValues,
        /* [out] */ Int64 * pValue);

    CARAPI Replace(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues * pInitialValues,
        /* [out] */ Int64 * pValue);

    CARAPI ReplaceOrThrow(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues * pInitialValues,
        /* [out] */ Int64 * pValue);

    CARAPI InsertWithOnConflict(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues * pInitialValues,
        /* [in] */ Int64 conflictAlgorithm,
        /* [out] */ Int64 * pValue);

    CARAPI Delete(
        /* [in] */ const String& table,
        /* [in] */ const String& whereClause,
        /* [in] */ const ArrayOf<String> & whereArgs,
        /* [out] */ Int32 * pValue);

    CARAPI Update(
        /* [in] */ const String& table,
        /* [in] */ IContentValues * pValues,
        /* [in] */ const String& whereClause,
        /* [in] */ const ArrayOf<String> & whereArgs,
        /* [out] */ Int32 * pValue);

    CARAPI UpdateWithOnConflict(
        /* [in] */ const String& table,
        /* [in] */ IContentValues * pValues,
        /* [in] */ const String& whereClause,
        /* [in] */ const ArrayOf<String> & whereArgs,
        /* [in] */ Int32 conflictAlgorithm,
        /* [out] */ Int32 * pValue);

    CARAPI ExeSQL(
        /* [in] */ const String& sql);

    CARAPI ExeSQLEx(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<IInterface *> & bindArgs);

    CARAPI IsReadOnly(
        /* [out] */ Boolean * pIsReadOnlyed);

    CARAPI IsOpen(
        /* [out] */ Boolean * pIsOpened);

    CARAPI NeedUpgrade(
        /* [in] */ Int32 newVersion,
        /* [out] */ Boolean * pResult);

    CARAPI GetPath(
        /* [out] */ String * pPath);

    CARAPI LogTimeStat(
        /* [in] */ const String& sql,
        /* [in] */ Int64 beginMillis);

    CARAPI LogTimeStatEx(
        /* [in] */ const String& sql,
        /* [in] */ Int64 beginMillis,
        /* [in] */ const String& prefix);

    CARAPI SetLocale(
        /* [in] */ ILocale * pLocale);

    CARAPI AddToCompiledQueries(
        /* [in] */ const String& sql,
        /* [in] */ ISQLiteCompiledSql * pCompiledStatement);

    CARAPI GetCompiledStatementForSql(
        /* [in] */ const String& sql,
        /* [out] */ ISQLiteCompiledSql ** ppCompiledStatement);

    CARAPI IsInCompiledSqlCache(
        /* [in] */ const String& sql,
        /* [out] */ Boolean * pIsInCompiledSqlCached);

    CARAPI PurgeFromCompiledSqlCache(
        /* [in] */ const String& sql);

    CARAPI ResetCompiledSqlCache();

    CARAPI GetMaxSqlCacheSize(
        /* [out] */ Int32 * pMaxSqlCacheSize);

    CARAPI SetMaxSqlCacheSize(
        /* [in] */ Int32 cacheSize);

    CARAPI constructor(
        /* [in] */ const String& path,
        /* [in] */ ICursorFactory * pFactory,
        /* [in] */ Int32 flags);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSQLITEDATABASE_H__
