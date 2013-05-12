
#ifndef __CNUMBERPICKER_H__
#define __CNUMBERPICKER_H__

#include "_CNumberPicker.h"
#include "widget/NumberPicker.h"
#include "view/ViewMacro.h"

CarClass(CNumberPicker) , public NumberPicker
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI IsBaselineAligned(
        /* [out] */ Boolean* baselineAligned);

    CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    CARAPI GetBaselineAlignedChildIndex(
        /* [out] */ Int32* baselineAlignedChildIndex);

    CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 baselineAlignedChildIndex);

    CARAPI GetWeightSum(
        /* [out] */ Float* weightSum);

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

    CARAPI SetOnChangeListener(
        /* [in] */ IOnChangedListener* listener);

    CARAPI SetFormatter(
        /* [in] */ IFormatter* formatter);

    CARAPI SetRange(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SetRangeEx(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const ArrayOf<String> & displayedValues);

    CARAPI SetCurrent(
        /* [in] */ Int32 current);

    CARAPI SetSpeed(
        /* [in] */ Int64 speed);

    CARAPI CancelIncrement();

    CARAPI CancelDecrement();

    CARAPI GetCurrent(
        /* [out] */ Int32* current);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};

#endif // __CNUMBERPICKER_H__
