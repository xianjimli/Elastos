#ifndef __CDATABASE_H__
#define __CDATABASE_H__

#include "Database.h"
#include "_CDatabase.h"

CarClass(CDatabase), public Database
{
public:

    CARAPI Open(
        /** [in] **/String filename, 
        /** [in] **/Int32 mode);

    CARAPI OpenEx(
        /** [in] **/String filename, 
        /** [in] **/Int32 mode, 
        /** [in] **/String vfs);

    CARAPI OpenEx2(
        /** [in] **/String filename, 
        /** [in] **/Int32 mode, 
        /** [in] **/String vfs, 
        /** [in] **/Boolean ver2);

    CARAPI Open_aux_file(
        /** [in] **/String filename);

    CARAPI Close();

    CARAPI Exec(
        /** [in] **/String sql, 
        /** [in] **/ICallback* cb);

    CARAPI ExecEx(
        /** [in] **/String sql, 
        /** [in] **/ICallback* cb,
        /** [in] **/ArrayOf<String> args);

    CARAPI Last_insert_rowid(
        /** [out] **/Int64* id);

    CARAPI Interrupt();

    CARAPI Changes(
        /** [out] **/Int64* id);

    CARAPI Busy_handler(
        /** [in] **/IBusyHandler* bh);

    CARAPI Busy_timeout(
        /** [in] **/Int32 ms);

    CARAPI Get_table(
        /** [in] **/String sql, 
        /** [in] **/Int32 maxrows,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx(
        /** [in] **/String sql,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx2(
        /** [in] **/String sql, 
        /** [in] **/Int32 maxrows, 
        /** [in] **/ArrayOf<String> args,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx3(
        /** [in] **/String sql, 
        /** [in] **/ArrayOf<String> args,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx4(
        /** [in] **/String sql, 
        /** [in] **/ArrayOf<String> args, 
        /** [in] **/ITableResult* tbl);

    CARAPI Complete(
        /** [in] **/String sql,
        /** [out] **/Boolean* resukt);

    CARAPI Version(
        /** [out] **/String* str);

    CARAPI Dbversion(
        /** [out] **/String* str);

    CARAPI Create_function(
        /** [in] **/String name, 
        /** [in] **/Int32 nargs, 
        /** [in] **/IFunction* f);

    CARAPI Create_aggregate(
        /** [in] **/String name, 
        /** [in] **/Int32 nargs, 
        /** [in] **/IFunction* f);

    CARAPI Function_type(
        /** [in] **/String name, 
        /** [in] **/Int32 type);

    CARAPI Last_error(
        /** [out] **/Int32* num);

    CARAPI Error_message(
        /** [out] **/String* str);

    CARAPI Error_string(
        /** [in] **/Int32 error_code,
        /** [out] **/String* str);

    CARAPI Set_encoding(
        /** [in] **/String enc);

    CARAPI Set_authorizer(
        /** [in] **/IAuthorizer* auth);

    CARAPI Trace(
        /** [in] **/ITrace* tr);

    CARAPI Compile(
        /** [in] **/String sql,
        /** [out] **/IVm** vm);

    CARAPI CompileEx(
        /** [in] **/String sql, 
        /** [in] **/ArrayOf<String> args,
        /** [out] **/IVm** vm);

    CARAPI Prepare(
        /** [in] **/String sql,
        /** [out] **/IStmt** tmt);

    CARAPI Open_blob(
        /** [in] **/String db, 
        /** [in] **/String table, 
        /** [in] **/String column,
        /** [in] **/Int64 row, 
        /** [in] **/Boolean rw,
        /** [out] **/IBlob2** blob);

    CARAPI Is3(
        /** [out] **/Boolean* result);

    CARAPI Progress_handler(
        /** [in] **/Int32 n, 
        /** [in] **/IProgressHandler* p);

    CARAPI Key(
        /** [in] **/ArrayOf<Byte> ekey);

    CARAPI KeyEx(
        /** [in] **/String skey);

    CARAPI Rekey(
        /** [in] **/ArrayOf<Byte> ekey);

    CARAPI RekeyEx(
        /** [in] **/String skey);

    CARAPI Long_from_julian(
        /** [in] **/Double d,
        /** [out] **/Int64* result);

    CARAPI Long_from_julianEx(
        /** [in] **/String s,
        /** [out] **/Int64* result);

    CARAPI Julian_from_long(
        /** [in] **/Int64 ms,
        /** [out] **/Double* result);

protected:

    CARAPI Finalize();

    CARAPI Set_last_error(
        /** [in] **/Int32 error_code);

    CARAPI _enable_shared_cache(
        /** [in] **/Boolean onoff,
        /** [out] **/Boolean* result);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif //__CDATABASE_H__