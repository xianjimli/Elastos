
#include "inputmethodservice/CExtractEditText.h"


IVIEW_METHODS_IMPL(CExtractEditText, _ExtractEditText, _ExtractEditText);
IDrawableCallback_METHODS_IMPL(CExtractEditText, _ExtractEditText, _ExtractEditText);
IKeyEventCallback_METHODS_IMPL(CExtractEditText, _ExtractEditText, _ExtractEditText);
IAccessibilityEventSource_METHODS_IMPL(CExtractEditText, _ExtractEditText, _ExtractEditText);
ITEXTVIEW_METHODS_IMPL(CExtractEditText, _ExtractEditText, _ExtractEditText);

_ExtractEditText::_ExtractEditText():
    mSettingExtractedText(0)
{

}

_ExtractEditText::~_ExtractEditText()
{

}

ECode _ExtractEditText::SetExtractedText(
    /* [in] */ IExtractedText* text)
{
    mSettingExtractedText++;
    EditText::SetExtractedText(text);

    mSettingExtractedText--;
    return NOERROR;
}

Boolean _ExtractEditText::PerformClick()
{
    assert(mIME != NULL);

    if (!EditText::PerformClick() && mIME != NULL) {
        mIME->OnExtractedTextClicked();
        return TRUE;
    }

    return TRUE;
}

Boolean _ExtractEditText::OnTextContextMenuItem(
    /* [in] */ Int32 id)
{
    // Horrible hack: select word option has to be handled by original view to work.
    if (mIME != NULL && id != 0x01020028 /*android.R.id.startSelectingText*/) {
        Boolean ret = FALSE;
        mIME->OnExtractTextContextMenuItem(id, &ret);
        if (ret) {
            return TRUE;
        }
    }

    return EditText::OnTextContextMenuItem(id);
}

Boolean _ExtractEditText::IsInputMethodTarget()
{
    return TRUE;
}

Boolean _ExtractEditText::HasWindowFocus()
{
    return EditText::IsEnabled();
}

Boolean _ExtractEditText::IsFocused()
{
    return EditText::IsEnabled();
}

Boolean _ExtractEditText::HasFocus()
{
    return EditText::IsEnabled();
}

void _ExtractEditText::OnSelectionChanged(
    /* [in] */ Int32 selStart,
    /* [in] */ Int32 selEnd)
{
    if (mSettingExtractedText == 0 && mIME != NULL && selStart >= 0 && selEnd >= 0) {
        mIME->OnExtractedSelectionChanged(selStart, selEnd);
    }
}

CExtractEditText::CExtractEditText()
{}

PInterface CExtractEditText::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CExtractEditText::Probe(riid);
}

ECode CExtractEditText::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = EditText::OnPreDraw();
    return NOERROR;
}

ECode CExtractEditText::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    return EditText::SetSelection(start, stop);
}


ECode CExtractEditText::SetSelectionEx(
    /* [in] */ Int32 index)
{
    return EditText::SetSelection(index);
}

ECode CExtractEditText::SelectAll()
{
    return EditText::SelectAll();
}

ECode CExtractEditText::ExtendSelection(
    /* [in] */ Int32 index)
{
    return EditText::ExtendSelection(index);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context)
{
	return EditText::Init(context, NULL);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return EditText::Init(context, attrs, 0x0101006e /*com.android.internal.R.attr.editTextStyle*/);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return EditText::Init(context, attrs, defStyle);
}

ECode CExtractEditText::SetIME(
    /* [in] */ IInputMethodService* ime)
{
    mIME = ime;
    return NOERROR;
}

ECode CExtractEditText::StartInternalChanges()
{
    mSettingExtractedText += 1;
    return NOERROR;
}

ECode CExtractEditText::FinishInternalChanges()
{
    mSettingExtractedText -= 1;
    return NOERROR;
}

ECode CExtractEditText::HasVerticalScrollBar(
    /* [out] */ Boolean* has)
{
	assert(has != NULL);
	*has = EditText::ComputeVerticalScrollRange() > EditText::ComputeVerticalScrollExtent();
    return NOERROR;
}
