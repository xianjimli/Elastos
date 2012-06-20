
#include "app/CActivityResult.h"

ECode CActivityResult::constructor(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData)
{
    mResultCode = resultCode;
    mResultData = resultData;
    return NOERROR;
}

ECode CActivityResult::GetResultCode(
    /* [out] */ Int32* resultCode)
{
    if (resultCode == NULL) return E_INVALID_ARGUMENT;

    *resultCode = mResultCode;
    return NOERROR;
}

ECode CActivityResult::GetResultData(
    /* [out] */ IIntent** resultData)
{
    if (resultData == NULL) return E_INVALID_ARGUMENT;

    *resultData = mResultData;
    if (*resultData != NULL) (*resultData)->AddRef();
    return NOERROR;
}
