
#include "CStringWrapper.h"
#include "cmdef.h"

CStringWrapper::~CStringWrapper()
{
    String::Free(mString);
}

ECode CStringWrapper::GetLength(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mString.GetCharCount();
    return NOERROR;
}

ECode CStringWrapper::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    *c = mString.GetChar(index);
    return NOERROR;
}

ECode CStringWrapper::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringWrapper::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String::Duplicate(mString);
    return NOERROR;
}

ECode CStringWrapper::constructor(
    /* [in] */ String str)
{
    mString = str;
    return NOERROR;
}