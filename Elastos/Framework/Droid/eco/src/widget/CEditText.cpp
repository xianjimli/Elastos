
#include "widget/CEditText.h"

IVIEW_METHODS_IMPL(CEditText, EditText, EditText);

IDrawableCallback_METHODS_IMPL(CEditText, EditText, EditText);

IKeyEventCallback_METHODS_IMPL(CEditText, EditText, EditText);

IAccessibilityEventSource_METHODS_IMPL(CEditText, EditText, EditText);

ITEXTVIEW_METHODS_IMPL(CEditText, EditText, EditText);

PInterface CEditText::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CEditText::Probe(riid);
}

ECode CEditText::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}


ECode CEditText::SetSelectionEx(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CEditText::SelectAll()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CEditText::ExtendSelection(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CEditText::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CEditText::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CEditText::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

