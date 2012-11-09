
#include "CStringWrapper.h"
#include "cmdef.h"

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
    *str = mString;
    return NOERROR;
}

ECode CStringWrapper::constructor(
    /* [in] */ const String& str)
{
    if (str.IsNull())
        mString = String("");
    else
        mString = str;
    return NOERROR;
}
