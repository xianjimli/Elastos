
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
        /* [out] */ Int32* pos);

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
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* result);

    CARAPI OnFling(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float velocityX,
        /* [in] */ Float velocityY,
        /* [out] */ Boolean* result);

    CARAPI OnScroll(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float distanceX,
        /* [in] */ Float distanceY,
        /* [out] */ Boolean* result);

    CARAPI OnDown(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* result);

    CARAPI OnLongPress(
        /* [in] */ IMotionEvent* e);

    CARAPI OnShowPress(
        /* [in] */ IMotionEvent* e);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CGALLERY_H__
