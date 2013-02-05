
#ifndef __CDRAWFILTER_H__
#define __CDRAWFILTER_H__

#include "_CDrawFilter.h"
#include "graphics/DrawFilter.h"
#include "SkDrawFilter.h"


CarClass(CDrawFilter)//, public DrawFilter
{
public:
    CARAPI constructor();
    ~CDrawFilter();
private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkDrawFilter* nativeDrawFilter);
};

#endif // __CDRAWFILTER_H__
