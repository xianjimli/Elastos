#ifndef _TABLERESULT_H_
#define _TABLERESULT_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
//#include <elastos/Vector.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class TableResult
{
public:
    /**
     * Number of columns in the result set.
     */
    Int32 mNcolumns;

    /**
     * Number of rows in the result set.
     */
    Int32 mNrows;

    /**
     * Column names of the result set.
     */
    ArrayOf<String>* mColumn;

    /**
     * Types of columns of the result set or null.
     */
    ArrayOf<String>* mTypes;

    /**
     * Rows of the result set. Each row is stored as a String array.
     */
 //   Elastos::Vector<ArrayOf<String>*> *mRows;

    /**
     * Maximum number of rows to hold in the table.
     */
    Int32 mMaxrows;// = 0;

    /**
     * Flag to indicate Maximum number of rows condition.
     */
    Boolean mAtmaxrows;

public:
    /**
     * Create an empty result set.
     */
    TableResult();

    /**
     * Create an empty result set with maximum number of rows.
     */
    TableResult(
        /** [in] **/Int32 maxrows);

    CARAPI Init();

    CARAPI Init(
        /** [in] **/Int32 maxrows);

    /**
     * Clear result set.
     */
    CARAPI Clear();

    /**
     * Callback method used while the query is executed.
     */
    CARAPI Columns(
        /** [in] **/ArrayOf<String>* coldata);

    /**
     * Callback method used while the query is executed.
     */
    CARAPI Types(
        /** [in] **/ArrayOf<String>* types);

    /**
     * Callback method used while the query is executed.
     */
    CARAPI Newrow(
        /** [in] **/ArrayOf<String>* rowdata,
        /** [out] **/Boolean* row);

    /**
     * Make String representation of result set.
     */
    CARAPI ToString(
        /** [out] **/String* str);
};
#endif //_TABLERESULT_H_