
#include "widget/EditableInputConnection.h"
#include "view/inputmethod/CExtractedText.h"


AutoPtr<IEditable> EditableInputConnection::GetEditable()
{
    ITextView* tv = mTextView;
    if (tv != NULL) {
        	AutoPtr<IEditable> editable;
        	tv->GetEditableText((IEditable**)&editable);
        return editable;
    }
    return NULL;
}

Boolean EditableInputConnection::BeginBatchEdit()
{
    mTextView->BeginBatchEdit();
    return TRUE;
}

Boolean EditableInputConnection::EndBatchEdit()
{
    mTextView->EndBatchEdit();
    return TRUE;
}

Boolean EditableInputConnection::ClearMetaKeyStates(
    /* [in] */ Int32 states)
{
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return FALSE;
    AutoPtr<IKeyListener> kl;
    mTextView->GetKeyListener((IKeyListener**)&kl);
    if (kl != NULL) {
        // try {
        kl->ClearMetaKeyState(mTextView, content, states);
        // } catch (AbstractMethodError e) {
        //     // This is an old listener that doesn't implement the
        //     // new method.
        // }
    }
    return TRUE;
}

Boolean EditableInputConnection::CommitCompletion(
    /* [in] */ ICompletionInfo* text)
{
    // if (DEBUG) Log.v(TAG, "commitCompletion " + text);
    mTextView->BeginBatchEdit();
    mTextView->OnCommitCompletion(text);
    mTextView->EndBatchEdit();
    return TRUE;
}

Boolean EditableInputConnection::PerformEditorAction(
    /* [in] */ Int32 actionCode)
{
    // if (DEBUG) Log.v(TAG, "performEditorAction " + actionCode);
    mTextView->OnEditorAction(actionCode);
    return TRUE;
}

Boolean EditableInputConnection::PerformContextMenuAction(
    /* [in] */ Int32 id)
{
    // if (DEBUG) Log.v(TAG, "performContextMenuAction " + id);
    mTextView->BeginBatchEdit();
    Boolean ret = FALSE;
    mTextView->OnTextContextMenuItem(id, &ret);
    mTextView->EndBatchEdit();
    return TRUE;
}

AutoPtr<IExtractedText> EditableInputConnection::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags)
{
    if (mTextView != NULL) {
        AutoPtr<IExtractedText> et;
        CExtractedText::New((IExtractedText**)&et);
        Boolean state = FALSE;
        mTextView->ExtractText(request, et, &state);
        if (state) {
            if ((flags & InputConnection_GET_EXTRACTED_TEXT_MONITOR) != 0) {
                mTextView->SetExtracting(request);
            }
            return et;
        }
    }
    return NULL;
}

Boolean EditableInputConnection::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    Boolean ret = FALSE;
    mTextView->OnPrivateIMECommand(action, data, &ret);
    return TRUE;
}

Boolean EditableInputConnection::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    if (mTextView == NULL) {
        return BaseInputConnection::CommitText(text, newCursorPosition);
    }

    AutoPtr<ICharSequence> errorBefore;
    mTextView->GetError((ICharSequence**)&errorBefore);
    Boolean success = BaseInputConnection::CommitText(text, newCursorPosition);
    AutoPtr<ICharSequence> errorAfter;
    mTextView->GetError((ICharSequence**)&errorBefore);

    if (errorAfter != NULL && errorBefore == errorAfter) {
        mTextView->SetErrorEx(NULL, NULL);
    }

    return success;
}
