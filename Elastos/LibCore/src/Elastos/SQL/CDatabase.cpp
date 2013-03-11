
#include "cmdef.h"
#include "CDatabase.h"

ECode CDatabase::Available(

ECode CDatabase::Open(
        /** [in] **/String filename, 
        /** [in] **/Int32 mode)
{
	return Database::Open(filename, mode);
}

ECode CDatabase::OpenEx(
        /** [in] **/String filename, 
        /** [in] **/Int32 mode, 
        /** [in] **/String vfs)
{
    return Database::OpenEx(filename,mode,vfs);
}

ECode CDatabase::OpenEx2(
        /** [in] **/String filename, 
        /** [in] **/Int32 mode, 
        /** [in] **/String vfs, 
        /** [in] **/Boolean ver2)
{
    return Database::OpenEx2(filename,mode,vfs,ver2);
}

ECode CDatabase::Open_aux_file(
        /** [in] **/String filename)
{
    return Database::Open_aux_file(filename);
}

ECode CDatabase::Close()
{
    return Database::Close();
}

ECode CDatabase::Exec(
        /** [in] **/String sql, 
        /** [in] **/ICallback* cb)
{
    return Database::Exec(sql, cb);
}

ECode CDatabase::ExecEx(
        /** [in] **/String sql, 
        /** [in] **/ICallback* cb,
        /** [in] **/ArrayOf<String> args)
{
    return Database::ExecEx(sql, cb, args);
}

ECode CDatabase::Last_insert_rowid(
        /** [out] **/Int64* id)
{
    VALIDATE_NOT_NULL(id);

    return Database::Last_insert_rowid(id);
}

ECode CDatabase::Interrupt()
{
    return Database::Interrupt();
}    

ECode CDatabase::Changes(
        /** [out] **/Int64* id)
{
    VALIDATE_NOT_NULL(id);

    return Database::Changes(id);
}

ECode CDatabase::Busy_handler(
        /** [in] **/IBusyHandler* bh)
{
    return Database::Busy_handler(bh);
}

ECode CDatabase::Busy_timeout(
        /** [in] **/Int32 ms)
{
    return Database::Busy_timeout(ms);
}

ECode CDatabase::Get_table(
        /** [in] **/String sql, 
        /** [in] **/Int32 maxrows,
        /** [out] **/ITableResult** result)
{
    return Database::Get_table(sql, maxrows, result);
}

ECode CDatabase::Get_tableEx(
        /** [in] **/String sql,
        /** [out] **/ITableResult** result)
{
    return Database::Get_tableEx(sql, result);
}

ECode CDatabase::Get_tableEx2(
        /** [in] **/String sql, 
        /** [in] **/Int32 maxrows, 
        /** [in] **/ArrayOf<String> args,
        /** [out] **/ITableResult** result)
{
    return Database::Get_tableEx2(sql, maxrows, args, result);
}

ECode CDatabase::Get_tableEx3(
        /** [in] **/String sql, 
        /** [in] **/ArrayOf<String> args,
        /** [out] **/ITableResult** result)
{
    return Database::Get_tableEx3(sql, args, result);
}

ECode CDatabase::Get_tableEx4(
        /** [in] **/String sql, 
        /** [in] **/ArrayOf<String> args, 
        /** [in] **/ITableResult* tbl)
{
    return Database::Get_tableEx4(sql, args, tbl);
}

ECode CDatabase::Complete(
        /** [in] **/String sql,
        /** [out] **/Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Database::Complete(sql, result);
}

ECode CDatabase::Version(
        /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Database::Version(str);
}

ECode CDatabase::Dbversion(
        /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Database::Dbversion(str);
}

ECode CDatabase::Create_function(
        /** [in] **/String name, 
        /** [in] **/Int32 nargs, 
        /** [in] **/IFunction* f)
{
    return Database::Create_function(name, nargs, f);
}

ECode CDatabase::Create_aggregate(
        /** [in] **/String name, 
        /** [in] **/Int32 nargs, 
        /** [in] **/IFunction* f)
{
    return Database::Create_aggregate(name, nargs, f);
}

ECode CDatabase::Function_type(
        /** [in] **/String name, 
        /** [in] **/Int32 type)
{
    return Database::Function_type(name, type);
}

ECode CDatabase::Last_error(
        /** [out] **/Int32* num)
{
    VALIDATE_NOT_NULL(num);

    return Database::Last_error(num);
}

ECode CDatabase::Error_message(
        /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Database::Error_message(str)
}

ECode CDatabase::Error_string(
        /** [in] **/Int32 error_code,
        /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Database::Error_string(error_code, str);
}

ECode CDatabase::Set_encoding(
        /** [in] **/String enc)
{
    return Database::Set_encoding(enc);
}

ECode CDatabase::Set_authorizer(
        /** [in] **/IAuthorizer* auth)
{
    return Database::Set_authorizer(auth);
}

ECode CDatabase::Trace(
        /** [in] **/ITrace* tr)
{
    return Database::Trace(tr);
}

ECode CDatabase::Compile(
        /** [in] **/String sql,
        /** [out] **/IVm** vm)
{
    return Database::Compile(sql, vm);
}

ECode CDatabase::CompileEx(
        /** [in] **/String sql, 
        /** [in] **/ArrayOf<String> args,
        /** [out] **/IVm** vm)
{
    return Database::CompileEx(sql, args, vm);
}

ECode CDatabase::Prepare(
        /** [in] **/String sql,
        /** [out] **/IStmt** tmt)
{
    return Database::Prepare(sql, tmt);
}

ECode CDatabase::Open_blob(
        /** [in] **/String db, 
        /** [in] **/String table, 
        /** [in] **/String column,
        /** [in] **/Int64 row, 
        /** [in] **/Boolean rw,
        /** [out] **/IBlob2** blob)
{
    return Database::Open_blob(db, table, column, row, rw, blob);
}

ECode CDatabase::Is3(
        /** [out] **/Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Database::Is3(result);
}

ECode CDatabase::Progress_handler(
        /** [in] **/Int32 n, 
        /** [in] **/IProgressHandler* p)
{
    return Database::Progress_handler(n, p);
}

ECode CDatabase::Key(
        /** [in] **/ArrayOf<Byte> ekey)
{
    return Database::Key(ekey);
}

ECode CDatabase::KeyEx(
        /** [in] **/String skey)
{
    return Database::KeyEx(skey);
}

ECode CDatabase::Rekey(
        /** [in] **/ArrayOf<Byte> ekey)
{
    return Database::Rekey(ekey);
}

ECode CDatabase::RekeyEx(
        /** [in] **/String skey)
{
    return Database::RekeyEx(skey);
}

ECode CDatabase::Long_from_julian(
        /** [in] **/Double d,
        /** [out] **/Int64* result)
{
    VALIDATE_NOT_NULL(result);

    return Database::Long_from_julian(d, result);
}

ECode CDatabase::Long_from_julianEx(
        /** [in] **/String s,
        /** [out] **/Int64* result)
{
    VALIDATE_NOT_NULL(result);

    return Database::Long_from_julianEx(s, result);
}

ECode CDatabase::Julian_from_long(
        /** [in] **/Int64 ms,
        /** [out] **/Double* result)
{
    VALIDATE_NOT_NULL(result);

    return  Database::Julian_from_long(ms, result);
}

ECode CDatabase::Finalize()
{
    return Database::Finalize();
}

ECode CDatabase::Set_last_error(
        /** [in] **/Int32 error_code)
{
    return Database::Set_last_error(error_code);
}

ECode CDatabase::_enable_shared_cache(
        /** [in] **/Boolean onoff,
        /** [out] **/Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Database::_enable_shared_cache(onoff, result);
}

ECode CDatabase::constructor()
{
    return NOERROR;
}

Mutex* CDatabase::GetSelfLock()
{
    return &_m_syncLock;
}