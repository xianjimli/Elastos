#ifndef __SQLITEPROGRAM_H__
#define __SQLITEPROGRAM_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteClosable.h"
#include <elastos/AutoPtr.h>
#include <sqlite3.h>
class SQLiteProgram : public SQLiteClosable
{
public:
    SQLiteProgram();

    SQLiteProgram(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String sql);

    ~SQLiteProgram();

    /**
     * Returns a unique identifier for this program.
     *
     * @return a unique identifier for this program
     */
    CARAPI GetUniqueId(
        /* [out] */ Int32* value);

    virtual CARAPI GetSqlString(
        /* [out] */ String* value);

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
    virtual CARAPI BindLong(
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
        /* [in] */ String value); 

    /**
     * Bind a byte array value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ ArrayOf<Byte>* value); 

    /**
     * Clears all existing bindings. Unset bindings are treated as NULL.
     */
    virtual CARAPI ClearBindings();

    /**
     * Release this program's resources, making it invalid.
     */
    virtual CARAPI Close();

protected:
    CARAPI OnAllReferencesReleased();

    CARAPI OnAllReferencesReleasedFromContainer();

    /**
     * @deprecated This method is deprecated and must not be used.
     *
     * @param sql the SQL string to compile
     * @param forceCompilation forces the SQL to be recompiled in the event that there is an
     *  existing compiled SQL program already around
     */
//    @Deprecated
    virtual CARAPI Compile(
        /* [in] */ String sql,
        /* [in] */ Boolean forceCompilation);
private:
    CARAPI_(void) Init();

    CARAPI ReleaseCompiledSqlIfNotInCache();

    CARAPI Native_Compile(
        /* [in] */ String sql);

    CARAPI Native_Finalize();

    CARAPI Native_Bind_Null(
        /* [in] */ Int32 index);
    
    CARAPI Native_Bind_Long(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI Native_Bind_Double(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI Native_Bind_String(
        /* [in] */ Int32 index,
        /* [in] */ String value);

    CARAPI Native_Bind_Blob(
        /* [in] */ Int32 index,
        /* [in] */ ArrayOf<Byte> value);

    CARAPI Native_Clear_Bindings();
private:
    static const String TAG;

    /**
     * the SQLiteCompiledSql object for the given sql statement.
     */
    AutoPtr<ISQLiteCompiledSql> mCompiledSql;
protected:
    /** The database this program is compiled against.
     * @deprecated do not use this
     */
//    @Deprecated
    AutoPtr<ISQLiteDatabase> mDatabase;

    String mSql;

    /**
     * Native linkage, do not modify. This comes from the database and should not be modified
     * in here or in the native code.
     * @deprecated do not use this
     */
//    @Deprecated
    Int32 nHandle;

    /**
     * SQLiteCompiledSql statement id is populated with the corresponding object from the above
     * member. This member is used by the native_bind_* methods
     * @deprecated do not use this
     */
//    @Deprecated
    Int32 nStatement;
};
#endif //__SQLITEPROGRAM_H__