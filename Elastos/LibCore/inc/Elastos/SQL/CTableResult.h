#ifndef _CABLERESULT_H_
#define _CTABLERESULT_H_

#include "TableResult.h"
#include "_CTableResult.h"

CarClass(CTableResult), public TableResult
{
    CARAPI constructor();
    
    CARAPI Clear();

    CARAPI Columns(
        /** [in] **/const ArrayOf<String>& coldata);

    CARAPI Types(
        /** [in] **/const ArrayOf<String>& types);

    CARAPI Newrow(
        /** [in] **/const ArrayOf<String>& rowdata,
        /** [out] **/Boolean* row);

    CARAPI ToString(
        /** [out] **/String* str);
};

#endif //_CTABLERESULT_H_