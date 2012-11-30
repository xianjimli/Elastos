
#include "view/inputmethod/BaseInputConnection.h"
#include "text/Selection.h"
#include "text/CEditableFactory.h"
#include "text/method/MetaKeyKeyListener.h"
#include "text/SpannableStringInternal.h"
#include "text/TextUtils.h"
#include "os/SystemClock.h"
#include "view/CKeyEvent.h"
#include "utils/ArrayUtils.h"
#include "text/CSpannableStringBuilder.h"


const Boolean BaseInputConnection::DEBUG;
const CString BaseInputConnection::TAG = "BaseInputConnection";
const AutoPtr<IInterface> BaseInputConnection::COMPOSING = new ComposingText();

ComposingText::ComposingText()
{

}

PInterface ComposingText::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_INoCopySpan == riid) {
        return (INoCopySpan *)this;
    }

    return NULL;
}

UInt32 ComposingText::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ComposingText::Release()
{
    return ElRefBase::Release();
}

ECode ComposingText::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(INoCopySpan*)this) {
        *pIID = EIID_INoCopySpan;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

BaseInputConnection::BaseInputConnection(
    /* [in] */ IInputMethodManager* mgr,
    /* [in] */ Boolean fullEditor):
    mIMM(mgr),
    mTargetView(NULL),
    mDummyMode(!fullEditor),
    mDefaultComposingSpans(NULL)
{

}

BaseInputConnection::BaseInputConnection(
    /* [in] */ IView* targetView,
    /* [in] */ Boolean fullEditor):
    mTargetView(targetView),
    mDummyMode(!fullEditor),
    mDefaultComposingSpans(NULL)
{
    AutoPtr<IContext> context = NULL;
    targetView->GetContext((IContext**) &context);
    assert(context != NULL);
    context->GetSystemService(Context_INPUT_METHOD_SERVICE, (IInterface**) &mIMM);
    assert(mIMM != NULL);
}

BaseInputConnection::~BaseInputConnection()
{
    if (mDefaultComposingSpans != NULL) {
        FreeArray(mDefaultComposingSpans);
    }
}

ECode BaseInputConnection::RemoveComposingSpans(
    /* [in] */ ISpannable* text) {
    assert(text != NULL);
    text->RemoveSpan(COMPOSING);

    Int32 len = 0;
    text->GetLength(&len);

    ArrayOf<IInterface*>* sps;
    text->GetSpans(0, len, EIID_IInterface, &sps);
    if (sps != NULL) {
        Int32 spanLength = sps->GetLength();

        Int32 flag = 0;
        for (Int32 i = spanLength - 1; i >= 0; i--) {
            AutoPtr<IInterface> o = (*sps)[i];

            text->GetSpanFlags(o, &flag);
            if ((flag & Spanned_SPAN_COMPOSING) != 0) {
                text->RemoveSpan(o);
            }
        }

        FreeArray(sps);
    }

    return NOERROR;
}

ECode BaseInputConnection::SetComposingSpans(
    /* [in] */ ISpannable* text) {
    assert(text != NULL);
    Int32 len = 0;
    text->GetLength(&len);

    return SetComposingSpans(text, 0, len);
}

ECode BaseInputConnection::SetComposingSpans(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end) {
    assert(text != NULL);
    ArrayOf<IInterface*>* sps;
    text->GetSpans(0, end, EIID_IInterface, &sps);

    if (sps != NULL) {
        Int32 spanLength = sps->GetLength();
        Int32 flag = 0;

        for (int i = spanLength - 1; i >= 0; i--) {
            AutoPtr<IInterface> o = (*sps)[i];
            if (o == COMPOSING) {
                text->RemoveSpan(o);
                continue;
            }

            text->GetSpanFlags(o, &flag);
            if ((flag & (Spanned_SPAN_COMPOSING | Spanned_SPAN_POINT_MARK_MASK))
                    != (Spanned_SPAN_COMPOSING | Spanned_SPAN_EXCLUSIVE_EXCLUSIVE)) {

                Int32 s = 0, e = 0;
                text->GetSpanStart(o, &s);
                text->GetSpanEnd(o, &e);

                text->SetSpan(o, s, e,
                        (flag & ~Spanned_SPAN_POINT_MARK_MASK)
                                | Spanned_SPAN_COMPOSING
                                | Spanned_SPAN_EXCLUSIVE_EXCLUSIVE);
            }
        }
    }

    return text->SetSpan(COMPOSING, start, end,
            Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);
}

ECode BaseInputConnection::GetComposingSpanStart(
    /* [in] */ ISpannable* text,
    /* [out] */ Int32* start) {
    assert(text != NULL);
    return text->GetSpanStart(COMPOSING, start);
}

ECode BaseInputConnection::GetComposingSpanEnd(
    /* [in] */ ISpannable* text,
    /* [out] */ Int32* end) {
    assert(text != NULL);
    return text->GetSpanEnd(COMPOSING, end);
}

ECode BaseInputConnection::GetEditable(
    /* [out] */ IEditable** editable) {
    if (mEditable == NULL) {
        AutoPtr<IEditableFactory> editableFactory;
        ASSERT_SUCCEEDED(CEditableFactory::AcquireSingleton(
            (IEditableFactory**)&editableFactory));

        AutoPtr<ICharSequence> tmpStr = NULL;
        CStringWrapper::New(String(""), (ICharSequence**) &tmpStr);

        editableFactory->NewEditable(tmpStr, (IEditable**)&mEditable);
        Selection::SetSelection(mEditable, 0);
    }

    *editable = mEditable.Get();
    if (*editable != NULL) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode BaseInputConnection::BeginBatchEdit(
    /* [out] */ Boolean* state) {
    *state = FALSE;
    return NOERROR;
}

ECode BaseInputConnection::EndBatchEdit(
    /* [out] */ Boolean* state) {
    *state = FALSE;
    return NOERROR;
}

ECode BaseInputConnection::ClearMetaKeyStates(
    /* [in] */ Int32 states,
    /* [out] */ Boolean* clear) {
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);

    *clear = FALSE;
    if (content == NULL) {
        return NOERROR;
    }

    *clear = TRUE;

    MetaKeyKeyListener::ClearMetaKeyState(content, states);
    return NOERROR;
}

ECode BaseInputConnection::CommitCompletion(
    /* [in] */ ICompletionInfo* text,
    /* [out] */ Boolean* state) {
    *state = FALSE;
    return NOERROR;
}

ECode BaseInputConnection::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* state) {
    // if (DEBUG) Log.v(TAG, "commitText " + text);
    ReplaceText(text, newCursorPosition, FALSE);
    SendCurrentText();

    *state = TRUE;
    return NOERROR;
}

ECode BaseInputConnection::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength,
    /* [out] */ Boolean* state) {
    // if (DEBUG) Log.v(TAG, "deleteSurroundingText " + leftLength
    //         + " / " + rightLength);
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);

    *state = FALSE;
    if (content == NULL) {
        return NOERROR;
    }

    Boolean tempFlag = FALSE;
    BeginBatchEdit(&tempFlag);

    Int32 a = Selection::GetSelectionStart(content);
    int b = Selection::GetSelectionEnd(content);

    if (a > b) {
        int tmp = a;
        a = b;
        b = tmp;
    }

    // ignore the composing text.
    Int32 ca = 0;
    GetComposingSpanStart(content, &ca);

    Int32 cb = 0;
    GetComposingSpanEnd(content, &cb);

    if (cb < ca) {
        Int32 tmp = ca;
        ca = cb;
        cb = tmp;
    }
    if (ca != -1 && cb != -1) {
        if (ca < a) a = ca;
        if (cb > b) b = cb;
    }

    Int32 deleted = 0;

    if (leftLength > 0) {
        Int32 start = a - leftLength;
        if (start < 0) start = 0;

        AutoPtr<IEditable> tmpEditable = NULL;
        content->Delete(start, a, (IEditable**) &tmpEditable);
        deleted = a - start;
    }

    if (rightLength > 0) {
        b = b - deleted;

        Int32 end = b + rightLength;
        Int32 len = 0;
        content->GetLength(&len);

        if (end > len) {
            end = len;
        }

        AutoPtr<IEditable> tmpEditable = NULL;
        content->Delete(b, end, (IEditable**) &tmpEditable);
    }

    EndBatchEdit(&tempFlag);

    *state = TRUE;
    return NOERROR;
}

ECode BaseInputConnection::FinishComposingText(
    /* [out] */ Boolean* state) {
    // if (DEBUG) Log.v(TAG, "finishComposingText");
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);

    if (content != NULL) {
        Boolean tempFlag = FALSE;
        BeginBatchEdit(&tempFlag);
        RemoveComposingSpans(content);

        EndBatchEdit(&tempFlag);
        SendCurrentText();
    }

    *state = TRUE;
    return NOERROR;
}

ECode BaseInputConnection::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [out] */ Int32* capsMode) {
    *capsMode = 0;
    if (mDummyMode) {
        return NOERROR;
    }

    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);
    if (content == NULL) {
        return NOERROR;
    }

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    *capsMode = TextUtils::GetCapsMode(content, a, reqModes);
    return NOERROR;
}

ECode BaseInputConnection::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [out] */ IExtractedText** text) {
    *text = NULL;
    return NOERROR;
}

ECode BaseInputConnection::GetTextBeforeCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text) {
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);
    if (content == NULL) {
        *text = NULL;
        return NOERROR;
    }

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    if (a <= 0) {
        return CStringWrapper::New(String(""), text);
    }

    if (length > a) {
        length = a;
    }

    if ((flags & InputConnection_GET_TEXT_WITH_STYLES) != 0) {
        return content->SubSequence(a - length, a, text);
    }

    String str;
    TextUtils::Substring(content, a - length, a, &str);

    return CStringWrapper::New(str, text);
}

ECode BaseInputConnection::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text) {
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);
    if (content == NULL) {
        *text = NULL;
        return NOERROR;
    }

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    if (a == b) {
        *text = NULL;
        return NOERROR;
    }

    if ((flags & InputConnection_GET_TEXT_WITH_STYLES) != 0) {
        return content->SubSequence(a, b, text);
    }

    String str;
    TextUtils::Substring(content, a, b, &str);

    return CStringWrapper::New(str, text);
}

ECode BaseInputConnection::GetTextAfterCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text) {
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);
    if (content == NULL) {
        *text = NULL;
        return NOERROR;
    }

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    // Guard against the case where the cursor has not been positioned yet.
    if (b < 0) {
        b = 0;
    }

    Int32 len = 0;
    content->GetLength(&len);
    if (b + length > len) {
        length = len - b;
    }

    if ((flags & InputConnection_GET_TEXT_WITH_STYLES) != 0) {
        return content->SubSequence(b, b + length, text);
    }

    String str;
    TextUtils::Substring(content, b, b + length, &str);

    return CStringWrapper::New(str, text);
}

ECode BaseInputConnection::PerformEditorAction(
    /* [in] */ Int32 actionCode,
    /* [out] */ Boolean* state) {
    Int64 eventTime = SystemClock::UptimeMillis();

    AutoPtr<IKeyEvent> event;
    Boolean tempFlag = FALSE;
    CKeyEvent::New(eventTime, eventTime,
            KeyEvent_ACTION_DOWN, KeyEvent_KEYCODE_ENTER, 0, 0, 0, 0,
            KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE
            | KeyEvent_FLAG_EDITOR_ACTION, (IKeyEvent**) &event);

    // SendKeyEvent(new KeyEvent(eventTime, eventTime,
    //         KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_ENTER, 0, 0, 0, 0,
    //         KeyEvent.FLAG_SOFT_KEYBOARD | KeyEvent.FLAG_KEEP_TOUCH_MODE
    //         | KeyEvent.FLAG_EDITOR_ACTION));

    SendKeyEvent(event, &tempFlag);

    // SendKeyEvent(new KeyEvent(SystemClock.uptimeMillis(), eventTime,
    //         KeyEvent.ACTION_UP, KeyEvent.KEYCODE_ENTER, 0, 0, 0, 0,
    //         KeyEvent.FLAG_SOFT_KEYBOARD | KeyEvent.FLAG_KEEP_TOUCH_MODE
    //         | KeyEvent.FLAG_EDITOR_ACTION));
    CKeyEvent::New(SystemClock::UptimeMillis(), eventTime,
                KeyEvent_ACTION_UP, KeyEvent_KEYCODE_ENTER, 0, 0, 0, 0,
                KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE
                | KeyEvent_FLAG_EDITOR_ACTION, (IKeyEvent**) &event);

    SendKeyEvent(event, &tempFlag);

    *state = TRUE;
    return NOERROR;
}

ECode BaseInputConnection::PerformContextMenuAction(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* state) {
    *state = FALSE;
    return NOERROR;
}

ECode BaseInputConnection::PerformPrivateCommand(
    /* [in] */ String action,
    /* [in] */ IBundle* data,
    /* [out] */ Boolean* state) {
    *state = FALSE;
    return NOERROR;
}

ECode BaseInputConnection::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* state) {
    // if (DEBUG) Log.v(TAG, "setComposingText " + text);
    ReplaceText(text, newCursorPosition, TRUE);

    *state = TRUE;
    return NOERROR;
}

ECode BaseInputConnection::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* state) {
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);

    if (content != NULL) {
        Boolean tempFlag = FALSE;
        BeginBatchEdit(&tempFlag);
        RemoveComposingSpans(content);

        Int32 a = start;
        Int32 b = end;
        if (a > b) {
            Int32 tmp = a;
            a = b;
            b = tmp;
        }

        // Clip the end points to be within the content bounds.
        Int32 length = 0;
        content->GetLength(&length);

        if (a < 0) a = 0;
        if (b < 0) b = 0;
        if (a > length) a = length;
        if (b > length) b = length;

        EnsureDefaultComposingSpans();
        if (mDefaultComposingSpans != NULL) {
            Int32 tmpLen = 0;
            if (mDefaultComposingSpans != NULL) {
                tmpLen = mDefaultComposingSpans->GetLength();
            }

            for (Int32 i = 0; i < tmpLen; ++i) {
                content->SetSpan((*mDefaultComposingSpans)[i], a, b,
                        Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);
            }
        }

        content->SetSpan(COMPOSING, a, b,
                Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);

        EndBatchEdit(&tempFlag);
        SendCurrentText();
    }

    *state = TRUE;
    return NOERROR;
}

ECode BaseInputConnection::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* state) {
    // if (DEBUG) Log.v(TAG, "setSelection " + start + ", " + end);
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);

    if (content == NULL) {
        *state = FALSE;
        return NOERROR;
    }

    int len = 0;
    content->GetLength(&len);
    if (start > len || end > len) {
        // If the given selection is out of bounds, just ignore it.
        // Most likely the text was changed out from under the IME,
        // the the IME is going to have to update all of its state
        // anyway.
        *state = TRUE;
        return NOERROR;
    }
    if (start == end && MetaKeyKeyListener::GetMetaState(content,
            MetaKeyKeyListener::META_SELECTING) != 0) {
        // If we are in selection mode, then we want to extend the
        // selection instead of replacing it.
        Selection::ExtendSelection(content, start);
    } else {
        Selection::SetSelection(content, start, end);
    }

    *state = TRUE;
    return NOERROR;
}

ECode BaseInputConnection::SendKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state) {
    assert(0);

    //TODO
    // synchronized (mIMM.mH) {
    //     Handler h = mTargetView != null ? mTargetView.getHandler() : null;
    //     if (h == null) {
    //         if (mIMM.mServedView != null) {
    //             h = mIMM.mServedView.getHandler();
    //         }
    //     }
    //     if (h != null) {
    //         h.sendMessage(h.obtainMessage(ViewRoot.DISPATCH_KEY_FROM_IME,
    //                 event));
    //     }
    // }

    *state = FALSE;
    return NOERROR;
}

ECode BaseInputConnection::ReportFullscreenMode(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* state) {
    mIMM->SetFullscreenMode(enabled);
    *state = TRUE;
    return NOERROR;
}

void BaseInputConnection::SendCurrentText() {
    if (!mDummyMode) {
        return;
    }

    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);

    if (content != NULL) {
        Int32 N = 0;
        content->GetLength(&N);

        if (N == 0) {
            return;
        }
        if (N == 1) {
            // If it's 1 character, we have a chance of being
            // able to generate normal key events...
            if (mKeyCharacterMap == NULL) {
                mKeyCharacterMap = ElKeyCharacterMap::Load(ElKeyCharacterMap::BUILT_IN_KEYBOARD);
            }

            ArrayOf<Char8>* chars = ArrayOf<Char8>::Alloc(ArrayUtils::IdealChar8ArraySize(2 * 1));
            content->GetChars(0, 1, chars, 0);

            Char16* tmpChars = new Char16[1];
            tmpChars[0] = (*chars)[0];
            ArrayOf<Char8>::Free(chars);

            Elastos::Vector<AutoPtr<IKeyEvent> >* events = NULL;
            mKeyCharacterMap->GetEvents(tmpChars, 1, &events);
            delete []tmpChars;

            if (events != NULL) {
                Boolean tmpState = FALSE;
                for (int i=0; i < (Int32)events->GetSize(); i++) {
                    // if (DEBUG) Log.v(TAG, "Sending: " + events[i]);
                    SendKeyEvent((*events)[i], &tmpState);
                }

                content->Clear();

                return;
            }
        }

        // Otherwise, revert to the special key event containing
        // the actual characters.
        String contentStr;
        content->ToString(&contentStr);
        AutoPtr<IKeyEvent> event = NULL;
        CKeyEvent::New(SystemClock::UptimeMillis(), contentStr,
            ElKeyCharacterMap::BUILT_IN_KEYBOARD, 0, (IKeyEvent**) &event);

/*        KeyEvent event = new KeyEvent(SystemClock.uptimeMillis(),
                content.toString(), KeyCharacterMap.BUILT_IN_KEYBOARD, 0);*/

        Boolean tmpState = FALSE;
        SendKeyEvent(event, &tmpState);
        content->Clear();
    }
}

void BaseInputConnection::EnsureDefaultComposingSpans() {
    if (mDefaultComposingSpans == NULL) {
        AutoPtr<IContext> context;
        if (mTargetView != NULL) {
            mTargetView->GetContext((IContext**) &context);
        } else if (mIMM->mServedView != NULL) {
            mIMM->mServedView->GetContext((IContext**) &context);
        } else {
            context = NULL;
        }

        if (context != NULL) {
            AutoPtr<ITheme> theme;
            context->GetTheme((ITheme**) &theme);

            AutoPtr<ITypedArray> ta;
            ArrayOf<Int32>* attrs = ArrayOf<Int32>::Alloc(1);
            (*attrs)[0] = 0x01010230 /*com.android.internal.R.attr.candidatesTextStyleSpans*/;

            theme->ObtainStyledAttributes(*attrs, (ITypedArray**) &ta);
            ArrayOf<Int32>::Free(attrs);

            // TypedArray ta = context.getTheme()
            //         .obtainStyledAttributes(new int[] {
            //                 com.android.internal.R.attr.candidatesTextStyleSpans
            //         });
            AutoPtr<ICharSequence> style = NULL;
            ta->GetText(0, (ICharSequence**) &style);
            ta->Recycle();

            if (style != NULL && ISpanned::Probe(style) != NULL) {
                Int32 len = 0;
                style->GetLength(&len);
                ISpanned::Probe(style)->GetSpans(0, len, EIID_IInterface, &mDefaultComposingSpans);
                // mDefaultComposingSpans = ((Spanned)style).getSpans(
                //         0, style.length(), Object.class);
            }
        }
    }
}

void BaseInputConnection::ReplaceText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [in] */ Boolean composing){
    AutoPtr<IEditable> content = NULL;
    GetEditable((IEditable**) &content);
    if (content == NULL) {
        return;
    }

    Boolean tmpState = FALSE;
    BeginBatchEdit(&tmpState);

    // delete composing text set previously.
    int a = 0;
    GetComposingSpanStart(content, &a);

    int b = 0;
    GetComposingSpanEnd(content, &b);

    // if (DEBUG) Log.v(TAG, "Composing span: " + a + " to " + b);

    if (b < a) {
        int tmp = a;
        a = b;
        b = tmp;
    }

    if (a != -1 && b != -1) {
        RemoveComposingSpans(content);
    } else {
        a = Selection::GetSelectionStart(content);
        b = Selection::GetSelectionEnd(content);
        if (a < 0) a = 0;
        if (b < 0) b = 0;
        if (b < a) {
            int tmp = a;
            a = b;
            b = tmp;
        }
    }

    if (composing) {
        // assert(text != NULL)
        AutoPtr<ISpannable> sp = NULL;
        // if (!(text instanceof Spannable)) {
        if (ISpannable::Probe(text) == NULL) {
            CSpannableStringBuilder::New(text, (ISpannableStringBuilder**) &sp);

            text = sp;
            EnsureDefaultComposingSpans();
            if (mDefaultComposingSpans != NULL) {
                Int32 tmpLen = 0;
                tmpLen = mDefaultComposingSpans->GetLength();

                Int32 len2 = 0;
                for (Int32 i = 0; i < tmpLen; ++i) {
                    sp->GetLength(&len2);
                    sp->SetSpan((*mDefaultComposingSpans)[i], 0, len2,
                            Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);
                }
            }
        } else {
            sp = (ISpannable*)text;
        }

        SetComposingSpans(sp);
    }

    // if (DEBUG) Log.v(TAG, "Replacing from " + a + " to " + b + " with \""
    //         + text + "\", composing=" + composing
    //         + ", type=" + text.getClass().getCanonicalName());

    // if (DEBUG) {
    //     LogPrinter lp = new LogPrinter(Log.VERBOSE, TAG);
    //     lp.println("Current text:");
    //     TextUtils.dumpSpans(content, lp, "  ");
    //     lp.println("Composing text:");
    //     TextUtils.dumpSpans(text, lp, "  ");
    // }

    // Position the cursor appropriately, so that after replacing the
    // desired range of text it will be located in the correct spot.
    // This allows us to deal with filters performing edits on the text
    // we are providing here.
    if (newCursorPosition > 0) {
        newCursorPosition += b - 1;
    } else {
        newCursorPosition += a;
    }

    if (newCursorPosition < 0) {
        newCursorPosition = 0;
    }

    Int32 len = 0;
    content->GetLength(&len);

    if (newCursorPosition > len) {
        newCursorPosition = len;
    }

    Selection::SetSelection(content, newCursorPosition);

    AutoPtr<IEditable> tmpObj = NULL;
    content->ReplaceEx(a, b, text, (IEditable**) &tmpObj);

    // if (DEBUG) {
    //     LogPrinter lp = new LogPrinter(Log.VERBOSE, TAG);
    //     lp.println("Final text:");
    //     TextUtils.dumpSpans(content, lp, "  ");
    // }

    EndBatchEdit(&tmpState);
}
