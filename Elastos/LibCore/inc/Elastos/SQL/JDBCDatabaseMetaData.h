#ifdef _JDBCDATABASEMETADATA_H_
#define _JDBCDATABASEMETADATA_H_

#include <elastos.h>
#include <elastos/AutoPtr.h>
using namespace Elastos;

class JDBCDatabaseMetaData
{
private:
    AutoPtr<IJDBCConnection> mConn;

    void Int32ernalImportedKeys(
        /* [in] */String table, 
        /* [in] */String pktable,
        /* [in] */JDBCResultSet in, 
        /* [in] */TableResultX out);

public:
    JDBCDatabaseMetaData();

    ~JDBCDatabaseMetaData();

    CARAPI Init(
        /* [in] */IJDBCConnection* conn);

    CARAPI AllProceduresAreCallable(
        /* [out] */Boolean* result);

    CARAPI AllTablesAreSelectable(
        /* [out] */Boolean* result);

    CARAPI GetURL(
        /* [out] */String* result);

    CARAPI GetUserName(
        /* [out] */String* result);

    CARAPI IsReadOnly(
        /* [out] */Boolean* result);

    CARAPI NullsAreSortedHigh(
        /* [out] */Boolean* result);

    CARAPI NullsAreSortedLow(
        /* [out] */Boolean* result);

    CARAPI NullsAreSortedAtStart(
        /* [out] */Boolean* result);

    CARAPI NullsAreSortedAtEnd(
        /* [out] */Boolean* result);

    CARAPI GetDatabaseProductName(
        /* [out] */String* result);

    CARAPI GetDatabaseProductVersion(
        /* [out] */String* result);

    CARAPI GetDriverName(
        /* [out] */String* result);

    CARAPI GetDriverVersion(
        /* [out] */String* result);

    CARAPI GetDriverMajorVersion(
        /* [out] */Int32* result);

    CARAPI GetDriverMinorVersion(
        /* [out] */Int32* result);

    CARAPI UsesLocalFiles(
        /* [out] */Boolean* result);

    CARAPI UsesLocalFilePerTable(
        /* [out] */Boolean* result);

    CARAPI SupportsMixedCaseIdentifiers(
        /* [out] */Boolean* result);

    CARAPI StoresUpperCaseIdentifiers(
        /* [out] */Boolean* result);

    CARAPI StoresLowerCaseIdentifiers(
        /* [out] */Boolean* result);

    CARAPI StoresMixedCaseIdentifiers(
        /* [out] */Boolean* result);

    CARAPI SupportsMixedCaseQuotedIdentifiers(
        /* [out] */Boolean* result);

    CARAPI StoresUpperCaseQuotedIdentifiers(
        /* [out] */Boolean* result);

    CARAPI StoresLowerCaseQuotedIdentifiers(
        /* [out] */Boolean* result);

    CARAPI StoresMixedCaseQuotedIdentifiers(
        /* [out] */Boolean* result);

    CARAPI GetIdentifierQuoteString(
        /* [out] */String* result);

    CARAPI GetSQLKeywords(
        /* [out] */String* result);

    CARAPI GetNumericFunctions(
        /* [out] */String* result);

    CARAPI GetStringFunctions(
        /* [out] */String* result);

    CARAPI GetSystemFunctions(
        /* [out] */String* result);

    CARAPI getTimeDateFunctions(
        /* [out] */String* result);

    CARAPI GetSearchStringEscape(
        /* [out] */String* result);

    CARAPI GetExtraNameCharacters(
        /* [out] */String* result);

    CARAPI SsupportsAlterTableWithAddColumn(
        /* [out] */Boolean* result);

    CARAPI SupportsAlterTableWithDropColumn(
        /* [out] */Boolean* result);

    CARAPI SupportsColumnAliasing(
        /* [out] */Boolean* result);

    CARAPI NullPlusNonNullIsNull(
        /* [out] */Boolean* result);
    
    CARAPI SupportsConvert(
        /* [out] */Boolean* result);

    CARAPI SupportsConvert(
        /* [in] */Int32 fromType, 
        /* [in] */Int32 toType,
        /* [out] */Boolean* result);

    CARAPI SupportsTableCorrelationNames(
        /* [out] */Boolean* result);

    CARAPI SupportsDifferentTableCorrelationNames(
        /* [out] */Boolean* result);

    CARAPI SupportsExpressionsInOrderBy(
        /* [out] */Boolean* result);

    CARAPI SupportsOrderByUnrelated(
        /* [out] */Boolean* result);

    CARAPI SupportsGroupBy(
        /* [out] */Boolean* result);

    CARAPI SupportsGroupByUnrelated(
        /* [out] */Boolean* result);

    CARAPI SupportsGroupByBeyondSelect(
        /* [out] */Boolean* result);

    CARAPI SupportsLikeEscapeClause(
        /* [out] */Boolean* result);

    CARAPI SupportsMultipleResultSets(
        /* [out] */Boolean* result);

    CARAPI SupportsMultipleTransactions(
        /* [out] */Boolean* result);

    CARAPI SupportsNonNullableColumns(
        /* [out] */Boolean* result);

    CARAPI SupportsMinimumSQLGrammar(
        /* [out] */Boolean* result);

    CARAPI SupportsCoreSQLGrammar(
        /* [out] */Boolean* result);

    CARAPI SupportsExtendedSQLGrammar(
        /* [out] */Boolean* result);

    CARAPI SupportsANSI92EntryLevelSQL(
        /* [out] */Boolean* result);

    CARAPI SupportsANSI92Int32ermediateSQL(
        /* [out] */Boolean* result);

    CARAPI SupportsANSI92FullSQL(
        /* [out] */Boolean* result);

    CARAPI SupportsInt32egrityEnhancementFacility(
        /* [out] */Boolean* result);

    CARAPI SupportsOuterJoins(
        /* [out] */Boolean* result);

    CARAPI SupportsFullOuterJoins(
        /* [out] */Boolean* result);

    CARAPI SupportsLimitedOuterJoins(
        /* [out] */Boolean* result);

    CARAPI GetSchemaTerm(
        /* [out] */String* result);

    CARAPI GetProcedureTerm(
        /* [out] */String* result);

    CARAPI GetCatalogTerm(
        /* [out] */String* result);

    CARAPI IsCatalogAtStart(
        /* [out] */Boolean* result);

    CARAPI GetCatalogSeparator(
        /* [out] */String* result);

    CARAPI SupportsSchemasInDataManipulation(
        /* [out] */Boolean* result);

    CARAPI SupportsSchemasInProcedureCalls(
        /* [out] */Boolean* result);

    CARAPI SupportsSchemasInt32ableDefinitions(
        /* [out] */Boolean* result);
    
    CARAPI SupportsSchemasInIndexDefinitions(
        /* [out] */Boolean* result);

    CARAPI SupportsSchemasInPrivilegeDefinitions(
        /* [out] */Boolean* result);

    CARAPI SupportsCatalogsInDataManipulation(
        /* [out] */Boolean* result);

    CARAPI SupportsCatalogsInProcedureCalls(
        /* [out] */Boolean* result);

    CARAPI SupportsCatalogsInt32ableDefinitions(
        /* [out] */Boolean* result);

    CARAPI SupportsCatalogsInIndexDefinitions(
        /* [out] */Boolean* result);

    CARAPI SupportsCatalogsInPrivilegeDefinitions(
        /* [out] */Boolean* result);

    CARAPI SupportsPositionedDelete(
        /* [out] */Boolean* result);

    CARAPI SupportsPositionedUpdate(
        /* [out] */Boolean* result);

    CARAPI SupportsSelectForUpdate(
        /* [out] */Boolean* result);

    CARAPI SupportsStoredProcedures(
        /* [out] */Boolean* result);

    CARAPI SupportsSubqueriesInComparisons(
        /* [out] */Boolean* result);

    CARAPI SupportsSubqueriesInExists(
        /* [out] */Boolean* result);

    CARAPI SupportsSubqueriesInIns(
        /* [out] */Boolean* result);

    CARAPI SupportsSubqueriesInQuantifieds(
        /* [out] */Boolean* result);

    CARAPI SupportsCorrelatedSubqueries(
        /* [out] */Boolean* result);

    CARAPI SupportsUnion(
        /* [out] */Boolean* result);

    CARAPI SupportsUnionAll(
        /* [out] */Boolean* result);

    CARAPI SupportsOpenCursorsAcrossCommit(
        /* [out] */Boolean* result);

    CARAPI SupportsOpenCursorsAcrossRollback(
        /* [out] */Boolean* result);

    CARAPI SupportsOpenStatementsAcrossCommit(
        /* [out] */Boolean* result);

    CARAPI SupportsOpenStatementsAcrossRollback(
        /* [out] */Boolean* result);

    CARAPI GetMaxBinaryLiteralLength(
        /* [out] */Int32* result);

    CARAPI GetMaxCharLiteralLength(
        /* [out] */Int32* result);

    CARAPI GetMaxColumnNameLength(
        /* [out] */Int32* result);

    CARAPI GetMaxColumnsInGroupBy(
        /* [out] */Int32* result);

    CARAPI GetMaxColumnsInIndex(
        /* [out] */Int32* result);

    CARAPI GetMaxColumnsInOrderBy(
        /* [out] */Int32* result);

    CARAPI GetMaxColumnsInSelect(
        /* [out] */Int32* result);

    CARAPI GetMaxColumnsInt32able(
        /* [out] */Int32* result);

    CARAPI GetMaxConnections(
        /* [out] */Int32* result);

    CARAPI GetMaxCursorNameLength(
        /* [out] */Int32* result);

    CARAPI GetMaxIndexLength(
        /* [out] */Int32* result);

    CARAPI GetMaxSchemaNameLength(
        /* [out] */Int32* result);

    CARAPI GetMaxProcedureNameLength(
        /* [out] */Int32* result);

    CARAPI GetMaxCatalogNameLength(
        /* [out] */Int32* result);

    CARAPI GetMaxRowSize(
        /* [out] */Int32* result);

    CARAPI DoesMaxRowSizeIncludeBlobs(
        /* [out] */Boolean* result);

    CARAPI GetMaxStatementLength(
        /* [out] */Int32* result);

    CARAPI GetMaxStatements(
        /* [out] */Int32* result);

    CARAPI GetMaxTableNameLength(
        /* [out] */Int32* result);

    CARAPI GetMaxTablesInSelect(
        /* [out] */Int32* result);

    CARAPI GetMaxUserNameLength(
        /* [out] */Int32* result);

    CARAPI GetDefaultTransactionIsolation(
        /* [out] */Int32* result);

    CARAPI SupportsTransactions(
        /* [out] */Boolean* result);

    CARAPI SupportsTransactionIsolationLevel(
        /* [in] */Int32 level,
        /* [out] */Boolean* result);

    CARAPI SupportsDataDefinitionAndDataManipulationTransactions(
        /* [out] */Boolean* result);

    CARAPI SupportsDataManipulationTransactionsOnly(
        /* [out] */Boolean* result);

    CARAPI DataDefinitionCausesTransactionCommit(
        /* [out] */Boolean* result);

    CARAPI DataDefinitionIgnoredInt32ransactions(
        /* [out] */Boolean* result);

    CARAPI GetProcedures(
        /* [in] */String catalog, 
        /* [in] */String schemaPattern,
        /* [in] */ String procedureNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetProcedureColumns(
        /* [in] */String catalog,
        /* [in] */ String schemaPattern,
        /* [in] */String procedureNamePattern, 
        /* [in] */String columnNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetTables(
        /* [in] */String catalog, 
        /* [in] */String schemaPattern,
        /* [in] */String tableNamePattern, 
        /* [in] */ArrayOf<String> types,
        /* [out] */IResultSet** result);

    CARAPI GetSchemas(
        /* [out] */IResultSet** result);

    CARAPI GetCatalogs(
        /* [out] */IResultSet** result);

    CARAPI GetTableTypes(
        /* [out] */IResultSet** result);

    CARAPI GetColumns(
        /* [in] */String catalog, 
        /* [in] */String schemaPattern,
        /* [in] */String tableNamePattern,
        /* [in] */String columnNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetColumnPrivileges(
        /* [in] */String catalog, 
        /* [in] */String schema,
        /* [in] */String table,
        /* [in] */String columnNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetTablePrivileges(
        /* [in] */String catalog,
        /* [in] */String schemaPattern,
        /* [in] */String tableNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetBestRowIdentifier(
        /* [in] */String catalog, 
        /* [in] */String schema,
        /* [in] */String table, 
        /* [in] */Int32 scope,
        /* [in] */Boolean nullable,
        /* [out] */IResultSet** result);

    CARAPI GetVersionColumns(
        /* [in] */String catalog, 
        /* [in] */String schema,
        /* [in] */String table,
        /* [out] */IResultSet** result);

    CARAPI GetPrimaryKeys(
        /* [in] */String catalog, 
        /* [in] */String schema,
        /* [in] */String table,
        /* [out] */IResultSet** result);

    CARAPI GetImportedKeys(
        /* [in] */String catalog, 
        /* [in] */String schema,
        /* [in] */String table,
        /* [out] */IResultSet** result);

    CARAPI GetExportedKeys(
        /* [in] */String catalog, 
        /* [in] */String schema,
        /* [in] */String table,
        /* [out] */IResultSet** result);

    CARAPI GetCrossReference(
        /* [in] */String primaryCatalog,
        /* [in] */String primarySchema,
        /* [in] */String primaryTable,
        /* [in] *//* [in] */String foreignCatalog,
        /* [in] */String foreignSchema,
        /* [in] */String foreignTable,
        /* [out] */IResultSet** result);

    CARAPI GetTypeInfo(
        /* [out] */IResultSet** result);

    CARAPI GetIndexInfo(
        /* [in] */String catalog, 
        /* [in] */String schema, 
        /* [in] */String table,
        /* [in] */Boolean unique, 
        /* [in] */Boolean approximate,
        /* [out] */IResultSet** result);

    CARAPI SupportsResultSetType(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI SupportsResultSetConcurrency(
        /* [in] */Int32 type, 
        /* [in] */Int32 concurrency,
        /* [out] */Boolean* result);

    CARAPI OwnUpdatesAreVisible(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI OwnDeletesAreVisible(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI OwnInsertsAreVisible(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI OthersUpdatesAreVisible(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI OthersDeletesAreVisible(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI OthersInsertsAreVisible(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI UpdatesAreDetected(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI DeletesAreDetected(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI InsertsAreDetected(
        /* [in] */Int32 type,
        /* [out] */Boolean* result);

    CARAPI SupportsBatchUpdates(
        /* [out] */Boolean* result);

    CARAPI GetUDTs(
        /* [in] */String catalog, 
        /* [in] */String schemaPattern, 
        /* [in] */String typeNamePattern, 
        /* [in] */ArrayOf<Int32> types,
        /* [out] */IConnection** result);

    CARAPI GetConnection(
        /* [out] */IResultSet** result);

    CARAPI static MapTypeName(
        /* [in] */Int32 type,
        /* [out] */String* result);

    CARAPI static MapSqlType(
        /* [in] */String type,
        /* [out] */Int32* result);

    CARAPI static GetM(
        /* [in] */String typeStr, 
        /* [in] */Int32 type,
        /* [out] */Int32* result);

    CARAPI static GetD(
        /* [in] */String typeStr, 
        /* [in] */Int32 type,
        /* [out] */Int32* result);

    CARAPI SupportsSavepoInt32s(
        /* [out] */Boolean* result);

    CARAPI SupportsNamedParameters(
        /* [out] */Boolean* result);

    CARAPI SupportsMultipleOpenResults(
        /* [out] */Boolean* result);

    CARAPI SupportsGetGeneratedKeys(
        /* [out] */Boolean* result);

    CARAPI SupportsResultSetHoldability(
        /* [in] */Int32 x,
        /* [out] */Boolean* result);

    CARAPI SupportsStatementPooling(
        /* [out] */Boolean* result);

    CARAPI LocatorsUpdateCopy(
        /* [out] */Boolean* result);

    CARAPI GetSuperTypes(
        /* [in] */String catalog, 
        /* [in] */String schemaPattern,
        /* [in] */String typeNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetSuperTables(
        /* [in] */String catalog, 
        /* [in] */String schemaPattern,
        /* [in] */String tableNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetAttributes(
        /* [in] */String catalog, 
        /* [in] */String schemaPattern,
        /* [in] */String typeNamePattern,
        /* [in] */String attributeNamePattern,
        /* [out] */IResultSet** result);

    CARAPI GetResultSetHoldability(
        /* [out] */Int32* result);

    CARAPI GetDatabaseMajorVersion(
        /* [out] */Int32* result);

    CARAPI GetDatabaseMinorVersion(
        /* [out] */Int32* result);

    CARAPI GetJDBCMajorVersion(
        /* [out] */Int32* result);
    
    CARAPI GetJDBCMinorVersion(
        /* [out] */Int32* result);

    CARAPI GetSQLStateType(
        /* [out] */Int32* result);

    CARAPI GetRowIdLifetime(
        /* [out] */IRowIdLifetime** result);

    CARAPI GetSchemas(
        /* [in] */String cat, 
        /* [in] */String schema,
        /* [out] */IResultSet** result);

    CARAPI SupportsStoredFunctionsUsingCallSyntax(
        /* [out] */Boolean* result);

    CARAPI AutoCommitFailureClosesAllResultSets(
        /* [out] */Boolean* result);

    CARAPI GetClientInfoProperties(
        /* [out] */IResultSet** result);

    CARAPI GetFunctions(
        /* [in] */String cat, 
        /* [in] */String schema, 
        /* [in] */String func,
        /* [out] */IResultSet** result);

    CARAPI GetFunctionColumns(
        /* [in] */String cat, 
        /* [in] */String schema,
        /* [in] */String func, 
        /* [in] */String colpat,
        /* [out] */IResultSet** result);

//    CARAPI <T> T unwrap(java.lang.Class<T> iface);

//    CARAPI Boolean isWrapperFor(java.lang.Class iface);
};
#endif//_JDBCDATABASEMETADATA_H_