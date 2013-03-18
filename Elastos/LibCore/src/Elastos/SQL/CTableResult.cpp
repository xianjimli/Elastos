#include "CTableResult.h"

ECode CTableResult::constructor()
{
    return NOERROR;
}
    
ECode CTableResult::Clear()
{
    return TableResult::Clear();
}

ECode CTableResult::Columns(
    /** [in] **/const ArrayOf<String>& coldata)
{
    return TableResult::Columns(coldata);
}

ECode CTableResult::Types(
    /** [in] **/const ArrayOf<String>& types)
{
    return TableResult::Types(types);
}

ECode CTableResult::Newrow(
    /** [in] **/const ArrayOf<String>& rowdata,
    /** [out] **/Boolean* row)
{
    return TableResult::Newrow(rowdata, row);
}

ECode CTableResult::ToString(
    /** [out] **/String* str)
{
    return TableResult::ToString(str);
}