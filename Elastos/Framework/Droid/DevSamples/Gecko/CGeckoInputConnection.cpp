
#include "GeckoApp.h"
#include "GeckoAppShell.h"
#include "CGeckoInputConnection.h"
#include "GeckoEvent.h"
#include <elastos/Math.h>
#include <stdlib.h>
#include <stdio.h>

CGeckoInputConnection::CGeckoInputConnection()
{
}

CGeckoInputConnection::~CGeckoInputConnection()
{
    if (mComposingText) {
        mComposingText=NULL;
    }
}

PInterface CGeckoInputConnection::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ITextWatcher == riid) {
        return (IInterface*)(ITextWatcher*)this;
    }else if (EIID_IGeckoInputConnection == riid)
    {
        return (IInterface*)(IGeckoInputConnection*)this;
    }else if(EIID_IObject==EIID_IObject)
    {
        return (IInterface*)(IObject*)this;
    }else {
        return NULL;
    }

    return NULL;
}

UInt32 CGeckoInputConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CGeckoInputConnection::Release()
{
    return ElRefBase::Release();
}

ECode CGeckoInputConnection::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{

    if (pIID == NULL) return E_INVALID_ARGUMENT;
    if (pObject == (IInterface*)(ITextWatcher*)this)
    {
        *pIID = EIID_ITextWatcher;
    }else if (pObject == (IInterface*)(IGeckoInputConnection*)this) {
        *pIID = EIID_IGeckoInputConnection;
    }
    else if (pObject == (IInterface*)(IObject*)this) {
        *pIID = EIID_IObject;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


ECode CGeckoInputConnection::GetTextBeforeCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence ** ppText)
{
    return GetTextAfterCursor(-length, flags, ppText);
}

ECode CGeckoInputConnection::GetTextAfterCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence ** ppText)
{

    if (NULL == ppText) return E_INVALID_ARGUMENT;
    GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_GET_SELECTION, 0, 0));
    //try {
    //      mQueryResult->take();
    //} catch (InterruptedException e) {
    //    Log.e("GeckoAppJava", "IME: getTextBefore/AfterCursor interrupted", e);
    //    return null;
    //}

    /* Compatible with both positive and negative length
        (no need for separate code for getTextBeforeCursor) */
    int textStart = mSelectionStart;
    int textLength = length;

    if (length < 0) {
      textStart += length;
      textLength = -length;
      if (textStart < 0) {
        textStart = 0;
        textLength = mSelectionStart;
      }
    }

    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(GeckoEvent::IME_GET_TEXT, textStart, textLength));
    //try {
        //*ppText = mQueryResult.take();
    //} catch (InterruptedException e) {
        //Log.e("GeckoAppJava", "IME: getTextBefore/AfterCursor: Interrupted!", e);
        //return null;
    //}

    return NOERROR;
}

ECode CGeckoInputConnection::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence ** ppText)
{
    // TODO: Add your code here
    return mBaseInputConnection->GetSelectedText(flags, ppText);
}

ECode CGeckoInputConnection::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [out] */ Int32 * pCapsMode)
{
    if (NULL == pCapsMode) return E_INVALID_ARGUMENT;
    *pCapsMode = 0;
    return NOERROR;
}

ECode CGeckoInputConnection::GetExtractedText(
    /* [in] */ IExtractedTextRequest * pRequest,
    /* [in] */ Int32 flags,
    /* [out] */ IExtractedText ** ppExtractedText)
{
    if (pRequest == NULL || ppExtractedText == NULL)
        return E_INVALID_ARGUMENT;

    //AutoPtr<CExtractedText> extract = NULL;
    //ECode ec = CExtractedText::NewByFriend((CExtractedText **)&extract);
    //if (NULL== extract) return E_OUT_OF_MEMORY;
    //extract->mFlags = 0;
    //extract->mPartialStartOffset = -1;
    //extract->mPartialEndOffset = -1;

    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(GeckoEvent::IME_GET_SELECTION, 0, 0));
    //try {
     //   mQueryResult->Take();
    //} catch (InterruptedException e) {
    //    Log.e("GeckoAppJava", "IME: getExtractedText interrupted", e);
    //    return null;
    //}
    //extract->mSelectionStart = mSelectionStart;
    //extract->mSelectionEnd = mSelectionStart + mSelectionLength;

    // bug 617298 - IME_GET_TEXT sometimes gives the wrong result due to
    // a stale cache. Use a set of three workarounds:
    // 1. Sleep for 20 milliseconds and hope the child updates us with the new text.
    //    Very evil and, consequentially, most effective.
    //try {
    //    CThread::Sleep(20, NULL);
    //} catch (InterruptedException e) {}

    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(GeckoEvent::IME_GET_TEXT, 0, Math::INT32_MAX_VALUE));
    //try {
    //    extract->mStartOffset = 0;
    //    extract->mText = mQueryResult.Take();

        // 2. Make a guess about what the text actually is
    //    if (mComposing && extract->mSelectionEnd > extract->mText->GetLength())
    //        extract->mText = extract->mText->SubSequence(0, Math::Min(extract->mText->GetLength(), mCompositionStart)) + mComposingText;

        // 3. If all else fails, make sure our selection indexes make sense
    //    extract->mSelectionStart = Math::Min(extract->mSelectionStart, extract->mText.GetLength());
    //    extract->mSelectionEnd = Math::Min(extract->mSelectionEnd, extract->mText->GetLength());

        if ((flags & InputConnection_GET_EXTRACTED_TEXT_MONITOR) != 0)
            mUpdateRequest = pRequest;
    //    *ppExtractedText = extract;

    //} catch (InterruptedException e) {
    //    Log.e("GeckoAppJava", "IME: getExtractedText interrupted", e);
    //    return null;
    //}

    return NOERROR;
}

ECode CGeckoInputConnection::GetEditable(IEditable** ppE)
{
    if (ppE) *ppE = NULL;
    return NOERROR;
}

ECode CGeckoInputConnection::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength,
    /* [out] */ Boolean * pFlag)
{
/* deleteSurroundingText is supposed to ignore the composing text,
            so we cancel any pending composition, delete the text, and then
            restart the composition */

    if (mComposing) {
        String s;
        // Cancel current composition
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(0, 0, 0, 0, 0, 0, s));
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_COMPOSITION_END, 0, 0));
    }

    // Select text to be deleted
    Int32 delStart, delLen;
    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(GeckoEvent::IME_GET_SELECTION, 0, 0));
    //try {
    //    mQueryResult->take();
    //} catch (InterruptedException e) {
    //    Log.e("GeckoAppJava", "IME: deleteSurroundingText interrupted", e);
    //    return false;
    //}
    delStart = mSelectionStart > leftLength ?
                mSelectionStart - leftLength : 0;
    delLen = mSelectionStart + rightLength - delStart;
    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(GeckoEvent::IME_SET_SELECTION, delStart, delLen));

    // Restore composition / delete text
    if (mComposing) {
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_COMPOSITION_BEGIN, 0, 0));
        if (mComposingText->GetLength() > 0) {
            /* IME_SET_TEXT doesn't work well with empty strings */
            GeckoAppShell::SendEventToGecko(
                new GeckoEvent(
                               0,
                               mComposingText->GetLength(),
                               GeckoEvent::IME_RANGE_RAWINPUT,
                               GeckoEvent::IME_RANGE_UNDERLINE,
                               0,
                               0,
                               *mComposingText));
        }
    } else {
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_DELETE_TEXT, 0, 0));
    }

    if (pFlag) *pFlag = TRUE;
    return NOERROR;
}

ECode CGeckoInputConnection::SetComposingText(
    /* [in] */ ICharSequence * text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean * pFlag)
{
    if (NULL == pFlag) return E_INVALID_ARGUMENT;
    // Set new composing text
    if (text != NULL) text->ToString(mComposingText);
    else  mComposingText->SetTo("");
    ECode ec = NOERROR;

    if (!mComposing) {
        if (mComposingText->GetLength() == 0) {
            // Some IMEs such as iWnn sometimes call with empty composing
            // text.  (See bug 664364)
            // If composing text is empty, ignore this and don't start
            // compositing.
            *pFlag = TRUE;
            return NOERROR;
        }

        // Get current selection
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_GET_SELECTION, 0, 0));
        //try {
        //    mQueryResult->take();
        //} catch (InterruptedException e) {
        //    Log.e("GeckoAppJava", "IME: setComposingText interrupted", e);
        //    return false;
        //}
        // Make sure we are in a composition
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_COMPOSITION_BEGIN, 0, 0));
        mComposing = TRUE;
        mCompositionStart = mSelectionLength >= 0 ?
            mSelectionStart : mSelectionStart + mSelectionLength;
    }

    Int32 nLen = 0;
    // Set new selection
    // New selection should be within the composition
    mCompositionSelStart = newCursorPosition > 0 ? mComposingText->GetLength() : 0;
    mCompositionSelLen = 0;

    AutoPtr<ISpanned> span = ISpanned::Probe(text);
    // Handle composition text styles
    if (text != NULL && span) {

        Int32 spanStart = 0, spanEnd = 0;
        Boolean pastSelStart = FALSE, pastSelEnd = FALSE;
        AutoPtr<ICharacterStyle> CharacterStyle;
        InterfaceID id;
        //CCharacterStyle->GetInterfaceID(CharacterStyle, &id);

        do {
            Int32 rangeType = GeckoEvent::IME_RANGE_CONVERTEDTEXT;
            Int32 rangeStyles = 0, rangeForeColor = 0, rangeBackColor = 0;
            text->GetLength(&nLen);

            // Find next offset where there is a style transition
            ec = span->NextSpanTransition(spanStart + 1, nLen,
                id, &spanEnd);
            if (FAILED(ec)) return ec;

            // We need to count the selection as a transition
            if (mCompositionSelLen >= 0) {
                if (!pastSelStart && spanEnd >= mCompositionSelStart) {
                    spanEnd = mCompositionSelStart;
                    pastSelStart = TRUE;
                } else if (!pastSelEnd && spanEnd >=
                        mCompositionSelStart + mCompositionSelLen) {
                    spanEnd = mCompositionSelStart + mCompositionSelLen;
                    pastSelEnd = TRUE;
                    rangeType = GeckoEvent::IME_RANGE_SELECTEDRAWTEXT;
                }
            } else {
                if (!pastSelEnd && spanEnd >=
                        mCompositionSelStart + mCompositionSelLen) {
                    spanEnd = mCompositionSelStart + mCompositionSelLen;
                    pastSelEnd = TRUE;
                } else if (!pastSelStart &&
                        spanEnd >= mCompositionSelStart) {
                    spanEnd = mCompositionSelStart;
                    pastSelStart = TRUE;
                    rangeType = GeckoEvent::IME_RANGE_SELECTEDRAWTEXT;
                }
            }
            // Empty range, continue
            if (spanEnd <= spanStart)
                continue;

            // Get and iterate through list of span objects within range
            ArrayOf<ICharacterStyle*>* styles;
            ec  = span->GetSpans(
                spanStart, spanEnd, id, (ArrayOf<IInterface*>**)&styles);

            for (UInt32 i = 0; i < styles->GetLength(); i++) {
                 ICharacterStyle* style = (*styles)[i];

                //if (IUnderlineSpan::Probe(style)) {
                    // Text should be underlined
                //    rangeStyles |= CGeckoEvent::IME_RANGE_UNDERLINE;

                //} else if (IForegroundColorSpan::Probe(style)) {
                    // Text should be of a different foreground color
                 //   rangeStyles |= CGeckoEvent::IME_RANGE_FORECOLOR;
                //    rangeForeColor = ((IForegroundColorSpan*)style)->GetForegroundColor();

                //} else if (IBackgroundColorSpan::Probe(style)) {
                    // Text should be of a different background color
                //    rangeStyles |= CGeckoEvent::IME_RANGE_BACKCOLOR;
                //    rangeBackColor = ((IBackgroundColorSpan*)style)->GetBackgroundColor();
                //}
            }

            // Add range to array, the actual styles are
            //  applied when IME_SET_TEXT is sent
            GeckoAppShell::SendEventToGecko(
                new GeckoEvent(spanStart, spanEnd - spanStart,
                               rangeType, rangeStyles,
                               rangeForeColor, rangeBackColor));

            spanStart = spanEnd;
            text->GetLength(&nLen);
            ArrayOf<ICharacterStyle*>::Free(styles);
        } while (spanStart < nLen);
    } else {
        text->GetLength(&nLen);
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(0, text == NULL ? 0 : nLen,
                           GeckoEvent::IME_RANGE_RAWINPUT,
                           GeckoEvent::IME_RANGE_UNDERLINE, 0, 0));
    }

    // Change composition (treating selection end as where the caret is)
    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(mCompositionSelStart + mCompositionSelLen, 0,
                       GeckoEvent::IME_RANGE_CARETPOSITION, 0, 0, 0,
                       *mComposingText));
    *pFlag = TRUE;
    return ec;
}

ECode CGeckoInputConnection::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return mBaseInputConnection->SetComposingRegion(start, end, pFlag);
}

ECode CGeckoInputConnection::FinishComposingText(
    /* [out] */ Boolean * pFlag)
{

    if (mComposing) {
        // Set style to none
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(0, mComposingText->GetLength(),
                           GeckoEvent::IME_RANGE_RAWINPUT, 0, 0, 0,
                           *mComposingText));
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_COMPOSITION_END, 0, 0));
        mComposing = FALSE;
        mComposingText->SetTo("");

        // Make sure caret stays at the same position
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_SET_SELECTION,
                           mCompositionStart + mCompositionSelStart, 0));
    }
    return true;
}

ECode CGeckoInputConnection::CommitText(
    /* [in] */ ICharSequence * pText,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean * pFlag)
{
    Boolean bRet = FALSE;
    SetComposingText(pText, newCursorPosition, &bRet);
    FinishComposingText(&bRet);

    if (pFlag) *pFlag = TRUE;
    return NOERROR;
}

ECode CGeckoInputConnection::CommitCompletion(
    /* [in] */ ICompletionInfo * pText,
    /* [out] */ Boolean * pFlag)
{
    String text;
    //pText->GetText(&text);
    ICharSequence* pCharS;
    ECode ec = CStringWrapper::New(text, &pCharS);
    if (FAILED(ec)) return ec;
    ec = CommitText(pCharS, 1, pFlag);
    pCharS->Release();
    return ec;
}

ECode CGeckoInputConnection::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean * pFlag)
{
    if (NULL == pFlag) return E_INVALID_ARGUMENT;

    if (mComposing) {
        /* Translate to fake selection positions */
        start -= mCompositionStart;
        end -= mCompositionStart;

        if (start < 0)
            start = 0;
        else if (start > mComposingText->GetLength())
            start = mComposingText->GetLength();

        if (end < 0)
            end = 0;
        else if (end > mComposingText->GetLength())
            end = mComposingText->GetLength();

        mCompositionSelStart = start;
        mCompositionSelLen = end - start;
    } else {
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_SET_SELECTION,
                           start, end - start));
    }
    *pFlag = true;
    return NOERROR;
}

ECode CGeckoInputConnection::PerformEditorAction(
    /* [in] */ Int32 editorAction,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return mBaseInputConnection->PerformEditorAction(editorAction, pFlag);
}

ECode CGeckoInputConnection::PerformContextMenuAction(
    /* [in] */ Int32 id,
    /* [out] */ Boolean * pFlag)
{
    //Log.d("GeckoAppJava", "IME: performContextMenuAction");

    // First we need to ask Gecko to tell us the full contents of the
    // text field we're about to operate on.
    String text;
    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(GeckoEvent::IME_GET_TEXT, 0, Math::INT32_MAX_VALUE));
    //try {
    //    text = mQueryResult->Take();
    //} catch (InterruptedException e) {
    //    Log.e("GeckoAppJava", "IME: performContextMenuAction interrupted", e);
    //    return false;
    //}
    Boolean bResult = FALSE;
    switch (id) {
        //case R->id->selectAll:
            SetSelection(0, text.GetLength(), &bResult);
            break;
        //case R->id->cut:
            // Fill the clipboard
            GeckoAppShell::SetClipboardText(text);
            // If GET_TEXT returned an empty selection, we'll select everything
            if (mSelectionLength <= 0)
                GeckoAppShell::SendEventToGecko(
                    new GeckoEvent(GeckoEvent::IME_SET_SELECTION, 0, text.GetLength()));
            GeckoAppShell::SendEventToGecko(
                new GeckoEvent(GeckoEvent::IME_DELETE_TEXT, 0, 0));
            break;
        //case R->id->paste:
            String clipboardText;
            GeckoAppShell::GetClipboardText(&clipboardText);
            ICharSequence* pCharS;
            ECode ec = CStringWrapper::New(clipboardText, &pCharS);
            if (FAILED(ec)) return ec;
            CommitText(pCharS, 1, &bResult);
            pCharS->Release();
            break;
        //case R->id->copy:
            // If there is no selection set, we must be doing "Copy All",
            // otherwise, we need to get the selection from Gecko
            if (mSelectionLength > 0) {
                GeckoAppShell::SendEventToGecko(
                    new GeckoEvent(GeckoEvent::IME_GET_SELECTION, 0, 0));
                //try {
                //    text = mQueryResult.take();
                //} catch (InterruptedException e) {
                //    Log.e("GeckoAppJava", "IME: performContextMenuAction interrupted", e);
                //    return false;
                //}
            }
            GeckoAppShell::SetClipboardText(text);
            break;
    }
    if (pFlag) *pFlag = TRUE;
    return NOERROR;
}

ECode CGeckoInputConnection::BeginBatchEdit(
    /* [out] */ Boolean * pFlag)
{
    if (NULL == pFlag) return E_INVALID_ARGUMENT;
    *pFlag = TRUE;
    return NOERROR;
}

ECode CGeckoInputConnection::EndBatchEdit(
    /* [out] */ Boolean * pFlag)
{
    if (NULL == pFlag) return E_INVALID_ARGUMENT;
    *pFlag = TRUE;
    return NOERROR;
}

ECode CGeckoInputConnection::SendKeyEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return mBaseInputConnection->SendKeyEvent(pEvent, pFlag);
}

ECode CGeckoInputConnection::ClearMetaKeyStates(
    /* [in] */ Int32 states,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return mBaseInputConnection->ClearMetaKeyStates(states, pFlag);
}

ECode CGeckoInputConnection::ReportFullscreenMode(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return mBaseInputConnection->ReportFullscreenMode(enabled, pFlag);
}

ECode CGeckoInputConnection::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle * pData,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return mBaseInputConnection->PerformPrivateCommand(action, pData, pFlag);
}

ECode CGeckoInputConnection::constructor(
     IView * pTargetView)
{
    //mQueryResult = new SynchronousQueue<String>();
    mComposing = FALSE;
    mComposingText = NULL;
    mCompositionStart = 0;
    mCompositionSelStart = 0;
    mCompositionSelLen = 0;
    mUpdateRequest = NULL;
    //mUpdateExtract = NULL;
    mSelectionStart = NULL;
    mSelectionLength = NULL;
    ECode ec = CBaseInputConnection::New(
        pTargetView, TRUE, (IInputConnection**)&mBaseInputConnection);
    return ec;
}

ECode CGeckoInputConnection::BeforeTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count,
            /* [in] */ Int32 after)
{
    return E_NOT_IMPLEMENTED;
}

ECode CGeckoInputConnection::OnTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 start,
            /* [in] */ Int32 before,
            /* [in] */ Int32 count)
{
    GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_SET_SELECTION, start, before));

    if (count == 0) {
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_DELETE_TEXT, 0, 0));
    }
    else {
        Boolean bRet;
        // Start and stop composition to force UI updates.
        FinishComposingText(&bRet);
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_COMPOSITION_BEGIN, 0, 0));

        AutoPtr<ICharSequence> subStr;
        s->SubSequence(start, start + count, (ICharSequence**)&subStr);
        String subString;
        subStr->ToString(&subString);
        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(0, count,
                           GeckoEvent::IME_RANGE_RAWINPUT, 0, 0, 0,
                           subString));

        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_COMPOSITION_END, 0, 0));

        GeckoAppShell::SendEventToGecko(
            new GeckoEvent(GeckoEvent::IME_SET_SELECTION, start + count, 0));
    }

    // Block this thread until all pending events are processed
    GeckoAppShell::GeckoEventSync();

    return NOERROR;
}

ECode CGeckoInputConnection::AfterTextChanged(
            /* [in] */ IEditable* s)
{
    return E_NOT_IMPLEMENTED;
}

ECode CGeckoInputConnection::OnKeyDel(
        /* [out] */ Boolean * pFlag)
{
    // Some IMEs don't update us on deletions
    // In that case we are not updated when a composition
    // is destroyed, and Bad Things happen
    if (NULL == pFlag) return E_INVALID_ARGUMENT;

    if (!mComposing)
        *pFlag = FALSE;

    if (mComposingText->GetLength() > 0) {
         *mComposingText = mComposingText->Substring(
                                    0,
                                    mComposingText->GetLength() - 1);

        if (mComposingText->GetLength() > 0)
            *pFlag = FALSE;
    }

    Boolean bResult;
    CommitText(NULL, 1, &bResult);

    *pFlag = TRUE;
    return NOERROR;
}

ECode CGeckoInputConnection::NotifyTextChange(
    /* [in] */ ILocalInputMethodManager* imm,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 oldEnd,
    /* [in] */ Int32 newEnd)
{
    String editable;
    //GeckoApp::surfaceView->GetEditable(&editable);
    if (!text.Equals(editable)) {
        //GeckoApp::surfaceView->SetEditable(text);
    }

    if (mUpdateRequest == NULL)
        return E_INVALID_OPERATION;

    //mUpdateExtract->mFlags = 0;

    // We update from (0, oldEnd) to (0, newEnd) because some Android IMEs
    // assume that updates start at zero, according to jchen.
    //mUpdateExtract->mPartialStartOffset = 0;
    //mUpdateExtract->mPartialEndOffset = oldEnd;

    // Faster to not query for selection
    //mUpdateExtract->mSelectionStart = newEnd;
    //mUpdateExtract->mSelectionEnd = newEnd;

    //mUpdateExtract->mText = text.Substring(0, newEnd);
    //mUpdateExtract->mStartOffset = 0;

    //imm->UpdateExtractedText(CGeckoApp::surfaceView,
    //    mUpdateRequest->mToken, mUpdateExtract);

    return NOERROR;
}


ECode CGeckoInputConnection::NotifySelectionChange(
    /* [in] */ ILocalInputMethodManager* imm,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{

    //if (mComposing)
        //imm->PdateSelection(CGeckoApp::surfaceView,
        //    mCompositionStart + mCompositionSelStart,
        //    mCompositionStart + mCompositionSelStart + mCompositionSelLen,
        //    mCompositionStart,
         //   mCompositionStart + mComposingText->GetLength());
    //else
        //imm->UpdateSelection(CGeckoApp::surfaceView, start, end, -1, -1);

    //Int32 maxLen = CGeckoApp::surfaceView->mEditable->GetLength();
    //Selection->SetSelection(CGeckoApp::surfaceView->mEditable,
    //                       Math::Min(start, maxLen),
     //                      Math::Min(end, maxLen));

    return NOERROR;
}

ECode CGeckoInputConnection::Reset()
{
    mComposing = FALSE;
    mComposingText->SetTo("");

    if (mUpdateRequest)
    {
        mUpdateRequest = NULL;
    }
    return NOERROR;
}

ECode CGeckoInputConnection::SetSelectionStart(
    /* [in] */ Int32 selectionStart)
{
    mSelectionStart = selectionStart;
    return NOERROR;
}

ECode CGeckoInputConnection::SetSelectionLength(
    /* [in] */ Int32 selectionLength)
{
    mSelectionLength = selectionLength;
    return NOERROR;
}

ECode CGeckoInputConnection::PutResult(
    /* [in] */ const String& result)
{
    //mQueryResult.Put(result);
    return NOERROR;
}

