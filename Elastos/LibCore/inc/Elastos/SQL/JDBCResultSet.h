#ifndef _JDBCRESULTSET_H_
#define _JDBCRESULTSET_H_

#include <elastos.h>
#include <elastos/AutoPtr.h>
using namespace Elastos;

class JDBCResultSet
{
protected:
    /**
     * Table returned by Database.get_table()
     */
    AutoPtr<ITableResult> mTr;

private:
    /**
     * Current row to be retrieved.
     */
    Int32 mRow;

    /**
     * Statement from which result set was produced.
     */
    AutoPtr<IJDBCStatement> ms;

    /**
     * Meta data for result set or null.
     */
    AutoPtr<IJDBCResultSetMetaData> md;

    /**
     * Last result cell retrieved or null.
     */
    String mLastg;

    /**
     * Updatability of this result set.
     */
    Int32 mUpdatable;

    /**
     * When updatable this is the table name.
     */
    String mUptable;

    /**
     * When updatable these are the PK column names of uptable.
     */
    //String pkcols[];
    ArrayOf<String> mPkcols;

    /**
     * When updatable these are the PK column indices (0-based) of uptable.
     */
    // /Int32 pkcoli[];
    ArrayOf<Int32> mPkcoli;

    /*
     * Constants to reflect updateability.
     */
    const static Int32 UPD_UNKNOWN;// = -1;
    const static Int32 UPD_NO;// = 0;
    const static Int32 UPD_INS;// = 1;
    const static Int32 UPD_INSUPDDEL;// = 2;

    /**
     * Flag for cursor being (not) on insert row.
     */
    Boolean mOninsrow;

    /**
     * Row buffer for insert/update row.
     */
    //String rowbuf[];
    ArrayOf<String> mRowbuf;

    static const Boolean mNullrepl;// =SQLite.Database.version().compareTo("2.5.0") < 0;
private:
    Integer InternalGetInt(/////////????????????????????
        /* [in] */Int32 columnIndex);

    Int16 InternalGetShort(
        /* [in] */Int32 columnIndex);

    CARAPI InternalGetTime(
        /* [in] */Int32 columnIndex,
        /* [in] */ICalendar* cal
        /* [out] */ITime** time);

    CARAPI InternalGetTimestamp(
        /* [in] */Int32 columnIndex,
        /* [in] */ICalendar* cal,
        /* [out] */ITimestamp** stamp);

    CARAPI InternalGetDate(
        /* [in] */Int32 columnIndex,
        /* [in] */ICalendar* cal,
        /* [out] */IDate** date);

    Int64 InternalGetDouble(
        /* [in] */Int32 columnIndex);

    Float InternalGetFloat(
        /* [in] */Int32 columnIndex);

    Int64 InternalGetLong(
        /* [in] */Int32 columnIndex);

public:
    JDBCResultSet();

    ~JDBCResultSet();

    Init(
        /* [in] */ITableResult* tr, 
        /* [in] */IJDBCStatement* s);

    CARAPI IsUpdatable(
        /* [out] */Boolean* result);

    CARAPI FillRowbuf();

    CARAPI Next(
        /* [out] */Boolean* result);

    CARAPI FindColumn(
        /* [in] */String columnName,
        /* [out] */Int32* column);
  
    CARAPI GetRow(
        /* [out] */Int32* row);

    CARAPI Previous(
        /* [out] */Boolean* result);

    CARAPI Absolute(
        /* [in] */Int32 row,
        /* [out] */Boolean* result);

    CARAPI Relative(
        /* [in] */Int32 row,
        /* [out] */Boolean* result);

    CARAPI SetFetchDirection(
        /* [in] */Int32 dir);

    CARAPI GetFetchDirection(
        /* [out] */Int32* result);

    CARAPI SetFetchSize(
        /* [in] */Int32 fsize);

    CARAPI GetFetchSize(
        /* [out] */Int32* size);

    CARAPI GetString(
        /* [in] */Int32 columnIndex
        /* [out] */String* str);

    CARAPI GetStringEx(
        /* [in] */String columnName,
        /* [out] */String* str);

    CARAPI GetInt(
        /* [in] */Int32 columnIndex,
        /* [out] */Int32* result);

    CARAPI GetIntEx(
        /* [in] */String columnName,
        /* [out] */Int32* result);

    CARAPI GetBoolean(
        /* [in] */Int32 columnIndex,
        /* [out] */Boolean* result);

    CARAPI GetBooleanEx(
        /* [in] */String columnName,
        /* [out] */Boolean* result);

    CARAPI GetMetaData(
        /* [out] */IResultSetMetaData** data);

    CARAPI GetShort(
        /* [in] */Int32 columnIndex,
        /* [out] */Int16* result);

    CARAPI GetShortEx(
        /* [in] */String columnName,
        /* [out] */Int16* result);

    CARAPI GetTime(
        /* [in] */Int32 columnIndex,
        /* [out] */ITime** time);

    CARAPI GetTimeEx(
        /* [in] */String columnName,
        /* [out] */ITime** time);

    CARAPI GetTimeEx2(
        /* [in] */Int32 columnIndex, 
        /* [in] */ICalendar* cal,
        /* [out] */ITime** time);

    CARAPI GetTimeEx3(
        /* [in] */String columnName, 
        /* [in] */ICalendar* cal,
        /* [out] */ITime** time);

    CARAPI GetTimestamp(
        /* [in] */Int32 columnIndex,
        /* [out] */ITimestamp** stamp);

    CARAPI GetTimestampEx(
        /* [in] */String columnName,
        /* [out] */ITimestamp** stamp);

    CARAPI GetTimestampEx2(
        /* [in] */Int32 columnIndex,
        /* [in] */ICalendar* cal,
        /* [out] */ITimestamp** stamp);

    CARAPI GetTimestampEx3(
        /* [in] */String columnName,
        /* [in] */ICalendar* cal,
        /* [out] */ITimestamp** stamp);

    CARAPI GetDate(
        /* [in] */Int32 columnIndex,
        /* [out] */IDate** date);

    CARAPI GetDateEx(
        /* [in] */String columnName,
        /* [out] */IDate** date);

    CARAPI GetDateEx2(
        /* [in] */Int32 columnIndex, 
        /* [in] */ICalendar* cal,
        /* [out] */IDate** date);

    CARAPI GetDateEx3(
        /* [in] */String columnName, 
        /* [in] */ICalendar* cal,
        /* [out] */IDate** date);

    CARAPI GetDouble(
        /* [in] */Int32 columnIndex,
        /* [out] */Int64* result);
    
    CARAPI GetDoubleEx(
        /* [in] */String columnName,
        /* [out] */Int64* result);

    CARAPI GetFloat(
        /* [in] */Int32 columnIndex,
        /* [out] */Float* result);

    CARAPI GetFloatEx(
        /* [in] */String columnName,
        /* [out] */Float* result);

    CARAPI GetLong(
        /* [in] */Int32 columnIndex,
        /* [out] */Int64* result);

    CARAPI GetLongEx(
        /* [in] */String columnName,
        /* [out] */Int64* result);

    //@Deprecated
    CARAPI GetUnicodeStream(
        /* [in] */Int32 columnIndex,
        /* [out] */IInputStream** stream);

    //@Deprecated
    CARAPI GetUnicodeStreamEx(
        /* [in] */String columnName,
        /* [out] */IInputStream** stream);

    CARAPI GetAsciiStream(
        /* [in] */String columnName,
        /* [out] */IInputStream** stream);

    CARAPI GetAsciiStreamEx(
        /* [in] */Int32 columnIndex,
        /* [out] */IInputStream** stream);

    CARAPI GetBigDecimal(
        /* [in] */String columnName,
        /* [out] */IBigDecimal** result);

    //@Deprecated
    CARAPI GetBigDecimalEx(
        /* [in] */String columnName, 
        /* [in] */Int32 scale,
        /* [out] */IBigDecimal** result);

    CARAPI GetBigDecimalEx2(
        /* [in] */Int32 columnIndex,
        /* [out] */IBigDecimal** result);

    //@Deprecated
    CARAPI SetBigDecimalEx3(
        /* [in] */Int32 columnIndex, 
        /* [in] */Int32 scale,
        /* [out] */IBigDecimal** result);

    CARAPI GetBinaryStream(
        /* [in] */Int32 columnIndex,
        /* [out] */IInputStream** stream);

    CARAPI GetBinaryStreamEx(
        /* [in] */String columnName,
        /* [out] */IInputStream** stream);

    CARAPI GetByte(
        /* [in] */Int32 columnIndex,
        /* [out] */Byte* result);

    CARAPI GetByteEx(
        /* [in] */String columnName,
        /* [out] */Byte* result);

    CARAPI GetBytes(
        /* [in] */Int32 columnIndex,
        /* [out,callee] */ArrayOf<Byte>* bt);

    CARAPI GetBytesEx(
        /* [in] */String columnName,
        /* [out,callee] */ArrayOf<Byte>* bt);

    CARAPI GetCursorName(
        /* [out] */String* name);

    CARAPI GetObject(
        /* [in] */Int32 columnIndex,
        /* [out] */IInterface** obj);

    CARAPI GetObjectEx(
        /* [in] */String columnName,
        /* [out] */IInterface** obj);

    CARAPI GetObjectEx2(
        /* [in] */Int32 columnIndex, 
        /* [in] */java.util.Map map, ////////////////////////////////////
        /* [out] */IInterface** obj);

    CARAPI GetObjectEx3(
        /* [in] */String columnName, 
        /* [in] */java.util.Map map, /////////////////////////////
        /* [out] */IInterface** obj);

    CARAPI GetRef(
        /* [in] */Int32 columnIndex,
        /* [out] */IRef** ref);

    CARAPI GetRefEx(
        /* [in] */String columnName,
        /* [out] */IRef** ref);

    CARAPI GetBlob(
        /* [in] */Int32 columnIndex,
        /* [out] */IBlob** blob);

    CARAPI GetBlobEx(
        /* [in] */String columnName,
        /* [out] */IBlob** blob);

    CARAPI GetClob(
        /* [in] */Int32 columnIndex,
        /* [out] */IClob** clob);

    CARAPI GetClobEx(
        /* [in] */String columnName,
        /* [out] */IClob** clob);

    CARAPI GetArray(
        /* [in] */Int32 columnIndex,
        /* [out] */IArray** array);

    CARAPI GetArrayEx(
        /* [in] */String columnName,
        /* [out] */IArray** array);

    CARAPI GetCharacterStream(
        /* [in] */Int32 columnIndex,
        /* [out] */IReader** stream);

    CARAPI GetCharacterStreamEx(
        /* [in] */String columnName,
        /* [out] */IReader** stream);

//   CARAPI SQLWarning GetWarnings();

    CARAPI WasNull(
        /* [out] */Boolean* result);
    
    CARAPI ClearWarnings();

    CARAPI IsFirst(
        /* [out] */Boolean* result);

    CARAPI IsBeforeFirst(
        /* [out] */Boolean* result);

    CARAPI BeforeFirst();

    CARAPI First(
        /* [out] */Boolean* result);

    CARAPI IsAfterLast(
        /* [out] */Boolean* result);

    CARAPI AfterLast();

    CARAPI IsLast(
        /* [out] */Boolean* result);

    CARAPI Last(
        /* [out] */Boolean* result);

    CARAPI GetType(
        /* [out] */Int32* result);

    CARAPI GetConcurrency(
        /* [out] */Int32* result);

    CARAPI RowUpdated(
        /* [out] */Boolean* result);

    CARAPI RowInserted(
        /* [out] */Boolean* result);

    CARAPI RowDeleted(
        /* [out] */Boolean* result);

    CARAPI InsertRow();

    CARAPI UpdateRow();

    CARAPI DeleteRow();

    CARAPI RefreshRow();

    CARAPI CancelRowUpdates();

    CARAPI MoveToInsertRow();

    CARAPI MoveToCurrentRow();

    CARAPI UpdateNull(
        /* [in] */Int32 colIndex);

    CARAPI UpdateBoolean(
        /* [in] */Int32 colIndex, 
        /* [in] */Boolean b);

    CARAPI UpdateByte(
        /* [in] */Int32 colIndex, 
        /* [in] */Byte b);

    CARAPI UpdateShort(
       /* [in] */Int32 colIndex, 
       /* [in] */Int16 b);

    CARAPI UpdateInt(
        /* [in] */Int32 colIndex, 
        /* [in] */Int32 b);

    CARAPI UpdateLong(
        /* [in] */Int32 colIndex, 
        /* [in] */Int64 b);

    CARAPI UpdateFloat(
        /* [in] */Int32 colIndex, 
        /* [in] */Float f);

    CARAPI UpdateDouble(
        /* [in] */Int32 colIndex, 
        /* [in] */Int64 f);

    CARAPI UpdateBigDecimal(
        /* [in] */Int32 colIndex, 
        /* [in] */IBigDecimal** f);

    CARAPI UpdateString(
        /* [in] */Int32 colIndex, 
        /* [in] */String s);

    CARAPI UpdateBytes(
        /* [in] */Int32 colIndex,
        /* [in] */ArrayOf<byte> s);

    CARAPI UpdateDate(
        /* [in] */Int32 colIndex, 
        /* [in] */IDate* d);

    CARAPI UpdateTime(
        /* [in] */Int32 colIndex, 
        /* [in] */ITime* t);

    CARAPI UpdateTimestamp(
        /* [in] */Int32 colIndex, 
        /* [in] */ITimestamp* t);

    CARAPI UpdateAsciiStream(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* in, 
        /* [in] */Int32 s);

    CARAPI UpdateBinaryStream(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* in, 
        /* [in] */Int32 s);

    CARAPI UpdateCharacterStream(
       /* [in] */Int32 colIndex,
       /* [in] */IReader* in, 
       /* [in] */Int32 s);

    CARAPI UpdateObject(
        /* [in] */Int32 colIndex, 
        /* [in] */IInterface* obj);

    CARAPI UpdateObjectEx(
        /* [in] */Int32 colIndex, 
        /* [in] */IInterface* obj, 
        /* [in] */Int32 s);

    CARAPI UpdateNullEx(
        /* [in] */String colName);

    CARAPI UpdateBooleanEx(
        /* [in] */String colName, 
        /* [in] */Boolean b);

    CARAPI UpdateByteEx(
        /* [in] */String colName, 
        /* [in] */Byte b);

    CARAPI UpdateShortEx(
        /* [in] */String colName, 
        /* [in] */Int16 b);

    CARAPI UpdateIntEx(
        /* [in] */String colName, 
        /* [in] */Int32 b);

    CARAPI UpdateLongEx(
        /* [in] */String colName, 
        /* [in] */Int64 b);

    CARAPI UpdateFloatEx(
        /* [in] */String colName, 
        /* [in] */Float f);

    CARAPI UpdateDoubleEx(
        /* [in] */String colName, 
        /* [in] */Double f);

    CARAPI UpdateBigDecimalEx(
        /* [in] */String colName, 
        /* [in] */IBigDecimal* f);

    CARAPI UpdateStringEx(
        /* [in] */String colName, 
        /* [in] */String s);

    CARAPI UpdateBytesEx(
        /* [in] */String colName, 
        /* [in] */ArrayOf<Byte> s);

    CARAPI UpdateDateEx(
        /* [in] */String colName, 
        /* [in] */IDate* d);

    CARAPI UpdateTimeEx(
        /* [in] */String colName, 
        /* [in] */ITime* t);

    CARAPI UpdateTimestampEx(
        /* [in] */String colName, 
        /* [in] */ITimestamp* t);

    CARAPI UpdateAsciiStreamEx(
        /* [in] */String colName, 
        /* [in] */IInputStream* in,
        /* [in] */Int32 s);

    CARAPI UpdateBinaryStreamEx(
        /* [in] */String colName, 
        /* [in] */IInputStream* in,
        /* [in] */Int32 s);

    CARAPI UpdateCharacterStreamEx(
        /* [in] */String colName, 
        /* [in] */IReader* in,
        /* [in] */Int32 s);

    CARAPI UpdateObjectEx2(
        /* [in] */String colName, 
        /* [in] */IInterface* obj);

    CARAPI UpdateObjectEx3(
        /* [in] */String colName, 
        /* [in] */IInterface* obj, 
        /* [out] */Int32 s);

    CARAPI GetStatement(
        /* [out] */IStatement* result);

    CARAPI Close();

    CARAPI GetURL(
        /* [in] */Int32 colIndex,
        /* [out] */IURL** url);

    CARAPI GetURLEx(
        /* [in] */String colName,
         /* [out] */IURL** url);

    CARAPI UpdateRef(
        /* [in] */Int32 colIndex, 
        /* [in] */IRef* x);

    CARAPI UpdateRefEx(
        /* [in] */String colName, 
        /* [in] */IRef* x);

    CARAPI UpdateBlob(
        /* [in] */Int32 colIndex, 
        /* [in] */IBlob* x);

    CARAPI UpdateBlobEx(
        /* [in] */String colName, 
        /* [in] */IBlob* x);

    CARAPI UpdateClob(
        /* [in] */Int32 colIndex, 
        /* [in] */IClob* x);

    CARAPI UpdateClobEx(
        /* [in] */String colName, 
        /* [in] */IClob* x);

    CARAPI UpdateArray(
        /* [in] */Int32 colIndex, 
        /* [in] */IArray* x);

    CARAPI UpdateArrayEx(
        /* [in] */String colName, 
        /* [in] */IArray* x);

    CARAPI GetRowId(
        /* [in] */Int32 colIndex,
        /* [out] */IRowId** id);

    CARAPI GetRowIdEx(
        /* [in] */String colName,
        /* [out] */IRowId** id);

    CARAPI UpdateRowId(
        /* [in] */Int32 colIndex, 
        /* [in] */IRowId* x);

    CARAPI UpdateRowIdEx(
        /* [in] */String colName, 
        /* [in] */IRowId* x);

    CARAPI GetHoldability(
        /* [out] */Int32* result);

    CARAPI IsClosed(
        /* [out] */Boolean* result);

    CARAPI UpdateNString(
        /* [in] */Int32 colIndex, 
        /* [in] */String nString);

    CARAPI UpdateNStringEx(
        /* [in] */String colName, 
        /* [in] */String nString);

    CARAPI UpdateNClob(
        /* [in] */Int32 colIndex, 
        /* [in] */INClob* nclob);

    CARAPI UpdateNClobEx(
        /* [in] */String colName,
        /* [in] */ INClob* nclob);

    CARAPI GetNClob(
        /* [in] */Int32 colIndex,
        /* [out] */INClob** clob);

    CARAPI GetNClobEx(
        /* [in] */String colName,
        /* [out] */INClob** clob);

    CARAPI GetSQLXML(
        /* [in] */Int32 colIndex,
        /* [out] */ISQLXML** xml);

    CARAPI SQLXML GetSQLXMLEx(
        /* [in] */String colName,
        /* [out] */ISQLXML** xml);

    CARAPI UpdateSQLXML(
        /* [in] */Int32 colIndex, 
        /* [in] */ISQLXML* xml);

    CARAPI UpdateSQLXMLEx(
        /* [in] */String colName, 
        /* [in] */ISQLXML* xml);

    CARAPI GetNString(
        /* [in] */Int32 colIndex,
        /* [out] */String* str);

    CARAPI String GetNStringEx(
        /* [in] */String colName,
        /* [out] */String* str);

    CARAPI GetNCharacterStream(
        /* [in] */Int32 colIndex,
        /* [out] */IReader** read);

    CARAPI GetNCharacterStreamEx(
        /* [in] */String colName,
        /* [out] */IReader** read);

    CARAPI UpdateNCharacterStream(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateNCharacterStreamEx(
        /* [in] */String colName, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateAsciiStreamEx2(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI UpdateAsciiStreamEx3(
        /* [in] */String colName, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI UpdateBinaryStreamEx2(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI UpdateBinaryStreamEx3(
        /* [in] */String colName, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI UpdateCharacterStreamEx2(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateCharacterStreamEx3(
        /* [in] */String colName, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateBlobEx2(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI UpdateBlobEx3(
        /* [in] */String colName, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI UpdateClobEx2(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateClobEx3(
        /* [in] */String colName, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateNClobEx2(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateNClobEx3(
        /* [in] */String colName, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI UpdateNCharacterStreamEx2(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x);

    CARAPI UpdateNCharacterStreamEx3(
        /* [in] */String colName, 
        /* [in] */IReader* x);

    CARAPI UpdateAsciiStreamEx3(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* x);

    CARAPI UpdateAsciiStreamEx4(
        /* [in] */String colName, 
        /* [in] */IInputStream* x);

    CARAPI UpdateBinaryStreamEx3(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* x);

    CARAPI UpdateBinaryStreamEx4(
        /* [in] */String colName, 
        /* [in] */IInputStream* x);

    CARAPI UpdateCharacterStreamEx3(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x);

    CARAPI UpdateCharacterStreamEx4(
        /* [in] */String colName, 
        /* [in] */IReader* x);

    CARAPI UpdateBlobEx3(
        /* [in] */Int32 colIndex, 
        /* [in] */IInputStream* x);

    CARAPI UpdateBlobEx4(
        /* [in] */String colName, 
        /* [in] */IInputStream* x);

    CARAPI UpdateClobEx3(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x);

    CARAPI UpdateClobEx4(
        /* [in] */String colName, 
        /* [in] */IReader* x);

    CARAPI UpdateNClobEx3(
        /* [in] */Int32 colIndex, 
        /* [in] */IReader* x);

    CARAPI UpdateNClobEx4(
        /* [in] */String colName, 
        /* [in] */IReader* x);

//    CARAPI <T> T Unwrap(/* [in] */java.lang.Class<T> iface);

//    CARAPI boolean IsWrapperFor(/* [in] */java.lang.Class iface);
};
#endif//_JDBCRESULTSET_H_
