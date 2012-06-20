
#include "ext/frameworkdef.h"
#include "graphics/CMatrix.h"
#include "graphics/Graphics.h"

CMatrix::~CMatrix()
{
    Finalizer(mNativeInstance);
}

ECode CMatrix::constructor()
{
    mNativeInstance = NativeCreate(0);
    return NOERROR;
}

ECode CMatrix::constructor(
    /* [in] */ IMatrix* src)
{
    SkMatrix* nSrc = src != NULL? ((CMatrix*)src)->mNativeInstance : NULL;
    mNativeInstance = NativeCreate(nSrc);
    return NOERROR;
}

/**
 * Returns true if the matrix is identity.
 * This maybe faster than testing if (getType() == 0)
 */
ECode CMatrix::IsIdentity(
    /* [out] */ Boolean* isIdentity)
{
    VALIDATE_NOT_NULL(isIdentity);
    *isIdentity = NativeIsIdentity(mNativeInstance);
    return NOERROR;
}

/* Returns true if will map a rectangle to another rectangle. This can be
 * true if the matrix is identity, scale-only, or rotates a multiple of 90
 * degrees.
 */
ECode CMatrix::RectStaysRect(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativeRectStaysRect(mNativeInstance);
    return NOERROR;
}

/**
 * (deep) copy the src matrix into this matrix. If src is null, reset this
 * matrix to the identity matrix.
 */
ECode CMatrix::Set(
    /* [in] */ IMatrix* src)
{
    if (src == NULL) {
        Reset();
    }
    else {
        NativeSet(mNativeInstance, ((CMatrix*)src)->mNativeInstance);
    }
    return NOERROR;
}

/** Returns true iff obj is a Matrix and its values equal our values.
 */
ECode CMatrix::Equals(
    /* [in] */ IMatrix* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    *isEqual = obj != NULL &&
               NativeEquals(mNativeInstance, ((CMatrix*)obj)->mNativeInstance);
    return NOERROR;
}

/** Set the matrix to identity */
ECode CMatrix::Reset()
{
    NativeReset(mNativeInstance);
    return NOERROR;
}


/** Set the matrix to translate by (dx, dy). */
ECode CMatrix::SetTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeSetTranslate(mNativeInstance, dx, dy);
    return NOERROR;
}

/**
 * Set the matrix to scale by sx and sy, with a pivot point at (px, py).
 * The pivot point is the coordinate that should remain unchanged by the
 * specified transformation.
 */
ECode CMatrix::SetScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetScale(mNativeInstance, sx, sy, px, py);
    return NOERROR;
}

/** Set the matrix to scale by sx and sy. */
ECode CMatrix::SetScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    NativeSetScale(mNativeInstance, sx, sy);
    return NOERROR;
}

/**
 * Set the matrix to rotate by the specified number of degrees, with a pivot
 * point at (px, py). The pivot point is the coordinate that should remain
 * unchanged by the specified transformation.
 */
ECode CMatrix::SetRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetRotate(mNativeInstance, degrees, px, py);
    return NOERROR;
}

/**
 * Set the matrix to rotate about (0,0) by the specified number of degrees.
 */
ECode CMatrix::SetRotateEx(
    /* [in] */ Float degrees)
{
    NativeSetRotate(mNativeInstance, degrees);
    return NOERROR;
}

/**
 * Set the matrix to rotate by the specified sine and cosine values, with a
 * pivot point at (px, py). The pivot point is the coordinate that should
 * remain unchanged by the specified transformation.
 */
ECode CMatrix::SetSinCos(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetSinCos(mNativeInstance, sinValue, cosValue, px, py);
    return NOERROR;
}

/** Set the matrix to rotate by the specified sine and cosine values. */
ECode CMatrix::SetSinCosEx(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue)
{
    NativeSetSinCos(mNativeInstance, sinValue, cosValue);
    return NOERROR;
}

/**
 * Set the matrix to skew by sx and sy, with a pivot point at (px, py).
 * The pivot point is the coordinate that should remain unchanged by the
 * specified transformation.
 */
ECode CMatrix::SetSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetSkew(mNativeInstance, kx, ky, px, py);
    return NOERROR;
}

/** Set the matrix to skew by sx and sy. */
ECode CMatrix::SetSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    NativeSetSkew(mNativeInstance, kx, ky);
    return NOERROR;
}

/**
 * Set the matrix to the concatenation of the two specified matrices,
 * returning true if the the result can be represented. Either of the two
 * matrices may also be the target matrix. this = a * b
 */
ECode CMatrix::SetConcat(
    /* [in] */ IMatrix* a,
    /* [in] */ IMatrix* b,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(a);
    VALIDATE_NOT_NULL(b);
    VALIDATE_NOT_NULL(result);
    *result = NativeSetConcat(mNativeInstance,
            ((CMatrix*)a)->mNativeInstance,
            ((CMatrix*)b)->mNativeInstance);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified translation.
 * M' = M * T(dx, dy)
 */
ECode CMatrix::PreTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreTranslate(mNativeInstance, dx, dy);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified scale.
 * M' = M * S(sx, sy, px, py)
 */
ECode CMatrix::PreScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreScale(mNativeInstance, sx, sy, px, py);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified scale.
 * M' = M * S(sx, sy)
 */
ECode CMatrix::PreScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreScale(mNativeInstance, sx, sy);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified rotation.
 * M' = M * R(degrees, px, py)
 */
ECode CMatrix::PreRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreRotate(mNativeInstance, degrees, px, py);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified rotation.
 * M' = M * R(degrees)
 */
ECode CMatrix::PreRotateEx(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreRotate(mNativeInstance, degrees);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified skew.
 * M' = M * K(kx, ky, px, py)
 */
ECode CMatrix::PreSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreSkew(mNativeInstance, kx, ky, px, py);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified skew.
 * M' = M * K(kx, ky)
 */
ECode CMatrix::PreSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreSkew(mNativeInstance, kx, ky);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified matrix.
 * M' = M * other
 */
ECode CMatrix::PreConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePreConcat(mNativeInstance, ((CMatrix*)other)->mNativeInstance);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified translation.
 * M' = T(dx, dy) * M
 */
ECode CMatrix::PostTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostTranslate(mNativeInstance, dx, dy);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified scale.
 * M' = S(sx, sy, px, py) * M
 */
ECode CMatrix::PostScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostScale(mNativeInstance, sx, sy, px, py);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified scale.
 * M' = S(sx, sy) * M
 */
ECode CMatrix::PostScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostScale(mNativeInstance, sx, sy);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified rotation.
 * M' = R(degrees, px, py) * M
 */
ECode CMatrix::PostRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostRotate(mNativeInstance, degrees, px, py);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified rotation.
 * M' = R(degrees) * M
 */
ECode CMatrix::PostRotateEx(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostRotate(mNativeInstance, degrees);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified skew.
 * M' = K(kx, ky, px, py) * M
 */
ECode CMatrix::PostSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostSkew(mNativeInstance, kx, ky, px, py);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified skew.
 * M' = K(kx, ky) * M
 */
ECode CMatrix::PostSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostSkew(mNativeInstance, kx, ky);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified matrix.
 * M' = other * M
 */
ECode CMatrix::PostConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativePostConcat(mNativeInstance, ((CMatrix*)other)->mNativeInstance);
    return NOERROR;
}

/**
 * Set the matrix to the scale and translate values that map the source
 * rectangle to the destination rectangle, returning true if the the result
 * can be represented.
 *
 * @param src the source rectangle to map from.
 * @param dst the destination rectangle to map to.
 * @param stf the ScaleToFit option
 * @return true if the matrix can be represented by the rectangle mapping.
 */
ECode CMatrix::SetRectToRect(
    /* [in] */ IRectF* src,
    /* [in] */ IRectF* dst,
    /* [in] */ MatrixScaleToFit stf,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (dst == NULL || src == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *result = NativeSetRectToRect(mNativeInstance, src, dst,
            (SkMatrix::ScaleToFit)stf);
    return NOERROR;
}

// private helper to perform range checks on arrays of "points"
ECode CMatrix::CheckPointArrays(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 pointCount)
{
    // check for too-small and too-big indices
    Int32 srcStop = srcIndex + (pointCount << 1);
    Int32 dstStop = dstIndex + (pointCount << 1);
    if ((pointCount | srcIndex | dstIndex | srcStop | dstStop) < 0 ||
            srcStop > src.GetLength() || dstStop > dst.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Set the matrix such that the specified src points would map to the
 * specified dst points. The "points" are represented as an array of floats,
 * order [x0, y0, x1, y1, ...], where each "point" is 2 float values.
 *
 * @param src   The array of src [x,y] pairs (points)
 * @param srcIndex Index of the first pair of src values
 * @param dst   The array of dst [x,y] pairs (points)
 * @param dstIndex Index of the first pair of dst values
 * @param pointCount The number of pairs/points to be used. Must be [0..4]
 * @return true if the matrix was set to the specified transformation
 */
ECode CMatrix::SetPolyToPoly(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 pointCount,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (pointCount > 4) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckPointArrays(src, srcIndex, dst, dstIndex, pointCount));
    *result = NativeSetPolyToPoly(mNativeInstance, src, srcIndex,
                                  dst, dstIndex, pointCount);
    return NOERROR;
}

/**
 * If this matrix can be inverted, return true and if inverse is not null,
 * set inverse to be the inverse of this matrix. If this matrix cannot be
 * inverted, ignore inverse and return false.
 */
ECode CMatrix::Invert(
    /* [in] */ IMatrix* inverse,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativeInvert(mNativeInstance, ((CMatrix*)inverse)->mNativeInstance);
    return NOERROR;
}

/**
* Apply this matrix to the array of 2D points specified by src, and write
 * the transformed points into the array of points specified by dst. The
 * two arrays represent their "points" as pairs of floats [x, y].
 *
 * @param dst   The array of dst points (x,y pairs)
 * @param dstIndex The index of the first [x,y] pair of dst floats
 * @param src   The array of src points (x,y pairs)
 * @param srcIndex The index of the first [x,y] pair of src floats
 * @param pointCount The number of points (x,y pairs) to transform
 */
ECode CMatrix::MapPoints(
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 pointCount)
{
    FAIL_RETURN(CheckPointArrays(src, srcIndex, dst, dstIndex, pointCount));
    NativeMapPoints(mNativeInstance, dst, dstIndex, src, srcIndex,
                    pointCount, TRUE);
    return NOERROR;
}

/**
* Apply this matrix to the array of 2D vectors specified by src, and write
 * the transformed vectors into the array of vectors specified by dst. The
 * two arrays represent their "vectors" as pairs of floats [x, y].
 *
 * @param dst   The array of dst vectors (x,y pairs)
 * @param dstIndex The index of the first [x,y] pair of dst floats
 * @param src   The array of src vectors (x,y pairs)
 * @param srcIndex The index of the first [x,y] pair of src floats
 * @param vectorCount The number of vectors (x,y pairs) to transform
 */
ECode CMatrix::MapVectors(
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 vectorCount)
{
    FAIL_RETURN(CheckPointArrays(src, srcIndex, dst, dstIndex, vectorCount));
    NativeMapPoints(mNativeInstance, dst, dstIndex, src, srcIndex,
                    vectorCount, FALSE);
    return NOERROR;
}

/**
 * Apply this matrix to the array of 2D points specified by src, and write
 * the transformed points into the array of points specified by dst. The
 * two arrays represent their "points" as pairs of floats [x, y].
 *
 * @param dst   The array of dst points (x,y pairs)
 * @param src   The array of src points (x,y pairs)
 */
ECode CMatrix::MapPointsEx(
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    if (dst.GetLength() != src.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return MapPoints(dst, 0, src, 0, dst.GetLength() >> 1);
}

/**
 * Apply this matrix to the array of 2D vectors specified by src, and write
 * the transformed vectors into the array of vectors specified by dst. The
 * two arrays represent their "vectors" as pairs of floats [x, y].
 *
 * @param dst   The array of dst vectors (x,y pairs)
 * @param src   The array of src vectors (x,y pairs)
 */
ECode CMatrix::MapVectorsEx(
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    if (dst.GetLength() != src.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return MapVectors(dst, 0, src, 0, dst.GetLength() >> 1);
}

/**
 * Apply this matrix to the array of 2D points, and write the transformed
 * points back into the array
 *
 * @param pts The array [x0, y0, x1, y1, ...] of points to transform.
 */
ECode CMatrix::MapPointsEx2(
    /* [in] */ const ArrayOf<Float>& pts)
{
    return MapPoints(pts, 0, pts, 0, pts.GetLength() >> 1);
}

/**
 * Apply this matrix to the array of 2D vectors, and write the transformed
 * vectors back into the array.
 * @param vecs The array [x0, y0, x1, y1, ...] of vectors to transform.
 */
ECode CMatrix::MapVectorsEx2(
    /* [in] */ const ArrayOf<Float>& vecs)
{
    return MapVectors(vecs, 0, vecs, 0, vecs.GetLength() >> 1);
}

/**
 * Apply this matrix to the src rectangle, and write the transformed
 * rectangle into dst. This is accomplished by transforming the 4 corners of
 * src, and then setting dst to the bounds of those points.
 *
 * @param dst Where the transformed rectangle is written.
 * @param src The original rectangle to be transformed.
 * @return the result of calling rectStaysRect()
 */
ECode CMatrix::MapRect(
    /* [in] */ IRectF* dst,
    /* [in] */ IRectF* src,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (dst == NULL || src == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *result = NativeMapRect(mNativeInstance, dst, src);
    return NOERROR;
}

/**
 * Apply this matrix to the rectangle, and write the transformed rectangle
 * back into it. This is accomplished by transforming the 4 corners of rect,
 * and then setting it to the bounds of those points
 *
 * @param rect The rectangle to transform.
 * @return the result of calling rectStaysRect()
 */
ECode CMatrix::MapRectEx(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* result)
{
    return MapRect(rect, rect, result);
}

/**
 * Return the mean radius of a circle after it has been mapped by
 * this matrix. NOTE: in perspective this value assumes the circle
 * has its center at the origin.
 */
ECode CMatrix::MapRadius(
    /* [in] */ Float radius,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativeMapRadius(mNativeInstance, radius);
    return NOERROR;
}

/** Copy 9 values from the matrix into the array.
*/
ECode CMatrix::GetValues(
    /* [in] */ const ArrayOf<Float>& values)
{
    if (values.GetLength() < 9) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeGetValues(mNativeInstance, values);
    return NOERROR;
}

/** Copy 9 values from the array into the matrix.
    Depending on the implementation of Matrix, these may be
    transformed into 16.16 integers in the Matrix, such that
    a subsequent call to getValues() will not yield exactly
    the same values.
*/
ECode CMatrix::SetValues(
    /* [in] */ const ArrayOf<Float>& values)
{
    if (values.GetLength() < 9) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeSetValues(mNativeInstance, values);
    return NOERROR;
}

SkMatrix* CMatrix::NativeCreate(
    /* [in] */ SkMatrix* nSrc)
{
    SkMatrix* obj = new SkMatrix();
    if (nSrc) {
        *obj = *nSrc;
    }
    else {
        obj->reset();
    }
    return obj;
}

Boolean CMatrix::NativeIsIdentity(
    /* [in] */ SkMatrix* nObj)
{
    return nObj->isIdentity();
}

Boolean CMatrix::NativeRectStaysRect(
    /* [in] */ SkMatrix* nObj)
{
    return nObj->rectStaysRect();
}

void CMatrix::NativeReset(
    /* [in] */ SkMatrix* nObj)
{
    nObj->reset();
}

void CMatrix::NativeSet(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ SkMatrix* nOther)
{
    *nObj = *nOther;
}

void CMatrix::NativeSetTranslate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    nObj->setTranslate(dx_, dy_);
}

void CMatrix::NativeSetScale(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    nObj->setScale(sx_, sy_, px_, py_);
}

void CMatrix::NativeSetScale(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    nObj->setScale(sx_, sy_);
}

void CMatrix::NativeSetRotate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    nObj->setRotate(degrees_, px_, py_);
}

void CMatrix::NativeSetRotate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    nObj->setRotate(degrees_);
}

void CMatrix::NativeSetSinCos(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sinValue_ = SkFloatToScalar(sinValue);
    SkScalar cosValue_ = SkFloatToScalar(cosValue);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    nObj->setSinCos(sinValue_, cosValue_, px_, py_);
}

void CMatrix::NativeSetSinCos(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue)
{
    SkScalar sinValue_ = SkFloatToScalar(sinValue);
    SkScalar cosValue_ = SkFloatToScalar(cosValue);
    nObj->setSinCos(sinValue_, cosValue_);
}

void CMatrix::NativeSetSkew(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    nObj->setSkew(kx_, ky_, px_, py_);
}

void CMatrix::NativeSetSkew(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    nObj->setSkew(kx_, ky_);
}

Boolean CMatrix::NativeSetConcat(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ SkMatrix* nA,
    /* [in] */ SkMatrix* nB)
{
    return nObj->setConcat(*nA, *nB);
}

Boolean CMatrix::NativePreTranslate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    return nObj->preTranslate(dx_, dy_);
}

Boolean CMatrix::NativePreScale(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return nObj->preScale(sx_, sy_, px_, py_);
}

Boolean CMatrix::NativePreScale(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    return nObj->preScale(sx_, sy_);
}

Boolean CMatrix::NativePreRotate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return nObj->preRotate(degrees_, px_, py_);
}

Boolean CMatrix::NativePreRotate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    return nObj->preRotate(degrees_);
}

Boolean CMatrix::NativePreSkew(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return nObj->preSkew(kx_, ky_, px_, py_);
}

Boolean CMatrix::NativePreSkew(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    return nObj->preSkew(kx_, ky_);
}

Boolean CMatrix::NativePreConcat(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ SkMatrix* nOther)
{
    return nObj->preConcat(*nOther);
}

Boolean CMatrix::NativePostTranslate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    return nObj->postTranslate(dx_, dy_);
}

Boolean CMatrix::NativePostScale(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return nObj->postScale(sx_, sy_, px_, py_);
}

Boolean CMatrix::NativePostScale(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    return nObj->postScale(sx_, sy_);
}

Boolean CMatrix::NativePostRotate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return nObj->postRotate(degrees_, px_, py_);
}

Boolean CMatrix::NativePostRotate(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    return nObj->postRotate(degrees_);
}

Boolean CMatrix::NativePostSkew(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return nObj->postSkew(kx_, ky_, px_, py_);
}

Boolean CMatrix::NativePostSkew(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    return nObj->postSkew(kx_, ky_);
}

Boolean CMatrix::NativePostConcat(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ SkMatrix* nOther)
{
    return nObj->postConcat(*nOther);
}

Boolean CMatrix::NativeSetRectToRect(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ IRectF* src,
    /* [in] */ IRectF* dst,
    /* [in] */ SkMatrix::ScaleToFit stf)
{
    SkRect src_;
    Graphics::IRectF2SkRect(src, &src_);
    SkRect dst_;
    Graphics::IRectF2SkRect(dst, &dst_);
    return nObj->setRectToRect(src_, dst_, stf);
}

Boolean CMatrix::NativeSetPolyToPoly(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 ptCount)
{
    SkASSERT(srcIndex >= 0);
    SkASSERT(dstIndex >= 0);
    SkASSERT(ptCount <= 4);

    SkASSERT(src.GetLength() >= srcIndex + (ptCount << 1));
    SkASSERT(dst.GetLength() >= dstIndex + (ptCount << 1));

    float* src_ = src.GetPayload() + srcIndex;
    float* dst_ = dst.GetPayload() + dstIndex;

#ifdef SK_SCALAR_IS_FIXED
    SkPoint srcPt[4], dstPt[4];
    for (Int32 i = 0; i < ptCount; i++) {
        Int32 x = i << 1;
        Int32 y = x + 1;
        srcPt[i].set(SkFloatToScalar(src_[x]), SkFloatToScalar(src_[y]));
        dstPt[i].set(SkFloatToScalar(dst_[x]), SkFloatToScalar(dst_[y]));
    }
    return nObj->setPolyToPoly(srcPt, dstPt, ptCount);
#else
    return nObj->setPolyToPoly((const SkPoint*)src_, (const SkPoint*)dst_,
                               ptCount);
#endif
}

Boolean CMatrix::NativeInvert(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ SkMatrix* inverse)
{
    return nObj->invert(inverse);
}

void CMatrix::NativeMapPoints(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ const ArrayOf<Float>& dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 ptCount,
    /* [in] */ Boolean isPts)
{
    SkASSERT(ptCount >= 0);
    SkASSERT(src.GetLength() >= srcIndex + (ptCount << 1));
    SkASSERT(dst.GetLength() >= dstIndex + (ptCount << 1));
    float* srcArray = src.GetPayload() + srcIndex;
    float* dstArray = dst.GetPayload() + dstIndex;

#ifdef SK_SCALAR_IS_FIXED
    // we allocate twice the count, 1 set for src, 1 for dst
    SkAutoSTMalloc<32, SkPoint> storage(ptCount * 2);
    SkPoint* pts = storage.get();
    SkPoint* srcPt = pts;
    SkPoint* dstPt = pts + ptCount;

    Int32 i;
    for (i = 0; i < ptCount; i++) {
        srcPt[i].set(SkFloatToScalar(srcArray[i << 1]),
                     SkFloatToScalar(srcArray[(i << 1) + 1]));
    }

    if (isPts) {
        nObj->mapPoints(dstPt, srcPt, ptCount);
    }
    else {
        nObj->mapVectors(dstPt, srcPt, ptCount);
    }

    for (i = 0; i < ptCount; i++) {
        dstArray[i << 1]  = SkScalarToFloat(dstPt[i].fX);
        dstArray[(i << 1) + 1]  = SkScalarToFloat(dstPt[i].fY);
    }
#else
    if (isPts) {
        nObj->mapPoints((SkPoint*)dstArray, (const SkPoint*)srcArray,
                        ptCount);
    }
    else {
        nObj->mapVectors((SkVector*)dstArray, (const SkVector*)srcArray,
                         ptCount);
    }
#endif
}

Boolean CMatrix::NativeMapRect(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ IRectF* dst,
    /* [in] */ IRectF* src)
{
    SkRect dst_, src_;
    Graphics::IRectF2SkRect(src, &src_);
    Boolean rectStaysRect = nObj->mapRect(&dst_, src_);
    Graphics::SkRect2IRectF(dst_, dst);
    return rectStaysRect;
}

Float CMatrix::NativeMapRadius(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ Float radius)
{
    return SkScalarToFloat(nObj->mapRadius(SkFloatToScalar(radius)));
}

void CMatrix::NativeGetValues(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ const ArrayOf<Float>& values)
{
    SkASSERT(values.GetLength() >= 9);
    float* dst = values.GetPayload();

#ifdef SK_SCALAR_IS_FIXED
    for (Int32 i = 0; i < 6; i++) {
        dst[i] = SkFixedToFloat(nObj->get(i));
    }
    for (Int32 j = 6; j < 9; j++) {
        dst[j] = SkFractToFloat(nObj->get(j));
    }
#else
    for (Int32 i = 0; i < 9; i++) {
        dst[i] = nObj->get(i);
    }
#endif
}

void CMatrix::NativeSetValues(
    /* [in] */ SkMatrix* nObj,
    /* [in] */ const ArrayOf<Float>& values)
{
    SkASSERT(values.GetLength() >= 9);
    const float* src = values.GetPayload();

#ifdef SK_SCALAR_IS_FIXED
    for (Int32 i = 0; i < 6; i++) {
        nObj->set(i, SkFloatToFixed(src[i]));
    }
    for (Int32 j = 6; j < 9; j++) {
        nObj->set(j, SkFloatToFract(src[j]));
    }
#else
    for (Int32 i = 0; i < 9; i++) {
        nObj->set(i, src[i]);
    }
#endif
}

Boolean CMatrix::NativeEquals(
    /* [in] */ SkMatrix* nA,
    /* [in] */ SkMatrix* nB)
{
    return *nA == *nB;
}

void CMatrix::Finalizer(
    /* [in] */ SkMatrix* nObj)
{
    delete nObj;
}

SkMatrix* CMatrix::Ni()
{
    return mNativeInstance;
}
