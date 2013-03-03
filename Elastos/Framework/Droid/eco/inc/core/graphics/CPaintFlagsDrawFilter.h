
#ifndef __CPAINTFLAGSDRAWFILTER_H__
#define __CPAINTFLAGSDRAWFILTER_H__

#include "_CPaintFlagsDrawFilter.h"
#include "graphics/DrawFilter.h"

CarClass(CPaintFlagsDrawFilter), public DrawFilter
{
public:
    CARAPI constructor(
        /* [in] */ Int32 clearBits,
        /* [in] */ Int32 setBits);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(SkDrawFilter*) NativeConstructor(
        /* [in] */ Int32 clearBits,
        /* [in] */ Int32 setBits);
};

#endif // __CPAINTFLAGSDRAWFILTER_H__
