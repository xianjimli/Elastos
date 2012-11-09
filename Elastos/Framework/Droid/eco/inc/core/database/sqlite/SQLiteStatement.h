#ifndef __SQLITESTATEMENT_H__
#define __SQLITESTATEMENT_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteProgram.h"
/**
 * A pre-compiled statement against a {@link SQLiteDatabase} that can be reused.
 * The statement cannot return multiple rows, but 1x1 result sets are allowed.
 * Don't use SQLiteStatement constructor directly, please use
 * {@link SQLiteDatabase#compileStatement(String)}
 *
 * SQLiteStatement is not internally synchronized so code using a SQLiteStatement from multiple
 * threads should perform its own synchronization when using the SQLiteStatement.
 */
class SQLiteStatement : public SQLiteProgram
{
public:
    SQLiteStatement();
    ~SQLiteStatement();

    /**
     * Execute this SQL statement, if it is not a query. For example,
     * CREATE TABLE, DELTE, INSERT, etc.
     *
     * @throws android.database.SQLException If the SQL string is invalid for
     *         some reason
     */
    virtual CARAPI Execute();

    /**
     * Execute this SQL statement and return the ID of the row inserted due to this call.
     * The SQL statement should be an INSERT for this to be a useful call.
     *
     * @return the row ID of the last row inserted, if this insert is successful. -1 otherwise.
     *
     * @throws android.database.SQLException If the SQL string is invalid for
     *         some reason
     */
    virtual CARAPI ExecuteInsert(
        /* [out] */ Int64* value);

    /**
     * Execute a statement that returns a 1 by 1 table with a numeric value.
     * For example, SELECT COUNT(*) FROM table;
     *
     * @return The result of the query.
     *
     * @throws android.database.sqlite.SQLiteDoneException if the query returns zero rows
     */
    virtual CARAPI SimpleQueryForLong(
        /* [out] */ Int64* value);

    /**
     * Execute a statement that returns a 1 by 1 table with a text value.
     * For example, SELECT COUNT(*) FROM table;
     *
     * @return The result of the query.
     *
     * @throws android.database.sqlite.SQLiteDoneException if the query returns zero rows
     */
    virtual CARAPI SimpleQueryForString(
        /* [out] */ String* value);

    /**
     * Don't use SQLiteStatement constructor directly, please use
     * {@link SQLiteDatabase#compileStatement(String)}
     * @param db
     * @param sql
     */
    SQLiteStatement(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String sql);

private:
    CARAPI Native_Execute();

    CARAPI_(Int64) Native_1x1_Long();
    
    CARAPI_(String) Native_1x1_String();
};
#endif //__SQLITESTATEMENT_H__