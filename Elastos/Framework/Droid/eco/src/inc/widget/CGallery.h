
#ifndef __CGALLERY_H__
#define __CGALLERY_H__

#include "_CGallery.h"
#include "widget/Gallery.h"
#include "view/ViewMacro.h"
#include "widget/AdapterViewMacro.h"

CarClass(CGallery), public Gallery
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IADAPTERVIEW_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetSelectionEx(
        /* [in] */ Int32 position,
        /* [in] */ Boolean animate);

    CARAPI PointToPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Int32 * pPos);

    CARAPI LayoutEx(
        /* [in] */ Int32 delta,
        /* [in] */ Boolean animate);

    CARAPI SetCallbackDuringFling(
        /* [in] */ Boolean shouldCallback);

    CARAPI SetCallbackOnUnselectedItemClick(
        /* [in] */ Boolean shouldCallback);

    CARAPI SetAnimationDuration(
        /* [in] */ Int32 animationDurationMillis);

    CARAPI SetSpacing(
        /* [in] */ Int32 spacing);

    CARAPI SetUnselectedAlpha(
        /* [in] */ Float unselectedAlpha);

    CARAPI OnSingleTapUp(
        /* [in] */ IMotionEvent * pE,
        /* [out] */ Boolean * pResult);

    CARAPI OnFling(
        /* [in] */ IMotionEvent * pE1,
        /* [in] */ IMotionEvent * pE2,
        /* [in] */ Float velocityX,
        /* [in] */ Float velocityY,
        /* [out] */ Boolean * pResult);

    CARAPI OnScroll(
        /* [in] */ IMotionEvent * pE1,
        /* [in] */ IMotionEvent * pE2,
        /* [in] */ Float distanceX,
        /* [in] */ Float distanceY,
        /* [out] */ Boolean * pResult);

    CARAPI OnDown(
        /* [in] */ IMotionEvent * pE,
        /* [out] */ Boolean * pResult);

    CARAPI OnLongPress(
        /* [in] */ IMotionEvent * pE);

    CARAPI OnShowPress(
        /* [in] */ IMotionEvent * pE);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CGALLERY_H__
