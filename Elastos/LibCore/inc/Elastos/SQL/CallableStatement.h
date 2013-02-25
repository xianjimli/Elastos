#ifndef _CALLABLESTATEMENT_H_
#define _CALLABLESTATEMENT_H_

#include <elastos.h>
#include "PreparedStatement.h"

using namespace Elastos;

class CallableStatement : public PreparedStatement{
public:
	/**
     * Gets the value of a specified JDBC {@code ARRAY} parameter as a
     * {@code java.sql.Array}.
     *
     * @param parameterIndex
     *            the parameter index, where the first parameter has
     *            index 1.
     * @return a {@code java.sql.Array} containing the parameter value.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetArray(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IArray** array)=0;

    /**
     * Gets the value of a specified JDBC {@code ARRAY} parameter as a {@code
     * java.sql.Array}.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return a {@code java.sql.Array} containing the parameter's value.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
    virtual CARAPI  GetArrayEx(
        /** [in] **/String parameterName,
        /** [out] **/IArray** array)=0;

    /**
     * Returns a new {@link BigDecimal} representation of the JDBC {@code
     * NUMERIC} parameter specified by the input index.
     *
     * @param parameterIndex
     *            the parameter number index where the first parameter has index
     *            1.
     * @return a {@code java.math.BigDecimal} representing the value of the
     *         specified parameter. The value {@code null} is returned if
     *         the parameter in question is an SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetBigDecimal(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IBigDecimal** decimal)=0;

    /**
     * Returns a new {@link BigDecimal} representation of the JDBC {@code
     * NUMERIC} parameter specified by the input index. The number of digits
     * after the decimal point is specified by {@code scale}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @param scale
     *            the number of digits after the decimal point to get.
     * @return a {@code java.math.BigDecimal} representing the value of the
     *         specified parameter. The value {@code null} is returned if
     *         the parameter in question is an SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @deprecated Use {@link #getBigDecimal(int)} or
     *             {@link #getBigDecimal(String)}
     */
    //@Deprecated
    virtual CARAPI  GetBigDecimalEx(
        /** [in] **/Int32 parameterIndex, 
        /** [in] **/Int32 scale,
        /** [out] **/IBigDecimal** decimal)=0;

    /**
     * Returns a new {@link BigDecimal} representation of the JDBC {@code
     * NUMERIC} parameter specified by the input name.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return a {@code java.math.BigDecimal} representing the value of the
     *         specified parameter. The value {@code null} is returned if
     *         the parameter in question is an SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI   GetBigDecimalEx2(
        /** [in] **/String parameterName,
        /** [out] **/IBigDecimal** decimal)=0;

    /**
     * Gets the value of a specified JDBC {@code BLOB} parameter as a {@code
     * java.sql.Blob}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return a {@code java.sql.Blob} representing the value of the
     *         specified parameter. The value {@code null} is returned if
     *         the parameter in question is an SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetBlob(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IBlob** blob)=0;

    /**
     * Gets the value of a specified JDBC {@code BLOB} parameter as a {@code
     * java.sql.Blob}.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return a {@code java.sql.Blob} representing the value of the
     *         specified parameter. The value {@code null} is returned if
     *         the parameter in question is an SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetBlobEx(
        /** [in] **/String parameterName,
        /** [out] **/IBlob** blob)=0;

    /**
     * Gets the value of a specified JDBC {@code BIT} parameter as a boolean.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return a {@code boolean} representing the parameter value. {@code false}
     *            is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetBoolean(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/boolean** result)=0;

    /**
     * Gets the value of a specified JDBC {@code BIT} parameter as a {@code
     * boolean}.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return a {@code boolean} representation of the value of the parameter.
     *         {@code false} is returned if the SQL value is {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetBooleanEx(
        /** [in] **/String parameterName,
        /** [out] **/boolean** result)=0;

    /**
     * Gets the value of a specified JDBC {@code TINYINT} parameter as a {@code
     * byte}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return a {@code byte} representation of the value of the parameter.
     *            {@code 0} is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetByte(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/Byte* result)=0;

    /**
     * Gets the value of a specified JDBC {@code TINYINT} parameter as a Java
     * {@code byte}.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return a {@code byte} representation of the value of the parameter.
     *         {@code 0} is returned if the SQL value is {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetByteEx(
        /** [in] **/String parameterName,
        /** [out] **/Byte* result)=0;

    /**
     * Returns a byte array representation of the indexed JDBC {@code BINARY} or
     * {@code VARBINARY} parameter.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return an array of bytes giving the value of the parameter. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetBytesEx2(
        /** [in] **/Int32 parameterIndex,
        /** [out,callee] **/ArrayOf<Byte>* bytes)=0;

    /**
     * Returns a byte array representation of the named JDBC {@code BINARY} or
     * {@code VARBINARY} parameter.
     *
     * @param parameterName
     *            the name of the parameter.
     * @return an array of bytes giving the value of the parameter. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetBytesEx3(
        /** [in] **/String parameterName,
        /** [out,callee] **/ArrayOf<Byte>* bytes)=0;

    /**
     * Gets the value of a specified JDBC {@code CLOB} parameter as a {@code
     * java.sql.Clob}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return a {@code java.sql.Clob} representing the value of the
     *            parameter. {@code null} is returned if the value is SQL
     *            {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Clob
     */
    virtual CARAPI  GetClob(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IClob** clob)=0;

    /**
     * Gets the value of a specified JDBC {@code CLOB} parameter as a {@code
     * java.sql.Clob}.
     *
     * @param parameterName
     *            the name of the parameter.
     * @return a {@code java.sql.Clob} with the value of the parameter. {@code
     *         null} is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Clob
     */
    virtual CARAPI  GetClobEx(
        /** [in] **/String parameterName,
        /** [out] **/IClob** clob)=0;

    /**
     * Gets the value of the specified JDBC {@code DATE} parameter as a {@code
     * java.sql.Date}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the {@code java.sql.Date} representing the parameter's value.
     *         {@code null} is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Date
     */
    virtual CARAPI  GetDate(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of the specified JDBC {@code DATE} parameter as a {@code
     * java.sql.Date}, using the specified {@code Calendar} to construct the date.
     * <p>
     * The JDBC driver uses the calendar to create the Date using a particular
     * timezone and locale. The default behavior of the driver is to use the Java
     * virtual CARAPI  machine default settings.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @param cal
     *            the {@code Calendar} to use to construct the date
     * @return the {@code java.sql.Date} giving the parameter's value. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Date
     */
    virtual CARAPI  GetDateEx(
        /** [in] **/Int32 parameterIndex,
        /** [in] **/ICalendar* cal,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of the specified JDBC {@code DATE} parameter as a {@code
     * java.sql.Date}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return the {@code java.sql.Date} giving the parameter's value. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Date
     */
    virtual CARAPI  GetDateEx2(
        /** [in] **/String parameterName,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of the specified JDBC {@code DATE} parameter as a {@code
     * java.sql.Date}, using the specified {@code Calendar} to construct the date.
     * <p>
     * The JDBC driver uses the calendar to create the date using a particular
     * timezone and locale. The default behavior of the driver is to use the Java
     * virtual CARAPI  machine default settings.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @param cal
     *            used for creating the returned {@code Date}.
     * @return the {@code java.sql.Date} giving the parameter's value. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Date
     */
    virtual CARAPI  GetDateEx3(
        /** [in] **/String parameterName,
        /** [in] **/ICalendar* cal,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of the specified JDBC {@code DOUBLE} parameter as a
     * {@code double}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the parameter's value as a {@code double}. {@code 0.0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetDouble(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the value of the specified JDBC {@code DOUBLE} parameter as a
     * {@code double}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return the parameter's value as a {@code double}. {@code 0.0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
    virtual CARAPI  GetDoubleEx(
        /** [in] **/String parameterName,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the value of the specified JDBC {@code FLOAT} parameter as a {@code
     * float}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the parameter's value as a {@code float}. {@code 0.0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetFloat(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/Float* result)=0;

    /**
     * Gets the value of the specified JDBC {@code FLOAT} parameter as a Java
     * {@code float}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return the parameter's value as a {@code float}. {@code 0.0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
    virtual CARAPI  GetFloatEx(
        /** [in] **/String parameterName,
        /** [out] **/Float* result)=0;

    /**
     * Gets the value of the specified JDBC {@code INTEGER} parameter as an
     * {@code int}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the {@code int} giving the parameter's value. {@code 0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetInt(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/Int32* result)=0;

    /**
     * Gets the value of the specified JDBC {@code INTEGER} parameter as an
     * {@code int}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return the {@code int} giving the parameter's value. {@code 0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetIntEx(
        /** [in] **/String parameterName,
        /** [out] **/Int32* result)=0;

    /**
     * Gets the value of the specified JDBC {@code BIGINT} parameter as a
     * {@code long}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the {@code long} giving the parameter's value. {@code 0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetLong(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the value of the specified JDBC {@code BIGINT} parameter as a
     * {@code long}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return the {@code long} giving the parameter's value. {@code 0}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetLongEx(
        /** [in] **/String parameterName,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the value of the specified parameter as a Java {@code Object}.
     * <p>
     * The object type returned is the JDBC type registered for the parameter
     * with a {@code registerOutParameter} call. If a parameter was registered
     * as a {@code java.sql.Types.OTHER} then it may hold abstract types that
     * are particular to the connected database.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return an Object holding the value of the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetObject(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IInterface** obj)=0;

    /**
     * Gets the value of the specified parameter as an {@code Object}. The
     * {@code Map} gives the correspondence between SQL types and Java classes.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @param map
     *            the {@code Map} giving the correspondence between SQL
     *            types and Java classes.
     * @return an Object holding the value of the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
//    virtual CARAPI  Object GetObjectEx(int parameterIndex, Map<String, Class<?>> map)=0;

    /**
     * Gets the value of the specified parameter as an {@code Object}.
     * <p>
     * The object type returned is the JDBC type that was registered for
     * the parameter by an earlier call to {@link #registerOutParameter}.
     * If a parameter was registered as a {@code java.sql.Types.OTHER}
     * then it may hold abstract types that are particular to the
     * connected database.
     *
     * @param parameterName
     *            the parameter name.
     * @return the Java {@code Object} representation of the value of the
     *         parameter.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
    virtual CARAPI  GetObjectEx2(
        /** [in] **/String parameterName,
        /** [out] **/IInterface** obj)=0;

    /**
     * Gets the value of a specified parameter as an {@code Object}. The
     * actual return type is determined by the {@code Map} parameter which
     * gives the correspondence between SQL types and Java classes.
     *
     * @param parameterName
     *            the parameter name.
     * @param map
     *            the {@code Map} of SQL types to their Java counterparts
     * @return an {@code Object} holding the value of the parameter.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
//    virtual CARAPI  Object GetObjectEx3(String parameterName, Map<String, Class<?>> map)=0;

    /**
     * Gets the value of a specified SQL {@code REF(<structured type>)}
     * parameter as a {@code java.sql.Ref}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return a {@code java.sql.Ref} with the parameter value. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetRef(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IRef** result)=0;

    /**
     * Gets the value of a specified SQL {@code REF(<structured type>)}
     * parameter as a {@code java.sql.Ref}.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return the parameter's value in the form of a {@code
     *         java.sql.Ref}. A {@code null} reference is returned if the
     *         parameter's value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a problem accessing the database.
     * @see Ref
     */
    virtual CARAPI  GetRefEx(
        /** [in] **/String parameterName,
        /** [out] **/IRef** result)=0;

    /**
     * Gets the value of a specified JDBC {@code SMALLINT} parameter as a
     * {@code short}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the parameter's value as a {@code short}. 0 is returned
     *         if the parameter's value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  GetShort(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/Int16* result)=0;

    /**
     * Gets the value of a specified JDBC {@code SMALLINT} parameter as a
     * {@code short}.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return the parameter's value as a {@code short}. 0 is returned
     *         if the parameter's value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
    virtual CARAPI  GetShortEx(
        /** [in] **/String parameterName,
        /** [out] **/Int16* result)=0;

    /**
     * Returns the indexed parameter's value as a {@code String}. The
     * parameter value must be one of the JDBC types {@code CHAR},
     * {@code VARCHAR} or {@code LONGVARCHAR}.
     * <p>
     * The {@code String} corresponding to a {@code CHAR} of fixed length
     * will be of identical length to the value in the database inclusive
     * of padding characters.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the parameter's value as a {@code String}. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
    virtual CARAPI  GetString(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/String result)=0;

    /**
     * Returns the named parameter's value as a string. The parameter value must
     * be one of the JDBC types {@code CHAR}, {@code VARCHAR} or {@code
     * LONGVARCHAR}.
     * <p>
     * The string corresponding to a {@code CHAR} of fixed length will be of
     * identical length to the value in the database inclusive of padding
     * characters.
     *
     * @param parameterName
     *            the desired parameter's name.
     * @return the parameter's value as a {@code String}. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a problem accessing the database.
     */
    virtual CARAPI  GetStringEx(
        /** [in] **/String parameterName,
        /** [out] **/String result)=0;

    /**
     * Gets the value of a specified JDBC {@code TIME} parameter as a {@code
     * java.sql.Time}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return the parameter's value as a {@code java.sql.Time}.
     *         {@code null} is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Time
     */
    virtual CARAPI  GetTime(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/ITime** time)=0;

    /**
     * Gets the value of a specified JDBC {@code TIME} parameter as a {@code
     * java.sql.Time}, using the supplied {@code Calendar} to construct the
     * time. The JDBC driver uses the calendar to handle specific timezones
     * and locales in order to determine {@code Time}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @param cal
     *            the calendar to use in constructing {@code Time}.
     * @return the parameter's value as a {@code java.sql.Time}.
     *         {@code null} is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Time
     * @see java.util.Calendar
     */
    virtual CARAPI  GetTimeEx(
        /** [in] **/Int32 parameterIndex, 
        /** [in] **/ICalendar* cal,
        /** [out] **/ITime** time)=0;

    /**
     * Gets the value of a specified JDBC {@code TIME} parameter as a {@code
     * java.sql.Time}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return a new {@code java.sql.Time} with the parameter's value. A {@code
     *         null} reference is returned for an SQL value of {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Time
     */
    virtual CARAPI  GetTimeEx2(
        /** [in] **/String parameterName,
        /** [out] **/ITime** time)=0;

    /**
     * Gets the value of a specified JDBC {@code TIME} parameter as a {@code
     * java.sql.Time}, using the supplied {@code Calendar} to construct
     * the time. The JDBC driver uses the calendar to handle specific
     * timezones and locales when creating {@code Time}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @param cal
     *            used for creating the returned {@code Time}
     * @return a new {@code java.sql.Time} with the parameter's value. A {@code
     *         null} reference is returned for an SQL value of {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Time
     * @see java.util.Calendar
     */
    virtual CARAPI  GetTimeEx3(
    	/** [in] **/String parameterName,
    	/** [in] **/ICalendar* cal,
    	/** [out] **/ITime** time)=0;

    /**
     * Returns the indexed parameter's {@code TIMESTAMP} value as a {@code
     * java.sql.Timestamp}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1
     * @return the parameter's value as a {@code java.sql.Timestamp}. A
     *         {@code null} reference is returned for an SQL value of {@code
     *         NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Timestamp
     */
    virtual CARAPI  GetTimestamp(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/ITimestamp* result)=0;

    /**
     * Returns the indexed parameter's {@code TIMESTAMP} value as a {@code
     * java.sql.Timestamp}. The JDBC driver uses the supplied {@code Calendar}
     * to handle specific timezones and locales when creating the result.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1
     * @param cal
     *            used for creating the returned {@code Timestamp}
     * @return the parameter's value as a {@code java.sql.Timestamp}. A
     *         {@code null} reference is returned for an SQL value of {@code
     *         NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Timestamp
     */
    virtual CARAPI  GetTimestampEx(
        /** [in] **/Int32 parameterIndex,
        /** [in] **/ICalendar* cal,
        /** [out] **/ITimestamp* result)=0;

    /**
     * Returns the named parameter's {@code TIMESTAMP} value as a {@code
     * java.sql.Timestamp}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return the parameter's value as a {@code java.sql.Timestamp}. A
     *         {@code null} reference is returned for an SQL value of {@code
     *         NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Timestamp
     */
    virtual CARAPI  GetTimestampEx2(
        /** [in] **/String parameterName,
        /** [out] **/ITimestamp* result)=0;

    /**
     * Returns the indexed parameter's {@code TIMESTAMP} value as a {@code
     * java.sql.Timestamp}. The JDBC driver uses the supplied {@code Calendar}
     * to handle specific timezones and locales when creating the result.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @param cal
     *            used for creating the returned {@code Timestamp}
     * @return the parameter's value as a {@code java.sql.Timestamp}. A
     *         {@code null} reference is returned for an SQL value of {@code
     *         NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Timestamp
     */
    virtual CARAPI  GetTimestampEx3(
        /** [in] **/String parameterName,
        /** [in] **/ICalendar* cal,
        /** [out] **/ITimestamp* result)=0;

    /**
     * Gets the value of a specified JDBC {@code DATALINK} parameter as a
     * {@code java.net.URL}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @return a {@code URL} giving the parameter's value. {@code null}
     *         is returned if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see java.net.URL
     */
    virtual CARAPI  GetURL(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IURL** url)=0;

    /**
     * Returns the named parameter's JDBC {@code DATALINK} value in a new Java
     * {@code java.net.URL}.
     *
     * @param parameterName
     *            the name of the desired parameter.
     * @return a new {@code java.net.URL} encapsulating the parameter value. A
     *         {@code null} reference is returned for an SQL value of {@code
     *         NULL}.
     * @throws SQLException
     *             if a database error occurs.
     * @see java.net.URL
     */
    virtual CARAPI  GetURLEx(
        /** [in] **/String parameterName,
        /** [out] **/IURL** url)=0;

    /**
     * Defines the type of a specified {@code OUT} parameter. All {@code OUT}
     * parameters must have their type defined before a stored procedure is
     * executed.
     * <p>
     * The type supplied in the {@code sqlType} parameter fixes the
     * type that will be returned by the getter methods of
     * {@code CallableStatement}.
     * If a database specific type is expected for a parameter, the Type {@code
     * java.sql.Types.OTHER} should be used. Note that there is another variant
     * of this method for User Defined Types or a {@code REF} type.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1
     * @param sqlType
     *            the JDBC type as defined by {@code java.sql.Types}. The JDBC
     *            types {@code NUMERIC} and {@code DECIMAL} should be defined
     *            using {@link #registerOutParameter(int, int, int)}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Types
     */
    virtual CARAPI  RegisterOutParameter(
        /** [in] **/Int32 parameterIndex,
        /** [in] **/Int32 sqlType)=0;

    /**
     * Defines the Type of a specified {@code OUT} parameter. All {@code OUT}
     * parameters must have their type defined before a stored procedure is
     * executed. This version of the {@code registerOutParameter} method, which
     * has a scale parameter, should be used for the JDBC types {@code NUMERIC}
     * and {@code DECIMAL}, where there is a need to specify the number of
     * digits expected after the decimal point.
     * <p>
     * The type supplied in the {@code sqlType} parameter fixes the
     * type that will be returned by the getter methods of
     * {@code CallableStatement}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1
     * @param sqlType
     *            the JDBC type as defined by {@code java.sql.Types}.
     * @param scale
     *            the number of digits after the decimal point. Must be greater
     *            than or equal to 0.
     * @throws SQLException
     *             if a database error occurs.
     * @see Types
     */
    virtual CARAPI  RegisterOutParameterEx(
        /** [in] **/Int32 parameterIndex, 
        /** [in] **/Int32 sqlType, 
        /** [in] **/Int32 scale)=0;

    /**
     * Defines the Type of a specified {@code OUT} parameter. This variant
     * of the method is designed for use with parameters that are
     * <i>User Defined Types</i> (UDT) or a {@code REF} type, although it
     * can be used for any type.
     *
     * @param paramIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @param sqlType
     *            a JDBC type expressed as a constant from {@link Types}.
     * @param typeName
     *            an SQL type name. For a {@code REF} type, this name should be
     *            the fully qualified name of the referenced type.
     * @throws SQLException
     *             if a database error occurs.
     * @see Ref
     */
    virtual CARAPI  RegisterOutParameterEx2(
        /** [in] **/Int32 paramIndex, 
        /** [in] **/Int32 sqlType,
        /** [in] **/String typeName)=0;

    /**
     * Defines the Type of a specified {@code OUT} parameter. All OUT parameters
     * must have their Type defined before a stored procedure is executed.
     * <p>
     * The type supplied in the {@code sqlType} parameter fixes the
     * type that will be returned by the getter methods of
     * {@code CallableStatement}.
     * If a database-specific type is expected for a parameter, the Type {@code
     * java.sql.Types.OTHER} should be used. Note that there is another variant
     * of this method for User Defined Types or a {@code REF} type.
     *
     * @param parameterName
     *            the parameter name.
     * @param sqlType
     *            a JDBC type expressed as a constant from {@link Types}. Types
     *            {@code NUMERIC} and {@code DECIMAL} should be defined using
     *            the variant of this method that takes a {@code scale}
     *            parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  RegisterOutParameterEx3(
        /** [in] **/String parameterName, 
        /** [in] **/Int32 sqlType)=0;

    /**
     * Defines the Type of a specified {@code OUT} parameter. All {@code OUT}
     * parameters must have their Type defined before a stored procedure is
     * executed. This version of the {@code registerOutParameter} method, which
     * has a scale parameter, should be used for the JDBC types {@code NUMERIC}
     * and {@code DECIMAL}, where there is a need to specify the number of
     * digits expected after the decimal point.
     * <p>
     * The type supplied in the {@code sqlType} parameter fixes the
     * type that will be returned by the getter methods of
     * {@code CallableStatement}.
     *
     * @param parameterName
     *            the parameter name.
     * @param sqlType
     *            a JDBC type expressed as a constant from {@link Types}.
     * @param scale
     *            the number of digits after the decimal point. Must be greater
     *            than or equal to 0.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  RegisterOutParameterEx4(
        /** [in] **/String parameterName,
        /** [in] **/Int32 sqlType,
        /** [in] **/Int32 scale)=0;

    /**
     * Defines the Type of a specified {@code OUT} parameter. This variant of
     * the method is designed for use with parameters that are <i>User Defined
     * Types</i> (UDT) or a {@code REF} type, although it can be used for any
     * type.
     *
     * @param parameterName
     *            the parameter name
     * @param sqlType
     *            a JDBC type expressed as a constant from {@link Types}
     * @param typeName
     *            the fully qualified name of an SQL structured type. For a
     *            {@code REF} type, this name should be the fully qualified name
     *            of the referenced type.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  RegisterOutParameterEx5(
        /** [in] **/String parameterName, 
        /** [in] **/Int32 sqlType,
        /** [in] **/String typeName)=0;

    /**
     * Sets the value of a specified parameter to the content of a supplied
     * {@code InputStream}, which has a specified number of bytes.
     * <p>
     * This is a good method for setting an SQL {@code LONGVARCHAR} parameter
     * where the length of the data is large. Data is read from the {@code
     * InputStream} until end-of-file is reached or the specified number of
     * bytes is copied.
     *
     * @param parameterName
     *            the parameter name
     * @param theInputStream
     *            the ASCII input stream carrying the data to update the
     *            parameter with.
     * @param length
     *            the number of bytes in the {@code InputStream} to copy to the
     *            parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetAsciiStream(
        /** [in] **/String parameterName,
        /** [in] **/IInputStream* theInputStream, 
        /** [in] **/Int32 length)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code
     * java.math.BigDecimal} value.
     *
     * @param parameterName
     *            the name of the parameter.
     * @param theBigDecimal
     *            the {@code java.math.BigInteger} value to set.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetBigDecimal(
        /** [in] **/String parameterName,
        /** [in] **/IBigDecimal* theBigDecimal)=0;

    /**
     * Sets the value of a specified parameter to the content of a supplied
     * binary {@code InputStream}, which has a specified number of bytes.
     * <p>
     * Use this method when a large amount of data needs to be set into a
     * {@code LONGVARBINARY} parameter.
     *
     * @param parameterName
     *            the name of the parameter.
     * @param theInputStream
     *            the binary {@code InputStream} carrying the data to update the
     *            parameter.
     * @param length
     *            the number of bytes in the {@code InputStream} to copy to the
     *            parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetBinaryStream(
        /** [in] **/String parameterName,
        /** [in] **/IInputStream* theInputStream, 
        /** [in] **/Int32 length)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code boolean}
     * value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theBoolean
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetBoolean(
        /** [in] **/String parameterName, 
        /** [in] **/Boolean theBoolean)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code byte} value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theByte
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetByte(
        /** [in] **/String parameterName, 
        /** [in] **/Byte theByte)=0;

    /**
     * Sets the value of a specified parameter to a supplied array of bytes. The
     * array is mapped to {@code VARBINARY} or else {@code LONGVARBINARY} in the
     * connected database.
     *
     * @param parameterName
     *            the parameter name.
     * @param theBytes
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetBytes(
        /** [in] **/String parameterName, 
        /** [in] **/ArrayOf<Byte> theBytes)=0;

    /**
     * Sets the value of a specified parameter to the character content of a
     * {@code Reader} object, with the specified length of character data.
     *
     * @param parameterName
     *            the parameter name.
     * @param reader
     *            the new value with which to update the parameter.
     * @param length
     *            a count of the characters contained in {@code reader}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetCharacterStream(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* reader,
        /** [in] **/Int32 length)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code
     * java.sql.Date} value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theDate
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetDate(
        /** [in] **/String parameterName, 
        /** [in] **/IDate* theDate)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code
     * java.sql.Date} value, using a supplied calendar to map the date. The
     * calendar allows the application to control the timezone used to compute
     * the SQL {@code DATE} in the database. In case that no calendar is
     * supplied, the driver uses the default timezone of the Java virtual CARAPI 
     * machine.
     *
     * @param parameterName
     *            the parameter name.
     * @param theDate
     *            the new value with which to update the parameter.
     * @param cal
     *            a {@code Calendar} to use to construct the SQL {@code DATE}
     *            value.
     * @throws SQLException
     *             if a database error occurs.
     * @see java.util.Calendar
     * @see Date
     */
    virtual CARAPI  SetDateEx(
        /** [in] **/String parameterName, 
        /** [in] **/IDate* theDate, 
        /** [in] **/ICalendar* cal)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code double}
     * value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theDouble
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetDouble(
        /** [in] **/String parameterName, 
        /** [in] **/Int64 theDouble)=0;

    /**
     * Sets the value of a specified parameter to to a supplied {@code float}
     * value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theFloat
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetFloat(
        /** [in] **/String parameterName, 
        /** [in] **/Float theFloat)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code int} value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theInt
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetInt(
        /** [in] **/String parameterName, 
        /** [in] **/Int32 theInt)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code long} value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theLong
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetLong(
        /** [in] **/String parameterName, 
        /** [in] **/Int64 theLong)=0;

    /**
     * Sets the value of a specified parameter to SQL {@code NULL}. Don't use
     * this version of {@code setNull} for <i>User Defined Types</i> (UDT) or
     * for {@code REF} type parameters.
     *
     * @param parameterName
     *            the parameter name.
     * @param sqlType
     *            a JDBC type expressed as a constant from {@link Types}.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetNull(
        /** [in] **/String parameterName, 
        /** [in] **/Int32 sqlType)=0;

    /**
     * Sets the value of a specified parameter to be SQL {@code NULL} where the
     * parameter type is either {@code REF} or user defined (e.g. {@code STRUCT}
     * , {@code JAVA_OBJECT} etc).
     * <p>
     * For reasons of portability, the caller is expected to supply both the SQL
     * type code and type name (which is just the parameter name if the type is
     * user defined, referred to as a {@code UDT}, or the name of the referenced
     * type in case of a {@code REF} type).
     *
     * @param parameterName
     *            the parameter name.
     * @param sqlType
     *            a JDBC type expressed as a constant from {@link Types}.
     * @param typeName
     *            if the target parameter is a user defined type then this
     *            should contain the full type name. The fully qualified name of
     *            a {@code UDT} or {@code REF} type is ignored if the parameter
     *            is not a {@code UDT}.
     * @throws SQLException
     *             if a database error occurs.
     * @see Types
     */
    virtual CARAPI  SetNullEx(
        /** [in] **/String parameterName, 
        /** [in] **/Int32 sqlType, 
        /** [in] **/String typeName)=0;

    /**
     * Sets the value of a specified parameter using a supplied object. Prior to
     * issuing this request to the connected database {@code theObject} is
     * transformed to the corresponding SQL type according to the standard Java
     * to SQL mapping rules.
     * <p>
     * If the object's class implements the interface {@code SQLData}, the JDBC
     * driver calls {@code SQLData.writeSQL} to write it to the SQL data stream.
     * If {@code theObject} implements any of the following interfaces then the
     * driver is in charge of mapping the value to the appropriate SQL type.
     * <ul><li>{@link Ref}</li>
     * <li>{@link Struct}</li>
     * <li>{@link Array}</li>
     * <li>{@link Clob}</li>
     * <li>{@link Blob}</li> </ul>
     *
     * @param parameterName
     *            the parameter name
     * @param theObject
     *            the new value with which to update the parameter
     * @throws SQLException
     *             if a database error occurs.
     * @see SQLData
     */
    virtual CARAPI  SetObject(
        /** [in] **/String parameterName, 
        /** [in] **/IInterfaces* theObject)=0;

    /**
     * Sets the value of a specified parameter using a supplied object.
     * <p>
     * The parameter {@code theObject} is converted to the given {@code
     * targetSqlType} before it is sent to the database. If the object has a
     * custom mapping (its class implements the interface {@code SQLData}), the
     * JDBC driver calls the method {@code SQLData.writeSQL} to write it to the
     * SQL data stream. If {@code theObject} is an instance of one of the
     * following types
     * <ul>
     * <li>{@link Ref}</li>
     * <li>{@link Struct}</li>
     * <li>{@link Array}</li>
     * <li>{@link Clob}</li>
     * <li>{@link Blob}</li>
     * </ul>
     * then the driver is in charge of mapping the value to the appropriate
     * SQL type and deliver it to the database.
     *
     * @param parameterName
     *            the parameter name.
     * @param theObject
     *            the new value with which to update the parameter.
     * @param targetSqlType
     *            a JDBC type expressed as a constant from {@link Types}.
     * @throws SQLException
     *             if a database error occurs.
     * @see SQLData
     */
    virtual CARAPI  SetObjectEx(
        /** [in] **/String parameterName, 
        /** [in] **/IInterfaces* theObject,
        /** [in] **/Int32 targetSqlType)=0;

    /**
     * Sets the value of a specified parameter using a supplied object.
     * <p>
     * The object is converted to the given {@code targetSqlType} before it is
     * sent to the database. If the object has a custom mapping (its class
     * implements the interface {@code SQLData}), the JDBC driver calls the
     * method {@code SQLData.writeSQL} to write it to the SQL data stream. If
     * {@code theObject} implements any of the following interfaces
     * <ul>
     * <li>{@link Ref}</li>
     * <li>{@link Struct}</li>
     * <li>{@link Array}</li>
     * <li>{@link Clob}</li>
     * <li>{@link Blob}</li>
     * </ul>
     * then the driver is charge of mapping the value to the appropriate
     * SQL type.
     *
     * @param parameterName
     *            the parameter name.
     * @param theObject
     *            the new value with which to update the parameter.
     * @param targetSqlType
     *            a JDBC type expressed as a constant from {@link Types}.
     * @param scale
     *            where applicable, the number of digits after the decimal.
     *            point.
     * @throws SQLException
     *             if a database error occurs.
     * @see SQLData
     */
    virtual CARAPI  SetObjectEx2(
        /** [in] **/String parameterName, 
        /** [in] **/IInterfaces* theObject,
        /** [in] **/Int32 targetSqlType, 
        /** [in] **/Int32 scale)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code short}
     * value.
     *
     * @param parameterName
     *            the name of the parameter.
     * @param theShort
     *            a short value to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetShort(
        /** [in] **/String parameterName, 
        /** [in] **/Int16 theShort)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code String}.
     *
     * @param parameterName
     *            the name of the parameter.
     * @param theString
     *            a {@code String} value to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  SetString(
        /** [in] **/String parameterName, 
        /** [in] **/String theString)=0;

    /**
     * Sets the value of the parameter named {@code parameterName} to the value
     * of the supplied {@code java.sql.Time}.
     *
     * @param parameterName
     *            the parameter name.
     * @param theTime
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     * @see Time
     */
    virtual CARAPI  SetTime(
        /** [in] **/String parameterName, 
        /** [in] **/ITime* theTime)=0;

    /**
     * Sets the value of the parameter named {@code parameterName} to the value
     * of the supplied {@code java.sql.Time} using the supplied calendar.
     * <p>
     * The driver uses the supplied {@code Calendar} to create the SQL
     * {@code TIME} value, which allows it to use a custom timezone -
     * otherwise the driver uses the default timezone of the Java
     * virtual CARAPI  machine.
     *
     * @param parameterName
     *            the parameter name.
     * @param theTime
     *            the new value with which to update the parameter.
     * @param cal
     *            used for creating the new SQL {@code TIME} value.
     * @throws SQLException
     *             if a database error occurs.
     * @see Time
     */
    virtual CARAPI  SetTimeEx(
        /** [in] **/String parameterName, 
        /** [in] **/ITime* theTime, 
        /** [in] **/ICalendar* cal)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code
     * java.sql.Timestamp} value.
     *
     * @param parameterName
     *            the parameter name.
     * @param theTimestamp
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     * @see Timestamp
     */
    virtual CARAPI SetTimestamp(
        /** [in] **/String parameterName, 
        /** [in] **/ITimestamp* theTimestamp)=0;

    /**
     * Sets the value of a specified parameter to a supplied {@code
     * java.sql.Timestamp} value, using the supplied calendar.
     * <p>
     * The driver uses the supplied calendar to create the SQL {@code TIMESTAMP}
     * value, which allows it to use a custom timezone - otherwise the driver
     * uses the default timezone of the Java virtual CARAPI  machine.
     *
     * @param parameterName
     *            the parameter name.
     * @param theTimestamp
     *            the new value with which to update the parameter.
     * @param cal
     *            used for creating the new SQL {@code TIME} value.
     * @throws SQLException
     *             if a database error occurs.
     * @see Timestamp
     * @see java.util.Calendar
     */
    virtual CARAPI SetTimestampEx(
        /** [in] **/String parameterName, 
        /** [in] **/ITimestamp* theTimestamp,
        /** [in] **/ICalendar* cal)=0;

    /**
     * Sets the value of a specified parameter to the supplied {@code
     * java.net.URL}.
     *
     * @param parameterName
     *            the parameter name.
     * @param theURL
     *            the new value with which to update the parameter.
     * @throws SQLException
     *             if a database error occurs.
     * @see java.net.URL
     */
    virtual CARAPI SetURL(
        /** [in] **/String parameterName, 
        /** [in] **/IURL* theURL)=0;

    /**
     * Gets whether the value of the last {@code OUT} parameter read was SQL
     * {@code NULL}.
     *
     * @return true if the last parameter was SQL {@code NULL}, {@code false}
     *         otherwise.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WasNull(
        /** [out] **/Boolean* result)=0;

    /**
     * Gets the value of a specified {@code ROWID} parameter as a {@code
     * java.sql.RowId}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetRowId(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IRowId** id)=0;

    /**
     * Returns the value of the specified SQL ROWID parameter as a {@code
     * java.sql.RowId}.
     * @param parameterName the parameter name
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI GetRowIdEx(
        /** [in] **/String parameterName,
        /** [out] **/IRowId** id)=0;

    /**
     * Sets the named parameter to the given {@code rowId}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetRowId(
        /** [in] **/String parameterName, 
        /** [in] **/IRowId* rowId)=0;

    /**
     * Sets the named parameter to the given {@code string}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetNString(
        /** [in] **/String parameterName,
        /** [in] **/String string)=0;

    /**
     * Sets the named parameter to the characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetNCharacterStream(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Sets the named parameter to the given {@code nclob}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetNClob(
        /** [in] **/String parameterName, 
        /** [in] **/INClob* nclob)=0;

    /**
     * Sets the named parameter to the next {@code length} characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetClob(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Sets the named parameter to the next {@code length} bytes from the given {@code inputStream}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetBlob(
        /** [in] **/String parameterName, 
        /** [in] **/IInputStream* inputStream, 
        /** [in] **/Int64 length)=0;

    /**
     * Sets the named parameter to the next {@code length} characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetNClob(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Returns the value of the specified SQL NCLOB parameter as a {@code
     * java.sql.NClob}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetNClob(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/INClob** clob)=0;

    /**
     * Returns the value of the specified SQL NCLOB parameter as a {@code
     * java.sql.NClob}.
     * @param parameterName the parameter name
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI GetNClob(
        /** [in] **/String parameterName,
        /** [out] **/INClob** clob)=0;

    /**
     * Sets the named parameter to the given {@code sqlXml}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetSQLXML(
        /** [in] **/String parameterName, 
        /** [in] **/ISQLXML* sqlXml)=0;

    /**
     * Returns the value of the specified SQL XML parameter as a {@code
     * java.sql.SQLXML}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetSQLXML(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/ISQLXML** sqlXml)=0;

    /**
     * Returns the value of the specified SQL XML parameter as a {@code
     * java.sql.SQLXML}.
     * @param parameterName the parameter name
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI GetSQLXMLEx(
        /** [in] **/String parameterName,
        /** [out] **/ISQLXML** sqlXml)=0;

    /**
     * Returns the value of the specified SQL NCHAR, NVARCHAR, or LONGNVARCHAR parameter as a
     * {@code java.lang.String}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetNString(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/String* str)=0;

    /**
     * Returns the value of the specified SQL NCHAR, NVARCHAR, or LONGNVARCHAR parameter as a {@code
     * java.lang.String}.
     * @param parameterName the parameter name
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI GetNStringEx(
        /** [in] **/String parameterName,
        /** [out] **/String* str)=0;

    /**
     * Returns the value of the specified SQL NCHAR, NVARCHAR, or LONGNVARCHAR parameter
     * as a {@link Reader}.
     *
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetNCharacterStream(
        /** [in] **/Int32 parameterIndex,
        /** [out] **/IReader** stream)=0;

    /**
     * Returns the value of the specified SQL NCHAR, NVARCHAR, or LONGNVARCHAR parameter as a {@code
     * java.io.Reader}.
     * @param parameterName the parameter name
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI GetNCharacterStreamEx(
        /** [in] **/String parameterName,
        /** [out] **/IReader** stream)=0;

    /**
     * Returns the value of the specified parameter as a {@code java.io.Reader}.
     * @param parameterIndex
     *            the parameter number index, where the first parameter has
     *            index 1.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetCharacterStreamEx2(
        /** [in] **/Int32s parameterIndex,
        /** [out] **/IReader** stream)=0;

    /**
     * Returns the value of the specified parameter as a {@code java.io.Reader}.
     * @param parameterName the parameter name
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI GetCharacterStreamEx3(
        /** [in] **/String parameterName,
        /** [out] **/IReader** stream)=0;

    /**
     * Sets the named parameter to the given {@code blob}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetBlob(
        /** [in] **/String parameterName, 
        /** [in] **/IBlob* blob)=0;

    /**
     * Sets the named parameter to the given {@code clob}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetClob(
        /** [in] **/String parameterName, 
        /** [in] **/IClob* clob)=0;

    /**
     * Sets the named parameter to the next {@code length} bytes from the given {@code inputStream}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetAsciiStream(
        /** [in] **/String parameterName, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Sets the named parameter to the bytes from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetAsciiStreamEx(
        /** [in] **/String parameterName, 
        /** [in] **/IInputStream* x)=0;

    /**
     * Sets the named parameter to the next {@code length} bytes from the given {@code inputStream}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetBinaryStream(
        /** [in] **/String parameterName, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Sets the named parameter to the bytes from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetBinaryStreamEx(
        /** [in] **/String parameterName, 
        /** [in] **/IInputStream* x)=0;

    /**
     * Sets the named parameter to the next {@code length} characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetCharacterStream(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Sets the named parameter to the characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetCharacterStreamEx(
        /** [in] **/String parameterName,
        /** [in] **/IReader* reader)=0;

    /**
     * Sets the named parameter to the characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetNCharacterStream(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* value)=0;

    /**
     * Sets the named parameter to the characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetClob(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* reader)=0;

    /**
     * Sets the named parameter to the bytes from the given {@code inputStream}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetBlob(
        /** [in] **/String parameterName, 
        /** [in] **/IInputStream* inputStream)=0;

    /**
     * Sets the named parameter to the characters from the given {@code reader}.
     * @throws SQLException if a database error occurs
     */
    virtual CARAPI SetNClob(
        /** [in] **/String parameterName, 
        /** [in] **/IReader* reader)=0;
};
#endif //_CALLABLESTATEMENT_H_
