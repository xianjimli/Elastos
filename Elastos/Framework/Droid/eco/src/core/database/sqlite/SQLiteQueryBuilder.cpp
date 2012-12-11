#include "database/sqlite/SQLiteQueryBuilder.h"

const CString SQLiteQueryBuilder::TAG = "Database";

SQLiteQueryBuilder::SQLiteQueryBuilder()
    : mProjectionMap(NULL)
    , mWhereClause(NULL)
    , mDistinct(FALSE)
    , mStrictProjectionMap(FALSE)
{}

SQLiteQueryBuilder::~SQLiteQueryBuilder()
{
    if (mProjectionMap != NULL) {
        delete mProjectionMap;
    }

    if (mWhereClause != NULL) {
        delete mWhereClause;
    }
}

ECode SQLiteQueryBuilder::SetDistinct(
    /*[in]*/ Boolean distinct)
{
    mDistinct = distinct;
    return NOERROR;
}

String SQLiteQueryBuilder::GetTables()
{
    return mTables;
}

ECode SQLiteQueryBuilder::SetTables(
    /* [in]*/ const String& inTables)
{
    mTables = inTables;
    return NOERROR;
}

ECode SQLiteQueryBuilder::AppendWhere(
    /* [in] */ ICharSequence* inWhere)
{
    if (mWhereClause == NULL) {
        mWhereClause = new StringBuffer();
    }
    if (mWhereClause->GetLength() == 0) {
        *mWhereClause += '(';
    }
    String str;
    inWhere->ToString(&str);
    *mWhereClause += str;
    return NOERROR;
}

ECode SQLiteQueryBuilder::AppendWhereEscapeString(
    /* [in] */ const String& inWhere)
{
    if (mWhereClause == NULL) {
        mWhereClause = new StringBuffer(inWhere.GetLength() + 16);
    }
    if (mWhereClause->GetLength() == 0) {
        mWhereClause += '(';
    }
    // DatabaseUtils.appendEscapedSQLString(mWhereClause, inWhere);
    return NOERROR;
}

ECode SQLiteQueryBuilder::SetCursorFactory(
    /* [in] */ ICursorFactory* factory)
{
    mFactory = factory;
    return NOERROR;
}

ECode SQLiteQueryBuilder::SetStrictProjectionMap(
    /* [in] */ Boolean flag)
{
    mStrictProjectionMap = flag;
    return NOERROR;
}

ECode SQLiteQueryBuilder::BuildQueryString(
    /* [in] */ Boolean distinct,
    /* [in] */ const String& tables,
    /* [in] */ ArrayOf<String>* columns,
    /* [in] */ const String& where,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [in] */ const String& limit,
    /* [out] */ String* queryStr)
{
    if (groupBy.IsNullOrEmpty() && !having.IsNullOrEmpty()) {
        // throw new IllegalArgumentException(
        //         "HAVING clauses are only permitted when using a groupBy clause");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!limit.IsNullOrEmpty() /*&& !sLimitPattern.matcher(limit).matches()*/) {
        // throw new IllegalArgumentException("invalid LIMIT clauses:" + limit);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuffer query(120);

    query += "SELECT ";
    if (distinct) {
        query += "DISTINCT ";
    }
    if (columns != NULL && columns->GetLength() != 0) {
        AppendColumns(query, *columns);
    }
    else {
        query += "* ";
    }
    query += "FROM ";
    query += tables;
    AppendClause(query, " WHERE ", where);
    AppendClause(query, " GROUP BY ", groupBy);
    AppendClause(query, " HAVING ", having);
    AppendClause(query, " ORDER BY ", orderBy);
    AppendClause(query, " LIMIT ", limit);

    *queryStr = String(query);
    return NOERROR;
}

void SQLiteQueryBuilder::AppendClause(
    /* [in] */ StringBuffer& s,
    /* [in] */ CString name,
    /* [in] */ CString clause)
{
    if (!clause.IsNullOrEmpty()) {
        s += name;
        s += clause;
    }
}

void SQLiteQueryBuilder::AppendClauseEscapeClause(
    /* [in] */ StringBuffer& s,
    /* [in] */ CString name,
    /* [in] */ CString clause)
{
    if (!clause.IsNullOrEmpty()) {
        s += name;
        // DatabaseUtils.appendEscapedSQLString(s, clause);
    }
}

void SQLiteQueryBuilder::AppendColumns(
    /* [in] */ StringBuffer& s,
    /* [in] */ const ArrayOf<String>& columns)
{
    Int32 n = columns.GetLength();

    for (Int32 i = 0; i < n; i++) {
        String column = columns[i];

        if (!column.IsNull()) {
            if (i > 0) {
                s += ", ";
            }
            s += column;
        }
    }
    s += ' ';
}

ECode SQLiteQueryBuilder::Query(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    return Query(db, projectionIn, selection, selectionArgs, groupBy, having, sortOrder,
                String(NULL) /* limit */, cursor);
}

ECode SQLiteQueryBuilder::Query(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ ICursor** cursor)
{
    if (mTables.IsNull()) {
        *cursor = NULL;
    }

    String sql;
    FAIL_RETURN(BuildQuery(
            projectionIn, selection, selectionArgs, groupBy, having,
            sortOrder, limit, &sql));

    // if (Log.isLoggable(TAG, Log.DEBUG)) {
    //     Log.d(TAG, "Performing query: " + sql);
    // }
    // return db->RawQueryWithFactory(
    //         mFactory, sql, selectionArgs,
    //         SQLiteDatabase.findEditTable(mTables), cursor);
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteQueryBuilder::BuildQuery(
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String* str)
{
    ArrayOf<String>* projection;
    // String[] projection = computeProjection(projectionIn);

    StringBuffer where;
    Boolean hasBaseWhereClause = mWhereClause != NULL && mWhereClause->GetLength() > 0;

    if (hasBaseWhereClause) {
        where += mWhereClause;
        where += ')';
    }

    // Tack on the user's selection, if present.
    if (!selection.IsNullOrEmpty()) {
        if (hasBaseWhereClause) {
            where += " AND ";
        }

        where += '(';
        where += selection;
        where += ')';
    }

    return BuildQueryString(
            mDistinct, mTables, projection, String(where),
            groupBy, having, sortOrder, limit, str);
}

ECode SQLiteQueryBuilder::BuildUnionSubQuery(
    /* [in] */ const String& typeDiscriminatorColumn,
    /* [in] */ const ArrayOf<String>& unionColumns,
    /* [in] */ Int32 computedColumnsOffset,
    /* [in] */ const String& typeDiscriminatorValue,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [out] */ String* str)
{
    // int unionColumnsCount = unionColumns.length;
    // String[] projectionIn = new String[unionColumnsCount];

    // for (int i = 0; i < unionColumnsCount; i++) {
    //     String unionColumn = unionColumns[i];

    //     if (unionColumn.equals(typeDiscriminatorColumn)) {
    //         projectionIn[i] = "'" + typeDiscriminatorValue + "' AS "
    //                 + typeDiscriminatorColumn;
    //     } else if (i <= computedColumnsOffset
    //                || columnsPresentInTable.contains(unionColumn)) {
    //         projectionIn[i] = unionColumn;
    //     } else {
    //         projectionIn[i] = "NULL AS " + unionColumn;
    //     }
    // }
    // return buildQuery(
    //         projectionIn, selection, selectionArgs, groupBy, having,
    //         null /* sortOrder */,
    //         null /* limit */);
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteQueryBuilder::BuildUnionQuery(
    /* [in] */ const ArrayOf<String>& subQueries,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String* str)
{
    StringBuffer query(128);
    Int32 subQueryCount = subQueries.GetLength();
    CString unionOperator = mDistinct ? " UNION " : " UNION ALL ";

    for (Int32 i = 0; i < subQueryCount; i++) {
        if (i > 0) {
            query += unionOperator;
        }
        query += subQueries[i];
    }
    AppendClause(query, " ORDER BY ", sortOrder);
    AppendClause(query, " LIMIT ", limit);
    *str = String(query);
    return NOERROR;
}

ECode SQLiteQueryBuilder::ComputeProjection(
    /* [in] */ const ArrayOf<String>& projectionIn,
    /* [out, callee] */ ArrayOf<String>** projectionOut)
{
    // if (projectionIn != null && projectionIn.length > 0) {
    //     if (mProjectionMap != null) {
    //         String[] projection = new String[projectionIn.length];
    //         int length = projectionIn.length;

    //         for (int i = 0; i < length; i++) {
    //             String userColumn = projectionIn[i];
    //             String column = mProjectionMap.get(userColumn);

    //             if (column != null) {
    //                 projection[i] = column;
    //                 continue;
    //             }

    //             if (!mStrictProjectionMap &&
    //                     ( userColumn.contains(" AS ") || userColumn.contains(" as "))) {
    //                 /* A column alias already exist */
    //                 projection[i] = userColumn;
    //                 continue;
    //             }

    //             throw new IllegalArgumentException("Invalid column "
    //                     + projectionIn[i]);
    //         }
    //         return projection;
    //     } else {
    //         return projectionIn;
    //     }
    // } else if (mProjectionMap != null) {
    //     // Return all columns in projection map.
    //     Set<Entry<String, String>> entrySet = mProjectionMap.entrySet();
    //     String[] projection = new String[entrySet.size()];
    //     Iterator<Entry<String, String>> entryIter = entrySet.iterator();
    //     int i = 0;

    //     while (entryIter.hasNext()) {
    //         Entry<String, String> entry = entryIter.next();

    //         // Don't include the _count column when people ask for no projection.
    //         if (entry.getKey().equals(BaseColumns._COUNT)) {
    //             continue;
    //         }
    //         projection[i++] = entry.getValue();
    //     }
    //     return projection;
    // }
    // return null;
    return E_NOT_IMPLEMENTED;
}
