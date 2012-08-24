
#ifndef __CPATTERNMATCHER_H__
#define __CPATTERNMATCHER_H__

#include "_CPatternMatcher.h"
#include "os/_PatternMatcher.h"

CarClass(CPatternMatcher), public _PatternMatcher
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetType(
        /* [out] */ Int32* path);

    CARAPI Match(
        /* [in] */ const String& str,
        /* [out] */ Boolean* isMatch);

    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI DescribeContents(
        /* [out] */ Int32* count);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ Int32 type);

    CARAPI constructor(
        /* [in] */ IParcel* src);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPATTERNMATCHER_H__
