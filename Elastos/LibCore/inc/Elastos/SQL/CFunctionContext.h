#ifndef _CFUNCTIONCONTEXT_H_
#define _CFUNCTIONCONTEXT_H_

#include "FunctionContext.h"
#include "_CFunctionContext.h"

CarClass(CFunctionContext), public FunctionContext
{
public:
    CARAPI constructor();

    CARAPI Set_result(
        /* [in] */const String& r);

    CARAPI Set_resultEx(
        /* [in] */ Int32 r);

    CARAPI Set_resultEx2(
        /* [in] */ Double r);

    CARAPI Set_error(
        /* [in] */const String& r);

    CARAPI Set_resultEx3(
        /* [in] */ const ArrayOf<Byte>& r);

    CARAPI Set_result_zeroblob(
        /* [in] */ Int32 n);

    CARAPI Count(
        /* [out] */ Int32* number);
};

#endif// _CFUNCTIONCONTEXT_H_