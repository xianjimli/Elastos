
#include "ext/frameworkdef.h"
#include "utils/CParcelableStringWrapper.h"

ECode CParcelableStringWrapper::constructor()
{
    return NOERROR;
}

ECode CParcelableStringWrapper::constructor(
    /* [in] */ const String& str)
{
    if (str.IsNull()) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    mString = str;
    return NOERROR;
}

ECode CParcelableStringWrapper::GetLength(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mString.GetCharCount();
    return NOERROR;
}

ECode CParcelableStringWrapper::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    *c = mString.GetChar(index);

    return NOERROR;
}

ECode CParcelableStringWrapper::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcelableStringWrapper::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mString;
    return NOERROR;
}

ECode CParcelableStringWrapper::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return source->ReadString(&mString);
}

ECode CParcelableStringWrapper::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return dest->WriteString(mString);
}
