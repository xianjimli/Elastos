#ifndef _JDBCPREPAREDSTATEMENT_H_
#define _JDBCPREPAREDSTATEMENT_H_

#include <elastos.h>
#include <elastos/AutoPtr.h>
using namespace Elastos;

class BatchArg {
    String mArg;
    Boolean mBlob;

    BatchArg(
        /* [in] */String arg, 
        /* [in] */Boolean blob);
};


class JDBCPreparedStatement
{
private:
    String sql;

    ArrayOf<String> mArgs;

    ArrayOf<Boolean> mBlobs;

    //ArrayList<BatchArg> batch;

    static const Boolean mNullrepl;// =SQLite.Database.version().compareTo("2.5.0") < 0;
private:
    String Fixup(
        /* [in] */String sql);

    String Fixup2(
        /* [in] */String sql);

public:
    JDBCPreparedStatement();

    ~JDBCPreparedStatement();

    CARAPI Init(
        /* [in] */IJDBCConnection* conn, 
        /* [in] */String sql);

    CARAPI ExecuteQuery(
        /* [out] */IResultSet** query);

    CARAPI ExecuteUpdate(
        /* [out] */Int32* date);

    CARAPI SetNull(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Int32 sqlType);
    
    CARAPI SetBoolean(
        /* [in] */Int32 parameterIndex,
        /* [in] */Boolean x);

    CARAPI SetByte(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Byte x);

    CARAPI SetShort(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Int16 x);

    CARAPI SetInt(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Int32 x);

    CARAPI SetLong(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Int64 x);

    CARAPI SetFloat(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Float x);

    CARAPI SetDouble(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Int64 x);

    CARAPI SetBigDecimal(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IBigDecimal* x);

    CARAPI SetString(
        /* [in] */Int32 parameterIndex, 
        /* [in] */String x);

    CARAPI SetBytes(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ArrayOf<Byte> x);

    CARAPI SetDate(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IDate* x);

    CARAPI SetTime(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ITime* x) ;

    CARAPI SetTimestamp(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ITimestamp* x);

    CARAPI SetAsciiStream(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x,
        /* [in] */Int32 length);

    //@Deprecated
    CARAPI SetUnicodeStream(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x, 
        /* [in] */Int32 length);

    CARAPI SetBinaryStream(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x,
        /* [in] */Int32 length);

    CARAPI ClearParameters();

    CARAPI SetObject(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInterface* x, 
        /* [in] */Int32 targetSqlType,
        /* [in] */Int32 scale);

    CARAPI SetObjectEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInterface* x,
        /* [in] */Int32 targetSqlType);

    CARAPI SetObjectEx2(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInterface* x);

    CARAPI Execute(
        /* [out] */Boolean* result);

    CARAPI AddBatch();

    CARAPI ExecuteBatch(
        /* [out] */ArrayOf<Int32>* batch);

    CARAPI ClearBatch();

    CARAPI Close();

    CARAPI SetCharacterStream(
        /* [in] */Int32 parameterIndex,
        /* [in] */IReader* reader,
        /* [in] */Int32 length);

    CARAPI SetRef(
        /* [in] */Int32 i, 
        /* [in] */IRef* x);

    CARAPI SetBlob(
        /* [in] */Int32 i, 
        /* [in] */IBlob* x);

    CARAPI SetClob(
        /* [in] */Int32 i, 
        /* [in] */IClob* x);

    CARAPI SetArray(
        /* [in] */Int32 i, 
        /* [in] */IArray* x);

    CARAPI GetMetaData(
        /* [out] */IResultSetMetaData** data);

    CARAPI SetDateEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IDate* x, 
        /* [in] */ICalendar* cal);

    CARAPI SetTimeEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ITime* x, 
        /* [in] */ICalendar* cal);

    CARAPI SetTimestampEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ITimestamp* x,
        /* [in] */ICalendar* cal);

    CARAPI SetNullEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */Int32 sqlType, 
        /* [in] */String typeName);

    CARAPI GetParameterMetaData(
        /* [out] */IParameterMetaData** data);

    CARAPI RegisterOutputParameter(
        /* [in] */String parameterName, 
        /* [in] */Int32 sqlType);

    CARAPI RegisterOutputParameterEx(
        /* [in] */String parameterName, 
        /* [in] */Int32 sqlType,
        /* [in] */Int32 scale);

    CARAPI RegisterOutputParameterEx2(
        /* [in] */String parameterName,
        /* [in] */ Int32 sqlType,
        /* [in] */String typeName);

    CARAPI GetURL(
        /* [in] */Int32 parameterIndex,
        /* [out] */IURL** url);

    CARAPI SetURL(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IURL* url);

    CARAPI SetNullEx2(
        /* [in] */String parameterName, 
        /* [in] */Int32 sqlType);

    CARAPI SetBooleanEx(
        /* [in] */String parameterName, 
        /* [in] */Boolean val);

    CARAPI SetByteEx(
        /* [in] */String parameterName,
        /* [in] */ Byte val);

    CARAPI SetShortEx(
        /* [in] */String parameterName, 
        /* [in] */Int16 val);

    CARAPI SetIntEx(
        /* [in] */String parameterName, 
        /* [in] */Int32 val);

    CARAPI SetLongEx(
        /* [in] */String parameterName, 
        /* [in] */Int64 val);

    CARAPI SetFloatEx(
        /* [in] */String parameterName, 
        /* [in] */Float val);

    CARAPI SetDoubleEx(
        /* [in] */String parameterName, 
        /* [in] */Double val);

    CARAPI SetBigDecimalEx(
        /* [in] */String parameterName, 
        /* [in] */IBigDecimal* val);

    CARAPI SetStringEx(
        /* [in] */String parameterName, 
        /* [in] */String val);

    CARAPI SetBytesEx(
        /* [in] */String parameterName, 
        /* [in] */ArrayOf<Byte> val);

    CARAPI SetDateEx(
        /* [in] */String parameterName, 
        /* [in] */IDate* val);

    CARAPI SetTimeEx(
        /* [in] */String parameterName, 
        /* [in] */ITime* val);

    CARAPI SetTimestampEx(
        /* [in] */String parameterName, 
        /* [in] */ITimestamp* val);

    CARAPI SetAsciiStreamEx(
        /* [in] */String parameterName,
        /* [in] */IInputStream* s, 
        /* [in] */Int32 length);

    CARAPI SetBinaryStreamEx(
        /* [in] */String parameterName,
        /* [in] */IInputStream* s, 
        /* [in] */Int32 length);

    CARAPI SetObjectEx3(
        /* [in] */String parameterName, 
        /* [in] */IInterface* val, 
        /* [in] */Int32 targetSqlType,
        /* [in] */Int32 scale);

    CARAPI SetObjectEx4(
        /* [in] */String parameterName, 
        /* [in] */IInterface* val, 
        /* [in] */Int32 targetSqlType);

    CARAPI SetObjectEx5(
        /* [in] */String parameterName, 
        /* [in] */IInterface* val);

    CARAPI SetCharacterStreamEx(
        /* [in] */String parameterName,
        /* [in] */IReader* r, 
        /* [in] */Int32 length);

    CARAPI SetDateEx2(
        /* [in] */String parameterName, 
        /* [in] */IDate* val,
        /* [in] */ICalendar* cal);

    CARAPI SetTimeEx2(
        /* [in] */String parameterName, 
        /* [in] */ITime* val,
        /* [in] */ICalendar* cal);

    CARAPI SetTimestampEx2(
        /* [in] */String parameterName, 
        /* [in] */ITimestamp* val,
        /* [in] */ICalendar* cal);

    CARAPI SetNullEx3(
        /* [in] */String parameterName, 
        /* [in] */Int32 sqlType, 
        /* [in] */String typeName);

    CARAPI GetString(
        /* [in] */String parameterName,
        /* [out] */String* result);

    CARAPI GetBoolean(
        /* [in] */String parameterName,
        /* [out] */Boolean* result);

    CARAPI GetByte(
        /* [in] */String parameterName,
        /* [out] */Byte* result);

    CARAPI GetShort(
        /* [in] */String parameterName,
        /* [out] */Int16* result);

    CARAPI GetInt(
        /* [in] */String parameterName,
        /* [out] */Int32* result) ;

    CARAPI GetLong(
        /* [in] */String parameterName,
        /* [out] */Int64* result);

    CARAPI GetFloat(
        /* [in] */String parameterName,
        /* [out] */Float* result);

    CARAPI GetDouble(
        /* [in] */String parameterName,
        /* [out] */Double* result);

    CARAPI GetBytes(
        /* [in] */String parameterName,
        /* [out] */ArrayOf<Byte>* bytes);

    CARAPI GetDate(
        /* [in] */String parameterName,
        /* [out] */IDate** date);

    CARAPI GetTime(
        /* [in] */String parameterName,
        /* [out] */ITime** time);

    CARAPI GetTimestamp(
        /* [in] */String parameterName,
        /* [out] */ITimestamp** stamp);

    CARAPI GetObject(
        /* [in] */String parameterName,
        /* [out] */IInterface** obj);

    CARAPI GetObjectEx(
        /* [in] */Int32 parameterIndex,
        /* [out] */IInterface** obj);

    CARAPI GetBigDecimal(
        /* [in] */String parameterName,
        /* [out] */IBigDecimal** result);

//    Object GetObjectEx2(
//        /* [in] */String parameterName, 
//        /* [in] */Map map);

//    Object GetObjectEx3(
//        /* [in] */Int32 parameterIndex, 
 //       /* [in] */Map map);

    CARAPI GetRef(
        /* [in] */Int32 parameterIndex,
        /* [out] */IRef** ref);

    CARAPI GetRefEx(
        /* [in] */String parameterName,
        /* [out] */IRef** ref);

    CARAPI GetBlob(
        /* [in] */String parameterName,
        /* [out] */IBlob** blob);

    CARAPI GetBlobEx(
        /* [in] */Int32 parameterIndex,
        /* [out] */IBlob** blob);

    CARAPI GetClob(
        /* [in] */String parameterName,
        /* [out] */IClob** clob);

    CARAPI GetClobEx(
        /* [in] */Int32 parameterIndex,
        /* [out] */IClob** clob);

    CARAPI GetArray(
        /* [in] */String parameterName,
        /* [out] */IArray** array);

    CARAPI GetArrayEx(
        /* [in] */Int32 parameterIndex,
        /* [out] */IArray** array);

    CARAPI GetDate(
        /* [in] */String parameterName, 
        /* [in] */ICalendar* cal,
        /* [out] */IDate** date);

    CARAPI GetDateEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ICalendar* cal,
        /* [out] */IDate** date);

    CARAPI GetTime(
        /* [in] */String parameterName, 
        /* [in] */ICalendar* cal,
        /* [out] */ITime** time);

    CARAPI GetTimeEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ICalendar* cal,
        /* [out] */ITime** time);

    CARAPI GetTimestamp(
        /* [in] */String parameterName,
        /* [in] */ ICalendar* cal,
        /* [out] */ITimestamp** tamp);

    CARAPI GetTimestampEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ICalendar* cal.
        /* [out] */ITimestamp** tamp);

    CARAPI GetURL(
        /* [in] */String parameterName,
        /* [out] */IURL** url);

    CARAPI SetRowId(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IRowId* x);

    CARAPI SetRowId(
        /* [in] */String parameterName, 
        /* [in] */IRowId* x);

    CARAPI SetNString(
        /* [in] */Int32 parameterIndex, 
        /* [in] */String value);

    CARAPI SetNString(
        /* [in] */String parameterName, 
        /* [in] */String value);

    CARAPI SetNCharacterStream(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI SetNCharacterStreamEx(
        /* [in] */String parameterName, 
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI SetNClob(
        /* [in] */Int32 parameterIndex, 
        /* [in] */INClobI value);

    CARAPI SetNClobEx(
        /* [in] */String parameterName, 
        /* [in] */INClob* value);

    CARAPI SetClobEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReader* x, 
        /* [in] */Int64 len);

    CARAPI SetClobEx2(
        /* [in] */String parameterName, 
        /* [in] */IReader* x, 
        /* [in] */Int64 len);

    CARAPI SetBlobEx(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x, 
        /* [in] */Int64 len);

    CARAPI SetBlobEx2(
        /* [in] */String parameterName, 
        /* [in] */IInputStream* x, 
        /* [in] */Int64 len);

    CARAPI SetNClobEx2(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReaderI x, 
        /* [in] */Int64 len);

    CARAPI SetNClobEx3(
        /* [in] */String parameterName,
        /* [in] */IReader* x, 
        /* [in] */Int64 len);

    CARAPI SetSQLXML(
        /* [in] */Int32 parameterIndex, 
        /* [in] */ISQLXML* xml);

    CARAPI SetSQLXMLEx(
        /* [in] */String parameterName, 
        /* [in] */ISQLXML* xml);

    CARAPI SetAsciiStreamEx2(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI SetAsciiStreamEx3(
        /* [in] */String parameterName, 
        /* [in] */IInputStreamO x,
        /* [in] */Int64 len);

    CARAPI SetBinaryStreamEx2(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI SetBinaryStreamEx3(
        /* [in] */String parameterName, 
        /* [in] */IInputStream* x,
        /* [in] */Int64 len);

    CARAPI SetCharacterStreamEx2(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReaderO x,
        /* [in] */Int64 len);

    CARAPI SetCharacterStreamEx3(
        /* [in] */String parameterName,
        /* [in] */IReader* x,
        /* [in] */Int64 len);

    CARAPI SetAsciiStreamEx4(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x);

    CARAPI SetAsciiStreamEx5(
        /* [in] */String parameterName, 
        /* [in] */IInputStreamO x);

    CARAPI SetBinaryStreamEx4(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x);

    CARAPI SetBinaryStreamEx5(
        /* [in] */String parameterName, 
        /* [in] */IInputStream* x);

    CARAPI SetCharacterStreamEx4(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReader* x);

    CARAPI SetCharacterStreamEx5(
        /* [in] */String parameterName,
        /* [in] */IReader* x);

    CARAPI SetNCharacterStream(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReader* x);

    CARAPI SetNCharacterStreamEx(
        /* [in] */String parameterName,
         /* [in] */IReader* x);

    CARAPI SetClobEx3(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReader* x);

    CARAPI SetClobEx4(
        /* [in] */String parameterName, 
        /* [in] */IReader* x);

    CARAPI SetBlobEx3(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IInputStream* x);

    CARAPI SetBlobEx4(
        /* [in] */String parameterName,
        /* [in] */IInputStream* x);

    CARAPI SetNClobEx4(
        /* [in] */Int32 parameterIndex, 
        /* [in] */IReader* x);

    CARAPI SetNClobEx5(
        /* [in] */String parameterName, 
        /* [in] */IReader* x);
};
#endif//_JDBCPREPAREDSTATEMENT_H_
