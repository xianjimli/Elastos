
#include "cmdef.h"
#include "CParsePosition.h"
ECode CParsePosition::constructor(
    /* [in] */ Int32 index)
{
    ParsePosition::Init(index);
    return NOERROR;
}

ECode CParsePosition::GetErrorIndex(
    /* [out] */ Int32 * pErrorIndex)
{
    VALIDATE_NOT_NULL(pErrorIndex);

    *pErrorIndex = ParsePosition::GetErrorIndex();
    return NOERROR;
}

ECode CParsePosition::GetIndex(
    /* [out] */ Int32 * pIndex)
{
    VALIDATE_NOT_NULL(pIndex);

    *pIndex = ParsePosition::GetIndex();
    return NOERROR;
}

ECode CParsePosition::SetErrorIndex(
    /* [in] */ Int32 index)
{
    return ParsePosition::SetErrorIndex(index);
}

ECode CParsePosition::SetIndex(
    /* [in] */ Int32 index)
{
    return ParsePosition::SetIndex(index);
}



