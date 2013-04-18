
#include "CCoderResultHelper.h"
#include "CCoderResult.h"

ECode CCoderResultHelper::GetUNDERFLOW(
    /* [out] */ ICoderResult** result){
    return CCoderResult::GetUNDERFLOW(result);
}

ECode CCoderResultHelper::GetOVERFLOW(
    /* [out] */ ICoderResult** result){
    return CCoderResult::GetOVERFLOW(result);
}

ECode CCoderResultHelper::MalformedForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    return CCoderResult::MalformedForLength(length, result);
}

ECode CCoderResultHelper::UnmappableForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    return CCoderResult::UnmappableForLength(length, result);
}