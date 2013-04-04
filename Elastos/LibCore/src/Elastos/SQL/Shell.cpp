
#include "Shell.h"
#include "StringBuffer.h"
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
{/*
    assert(result != NULL);
    Int32 i;
    String tname;
    switch (mMode) {
    case IShell_MODE_Line:
        if (args.GetLength() == 0) {
            break;
        }
        if (mCount++ > 0) {
            mPw->Println("");
        }
        for (i = 0; i < args.GetLength(); i++) {
            mPw->Println(cols[i] + " = " +
               args[i] == null ? "NULL" : args[i]);
        }
        break;
    case IShell_MODE_Column:
        String csep = "";
        if (mCount++ == 0) {
        mColwidth = ArrayOf<Int32>::Alloc(args.GetLength());
        for (i = 0; i < args.GetLength(); i++) {
            Int32 w, n;
            w = (*mCols)[i].GetLength();
            if (w < 10) {
                w = 10;
            }
            mColwidth[i] = w;
            if (mShowHeader) {
            mPw->Print(csep + cols[i]);
            csep = " ";
            }
        }
        if (mShowHeader) {
            mPw->Println("");
        }
        }
        if (args.GetLength() == 0) {
        break;
        }
        csep = "";
        for (i = 0; i < args.GetLength(); i++) {
        mPw->Print(csep + (args[i] == NULL ? "NULL" : args[i]));
        csep = " ";
        }
        mPw->Println("");
        break;
    case IShell_MODE_Semi:
    case IShell_MODE_List:
        if (mCount++ == 0 && mShowHeader) {
        for (i = 0; i < args.GetLength(); i++) {
            mPw->Print(cols[i] +
                 (i == args.GetLength() - 1 ? "\n" : sep));
        }
        }
        if (args.GetLength() == 0) {
        break;
        }
        for (i = 0; i < args.GetLength(); i++) {
        mPw->Print(args[i] == NULL ? "NULL" : args[i]);
        if (mode == IShell_MODE_Semi) {
            mPw->Print(";");
        } else if (i < args.GetLength() - 1) {
            mPw->Print(sep);
        }
        }
        mPw->Println("");
        break;
    case IShell_MODE_Html:
        if (mCount++ == 0 && mShowHeader) {
            mPw->Print("<TR>");
        for (i = 0; i < args.GetLength(); i++) {
            mPw->Print("<TH>" + html_quote(cols[i]) + "</TH>");
        }
        mPw->Println("</TR>");
        }
        if (args.GetLength() == 0) {
        break;
        }
        mPw->Print("<TR>");
        for (i = 0; i < args.GetLength(); i++) {
        mPw->Print("<TD>" + html_quote(args[i]) + "</TD>");
        }
        mPw->Println("</TR>");
        break;
    case IShell_MODE_Insert:
        if (args.GetLength() == 0) {
        break;
        }
        tname = mTableName;
        if (!mDestTable.IsNull()) {
            tname = mDestTable;
        }
        mPw->Print("INSERT INTO " + tname + " VALUES(");
        for (i = 0; i < args.GetLength(); i++) {
            String tsep = i > 0 ? "," : "";
        if (args[i] == NULL) {
            mPw->Print(tsep + "NULL");
        } else if (is_numeric(args[i])) {
            mPw->Print(tsep + args[i]);
        } else {
            mPw->Print(tsep + sql_quote(args[i]));
        }
        }
        mPw->Println(");");
        break;
    case IShell_MODE_Insert2:
        if (args.GetLength() == 0) {
        break;
        }
        tname = mTableName;
        if (!mDestTable.IsNull()) {
            tname = mDestTable;
        }
        mPw->Print("INSERT INTO " + tname + " VALUES(");
        for (i = 0; i < args.GetLength(); i++) {
            String tsep = i > 0 ? "," : "";
        mPw->Print(tsep + args[i]);
        }
        mPw->Println(");");
        break;
    }
    *result = FALSE;*/
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
{/*
    this.s = s;
    s.pw.println("BEGIN TRANSACTION;");
        if (tables == null || tables.length == 0) {
        try {
            s.db.exec("SELECT name, type, sql FROM sqlite_master " +
              "WHERE type!='meta' AND sql NOT NULL " +
              "ORDER BY substr(type,2,1), name", this);
        } catch (Exception e) {
            s.err.println("SQL Error: " + e);
        s.err.flush();
        }
    } else {
        String arg[] = new String[1];
        for (int i = 0; i < tables.length; i++) {
            arg[0] = tables[i];
        try {
            s.db.exec("SELECT name, type, sql FROM sqlite_master " +
                  "WHERE tbl_name LIKE '%q' AND type!='meta' " +
                  " AND sql NOT NULL " +
                  " ORDER BY substr(type,2,1), name",
                  this, arg);
        } catch (Exception e) {
            s.err.println("SQL Error: " + e);
            s.err.flush();
        }
        }
    }
    s.pw.println("COMMIT;");*/
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
    assert(result != NULL);/*
    if (args.length != 3) {
        return true;
    }
    s.pw.println(args[2] + ";");
    if (args[1].compareTo("table") == 0) {
        Shell s2 = (Shell) s.clone();
        s2.mode = Shell.MODE_Insert;
        s2.set_table_name(args[0]);
        String qargs[] = new String[1];
        qargs[0] = args[0];
        try {
            if (s2.db.is3()) {
            TableResult t = null;
            t = s2.db.get_table("PRAGMA table_info('%q')", qargs);
            String query;
            if (t != null) {
                StringBuffer sb = new StringBuffer();
            String sep = "";

            sb.append("SELECT ");
            for (int i = 0; i < t.nrows; i++) {
                String col = ((String[]) t.rows.elementAt(i))[1];
                sb.append(sep + "quote(" +
                      Shell.sql_quote_dbl(col) + ")");
                sep = ",";
            }
            sb.append(" from '%q'");
            query = sb.toString();
            s2.mode = Shell.MODE_Insert2;
            } else {
                query = "SELECT * from '%q'";
            }
            s2.db.exec(query, s2, qargs);
        } else {
            s2.db.exec("SELECT * from '%q'", s2, qargs);
        }
        } catch (Exception e) {
            s.err.println("SQL Error: " + e);
        s.err.flush();
        return true;
        }
    }
    return false;*/
    return NOERROR;
}