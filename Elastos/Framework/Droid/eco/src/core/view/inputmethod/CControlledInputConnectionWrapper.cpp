
#include "view/inputmethod/CControlledInputConnectionWrapper.h"

#include <stdio.h>
#include <unistd.h>

ECode CControlledInputConnectionWrapper::constructor(
    /* [in] */ IApartment* mainLooper,
    /* [in] */ IInputConnection* conn,
    /* [in] */ ILocalInputMethodManager* agmer)
{
    mHost = (CLocalInputMethodManager*)agmer;
    return IInputConnectionWrapper::Init(mainLooper, conn);
}

ECode CControlledInputConnectionWrapper::GetTextAfterCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetTextAfterCursor(length, flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetTextBeforeCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetTextBeforeCursor(length, flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetSelectedText(flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetCursorCapsMode(reqModes, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetExtractedText(request, flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    String str;
    text->ToString(&str);
    printf("==== File: %s, Line: %d, pid: %d, text: %s ====\n", __FILE__, __LINE__, getpid(), str.string());
    return IInputConnectionWrapper::CommitText(text, newCursorPosition);
}

ECode CControlledInputConnectionWrapper::CommitCompletion(
    /* [in] */ ICompletionInfo* text)
{
    return IInputConnectionWrapper::CommitCompletion(text);
}

ECode CControlledInputConnectionWrapper::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return IInputConnectionWrapper::SetSelection(start, end);
}

ECode CControlledInputConnectionWrapper::PerformEditorAction(
    /* [in] */ Int32 id)
{
    return IInputConnectionWrapper::PerformEditorAction(id);
}

ECode CControlledInputConnectionWrapper::PerformContextMenuAction(
    /* [in] */ Int32 id)
{
    return IInputConnectionWrapper::PerformContextMenuAction(id);
}

ECode CControlledInputConnectionWrapper::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return IInputConnectionWrapper::SetComposingRegion(start, end);
}

ECode CControlledInputConnectionWrapper::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    String str;
    text->ToString(&str);
    printf("==== File: %s, Line: %d, pid: %d, text: %s ====\n", __FILE__, __LINE__, getpid(), str.string());
    return IInputConnectionWrapper::SetComposingText(text, newCursorPosition);
}

ECode CControlledInputConnectionWrapper::FinishComposingText()
{
    return IInputConnectionWrapper::FinishComposingText();
}

ECode CControlledInputConnectionWrapper::SendKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return IInputConnectionWrapper::SendKeyEvent(event);
}

ECode CControlledInputConnectionWrapper::ClearMetaKeyStates(
    /* [in] */ Int32 states)
{
    return IInputConnectionWrapper::ClearMetaKeyStates(states);
}

ECode CControlledInputConnectionWrapper::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength)
{
    return IInputConnectionWrapper::DeleteSurroundingText(leftLength, rightLength);
}

ECode CControlledInputConnectionWrapper::BeginBatchEdit()
{
    return IInputConnectionWrapper::BeginBatchEdit();
}

ECode CControlledInputConnectionWrapper::EndBatchEdit()
{
    return IInputConnectionWrapper::EndBatchEdit();
}

ECode CControlledInputConnectionWrapper::ReportFullscreenMode(
    /* [in] */ Boolean enabled)
{
    return IInputConnectionWrapper::ReportFullscreenMode(enabled);
}

ECode CControlledInputConnectionWrapper::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return IInputConnectionWrapper::PerformPrivateCommand(action, data);
}

ECode CControlledInputConnectionWrapper::GetDescription(
    /* [out] */ String* str)
{
    return IInputConnectionWrapper::GetDescription(str);
}

Boolean CControlledInputConnectionWrapper::IsActive()
{
    return mHost->mActive;
}
