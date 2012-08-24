
#ifndef __CMATRIX_H__
#define __CMATRIX_H__

#include "_CMatrix.h"
#include <SkMatrix.h>

class Canvas;

CarClass(CMatrix)
{
    friend class Canvas;
    friend class Shader;
    friend class CPath;

public:
    ~CMatrix();

    CARAPI constructor();
    CARAPI constructor(
        /* [in] */ IMatrix* src);

    /**
     * Returns true if the matrix is identity.
     * This maybe faster than testing if (getType() == 0)
     */
    CARAPI IsIdentity(
        /* [out] */ Boolean* isIdentity);

    /* Returns true if will map a rectangle to another rectangle. This can be
     * true if the matrix is identity, scale-only, or rotates a multiple of 90
     * degrees.
     */
    CARAPI RectStaysRect(
        /* [out] */ Boolean* result);

    /**
     * (deep) copy the src matrix into this matrix. If src is null, reset this
     * matrix to the identity matrix.
     */
    CARAPI Set(
        /* [in] */ IMatrix* src);

    /** Returns true iff obj is a Matrix and its values equal our values.
     */
    CARAPI Equals(
        /* [in] */ IMatrix* obj,
        /* [out] */ Boolean* isEqual);

    /** Set the matrix to identity */
    CARAPI Reset();

    /** Set the matrix to translate by (dx, dy). */
    CARAPI SetTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    /**
     * Set the matrix to scale by sx and sy, with a pivot point at (px, py).
     * The pivot point is the coordinate that should remain unchanged by the
     * specified transformation.
     */
    CARAPI SetScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /** Set the matrix to scale by sx and sy. */
    CARAPI SetScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    /**
     * Set the matrix to rotate by the specified number of degrees, with a pivot
     * point at (px, py). The pivot point is the coordinate that should remain
     * unchanged by the specified transformation.
     */
    CARAPI SetRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /**
     * Set the matrix to rotate about (0,0) by the specified number of degrees.
     */
    CARAPI SetRotateEx(
        /* [in] */ Float degrees);

    /**
     * Set the matrix to rotate by the specified sine and cosine values, with a
     * pivot point at (px, py). The pivot point is the coordinate that should
     * remain unchanged by the specified transformation.
     */
    CARAPI SetSinCos(
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /** Set the matrix to rotate by the specified sine and cosine values. */
    CARAPI SetSinCosEx(
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue);

    /**
     * Set the matrix to skew by sx and sy, with a pivot point at (px, py).
     * The pivot point is the coordinate that should remain unchanged by the
     * specified transformation.
     */
    CARAPI SetSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /** Set the matrix to skew by sx and sy. */
    CARAPI SetSkewEx(
        /* [in] */ Float kx,
        /* [in] */ Float ky);

    /**
     * Set the matrix to the concatenation of the two specified matrices,
     * returning true if the the result can be represented. Either of the two
     * matrices may also be the target matrix. this = a * b
     */
    CARAPI SetConcat(
        /* [in] */ IMatrix* a,
        /* [in] */ IMatrix* b,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified translation.
     * M' = M * T(dx, dy)
     */
    CARAPI PreTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified scale.
     * M' = M * S(sx, sy, px, py)
     */
    CARAPI PreScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified scale.
     * M' = M * S(sx, sy)
     */
    CARAPI PreScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified rotation.
     * M' = M * R(degrees, px, py)
     */
    CARAPI PreRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified rotation.
     * M' = M * R(degrees)
     */
    CARAPI PreRotateEx(
        /* [in] */ Float degrees,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified skew.
     * M' = M * K(kx, ky, px, py)
     */
    CARAPI PreSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified skew.
     * M' = M * K(kx, ky)
     */
    CARAPI PreSkewEx(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified matrix.
     * M' = M * other
     */
    CARAPI PreConcat(
        /* [in] */ IMatrix* other,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified translation.
     * M' = T(dx, dy) * M
     */
    CARAPI PostTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified scale.
     * M' = S(sx, sy, px, py) * M
     */
    CARAPI PostScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified scale.
     * M' = S(sx, sy) * M
     */
    CARAPI PostScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified rotation.
     * M' = R(degrees, px, py) * M
     */
    CARAPI PostRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified rotation.
     * M' = R(degrees) * M
     */
    CARAPI PostRotateEx(
        /* [in] */ Float degrees,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified skew.
     * M' = K(kx, ky, px, py) * M
     */
    CARAPI PostSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified skew.
     * M' = K(kx, ky) * M
     */
    CARAPI PostSkewEx(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified matrix.
     * M' = other * M
     */
    CARAPI PostConcat(
        /* [in] */ IMatrix* other,
        /* [out] */ Boolean* result);

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
    CARAPI SetRectToRect(
        /* [in] */ IRectF* src,
        /* [in] */ IRectF* dst,
        /* [in] */ MatrixScaleToFit stf,
        /* [out] */ Boolean* result);

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
    CARAPI SetPolyToPoly(
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ Int32 pointCount,
        /* [out] */ Boolean* result);

    /**
     * If this matrix can be inverted, return true and if inverse is not null,
     * set inverse to be the inverse of this matrix. If this matrix cannot be
     * inverted, ignore inverse and return false.
     */
    CARAPI Invert(
        /* [in] */ IMatrix* inverse,
        /* [out] */ Boolean* result);

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
    CARAPI MapPoints(
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ Int32 pointCount);

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
    CARAPI MapVectors(
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ Int32 vectorCount);

    /**
     * Apply this matrix to the array of 2D points specified by src, and write
     * the transformed points into the array of points specified by dst. The
     * two arrays represent their "points" as pairs of floats [x, y].
     *
     * @param dst   The array of dst points (x,y pairs)
     * @param src   The array of src points (x,y pairs)
     */
    CARAPI MapPointsEx(
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ const ArrayOf<Float>& src);

    /**
     * Apply this matrix to the array of 2D vectors specified by src, and write
     * the transformed vectors into the array of vectors specified by dst. The
     * two arrays represent their "vectors" as pairs of floats [x, y].
     *
     * @param dst   The array of dst vectors (x,y pairs)
     * @param src   The array of src vectors (x,y pairs)
     */
    CARAPI MapVectorsEx(
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ const ArrayOf<Float>& src);

    /**
     * Apply this matrix to the array of 2D points, and write the transformed
     * points back into the array
     *
     * @param pts The array [x0, y0, x1, y1, ...] of points to transform.
     */
    CARAPI MapPointsEx2(
        /* [in] */ const ArrayOf<Float>& pts);

    /**
     * Apply this matrix to the array of 2D vectors, and write the transformed
     * vectors back into the array.
     * @param vecs The array [x0, y0, x1, y1, ...] of vectors to transform.
     */
    CARAPI MapVectorsEx2(
        /* [in] */ const ArrayOf<Float>& vecs);

    /**
     * Apply this matrix to the src rectangle, and write the transformed
     * rectangle into dst. This is accomplished by transforming the 4 corners of
     * src, and then setting dst to the bounds of those points.
     *
     * @param dst Where the transformed rectangle is written.
     * @param src The original rectangle to be transformed.
     * @return the result of calling rectStaysRect()
     */
    CARAPI MapRect(
        /* [in] */ IRectF* dst,
        /* [in] */ IRectF* src,
        /* [out] */ Boolean* result);

    /**
     * Apply this matrix to the rectangle, and write the transformed rectangle
     * back into it. This is accomplished by transforming the 4 corners of rect,
     * and then setting it to the bounds of those points
     *
     * @param rect The rectangle to transform.
     * @return the result of calling rectStaysRect()
     */
    CARAPI MapRectEx(
        /* [in] */ IRectF* rect,
        /* [out] */ Boolean* result);

    /**
     * Return the mean radius of a circle after it has been mapped by
     * this matrix. NOTE: in perspective this value assumes the circle
     * has its center at the origin.
     */
    CARAPI MapRadius(
        /* [in] */ Float radius,
        /* [out] */ Float* result);

    /** Copy 9 values from the matrix into the array.
    */
    CARAPI GetValues(
        /* [in] */ const ArrayOf<Float>& values);

    /** Copy 9 values from the array into the matrix.
        Depending on the implementation of Matrix, these may be
        transformed into 16.16 integers in the Matrix, such that
        a subsequent call to getValues() will not yield exactly
        the same values.
    */
    CARAPI SetValues(
        /* [in] */ const ArrayOf<Float>& values);

public:
    /*package*/ CARAPI_(SkMatrix*) Ni();

private:
    // private helper to perform range checks on arrays of "points"
    static CARAPI CheckPointArrays(
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ Int32 pointCount);

    static CARAPI_(SkMatrix*) NativeCreate(
        /* [in] */ SkMatrix* nSrc);

    static CARAPI_(Boolean) NativeIsIdentity(
        /* [in] */ SkMatrix* nObj);

    static CARAPI_(Boolean) NativeRectStaysRect(
        /* [in] */ SkMatrix* nObj);

    static CARAPI_(void) NativeReset(
        /* [in] */ SkMatrix* nObj);

    static CARAPI_(void) NativeSet(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ SkMatrix* nOther);

    static CARAPI_(void) NativeSetTranslate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(void) NativeSetScale(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(void) NativeSetScale(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    static CARAPI_(void) NativeSetRotate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(void) NativeSetRotate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float degrees);

    static CARAPI_(void) NativeSetSinCos(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(void) NativeSetSinCos(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue);

    static CARAPI_(void) NativeSetSkew(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(void) NativeSetSkew(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float kx,
        /* [in] */ Float ky);

    static CARAPI_(Boolean) NativeSetConcat(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ SkMatrix* nA,
        /* [in] */ SkMatrix* nB);

    static CARAPI_(Boolean) NativePreTranslate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(Boolean) NativePreScale(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(Boolean) NativePreScale(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    static CARAPI_(Boolean) NativePreRotate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(Boolean) NativePreRotate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float degrees);

    static CARAPI_(Boolean) NativePreSkew(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(Boolean) NativePreSkew(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float kx,
        /* [in] */ Float ky);

    static CARAPI_(Boolean) NativePreConcat(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ SkMatrix* nOther);

    static CARAPI_(Boolean) NativePostTranslate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(Boolean) NativePostScale(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(Boolean) NativePostScale(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    static CARAPI_(Boolean) NativePostRotate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(Boolean) NativePostRotate(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float degrees);

    static CARAPI_(Boolean) NativePostSkew(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py);

    static CARAPI_(Boolean) NativePostSkew(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float kx,
        /* [in] */ Float ky);

    static CARAPI_(Boolean) NativePostConcat(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ SkMatrix* nOther);

    static CARAPI_(Boolean) NativeSetRectToRect(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ IRectF* src,
        /* [in] */ IRectF* dst,
        /* [in] */ SkMatrix::ScaleToFit stf);

    static CARAPI_(Boolean) NativeSetPolyToPoly(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ Int32 ptCount);

    static CARAPI_(Boolean) NativeInvert(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ SkMatrix* inverse);

    static CARAPI_(void) NativeMapPoints(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ Int32 ptCount,
        /* [in] */ Boolean isPts);

    static CARAPI_(Boolean) NativeMapRect(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ IRectF* dst,
        /* [in] */ IRectF* src);

    static CARAPI_(Float) NativeMapRadius(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ Float radius);

    static CARAPI_(void) NativeGetValues(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ const ArrayOf<Float>& values);

    static CARAPI_(void) NativeSetValues(
        /* [in] */ SkMatrix* nObj,
        /* [in] */ const ArrayOf<Float>& values);

    static CARAPI_(Boolean) NativeEquals(
        /* [in] */ SkMatrix* nA,
        /* [in] */ SkMatrix* nB);

    static CARAPI_(void) Finalizer(
        /* [in] */ SkMatrix* nObj);

private:
    SkMatrix* mNativeInstance;
};

#endif //__CMATRIX_H__
