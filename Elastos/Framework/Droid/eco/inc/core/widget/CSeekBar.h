
#ifndef __CSEEKBAR_H__
#define __CSEEKBAR_H__

#include "_CSeekBar.h"
#include "widget/AbsSeekBar.h"
#include "view/ViewMacro.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CSeekBar) , public AbsSeekBar
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI IsIndeterminate(
        /* [out] */ Boolean * pIndeterminate);

    CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    CARAPI GetIndeterminateDrawable(
        /* [out] */ IDrawable ** ppD);

    CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable * pD);

    CARAPI GetProgressDrawable(
        /* [out] */ IDrawable ** ppD);

    CARAPI SetProgressDrawable(
        /* [in] */ IDrawable * pD);

    CARAPI SetProgress(
        /* [in] */ Int32 progress);

    CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    CARAPI GetProgress(
        /* [out] */ Int32 * pProgress);

    CARAPI GetSecondaryProgress(
        /* [out] */ Int32 * pSecondaryProgress);

    CARAPI GetMax(
        /* [out] */ Int32 * pMax);

    CARAPI SetMax(
        /* [in] */ Int32 max);

    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    CARAPI SetInterpolator(
        /* [in] */ IContext * pCtx,
        /* [in] */ Int32 resID);

    CARAPI SetInterpolatorEx(
        /* [in] */ IInterpolator * pInterpolator);

    CARAPI GetInterpolator(
        /* [out] */ IInterpolator ** ppInterpolator);

    CARAPI SetThumb(
        /* [in] */ IDrawable * pThumb);

    CARAPI GetThumbOffset(
        /* [out] */ Int32 * pOffset);

    CARAPI SetThumbOffset(
        /* [in] */ Int32 thumbOffset);

    CARAPI SetKeyProgressIncrement(
        /* [in] */ Int32 increment);

    CARAPI GetKeyProgressIncrement(
        /* [out] */ Int32 * pIncrement);

    CARAPI_(Mutex*) GetSelfLock();

    /**
     * Sets a listener to receive notifications of changes to the SeekBar's progress level. Also
     * provides notifications of when the user starts and stops a touch gesture within the SeekBar.
     *
     * @param l The seek bar notification listener
     *
     * @see SeekBar.OnSeekBarChangeListener
     */
    CARAPI SetOnSeekBarChangeListener(
        /* [in] */ IOnSeekBarChangeListener* l);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

protected:
    //java package access permission
    virtual CARAPI_(void) OnStartTrackingTouch();

    virtual CARAPI_(void) OnStopTrackingTouch();

    virtual CARAPI_(void) OnProgressRefresh(
        /* [in] */ Float scale,
        /* [in] */ Boolean fromUser);

private:
    AutoPtr<IOnSeekBarChangeListener> mOnSeekBarChangeListener;
};

#endif //__CSEEKBAR_H__
