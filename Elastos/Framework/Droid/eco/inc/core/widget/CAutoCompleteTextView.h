
#ifndef __CAUTOCOMPLETETEXTVIEW_H__
#define __CAUTOCOMPLETETEXTVIEW_H__

#include "_CAutoCompleteTextView.h"
#include "widget/AutoCompleteTextView.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

CarClass(CAutoCompleteTextView), public AutoCompleteTextView
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    CARAPI SetSelectionEx(
        /* [in] */ Int32 index);

    CARAPI SelectAll();

    CARAPI ExtendSelection(
        /* [in] */ Int32 index);

    CARAPI SetCompletionHint(
        /* [in] */ ICharSequence* hint);

    CARAPI GetDropDownWidth(
        /* [out] */ Int32* width);

    CARAPI SetDropDownWidth(
        /* [in] */ Int32 width);

    CARAPI GetDropDownHeight(
        /* [out] */ Int32* height);

    CARAPI SetDropDownHeight(
        /* [in] */ Int32 height);

    CARAPI GetDropDownAnchor(
        /* [out] */ Int32* id);

    CARAPI SetDropDownAnchor(
        /* [in] */ Int32 id);

    CARAPI GetDropDownBackground(
        /* [out] */ IDrawable** drawable);

    CARAPI SetDropDownBackgroundDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetDropDownBackgroundResource(
        /* [in] */ Int32 id);

    CARAPI SetDropDownVerticalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetDropDownVerticalOffset(
        /* [out] */ Int32* offset);

    CARAPI SetDropDownHorizontalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetDropDownHorizontalOffset(
        /* [out] */ Int32* offset);

    CARAPI SetDropDownAnimationStyle(
        /* [in] */ Int32 animationStyle);

    CARAPI GetDropDownAnimationStyle(
        /* [out] */ Int32* style);

    CARAPI IsDropDownAlwaysVisible(
        /* [out] */ Boolean* visible);

    CARAPI SetDropDownAlwaysVisible(
        /* [in] */ Boolean dropDownAlwaysVisible);

    CARAPI IsDropDownDismissedOnCompletion(
        /* [out] */ Boolean* dropDownDismissedOnCompletion);

    CARAPI SetDropDownDismissedOnCompletion(
        /* [in] */ Boolean dropDownDismissedOnCompletion);

    CARAPI GetThreshold(
        /* [out] */ Int32* threshold);

    CARAPI SetThreshold(
        /* [in] */ Int32 threshold);

    CARAPI SetOnItemClickListener(
        /* [in] */ IOnItemClickListener* listener);

    CARAPI SetOnItemSelectedListener(
        /* [in] */ IOnItemSelectedListener* listener);

    CARAPI GetOnItemClickListener(
        /* [out] */ IOnItemClickListener** l);

    CARAPI GetOnItemSelectedListener(
        /* [out] */ IOnItemSelectedListener** l);

    CARAPI GetAdapter(
        /* [out] */ IListAdapter** adapter);

    CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    CARAPI EnoughToFilter(
        /* [out] */ Boolean* result);

    CARAPI IsPopupShowing(
        /* [out] */ Boolean* result);

    CARAPI ClearListSelection();

    CARAPI SetListSelection(
        /* [in] */ Int32 position);

    CARAPI GetListSelection(
        /* [out] */ Int32* selection);

    CARAPI PerformCompletion();

    CARAPI IsPerformingCompletion(
        /* [out] */ Boolean* result);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    CARAPI DismissDropDown();

    CARAPI ShowDropDownAfterLayout();

    CARAPI EnsureImeVisible(
        /* [in] */ Boolean visible);

    CARAPI IsInputMethodNotNeeded(
        /* [out] */ Boolean* needed);

    CARAPI ShowDropDown();

    CARAPI SetForceIgnoreOutsideTouch(
        /* [in] */ Boolean forceIgnoreOutsideTouch);

    CARAPI SetValidator(
        /* [in] */ IValidator* validator);

    CARAPI GetValidator(
        /* [out] */ IValidator** validator);

    CARAPI PerformValidation();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CAUTOCOMPLETETEXTVIEW_H__
