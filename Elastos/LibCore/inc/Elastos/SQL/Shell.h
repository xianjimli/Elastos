#ifndef _SHELL_H_
#define _SHELL_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class Shell : public ICallback
{
    AutoPtr<IDatabase> mDb;

    Boolean mEcho;

    Int32 mCount;

    Int32 mMode;

    Boolean mShowHeader;

    String mTableName;

    String mSep;

    ArrayOf<String>* mCols;

    ArrayOf<Int32>* mColwidth;

    String mDestTable;

    AutoPtr<IPrintWriter> pw;

    AutoPtr<IPrintWriter> err;
public:
    Shell();

    Shell(
        /* [in] */IPrintWriter* pw, 
        /* [in] */IPrintWriter* err);

//    CARAPI Shell(            //IPrintStream  in io not IMPLEMENTED
//        /* [in] */IPrintStream* ps, 
//        /* [in] */IPrintStream* errs);

    CARAPI Sql_quote_dbl(
        /* [in] */String str,
        /* [out] */String* result);

    CARAPI Sql_quote(
        /* [in] */String str,
        /* [out] */String* result);

    CARAPI Columns(
        /* [in] */const ArrayOf<String>& args);

    CARAPI Types(
        /* [in] */const ArrayOf<String>& args);

    CARAPI Newrow(
        /* [in] */const ArrayOf<String>& args,
        /* [out] */Boolean* result);

    //public static void main(String args[]);

protected:
    CARAPI Clone(
        /* [out] */IInterface** obj);

    CARAPI Html_quote(
        /* [in] */String str,
        /* [out] */String* result);

    CARAPI Is_numeric(
        /* [in] */String str,
        /* [out] */Boolean* result);

    CARAPI Set_table_name(
        /* [in] */String str);

    CARAPI Do_meta(
        /* [in] */String line);

    CARAPI Read_line(
        /* [in] */IBufferedReader* is, 
        /* [in] */String prompt,
        /* [out] */String* str);

    CARAPI Do_input(
        /* [in] */IBufferedReader* is);

    CARAPI Do_cmd(
        /* [in] */String sql);
};

/**
 * Internal class for dumping an entire database.
 * It contains a special callback interface to traverse the
 * tables of the current database and output create SQL statements
 * and for the data insert SQL statements.
 */

class DBDump : public ICallback 
{
    AutoPtr<Shell> ms;

public:
    DBDump(
        /* [in] */AutoPtr<Shell> s, 
        /* [in] */const ArrayOf<String>& tables);

    CARAPI Columns(
        /* [in] */const ArrayOf<String>& args);

    CARAPI Types(
        /* [in] */const ArrayOf<String>& args);

    CARAPI Newrow(
        /* [in] */const ArrayOf<String>& args,
        /* [out] */Boolean* result);
};

#endif // _SHELL_H_