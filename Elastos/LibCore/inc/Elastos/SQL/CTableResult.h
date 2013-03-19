#ifndef _CABLERESULT_H_
#define _CTABLERESULT_H_

#include "TableResult.h"
#include "_CTableResult.h"

CarClass(CTableResult), public TableResult
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /** [in] **/Int32 maxrows);
    
    CARAPI Clear();

    CARAPI Columns(
        /** [in] **/ArrayOf<String>* coldata);

    CARAPI Types(
        /** [in] **/ArrayOf<String>* types);

    CARAPI Newrow(
        /** [in] **/ArrayOf<String>* rowdata,
        /** [out] **/Boolean* row);

    CARAPI ToString(
        /** [out] **/String* str);
};

#endif //_CTABLERESULT_H_