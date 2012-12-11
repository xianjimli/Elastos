#ifndef __SQLITEPROGRAM_H__
#define __SQLITEPROGRAM_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteClosable.h"
#include "database/sqlite/SQLiteDatabase.h"
#include "database/sqlite/SQLiteCompiledSql.h"
#include <elastos/AutoPtr.h>
#include <sqlite3.h>

/**
 * A base class for compiled SQLite programs.
 *
 * SQLiteProgram is not internally synchronized so code using a SQLiteProgram from multiple
 * threads should perform its own synchronization when using the SQLiteProgram.
 */
class SQLiteProgram : public SQLiteClosable
{
public:
    /* package */ SQLiteProgram(
        /* [in] */ SQLiteDatabase* db,
        /* [in] */ const String& sql);

    virtual ~SQLiteProgram();

    /**
     * Returns a unique identifier for this program.
     *
     * @return a unique identifier for this program
     */
    CARAPI_(Int32) GetUniqueId();

    /* package */ virtual CARAPI_(String) GetSqlString();

    /**
     * Bind a NULL value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind null to
     */
    virtual CARAPI BindNull(
        /* [in] */ Int32 index);

    /**
     * Bind a long value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    /**
     * Bind a double value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    /**
     * Bind a String value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    /**
     * Bind a byte array value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value);

    /**
     * Clears all existing bindings. Unset bindings are treated as NULL.
     */
    virtual CARAPI ClearBindings();

    /**
     * Release this program's resources, making it invalid.
     */
    virtual CARAPI Close();

protected:
    //@Override
    CARAPI OnAllReferencesReleased();

    //@Override
    CARAPI OnAllReferencesReleasedFromContainer();

    /**
     * @deprecated This method is deprecated and must not be used.
     *
     * @param sql the SQL string to compile
     * @param forceCompilation forces the SQL to be recompiled in the event that there is an
     *  existing compiled SQL program already around
     */
    //@Deprecated
    virtual CARAPI_(void) Compile(
        /* [in] */ const String& sql,
        /* [in] */ Boolean forceCompilation);

    /**
     * @deprecated This method is deprecated and must not be used.
     * Compiles SQL into a SQLite program.
     *
     * <P>The database lock must be held when calling this method.
     * @param sql The SQL to compile.
     */
    //@Deprecated
    CARAPI NativeCompile(
        /* [in] */ const String& sql);

    CARAPI NativeBindNull(
        /* [in] */ Int32 index);

    CARAPI NativeBindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI NativeBindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI NativeBindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    CARAPI NativeBindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value);

private:
    CARAPI_(void) ReleaseCompiledSqlIfNotInCache();

    CARAPI NativeClearBindings();

public:
    /** The SQL used to create this query */
    /* package */ String mSql;

protected:
    /** The database this program is compiled against.
     * @deprecated do not use this
     */
//    @Deprecated
    AutoPtr<SQLiteDatabase> mDatabase;

    /**
     * Native linkage, do not modify. This comes from the database and should not be modified
     * in here or in the native code.
     * @deprecated do not use this
     */
//    @Deprecated
    sqlite3* mNativeHandle;

    /**
     * SQLiteCompiledSql statement id is populated with the corresponding object from the above
     * member. This member is used by the native_bind_* methods
     * @deprecated do not use this
     */
//    @Deprecated
    sqlite3_stmt* mNativeStatement;

private:
    static const CString TAG;

    /**
     * the SQLiteCompiledSql object for the given sql statement.
     */
    AutoPtr<SQLiteCompiledSql> mCompiledSql;
};

#endif //__SQLITEPROGRAM_H__