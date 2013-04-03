
#ifndef __CFACEDETECTORFACE_H__
#define __CFACEDETECTORFACE_H__

#include "_CFaceDetectorFace.h"
#include "media/FaceDetector.h"

CarClass(CFaceDetectorFace), public FaceDetector::Face
{
public:
    CARAPI Confidence(
        /* [out] */ Float * pFactor);

    CARAPI GetMidPoint(
        /* [in] */ IPointF * pPoint);

    CARAPI EyesDistance(
        /* [out] */ Float * pDistance);

    CARAPI Pose(
        /* [in] */ Int32 euler,
        /* [out] */ Float * pAngle);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CFACEDETECTORFACE_H__
