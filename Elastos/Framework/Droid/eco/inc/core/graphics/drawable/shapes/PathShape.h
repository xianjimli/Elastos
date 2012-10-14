
#ifndef __PATHSHAPE_H__
#define __PATHSHAPE_H__

#include "graphics/drawable/shapes/Shape.h"
#include <elastos/AutoPtr.h>

/**
 * Creates geometric paths, utilizing the {@link android.graphics.Path} class.
 * The path can be drawn to a Canvas with its own draw() method,
 * but more graphical control is available if you instead pass
 * the PathShape to a {@link android.graphics.drawable.ShapeDrawable}.
 */
class PathShape : public Shape
{
public:
    /**
     * PathShape constructor.
     *
     * @param path       a Path that defines the geometric paths for this shape
     * @param stdWidth   the standard width for the shape. Any changes to the
     *                   width with resize() will result in a width scaled based
     *                   on the new width divided by this width.
     * @param stdHeight  the standard height for the shape. Any changes to the
     *                   height with resize() will result in a height scaled based
     *                   on the new height divided by this height.
     */
    PathShape(
        /* [in] */ IPath* path,
        /* [in] */ Float stdWidth,
        /* [in] */ Float stdHeight);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

    // @Override
    // public PathShape clone() throws CloneNotSupportedException {
    //     PathShape shape = (PathShape) super.clone();
    //     shape.mPath = new Path(mPath);
    //     return shape;
    // }

protected:
    PathShape();

    CARAPI Init(
        /* [in] */ IPath* path,
        /* [in] */ Float stdWidth,
        /* [in] */ Float stdHeight);

    //@Override
    CARAPI_(void) OnResize(
        /* [in] */ Float width,
        /* [in] */ Float height);

    CARAPI_(void) Clone(
        /* [in] */ PathShape* other);

private:
    AutoPtr<IPath> mPath;
    Float mStdWidth;
    Float mStdHeight;

    Float mScaleX;    // cached from onResize
    Float mScaleY;    // cached from onResize
};

#endif //__PATHSHAPE_H__
