
#ifndef __CFACEDETECTOR_H__
#define __CFACEDETECTOR_H__

#include "_CFaceDetector.h"
#include "media/FaceDetector.h"

CarClass(CFaceDetector), public FaceDetector
{
public:
    CARAPI FindFaces(
        /* [in] */ IBitmap * pBitmap,
        /* [in] */ const ArrayOf<IFaceDetectorFace *> & faces,
        /* [out] */ Int32 * pNum);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 maxFaces);

private:
    // TODO: Add your private member variables here.
};

#endif // __CFACEDETECTOR_H__
