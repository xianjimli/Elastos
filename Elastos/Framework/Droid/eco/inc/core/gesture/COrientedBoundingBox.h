
#ifndef __CORIENTEDBOUNDINGBOX_H__
#define __CORIENTEDBOUNDINGBOX_H__

#include "_COrientedBoundingBox.h"
#include <elastos.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * An oriented bounding box
 */
CarClass(COrientedBoundingBox)
{
public:
    CARAPI GetSquareness(
        /* [out] */ Float * squareness);

    CARAPI GetWidth(
        /* [out] */ Float * width);

    CARAPI GetHeight(
        /* [out] */ Float * height);

    CARAPI GetOrientation(
        /* [out] */ Float * orientation);

    CARAPI GetCenterX(
        /* [out] */ Float * centerX);
    
    CARAPI GetCenterY(
        /* [out] */ Float * centerY);

    CARAPI constructor(
        /* [in] */ Float angle,
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Float w,
        /* [in] */ Float h);

    /**
     * Currently used for debugging purpose only.
     *
     * @hide
     */
    CARAPI ToPath(
         /* [out] */ IPath** path);

private:
    Float mSquareness;
    Float mWidth;
    Float mHeight;
    Float mOrientation;
    Float mCenterX;
    Float mCenterY;
};

#endif //__CORIENTEDBOUNDINGBOX_H__

