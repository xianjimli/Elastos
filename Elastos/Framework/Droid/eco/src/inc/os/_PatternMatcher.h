
#ifndef __PATTERNMATCHER_H__
#define __PATTERNMATCHER_H__

#include <elastos.h>

using namespace Elastos;

class _PatternMatcher
{
public:
    CARAPI_(String) GetPath();

    CARAPI_(Boolean) Match(
        /* [in] */ String str);
};

#endif //__PATTERNMATCHER_H__

