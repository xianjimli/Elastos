
#include "CFunctionContext.h"

ECode CFunctionContext::Set_result(
    /* [in] */ const String& r)
{
    return FunctionContext::Set_result(r);
}

ECode CFunctionContext::Set_resultEx(
    /* [in] */ Int32 r)
{
    return FunctionContext::Set_resultEx(r);
}

ECode CFunctionContext::Set_resultEx2(
    /* [in] */ Double r)
{
    return FunctionContext::Set_resultEx2(r);
}

ECode CFunctionContext::Set_error(
    /* [in] */ const String& r)
{
    return FunctionContext::Set_error(r);
}

ECode CFunctionContext::Set_resultEx3(
    /* [in] */ const ArrayOf<Byte>& r)
{
    return FunctionContext::Set_resultEx3(r);
}

ECode CFunctionContext::Set_result_zeroblob(
    /* [in] */ Int32 n)
{
    return FunctionContext::Set_result_zeroblob(n);
}

ECode CFunctionContext::Count(
    /* [out] */ Int32* number)
{
    return FunctionContext::Count(number);
}

ECode CFunctionContext::constructor()
{
    return NOERROR;
}