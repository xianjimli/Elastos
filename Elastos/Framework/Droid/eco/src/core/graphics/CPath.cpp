
#include "graphics/CPath.h"
#include "graphics/Graphics.h"
#include "graphics/CMatrix.h"
#include "ext/frameworkdef.h"

// these must be in the same order as their native values
const PathFillType CPath::sFillTypeArray[4] = {
    PathFillType_WINDING,
    PathFillType_EVEN_ODD,
    PathFillType_INVERSE_WINDING,
    PathFillType_INVERSE_EVEN_ODD
};

CPath::CPath()
    : mNativePath(NULL)
{}

CPath::~CPath()
{
    Finalizer(mNativePath);
}

/**
 * Clear any lines and curves from the path, making it empty.
 * This does NOT change the fill-type setting.
 */
ECode CPath::Reset()
{
    NativeReset(mNativePath);
    return NOERROR;
}

/**
 * Rewinds the path: clears any lines and curves from the path but
 * keeps the internal data structure for faster reuse.
 */
ECode CPath::Rewind()
{
    NativeRewind(mNativePath);
    return NOERROR;
}

/** Replace the contents of this with the contents of src.
*/
ECode CPath::Set(
    /* [in] */ IPath* src)
{
    if (this != (CPath*)src) {
        NativeSet(mNativePath, ((CPath*)src)->mNativePath);
    }
    return NOERROR;
}

/**
 * Return the path's fill type. This defines how "inside" is
 * computed. The default value is WINDING.
 *
 * @return the path's fill type
 */
ECode CPath::GetFillType(
    /* [out] */ PathFillType* type)
{
    VALIDATE_NOT_NULL(type);

    *type = sFillTypeArray[NativeGetFillType(mNativePath)];
    return NOERROR;
}

/**
 * Set the path's fill type. This defines how "inside" is computed.
 *
 * @param ft The new fill type for this path
 */
ECode CPath::SetFillType(
    /* [in] */ PathFillType ft)
{
    NativeSetFillType(mNativePath, ft);
    return NOERROR;
}

/**
 * Returns true if the filltype is one of the INVERSE variants
 *
 * @return true if the filltype is one of the INVERSE variants
 */
ECode CPath::IsInverseFillType(
    /* [out] */ Boolean* isInverseFillType)
{
    VALIDATE_NOT_NULL(isInverseFillType);

    Int32 ft = NativeGetFillType(mNativePath);
    *isInverseFillType = (ft & 2) != 0;
    return NOERROR;
}

/**
 * Toggles the INVERSE state of the filltype
 */
ECode CPath::ToggleInverseFillType()
{
    Int32 ft = NativeGetFillType(mNativePath);
    ft ^= 2;
    NativeSetFillType(mNativePath, ft);
    return NOERROR;
}

/**
 * Returns true if the path is empty (contains no lines or curves)
 *
 * @return true if the path is empty (contains no lines or curves)
 */
ECode CPath::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);

    *isEmpty = NativeIsEmpty(mNativePath);
    return NOERROR;
}

/**
 * Returns true if the path specifies a rectangle. If so, and if rect is
 * not null, set rect to the bounds of the path. If the path does not
 * specify a rectangle, return false and ignore rect.
 *
 * @param rect If not null, returns the bounds of the path if it specifies
 *             a rectangle
 * @return     true if the path specifies a rectangle
 */
ECode CPath::IsRect(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isRect)
{
    VALIDATE_NOT_NULL(isRect);

    *isRect = NativeIsRect(mNativePath, rect);
    return NOERROR;
}

/**
 * Compute the bounds of the control points of the path, and write the
 * answer into bounds. If the path contains 0 or 1 points, the bounds is
 * set to (0,0,0,0)
 *
 * @param bounds Returns the computed bounds of the path's control points.
 * @param exact This parameter is no longer used.
 */
ECode CPath::ComputeBounds(
    /* [in] */ IRectF* bounds,
    /* [in] */ Boolean exact)
{
    NativeComputeBounds(mNativePath, bounds);
    return NOERROR;
}

/**
 * Hint to the path to prepare for adding more points. This can allow the
 * path to more efficiently allocate its storage.
 *
 * @param extraPtCount The number of extra points that may be added to this
 *                     path
 */
ECode CPath::IncReserve(
    /* [in] */ Int32 extraPtCount)
{
    NativeIncReserve(mNativePath, extraPtCount);
    return NOERROR;
}

/**
 * Set the beginning of the next contour to the point (x,y).
 *
 * @param x The x-coordinate of the start of a new contour
 * @param y The y-coordinate of the start of a new contour
 */
ECode CPath::MoveTo(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    NativeMoveTo(mNativePath, x, y);
    return NOERROR;
}

/**
 * Set the beginning of the next contour relative to the last point on the
 * previous contour. If there is no previous contour, this is treated the
 * same as moveTo().
 *
 * @param dx The amount to add to the x-coordinate of the end of the
 *           previous contour, to specify the start of a new contour
 * @param dy The amount to add to the y-coordinate of the end of the
 *           previous contour, to specify the start of a new contour
 */
ECode CPath::RMoveTo(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeRMoveTo(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Add a line from the last point to the specified point (x,y).
 * If no moveTo() call has been made for this contour, the first point is
 * automatically set to (0,0).
 *
 * @param x The x-coordinate of the end of a line
 * @param y The y-coordinate of the end of a line
 */
ECode CPath::LineTo(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    NativeLineTo(mNativePath, x, y);
    return NOERROR;
}

/**
 * Same as lineTo, but the coordinates are considered relative to the last
 * point on this contour. If there is no previous point, then a moveTo(0,0)
 * is inserted automatically.
 *
 * @param dx The amount to add to the x-coordinate of the previous point on
 *           this contour, to specify a line
 * @param dy The amount to add to the y-coordinate of the previous point on
 *           this contour, to specify a line
 */
ECode CPath::RLineTo(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeRLineTo(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Add a quadratic bezier from the last point, approaching control point
 * (x1,y1), and ending at (x2,y2). If no moveTo() call has been made for
 * this contour, the first point is automatically set to (0,0).
 *
 * @param x1 The x-coordinate of the control point on a quadratic curve
 * @param y1 The y-coordinate of the control point on a quadratic curve
 * @param x2 The x-coordinate of the end point on a quadratic curve
 * @param y2 The y-coordinate of the end point on a quadratic curve
 */
ECode CPath::QuadTo(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2)
{
    NativeQuadTo(mNativePath, x1, y1, x2, y2);
    return NOERROR;
}

/**
 * Same as quadTo, but the coordinates are considered relative to the last
 * point on this contour. If there is no previous point, then a moveTo(0,0)
 * is inserted automatically.
 *
 * @param dx1 The amount to add to the x-coordinate of the last point on
 *            this contour, for the control point of a quadratic curve
 * @param dy1 The amount to add to the y-coordinate of the last point on
 *            this contour, for the control point of a quadratic curve
 * @param dx2 The amount to add to the x-coordinate of the last point on
 *            this contour, for the end point of a quadratic curve
 * @param dy2 The amount to add to the y-coordinate of the last point on
 *            this contour, for the end point of a quadratic curve
 */
ECode CPath::RQuadTo(
    /* [in] */ Float dx1,
    /* [in] */ Float dy1,
    /* [in] */ Float dx2,
    /* [in] */ Float dy2)
{
    NativeRQuadTo(mNativePath, dx1, dy1, dx2, dy2);
    return NOERROR;
}

/**
 * Add a cubic bezier from the last point, approaching control points
 * (x1,y1) and (x2,y2), and ending at (x3,y3). If no moveTo() call has been
 * made for this contour, the first point is automatically set to (0,0).
 *
 * @param x1 The x-coordinate of the 1st control point on a cubic curve
 * @param y1 The y-coordinate of the 1st control point on a cubic curve
 * @param x2 The x-coordinate of the 2nd control point on a cubic curve
 * @param y2 The y-coordinate of the 2nd control point on a cubic curve
 * @param x3 The x-coordinate of the end point on a cubic curve
 * @param y3 The y-coordinate of the end point on a cubic curve
 */
ECode CPath::CubicTo(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    NativeCubicTo(mNativePath, x1, y1, x2, y2, x3, y3);
    return NOERROR;
}

/**
 * Same as cubicTo, but the coordinates are considered relative to the
 * current point on this contour. If there is no previous point, then a
 * moveTo(0,0) is inserted automatically.
 */
ECode CPath::RCubicTo(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    NativeRCubicTo(mNativePath, x1, y1, x2, y2, x3, y3);
    return NOERROR;
}

/**
 * Append the specified arc to the path as a new contour. If the start of
 * the path is different from the path's current last point, then an
 * automatic lineTo() is added to connect the current contour to the
 * start of the arc. However, if the path is empty, then we call moveTo()
 * with the first point of the arc. The sweep angle is tread mod 360.
 *
 * @param oval        The bounds of oval defining shape and size of the arc
 * @param startAngle  Starting angle (in degrees) where the arc begins
 * @param sweepAngle  Sweep angle (in degrees) measured clockwise, treated
 *                    mod 360.
 * @param forceMoveTo If true, always begin a new contour with the arc
 */
ECode CPath::ArcTo(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean forceMoveTo)
{
    NativeArcTo(mNativePath, oval, startAngle, sweepAngle, forceMoveTo);
    return NOERROR;
}

/**
 * Append the specified arc to the path as a new contour. If the start of
 * the path is different from the path's current last point, then an
 * automatic lineTo() is added to connect the current contour to the
 * start of the arc. However, if the path is empty, then we call moveTo()
 * with the first point of the arc.
 *
 * @param oval        The bounds of oval defining shape and size of the arc
 * @param startAngle  Starting angle (in degrees) where the arc begins
 * @param sweepAngle  Sweep angle (in degrees) measured clockwise
 */
ECode CPath::ArcToEx(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    NativeArcTo(mNativePath, oval, startAngle, sweepAngle, FALSE);
    return NOERROR;
}

/**
 * Close the current contour. If the current point is not equal to the
 * first point of the contour, a line segment is automatically added.
 */
ECode CPath::Close()
{
    NativeClose(mNativePath);
    return NOERROR;
}

/**
 * Add a closed rectangle contour to the path
 *
 * @param rect The rectangle to add as a closed contour to the path
 * @param dir  The direction to wind the rectangle's contour
 */
ECode CPath::AddRect(
    /* [in] */ IRectF* rect,
    /* [in] */ PathDirection dir)
{
    if (rect == NULL) {
//        throw new NullPointerException("need rect parameter");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeAddRect(mNativePath, rect, dir);
    return NOERROR;
}

/**
 * Add a closed rectangle contour to the path
 *
 * @param left   The left side of a rectangle to add to the path
 * @param top    The top of a rectangle to add to the path
 * @param right  The right side of a rectangle to add to the path
 * @param bottom The bottom of a rectangle to add to the path
 * @param dir    The direction to wind the rectangle's contour
 */
ECode CPath::AddRectEx(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ PathDirection dir)
{
    NativeAddRect(mNativePath, left, top, right, bottom, dir);
    return NOERROR;
}

/**
 * Add a closed oval contour to the path
 *
 * @param oval The bounds of the oval to add as a closed contour to the path
 * @param dir  The direction to wind the oval's contour
 */
ECode CPath::AddOval(
    /* [in] */ IRectF* oval,
    /* [in] */ PathDirection dir)
{
    if (oval == NULL) {
//        throw new NullPointerException("need oval parameter");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeAddOval(mNativePath, oval, dir);
    return NOERROR;
}

/**
 * Add a closed circle contour to the path
 *
 * @param x   The x-coordinate of the center of a circle to add to the path
 * @param y   The y-coordinate of the center of a circle to add to the path
 * @param radius The radius of a circle to add to the path
 * @param dir    The direction to wind the circle's contour
 */
ECode CPath::AddCircle(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ PathDirection dir)
{
    NativeAddCircle(mNativePath, x, y, radius, dir);
    return NOERROR;
}

/**
 * Add the specified arc to the path as a new contour.
 *
 * @param oval The bounds of oval defining the shape and size of the arc
 * @param startAngle Starting angle (in degrees) where the arc begins
 * @param sweepAngle Sweep angle (in degrees) measured clockwise
 */
ECode CPath::AddArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    if (oval == NULL) {
//        throw new NullPointerException("need oval parameter");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeAddArc(mNativePath, oval, startAngle, sweepAngle);
    return NOERROR;
}

/**
    * Add a closed round-rectangle contour to the path
 *
 * @param rect The bounds of a round-rectangle to add to the path
 * @param rx   The x-radius of the rounded corners on the round-rectangle
 * @param ry   The y-radius of the rounded corners on the round-rectangle
 * @param dir  The direction to wind the round-rectangle's contour
 */
ECode CPath::AddRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ PathDirection dir)
{
    if (rect == NULL) {
//        throw new NullPointerException("need rect parameter");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeAddRoundRect(mNativePath, rect, rx, ry, dir);
    return NOERROR;
}

/**
 * Add a closed round-rectangle contour to the path. Each corner receives
 * two radius values [X, Y]. The corners are ordered top-left, top-right,
 * bottom-right, bottom-left
 *
 * @param rect The bounds of a round-rectangle to add to the path
 * @param radii Array of 8 values, 4 pairs of [X,Y] radii
 * @param dir  The direction to wind the round-rectangle's contour
 */
ECode CPath::AddRoundRectEx(
    /* [in] */ IRectF* rect,
    /* [in] */ const ArrayOf<Float>& radii,
    /* [in] */ PathDirection dir)
{
    if (rect == NULL) {
//        throw new NullPointerException("need rect parameter");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (radii.GetLength() < 8) {
//        throw new ArrayIndexOutOfBoundsException("radii[] needs 8 values");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeAddRoundRect(mNativePath, rect, radii, dir);
    return NOERROR;
}

/**
 * Add a copy of src to the path, offset by (dx,dy)
 *
 * @param src The path to add as a new contour
 * @param dx  The amount to translate the path in X as it is added
 */
ECode CPath::AddPath(
    /* [in] */ IPath* src,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeAddPath(mNativePath, ((CPath*)src)->mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Add a copy of src to the path
 *
 * @param src The path that is appended to the current path
 */
ECode CPath::AddPathEx(
    /* [in] */ IPath* src)
{
    NativeAddPath(mNativePath, ((CPath*)src)->mNativePath);
    return NOERROR;
}

/**
 * Add a copy of src to the path, transformed by matrix
 *
 * @param src The path to add as a new contour
 */
ECode CPath::AddPathEx2(
    /* [in] */ IPath* src,
    /* [in] */ IMatrix* matrix)
{
    NativeAddPath(mNativePath, ((CPath*)src)->mNativePath,
            ((CMatrix*)matrix)->mNativeInstance);
    return NOERROR;
}

/**
 * Offset the path by (dx,dy), returning true on success
 *
 * @param dx  The amount in the X direction to offset the entire path
 * @param dy  The amount in the Y direction to offset the entire path
 * @param dst The translated path is written here. If this is null, then
 *            the original path is modified.
 */
ECode CPath::Offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ IPath* dst)
{
    SkPath* dstNative = 0;
    if (dst != NULL) {
        dstNative = ((CPath*)dst)->mNativePath;
    }
    NativeOffset(mNativePath, dx, dy, dstNative);
    return NOERROR;
}

/**
 * Offset the path by (dx,dy), returning true on success
 *
 * @param dx The amount in the X direction to offset the entire path
 * @param dy The amount in the Y direction to offset the entire path
 */
ECode CPath::OffsetEx(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeOffset(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Sets the last point of the path.
 *
 * @param dx The new X coordinate for the last point
 * @param dy The new Y coordinate for the last point
 */
ECode CPath::SetLastPoint(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeSetLastPoint(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Transform the points in this path by matrix, and write the answer
 * into dst. If dst is null, then the the original path is modified.
 *
 * @param matrix The matrix to apply to the path
 * @param dst    The transformed path is written here. If dst is null,
 *               then the the original path is modified
 */
ECode CPath::Transform(
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPath* dst)
{
    SkPath* dstNative = 0;
    if (dst != NULL) {
        dstNative = ((CPath*)dst)->mNativePath;
    }
    NativeTransform(mNativePath, ((CMatrix*)matrix)->mNativeInstance, dstNative);
    return NOERROR;
}

/**
 * Transform the points in this path by matrix.
 *
 * @param matrix The matrix to apply to the path
 */
ECode CPath::TransformEx(
    /* [in] */ IMatrix* matrix)
{
    NativeTransform(mNativePath, ((CMatrix*)matrix)->mNativeInstance);
    return NOERROR;
}

ECode CPath::constructor()
{
    mNativePath = Init1();
    return NOERROR;
}

ECode CPath::constructor(
    /* [in] */ IPath* src)
{
    SkPath* valNative = 0;
    if (src != NULL) {
        valNative = ((CPath*)src)->mNativePath;
    }
    mNativePath = Init2(valNative);
    return NOERROR;
}

SkPath* CPath::Ni()
{
    return mNativePath;
}

SkPath* CPath::Init1()
{
    return new SkPath();
}

SkPath* CPath::Init2(
    /* [in] */ SkPath* nPath)
{
    return new SkPath(*nPath);
}

void CPath::NativeReset(
    /* [in] */ SkPath* nPath)
{
    nPath->reset();
}

void CPath::NativeRewind(
    /* [in] */ SkPath* nPath)
{
    nPath->rewind();
}

void CPath::NativeSet(
    /* [in] */ SkPath* nDst,
    /* [in] */ SkPath* nSrc)
{
    *nDst = *nSrc;
}

Int32 CPath::NativeGetFillType(
    /* [in] */ SkPath* nPath)
{
    return nPath->getFillType();
}

void CPath::NativeSetFillType(
    /* [in] */ SkPath* nPath,
    /* [in] */ PathFillType ft)
{
    nPath->setFillType((SkPath::FillType)ft);
}

Boolean CPath::NativeIsEmpty(
    /* [in] */ SkPath* nPath)
{
    return nPath->isEmpty();
}

Boolean CPath::NativeIsRect(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* rect)
{
    SkRect rect_;
    Boolean result = nPath->isRect(&rect_);
    Graphics::SkRect2IRectF(rect_, rect);
    return result;
}

void CPath::NativeComputeBounds(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* bounds)
{
    const SkRect& bounds_ = nPath->getBounds();
    Graphics::SkRect2IRectF(bounds_, bounds);
}

void CPath::NativeIncReserve(
    /* [in] */ SkPath* nPath,
    /* [in] */ Int32 extraPtCount)
{
    nPath->incReserve(extraPtCount);
}

void CPath::NativeMoveTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    SkScalar x_ = SkFloatToScalar(x);
    SkScalar y_ = SkFloatToScalar(y);
    nPath->moveTo(x_, y_);
}

void CPath::NativeRMoveTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    nPath->rMoveTo(dx_, dy_);
}

void CPath::NativeLineTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    SkScalar x_ = SkFloatToScalar(x);
    SkScalar y_ = SkFloatToScalar(y);
    nPath->lineTo(x_, y_);
}

void CPath::NativeRLineTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    nPath->rLineTo(dx_, dy_);
}

void CPath::NativeQuadTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2)
{
    SkScalar x1_ = SkFloatToScalar(x1);
    SkScalar y1_ = SkFloatToScalar(y1);
    SkScalar x2_ = SkFloatToScalar(x2);
    SkScalar y2_ = SkFloatToScalar(y2);
    nPath->quadTo(x1_, y1_, x2_, y2_);
}

void CPath::NativeRQuadTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float dx1,
    /* [in] */ Float dy1,
    /* [in] */ Float dx2,
    /* [in] */ Float dy2)
{
    SkScalar dx1_ = SkFloatToScalar(dx1);
    SkScalar dy1_ = SkFloatToScalar(dy1);
    SkScalar dx2_ = SkFloatToScalar(dx2);
    SkScalar dy2_ = SkFloatToScalar(dy2);
    nPath->rQuadTo(dx1_, dy1_, dx2_, dy2_);
}

void CPath::NativeCubicTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    SkScalar x1_ = SkFloatToScalar(x1);
    SkScalar y1_ = SkFloatToScalar(y1);
    SkScalar x2_ = SkFloatToScalar(x2);
    SkScalar y2_ = SkFloatToScalar(y2);
    SkScalar x3_ = SkFloatToScalar(x3);
    SkScalar y3_ = SkFloatToScalar(y3);
    nPath->cubicTo(x1_, y1_, x2_, y2_, x3_, y3_);
}

void CPath::NativeRCubicTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    SkScalar x1_ = SkFloatToScalar(x1);
    SkScalar y1_ = SkFloatToScalar(y1);
    SkScalar x2_ = SkFloatToScalar(x2);
    SkScalar y2_ = SkFloatToScalar(y2);
    SkScalar x3_ = SkFloatToScalar(x3);
    SkScalar y3_ = SkFloatToScalar(y3);
    nPath->rCubicTo(x1_, y1_, x2_, y2_, x3_, y3_);
}

void CPath::NativeArcTo(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean forceMoveTo)
{
    SkRect oval_;
    Graphics::IRectF2SkRect(oval, &oval_);
    SkScalar startAngle_ = SkFloatToScalar(startAngle);
    SkScalar sweepAngle_ = SkFloatToScalar(sweepAngle);
    nPath->arcTo(oval_, startAngle_, sweepAngle_, forceMoveTo);
}

void CPath::NativeClose(
    /* [in] */ SkPath* nPath)
{
    nPath->close();
}

void CPath::NativeAddRect(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* rect,
    /* [in] */ PathDirection dir)
{
    SkRect rect_;
    Graphics::IRectF2SkRect(rect, &rect_);
    nPath->addRect(rect_, (SkPath::Direction)dir);
}

void CPath::NativeAddRect(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ PathDirection dir)
{
    SkScalar left_ = SkFloatToScalar(left);
    SkScalar top_ = SkFloatToScalar(top);
    SkScalar right_ = SkFloatToScalar(right);
    SkScalar bottom_ = SkFloatToScalar(bottom);
    nPath->addRect(left_, top_, right_, bottom_, (SkPath::Direction)dir);
}

void CPath::NativeAddOval(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* oval,
    /* [in] */ PathDirection dir)
{
    SkRect oval_;
    Graphics::IRectF2SkRect(oval, &oval_);
    nPath->addOval(oval_, (SkPath::Direction)dir);
}

void CPath::NativeAddCircle(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ PathDirection dir)
{
    SkScalar x_ = SkFloatToScalar(x);
    SkScalar y_ = SkFloatToScalar(y);
    SkScalar radius_ = SkFloatToScalar(radius);
    nPath->addCircle(x_, y_, radius_, (SkPath::Direction)dir);
}

void CPath::NativeAddArc(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    SkRect oval_;
    Graphics::IRectF2SkRect(oval, &oval_);
    SkScalar startAngle_ = SkFloatToScalar(startAngle);
    SkScalar sweepAngle_ = SkFloatToScalar(sweepAngle);
    nPath->addArc(oval_, startAngle_, sweepAngle_);
}

void CPath::NativeAddRoundRect(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ PathDirection dir)
{
    SkRect rect_;
    Graphics::IRectF2SkRect(rect, &rect_);
    SkScalar rx_ = SkFloatToScalar(rx);
    SkScalar ry_ = SkFloatToScalar(ry);
    nPath->addRoundRect(rect_, rx_, ry_, (SkPath::Direction)dir);
}

void CPath::NativeAddRoundRect(
    /* [in] */ SkPath* nPath,
    /* [in] */ IRectF* rect,
    /* [in] */ const ArrayOf<Float>& radii,
    /* [in] */ PathDirection dir)
{
    SkRect rect_;
    Graphics::IRectF2SkRect(rect, &rect_);
    SkScalar dst[8];

    for (int i = 0; i < 8; i++) {
        dst[i] = SkFloatToScalar(radii[i]);
    }
    nPath->addRoundRect(rect_, dst, (SkPath::Direction)dir);
}

void CPath::NativeAddPath(
    /* [in] */ SkPath* nPath,
    /* [in] */ SkPath* src,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    nPath->addPath(*src, dx_, dy_);
}

void CPath::NativeAddPath(
    /* [in] */ SkPath* nPath,
    /* [in] */ SkPath* src)
{
    nPath->addPath(*src);
}

void CPath::NativeAddPath(
    /* [in] */ SkPath* nPath,
    /* [in] */ SkPath* src,
    /* [in] */ SkMatrix* matrix)
{
    nPath->addPath(*src, *matrix);
}

void CPath::NativeOffset(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ SkPath* dst_path)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    nPath->offset(dx_, dy_, dst_path);
}

void CPath::NativeOffset(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    nPath->offset(dx_, dy_);
}

void CPath::NativeSetLastPoint(
    /* [in] */ SkPath* nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    nPath->setLastPt(dx_, dy_);
}

void CPath::NativeTransform(
    /* [in] */ SkPath* nPath,
    /* [in] */ SkMatrix* matrix,
    /* [in] */ SkPath* dst_path)
{
    nPath->transform(*matrix, dst_path);
}

void CPath::NativeTransform(
    /* [in] */ SkPath* nPath,
    /* [in] */ SkMatrix* matrix)
{
    nPath->transform(*matrix);
}

void CPath::Finalizer(
    /* [in] */ SkPath* nPath)
{
    delete nPath;
}
