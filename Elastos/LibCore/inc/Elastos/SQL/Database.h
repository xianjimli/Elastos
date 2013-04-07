#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <Elastos.SQL_server.h>
#include <elastos/Mutex.h>

#ifndef HAVE_SQLITE3_SHARED_CACHE
#define HAVE_SQLITE3_SHARED_CACHE 0
#endif

using namespace Elastos;
using namespace Elastos::Core::Threading;
/**
 * Main class wrapping an SQLite database.
 */
class Database {
protected:

    /**
     * Internal handle for the native SQLite API.
     */
    Int32 mHandle;// = 0;

    /**
     * Internal last error code for exec() methods.
     */

    Int32 mError_code;// = 0;
protected:

    /**
     * Destructor for object.
     */
    CARAPI Finalize();

    CARAPI FinalizeLocked();

    /**
     * Internal: set error code.
     * @param error_code new error code
     */
    CARAPI Set_last_error(
        /** [in] **/Int32 error_code);

    /**
     * Enable/disable shared cache mode (SQLite 3.x only).
     *
     * @param onoff boolean to enable or disable shared cache
     * @return boolean when true, function supported/succeeded
     */
    //native
    CARAPI _enable_shared_cache(
        /** [in] **/Boolean onoff,
        /** [out] **/Boolean* result);
public:
    Database();

    /**
     * Open an SQLite database file.
     *
     * @param filename the name of the database file
     * @param mode open mode (e.g. SQLITE_OPEN_READONLY)
     */
    CARAPI Open(
        /** [in] **/const String &filename,
        /** [in] **/Int32 mode);

    CARAPI OpenLocked(
        /** [in] **/const String &filename,
        /** [in] **/Int32 mode);

    /**
     * Open an SQLite database file.
     *
     * @param filename the name of the database file
     * @param mode open mode (e.g. SQLITE_OPEN_READONLY)
     * @param vfs VFS name (for SQLite >= 3.5)
     */
    CARAPI OpenEx(
        /** [in] **/const String &filename,
        /** [in] **/Int32 mode,
        /** [in] **/const String &vfs);

    CARAPI OpenExLocked(
    /** [in] **/const String &filename,
    /** [in] **/Int32 mode,
    /** [in] **/const String &vfs);

    /**
     * Open an SQLite database file.
     *
     * @param filename the name of the database file
     * @param mode open mode (e.g. SQLITE_OPEN_READONLY)
     * @param vfs VFS name (for SQLite >= 3.5)
     * @param ver2 flag to force version on create (false = SQLite3, true = SQLite2)
     */
    CARAPI OpenEx2(
        /** [in] **/const String &filename,
        /** [in] **/Int32 mode,
        /** [in] **/const String &vfs,
        /** [in] **/Boolean ver2);

    CARAPI OpenEx2Locked(
        /** [in] **/const String &filename,
        /** [in] **/Int32 mode,
        /** [in] **/const String &vfs,
        /** [in] **/Boolean ver2);

    /**
     * Open SQLite auxiliary database file for temporary
     * tables.
     *
     * @param filename the name of the auxiliary file or null
     */
    CARAPI Open_aux_file(
        /** [in] **/const String &filename);

    CARAPI Open_aux_fileLocked(
        /** [in] **/const String &filename);

    /**
     * Close the underlying SQLite database file.
     */
    CARAPI Close();

    CARAPI CloseLocked();

    /**
     * Execute an SQL statement and invoke callback methods
     * for each row of the result set.<P>
     *
     * It the method fails, an SQLite.Exception is thrown and
     * an error code is set, which later can be retrieved by
     * the last_error() method.
     *
     * @param sql the SQL statement to be executed
     * @param cb the object implementing the callback methods
     */
    CARAPI Exec(
        /** [in] **/const String &sql,
        /** [in] **/ICallback* cb);

    CARAPI ExecLocked(
        /** [in] **/const String &sql,
        /** [in] **/ICallback* cb);

    /**
     * Execute an SQL statement and invoke callback methods
     * for each row of the result set. Each '%q' or %Q in the
     * statement string is substituted by its corresponding
     * element in the argument vector.
     * <BR><BR>
     * Example:<BR>
     * <PRE>
     *   String args[] = new String[1];
     *   args[0] = "tab%";
     *   db.exec("select * from sqlite_master where type like '%q'",
     *           null, args);
     * </PRE>
     *
     * It the method fails, an SQLite.Exception is thrown and
     * an error code is set, which later can be retrieved by
     * the last_error() method.
     *
     * @param sql the SQL statement to be executed
     * @param cb the object implementing the callback methods
     * @param args arguments for the SQL statement, '%q' substitution
     */

    CARAPI ExecEx(
        /** [in] **/const String &sql,
        /** [in] **/ICallback* cb,
        /** [in] **/ArrayOf<String>* args);

    CARAPI ExecExLocked(
        /** [in] **/const String &sql,
        /** [in] **/ICallback* cb,
        /** [in] **/ArrayOf<String>* args);

    CARAPI Last_insert_rowid(
        /** [out] **/Int64* id);

    CARAPI Last_insert_rowidLocked(
        /** [out] **/Int64* id);

    /**
     * Abort the current SQLite operation.
     */
    CARAPI Interrupt();

    CARAPI InterruptLocked();
    /**
     * Return the number of changed rows for the last statement.
     */
    CARAPI Changes(
        /** [out] **/Int64* id);

    CARAPI ChangesLocked(
        /** [out] **/Int64* id);

    /**
     * Establish a busy callback method which gets called when
     * an SQLite table is locked.
     *
     * @param bh the object implementing the busy callback method
     */

    CARAPI Busy_handler(
        /** [in] **/IBusyHandler* bh);

    CARAPI Busy_handlerLocked(
        /** [in] **/IBusyHandler* bh);

    /**
     * Set the timeout for waiting for an SQLite table to become
     * unlocked.
     *
     * @param ms number of millisecond to wait
     */

    CARAPI Busy_timeout(
        /** [in] **/Int32 ms);

    CARAPI Busy_timeoutLocked(
        /** [in] **/Int32 ms);
    /**
     * Convenience method to retrieve an entire result
     * set into memory.
     *
     * @param sql the SQL statement to be executed
     * @param maxrows the max. number of rows to retrieve
     * @return result set
     */

    CARAPI Get_table(
        /** [in] **/const String &sql,
        /** [in] **/Int32 maxrows,
        /** [out] **/ITableResult** result);

    /**
     * Convenience method to retrieve an entire result
     * set into memory.
     *
     * @param sql the SQL statement to be executed
     * @return result set
     */
    CARAPI Get_tableEx(
        /** [in] **/const String &sql,
        /** [out] **/ITableResult** result);

    /**
     * Convenience method to retrieve an entire result
     * set into memory.
     *
     * @param sql the SQL statement to be executed
     * @param maxrows the max. number of rows to retrieve
     * @param args arguments for the SQL statement, '%q' substitution
     * @return result set
     */
    CARAPI Get_tableEx2(
        /** [in] **/const String &sql,
        /** [in] **/Int32 maxrows,
        /** [in] **/ArrayOf<String>* args,
        /** [out] **/ITableResult** result);

    /**
     * Convenience method to retrieve an entire result
     * set into memory.
     *
     * @param sql the SQL statement to be executed
     * @param args arguments for the SQL statement, '%q' substitution
     * @return result set
     */
    CARAPI Get_tableEx3(
        /** [in] **/const String &sql,
        /** [in] **/ArrayOf<String>* args,
        /** [out] **/ITableResult** result);

    /**
     * Convenience method to retrieve an entire result
     * set into memory.
     *
     * @param sql the SQL statement to be executed
     * @param args arguments for the SQL statement, '%q' substitution
     * @param tbl TableResult to receive result set
     * @return result set
     */
    CARAPI Get_tableEx4(
        /** [in] **/const String &sql,
        /** [in] **/ArrayOf<String>* args,
        /** [in] **/ITableResult* tbl);

    /**
     * See if an SQL statement is complete.
     * Returns true if the input string comprises
     * one or more complete SQL statements.
     *
     * @param sql the SQL statement to be checked
     */
    // synchronized//////////////////////////////////
    CARAPI Complete(
        /** [in] **/const String &sql,
        /** [out] **/Boolean* resukt);

    CARAPI CompleteLocked(
        /** [in] **/const String &sql,
        /** [out] **/Boolean* resukt);

    /**
     * Return SQLite version number as string.
     * Don't rely on this when both SQLite 2 and 3 are compiled
     * into the native part. Use the class method in this case.
     */
     //native
    CARAPI Version(
        /** [out] **/String* str);


    /**
     * Return SQLite version number as string.
     * If the database is not open, <tt>unknown</tt> is returned.
     */
    //native
    CARAPI Dbversion(
        /** [out] **/String* str);

    /**
     * Create regular function.
     *
     * @param name the name of the new function
     * @param nargs number of arguments to function
     * @param f interface of function
     */
    CARAPI Create_function(
        /** [in] **/const String &name,
        /** [in] **/Int32 nargs,
        /** [in] **/IFunction* f);

    CARAPI Create_functionLocked(
        /** [in] **/const String &name,
        /** [in] **/Int32 nargs,
        /** [in] **/IFunction* f);

    /**
     * Create aggregate function.
     *
     * @param name the name of the new function
     * @param nargs number of arguments to function
     * @param f interface of function
     */
    CARAPI Create_aggregate(
        /** [in] **/const String &name,
        /** [in] **/Int32 nargs,
        /** [in] **/IFunction* f);

    CARAPI Create_aggregateLocked(
        /** [in] **/const String &name,
        /** [in] **/Int32 nargs,
        /** [in] **/IFunction* f);

    /**
     * Set function return type. Only available in SQLite 2.6.0 and
     * above, otherwise a no-op.
     *
     * @param name the name of the function whose return type is to be set
     * @param type return type code, e.g. SQLite.Constants.SQLITE_NUMERIC
     */
    CARAPI Function_type(
        /** [in] **/const String &name,
        /** [in] **/Int32 type);

    CARAPI Function_typeLocked(
        /** [in] **/const String &name,
        /** [in] **/Int32 type);
    /**
     * Return the code of the last error occured in
     * any of the exec() methods. The value is valid
     * after an Exception has been reported by one of
     * these methods. See the <A HREF="Constants.html">Constants</A>
     * class for possible values.
     *
     * @return SQLite error code
     */
    CARAPI Last_error(
        /** [out] **/Int32* num);

    /**
     * Return last error message of SQLite3 engine.
     *
     * @return error string or null
     */
    CARAPI Error_message(
        /** [out] **/String* str);

    CARAPI Error_messageLocked(
        /** [out] **/String* str);

    /**
     * Return error string given SQLite error code (SQLite2).
     *
     * @param error_code the error code
     * @return error string
     */
    //native
    CARAPI Error_string(
        /** [in] **/Int32 error_code,
        /** [out] **/String* str);

    /**
     * Set character encoding.
     * @param enc name of encoding
     */
    CARAPI Set_encoding(
        /** [in] **/const String &enc);

    CARAPI Set_encodingLocked(
        /** [in] **/const String &enc);

    /**
     * Set authorizer function. Only available in SQLite 2.7.6 and
     * above, otherwise a no-op.
     *
     * @param auth the authorizer function
     */
    CARAPI Set_authorizer(
        /** [in] **/IAuthorizer* auth);

    CARAPI Set_authorizerLocked(
        /** [in] **/IAuthorizer* auth);

    /**
     * Set trace function. Only available in SQLite 2.7.6 and above,
     * otherwise a no-op.
     *
     * @param tr the trace function
     */
    CARAPI Trace(
        /** [in] **/ITrace* tr);

    CARAPI TraceLocked(
        /** [in] **/ITrace* tr);

    /**
     * Compile and return SQLite VM for SQL statement. Only available
     * in SQLite 2.8.0 and above, otherwise a no-op.
     *
     * @param sql SQL statement to be compiled
     * @return a Vm object
     */
    CARAPI Compile(
        /** [in] **/const String &sql,
        /** [out] **/IVm** vm);

    CARAPI CompileLocked(
        /** [in] **/const String &sql,
        /** [out] **/IVm** vm);

    /**
     * Compile and return SQLite VM for SQL statement. Only available
     * in SQLite 3.0 and above, otherwise a no-op.
     *
     * @param sql SQL statement to be compiled
     * @param args arguments for the SQL statement, '%q' substitution
     * @return a Vm object
     */
    CARAPI CompileEx(
        /** [in] **/const String &sql,
        /** [in] **/ArrayOf<String>* args,
        /** [out] **/IVm** vm);

    CARAPI CompileExLocked(
        /** [in] **/const String &sql,
        /** [in] **/ArrayOf<String>* args,
        /** [out] **/IVm** vm);

    /**
     * Prepare and return SQLite3 statement for SQL. Only available
     * in SQLite 3.0 and above, otherwise a no-op.
     *
     * @param sql SQL statement to be prepared
     * @return a Stmt object
     */
    CARAPI Prepare(
        /** [in] **/const String &sql,
        /** [out] **/IStmt** tmt);

    CARAPI PrepareLocked(
        /** [in] **/const String &sql,
        /** [out] **/IStmt** tmt);

    /**
     * Open an SQLite3 blob. Only available in SQLite 3.4.0 and above.
     * @param db database name
     * @param table table name
     * @param column column name
     * @param row row identifier
     * @param rw if true, open for read-write, else read-only
     * @return a Blob object
     */
    CARAPI Open_blob(
        /** [in] **/const String &db,
        /** [in] **/const String &table,
        /** [in] **/const String &column,
        /** [in] **/Int64 row,
        /** [in] **/Boolean rw,
        /** [out] **/IBlob2** blob);

    CARAPI Open_blobLocked(
        /** [in] **/const String &db,
        /** [in] **/const String &table,
        /** [in] **/const String &column,
        /** [in] **/Int64 row,
        /** [in] **/Boolean rw,
        /** [out] **/IBlob2** blob);

    /**
     * Check type of open database.
     * @return true if SQLite3 database
     */
    //native
    CARAPI Is3(
        /** [out] **/Boolean* result);

    /**
     * Establish a progress callback method which gets called after
     * N SQLite VM opcodes.
     *
     * @param n number of SQLite VM opcodes until callback is invoked
     * @param p the object implementing the progress callback method
     */
    CARAPI Progress_handler(
        /** [in] **/Int32 n,
        /** [in] **/IProgressHandler* p);

    CARAPI Progress_handlerLocked(
        /** [in] **/Int32 n,
        /** [in] **/IProgressHandler* p);

    /**
     * Specify key for encrypted database. To be called
     * right after open() on SQLite3 databases.
     * Not available in public releases of SQLite.
     *
     * @param ekey the key as byte array
     */
    CARAPI Key(
        /** [in] **/ArrayOf<Byte>* ekey);

    CARAPI KeyLocked(
        /** [in] **/ArrayOf<Byte>* ekey);

    /**
     * Specify key for encrypted database. To be called
     * right after open() on SQLite3 databases.
     * Not available in public releases of SQLite.
     *
     * @param skey the key as String
     */
    CARAPI KeyEx(
        /** [in] **/const String &skey);

    CARAPI KeyExLocked(
        /** [in] **/const String &skey);

    /**
     * Change the key of a encrypted database. The
     * SQLite3 database must have been open()ed.
     * Not available in public releases of SQLite.
     *
     * @param ekey the key as byte array
     */
    CARAPI Rekey(
        /** [in] **/ArrayOf<Byte>* ekey);

    CARAPI RekeyLocked(
        /** [in] **/ArrayOf<Byte>* ekey);

    /**
     * Change the key of a encrypted database. The
     * SQLite3 database must have been open()ed.
     * Not available in public releases of SQLite.
     *
     * @param skey the key as String
     */
    CARAPI RekeyEx(
        /** [in] **/const String &skey);

    CARAPI RekeyExLocked(
        /** [in] **/const String &skey);

    /**
     * Make long value from julian date for java.lang.Date
     *
     * @param d double value (julian date in SQLite3 format)
     * @return long
     */
    CARAPI Long_from_julian(
        /** [in] **/Double d,
        /** [out] **/Int64* result);

    /**
     * Make long value from julian date for java.lang.Date
     *
     * @param s string (double value) (julian date in SQLite3 format)
     * @return long
     */
    CARAPI Long_from_julianEx(
        /** [in] **/const String &s,
        /** [out] **/Int64* result);

    /**
     * Make julian date value from java.lang.Date
     *
     * @param ms millisecond value of java.lang.Date
     * @return double
     */
    CARAPI Julian_from_long(
        /** [in] **/Int64 ms,
        /** [out] **/Double* result);
private:
    /*
     * For backward compatibility to older sqlite.jar, sqlite_jni
     */
    //native
    CARAPI _open(
        /* [in] */ const String& filename,
        /* [in] */ const Int32 mode);

    /*
     * Newer full interface
     */
    //native
    CARAPI _open4(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode,
        /* [in] */ const String& vfs,
        /* [in] */ Boolean ver2);

    //native
    CARAPI _open_aux_file(String filename);;

    //native
    CARAPI _finalize();

    //native
    CARAPI _close();

    //native
    CARAPI _exec(
        /** [in] **/String sql,
        /** [in] **/ICallback* cb);

    //native
    CARAPI _execEx(
        /** [in] **/String sql,
        /** [in] **/ICallback* cb,
        /** [in] **/ArrayOf<String>* args);

    /**
     * Return the row identifier of the last inserted
     * row.
     */
    //native
    CARAPI_(Int64) _last_insert_rowid();

    //native
    CARAPI _interrupt();

    //native
    CARAPI_(Int64) _changes();

    //native
    CARAPI _busy_handler(
        /** [in] **/IBusyHandler* bh);

    //native
    CARAPI _busy_timeout(
        /** [in] **/Int32 ms);

    //native
    CARAPI_(Boolean) _complete(
        /** [in] **/String sql);

    //native
    CARAPI _create_function(
        /** [in] **/String name,
        /** [in] **/Int32 nargs,
        /** [in] **/IFunction* f);

    //native
    CARAPI _create_aggregate(
        /** [in] **/String name,
        /** [in] **/Int32 nargs,
        /** [in] **/IFunction* f);

    //native
    CARAPI _function_type(
        /** [in] **/String name,
        /** [in] **/Int32 type);

    //native
    CARAPI_(String) _errmsg();

    //native
    CARAPI _set_encoding(
        /** [in] **/String enc);

    //native
    CARAPI _set_authorizer(
        /** [in] **/IAuthorizer* auth);


    //native
    CARAPI _trace(
        /** [in] **/ITrace* tr);

    /**
     * Internal compile method.
     * @param sql SQL statement
     * @param vm Vm object
     */
    //native
    CARAPI Vm_compile(
        /** [in] **/String sql,
        /** [in] **/IVm* vm);

    /**
     * Internal compile method, SQLite 3.0 only.
     * @param sql SQL statement
     * @param args arguments for the SQL statement, '%q' substitution
     * @param vm Vm object
     */
    //native
    CARAPI Vm_compile_args(
        /** [in] **/String sql,
        /** [in] **/IVm* vm,
        /** [in] **/ArrayOf<String>* args);

    /**
     * Internal SQLite3 prepare method.
     * @param sql SQL statement
     * @param stmt Stmt object
     */
    //native
    CARAPI Stmt_prepare(
        /** [in] **/String sql,
        /** [in] **/IStmt* stmt);

    /**
     * Internal SQLite open blob method.
     * @param db database name
     * @param table table name
     * @param column column name
     * @param row row identifier
     * @param rw if true, open for read-write, else read-only
     * @param blob Blob object
     */
    //native
    CARAPI _open_blob(
        /** [in] **/String db,
        /** [in] **/String table,
        /** [in] **/String column,
		/** [in] **/Int64 row,
        /** [in] **/Boolean rw,
        /** [in] **/IBlob2* blob);

    //native
    CARAPI _progress_handler(
        /** [in] **/Int32 n,
        /** [in] **/IProgressHandler* p);

    //native
    CARAPI _key(
        /** [in] **/ArrayOf<Byte>* ekey);


    //native
    CARAPI _rekey(
        /** [in] **/ArrayOf<Byte>* ekey);



    /**
     * Internal native initializer.
     */
    //native
    CARAPI internal_init();



    /**
     * Static initializer to load the native part.
     */
/*
    static {
	try {
	    String path = System.getProperty("SQLite.library.path");
	    if (path == null || path.length() == 0) {
		System.loadLibrary("sqlite_jni");
	    } else {
		try {
		    java.lang.reflect.Method mapLibraryName;
		    Class param[] = new Class[1];
		    param[0] = String.class;
		    mapLibraryName = System.class.getMethod("mapLibraryName",
							    param);
		    Object args[] = new Object[1];
		    args[0] = "sqlite_jni";
		    String mapped = (String) mapLibraryName.invoke(null, args);
		    System.load(path + java.io.File.separator + mapped);
		} catch (Throwable t) {
		    System.err.println("Unable to load sqlite_jni from" +
				       "SQLite.library.path=" + path +
				       ", trying system default: " + t);
		    System.loadLibrary("sqlite_jni");
		}
	    }
	} catch (Throwable t) {
	    System.err.println("Unable to load sqlite_jni: " + t);
	}*/
	/*
	 * Call native initializer functions now, since the
	 * native part could have been linked statically, i.e.
	 * the try/catch above would have failed in that case.
	 */
/*	try {
	    internal_init();
	    new FunctionContext();
	} catch (java.lang.Exception e) {
	}
    }*/

protected:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif //__DATABASE_H__
