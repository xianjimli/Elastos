#include "BidiRun.h"
ECode BidiRun::Init(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ Int32 level)
{
    mStart = start;
    mLimit = limit;
    mLevel = level;
}

ECode BidiRun::GetLevel(
    /* [out] */ Int32 * level)
{
    assert(level != NULL);
    *level = mLevel;
    return NOERROR;
}

ECode BidiRun::GetLimit(
    /* [out] */ Int32 * limit)
{
    assert(limit != NULL);
    *limit = mLimit;
    return NOERROR;
}

ECode BidiRun::GetStart(
    /* [out] */ Int32 * start)
{
    assert(start != NULL);
    *start = mStart;
    return NOERROR;
}