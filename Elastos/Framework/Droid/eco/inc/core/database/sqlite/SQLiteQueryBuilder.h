#ifndef __SQLITEQUERYBUILDER_H__
#define __SQLITEQUERYBUILDER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

/**
 * This is a convience class that helps build SQL queries to be sent to
 * {@link SQLiteDatabase} objects.
 */
class SQLiteQueryBuilder
{
public:
    SQLiteQueryBuilder();

    virtual ~SQLiteQueryBuilder();

    /**
     * Mark the query as DISTINCT.
     *
     * @param distinct if true the query is DISTINCT, otherwise it isn't
     */
    virtual CARAPI SetDistinct(
        /*[in]*/ Boolean distinct);

    /**
     * Returns the list of tables being queried
     *
     * @return the list of tables being queried
     */
    virtual CARAPI_(String) GetTables();

    /**
     * Sets the list of tables to query. Multiple tables can be specified to perform a join.
     * For example:
     *   setTables("foo, bar")
     *   setTables("foo LEFT OUTER JOIN bar ON (foo.id = bar.foo_id)")
     *
     * @param inTables the list of tables to query on
     */
    virtual CARAPI SetTables(
        /* [in]*/ const String& inTables);

    /**
     * Append a chunk to the WHERE clause of the query. All chunks appended are surrounded
     * by parenthesis and ANDed with the selection passed to {@link #query}. The final
     * WHERE clause looks like:
     *
     * WHERE (&lt;append chunk 1>&lt;append chunk2>) AND (&lt;query() selection parameter>)
     *
     * @param inWhere the chunk of text to append to the WHERE clause.
     */
    virtual CARAPI AppendWhere(
        /* [in] */ ICharSequence* inWhere);

    /**
     * Append a chunk to the WHERE clause of the query. All chunks appended are surrounded
     * by parenthesis and ANDed with the selection passed to {@link #query}. The final
     * WHERE clause looks like:
     *
     * WHERE (&lt;append chunk 1>&lt;append chunk2>) AND (&lt;query() selection parameter>)
     *
     * @param inWhere the chunk of text to append to the WHERE clause. it will be escaped
     * to avoid SQL injection attacks
     */
    virtual CARAPI AppendWhereEscapeString(
        /* [in] */ const String& inWhere);

    /**
     * Sets the projection map for the query.  The projection map maps
     * from column names that the caller passes into query to database
     * column names. This is useful for renaming columns as well as
     * disambiguating column names when doing joins. For example you
     * could map "name" to "people.name".  If a projection map is set
     * it must contain all column names the user may request, even if
     * the key and value are the same.
     *
     * @param columnMap maps from the user column names to the database column names
     */
    // public void setProjectionMap(Map<String, String> columnMap);

    /**
     * Sets the cursor factory to be used for the query.  You can use
     * one factory for all queries on a database but it is normally
     * easier to specify the factory when doing this query.  @param
     * factory the factor to use
     */
    virtual CARAPI SetCursorFactory(
        /* [in] */ ICursorFactory* factory);

    /**
     * @hide
     */
    virtual CARAPI SetStrictProjectionMap(
        /* [in] */ Boolean flag);

    /**
     * Build an SQL query string from the given clauses.
     *
     * @param distinct true if you want each row to be unique, false otherwise.
     * @param tables The table names to compile the query against.
     * @param columns A list of which columns to return. Passing null will
     *            return all columns, which is discouraged to prevent reading
     *            data from storage that isn't going to be used.
     * @param where A filter declaring which rows to return, formatted as an SQL
     *            WHERE clause (excluding the WHERE itself). Passing null will
     *            return all rows for the given URL.
     * @param groupBy A filter declaring how to group rows, formatted as an SQL
     *            GROUP BY clause (excluding the GROUP BY itself). Passing null
     *            will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in the cursor,
     *            if row grouping is being used, formatted as an SQL HAVING
     *            clause (excluding the HAVING itself). Passing null will cause
     *            all row groups to be included, and is required when row
     *            grouping is not being used.
     * @param orderBy How to order the rows, formatted as an SQL ORDER BY clause
     *            (excluding the ORDER BY itself). Passing null will use the
     *            default sort order, which may be unordered.
     * @param limit Limits the number of rows returned by the query,
     *            formatted as LIMIT clause. Passing null denotes no LIMIT clause.
     * @return the SQL query string
     */
    static CARAPI BuildQueryString(
        /* [in] */ Boolean distinct,
        /* [in] */ const String& tables,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& where,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ String* queryStr);

    /**
     * Add the names that are non-null in columns to s, separating
     * them with commas.
     */
    static CARAPI_(void) AppendColumns(
        /* [in] */ StringBuffer& s,
        /* [in] */ const ArrayOf<String>& columns);

    /**
     * Perform a query by combining all current settings and the
     * information passed into this method.
     *
     * @param db the database to query on
     * @param projectionIn A list of which columns to return. Passing
     *   null will return all columns, which is discouraged to prevent
     *   reading data from storage that isn't going to be used.
     * @param selection A filter declaring which rows to return,
     *   formatted as an SQL WHERE clause (excluding the WHERE
     *   itself). Passing null will return all rows for the given URL.
     * @param selectionArgs You may include ?s in selection, which
     *   will be replaced by the values from selectionArgs, in order
     *   that they appear in the selection. The values will be bound
     *   as Strings.
     * @param groupBy A filter declaring how to group rows, formatted
     *   as an SQL GROUP BY clause (excluding the GROUP BY
     *   itself). Passing null will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in
     *   the cursor, if row grouping is being used, formatted as an
     *   SQL HAVING clause (excluding the HAVING itself).  Passing
     *   null will cause all row groups to be included, and is
     *   required when row grouping is not being used.
     * @param sortOrder How to order the rows, formatted as an SQL
     *   ORDER BY clause (excluding the ORDER BY itself). Passing null
     *   will use the default sort order, which may be unordered.
     * @return a cursor over the result set
     * @see android.content.ContentResolver#query(android.net.Uri, String[],
     *      String, String[], String)
     */
    virtual CARAPI Query(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    /**
     * Perform a query by combining all current settings and the
     * information passed into this method.
     *
     * @param db the database to query on
     * @param projectionIn A list of which columns to return. Passing
     *   null will return all columns, which is discouraged to prevent
     *   reading data from storage that isn't going to be used.
     * @param selection A filter declaring which rows to return,
     *   formatted as an SQL WHERE clause (excluding the WHERE
     *   itself). Passing null will return all rows for the given URL.
     * @param selectionArgs You may include ?s in selection, which
     *   will be replaced by the values from selectionArgs, in order
     *   that they appear in the selection. The values will be bound
     *   as Strings.
     * @param groupBy A filter declaring how to group rows, formatted
     *   as an SQL GROUP BY clause (excluding the GROUP BY
     *   itself). Passing null will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in
     *   the cursor, if row grouping is being used, formatted as an
     *   SQL HAVING clause (excluding the HAVING itself).  Passing
     *   null will cause all row groups to be included, and is
     *   required when row grouping is not being used.
     * @param sortOrder How to order the rows, formatted as an SQL
     *   ORDER BY clause (excluding the ORDER BY itself). Passing null
     *   will use the default sort order, which may be unordered.
     * @param limit Limits the number of rows returned by the query,
     *   formatted as LIMIT clause. Passing null denotes no LIMIT clause.
     * @return a cursor over the result set
     * @see android.content.ContentResolver#query(android.net.Uri, String[],
     *      String, String[], String)
     */
    virtual CARAPI Query(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ ICursor** cursor);

    /**
     * Construct a SELECT statement suitable for use in a group of
     * SELECT statements that will be joined through UNION operators
     * in buildUnionQuery.
     *
     * @param projectionIn A list of which columns to return. Passing
     *    null will return all columns, which is discouraged to
     *    prevent reading data from storage that isn't going to be
     *    used.
     * @param selection A filter declaring which rows to return,
     *   formatted as an SQL WHERE clause (excluding the WHERE
     *   itself).  Passing null will return all rows for the given
     *   URL.
     * @param selectionArgs You may include ?s in selection, which
     *   will be replaced by the values from selectionArgs, in order
     *   that they appear in the selection.  The values will be bound
     *   as Strings.
     * @param groupBy A filter declaring how to group rows, formatted
     *   as an SQL GROUP BY clause (excluding the GROUP BY itself).
     *   Passing null will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in
     *   the cursor, if row grouping is being used, formatted as an
     *   SQL HAVING clause (excluding the HAVING itself).  Passing
     *   null will cause all row groups to be included, and is
     *   required when row grouping is not being used.
     * @param sortOrder How to order the rows, formatted as an SQL
     *   ORDER BY clause (excluding the ORDER BY itself). Passing null
     *   will use the default sort order, which may be unordered.
     * @param limit Limits the number of rows returned by the query,
     *   formatted as LIMIT clause. Passing null denotes no LIMIT clause.
     * @return the resulting SQL SELECT statement
     */
    virtual CARAPI BuildQuery(
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String* str);

    /**
     * Construct a SELECT statement suitable for use in a group of
     * SELECT statements that will be joined through UNION operators
     * in buildUnionQuery.
     *
     * @param typeDiscriminatorColumn the name of the result column
     *   whose cells will contain the name of the table from which
     *   each row was drawn.
     * @param unionColumns the names of the columns to appear in the
     *   result.  This may include columns that do not appear in the
     *   table this SELECT is querying (i.e. mTables), but that do
     *   appear in one of the other tables in the UNION query that we
     *   are constructing.
     * @param columnsPresentInTable a Set of the names of the columns
     *   that appear in this table (i.e. in the table whose name is
     *   mTables).  Since columns in unionColumns include columns that
     *   appear only in other tables, we use this array to distinguish
     *   which ones actually are present.  Other columns will have
     *   NULL values for results from this subquery.
     * @param computedColumnsOffset all columns in unionColumns before
     *   this index are included under the assumption that they're
     *   computed and therefore won't appear in columnsPresentInTable,
     *   e.g. "date * 1000 as normalized_date"
     * @param typeDiscriminatorValue the value used for the
     *   type-discriminator column in this subquery
     * @param selection A filter declaring which rows to return,
     *   formatted as an SQL WHERE clause (excluding the WHERE
     *   itself).  Passing null will return all rows for the given
     *   URL.
     * @param selectionArgs You may include ?s in selection, which
     *   will be replaced by the values from selectionArgs, in order
     *   that they appear in the selection.  The values will be bound
     *   as Strings.
     * @param groupBy A filter declaring how to group rows, formatted
     *   as an SQL GROUP BY clause (excluding the GROUP BY itself).
     *   Passing null will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in
     *   the cursor, if row grouping is being used, formatted as an
     *   SQL HAVING clause (excluding the HAVING itself).  Passing
     *   null will cause all row groups to be included, and is
     *   required when row grouping is not being used.
     * @return the resulting SQL SELECT statement
     */
    virtual CARAPI BuildUnionSubQuery(
        /* [in] */ const String& typeDiscriminatorColumn,
        /* [in] */ const ArrayOf<String>& unionColumns,
        /* [in] */ Int32 computedColumnsOffset,
        /* [in] */ const String& typeDiscriminatorValue,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [out] */ String* str);

    /**
     * Given a set of subqueries, all of which are SELECT statements,
     * construct a query that returns the union of what those
     * subqueries return.
     * @param subQueries an array of SQL SELECT statements, all of
     *   which must have the same columns as the same positions in
     *   their results
     * @param sortOrder How to order the rows, formatted as an SQL
     *   ORDER BY clause (excluding the ORDER BY itself).  Passing
     *   null will use the default sort order, which may be unordered.
     * @param limit The limit clause, which applies to the entire union result set
     *
     * @return the resulting SQL SELECT statement
     */
    virtual CARAPI BuildUnionQuery(
        /* [in] */ const ArrayOf<String>& subQueries,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String* str);

private:
    static CARAPI_(void) AppendClause(
        /* [in] */ StringBuffer& s,
        /* [in] */ CString name,
        /* [in] */ CString clause);

    static CARAPI_(void) AppendClauseEscapeClause(
        /* [in] */ StringBuffer& s,
        /* [in] */ CString name,
        /* [in] */ CString clause);

    CARAPI ComputeProjection(
        /* [in] */ const ArrayOf<String>& projectionIn,
        /* [out, callee] */ ArrayOf<String>** projectionOut);

private:
    const static CString TAG;
    // static final Pattern sLimitPattern =
    //         Pattern.compile("\\s*\\d+\\s*(,\\s*\\d+\\s*)?");

    HashMap<String, String>* mProjectionMap;
    String mTables;
    StringBuffer* mWhereClause;
//    StringBuilder mWhereClause = NULL;  // lazily created
    Boolean mDistinct;
    AutoPtr<ICursorFactory> mFactory;
    Boolean mStrictProjectionMap;
};


#endif //__SQLITEQUERYBUILDER_H__