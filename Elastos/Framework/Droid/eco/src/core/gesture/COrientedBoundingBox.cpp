
#include <gesture/COrientedBoundingBox.h>
#include <graphics/CPath.h>
#include <graphics/CMatrix.h>
#include "ext/frameworkext.h"

using namespace Elastos;
using namespace Elastos::Core;

/**
 * An oriented bounding box
 */
ECode COrientedBoundingBox::constructor(
    /* [in] */ Float angle,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float w,
    /* [in] */ Float h)
{
    mOrientation = angle;
    mWidth = w;
    mHeight = h;
    mCenterX = cx;
    mCenterY = cy;
    Float ratio = w / h;
    if (ratio > 1) {
        mSquareness = 1 / ratio;
    } else {
        mSquareness = ratio;
    }
    return NOERROR;
}

/**
 * Currently used for debugging purpose only.
 *
 * @hide
 */

ECode COrientedBoundingBox::ToPath(
    /* [out] */ IPath** path )
{
    VALIDATE_NOT_NULL(path);
    CPath::New(path);
    ArrayOf<Float> *point = ArrayOf<Float>::Alloc(2);
    (*point)[0] = -mWidth / 2;
    (*point)[1] = mHeight / 2;
    IMatrix* matrix = NULL;
    CMatrix::New(&matrix);
    matrix->SetRotateEx(mOrientation);
    Boolean res;
    matrix->PostTranslate(mCenterX, mCenterY, &res);
    matrix->MapPointsEx2(*point);
    (*path)->MoveTo((*point)[0], (*point)[1]);

    (*point)[0] = -mWidth / 2;
    (*point)[1] = -mHeight / 2;
    matrix->MapPointsEx2(*point);
    (*path)->LineTo((*point)[0], (*point)[1]);

    (*point)[0] = mWidth / 2;
    (*point)[1] = -mHeight / 2;
    matrix->MapPointsEx2(*point);
    (*path)->LineTo((*point)[0], (*point)[1]);

    (*point)[0] = mWidth / 2;
    (*point)[1] = mHeight / 2;
    matrix->MapPointsEx2(*point);
    (*path)->LineTo((*point)[0], (*point)[1]);

    (*path)->Close();

    return NOERROR;

}

ECode COrientedBoundingBox::GetSquareness(
    /* [out] */ Float * squareness)
{
    VALIDATE_NOT_NULL(squareness);
    *squareness = mSquareness;
    return NOERROR;
}

ECode COrientedBoundingBox::GetWidth(
    /* [out] */ Float * width)
{
    VALIDATE_NOT_NULL(width);
    *width = mWidth;
    return NOERROR;
}

ECode COrientedBoundingBox::GetHeight(
    /* [out] */ Float * height)
{
    VALIDATE_NOT_NULL(height);
    *height = mHeight;
    return NOERROR;
}

ECode COrientedBoundingBox::GetOrientation(
    /* [out] */ Float * orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mOrientation;
    return NOERROR;
}

ECode COrientedBoundingBox::GetCenterX(
    /* [out] */ Float * centerX)
{
    VALIDATE_NOT_NULL(centerX);
    *centerX = mCenterX;
    return NOERROR;
}

ECode COrientedBoundingBox::GetCenterY(
    /* [out] */ Float * centerY)
{
    VALIDATE_NOT_NULL(centerY);
    *centerY = mCenterY;
    return NOERROR;
}

