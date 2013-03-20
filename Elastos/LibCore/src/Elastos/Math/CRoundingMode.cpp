
#include "CRoundingMode.h"

ECode CRoundingMode::constructor(
    /* [in] */ Int32 rm)
{
    return RoundingMode::Init(rm);
}

ECode CRoundingMode::ValueOf(
    /* [in] */ Int32 mode,
    /* [out] */ RRoundingMode * pNewObj)
{
    return RoundingMode::ValueOf(mode, pNewObj);
}

//ECode CRoundingMode::Func(
//    /* [in] */ RoundingMode roundingModeEnum)
//{
    // TODO: Add your code here
//    return E_NOT_IMPLEMENTED;
//}


