#include "Database.h"
#include "cmdef.h"
#include <sqlite3.h>

Database::Database()
    :mHandle(0)
    ,mError_code(0)
{
}

ECode Database::Finalize()
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return FinalizeLocked();
}

ECode Database::FinalizeLocked()
{
    return _finalize();
}


ECode Database::Set_last_error(
    /** [in] **/Int32 error_code)
{
    mError_code = error_code;

    return NOERROR;
}

ECode Database::_enable_shared_cache(
    /** [in] **/Boolean onoff,
    /** [out] **/Boolean* result)
{
    assert(result != NULL);
#if HAVE_SQLITE3_SHARED_CACHE
    *result = (sqlite3_enable_shared_cache(onoff == TRUE) == SQLITE_OK) ? 
        TRUE : FALSE;
#else
    *result = FALSE;
#endif
    return NOERROR;
}

ECode Database::Open(
    /** [in] **/const String &filename, 
    /** [in] **/Int32 mode)
{
    if ((mode & 0200) != 0) {
        mode = IConstants_SQLITE_OPEN_READWRITE |
           IConstants_SQLITE_OPEN_CREATE;
    } else if ((mode & 0400) != 0) {
        mode = IConstants_SQLITE_OPEN_READONLY;
    }
    
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return OpenLocked(filename, mode);
}

ECode Database::OpenLocked(
    /** [in] **/const String &filename, 
    /** [in] **/Int32 mode)
{/*
    try {
    _open4(filename, mode, null, false);
    } catch (SQLite.Exception se) {
    throw se;
    } catch (java.lang.OutOfMemoryError me) {
    throw me;
    } catch (Throwable t) {
    _open(filename, mode);
    }*/
    return NOERROR; 
}

ECode Database::OpenEx(
    /** [in] **/const String &filename, 
    /** [in] **/Int32 mode, 
    /** [in] **/const String &vfs)
{
    if ((mode & 0200) != 0) {
        mode = IConstants_SQLITE_OPEN_READWRITE |
           IConstants_SQLITE_OPEN_CREATE;
    } else if ((mode & 0400) != 0) {
        mode = IConstants_SQLITE_OPEN_READONLY;
    }

    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return OpenExLocked(filename, mode, vfs);
}

ECode Database::OpenExLocked(
    /** [in] **/const String &filename, 
    /** [in] **/Int32 mode, 
    /** [in] **/const String &vfs)
{/*
    try {
    _open4(filename, mode, vfs, false);
    } catch (SQLite.Exception se) {
    throw se;
    } catch (java.lang.OutOfMemoryError me) {
    throw me;
    } catch (Throwable t) {
    _open(filename, mode);
    }*/
    return NOERROR; 
}

ECode Database::OpenEx2(
    /** [in] **/const String &filename, 
    /** [in] **/Int32 mode, 
    /** [in] **/const String &vfs, 
    /** [in] **/Boolean ver2)
{
    if ((mode & 0200) != 0) {
        mode = IConstants_SQLITE_OPEN_READWRITE |
           IConstants_SQLITE_OPEN_CREATE;
    } else if ((mode & 0400) != 0) {
        mode = IConstants_SQLITE_OPEN_READONLY;
    }

    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return OpenEx2Locked(filename, mode, vfs, ver2);
}

ECode Database::OpenEx2Locked(
    /** [in] **/const String &filename, 
    /** [in] **/Int32 mode, 
    /** [in] **/const String &vfs, 
    /** [in] **/Boolean ver2)
{/*
    try {
    _open4(filename, mode, vfs, ver2);
    } catch (SQLite.Exception se) {
    throw se;
    } catch (java.lang.OutOfMemoryError me) {
    throw me;
    } catch (Throwable t) {
    _open(filename, mode);
    }*/

    return NOERROR; 
}

ECode Database::Open_aux_file(
    /** [in] **/const String &filename)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Open_aux_fileLocked(filename);
}

ECode Database::Open_aux_fileLocked(
    /** [in] **/const String &filename)
{
    return _open_aux_file(filename);
}

ECode Database::Close()
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return CloseLocked();
}

ECode Database::CloseLocked()
{
    return _close();
}

ECode Database::Exec(
    /** [in] **/const String &sql, 
    /** [in] **/ICallback* cb)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return ExecLocked(sql, cb);
}

ECode Database::ExecLocked(
    /** [in] **/const String &sql, 
    /** [in] **/ICallback* cb)
{
    return _exec(sql, cb);
}

ECode Database::ExecEx(
    /** [in] **/const String &sql, 
    /** [in] **/ICallback* cb,
    /** [in] **/ArrayOf<String>* args)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return ExecExLocked(sql, cb, args);
}

ECode Database::ExecExLocked(
    /** [in] **/const String &sql, 
    /** [in] **/ICallback* cb,
    /** [in] **/ArrayOf<String>* args)
{
    return _execEx(sql, cb, args);
}

ECode Database::Last_insert_rowid(
    /** [out] **/Int64* id)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Last_insert_rowidLocked(id);
}

ECode Database::Last_insert_rowidLocked(
    /** [out] **/Int64* id)
{
    assert(id != NULL);
    *id = _last_insert_rowid();
    return NOERROR;
}

ECode Database::Interrupt()
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return InterruptLocked();
}

ECode Database::InterruptLocked()
{
    return _interrupt();
}

ECode Database::Changes(
    /** [out] **/Int64* id)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return ChangesLocked(id);
}

ECode Database::ChangesLocked(
    /** [out] **/Int64* id)
{
    assert(id != NULL);
    *id = _changes();
    return NOERROR;
}

ECode Database::Busy_handler(
    /** [in] **/IBusyHandler* bh)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Busy_handlerLocked(bh);
}

ECode Database::Busy_handlerLocked(
    /** [in] **/IBusyHandler* bh)
{
    return _busy_handler(bh);
}

ECode Database::Busy_timeout(
    /** [in] **/Int32 ms)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Busy_timeoutLocked(ms);
}

ECode Database::Busy_timeoutLocked(
    /** [in] **/Int32 ms)
{
    return _busy_timeout(ms);
}

ECode Database::Get_table(
    /** [in] **/const String &sql, 
    /** [in] **/Int32 maxrows,
    /** [out] **/ITableResult** result)
{/*
    TableResult ret = new TableResult(maxrows);
    if (!is3()) {
        try {
        exec(sql, ret);
        } catch (SQLite.Exception e) {
        if (maxrows <= 0 || !ret.atmaxrows) {
            throw e;
        }
        }
    } else {
        synchronized(this) {
        // only one statement !!! 
        Vm vm = compile(sql);
        set_last_error(vm.error_code);
        if (ret.maxrows > 0) {
            while (ret.nrows < ret.maxrows && vm.step(ret)) {
            set_last_error(vm.error_code);
            }
        } else {
            while (vm.step(ret)) {
            set_last_error(vm.error_code);
            }
        }
        vm.finalize();
        }
    }
    return ret;*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::Get_tableEx(
    /** [in] **/const String &sql,
    /** [out] **/ITableResult** result)
{
    return Get_table(sql, 0, result);
}

ECode Database::Get_tableEx2(
    /** [in] **/const String &sql, 
    /** [in] **/Int32 maxrows, 
    /** [in] **/ArrayOf<String>* args,
    /** [out] **/ITableResult** result)
{/*
    TableResult ret = new TableResult(maxrows);
    if (!is3()) {
        try {
        exec(sql, ret, args);
        } catch (SQLite.Exception e) {
        if (maxrows <= 0 || !ret.atmaxrows) {
            throw e;
        }
        }
    } else {
        synchronized(this) {
        // only one statement !!! 
        Vm vm = compile(sql, args);
        set_last_error(vm.error_code);
        if (ret.maxrows > 0) {
            while (ret.nrows < ret.maxrows && vm.step(ret)) {
            set_last_error(vm.error_code);
            }
        } else {
            while (vm.step(ret)) {
            set_last_error(vm.error_code);
            }
        }
        vm.finalize();
        }
    }
    return ret;*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::Get_tableEx3(
    /** [in] **/const String &sql, 
    /** [in] **/ArrayOf<String>* args,
    /** [out] **/ITableResult** result)
{
    return Get_tableEx2(sql, 0, args, result);
}

ECode Database::Get_tableEx4(
    /** [in] **/const String &sql, 
    /** [in] **/ArrayOf<String>* args, 
    /** [in] **/ITableResult* tbl)
{/*
    tbl.clear();
    if (!is3()) {
        try {
        exec(sql, tbl, args);
        } catch (SQLite.Exception e) {
        if (tbl.maxrows <= 0 || !tbl.atmaxrows) {
            throw e;
        }
        }
    } else {
        synchronized(this) {
        // only one statement !!! 
        Vm vm = compile(sql, args);
        if (tbl.maxrows > 0) {
            while (tbl.nrows < tbl.maxrows && vm.step(tbl)) {
            set_last_error(vm.error_code);
            }
        } else {
            while (vm.step(tbl)) {
            set_last_error(vm.error_code);
            }
        }
        vm.finalize();
        }
    }*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::Complete(
    /** [in] **/const String &sql,
    /** [out] **/Boolean* result)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return CompleteLocked(sql, result);
}

ECode Database::CompleteLocked(
    /** [in] **/const String &sql,
    /** [out] **/Boolean* result)
{
    assert(result != NULL);
    *result = _complete(sql);
    return NOERROR;
}

ECode Database::Version(
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::Dbversion(
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::Create_function(
    /** [in] **/const String &name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Create_functionLocked(name, nargs, f);
}

ECode Database::Create_functionLocked(
    /** [in] **/const String &name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    return _create_function(name, nargs, f);
}

ECode Database::Create_aggregate(
    /** [in] **/const String &name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Create_aggregateLocked(name, nargs, f);
}

ECode Database::Create_aggregateLocked(
    /** [in] **/const String &name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    return _create_aggregate(name, nargs, f);
}

ECode Database::Function_type(
    /** [in] **/const String &name, 
    /** [in] **/Int32 type)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Function_typeLocked(name, type);
}

ECode Database::Function_typeLocked(
    /** [in] **/const String &name, 
    /** [in] **/Int32 type)
{
    return _function_type(name, type);
}

ECode Database::Last_error(
    /** [out] **/Int32* num)
{
    assert(num != NULL);
    *num = mError_code;
    return NOERROR;
}

ECode Database::Error_message(
    /** [out] **/String* str)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Error_messageLocked(str);
}

ECode Database::Error_messageLocked(
    /** [out] **/String* str)
{
    assert(str != NULL);
    *str = _errmsg();
    return NOERROR;
}

ECode Database::Error_string(
    /** [in] **/Int32 error_code,
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::Set_encoding(
    /** [in] **/const String &enc)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Set_encodingLocked(enc);
}

ECode Database::Set_encodingLocked(
    /** [in] **/const String &enc)
{
    return _set_encoding(enc);
}

ECode Database::Set_authorizer(
    /** [in] **/IAuthorizer* auth)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Set_authorizerLocked(auth);
}

ECode Database::Set_authorizerLocked(
    /** [in] **/IAuthorizer* auth)
{
    return _set_authorizer(auth);
}

ECode Database::Trace(
    /** [in] **/ITrace* tr)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return TraceLocked(tr);
}

ECode Database::TraceLocked(
    /** [in] **/ITrace* tr)
{
    return _trace(tr);
}

ECode Database::Compile(
    /** [in] **/const String &sql,
    /** [out] **/IVm** vm)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return CompileLocked(sql, vm);
}

ECode Database::CompileLocked(
    /** [in] **/const String &sql,
    /** [out] **/IVm** vm)
{/*
    Vm vm = new Vm();
    vm_compile(sql, vm);
    return vm;*/
    return NOERROR;
}

ECode Database::CompileEx(
    /** [in] **/const String &sql, 
    /** [in] **/ArrayOf<String>* args,
    /** [out] **/IVm** vm)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return CompileExLocked(sql, args, vm);
}

ECode Database::CompileExLocked(
    /** [in] **/const String &sql, 
    /** [in] **/ArrayOf<String>* args,
    /** [out] **/IVm** vm)
{/*
     vm = new Vm();
    vm_compile_args(sql, vm, args);
    return vm;*/
    return NOERROR;
}

ECode Database::Prepare(
    /** [in] **/const String &sql,
    /** [out] **/IStmt** tmt)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return PrepareLocked(sql, tmt);
}

ECode Database::PrepareLocked(
    /** [in] **/const String &sql,
    /** [out] **/IStmt** tmt)
{/*
    Stmt stmt = new Stmt();
    stmt_prepare(sql, stmt);
    return stmt;*/
    return NOERROR;
}

ECode Database::Open_blob(
    /** [in] **/const String &db, 
    /** [in] **/const String &table, 
    /** [in] **/const String &column,
    /** [in] **/Int64 row, 
    /** [in] **/Boolean rw,
    /** [out] **/IBlob2** blob)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Open_blobLocked(db, table, column, row, rw, blob);
}

ECode Database::Open_blobLocked(
    /** [in] **/const String &db, 
    /** [in] **/const String &table, 
    /** [in] **/const String &column,
    /** [in] **/Int64 row, 
    /** [in] **/Boolean rw,
    /** [out] **/IBlob2** blob)
{/*
    Blob2 blob = new Blob();
    _open_blob(db, table, column, row, rw, blob);
    return blob;*/
    return NOERROR;
}

ECode Database::Is3(
    /** [out] **/Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::Progress_handler(
    /** [in] **/Int32 n, 
    /** [in] **/IProgressHandler* p)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return Progress_handlerLocked(n, p);
}

ECode Database::Progress_handlerLocked(
    /** [in] **/Int32 n, 
    /** [in] **/IProgressHandler* p)
{
    return _progress_handler(n, p);
}

ECode Database::Key(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return KeyLocked(ekey);
}

ECode Database::KeyLocked(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    return _key(ekey);
}


ECode Database::KeyEx(
    /** [in] **/const String &skey)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return KeyExLocked(skey);
}

ECode Database::KeyExLocked(
    /** [in] **/const String &skey)
{/*
    byte ekey[] = null;
    if (skey != null && skey.length() > 0) {
    ekey = new byte[skey.length()];
    for (int i = 0; i< skey.length(); i++) {
        char c = skey.charAt(i);
        ekey[i] = (byte) ((c & 0xff) ^ (c >> 8));
    }
    }
    _key(ekey);*/
    return NOERROR;
}

ECode Database::Rekey(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return RekeyLocked(ekey);
}

ECode Database::RekeyLocked(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    return _rekey(ekey);
}

ECode Database::RekeyEx(
    /** [in] **/const String &skey)
{
    //Mutex* selfLock = GetSelfLock();
    //Mutex::Autolock lock(*selfLock);

    return RekeyExLocked(skey);
}

ECode Database::RekeyExLocked(
    /** [in] **/const String &skey)
{/*
    byte ekey[] = null;
    if (skey != null && skey.length() > 0) {
    ekey = new byte[skey.length()];
    for (int i = 0; i< skey.length(); i++) {
        char c = skey.charAt(i);
        ekey[i] = (byte) ((c & 0xff) ^ (c >> 8));
    }
    }
    _rekey(ekey);*/
    return NOERROR;
}

ECode Database::Long_from_julian(
    /** [in] **/Double d,
    /** [out] **/Int64* result)
{
    assert(result != NULL);
    d -= 2440587.5;
    d *= 86400000.0;
    *result = (Int64)d;
    return NOERROR;
}

ECode Database::Long_from_julianEx(
    /** [in] **/const String &s,
    /** [out] **/Int64* result)
{/*
    try {
        double d = Double.parseDouble(s); // android-changed: performance
        return long_from_julian(d);
    } catch (java.lang.Exception ee) {
        throw new SQLite.Exception("not a julian date");
    }
*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::Julian_from_long(
    /** [in] **/Int64 ms,
    /** [out] **/Double* result)
{
    assert(result != NULL);
    Double adj = (ms < 0) ? 0 : 0.5;
    Double d = (ms + adj) / 86400000.0 + 2440587.5;
    *result = d;
    return NOERROR;
}

ECode Database::_open(
    /** [in] **/String filename, 
    /** [in] **/Int32 mode)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_open4(
    /** [in] **/String filename, 
    /** [in] **/Int32 mode, 
    /** [in] **/String vfs,
	/** [in] **/Boolean ver2)
{
    
    return NOERROR;
}

ECode Database::_open_aux_file(String filename)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_finalize()
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_close()
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_exec(
    /** [in] **/String sql, 
    /** [in] **/ICallback* cb)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_execEx(
    /** [in] **/String sql, 
    /** [in] **/ICallback* cb, 
    /** [in] **/ArrayOf<String>* args)
{
    return E_NOT_IMPLEMENTED;
}

Int64 Database::_last_insert_rowid()
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_interrupt()
{
    return E_NOT_IMPLEMENTED;
}

Int64 Database::_changes()
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_busy_handler(
    /** [in] **/IBusyHandler* bh)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_busy_timeout(
    /** [in] **/Int32 ms)
{
    return E_NOT_IMPLEMENTED;
}

Boolean Database::_complete(
    /** [in] **/String sql)
{
    return FALSE;
}

ECode Database::_create_function(
    /** [in] **/String name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    return E_NOT_IMPLEMENTED;
}
    
ECode Database::_create_aggregate(
    /** [in] **/String name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_function_type(
    /** [in] **/String name, 
    /** [in] **/Int32 type)
{
    return E_NOT_IMPLEMENTED;
}

String Database::_errmsg()
{
    return String("aaa");
}
    

ECode Database::_set_encoding(
    /** [in] **/String enc)
{
    return E_NOT_IMPLEMENTED;
}


ECode Database::_set_authorizer(
    /** [in] **/IAuthorizer* auth)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_trace(
    /** [in] **/ITrace* tr)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::vm_compile(
    /** [in] **/String sql, 
    /** [in] **/IVm* vm)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::vm_compile_args(
    /** [in] **/String sql, 
    /** [in] **/IVm* vm, 
    /** [in] **/ArrayOf<String>* args)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::stmt_prepare(
    /** [in] **/String sql, 
    /** [in] **/IStmt* stmt)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_open_blob(
    /** [in] **/String db, 
    /** [in] **/String table, 
    /** [in] **/String column,
	/** [in] **/Int64 row, 
    /** [in] **/Boolean rw, 
    /** [in] **/IBlob2* blob)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_progress_handler(
    /** [in] **/Int32 n, 
    /** [in] **/IProgressHandler* p)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_key(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::_rekey(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    return E_NOT_IMPLEMENTED;
}

ECode Database::internal_init()
{
    return E_NOT_IMPLEMENTED;
}