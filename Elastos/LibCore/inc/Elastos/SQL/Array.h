#ifndef  _WRRAY_H_
#define _WRRAY_H_

#include <elastos.h>

using namespace Elastos;

class Array {
public:
    /**
     * Retrieves the contents of the SQL {@code ARRAY} value as a Java array
     * object.
     *
     * @return A Java array containing the elements of this Array
     * @throws SQLException
     *             if there is a database error.
     */
    //public Object getArray() throws SQLException;
    virtual CARAPI  GetArray(
        /** [out] **/  IInterface** obj)=0;

    /**
     * Returns part of the SQL {@code ARRAY} associated with this array,
     * starting at a particular {@code index} and comprising up to {@code count}
     * successive elements of the SQL array.
     *
     * @param index
     *            the start position in the array where the values are
     *            retrieved.
     * @param count
     *            the number of elements to retrieve.
     * @return A Java array containing the desired set of elements from this Array
     * @throws SQLException
     *             if there is a database error.
     */
    //public Object getArray(long index, int count) throws SQLException;
    virtual CARAPI  GetArrayEx(
    	/** [in] **/ Int64 index,
        /** [in] **/ Int32 count,
        /** [out] **/  IInterface** obj)=0;

    /**
     * Returns part of the SQL {@code ARRAY} associated with this array,
     * starting at a particular {@code index} and comprising up to {@code count}
     * successive elements of the SQL array.
     *
     * @param index
     *            the start position in the array where the values are
     *            retrieved.
     * @param count
     *            the number of elements to retrieve.
     * @param map
     *            the map defining the correspondence between SQL type names
     *            and Java types.
     * @return A Java array containing the desired set of elements from this Array
     * @throws SQLException
     *             if there is a database error.
     */
//    public Object getArray(long index, int count, Map<String, Class<?>> map)
//            throws SQLException;

    /**
     * Returns the data from the underlying SQL {@code ARRAY} as a Java array.
     *
     * @param map
     *            the map defining the correspondence between SQL type names
     *            and Java types.
     * @return A Java array containing the elements of this array
     * @throws SQLException
     *             if there is a database error.
     */
//    public Object getArray(Map<String, Class<?>> map) throws SQLException;

    /**
     * Returns the JDBC type of the entries in this array's underlying
     * SQL array.
     *
     * @return An integer constant from the {@code java.sql.Types} class
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI  GetBaseType(
    	/** [out] **/ Int32* type)=0;

    /**
     * Returns the SQL type name of the entries in this array's underlying
     * SQL array.
     *
     * @return The database specific name or a fully-qualified SQL type name.
     * @throws SQLException
     *              if there is a database error.
     */
    virtual CARAPI  GetBaseTypeName(
    	/** [out] **/ String name)=0;

    /**
     * Returns a ResultSet object which holds the entries of the SQL {@code
     * ARRAY} associated with this array.
     *
     * @return the elements of the array as a {@code ResultSet}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI  getResultSet(
    	/** [out] **/ IResultSet** result)=0;

    /**
     * Returns a {@code ResultSet} object that holds the entries of a subarray,
     * beginning at a particular index and comprising up to {@code count}
     * successive entries.
     *
     * @param index
     *            the start position in the array where the values are
     *            retrieved.
     * @param count
     *            the number of elements to retrieve.
     * @return the elements of the array as a {@code ResultSet}.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI  ResultSet GetResultSetEx(
        /** [in] **/ Int64 index,
        /** [in] **/ Int32 count,
        /** [out] **/ IResultSet** result)=0;

    /**
     * Returns a {@code ResultSet} object that holds the entries of a subarray,
     * beginning at a particular index and comprising up to {@code count}
     * successive entries.
     *
     * @param index
     *            the start position in the array where the values are
     *            retrieved.
     * @param count
     *            the number of elements to retrieve.
     * @param map
     *            the map defining the correspondence between SQL type names
     *            and Java types.
     * @return the {@code ResultSet} the array's custom type values. if a
     *         database error has occurred.
     * @throws SQLException
     *             if there is a database error.
     */
//    public ResultSet getResultSet(long index, int count,
//            Map<String, Class<?>> map) throws SQLException;


    /**
     * Returns a {@code ResultSet} object which holds the entries of the SQL
     * {@code ARRAY} associated with this array.
     *
     * @param map
     *            the map defining the correspondence between SQL type names
     *            and Java types.
     * @return the array as a {@code ResultSet}.
     * @throws SQLException
     *             if there is a database error.
     */
//    public ResultSet getResultSet(Map<String, Class<?>> map)
//            throws SQLException;

    /**
     * Frees any resources held by this array. After {@code free} is called, calling
     * method other than {@code free} will throw {@code SQLException} (calling {@code free}
     * repeatedly will do nothing).
     * @throws SQLException
     */
    virtual CARAPI  Free()=0;
};
#endif //_WRRAY_H_
