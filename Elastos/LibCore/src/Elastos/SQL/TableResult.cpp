#include "TableResult.h"

TableResult::TableResult()
    :mMaxrows(0)
{
}

TableResult::TableResult(
    /** [in] **/Int32 maxrows)
    :mMaxrows(0)
{
}

ECode TableResult::Clear()
{
    return NOERROR;
}

ECode TableResult::Columns(
    /** [in] **/const ArrayOf<String>& coldata)
{
    return NOERROR;
}

ECode TableResult::Types(
    /** [in] **/const ArrayOf<String>& types)
{
    return NOERROR;
}

ECode TableResult::Newrow(
    /** [in] **/const ArrayOf<String>& rowdata,
    /** [out] **/Boolean* row)
{
    return NOERROR;
}

ECode TableResult::ToString(
    /** [out] **/String* str)
{
    return NOERROR;
}