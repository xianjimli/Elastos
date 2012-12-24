#ifdef _SQLINPUT_H_
#define _SQLINPUT_H_

#include <elastos.h>

using namespace Elastos;

class SQLInput {
public:
	/**
     * Returns the next attribute in the stream in the form of a {@code String}.
     *
     * @return the next attribute. {@code null} if the value is SQL {@code NULL}.
     *
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadString(
        /** [out] **/String* str)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code boolean}
     * .
     *
     * @return the next attribute as a {@code boolean}. {@code false} if the
     *         value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadBoolean(
        /** [out] **/ Boolean *result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code byte}.
     *
     * @return the next attribute as a {@code byte}. 0 if the value is SQL
     *         {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadByte(
        /** [out] **/Byte* result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code short}.
     *
     * @return the next attribute as a {@code short}. 0 if the value is SQL
     *         {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadShort(
        /** [out] **/Int16* result)=0;

    /**
     * Returns the next attribute in the stream in the form of an {@code int}.
     *
     * @return the next attribute as an {@code int}. 0 if the value is SQL
     *         {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadInt(
        /** [out] **/Int32* result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code long}.
     *
     * @return the next attribute as a {@code long}. 0 if the value is SQL
     *         {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadLong(
        /** [out] **/Int64* result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code float}.
     *
     * @return the next attribute as a {@code float}. 0 if the value is SQL
     *         {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadFloat(
        /** [out] **/Float* result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code double}.
     *
     * @return the next attribute as a {@code double}. 0 if the value is SQL
     *         {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadDouble(
        /** [out] **/Double* result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.math.BigDecimal}.
     *
     * @return the attribute as a {@code java.math.BigDecimal}. {@code null} if
     *         the read returns SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see java.math.BigDecimal
     */
    virtual CARAPI ReadBigDecimal(
        /** [out] **/IBigDecimal** result)=0;

    /**
     * Returns the next attribute in the stream in the form of a byte array.
     *
     * @return the attribute as a byte array. {@code null} if the read returns
     *         SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadBytes(
        /** [out,collee] **/ArrayOf<Byte>* result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.Date}.
     *
     * @return the next attribute as a {@code java.sql.Date}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see Date
     */
    virtual CARAPI ReadDate(
        /** [out] **/IDate** result)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.Time}.
     *
     * @return the attribute as a {@code java.sql.Time}. {@code null} if the
     *         read returns SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see Time
     */
    virtual CARAPI ReadTime(
        /** [out] **/ITime** time)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.Timestamp}.
     *
     * @return the attribute as a {@code java.sql.Timestamp}. {@code null} if
     *         the read returns SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see Timestamp
     */
    virtual CARAPI ReadTimestamp(
        /** [out] **/ITimestamp** result)=0;

    /**
     * Returns the next attribute in the stream in the form of a Unicode
     * character stream embodied as a {@code java.io.Reader}.
     *
     * @return the next attribute as a {@code java.io.Reader}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see java.io.Reader
     */
    virtual CARAPI ReadCharacterStream(
        /** [out] **/IReader** stream)=0;

    /**
     * Returns the next attribute in the stream in the form of an ASCII
     * character stream embodied as a {@code java.io.InputStream}.
     *
     * @return the next attribute as a {@code java.io.InputStream}. {@code null}
     *         if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see java.io.InputStream
     */
    virtual CARAPI ReadAsciiStream(
        /** [out] **/IInputStream** stream)=0;

    /**
     * Returns the next attribute in the stream in the form of a stream of bytes
     * embodied as a {@code java.io.InputStream}.
     *
     * @return the next attribute as a {@code java.io.InputStream}. {@code null}
     *         if the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see java.io.InputStream
     */
    virtual CARAPI ReadBinaryStream(
        /** [out] **/IInputStream** stream)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.lang.Object}.
     * <p>
     * The type of the {@code Object} returned is determined by the type mapping
     * for this JDBC driver, including any customized mappings, if present. A
     * type map is given to the {@code SQLInput} by the JDBC driver before the
     * {@code SQLInput} is given to the application.
     * <p>
     * If the attribute is an SQL structured or distinct type, its SQL type is
     * determined. If the stream's type map contains an element for that SQL
     * type, the driver creates an object for the relevant type and invokes the
     * method {@code SQLData.readSQL} on it, which reads supplementary data from
     * the stream using whichever protocol is defined for that method.
     *
     * @return the next attribute as an Object. {@code null} if the value is SQL
     *         {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadObject(
        /** [out] **/IInterface** obj)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.Ref}.
     *
     * @return the next attribute as a {@code java.sql.Ref}. {@code null} if the
     *         value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see Ref
     */
    virtual CARAPI ReadRef(
        /** [out] **/IRef** ref)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.Blob}.
     *
     * @return the next attribute as a {@code java.sql.Blob}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadBlob(
        /** [out] **/IBlob** blob)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.Clob}.
     *
     * @return the next attribute as a {@code java.sql.Clob}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see Clob
     */
    virtual CARAPI ReadClob(
        /** [out] **/IClob** clob)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.Array}.
     *
     * @return the next attribute as an {@code Array}. {@code null} if the value
     *         is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see Array
     */
    virtual CARAPI ReadArray(
        /** [out] **/IArray** array)=0;

    /**
     * Reports whether the last value read was SQL {@code NULL}.
     *
     * @return {@code true} if the last value read was SQL {@code NULL}, {@code
     *         false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI WasNull(
        /** [out] **/ Boolean *result)=0;

    /**
     * Reads the next attribute in the stream (SQL DATALINK value) and returns
     * it as a {@code java.net.URL} object.
     *
     * @return the next attribute as a {@code java.net.URL}. {@code null} if the
     *         value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     * @see java.net.URL
     */
    virtual CARAPI ReadURL(
        /** [out] **/IURL** url)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.NClob}.
     *
     * @return the next attribute as a {@code java.sql.NClob}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadNClob(
        /** [out] **/INClob** nclob)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.lang.String}. Used for the NCHAR, NVARCHAR and LONGNVARCHAR types.
     * See {@link #readString} otherwise.
     *
     * @return the next attribute as a {@code java.lang.String}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadNString(
        /** [out] **/String* str)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.SQLXML}.
     *
     * @return the next attribute as a {@code java.sql.SQLXML}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadSQLXML(
        /** [out] **/ISQLXML** xml)=0;

    /**
     * Returns the next attribute in the stream in the form of a {@code
     * java.sql.RowId}. Used for the ROWID type.
     *
     * @return the next attribute as a {@code java.sql.RowId}. {@code null} if
     *         the value is SQL {@code NULL}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI ReadRowId(
        /** [out] **/IRowId** id)=0;
};
#endif//_SQLINPUT_H_