
#include "graphics/CPathMeasure.h"

/**
 * Assign a new path, or null to have none.
 */
ECode CPathMeasure::setPath(
    /* [in] */ IPath * pPath,
    /* [in] */ Boolean forceClosed)
{
    // note: the native side makes a copy of path, so we don't need a java
    // reference to it here, since it's fine if it gets GC'd
    native_setPath(native_instance, pPath != NULL ? ((CPath*)pPath)->Ni() : 0,
                    forceClosed);
    return NOERROR;
}

/**
* pResult pLength the total length of the current contour, or 0 if no path is
* associated with this measure object.
*/
ECode CPathMeasure::getLength(
    /* [out] */ Float * pLength)
{
    *pLength = native_getLength(native_instance);
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
ECode CPathMeasure::getPosTan(
    /* [in] */ Float distance,
    /* [out, callee] */ ArrayOf<Float> ** ppPos,
    /* [out, callee] */ ArrayOf<Float> ** ppTan,
    /* [out] */ Boolean * pResult)
{
    if (((*ppPos)->GetPayload() != NULL && (*ppPos)->GetLength() < 2) ||
        ((*ppTan)->GetPayload() != NULL && (*ppTan)->GetLength() < 2)){
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *pResult = native_getPosTan(native_instance, distance, ppPos, ppTan);
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
ECode CPathMeasure::getMatrix(
    /* [in] */ Float distance,
    /* [in] */ IMatrix * pMatrix,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean * pResult)
{
    *pResult = native_getMatrix(native_instance, distance, ((CMatrix*)pMatrix)->Ni(),
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
ECode CPathMeasure::getSegment(
    /* [in] */ Float startD,
    /* [in] */ Float stopD,
    /* [in] */ IPath * pDst,
    /* [in] */ Boolean startWithMoveTo,
    /* [out] */ Boolean * pResult)
{
    *pResult = native_getSegment(native_instance, startD, stopD, ((CPath*)pDst)->Ni(),
                                 startWithMoveTo);
    return NOERROR;
}

/**
* @param pResult true if the current contour is closed()
*/
ECode CPathMeasure::isClosed(
    /* [out] */ Boolean * pResult)
{
    *pResult = native_isClosed(native_instance);
    return NOERROR;
}

/**
* Move to the next contour in the path. @param pResult true if one exists, or
* false if we're done with the path.
*/
ECode CPathMeasure::nextContour(
    /* [out] */ Boolean * pResult)
{
    *pResult = native_nextContour(native_instance);
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
    native_instance = native_create(0, false);
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
    native_instance = native_create(pPath != NULL ? ((CPath*)pPath)->Ni() : 0,
                                        forceClosed);
    return NOERROR;
}

PathMeasurePair* CPathMeasure::native_create(SkPath* native_path, Boolean forceClosed)
{
    return native_path ? new PathMeasurePair(*native_path, forceClosed) : new PathMeasurePair;;
}

void CPathMeasure::native_setPath(PathMeasurePair* native_instance, SkPath* native_path, Boolean forceClosed)
{
    if (NULL == native_path) {
        native_instance->fPath.reset();
    } else {
        native_instance->fPath = *native_path;
    }
    native_instance->fMeasure.setPath(&native_instance->fPath, forceClosed);
}

Float CPathMeasure::native_getLength(PathMeasurePair* native_instance)
{
    return SkScalarToFloat(native_instance->fMeasure.getLength());;
}

void CPathMeasure::convertTwoElemFloatArray(ArrayOf<Float> *  array, SkScalar* src) 
{
	SkASSERT(array);

	if (array->GetLength() < 2) {
		return ;
	}
	
    array->GetPayload()[0] = SkScalarToFloat(src[0]);
    array->GetPayload()[1] = SkScalarToFloat(src[1]);
}

Boolean CPathMeasure::native_getPosTan(PathMeasurePair* native_instance, Float distance,
        ArrayOf<Float> ** ppPos, ArrayOf<Float> ** ppTan)
{
    SkScalar    tmpPos[2], tmpTan[2];
    SkScalar*   posPtr = (*ppPos)->GetPayload() ? tmpPos : NULL;
    SkScalar*   tanPtr = (*ppTan)->GetPayload() ? tmpTan : NULL;

    if (!native_instance->fMeasure.getPosTan(SkFloatToScalar(distance), (SkPoint*)posPtr,
            (SkVector*)tanPtr)) {
        return false;
    }

    if ((*ppPos)->GetPayload()) {
        convertTwoElemFloatArray(*ppPos, tmpPos);
    }
    if ((*ppTan)->GetPayload()) {
        convertTwoElemFloatArray(*ppTan, tmpTan);
    }

    return true;
}

Boolean CPathMeasure::native_getMatrix(PathMeasurePair* native_instance, Float distance,
    SkMatrix* native_matrix, Int32 flags)
{
    return native_instance->fMeasure.getMatrix(SkFloatToScalar(distance), native_matrix,
        (SkPathMeasure::MatrixFlags)flags);
}

Boolean CPathMeasure::native_getSegment(PathMeasurePair* native_instance, Float startF, Float stopF,
        SkPath* native_path, Boolean startWithMoveTo)
{
    return native_instance->fMeasure.getSegment(SkFloatToScalar(startF), SkFloatToScalar(stopF),
        native_path, startWithMoveTo);
}

Boolean CPathMeasure::native_isClosed(PathMeasurePair* native_instance)
{
    return native_instance->fMeasure.isClosed();;
}

Boolean CPathMeasure::native_nextContour(PathMeasurePair* native_instance)
{
    return native_instance->fMeasure.nextContour();;
}

void CPathMeasure::native_destroy(PathMeasurePair* native_instance)
{
    delete native_instance;
}