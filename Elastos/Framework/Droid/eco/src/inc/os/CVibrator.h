
#ifndef __CVIBRATOR_H__
#define __CVIBRATOR_H__

#include "_CVibrator.h"

using namespace Elastos;

CarClass(CVibrator)
{
public:
    CARAPI Vibrate(
        /* [in] */ Int64 milliseconds);

    CARAPI VibrateEx(
        /* [in] */ const ArrayOf<Int64>& pattern,
        /* [in] */ Int32 repeat);

    CARAPI Cancel();

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CVIBRATOR_H__
