#include "Database.h"
#include "cmdef.h"
#include "CTableResult.h"
//#include "TableResult.h"
#include "CVm.h"
#include "CStmt.h"
#include "CBlob2.h"
#include <sqlite3.h>
#include <stdio.h>

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
    /* [in] */ const String& filename,
    /* [in] */ Int32 mode)
{
    return _open4(filename, mode, String(NULL), FALSE);
}

/* free memory proc */

typedef void (freemem)(void *);

/* internal handle for SQLite database */

typedef struct {
    void *sqlite;       /* SQLite handle */
    int ver;            /* version code */
//    jobject bh;         /* BusyHandler object */
//    jobject cb;         /* Callback object */
//    jobject ai;         /* Authorizer object */
//    jobject tr;         /* Trace object */
//    jobject ph;         /* ProgressHandler object */
//    JNIEnv *env;        /* Java environment for callbacks */
    int row1;           /* true while processing first row */
    // int haveutf;        /* true for SQLite UTF-8 support */
//    jstring enc;        /* encoding or 0 */
    struct hfunc *funcs;    /* SQLite user defined function handles */
#if HAVE_SQLITE_COMPILE
    struct hvm *vms;        /* Compiled SQLite VMs */
#endif
    sqlite3_stmt *stmt;     /* For callback() */
    struct hbl *blobs;        /* SQLite3 blob handles */
} handle;

/* internal handle for SQLite user defined function */

typedef struct hfunc {
    struct hfunc *next;     /* next function */
    // jobject fc;         /* FunctionContext object */
    // jobject fi;         /* Function object */
    // jobject db;         /* Database object */
    handle *h;          /* SQLite database handle */
    void *sf;           /* SQLite function handle */
    // JNIEnv *env;        /* Java environment for callbacks */
} hfunc;


//#if HAVE_SQLITE_COMPILE
/* internal handle for SQLite VM (sqlite_compile()) */

typedef struct hvm {
    struct hvm *next;       /* next vm handle */
    void *vm;           /* SQLite 2/3 VM/statement */
    char *tail;         /* tail SQL string */
    int tail_len;       /* only for SQLite3/prepare */
    handle *h;          /* SQLite database handle */
    handle hh;          /* fake SQLite database handle */
} hvm;
//#endif


/* internal handle for sqlite3_blob */

typedef struct hbl {
    struct hbl *next;       /* next blob handle */
    sqlite3_blob *blob;     /* SQLite3 blob */
    handle *h;          /* SQLite database handle */
} hbl;

ECode Database::_open4(
    /* [in] */ const String& filename,
    /* [in] */ Int32 mode,
    /* [in] */ const String& vfsname,
    /* [in] */ Boolean ver2)
{
    handle* h = (handle*)mHandle;
    Int32 maj, min, lev;

    if (h) {
        if (h->sqlite) {
            sqlite3_close((sqlite3*)h->sqlite);
            h->sqlite = NULL;
        }
    }
    else {
        h = (handle*)malloc(sizeof (handle));
        if (!h) {
            // throwoom(env, "unable to get SQLite handle");
            return E_OUT_OF_MEMORY_ERROR;
        }
        h->sqlite = NULL;
    // CHECK THIS
        h->stmt = NULL;
        h->funcs = NULL;
        h->ver = 0;
#if HAVE_SQLITE_COMPILE
        h->vms = NULL;
#endif
        h->blobs = NULL;
    }
    if (filename.IsNull()) {
        // throwex(env, err ? err : "invalid file name");
        return E_SQLITE_EXCEPTION;
    }
    if (sqlite3_open_v2(filename.string(), (sqlite3 **)&h->sqlite,
            mode, vfsname.string()) != SQLITE_OK) {
        if (h->sqlite) {
            sqlite3_close((sqlite3 *)h->sqlite);
            h->sqlite = NULL;
        }
    }
    if (h->sqlite) {
        mHandle = (Int32)h;
        sscanf(sqlite3_libversion(), "%d.%d.%d", &maj, &min, &lev);
        sqlite3_enable_load_extension((sqlite3 *)h->sqlite, 1);
        h->ver = ((maj & 0xFF) << 16) | ((min & 0xFF) << 8) | (lev & 0xFF);
        return NOERROR;
    }
    // throwex(env, err ? err : "unknown error in open");
    return E_SQLITE_EXCEPTION;
}

ECode Database::_open_aux_file(
    /* [in] */ String filename)
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        return E_SQLITE_EXCEPTION;
    }

    return NOERROR;
}


ECode Database::Doclose(
    /* [in] */ Int32 final)
{
    handle* h = (handle*)mHandle;

    if (h) {
        hfunc *f;
        hbl *bl;

#if HAVE_SQLITE_COMPILE
        hvm *v;
        while ((v = h->vms)) {
            h->vms = v->next;
            v->next = 0;
            v->h = 0;
            if (v->vm) {
                sqlite3_finalize((sqlite3_stmt *) v->vm);
            v->vm = 0;
            }
        }
#endif
        if (h->sqlite) {
            sqlite3_close((sqlite3 *) h->sqlite);
            h->sqlite = 0;
        }
        while ((f = h->funcs)) {
            h->funcs = f->next;
            f->h = 0;
            f->sf = 0;
            free(f);
        }

        while ((bl = h->blobs)) {
            h->blobs = bl->next;
            bl->next = 0;
            bl->h = 0;
            if (bl->blob) {
                sqlite3_blob_close(bl->blob);
            }
            bl->blob = 0;
        }

    free(h);

    return NOERROR;
    }
    return NOERROR;
}

ECode Database::_finalize()
{
    return Doclose(1);
}

ECode Database::_close()
{
    return Doclose(0);
}

static int callback(void *udata, int ncol, char **data, char **cols)
{
    return 0;
}

ECode Database::_exec(
    /** [in] **/String sql,
    /** [in] **/ICallback* cb)
{
    handle* h = (handle*)mHandle;
    freemem *freeproc;

    if (sql.IsNull()) {
        //throwex(env, "invalid SQL statement");
        return E_SQLITE_EXCEPTION;
    }
    if (h) {
        if (h->sqlite) {
            Int32 rc;
            char *err = NULL;
            h->row1 = 1;
            rc = sqlite3_exec((sqlite3 *) h->sqlite, sql.string(),
                      callback, h, &err);
            freeproc = (freemem *) sqlite3_free;
            if (err) {
                freeproc(err);
            }
            if (rc != SQLITE_OK) {
                return E_SQLITE_EXCEPTION;
            }
            return NOERROR;
        }
    }
    return NOERROR;
}

/* free memory proc */

typedef void (freemem)(void *);

static void freep(char **strp)
{
    if (strp && *strp) {
    free(*strp);
    *strp = 0;
    }
}

ECode Database::_execEx(
    /** [in] **/String sql,
    /** [in] **/ICallback* cb,
    /** [in] **/ArrayOf<String>* args)
{
    handle* h = (handle*)mHandle;
    freemem *freeproc = 0;

    if (sql.IsNull()) {
        //throwex(env, "invalid SQL statement");
        return E_SQLITE_EXCEPTION;
    }
    if (h) {
        if (h->sqlite) {
            Int32 rc = SQLITE_ERROR, nargs, i;
            char *err = 0, *p;
            const char *str = (const char *)sql;
            struct args {
                char *arg;
                //jobject obj;
                //transstr trans;
            } *argv = 0;
            char **cargv = 0;
            p = (char *) str;
            nargs = 0;
            while (*p) {
                if (*p == '%') {
                    ++p;
                    if (*p == 'q' || *p == 's') {
                        nargs++;
                        if (nargs > MAX_PARAMS) {
                            //(*env)->ReleaseStringUTFChars(env, sql, str);
                            //delglobrefp(env, &h->cb);
                            //throwex(env, "too much SQL parameters");
                            return E_SQLITE_EXCEPTION;
                        }
                    } else if (h->ver >= 0x020500 && *p == 'Q') {
                        nargs++;
                        if (nargs > MAX_PARAMS) {
                            //(*env)->ReleaseStringUTFChars(env, sql, str);
                            //delglobrefp(env, &h->cb);
                            //throwex(env, "too much SQL parameters");
                            return E_SQLITE_EXCEPTION;
                        }
                    } else if (*p != '%') {
                        //(*env)->ReleaseStringUTFChars(env, sql, str);
                        //delglobrefp(env, &h->cb);
                        //throwex(env, "bad % specification in query");
                        return E_SQLITE_EXCEPTION;
                    }
                }
                ++p;
            }
            cargv = (char **)malloc((sizeof (*argv) + sizeof (char *))
                   * MAX_PARAMS);
            if (!cargv) {
                //(*env)->ReleaseStringUTFChars(env, sql, str);
                //delglobrefp(env, &h->cb);
                //throwoom(env, "unable to allocate arg vector");
                return E_SQLITE_EXCEPTION;
            }
            argv = (struct args *) (cargv + MAX_PARAMS);
            for (i = 0; i < MAX_PARAMS; i++) {
                cargv[i] = 0;
                argv[i].arg = 0;
            }
            for (i = 0; i < nargs; i++) {
                //argv[i].arg = cargv[i] = (char *)(*args)[i];
            }
            h->row1 = 1;

            if (TRUE) {
                char *s = sqlite3_mprintf(sql.string(),
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

                if (s) {
                    rc = sqlite3_exec((sqlite3 *) h->sqlite, s, callback,
                              h, &err);
                    sqlite3_free(s);
                } else {
                    rc = SQLITE_NOMEM;
                }
                freeproc = (freemem *) sqlite3_free;
            }

            freep((char **) &cargv);

            if (err && freeproc) {
                freeproc(err);
            }

            if (rc != SQLITE_OK) {
                return E_SQLITE_EXCEPTION;
            }

            return NOERROR;
        }
    }
    //throwclosed(env);
    return NOERROR;
}

Int64 Database::_last_insert_rowid()
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        return (Int64) sqlite3_last_insert_rowid((sqlite3 *) h->sqlite);
    }

    return (Int64) 0;
}

ECode Database::_interrupt()
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        sqlite3_interrupt((sqlite3 *) h->sqlite);
        return NOERROR;
    }
    return NOERROR;
}

Int64 Database::_changes()
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        return (Int64) sqlite3_changes((sqlite3 *) h->sqlite);
    }
    return (Int64) 0;
}

static int busyhandler3(void *udata, int count)
{/*
    handle *h = (handle *) udata;
    JNIEnv *env = h->env;*/
    Int32 ret = 0;
/*
    if (env && h->bh) {
    jclass cls = (*env)->GetObjectClass(env, h->bh);
    jmethodID mid = (*env)->GetMethodID(env, cls, "busy",
                        "(Ljava/lang/String;I)Z");

    if (mid == 0) {
        (*env)->DeleteLocalRef(env, cls);
        return ret;
    }
    ret = (*env)->CallBooleanMethod(env, h->bh, mid, 0, (jint) count)
        != JNI_FALSE;
    (*env)->DeleteLocalRef(env, cls);
    }*/
    return ret;
}

ECode Database::_busy_handler(
    /** [in] **/IBusyHandler* bh)
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        sqlite3_busy_handler((sqlite3 *) h->sqlite, busyhandler3, h);
        return NOERROR;
    }

    return NOERROR;
}

ECode Database::_busy_timeout(
    /** [in] **/Int32 ms)
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        sqlite3_busy_timeout((sqlite3 * ) h->sqlite, ms);
        return NOERROR;
    }
    return NOERROR;
}

Boolean Database::_complete(
    /** [in] **/String sql)
{
    Boolean result;

    if (sql.IsNull()) {
        return FALSE;
    }

    result = sqlite3_complete(sql.string()) ? TRUE : FALSE;

    return result;
}

static void call3_common(sqlite3_context *sf, Int32 isstep, Int32 nargs, sqlite3_value **args)
{
    hfunc *f = (hfunc *) sqlite3_user_data(sf);

    if (f) {
        f->sf = sf;
    }
}

static void call3_func(sqlite3_context *sf, Int32 nargs, sqlite3_value **args)
{
    call3_common(sf, 0, nargs, args);
}

static void call3_step(sqlite3_context *sf, Int32 nargs, sqlite3_value **args)
{
    call3_common(sf, 1, nargs, args);
}

static void call3_final(sqlite3_context *sf)
{
    hfunc *f = (hfunc *) sqlite3_user_data(sf);

    if (f) {
        f->sf = sf;
    }
}

ECode Database::mkfunc_common(
    /** [in] **/ Int32 isagg,
    /** [in] **/ String name,
    /** [in] **/ Int32 nargs,
    /** [in] **/ IFunction* fi)
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        hfunc *f;
        Int32 ret;

        if (!fi) {
            //throwex(env, "null SQLite.Function not allowed");
            return E_SQLITE_EXCEPTION;
        }
        f = (hfunc *)malloc(sizeof (hfunc));
        if (!f) {
            //throwoom(env, "unable to get SQLite.FunctionContext handle");
            return E_SQLITE_EXCEPTION;
        }

        f->h = h;
        f->next = h->funcs;
        h->funcs = f;
        f->sf = 0;

        ret = sqlite3_create_function((sqlite3 *) h->sqlite,
                          name.string(),
                          (Int32) nargs,
                          SQLITE_UTF8, f,
                          isagg ? NULL : call3_func,
                          isagg ? call3_step : NULL,
                          isagg ? call3_final : NULL);

        if (ret != SQLITE_OK) {
            //throwex(env, "error creating function/aggregate");
            return E_SQLITE_EXCEPTION;
        }

        return NOERROR;
    }
    return NOERROR;
}

ECode Database::_create_function(
    /** [in] **/String name,
    /** [in] **/Int32 nargs,
    /** [in] **/IFunction* fi)
{
    return mkfunc_common(0, name, nargs, fi);
}

ECode Database::_create_aggregate(
    /** [in] **/String name,
    /** [in] **/Int32 nargs,
    /** [in] **/IFunction* fi)
{
    return mkfunc_common(1, name, nargs, fi);
}

ECode Database::_function_type(
    /** [in] **/String name,
    /** [in] **/Int32 type)
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        return NOERROR;
    }

    return NOERROR;
}

String Database::_errmsg()
{

    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        return String(sqlite3_errmsg((sqlite3 *) h->sqlite));
    }

    return String("");
}


ECode Database::_set_encoding(
    /** [in] **/String enc)
{
    return NOERROR;
}


ECode Database::_set_authorizer(
    /** [in] **/IAuthorizer* auth)
{
    return NOERROR;
}

static void dotrace(void *arg, const char *msg)
{
    return;
}

ECode Database::_trace(
    /** [in] **/ITrace* tr)
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
        sqlite3_trace((sqlite3 *) h->sqlite, tr ? dotrace : 0, h);
        return NOERROR;
    }
    return NOERROR;
}

ECode Database::Vm_compile(
    /** [in] **/String sql,
    /** [in] **/IVm* vm)
{
#if HAVE_SQLITE_COMPILE
    handle* h = (handle*)mHandle;
    void *svm = 0;
    hvm *v;
    char *err = 0;

    const char *tail;
    Int32 ret;

    if (!h) {
        return E_SQLITE_EXCEPTION;
    }
    if (!vm) {
        return E_SQLITE_EXCEPTION;
    }
    if (sql.IsNull()) {
        return E_SQLITE_EXCEPTION;
    }



    ret = sqlite3_prepare((sqlite3 *) h->sqlite, sql.string(), -1,
              (sqlite3_stmt **) &svm, &tail);

    if (ret != SQLITE_OK) {
        if (svm) {
            sqlite3_finalize((sqlite3_stmt *) svm);
            svm = 0;
        }
    }


    if (ret != SQLITE_OK) {
        return E_SQLITE_EXCEPTION;
    }
    if (!svm) {
        return NOERROR;
    }
    v = malloc(sizeof (hvm) + strlen(tail) + 1);
    if (!v) {
        sqlite3_finalize((sqlite3_stmt *) svm);
        return E_SQLITE_EXCEPTION;
    }
    v->next = h->vms;
    h->vms = v;
    v->vm = svm;
    v->h = h;
    v->tail = (char *) (v + 1);

    strcpy(v->tail, tail);
    v->hh.sqlite = 0;
    v->hh.ver = h->ver;
    v->hh.row1 = 1;
    v->hh.funcs = 0;
    v->hh.vms = 0;

#else
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    return NOERROR;
}

ECode Database::Vm_compile_args(
    /** [in] **/String sql,
    /** [in] **/IVm* vm,
    /** [in] **/ArrayOf<String>* args1)
{
#if HAVE_SQLITE_COMPILE
    handle* h = (handle*)mHandle;

    if (!h || !h->sqlite) {
        //throwclosed(env);
        return E_SQLITE_EXCEPTION;
    }
    if (!vm) {
        //throwex(env, "null vm");
        return E_SQLITE_EXCEPTION;
    }
    if (sql.IsNull()) {
        //throwex(env, "null sql");
        return E_SQLITE_EXCEPTION;
    } else {
        void *svm = 0;
        hvm *v;
        //jvalue vv;
        int rc = SQLITE_ERROR, nargs, i;
        char *p;
        const char *str = (const char *)sql;
        const char *tail;
        struct args {
            char *arg;
            //jobject obj;
            //transstr trans;
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
                        return E_SQLITE_EXCEPTION;
                    }
                } else if (*p != '%') {
                    return E_SQLITE_EXCEPTION;
                }
            }
            ++p;
        }
        cargv = (char **)malloc((sizeof (*argv) + sizeof (char *)) * MAX_PARAMS);
        if (!cargv) {
            return E_SQLITE_EXCEPTION;
        }
        argv = (struct args *) (cargv + MAX_PARAMS);
        for (i = 0; i < MAX_PARAMS; i++) {
            cargv[i] = 0;
            argv[i].arg = 0;
            //argv[i].obj = 0;
            //argv[i].trans.result = argv[i].trans.tofree = 0;
        }
        for (i = 0; i < nargs; i++) {
           // cargv[i] = (*args1)[i];
            argv[i].arg = tmp;
        }
        h->row1 = 1;

        if (TRUE) {
            char *s = sqlite3_mprintf(sql.string(),
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
                freep((char **) &cargv);
                //(*env)->ReleaseStringUTFChars(env, sql, str);
                //setvmerr(env, vm, rc);
                //throwex(env, "error in prepare");
                return E_SQLITE_EXCEPTION;
            }
            v = (hvm *)malloc(sizeof (hvm) + strlen(tail) + 1);
            if (!v) {
                sqlite3_free(s);
                freep((char **) &cargv);
                //(*env)->ReleaseStringUTFChars(env, sql, str);
                sqlite3_finalize((sqlite3_stmt *) svm);
                //setvmerr(env, vm, SQLITE_NOMEM);
                //throwoom(env, "unable to get SQLite handle");
                return E_SQLITE_EXCEPTION;
            }
            v->vm = svm;
            v->h = h;
            v->tail = (char *) (v + 1);

            strcpy(v->tail, tail);
            sqlite3_free(s);
            v->hh.sqlite = 0;
            v->hh.ver = h->ver;
            v->hh.row1 = 1;
            v->hh.funcs = 0;
            //vv.j = 0;
            //vv.l = (jobject) v;
            //(*env)->SetLongField(env, vm, F_SQLite_Vm_handle, vv.j);
        }

    freep((char **) &cargv);
    //(*env)->ReleaseStringUTFChars(env, sql, str);
    }
#else
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    return NOERROR;
}

ECode Database::Stmt_prepare(
    /** [in] **/String sql,
    /** [in] **/IStmt* stmt)
{
#if HAVE_SQLITE3
    handle* h = (handle*)mHandle;
    void *svm = 0;
    hvm *v;
    jvalue vv;
    Int32 len16;
    const char *sql16, *tail = 0;
    Int32 ret;

    if (!h) {
        //throwclosed(env);
        return E_SQLITE_EXCEPTION;
    }
    if (!stmt) {
        //throwex(env, "null stmt");
        return E_SQLITE_EXCEPTION;
    }
    if (sql.IsNull()) {
        //throwex(env, "null sql");
        return E_SQLITE_EXCEPTION;
    }

    len16 = sql.GetLength() * sizeof(char);
    if (len16 < 1) {
        return E_SQLITE_EXCEPTION;
    }

    sql16 = sql.GetPayload();
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
        //const char *err = sqlite3_errmsg(h->sqlite);
        //(*env)->ReleaseStringChars(env, sql, sql16);
        //setstmterr(env, stmt, ret);
        //throwex(env, err ? err : "error in prepare");
        return E_SQLITE_EXCEPTION;
    }
    if (!svm) {
        (*env)->ReleaseStringChars(env, sql, sql16);
        return E_SQLITE_EXCEPTION;
    }
    len16 = len16 + sizeof(char) - ((char *) tail - (char *) sql16);
    if (len16 < (Int32) sizeof (char)) {
        len16 = sizeof (char);
    }
    v = malloc(sizeof (hvm) + len16);
    if (!v) {
        //(*env)->ReleaseStringChars(env, sql, sql16);
        sqlite3_finalize((sqlite3_stmt *) svm);
        //throwoom(env, "unable to get SQLite handle");
        return E_SQLITE_EXCEPTION;
    }
    v->next = h->vms;
    h->vms = v;
    v->vm = svm;
    v->h = h;
    v->tail = (char *) (v + 1);

    memcpy(v->tail, tail, len16);
    len16 /= sizeof (char);
    ((char *) v->tail)[len16 - 1] = 0;
    //(*env)->ReleaseStringChars(env, sql, sql16);
    v->hh.sqlite = 0;
    //v->hh.haveutf = h->haveutf;
    v->hh.ver = h->ver;
    //v->hh.bh = v->hh.cb = v->hh.ai = v->hh.tr = v->hh.ph = 0;
    v->hh.row1 = 1;
    //v->hh.enc = h->enc;
    v->hh.funcs = 0;
    v->hh.vms = 0;
    //v->hh.env = 0;
    //vv.j = 0;
    //vv.l = (jobject) v;
    //(*env)->SetLongField(env, stmt, F_SQLite_Stmt_handle, vv.j);
#else
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    return E_NOT_IMPLEMENTED;
}

ECode Database::_open_blob(
    /** [in] **/String db,
    /** [in] **/String table,
    /** [in] **/String column,
    /** [in] **/Int64 row,
    /** [in] **/Boolean rw,
    /** [in] **/IBlob2* blob2)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_INCRBLOBIO
    handle* h = (handle*)mHandle;
    hbl *bl;
    sqlite3_blob *blob;
    jvalue vv;
    Int32 ret;

    if (!blob2) {
        //throwex(env, "null blob");
        return E_SQLITE_EXCEPTION;
    }
    if (h && h->sqlite) {
        ret = sqlite3_blob_open(h->sqlite,
                    db.string(), table.string(), column.string(),
                    row, rw, &blob);

        if (ret != SQLITE_OK) {
            //const char *err = sqlite3_errmsg(h->sqlite);
            //seterr(env, obj, ret);
            //throwex(env, err ? err : "error in blob open");
            return E_SQLITE_EXCEPTION;
        }
        bl = malloc(sizeof (hbl));
        if (!bl) {
            sqlite3_blob_close(blob);
            //throwoom(env, "unable to get SQLite blob handle");
            return E_SQLITE_EXCEPTION;
        }
        bl->next = h->blobs;
        h->blobs = bl;
        bl->blob = blob;
        bl->h = h;
        //vv.j = 0;
        //vv.l = (jobject) bl;
        //(*env)->SetLongField(env, blobj, F_SQLite_Blob_handle, vv.j);
        //(*env)->SetIntField(env, blobj, F_SQLite_Blob_size,
        //            sqlite3_blob_bytes(blob));
        return NOERROR;
    }
    //throwex(env, "not an open database");
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    return NOERROR;
}

static int progresshandler(void *udata)
{
    //handle *h = (handle *) udata;
    int ret = 0;

    return ret;
}

ECode Database::_progress_handler(
    /** [in] **/Int32 n,
    /** [in] **/IProgressHandler* p)
{
    handle* h = (handle*)mHandle;

    if (h && h->sqlite) {
    // CHECK THIS
#if HAVE_SQLITE_PROGRESS_HANDLER
    if (p) {
        sqlite3_progress_handler((sqlite3 *) h->sqlite,
                     n, progresshandler, h);
    } else {
        sqlite3_progress_handler((sqlite3 *) h->sqlite,
                     0, 0, 0);
    }
    return NOERROR;
#else
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    }

    return NOERROR;
}

ECode Database::_key(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    Int32 len;
    Byte *data;
#if HAVE_SQLITE3_KEY
    handle* h = (handle*)mHandle;
#endif

    len = ekey->GetLength();
    data = ekey->GetPayload();
    if (len == 0) {
        data = NULL;
    }
    if (!data) {
        len = 0;
    }
#if HAVE_SQLITE3_KEY
    if (h && h->sqlite) {
        sqlite3_key((sqlite3 *) h->sqlite, data, len);
        if (data) {
            memset(data, 0, len);
        }
    } else {
        if (data) {
            memset(data, 0, len);
        }
    }
#else
    if (data) {
        memset(data, 0, len);
    }
    // no error
#endif
    return E_NOT_IMPLEMENTED;
}

ECode Database::_rekey(
    /** [in] **/ArrayOf<Byte>* ekey)
{
    Int32 len;
    Byte *data;
#if HAVE_SQLITE3_KEY
    handle* h = (handle*)mHandle;
#endif

    len = ekey->GetLength();
    data = ekey->GetPayload();
    if (len == 0) {
        data = NULL;
    }
    if (!data) {
        len = 0;
    }
#if HAVE_SQLITE3_KEY
    if (h && h->sqlite) {
        sqlite3_rekey((sqlite3 *) h->sqlite, data, len);
        if (data) {
            memset(data, 0, len);
        }
    } else {
        if (data) {
            memset(data, 0, len);
        }
    }
#else
    if (data) {
    memset(data, 0, len);
    }
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    return NOERROR;
}

ECode Database::internal_init()
{
    return E_NOT_IMPLEMENTED;
}