
#ifndef __CDRAWFILTER_H__
#define __CDRAWFILTER_H__

#include "ext/frameworkext.h"
#include <SkDrawFilter.h>


extern "C" const InterfaceID EIID_DrawFilter;

class DrawFilter
{
public:
    virtual ~DrawFilter();

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkDrawFilter* nativeDrawFilter);

protected:
    SkDrawFilter* nativeDrawFilter;
};

#endif // __CDRAWFILTER_H__
