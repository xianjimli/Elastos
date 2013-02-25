
#ifndef __CGESTUREUTILS_H__
#define __CGESTUREUTILS_H__

#include "_CGestureUtils.h"
#include "ext/frameworkext.h"
#include <elastos.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * Utility functions for gesture processing & analysis, including methods for:
 * <ul> 
 * <li>feature extraction (e.g., samplers and those for calculating bounding
 * boxes and gesture path lengths);
 * <li>geometric transformation (e.g., translation, rotation and scaling);
 * <li>gesture similarity comparison (e.g., calculating Euclidean or Cosine
 * distances between two gestures).
 * </ul>
 */
CarClass(CGestureUtils)
{
public:
    CARAPI constructor();
    /**
     * Closes the specified stream.
     *
     * @param stream The stream to close.
     */
    static CARAPI CloseStream(
        /* [in] */ ICloseable* stream);

    /**
     * Samples the gesture spatially by rendering the gesture into a 2D 
     * grayscale bitmap. Scales the gesture to fit the size of the bitmap. 
     * The scaling does not necessarily keep the aspect ratio of the gesture. 
     * 
     * @param gesture the gesture to be sampled
     * @param bitmapSize the size of the bitmap
     * @return a bitmapSize x bitmapSize grayscale bitmap that is represented 
     *         as a 1D array. The float at index i represents the grayscale 
     *         value at pixel [i%bitmapSize, i/bitmapSize] 
     */
    static CARAPI SpatialSampling(
        /* [in] */ IGesture* gesture,
        /* [in] */ Int32 bitmapSize,
        /* [out, callee] */ ArrayOf<Float> ** ret);

    /**
     * Samples the gesture spatially by rendering the gesture into a 2D 
     * grayscale bitmap. Scales the gesture to fit the size of the bitmap. 
     * 
     * @param gesture the gesture to be sampled
     * @param bitmapSize the size of the bitmap
     * @param keepAspectRatio if the scaling should keep the gesture's 
     *        aspect ratio
     * 
     * @return a bitmapSize x bitmapSize grayscale bitmap that is represented 
     *         as a 1D array. The float at index i represents the grayscale 
     *         value at pixel [i%bitmapSize, i/bitmapSize] 
     */
    static CARAPI SpatialSampling(
        /* [in] */ IGesture* gesture,
        /* [in] */ Int32 bitmapSize,
        /* [in] */ Boolean keepAspectRatio,
        /* [out, callee] */ ArrayOf<Float> ** ret);

    /**
     * Samples a stroke temporally into a given number of evenly-distributed 
     * points.
     * 
     * @param stroke the gesture stroke to be sampled
     * @param numPoints the number of points
     * @return the sampled points in the form of [x1, y1, x2, y2, ..., xn, yn]
     */
    static CARAPI TemporalSampling(
        /* [in] */ IGestureStroke* stroke,
        /* [in] */ Int32 numPoints,
        /* [out, callee] */ ArrayOf<Float> ** ret);

    /**
     * Calculates the centroid of a set of points.
     * 
     * @param points the points in the form of [x1, y1, x2, y2, ..., xn, yn]
     * @return the centroid
     */
    static CARAPI ComputeCentroid(
        /* [in] */ const ArrayOf<Float>& points,
        /* [out, callee] */ ArrayOf<Float> ** ret);


    static CARAPI_(Float) ComputeTotalLength(
        /* [in] */ const ArrayOf<Float>& points);

    static CARAPI_(Float) ComputeStraightness(
        /* [in] */ const ArrayOf<Float>& points);

    static CARAPI_(Float) ComputeStraightness(
        /* [in] */ const ArrayOf<Float>& points,
        /* [in] */ Float totalLen);

    /**
     * Calculates the squared Euclidean distance between two vectors.
     * 
     * @param vector1
     * @param vector2
     * @return the distance
     */
    static CARAPI_(Float) SquaredEuclideanDistance(
        /* [in] */ const ArrayOf<Float>& vector1,
        /* [in] */ const ArrayOf<Float>& vector2);

    /**
     * Calculates the cosine distance between two instances.
     * 
     * @param vector1
     * @param vector2
     * @return the distance between 0 and Math.PI
     */
    static CARAPI_(Float) CosineDistance(
        /* [in] */ const ArrayOf<Float>& vector1,
        /* [in] */ const ArrayOf<Float>& vector2);

    /**
     * Calculates the "minimum" cosine distance between two instances.
     * 
     * @param vector1
     * @param vector2
     * @param numOrientations the maximum number of orientation allowed
     * @return the distance between the two instances (between 0 and Math.PI)
     */
    static CARAPI_(Float) MinimumCosineDistance(
        /* [in] */ const ArrayOf<Float>& vector1,
        /* [in] */ const ArrayOf<Float>& vector2,
        /* [in] */ Int32 numOrientations);

    /**
     * Computes an oriented, minimum bounding box of a set of points.
     * 
     * @param originalPoints
     * @return an oriented bounding box
     */
    //public static OrientedBoundingBox computeOrientedBoundingBox(ArrayList<GesturePoint> originalPoints)
    static CARAPI_(IOrientedBoundingBox *) ComputeOrientedBoundingBox(
        /* [in] */ IObjectContainer* originalPoints);

    /**
     * Computes an oriented, minimum bounding box of a set of points.
     * 
     * @param originalPoints
     * @return an oriented bounding box
     */
    static CARAPI_(IOrientedBoundingBox *) ComputeOrientedBoundingBox(
        /* [in] */ const ArrayOf<Float>& originalPoints);

    static CARAPI_(ArrayOf<Float> *) Rotate(
        /* [out] */ ArrayOf<Float>* points,
        /* [in] */ Float angle);

    static CARAPI_(ArrayOf<Float> *)  Translate(
        /* [out] */ ArrayOf<Float>* points,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(ArrayOf<Float> *) Scale(
        /* [out] */ ArrayOf<Float>* points,
        /* [in] */ Float sx,
        /* [in] */ Float sy);

private:
    static const Float SCALING_THRESHOLD;// = 0.26f;
    static const Float NONUNIFORM_SCALE;// = (Float) Math::Sqrt(2);

    static void Plot(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ ArrayOf<Float>* sample,
        /* [in] */ Int32 sampleSize);

    /**
     * Calculates the variance-covariance matrix of a set of points.
     *
     * @param points the points in the form of [x1, y1, x2, y2, ..., xn, yn]
     * @return the variance-covariance matrix
     */
    static ArrayOf<ArrayOf_<Float,2> > *  ComputeCoVariance(
        /* [in] */ const ArrayOf<Float>& points);


    static CARAPI_(IOrientedBoundingBox *) ComputeOrientedBoundingBox(
        /* [out] */ ArrayOf<Float>* points,
        /* [in] */ const ArrayOf<Float>& centroid);

    static CARAPI_(ArrayOf<Float>*) ComputeOrientation(
        /* [in] */ const ArrayOf<ArrayOf_<Float,2> >&  covarianceMatrix);

};

#endif  //__CGESTUREUTILS_H__
