
#include "JDBCResultSet.h"
#include "cmdef.h"

const Int32 JDBCResultSet::UPD_UNKNOWN = -1;
const Int32 JDBCResultSet::UPD_NO = 0;
const Int32 JDBCResultSet::UPD_INS = 1;
const Int32 JDBCResultSet::UPD_INSUPDDEL = 2;
const Boolean JDBCResultSet::mNullrepl = SQLite.Database.version().compareTo("2.5.0") < 0;

JDBCResultSet::JDBCResultSet():mTr(NULL),ms(NULL), md(NULL) {
}

JDBCResultSet::~JDBCResultSet() {
}

Integer JDBCResultSet::InternalGetInt(
    /* [in] */Int32 columnIndex)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        return Integer.valueOf(lastg);
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

Int16 JDBCResultSet::InternalGetShort(
    /* [in] */Int32 columnIndex)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        return Short.valueOf(lastg);
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

ECode JDBCResultSet::InternalGetTime(
    /* [in] */Int32 columnIndex,
    /* [in] */ICalendar* cal
    /* [out] */ITime** time)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        if (s.conn.useJulian) {
        try {
            return new java.sql.Time(SQLite.Database.long_from_julian(lastg));
        } catch (java.lang.Exception ee) {
            return java.sql.Time.valueOf(lastg);
        }
        } else {
        try {
            return java.sql.Time.valueOf(lastg);
        } catch (java.lang.Exception ee) {
            return new java.sql.Time(SQLite.Database.long_from_julian(lastg));
        }
        }
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

ECode JDBCResultSet::InternalGetTimestamp(
    /* [in] */Int32 columnIndex,
    /* [in] */ICalendar* cal,
    /* [out] */ITimestamp** stamp)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        if (s.conn.useJulian) {
        try {
            return new java.sql.Timestamp(SQLite.Database.long_from_julian(lastg));
        } catch (java.lang.Exception ee) {
            return java.sql.Timestamp.valueOf(lastg);
        }
        } else {
        try {
            return java.sql.Timestamp.valueOf(lastg);
        } catch (java.lang.Exception ee) {
            return new java.sql.Timestamp(SQLite.Database.long_from_julian(lastg));
        }
        }
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

ECode JDBCResultSet::InternalGetDate(
    /* [in] */Int32 columnIndex,
    /* [in] */ICalendar* cal,
    /* [out] */IDate** date)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        if (s.conn.useJulian) {
        try {
            return new java.sql.Date(SQLite.Database.long_from_julian(lastg));
        } catch (java.lang.Exception ee) {
            return java.sql.Date.valueOf(lastg);
        }
        } else {
        try {
            return java.sql.Date.valueOf(lastg);
        } catch (java.lang.Exception ee) {
            return new java.sql.Date(SQLite.Database.long_from_julian(lastg));
        }
        }
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

Int64 JDBCResultSet::InternalGetDouble(
    /* [in] */Int32 columnIndex)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        return Double.valueOf(lastg);
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

Float JDBCResultSet::InternalGetFloat(
    /* [in] */Int32 columnIndex)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        return Float.valueOf(lastg);
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

Int64 JDBCResultSet::InternalGetLong(
    /* [in] */Int32 columnIndex)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    try {
        return Long.valueOf(lastg);
    } catch (java.lang.Exception e) {
        lastg = null;
    }
    return null;*/
    return NOERROR;
}

ECode JDBCResultSet::Init(
    /* [in] */ITableResult* tr, 
    /* [in] */IJDBCStatement* s)
{
    mTr = tr;
    ms = s;
    md = NULL;
    mLastg = NULL;
    mRow = -1;
    mUpdatable = UPD_UNKNOWN;
    mOninsrow = FALSE;
    mRowbuf = NULL;
    return NOERROR;
}

ECode JDBCResultSet::IsUpdatable(
    /* [out] */Boolean* result)
{/*
    if (updatable == UPD_UNKNOWN) {
        try {
        JDBCResultSetMetaData m =
            (JDBCResultSetMetaData) getMetaData();
        java.util.HashSet<String> h = new java.util.HashSet<String>();
        String lastt = null;
        for (int i = 1; i <= tr.ncolumns; i++) {
            lastt = m.getTableName(i);
            h.add(lastt);
        }
        if (h.size() > 1 || lastt == null) {
            updatable = UPD_NO;
            throw new SQLException("view or join");
        }
        updatable = UPD_INS;
        uptable = lastt;
        JDBCResultSet pk = (JDBCResultSet)
            s.conn.getMetaData().getPrimaryKeys(null, null, uptable);
        if (pk.tr.nrows > 0) {
            boolean colnotfound = false;
            pkcols = new String[pk.tr.nrows];
            pkcoli = new int[pk.tr.nrows];
            for (int i = 0; i < pk.tr.nrows; i++) {
            String rd[] = (String []) pk.tr.rows.elementAt(i);
            pkcols[i] = rd[3];
            try {
                pkcoli[i] = findColumn(pkcols[i]) - 1;
            } catch (SQLException ee) {
                colnotfound = true;
            }
            }
            if (!colnotfound) {
            updatable = UPD_INSUPDDEL;
            }
        }
        pk.close();
        } catch (SQLException e) {
        updatable = UPD_NO;
        }
    }
    if (updatable < UPD_INS) {
        throw new SQLException("result set not updatable");
    }
    return true;*/
    return NOERROR;
}

ECode JDBCResultSet::FillRowbuf()
{/*
    if (rowbuf == null) {
        if (row < 0) {
        throw new SQLException("cursor outside of result set");
        }
        rowbuf = new String[tr.ncolumns];
        System.arraycopy((String []) tr.rows.elementAt(row), 0,
                 rowbuf, 0, tr.ncolumns);
    }*/
    return NOERROR;
}

ECode JDBCResultSet::Next(
    /* [out] */Boolean* result)
{/*
    if (tr == null) {
        return false;
    }
    row++;
    return row < tr.nrows;*/
    return NOERROR;
}

ECode JDBCResultSet::FindColumn(
    /* [in] */String columnName,
    /* [out] */Int32* column)
{/*
    JDBCResultSetMetaData m = (JDBCResultSetMetaData) getMetaData();
    return m.findColByName(columnName);*/
    return NOERROR;
}

ECode JDBCResultSet::GetRow(
    /* [out] */Int32* row)
{/*
    if (tr == null) {
        throw new SQLException("no rows");
    }
    return row + 1;*/
    return NOERROR;
}

ECode JDBCResultSet::Previous(
    /* [out] */Boolean* result)
{/*
    if (tr == null) {
        throw new SQLException("result set already closed.");
    }
    if (row >= 0) {
        row--;
    }
    return row >= 0;*/
    return NOERROR;
}

ECode JDBCResultSet::Absolute(
    /* [in] */Int32 row,
    /* [out] */Boolean* result)
{/*
    if (tr == null) {
        return false;
    }
    if (row < 0) {
        row = tr.nrows + 1 + row;
    }
    row--;
    if (row < 0 || row > tr.nrows) {
        return false;
    }
    this.row = row;
    return true;*/
    return NOERROR;

}

ECode JDBCResultSet::Relative(
    /* [in] */Int32 row,
    /* [out] */Boolean* result)
{/*
    if (tr == null) {
        return false;
    }
    if (this.row + row < 0 || this.row + row >= tr.nrows) {
        return false;
    }
    this.row += row;
    return true;*/
    return NOERROR;
}

ECode JDBCResultSet::SetFetchDirection(
    /* [in] */Int32 dir)
{/*
    if (dir != ResultSet.FETCH_FORWARD) {
        throw new SQLException("only forward fetch direction supported");
    }*/
    return NOERROR;
}

ECode JDBCResultSet::GetFetchDirection(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = ResultSet.FETCH_FORWARD;
    return NOERROR;
}

ECode JDBCResultSet::SetFetchSize(
    /* [in] */Int32 fsize)
{
    if (fsize != 1) {
        //throw new SQLException("fetch size must be 1");
        return E_SQL_EXCEPTION;
    }
    return NOERROR;
}
}

ECode JDBCResultSet::GetFetchSize(
    /* [out] */Int32* size)
{
    assert(size != NULL);
    *size = 1;
    return NOERROR;
}

ECode JDBCResultSet::GetString(
    /* [in] */Int32 columnIndex
    /* [out] */String* str)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    return lastg;*/
    return NOERROR;
}

ECode JDBCResultSet::GetStringEx(
    /* [in] */String columnName,
    /* [out] */String* str)
{/*
    int col = findColumn(columnName);
    return getString(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetInt(
    /* [in] */Int32 columnIndex,
    /* [out] */Int32* result)
{/*
    Integer i = internalGetInt(columnIndex);
    if (i != null) {
        return i.intValue();
    }
    return 0;*/
    return NOERROR;
}

ECode JDBCResultSet::GetIntEx(
    /* [in] */String columnName,
    /* [out] */Int32* result)
{/*
    int col = findColumn(columnName);
    return getInt(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetBoolean(
    /* [in] */Int32 columnIndex,
    /* [out] */Boolean* result)
{/*
    return getInt(columnIndex) == 1 ||
        Boolean.parseBoolean(getString(columnIndex));*/
    return NOERROR;
}

ECode JDBCResultSet::GetBooleanEx(
    /* [in] */String columnName,
    /* [out] */Boolean* result)
{/*
    int col = findColumn(columnName);
    return getBoolean(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetMetaData(
    /* [out] */IResultSetMetaData** data)
{/*
    if (md == null) {
        md = new JDBCResultSetMetaData(this);
    }
    return md;*/
    return NOERROR;
}

ECode JDBCResultSet::GetShort(
    /* [in] */Int32 columnIndex,
    /* [out] */Int16* result)
{/*
    Short sh = internalGetShort(columnIndex);
    if (sh != null) {
        return sh.shortValue();
    }
    return 0;*/
    return NOERROR;
}

ECode JDBCResultSet::GetShortEx(
    /* [in] */String columnName,
    /* [out] */Int16* result)
{/*
    int col = findColumn(columnName);
    return getShort(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetTime(
    /* [in] */Int32 columnIndex,
    /* [out] */ITime** time)
{
    //return internalGetTime(columnIndex, null);
    return NOERROR;
}

ECode JDBCResultSet::GetTimeEx(
    /* [in] */String columnName,
    /* [out] */ITime** time)
{/*
    int col = findColumn(columnName);
    return getTime(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetTimeEx2(
    /* [in] */Int32 columnIndex, 
    /* [in] */ICalendar* cal,
    /* [out] */ITime** time)
{
    //return internalGetTime(columnIndex, cal);
    return NOERROR;
}

ECode JDBCResultSet::GetTimeEx3(
    /* [in] */String columnName, 
    /* [in] */ICalendar* cal,
    /* [out] */ITime** time)
{/*
    int col = findColumn(columnName);
    return getTime(col, cal);*/
    return NOERROR;
}

ECode JDBCResultSet::GetTimestamp(
    /* [in] */Int32 columnIndex,
    /* [out] */ITimestamp** stamp)
{
    //return internalGetTimestamp(columnIndex, null);
    return NOERROR;
}

ECode JDBCResultSet::GetTimestampEx(
    /* [in] */String columnName,
    /* [out] */ITimestamp** stamp)
{/*
    int col = findColumn(columnName);
    return getTimestamp(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetTimestampEx2(
    /* [in] */Int32 columnIndex,
    /* [in] */ICalendar* cal,
    /* [out] */ITimestamp** stamp)
{
    //return internalGetTimestamp(columnIndex, cal);
    return NOERROR;
}

ECode JDBCResultSet::GetTimestampEx3(
    /* [in] */String columnName,
    /* [in] */ICalendar* cal,
    /* [out] */ITimestamp** stamp)
{/*
    int col = findColumn(columnName);
    return getTimestamp(col, cal);*/
    return NOERROR;
}

ECode JDBCResultSet::GetDate(
    /* [in] */Int32 columnIndex,
    /* [out] */IDate** date)
{
    //return internalGetDate(columnIndex, null);
    return NOERROR;
}

ECode JDBCResultSet::GetDateEx(
    /* [in] */String columnName,
    /* [out] */IDate** date)
{/*
    int col = findColumn(columnName);
    return getDate(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetDateEx2(
    /* [in] */Int32 columnIndex, 
    /* [in] */ICalendar* cal,
    /* [out] */IDate** date)
{
    //return internalGetDate(columnIndex, cal);
    return NOERROR;
}

ECode JDBCResultSet::GetDateEx3(
    /* [in] */String columnName, 
    /* [in] */ICalendar* cal,
    /* [out] */IDate** date)
{/*
    int col = findColumn(columnName);
    return getDate(col, cal);*/
    return NOERROR;
}

ECode JDBCResultSet::GetDouble(
    /* [in] */Int32 columnIndex,
    /* [out] */Int64* result)
{/*
    Double d = internalGetDouble(columnIndex);
    if (d != null) {
        return d.doubleValue();
    }
    return 0;*/
    return NOERROR;
}

ECode JDBCResultSet::GetDoubleEx(
    /* [in] */String columnName,
    /* [out] */Int64* result)
{/*
    int col = findColumn(columnName);
    return getDouble(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetFloat(
    /* [in] */Int32 columnIndex,
    /* [out] */Float* result)
{/*
    Float f = internalGetFloat(columnIndex);
    if (f != null) {
        return f.floatValue();
    }
    return 0;*/
    return NOERROR;
}

ECode JDBCResultSet::GetFloatEx(
    /* [in] */String columnName,
    /* [out] */Float* result)
{/*
    int col = findColumn(columnName);
    return getFloat(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetLong(
    /* [in] */Int32 columnIndex,
    /* [out] */Int64* result)
{/*
    Long l = internalGetLong(columnIndex);
    if (l != null) {
        return l.longValue();
    }
    return 0;*/
    return NOERROR;
}

ECode JDBCResultSet::GetLongEx(
    /* [in] */String columnName,
    /* [out] */Int64* result)
{/*
    int col = findColumn(columnName);
    return getLong(col);*/
    return NOERROR;
}

//@Deprecated
ECode JDBCResultSet::GetUnicodeStream(
    /* [in] */Int32 columnIndex,
    /* [out] */IInputStream** stream)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR;
}

//@Deprecated
ECode JDBCResultSet::GetUnicodeStreamEx(
    /* [in] */String columnName,
    /* [out] */IInputStream** stream)
{/*
    int col = findColumn(columnName);
    return getUnicodeStream(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetAsciiStream(
    /* [in] */String columnName,
    /* [out] */IInputStream** stream)
{/*
    int col = findColumn(columnName);
    return getAsciiStream(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetAsciiStreamEx(
    /* [in] */Int32 columnIndex,
    /* [out] */IInputStream** stream)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCResultSet::GetBigDecimal(
    /* [in] */String columnName,
    /* [out] */IBigDecimal** result)
{/*
    int col = findColumn(columnName);
    return getBigDecimal(col);*/
    return NOERROR;
}

//@Deprecated
ECode JDBCResultSet::GetBigDecimalEx(
    /* [in] */String columnName, 
    /* [in] */Int32 scale,
    /* [out] */IBigDecimal** result)
{/*
    int col = findColumn(columnName);
    return getBigDecimal(col, scale);*/
    return NOERROR;
}

ECode JDBCResultSet::GetBigDecimalEx2(
    /* [in] */Int32 columnIndex,
    /* [out] */IBigDecimal** result)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR;
}

//@Deprecated
ECode JDBCResultSet::SetBigDecimalEx3(
    /* [in] */Int32 columnIndex, 
    /* [in] */Int32 scale,
    /* [out] */IBigDecimal** result)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR;
}

ECode JDBCResultSet::GetBinaryStream(
    /* [in] */Int32 columnIndex,
    /* [out] */IInputStream** stream)
{/*
    byte data[] = getBytes(columnIndex);
    if (data != null) {
        return new java.io.ByteArrayInputStream(data);
    }
    return null;*/
    return NOERROR;
}

ECode JDBCResultSet::GetBinaryStreamEx(
    /* [in] */String columnName,
    /* [out] */IInputStream** stream)
{/*
    byte data[] = getBytes(columnName);
    if (data != null) {
        return new java.io.ByteArrayInputStream(data);
    }
    return null;*/
    return NOERROR;
}

ECode JDBCResultSet::GetByte(
    /* [in] */Int32 columnIndex,
    /* [out] */Byte* result)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCResultSet::GetByteEx(
    /* [in] */String columnName,
    /* [out] */Byte* result)
{/*
    int col = findColumn(columnName);
    return getByte(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetBytes(
    /* [in] */Int32 columnIndex,
    /* [out,callee] */ArrayOf<Byte>* bt)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    byte ret[] = null;
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    if (lastg != null) {
        ret = SQLite.StringEncoder.decode(lastg);
    }
    return ret;*/
    return NOERROR;
}

ECode JDBCResultSet::GetBytesEx(
    /* [in] */String columnName,
    /* [out,callee] */ArrayOf<Byte>* bt)
{/*
    int col = findColumn(columnName);
    return getBytes(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetCursorName(
    /* [out] */String* name)
{
    assert(name != NULL);
    *name = NULL;
    return NOERROR;
}

ECode JDBCResultSet::GetObject(
    /* [in] */Int32 columnIndex,
    /* [out] */IInterface** obj)
{/*
    if (tr == null || columnIndex < 1 || columnIndex > tr.ncolumns) {
        throw new SQLException("column " + columnIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[columnIndex - 1];
    Object ret = lastg;
    if (tr instanceof TableResultX) {
        switch (((TableResultX) tr).sql_type[columnIndex - 1]) {
        case Types.SMALLINT:
        ret = internalGetShort(columnIndex);
        break;
        case Types.INTEGER:
        ret = internalGetInt(columnIndex);
        break;
        case Types.DOUBLE:
        ret = internalGetDouble(columnIndex);
        break;
        case Types.FLOAT:
        ret = internalGetFloat(columnIndex);
        break;
        case Types.BIGINT:
        ret = internalGetLong(columnIndex);
        break;
        case Types.BINARY:
        case Types.VARBINARY:
        case Types.LONGVARBINARY:
        ret = getBytes(columnIndex);
        break;
        case Types.NULL:
        ret = null;
        break;
        // defaults to String below
        }
    }
    return ret;*/
    return NOERROR;
}

ECode JDBCResultSet::GetObjectEx(
    /* [in] */String columnName,
    /* [out] */IInterface** obj)
{/*
    int col = findColumn(columnName);
    return getObject(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetObjectEx2(
    /* [in] */Int32 columnIndex, 
    /* [in] */java.util.Map map, ////////////////////////////////////
    /* [out] */IInterface** obj)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR;
}

ECode JDBCResultSet::GetObjectEx3(
    /* [in] */String columnName, 
    /* [in] */java.util.Map map, /////////////////////////////
    /* [out] */IInterface** obj)
{/*
    int col = findColumn(columnName);
    return getObject(col, map);*/
    return NOERROR;
}

ECode JDBCResultSet::GetRef(
    /* [in] */Int32 columnIndex,
    /* [out] */IRef** ref)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::GetRefEx(
    /* [in] */String columnName,
    /* [out] */IRef** ref)
{/*
    int col = findColumn(columnName);
    return getRef(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetBlob(
    /* [in] */Int32 columnIndex,
    /* [out] */IBlob** blob)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::GetBlobEx(
    /* [in] */String columnName,
    /* [out] */IBlob** blob)
{/*
    int col = findColumn(columnName);
    return getBlob(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetClob(
    /* [in] */Int32 columnIndex,
    /* [out] */IClob** clob)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::GetClobEx(
    /* [in] */String columnName,
    /* [out] */IClob** clob)
{/*
    int col = findColumn(columnName);
    return getClob(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetArray(
    /* [in] */Int32 columnIndex,
    /* [out] */IArray** array)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::GetArrayEx(
    /* [in] */String columnName,
    /* [out] */IArray** array)
{/*
    int col = findColumn(columnName);
    return getArray(col);*/
}

ECode JDBCResultSet::GetCharacterStream(
    /* [in] */Int32 columnIndex,
    /* [out] */IReader** stream)
{/*
    String data = getString(columnIndex);
    if (data != null) {
        char[] cdata = data.toCharArray();
        return new java.io.CharArrayReader(cdata);
    }
    return null;*/
    return NOERROR;
}

ECode JDBCResultSet::GetCharacterStreamEx(
    /* [in] */String columnName,
    /* [out] */IReader** stream)
{/*
    String data = getString(columnName);
    if (data != null) {
        char[] cdata = data.toCharArray();
        return new java.io.CharArrayReader(cdata);
    }
    return null;*/
    return NOERROR;
}

//   ECode JDBCResultSet::SQLWarning GetWarnings(){}

ECode JDBCResultSet::WasNull(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = mLastg == null;
    return NOERROR;
}

ECode JDBCResultSet::ClearWarnings()
{
    return E_SQL_EXCEPTION;
}

ECode JDBCResultSet::IsFirst(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    if (mTr == NULL) {
        *result = TRUE;
        return NOERROR;
    }
    *result = mRow == 0;
    return NOERROR;
}

ECode JDBCResultSet::IsBeforeFirst(
    /* [out] */Boolean* result)
{/*
    if (tr == null || tr.nrows <= 0) {
        return false;
    }
    return row < 0;*/
    return NOERROR;
}

ECode JDBCResultSet::BeforeFirst()
{/*
    if (tr == null) {
        return;
    }
    row = -1;*/
    return NOERROR;
}

ECode JDBCResultSet::First(
    /* [out] */Boolean* result)
{/*
    if (tr == null || tr.nrows <= 0) {
        return false;
    }
    row = 0;
    return true;*/
    return NOERROR;
}

ECode JDBCResultSet::IsAfterLast(
    /* [out] */Boolean* result)
{/*
    if (tr == null || tr.nrows <= 0) {
        return false;
    }
    return row >= tr.nrows;*/
    return NOERROR;
}

ECode JDBCResultSet::AfterLast()
{/*
    if (tr == null) {
        return;
    }
    row = tr.nrows;*/
    return NOERROR;
}

ECode JDBCResultSet::IsLast(
    /* [out] */Boolean* result)
{/*
    if (tr == null) {
        return true;
    }
    return row == tr.nrows - 1;*/
    return NOERROR;
}

ECode JDBCResultSet::Last(
    /* [out] */Boolean* result)
{/*
    if (tr == null || tr.nrows <= 0) {
        return false;
    }
    row = tr.nrows -1;
    return true;*/
    return NOERROR;
}

ECode JDBCResultSet::GetType(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = TYPE_SCROLL_SENSITIVE;
    return NOERROR;
}

ECode JDBCResultSet::GetConcurrency(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = CONCUR_UPDATABLE;
    return NOERROR;
}

ECode JDBCResultSet::RowUpdated(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCResultSet::RowInserted(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCResultSet::RowDeleted(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode JDBCResultSet::InsertRow()
{/*
    isUpdatable();
    if (!oninsrow || rowbuf == null) {
        throw new SQLException("no insert data provided");
    }
    JDBCResultSetMetaData m = (JDBCResultSetMetaData) getMetaData();
    StringBuffer sb = new StringBuffer();
    sb.append("INSERT INTO ");
    sb.append(SQLite.Shell.sql_quote_dbl(uptable));
    sb.append("(");
    for (int i = 0; i < tr.ncolumns; i++) {
        sb.append(SQLite.Shell.sql_quote_dbl(m.getColumnName(i + 1)));
        if (i < tr.ncolumns - 1) {
        sb.append(",");
        }
    }
    sb.append(") VALUES(");
    for (int i = 0; i < tr.ncolumns; i++) {
        sb.append(nullrepl ? "'%q'" : "%Q");
        if (i < tr.ncolumns - 1) {
        sb.append(",");
        }
    }
    sb.append(")");
    try {
        this.s.conn.db.exec(sb.toString(), null, rowbuf);
    } catch (SQLite.Exception e) {
        throw new SQLException(e.getMessage());
    }
    tr.newrow(rowbuf);
    rowbuf = null;
    oninsrow = false;
    last();*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateRow()
{/*
    isUpdatable();
    if (rowbuf == null) {
        throw new SQLException("no update data provided");
    }
    if (oninsrow) {
        throw new SQLException("cursor on insert row");
    }
    if (updatable < UPD_INSUPDDEL) {
        throw new SQLException("no primary key on table defined");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    JDBCResultSetMetaData m = (JDBCResultSetMetaData) getMetaData();
    String args[] = new String[tr.ncolumns + pkcols.length];
    StringBuffer sb = new StringBuffer();
    sb.append("UPDATE ");
    sb.append(SQLite.Shell.sql_quote_dbl(uptable));
    sb.append(" SET ");
    int i;
    for (i = 0; i < tr.ncolumns; i++) {
        sb.append(SQLite.Shell.sql_quote_dbl(m.getColumnName(i + 1)));
        sb.append(" = " + (nullrepl ? "'%q'" : "%Q"));
        if (i < tr.ncolumns - 1) {
        sb.append(",");
        }
        args[i] = rowbuf[i];
    }
    sb. append(" WHERE ");
    for (int k = 0; k < pkcols.length; k++, i++) {
        sb.append(SQLite.Shell.sql_quote_dbl(pkcols[k]));
        sb.append(" = " + (nullrepl ? "'%q'" : "%Q"));
        if (k < pkcols.length - 1) {
        sb.append(" AND ");
        }
        args[i] = rd[pkcoli[k]];
    }
    try {
        this.s.conn.db.exec(sb.toString(), null, args);
    } catch (SQLite.Exception e) {
        throw new SQLException(e.getMessage());
    }
    System.arraycopy(rowbuf, 0, rd, 0, rowbuf.length);
    rowbuf = null;*/
    return NOERROR;
}

ECode JDBCResultSet::DeleteRow()
{/*
    isUpdatable();
    if (oninsrow) {
        throw new SQLException("cursor on insert row");
    }
    if (updatable < UPD_INSUPDDEL) {
        throw new SQLException("no primary key on table defined");
    }
    fillRowbuf();
    StringBuffer sb = new StringBuffer();
    sb.append("DELETE FROM ");
    sb.append(SQLite.Shell.sql_quote_dbl(uptable));
    sb.append(" WHERE ");
    String args[] = new String[pkcols.length];
    for (int i = 0; i < pkcols.length; i++) {
        sb.append(SQLite.Shell.sql_quote_dbl(pkcols[i]));
        sb.append(" = " + (nullrepl ? "'%q'" : "%Q"));
        if (i < pkcols.length - 1) {
        sb.append(" AND ");
        }
        args[i] = rowbuf[pkcoli[i]];
    }
    try {
        this.s.conn.db.exec(sb.toString(), null, args);
    } catch (SQLite.Exception e) {
        throw new SQLException(e.getMessage());
    }
    rowbuf = null;*/
    return NOERROR;
}

ECode JDBCResultSet::RefreshRow()
{/*
    isUpdatable();
    if (oninsrow) {
        throw new SQLException("cursor on insert row");
    }
    if (updatable < UPD_INSUPDDEL) {
        throw new SQLException("no primary key on table defined");
    }
    JDBCResultSetMetaData m = (JDBCResultSetMetaData) getMetaData();
    String rd[] = (String []) tr.rows.elementAt(row);
    StringBuffer sb = new StringBuffer();
    sb.append("SELECT ");
    for (int i = 0; i < tr.ncolumns; i++) {
        sb.append(SQLite.Shell.sql_quote_dbl(m.getColumnName(i + 1)));
        if (i < tr.ncolumns - 1) {
        sb.append(",");
        }
    }
    sb.append(" FROM ");
    sb.append(SQLite.Shell.sql_quote_dbl(uptable));
    sb.append(" WHERE ");
    String args[] = new String[pkcols.length];
    for (int i = 0; i < pkcols.length; i++) {
        sb.append(SQLite.Shell.sql_quote_dbl(pkcols[i]));
        sb.append(" = " + (nullrepl ? "'%q'" : "%Q"));
        if (i < pkcols.length - 1) {
        sb.append(" AND ");
        }
        args[i] = rd[pkcoli[i]];
    }
    SQLite.TableResult trnew = null;
    try {
        trnew = this.s.conn.db.get_table(sb.toString(), args);
    } catch (SQLite.Exception e) {
        throw new SQLException(e.getMessage());
    }
    if (trnew.nrows != 1) {
        throw new SQLException("wrong size of result set");
    }
    rowbuf = null;
    tr.rows.setElementAt(trnew.rows.elementAt(0), row);*/
    return NOERROR;
}

ECode JDBCResultSet::CancelRowUpdates()
{
    mRowbuf = NULL;
    return NOERROR;
}

ECode JDBCResultSet::MoveToInsertRow()
{/*
    isUpdatable();
    if (!oninsrow) {
        oninsrow = true;
        rowbuf = new String[tr.nrows];
    }*/
    return NOERROR;
}

ECode JDBCResultSet::MoveToCurrentRow()
{/*
    if (oninsrow) {
        oninsrow = false;
        rowbuf = null;
    }*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateNull(
    /* [in] */Int32 colIndex)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = null;*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateBoolean(
    /* [in] */Int32 colIndex, 
    /* [in] */Boolean b)
{
    //updateString(colIndex, b ? "1" : "0");
    return NOERROR;
}

ECode JDBCResultSet::UpdateByte(
    /* [in] */Int32 colIndex, 
    /* [in] */Byte b)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR;
}

ECode JDBCResultSet::UpdateShort(
   /* [in] */Int32 colIndex, 
   /* [in] */Int16 b)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = Short.toString(b);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateInt(
    /* [in] */Int32 colIndex, 
    /* [in] */Int32 b)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = Integer.toString(b);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateLong(
    /* [in] */Int32 colIndex, 
    /* [in] */Int64 b)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = Long.toString(b);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateFloat(
    /* [in] */Int32 colIndex, 
    /* [in] */Float f)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = Float.toString(f);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateDouble(
    /* [in] */Int32 colIndex, 
    /* [in] */Int64 f)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = Double.toString(f);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateBigDecimal(
    /* [in] */Int32 colIndex, 
    /* [in] */IBigDecimal** f)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR;
}

ECode JDBCResultSet::UpdateString(
    /* [in] */Int32 colIndex, 
    /* [in] */String s)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = s;*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateBytes(
    /* [in] */Int32 colIndex,
    /* [in] */ArrayOf<byte> s)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    if (this.s.conn.db.is3()) {
        rowbuf[colIndex - 1] = SQLite.StringEncoder.encodeX(s);
    } else {
        rowbuf[colIndex - 1] = SQLite.StringEncoder.encode(s);
    }*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateDate(
    /* [in] */Int32 colIndex, 
    /* [in] */IDate* d)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = d.toString();*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateTime(
    /* [in] */Int32 colIndex, 
    /* [in] */ITime* t)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = t.toString();*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateTimestamp(
    /* [in] */Int32 colIndex, 
    /* [in] */ITimestamp* t)
{/*
    isUpdatable();
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    fillRowbuf();
    rowbuf[colIndex - 1] = t.toString();*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateAsciiStream(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* in, 
    /* [in] */Int32 s)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR; 
}

ECode JDBCResultSet::UpdateBinaryStream(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* in, 
    /* [in] */Int32 s)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR; 
}

ECode JDBCResultSet::UpdateCharacterStream(
   /* [in] */Int32 colIndex,
   /* [in] */IReader* in, 
   /* [in] */Int32 s)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR; 
}

ECode JDBCResultSet::UpdateObject(
    /* [in] */Int32 colIndex, 
    /* [in] */IInterface* obj)
{
    //updateString(colIndex, obj.toString());
    return NOERROR; 
}

ECode JDBCResultSet::UpdateObjectEx(
    /* [in] */Int32 colIndex, 
    /* [in] */IInterface* obj, 
    /* [in] */Int32 s)
{
    //updateString(colIndex, obj.toString());
    return NOERROR; 
}

ECode JDBCResultSet::UpdateNullEx(
    /* [in] */String colName)
{/*
    int col = findColumn(colName);
    updateNull(col);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateBooleanEx(
    /* [in] */String colName, 
    /* [in] */Boolean b)
{/*
    int col = findColumn(colName);
    updateBoolean(col, b);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateByteEx(
    /* [in] */String colName, 
    /* [in] */Byte b)
{/*
    int col = findColumn(colName);
    updateByte(col, b);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateShortEx(
    /* [in] */String colName, 
    /* [in] */Int16 b)
{/*
    int col = findColumn(colName);
    updateShort(col, b);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateIntEx(
    /* [in] */String colName, 
    /* [in] */Int32 b)
{/*
    int col = findColumn(colName);
    updateInt(col, b);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateLongEx(
    /* [in] */String colName, 
    /* [in] */Int64 b)
{/*
    int col = findColumn(colName);
    updateLong(col, b);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateFloatEx(
    /* [in] */String colName, 
    /* [in] */Float f)
{/*
    int col = findColumn(colName);
    updateFloat(col, f);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateDoubleEx(
    /* [in] */String colName, 
    /* [in] */Double f)
{/*
    int col = findColumn(colName);
    updateDouble(col, f);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateBigDecimalEx(
    /* [in] */String colName, 
    /* [in] */IBigDecimal* f)
{/*
    int col = findColumn(colName);
    updateBigDecimal(col, f);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateStringEx(
    /* [in] */String colName, 
    /* [in] */String s)
{/*
    int col = findColumn(colName);
    updateString(col, s);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateBytesEx(
    /* [in] */String colName, 
    /* [in] */ArrayOf<Byte> s)
{/*
    int col = findColumn(colName);
    updateBytes(col, s);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateDateEx(
    /* [in] */String colName, 
    /* [in] */IDate* d)
{/*
    int col = findColumn(colName);
    updateDate(col, d);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateTimeEx(
    /* [in] */String colName, 
    /* [in] */ITime* t)
{/*
    int col = findColumn(colName);
    updateTime(col, t);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateTimestampEx(
    /* [in] */String colName, 
    /* [in] */ITimestamp* t)
{/*
    int col = findColumn(colName);
    updateTimestamp(col, t);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateAsciiStreamEx(
    /* [in] */String colName, 
    /* [in] */IInputStream* in,
    /* [in] */Int32 s)
{/*
    int col = findColumn(colName);
    updateAsciiStream(col, in, s);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateBinaryStreamEx(
    /* [in] */String colName, 
    /* [in] */IInputStream* in,
    /* [in] */Int32 s)
{/*
    int col = findColumn(colName);
    updateBinaryStream(col, in, s);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateCharacterStreamEx(
    /* [in] */String colName, 
    /* [in] */IReader* in,
    /* [in] */Int32 s)
{/*
    int col = findColumn(colName);
    updateCharacterStream(col, in, s);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateObjectEx2(
    /* [in] */String colName, 
    /* [in] */IInterface* obj)
{/*
    int col = findColumn(colName);
    updateObject(col, obj);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateObjectEx3(
    /* [in] */String colName, 
    /* [in] */IInterface* obj, 
    /* [out] */Int32 s)
{/*
    int col = findColumn(colName);
    updateObject(col, obj, s);*/
    return NOERROR; 
}

ECode JDBCResultSet::GetStatement(
    /* [out] */IStatement* result)
{/*
    if (s == null) {
        throw new SQLException("stale result set");
    }
    return s;*/
    return NOERROR; 
}

ECode JDBCResultSet::Close()
{/*
    s = null;
    tr = null;
    lastg = null;
    oninsrow = false;
    rowbuf = null;
    row = -1;*/
    return NOERROR; 
}

ECode JDBCResultSet::GetURL(
    /* [in] */Int32 colIndex,
    /* [out] */IURL** url)
{/*
    if (tr == null || colIndex < 1 || colIndex > tr.ncolumns) {
        throw new SQLException("column " + colIndex + " not found");
    }
    String rd[] = (String []) tr.rows.elementAt(row);
    lastg = rd[colIndex - 1];
    java.net.URL url = null;
    if (lastg == null) {
        return url;
    }
    try {
        url = new java.net.URL(lastg);
    } catch (java.lang.Exception e) {
        url = null;
    }
    return url;*/
    return NOERROR; 
}

ECode JDBCResultSet::GetURLEx(
    /* [in] */String colName,
     /* [out] */IURL** url)
{/*
    int col = findColumn(colName);
    return getURL(col);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateRef(
    /* [in] */Int32 colIndex, 
    /* [in] */IRef* x)
{
    //throw new SQLFeatureNotSupportedException();
    return NOERROR; 
}

ECode JDBCResultSet::UpdateRefEx(
    /* [in] */String colName, 
    /* [in] */IRef* x)
{/*
    int col = findColumn(colName);
    updateRef(col, x);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateBlob(
    /* [in] */Int32 colIndex, 
    /* [in] */IBlob* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateBlobEx(
    /* [in] */String colName, 
    /* [in] */IBlob* x)
{/*
    int col = findColumn(colName);
    updateBlob(col, x);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateClob(
    /* [in] */Int32 colIndex, 
    /* [in] */IClob* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateClobEx(
    /* [in] */String colName, 
    /* [in] */IClob* x)
{/*
    int col = findColumn(colName);
    updateClob(col, x);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateArray(
    /* [in] */Int32 colIndex, 
    /* [in] */IArray* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateArrayEx(
    /* [in] */String colName, 
    /* [in] */IArray* x)
{/*
    int col = findColumn(colName);
    updateArray(col, x);*/
    return NOERROR; 
}

ECode JDBCResultSet::GetRowId(
    /* [in] */Int32 colIndex,
    /* [out] */IRowId** id)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::GetRowIdEx(
    /* [in] */String colName,
    /* [out] */IRowId** id)
{/*
    int col = findColumn(colName);
    return getRowId(col);*/
    return NOERROR; 
}

ECode JDBCResultSet::UpdateRowId(
    /* [in] */Int32 colIndex, 
    /* [in] */IRowId* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateRowIdEx(
    /* [in] */String colName, 
    /* [in] */IRowId* x)
{/*
    int col = findColumn(colName);
    updateRowId(col, x);*/
    return NOERROR; 
}

ECode JDBCResultSet::GetHoldability(
    /* [out] */Int32* result)
{
    assert(result != NULL);
    *result = ResultSet.CLOSE_CURSORS_AT_COMMIT;
    return NOERROR; 
}

ECode JDBCResultSet::IsClosed(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = tr == NULL;
    return NOERROR;
}

ECode JDBCResultSet::UpdateNString(
    /* [in] */Int32 colIndex, 
    /* [in] */String nString)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateNStringEx(
    /* [in] */String colName, 
    /* [in] */String nString)
{/*
    int col = findColumn(colName);
    updateNString(col, nString);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateNClob(
    /* [in] */Int32 colIndex, 
    /* [in] */INClob* nclob)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateNClobEx(
    /* [in] */String colName,
    /* [in] */ INClob* nclob)
{/*
    int col = findColumn(colName);
    updateNClob(col, nclob);*/
    return NOERROR;
}

ECode JDBCResultSet::GetNClob(
    /* [in] */Int32 colIndex,
    /* [out] */INClob** clob)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::GetNClobEx(
    /* [in] */String colName,
    /* [out] */INClob** clob)
{/*
    int col = findColumn(colName);
    return getNClob(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetSQLXML(
    /* [in] */Int32 colIndex,
    /* [out] */ISQLXML** xml)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::SQLXML GetSQLXMLEx(
    /* [in] */String colName,
    /* [out] */ISQLXML** xml)
{/*
    int col = findColumn(colName);
    return getSQLXML(col);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateSQLXML(
    /* [in] */Int32 colIndex, 
    /* [in] */ISQLXML* xml)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateSQLXMLEx(
    /* [in] */String colName, 
    /* [in] */ISQLXML* xml)
{/*
    int col = findColumn(colName);
    updateSQLXML(col, xml);*/
    return NOERROR;
}

ECode JDBCResultSet::GetNString(
    /* [in] */Int32 colIndex,
    /* [out] */String* str)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::String GetNStringEx(
    /* [in] */String colName,
    /* [out] */String* str)
{/*
    int col = findColumn(colName);
    return getNString(col);*/
    return NOERROR;
}

ECode JDBCResultSet::GetNCharacterStream(
    /* [in] */Int32 colIndex,
    /* [out] */IReader** read)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::GetNCharacterStreamEx(
    /* [in] */String colName,
    /* [out] */IReader** read)
{/*
    int col = findColumn(colName);
    return getNCharacterStream(col);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateNCharacterStream(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateNCharacterStreamEx(
    /* [in] */String colName, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{/*
    int col = findColumn(colName);
    updateNCharacterStream(col, x, len);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateAsciiStreamEx2(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateAsciiStreamEx3(
    /* [in] */String colName, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{/*
    int col = findColumn(colName);
    updateAsciiStream(col, x, len);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateBinaryStreamEx2(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateBinaryStreamEx3(
    /* [in] */String colName, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{/*
    int col = findColumn(colName);
    updateBinaryStream(col, x, len);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateCharacterStreamEx2(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateCharacterStreamEx3(
    /* [in] */String colName, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{/*
    int col = findColumn(colName);
    updateCharacterStream(col, x, len);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateBlobEx2(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateBlobEx3(
    /* [in] */String colName, 
    /* [in] */IInputStream* x,
    /* [in] */Int64 len)
{/*
    int col = findColumn(colName);
    updateBlob(col, x, len);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateClobEx2(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateClobEx3(
    /* [in] */String colName, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{/*
    int col = findColumn(colName);
    updateClob(col, x, len);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateNClobEx2(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateNClobEx3(
    /* [in] */String colName, 
    /* [in] */IReader* x,
    /* [in] */Int64 len)
{/*
    int col = findColumn(colName);
    updateNClob(col, x, len);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateNCharacterStreamEx2(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateNCharacterStreamEx3(
    /* [in] */String colName, 
    /* [in] */IReader* x)
{/*
    int col = findColumn(colName);
    updateNCharacterStream(col, x);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateAsciiStreamEx3(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateAsciiStreamEx4(
    /* [in] */String colName, 
    /* [in] */IInputStream* x)
{/*
    int col = findColumn(colName);
    updateAsciiStream(col, x);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateBinaryStreamEx3(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateBinaryStreamEx4(
    /* [in] */String colName, 
    /* [in] */IInputStream* x)
{/*
    nt col = findColumn(colName);
    updateBinaryStream(col, x);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateCharacterStreamEx3(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateCharacterStreamEx4(
    /* [in] */String colName, 
    /* [in] */IReader* x)
{/*
    int col = findColumn(colName);
    updateCharacterStream(col, x);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateBlobEx3(
    /* [in] */Int32 colIndex, 
    /* [in] */IInputStream* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateBlobEx4(
    /* [in] */String colName, 
    /* [in] */IInputStream* x)
{/*
    int col = findColumn(colName);
    updateBlob(col, x);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateClobEx3(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateClobEx4(
    /* [in] */String colName, 
    /* [in] */IReader* x)
{/*
    int col = findColumn(colName);
    updateClob(col, x);*/
    return NOERROR;
}

ECode JDBCResultSet::UpdateNClobEx3(
    /* [in] */Int32 colIndex, 
    /* [in] */IReader* x)
{
    //throw new SQLFeatureNotSupportedException();
}

ECode JDBCResultSet::UpdateNClobEx4(
    /* [in] */String colName, 
    /* [in] */IReader* x)
{/*
    int col = findColumn(colName);
    updateNClob(col, x);*/
    return NOERROR;
}