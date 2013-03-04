
#include "ext/frameworkdef.h"
#include <gesture/CPrediction.h>

ECode CPrediction::constructor(
    /* [in] */ const String& label,
    /* [in] */ Double predictionScore)
{
    mName = label;
    mScore = predictionScore;
    return NOERROR;
}

ECode CPrediction::GetScore(
        /* [out] */ Double *score)
{
    VALIDATE_NOT_NULL(score);
    *score = mScore;
    return NOERROR;
}

ECode CPrediction::SetScore(
        /* [in] */ Double score)
{
    mScore = score;
    return NOERROR;
}

/*@Override*/
ECode CPrediction::ToString(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=mName;
    return NOERROR;
}
