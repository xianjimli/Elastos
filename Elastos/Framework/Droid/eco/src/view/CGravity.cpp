
#include "view/CGravity.h"
#include "graphics/CRect.h"

/**
  * Apply a gravity constant to an object.
  *
  * @param gravity The desired placement of the object, as defined by the
  *                constants in this class.
  * @param w The horizontal size of the object.
  * @param h The vertical size of the object.
  * @param container The frame of the containing space, in which the object
  *                  will be placed.  Should be large enough to contain the
  *                  width and height of the object.
  * @param outRect Receives the computed frame of the object in its
  *                container.
  */
ECode CGravity::Apply(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* container,
    /* [in] */ IRect* outRect)
{
    return ApplyEx(gravity, w, h, container, 0, 0, outRect);
}

/**
  * Apply a gravity constant to an object.
  *
  * @param gravity The desired placement of the object, as defined by the
  *                constants in this class.
  * @param w The horizontal size of the object.
  * @param h The vertical size of the object.
  * @param container The frame of the containing space, in which the object
  *                  will be placed.  Should be large enough to contain the
  *                  width and height of the object.
  * @param xAdj Offset to apply to the X axis.  If gravity is LEFT this
  *             pushes it to the right; if gravity is RIGHT it pushes it to
  *             the left; if gravity is CENTER_HORIZONTAL it pushes it to the
  *             right or left; otherwise it is ignored.
  * @param yAdj Offset to apply to the Y axis.  If gravity is TOP this pushes
  *             it down; if gravity is BOTTOM it pushes it up; if gravity is
  *             CENTER_VERTICAL it pushes it down or up; otherwise it is
  *             ignored.
  * @param outRect Receives the computed frame of the object in its
  *                container.
  */
ECode CGravity::ApplyEx(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* _container,
    /* [in] */ Int32 xAdj,
    /* [in] */ Int32 yAdj,
    /* [in] */ IRect* _outRect)
{
    CRect* container = (CRect*)_container;
    CRect* outRect = (CRect*)_outRect;

    switch (gravity & ((Gravity_AXIS_PULL_BEFORE | Gravity_AXIS_PULL_AFTER)
            << Gravity_AXIS_X_SHIFT)) {
        case 0:
            outRect->mLeft = container->mLeft
                    + ((container->mRight - container->mLeft - w) / 2) + xAdj;
            outRect->mRight = outRect->mLeft + w;
            if ((gravity & (Gravity_AXIS_CLIP << Gravity_AXIS_X_SHIFT))
                    == (Gravity_AXIS_CLIP << Gravity_AXIS_X_SHIFT)) {
                if (outRect->mLeft < container->mLeft) {
                    outRect->mLeft = container->mLeft;
                }
                if (outRect->mRight > container->mRight) {
                    outRect->mRight = container->mRight;
                }
            }
            break;
        case Gravity_AXIS_PULL_BEFORE << Gravity_AXIS_X_SHIFT:
            outRect->mLeft = container->mLeft + xAdj;
            outRect->mRight = outRect->mLeft + w;
            if ((gravity & (Gravity_AXIS_CLIP << Gravity_AXIS_X_SHIFT))
                    == (Gravity_AXIS_CLIP << Gravity_AXIS_X_SHIFT)) {
                if (outRect->mRight > container->mRight) {
                    outRect->mRight = container->mRight;
                }
            }
            break;
        case Gravity_AXIS_PULL_AFTER << Gravity_AXIS_X_SHIFT:
            outRect->mRight = container->mRight - xAdj;
            outRect->mLeft = outRect->mRight - w;
            if ((gravity & (Gravity_AXIS_CLIP << Gravity_AXIS_X_SHIFT))
                    == (Gravity_AXIS_CLIP << Gravity_AXIS_X_SHIFT)) {
                if (outRect->mLeft < container->mLeft) {
                    outRect->mLeft = container->mLeft;
                }
            }
            break;
        default:
            outRect->mLeft = container->mLeft + xAdj;
            outRect->mRight = container->mRight + xAdj;
            break;
    }

    switch (gravity & ((Gravity_AXIS_PULL_BEFORE | Gravity_AXIS_PULL_AFTER)
            << Gravity_AXIS_Y_SHIFT)) {
        case 0:
            outRect->mTop = container->mTop
                    + ((container->mBottom - container->mTop - h)/2) + yAdj;
            outRect->mBottom = outRect->mTop + h;
            if ((gravity & (Gravity_AXIS_CLIP << Gravity_AXIS_Y_SHIFT))
                    == (Gravity_AXIS_CLIP << Gravity_AXIS_Y_SHIFT)) {
                if (outRect->mTop < container->mTop) {
                    outRect->mTop = container->mTop;
                }
                if (outRect->mBottom > container->mBottom) {
                    outRect->mBottom = container->mBottom;
                }
            }
            break;
        case Gravity_AXIS_PULL_BEFORE << Gravity_AXIS_Y_SHIFT:
            outRect->mTop = container->mTop + yAdj;
            outRect->mBottom = outRect->mTop + h;
            if ((gravity & (Gravity_AXIS_CLIP << Gravity_AXIS_Y_SHIFT))
                    == (Gravity_AXIS_CLIP << Gravity_AXIS_Y_SHIFT)) {
                if (outRect->mBottom > container->mBottom) {
                    outRect->mBottom = container->mBottom;
                }
            }
            break;
        case Gravity_AXIS_PULL_AFTER << Gravity_AXIS_Y_SHIFT:
            outRect->mBottom = container->mBottom - yAdj;
            outRect->mTop = outRect->mBottom - h;
            if ((gravity & (Gravity_AXIS_CLIP << Gravity_AXIS_Y_SHIFT))
                    == (Gravity_AXIS_CLIP << Gravity_AXIS_Y_SHIFT)) {
                if (outRect->mTop < container->mTop) {
                    outRect->mTop = container->mTop;
                }
            }
            break;
        default:
            outRect->mTop = container->mTop + yAdj;
            outRect->mBottom = container->mBottom + yAdj;
            break;
    }
    return NOERROR;
}

/**
  * Apply additional gravity behavior based on the overall "display" that an
  * object exists in.  This can be used after
  * {@link #apply(int, int, int, Rect, int, int, Rect)} to place the object
  * within a visible display.  By default this moves or clips the object
  * to be visible in the display; the gravity flags
  * {@link #DISPLAY_CLIP_HORIZONTAL} and {@link #DISPLAY_CLIP_VERTICAL}
  * can be used to change this behavior.
  *
  * @param gravity Gravity constants to modify the placement within the
  * display.
  * @param display The rectangle of the display in which the object is
  * being placed.
  * @param inoutObj Supplies the current object position; returns with it
  * modified if needed to fit in the display.
  */
ECode CGravity::ApplyDisplay(
    /* [in] */ Int32 gravity,
    /* [in] */ IRect* _display,
    /* [in] */ IRect* _inoutObj)
{
    CRect* display = (CRect*)_display;
    CRect* inoutObj = (CRect*)_inoutObj;

    if ((gravity & Gravity_DISPLAY_CLIP_VERTICAL) != 0) {
        if (inoutObj->mTop < display->mTop) inoutObj->mTop = display->mTop;
        if (inoutObj->mBottom > display->mBottom) inoutObj->mBottom = display->mBottom;
    }
    else {
        Int32 off = 0;
        if (inoutObj->mTop < display->mTop) off = display->mTop-inoutObj->mTop;
        else if (inoutObj->mBottom > display->mBottom) off = display->mBottom-inoutObj->mBottom;
        if (off != 0) {
            if (inoutObj->GetHeight() > (display->mBottom-display->mTop)) {
                inoutObj->mTop = display->mTop;
                inoutObj->mBottom = display->mBottom;
            }
            else {
                inoutObj->mTop += off;
                inoutObj->mBottom += off;
            }
        }
    }

    if ((gravity & Gravity_DISPLAY_CLIP_HORIZONTAL) != 0) {
        if (inoutObj->mLeft < display->mLeft) inoutObj->mLeft = display->mLeft;
        if (inoutObj->mRight > display->mRight) inoutObj->mRight = display->mRight;
    }
    else {
        Int32 off = 0;
        if (inoutObj->mLeft < display->mLeft) off = display->mLeft-inoutObj->mLeft;
        else if (inoutObj->mRight > display->mRight) off = display->mRight-inoutObj->mRight;
        if (off != 0) {
            if (inoutObj->GetWidth() > (display->mRight-display->mLeft)) {
                inoutObj->mLeft = display->mLeft;
                inoutObj->mRight = display->mRight;
            }
            else {
                inoutObj->mLeft += off;
                inoutObj->mRight += off;
            }
        }
    }
    return NOERROR;
}

/**
  * <p>Indicate whether the supplied gravity has a vertical pull.</p>
  *
  * @param gravity the gravity to check for vertical pull
  * @return true if the supplied gravity has a vertical pull
  */
ECode CGravity::IsVertical(
    /* [in] */ Int32 gravity,
    /* [out] */ Boolean* isVertical)
{
    if (isVertical == NULL) return E_INVALID_ARGUMENT;

    *isVertical = gravity > 0 && (gravity & Gravity_VERTICAL_GRAVITY_MASK) != 0;
    return NOERROR;
}

/**
  * <p>Indicate whether the supplied gravity has an horizontal pull.</p>
  *
  * @param gravity the gravity to check for horizontal pull
  *@return true if the supplied gravity has an horizontal pull
  */
ECode CGravity::IsHorizontal(
    /* [in] */ Int32 gravity,
    /* [out] */ Boolean* isHorizontal)
{
    if (isHorizontal == NULL) return E_INVALID_ARGUMENT;

    *isHorizontal = gravity > 0 && (gravity & Gravity_HORIZONTAL_GRAVITY_MASK) != 0;
    return NOERROR;
}
