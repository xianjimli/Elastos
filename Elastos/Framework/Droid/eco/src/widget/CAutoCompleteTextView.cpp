
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

    return NOERROR;
}

ECode CAutoCompleteTextView::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetSelectionEx(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SelectAll()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::ExtendSelection(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetCompletionHint(
    /* [in] */ ICharSequence * pHint)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetDropDownWidth(
    /* [out] */ Int32 * pWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownWidth(
    /* [in] */ Int32 width)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetDropDownHeight(
    /* [out] */ Int32 * pHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownHeight(
    /* [in] */ Int32 height)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetDropDownAnchor(
    /* [out] */ Int32 * pId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownAnchor(
    /* [in] */ Int32 id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetDropDownBackground(
    /* [out] */ IDrawable ** ppDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownBackgroundDrawable(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownBackgroundResource(
    /* [in] */ Int32 id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownVerticalOffset(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetDropDownVerticalOffset(
    /* [out] */ Int32 * pOffset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownHorizontalOffset(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetDropDownHorizontalOffset(
    /* [out] */ Int32 * pOffset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetDropDownAnimationStyle(
    /* [out] */ Int32 * pStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::IsDropDownAlwaysVisible(
    /* [out] */ Boolean * pVisible)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::IsDropDownDismissedOnCompletion(
    /* [out] */ Boolean * pDropDownDismissedOnCompletion)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetDropDownDismissedOnCompletion(
    /* [in] */ Boolean dropDownDismissedOnCompletion)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetThreshold(
    /* [out] */ Int32 * pThreshold)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetThreshold(
    /* [in] */ Int32 threshold)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetOnItemClickListener(
    /* [in] */ IOnItemClickListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetOnItemSelectedListener(
    /* [in] */ IOnItemSelectedListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetItemClickListener(
    /* [out] */ IOnItemClickListener ** ppL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetItemSelectedListener(
    /* [out] */ IOnItemSelectedListener ** ppL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetOnItemClickListener(
    /* [out] */ IOnItemClickListener ** ppL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetOnItemSelectedListener(
    /* [out] */ IOnItemSelectedListener ** ppL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetAdapter(
    /* [out] */ IListAdapter ** ppAdapter)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetAdapter(
    /* [in] */ IListAdapter * pAdapter)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::EnoughToFilter(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::IsPopupShowing(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::ClearListSelection()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetListSelection(
    /* [in] */ Int32 position)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetListSelection(
    /* [out] */ Int32 * pSelection)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::PerformCompletion()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::IsPerformingCompletion(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::DismissDropDown()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::ShowDropDownAfterLayout()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::EnsureImeVisible(
    /* [in] */ Boolean visible)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::IsInputMethodNotNeeded(
    /* [out] */ Boolean * pNeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::ShowDropDown()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::SetValidator(
    /* [in] */ IValidator * pValidator)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::GetValidator(
    /* [out] */ IValidator ** ppValidator)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::PerformValidation()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

