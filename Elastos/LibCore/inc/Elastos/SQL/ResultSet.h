#ifndef _RESULTSET_H_
#define _RESULTSET_H_

#include <elastos.h>
#include "Wrapper.h"

using namespace Elastos;

class ResultSet : public Wrapper{
public:
    /**
     * Moves the cursor to a specified row number in the {@code ResultSet}.
     *
     * @param row
     *            the index of the row starting at index 1. Index {@code -1}
     *            returns the last row.
     * @return {@code true} if the new cursor position is on the {@code
     *         ResultSet}, {@code false} otherwise.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI Absolute(
        /** [in] **/Int32 row,
        /** [out] **/ Boolean *result)=0;

    /**
     * Moves the cursor to the end of the {@code ResultSet}, after the last row.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI AfterLast()=0;

    /**
     * Moves the cursor to the start of the {@code ResultSet}, before the first
     * row.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI BeforeFirst()=0;

    /**
     * Cancels any updates made to the current row in the {@code ResultSet}.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI CancelRowUpdates()=0;

    /**
     * Clears all warnings related to this {@code ResultSet}.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI ClearWarnings()=0;

    /**
     * Releases this {@code ResultSet}'s database and JDBC resources. You are
     * strongly advised to use this method rather than relying on the release
     * being done when the {@code ResultSet}'s finalize method is called during
     * garbage collection process. Note that the {@code close()} method might
     * take some time to complete since it is dependent on the behavior of the
     * connection to the database and the database itself.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI Close()=0;

    /**
     * Deletes the current row from the {@code ResultSet} and from the
     * underlying database.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI DeleteRow()=0;

    /**
     * Gets the index number for a column in the {@code ResultSet} from the
     * provided column name.
     *
     * @param columnName
     *            the column name.
     * @return the column's index in the {@code ResultSet} identified by column
     *         name.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI FindColumn(
        /** [in] **/String columnName,
        /** [out] **/Int32* result)=0;

    /**
     * Shifts the cursor position to the first row in the {@code ResultSet}.
     *
     * @return {@code true} if the position is in a legitimate row, {@code
     *         false} if the {@code ResultSet} contains no rows.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI First(
        /** [out] **/Boolean* result)=0;

    /**
     * Gets the content of a column specified by column index in the current row
     * of this {@code ResultSet} as a {@code java.sql.Array}.
     *
     * @param columnIndex
     *            the index of the column to read
     * @return a {@code java.sql.Array} with the data from the column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetArray(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IArray** result)=0;

    /**
     * Gets the value of a column specified by column name as a {@code
     * java.sql.Array}.
     *
     * @param colName
     *            the name of the column to read.
     * @return a {@code java.sql.Array} with the data from the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetArrayEx(
        /** [in] **/String colName,
        /** [out] **/IArray** result)=0;

    /**
     * Gets the value of a column specified by column index as an ASCII
     * character stream.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return an {@code InputStream} with the data from the column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetAsciiStream(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IInputStream** stream)=0;

    /**
     * Gets the value of a column specified by column name as an ASCII character
     * stream.
     *
     * @param columnName
     *            the name of the column to read
     * @return an {@code InputStream} with the data from the column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetAsciiStreamEx(
        /** [in] **/String columnName,
        /** [out] **/IInputStream** stream)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.math.BigDecimal}.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code BigDecimal} with the value of the column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBigDecimal(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IBigDecimal** result)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.math.BigDecimal}.
     *
     * @deprecated use {@link #getBigDecimal(int)} or
     *             {@link #getBigDecimal(String)}
     * @param columnIndex
     *            the index of the column to read.
     * @param scale
     *            the number of digits after the decimal point
     * @return a {@code BigDecimal} with the value of the column.
     * @throws SQLException
     *             if a database error happens.
     */
    //@Deprecated
    virtual CARAPI BigDecimal GetBigDecimalEx(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Int32 scale,
        /** [out] **/IBigDecimal** result)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code
     * java.math.BigDecimal}.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a BigDecimal with value of the column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBigDecimalEx2(
        /** [in] **/String columnName,
        /** [out] **/IBigDecimal** result)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code
     * java.math.BigDecimal}.
     *
     * @deprecated use {@link #getBigDecimal(int)} or
     *             {@link #getBigDecimal(String)}
     * @param columnName
     *            the name of the column to read.
     * @param scale
     *            the number of digits after the decimal point
     * @return a BigDecimal with value of the column.
     * @throws SQLException
     *             if a database error happens.
     */
    //@Deprecated
    virtual CARAPI BigDecimal GetBigDecimalEx3(
        /** [in] **/String columnName, 
        /** [in] **/Int32 scale,
        /** [out] **/IBigDecimal** result)=0;

    /**
     * Gets the value of a column specified by column index as a binary
     * stream.
     * <p>
     * This method can be used to read {@code LONGVARBINARY} values. All of the
     * data in the {@code InputStream} should be read before getting data from
     * any other column. A further call to a getter method will implicitly close
     * the {@code InputStream}.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return an {@code InputStream} with the data from the column. If the
     *         column value is SQL {@code NULL}, {@code null} is returned.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBinaryStream(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IInputStream** stream)=0;

    /**
     * Gets the value of a column specified by column name as a binary stream.
     * <p>
     * This method can be used to read {@code LONGVARBINARY} values. All of the
     * data in the {@code InputStream} should be read before getting data from
     * any other column. A further call to a getter method will implicitly close
     * the {@code InputStream}.
     *
     * @param columnName
     *            the name of the column to read.
     * @return an {@code InputStream} with the data from the column if the
     *         column value is SQL {@code NULL}, {@code null} is returned.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBinaryStreamEx(
        /** [in] **/String columnName,
        /** [out] **/IInputStream** stream)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.sql.Blob} object.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code java.sql.Blob} with the value of the column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBlob(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IBlob** blob)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code
     * java.sql.Blob} object.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code java.sql.Blob} with the value of the column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBlobEx(
        /** [in] **/String columnName,
        /** [out] **/IBlob** blob)=0;

    /**
     * Gets the value of a column specified by column index as a {@code boolean}
     * .
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code boolean} value from the column. If the column is SQL
     *         {@code NULL}, {@code false} is returned.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBoolean(
        /** [in] **/Int32 columnIndex,
        /** [out] **/Boolean* result)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code boolean}
     * .
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code boolean} value from the column. If the column is SQL
     *         {@code NULL}, {@code false} is returned.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBooleanEx(
        /** [in] **/String columnName,
        /** [out] **/Boolean* result)=0;

    /**
     * Gets the value of a column specified by column index as a {@code byte}.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code byte} equal to the value of the column. 0 if the value
     *         is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetByte(
        /** [in] **/Int32 columnIndex,
        /** [out] **/Byte* result)=0;

    /**
     * Gets the value of a column specified by column name as a {@code byte}.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code byte} equal to the value of the column. 0 if the value
     *         is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetByteEx(
        /** [in] **/String columnName,
        /** [out] **/Byte* result)=0;

    /**
     * Gets the value of a column specified by column index as a byte array.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a byte array containing the value of the column. {@code null} if
     *         the column contains SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBytes(
        /** [in] **/Int32 columnIndex,
        /** [out,collee] **/ArrayOf<Byte>* bt)=0;

    /**
     * Gets the value of a column specified by column name as a byte array.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a byte array containing the value of the column. {@code null} if
     *         the column contains SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetBytesEx(
        /** [in] **/String columnName,
        /** [out,collee] **/ArrayOf<Byte>* bt)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.io.Reader} object.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code Reader} holding the value of the column. {@code null} if
     *         the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     * @see java.io.Reader
     */
    virtual CARAPI GetCharacterStream(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IReader** stream)=0;

    /**
     * Gets the value of a column specified by column name as a {@code
     * java.io.Reader} object.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code Reader} holding the value of the column. {@code null} if
     *         the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetCharacterStreamEx(
        /** [in] **/String columnName,
        /** [out] **/IReader** stream)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.sql.Clob}.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code Clob} object representing the value in the column.
     *         {@code null} if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetClob(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IClob** clob)=0;

    /**
     * Gets the value of a column specified by column name as a {@code
     * java.sql.Clob}.
     *
     * @param colName
     *            the name of the column to read.
     * @return a {@code Clob} object representing the value in the column.
     *         {@code null} if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetClobEx(
        /** [in] **/String colName,
        /** [out] **/IClob** clob)=0;

    /**
     * Gets the concurrency mode of this {@code ResultSet}.
     *
     * @return the concurrency mode - one of: {@code ResultSet.CONCUR_READ_ONLY}
     *         , {@code ResultSet.CONCUR_UPDATABLE}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetConcurrency(
        /** [out] **/Int32* result)=0;

    /**
     * Gets the name of the SQL cursor of this {@code ResultSet}.
     *
     * @return the SQL cursor name.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetCursorName(
        /** [out] **/String* name)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.sql.Date}.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code java.sql.Date} matching the column value. {@code null}
     *         if the column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetDate(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.sql.Date}. This method uses a supplied calendar to compute the Date.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @param cal
     *            a {@code java.util.Calendar} to use in constructing the Date.
     * @return a {@code java.sql.Date} matching the column value. {@code null}
     *         if the column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetDateEx(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/ICalendar* cal,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of a column specified by column name as a {@code
     * java.sql.Date}.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code java.sql.Date} matching the column value. {@code null}
     *         if the column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetDateEx(
        /** [in] **/String columnName,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code
     * java.sql.Date} object.
     *
     * @param columnName
     *            the name of the column to read.
     * @param cal
     *            {@code java.util.Calendar} to use in constructing the Date.
     * @return a {@code java.sql.Date} matching the column value. {@code null}
     *         if the column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetDateEx(
        /** [in] **/String columnName,
        /** [in] **/ICalendar* cal,
        /** [out] **/IDate** date)=0;

    /**
     * Gets the value of a column specified by column index as a {@code double}
     * value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code double} equal to the column value. {@code 0.0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetDouble(
        /** [in] **/Int32 columnIndex,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the value of a column specified by column name as a {@code double}
     * value.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code double} equal to the column value. {@code 0.0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetDoubleEx(
        /** [in] **/String columnName,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the direction in which rows are fetched for this {@code ResultSet}
     * object.
     *
     * @return the fetch direction. Will be one of:
     *         <ul>
     *         <li>ResultSet.FETCH_FORWARD</li><li>ResultSet.FETCH_REVERSE</li>
     *         <li>ResultSet.FETCH_UNKNOWN</li>
     *         </ul>
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetFetchDirection(
        /** [out] **/Int32* result)=0;

    /**
     * Gets the fetch size (in number of rows) for this {@code ResultSet}.
     *
     * @return the fetch size as an int
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetFetchSize(
        /** [out] **/Int32* result)=0;

    /**
     * Gets the value of a column specified by column index as a {@code float}
     * value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code float} equal to the column value. {@code 0.0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetFloat(
        /** [in] **/Int32 columnIndex,
        /** [out] **/Float* result)=0;

    /**
     * Gets the value of a column specified by column name as a {@code float}
     * value.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code float} equal to the column value. {@code 0.0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetFloatEx(
        /** [in] **/String columnName,
        /** [out] **/Float* result)=0;

    /**
     * Gets the value of a column specified by column index as an {@code int}
     * value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return an {@code int} equal to the column value. {@code 0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetInt(
        /** [in] **/Int32 columnIndex,
        /** [out] **/Int32* result)=0;

    /**
     * Gets the value of a column specified by column name, as an {@code int}
     * value.
     *
     * @param columnName
     *            the name of the column to read.
     * @return an {@code int} equal to the column value. {@code 0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetIntEx(
        /** [in] **/String columnName,
        /** [out] **/Int32* result )=0;

    /**
     * Gets the value of a column specified by column index as a {@code long}
     * value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a {@code long} equal to the column value. {@code 0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetLong(
        /** [in] **/Int32 columnIndex,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code long}
     * value.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a {@code long} equal to the column value. {@code 0} if the
     *         column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetLongEx(
        /** [in] **/String columnName,
        /** [out] **/Int64* result)=0;

    /**
     * Gets the metadata for this {@code ResultSet}. This defines the number,
     * types and properties of the columns in the {@code ResultSet}.
     *
     * @return a {@code ResultSetMetaData} object with information about this
     *         {@code ResultSet}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetMetaData(
        /** [out] **/IResultSetMetaData** data)=0;

    /**
     * Gets the value of a specified column as a Java {@code Object}. The type
     * of the returned object will be the default according to the column's SQL
     * type, following the JDBC specification for built-in types.
     * <p>
     * For SQL User Defined Types, if a column value is Structured or Distinct,
     * this method behaves the same as a call to: {@code
     * getObject(columnIndex,this.getStatement().getConnection().getTypeMap())}
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return an {@code Object} containing the value of the column. {@code
     *         null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetObject(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IInterface** obj)=0;

    /**
     * Gets the value of a column specified by column index as a Java {@code
     * Object}.
     * <p>
     * The type of the Java object will be determined by the supplied Map to
     * perform the mapping of SQL {@code Struct} or Distinct types into Java
     * objects.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @param map
     *            a {@code java.util.Map} containing a mapping from SQL Type
     *            names to Java classes.
     * @return an {@code Object} containing the value of the column. {@code
     *         null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
//    virtual CARAPI Object GetObjectEx(int columnIndex, Map<String, Class<?>> map)=0;

    /**
     * Gets the value of a specified column as a Java {@code Object}. The type
     * of the returned object will be the default according to the column's SQL
     * type, following the JDBC specification for built-in types.
     * <p>
     * For SQL User Defined Types, if a column value is structured or distinct,
     * this method behaves the same as a call to: {@code
     * getObject(columnIndex,this.getStatement().getConnection().getTypeMap())}
     *
     * @param columnName
     *            the name of the column to read.
     * @return an {@code Object} containing the value of the column. {@code
     *         null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetObjectEx2(
        /** [in] **/String columnName,
        /** [out] **/IInterface** obj)=0;

    /**
     * Gets the value of a column specified by column name as a Java {@code
     * Object}.
     * <p>
     * The type of the Java object will be determined by the supplied Map to
     * perform the mapping of SQL Struct or Distinct types into Java objects.
     *
     * @param columnName
     *            the name of the column to read.
     * @param map
     *            a {@code java.util.Map} containing a mapping from SQL Type names to
     *            Java classes.
     * @return an {@code Object} containing the value of the column. {@code
     *         null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
 //   virtual CARAPI Object GetObjectEx3(String columnName, Map<String, Class<?>> map)=0;

    /**
     * Gets the value of a column specified by column index as a Java {@code
     * java.sql.Ref}.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a Ref representing the value of the SQL REF in the column
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetRef(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IRef** ref)=0;

    /**
     * Gets the value of a column specified by column name as a Java {@code
     * java.sql.Ref}.
     *
     * @param colName
     *            the name of the column to read.
     * @return a Ref representing the value of the SQL {@code REF} in the column
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetRefEx(
        /** [in] **/String colName,
        /** [out] **/IRef** ref)=0;

    /**
     * Gets the number of the current row in the {@code ResultSet}. Row numbers
     * start at 1 for the first row.
     *
     * @return the index number of the current row. {@code 0} is returned if
     *         there is no current row.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetRow(
        /** [out] **/Int32* row)=0;

    /**
     * Gets the value of a column specified by column index as a short value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a short value equal to the value of the column. {@code 0} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetShort(
        /** [in] **/Int32 columnIndex,
        /** [out] **/Int16* result)=0;

    /**
     * Gets the value of a column specified by column name, as a short value.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a short value equal to the value of the column. {@code 0} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetShortEx(
        /** [in] **/String columnName,
        /** [out] **/Int16* result)=0;

    /**
     * Gets the statement that produced this {@code ResultSet}. If the {@code
     * ResultSet} was not created by a statement (i.e. because it was returned
     * from one of the {@link DatabaseMetaData} methods), {@code null} is
     * returned.
     *
     * @return the Statement which produced this {@code ResultSet}, or {@code
     *         null} if the {@code ResultSet} was not created by a Statement.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetStatement(
        /** [out] **/IStatement** result)=0;

    /**
     * Gets the value of a column specified by column index as a String.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return the String representing the value of the column, {@code null} if
     *         the column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetString(
        /** [in] **/Int32 columnIndex,
        /** [out] **/String* str)=0;

    /**
     * Gets the value of a column specified by column name, as a String.
     *
     * @param columnName
     *            the name of the column to read.
     * @return the String representing the value of the column, {@code null} if
     *         the column is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetStringEx(
        /** [in] **/String columnName,
        /** [out] **/String* str)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.sql.Time} value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a Time representing the column value, {@code null} if the column
     *         value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI  GetTime(
        /** [in] **/Int32 columnIndex,
        /** [out] **/ITime** time)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.sql.Time} value. The supplied {@code Calendar} is used to
     * map the SQL {@code Time} value to a Java Time value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @param cal
     *            a {@code Calendar} to use in creating the Java Time value.
     * @return a Time representing the column value, {@code null} if the column
     *         value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetTimeEx(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/ICalendar* cal,
        /** [out] **/ITime** time)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code
     * java.sql.Time} value.
     *
     * @param columnName
     *            the name of the column to read.
     * @return the column value, {@code null} if the column value is SQL {@code
     *         NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetTimeEx2(
        /** [in] **/String columnName,
        /** [out] **/ITime** time)=0;

    /**
     * Gets the value of a column specified by column index, as a {@code
     * java.sql.Time} value. The supplied {@code Calendar} is used to
     * map the SQL {@code Time} value to a Java Time value.
     *
     * @param columnName
     *            the name of the column to read.
     * @param cal
     *            a {@code Calendar} to use in creating the Java time value.
     * @return a Time representing the column value, {@code null} if the column
     *         value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetTimeEx3(
        /** [in] **/String columnName, 
        /** [in] **/ICalendar* cal,
        /** [out] **/ITime** time)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.sql.Timestamp} value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a timestamp representing the column value, {@code null} if the
     *         column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetTimestamp(
        /** [in] **/Int32 columnIndex,
        /** [out] **/ITimestamp** result)=0;

    /**
     * Gets the value of a column specified by column index, as a {@code
     * java.sql.Timestamp} value. The supplied Calendar is used when mapping
     * the SQL {@code Timestamp} value to a Java {@code Timestamp} value.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @param cal
     *            Calendar to use in creating the Java timestamp value.
     * @return a timestamp representing the column value, {@code null} if the
     *         column value is SQL NULL.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetTimestampEx(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/ICalendar* cal,
        /** [out] **/ITimestamp** result)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code
     * java.sql.Timestamp} value.
     *
     * @param columnName
     *            the name of the column to read.
     * @return a timestamp representing the column value, {@code null} if the
     *         column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetTimestampEx2(
        /** [in] **/String columnName,
        /** [out] **/ITimestamp** result)=0;

    /**
     * Gets the value of a column specified by column name, as a {@code
     * java.sql.Timestamp} value. The supplied Calendar is used when mapping
     * the SQL {@code Timestamp} value to a Java {@code Timestamp} value.
     *
     * @param columnName
     *            the name of the column to read.
     * @param cal
     *            Calendar to use in creating the Java {@code Timestamp} value.
     * @return a timestamp representing the column value, {@code null} if the
     *         column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetTimestampEx3(
        /** [in] **/String columnName, 
        /** [in] **/ICalendar* cal,
        /** [out] **/ITimestamp** result)=0;

    /**
     * Gets the type of the {@code ResultSet}.
     *
     * @return The {@code ResultSet} type, one of:
     *         <ul>
     *         <li>{@code ResultSet.TYPE_FORWARD_ONLY}</li> <li>{@code
     *         ResultSet.TYPE_SCROLL_INSENSITIVE}</li> <li>{@code
     *         ResultSet.TYPE_SCROLL_SENSITIVE}</li>
     *         </ul>
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetType(
        /** [out] **/Int32* type)=0;

    /**
     * Gets the value of the column as an {@code InputStream} of unicode
     * characters.
     *
     * @deprecated Use {@link #getCharacterStream}.
     * @param columnIndex
     *            the index of the column to read.
     * @return an {@code InputStream} holding the value of the column. {@code
     *         null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    //@Deprecated
    virtual CARAPI GetUnicodeStream(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IInputStream** stream)=0;

    /**
     * Gets the value of the column as an {@code InputStream} of Unicode
     * characters.
     *
     * @deprecated Use {@link #getCharacterStream}
     * @param columnName
     *            the name of the column to read.
     * @return an {@code InputStream} holding the value of the column. {@code
     *         null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    //@Deprecated
    virtual CARAPI GetUnicodeStreamEx(
        /** [in] **/String columnName,
        /** [out] **/IInputStream** stream)=0;

    /**
     * Gets the value of a column specified by column index as a {@code
     * java.net.URL}.
     *
     * @param columnIndex
     *            the index of the column to read.
     * @return a URL. {@code null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetURL(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IURL** url)=0;

    /**
     * Gets the value of a column specified by column name as a {@code
     * java.net.URL} object.
     *
     * @param columnName
     *            the name of the column to read.
     * @return the column vaule as a URL. {@code null} if the column value is SQL {@code NULL}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetURLEx(
        /** [in] **/String columnName,
        /** [out] **/IURL** url)=0;

    /**
     * Gets the first warning generated by calls on this {@code ResultSet}.
     * Subsequent warnings on this {@code ResultSet} are chained to the first
     * one.
     * <p>
     * The warnings are cleared when a new Row is read from the {@code
     * ResultSet}. The warnings returned by this method are only the warnings
     * generated by {@code ResultSet} method calls - warnings generated by
     * Statement methods are held by the Statement.
     * <p>
     * An {@code SQLException} is generated if this method is called on a closed
     * {@code ResultSet}.
     *
     * @return an SQLWarning which is the first warning for this {@code
     *         ResultSet}. {@code null} if there are no warnings.
     * @throws SQLException
     *             if a database error happens.
     */
//    virtual CARAPI SQLWarning GetWarnings()=0;

    /**
     * Insert the insert row into the {@code ResultSet} and into the underlying
     * database. The cursor must be set to the Insert Row before this method is
     * invoked.
     *
     * @throws SQLException
     *             if a database error happens. Particular cases include the
     *             cursor not being on the Insert Row or if any columns in the
     *             row do not have a value where the column is declared as
     *             not-nullable.
     */
    virtual CARAPI InsertRow()=0;

    /**
     * Gets if the cursor is after the last row of the {@code ResultSet}.
     *
     * @return {@code true} if the cursor is after the last row in the {@code
     *         ResultSet}, {@code false} if the cursor is at any other position
     *         in the {@code ResultSet}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI IsAfterLast(
        /** [out] **/Boolean* result)=0;

    /**
     * Gets if the cursor is before the first row of the {@code ResultSet}.
     *
     * @return {@code true} if the cursor is before the first row in the {@code
     *         ResultSet}, {@code false} if the cursor is at any other position
     *         in the {@code ResultSet}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI IsBeforeFirst(
        /** [out] **/Boolean* result)=0;

    /**
     * Gets if the cursor is on the first row of the {@code ResultSet}.
     *
     * @return {@code true} if the cursor is on the first row in the {@code
     *         ResultSet}, {@code false} if the cursor is at any other position
     *         in the {@code ResultSet}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI IsFirst(
        /** [out] **/Boolean* result)=0;

    /**
     * Gets if the cursor is on the last row of the {@code ResultSet}
     *
     * @return {@code true} if the cursor is on the last row in the {@code
     *         ResultSet}, {@code false} if the cursor is at any other position
     *         in the {@code ResultSet}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI IsLast(
        /** [out] **/Boolean* result)=0;

    /**
     * Shifts the cursor position to the last row of the {@code ResultSet}.
     *
     * @return {@code true} if the new position is in a legitimate row, {@code
     *         false} if the {@code ResultSet} contains no rows.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI Last(
        /** [out] **/Boolean* result)=0;

    /**
     * Moves the cursor to the remembered position, namely the
     * row that was the current row before a call to {@code moveToInsertRow}.
     * This only applies if the cursor is on the Insert Row.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI MoveToCurrentRow()=0;

    /**
     * Moves the cursor position to the Insert Row. The current position is
     * remembered and the cursor is positioned at the Insert Row. The columns in
     * the Insert Row should be filled in with the appropriate update methods,
     * before calling {@code insertRow} to insert the new row into the database.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI MoveToInsertRow()=0;

    /**
     * Shifts the cursor position down one row in this {@code ResultSet} object.
     * <p>
     * Any input streams associated with the current row are closed and any
     * warnings are cleared.
     *
     * @return {@code true} if the updated cursor position is pointing to a
     *         valid row, {@code false} otherwise (i.e. when the cursor is after
     *         the last row in the {@code ResultSet}).
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI Next(
        /** [out] **/Boolean* result)=0;

    /**
     * Relocates the cursor position to the preceding row in this {@code
     * ResultSet}.
     *
     * @return {@code true} if the new position is in a legitimate row, {@code
     *         false} if the cursor is now before the first row.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI Previous(
        /** [out] **/Boolean* result)=0;

    /**
     * Refreshes the current row with its most up to date value in the database.
     * Must not be called when the cursor is on the Insert Row.
     * <p>
     * If any columns in the current row have been updated but the {@code
     * updateRow} has not been called, then the updates are lost when this
     * method is called.
     *
     * @throws SQLException
     *             if a database error happens., including if the current row is
     *             the Insert row.
     */
    virtual CARAPI RefreshRow()=0;

    /**
     * Moves the cursor position up or down by a specified number of rows. If
     * the new position is beyond the start row (or end row), the cursor position is
     * set before the first row (or, respectively, after the last row).
     *
     * @param rows
     *            a number of rows to move the cursor - may be positive or
     *            negative
     * @return {@code true} if the new cursor position is on a row, {@code
     *         false} otherwise
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI Relative(
        /** [in] **/Int32 rows,
        /** [out] **/Boolean* result)=0;

    /**
     * Indicates whether a row has been deleted. This method depends on whether
     * the JDBC driver and database can detect deletions.
     *
     * @return {@code true} if a row has been deleted and if deletions are
     *         detected, {@code false} otherwise.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI RowDeleted(
        /** [out] **/Boolean* result)=0;

    /**
     * Indicates whether the current row has had an insertion operation. This
     * method depends on whether the JDBC driver and database can detect
     * insertions.
     *
     * @return {@code true} if a row has been inserted and if insertions are
     *         detected, {@code false} otherwise.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI RowInserted(
        /** [out] **/Boolean* result)=0;

    /**
     * Indicates whether the current row has been updated. This method depends
     * on whether the JDBC driver and database can detect updates.
     *
     * @return {@code true} if the current row has been updated and if updates
     *         can be detected, {@code false} otherwise.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI RowUpdated(
        /** [out] **/Boolean* result)=0;

    /**
     * Indicates which direction (forward/reverse) will be used to process the
     * rows of this {@code ResultSet} object. This is treated as a hint by the
     * JDBC driver.
     *
     * @param direction
     *            can be {@code ResultSet.FETCH_FORWARD}, {@code
     *            ResultSet.FETCH_REVERSE}, or {@code ResultSet.FETCH_UNKNOWN}
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI SetFetchDirection(
        /** [in] **/Int32 direction)=0;

    /**
     * Indicates the number of rows to fetch from the database when extra rows
     * are required for this {@code ResultSet}. This used as a hint to the JDBC
     * driver.
     *
     * @param rows
     *            the number of rows to fetch. {@code 0} implies that the JDBC
     *            driver can make its own decision about the fetch size. The
     *            number should not be greater than the maximum number of rows
     *            established by the statement that generated the {@code
     *            ResultSet}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI SetFetchSize(
        /** [in] **/Int32 rows)=0;

    /**
     * Updates a column specified by a column index with a {@code
     * java.sql.Array} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateArray(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IArray* x)=0;

    /**
     * Updates a column specified by a column name with a {@code java.sql.Array}
     * value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateArrayEx(
        /** [in] **/String columnName, 
        /** [in] **/IArray* x)=0;

    /**
     * Updates a column specified by a column index with an ASCII stream value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @param length
     *            the length of the data to write from the stream
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateAsciiStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int32 length)=0;

    /**
     * Updates a column specified by a column name with an Ascii stream value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @param length
     *            the length of the data to write from the stream
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateAsciiStreamEx(
        /** [in] **/String columnName, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int32 length)=0;

    /**
     * Updates a column specified by a column index with a {@code
     * java.sql.BigDecimal} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBigDecimal(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IBigDecimal* x)=0;

    /**
     * Updates a column specified by a column name with a {@code
     * java.sql.BigDecimal} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBigDecimalEx(
        /** [in] **/String columnName, 
        /** [in] **/IBigDecimal* x)=0;

    /**
     * Updates a column specified by a column index with a binary stream value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @param length
     *            the number of bytes to be read from the the stream.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBinaryStreamEx2(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int32 length)=0;

    /**
     * Updates a column specified by a column name with a binary stream value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @param length
     *            he number of bytes to be read from the the stream.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBinaryStreamEx3(
        /** [in] **/String columnName, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int32 length)=0;

    /**
     * Updates a column specified by a column index with a {@code java.sql.Blob}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBlob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IBlob* x)=0;

    /**
     * Updates a column specified by a column name with a {@code java.sql.Blob}
     * value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBlobEx(
        /** [in] **/String columnName, 
        /** [in] **/IBlob* x)=0;

    /**
     * Updates a column specified by a column index with a {@code boolean}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBoolean(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Boolean* x)=0;

    /**
     * Updates a column specified by a column name with a {@code boolean} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBooleanEx(
        /** [in] **/String columnName, 
        /** [in] **/Boolean* x)=0;

    /**
     * Updates a column specified by a column index with a {@code byte} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateByte(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Byte x)=0;

    /**
     * Updates a column specified by a column name with a {@code byte} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateByteEx(
        /** [in] **/String columnName, 
        /** [in] **/Byte x)=0;

    /**
     * Updates a column specified by a column index with a {@code byte} array
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBytes(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/ArrayOf<Byte> x)=0;

    /**
     * Updates a column specified by a column name with a byte array value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateBytesEx(
        /** [in] **/String columnName, 
        /** [in] **/ArrayOf<Byte> x)=0;

    /**
     * Updates a column specified by a column index with a character stream
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @param length
     *            the length of data to write from the stream
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateCharacterStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* x, 
        /** [in] **/Int32 length)=0;

    /**
     * Updates a column specified by a column name with a character stream
     * value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param reader
     *            the new value for the specified column.
     * @param length
     *            the length of data to write from the Reader
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateCharacterStreamEx(
        /** [in] **/String columnName, 
        /** [in] **/IReader* reader,
        /** [in] **/Int32 length)=0;

    /**
     * Updates a column specified by a column index with a {@code java.sql.Clob}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateClob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IClob* x)=0;

    /**
     * Updates a column specified by a column name with a {@code java.sql.Clob}
     * value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateClobEx(
        /** [in] **/String columnName, 
        /** [in] **/IClob* x)=0;

    /**
     * Updates a column specified by a column index with a {@code java.sql.Date}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateDate(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IDate* x)=0;

    /**
     * Updates a column specified by a column name with a {@code java.sql.Date}
     * value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateDateEx(
        /** [in] **/String columnName, 
        /** [in] **/IDate* x)=0;

    /**
     * Updates a column specified by a column index with a {@code double} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateDouble(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Int64 x)=0;

    /**
     * Updates a column specified by a column name with a {@code double} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateDoubleEx(
        /** [in] **/String columnName, 
        /** [in] **/Int64 x)=0;

    /**
     * Updates a column specified by a column index with a {@code float} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateFloat(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Float x)=0;

    /**
     * Updates a column specified by a column name with a {@code float} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateFloatEx(
        /** [in] **/String columnName, 
        /** [in] **/Float x)=0;

    /**
     * Updates a column specified by a column index with an {@code int} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateInt(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Int32 x)=0;

    /**
     * Updates a column specified by a column name with an {@code int} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateIntEx(
        /** [in] **/String columnName, 
        /** [in] **/Int32 x)=0;

    /**
     * Updates a column specified by a column index with a {@code long} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column..
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateLong(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Int64 x)=0;

    /**
     * Updates a column specified by a column name with a {@code long} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateLongEx(
        /** [in] **/String columnName, 
        /** [in] **/Int64 x)=0;

    /**
     * Updates a column specified by a column index with a {@code null} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateNull(
        /** [in] **/Int32 columnIndex)=0;

    /**
     * Updates a column specified by a column name with a {@code null} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateNullEx(
        /** [in] **/String columnName)=0;

    /**
     * Updates a column specified by a column index with an {@code Object}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateObject(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInterface* x)=0;

    /**
     * Updates a column specified by a column index with an {@code Object}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @param scale
     *            for the types {@code java.sql.Types.DECIMAL} or {@code
     *            java.sql.Types.NUMERIC}, this specifies the number of digits
     *            after the decimal point.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateObjectEx(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInterface* x, 
        /** [in] **/Int32 scale)=0;

    /**
     * Updates a column specified by a column name with an {@code Object} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateObjectEx2(
        /** [in] **/String columnName, 
        /** [in] **/IInterface* x)=0;

    /**
     * Updates a column specified by a column name with an {@code Object} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @param scale
     *            for the types {@code java.sql.Types.DECIMAL} or {@code
     *            java.sql.Types.NUMERIC}, this specifies the number of digits
     *            after the decimal point.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateObjectEx3(
        /** [in] **/String columnName, 
        /** [in] **/IInterface* x, 
        /** [in] **/Int32 scale)=0;

    /**
     * Updates a column specified by a column index with a {@code java.sql.Ref}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateRef(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IRef* x)=0;

    /**
     * Updates a column specified by a column name with a {@code java.sql.Ref}
     * value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateRefEx(
        /** [in] **/String columnName, 
        /** [in] **/IRef* x)=0;

    /**
     * Updates the database with the new contents of the current row of this
     * {@code ResultSet} object.
     *
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateRow()=0;

    /**
     * Updates a column specified by a column index with a {@code short} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateShort(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/Int16 x)=0;

    /**
     * Updates a column specified by a column name with a {@code short} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateShortEx(
        /** [in] **/String columnName, 
        /** [in] **/Int16 x)=0;

    /**
     * Updates a column specified by a column index with a {@code String} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateString(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/String x)=0;

    /**
     * Updates a column specified by a column name with a {@code String} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateStringEx(
        /** [in] **/String columnName, 
        /** [in] **/String x)=0;

    /**
     * Updates a column specified by a column index with a {@code Time} value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateTime(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/ITime* x)=0;

    /**
     * Updates a column specified by a column name with a {@code Time} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateTimeEx(
        /** [in] **/String columnName, 
        /** [in] **/ITime* x)=0;

    /**
     * Updates a column specified by a column index with a {@code Timestamp}
     * value.
     *
     * @param columnIndex
     *            the index of the column to update.
     * @param x
     *            the new timestamp value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateTimestamp(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/ITimestamp* x) =0;

    /**
     * Updates a column specified by column name with a {@code Timestamp} value.
     *
     * @param columnName
     *            the name of the column to update.
     * @param x
     *            the new timestamp value for the specified column.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI UpdateTimestampEx(
        /** [in] **/String columnName, 
        /** [in] **/ITimestamp* x)=0;

    /**
     * Determines whether the last column read from this {@code ResultSet}
     * contained SQL {@code NULL}.
     *
     * @return {@code {@code true} if the last column contained SQL {@code
     *         NULL}, {@code false} otherwise
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI WasNull(
        /** [out] **/Boolean* result)=0;

    /**
     * Returns a {@code RowId} corresponding to the SQL ROWID at the 1-based {@code columnIndex}.
     * @throws SQLException
     */
    virtual CARAPI GetRowId(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IRowId** id)=0;

    /**
     * Returns a {@code RowId} corresponding to the SQL ROWID at the named column.
     * @throws SQLException
     */
    virtual CARAPI GetRowIdEx(
        /** [in] **/String columnLabel,
        /** [out] **/IRowId** id)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateRowId(
    	/** [in] **/Int32 columnIndex, 
    	/** [in] **/IRowId* value)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateRowIdEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IRowId* value)=0;

    /**
     * Returns the holdability of this result set: {@link #HOLD_CURSORS_OVER_COMMIT} or
     * {@link #CLOSE_CURSORS_AT_COMMIT}.
     * @throws SQLException
     */
    virtual CARAPI GetHoldability(
        /** [out] **/Int32* result)=0;

    /**
     * Returns true if this result set has been closed, false otherwise.
     * @throws SQLException
     */
    virtual CARAPI IsClosed(
        /** [out] **/Boolean* result)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNString(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/String nString)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNStringEx(
        /** [in] **/String columnLabel, 
        /** [in] **/String nString)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNClob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/INClob* nClob)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNClobEx(
        /** [in] **/String columnLabel, 
        /** [in] **/INClob* nClob)=0;

    /**
     * Returns an {@code NClob} corresponding to the value at the 1-based {@code columnIndex}.
     * @throws SQLException
     */
    virtual CARAPI GetNClob(
        /** [in] **/Int32 columnIndex,
        /** [out] **/INClob** clob)=0;

    /**
     * Returns an {@code NClob} corresponding to the value in the named column.
     * @throws SQLException
     */
    virtual CARAPI GetNClobEx(
        /** [in] **/String columnLabel,
        /** [out] **/INClob** clob)=0;

    /**
     * Returns an {@code SQLXML} corresponding to the value at the 1-based {@code columnIndex}.
     * @throws SQLException
     */
    virtual CARAPI GetSQLXML(
    	/** [in] **/Int32 columnIndex,
    	/** [out] **/ISQLXML** xml)=0;

    /**
     * Returns an {@code SQLXML} corresponding to the value in the named column.
     * @throws SQLException
     */
    virtual CARAPI GtSQLXMLEx(
        /** [in] **/String columnLabel,
        /** [out] **/ISQLXML** xml)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateSQLXML(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/ISQLXML* xmlObject)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateSQLXMLEx(
        /** [in] **/String columnLabel, 
        /** [in] **/ISQLXML* xmlObject)=0;

    /**
     * Returns a {@code String} corresponding to the value at the 1-based {@code columnIndex}.
     * @throws SQLException
     */
    virtual CARAPI GetNString(
        /** [in] **/Int32 columnIndex,
        /** [out] **/String* str)=0;

    /**
     * Returns a {@code String} corresponding to the value in the named column.
     * @throws SQLException
     */
    virtual CARAPI GetNStringEx(
        /** [in] **/String columnLabel,
        /** [out] **/String* str)=0;

    /**
     * Returns a {@code Reader} corresponding to the value at the 1-based {@code columnIndex}.
     * @throws SQLException
     */
    virtual CARAPI GetNCharacterStream(
        /** [in] **/Int32 columnIndex,
        /** [out] **/IReader** stream)=0;

    /**
     * Returns a {@code Reader} corresponding to the value in the named column.
     * @throws SQLException
     */
    virtual CARAPI GetNCharacterStreamEx(
        /** [in] **/String columnLabel,
        /** [out] **/IReader** stream)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNCharacterStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNCharacterStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateAsciiStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateBinaryStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateCharacterStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateAsciiStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateBinaryStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IInputStream* x, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateCharacterStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateBlob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInputStream* inputStream, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateBlobEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IInputStream* inputStream, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateClob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateClobEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNClob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNClobEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader* reader, 
        /** [in] **/Int64 length)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNCharacterStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* x)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNCharacterStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader* reader)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateAsciiStream(
        /** [in] **/int columnIndex, 
        /** [in] **/IInputStream* x)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI  UpdateBinaryStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInputStream* x)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateCharacterStream(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* x)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateAsciiStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IInputStream* x)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateBinaryStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IInputStream* x)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateCharacterStreamEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader* reader)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateBlob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IInputStream* inputStream)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateBlobEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IInputStream* inputStream)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateClob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* reader)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateClobEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader* reader)=0;

    /**
     * Updates the value at the 1-based {@code columnIndex}.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNClob(
        /** [in] **/Int32 columnIndex, 
        /** [in] **/IReader* reader)=0;

    /**
     * Updates the value in the named column.
     * The underlying database isn't changed until the next row update or insert operation.
     * @throws SQLException
     */
    virtual CARAPI UpdateNClobEx(
        /** [in] **/String columnLabel, 
        /** [in] **/IReader* reader)=0;

public:    
	/**
     * A constant used to indicate that a {@code ResultSet} object must be
     * closed when the method {@code Connection.commit} is invoked.
     */
    static const Int32 CLOSE_CURSORS_AT_COMMIT;// = 2;

    /**
     * A constant used to indicate that a {@code ResultSet} object must not be
     * closed when the method {@code Connection.commit} is invoked.
     */
    static const Int32 HOLD_CURSORS_OVER_COMMIT;// = 1;

    /**
     * A constant used to indicate the concurrency mode for a {@code ResultSet}
     * object that cannot be updated.
     */
    static const Int32 CONCUR_READ_ONLY;// = 1007;

    /**
     * A constant used to indicate the concurrency mode for a {@code ResultSet}
     * object that can be updated.
     */
    static const Int32 CONCUR_UPDATABLE;// = 1008;

    /**
     * A constant used to indicate processing of the rows of a {@code ResultSet}
     * in the forward direction, first to last.
     */
    static const Int32 FETCH_FORWARD;// = 1000;

    /**
     * A constant used to indicate processing of the rows of a {@code ResultSet}
     * in the reverse direction, last to first.
     */
    static const Int32 FETCH_REVERSE;// = 1001;

    /**
     * A constant used to indicate that the order of processing of the rows of a
     * {@code ResultSet} is unknown.
     */
    static const Int32 FETCH_UNKNOWN;// = 1002;

    /**
     * A constant used to indicate a {@code ResultSet} object whose cursor can
     * only move forward.
     */
    static const Int32 TYPE_FORWARD_ONLY;// = 1003;

    /**
     * A constant used to indicate a {@code ResultSet} object which is
     * scrollable but is insensitive to changes made by others.
     */
    static const Int32 TYPE_SCROLL_INSENSITIVE;// = 1004;

    /**
     * A constant used to indicate a {@code ResultSet} object which is
     * scrollable and sensitive to changes made by others.
     */
    static const Int32 TYPE_SCROLL_SENSITIVE;// = 1005;
};
#endif//_RESULTSET_H_
