
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

ECode CEditText::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = EditText::OnPreDraw();
    return NOERROR;
}

ECode CEditText::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    return EditText::SetSelection(start, stop);
}


ECode CEditText::SetSelectionEx(
    /* [in] */ Int32 index)
{
    return EditText::SetSelection(index);
}

ECode CEditText::SelectAll()
{
    return EditText::SelectAll();
}

ECode CEditText::ExtendSelection(
    /* [in] */ Int32 index)
{
    return EditText::ExtendSelection(index);
}

ECode CEditText::constructor(
    /* [in] */ IContext* context)
{
    return EditText::Init(context);
}

ECode CEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return EditText::Init(context, attrs);
}

ECode CEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return EditText::Init(context, attrs, defStyle);
}

