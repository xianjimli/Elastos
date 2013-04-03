#include "Database.h"
#include "cmdef.h"
#include <sqlite3.h>
#include "CTableResult.h"
//#include "TableResult.h"
#include "CVm.h"
#include "CStmt.h"
#include "CBlob2.h"

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
{
    //TableResult ret = new TableResult(maxrows);
    AutoPtr<ITableResult> ret;
    CTableResult::New(maxrows, (ITableResult**)&ret);
    TableResult* tr = (TableResult *)ret->Probe(EIID_ITableResult);

    Boolean res;
    Is3(&res);
    if (!res) {
        if(NOERROR != Exec(sql, (ICallback*)ret))
        {
            if (maxrows <= 0 || !tr->mAtmaxrows) {
                return E_SQL_EXCEPTION;
            }
        }
    } else {
  //      synchronized(this) {
        // only one statement !!! 
        AutoPtr<IVm> vm;
        Compile(sql, (IVm**)&vm);
        Vm* cvm = (Vm *)vm->Probe(EIID_IVm);
        Set_last_error(cvm->mError_code);
        if (tr->mMaxrows > 0) {
            Boolean result;
            cvm->Step((ICallback*)ret, &result);
            while (tr->mNrows < tr->mMaxrows && result) {
                Set_last_error(cvm->mError_code);
                cvm->Step((ICallback*)ret, &result);
            }
        } else {
            Boolean result;
            cvm->Step((ICallback*)ret, &result);
            while (result) {
                Set_last_error(cvm->mError_code);
                cvm->Step((ICallback*)ret, &result);
            }
        }
        cvm->Finalize();
      //  }
    }
    *result = ret;
    return NOERROR;
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
{
    //TableResult ret = new TableResult(maxrows);
    AutoPtr<ITableResult> ret;
    CTableResult::New(maxrows, (ITableResult**)&ret);
    TableResult* tr = (TableResult *)ret->Probe(EIID_ITableResult);

    Boolean res;
    Is3(&res);
    if (!res) {
        if(NOERROR != ExecEx(sql, (ICallback*)ret, args)){
            if (tr->mMaxrows <= 0 || !tr->mAtmaxrows) {
                //throw e;
                return E_SQL_EXCEPTION;
            }
        }
 
    } else {
      //  synchronized(this) {
        // only one statement !!! 
        //Vm vm = compile(sql, args);
        AutoPtr<IVm> vm;
        CompileEx(sql, args, (IVm**)&vm);
        Vm* cvm = (Vm *)vm->Probe(EIID_IVm);

        Set_last_error(cvm->mError_code);
        if (tr->mMaxrows > 0) {
            Boolean result;
            cvm->Step((ICallback*)ret, &result);
            while (tr->mNrows < tr->mMaxrows && result) {
                Set_last_error(cvm->mError_code);
                cvm->Step((ICallback*)ret, &result);
            }
        } else {
            Boolean result;
            cvm->Step((ICallback*)ret, &result);
            while (result) {
                Set_last_error(cvm->mError_code);
                cvm->Step((ICallback*)ret, &result);
            }
        }
        cvm->Finalize();
     //   }
    }
    *result = ret;
    return NOERROR;
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
{
    TableResult* tr = (TableResult *)tbl->Probe(EIID_ITableResult);
    tr->Clear();
    Boolean res;
    Is3(&res);
    if (!res) {
        if(NOERROR != ExecEx(sql, tbl, args)){
            if (tr->mMaxrows <= 0 || !tr->mAtmaxrows) {
                return E_SQL_EXCEPTION;
            }
        }
    } else {
     //   synchronized(this) {
        // only one statement !!! 
        //Vm vm = compile(sql, args);
        AutoPtr<IVm> vm;
        CompileEx(sql, args, (IVm**)&vm);
        Vm* cvm = (Vm *)vm->Probe(EIID_IVm);
        if (tr->mMaxrows > 0) {
            Boolean result;
            cvm->Step((ICallback*)tbl, &result);
            while (tr->mNrows < tr->mMaxrows && result) {
                Set_last_error(cvm->mError_code);
                cvm->Step((ICallback*)tbl, &result);
            }
        } else {
            Boolean result;
            cvm->Step((ICallback*)tbl, &result);
            while (result) {
                Set_last_error(cvm->mError_code);
                cvm->Step((ICallback*)tbl, &result);
            }
        }
        vm->Finalize();
     //   }
    }
    return NOERROR;
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
        /* CHECK THIS */
#if HAVE_BOTH_SQLITE
    *str = String(sqlite_libversion());
    return NOERROR;
#else
#if HAVE_SQLITE2
    *str = String(sqlite_libversion());
    return NOERROR;
#else
    *str = String(sqlite3_libversion());
    return NOERROR;
#endif
#endif
    return NOERROR;
}

ECode Database::Dbversion(
    /** [out] **/String* str)
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        return (*env)->NewStringUTF(env, sqlite3_libversion());
    } else {
        return (*env)->NewStringUTF(env, sqlite_libversion());
    }
#else
#if HAVE_SQLITE2
    return (*env)->NewStringUTF(env, sqlite_libversion());
#else
    return (*env)->NewStringUTF(env, sqlite3_libversion());
#endif
#endif
    }
    return (*env)->NewStringUTF(env, "unknown");*/
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
{
    AutoPtr<IVm> mvm;
    CVm::New((IVm**)&mvm);
    Vm_compile(sql, mvm);
    *vm = (IVm*)mvm;
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
{
    AutoPtr<IVm> mvm;
    CVm::New((IVm**)&mvm);
    Vm_compile_args(sql, mvm, args);
    *vm = (IVm*)mvm;
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
{
    AutoPtr<IStmt> stmt;
    CStmt::New((IStmt**)&stmt);
    Stmt_prepare(sql, stmt);
    *tmt = (IStmt*)stmt;
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
{
    AutoPtr<IBlob2> blob2;
    CBlob2::New((IBlob2**)&blob2);
    _open_blob(db, table, column, row, rw, blob2);
    *blob = (IBlob2*)blob2;
    return NOERROR;
}

ECode Database::Is3(
    /** [out] **/Boolean* result)
{/*
#if HAVE_BOTH_SQLITE
    handle *h = gethandle(env, obj);

    if (h) {
    return h->is3 ? JNI_TRUE : JNI_FALSE;
    }
    return JNI_FALSE;
#else
#if HAVE_SQLITE2
    return JNI_FALSE;
#endif
#if HAVE_SQLITE3
    return JNI_TRUE;
#endif
#endif*/
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
{
    ArrayOf<Byte> *ekey;
    if (!skey.IsNull() && skey.GetLength() > 0) {
    ekey = ArrayOf<Byte>::Alloc(skey.GetLength());
    for (Int32 i = 0; i< skey.GetLength(); i++) {
        Char8 c = skey.GetChar(i);
        (*ekey)[i] = (Byte) ((c & 0xff) ^ (c >> 8));
    }
    }
    _key(ekey);
    ArrayOf<Byte>::Free(ekey);
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
{
    ArrayOf<Byte> *ekey = NULL;
    if (!skey.IsNull()  && skey.GetLength() > 0) {
        ekey = ArrayOf<Byte>::Alloc(skey.GetLength());
        for (Int32 i = 0; i< skey.GetLength(); i++) {
            Char8 c = skey.GetChar(i);
            (*ekey)[i] = (Byte) ((c & 0xff) ^ (c >> 8));
        }
    }
    _rekey(ekey);
    ArrayOf<Byte>::Free(ekey);
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
{
    Double d = s.ToDouble();
    Long_from_julian(d, result);
    return NOERROR;
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
    _open4(filename, mode, String(NULL), FALSE);
    return E_NOT_IMPLEMENTED;
}

ECode Database::_open4(
    /** [in] **/String filename, 
    /** [in] **/Int32 mode, 
    /** [in] **/String vfs,
    /** [in] **/Boolean ver2)
{/*
    handle *h = gethandle(env, obj);
    jthrowable exc;
    char *err = 0;
    transstr filename;
    int maj, min, lev;
#if HAVE_SQLITE3_OPEN_V2
    transstr vfsname;

    vfsname.result = 0;
    vfsname.tofree = 0;
    vfsname.jstr = 0;
#endif

    if (h) {
    if (h->sqlite) {
#if HAVE_BOTH_SQLITE
        if (h->is3) {
        sqlite3_close((sqlite3 *) h->sqlite);
        } else {
        sqlite_close((sqlite *) h->sqlite);
        }
        h->is3 = 0;
#else
#if HAVE_SQLITE2
        sqlite_close((sqlite *) h->sqlite);
#endif
#if HAVE_SQLITE3
        sqlite3_close((sqlite3 *) h->sqlite);
#endif
#endif
        h->sqlite = 0;
    }
    } else {
    h = malloc(sizeof (handle));
    if (!h) {
        throwoom(env, "unable to get SQLite handle");
        return;
    }
    h->sqlite = 0;
    h->bh = h->cb = h->ai = h->tr = h->ph = 0;
    // CHECK THIS 
#if HAVE_BOTH_SQLITE
    h->is3 = 0;
    h->stmt = 0;
    h->haveutf = 1;
#else
#if HAVE_SQLITE2
    h->haveutf = strcmp(sqlite_libencoding(), "UTF-8") == 0;
#endif
#if HAVE_SQLITE3
    h->stmt = 0;
    h->haveutf = 1;
#endif
#endif
    h->enc = 0;
    h->funcs = 0;
    h->ver = 0;
#if HAVE_SQLITE_COMPILE
    h->vms = 0;
#endif
#if HAVE_SQLITE3 && HAVE_SQLITE3_INCRBLOBIO
    h->blobs = 0;
#endif
    }
    h->env = 0;
    if (!file) {
    throwex(env, err ? err : "invalid file name");
    return;
    }
    trans2iso(env, h->haveutf, h->enc, file, &filename);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
    (*env)->DeleteLocalRef(env, exc);
    return;
    }
#if HAVE_SQLITE3_OPEN_V2
    if (vfs) {
    trans2iso(env, 1, h->enc, vfs, &vfsname);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        transfree(&filename);
        (*env)->DeleteLocalRef(env, exc);
        return;
    }
    }
#endif
#if HAVE_BOTH_SQLITE
    {
    FILE *f = fopen(filename.result, "rb");
    int c_0 = EOF;

    if (f) {
        c_0 = fgetc(f);
        fclose(f);
    }
    if (c_0 != '*' && ver2 == JNI_FALSE) {
#if HAVE_SQLITE3_OPEN_V2
        int rc = sqlite3_open_v2(filename.result, (sqlite3 **) &h->sqlite,
                     (int) mode, vfsname.result);
#else
        int rc = sqlite3_open(filename.result, (sqlite3 **) &h->sqlite);
#endif

        if (rc == SQLITE_OK) {
        h->is3 = 1;
        } else if (h->sqlite) {
        sqlite3_close((sqlite3 *) h->sqlite);
        h->sqlite = 0;
        }
    } else {
        h->sqlite = (void *) sqlite_open(filename.result,
                         (int) mode, &err);
    }
    }
#else
#if HAVE_SQLITE2
    h->sqlite = (void *) sqlite_open(filename.result, (int) mode, &err);
#endif
#if HAVE_SQLITE3
#if HAVE_SQLITE3_OPEN_V2
    if (sqlite3_open_v2(filename.result, (sqlite3 **) &h->sqlite,
            (int) mode, vfsname.result) != SQLITE_OK)
#else
    if (sqlite3_open(filename.result, (sqlite3 **) &h->sqlite) != SQLITE_OK)
#endif
    {
    if (h->sqlite) {
        sqlite3_close((sqlite3 *) h->sqlite);
        h->sqlite = 0;
    }
    }
#endif
#endif
    transfree(&filename);
#if HAVE_SQLITE3_OPEN_V2
    transfree(&vfsname);
#endif
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
    (*env)->DeleteLocalRef(env, exc);
#if HAVE_SQLITE2
    if (err) {
        sqlite_freemem(err);
    }
#endif
    if (h->sqlite) {
#if HAVE_BOTH_SQLITE
        if (h->is3) {
        sqlite3_close((sqlite3 *) h->sqlite);
        h->is3 = 0;
        } else {
        sqlite_close((sqlite *) h->sqlite);
        }
#else
#if HAVE_SQLITE2
        sqlite_close((sqlite *) h->sqlite);
#endif
#if HAVE_SQLITE3
        sqlite3_close((sqlite3 *) h->sqlite);
#endif
#endif
    }
    h->sqlite = 0;
    return;
    }
    if (h->sqlite) {
    jvalue v;

    v.j = 0;
    v.l = (jobject) h;
    (*env)->SetLongField(env, obj, F_SQLite_Database_handle, v.j);
#if HAVE_SQLITE2
    if (err) {
        sqlite_freemem(err);
    }
#endif
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        sscanf(sqlite3_libversion(), "%d.%d.%d", &maj, &min, &lev);
#if HAVE_SQLITE3_LOAD_EXTENSION
        sqlite3_enable_load_extension((sqlite3 *) h->sqlite, 1);
#endif
    } else {
        sscanf(sqlite_libversion(), "%d.%d.%d", &maj, &min, &lev);
    }
#else
#if HAVE_SQLITE2
    sscanf(sqlite_libversion(), "%d.%d.%d", &maj, &min, &lev);
#endif
#if HAVE_SQLITE3
    sscanf(sqlite3_libversion(), "%d.%d.%d", &maj, &min, &lev);
#if HAVE_SQLITE3_LOAD_EXTENSION
    sqlite3_enable_load_extension((sqlite3 *) h->sqlite, 1);
#endif
#endif
#endif
    h->ver = ((maj & 0xFF) << 16) | ((min & 0xFF) << 8) | (lev & 0xFF);
    return;
    }
    throwex(env, err ? err : "unknown error in open");
#if HAVE_SQLITE2
    if (err) {
    sqlite_freemem(err);
    }
#endif*/
    return NOERROR;
}

ECode Database::_open_aux_file(String filename)
{/*
    handle *h = gethandle(env, obj);
#if HAVE_SQLITE_OPEN_AUX_FILE
    jthrowable exc;
    char *err = 0;
    transstr filename;
    int ret;
#endif

    if (h && h->sqlite) {
#if HAVE_SQLITE_OPEN_AUX_FILE
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        throwex(env, "unsupported");
    }
#endif
    trans2iso(env, h->haveutf, h->enc, file, &filename);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
    }
    ret = sqlite_open_aux_file((sqlite *) h->sqlite,
                   filename.result, &err);
    transfree(&filename);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        if (err) {
        sqlite_freemem(err);
        }
        return;
    }
    if (ret != SQLITE_OK) {
        throwex(env, err ? err : sqlite_error_string(ret));
    }
    if (err) {
        sqlite_freemem(err);
    }
#else
    throwex(env, "unsupported");
#endif
    return;
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_finalize()
{
    //doclose(env, obj, 1);
    return E_NOT_IMPLEMENTED;
}

ECode Database::_close()
{
    //doclose(env, obj, 0);
    return E_NOT_IMPLEMENTED;
}

ECode Database::_exec(
    /** [in] **/String sql, 
    /** [in] **/ICallback* cb)
{/*
    handle *h = gethandle(env, obj);
    freemem *freeproc;

    if (!sql) {
    throwex(env, "invalid SQL statement");
    return;
    }
    if (h) {
    if (h->sqlite) {
        jthrowable exc;
        int rc;
        char *err = 0;
        transstr sqlstr;
        jobject oldcb = globrefpop(env, &h->cb);

        globrefset(env, cb, &h->cb);
        h->env = env;
        h->row1 = 1;
        trans2iso(env, h->haveutf, h->enc, sql, &sqlstr);
        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
        }
#if HAVE_BOTH_SQLITE
        if (h->is3) {
        rc = sqlite3_exec((sqlite3 *) h->sqlite, sqlstr.result,
                  callback, h, &err);
        freeproc = (freemem *) sqlite3_free;
        } else {
        rc = sqlite_exec((sqlite *) h->sqlite, sqlstr.result,
                 callback, h, &err);
        freeproc = (freemem *) sqlite_freemem;
        }
#else
#if HAVE_SQLITE2
        rc = sqlite_exec((sqlite *) h->sqlite, sqlstr.result,
                 callback, h, &err);
        freeproc = (freemem *) sqlite_freemem;
#endif
#if HAVE_SQLITE3
        rc = sqlite3_exec((sqlite3 *) h->sqlite, sqlstr.result,
                  callback, h, &err);
        freeproc = (freemem *) sqlite3_free;
#endif
#endif
        transfree(&sqlstr);
        exc = (*env)->ExceptionOccurred(env);
        delglobrefp(env, &h->cb);
        h->cb = oldcb;
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        if (err) {
            freeproc(err);
        }
        return;
        }
        if (rc != SQLITE_OK) {
        char msg[128];

        seterr(env, obj, rc);
        if (!err) {
            sprintf(msg, "error %d in sqlite*_exec", rc);
        }
        throwex(env, err ? err : msg);
        }
        if (err) {
        freeproc(err);
        }
        return;
    }
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_execEx(
    /** [in] **/String sql, 
    /** [in] **/ICallback* cb, 
    /** [in] **/ArrayOf<String>* args)
{/*
    handle *h = gethandle(env, obj);
    freemem *freeproc = 0;

    if (!sql) {
    throwex(env, "invalid SQL statement");
    return;
    }
    if (h) {
    if (h->sqlite) {
        jthrowable exc;
        int rc = SQLITE_ERROR, nargs, i;
        char *err = 0, *p;
        const char *str = (*env)->GetStringUTFChars(env, sql, 0);
        transstr sqlstr;
        struct args {
        char *arg;
        jobject obj;
        transstr trans;
        } *argv = 0;
        char **cargv = 0;
        jobject oldcb = globrefpop(env, &h->cb);

        globrefset(env, cb, &h->cb);
        p = (char *) str;
        nargs = 0;
        while (*p) {
        if (*p == '%') {
            ++p;
            if (*p == 'q' || *p == 's') {
            nargs++;
            if (nargs > MAX_PARAMS) {
                (*env)->ReleaseStringUTFChars(env, sql, str);
                delglobrefp(env, &h->cb);
                h->cb = oldcb;
                throwex(env, "too much SQL parameters");
                return;
            }
            } else if (h->ver >= 0x020500 && *p == 'Q') {
            nargs++;
            if (nargs > MAX_PARAMS) {
                (*env)->ReleaseStringUTFChars(env, sql, str);
                delglobrefp(env, &h->cb);
                h->cb = oldcb;
                throwex(env, "too much SQL parameters");
                return;
            }
            } else if (*p != '%') {
            (*env)->ReleaseStringUTFChars(env, sql, str);
            delglobrefp(env, &h->cb);
            h->cb = oldcb;
            throwex(env, "bad % specification in query");
            return;
            }
        }
        ++p;
        }
        cargv = malloc((sizeof (*argv) + sizeof (char *))
               * MAX_PARAMS);
        if (!cargv) {
        (*env)->ReleaseStringUTFChars(env, sql, str);
        delglobrefp(env, &h->cb);
        h->cb = oldcb;
        throwoom(env, "unable to allocate arg vector");
        return;
        }
        argv = (struct args *) (cargv + MAX_PARAMS);
        for (i = 0; i < MAX_PARAMS; i++) {
        cargv[i] = 0;
        argv[i].arg = 0;
        argv[i].obj = 0;
        argv[i].trans.result = argv[i].trans.tofree = 0;
        }
        exc = 0;
        for (i = 0; i < nargs; i++) {
        jobject so = (*env)->GetObjectArrayElement(env, args, i);

        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
            (*env)->DeleteLocalRef(env, exc);
            break;
        }
        if (so) {
            argv[i].obj = so;
            argv[i].arg = cargv[i] =
            trans2iso(env, h->haveutf, h->enc, argv[i].obj,
                  &argv[i].trans);
        }
        }
        if (exc) {
        for (i = 0; i < nargs; i++) {
            if (argv[i].obj) {
            transfree(&argv[i].trans);
            }
        }
        freep((char **) &cargv);
        (*env)->ReleaseStringUTFChars(env, sql, str);
        delglobrefp(env, &h->cb);
        h->cb = oldcb;
        return;
        }
        h->env = env;
        h->row1 = 1;
        trans2iso(env, h->haveutf, h->enc, sql, &sqlstr);
        exc = (*env)->ExceptionOccurred(env);
        if (!exc) {
#if HAVE_BOTH_SQLITE
        if (h->is3) {
#if defined(_WIN32) || !defined(CANT_PASS_VALIST_AS_CHARPTR)
            char *s = sqlite3_vmprintf(sqlstr.result, (char *) cargv);
#else
            char *s = sqlite3_mprintf(sqlstr.result,
                          cargv[0], cargv[1],
                          cargv[2], cargv[3],
                          cargv[4], cargv[5],
                          cargv[6], cargv[7],
                          cargv[8], cargv[9],
                          cargv[10], cargv[11],
                          cargv[12], cargv[13],
                          cargv[14], cargv[15],
                          cargv[16], cargv[17],
                          cargv[18], cargv[19],
                          cargv[20], cargv[21],
                          cargv[22], cargv[23],
                          cargv[24], cargv[25],
                          cargv[26], cargv[27],
                          cargv[28], cargv[29],
                          cargv[30], cargv[31]);
#endif

            if (s) {
            rc = sqlite3_exec((sqlite3 *) h->sqlite, s, callback,
                      h, &err);
            sqlite3_free(s);
            } else {
            rc = SQLITE_NOMEM;
            }
            freeproc = (freemem *) sqlite3_free;
        } else {
#if defined(_WIN32) || !defined(CANT_PASS_VALIST_AS_CHARPTR)
            rc = sqlite_exec_vprintf((sqlite *) h->sqlite,
                         sqlstr.result, callback, h, &err,
                         (char *) cargv);
#else
            rc = sqlite_exec_printf((sqlite *) h->sqlite,
                        sqlstr.result, callback,
                        h, &err,
                        cargv[0], cargv[1],
                        cargv[2], cargv[3],
                        cargv[4], cargv[5],
                        cargv[6], cargv[7],
                        cargv[8], cargv[9],
                        cargv[10], cargv[11],
                        cargv[12], cargv[13],
                        cargv[14], cargv[15],
                        cargv[16], cargv[17],
                        cargv[18], cargv[19],
                        cargv[20], cargv[21],
                        cargv[22], cargv[23],
                        cargv[24], cargv[25],
                        cargv[26], cargv[27],
                        cargv[28], cargv[29],
                        cargv[30], cargv[31]);
#endif
            freeproc = (freemem *) sqlite_freemem;
        }
#else
#if HAVE_SQLITE2
#if defined(_WIN32) || !defined(CANT_PASS_VALIST_AS_CHARPTR)
        rc = sqlite_exec_vprintf((sqlite *) h->sqlite, sqlstr.result,
                     callback, h, &err, (char *) cargv);
#else
        rc = sqlite_exec_printf((sqlite *) h->sqlite, sqlstr.result,
                    callback, h, &err,
                    cargv[0], cargv[1],
                    cargv[2], cargv[3],
                    cargv[4], cargv[5],
                    cargv[6], cargv[7],
                    cargv[8], cargv[9],
                    cargv[10], cargv[11],
                    cargv[12], cargv[13],
                    cargv[14], cargv[15],
                    cargv[16], cargv[17],
                    cargv[18], cargv[19],
                    cargv[20], cargv[21],
                    cargv[22], cargv[23],
                    cargv[24], cargv[25],
                    cargv[26], cargv[27],
                    cargv[28], cargv[29],
                    cargv[30], cargv[31]);
#endif
        freeproc = (freemem *) sqlite_freemem;
#endif
#if HAVE_SQLITE3
#if defined(_WIN32) || !defined(CANT_PASS_VALIST_AS_CHARPTR)
        char *s = sqlite3_vmprintf(sqlstr.result, (char *) cargv);
#else
        char *s = sqlite3_mprintf(sqlstr.result,
                      cargv[0], cargv[1],
                      cargv[2], cargv[3],
                      cargv[4], cargv[5],
                      cargv[6], cargv[7],
                      cargv[8], cargv[9],
                      cargv[10], cargv[11],
                      cargv[12], cargv[13],
                      cargv[14], cargv[15],
                      cargv[16], cargv[17],
                      cargv[18], cargv[19],
                      cargv[20], cargv[21],
                      cargv[22], cargv[23],
                      cargv[24], cargv[25],
                      cargv[26], cargv[27],
                      cargv[28], cargv[29],
                      cargv[30], cargv[31]);
#endif

        if (s) {
            rc = sqlite3_exec((sqlite3 *) h->sqlite, s, callback,
                      h, &err);
            sqlite3_free(s);
        } else {
            rc = SQLITE_NOMEM;
        }
        freeproc = (freemem *) sqlite3_free;
#endif
#endif
        exc = (*env)->ExceptionOccurred(env);
        }
        for (i = 0; i < nargs; i++) {
        if (argv[i].obj) {
            transfree(&argv[i].trans);
        }
        }
        transfree(&sqlstr);
        (*env)->ReleaseStringUTFChars(env, sql, str);
        freep((char **) &cargv);
        delglobrefp(env, &h->cb);
        h->cb = oldcb;
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        if (err && freeproc) {
            freeproc(err);
        }
        return;
        }
        if (rc != SQLITE_OK) {
        char msg[128];

        seterr(env, obj, rc);
        if (!err) {
            sprintf(msg, "error %d in sqlite*_exec", rc);
        }
        throwex(env, err ? err : msg);
        }
        if (err && freeproc) {
        freeproc(err);
        }
        return;
    }
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

Int64 Database::_last_insert_rowid()
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        return (jlong) sqlite3_last_insert_rowid((sqlite3 *) h->sqlite);
    } else {
        return (jlong) sqlite_last_insert_rowid((sqlite *) h->sqlite);
    }
#else
#if HAVE_SQLITE2
    return (jlong) sqlite_last_insert_rowid((sqlite *) h->sqlite);
#endif
#if HAVE_SQLITE3
    return (jlong) sqlite3_last_insert_rowid((sqlite3 *) h->sqlite);
#endif
#endif
    }
    throwclosed(env);
    return (jlong) 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_interrupt()
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        sqlite3_interrupt((sqlite3 *) h->sqlite);
    } else {
        sqlite_interrupt((sqlite *) h->sqlite);
    }
#else
#if HAVE_SQLITE2
    sqlite_interrupt((sqlite *) h->sqlite);
#endif
#if HAVE_SQLITE3
    sqlite3_interrupt((sqlite3 *) h->sqlite);
#endif
#endif
    return;
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

Int64 Database::_changes()
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        return (jlong) sqlite3_changes((sqlite3 *) h->sqlite);
    } else {
        return (jlong) sqlite_changes((sqlite *) h->sqlite);
    }
#else
#if HAVE_SQLITE2
    return (jlong) sqlite_changes((sqlite *) h->sqlite);
#endif
#if HAVE_SQLITE3
    return (jlong) sqlite3_changes((sqlite3 *) h->sqlite);
#endif
#endif
    }
    throwclosed(env);
    return (jlong) 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_busy_handler(
    /** [in] **/IBusyHandler* bh)
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
    delglobrefp(env, &h->bh);
    globrefset(env, bh, &h->bh);
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        sqlite3_busy_handler((sqlite3 *) h->sqlite, busyhandler3, h);
    } else {
        sqlite_busy_handler((sqlite *) h->sqlite, busyhandler, h);
    }
#else
#if HAVE_SQLITE2
    sqlite_busy_handler((sqlite *) h->sqlite, busyhandler, h);
#endif
#if HAVE_SQLITE3
    sqlite3_busy_handler((sqlite3 *) h->sqlite, busyhandler3, h);
#endif
#endif
    return;
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_busy_timeout(
    /** [in] **/Int32 ms)
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        sqlite3_busy_timeout((sqlite3 * ) h->sqlite, ms);
    } else {
        sqlite_busy_timeout((sqlite *) h->sqlite, ms);
    }
#else
#if HAVE_SQLITE2
    sqlite_busy_timeout((sqlite *) h->sqlite, ms);
#endif
#if HAVE_SQLITE3
    sqlite3_busy_timeout((sqlite3 * ) h->sqlite, ms);
#endif
#endif
    return;
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

Boolean Database::_complete(
    /** [in] **/String sql)
{/*
    transstr sqlstr;
    jboolean result;

    if (!sql) {
    return JNI_FALSE;
    }
#if HAVE_BOTH_SQLITE || HAVE_SQLITE3
    // CHECK THIS 
    trans2iso(env, 1, 0, sql, &sqlstr);
    result = sqlite3_complete(sqlstr.result) ? JNI_TRUE : JNI_FALSE;
#else
    trans2iso(env, strcmp(sqlite_libencoding(), "UTF-8") == 0, 0,
          sql, &sqlstr);
    result = sqlite_complete(sqlstr.result) ? JNI_TRUE : JNI_FALSE;
#endif
    transfree(&sqlstr);
    return result;*/
    return FALSE;
}

ECode Database::_create_function(
    /** [in] **/String name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    //mkfunc_common(env, 0, obj, name, nargs, fi);
    return E_NOT_IMPLEMENTED;
}
    
ECode Database::_create_aggregate(
    /** [in] **/String name, 
    /** [in] **/Int32 nargs, 
    /** [in] **/IFunction* f)
{
    //mkfunc_common(env, 1, obj, name, nargs, fi);
    return E_NOT_IMPLEMENTED;
}

ECode Database::_function_type(
    /** [in] **/String name, 
    /** [in] **/Int32 type)
{/*
handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        return;
    }
#endif
#if HAVE_SQLITE2
#if HAVE_SQLITE_FUNCTION_TYPE
    {
        int ret;
        transstr namestr;
        jthrowable exc;

        trans2iso(env, h->haveutf, h->enc, name, &namestr);
        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
        }
        ret = sqlite_function_type(h->sqlite, namestr.result, (int) type);
        transfree(&namestr);
        if (ret != SQLITE_OK) {
        throwex(env, sqlite_error_string(ret));
        }
    }
#endif
#endif
    return;
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

String Database::_errmsg()
{/*
#if HAVE_SQLITE3
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (!h->is3) {
        return 0;
    }
#endif
    return (*env)->NewStringUTF(env,
                    sqlite3_errmsg((sqlite3 *) h->sqlite));
    }
#endif
    return 0;*/
    return String("aaa");
}
    

ECode Database::_set_encoding(
    /** [in] **/String enc)
{/*
    handle *h = gethandle(env, obj);

    if (h && !h->haveutf) {
#if HAVE_BOTH_SQLITE
    if (!h->is3) {
        delglobrefp(env, &h->enc);
        h->enc = enc;
        globrefset(env, enc, &h->enc);
    }
#else
#if HAVE_SQLITE2
    delglobrefp(env, &h->enc);
    h->enc = enc;
    globrefset(env, enc, &h->enc);
#endif
#endif
    }*/
    return E_NOT_IMPLEMENTED;
}


ECode Database::_set_authorizer(
    /** [in] **/IAuthorizer* auth)
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
    delglobrefp(env, &h->ai);
    globrefset(env, auth, &h->ai);
#if HAVE_SQLITE_SET_AUTHORIZER
    h->env = env;
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        sqlite3_set_authorizer((sqlite3 *) h->sqlite,
                   h->ai ? doauth : 0, h);
    } else {
        sqlite_set_authorizer((sqlite *) h->sqlite,
                  h->ai ? doauth : 0, h);
    }
#else
#if HAVE_SQLITE2
    sqlite_set_authorizer((sqlite *) h->sqlite, h->ai ? doauth : 0, h);
#endif
#if HAVE_SQLITE3
    sqlite3_set_authorizer((sqlite3 *) h->sqlite, h->ai ? doauth : 0, h);
#endif
#endif
#endif
    return;
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_trace(
    /** [in] **/ITrace* tr)
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
    delglobrefp(env, &h->tr);
    globrefset(env, tr, &h->tr);
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        sqlite3_trace((sqlite3 *) h->sqlite, h->tr ? dotrace : 0, h);
    } else {
#if HAVE_SQLITE_TRACE
        sqlite_trace((sqlite *) h->sqlite, h->tr ? dotrace : 0, h);
#endif
    }
#else
#if HAVE_SQLITE2
#if HAVE_SQLITE_TRACE
    sqlite_trace((sqlite *) h->sqlite, h->tr ? dotrace : 0, h);
#endif
#endif
#if HAVE_SQLITE3
    sqlite3_trace((sqlite3 *) h->sqlite, h->tr ? dotrace : 0, h);
#endif
#endif
    return;
    }
    throwclosed(env);*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::Vm_compile(
    /** [in] **/String sql, 
    /** [in] **/IVm* vm)
{/*
#if HAVE_SQLITE_COMPILE
    handle *h = gethandle(env, obj);
    void *svm = 0;
    hvm *v;
    char *err = 0;
#if HAVE_SQLITE2
    char *errfr = 0;
#endif
    const char *tail;
    transstr tr;
    jvalue vv;
    int ret;
    jthrowable exc;

    if (!h) {
    throwclosed(env);
    return;
    }
    if (!vm) {
    throwex(env, "null vm");
    return;
    }
    if (!sql) {
    throwex(env, "null sql");
    return;
    }
    trans2iso(env, h->haveutf, h->enc, sql, &tr);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
    (*env)->DeleteLocalRef(env, exc);
    return;
    }
    h->env = env;
#if HAVE_BOTH_SQLITE
    if (h->is3) {
#if HAVE_SQLITE3_PREPARE_V2
    ret = sqlite3_prepare_v2((sqlite3 *) h->sqlite, tr.result, -1,
                 (sqlite3_stmt **) &svm, &tail);
#else
    ret = sqlite3_prepare((sqlite3 *) h->sqlite, tr.result, -1,
                  (sqlite3_stmt **) &svm, &tail);
#endif
    if (ret != SQLITE_OK) {
        if (svm) {
        sqlite3_finalize((sqlite3_stmt *) svm);
        svm = 0;
        }
        err = (char *) sqlite3_errmsg((sqlite3 *) h->sqlite);
    }
    } else {
    ret = sqlite_compile((sqlite *) h->sqlite, tr.result, &tail,
                 (sqlite_vm **) &svm, &errfr);
    if (ret != SQLITE_OK) {
        err = errfr;
        if (svm) {
        sqlite_finalize((sqlite_vm *) svm, 0);
        }
    }
    }
#else
#if HAVE_SQLITE2
    ret = sqlite_compile((sqlite *) h->sqlite, tr.result, &tail,
             (sqlite_vm **) &svm, &errfr);
    if (ret != SQLITE_OK) {
    err = errfr;
    if (svm) {
        sqlite_finalize((sqlite_vm *) svm, 0);
        svm = 0;
    }
    }
#endif
#if HAVE_SQLITE3
#if HAVE_SQLITE3_PREPARE_V2
    ret = sqlite3_prepare_v2((sqlite3 *) h->sqlite, tr.result, -1,
                 (sqlite3_stmt **) &svm, &tail);
#else
    ret = sqlite3_prepare((sqlite3 *) h->sqlite, tr.result, -1,
              (sqlite3_stmt **) &svm, &tail);
#endif
    if (ret != SQLITE_OK) {
    if (svm) {
        sqlite3_finalize((sqlite3_stmt *) svm);
        svm = 0;
    }
    err = (char *) sqlite3_errmsg((sqlite3 *) h->sqlite);
    }
#endif
#endif
    if (ret != SQLITE_OK) {
    transfree(&tr);
    setvmerr(env, vm, ret);
    throwex(env, err ? err : "error in prepare/compile");
#if HAVE_SQLITE2
    if (errfr) {
        sqlite_freemem(errfr);
    }
#endif
    return;
    }
#if HAVE_SQLITE2
    if (errfr) {
    sqlite_freemem(errfr);
    }
#endif
    if (!svm) {
    transfree(&tr);
    return;
    }
    v = malloc(sizeof (hvm) + strlen(tail) + 1);
    if (!v) {
    transfree(&tr);
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        sqlite3_finalize((sqlite3_stmt *) svm);
    } else {
        sqlite_finalize((sqlite_vm *) svm, 0);
    }
#else
#if HAVE_SQLITE2
    sqlite_finalize((sqlite_vm *) svm, 0);
#endif
#if HAVE_SQLITE3
    sqlite3_finalize((sqlite3_stmt *) svm);
#endif
#endif
    throwoom(env, "unable to get SQLite handle");
    return;
    }
    v->next = h->vms;
    h->vms = v;
    v->vm = svm;
    v->h = h;
    v->tail = (char *) (v + 1);
#if HAVE_BOTH_SQLITE
    v->is3 = v->hh.is3 = h->is3;
#endif
    strcpy(v->tail, tail);
    v->hh.sqlite = 0;
    v->hh.haveutf = h->haveutf;
    v->hh.ver = h->ver;
    v->hh.bh = v->hh.cb = v->hh.ai = v->hh.tr = v->hh.ph = 0;
    v->hh.row1 = 1;
    v->hh.enc = h->enc;
    v->hh.funcs = 0;
    v->hh.vms = 0;
    v->hh.env = 0;
    vv.j = 0;
    vv.l = (jobject) v;
    (*env)->SetLongField(env, vm, F_SQLite_Vm_handle, vv.j);
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::Vm_compile_args(
    /** [in] **/String sql, 
    /** [in] **/IVm* vm, 
    /** [in] **/ArrayOf<String>* args)
{/*
#if HAVE_SQLITE_COMPILE
#if HAVE_SQLITE3
    handle *h = gethandle(env, obj);
#endif

#if HAVE_BOTH_SQLITE
    if (h && !h->is3) {
    throwex(env, "unsupported");
    return;
    }
#else
#if HAVE_SQLITE2
    throwex(env, "unsupported");
#endif
#endif
#if HAVE_SQLITE3 
    if (!h || !h->sqlite) {
    throwclosed(env);
    return;
    }
    if (!vm) {
    throwex(env, "null vm");
    return;
    }
    if (!sql) {
    throwex(env, "null sql");
    return;
    } else {
    void *svm = 0;
    hvm *v;
    jvalue vv;
    jthrowable exc;
    int rc = SQLITE_ERROR, nargs, i;
    char *p;
    const char *str = (*env)->GetStringUTFChars(env, sql, 0);
    const char *tail;
    transstr sqlstr;
    struct args {
        char *arg;
        jobject obj;
        transstr trans;
    } *argv = 0;
    char **cargv = 0;

    p = (char *) str;
    nargs = 0;
    while (*p) {
        if (*p == '%') {
        ++p;
        if (*p == 'q' || *p == 'Q' || *p == 's') {
            nargs++;
            if (nargs > MAX_PARAMS) {
            (*env)->ReleaseStringUTFChars(env, sql, str);
            throwex(env, "too much SQL parameters");
            return;
            }
        } else if (*p != '%') {
            (*env)->ReleaseStringUTFChars(env, sql, str);
            throwex(env, "bad % specification in query");
            return;
        }
        }
        ++p;
    }
    cargv = malloc((sizeof (*argv) + sizeof (char *)) * MAX_PARAMS);
    if (!cargv) {
        (*env)->ReleaseStringUTFChars(env, sql, str);
        throwoom(env, "unable to allocate arg vector");
        return;
    }
    argv = (struct args *) (cargv + MAX_PARAMS);
    for (i = 0; i < MAX_PARAMS; i++) {
        cargv[i] = 0;
        argv[i].arg = 0;
        argv[i].obj = 0;
        argv[i].trans.result = argv[i].trans.tofree = 0;
    }
    exc = 0;
    for (i = 0; i < nargs; i++) {
        jobject so = (*env)->GetObjectArrayElement(env, args, i);

        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        break;
        }
        if (so) {
        argv[i].obj = so;
        argv[i].arg = cargv[i] =
            trans2iso(env, 1, 0, argv[i].obj, &argv[i].trans);
        }
    }
    if (exc) {
        for (i = 0; i < nargs; i++) {
        if (argv[i].obj) {
            transfree(&argv[i].trans);
        }
        }
        freep((char **) &cargv);
        (*env)->ReleaseStringUTFChars(env, sql, str);
        return;
    }
    h->row1 = 1;
    trans2iso(env, 1, 0, sql, &sqlstr);
    exc = (*env)->ExceptionOccurred(env);
    if (!exc) {
#if defined(_WIN32) || !defined(CANT_PASS_VALIST_AS_CHARPTR)
        char *s = sqlite3_vmprintf(sqlstr.result, (char *) cargv);
#else
        char *s = sqlite3_mprintf(sqlstr.result,
                      cargv[0], cargv[1],
                      cargv[2], cargv[3],
                      cargv[4], cargv[5],
                      cargv[6], cargv[7],
                      cargv[8], cargv[9],
                      cargv[10], cargv[11],
                      cargv[12], cargv[13],
                      cargv[14], cargv[15],
                      cargv[16], cargv[17],
                      cargv[18], cargv[19],
                      cargv[20], cargv[21],
                      cargv[22], cargv[23],
                      cargv[24], cargv[25],
                      cargv[26], cargv[27],
                      cargv[28], cargv[29],
                      cargv[30], cargv[31]);
#endif
        if (!s) {
        rc = SQLITE_NOMEM;
        } else {
#if HAVE_SQLITE3_PREPARE_V2
        rc = sqlite3_prepare_v2((sqlite3 *) h->sqlite, s, -1,
                    (sqlite3_stmt **) &svm, &tail);
#else
        rc = sqlite3_prepare((sqlite3 *) h->sqlite, s, -1,
                      (sqlite3_stmt **) &svm, &tail);
#endif
        if (rc != SQLITE_OK) {
            if (svm) {
            sqlite3_finalize((sqlite3_stmt *) svm);
            svm = 0;
            }
        }
        }
        if (rc != SQLITE_OK) {
        sqlite3_free(s);
        for (i = 0; i < nargs; i++) {
            if (argv[i].obj) {
            transfree(&argv[i].trans);
            }
        }
        freep((char **) &cargv);
        transfree(&sqlstr);
        (*env)->ReleaseStringUTFChars(env, sql, str);
        setvmerr(env, vm, rc);
        throwex(env, "error in prepare");
        return;
        }
        v = malloc(sizeof (hvm) + strlen(tail) + 1);
        if (!v) {
        sqlite3_free(s);
        for (i = 0; i < nargs; i++) {
            if (argv[i].obj) {
            transfree(&argv[i].trans);
            }
        }
        freep((char **) &cargv);
        transfree(&sqlstr);
        (*env)->ReleaseStringUTFChars(env, sql, str);
        sqlite3_finalize((sqlite3_stmt *) svm);
        setvmerr(env, vm, SQLITE_NOMEM);
        throwoom(env, "unable to get SQLite handle");
        return;
        }
        v->next = h->vms;
        h->vms = v;
        v->vm = svm;
        v->h = h;
        v->tail = (char *) (v + 1);
#if HAVE_BOTH_SQLITE
        v->is3 = v->hh.is3 = h->is3;
#endif
        strcpy(v->tail, tail);
        sqlite3_free(s);
        v->hh.sqlite = 0;
        v->hh.haveutf = h->haveutf;
        v->hh.ver = h->ver;
        v->hh.bh = v->hh.cb = v->hh.ai = v->hh.tr = v->hh.ph = 0;
        v->hh.row1 = 1;
        v->hh.enc = h->enc;
        v->hh.funcs = 0;
        v->hh.vms = 0;
        v->hh.env = 0;
        vv.j = 0;
        vv.l = (jobject) v;
        (*env)->SetLongField(env, vm, F_SQLite_Vm_handle, vv.j);
    }
    for (i = 0; i < nargs; i++) {
        if (argv[i].obj) {
        transfree(&argv[i].trans);
        }
    }
    freep((char **) &cargv);
    transfree(&sqlstr);
    (*env)->ReleaseStringUTFChars(env, sql, str);
    if (exc) {
        (*env)->DeleteLocalRef(env, exc);
    }
    }
#endif
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::Stmt_prepare(
    /** [in] **/String sql, 
    /** [in] **/IStmt* stmt)
{/*
#if HAVE_SQLITE3
    handle *h = gethandle(env, obj);
    void *svm = 0;
    hvm *v;
    jvalue vv;
    jsize len16;
    const jchar *sql16, *tail = 0;
    int ret;

    if (!h) {
    throwclosed(env);
    return;
    }
    if (!stmt) {
    throwex(env, "null stmt");
    return;
    }
    if (!sql) {
    throwex(env, "null sql");
    return;
    }
#ifdef HAVE_BOTH_SQLITE
    if (!h->is3) {
    throwex(env, "only on SQLite3 database");
    return;
    }
#endif
    len16 = (*env)->GetStringLength(env, sql) * sizeof (jchar);
    if (len16 < 1) {
    return;
    }
    h->env = env;
    sql16 = (*env)->GetStringChars(env, sql, 0);
#if HAVE_SQLITE3_PREPARE16_V2
    ret = sqlite3_prepare16_v2((sqlite3 *) h->sqlite, sql16, len16,
                   (sqlite3_stmt **) &svm, (const void **) &tail);
#else
    ret = sqlite3_prepare16((sqlite3 *) h->sqlite, sql16, len16,
                (sqlite3_stmt **) &svm, (const void **) &tail);
#endif
    if (ret != SQLITE_OK) {
    if (svm) {
        sqlite3_finalize((sqlite3_stmt *) svm);
        svm = 0;
    }
    }
    if (ret != SQLITE_OK) {
    const char *err = sqlite3_errmsg(h->sqlite);

    (*env)->ReleaseStringChars(env, sql, sql16);
    setstmterr(env, stmt, ret);
    throwex(env, err ? err : "error in prepare");
    return;
    }
    if (!svm) {
    (*env)->ReleaseStringChars(env, sql, sql16);
    return;
    }
    len16 = len16 + sizeof (jchar) - ((char *) tail - (char *) sql16);
    if (len16 < (jsize) sizeof (jchar)) {
    len16 = sizeof (jchar);
    }
    v = malloc(sizeof (hvm) + len16);
    if (!v) {
    (*env)->ReleaseStringChars(env, sql, sql16);
    sqlite3_finalize((sqlite3_stmt *) svm);
    throwoom(env, "unable to get SQLite handle");
    return;
    }
    v->next = h->vms;
    h->vms = v;
    v->vm = svm;
    v->h = h;
    v->tail = (char *) (v + 1);
#if HAVE_BOTH_SQLITE
    v->is3 = v->hh.is3 = 1;
#endif
    memcpy(v->tail, tail, len16);
    len16 /= sizeof (jchar);
    ((jchar *) v->tail)[len16 - 1] = 0;
    (*env)->ReleaseStringChars(env, sql, sql16);
    v->hh.sqlite = 0;
    v->hh.haveutf = h->haveutf;
    v->hh.ver = h->ver;
    v->hh.bh = v->hh.cb = v->hh.ai = v->hh.tr = v->hh.ph = 0;
    v->hh.row1 = 1;
    v->hh.enc = h->enc;
    v->hh.funcs = 0;
    v->hh.vms = 0;
    v->hh.env = 0;
    vv.j = 0;
    vv.l = (jobject) v;
    (*env)->SetLongField(env, stmt, F_SQLite_Stmt_handle, vv.j);
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_open_blob(
    /** [in] **/String db, 
    /** [in] **/String table, 
    /** [in] **/String column,
    /** [in] **/Int64 row, 
    /** [in] **/Boolean rw, 
    /** [in] **/IBlob2* blob)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_INCRBLOBIO
    handle *h = gethandle(env, obj);
    hbl *bl;
    jthrowable exc;
    transstr dbn, tbl, col;
    sqlite3_blob *blob;
    jvalue vv;
    int ret;

    if (!blobj) {
    throwex(env, "null blob");
    return;
    }
    if (h && h->sqlite) {
    trans2iso(env, h->haveutf, h->enc, dbname, &dbn);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
    }
    trans2iso(env, h->haveutf, h->enc, table, &tbl);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        transfree(&dbn);
        (*env)->DeleteLocalRef(env, exc);
        return;
    }
    trans2iso(env, h->haveutf, h->enc, column, &col);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        transfree(&tbl);
        transfree(&dbn);
        (*env)->DeleteLocalRef(env, exc);
        return;
    }
    ret = sqlite3_blob_open(h->sqlite,
                dbn.result, tbl.result, col.result,
                row, rw, &blob);
    transfree(&col);
    transfree(&tbl);
    transfree(&dbn);
    if (ret != SQLITE_OK) {
        const char *err = sqlite3_errmsg(h->sqlite);

        seterr(env, obj, ret);
        throwex(env, err ? err : "error in blob open");
        return;
    }
    bl = malloc(sizeof (hbl));
    if (!bl) {
        sqlite3_blob_close(blob);
        throwoom(env, "unable to get SQLite blob handle");
        return;
    }
    bl->next = h->blobs;
    h->blobs = bl;
    bl->blob = blob;
    bl->h = h;
    vv.j = 0;
    vv.l = (jobject) bl;
    (*env)->SetLongField(env, blobj, F_SQLite_Blob_handle, vv.j);
    (*env)->SetIntField(env, blobj, F_SQLite_Blob_size,
                sqlite3_blob_bytes(blob));
    return;
    }
    throwex(env, "not an open database");
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_progress_handler(
    /** [in] **/Int32 n, 
    /** [in] **/IProgressHandler* p)
{/*
    handle *h = gethandle(env, obj);

    if (h && h->sqlite) {
    // CHECK THIS 
#if HAVE_SQLITE_PROGRESS_HANDLER
    delglobrefp(env, &h->ph);
#if HAVE_BOTH_SQLITE
    if (h->is3) {
        if (ph) {
        globrefset(env, ph, &h->ph);
        sqlite3_progress_handler((sqlite3 *) h->sqlite,
                     n, progresshandler, h);
        } else {
        sqlite3_progress_handler((sqlite3 *) h->sqlite,
                     0, 0, 0);
        }
    } else {
        if (ph) {
        globrefset(env, ph, &h->ph);
        sqlite_progress_handler((sqlite *) h->sqlite,
                    n, progresshandler, h);
        } else {
        sqlite_progress_handler((sqlite *) h->sqlite,
                    0, 0, 0);
        }
    }
#else
#if HAVE_SQLITE2
    if (ph) {
        globrefset(env, ph, &h->ph);
        sqlite_progress_handler((sqlite *) h->sqlite,
                    n, progresshandler, h);
    } else {
        sqlite_progress_handler((sqlite *) h->sqlite,
                    0, 0, 0);
    }
#endif
#if HAVE_SQLITE3
    if (ph) {
        globrefset(env, ph, &h->ph);
        sqlite3_progress_handler((sqlite3 *) h->sqlite,
                     n, progresshandler, h);
    } else {
        sqlite3_progress_handler((sqlite3 *) h->sqlite,
                     0, 0, 0);
    }
#endif
#endif
    return;
#else
    throwex(env, "unsupported");
    return;
#endif
    }
    throwclosed(env);
    */
    return E_NOT_IMPLEMENTED;
}

ECode Database::_key(
    /** [in] **/ArrayOf<Byte>* ekey)
{/*
    jsize len;
    jbyte *data;
#if HAVE_SQLITE3_KEY
    handle *h = gethandle(env, obj);
#endif

    len = (*env)->GetArrayLength(env, key);
    data = (*env)->GetByteArrayElements(env, key, 0);
    if (len == 0) {
    data = 0;
    }
    if (!data) {
    len = 0;
    }
#if HAVE_SQLITE3_KEY
    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (!h->is3) {
        if (data) {
        memset(data, 0, len);
        }
        throwex(env, "unsupported");
    }
#endif
    sqlite3_key((sqlite3 *) h->sqlite, data, len);
    if (data) {
        memset(data, 0, len);
    }
    } else {
    if (data) {
        memset(data, 0, len);
    }
    throwclosed(env);
    }
#else
    if (data) {
    memset(data, 0, len);
    }
    // no error 
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Database::_rekey(
    /** [in] **/ArrayOf<Byte>* ekey)
{/*
    jsize len;
    jbyte *data;
#if HAVE_SQLITE3_KEY
    handle *h = gethandle(env, obj);
#endif

    len = (*env)->GetArrayLength(env, key);
    data = (*env)->GetByteArrayElements(env, key, 0);
    if (len == 0) {
    data = 0;
    }
    if (!data) {
    len = 0;
    }
#if HAVE_SQLITE3_KEY
    if (h && h->sqlite) {
#if HAVE_BOTH_SQLITE
    if (!h->is3) {
        if (data) {
        memset(data, 0, len);
        }
        throwex(env, "unsupported");
    }
#endif
    sqlite3_rekey((sqlite3 *) h->sqlite, data, len);
    if (data) {
        memset(data, 0, len);
    }
    } else {
    if (data) {
        memset(data, 0, len);
    }
    throwclosed(env);
    }
#else
    if (data) {
    memset(data, 0, len);
    }
    throwex(env, "unsupported");
#endif
    */
    return E_NOT_IMPLEMENTED;
}

ECode Database::internal_init()
{
    return E_NOT_IMPLEMENTED;
}