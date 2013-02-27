
#include "graphics/CPathMeasure.h"

/**
 * Assign a new path, or null to have none.
 */
ECode CPathMeasure::SetPath(
    /* [in] */ IPath * pPath,
    /* [in] */ Boolean forceClosed)
{
    // note: the native side makes a copy of path, so we don't need a java
    // reference to it here, since it's fine if it gets GC'd
    Native_setPath(
        native_instance,
        pPath != NULL ? ((CPath*)pPath)->Ni() : 0,
        forceClosed);
    return NOERROR;
}

/**
* pResult pLength the total length of the current contour, or 0 if no path is
* associated with this measure object.
*/
ECode CPathMeasure::GetLength(
    /* [out] */ Float * pLength)
{
    *pLength = Native_getLength(native_instance);
    return NOERROR;
}

/**
* Pins distance to 0 <= distance <= getLength(), and then computes the
* corresponding position and tangent. Returns false if there is no path,
* or a zero-length path was specified, in which case position and tangent
* are unchanged.
*
* @param distance The distance along the current contour to sample
* @param pos If not null, returns the sampled position (x==[0], y==[1])
* @param tan If not null, returns the sampled tangent (x==[0], y==[1])
* @param pResult false if there was no path associated with this measure object
*/
ECode CPathMeasure::GetPosTan(
    /* [in] */ Float distance,
    /* [out, callee] */ ArrayOf<Float> ** ppPos,
    /* [out, callee] */ ArrayOf<Float> ** ppTan,
    /* [out] */ Boolean * pResult)
{
    if (((*ppPos)->GetPayload() != NULL && (*ppPos)->GetLength() < 2) ||
        ((*ppTan)->GetPayload() != NULL && (*ppTan)->GetLength() < 2)){
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *pResult = Native_getPosTan(
                native_instance,
                distance,
                ppPos,
                ppTan);

    return NOERROR;
}


/**
* Pins distance to 0 <= distance <= getLength(), and then computes the
* corresponding matrix. @param pResult false if there is no path, or a zero-length
* path was specified, in which case matrix is unchanged.
*
* @param distance The distance along the associated path
* @param matrix Allocated by the caller, this is set to the transformation
*        associated with the position and tangent at the specified distance
* @param flags Specified what aspects should be returned in the matrix.
*/
ECode CPathMeasure::GetMatrix(
    /* [in] */ Float distance,
    /* [in] */ IMatrix * pMatrix,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean * pResult)
{
    *pResult = Native_getMatrix(
                native_instance,
                distance,
                ((CMatrix*)pMatrix)->Ni(),
                flags);

    return NOERROR;
}

/**
* Given a start and stop distance, @param pResult in dst the intervening
* segment(s). If the segment is zero-length, @param pResult false, else @param pResult
* true. startD and stopD are pinned to legal values (0..getLength()).
* If startD <= stopD then @param pResult false (and leave dst untouched).
* Begin the segment with a moveTo if startWithMoveTo is true
*/
ECode CPathMeasure::GetSegment(
    /* [in] */ Float startD,
    /* [in] */ Float stopD,
    /* [in] */ IPath * pDst,
    /* [in] */ Boolean startWithMoveTo,
    /* [out] */ Boolean * pResult)
{
    *pResult = Native_getSegment(
                native_instance,
                startD,
                stopD,
                ((CPath*)pDst)->Ni(),
                startWithMoveTo);

    return NOERROR;
}

/**
* @param pResult true if the current contour is closed()
*/
ECode CPathMeasure::IsClosed(
    /* [out] */ Boolean * pResult)
{
    *pResult = Native_isClosed(native_instance);
    return NOERROR;
}

/**
* Move to the next contour in the path. @param pResult true if one exists, or
* false if we're done with the path.
*/
ECode CPathMeasure::NextContour(
    /* [out] */ Boolean * pResult)
{
    *pResult = Native_nextContour(native_instance);
    return NOERROR;
}

/**
* Create an empty PathMeasure object. To uses this to measure the length
* of a path, and/or to find the position and tangent along it, call
* setPath.
*
* Note that once a path is associated with the measure object, it is
* undefined if the path is subsequently modified and the the measure object
* is used. If the path is modified, you must call setPath with the path.
*/
ECode CPathMeasure::constructor()
{
    native_instance = Native_create(0, false);
    return NOERROR;
}

/**
* Create a PathMeasure object associated with the specified path object
* (already created and specified). The meansure object can now return the
* path's length, and the position and tangent of any position along the
* path.
*
* Note that once a path is associated with the measure object, it is
* undefined if the path is subsequently modified and the the measure object
* is used. If the path is modified, you must call setPath with the path.
*
* @param path The path that will be measured by this object
* @param forceClosed If true, then the path will be considered as "closed"
*        even if its contour was not explicitly closed.
*/
ECode CPathMeasure::constructor(
    /* [in] */ IPath * pPath,
    /* [in] */ Boolean forceClosed)
{
    native_instance = Native_create(
                        pPath != NULL ? ((CPath*)pPath)->Ni() : 0,
                        forceClosed);
    return NOERROR;
}

PathMeasurePair* CPathMeasure::Native_create(
    /* [in] */ SkPath* native_path,
    /* [in] */ Boolean forceClosed)
{
    return native_path ? new PathMeasurePair(*native_path, forceClosed) : new PathMeasurePair;
}

void CPathMeasure::Native_setPath(
    /* [in] */ PathMeasurePair* native_instance,
    /* [in] */ SkPath* native_path,
    /* [in] */ Boolean forceClosed)
{
    if (NULL == native_path) {
        native_instance->fPath.reset();
    } else {
        native_instance->fPath = *native_path;
    }
    native_instance->fMeasure.setPath(&native_instance->fPath, forceClosed);
}

Float CPathMeasure::Native_getLength(
    /* [in] */ PathMeasurePair* native_instance)
{
    return SkScalarToFloat(native_instance->fMeasure.getLength());;
}

void CPathMeasure::ConvertTwoElemFloatArray(
    /* [out] */ ArrayOf<Float> *  array,
    /* [in] */ SkScalar* src)
{
    SkASSERT(array);

    if (array->GetLength() < 2) {
        return ;
    }

    array->GetPayload()[0] = SkScalarToFloat(src[0]);
    array->GetPayload()[1] = SkScalarToFloat(src[1]);
}

Boolean CPathMeasure::Native_getPosTan(
    /* [in] */ PathMeasurePair* native_instance,
    /* [in] */ Float distance,
    /* [out] */ ArrayOf<Float> ** ppPos,
    /* [out] */ ArrayOf<Float> ** ppTan)
{
    SkScalar    tmpPos[2], tmpTan[2];
    SkScalar*   posPtr = (*ppPos)->GetPayload() ? tmpPos : NULL;
    SkScalar*   tanPtr = (*ppTan)->GetPayload() ? tmpTan : NULL;

    if (!native_instance->fMeasure.getPosTan(SkFloatToScalar(distance), (SkPoint*)posPtr,
            (SkVector*)tanPtr)) {
        return false;
    }

    if ((*ppPos)->GetPayload()) {
        ConvertTwoElemFloatArray(*ppPos, tmpPos);
    }
    if ((*ppTan)->GetPayload()) {
        ConvertTwoElemFloatArray(*ppTan, tmpTan);
    }

    return true;
}

Boolean CPathMeasure::Native_getMatrix(
    /* [in] */ PathMeasurePair* native_instance,
    /* [in] */ Float distance,
    /* [out] */ SkMatrix* native_matrix,
    /* [in] */ Int32 flags)
{
    return native_instance->fMeasure.getMatrix(SkFloatToScalar(distance), native_matrix,
        (SkPathMeasure::MatrixFlags)flags);
}

Boolean CPathMeasure::Native_getSegment(
    /* [in] */ PathMeasurePair* native_instance,
    /* [in] */ Float startF,
    /* [in] */ Float stopF,
    /* [out] */ SkPath* native_path,
    /* [in] */ Boolean startWithMoveTo)
{
    return native_instance->fMeasure.getSegment(SkFloatToScalar(startF), SkFloatToScalar(stopF),
        native_path, startWithMoveTo);
}

Boolean CPathMeasure::Native_isClosed(
    /* [in] */ PathMeasurePair* native_instance)
{
    return native_instance->fMeasure.isClosed();;
}

Boolean CPathMeasure::Native_nextContour(
    /* [in] */ PathMeasurePair* native_instance)
{
    return native_instance->fMeasure.nextContour();;
}

void CPathMeasure::Native_destroy(
    /* [in] */ PathMeasurePair* native_instance)
{
    delete native_instance;
}