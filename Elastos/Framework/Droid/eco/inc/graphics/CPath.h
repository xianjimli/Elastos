
#ifndef __CPATH_H__
#define __CPATH_H__

#include "_CPath.h"
#include <skia/core/SkPath.h>
#include <skia/core/SkMatrix.h>

CarClass(CPath)
{
    friend class Canvas;

public:
    CPath();

    ~CPath();

    /**
     * Clear any lines and curves from the path, making it empty.
     * This does NOT change the fill-type setting.
     */
    CARAPI Reset();

    /**
     * Rewinds the path: clears any lines and curves from the path but
     * keeps the internal data structure for faster reuse.
     */
    CARAPI Rewind();

    /** Replace the contents of this with the contents of src.
    */
    CARAPI Set(
        /* [in] */ IPath* src);

    /**
     * Return the path's fill type. This defines how "inside" is
     * computed. The default value is WINDING.
     *
     * @return the path's fill type
     */
    CARAPI GetFillType(
        /* [out] */ PathFillType* type);

    /**
     * Set the path's fill type. This defines how "inside" is computed.
     *
     * @param ft The new fill type for this path
     */
    CARAPI SetFillType(
        /* [in] */ PathFillType ft);

    /**
     * Returns true if the filltype is one of the INVERSE variants
     *
     * @return true if the filltype is one of the INVERSE variants
     */
    CARAPI IsInverseFillType(
        /* [out] */ Boolean* isInverseFillType);

    /**
     * Toggles the INVERSE state of the filltype
     */
    CARAPI ToggleInverseFillType();

    /**
     * Returns true if the path is empty (contains no lines or curves)
     *
     * @return true if the path is empty (contains no lines or curves)
     */
    CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty);

    /**
     * Returns true if the path specifies a rectangle. If so, and if rect is
     * not null, set rect to the bounds of the path. If the path does not
     * specify a rectangle, return false and ignore rect.
     *
     * @param rect If not null, returns the bounds of the path if it specifies
     *             a rectangle
     * @return     true if the path specifies a rectangle
     */
    CARAPI IsRect(
        /* [in] */ IRectF* rect,
        /* [out] */ Boolean* isRect);

    /**
     * Compute the bounds of the control points of the path, and write the
     * answer into bounds. If the path contains 0 or 1 points, the bounds is
     * set to (0,0,0,0)
     *
     * @param bounds Returns the computed bounds of the path's control points.
     * @param exact This parameter is no longer used.
     */
    CARAPI ComputeBounds(
        /* [in] */ IRectF* bounds,
        /* [in] */ Boolean exact);

    /**
     * Hint to the path to prepare for adding more points. This can allow the
     * path to more efficiently allocate its storage.
     *
     * @param extraPtCount The number of extra points that may be added to this
     *                     path
     */
    CARAPI IncReserve(
        /* [in] */ Int32 extraPtCount);

    /**
     * Set the beginning of the next contour to the point (x,y).
     *
     * @param x The x-coordinate of the start of a new contour
     * @param y The y-coordinate of the start of a new contour
     */
    CARAPI MoveTo(
        /* [in] */ Float x,
        /* [in] */ Float y);

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
    CARAPI RMoveTo(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    /**
     * Add a line from the last point to the specified point (x,y).
     * If no moveTo() call has been made for this contour, the first point is
     * automatically set to (0,0).
     *
     * @param x The x-coordinate of the end of a line
     * @param y The y-coordinate of the end of a line
     */
    CARAPI LineTo(
        /* [in] */ Float x,
        /* [in] */ Float y);

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
    CARAPI RLineTo(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

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
    CARAPI QuadTo(
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Float x2,
        /* [in] */ Float y2);

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
    CARAPI RQuadTo(
        /* [in] */ Float dx1,
        /* [in] */ Float dy1,
        /* [in] */ Float dx2,
        /* [in] */ Float dy2);

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
    CARAPI CubicTo(
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Float x2,
        /* [in] */ Float y2,
        /* [in] */ Float x3,
        /* [in] */ Float y3);

    /**
     * Same as cubicTo, but the coordinates are considered relative to the
     * current point on this contour. If there is no previous point, then a
     * moveTo(0,0) is inserted automatically.
     */
    CARAPI RCubicTo(
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Float x2,
        /* [in] */ Float y2,
        /* [in] */ Float x3,
        /* [in] */ Float y3);

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
    CARAPI ArcTo(
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle,
        /* [in] */ Boolean forceMoveTo);

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
    CARAPI ArcToEx(
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle);

    /**
     * Close the current contour. If the current point is not equal to the
     * first point of the contour, a line segment is automatically added.
     */
    CARAPI Close();

    /**
     * Add a closed rectangle contour to the path
     *
     * @param rect The rectangle to add as a closed contour to the path
     * @param dir  The direction to wind the rectangle's contour
     */
    CARAPI AddRect(
        /* [in] */ IRectF* rect,
        /* [in] */ PathDirection dir);

    /**
     * Add a closed rectangle contour to the path
     *
     * @param left   The left side of a rectangle to add to the path
     * @param top    The top of a rectangle to add to the path
     * @param right  The right side of a rectangle to add to the path
     * @param bottom The bottom of a rectangle to add to the path
     * @param dir    The direction to wind the rectangle's contour
     */
    CARAPI AddRectEx(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ PathDirection dir);

    /**
     * Add a closed oval contour to the path
     *
     * @param oval The bounds of the oval to add as a closed contour to the path
     * @param dir  The direction to wind the oval's contour
     */
    CARAPI AddOval(
        /* [in] */ IRectF* oval,
        /* [in] */ PathDirection dir);

    /**
     * Add a closed circle contour to the path
     *
     * @param x   The x-coordinate of the center of a circle to add to the path
     * @param y   The y-coordinate of the center of a circle to add to the path
     * @param radius The radius of a circle to add to the path
     * @param dir    The direction to wind the circle's contour
     */
    CARAPI AddCircle(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float radius,
        /* [in] */ PathDirection dir);

    /**
     * Add the specified arc to the path as a new contour.
     *
     * @param oval The bounds of oval defining the shape and size of the arc
     * @param startAngle Starting angle (in degrees) where the arc begins
     * @param sweepAngle Sweep angle (in degrees) measured clockwise
     */
    CARAPI AddArc(
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle);

    /**
        * Add a closed round-rectangle contour to the path
     *
     * @param rect The bounds of a round-rectangle to add to the path
     * @param rx   The x-radius of the rounded corners on the round-rectangle
     * @param ry   The y-radius of the rounded corners on the round-rectangle
     * @param dir  The direction to wind the round-rectangle's contour
     */
    CARAPI AddRoundRect(
        /* [in] */ IRectF* rect,
        /* [in] */ Float rx,
        /* [in] */ Float ry,
        /* [in] */ PathDirection dir);

    /**
     * Add a closed round-rectangle contour to the path. Each corner receives
     * two radius values [X, Y]. The corners are ordered top-left, top-right,
     * bottom-right, bottom-left
     *
     * @param rect The bounds of a round-rectangle to add to the path
     * @param radii Array of 8 values, 4 pairs of [X,Y] radii
     * @param dir  The direction to wind the round-rectangle's contour
     */
    CARAPI AddRoundRectEx(
        /* [in] */ IRectF* rect,
        /* [in] */ const ArrayOf<Float>& radii,
        /* [in] */ PathDirection dir);

    /**
     * Add a copy of src to the path, offset by (dx,dy)
     *
     * @param src The path to add as a new contour
     * @param dx  The amount to translate the path in X as it is added
     */
    CARAPI AddPath(
        /* [in] */ IPath* src,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    /**
     * Add a copy of src to the path
     *
     * @param src The path that is appended to the current path
     */
    CARAPI AddPathEx(
        /* [in] */ IPath* src);

    /**
     * Add a copy of src to the path, transformed by matrix
     *
     * @param src The path to add as a new contour
     */
    CARAPI AddPathEx2(
        /* [in] */ IPath* src,
        /* [in] */ IMatrix* matrix);

    /**
     * Offset the path by (dx,dy), returning true on success
     *
     * @param dx  The amount in the X direction to offset the entire path
     * @param dy  The amount in the Y direction to offset the entire path
     * @param dst The translated path is written here. If this is null, then
     *            the original path is modified.
     */
    CARAPI Offset(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ IPath* dst);

    /**
     * Offset the path by (dx,dy), returning true on success
     *
     * @param dx The amount in the X direction to offset the entire path
     * @param dy The amount in the Y direction to offset the entire path
     */
    CARAPI OffsetEx(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    /**
     * Sets the last point of the path.
     *
     * @param dx The new X coordinate for the last point
     * @param dy The new Y coordinate for the last point
     */
    CARAPI SetLastPoint(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    /**
     * Transform the points in this path by matrix, and write the answer
     * into dst. If dst is null, then the the original path is modified.
     *
     * @param matrix The matrix to apply to the path
     * @param dst    The transformed path is written here. If dst is null,
     *               then the the original path is modified
     */
    CARAPI Transform(
        /* [in] */ IMatrix* matrix,
        /* [in] */ IPath* dst);

    /**
     * Transform the points in this path by matrix.
     *
     * @param matrix The matrix to apply to the path
     */
    CARAPI TransformEx(
        /* [in] */ IMatrix* matrix);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPath* src);

public:
    /*package*/ CARAPI_(SkPath*) Ni();

private:
    static CARAPI_(SkPath*) Init1();

    static CARAPI_(SkPath*) Init2(
        /* [in] */ SkPath* nPath);

    static CARAPI_(void) NativeReset(
        /* [in] */ SkPath* nPath);

    static CARAPI_(void) NativeRewind(
        /* [in] */ SkPath* nPath);

    static CARAPI_(void) NativeSet(
        /* [in] */ SkPath* nDst,
        /* [in] */ SkPath* nSrc);

    static CARAPI_(Int32) NativeGetFillType(
        /* [in] */ SkPath* nPath);

    static CARAPI_(void) NativeSetFillType(
        /* [in] */ SkPath* nPath,
        /* [in] */ PathFillType ft);

    static CARAPI_(Boolean) NativeIsEmpty(
        /* [in] */ SkPath* nPath);

    static CARAPI_(Boolean) NativeIsRect(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* rect);

    static CARAPI_(void) NativeComputeBounds(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* bounds);

    static CARAPI_(void) NativeIncReserve(
        /* [in] */ SkPath* nPath,
        /* [in] */ Int32 extraPtCount);

    static CARAPI_(void) NativeMoveTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static CARAPI_(void) NativeRMoveTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(void) NativeLineTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static CARAPI_(void) NativeRLineTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(void) NativeQuadTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Float x2,
        /* [in] */ Float y2);

    static CARAPI_(void) NativeRQuadTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float dx1,
        /* [in] */ Float dy1,
        /* [in] */ Float dx2,
        /* [in] */ Float dy2);

    static CARAPI_(void) NativeCubicTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Float x2,
        /* [in] */ Float y2,
        /* [in] */ Float x3,
        /* [in] */ Float y3);

    static CARAPI_(void) NativeRCubicTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Float x2,
        /* [in] */ Float y2,
        /* [in] */ Float x3,
        /* [in] */ Float y3);

    static CARAPI_(void) NativeArcTo(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle,
        /* [in] */ Boolean forceMoveTo);

    static CARAPI_(void) NativeClose(
        /* [in] */ SkPath* nPath);

    static CARAPI_(void) NativeAddRect(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* rect,
        /* [in] */ PathDirection dir);

    static CARAPI_(void) NativeAddRect(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ PathDirection dir);

    static CARAPI_(void) NativeAddOval(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* oval,
        /* [in] */ PathDirection dir);

    static CARAPI_(void) NativeAddCircle(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float radius,
        /* [in] */ PathDirection dir);

    static CARAPI_(void) NativeAddArc(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle);

    static CARAPI_(void) NativeAddRoundRect(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* rect,
        /* [in] */ Float rx,
        /* [in] */ Float ry,
        /* [in] */ PathDirection dir);

    static CARAPI_(void) NativeAddRoundRect(
        /* [in] */ SkPath* nPath,
        /* [in] */ IRectF* rect,
        /* [in] */ const ArrayOf<Float>& radii,
        /* [in] */ PathDirection dir);

    static CARAPI_(void) NativeAddPath(
        /* [in] */ SkPath* nPath,
        /* [in] */ SkPath* src,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(void) NativeAddPath(
        /* [in] */ SkPath* nPath,
        /* [in] */ SkPath* src);

    static CARAPI_(void) NativeAddPath(
        /* [in] */ SkPath* nPath,
        /* [in] */ SkPath* src,
        /* [in] */ SkMatrix* matrix);

    static CARAPI_(void) NativeOffset(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ SkPath* dst_path);

    static CARAPI_(void) NativeOffset(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(void) NativeSetLastPoint(
        /* [in] */ SkPath* nPath,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(void) NativeTransform(
        /* [in] */ SkPath* nPath,
        /* [in] */ SkMatrix* matrix,
        /* [in] */ SkPath* dst_path);

    static CARAPI_(void) NativeTransform(
        /* [in] */ SkPath* nPath,
        /* [in] */ SkMatrix* matrix);

    static CARAPI_(void) Finalizer(
        /* [in] */ SkPath* nPath);

private:
    // these must be in the same order as their native values
    static const PathFillType sFillTypeArray[4];

    SkPath* mNativePath;
};

#endif //__CPATH_H__
