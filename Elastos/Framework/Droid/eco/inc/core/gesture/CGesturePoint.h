
#ifndef __CGESTUREPOINT_H__
#define __CGESTUREPOINT_H__

#include "_CGesturePoint.h"
#include "ext/frameworkext.h"
#include <Elastos.IO.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * A timed point of a gesture stroke. Multiple points form a stroke.
 */
CarClass(CGesturePoint)
{
public:
    CARAPI GetX(
        /* [out] */ Float * x);

    CARAPI GetY(
        /* [out] */ Float * y);

    CARAPI GetTimestamp(
        /* [out] */ Int64 * timestamp);

    CARAPI Clone(
        /* [out] */ IGesturePoint ** gesturePoint);

    CARAPI constructor(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int64 t);

    static CARAPI Deserialize(
        /* [in] */ IDataInputStream* inStream,
        /* [out] */ IGesturePoint** gesturePoint);

    static CARAPI DeserializeEx(
        /* [in] */ IParcel* inStream,
        /* [out] */ IGesturePoint** gesturePoint);

private:
    Float mX;
    Float mY;
    Int64 mTimestamp;
};

#endif //__CGESTUREPOINT_H__
