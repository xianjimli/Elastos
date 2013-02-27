
#ifndef __CPOINTFHELPER_H__
#define __CPOINTFHELPER_H__

#include "_CPointFHelper.h"

CarClass(CPointFHelper)
{
public:
    CARAPI Length(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Float * pLength);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPOINTFHELPER_H__
