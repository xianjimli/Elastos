#ifdef _SQLOUTPUT_H_
#define _SQLOUTPUT_H_

#include <elastos.h>

using namespace Elastos;

class SQLOutput {
public:
	/**
     * Write a {@code String} value into the output stream.
     *
     * @param theString
     *            the {@code String} to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteString(
        /** [in] **/String theString)=0;

    /**
     * Write a {@code boolean} value into the output stream.
     *
     * @param theFlag
     *            the {@code boolean} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteBoolean(
        /** [in] **/Boolean theFlag)=0;

    /**
     * Write a {@code byte} value into the output stream.
     *
     * @param theByte
     *            the {@code byte} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteByte(
        /** [in] **/Byte theByte)=0;

    /**
     * Write a {@code short} value into the output stream.
     *
     * @param theShort
     *            the {@code short} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteShort(
        /** [in] **/Int16 theShort)=0;

    /**
     * Write an {@code int} value into the output stream.
     *
     * @param theInt
     *            the {@code int} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteInt(
        /** [in] **/Int32 theInt)=0;

    /**
     * Write a {@code long} value into the output stream.
     *
     * @param theLong
     *            the {@code long} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI  WriteLong(
        /** [in] **/Int64 theLong)=0;

    /**
     * Write a {@code float} value into the output stream.
     *
     * @param theFloat
     *            the {@code float} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteFloat(
        /** [in] **/Float theFloat)=0;

    /**
     * Write a {@code double} value into the output stream.
     *
     * @param theDouble
     *            the {@code double} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteDouble(
        /** [in] **/Double theDouble)=0;

    /**
     * Write a {@code java.math.BigDecimal} value into the output stream.
     *
     * @param theBigDecimal
     *            the {@code BigDecimal} value to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteBigDecimal(
        /** [in] **/IBigDecimal* theBigDecimal)=0;

    /**
     * Write an array of bytes into the output stream.
     *
     * @param theBytes
     *            the array of bytes to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteBytes(
        /** [in] **/ArrayOf<Byte> theBytes)=0;

    /**
     * Write a {@code java.sql.Date} value into the output stream.
     *
     * @param theDate
     *            the {@code Date} value to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Date
     */
    virtual CARAPI WriteDate(
        /** [in] **/IDate* theDate)=0;

    /**
     * Write a {@code java.sql.Time} value into the output stream.
     *
     * @param theTime
     *            the {@code Time} value to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Time
     */
    virtual CARAPI WriteTime(
        /** [in] **/ITime* theTime)=0;

    /**
     * Write a {@code java.sql.Timestamp} value into the output stream.
     *
     * @param theTimestamp
     *            the {@code Timestamp} value to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Timestamp
     */
    virtual CARAPI WriteTimestamp(
        /** [in] **/ITimestamp* theTimestamp)=0;

    /**
     * Write a stream of unicode characters into the output stream.
     *
     * @param theStream
     *            the stream of unicode characters to write, as a {@code
     *            java.io.Reader} object.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteCharacterStream(
        /** [in] **/IReader* theStream)=0;

    /**
     * Write a stream of ASCII characters into the output stream.
     *
     * @param theStream
     *            the stream of ASCII characters to write, as a {@code
     *            java.io.InputStream} object
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteAsciiStream(
        /** [in] **/IInputStream* theStream)=0;

    /**
     * Write a stream of uninterpreted bytes into the output stream.
     *
     * @param theStream
     *            the stream of bytes to write, as a {@code java.io.InputStream}
     *            object
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteBinaryStream(
        /** [in] **/IInputStream* theStream)=0;

    /**
     * Write an {@code SQLData} object into the output stream.
     * <p>
     * If the {@code SQLData} object is null, writes {@code NULL} to the stream.
     * <p>
     * Otherwise, calls the {@code SQLData.writeSQL} method of the object, which
     * writes the object's attributes to the stream by calling the appropriate
     * SQLOutput writer methods for each attribute, in order. The order of the
     * attributes is the order they are listed in the SQL definition of the User
     * Defined Type.
     *
     * @param theObject
     *            the {@code SQLData} object to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see SQLData
     */
    virtual CARAPI WriteObject(
        /** [in] **/ISQLData* theObject)=0;

    /**
     * Write an SQL {@code Ref} value into the output stream.
     *
     * @param theRef
     *            the {@code java.sql.Ref} object to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Ref
     */
    virtual CARAPI WriteRef(
        /** [in] **/IRef* theRef)=0;

    /**
     * Write an SQL {@code Blob} value into the output stream.
     *
     * @param theBlob
     *            the {@code java.sql.Blob} object to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Blob
     */
    virtual CARAPI WriteBlob(
        /** [in] **/IBlob* theBlob)=0;

    /**
     * Write an SQL {@code Clob} value into the output stream.
     *
     * @param theClob
     *            the {@code java.sql.Clob} object to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Clob
     */
    virtual CARAPI WriteClob(
        /** [in] **/IClob* theClob)=0;

    /**
     * Write an SQL {@code Struct} value into the output stream.
     *
     * @param theStruct
     *            the {@code java.sql.Struct} object to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Struct
     */
    virtual CARAPI WriteStruct(
        /** [in] **/IStruct* theStruct)=0;

    /**
     * Write an SQL {@code Array} value into the output stream.
     *
     * @param theArray
     *            the {@code java.sql.Array} object to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see Array
     */
    virtual CARAPI WriteArray(
        /** [in] **/IArray* theArray)=0;

    /**
     * Write a {@code URL} into the output stream as an SQL DATALINK.
     *
     * @param theURL
     *            the datalink value as a {@code java.net.URL} to write.
     * @throws SQLException
     *             if a database error occurs.
     * @see java.net.URL
     */
    virtual CARAPI WriteURL(
        /** [in] **/IURL* theURL)=0;

    /**
     * Write a {@code String} into the output stream as an SQL NCHAR, NVARCHAR,
     * or LONGNVARCHAR.
     *
     * @param theString
     *            the {@code String} to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteNString(
        /** [in] **/String theString)=0;

    /**
     * Write a {@code Clob} into the output stream as an SQL NCLOB.
     *
     * @param theNClob
     *            the {@code java.sql.Clob} object to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteNClob(
        /** [in] **/INClob* theNClob)=0;

    /**
     * Write a {@code RowId} into the output stream as an SQL ROWID.
     *
     * @param theRowId
     *            the {@code java.sql.RowId} object to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteRowId(
        /** [in] **/IRowId* theRowId)=0;

    /**
     * Write a {@code SQLXML} into the output stream as an SQL XML.
     *
     * @param theXml
     *            the {@code java.sql.SQLXML} object to write.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI WriteSQLXML(
        /** [in] **/ISQLXML* theXml)=0;
};
#endif//_SQLOUTPUT_H_