
#ifndef __CPOINTFHELPER_H__
#define __CPOINTFHELPER_H__

#include "_CPointFHelper.h"

CarClass(CPointFHelper)
{
public:
    CARAPI Length(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Float* length);
};

#endif // __CPOINTFHELPER_H__
