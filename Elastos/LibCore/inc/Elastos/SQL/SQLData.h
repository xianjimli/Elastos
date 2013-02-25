#ifndef _SQLDATA_H_
#define _SQLDATA_H_

#include <elastos.h>

using namespace Elastos;

class SQLData {
public:
	/**
     * Gets the SQL name of the <i>User Defined Type</i> (UDT) that this object
     * represents. This method, usually invoked by the JDBC driver, retrieves
     * the name of the UDT instance associated with this {@code SQLData} object.
     *
     * @return a string with UDT type name for this object mapping, passed to
     *         {@code readSQL} when the object was created.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetSQLTypeName(
        /** [out] **/String* name)=0;

    /**
     * Reads data from the database into this object. This method follows these
     * steps:
     * <p>
     * <ul>
     * <li>Utilize the passed input stream to read the attributes or entries of
     * the SQL type</li>
     * <li>This is carried out by reading each entry from the input stream,
     * ordered as they are in the SQL definition.</li>
     * <li>Assign the data to the appropriate fields or elements. This is done
     * by calling the relevant reader method for the type involved (e.g. {@code
     * SQLInput.readString}, {@code SQLInputreadBigDecimal}). If the type is
     * distinct, then read its only data entry. For structured types, read every
     * entry.</li>
     * </ul>
     * <p>
     * The supplied input stream is typically initialized by the calling JDBC
     * driver with the type map before {@code readSQL} is called.
     *
     * @param stream
     *            the {@code SQLInput} stream from which the type map data is
     *            read for the custom mapping.
     * @param typeName
     *            the SQL type name for the type which is being mapped.
     * @throws SQLException
     *             if a database error occurs.
     * @see SQLInput
     */
    virtual CARAPI ReadSQL(
        /** [in] **/ISQLInput* stream, 
        /** [in] **/String typeName)=0;

    /**
     * Writes the object to a supplied {@code SQLOutput} data stream, writing it
     * out as an SQL value to the data source.
     * <p>
     * This method follows the following steps:
     * <ul>
     * <li>Write each attribute of the SQL type to the output stream.</li>
     * <li>Write each item by calling a method on the output stream, in the
     * order they appear in the SQL definition of the type. Use the appropriate
     * {@code SQLOutput} methods (e.g. {@code writeInt}, {@code writeString}).
     * Write a single data element for a distinct type. For a structured type,
     * write a value for each attribute of the the SQL type.</li>
     * </ul>
     *
     * @param stream
     *            the {@code SQLOutput} stream to use to write out the data for
     *            the custom mapping.
     * @throws SQLException
     *             if a database error occurs.
     * @see SQLOutput
     */
    virtual CARAPI WriteSQL(
        /** [in] **/ISQLOutput* stream)=0;
};
#endif//_SQLDATA_H_
