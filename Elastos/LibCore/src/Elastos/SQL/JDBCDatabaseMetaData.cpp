
#include "JDBCDatabaseMetaData.h"
#include "cmdef.h"

JDBCDatabaseMetaData::JDBCDatabaseMetaData():mConn(NULL){
}

JDBCDatabaseMetaData::~JDBCDatabaseMetaData() {
}

void JDBCDatabaseMetaData::InternalImportedKeys(
        /* [in] */String table, 
        /* [in] */String pktable,
        /* [in] */JDBCResultSet in, 
        /* [in] */TableResultX out)
{/*
    Hashtable<String, Integer> h0 = new Hashtable<String, Integer>();
    for (int i = 0; i < in.tr.ncolumns; i++) {
        h0.put(in.tr.column[i], Integer.valueOf(i)); // android-changed
    }
    for (int i = 0; i < in.tr.nrows; i++) {
        String r0[] = (String [])(in.tr.rows.elementAt(i));
        int col = ((Integer) h0.get("table")).intValue();
        String pktab = r0[col];
        if (pktable != null && !pktable.equalsIgnoreCase(pktab)) {
        continue;
        }
        col = ((Integer) h0.get("from")).intValue();
        String fkcol = r0[col];
        col = ((Integer) h0.get("to")).intValue();
        String pkcol = r0[col];
        col = ((Integer) h0.get("seq")).intValue();
        String seq = r0[col];
        String row[] = new String[out.ncolumns];
        row[0]  = "";
        row[1]  = "";
        row[2]  = pktab;
        row[3]  = pkcol;
        row[4]  = "";
        row[5]  = "";
        row[6]  = table;
        row[7]  = fkcol == null ? pkcol : fkcol;
        row[8]  = Integer.toString(Integer.parseInt(seq) + 1);
        row[9]  =
        "" + java.sql.DatabaseMetaData.importedKeyNoAction;
        row[10] =
        "" + java.sql.DatabaseMetaData.importedKeyNoAction;
        row[11] = null;
        row[12] = null;
        row[13] =
        "" + java.sql.DatabaseMetaData.importedKeyNotDeferrable;
        out.newrow(row);
    }*/
}

ECode JDBCDatabaseMetaData::Init(
    /* [in] */IJDBCConnection* conn)
{
    mConn = conn;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::AllProceduresAreCallable(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::AllTablesAreSelectable(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetURL(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = mConn->mUrl;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetUserName(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::IsReadOnly(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::NullsAreSortedHigh(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::NullsAreSortedLow(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::NullsAreSortedAtStart(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::NullsAreSortedAtEnd(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDatabaseProductName(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "SQLite";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDatabaseProductVersion(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = SQLite.Database.version();
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDriverName(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "SQLite/JDBC";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDriverVersion(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result =  "" + SQLite.JDBCDriver.MAJORVERSION + "." +
        SQLite.Constants.drv_minor;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDriverMajorVersion(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = SQLite.JDBCDriver.MAJORVERSION;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDriverMinorVersion(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = SQLite.Constants.drv_minor;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::UsesLocalFiles(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::UsesLocalFilePerTable(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsMixedCaseIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::StoresUpperCaseIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::StoresLowerCaseIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::StoresMixedCaseIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsMixedCaseQuotedIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::StoresUpperCaseQuotedIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::StoresLowerCaseQuotedIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::StoresMixedCaseQuotedIdentifiers(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetIdentifierQuoteString(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "\"";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetSQLKeywords(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "SELECT,UPDATE,CREATE,TABLE,VIEW,DELETE,FROM,WHERE" +
        ",COMMIT,ROLLBACK,TRIGGER";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetNumericFunctions(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetStringFunctions(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetSystemFunctions(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::getTimeDateFunctions(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetSearchStringEscape(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "\\";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetExtraNameCharacters(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SsupportsAlterTableWithAddColumn(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsAlterTableWithDropColumn(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsColumnAliasing(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::NullPlusNonNullIsNull(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsConvert(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsConvert(
    /* [in] */Int32 fromType, 
    /* [in] */Int32 toType,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsTableCorrelationNames(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsDifferentTableCorrelationNames(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsExpressionsInOrderBy(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsOrderByUnrelated(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsGroupBy(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsGroupByUnrelated(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsGroupByBeyondSelect(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsLikeEscapeClause(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsMultipleResultSets(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsMultipleTransactions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsNonNullableColumns(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsMinimumSQLGrammar(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsCoreSQLGrammar(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsExtendedSQLGrammar(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsANSI92EntryLevelSQL(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsANSI92Int32ermediateSQL(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsANSI92FullSQL(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsInt32egrityEnhancementFacility(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsOuterJoins(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsFullOuterJoins(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsLimitedOuterJoins(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetSchemaTerm(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetProcedureTerm(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetCatalogTerm(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::IsCatalogAtStart(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetCatalogSeparator(
    /* [out] */String* result)
{
    assert(result != NULL);
    *result = "";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSchemasInDataManipulation(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSchemasInProcedureCalls(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSchemasInt32ableDefinitions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSchemasInIndexDefinitions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSchemasInPrivilegeDefinitions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsCatalogsInDataManipulation(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsCatalogsInProcedureCalls(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsCatalogsInt32ableDefinitions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsCatalogsInIndexDefinitions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsCatalogsInPrivilegeDefinitions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsPositionedDelete(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsPositionedUpdate(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSelectForUpdate(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsStoredProcedures(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSubqueriesInComparisons(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSubqueriesInExists(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSubqueriesInIns(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSubqueriesInQuantifieds(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsCorrelatedSubqueries(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsUnion(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsUnionAll(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsOpenCursorsAcrossCommit(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsOpenCursorsAcrossRollback(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsOpenStatementsAcrossCommit(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsOpenStatementsAcrossRollback(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxBinaryLiteralLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxCharLiteralLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxColumnNameLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxColumnsInGroupBy(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxColumnsInIndex(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxColumnsInOrderBy(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxColumnsInSelect(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxColumnsInt32able(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxConnections(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxCursorNameLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 8;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxIndexLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxSchemaNameLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxProcedureNameLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxCatalogNameLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxRowSize(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::DoesMaxRowSizeIncludeBlobs(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxStatementLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxStatements(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxTableNameLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxTablesInSelect(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetMaxUserNameLength(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDefaultTransactionIsolation(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = Connection.TRANSACTION_SERIALIZABLE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsTransactions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsTransactionIsolationLevel(
    /* [in] */Int32 level,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result =  level == Connection.TRANSACTION_SERIALIZABLE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsDataDefinitionAndDataManipulationTransactions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsDataManipulationTransactionsOnly(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::DataDefinitionCausesTransactionCommit(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::DataDefinitionIgnoredInt32ransactions(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetProcedures(
    /* [in] */String catalog, 
    /* [in] */String schemaPattern,
    /* [in] */ String procedureNamePattern,
    /* [out] */IResultSet** result)
{
    assert(result != NULL);
    result = NULL;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetProcedureColumns(
    /* [in] */String catalog,
    /* [in] */ String schemaPattern,
    /* [in] */String procedureNamePattern, 
    /* [in] */String columnNamePattern,
    /* [out] */IResultSet** result)
{
    assert(result != NULL);
    result = NULL;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetTables(
    /* [in] */String catalog, 
    /* [in] */String schemaPattern,
    /* [in] */String tableNamePattern, 
    /* [in] */ArrayOf<String> types,
    /* [out] */IResultSet** result)
{/*
    JDBCStatement s = new JDBCStatement(conn);
    StringBuffer sb = new StringBuffer();
    sb.append("SELECT '' AS 'TABLE_CAT', " +
          "'' AS 'TABLE_SCHEM', " +
          "tbl_name AS 'TABLE_NAME', " +
          "upper(type) AS 'TABLE_TYPE', " +
          "'' AS REMARKS FROM sqlite_master " +
          "WHERE tbl_name like ");
    if (tableNamePattern != null) {
        sb.append(SQLite.Shell.sql_quote(tableNamePattern));
    } else {
        sb.append("'%'");
    }
    sb.append(" AND ");
    if (types == null || types.length == 0) {
        sb.append("(type = 'table' or type = 'view')");
    } else {
        sb.append("(");
        String sep = ""; 
        for (int i = 0; i < types.length; i++) {
        sb.append(sep);
        sb.append("type = ");
        sb.append(SQLite.Shell.sql_quote(types[i].toLowerCase()));
        sep = " or ";
        }
        sb.append(")");
    }
    ResultSet rs = null;
    try {
        rs = s.executeQuery(sb.toString());
        s.close();
    } catch (SQLException e) {
        throw e;
    } finally {
        s.close();
    }
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetSchemas(
    /* [out] */IResultSet** result)
{/*
    String cols[] = { "TABLE_SCHEM" };
    SQLite.TableResult tr = new SQLite.TableResult();
    tr.columns(cols);
    String row[] = { "" };
    tr.newrow(row);
    JDBCResultSet rs = new JDBCResultSet(tr, null);
    return (ResultSet) rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetCatalogs(
    /* [out] */IResultSet** result)
{/*
    String cols[] = { "TABLE_CAT" };
    SQLite.TableResult tr = new SQLite.TableResult();
    tr.columns(cols);
    String row[] = { "" };
    tr.newrow(row);
    JDBCResultSet rs = new JDBCResultSet(tr, null);
    return (ResultSet) rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetTableTypes(
    /* [out] */IResultSet** result)
{/*
    String cols[] = { "TABLE_TYPE" };
    SQLite.TableResult tr = new SQLite.TableResult();
    tr.columns(cols);
    String row[] = new String[1];
    row[0] = "TABLE";
    tr.newrow(row);
    row = new String[1];
    row[0] = "VIEW";
    tr.newrow(row);
    JDBCResultSet rs = new JDBCResultSet(tr, null);
    return (ResultSet) rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetColumns(
    /* [in] */String catalog, 
    /* [in] */String schemaPattern,
    /* [in] */String tableNamePattern,
    /* [in] */String columnNamePattern,
    /* [out] */IResultSet** result)
{/*
    if (conn.db == null) {
        throw new SQLException("connection closed.");
    }
    JDBCStatement s = new JDBCStatement(conn);
    JDBCResultSet rs0 = null;
    try {
        try {
        conn.db.exec("SELECT 1 FROM sqlite_master LIMIT 1", null);
        } catch (SQLite.Exception se) {
        throw new SQLException("schema reload failed");
        }
        rs0 = (JDBCResultSet)
        (s.executeQuery("PRAGMA table_info(" +
                SQLite.Shell.sql_quote(tableNamePattern) +
                ")"));
        s.close();
    } catch (SQLException e) {
        throw e;
    } finally {
        s.close();
    }
    if (rs0.tr.nrows < 1) {
        throw new SQLException("no such table: " + tableNamePattern);
    }
    String cols[] = {
        "TABLE_CAT", "TABLE_SCHEM", "TABLE_NAME",
        "COLUMN_NAME", "DATA_TYPE", "TYPE_NAME",
        "COLUMN_SIZE", "BUFFER_LENGTH", "DECIMAL_DIGITS",
        "NUM_PREC_RADIX", "NULLABLE", "REMARKS",
        "COLUMN_DEF", "SQL_DATA_TYPE", "SQL_DATETIME_SUB",
        "CHAR_OCTET_LENGTH", "ORDINAL_POSITION", "IS_NULLABLE"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.SMALLINT, Types.VARCHAR,
        Types.INTEGER, Types.INTEGER, Types.INTEGER,
        Types.INTEGER, Types.INTEGER, Types.VARCHAR,
        Types.VARCHAR, Types.INTEGER, Types.INTEGER,
        Types.INTEGER, Types.INTEGER, Types.VARCHAR
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet((SQLite.TableResult) tr, null);
    if (rs0 != null && rs0.tr != null && rs0.tr.nrows > 0) {
        Hashtable<String, Integer> h = new Hashtable<String, Integer>();
        for (int i = 0; i < rs0.tr.ncolumns; i++) {
        h.put(rs0.tr.column[i], Integer.valueOf(i)); // android-changed
        }
        if (columnNamePattern != null &&
        columnNamePattern.charAt(0) == '%') {
        columnNamePattern = null;
        }
        for (int i = 0; i < rs0.tr.nrows; i++) {
        String r0[] = (String [])(rs0.tr.rows.elementAt(i));
        int col = ((Integer) h.get("name")).intValue();
        if (columnNamePattern != null) {
            if (r0[col].compareTo(columnNamePattern) != 0) {
            continue;
            }
        }
        String row[] = new String[cols.length];
        row[0]  = "";
        row[1]  = "";
        row[2]  = tableNamePattern;
        row[3]  = r0[col];
        col = ((Integer) h.get("type")).intValue();
        String typeStr = r0[col];
        int type = mapSqlType(typeStr);
        row[4]  = "" + type;
        row[5]  = mapTypeName(type);
        row[6]  = "" + getD(typeStr, type);
        row[7]  = "" + getM(typeStr, type);
        row[8]  = "10";
        row[9]  = "0";
        row[11] = null;
        col = ((Integer) h.get("dflt_value")).intValue();
        row[12] = r0[col];
        row[13] = "0";
        row[14] = "0";
        row[15] = "65536";
        col = ((Integer) h.get("cid")).intValue();
        row[16] = Integer.toString(Integer.parseInt(r0[col]) + 1); // android-changed
        col = ((Integer) h.get("notnull")).intValue();
        row[17] = (r0[col].charAt(0) == '0') ? "YES" : "NO";
        row[10] = (r0[col].charAt(0) == '0') ? "" + columnNullable :
              "" + columnNoNulls;
        tr.newrow(row);
        }
    }
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetColumnPrivileges(
    /* [in] */String catalog, 
    /* [in] */String schema,
    /* [in] */String table,
    /* [in] */String columnNamePattern,
    /* [out] */IResultSet** result)
{
    /*
    String cols[] = {
        "TABLE_CAT", "TABLE_SCHEM", "TABLE_NAME",
        "COLUMN_NAME", "GRANTOR", "GRANTEE",
        "PRIVILEGE", "IS_GRANTABLE"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet((SQLite.TableResult) tr, null);
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetTablePrivileges(
    /* [in] */String catalog,
    /* [in] */String schemaPattern,
    /* [in] */String tableNamePattern,
    /* [out] */IResultSet** result)
{/*
    String cols[] = {
        "TABLE_CAT", "TABLE_SCHEM", "TABLE_NAME",
        "COLUMN_NAME", "GRANTOR", "GRANTEE",
        "PRIVILEGE", "IS_GRANTABLE"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet((SQLite.TableResult) tr, null);
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetBestRowIdentifier(
    /* [in] */String catalog, 
    /* [in] */String schema,
    /* [in] */String table, 
    /* [in] */Int32 scope,
    /* [in] */Boolean nullable,
    /* [out] */IResultSet** result)
{/*
    JDBCStatement s0 = new JDBCStatement(conn);
    JDBCResultSet rs0 = null;
    JDBCStatement s1 = new JDBCStatement(conn);
    JDBCResultSet rs1 = null;
    try {
        try {
        conn.db.exec("SELECT 1 FROM sqlite_master LIMIT 1", null);
        } catch (SQLite.Exception se) {
        throw new SQLException("schema reload failed");
        }
        rs0 = (JDBCResultSet)
        (s0.executeQuery("PRAGMA index_list(" +
                 SQLite.Shell.sql_quote(table) + ")"));
        rs1 = (JDBCResultSet)
        (s1.executeQuery("PRAGMA table_info(" +
                 SQLite.Shell.sql_quote(table) + ")"));
    } catch (SQLException e) {
        throw e;
    } finally {
        s0.close();
        s1.close();
    }
    String cols[] = {
        "SCOPE", "COLUMN_NAME", "DATA_TYPE",
        "TYPE_NAME", "COLUMN_SIZE", "BUFFER_LENGTH",
        "DECIMAL_DIGITS", "PSEUDO_COLUMN"
    };
    int types[] = {
        Types.SMALLINT, Types.VARCHAR, Types.SMALLINT,
        Types.VARCHAR, Types.INTEGER, Types.INTEGER,
        Types.SMALLINT, Types.SMALLINT
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet((SQLite.TableResult) tr, null);
    if (rs0 != null && rs0.tr != null && rs0.tr.nrows > 0 &&
        rs1 != null && rs1.tr != null && rs1.tr.nrows > 0) {
        Hashtable<String, Integer> h0 = new Hashtable<String, Integer>();
        for (int i = 0; i < rs0.tr.ncolumns; i++) {
        h0.put(rs0.tr.column[i], Integer.valueOf(i)); // android-changed
        }
        Hashtable<String, Integer> h1 = new Hashtable<String, Integer>();
        for (int i = 0; i < rs1.tr.ncolumns; i++) {
        h1.put(rs1.tr.column[i], Integer.valueOf(i)); // android-changed
        }
        for (int i = 0; i < rs0.tr.nrows; i++) {
        String r0[] = (String [])(rs0.tr.rows.elementAt(i));
        int col = ((Integer) h0.get("unique")).intValue();
        String uniq = r0[col];
        col = ((Integer) h0.get("name")).intValue();
        String iname = r0[col];
        if (uniq.charAt(0) == '0') {
            continue;
        }
        JDBCStatement s2 = new JDBCStatement(conn);
        JDBCResultSet rs2 = null;
        try {
            rs2 = (JDBCResultSet)
            (s2.executeQuery("PRAGMA index_info(" +
                     SQLite.Shell.sql_quote(iname) + ")"));
        } catch (SQLException e) {
        } finally {
            s2.close();
        }
        if (rs2 == null || rs2.tr == null || rs2.tr.nrows <= 0) {
            continue;
        }
        Hashtable<String, Integer> h2 =
            new Hashtable<String, Integer>();
        for (int k = 0; k < rs2.tr.ncolumns; k++) {
            h2.put(rs2.tr.column[k], Integer.valueOf(k)); // android-changed
        }
        for (int k = 0; k < rs2.tr.nrows; k++) {
            String r2[] = (String [])(rs2.tr.rows.elementAt(k));
            col = ((Integer) h2.get("name")).intValue();
            String cname = r2[col];
            for (int m = 0; m < rs1.tr.nrows; m++) {
            String r1[] = (String [])(rs1.tr.rows.elementAt(m));
            col = ((Integer) h1.get("name")).intValue();
            if (cname.compareTo(r1[col]) == 0) {
                String row[] = new String[cols.length];
                row[0] = "" + scope;
                row[1] = cname;
                row[2] = "" + Types.VARCHAR;
                row[3] = "VARCHAR";
                row[4] = "65536";
                row[5] = "0";
                row[6] = "0";
                row[7] = "" + bestRowNotPseudo;
                tr.newrow(row);
            }
            }
        }
        }
    }
    if (tr.nrows <= 0) {
        String row[] = new String[cols.length];
        row[0] = "" + scope;
        row[1] = "_ROWID_";
        row[2] = "" + Types.INTEGER;
        row[3] = "INTEGER";
        row[4] = "10";
        row[5] = "0";
        row[6] = "0";
        row[7] = "" + bestRowPseudo;
        tr.newrow(row);
    }
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetVersionColumns(
    /* [in] */String catalog, 
    /* [in] */String schema,
    /* [in] */String table,
    /* [out] */IResultSet** result)
{/*
    String cols[] = {
        "SCOPE", "COLUMN_NAME", "DATA_TYPE",
        "TYPE_NAME", "COLUMN_SIZE", "BUFFER_LENGTH",
        "DECIMAL_DIGITS", "PSEUDO_COLUMN"
    };
    int types[] = {
        Types.SMALLINT, Types.VARCHAR, Types.SMALLINT,
        Types.VARCHAR, Types.INTEGER, Types.INTEGER,
        Types.SMALLINT, Types.SMALLINT
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet((SQLite.TableResult) tr, null);
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetPrimaryKeys(
    /* [in] */String catalog, 
    /* [in] */String schema,
    /* [in] */String table,
    /* [out] */IResultSet** result)
{/*
    JDBCStatement s0 = new JDBCStatement(conn);
    JDBCResultSet rs0 = null;
    try {
        try {
        conn.db.exec("SELECT 1 FROM sqlite_master LIMIT 1", null);
        } catch (SQLite.Exception se) {
        throw new SQLException("schema reload failed");
        }
        rs0 = (JDBCResultSet)
        (s0.executeQuery("PRAGMA index_list(" +
                 SQLite.Shell.sql_quote(table) + ")"));
    } catch (SQLException e) {
        throw e;
    } finally {
        s0.close();
    }
    String cols[] = {
        "TABLE_CAT", "TABLE_SCHEM", "TABLE_NAME",
        "COLUMN_NAME", "KEY_SEQ", "PK_NAME"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.SMALLINT, Types.VARCHAR
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet((SQLite.TableResult) tr, null);
    if (rs0 != null && rs0.tr != null && rs0.tr.nrows > 0) {
        Hashtable<String, Integer> h0 = new Hashtable<String, Integer>();
        for (int i = 0; i < rs0.tr.ncolumns; i++) {
        h0.put(rs0.tr.column[i], Integer.valueOf(i)); // android-changed
        }
        for (int i = 0; i < rs0.tr.nrows; i++) {
        String r0[] = (String [])(rs0.tr.rows.elementAt(i));
        int col = ((Integer) h0.get("unique")).intValue();
        String uniq = r0[col];
        col = ((Integer) h0.get("name")).intValue();
        String iname = r0[col];
        if (uniq.charAt(0) == '0') {
            continue;
        }
        JDBCStatement s1 = new JDBCStatement(conn);
        JDBCResultSet rs1 = null;
        try {
            rs1 = (JDBCResultSet)
            (s1.executeQuery("PRAGMA index_info(" +
                     SQLite.Shell.sql_quote(iname) + ")"));
        } catch (SQLException e) {
        } finally {
            s1.close();
        }
        if (rs1 == null || rs1.tr == null || rs1.tr.nrows <= 0) {
            continue;
        }
        Hashtable<String, Integer> h1 =
            new Hashtable<String, Integer>();
        for (int k = 0; k < rs1.tr.ncolumns; k++) {
            h1.put(rs1.tr.column[k], Integer.valueOf(k)); // android-changed
        }
        for (int k = 0; k < rs1.tr.nrows; k++) {
            String r1[] = (String [])(rs1.tr.rows.elementAt(k));
            String row[] = new String[cols.length];
            row[0]  = "";
            row[1]  = "";
            row[2]  = table;
            col = ((Integer) h1.get("name")).intValue();
            row[3] = r1[col];
            col = ((Integer) h1.get("seqno")).intValue();
            row[4]  = Integer.toString(Integer.parseInt(r1[col]) + 1);
            row[5]  = iname;
            tr.newrow(row);
        }
        }
    }
    if (tr.nrows > 0) {
        return rs;
    }
    JDBCStatement s1 = new JDBCStatement(conn);
    try {
        rs0 = (JDBCResultSet)
        (s1.executeQuery("PRAGMA table_info(" +
                 SQLite.Shell.sql_quote(table) + ")"));
    } catch (SQLException e) {
        throw e;
    } finally {
        s1.close();
    }
    if (rs0 != null && rs0.tr != null && rs0.tr.nrows > 0) {
        Hashtable<String, Integer> h0 = new Hashtable<String, Integer>();
        for (int i = 0; i < rs0.tr.ncolumns; i++) {
        h0.put(rs0.tr.column[i], Integer.valueOf(i)); // android-changed
        }
        for (int i = 0; i < rs0.tr.nrows; i++) {
        String r0[] = (String [])(rs0.tr.rows.elementAt(i));
        int col = ((Integer) h0.get("type")).intValue();
        String type = r0[col];
        if (!type.equalsIgnoreCase("integer")) {
            continue;
        }
        col = ((Integer) h0.get("pk")).intValue();
        String pk = r0[col];
        if (pk.charAt(0) == '0') {
            continue;
        }
        String row[] = new String[cols.length];
        row[0]  = "";
        row[1]  = "";
        row[2]  = table;
        col = ((Integer) h0.get("name")).intValue();
        row[3] = r0[col];
        col = ((Integer) h0.get("cid")).intValue();
        row[4] = Integer.toString(Integer.parseInt(r0[col]) + 1);
        row[5] = "";
        tr.newrow(row);
        }
    }
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetImportedKeys(
    /* [in] */String catalog, 
    /* [in] */String schema,
    /* [in] */String table,
    /* [out] */IResultSet** result)
{/*
    JDBCStatement s0 = new JDBCStatement(conn);
    JDBCResultSet rs0 = null;
    try {
        try {
        conn.db.exec("SELECT 1 FROM sqlite_master LIMIT 1", null);
        } catch (SQLite.Exception se) {
        throw new SQLException("schema reload failed");
        }
        rs0 = (JDBCResultSet)
        (s0.executeQuery("PRAGMA foreign_key_list(" +
                 SQLite.Shell.sql_quote(table) + ")"));
    } catch (SQLException e) {
        throw e;
    } finally {
        s0.close();
    }
    String cols[] = {
        "PKTABLE_CAT", "PKTABLE_SCHEM", "PKTABLE_NAME",
        "PKCOLUMN_NAME", "FKTABLE_CAT", "FKTABLE_SCHEM",
        "FKTABLE_NAME", "FKCOLUMN_NAME", "KEY_SEQ",
        "UPDATE_RULE", "DELETE_RULE", "FK_NAME",
        "PK_NAME", "DEFERRABILITY"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.SMALLINT,
        Types.SMALLINT, Types.SMALLINT, Types.VARCHAR,
        Types.VARCHAR, Types.SMALLINT
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet((SQLite.TableResult) tr, null);
    if (rs0 != null && rs0.tr != null && rs0.tr.nrows > 0) {
        internalImportedKeys(table, null, rs0, tr);
    }
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetExportedKeys(
    /* [in] */String catalog, 
    /* [in] */String schema,
    /* [in] */String table,
    /* [out] */IResultSet** result)
{/*
    String cols[] = {
        "PKTABLE_CAT", "PKTABLE_SCHEM", "PKTABLE_NAME",
        "PKCOLUMN_NAME", "FKTABLE_CAT", "FKTABLE_SCHEM",
        "FKTABLE_NAME", "FKCOLUMN_NAME", "KEY_SEQ",
        "UPDATE_RULE", "DELETE_RULE", "FK_NAME",
        "PK_NAME", "DEFERRABILITY"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.SMALLINT,
        Types.SMALLINT, Types.SMALLINT, Types.VARCHAR,
        Types.VARCHAR, Types.SMALLINT
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet(tr, null);
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetCrossReference(
    /* [in] */String primaryCatalog,
    /* [in] */String primarySchema,
    /* [in] */String primaryTable,
    /* [in] *//* [in] */String foreignCatalog,
    /* [in] */String foreignSchema,
    /* [in] */String foreignTable,
    /* [out] */IResultSet** result)
{/*
    JDBCResultSet rs0 = null;
    if (foreignTable != null && foreignTable.charAt(0) != '%') {
        JDBCStatement s0 = new JDBCStatement(conn);
        try {
        try {
            conn.db.exec("SELECT 1 FROM sqlite_master LIMIT 1", null);
        } catch (SQLite.Exception se) {
            throw new SQLException("schema reload failed");
        }
        rs0 = (JDBCResultSet)
            (s0.executeQuery("PRAGMA foreign_key_list(" +
                     SQLite.Shell.sql_quote(foreignTable) + ")"));
        } catch (SQLException e) {
        throw e;
        } finally {
        s0.close();
        }
    }
    String cols[] = {
        "PKTABLE_CAT", "PKTABLE_SCHEM", "PKTABLE_NAME",
        "PKCOLUMN_NAME", "FKTABLE_CAT", "FKTABLE_SCHEM",
        "FKTABLE_NAME", "FKCOLUMN_NAME", "KEY_SEQ",
        "UPDATE_RULE", "DELETE_RULE", "FK_NAME",
        "PK_NAME", "DEFERRABILITY"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.VARCHAR, Types.VARCHAR, Types.SMALLINT,
        Types.SMALLINT, Types.SMALLINT, Types.VARCHAR,
        Types.VARCHAR, Types.SMALLINT
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet(tr, null);
    if (rs0 != null && rs0.tr != null && rs0.tr.nrows > 0) {
        String pktable = null;
        if (primaryTable != null && primaryTable.charAt(0) != '%') {
        pktable = primaryTable;
        }
        internalImportedKeys(foreignTable, pktable, rs0, tr);
    }
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetTypeInfo(
    /* [out] */IResultSet** result)
{/*
    String cols[] = {
        "TYPE_NAME", "DATA_TYPE", "PRECISION",
        "LITERAL_PREFIX", "LITERAL_SUFFIX", "CREATE_PARAMS",
        "NULLABLE", "CASE_SENSITIVE", "SEARCHABLE",
        "UNSIGNED_ATTRIBUTE", "FIXED_PREC_SCALE", "AUTO_INCREMENT",
        "LOCAL_TYPE_NAME", "MINIMUM_SCALE", "MAXIMUM_SCALE",
        "SQL_DATA_TYPE", "SQL_DATETIME_SUB", "NUM_PREC_RADIX"
    };
    int types[] = {
        Types.VARCHAR, Types.SMALLINT, Types.INTEGER,
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.SMALLINT, Types.BIT, Types.SMALLINT,
        Types.BIT, Types.BIT, Types.BIT,
        Types.VARCHAR, Types.SMALLINT, Types.SMALLINT,
        Types.INTEGER, Types.INTEGER, Types.INTEGER
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet(tr, null);
    String row1[] = {
        "VARCHAR", "" + Types.VARCHAR, "65536",
        "'", "'", null,
        "" + typeNullable, "1", "" + typeSearchable,
        "0", "0", "0",
        null, "0", "0",
        "0", "0", "0"
    };
    tr.newrow(row1);
    String row2[] = {
        "INTEGER", "" + Types.INTEGER, "32",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "2"
    };
    tr.newrow(row2);
    String row3[] = {
        "DOUBLE", "" + Types.DOUBLE, "16",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "10"
    };
    tr.newrow(row3);
    String row4[] = {
        "FLOAT", "" + Types.FLOAT, "7",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "10"
    };
    tr.newrow(row4);
    String row5[] = {
        "SMALLINT", "" + Types.SMALLINT, "16",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "2"
    };
    tr.newrow(row5);
    String row6[] = {
        "BIT", "" + Types.BIT, "1",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "2"
    };
    tr.newrow(row6);
    String row7[] = {
        "TIMESTAMP", "" + Types.TIMESTAMP, "30",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "0"
    };
    tr.newrow(row7);
    String row8[] = {
        "DATE", "" + Types.DATE, "10",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "0"
    };
    tr.newrow(row8);
    String row9[] = {
        "TIME", "" + Types.TIME, "8",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "0"
    };
    tr.newrow(row9);
    String row10[] = {
        "BINARY", "" + Types.BINARY, "65536",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "0"
    };
    tr.newrow(row10);
    String row11[] = {
        "VARBINARY", "" + Types.VARBINARY, "65536",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "0"
    };
    tr.newrow(row11);
    String row12[] = {
        "REAL", "" + Types.REAL, "16",
        null, null, null,
        "" + typeNullable, "0", "" + typeSearchable,
        "0", "0", "1",
        null, "0", "0",
        "0", "0", "10"
    };
    tr.newrow(row12);
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::GetIndexInfo(
    /* [in] */String catalog, 
    /* [in] */String schema, 
    /* [in] */String table,
    /* [in] */Boolean unique, 
    /* [in] */Boolean approximate,
    /* [out] */IResultSet** result)
{
    /*
    JDBCStatement s0 = new JDBCStatement(conn);
    JDBCResultSet rs0 = null;
    try {
        try {
        conn.db.exec("SELECT 1 FROM sqlite_master LIMIT 1", null);
        } catch (SQLite.Exception se) {
        throw new SQLException("schema reload failed");
        }
        rs0 = (JDBCResultSet)
        (s0.executeQuery("PRAGMA index_list(" +
                 SQLite.Shell.sql_quote(table) + ")"));
    } catch (SQLException e) {
        throw e;
    } finally {
        s0.close();
    }
    String cols[] = {
        "TABLE_CAT", "TABLE_SCHEM", "TABLE_NAME",
        "NON_UNIQUE", "INDEX_QUALIFIER", "INDEX_NAME",
        "TYPE", "ORDINAL_POSITION", "COLUMN_NAME",
        "ASC_OR_DESC", "CARDINALITY", "PAGES",
        "FILTER_CONDITION"
    };
    int types[] = {
        Types.VARCHAR, Types.VARCHAR, Types.VARCHAR,
        Types.BIT, Types.VARCHAR, Types.VARCHAR,
        Types.SMALLINT, Types.SMALLINT, Types.VARCHAR,
        Types.VARCHAR, Types.INTEGER, Types.INTEGER,
        Types.VARCHAR
    };
    TableResultX tr = new TableResultX();
    tr.columns(cols);
    tr.sql_types(types);
    JDBCResultSet rs = new JDBCResultSet(tr, null);
    if (rs0 != null && rs0.tr != null && rs0.tr.nrows > 0) {
        Hashtable<String, Integer> h0 = new Hashtable<String, Integer>();
        for (int i = 0; i < rs0.tr.ncolumns; i++) {
        h0.put(rs0.tr.column[i], Integer.valueOf(i)); // android-changed
        }
        for (int i = 0; i < rs0.tr.nrows; i++) {
        String r0[] = (String [])(rs0.tr.rows.elementAt(i));
        int col = ((Integer) h0.get("unique")).intValue();
        String uniq = r0[col];
        col = ((Integer) h0.get("name")).intValue();
        String iname = r0[col];
        if (unique && uniq.charAt(0) == '0') {
            continue;
        }
        JDBCStatement s1 = new JDBCStatement(conn);
        JDBCResultSet rs1 = null;
        try {
            rs1 = (JDBCResultSet)
            (s1.executeQuery("PRAGMA index_info(" +
                     SQLite.Shell.sql_quote(iname) + ")"));
        } catch (SQLException e) {
        } finally {
            s1.close();
        }
        if (rs1 == null || rs1.tr == null || rs1.tr.nrows <= 0) {
            continue;
        }
        Hashtable<String, Integer> h1 =
            new Hashtable<String, Integer>();
        for (int k = 0; k < rs1.tr.ncolumns; k++) {
            h1.put(rs1.tr.column[k], Integer.valueOf(k)); // android-changed
        }
        for (int k = 0; k < rs1.tr.nrows; k++) {
            String r1[] = (String [])(rs1.tr.rows.elementAt(k));
            String row[] = new String[cols.length];
            row[0]  = "";
            row[1]  = "";
            row[2]  = table;
            row[3]  = (uniq.charAt(0) != '0' ||
            (iname.charAt(0) == '(' &&
             iname.indexOf(" autoindex ") > 0)) ? "0" : "1";
            row[4]  = "";
            row[5]  = iname;
            row[6]  = "" + tableIndexOther;
            col = ((Integer) h1.get("seqno")).intValue();
            row[7]  = Integer.toString(Integer.parseInt(r1[col]) + 1);
            col = ((Integer) h1.get("name")).intValue();
            row[8]  = r1[col];
            row[9]  = "A";
            row[10] = "0";
            row[11] = "0";
            row[12] = null;
            tr.newrow(row);
        }
        }
    }
    return rs;*/
    return E_NOT_IMPLEMENTED;
}

ECode JDBCDatabaseMetaData::SupportsResultSetType(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = type == ResultSet.TYPE_FORWARD_ONLY ||
        type == ResultSet.TYPE_SCROLL_INSENSITIVE ||
        type == ResultSet.TYPE_SCROLL_SENSITIVE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsResultSetConcurrency(
    /* [in] */Int32 type, 
    /* [in] */Int32 concurrency,
    /* [out] */Boolean* result)
{
    assert(result != NULL);

    if (type == ResultSet.TYPE_FORWARD_ONLY ||
        type == ResultSet.TYPE_SCROLL_INSENSITIVE ||
        type == ResultSet.TYPE_SCROLL_SENSITIVE) {
        *result =  concurrency == ResultSet.CONCUR_READ_ONLY ||
        concurrency == ResultSet.CONCUR_UPDATABLE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::OwnUpdatesAreVisible(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);

    if (type == ResultSet.TYPE_FORWARD_ONLY ||
        type == ResultSet.TYPE_SCROLL_INSENSITIVE ||
        type == ResultSet.TYPE_SCROLL_SENSITIVE) {
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::OwnDeletesAreVisible(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);

    if (type == ResultSet.TYPE_FORWARD_ONLY ||
        type == ResultSet.TYPE_SCROLL_INSENSITIVE ||
        type == ResultSet.TYPE_SCROLL_SENSITIVE) {
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::OwnInsertsAreVisible(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);

    if (type == ResultSet.TYPE_FORWARD_ONLY ||
        type == ResultSet.TYPE_SCROLL_INSENSITIVE ||
        type == ResultSet.TYPE_SCROLL_SENSITIVE) {
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::OthersUpdatesAreVisible(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::OthersDeletesAreVisible(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::OthersInsertsAreVisible(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::UpdatesAreDetected(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::DeletesAreDetected(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::InsertsAreDetected(
    /* [in] */Int32 type,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsBatchUpdates(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetUDTs(
    /* [in] */String catalog, 
    /* [in] */String schemaPattern, 
    /* [in] */String typeNamePattern, 
    /* [in] */ArrayOf<Int32> types,
    /* [out] */IConnection** result)
{
    assert(result != NULL);
    result = NULL;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetConnection(
    /* [out] */IResultSet** result)
{
    result = &mConn;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::static MapTypeName(
    /* [in] */Int32 type,
    /* [out] */String* result)
{
    assert(result != NULL);
    switch (type) {
    case Types.INTEGER:    *result = "integer";return NOERROR;
    case Types.SMALLINT:    *result = "smallint";return NOERROR;
    case Types.FLOAT:    *result = "float";return NOERROR;
    case Types.DOUBLE:    *result = "double";return NOERROR;
    case Types.TIMESTAMP:    *result = "timestamp";return NOERROR;
    case Types.DATE:    *result = "date";return NOERROR;
    case Types.TIME:    *result = "time";return NOERROR;
    case Types.BINARY:    *result = "binary";return NOERROR;
    case Types.VARBINARY:    *result = "varbinary";return NOERROR;
    case Types.REAL:    *result = "real";return NOERROR;
    }

    *result = "varchar";
    return NOERROR;
}

ECode JDBCDatabaseMetaData::static MapSqlType(
    /* [in] */String type,
    /* [out] */Int32* result)
{
    if (type == null) {
        *result = Types.VARCHAR;
        return NOERROR;
    }
    type = type.toLowerCase();
    if (type.startsWith("inter")) {
        *result = Types.VARCHAR;
        return NOERROR;
    }
    if (type.startsWith("numeric") ||
        type.startsWith("int")) {
        *result = Types.INTEGER;
        return NOERROR;
    }
    if (type.startsWith("tinyint") ||
        type.startsWith("smallint")) {
        *result = Types.SMALLINT;
        return NOERROR;
    }
    if (type.startsWith("float")) {
        *result = Types.FLOAT;
        return NOERROR;
    }
    if (type.startsWith("double")) {
        *result = Types.DOUBLE;
        return NOERROR;
    }
    if (type.startsWith("datetime") ||
        type.startsWith("timestamp")) {
        *result = Types.TIMESTAMP;
        return NOERROR;
    }
    if (type.startsWith("date")) {
        *result = Types.DATE;
        return NOERROR;
    }
    if (type.startsWith("time")) {
        *result = Types.TIME;
        return NOERROR;
    }
    if (type.startsWith("blob")) {
        *result = Types.BINARY;
        return NOERROR;
    }
    if (type.startsWith("binary")) {
        *result = Types.BINARY;
        return NOERROR;
    }
    if (type.startsWith("varbinary")) {
        *result = Types.VARBINARY;
        return NOERROR;
    }
    if (type.startsWith("real")) {
        *result = Types.REAL;
        return NOERROR;
    }
    *result = Types.VARCHAR;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::static GetM(
    /* [in] */String typeStr, 
    /* [in] */Int32 type,
    /* [out] */Int32* result)
{/*
    int m = 65536;
    switch (type) {
    case Types.INTEGER:    m = 11; break;
    case Types.SMALLINT:    m = 6;  break;
    case Types.FLOAT:    m = 25; break;
    case Types.REAL:
    case Types.DOUBLE:    m = 54; break;
    case Types.TIMESTAMP:    return 30;
    case Types.DATE:    return 10;
    case Types.TIME:    return 8;
    }
    typeStr = typeStr.toLowerCase();
    int i1 = typeStr.indexOf('(');
    if (i1 > 0) {
        ++i1;
        int i2 = typeStr.indexOf(',', i1);
        if (i2 < 0) {
        i2 = typeStr.indexOf(')', i1);
        }
        if (i2 - i1 > 0) {
        String num = typeStr.substring(i1, i2);
        try {
            m = java.lang.Integer.parseInt(num, 10);
        } catch (NumberFormatException e) {
        }
        }
    }
    return m;*/
    return NOERROR;
}

ECode JDBCDatabaseMetaData::static GetD(
    /* [in] */String typeStr, 
    /* [in] */Int32 type,
    /* [out] */Int32* result)
{/*
    int d = 0;
    switch (type) {
    case Types.INTEGER:    d = 10; break;
    case Types.SMALLINT:    d = 5;  break;
    case Types.FLOAT:    d = 24; break;
    case Types.REAL:
    case Types.DOUBLE:    d = 53; break;
    default:        return getM(typeStr, type);
    }
    typeStr = typeStr.toLowerCase();
    int i1 = typeStr.indexOf('(');
    if (i1 > 0) {
        ++i1;
        int i2 = typeStr.indexOf(',', i1);
        if (i2 < 0) {
        return getM(typeStr, type);
        }
        i1 = i2;
        i2 = typeStr.indexOf(')', i1);
        if (i2 - i1 > 0) {
        String num = typeStr.substring(i1, i2);
        try {
            d = java.lang.Integer.parseInt(num, 10);
        } catch (NumberFormatException e) {
        }
        }
    }
    return d;*/
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsSavepoInt32s(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsNamedParameters(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsMultipleOpenResults(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsGetGeneratedKeys(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsResultSetHoldability(
    /* [in] */Int32 x,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::SupportsStatementPooling(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::LocatorsUpdateCopy(
    /* [out] */Boolean* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCDatabaseMetaData::GetSuperTypes(
    /* [in] */String catalog, 
    /* [in] */String schemaPattern,
    /* [in] */String typeNamePattern,
    /* [out] */IResultSet** result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCDatabaseMetaData::GetSuperTables(
    /* [in] */String catalog, 
    /* [in] */String schemaPattern,
    /* [in] */String tableNamePattern,
    /* [out] */IResultSet** result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCDatabaseMetaData::GetAttributes(
    /* [in] */String catalog, 
    /* [in] */String schemaPattern,
    /* [in] */String typeNamePattern,
    /* [in] */String attributeNamePattern,
    /* [out] */IResultSet** result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCDatabaseMetaData::GetResultSetHoldability(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = ResultSet.HOLD_CURSORS_OVER_COMMIT;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDatabaseMajorVersion(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = SQLite.JDBCDriver.MAJORVERSION;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetDatabaseMinorVersion(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = SQLite.Constants.drv_minor;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetJDBCMajorVersion(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 1;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetJDBCMinorVersion(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = 0;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetSQLStateType(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = sqlStateXOpen;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetRowIdLifetime(
    /* [out] */IRowIdLifetime** result)
{
    assert(result != NULL);
    *result = RowIdLifetime.ROWID_UNSUPPORTED;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetSchemas(
    /* [in] */String cat, 
    /* [in] */String schema,
    /* [out] */IResultSet** result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCDatabaseMetaData::SupportsStoredFunctionsUsingCallSyntax(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::AutoCommitFailureClosesAllResultSets(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCDatabaseMetaData::GetClientInfoProperties(
    /* [out] */IResultSet** result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCDatabaseMetaData::GetFunctions(
    /* [in] */String cat, 
    /* [in] */String schema, 
    /* [in] */String func,
    /* [out] */IResultSet** result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCDatabaseMetaData::GetFunctionColumns(
    /* [in] */String cat, 
    /* [in] */String schema,
    /* [in] */String func, 
    /* [in] */String colpat,
    /* [out] */IResultSet** result)
{
    return E_SQL_EXCEPTION;
}