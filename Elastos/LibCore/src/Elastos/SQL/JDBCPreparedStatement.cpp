
#include "JDBCPreparedStatement.h"
#include "cmdef.h"

const Boolean JDBCPreparedStatement::mNullrepl = SQLite.Database.version().compareTo("2.5.0") < 0;

BatchArg::BatchArg(
    /* [in] */String arg, 
    /* [in] */Boolean blob)
{
    if (arg == NULL) {
        mArg = null;
    } else {
        mArg = new String(arg);
    }
    mBlob = blob;
}

JDBCPreparedStatement::JDBCPreparedStatement()
{

}

JDBCPreparedStatement::~JDBCPreparedStatement()
{

}

String Fixup(
    /* [in] */String sql)
{/*
    StringBuffer sb = new StringBuffer();
    boolean inq = false;
    int nparm = 0;
    for (int i = 0; i < sql.length(); i++) {
        char c = sql.charAt(i);
        if (c == '\'') {
        if (inq) {
                    char nextChar = 0;
                    if(i + 1 < sql.length()) {
                        nextChar = sql.charAt(i + 1);
                    }
            if (nextChar == '\'') {
                        sb.append(c);
                        sb.append(nextChar);
                        i++;
                    } else {
            inq = false;
                        sb.append(c);
                    }
        } else {
            inq = true;
                    sb.append(c);
        }
        } else if (c == '?') {
        if (inq) {
            sb.append(c);
        } else {
            ++nparm;
            sb.append(nullrepl ? "'%q'" : "%Q");
        }
        } else if (c == ';') {
        if (!inq) {
            break;
        }
        sb.append(c);
        } else if (c == '%') {
        sb.append("%%");
        } else {
        sb.append(c);
        }
    }
    args = new String[nparm];
    blobs = new boolean[nparm];
    try {
        clearParameters();
    } catch (SQLException e) {
    }
    return sb.toString();*/
}

String Fixup2(
    /* [in] */String sql)
{/*
    if (!conn.db.is3()) {
        return sql;
    }
    StringBuffer sb = new StringBuffer();
    int parm = -1;
    for (int i = 0; i < sql.length(); i++) {
        char c = sql.charAt(i);
        if (c == '%') {
        sb.append(c);
        ++i;
        c = sql.charAt(i);
        if (c == 'Q') {
            parm++;
            if (blobs[parm]) {
            c = 's';
            }
        }
        }
        sb.append(c);
    }
    return sb.toString();*/
}

ECode JDBCPreparedStatement::Init(
    /* [in] */IJDBCConnection* conn, 
    /* [in] */String sql)
{
    super(conn);
    mArgs = null;
    mBlobs = null;
    mBatch = null;
    mSql = fixup(sql);
    return NOERROR;
}

ECode JDBCPreparedStatement::ExecuteQuery(
    /* [out] */IResultSet** query)
{
    //return executeQuery(fixup2(sql), args, false);
}

ECode JDBCPreparedStatement::ExecuteUpdate(
    /* [out] */Int32* date)
{/*
    executeQuery(fixup2(sql), args, true);
    return updcnt;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetNull(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Int32 sqlType)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = nullrepl ? "" : null;
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetBoolean(
    /* [in] */Int32 parameterIndex,
    /* [in] */Boolean x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = x ? "1" : "0";
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetByte(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Byte x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = "" + x;
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetShort(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Int16 x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = "" + x;
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetInt(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Int32 x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = "" + x;
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetLong(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Int64 x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = "" + x;
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetFloat(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Float x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = "" + x;
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetDouble(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Int64 x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    args[parameterIndex - 1] = "" + x;
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetBigDecimal(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IBigDecimal* x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        args[parameterIndex - 1] = "" + x;
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetString(
    /* [in] */Int32 parameterIndex, 
    /* [in] */String x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        args[parameterIndex - 1] = x;
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetBytes(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ArrayOf<Byte> x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    blobs[parameterIndex - 1] = false;
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        if (conn.db.is3()) {
        args[parameterIndex - 1] = SQLite.StringEncoder.encodeX(x);
        blobs[parameterIndex - 1] = true;
        } else {
        args[parameterIndex - 1] = SQLite.StringEncoder.encode(x);
        }
    }*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetDate(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IDate* x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        if (conn.useJulian) {
        args[parameterIndex - 1] = java.lang.Double.toString(SQLite.Database.julian_from_long(x.getTime()));
        } else {
        args[parameterIndex - 1] = x.toString();
        }
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetTime(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ITime* x) 
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        if (conn.useJulian) {
        args[parameterIndex - 1] = java.lang.Double.toString(SQLite.Database.julian_from_long(x.getTime()));
        } else {
        args[parameterIndex - 1] = x.toString();
        }
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetTimestamp(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ITimestamp* x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        if (conn.useJulian) {
        args[parameterIndex - 1] = java.lang.Double.toString(SQLite.Database.julian_from_long(x.getTime()));
        } else {
        args[parameterIndex - 1] = x.toString();
        }
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetAsciiStream(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x,
    /* [in] */Int32 length)
{
    return E_SQL_EXCEPTION;
}

//@Deprecated
ECode JDBCPreparedStatement::SetUnicodeStream(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x, 
    /* [in] */Int32 length)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBinaryStream(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x,
    /* [in] */Int32 length)
{/*
    try {
        byte[] data = new byte[length];
        x.read(data, 0, length);
        setBytes(parameterIndex, data);
    } catch (java.io.IOException e) {
        throw new SQLException("I/O failed");
    }*/
    return NOERROR;
}

ECode JDBCPreparedStatement::ClearParameters()
{/*
    for (int i = 0; i < args.length; i++) {
        args[i] = nullrepl ? "" : null;
        blobs[i] = false;
    }*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetObject(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInterface* x, 
    /* [in] */Int32 targetSqlType,
    /* [in] */Int32 scale)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        if (x instanceof byte[]) {
        byte[] bx = (byte[]) x;
        if (conn.db.is3()) {
            args[parameterIndex - 1] =
              SQLite.StringEncoder.encodeX(bx);
            blobs[parameterIndex - 1] = true;
            return;
        }
        args[parameterIndex - 1] = SQLite.StringEncoder.encode(bx);
        } else {
        args[parameterIndex - 1] = x.toString();
        }
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetObjectEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInterface* x,
    /* [in] */Int32 targetSqlType)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        if (x instanceof byte[]) {
        byte[] bx = (byte[]) x;
        if (conn.db.is3()) {
            args[parameterIndex - 1] =
            SQLite.StringEncoder.encodeX(bx);
            blobs[parameterIndex - 1] = true;
            return;
        }
        args[parameterIndex - 1] = SQLite.StringEncoder.encode(bx);
        } else {
        args[parameterIndex - 1] = x.toString();
        }
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetObjectEx2(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInterface* x)
{/*
    if (parameterIndex < 1 || parameterIndex > args.length) {
        throw new SQLException("bad parameter index");
    }
    if (x == null) {
        args[parameterIndex - 1] = nullrepl ? "" : null;
    } else {
        if (x instanceof byte[]) {
        byte[] bx = (byte[]) x;
        if (conn.db.is3()) {
            args[parameterIndex - 1] =
            SQLite.StringEncoder.encodeX(bx);
            blobs[parameterIndex - 1] = true;
            return;
        }
        args[parameterIndex - 1] = SQLite.StringEncoder.encode(bx);
        } else {
        args[parameterIndex - 1] = x.toString();
        }
    }
    blobs[parameterIndex - 1] = false;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::Execute(
    /* [out] */Boolean* result)
{
    //return executeQuery(fixup2(sql), args, false) != null;
}

ECode JDBCPreparedStatement::AddBatch()
{/*
    if (batch == null) {
        batch = new ArrayList<BatchArg>(args.length);
    }
    for (int i = 0; i < args.length; i++) {
        batch.add(new BatchArg(args[i], blobs[i]));
    }*/
    return NOERROR;
}

ECode JDBCPreparedStatement::ExecuteBatch(
    /* [out] */ArrayOf<Int32>* batch)
{/*
    if (batch == null) {
        return new int[0];
    }
    int[] ret = new int[batch.size() / args.length];
    for (int i = 0; i < ret.length; i++) {
        ret[i] = EXECUTE_FAILED;
    }
    int errs = 0;
    int index = 0;
    for (int i = 0; i < ret.length; i++) {
        for (int k = 0; k < args.length; k++) {
        BatchArg b = (BatchArg) batch.get(index++);

        args[k] = b.arg;
        blobs[k] = b.blob;
        }
        try {
        ret[i] = executeUpdate();
        } catch (SQLException e) {
        ++errs;
        }
    }
    if (errs > 0) {
        throw new BatchUpdateException("batch failed", ret);
    }
    return ret;*/
    return NOERROR;
}

ECode JDBCPreparedStatement::ClearBatch()
{/*
    if (batch != null) {
        batch.clear();
        batch = null;
    }*/
    return NOERROR;
}

ECode JDBCPreparedStatement::Close()
{/*
    clearBatch();
    super.close();*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetCharacterStream(
    /* [in] */Int32 parameterIndex,
    /* [in] */IReader* reader,
    /* [in] */Int32 length)
{/*
    try {
        char[] data = new char[length];
        reader.read(data);
        setString(parameterIndex, new String(data));
    } catch (java.io.IOException e) {
        throw new SQLException("I/O failed");
    }*/
    return NOERROR;
}

ECode JDBCPreparedStatement::SetRef(
    /* [in] */Int32 i, 
    /* [in] */IRef* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBlob(
    /* [in] */Int32 i, 
    /* [in] */IBlob* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetClob(
    /* [in] */Int32 i, 
    /* [in] */IClob* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetArray(
    /* [in] */Int32 i, 
    /* [in] */IArray* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::GetMetaData(
    /* [out] */IResultSetMetaData** data)
{
    //return rs.getMetaData();
    return NOERROR;
}

ECode JDBCPreparedStatement::SetDateEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IDate* x, 
    /* [in] */ICalendar* cal)
{
    //setDate(parameterIndex, x);
    return NOERROR;
}

ECode JDBCPreparedStatement::SetTimeEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ITime* x, 
    /* [in] */ICalendar* cal)
{
    //setTime(parameterIndex, x);
    return NOERROR;
}

ECode JDBCPreparedStatement::SetTimestampEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ITimestamp* x,
    /* [in] */ICalendar* cal)
{
    //setTimestamp(parameterIndex, x);
    return NOERROR;
}

ECode JDBCPreparedStatement::SetNullEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */Int32 sqlType, 
    /* [in] */String typeName)
{
    //setNull(parameterIndex, sqlType);
    return NOERROR;
}

ECode JDBCPreparedStatement::GetParameterMetaData(
    /* [out] */IParameterMetaData** data)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::RegisterOutputParameter(
    /* [in] */String parameterName, 
    /* [in] */Int32 sqlType)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::RegisterOutputParameterEx(
    /* [in] */String parameterName, 
    /* [in] */Int32 sqlType,
    /* [in] */Int32 scale)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::RegisterOutputParameterEx2(
    /* [in] */String parameterName,
    /* [in] */ Int32 sqlType,
    /* [in] */String typeName)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetURL(
    /* [in] */Int32 parameterIndex,
    /* [out] */IURL** url)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetURL(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IURL* url)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNullEx2(
    /* [in] */String parameterName, 
    /* [in] */Int32 sqlType)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBooleanEx(
    /* [in] */String parameterName, 
    /* [in] */Boolean val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetByteEx(
    /* [in] */String parameterName,
    /* [in] */ Byte val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetShortEx(
    /* [in] */String parameterName, 
    /* [in] */Int16 val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetIntEx(
    /* [in] */String parameterName, 
    /* [in] */Int32 val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetLongEx(
    /* [in] */String parameterName, 
    /* [in] */Int64 val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetFloatEx(
    /* [in] */String parameterName, 
    /* [in] */Float val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetDoubleEx(
    /* [in] */String parameterName, 
    /* [in] */Double val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBigDecimalEx(
    /* [in] */String parameterName, 
    /* [in] */IBigDecimal* val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetStringEx(
    /* [in] */String parameterName, 
    /* [in] */String val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBytesEx(
    /* [in] */String parameterName, 
    /* [in] */ArrayOf<Byte> val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetDateEx(
    /* [in] */String parameterName, 
    /* [in] */IDate* val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetTimeEx(
    /* [in] */String parameterName, 
    /* [in] */ITime* val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetTimestampEx(
    /* [in] */String parameterName, 
    /* [in] */ITimestamp* val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetAsciiStreamEx(
    /* [in] */String parameterName,
    /* [in] */IInputStream* s, 
    /* [in] */Int32 length)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBinaryStreamEx(
    /* [in] */String parameterName,
    /* [in] */IInputStream* s, 
    /* [in] */Int32 length)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetObjectEx3(
    /* [in] */String parameterName, 
    /* [in] */IInterface* val, 
    /* [in] */Int32 targetSqlType,
    /* [in] */Int32 scale)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetObjectEx4(
    /* [in] */String parameterName, 
    /* [in] */IInterface* val, 
    /* [in] */Int32 targetSqlType)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetObjectEx5(
    /* [in] */String parameterName, 
    /* [in] */IInterface* val)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetCharacterStreamEx(
    /* [in] */String parameterName,
    /* [in] */IReader* r, 
    /* [in] */Int32 length)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetDateEx2(
    /* [in] */String parameterName, 
    /* [in] */IDate* val,
    /* [in] */ICalendar* cal)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetTimeEx2(
    /* [in] */String parameterName, 
    /* [in] */ITime* val,
    /* [in] */ICalendar* cal)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetTimestampEx2(
    /* [in] */String parameterName, 
    /* [in] */ITimestamp* val,
    /* [in] */ICalendar* cal)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNullEx3(
    /* [in] */String parameterName, 
    /* [in] */Int32 sqlType, 
    /* [in] */String typeName)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetString(
    /* [in] */String parameterName,
    /* [out] */String* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetBoolean(
    /* [in] */String parameterName,
    /* [out] */Boolean* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetByte(
    /* [in] */String parameterName,
    /* [out] */Byte* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetShort(
    /* [in] */String parameterName,
    /* [out] */Int16* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetInt(
    /* [in] */String parameterName,
    /* [out] */Int32* result) 
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetLong(
    /* [in] */String parameterName,
    /* [out] */Int64* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetFloat(
    /* [in] */String parameterName,
    /* [out] */Float* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetDouble(
    /* [in] */String parameterName,
    /* [out] */Double* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetBytes(
    /* [in] */String parameterName,
    /* [out] */ArrayOf<Byte>* bytes)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetDate(
    /* [in] */String parameterName,
    /* [out] */IDate** date)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetTime(
    /* [in] */String parameterName,
    /* [out] */ITime** time)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetTimestamp(
    /* [in] */String parameterName,
    /* [out] */ITimestamp** stamp)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetObject(
    /* [in] */String parameterName,
    /* [out] */IInterface** obj)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetObjectEx(
    /* [in] */Int32 parameterIndex,
    /* [out] */IInterface** obj)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetBigDecimal(
    /* [in] */String parameterName,
    /* [out] */IBigDecimal** result)
{
    return E_SQL_EXCEPTION;
}

//    Object GetObjectEx2(
//        /* [in] */String parameterName, 
//        /* [in] */Map map){}

//    Object GetObjectEx3(
//        /* [in] */Int32 parameterIndex, 
//       /* [in] */Map map){}

ECode JDBCPreparedStatement::GetRef(
    /* [in] */Int32 parameterIndex,
    /* [out] */IRef** ref)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetRefEx(
    /* [in] */String parameterName,
    /* [out] */IRef** ref)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetBlob(
    /* [in] */String parameterName,
    /* [out] */IBlob** blob)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetBlobEx(
    /* [in] */Int32 parameterIndex,
    /* [out] */IBlob** blob)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetClob(
    /* [in] */String parameterName,
    /* [out] */IClob** clob)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetClobEx(
    /* [in] */Int32 parameterIndex,
    /* [out] */IClob** clob)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetArray(
    /* [in] */String parameterName,
    /* [out] */IArray** array)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetArrayEx(
    /* [in] */Int32 parameterIndex,
    /* [out] */IArray** array)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetDate(
    /* [in] */String parameterName, 
    /* [in] */ICalendar* cal,
    /* [out] */IDate** date)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetDateEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ICalendar* cal,
    /* [out] */IDate** date)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetTime(
    /* [in] */String parameterName, 
    /* [in] */ICalendar* cal,
    /* [out] */ITime** time)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetTimeEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ICalendar* cal,
    /* [out] */ITime** time)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetTimestamp(
    /* [in] */String parameterName,
    /* [in] */ ICalendar* cal,
    /* [out] */ITimestamp** tamp)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetTimestampEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ICalendar* cal.
    /* [out] */ITimestamp** tamp)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::GetURL(
    /* [in] */String parameterName,
    /* [out] */IURL** url)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCPreparedStatement::SetRowId(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IRowId* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetRowId(
    /* [in] */String parameterName, 
    /* [in] */IRowId* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNString(
    /* [in] */Int32 parameterIndex, 
    /* [in] */String value)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNString(
    /* [in] */String parameterName, 
    /* [in] */String value)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNCharacterStream(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNCharacterStreamEx(
    /* [in] */String parameterName, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNClob(
    /* [in] */Int32 parameterIndex, 
    /* [in] */INClobI value)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNClobEx(
    /* [in] */String parameterName, 
    /* [in] */INClob* value)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetClobEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReader* x, 
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetClobEx2(
    /* [in] */String parameterName, 
    /* [in] */IReader* x, 
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBlobEx(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x, 
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBlobEx2(
    /* [in] */String parameterName, 
    /* [in] */IInputStream* x, 
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNClobEx2(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReaderI x, 
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNClobEx3(
    /* [in] */String parameterName,
    /* [in] */IReader* x, 
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetSQLXML(
    /* [in] */Int32 parameterIndex, 
    /* [in] */ISQLXML* xml)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetSQLXMLEx(
    /* [in] */String parameterName, 
    /* [in] */ISQLXML* xml)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetAsciiStreamEx2(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetAsciiStreamEx3(
    /* [in] */String parameterName, 
    /* [in] */IInputStreamO x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBinaryStreamEx2(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBinaryStreamEx3(
    /* [in] */String parameterName, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetCharacterStreamEx2(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReaderO x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetCharacterStreamEx3(
    /* [in] */String parameterName,
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetAsciiStreamEx4(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetAsciiStreamEx5(
    /* [in] */String parameterName, 
    /* [in] */IInputStreamO x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBinaryStreamEx4(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBinaryStreamEx5(
    /* [in] */String parameterName, 
    /* [in] */IInputStream* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetCharacterStreamEx4(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetCharacterStreamEx5(
    /* [in] */String parameterName,
    /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNCharacterStream(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNCharacterStreamEx(
    /* [in] */String parameterName,
     /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetClobEx3(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetClobEx4(
    /* [in] */String parameterName, 
    /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBlobEx3(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IInputStream* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetBlobEx4(
    /* [in] */String parameterName,
    /* [in] */IInputStream* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNClobEx4(
    /* [in] */Int32 parameterIndex, 
    /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCPreparedStatement::SetNClobEx5(
    /* [in] */String parameterName, 
    /* [in] */IReader* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}