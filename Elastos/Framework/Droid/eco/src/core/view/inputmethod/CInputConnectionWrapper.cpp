
#include "view/inputmethod/CInputConnectionWrapper.h"
#include "ext/frameworkext.h"


CInputConnectionWrapper::CInputConnectionWrapper()
    : mMutable(FALSE)
{

}

ECode CInputConnectionWrapper::constructor(
    /* [in] */ IInputConnection* target,
    /* [in] */ Boolean mutables)
{
    mMutable = mutables;
    mTarget = target;
    return NOERROR;
}

ECode CInputConnectionWrapper::SetTarget(
    /* [in] */ IInputConnection* target)
{
    if (mTarget != NULL && !mMutable) {
        // throw new SecurityException("not mutable");
        return E_SECURITY_EXCEPTION;
    }

    mTarget = target;
    return NOERROR;
}

ECode CInputConnectionWrapper::GetTextBeforeCursor(
    /* [in] */ Int32 n,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
    return mTarget->GetTextBeforeCursor(n, flags, text);
}

ECode CInputConnectionWrapper::GetTextAfterCursor(
    /* [in] */ Int32 n,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
    return mTarget->GetTextAfterCursor(n, flags, text);
}

ECode CInputConnectionWrapper::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
    return mTarget->GetSelectedText(flags, text);
}

ECode CInputConnectionWrapper::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [out] */ Int32* capsMode)
{
    return mTarget->GetCursorCapsMode(reqModes, capsMode);
}

ECode CInputConnectionWrapper::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [out] */ IExtractedText** extractedText)
{
    return mTarget->GetExtractedText(request, flags, extractedText);
}

ECode CInputConnectionWrapper::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength,
    /* [out] */ Boolean* flag)
{
    return mTarget->DeleteSurroundingText(leftLength, rightLength, flag);
}

ECode CInputConnectionWrapper::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* flag)
{
    return mTarget->SetComposingText(text, newCursorPosition, flag);
}

ECode CInputConnectionWrapper::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* flag)
{
    return mTarget->SetComposingRegion(start, end, flag);
}

ECode CInputConnectionWrapper::FinishComposingText(
    /* [out] */ Boolean* flag)
{
    return mTarget->FinishComposingText(flag);
}

ECode CInputConnectionWrapper::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* flag)
{
    return mTarget->CommitText(text, newCursorPosition, flag);
}

ECode CInputConnectionWrapper::CommitCompletion(
    /* [in] */ ICompletionInfo* text,
    /* [out] */ Boolean* flag)
{
    return mTarget->CommitCompletion(text, flag);
}

ECode CInputConnectionWrapper::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* flag)
{
    return mTarget->SetSelection(start, end, flag);
}

ECode CInputConnectionWrapper::PerformEditorAction(
    /* [in] */ Int32 editorAction,
    /* [out] */ Boolean* flag)
{
    return mTarget->PerformEditorAction(editorAction, flag);
}

ECode CInputConnectionWrapper::PerformContextMenuAction(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* flag)
{
    return mTarget->PerformContextMenuAction(id, flag);
}

ECode CInputConnectionWrapper::BeginBatchEdit(
    /* [out] */ Boolean* flag)
{
    return mTarget->BeginBatchEdit(flag);
}

ECode CInputConnectionWrapper::EndBatchEdit(
    /* [out] */ Boolean* flag)
{
    return mTarget->EndBatchEdit(flag);
}

ECode CInputConnectionWrapper::SendKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* flag)
{
    return mTarget->SendKeyEvent(event, flag);
}

ECode CInputConnectionWrapper::ClearMetaKeyStates(
    /* [in] */ Int32 states,
    /* [out] */ Boolean* flag)
{
    return mTarget->ClearMetaKeyStates(states, flag);
}

ECode CInputConnectionWrapper::ReportFullscreenMode(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* flag)
{
    return mTarget->ReportFullscreenMode(enabled, flag);
}

ECode CInputConnectionWrapper::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data,
    /* [out] */ Boolean* flag)
{
    return mTarget->PerformPrivateCommand(action, data, flag);
}
