
#include "ext/frameworkdef.h"
#include "graphics/CPathMeasure.h"


CPathMeasure::~CPathMeasure()
{
    NativeDestroy(mNativeInstance);
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
    mNativeInstance = NativeCreate(0, FALSE);
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
    /* [in] */ IPath* path,
    /* [in] */ Boolean forceClosed)
{
    mNativeInstance = NativeCreate(
                        path != NULL ? ((CPath*)path)->Ni() : 0,
                        forceClosed);
    return NOERROR;
}

/**
 * Assign a new path, or null to have none.
 */
ECode CPathMeasure::SetPath(
    /* [in] */ IPath* path,
    /* [in] */ Boolean forceClosed)
{
    // note: the native side makes a copy of path, so we don't need a java
    // reference to it here, since it's fine if it gets GC'd
    NativeSetPath(
            mNativeInstance,
            path != NULL ? ((CPath*)path)->Ni() : 0,
            forceClosed);
    return NOERROR;
}

/**
* pResult pLength the total length of the current contour, or 0 if no path is
* associated with this measure object.
*/
ECode CPathMeasure::GetLength(
    /* [out] */ Float* length)
{
    VALIDATE_NOT_NULL(length);

    *length = NativeGetLength(mNativeInstance);
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
    /* [out] */ ArrayOf<Float>* pos,
    /* [out] */ ArrayOf<Float>* tan,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if ((pos != NULL && pos->GetLength() < 2) ||
        (tan != NULL && tan->GetLength() < 2)) {
        // throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    *result = NativeGetPosTan(
                mNativeInstance,
                distance,
                pos,
                tan);

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
    /* [in] */ IMatrix* matrix,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeGetMatrix(
                mNativeInstance,
                distance,
                ((CMatrix*)matrix)->Ni(),
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
    /* [in] */ IPath* dst,
    /* [in] */ Boolean startWithMoveTo,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeGetSegment(
                mNativeInstance,
                startD,
                stopD,
                ((CPath*)dst)->Ni(),
                startWithMoveTo);

    return NOERROR;
}

/**
* @param pResult true if the current contour is closed()
*/
ECode CPathMeasure::IsClosed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeIsClosed(mNativeInstance);
    return NOERROR;
}

/**
* Move to the next contour in the path. @param pResult true if one exists, or
* false if we're done with the path.
*/
ECode CPathMeasure::NextContour(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeNextContour(mNativeInstance);
    return NOERROR;
}

CPathMeasure::PathMeasurePair* CPathMeasure::NativeCreate(
    /* [in] */ SkPath* nativePath,
    /* [in] */ Boolean forceClosed)
{
    return nativePath ? new PathMeasurePair(*nativePath, forceClosed) : new PathMeasurePair;
}

void CPathMeasure::NativeSetPath(
    /* [in] */ PathMeasurePair* nativeInstance,
    /* [in] */ SkPath* nativePath,
    /* [in] */ Boolean forceClosed)
{
    if (NULL == nativePath) {
        nativeInstance->fPath.reset();
    }
    else {
        nativeInstance->fPath = *nativePath;
    }
    nativeInstance->fMeasure.setPath(&nativeInstance->fPath, forceClosed);
}

Float CPathMeasure::NativeGetLength(
    /* [in] */ PathMeasurePair* nativeInstance)
{
    return SkScalarToFloat(nativeInstance->fMeasure.getLength());;
}

void CPathMeasure::ConvertTwoElemFloatArray(
    /* [out] */ ArrayOf<Float>*  array,
    /* [in] */ SkScalar* src)
{
    assert(array != NULL);

    if (array->GetLength() < 2) {
        sk_throw();
    }

    (*array)[0] = SkScalarToFloat(src[0]);
    (*array)[1] = SkScalarToFloat(src[1]);
}

Boolean CPathMeasure::NativeGetPosTan(
    /* [in] */ PathMeasurePair* nativeInstance,
    /* [in] */ Float distance,
    /* [out] */ ArrayOf<Float>* pos,
    /* [out] */ ArrayOf<Float>* tan)
{
    SkScalar tmpPos[2], tmpTan[2];
    SkScalar* posPtr = pos ? tmpPos : NULL;
    SkScalar* tanPtr = tan ? tmpTan : NULL;

    if (!nativeInstance->fMeasure.getPosTan(SkFloatToScalar(distance), (SkPoint*)posPtr,
            (SkVector*)tanPtr)) {
        return FALSE;
    }

    if (pos) {
        ConvertTwoElemFloatArray(pos, tmpPos);
    }
    if (tan) {
        ConvertTwoElemFloatArray(tan, tmpTan);
    }
    return TRUE;
}

Boolean CPathMeasure::NativeGetMatrix(
    /* [in] */ PathMeasurePair* nativeInstance,
    /* [in] */ Float distance,
    /* [out] */ SkMatrix* nativeMatrix,
    /* [in] */ Int32 flags)
{
    return nativeInstance->fMeasure.getMatrix(SkFloatToScalar(distance), nativeMatrix,
        (SkPathMeasure::MatrixFlags)flags);
}

Boolean CPathMeasure::NativeGetSegment(
    /* [in] */ PathMeasurePair* nativeInstance,
    /* [in] */ Float startF,
    /* [in] */ Float stopF,
    /* [out] */ SkPath* nativePath,
    /* [in] */ Boolean startWithMoveTo)
{
    return nativeInstance->fMeasure.getSegment(SkFloatToScalar(startF), SkFloatToScalar(stopF),
        nativePath, startWithMoveTo);
}

Boolean CPathMeasure::NativeIsClosed(
    /* [in] */ PathMeasurePair* nativeInstance)
{
    return nativeInstance->fMeasure.isClosed();;
}

Boolean CPathMeasure::NativeNextContour(
    /* [in] */ PathMeasurePair* nativeInstance)
{
    return nativeInstance->fMeasure.nextContour();;
}

void CPathMeasure::NativeDestroy(
    /* [in] */ PathMeasurePair* nativeInstance)
{
    delete nativeInstance;
}
