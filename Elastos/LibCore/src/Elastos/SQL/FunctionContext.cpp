#include "FunctionContext.h"

FunctionContext::FunctionContext()
    :mHandle(0)
{
}

ECode FunctionContext::Set_result(
    /* [in] */ const String& r)
{
    return NOERROR;
}

ECode FunctionContext::Set_resultEx(
    /* [in] */ Int32 r)
{
    return NOERROR;
}

ECode FunctionContext::Set_resultEx2(
    /* [in] */ Double r)
{
    return NOERROR;
}

ECode FunctionContext::Set_error(
    /* [in] */ const String& r)
{
    return NOERROR;
}

ECode FunctionContext::Set_resultEx3(
    /* [in] */ const ArrayOf<Byte>& r)
{
    return NOERROR;
}

ECode FunctionContext::Set_result_zeroblob(
    /* [in] */ Int32 n)
{
    return NOERROR;
}

ECode FunctionContext::Count(
    /* [out] */ Int32* number)
{
    return NOERROR;
}

ECode FunctionContext::Internal_init()
{
    return NOERROR;
}

