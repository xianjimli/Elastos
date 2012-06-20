
#ifndef __CPorterDuffColorFilter_h__
#define __CPorterDuffColorFilter_h__

#include "_CPorterDuffColorFilter.h"
#include "graphics/ColorFilter.h"

CarClass(CPorterDuffColorFilter), public ColorFilter
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ Int32 srcColor,
        /* [in] */ PorterDuffMode mode);
};

#endif //__CPorterDuffColorFilter_h__
