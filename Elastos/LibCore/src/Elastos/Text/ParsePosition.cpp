#include "ParsePosition.h"

ParsePosition::ParsePosition()
                    :mCurrentPosition(-1),
                     mErrorIndex(-1)
{}

ParsePosition::~ParsePosition()
{}

void ParsePosition::Init(
        /* [in] */ Int32 index)
{
    mCurrentPosition = index;
}

Int32 ParsePosition::GetErrorIndex()
{
    return mErrorIndex;
}

Int32 ParsePosition::GetIndex()
{
    return mCurrentPosition;
}

ECode ParsePosition::SetErrorIndex(
        /* [in] */ Int32 index)
{
    mErrorIndex = index;
    return NOERROR;
}

ECode ParsePosition::SetIndex(
        /* [in] */ Int32 index)
{
    mCurrentPosition = index;
    return NOERROR;
}