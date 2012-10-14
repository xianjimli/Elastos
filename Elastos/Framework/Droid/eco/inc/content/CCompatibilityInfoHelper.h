
#ifndef __CCOMPATIBILITYINFOHELPER_H__
#define __CCOMPATIBILITYINFOHELPER_H__

#include "_CCompatibilityInfoHelper.h"

CarClass(CCompatibilityInfoHelper)
{
public:
    CARAPI UpdateCompatibleScreenFrame(
        /* [in] */ IDisplayMetrics* dm,
        /* [in] */ Int32 orientation,
        /* [in, out] */ IRect* outRect);
};

#endif //__CCOMPATIBILITYINFOHELPER_H__
