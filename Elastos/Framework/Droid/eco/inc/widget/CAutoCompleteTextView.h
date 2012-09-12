
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
        /* [in] */ ICharSequence * pHint);

    CARAPI GetDropDownWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI SetDropDownWidth(
        /* [in] */ Int32 width);

    CARAPI GetDropDownHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI SetDropDownHeight(
        /* [in] */ Int32 height);

    CARAPI GetDropDownAnchor(
        /* [out] */ Int32 * pId);

    CARAPI SetDropDownAnchor(
        /* [in] */ Int32 id);

    CARAPI GetDropDownBackground(
        /* [out] */ IDrawable ** ppDrawable);

    CARAPI SetDropDownBackgroundDrawable(
        /* [in] */ IDrawable * pD);

    CARAPI SetDropDownBackgroundResource(
        /* [in] */ Int32 id);

    CARAPI SetDropDownVerticalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetDropDownVerticalOffset(
        /* [out] */ Int32 * pOffset);

    CARAPI SetDropDownHorizontalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetDropDownHorizontalOffset(
        /* [out] */ Int32 * pOffset);

    CARAPI SetDropDownAnimationStyle(
        /* [in] */ Int32 animationStyle);

    CARAPI GetDropDownAnimationStyle(
        /* [out] */ Int32 * pStyle);

    CARAPI IsDropDownAlwaysVisible(
        /* [out] */ Boolean * pVisible);

    CARAPI SetDropDownAlwaysVisible(
        /* [in] */ Boolean dropDownAlwaysVisible);

    CARAPI IsDropDownDismissedOnCompletion(
        /* [out] */ Boolean * pDropDownDismissedOnCompletion);

    CARAPI SetDropDownDismissedOnCompletion(
        /* [in] */ Boolean dropDownDismissedOnCompletion);

    CARAPI GetThreshold(
        /* [out] */ Int32 * pThreshold);

    CARAPI SetThreshold(
        /* [in] */ Int32 threshold);

    CARAPI SetOnItemClickListener(
        /* [in] */ IOnItemClickListener * pListener);

    CARAPI SetOnItemSelectedListener(
        /* [in] */ IOnItemSelectedListener * pListener);

    CARAPI GetItemClickListener(
        /* [out] */ IOnItemClickListener ** ppL);

    CARAPI GetItemSelectedListener(
        /* [out] */ IOnItemSelectedListener ** ppL);

    CARAPI GetOnItemClickListener(
        /* [out] */ IOnItemClickListener ** ppL);

    CARAPI GetOnItemSelectedListener(
        /* [out] */ IOnItemSelectedListener ** ppL);

    CARAPI GetAdapter(
        /* [out] */ IListAdapter ** ppAdapter);

    CARAPI SetAdapter(
        /* [in] */ IListAdapter * pAdapter);

    CARAPI EnoughToFilter(
        /* [out] */ Boolean * pResult);

    CARAPI IsPopupShowing(
        /* [out] */ Boolean * pResult);

    CARAPI ClearListSelection();

    CARAPI SetListSelection(
        /* [in] */ Int32 position);

    CARAPI GetListSelection(
        /* [out] */ Int32 * pSelection);

    CARAPI PerformCompletion();

    CARAPI IsPerformingCompletion(
        /* [out] */ Boolean * pResult);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    CARAPI DismissDropDown();

    CARAPI ShowDropDownAfterLayout();

    CARAPI EnsureImeVisible(
        /* [in] */ Boolean visible);

    CARAPI IsInputMethodNotNeeded(
        /* [out] */ Boolean * pNeeded);

    CARAPI ShowDropDown();

    CARAPI SetForceIgnoreOutsideTouch(
        /* [in] */ Boolean forceIgnoreOutsideTouch);

    CARAPI SetValidator(
        /* [in] */ IValidator * pValidator);

    CARAPI GetValidator(
        /* [out] */ IValidator ** ppValidator);

    CARAPI PerformValidation();

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CAUTOCOMPLETETEXTVIEW_H__
