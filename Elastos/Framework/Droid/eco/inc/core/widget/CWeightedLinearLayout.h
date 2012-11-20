
#ifndef __CWEIGHTEDLINEARLAYOUT_H__
#define __CWEIGHTEDLINEARLAYOUT_H__

#include "_CWeightedLinearLayout.h"
#include "view/ViewMacro.h"
#include <widget/LinearLayout.h>

/**
 * A special layout when measured in AT_MOST will take up a given percentage of
 * the available space.
 */
CarClass(CWeightedLinearLayout), public LinearLayout
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CWeightedLinearLayout();

    ECode constructor(
        /* [in] */ IContext* ctx);

    ECode constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI IsBaselineAligned(
        /* [out] */  Boolean* baselineAligned);

    CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    CARAPI GetBaselineAlignedChildIndex(
        /* [out] */ Int32 * pBaselineAlignedChildIndex);

    CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 baselineAlignedChildIndex);

    CARAPI GetWeightSum(
        /* [out] */ Float * pWeightSum);

    CARAPI SetWeightSum(
        /* [in] */ Float weightSum);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI GetOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);

protected:
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    Float        mMajorWeight;
    Float        mMinorWeight;
};

#endif //__CWEIGHTEDLINEARLAYOUT_H__
