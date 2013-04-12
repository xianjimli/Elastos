
#include "CBidiRun.h"

ECode CBidiRun::constructor(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ Int32 level)
{
    return BidiRun::Init(start, limit, level);
}

ECode CBidiRun::GetLevel(
    /* [out] */ Int32 * pLevel)
{
    return BidiRun::GetLevel(pLevel);
}

ECode CBidiRun::GetLimit(
    /* [out] */ Int32 * pLimit)
{
    return BidiRun::GetLimit(pLimit);
}

ECode CBidiRun::GetStart(
    /* [out] */ Int32 * pStart)
{
    return BidiRun::GetStart(pStart);
}


