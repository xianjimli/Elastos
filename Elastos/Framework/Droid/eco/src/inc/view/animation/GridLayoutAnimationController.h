
#ifndef __GRIDLAYOUTANIMATIONCONTROLLER_H__
#define __GRIDLAYOUTANIMATIONCONTROLLER_H__

#include "view/animation/LayoutAnimationController.h"

class GridLayoutAnimationController : public LayoutAnimationController
{
public:
    /**
     * The set of parameters that has to be attached to each view contained in
     * the view group animated by the grid layout animation controller. These
     * parameters are used to compute the start time of each individual view's
     * animation.
     */
    class AnimationParameters : public LayoutAnimationController::AnimationParameters
    {
    public:
        /**
         * The view group's column to which the view belongs.
         */
        Int32 mColumn;

        /**
         * The view group's row to which the view belongs.
         */
        Int32 mRow;

        /**
         * The number of columns in the view's enclosing grid layout.
         */
        Int32 mColumnsCount;

        /**
         * The number of rows in the view's enclosing grid layout.
         */
        Int32 mRowsCount;
    };
public:
    GridLayoutAnimationController();

    GridLayoutAnimationController(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    GridLayoutAnimationController(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float columnDelay = 0.5f,
        /* [in] */ Float rowDelay = 0.5f);

    CARAPI_(Float) GetColumnDelay();

    CARAPI SetColumnDelay(
        /* [in] */ Float columnDelay);

    CARAPI_(Float) GetRowDelay();

    CARAPI SetRowDelay(
        /* [in] */ Float rowDelay);

    CARAPI_(Int32) GetDirection();

    CARAPI SetDirection(
        /* [in] */ Int32 direction);

    CARAPI_(Int32) GetDirectionPriority();

    CARAPI SetDirectionPriority(
        /* [in] */ Int32 directionPriority);

    CARAPI_(Boolean) WillOverlap();

protected:
    CARAPI_(Int64) GetDelayForView(
        /* [in] */ IView* view);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float columnDelay = 0.5f,
        /* [in] */ Float rowDelay = 0.5f);

private:
    CARAPI_(Int32) GetTransformedColumnIndex(
        /* [in] */ AnimationParameters* params);

    CARAPI_(Int32) GetTransformedRowIndex(
        /* [in] */ AnimationParameters* params);

private:
    Float mColumnDelay;
    Float mRowDelay;

    Int32 mDirection;
    Int32 mDirectionPriority;
};
#endif //__GRIDLAYOUTANIMATIONCONTROLLER_H__
