#include "BidiRun.h"
BidiRun::BidiRun(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ Int32 level)
    : mStart(start)
    , mLimit(limit)
    , mLevel(level)
{ }

Int32 BidiRun::GetLevel()
{
    return mLevel;
}

Int32 BidiRun::GetLimit()
{
    return mLimit;
}

Int32 BidiRun::GetStart()
{
    return mStart;
}