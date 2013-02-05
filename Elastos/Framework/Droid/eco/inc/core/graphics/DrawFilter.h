
#ifndef __CDRAWFILTER_H__
#define __CDRAWFILTER_H__

#include "ext/frameworkext.h"
#include <elastos.h>
#include "SkDrawFilter.h"

class DrawFilter
{
public:
    DrawFilter();
    ~DrawFilter();

protected:
    SkDrawFilter* nativeDrawFilter;
private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkDrawFilter* nativeDrawFilter);

};

#endif // __CDRAWFILTER_H__
