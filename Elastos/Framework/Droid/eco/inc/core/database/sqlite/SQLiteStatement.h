#ifndef __SQLITESTATEMENT_H__
#define __SQLITESTATEMENT_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteProgram.h"
#include "database/sqlite/SQLiteDatabase.h"
#include <elastos/ElRefBase.h>

/**
 * A pre-compiled statement against a {@link SQLiteDatabase} that can be reused.
 * The statement cannot return multiple rows, but 1x1 result sets are allowed.
 * Don't use SQLiteStatement constructor directly, please use
 * {@link SQLiteDatabase#compileStatement(String)}
 *
 * SQLiteStatement is not internally synchronized so code using a SQLiteStatement from multiple
 * threads should perform its own synchronization when using the SQLiteStatement.
 */
class SQLiteStatement
    : public ElRefBase
    , public SQLiteProgram
    , public ISQLiteStatement
{
public:
    /**
     * Don't use SQLiteStatement constructor directly, please use
     * {@link SQLiteDatabase#compileStatement(String)}
     * @param db
     * @param sql
     */
    /* package */ SQLiteStatement(
        /* [in] */ SQLiteDatabase* db,
        /* [in] */ const String& sql);

    ~SQLiteStatement();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
    virtual CARAPI SimpleQueryForInt64(
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

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI GetUniqueId(
        /* [out] */ Int32* value);

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

    /**
     * Clears all existing bindings. Unset bindings are treated as NULL.
     */
    CARAPI ClearBindings();

    /**
     * Release this program's resources, making it invalid.
     */
    CARAPI Close();

private:
    CARAPI NativeExecute();
    CARAPI Native1x1Int64(
        /* [out] */ Int64* value);
    CARAPI Native1x1String(
        /* [out] */ String* value);
};
#endif //__SQLITESTATEMENT_H__