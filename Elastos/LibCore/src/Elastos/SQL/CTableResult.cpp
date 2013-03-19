#include "cmdef.h"
#include "CTableResult.h"

ECode CTableResult::constructor()
{
    return TableResult::Init();
}

ECode CTableResult::constructor(
    /** [in] **/Int32 maxrows)
{
    return TableResult::Init(maxrows);
}
    
ECode CTableResult::Clear()
{
    return TableResult::Clear();
}

ECode CTableResult::Columns(
    /** [in] **/ArrayOf<String>* coldata)
{
    return TableResult::Columns(coldata);
}

ECode CTableResult::Types(
    /** [in] **/ArrayOf<String>* types)
{
    return TableResult::Types(types);
}

ECode CTableResult::Newrow(
    /** [in] **/ArrayOf<String>* rowdata,
    /** [out] **/Boolean* row)
{
    VALIDATE_NOT_NULL(row);

    return TableResult::Newrow(rowdata, row);
}

ECode CTableResult::ToString(
    /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);
    
    return TableResult::ToString(str);
}