
#include "Shell.h"
#include "StringBuffer.h"
#include "CTableResult.h"
#include "CShell.h"

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
    Int32 i, single = 0, dbl = 0;
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
    Int32 i, single = 0, dbl = 0;
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
    AutoPtr<IShell> s;
    CShell::New(mPw, mErr, (IShell**)&s); 
/*
    s->mDb = mDb;
    s->mEcho = mEcho;
    s->mMode = mMode;
    s->mCount = 0;
    s->mShowHeader = mShowHeader;
    s->mTableName = mTableName;
    s->mSep = mSep;
    s->mColwidth = mColwidth;

    *obj = (IInterface*)s;*/

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
    for (Int32 i = 0; i < str.GetLength(); i++) {
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
{/*

    StringTokenizer st = new StringTokenizer(line.toLowerCase());
    int n = st.countTokens();
    if (n <= 0) {
        return;
    }
    String cmd = st.nextToken();
    String args[] = new String[n - 1];
    int i = 0;
    while (st.hasMoreTokens()) {
        args[i] = st.nextToken();
        ++i;
    }
    if (cmd.compareTo(".dump") == 0) {
        new DBDump(this, args);
        return;
    }
    if (cmd.compareTo(".echo") == 0) {
        if (args.length > 0 &&
        (args[0].startsWith("y") || args[0].startsWith("on"))) {
        echo = true;
        }
        return;
    }
    if (cmd.compareTo(".exit") == 0) {
        try {
        db.close();
        } catch (Exception e) {
        }
        System.exit(0);
    }
    if (cmd.compareTo(".header") == 0) {
        if (args.length > 0 &&
        (args[0].startsWith("y") || args[0].startsWith("on"))) {
        showHeader = true;
        }
        return;
    }
    if (cmd.compareTo(".help") == 0) {
        pw.println(".dump ?TABLE? ...  Dump database in text fmt");
        pw.println(".echo ON|OFF       Command echo on or off");
        pw.println(".enc ?NAME?        Change encoding");
        pw.println(".exit              Exit program");
        pw.println(".header ON|OFF     Display headers on or off");
        pw.println(".help              This message");
        pw.println(".mode MODE         Set output mode to\n" +
               "                   line, column, insert\n" +
               "                   list, or html");
        pw.println(".mode insert TABLE Generate SQL insert stmts");
        pw.println(".schema ?PATTERN?  List table schema");
        pw.println(".separator STRING  Set separator string");
        pw.println(".tables ?PATTERN?  List table names");
        return;
    }
    if (cmd.compareTo(".mode") == 0) {
        if (args.length > 0) {
        if (args[0].compareTo("line") == 0) {
            mode = Shell.MODE_Line;
        } else if (args[0].compareTo("column") == 0) {
            mode = Shell.MODE_Column;
        } else if (args[0].compareTo("list") == 0) {
            mode = Shell.MODE_List;
        } else if (args[0].compareTo("html") == 0) {
            mode = Shell.MODE_Html;
        } else if (args[0].compareTo("insert") == 0) {
            mode = Shell.MODE_Insert;
            if (args.length > 1) {
            destTable = args[1];
            }
        }
        }
        return;
    }
    if (cmd.compareTo(".separator") == 0) {
        if (args.length > 0) {
        sep = args[0];
        }
        return;
    }
    if (cmd.compareTo(".tables") == 0) {
        TableResult t = null;
        if (args.length > 0) {
        try {
            String qarg[] = new String[1];
            qarg[0] = args[0];
            t = db.get_table("SELECT name FROM sqlite_master " +
                     "WHERE type='table' AND " +
                     "name LIKE '%%%q%%' " +
                     "ORDER BY name", qarg);
        } catch (Exception e) {
            err.println("SQL Error: " + e);
            err.flush();
        }
        } else {
        try {
            t = db.get_table("SELECT name FROM sqlite_master " +
                     "WHERE type='table' ORDER BY name");
        } catch (Exception e) {
            err.println("SQL Error: " + e);
            err.flush();
        }
        }
        if (t != null) {
        for (i = 0; i < t.nrows; i++) {
            String tab = ((String[]) t.rows.elementAt(i))[0];
            if (tab != null) {
            pw.println(tab);
            }
        }
        }
        return;
    }
    if (cmd.compareTo(".schema") == 0) {
        if (args.length > 0) {
        try {
            String qarg[] = new String[1];
            qarg[0] = args[0];
            db.exec("SELECT sql FROM sqlite_master " +
                "WHERE type!='meta' AND " +
                "name LIKE '%%%q%%' AND " +
                "sql NOTNULL " +
                "ORDER BY type DESC, name",
                this, qarg);
        } catch (Exception e) {
            err.println("SQL Error: " + e);
            err.flush();
        }
        } else {
        try {
            db.exec("SELECT sql FROM sqlite_master " +
                "WHERE type!='meta' AND " +
                "sql NOTNULL " +
                "ORDER BY tbl_name, type DESC, name",
                this);
        } catch (Exception e) {
            err.println("SQL Error: " + e);
            err.flush();
        }
        }
        return;
    }
    if (cmd.compareTo(".enc") == 0) {
        try {
        db.set_encoding(args.length > 0 ? args[0] : null);
        } catch (Exception e) {
        err.println("" + e);
        err.flush();
        }
        return;
    }
    if (cmd.compareTo(".rekey") == 0) {
        try {
        db.rekey(args.length > 0 ? args[0] : null);
        } catch (Exception e) {
        err.println("" + e);
        err.flush();
        }
        return;
    }
    err.println("Unknown command '" + cmd + "'");
    err.flush();*/
    return NOERROR;
}

ECode Shell::Read_line(
    /* [in] */IBufferedReader* is, 
    /* [in] */const String& prompt,
    /* [out] */String* str)
{
    assert(str != NULL);/*
    try {
        if (prompt != null) {
        pw.print(prompt);
        pw.flush();
        }
        String line = is.readLine();
        return line;
    } catch (IOException e) {
        return null;
    }*/
    return NOERROR;
}

ECode Shell::Do_input(
    /* [in] */IBufferedReader* is)
{/*
    String line, sql = null;
    String prompt = "SQLITE> ";
    while ((line = read_line(is, prompt)) != null) {
        if (echo) {
        pw.println(line);
        }
        if (line.length() > 0 && line.charAt(0) == '.') {
            do_meta(line);
        } else {
        if (sql == null) {
            sql = line;
        } else {
            sql = sql + " " + line;
        }
        if (Database.complete(sql)) {
            try {
            db.exec(sql, this);
            } catch (Exception e) {
            if (!echo) {
                err.println(sql);
            }
            err.println("SQL Error: " + e);
            err.flush();
            }
            sql = null;
            prompt = "SQLITE> ";
        } else {
            prompt = "SQLITE? ";
        }
        }
        pw.flush();
    }
    if (sql != null) {
        err.println("Incomplete SQL: " + sql);
        err.flush();
    }*/
    return NOERROR;
}

ECode Shell::Do_cmd(
    /* [in] */const String& sql)
{/*
    if (db == null) {
        return;
    }
        if (sql.length() > 0 && sql.charAt(0) == '.') {
        do_meta(sql);
    } else {
        try {
            db.exec(sql, this);
        } catch (Exception e) {
        err.println("SQL Error: " + e);
        err.flush();
        }
    }*/
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
          //  she->mErr->PrintlnString(String("SQL Error: " + String(ec)));
          //  she->mErr->Flush();
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
            //    she->mErr.PrintlnString("SQL Error: " + e);
            //    she->mErr.Flush();
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