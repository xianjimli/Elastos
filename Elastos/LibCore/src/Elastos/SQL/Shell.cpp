
#include "Shell.h"
#include "StringBuffer.h"
#include "CTableResult.h"
#include "CDatabaseHelper.h"
#include "CShell.h"
 #include <elastos/System.h>

Shell::Shell()
{}

ECode Shell::Init(
    /* [in] */IPrintWriter* pw, 
    /* [in] */IPrintWriter* err)
{
    mPw = pw;
    mErr = err;
    return NOERROR;
}

ECode Shell::Init(
    /* [in] */IPrintStream* ps, 
    /* [in] */IPrintStream* errs)
{
    //pw = new PrintWriter(ps);
    //err = new PrintWriter(errs);
    return NOERROR;
}

ECode Shell::Sql_quote_dbl(
    /* [in] */String str,
    /* [out] */String* result)
{
    assert(result != NULL);
    if (str.IsNull()) {
        *result = String("NULL");
        return NOERROR;
    }
    UInt32 i;
    Int32 single = 0, dbl = 0;
    for (i = 0; i < str.GetLength(); i++) {
        if (str.GetChar(i) == '\'') {
            single++;
        } else if (str.GetChar(i) == '"') {
            dbl++;
        }
    }
    if (dbl == 0) {
        StringBuffer tmp;
        tmp += "\"";
        tmp += str; 
        tmp += "\"";
        *result = String(tmp);
        return NOERROR;
    }
    StringBuffer sb;
    sb += ("\"");
    for (i = 0; i < str.GetLength(); i++) {
        Char8 c = str.GetChar(i);
        if (c == '"') {
        sb += ("\"\"");
        } else {
        sb += (c);
        }
    }
    *result = String(sb);
    return NOERROR;
}

ECode Shell::Sql_quote(
    /* [in] */String str,
    /* [out] */String* result)
{
    assert(result != NULL);
    if (str.IsNull()) {
        *result = String("NULL");
        return NOERROR;
    }
    UInt32 i;
    Int32 single = 0, dbl = 0;
    for (i = 0; i < str.GetLength(); i++) {
        if (str.GetChar(i) == '\'') {
            single++;
        } else if (str.GetChar(i) == '"') {
            dbl++;
        }
    }
    if (single == 0) {
        StringBuffer tmp;
        tmp += "'";
        tmp += str;
        tmp += "'";
        *result = String(tmp);
        return NOERROR;
    }
    if (dbl == 0) {
        StringBuffer tmp2;
        tmp2 += "\"";
        tmp2 += str; 
        tmp2 += "\"";
        *result = String(tmp2);
        return NOERROR;
    }
    StringBuffer sb;
    sb += "'";
    for (i = 0; i < str.GetLength(); i++) {
        Char8 c = str.GetChar(i);
        if (c == '\'') {
            sb += ("''");
        } else {
            sb += (c);
        }
    }
    *result = String(sb);
    return NOERROR;
}

ECode Shell::Columns(
    /* [in] */ArrayOf<String>* args)
{
    mCols = args;
    return NOERROR;
}

ECode Shell::Types(
    /* [in] */ArrayOf<String>* args)
{
    return NOERROR;
}

ECode Shell::Newrow(
    /* [in] */ArrayOf<String>* args,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    Int32 i;
    String tname;
    String csep;
    switch (mMode) {
    case IShell_MODE_Line:
        if (args->GetLength() == 0) {
            break;
        }
        if (mCount++ > 0) {
            mPw->PrintlnString(String(""));
        }
        for (i = 0; i < args->GetLength(); i++) {
            mPw->PrintlnString(String((*mCols)[i] + " = " +
               (*args)[i] == NULL ? "NULL" : (*args)[i]));
        }
        break;
    case IShell_MODE_Column:
        csep = "";
        if (mCount++ == 0) {
            mColwidth = ArrayOf<Int32>::Alloc(args->GetLength());
            for (i = 0; i < args->GetLength(); i++) {
                Int32 w;
                w = (*mCols)[i].GetLength();
                if (w < 10) {
                    w = 10;
                }
                (*mColwidth)[i] = w;
                if (mShowHeader) {
                    mPw->PrintString(String(csep + (*mCols)[i]));
                    csep = " ";
                }
            }
            if (mShowHeader) {
                mPw->PrintlnString(String(""));
            }
        }
        if (args->GetLength() == 0) {
            break;
        }
        csep = "";
        for (i = 0; i < args->GetLength(); i++) {
            mPw->PrintString(String(csep + (*args)[i] == NULL ? "NULL" : (*args)[i]));
            csep = " ";
        }
        mPw->PrintlnString(String(""));
        break;
    case IShell_MODE_Semi:
    case IShell_MODE_List:
        if (mCount++ == 0 && mShowHeader) {
            for (i = 0; i < args->GetLength(); i++) {
                mPw->PrintString(String((*mCols)[i] +
                     (i == args->GetLength() - 1 ? "\n" : mSep)));
            }
        }
        if (args->GetLength() == 0) {
            break;
        }
        for (i = 0; i < args->GetLength(); i++) {
            mPw->PrintString(String((*args)[i] == NULL ? "NULL" : (*args)[i]));
            if (mMode == IShell_MODE_Semi) {
                mPw->PrintString(String(";"));
            } else if (i < args->GetLength() - 1) {
                mPw->PrintString(mSep);
            }
        }
        mPw->PrintlnString(String(""));
        break;
    case IShell_MODE_Html:
        if (mCount++ == 0 && mShowHeader) {
            mPw->PrintString(String("<TR>"));
        for (i = 0; i < args->GetLength(); i++) {
            String str;
            Html_quote((*mCols)[i], &str);
            mPw->PrintString(String("<TH>" + str + "</TH>"));
        }
        mPw->PrintlnString(String("</TR>"));
        }
        if (args->GetLength() == 0) {
        break;
        }
        mPw->PrintString(String("<TR>"));
        for (i = 0; i < args->GetLength(); i++) {
            String str;
            Html_quote((*args)[i], &str);
            mPw->PrintString(String("<TD>" + str + "</TD>"));
        }
        mPw->PrintlnString(String("</TR>"));
        break;
    case IShell_MODE_Insert:
        if (args->GetLength() == 0) {
        break;
        }
        tname = mTableName;
        if (!mDestTable.IsNull()) {
            tname = mDestTable;
        }
        mPw->PrintString(String("INSERT INTO " + tname + " VALUES("));
        for (i = 0; i < args->GetLength(); i++) {
            String tsep = String(i > 0 ? "," : "");
            if (args[i] == NULL) {
                mPw->PrintString(String(tsep + "NULL"));
            } else {
                Boolean result;
                Is_numeric((*args)[i], &result);
                if (result) {
                    mPw->PrintString(String(tsep + (*args)[i]));
                } else {
                    String str;
                    Sql_quote((*args)[i], &str);
                    mPw->PrintString(String(tsep + str));
                }
            }
        }
        mPw->PrintlnString(String(");"));
        break;
    case IShell_MODE_Insert2:
        if (args->GetLength() == 0) {
        break;
        }
        tname = mTableName;
        if (!mDestTable.IsNull()) {
            tname = mDestTable;
        }
        mPw->PrintString(String("INSERT INTO " + tname + " VALUES("));
        for (i = 0; i < args->GetLength(); i++) {
            String tsep = String(i > 0 ? "," : "");
            mPw->PrintString(String(tsep + (*args)[i]));
        }
        mPw->PrintlnString(String(");"));
        break;
    }
    *result = FALSE;
    return NOERROR;
}

ECode Shell::Clone(
    /* [out] */IInterface** obj)
{
    //AutoPtr<Shell> s = new Shell(mPw, mErr);
    AutoPtr<IShell> sh;
    CShell::New(mPw, mErr, (IShell**)&sh); 
    Shell* s = (Shell *)sh->Probe(EIID_IShell);

    s->mDb = mDb;
    s->mEcho = mEcho;
    s->mMode = mMode;
    s->mCount = 0;
    s->mShowHeader = mShowHeader;
    s->mTableName = mTableName;
    s->mSep = mSep;
    s->mColwidth = mColwidth;

    *obj = (IInterface*)sh;

    return NOERROR;
}

ECode Shell::Html_quote(
    /* [in] */String str,
    /* [out] */String* result)
{
    assert(result != NULL);
    if (str.IsNull()) {
        *result = String("NULL");
        return NOERROR;
    }
    StringBuffer sb;
    for (UInt32 i = 0; i < str.GetLength(); i++) {
        Char8 c = str.GetChar(i);
        if (c == '<') {
            sb += ("&lt;");
        } else if (c == '>') {
            sb += ("&gt;");
        } else if (c == '&') {
            sb += ("&amp;");
        } else {
        Int32 x = c;
        if (x < 32 || x > 127) {
            sb += ("&#");
            sb += x;
            sb += (";");
        } else {
            sb += c;
        }
        }
    }
    *result = String(sb);
    return NOERROR;
}

ECode Shell::Is_numeric(
    /* [in] */String str,
    /* [out] */Boolean* result)
{
    assert(result != NULL);/*
    try {
        Double d = Double.valueOf(str);
    } catch (java.lang.Exception e) {
        return false;
    }
    *result = TRUE;*/
    return NOERROR;
}

ECode Shell::Set_table_name(
    /* [in] */const String& str)
{
    if (str.IsNull()) {
        mTableName = "";
        return NOERROR;
    }
    Boolean result;
    mDb->Is3(&result);
    if (result) {
        Sql_quote_dbl(str,&mTableName);
    } else {
        Sql_quote(str, &mTableName);
    }
    return NOERROR;
}

ECode Shell::Do_meta(
    /* [in] */const String& line)
{
    //emake
    //String tmp(String::Duplicate(line));
    //tmp.ToLowerCase();
    String tmp("?????????????");//?????????????????????????
    StringTokenizer *st = new StringTokenizer(tmp);
    Int32 n = st->GetCount();
    if (n <= 0) {
        return NOERROR;
    }
    String cmd = st->NextToken();
    //String args[] = new String[n - 1];
    ArrayOf<String>* args = ArrayOf<String>::Alloc(n - 1);
    Int32 i = 0;
    while (st->HasMoreTokens()) {
        (*args)[i] = st->NextToken();
        ++i;
    }
    if (cmd.Compare(".dump") == 0) {
        new DBDump((IShell *)this, *args);//??????????
        return NOERROR;
    }
    if (cmd.Compare(".echo") == 0) {
        if (args->GetLength() > 0 &&
            ((*args)[0].StartWith("y") || (*args)[0].StartWith("on"))) {
            mEcho = TRUE;
        }
        return NOERROR;
    }
    if (cmd.Compare(".exit") == 0) {
        mDb->Close();
        return NOERROR; //System::Exit(0); ???????????????????????????
    }
    if (cmd.Compare(".header") == 0) {
        if (args->GetLength() > 0 &&
            ((*args)[0].StartWith("y") || (*args)[0].StartWith("on"))) {
            mShowHeader = TRUE;
        }
        return NOERROR;
    }
    if (cmd.Compare(".help") == 0) {
        mPw->PrintlnString(String(".dump ?TABLE? ...  Dump database in text fmt"));
        mPw->PrintlnString(String(".echo ON|OFF       Command echo on or off"));
        mPw->PrintlnString(String(".enc ?NAME?        Change encoding"));
        mPw->PrintlnString(String(".exit              Exit program"));
        mPw->PrintlnString(String(".header ON|OFF     Display headers on or off"));
        mPw->PrintlnString(String(".help              This message"));
        mPw->PrintlnString(String(".mode MODE         Set output mode to\n") +
               String("                   line, column, insert\n") +
               String("                   list, or html"));
        mPw->PrintlnString(String(".mode insert TABLE Generate SQL insert stmts"));
        mPw->PrintlnString(String(".schema ?PATTERN?  List table schema"));
        mPw->PrintlnString(String(".separator STRING  Set separator string"));
        mPw->PrintlnString(String(".tables ?PATTERN?  List table names"));
        return NOERROR;
    }
    if (cmd.Compare(".mode") == 0) {
        if (args->GetLength() > 0) {
        if ((*args)[0].Compare("line") == 0) {
            mMode = IShell_MODE_Line;
        } else if ((*args)[0].Compare("column") == 0) {
            mMode = IShell_MODE_Column;
        } else if ((*args)[0].Compare("list") == 0) {
            mMode = IShell_MODE_List;
        } else if ((*args)[0].Compare("html") == 0) {
            mMode = IShell_MODE_Html;
        } else if ((*args)[0].Compare("insert") == 0) {
            mMode = IShell_MODE_Insert;
            if (args->GetLength() > 1) {
                mDestTable = (*args)[1];
            }
        }
        }
        return NOERROR;
    }
    if (cmd.Compare(".separator") == 0) {
        if (args->GetLength() > 0) {
            mSep = (*args)[0];
        }
        return NOERROR;
    }
    if (cmd.Compare(".tables") == 0) {
        AutoPtr<ITableResult> t;
        if (args->GetLength() > 0) {
            //String qarg[] = new String[1];
            ArrayOf<String> *qarg = ArrayOf<String>::Alloc(1);
            (*qarg)[0] = (*args)[0];
            if(NOERROR != mDb->Get_tableEx3(String("SELECT name FROM sqlite_master ") +
                     String("WHERE type='table' AND ") +
                     String("name LIKE '%%%q%%' ") +
                     String("ORDER BY name"), qarg, (ITableResult**)&t)){
                mErr->PrintlnString(String("SQL Error: "));
                mErr->Flush();
            }
        } else {
            if(NOERROR != mDb->Get_tableEx(String("SELECT name FROM sqlite_master ") +
                     String("WHERE type='table' ORDER BY name"), (ITableResult**)&t)){
                mErr->PrintlnString(String("SQL Error: "));
                mErr->Flush();
            }
        }
        if (t != NULL) {
            //for (i = 0; i < t->mNrows; i++) {
                //String tab = ((String[]) t->mRows.elementAt(i))[0]; ????????????????????????????
                //if (tab != null) {
                //    pw.println(tab);
                //}
            //}
        }
        return NOERROR;
    }
    if (cmd.Compare(".schema") == 0) {
        if (args->GetLength() > 0) {
            ArrayOf<String> *qarg = ArrayOf<String>::Alloc(1);
            (*qarg)[0] = (*args)[0];
            if(NOERROR != mDb->ExecEx(String("SELECT sql FROM sqlite_master ") +
                String("WHERE type!='meta' AND ") +
                String("name LIKE '%%%q%%' AND ") +
                String("sql NOTNULL ") +
                String("ORDER BY type DESC, name"),
                (ICallback*)this, qarg)) {
                mErr->PrintlnString(String("SQL Error: "));
                mErr->Flush();
            }
            ArrayOf<String>::Free(qarg);
        } else {
            if(NOERROR != mDb->Exec(String("SELECT sql FROM sqlite_master ") +
                    String("WHERE type!='meta' AND ") +
                    String("sql NOTNULL ") +
                    String("ORDER BY tbl_name, type DESC, name"),
                    (ICallback*)this)) {
                mErr->PrintlnString(String("SQL Error: "));
                mErr->Flush();
            }
        }
        return NOERROR;
    }
    if (cmd.Compare(".enc") == 0) {
        if(NOERROR != mDb->Set_encoding(args->GetLength() > 0 ? (*args)[0] : String("")))
        {
            mErr->PrintlnString(String("SQL Error: "));
            mErr->Flush();
        }
        return NOERROR;
    }
    if (cmd.Compare(".rekey") == 0) {
        ArrayOf<Byte>* tmpargs;
        if(args->GetLength() > 0){
            tmpargs = ArrayOf<Byte>::Alloc((*args)[0].GetLength());
            UInt32 i;
            for(i=0;i<(*args)[0].GetLength();i++)
            {
                (*tmpargs)[i] = (Byte)(*args)[0][i];
            }

        }
        if(NOERROR != mDb->Rekey(args->GetLength() > 0 ? tmpargs : NULL))
        {
            mErr->PrintlnString(String("SQL Error: "));
            mErr->Flush();
        }
        return NOERROR;
    }
    mErr->PrintlnString(String("Unknown command  "));
    mErr->Flush();
    return NOERROR;
}

ECode Shell::Read_line(
    /* [in] */IBufferedReader* is, 
    /* [in] */const String& prompt,
    /* [out] */String* str)
{
    assert(str != NULL);

    if (!prompt.IsNull()) {
        mPw->PrintString(prompt);
        mPw->Flush();
    }
    String line;
    if(NOERROR != is->ReadLine(&line))
    {
        *str = line;
        return NOERROR;
    } else {
        *str = "";
        return NOERROR;
    }
    return NOERROR;
}

ECode Shell::Do_input(
    /* [in] */IBufferedReader* is)
{
    String line, sql;
    sql = "";
    String prompt = String("SQLITE> ");
    Read_line(is, prompt, &line);
    while (!line.IsNull()) {
        if (mEcho) {
            mPw->PrintlnString(line);
        }
        if (line.GetLength() > 0 && line.GetChar(0) == '.') {
            Do_meta(line);
        } else {
            if (sql.IsNull()) {
                sql = line;
            } else {
                sql = sql + " " + line;
            }
            AutoPtr<IDatabaseHelper> dbh;
            Boolean result;
            CDatabaseHelper::AcquireSingleton((IDatabaseHelper**)&dbh);
            dbh->Complete(sql, &result);
            
            if (result) {
                if(NOERROR != mDb->Exec(sql, (ICallback*)this))
                {
                    if (!mEcho) {
                        mErr->PrintlnString(sql);
                    }
                    mErr->PrintlnString(String("SQL Error: "));
                    mErr->Flush();
                }
                sql = "";
                prompt = "SQLITE> ";
            } else {
                prompt = "SQLITE? ";
            }
        }
        mPw->Flush();
        Read_line(is, prompt, &line);
    }
    if (!sql.IsNull()) {
        mErr->PrintlnString(String("Incomplete SQL: " + sql));
        mErr->Flush();
    }
    return NOERROR;
}

ECode Shell::Do_cmd(
    /* [in] */const String& sql)
{
    if (mDb == NULL) {
        return NOERROR;
    }

    if (sql.GetLength() > 0 && sql.GetChar(0) == '.') {
        Do_meta(sql);
    } else {
        if(NOERROR != mDb->Exec(sql, (ICallback*)this))
        {
            mErr->PrintlnString(String("SQL Error: "));
            mErr->Flush();
        }
    }
    return NOERROR;
}

DBDump::DBDump(
    /* [in] */IShell* s, 
    /* [in] */const ArrayOf<String>& tables)
{
    ms = s;
    ECode ec;
    Shell* she = (Shell *)s->Probe(EIID_IShell);
    she->mPw->PrintlnString(String("BEGIN TRANSACTION;"));
    if (tables.GetPayload() == NULL || tables.GetLength() == 0) {
        ec = she->mDb->Exec(String("SELECT name, type, sql FROM sqlite_master ") +
          String("WHERE type!='meta' AND sql NOT NULL ") +
          String("ORDER BY substr(type,2,1), name"), (ICallback*)this);
        if(NOERROR != ec)
        {
            she->mErr->PrintlnString(String("SQL Error: "));
            she->mErr->Flush();
        }
    } else {
        //String arg[] = new String[1];
        ArrayOf<String>* arg = ArrayOf<String>::Alloc(1);
        for (Int32 i = 0; i < tables.GetLength(); i++) {
            (*arg)[0] = tables[i];
            ec = she->mDb->ExecEx(String("SELECT name, type, sql FROM sqlite_master ") +
                      String("WHERE tbl_name LIKE '%q' AND type!='meta' ") +
                      String(" AND sql NOT NULL ") +
                      String(" ORDER BY substr(type,2,1), name"),
                      (ICallback*)this, arg);
            if(NOERROR != ec)
            {
                she->mErr->PrintlnString(String("SQL Error: "));
                she->mErr->Flush();
            }
        }
        ArrayOf<String>::Free(arg);
    }
    she->mPw->PrintlnString(String("COMMIT;"));
}

ECode DBDump::Columns(
    /* [in] */ArrayOf<String>* args)
{
    return NOERROR;
}

ECode DBDump::Types(
    /* [in] */ArrayOf<String>* args)
{
    return NOERROR;
}

ECode DBDump::Newrow(
    /* [in] */ArrayOf<String>* args,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    if (args->GetLength() != 3) {
        *result = TRUE;
        return NOERROR;
    }
    String sep;
    Shell* she = (Shell *)ms->Probe(EIID_IShell);
    she->mPw->PrintlnString(String((*args)[2] + ";"));
    if ((*args)[1].Compare("table") == 0) {
        //Shell s2 = (Shell) s.clone();
        AutoPtr<IShell> s2;
        she->Clone((IInterface**)&s2);

        Shell* se = (Shell *)s2->Probe(EIID_IShell);
        se->mMode = IShell_MODE_Insert;
        se->Set_table_name((*args)[0]);
        //String qargs[] = new String[1];
        ArrayOf<String>* qargs = ArrayOf<String>::Alloc(1);
        (*qargs)[0] = (*args)[0];
  //      try {
            Boolean ret;
            se->mDb->Is3(&ret);
            if (ret) {
                AutoPtr<ITableResult> t = NULL;
                se->mDb->Get_tableEx3(String("PRAGMA table_info('%q')"), qargs, (ITableResult**)&t);
                TableResult* tr = (TableResult *)t->Probe(EIID_ITableResult);
                String query;
                if (t != NULL) {
                    StringBuffer sb;
                    sep = "";

                    sb += ("SELECT ");
                    for (Int32 i = 0; i < tr->mNrows; i++) {
     //                   String col = ((String[]) tr->mRows.elementAt(i))[1];
                        String str;
    //////                    Shell.sql_quote_dbl(col, &str);
                        sb += (sep + "quote(" + str + ")");
                        sep = ",";
                    }
                    sb += (" from '%q'");
                    query = String(sb);
                    se->mMode = IShell_MODE_Insert2;
                } else {
                    query = "SELECT * from '%q'";
                }
                se->mDb->ExecEx(query, (ICallback*)s2, qargs);
                if(qargs != NULL) ArrayOf<String>::Free(qargs);
            } else {
                se->mDb->ExecEx(String("SELECT * from '%q'"), (ICallback*)s2, qargs);
                if(qargs != NULL) ArrayOf<String>::Free(qargs);
            }
  //      } catch (Exception e) {
        //    she->mErr->PrintlnString("SQL Error: " + e);
        //    she->mErr->Flush();
  //          *result = TRUE;
 //           if(qargs !=NULL) ArrayOf<String>::Free(qargs);
 //           return NOERROR;
 //       }
    }
    *result = FALSE;
    return NOERROR;
}