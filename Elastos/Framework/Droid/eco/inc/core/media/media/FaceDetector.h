
#ifndef __FACEDETECTOR_H__
#define __FACEDETECTOR_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * Identifies the faces of people in a 
 * {@link android.graphics.Bitmap} graphic object.
 */
class FaceDetector
{
public:
    /**
     * A Face contains all the information identifying the location
     * of a face in a bitmap.
     */
    class Face
    {
    public:
        /** The minimum confidence factor of good face recognition */
        static const Float CONFIDENCE_THRESHOLD = 0.4f;
        /** The x-axis Euler angle of a face. */
        static const Int32 EULER_X = 0;
        /** The y-axis Euler angle of a face. */
        static const Int32 EULER_Y = 1;
        /** The z-axis Euler angle of a face. */
        static const Int32 EULER_Z = 2;

        /** 
         * Returns a confidence factor between 0 and 1. This indicates how
         * certain what has been found is actually a face. A confidence
         * factor above 0.3 is usually good enough.
         */
        virtual CARAPI_(Float) Confidence();

        /**
         * Sets the position of the mid-point between the eyes.
         * @param point the PointF coordinates (Float values) of the 
         *              face's mid-point
         */
        virtual CARAPI GetMidPoint(
            /* [in] */ IPointF* point);

        /**
         * Returns the distance between the eyes.
         */
        virtual CARAPI_(Float) EyesDistance();

        /**
         * Returns the face's pose. That is, the rotations around either 
         * the X, Y or Z axis (the positions in 3-dimensional Euclidean space).
         * 
         * @param euler the Euler axis to retrieve an angle from 
         *              (<var>EULER_X</var>, <var>EULER_Y</var> or 
         *              <var>EULER_Z</var>)
         * @return the Euler angle of the of the face, for the given axis
         */
        virtual CARAPI_(Float) Pose(
            /* [in] */ Int32 euler);

    
        // private ctor, user not supposed to build this object
        Face();

    private:
        Float   mConfidence;
        Float   mMidPointX;
        Float   mMidPointY;
        Float   mEyesDist;
        Float   mPoseEulerX;
        Float   mPoseEulerY;
        Float   mPoseEulerZ;
    };


    FaceDetector();

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
    FaceDetector(
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height, 
        /* [in] */ Int32 maxFaces);

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
    virtual CARAPI_(Int32) FindFaces(
        /* [in] */ IBitmap* bitmap, 
        /* [in] */ ArrayOf<AutoPtr<IFaceDetectorFace> >* faces);

protected:
    /* no user serviceable parts here ... */
    virtual CARAPI_(void) Finalize();

private:
    /*
     * We use a class initializer to allow the native code to cache some
     * field offsets.
     */
    static Boolean sInitialized;
    static CARAPI_(void) nativeClassInit();

    /*static {
        sInitialized = FALSE;
        try {
            System.loadLibrary("FFTEm");
            nativeClassInit();
            sInitialized = true;
        } catch (UnsatisfiedLinkError e) {
            Log.d("FFTEm", "face detection library not found!");
        }
    }*/

    CARAPI_(Int32)  fft_initialize(
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height, 
        /* [in] */ Int32 maxFaces);

    CARAPI_(Int32)  fft_detect(
        /* [in] */ IBitmap* bitmap);
    
    CARAPI_(void) fft_get_face(
        /* [in] */ IFaceDetectorFace* face, 
        /* [in] */ Int32 i);
    
    CARAPI_(void) fft_destroy();

    Int32     mFD;
    Int32     mSDK;
    Int32     mDCR;
    Int32     mWidth;
    Int32     mHeight;
    Int32     mMaxFaces;    
    ArrayOf<Byte>*    mBWBuffer;
};

#endif

