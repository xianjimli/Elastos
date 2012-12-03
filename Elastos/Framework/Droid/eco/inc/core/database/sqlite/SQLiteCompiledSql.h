#ifndef __SQLITECOMPILEDSQL_H__
#define __SQLITECOMPILEDSQL_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <sqlite3.h>
/**
 * This class encapsulates compilation of sql statement and release of the compiled statement obj.
 * Once a sql statement is compiled, it is cached in {@link SQLiteDatabase}
 * and it is released in one of the 2 following ways
 * 1. when {@link SQLiteDatabase} object is closed.
 * 2. if this is not cached in {@link SQLiteDatabase}, {@link android.database.Cursor#close()}
 * releaases this obj.
 */
class SQLiteCompiledSql
{
public:
    SQLiteCompiledSql();

    CARAPI Init(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String sql);

    /* package */ virtual CARAPI ReleaseSqlStatement();

    /**
     * returns true if acquire() succeeds. false otherwise.
     */
    /* package */ virtual CARAPI Acquire(
        /* [out] */ Boolean* result);

    /* package */ virtual CARAPI Release();

    /**
     * Make sure that the native resource is cleaned up.
     */
    ~SQLiteCompiledSql();

private:

    /**
     * Compiles the given SQL into a SQLite byte code program using sqlite3_prepare_v2(). If
     * this method has been called previously without a call to close and forCompilation is set
     * to false the previous compilation will be used. Setting forceCompilation to true will
     * always re-compile the program and should be done if you pass differing SQL strings to this
     * method.
     *
     * <P>Note: this method acquires the database lock.</P>
     *
     * @param sql the SQL string to compile
     * @param forceCompilation forces the SQL to be recompiled in the event that there is an
     *  existing compiled SQL program already around
     */
    virtual CARAPI Compile(
        /* [in] */ String sql,
        /* [in] */ Boolean forceCompilation);

    CARAPI NativeCompile(
        /* [in] */ String sql);

    CARAPI NativeFinalize();
    /**
     * Compiles SQL into a SQLite program.
     *
     * <P>The database lock must be held when calling this method.
     * @param sql The SQL to compile.
     */
//    private final native void native_compile(String sql);
//    private final native void native_finalize();
protected:
    /** The database this program is compiled against. */
    /* package */ AutoPtr<ISQLiteDatabase> mDatabase;

    /**
     * Native linkage, do not modify. This comes from the database.
     */
    /* package */ Int32 nHandle;

    /**
     * Native linkage, do not modify. When non-0 this holds a reference to a valid
     * sqlite3_statement object. It is only updated by the native code, but may be
     * checked in this class when the database lock is held to determine if there
     * is a valid native-side program or not.
     */
    /* package */ Int32 nStatement;
private:
    static const String TAG;

    /** the following are for debugging purposes */
    String mSqlStmt;

//    private Throwable mStackTrace = null;

    /** when in cache and is in use, this member is set */
    Boolean mInUse;
};

#endif //__SQLITECOMPILEDSQL_H__