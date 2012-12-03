
#include "database/sqlite/CSQLiteDatabase.h"
ECode CSQLiteDatabase::OnAllReferencesReleased()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::OnAllReferencesReleasedFromContainer()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::AcquireReference()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::ReleaseReference()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::ReleaseReferenceFromContainer()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::AddSQLiteClosable(
    /* [in] */ ISQLiteClosable * pClosable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::RemoveSQLiteClosable(
    /* [in] */ ISQLiteClosable * pClosable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::SetLockingEnabled(
    /* [in] */ Boolean lockingEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::OnCorruption()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::Lock()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::UnLock()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::BeginTransaction()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::BeginTransactionWithListener(
    /* [in] */ ISQLiteTransactionListener * pTransactionListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::EndTransaction()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::SetTransactionSuccessful()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::InTransaction(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::IsDbLockedByCurrentThreads(
    /* [out] */ Boolean * pIsDbLockedByCurrentThreads)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::YieldIfContended(
    /* [out] */ Boolean * pIsYielded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::YieldIfContendedSafely(
    /* [out] */ Boolean * pIsYielded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::YieldIfContendedSafelyEx(
    /* [in] */ Int64 sleepAfterYieldDelay,
    /* [out] */ Boolean * pIsYielded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::YieldIfContendedHelper(
    /* [in] */ Boolean checkFullyYielded,
    /* [in] */ Int64 sleepAfterYieldDelay,
    /* [out] */ Boolean * pIsYielded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::OpenDatabase(
    /* [in] */ const String& path,
    /* [in] */ ICursorFactory * pFactory,
    /* [in] */ Int32 flags,
    /* [out] */ ISQLiteDatabase ** ppDb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::GetVersion(
    /* [out] */ Int32 * pVersions)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::SetVersion(
    /* [in] */ Int32 versions)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::GetMaximumSize(
    /* [out] */ Int64 * pMaximumSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::SetMaximumSize(
    /* [in] */ Int64 numBytes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::GetPageSize(
    /* [out] */ Int64 * pPageSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::SetPageSize(
    /* [in] */ Int64 pageSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::MarkTableSyncable(
    /* [in] */ const String& table,
    /* [in] */ const String& deletedTable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::MarkTableSyncableEx(
    /* [in] */ const String& table,
    /* [in] */ const String& foreignKey,
    /* [in] */ const String& updateTable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::RowUpdated(
    /* [in] */ const String& table,
    /* [in] */ Int64 rowId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::FindEditTable(
    /* [in] */ const String& tables,
    /* [out] */ String * pResultTables)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::CompileStatement(
    /* [in] */ const String& sql,
    /* [out] */ ISQLiteStatement ** ppNewObj)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::Query(
    /* [in] */ Boolean distinct,
    /* [in] */ const String& table,
    /* [in] */ const ArrayOf<String> & columns,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [in] */ const String& limit,
    /* [out] */ ICursor ** ppCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::QueryWithFactory(
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
    /* [out] */ ICursor ** ppCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::QueryEx(
    /* [in] */ const String& table,
    /* [in] */ const ArrayOf<String> & columns,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [out] */ ICursor ** ppCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::QueryEx2(
    /* [in] */ const String& table,
    /* [in] */ const ArrayOf<String> & columns,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [in] */ const String& limit,
    /* [out] */ ICursor ** ppCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::RawQuery(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [out] */ ICursor ** ppCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::RawQueryWithFactory(
    /* [in] */ ICursorFactory * pCursorFactory,
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [in] */ const String& editTable,
    /* [out] */ ICursor ** ppCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::RawQueryEx(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String> & selectionArgs,
    /* [in] */ Int32 initialRead,
    /* [in] */ Int32 maxRead,
    /* [out] */ ICursor ** ppCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::Insert(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues * pValues,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::InsertOrThrow(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues * pValues,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::Replace(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues * pInitialValues,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::ReplaceOrThrow(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues * pInitialValues,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::InsertWithOnConflict(
    /* [in] */ const String& table,
    /* [in] */ const String& nullColumnHack,
    /* [in] */ IContentValues * pInitialValues,
    /* [in] */ Int64 conflictAlgorithm,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::Delete(
    /* [in] */ const String& table,
    /* [in] */ const String& whereClause,
    /* [in] */ const ArrayOf<String> & whereArgs,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::Update(
    /* [in] */ const String& table,
    /* [in] */ IContentValues * pValues,
    /* [in] */ const String& whereClause,
    /* [in] */ const ArrayOf<String> & whereArgs,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::UpdateWithOnConflict(
    /* [in] */ const String& table,
    /* [in] */ IContentValues * pValues,
    /* [in] */ const String& whereClause,
    /* [in] */ const ArrayOf<String> & whereArgs,
    /* [in] */ Int32 conflictAlgorithm,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::ExeSQL(
    /* [in] */ const String& sql)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::ExeSQLEx(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<IInterface *> & bindArgs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::IsReadOnly(
    /* [out] */ Boolean * pIsReadOnlyed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::IsOpen(
    /* [out] */ Boolean * pIsOpened)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::NeedUpgrade(
    /* [in] */ Int32 newVersion,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::GetPath(
    /* [out] */ String * pPath)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::LogTimeStat(
    /* [in] */ const String& sql,
    /* [in] */ Int64 beginMillis)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::LogTimeStatEx(
    /* [in] */ const String& sql,
    /* [in] */ Int64 beginMillis,
    /* [in] */ const String& prefix)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::SetLocale(
    /* [in] */ ILocale * pLocale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::AddToCompiledQueries(
    /* [in] */ const String& sql,
    /* [in] */ ISQLiteCompiledSql * pCompiledStatement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::GetCompiledStatementForSql(
    /* [in] */ const String& sql,
    /* [out] */ ISQLiteCompiledSql ** ppCompiledStatement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::IsInCompiledSqlCache(
    /* [in] */ const String& sql,
    /* [out] */ Boolean * pIsInCompiledSqlCached)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::PurgeFromCompiledSqlCache(
    /* [in] */ const String& sql)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::ResetCompiledSqlCache()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::GetMaxSqlCacheSize(
    /* [out] */ Int32 * pMaxSqlCacheSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::SetMaxSqlCacheSize(
    /* [in] */ Int32 cacheSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDatabase::constructor(
    /* [in] */ const String& path,
    /* [in] */ ICursorFactory * pFactory,
    /* [in] */ Int32 flags)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

