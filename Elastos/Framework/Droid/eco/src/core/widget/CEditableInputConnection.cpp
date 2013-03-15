
#include "widget/CEditableInputConnection.h"

BASEINPUTCONNECTION_METHODS_IMPL(CEditableInputConnection, EditableInputConnection, EditableInputConnection);

ECode CEditableInputConnection::constructor(
    /* [in] */ ITextView* textview)
{
    BaseInputConnection::Init(IView::Probe(textview), TRUE);
    mTextView = textview;
    return NOERROR;
}
