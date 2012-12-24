#ifdef _RESULTSETMETADATA_H_
#define _RESULTSETMETADATA_H_

#include <elastos.h>
#include "Wrapper.h"

using namespace Elastos;

class ResultSetMetaData : public Wrapper{
public:
	/**
     * Returns the title of an indexed column's catalog.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the catalog title.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetCatalogName(
        /** [in] **/Int32 column,
        /** [out] **/String* name)=0;

    /**
     * Returns the fully-qualified type of the class that is produced when
     * invoking {@code ResultSet.getObject} to recover this column's value.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the fully-qualified class name.
     * @throws SQLException
     *             if there is a database error.
     * @see ResultSet#getObject
     */
    virtual CARAPI GetColumnClassName(
        /** [in] **/Int32 column,
        /** [out] **/String* name)=0;

    /**
     * Returns number of columns contained in the associated result set.
     *
     * @return the column count.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetColumnCount(
        /** [out] **/Int32* count)=0;

    /**
     * Returns the indexed column's standard maximum width, expressed in number
     * of characters.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the column's max width.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI int GetColumnDisplaySize(
        /** [in] **/Int32 column,
        /** [out] **/Int32* size)=0;

    /**
     * Returns a recommended title for the indexed column, to be used when the
     * title needs to be displayed.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the column's title.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetColumnLabel(
        /** [in] **/Int32 column,
        /** [out] **/String* name)=0;

    /**
     * Returns the title of the indexed column.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the column title.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetColumnName(
        /** [in] **/Int32 column,
        /** [out] **/String* name)=0;

    /**
     * Returns the type of the indexed column as SQL type code.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the column type code.
     * @throws SQLException
     *             if there is a database error.
     * @see Types
     */
    virtual CARAPI GetColumnType(
        /** [in] **/Int32 column,
        /** [out] **/Int32* type)=0;

    /**
     * Returns the type name of the indexed column.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the type name.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetColumnTypeName(
        /** [in] **/Int32 column,
        /** [out] **/String* name)=0;

    /**
     * Returns the decimal precision of the indexed column.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the precision.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetPrecision(
        /** [in] **/Int32 column,
        /** [out] **/Int32* result)=0;

    /**
     * Returns the number of digits to the right of the decimal point of the
     * indexed column.
     *
     * @param column
     *            the column index, starting at 1.
     * @return number of decimal places.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetScale(
        /** [in] **/Int32 column,
        /** [out] **/Int32* result)=0;

    /**
     * Returns the name of the indexed columns schema.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the name of the columns schema.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI GetSchemaName(
        /** [in] **/Int32 column,
        /** [out] **/String* name)=0;

    /**
     * Returns the title of the indexed columns table.
     *
     * @param column
     *            the column index, starting at 1.
     * @return the table title.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI String GetTableName(
        /** [in] **/Int32 column,
        /** [out] **/String* name)=0;

    /**
     * Returns an indication of whether the indexed column is automatically
     * incremented and is therefore read-only.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if it is automatically numbered, {@code false}
     *         otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI boolean IsAutoIncrement(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;

    /**
     * Returns an indication of whether the case of the indexed column is
     * important.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if case matters, {@code false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI IsCaseSensitive(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;

    /**
     * Returns whether the indexed column contains a monetary amount.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if it is a monetary value, {@code false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI IsCurrency(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;

    /**
     * Returns an indication of whether writing to the indexed column is
     * guaranteed to be successful.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if the write is guaranteed, {@code false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI IsDefinitelyWritable(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;

    /**
     * Returns whether the indexed column is nullable.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if it is nullable, {@code false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI IsNullable(
        /** [in] **/Int32 column,
        /** [out] **/Int32* result)=0;

    /**
     * Returns an indication of whether writing to the indexed column is
     * guaranteed to be unsuccessful.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if the column is read-only, {@code false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI IsReadOnly(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;

    /**
     * Returns an indication of whether the indexed column is searchable.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if the indexed column is searchable, {@code false}
     *         otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI boolean IsSearchable(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;

    /**
     * Returns an indication of whether the values contained in the indexed
     * column are signed.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if they are signed, {@code false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI IsSigned(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;

    /**
     * Returns an indication of whether writing to the indexed column is
     * possible.
     *
     * @param column
     *            the column index, starting at 1.
     * @return {@code true} if it is possible to write, {@code false} otherwise.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI boolean IsWritable(
        /** [in] **/Int32 column,
        /** [out] **/Boolean* result)=0;
public:
    /**
     * Indicates that a column cannot contain {@code NULL} values.
     */
    static const Int32 columnNoNulls;// = 0;

    /**
     * Indicates that a column can contain {@code NULL} values.
     */
    static const Int32 columnNullable;// = 1;

    /**
     * Indicates that it is unknown whether a column can contain {@code NULL}s or not.
     */
    static const Int32 columnNullableUnknown;// = 2;
};
#endif//_RESULTSETMETADATA_H_
