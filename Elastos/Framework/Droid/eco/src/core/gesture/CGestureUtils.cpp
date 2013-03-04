
#include <gesture/CGestureUtils.h>
#include <gesture/COrientedBoundingBox.h>
#include "ext/frameworkext.h"
#include <elastos.h>
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const Float CGestureUtils::SCALING_THRESHOLD = 0.26f;
const Float CGestureUtils::NONUNIFORM_SCALE = (Float) Math::Sqrt(2);

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

/**
 * Closes the specified stream.
 *
 * @param stream The stream to close.
 */
ECode CGestureUtils::constructor()
{
    return NOERROR;
}

ECode CGestureUtils::CloseStream(
    /* [in] */ ICloseable* stream)
{
    if (stream != NULL) {
       if (FAILED(stream->Close() ) ) {
           Logger::E(GestureConstants_LOG_TAG, "Could not close stream");
        }
    }
    return NOERROR;
}
    
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
ECode CGestureUtils::SpatialSampling(
    /* [in] */ IGesture* gesture,
    /* [in] */ Int32 bitmapSize,
    /* [out, callee] */ ArrayOf<Float> ** ret)
{
    VALIDATE_NOT_NULL(gesture);
    VALIDATE_NOT_NULL(ret);
    return SpatialSampling(gesture, bitmapSize, FALSE, ret);
}

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
ECode CGestureUtils::SpatialSampling(
    /* [in] */ IGesture* gesture,
    /* [in] */ Int32 bitmapSize,
    /* [in] */ Boolean keepAspectRatio,
    /* [out, callee] */ ArrayOf<Float> ** ret)
{
    VALIDATE_NOT_NULL(gesture);
    VALIDATE_NOT_NULL(ret);
    Float targetPatchSize = bitmapSize - 1;
    ArrayOf<Float>* sample = ArrayOf<Float>::Alloc(bitmapSize * bitmapSize);

    //Arrays.fill(sample, 0);
    for (Int32 i=0; i<sample->GetLength();i++)
    {
         (*sample)[i]=0;
    }
  
    IRectF* rect = NULL;
    gesture->GetBoundingBox(&rect);
    Float gestureWidth;
    rect->Width(&gestureWidth);
    Float gestureHeight;
    rect->Height(&gestureHeight);
    Float sx = targetPatchSize / gestureWidth;
    Float sy = targetPatchSize / gestureHeight;
        
    if (keepAspectRatio) {
        Float scale = sx < sy ? sx : sy;
        sx = scale;
        sy = scale;
    } else {

        Float aspectRatio = gestureWidth / gestureHeight;
        if (aspectRatio > 1) {
            aspectRatio = 1 / aspectRatio;
         }
        if (aspectRatio < SCALING_THRESHOLD) {
            Float scale = sx < sy ? sx : sy;
            sx = scale;
            sy = scale;
        } else {
            if (sx > sy) {
                Float scale = sy * NONUNIFORM_SCALE;
                if (scale < sx) {
                    sx = scale;
                  }
            } else {
                Float scale = sx * NONUNIFORM_SCALE;
                if (scale < sy) {
                    sy = scale;
                  }
              }
        }
    }
    Float centerX;
    rect->CenterX(&centerX);
    Float centerY;
    rect->CenterY(&centerY);
    Float preDx = -centerX;
    Float preDy = -centerY;
    Float postDx = targetPatchSize / 2;
    Float postDy = targetPatchSize / 2;

    //List<IGestureStroke*>
    IObjectContainer * strokes = NULL;
    gesture->GetStrokes(&strokes);
    Int32 count = 0;
    strokes->GetObjectCount(&count);
    if (count > 0) {
        Int32 size;
        Float xpos;
        Float ypos;
        AutoPtr<IObjectEnumerator> iter;
        strokes->GetObjectEnumerator((IObjectEnumerator**)&iter);
        Boolean hasNext = FALSE;
//        Int32 index = 0;
        iter->MoveNext(&hasNext);
        while(hasNext) {
            IGestureStroke* stroke = NULL;
            iter->Current((IInterface**)&stroke);

        //for (int index = 0; index < count; index++) {
        //    final GestureStroke stroke = strokes.get(index);
            ArrayOf<Float>* strokepoints = NULL;
            stroke->GetPoints(&strokepoints);
            size = strokepoints->GetLength();
       /////////////////////////////////////////////////////////////////////////

            ArrayOf<Float>* pts = ArrayOf<Float>::Alloc(size);
            for (Int32 i = 0; i < size; i += 2) {
                (*pts)[i] = ((*strokepoints)[i] + preDx) * sx + postDx;
                (*pts)[i + 1] = ((*strokepoints)[i + 1] + preDy) * sy + postDy;
            }
            Float segmentEndX = -1;
            Float segmentEndY = -1;
            for (Int32 i = 0; i < size; i += 2) {
                Float segmentStartX = (*pts)[i] < 0 ? 0 : (*pts)[i];
                Float segmentStartY = (*pts)[i + 1] < 0 ? 0 : (*pts)[i + 1];
                if (segmentStartX > targetPatchSize) {
                    segmentStartX = targetPatchSize;
                }
                if (segmentStartY > targetPatchSize) {
                    segmentStartY = targetPatchSize;
                }
                Plot(segmentStartX, segmentStartY, sample, bitmapSize);
                if (segmentEndX != -1) {
                    // Evaluate horizontally
                    if (segmentEndX > segmentStartX) {
                        xpos = (Float) Math::Ceil(segmentStartX);
                        Float slope = (segmentEndY - segmentStartY) /
                                      (segmentEndX - segmentStartX);
                        while (xpos < segmentEndX) {
                            ypos = slope * (xpos - segmentStartX) + segmentStartY;
                            Plot(xpos, ypos, sample, bitmapSize);
                            xpos++;
                        }
                    } else if (segmentEndX < segmentStartX){
                        xpos = (Float) Math::Ceil(segmentEndX);
                        Float slope = (segmentEndY - segmentStartY) /
                                      (segmentEndX - segmentStartX);
                        while (xpos < segmentStartX) {
                            ypos = slope * (xpos - segmentStartX) + segmentStartY;
                            Plot(xpos, ypos, sample, bitmapSize);
                            xpos++;
                        }
                    }
                    // Evaluate vertically
                    if (segmentEndY > segmentStartY) {
                        ypos = (Float) Math::Ceil(segmentStartY);
                        Float invertSlope = (segmentEndX - segmentStartX) /
                                            (segmentEndY - segmentStartY);
                        while (ypos < segmentEndY) {
                            xpos = invertSlope * (ypos - segmentStartY) + segmentStartX;
                            Plot(xpos, ypos, sample, bitmapSize);
                            ypos++;
                        }
                    } else if (segmentEndY < segmentStartY) {
                        ypos = (Float) Math::Ceil(segmentEndY);
                        Float invertSlope = (segmentEndX - segmentStartX) /
                                            (segmentEndY - segmentStartY);
                        while (ypos < segmentStartY) {
                            xpos = invertSlope * (ypos - segmentStartY) + segmentStartX;
                            Plot(xpos, ypos, sample, bitmapSize);
                            ypos++;
                        }
                    }
                }
                segmentEndX = segmentStartX;
                segmentEndY = segmentStartY;
            }

            ArrayOf<Float>::Free(strokepoints);
            ArrayOf<Float>::Free(pts);
            iter->MoveNext(&hasNext);
        } //end while(hasNext)
    }//end if (count>0)
    strokes->Release();
    rect->Release();
    *ret = sample;
    return NOERROR;
}
  
void CGestureUtils::Plot(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ ArrayOf<Float>* sample,  //sample will be modified
    /* [in] */ Int32 sampleSize)
{
    assert(sample != NULL);
    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;
    Int32 xFloor = (Int32) Math::Floor(x);
    Int32 xCeiling = (Int32) Math::Ceil(x);
    Int32 yFloor = (Int32) Math::Floor(y);
    Int32 yCeiling = (Int32) Math::Ceil(y);

    // if it's an integer
    if (x == xFloor && y == yFloor) {
        Int32 index = yCeiling * sampleSize + xCeiling;
        if ((*sample)[index] < 1){
            (*sample)[index] = 1;
        }
    } else {
        Double xFloorSq = Math::Pow(xFloor - x, 2);
        Double yFloorSq = Math::Pow(yFloor - y, 2);
        Double xCeilingSq = Math::Pow(xCeiling - x, 2);
        Double yCeilingSq = Math::Pow(yCeiling - y, 2);
        Float topLeft = (Float) Math::Sqrt(xFloorSq + yFloorSq);
        Float topRight = (Float) Math::Sqrt(xCeilingSq + yFloorSq);
        Float btmLeft = (Float) Math::Sqrt(xFloorSq + yCeilingSq);
        Float btmRight = (Float) Math::Sqrt(xCeilingSq + yCeilingSq);
        Float sum = topLeft + topRight + btmLeft + btmRight;

        Float value = topLeft / sum;
        Int32 index = yFloor * sampleSize + xFloor;
        if (value > (*sample)[index]){
            (*sample)[index] = value;
        }

        value = topRight / sum;
        index = yFloor * sampleSize + xCeiling;
        if (value > (*sample)[index]){
            (*sample)[index] = value;
        }

        value = btmLeft / sum;
        index = yCeiling * sampleSize + xFloor;
        if (value > (*sample)[index]){
            (*sample)[index] = value;
        }
        
        value = btmRight / sum;
        index = yCeiling * sampleSize + xCeiling;
        if (value > (*sample)[index]){
            (*sample)[index] = value;
        }
    }
}
/**
 * Samples a stroke temporally into a given number of evenly-distributed
 * points.
 *
 * @param stroke the gesture stroke to be sampled
 * @param numPoints the number of points
 * @return the sampled points in the form of [x1, y1, x2, y2, ..., xn, yn]
 */
ECode CGestureUtils::TemporalSampling(
   /* [in] */ IGestureStroke* stroke,
   /* [in] */ Int32 numPoints,
   /* [out, callee] */ ArrayOf<Float> ** ret)
{
    VALIDATE_NOT_NULL(stroke);
    VALIDATE_NOT_NULL(ret);
    Float strokeLength;
    stroke->GetLength(&strokeLength);
    Float increment = strokeLength / (numPoints - 1);
    Int32 vectorLength = numPoints * 2;
    ArrayOf<Float>* vector = ArrayOf<Float>::Alloc(vectorLength);
    Float distanceSoFar = 0;
    ArrayOf<Float>* pts =NULL;
    stroke->GetPoints(&pts);
    Float lstPointX = (*pts)[0];
    Float lstPointY = (*pts)[1];
    Int32 index = 0;
    Float currentPointX = Math::FLOAT_MIN_VALUE;
    Float currentPointY = Math::FLOAT_MIN_VALUE;
    (*vector)[index] = lstPointX;
    index++;
    (*vector)[index] = lstPointY;
    index++;
    Int32 i = 0;
    Int32 count = pts->GetLength() / 2;
    while (i < count) {
        if (currentPointX == Math::FLOAT_MIN_VALUE) {
            i++;
            if (i >= count) {
                break;
            }
            currentPointX = (*pts)[i * 2];
            currentPointY = (*pts)[i * 2 + 1];
        }
        Float deltaX = currentPointX - lstPointX;
        Float deltaY = currentPointY - lstPointY;
        Float distance = (Float) Math::Sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distanceSoFar + distance >= increment) {
            Float ratio = (increment - distanceSoFar) / distance;
            Float nx = lstPointX + ratio * deltaX;
            Float ny = lstPointY + ratio * deltaY;
            (*vector)[index] = nx;
            index++;
            (*vector)[index] = ny;
            index++;
            lstPointX = nx;
            lstPointY = ny;
            distanceSoFar = 0;
        } else {
            lstPointX = currentPointX;
            lstPointY = currentPointY;
            currentPointX = Math::FLOAT_MIN_VALUE;
            currentPointY = Math::FLOAT_MIN_VALUE;
            distanceSoFar += distance;
        }
    }

    for (i = index; i < vectorLength; i += 2) {
        (*vector)[i] = lstPointX;
        (*vector)[i + 1] = lstPointY;
    }
    ArrayOf<Float>::Free(pts);
    *ret = vector;
    return NOERROR;
}

/**
 * Calculates the centroid of a set of points.
 *
 * @param points the points in the form of [x1, y1, x2, y2, ..., xn, yn]
 * @return the centroid
 */
ECode CGestureUtils::ComputeCentroid(
    /* [in] */ const ArrayOf<Float>& points,
    /* [out, callee] */ ArrayOf<Float> ** ret)
{
    VALIDATE_NOT_NULL(ret);
    Float centerX = 0;
    Float centerY = 0;
    Int32 count = points.GetLength();
    for (Int32 i = 0; i < count; i++) {
        centerX += points[i];
        i++;
        centerY += points[i];
    }
    ArrayOf<Float> *center = ArrayOf<Float>::Alloc(2);
    (*center)[0] = 2 * centerX / count;
    (*center)[1] = 2 * centerY / count;

    *ret = center;
    return NOERROR;
}

/**
 * Calculates the variance-covariance matrix of a set of points.
 *
 * @param points the points in the form of [x1, y1, x2, y2, ..., xn, yn]
 * @return the variance-covariance matrix
 */
ArrayOf<ArrayOf_<Float,2> > * CGestureUtils::ComputeCoVariance(
    /* [in] */ const ArrayOf<Float>& points)
{
    //float[][] array = new float[2][2];
    //Float *arrayRow= new Float[2];
    //Float **array = new Float[2][2];

    ArrayOf<ArrayOf_<Float,2> > *array=ArrayOf<ArrayOf_<Float,2> >::Alloc(2) ;
    (*array)[0][0] = 0;
    (*array)[0][1] = 0;
    (*array)[1][0] = 0;
    (*array)[1][1] = 0;
    Int32 count = points.GetLength();
    for (Int32 i = 0; i < count; i++) {
        Float x = points[i];
        i++;
        Float y = points[i];
        (*array)[0][0] += x * x;
        (*array)[0][1] += x * y;
        (*array)[1][0] = (*array)[0][1];
        (*array)[1][1] += y * y;
    }
    (*array)[0][0] /= (count / 2);
    (*array)[0][1] /= (count / 2);
    (*array)[1][0] /= (count / 2);
    (*array)[1][1] /= (count / 2);

    return array;
}

Float CGestureUtils::ComputeTotalLength(
    /* [in] */ const ArrayOf<Float>& points)
{
    Float sum = 0;
    Int32 count = points.GetLength() - 4;
    for (Int32 i = 0; i < count; i += 2) {
        Float dx = points[i + 2] - points[i];
        Float dy = points[i + 3] - points[i + 1];
        sum += Math::Sqrt(dx * dx + dy * dy);
    }
    return sum;
}

Float CGestureUtils::ComputeStraightness(
    /* [in] */ const ArrayOf<Float>& points)
{
    Float totalLen = ComputeTotalLength(points);
    Float dx = points[2] - points[0];
    Float dy = points[3] - points[1];
    return (Float) Math::Sqrt(dx * dx + dy * dy) / totalLen;
}

Float CGestureUtils::ComputeStraightness(
    /* [in] */ const ArrayOf<Float>& points,
    /* [in] */ Float totalLen)
{
    Float dx = points[2] - points[0];
    Float dy = points[3] - points[1];
    return (Float) Math::Sqrt(dx * dx + dy * dy) / totalLen;
}

/**
 * Calculates the squared Euclidean distance between two vectors.
 *
 * @param vector1
 * @param vector2
 * @return the distance
 */
Float CGestureUtils::SquaredEuclideanDistance(
    /* [in] */ const ArrayOf<Float>& vector1,
    /* [in] */ const ArrayOf<Float>& vector2)
{
    Float squaredDistance = 0;
    Int32 size = vector1.GetLength();
    for (Int32 i = 0; i < size; i++) {
        Float difference = vector1[i] - vector2[i];
        squaredDistance += difference * difference;
    }
    return squaredDistance / size;
}

/**
 * Calculates the cosine distance between two instances.
 *
 * @param vector1
 * @param vector2
 * @return the distance between 0 and Math.PI
 */
Float CGestureUtils::CosineDistance(
    /* [in] */ const ArrayOf<Float>& vector1,
    /* [in] */ const ArrayOf<Float>& vector2)
{
    Float sum = 0;
    Int32 len = vector1.GetLength();
    for (Int32 i = 0; i < len; i++) {
        sum += vector1[i] * vector2[i];
    }
    return (Float) Math::Acos(sum);
}

/**
 * Calculates the "minimum" cosine distance between two instances.
 *
 * @param vector1
 * @param vector2
 * @param numOrientations the maximum number of orientation allowed
 * @return the distance between the two instances (between 0 and Math.PI)
 */
Float CGestureUtils::MinimumCosineDistance(
    /* [in] */ const ArrayOf<Float>& vector1,
    /* [in] */ const ArrayOf<Float>& vector2,
    /* [in] */ Int32 numOrientations)
{
    Int32 len = vector1.GetLength();
    Float a = 0;
    Float b = 0;
    for (Int32 i = 0; i < len; i += 2) {
        a += vector1[i] * vector2[i] + vector1[i + 1] * vector2[i + 1];
        b += vector1[i] * vector2[i + 1] - vector1[i + 1] * vector2[i];
    }
    if (a != 0) {
        Float tan = b/a;
        Double angle = Math::Atan(tan);
        if (numOrientations > 2 && Math::Abs(angle) >= Math::DOUBLE_PI / numOrientations) {
            return (Float) Math::Acos(a);
        } else {
            Double cosine = Math::Cos(angle);
            Double sine = cosine * tan;
            return (Float) Math::Acos(a * cosine + b * sine);
        }
    } else {
        return (Float) Math::DOUBLE_PI / 2;
    }
}

/**
 * Computes an oriented, minimum bounding box of a set of points.
 *
 * @param originalPoints
 * @return an oriented bounding box
 */
//IObjectContainer<IGesturePoint*>
IOrientedBoundingBox* CGestureUtils::ComputeOrientedBoundingBox(
    /* [in] */ IObjectContainer * originalPoints)
{
    assert(originalPoints != NULL);
    Int32 count = 0;
    originalPoints->GetObjectCount(&count);
    ArrayOf<Float>* points = ArrayOf<Float>::Alloc(count * 2);
    //for (Int32 i = 0; i < count; i++) {
    if (count > 0) {
        Int32 i = 0;
        AutoPtr<IObjectEnumerator> iter;
        originalPoints->GetObjectEnumerator((IObjectEnumerator**)&iter);
        Boolean hasNext = FALSE;
        Int32 index = 0;
        iter->MoveNext(&hasNext);
        while(hasNext) {
            IGesturePoint* point=NULL;
            iter->Current((IInterface**)&point);
        //IGesturePoint* point = originalPoints.get(i);
            index = i * 2;
            Float x;
            Float y;
            point->GetX(&x);
            point->GetY(&y);
            (*points)[index] = x;
            (*points)[index + 1] = y;
            iter->MoveNext(&hasNext);
            i++;
        }
    }
    ArrayOf<Float>* meanVector = NULL;
    ComputeCentroid(*points, &meanVector);
    IOrientedBoundingBox* ret = ComputeOrientedBoundingBox(points, *meanVector);
    ArrayOf<Float>::Free(meanVector);
    ArrayOf<Float>::Free(points);
    return ret;
}

/**
 * Computes an oriented, minimum bounding box of a set of points.
 *
 * @param originalPoints
 * @return an oriented bounding box
 */
IOrientedBoundingBox* CGestureUtils::ComputeOrientedBoundingBox(
    /* [in] */ const ArrayOf<Float>& originalPoints)
{

    ArrayOf<Float>* meanVector = NULL;
    ArrayOf<Float> *points=originalPoints.Clone();
    ComputeCentroid(*points, &meanVector);
    IOrientedBoundingBox* ret = ComputeOrientedBoundingBox(points, *meanVector);
    ArrayOf<Float>::Free(meanVector);
    ArrayOf<Float>::Free(points);
    return ret;
}

IOrientedBoundingBox* CGestureUtils::ComputeOrientedBoundingBox(
    /* [out] */ ArrayOf<Float>* points,
    /* [in] */ const ArrayOf<Float>& centroid)
{
    assert(points != NULL);
    Translate(points, -centroid[0], -centroid[1]);

    ArrayOf<ArrayOf_<Float,2> > *array = ComputeCoVariance(*points);
    ArrayOf<Float>* targetVector = ComputeOrientation(*array);

    Float angle;
    if ((*targetVector)[0] == 0 && (*targetVector)[1] == 0) {
        angle = (Float) -Math::DOUBLE_PI/2;
    } else { // -PI<alpha<PI
        angle = (Float) Math::Atan2((*targetVector)[1], (*targetVector)[0]);
        Rotate(points, -angle);
    }

    Float minx = Math::FLOAT_MAX_VALUE;
    Float miny = Math::FLOAT_MAX_VALUE;
    Float maxx = Math::FLOAT_MIN_VALUE;
    Float maxy = Math::FLOAT_MIN_VALUE;
    Int32 count = points->GetLength();
    for (Int32 i = 0; i < count; i++) {
        if ((*points)[i] < minx) {
            minx = (*points)[i];
        }
        if ((*points)[i] > maxx) {
            maxx = (*points)[i];
        }
        i++;
        if ((*points)[i] < miny) {
            miny = (*points)[i];
        }
        if ((*points)[i] > maxy) {
            maxy = (*points)[i];
        }
    }
    IOrientedBoundingBox* ret = NULL;
    COrientedBoundingBox::New((Float) (angle * 180 / Math::DOUBLE_PI),
                              centroid[0],
                              centroid[1],
                              maxx - minx,
                              maxy - miny,
                              &ret);
    ArrayOf<ArrayOf_<Float,2> >::Free(array);
    ArrayOf<Float>::Free(targetVector);
    return ret;
}

ArrayOf<Float>* CGestureUtils::ComputeOrientation(
    /* [in] */ const ArrayOf<ArrayOf_<Float,2> >& covarianceMatrix)
{
    //VALIDATE_NOT_NULL(covarianceMatrix);

    ArrayOf<Float>* targetVector = ArrayOf<Float>::Alloc(2);
    if (covarianceMatrix[0][1] == 0 || covarianceMatrix[1][0] == 0) {
        (*targetVector)[0] = 1;
        (*targetVector)[1] = 0;
    }

    Float a = -covarianceMatrix[0][0] - covarianceMatrix[1][1];
    Float b = covarianceMatrix[0][0] * covarianceMatrix[1][1] - covarianceMatrix[0][1]
            * covarianceMatrix[1][0];
    Float value = a / 2;
    Float rightside = (Float) Math::Sqrt(Math::Pow(value, 2) - b);
    Float lambda1 = -value + rightside;
    Float lambda2 = -value - rightside;
    if (lambda1 == lambda2) {
        (*targetVector)[0] = 0;
        (*targetVector)[1] = 0;
    } else {
        Float lambda = lambda1 > lambda2 ? lambda1 : lambda2;
        (*targetVector)[0] = 1;
        (*targetVector)[1] = (lambda - covarianceMatrix[0][0]) / covarianceMatrix[0][1];
    }
    return targetVector;
}


ArrayOf<Float> * CGestureUtils::Rotate(
    /* [out] */ ArrayOf<Float>* points,
    /* [in] */ Float angle)
{
    assert(points != NULL);
    Float cos = (Float) Math::Cos(angle);
    Float sin = (Float) Math::Sin(angle);
    Int32 size = points->GetLength();
    for (Int32 i = 0; i < size; i += 2) {
        Float x = (*points)[i] * cos - (*points)[i + 1] * sin;
        Float y = (*points)[i] * sin + (*points)[i + 1] * cos;
        (*points)[i] = x;
        (*points)[i + 1] = y;
    }
    return points;
}

ArrayOf<Float> * CGestureUtils::Translate(
    /* [out] */ ArrayOf<Float>* points,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(points != NULL);
    Int32 size = points->GetLength();
    for (Int32 i = 0; i < size; i += 2) {
        (*points)[i] += dx;
        (*points)[i + 1] += dy;
    }
    return points;
}

ArrayOf<Float> * CGestureUtils::Scale(
    /* [out] */ ArrayOf<Float>* points,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    assert(points != NULL);
    Int32 size = points->GetLength();
    for (Int32 i = 0; i < size; i += 2) {
        (*points)[i] *= sx;
        (*points)[i + 1] *= sy;
    }
    return points;
}

