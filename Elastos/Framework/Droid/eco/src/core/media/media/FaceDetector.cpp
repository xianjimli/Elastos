
#include "media/FaceDetector.h"
#include "media/CFaceDetectorFace.h"

Boolean FaceDetector::sInitialized;

/** 
 * Returns a confidence factor between 0 and 1. This indicates how
 * certain what has been found is actually a face. A confidence
 * factor above 0.3 is usually good enough.
 */
Float FaceDetector::Face::Confidence() 
{
    return mConfidence;
}

/**
 * Sets the position of the mid-point between the eyes.
 * @param point the PointF coordinates (Float values) of the 
 *              face's mid-point
 */
ECode FaceDetector::Face::GetMidPoint(
    /* [in] */ IPointF* point) 
{
    // don't return a PointF to avoid allocations
    point->Set(mMidPointX, mMidPointY);
}

/**
 * Returns the distance between the eyes.
 */
Float FaceDetector::Face::EyesDistance()
{
    return mEyesDist;
}
/**
 * Returns the face's pose. That is, the rotations around either 
 * the X, Y or Z axis (the positions in 3-dimensional Euclidean space).
 * 
 * @param euler the Euler axis to retrieve an angle from 
 *              (<var>EULER_X</var>, <var>EULER_Y</var> or 
 *              <var>EULER_Z</var>)
 * @return the Euler angle of the of the face, for the given axis
 */
Float FaceDetector::Face::Pose(
    /* [in] */ Int32 euler)
{
    // don't use an array to avoid allocations
    if (euler == EULER_X)
        return mPoseEulerX;
    else if (euler == EULER_Y)
        return mPoseEulerY;
    else if (euler == EULER_Z)
        return mPoseEulerZ;
   //throw new IllegalArgumentException();
}

// private ctor, user not supposed to build this object
FaceDetector::Face::Face()
{
}


FaceDetector::FaceDetector()
{

}

/**
 * Creates a FaceDetector, configured with the size of the images to
 * be analysed and the maximum number of faces that can be detected.
 * These parameters cannot be changed once the object is constructed.
 * Note that the width of the image must be even.
 * 
 * @param width  the width of the image
 * @param height the height of the image
 * @param maxFaces the maximum number of faces to identify
 *
 */
FaceDetector::FaceDetector(
    /* [in] */ Int32 width, 
    /* [in] */ Int32 height, 
    /* [in] */ Int32 maxFaces)
{
    if (!sInitialized) {
        return;
    }
    fft_initialize(width, height, maxFaces);
    mWidth = width;
    mHeight = height;
    mMaxFaces = maxFaces;
    mBWBuffer = ArrayOf<Byte>::Alloc(width * height);
}

/**
 * Finds all the faces found in a given {@link android.graphics.Bitmap}. 
 * The supplied array is populated with {@link FaceDetector.Face}s for each
 * face found. The bitmap must be in 565 format (for now).
 * 
 * @param bitmap the {@link android.graphics.Bitmap} graphic to be analyzed
 * @param faces  an array in which to place all found 
 *               {@link FaceDetector.Face}s. The array must be sized equal
 *               to the <var>maxFaces</var> value set at initialization
 * @return the number of faces found
 * @throws IllegalArgumentException if the Bitmap dimensions don't match
 *               the dimensions defined at initialization or the given array 
 *               is not sized equal to the <var>maxFaces</var> value defined
 *               at initialization
 */
Int32 FaceDetector::FindFaces(
    /* [in] */ IBitmap* bitmap, 
    /* [in] */ ArrayOf<AutoPtr<IFaceDetectorFace> >* faces)
{
    if (!sInitialized) {
        return 0;
    }

    Int32 w, h;
    bitmap->GetWidth(&w);
    bitmap->GetHeight(&h);

    if (w != mWidth ||h  != mHeight) {
        /*throw new IllegalArgumentException(
                "bitmap size doesn't match initialization");*/
    }
    if (faces->GetLength() < mMaxFaces) {
        /*throw new IllegalArgumentException(
                "faces[] smaller than maxFaces");*/
    }
    
    Int32 numFaces = fft_detect(bitmap);
    if (numFaces >= mMaxFaces)
        numFaces = mMaxFaces;
    for (Int32 i=0 ; i<numFaces ; i++) {
        if ((*faces)[i] == NULL) {
            CFaceDetectorFace::New((IFaceDetectorFace**)&(*faces)[i]);
        }
        fft_get_face((*faces)[i], i);
    }
    return numFaces;
}


/* no user serviceable parts here ... */
void FaceDetector::Finalize() 
{
    fft_destroy();
}


void FaceDetector::nativeClassInit()
{

}

Int32  FaceDetector::fft_initialize(
    /* [in] */ Int32 width, 
    /* [in] */ Int32 height, 
    /* [in] */ Int32 maxFaces)
{
    return 0;
}

Int32  FaceDetector::fft_detect(
    /* [in] */ IBitmap* bitmap)
{
    return 0;
}

void FaceDetector::fft_get_face(
    /* [in] */ IFaceDetectorFace* face, 
    /* [in] */ Int32 i)
{

}

void FaceDetector::fft_destroy()
{

}

