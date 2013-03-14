#ifndef __CDATABASE_H__
#define __CDATABASE_H__

#include "Database.h"
#include "_CDatabase.h"

CarClass(CDatabase), public Database
{
public:
    CARAPI constructor();

    CARAPI Open(
        /** [in] **/const String &filename, 
        /** [in] **/Int32 mode);

    CARAPI OpenEx(
        /** [in] **/const String &filename, 
        /** [in] **/Int32 mode, 
        /** [in] **/const String &vfs);

    CARAPI OpenEx2(
        /** [in] **/const String &filename, 
        /** [in] **/Int32 mode, 
        /** [in] **/const String &vfs, 
        /** [in] **/Boolean ver2);

    CARAPI Open_aux_file(
        /** [in] **/const String &filename);

    CARAPI Close();

    CARAPI Exec(
        /** [in] **/const String &sql, 
        /** [in] **/ICallback* cb);

    CARAPI ExecEx(
        /** [in] **/const String &sql, 
        /** [in] **/ICallback* cb,
        /** [in] **/ArrayOf<String>* args);

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
        /** [in] **/const String &sql, 
        /** [in] **/Int32 maxrows,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx(
        /** [in] **/const String &sql,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx2(
        /** [in] **/const String &sql, 
        /** [in] **/Int32 maxrows, 
        /** [in] **/ArrayOf<String>* args,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx3(
        /** [in] **/const String &sql, 
        /** [in] **/ArrayOf<String>* args,
        /** [out] **/ITableResult** result);

    CARAPI Get_tableEx4(
        /** [in] **/const String &sql, 
        /** [in] **/ArrayOf<String>* args, 
        /** [in] **/ITableResult* tbl);

    CARAPI Complete(
        /** [in] **/const String &sql,
        /** [out] **/Boolean* resukt);

    CARAPI Version(
        /** [out] **/String* str);

    CARAPI Dbversion(
        /** [out] **/String* str);

    CARAPI Create_function(
        /** [in] **/const String &name, 
        /** [in] **/Int32 nargs, 
        /** [in] **/IFunction* f);

    CARAPI Create_aggregate(
        /** [in] **/const String &name, 
        /** [in] **/Int32 nargs, 
        /** [in] **/IFunction* f);

    CARAPI Function_type(
        /** [in] **/const String &name, 
        /** [in] **/Int32 type);

    CARAPI Last_error(
        /** [out] **/Int32* num);

    CARAPI Error_message(
        /** [out] **/String* str);

    CARAPI Error_string(
        /** [in] **/Int32 error_code,
        /** [out] **/String* str);

    CARAPI Set_encoding(
        /** [in] **/const String &enc);

    CARAPI Set_authorizer(
        /** [in] **/IAuthorizer* auth);

    CARAPI Trace(
        /** [in] **/ITrace* tr);

    CARAPI Compile(
        /** [in] **/const String &sql,
        /** [out] **/IVm** vm);

    CARAPI CompileEx(
        /** [in] **/const String &sql, 
        /** [in] **/ArrayOf<String>* args,
        /** [out] **/IVm** vm);

    CARAPI Prepare(
        /** [in] **/const String &sql,
        /** [out] **/IStmt** tmt);

    CARAPI Open_blob(
        /** [in] **/const String &db, 
        /** [in] **/const String &table, 
        /** [in] **/const String &column,
        /** [in] **/Int64 row, 
        /** [in] **/Boolean rw,
        /** [out] **/IBlob2** blob);

    CARAPI Is3(
        /** [out] **/Boolean* result);

    CARAPI Progress_handler(
        /** [in] **/Int32 n, 
        /** [in] **/IProgressHandler* p);

    CARAPI Key(
        /** [in] **/ArrayOf<Byte>* ekey);

    CARAPI KeyEx(
        /** [in] **/const String &skey);

    CARAPI Rekey(
        /** [in] **/ArrayOf<Byte>* ekey);

    CARAPI RekeyEx(
        /** [in] **/const String &skey);

    CARAPI Long_from_julian(
        /** [in] **/Double d,
        /** [out] **/Int64* result);

    CARAPI Long_from_julianEx(
        /** [in] **/const String &s,
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