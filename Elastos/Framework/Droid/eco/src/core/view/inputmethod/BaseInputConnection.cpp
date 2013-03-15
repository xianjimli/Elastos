
#include "view/inputmethod/BaseInputConnection.h"
#include "view/CKeyEvent.h"
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "view/ViewRoot.h"
#include "text/Selection.h"
#include "text/CEditableFactory.h"
#include "text/method/MetaKeyKeyListener.h"
#include "text/TextUtils.h"
#include "text/CSpannableStringBuilder.h"
#include "os/SystemClock.h"
#include "utils/ArrayUtils.h"


BaseInputConnection::ComposingText::ComposingText()
{}

PInterface BaseInputConnection::ComposingText::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_INoCopySpan == riid) {
        return (INoCopySpan *)this;
    }

    return NULL;
}

UInt32 BaseInputConnection::ComposingText::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BaseInputConnection::ComposingText::Release()
{
    return ElRefBase::Release();
}

ECode BaseInputConnection::ComposingText::GetInterfaceID(
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


const Boolean BaseInputConnection::DEBUG;
const CString BaseInputConnection::TAG = "BaseInputConnection";
const AutoPtr<IInterface> BaseInputConnection::COMPOSING = new BaseInputConnection::ComposingText();

BaseInputConnection::BaseInputConnection()
    : mDefaultComposingSpans(NULL)
{}

BaseInputConnection::BaseInputConnection(
    /* [in] */ ILocalInputMethodManager* mgr,
    /* [in] */ Boolean fullEditor)
    : mIMM(mgr)
    , mDummyMode(!fullEditor)
    , mDefaultComposingSpans(NULL)
{}

BaseInputConnection::BaseInputConnection(
    /* [in] */ IView* targetView,
    /* [in] */ Boolean fullEditor)
    : mTargetView(targetView)
    , mDummyMode(!fullEditor)
    , mDefaultComposingSpans(NULL)
{
    AutoPtr<IContext> context;
    targetView->GetContext((IContext**)&context);
    assert(context != NULL);
    context->GetSystemService(Context_INPUT_METHOD_SERVICE, (IInterface**)&mIMM);
    assert(mIMM != NULL);
}

BaseInputConnection::~BaseInputConnection()
{
    if (mDefaultComposingSpans != NULL) {
        for (Int32 i = 0; i < mDefaultComposingSpans->GetLength(); i++) {
            (*mDefaultComposingSpans)[i] = NULL;
        }
        ArrayOf< AutoPtr<IInterface> >::Free(mDefaultComposingSpans);
        mDefaultComposingSpans = NULL;
    }
}

ECode BaseInputConnection::RemoveComposingSpans(
    /* [in] */ ISpannable* text)
{
    assert(text != NULL);
    text->RemoveSpan(COMPOSING);
    Int32 len = 0;
    text->GetLength(&len);
    ArrayOf< AutoPtr<IInterface> >* sps = NULL;
    text->GetSpans(0, len, EIID_IInterface, (ArrayOf<IInterface*>**)&sps);
    if (sps != NULL) {
        for (Int32 i = sps->GetLength() - 1; i >= 0; i--) {
            IInterface* o = (*sps)[i];
            Int32 flag = 0;
            text->GetSpanFlags(o, &flag);
            if ((flag & Spanned_SPAN_COMPOSING) != 0) {
                text->RemoveSpan(o);
            }
        }

        for (Int32 i = 0; i < sps->GetLength(); i++) {
            (*sps)[i] = NULL;
        }
        ArrayOf< AutoPtr<IInterface> >::Free(sps);
    }
    return NOERROR;
}

ECode BaseInputConnection::SetComposingSpans(
    /* [in] */ ISpannable* text)
{
    assert(text != NULL);
    Int32 len = 0;
    text->GetLength(&len);
    return SetComposingSpans(text, 0, len);
}

ECode BaseInputConnection::SetComposingSpans(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    assert(text != NULL);
    ArrayOf< AutoPtr<IInterface> >* sps = NULL;
    text->GetSpans(0, end, EIID_IInterface, (ArrayOf<IInterface*>**)&sps);
    if (sps != NULL) {
        for (int i = sps->GetLength() - 1; i >= 0; i--) {
            IInterface* o = (*sps)[i];
            if (o == COMPOSING.Get()) {
                text->RemoveSpan(o);
                continue;
            }

            Int32 flag = 0;
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

        for (Int32 i = 0; i < sps->GetLength(); i++) {
            (*sps)[i] != NULL;
        }
        ArrayOf< AutoPtr<IInterface> >::Free(sps);
    }

    return text->SetSpan(COMPOSING, start, end,
            Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);
}

Int32 BaseInputConnection::GetComposingSpanStart(
    /* [in] */ ISpannable* text)
{
    assert(text != NULL);
    Int32 start;
    ASSERT_SUCCEEDED(text->GetSpanStart(COMPOSING, &start));
    return start;
}

Int32 BaseInputConnection::GetComposingSpanEnd(
    /* [in] */ ISpannable* text)
{
    assert(text != NULL);
    Int32 end;
    ASSERT_SUCCEEDED(text->GetSpanEnd(COMPOSING, &end));
    return end;
}

AutoPtr<IEditable> BaseInputConnection::GetEditable()
{
    if (mEditable == NULL) {
        AutoPtr<IEditableFactory> editableFactory;
        ASSERT_SUCCEEDED(CEditableFactory::AcquireSingleton(
            (IEditableFactory**)&editableFactory));
        AutoPtr<ICharSequence> tmpStr;
        CStringWrapper::New(String(""), (ICharSequence**)&tmpStr);
        editableFactory->NewEditable(tmpStr, (IEditable**)&mEditable);
        Selection::SetSelection(mEditable, 0);
    }
    return mEditable;
}

Boolean BaseInputConnection::BeginBatchEdit()
{
    return FALSE;
}

Boolean BaseInputConnection::EndBatchEdit()
{
    return FALSE;
}

Boolean BaseInputConnection::ClearMetaKeyStates(
    /* [in] */ Int32 states)
{
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return FALSE;
    MetaKeyKeyListener::ClearMetaKeyState(content, states);
    return TRUE;
}

Boolean BaseInputConnection::CommitCompletion(
    /* [in] */ ICompletionInfo* text)
{
    return FALSE;
}

Boolean BaseInputConnection::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    // if (DEBUG) Log.v(TAG, "commitText " + text);
    ReplaceText(text, newCursorPosition, FALSE);
    SendCurrentText();
    return TRUE;
}

Boolean BaseInputConnection::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength)
{
    // if (DEBUG) Log.v(TAG, "deleteSurroundingText " + leftLength
    //         + " / " + rightLength);
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return FALSE;

    BeginBatchEdit();

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    // ignore the composing text.
    Int32 ca = GetComposingSpanStart(content);
    Int32 cb = GetComposingSpanEnd(content);
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
        AutoPtr<IEditable> tmpEditable;
        content->Delete(start, a, (IEditable**)&tmpEditable);
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

        AutoPtr<IEditable> tmpEditable;
        content->Delete(b, end, (IEditable**)&tmpEditable);
    }

    EndBatchEdit();

    return TRUE;
}

Boolean BaseInputConnection::FinishComposingText()
{
    // if (DEBUG) Log.v(TAG, "finishComposingText");
    AutoPtr<IEditable> content = GetEditable();
    if (content != NULL) {
        BeginBatchEdit();
        RemoveComposingSpans(content);
        EndBatchEdit();
        SendCurrentText();
    }
    return TRUE;
}

Int32 BaseInputConnection::GetCursorCapsMode(
    /* [in] */ Int32 reqModes)
{
    if (mDummyMode) return 0;

    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return 0;

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    return TextUtils::GetCapsMode(content, a, reqModes);
}

AutoPtr<IExtractedText> BaseInputConnection::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags)
{
    return NULL;
}

AutoPtr<ICharSequence> BaseInputConnection::GetTextBeforeCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags)
{
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return NULL;

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    if (a <= 0) {
        AutoPtr<ICharSequence> text;
        CStringWrapper::New(String(""), (ICharSequence**)&text);
        return text;
    }

    if (length > a) {
        length = a;
    }

    if ((flags & InputConnection_GET_TEXT_WITH_STYLES) != 0) {
        AutoPtr<ICharSequence> text;
        content->SubSequence(a - length, a, (ICharSequence**)&text);
        return text;
    }

    String str;
    TextUtils::Substring(content, a - length, a, &str);
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(str, (ICharSequence**)&text);
    return text;
}

AutoPtr<ICharSequence> BaseInputConnection::GetSelectedText(
    /* [in] */ Int32 flags)
{
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return NULL;

    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    if (a == b) return NULL;

    if ((flags & InputConnection_GET_TEXT_WITH_STYLES) != 0) {
        AutoPtr<ICharSequence> text;
        content->SubSequence(a, b, (ICharSequence**)&text);
        return text;
    }

    String str;
    TextUtils::Substring(content, a, b, &str);
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(str, (ICharSequence**)&text);
    return text;
}

AutoPtr<ICharSequence> BaseInputConnection::GetTextAfterCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags)
{
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return NULL;

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
        AutoPtr<ICharSequence> text;
        content->SubSequence(b, b + length, (ICharSequence**)&text);
        return text;
    }

    String str;
    TextUtils::Substring(content, b, b + length, &str);
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(str, (ICharSequence**)&text);
    return text;
}

Boolean BaseInputConnection::PerformEditorAction(
    /* [in] */ Int32 actionCode)
{
    Int64 eventTime = SystemClock::GetUptimeMillis();

    AutoPtr<IKeyEvent> event;
    ASSERT_SUCCEEDED(CKeyEvent::New(eventTime, eventTime,
            KeyEvent_ACTION_DOWN, KeyEvent_KEYCODE_ENTER, 0, 0, 0, 0,
            KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE
            | KeyEvent_FLAG_EDITOR_ACTION, (IKeyEvent**)&event));
    SendKeyEvent(event);

    event = NULL;
    ASSERT_SUCCEEDED(CKeyEvent::New(SystemClock::GetUptimeMillis(), eventTime,
                KeyEvent_ACTION_UP, KeyEvent_KEYCODE_ENTER, 0, 0, 0, 0,
                KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE
                | KeyEvent_FLAG_EDITOR_ACTION, (IKeyEvent**)&event));
    SendKeyEvent(event);
    return TRUE;
}

Boolean BaseInputConnection::PerformContextMenuAction(
    /* [in] */ Int32 id)
{
    return FALSE;
}

Boolean BaseInputConnection::PerformPrivateCommand(
    /* [in] */ String action,
    /* [in] */ IBundle* data)
{
    return FALSE;
}

Boolean BaseInputConnection::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    // if (DEBUG) Log.v(TAG, "setComposingText " + text);
    ReplaceText(text, newCursorPosition, TRUE);
    return TRUE;
}

Boolean BaseInputConnection::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IEditable> content = GetEditable();
    if (content != NULL) {
        BeginBatchEdit();
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
            for (Int32 i = 0; i < mDefaultComposingSpans->GetLength(); ++i) {
                content->SetSpan((*mDefaultComposingSpans)[i], a, b,
                        Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);
            }
        }

        content->SetSpan(COMPOSING, a, b,
                Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);

        EndBatchEdit();
        SendCurrentText();
    }
    return TRUE;
}

Boolean BaseInputConnection::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // if (DEBUG) Log.v(TAG, "setSelection " + start + ", " + end);
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) return FALSE;
    int len = 0;
    content->GetLength(&len);
    if (start > len || end > len) {
        // If the given selection is out of bounds, just ignore it.
        // Most likely the text was changed out from under the IME,
        // the the IME is going to have to update all of its state
        // anyway.
        return TRUE;
    }
    if (start == end && MetaKeyKeyListener::GetMetaState(content,
            MetaKeyKeyListener::META_SELECTING) != 0) {
        // If we are in selection mode, then we want to extend the
        // selection instead of replacing it.
        Selection::ExtendSelection(content, start);
    }
    else {
        Selection::SetSelection(content, start, end);
    }
    return TRUE;
}

Boolean BaseInputConnection::SendKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    {
        Mutex::Autolock lock(((CLocalInputMethodManager*)mIMM.Get())->sStaticHandlerLock);

        AutoPtr<IApartment> h;
        if (mTargetView != NULL) {
            mTargetView->GetHandler((IApartment**)&h);
        }
        if (h == NULL) {
            if (((CLocalInputMethodManager*)mIMM.Get())->mServedView != NULL) {
                ((CLocalInputMethodManager*)mIMM.Get())->mServedView->GetHandler((IApartment**)&h);
            }
        }

        if (h != NULL) {
            // Todo: the following implement is wrong
            // h.sendMessage(h.obtainMessage(ViewRoot.DISPATCH_KEY_FROM_IME,
            //         event));
            void (STDCALL ViewRoot::*pHandlerFunc)(IKeyEvent*);
            pHandlerFunc = &ViewRoot::DispatchKeyFromIme;

            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr((IInterface*)event);
            h->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc,
                    params, 0, 0);
        }
    }
    return FALSE;
}

Boolean BaseInputConnection::ReportFullscreenMode(
    /* [in] */ Boolean enabled)
{
    mIMM->SetFullscreenMode(enabled);
    return TRUE;
}

void BaseInputConnection::SendCurrentText()
{
    if (!mDummyMode) {
        return;
    }

    AutoPtr<IEditable> content = GetEditable();
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
            ArrayOf_<Char8, 4> chars;
            assert(IGetChars::Probe(content) != NULL);
            IGetChars::Probe(content)->GetChars(0, 1, &chars, 0);
            //todo: error
            Char16 tmpChars[1];
            tmpChars[0] = chars[0];
            Vector< AutoPtr<IKeyEvent> >* events = NULL;
            mKeyCharacterMap->GetEvents(tmpChars, 1, &events);
            if (events != NULL) {
                Vector< AutoPtr<IKeyEvent> >::Iterator it;
                for (it = events->Begin(); it != events->End(); ++it) {
                    SendKeyEvent(*it);
                }
                content->Clear();
                delete events;
                return;
            }
        }

        // Otherwise, revert to the special key event containing
        // the actual characters.
        String contentStr;
        content->ToString(&contentStr);
        AutoPtr<IKeyEvent> event;
        CKeyEvent::New(SystemClock::GetUptimeMillis(), contentStr,
            ElKeyCharacterMap::BUILT_IN_KEYBOARD, 0, (IKeyEvent**)&event);
        SendKeyEvent(event);
        content->Clear();
    }
}

void BaseInputConnection::EnsureDefaultComposingSpans()
{
    if (mDefaultComposingSpans == NULL) {
        AutoPtr<IContext> context;
        if (mTargetView != NULL) {
            mTargetView->GetContext((IContext**)&context);
        }
        else if (((CLocalInputMethodManager*)mIMM.Get())->mServedView != NULL) {
            ((CLocalInputMethodManager*)mIMM.Get())->mServedView->GetContext((IContext**)&context);
        }
        if (context != NULL) {
            AutoPtr<ITheme> theme;
            context->GetTheme((ITheme**)&theme);
            ArrayOf_<Int32, 1> attrs;
            attrs[0] = 0x01010230 /*com.android.internal.R.attr.candidatesTextStyleSpans*/;
            AutoPtr<ITypedArray> ta;
            theme->ObtainStyledAttributes(attrs, (ITypedArray**)&ta);
            AutoPtr<ICharSequence> style;
            ta->GetText(0, (ICharSequence**)&style);
            ta->Recycle();
            if (style != NULL && ISpanned::Probe(style) != NULL) {
                Int32 len = 0;
                style->GetLength(&len);
                ISpanned::Probe(style)->GetSpans(0, len, EIID_IInterface, (ArrayOf<IInterface*>**)&mDefaultComposingSpans);
            }
        }
    }
}

void BaseInputConnection::ReplaceText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [in] */ Boolean composing)
{
    AutoPtr<IEditable> content = GetEditable();
    if (content == NULL) {
        return;
    }

    BeginBatchEdit();

    // delete composing text set previously.
    Int32 a = GetComposingSpanStart(content);
    Int32 b = GetComposingSpanEnd(content);

    // if (DEBUG) Log.v(TAG, "Composing span: " + a + " to " + b);

    if (b < a) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    if (a != -1 && b != -1) {
        RemoveComposingSpans(content);
    }
    else {
        a = Selection::GetSelectionStart(content);
        b = Selection::GetSelectionEnd(content);
        if (a < 0) a = 0;
        if (b < 0) b = 0;
        if (b < a) {
            Int32 tmp = a;
            a = b;
            b = tmp;
        }
    }

    if (composing) {
        assert(text != NULL);
        AutoPtr<ISpannable> sp;
        if (ISpannable::Probe(text) == NULL) {
            CSpannableStringBuilder::New(text, (ISpannableStringBuilder**)&sp);
            text = sp;
            EnsureDefaultComposingSpans();
            if (mDefaultComposingSpans != NULL) {
                for (Int32 i = 0; i < mDefaultComposingSpans->GetLength(); ++i) {
                    Int32 len2 = 0;
                    sp->GetLength(&len2);
                    sp->SetSpan((*mDefaultComposingSpans)[i], 0, len2,
                            Spanned_SPAN_EXCLUSIVE_EXCLUSIVE | Spanned_SPAN_COMPOSING);
                }
            }
        }
        else {
            sp = ISpannable::Probe(text);
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
    }
    else {
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

    AutoPtr<IEditable> tmpObj;
    content->ReplaceEx(a, b, text, (IEditable**)&tmpObj);

    // if (DEBUG) {
    //     LogPrinter lp = new LogPrinter(Log.VERBOSE, TAG);
    //     lp.println("Final text:");
    //     TextUtils.dumpSpans(content, lp, "  ");
    // }

    EndBatchEdit();
}

ECode BaseInputConnection::Init(
    /* [in] */ ILocalInputMethodManager* mgr,
    /* [in] */ Boolean fullEditor)
{
    mIMM = mgr;
    mDummyMode = !fullEditor;
    return NOERROR;
}

ECode BaseInputConnection::Init(
    /* [in] */ IView* targetView,
    /* [in] */ Boolean fullEditor)
{
    mTargetView = targetView;
    mDummyMode = !fullEditor;

    AutoPtr<IContext> context;
    targetView->GetContext((IContext**)&context);
    assert(context != NULL);
    context->GetSystemService(Context_INPUT_METHOD_SERVICE, (IInterface**)&mIMM);
    assert(mIMM != NULL);
    return NOERROR;
}
