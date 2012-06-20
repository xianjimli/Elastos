
#ifndef __CACTIVITYRESULT_H__
#define __CACTIVITYRESULT_H__

#include "_CActivityResult.h"
#include <elastos/AutoPtr.h>

CarClass(CActivityResult)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData);

    CARAPI GetResultCode(
        /* [out] */ Int32* resultCode);

    CARAPI GetResultData(
        /* [out] */ IIntent** resultData);

private:
    Int32 mResultCode;
    IIntent* mResultData;
};

#endif //__CACTIVITYRESULT_H__
