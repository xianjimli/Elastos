
#include "widget/CAutoCompleteTextView.h"

IVIEW_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView, AutoCompleteTextView);

IDrawableCallback_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView, AutoCompleteTextView);

IKeyEventCallback_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView, AutoCompleteTextView);

IAccessibilityEventSource_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView, AutoCompleteTextView);

ITEXTVIEW_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView, AutoCompleteTextView);

CARAPI_(PInterface) CAutoCompleteTextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CAutoCompleteTextView::Probe(riid);
}

ECode CAutoCompleteTextView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = AutoCompleteTextView::OnPreDraw();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    return AutoCompleteTextView::SetSelection(start, stop);
}


ECode CAutoCompleteTextView::SetSelectionEx(
    /* [in] */ Int32 index)
{
    return AutoCompleteTextView::SetSelection(index);
}

ECode CAutoCompleteTextView::SelectAll()
{
    return AutoCompleteTextView::SelectAll();
}

ECode CAutoCompleteTextView::ExtendSelection(
    /* [in] */ Int32 index)
{
    return AutoCompleteTextView::ExtendSelection(index);
}

ECode CAutoCompleteTextView::SetCompletionHint(
    /* [in] */ ICharSequence* hint)
{
    return AutoCompleteTextView::SetCompletionHint(hint);
}

ECode CAutoCompleteTextView::GetDropDownWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = AutoCompleteTextView::GetDropDownWidth();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownWidth(
    /* [in] */ Int32 width)
{
    return AutoCompleteTextView::SetDropDownWidth(width);
}

ECode CAutoCompleteTextView::GetDropDownHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = AutoCompleteTextView::GetDropDownHeight();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownHeight(
    /* [in] */ Int32 height)
{
    return AutoCompleteTextView::SetDropDownHeight(height);
}

ECode CAutoCompleteTextView::GetDropDownAnchor(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = AutoCompleteTextView::GetDropDownAnchor();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownAnchor(
    /* [in] */ Int32 id)
{
    return AutoCompleteTextView::SetDropDownAnchor(id);
}

ECode CAutoCompleteTextView::GetDropDownBackground(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = AutoCompleteTextView::GetDropDownBackground();
    *drawable = temp;
    if (*drawable) {
        (*drawable)->AddRef();
    }

    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return AutoCompleteTextView::SetDropDownBackgroundDrawable(d);
}

ECode CAutoCompleteTextView::SetDropDownBackgroundResource(
    /* [in] */ Int32 id)
{
    return AutoCompleteTextView::SetDropDownBackgroundResource(id);
}

ECode CAutoCompleteTextView::SetDropDownVerticalOffset(
    /* [in] */ Int32 offset)
{
    return AutoCompleteTextView::SetDropDownVerticalOffset(offset);
}

ECode CAutoCompleteTextView::GetDropDownVerticalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = AutoCompleteTextView::GetDropDownVerticalOffset();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownHorizontalOffset(
    /* [in] */ Int32 offset)
{
    return AutoCompleteTextView::SetDropDownHorizontalOffset(offset);
}

ECode CAutoCompleteTextView::GetDropDownHorizontalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = AutoCompleteTextView::GetDropDownHorizontalOffset();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    return AutoCompleteTextView::SetDropDownAnimationStyle(animationStyle);
}

ECode CAutoCompleteTextView::GetDropDownAnimationStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    *style = AutoCompleteTextView::GetDropDownAnimationStyle();
    return NOERROR;
}

ECode CAutoCompleteTextView::IsDropDownAlwaysVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = AutoCompleteTextView::IsDropDownAlwaysVisible();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    return AutoCompleteTextView::SetDropDownAlwaysVisible(dropDownAlwaysVisible);
}

ECode CAutoCompleteTextView::IsDropDownDismissedOnCompletion(
    /* [out] */ Boolean* dropDownDismissedOnCompletion)
{
    VALIDATE_NOT_NULL(dropDownDismissedOnCompletion);
    *dropDownDismissedOnCompletion =
        AutoCompleteTextView::IsDropDownDismissedOnCompletion();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownDismissedOnCompletion(
    /* [in] */ Boolean dropDownDismissedOnCompletion)
{
    return AutoCompleteTextView::SetDropDownDismissedOnCompletion(
        dropDownDismissedOnCompletion);
}

ECode CAutoCompleteTextView::GetThreshold(
    /* [out] */ Int32* threshold)
{
    VALIDATE_NOT_NULL(threshold);
    *threshold = AutoCompleteTextView::GetThreshold();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetThreshold(
    /* [in] */ Int32 threshold)
{
    return AutoCompleteTextView::SetThreshold(threshold);
}

ECode CAutoCompleteTextView::SetOnItemClickListener(
    /* [in] */ IOnItemClickListener* listener)
{
    return AutoCompleteTextView::SetOnItemClickListener(listener);
}

ECode CAutoCompleteTextView::SetOnItemSelectedListener(
    /* [in] */ IOnItemSelectedListener* listener)
{
    return AutoCompleteTextView::SetOnItemSelectedListener(listener);
}

ECode CAutoCompleteTextView::GetOnItemClickListener(
    /* [out] */ IOnItemClickListener** l)
{
    VALIDATE_NOT_NULL(l);
    AutoPtr<IOnItemClickListener> temp =
        AutoCompleteTextView::GetOnItemClickListener();
    *l = temp;
    if (*l) {
        (*l)->AddRef();
    }

    return NOERROR;
}

ECode CAutoCompleteTextView::GetOnItemSelectedListener(
    /* [out] */ IOnItemSelectedListener** l)
{
    VALIDATE_NOT_NULL(l);
    AutoPtr<IOnItemSelectedListener> temp =
        AutoCompleteTextView::GetOnItemSelectedListener();
    *l = temp;
    if (*l) {
        (*l)->AddRef();
    }

    return NOERROR;
}

ECode CAutoCompleteTextView::GetAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    AutoPtr<IListAdapter> temp = AutoCompleteTextView::GetAdapter();
    *adapter = temp;
    if (*adapter) {
        (*adapter)->AddRef();
    }

    return NOERROR;
}

ECode CAutoCompleteTextView::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    return AutoCompleteTextView::SetAdapter(adapter);
}

ECode CAutoCompleteTextView::EnoughToFilter(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = AutoCompleteTextView::EnoughToFilter();
    return NOERROR;
}

ECode CAutoCompleteTextView::IsPopupShowing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = AutoCompleteTextView::IsPopupShowing();
    return NOERROR;
}

ECode CAutoCompleteTextView::ClearListSelection()
{
    return AutoCompleteTextView::ClearListSelection();
}

ECode CAutoCompleteTextView::SetListSelection(
    /* [in] */ Int32 position)
{
    return AutoCompleteTextView::SetListSelection(position);
}

ECode CAutoCompleteTextView::GetListSelection(
    /* [out] */ Int32* selection)
{
    VALIDATE_NOT_NULL(selection);
    *selection = AutoCompleteTextView::GetListSelection();
    return NOERROR;
}

ECode CAutoCompleteTextView::PerformCompletion()
{
    return AutoCompleteTextView::PerformCompletion();
}

ECode CAutoCompleteTextView::IsPerformingCompletion(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = AutoCompleteTextView::IsPerformingCompletion();
    return NOERROR;
}

ECode CAutoCompleteTextView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    return AutoCompleteTextView::OnFilterComplete(count);
}

ECode CAutoCompleteTextView::DismissDropDown()
{
    return AutoCompleteTextView::DismissDropDown();
}

ECode CAutoCompleteTextView::ShowDropDownAfterLayout()
{
    return AutoCompleteTextView::ShowDropDownAfterLayout();
}

ECode CAutoCompleteTextView::EnsureImeVisible(
    /* [in] */ Boolean visible)
{
    return AutoCompleteTextView::EnsureImeVisible(visible);
}

ECode CAutoCompleteTextView::IsInputMethodNotNeeded(
    /* [out] */ Boolean* needed)
{
    VALIDATE_NOT_NULL(needed);
    *needed = AutoCompleteTextView::IsInputMethodNotNeeded();
    return NOERROR;
}

ECode CAutoCompleteTextView::ShowDropDown()
{
    return AutoCompleteTextView::ShowDropDown();
}

ECode CAutoCompleteTextView::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    return AutoCompleteTextView::SetForceIgnoreOutsideTouch(forceIgnoreOutsideTouch);
}

ECode CAutoCompleteTextView::SetValidator(
    /* [in] */ IValidator* validator)
{
    return AutoCompleteTextView::SetValidator(validator);
}

ECode CAutoCompleteTextView::GetValidator(
    /* [out] */ IValidator** validator)
{
    VALIDATE_NOT_NULL(validator);
    AutoPtr<IValidator> temp = AutoCompleteTextView::GetValidator();
    *validator = temp;
    if (*validator) {
        (*validator)->AddRef();
    }

    return NOERROR;
}

ECode CAutoCompleteTextView::PerformValidation()
{
    return AutoCompleteTextView::PerformValidation();
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext* context)
{
    return AutoCompleteTextView::Init(context);
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AutoCompleteTextView::Init(context, attrs);
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return AutoCompleteTextView::Init(context, attrs, defStyle);
}
