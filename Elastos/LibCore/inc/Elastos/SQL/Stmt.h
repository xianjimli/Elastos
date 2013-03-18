#ifndef _STMT_H_
#define _STMT_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class Stmt
{
private:
    /**
     * Internal handle for the SQLite3 statement.
     */
    Int64 mHandle;// = 0;

protected:
    /**
     * Internal last error code for prepare()/step() methods.
     */
    Int32 mError_code;// = 0;

public:
    Stmt();

    /**
     * Prepare the next SQL statement for the Stmt instance.
     * @return true when the next piece of the SQL statement sequence
     * has been prepared, false on end of statement sequence.
     */
    CARAPI Prepare(
        /** [out] **/Boolean* result);

    /**
     * Perform one step of compiled SQLite3 statement.
     *
     * Example:<BR>
     * <PRE>
     *   ...
     *   try {
     *     Stmt s = db.prepare("select * from x; select * from y;");
     *     s.bind(...);
     *     ...
     *     s.bind(...);
     *     while (s.step(cb)) {
     *       Object o = s.value(...);
     *       ...
     *     }
     *     // s.reset() for re-execution or
     *     // s.prepare() for the next piece of SQL
     *     while (s.prepare()) {
     *       s.bind(...);
     *       ...
     *       s.bind(...);
     *       while (s.step(cb)) {
     *         Object o = s.value(...);
     *         ...
     *       }
     *     }
     *   } catch (SQLite.Exception e) {
     *     s.close();
     *   }
     * </PRE>
     *
     * @return true when row data is available, false on end
     * of result set.
     */
    CARAPI Step(
        /** [out] **/Boolean* result);

    /**
     * Close the compiled SQLite3 statement.
     */
    CARAPI Close();

    /**
     * Reset the compiled SQLite3 statement without
     * clearing parameter bindings.
     */
    CARAPI Reset();

    /**
     * Clear all bound parameters of the compiled SQLite3 statement.
     */
    CARAPI Clear_bindings();

    /**
     * Bind positional integer value to compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     * @param value value of parameter
     */
    CARAPI Bind(
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 value);

    /**
     * Bind positional long value to compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     * @param value value of parameter
     */
    CARAPI BindEx(
        /** [in] **/Int32 pos, 
        /** [in] **/Int64 value);

    /**
     * Bind positional double value to compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     * @param value value of parameter
     */
    CARAPI BindEx2(
        /** [in] **/Int32 pos, 
        /** [in] **/Double value);

    /**
     * Bind positional byte array to compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     * @param value value of parameter, may be null
     */
    CARAPI BindEx3(
        /** [in] **/Int32 pos, 
        /** [in] **/ArrayOf<Byte>& value);

    /**
     * Bind positional String to compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     * @param value value of parameter, may be null
     */
    CARAPI BindEx4(
        /** [in] **/Int32 pos, 
        /** [in] **/ArrayOf<String>& value);

    /**
     * Bind positional SQL null to compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     */
    CARAPI BindEx5(
        /** [in] **/Int32 pos);

    /**
     * Bind positional zero'ed blob to compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     * @param length byte size of zero blob
     */
    CARAPI Bind_zeroblob(
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 length);

    /**
     * Return number of parameters in compiled SQLite3 statement.
     * @return int number of parameters
     */
    CARAPI Bind_parameter_count(
        /** [out] **/Int32* count);

    /**
     * Return name of parameter in compiled SQLite3 statement.
     * @param pos parameter index, 1-based
     * @return String parameter name
     */
    CARAPI Bind_parameter_name(
        /** [in] **/Int32 pos,
        /** [out] **/String* str);

    /**
     * Return index of named parameter in compiled SQLite3 statement.
     * @param name of parameter
     * @return int index of parameter, 1-based
     */

    CARAPI Bind_parameter_index(
        /** [in] **/String name,
        /** [out] **/Int32* index);


    /**
     * Retrieve integer column from exec'ed SQLite3 statement.
     * @param col column number, 0-based
     * @return int column value
     */

    CARAPI Column_int(
        /** [in] **/Int32 col,
        /** [out] **/Int32* result);

    /**
     * Retrieve long column from exec'ed SQLite3 statement.
     * @param col column number, 0-based
     * @return long column value
     */
    CARAPI Column_long(
        /** [in] **/Int32 col,
        /** [out] **/Int64* result);

    /**
     * Retrieve double column from exec'ed SQLite3 statement.
     * @param col column number, 0-based
     * @return double column value
     */
    CARAPI Column_double(
        /** [in] **/Int32 col,
        /** [out] **/Double* result);

    /**
     * Retrieve blob column from exec'ed SQLite3 statement.
     * @param col column number, 0-based
     * @return byte[] column value
     */
    CARAPI Column_bytes(
        /** [in] **/Int32 col,
        /** [out] **/ArrayOf<Byte>* bt);

    /**
     * Retrieve string column from exec'ed SQLite3 statement.
     * @param col column number, 0-based
     * @return String column value
     */
    CARAPI Column_string(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    /**
     * Retrieve column type from exec'ed SQLite3 statement.
     * @param col column number, 0-based
     * @return column type code, e.g. SQLite.Constants.SQLITE_INTEGER
     */
    CARAPI Column_type(
        /** [in] **/Int32 col,
        /** [out] **/Int32* type);

    /**
     * Retrieve number of columns of exec'ed SQLite3 statement.
     * @return int number of columns
     */
    CARAPI Column_count(
        /** [out] **/Int32* count);

    /**
     * Retrieve column data as object from exec'ed SQLite3 statement.
     * @param col column number, 0-based
     * @return Object or null
     */
    CARAPI Column(
        /** [in] **/Int32 col,
        /** [out] **/IInterface** obj);

    /**
     * Return table name of column of SQLite3 statement.
     * @param col column number, 0-based
     * @return String or null
     */
    CARAPI Column_table_name(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    /**
     * Return database name of column of SQLite3 statement.
     * @param col column number, 0-based
     * @return String or null
     */
    CARAPI Column_database_name(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    /**
     * Return declared column type of SQLite3 statement.
     * @param col column number, 0-based
     * @return String or null
     */
    CARAPI Column_decltype(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    /**
     * Return origin column name of column of SQLite3 statement.
     * @param col column number, 0-based
     * @return String or null
     */
    CARAPI Column_origin_name(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

protected:
    /**
     * Destructor for object.
     */
    CARAPI Finalize();

private:
    /**
     * Internal native initializer.
     */
    CARAPI Internal_init();
};

#endif //_STMT_H_