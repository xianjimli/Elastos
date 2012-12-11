#ifndef __SQLITECOMPILEDSQL_H__
#define __SQLITECOMPILEDSQL_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteDatabase.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <sqlite3.h>

class SQLiteDatabase;

/**
 * This class encapsulates compilation of sql statement and release of the compiled statement obj.
 * Once a sql statement is compiled, it is cached in {@link SQLiteDatabase}
 * and it is released in one of the 2 following ways
 * 1. when {@link SQLiteDatabase} object is closed.
 * 2. if this is not cached in {@link SQLiteDatabase}, {@link android.database.Cursor#close()}
 * releaases this obj.
 */
class SQLiteCompiledSql : public ElRefBase
{
public:
    SQLiteCompiledSql(
        /* [in] */ SQLiteDatabase* db,
        /* [in] */ const String& sql);

    /**
     * Make sure that the native resource is cleaned up.
     */
    virtual ~SQLiteCompiledSql();

    /* package */ virtual CARAPI_(void) ReleaseSqlStatement();

    /**
     * returns true if acquire() succeeds. false otherwise.
     */
    /* package */ virtual CARAPI_(Boolean) Acquire();

    // rename Release to Dismiss
    /* package */ virtual CARAPI_(void) Dismiss();

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
    CARAPI Compile(
        /* [in] */ const String& sql,
        /* [in] */ Boolean forceCompilation);

    /**
     * Compiles SQL into a SQLite program.
     *
     * <P>The database lock must be held when calling this method.
     * @param sql The SQL to compile.
     */
    CARAPI NativeCompile(
        /* [in] */ const String& sql);

    CARAPI_(void) NativeFinalize();

public:
    /** The database this program is compiled against. */
    /* package */ AutoPtr<SQLiteDatabase> mDatabase;

    /**
     * Native linkage, do not modify. This comes from the database.
     */
    /* package */ sqlite3* mNativeHandle;

    /**
     * Native linkage, do not modify. When non-0 this holds a reference to a valid
     * sqlite3_statement object. It is only updated by the native code, but may be
     * checked in this class when the database lock is held to determine if there
     * is a valid native-side program or not.
     */
    /* package */ sqlite3_stmt* mNativeStatement;

private:
    static const CString TAG;

    /** the following are for debugging purposes */
    String mSqlStmt;
//    private Throwable mStackTrace = null;

    /** when in cache and is in use, this member is set */
    Boolean mInUse;
    Mutex mLock;
};

#endif //__SQLITECOMPILEDSQL_H__