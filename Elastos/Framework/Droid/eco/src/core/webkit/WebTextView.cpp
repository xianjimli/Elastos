
#include "webkit/WebTextView.h"

#include "../../../res/gen/R.h"
#include "webkit/CWebView.h"
#include "view/CKeyEvent.h"
#include <Logger.h>
#include "webkit/DebugFlags.h"
#include "view/inputmethod/CEditorInfo.h"
#include "text/TextUtils.h"
#include <elastos/Math.h>
#include "widget/CAbsoluteLayoutLayoutParams.h"
#include "../../../res/gen/R.h"
#include "graphics/CPaint.h"
#include "graphics/ElPixelFormat.h"
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "widget/CEditableInputConnection.h"

const CString WebTextView::LOGTAG = "webtextview";

/******************************AutoCompleteAdapter*************************/
WebTextView::AutoCompleteAdapter::AutoCompleteAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* entries):ArrayAdapter(context, R::layout::search_dropdown_item_1line, entries)
{    
}

/**
 * {@inheritDoc}
 */
AutoPtr<IView> WebTextView::AutoCompleteAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<ITextView> tv = (ITextView*) (ArrayAdapter::GetView(position, convertView, parent)).Get();
    if (tv.Get() != NULL && mTextView != NULL) {
        float fTextSize = 0;
        mTextView -> GetTextSize(&fTextSize);
        tv -> SetTextSize(fTextSize);
    }    
    return tv;
}

/**
 * Set the TextView so we can match its text size.
 */
void WebTextView::AutoCompleteAdapter::SetTextView(
    /* [in] */ ITextView* tv)
{
    mTextView = tv;
}

/*******************************OutlineDrawable*********************************/
/*void*/ECode WebTextView::OutlineDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IRect> bounds = GetBounds();
    AutoPtr<IPaint> paint;
    CPaint::New( (IPaint**)&paint );
    paint -> SetAntiAlias(TRUE);
    // Draw the background.
    paint -> SetColor(Color_WHITE);
    canvas -> DrawRectEx(bounds.Get(), paint.Get());
    // Draw the outline.
    paint -> SetStyle(PaintStyle_STROKE);
    paint -> SetColor(Color_BLACK);
    canvas -> DrawRectEx(bounds.Get(), paint.Get());
}

// Always want it to be opaque.
CARAPI_(Int32) WebTextView::OutlineDrawable::GetOpacity()
{
    return ElPixelFormat::OPAQUE;
}

// These are needed because they are abstract in Drawable.
/*void*/ECode WebTextView::OutlineDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{}

/*void*/ECode WebTextView::OutlineDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{}

CARAPI_(PInterface) WebTextView::OutlineDrawable::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

/*******************************WebTextView*********************************/

/**
 * Create a new WebTextView.
 * @param   context The Context for this WebTextView.
 * @param   webView The WebView that created this.
 */
/* package */
WebTextView::WebTextView(
	/* [in] */ IContext* context,
	/* [in] */ IWebView* webView): AutoCompleteTextView(context,NULL,R::attr::webTextViewStyle)
{
    mWebView = webView;
    mMaxLength = -1;
}

//@Override
Boolean WebTextView::DispatchKeyEvent(
	/* [in] */ IKeyEvent* event)
{
	assert(event != NULL);

	Boolean bSystem = FALSE;
	event->IsSystem(&bSystem);
	if (bSystem) {
        return AutoCompleteTextView::DispatchKeyEvent(event);
    }
    // Treat ACTION_DOWN and ACTION MULTIPLE the same
    Int32 action;
    event->GetAction(&action);
    Boolean down = (action != KeyEvent_ACTION_UP);
    Int32 keyCode = FALSE;
    event->GetKeyCode(&keyCode);

    Boolean isArrowKey = FALSE;
    switch(keyCode) {
        case KeyEvent_KEYCODE_DPAD_LEFT:
        case KeyEvent_KEYCODE_DPAD_RIGHT:
        case KeyEvent_KEYCODE_DPAD_UP:
        case KeyEvent_KEYCODE_DPAD_DOWN:
            if (!((CWebView*)mWebView.Get())->NativeCursorMatchesFocus()) {
                return down ? ((CWebView*)mWebView.Get())->OnKeyDown(keyCode, event) : ((CWebView*)mWebView.Get())->OnKeyUp(keyCode, event);
            }
            isArrowKey = TRUE;
            break;
    }

    if (KeyEvent_KEYCODE_TAB == keyCode) {
        if (down) {
            OnEditorAction(EditorInfo_IME_ACTION_NEXT);
        }
        return TRUE;
    }
    AutoPtr<ISpannable> text = (ISpannable*)( (GetText().Get()) -> Probe(EIID_ISpannable) );
    Int32 oldLength = 0;
    text -> GetLength(&oldLength);
    // Normally the delete key's dom events are sent via onTextChanged.
    // However, if the length is zero, the text did not change, so we
    // go ahead and pass the key down immediately.
    if (KeyEvent_KEYCODE_DEL == keyCode && 0 == oldLength) {
        SendDomEvent(event);
        return TRUE;
    }

    if ((mSingle && KeyEvent_KEYCODE_ENTER == keyCode)) {
        if (IsPopupShowing()) {
            return AutoCompleteTextView::DispatchKeyEvent(event);
        }
        if (!down) {
            // Hide the keyboard, since the user has just submitted this
            // form.  The submission happens thanks to the two calls
            // to sendDomEvent.
            
            AutoPtr<ILocalInputMethodManager> iInputMethodManagerT;            
            iInputMethodManagerT = CLocalInputMethodManager::GetInstance(mContext);
            Boolean bHideSoftInputFromWindow;
            iInputMethodManagerT -> HideSoftInputFromWindow( (GetWindowToken().Get()), 0,&bHideSoftInputFromWindow);
            
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(KeyEvent_ACTION_DOWN,keyCode,(IKeyEvent**)&keyEvent);
            SendDomEvent(keyEvent.Get());
            SendDomEvent(event);
        }
        return AutoCompleteTextView::DispatchKeyEvent(event);
    } 
    else if (KeyEvent_KEYCODE_DPAD_CENTER == keyCode) {
        // Note that this handles center key and trackball.
        if (IsPopupShowing()) {
            return AutoCompleteTextView::DispatchKeyEvent(event);
        }
        if ( !( ((CWebView*)(mWebView.Get())) -> NativeCursorMatchesFocus() ) ) {
            return down ? ((CWebView*)mWebView.Get())->OnKeyDown(keyCode, event) : ((CWebView*)mWebView.Get())->OnKeyUp(keyCode, event);
        }
        // Center key should be passed to a potential onClick
        if (!down) {
            ((CWebView*)(mWebView.Get())) -> CenterKeyPressOnTextField();
        }
        // Pass to super to handle longpress.
        return AutoCompleteTextView::DispatchKeyEvent(event);
    }

    // Ensure there is a layout so arrow keys are handled properly.
    if ((GetLayout().Get()) == NULL) {
        Measure(mWidthSpec, mHeightSpec);
    }
    //AutoPtr<ISelection> selectionT;     //JAVA_DIR:text/Selection.java(public class) 
    //Selection::New((ISelection**)selectionT);
    Int32 oldStart = 0;
    Int32 oldEnd = 0;
    //selectionT -> GetSelectionStart( (ICharSequence*)(text.Get()), &oldStart);
    //selectionT -> GetSelectionEnd( (ICharSequence*)(text.Get()), &oldEnd );

    Boolean maxedOut = ( (mMaxLength != -1) && (oldLength == mMaxLength) );
    // If we are at max length, and there is a selection rather than a
    // cursor, we need to store the text to compare later, since the key
    // may have changed the string.
    String oldText;
    if (maxedOut && (oldEnd != oldStart)) {
        ( (ICharSequence*)(text.Get()) ) -> ToString(&oldText);
    } 
    else {
        oldText = "";
    }
    if (AutoCompleteTextView::DispatchKeyEvent(event)) {
        // If the WebTextView handled the key it was either an alphanumeric
        // key, a delete, or a movement within the text. All of those are
        // ok to pass to javascript.

        // UNLESS there is a max length determined by the html.  In that
        // case, if the string was already at the max length, an
        // alphanumeric key will be erased by the LengthFilter,
        // so do not pass down to javascript, and instead
        // return true.  If it is an arrow key or a delete key, we can go
        // ahead and pass it down.
        if (KeyEvent_KEYCODE_ENTER == keyCode) {
            // For multi-line text boxes, newlines will
            // trigger onTextChanged for key down (which will send both
            // key up and key down) but not key up.
            mGotEnterDown = TRUE;
        }        
        if (maxedOut && !isArrowKey && keyCode != KeyEvent_KEYCODE_DEL) {
            String strText;
            GetText() -> ToString(&strText);
            if (oldEnd == oldStart) {
                // Return true so the key gets dropped.
                return TRUE;
            }             
            else if (!oldText.Equals(strText)) {
                // FIXME: This makes the text work properly, but it
                // does not pass down the key event, so it may not
                // work for a textfield that has the type of
                // behavior of GoogleSuggest.  That said, it is
                // unlikely that a site would combine the two in
                // one textfield.
                AutoPtr<ISpannable> span = (ISpannable*)( (GetText().Get()) -> Probe(EIID_ISpannable) );
                Int32 newStart = 0;
                Int32 newEnd = 0;
                //AutoPtr<ISelection> selectionT;     //JAVA_DIR:text/Selection.java(public class) 
                //Selection::New((ISelection**)selectionT);
                //selectionT -> GetSelectionStart( (ICharSequence*)(text.Get()), &newStart);
                //selectionT -> GetSelectionEnd( (ICharSequence*)(text.Get()), &newEnd );
                String strSpan;
                ( ((ICharSequence*)span) -> ToString(&strSpan) );
                ((CWebView*)(mWebView.Get())) -> ReplaceTextfieldText(0, oldLength, strSpan,newStart, newEnd);
                return TRUE;
            }
        }
        /* FIXME:
         * In theory, we would like to send the events for the arrow keys.
         * However, the TextView can arbitrarily change the selection (i.e.
         * long press followed by using the trackball).  Therefore, we keep
         * in sync with the TextView via onSelectionChanged.  If we also
         * send the DOM event, we lose the correct selection.
        if (isArrowKey) {
            // Arrow key does not change the text, but we still want to send
            // the DOM events.
            sendDomEvent(event);
        }
         */
        return TRUE;
    }
    // Ignore the key up event for newlines. This prevents
    // multiple newlines in the native textarea.
    if (mGotEnterDown && !down) {
        return TRUE;
    }
    // if it is a navigation key, pass it to WebView
    if (isArrowKey) {
        // WebView check the trackballtime in onKeyDown to avoid calling
        // native from both trackball and key handling. As this is called
        // from WebTextView, we always want WebView to check with native.
        // Reset trackballtime to ensure it.
        ((CWebView*)(mWebView.Get())) -> ResetTrackballTime();
        return down ? ( ((CWebView*)(mWebView.Get())) -> OnKeyDown(keyCode, event) ) : ( ((CWebView*)(mWebView.Get())) -> OnKeyUp(keyCode, event) );
    }
    return FALSE;
}

/**
 *  Determine whether this WebTextView currently represents the node
 *  represented by ptr.
 *  @param  ptr Pointer to a node to compare to.
 *  @return boolean Whether this WebTextView already represents the node
 *          pointed to by ptr.
 */
/* package */
Boolean WebTextView::IsSameTextField(
	/* [in] */ Int32 ptr)
{
    return ptr == mNodePointer;
}

/**
 * Ensure that the underlying textfield is lined up with the WebTextView.
 */
void WebTextView::LineUpScroll()
{
    AutoPtr<ILayout> layout = GetLayout();
    if ( (mWebView.Get() != NULL) && (layout.Get() != NULL) ) {
        float maxScrollX = 0.0;
        //AutoPtr<ITouch> touchT;    //JAVA:    text/method  Touch.java
        //CTouch::New((ITouch**)touchT);
        Int32 nMaxScrollX = 0;
        //touchT -> GetMaxScrollX(this, layout, mScrollY,&nMaxScrollX);
        if (DebugFlags::sWEB_TEXT_VIEW) {
            String strOut = String("onTouchEvent x=") + String::FromInt32(mScrollX) + String(" y=") + String::FromInt32(mScrollY) + String(" maxX=") + String::FromInt32(maxScrollX) + String("\n");
            Utility::Logging::Logger::V(LOGTAG, strOut);
        }
        ((CWebView*)(mWebView.Get())) -> ScrollFocusedTextInput( (maxScrollX > 0 ? mScrollX / maxScrollX : 0), mScrollY);
    }
}

//@Override
AutoPtr<IInputConnection> WebTextView::OnCreateInputConnection(
	/* [in] */ IEditorInfo* outAttrs)
{
    AutoPtr<IInputConnection> connection = AutoCompleteTextView::OnCreateInputConnection(outAttrs);
    if (mWebView.Get() != NULL) {
        // Use the name of the textfield + the url.  Use backslash as an
        // arbitrary separator.
        String strFocusCandidateName;
        String strUrl;
        strFocusCandidateName = ((CWebView*)(mWebView.Get())) -> NativeFocusCandidateName();
        ((CWebView*)(mWebView.Get())) -> GetUrl(&strUrl);
        ((CEditorInfo*)outAttrs) -> mFieldName = strFocusCandidateName + "\\" + strUrl;
    }
    return connection;
}

//@Override
void WebTextView::OnDraw(
    /* [in] */ ICanvas* canvas)
{    
    // onDraw should only be called for password fields.  If WebTextView is
    // still drawing, but is no longer corresponding to a password field,
    // remove it.
    Boolean bIsPassword = ( (CWebView*)(mWebView.Get()) ) -> NativeFocusCandidateIsPassword();
    Boolean bSameTextField = IsSameTextField( ( (CWebView*)(mWebView.Get()) ) -> NativeFocusCandidatePointer() );

    if (mWebView.Get() == NULL || !bIsPassword || !bSameTextField) {
        // Although calling remove() would seem to make more sense here,
        // changing it to not be a password field will make it not draw.
        // Other code will make sure that it is removed completely, but this
        // way the user will not see it.
        SetInPassword(FALSE);
    } 
    else {
        AutoCompleteTextView::OnDraw(canvas);
    }
}

void WebTextView::OnDrawSubstitute()
{
    UpdateCursorControllerPositions();
}

//@Override
CARAPI WebTextView::OnEditorAction(
	/* [in] */ Int32 actionCode)
{
    switch (actionCode) {
        case EditorInfo_IME_ACTION_NEXT:{
            if ( ( (CWebView*)(mWebView.Get()) ) -> NativeMoveCursorToNextTextInput() ) {
                // Since the cursor will no longer be in the same place as the
                // focus, set the focus controller back to inactive
                ( (CWebView*)(mWebView.Get()) ) -> SetFocusControllerInactive();
                // Preemptively rebuild the WebTextView, so that the action will
                // be set properly.
                ( (CWebView*)(mWebView.Get()) ) -> RebuildWebTextView();
                SetDefaultSelection();
                ( (CWebView*)(mWebView.Get()) ) -> /*Invalidate()*/InvalidateEx2();    //WebView:AbsoluteLayout:ViewGroup:View
            }
            break;
        }
        case EditorInfo_IME_ACTION_DONE:{
            AutoCompleteTextView::OnEditorAction(actionCode);
            break;
        }
        case EditorInfo_IME_ACTION_GO:
        case EditorInfo_IME_ACTION_SEARCH:{
            // Send an enter and hide the soft keyboard
            
            AutoPtr<ILocalInputMethodManager> iInputMethodManagerT;
            iInputMethodManagerT = CLocalInputMethodManager::GetInstance(mContext);
            Boolean bHideSoftInputFromWindow;
            iInputMethodManagerT -> HideSoftInputFromWindow(GetWindowToken(), 0,&bHideSoftInputFromWindow);
            
            AutoPtr<IKeyEvent> keyEventDown;
            CKeyEvent::New(KeyEvent_ACTION_DOWN,KeyEvent_KEYCODE_ENTER,(IKeyEvent**)&keyEventDown);
            AutoPtr<IKeyEvent> keyEventUp;
            CKeyEvent::New(KeyEvent_ACTION_UP,KeyEvent_KEYCODE_ENTER,(IKeyEvent**)&keyEventUp);
            SendDomEvent(keyEventDown);
            SendDomEvent(keyEventUp);
        }
        default:
            break;
    }
}

//@Override
void WebTextView::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    mFromFocusChange = TRUE;
    AutoCompleteTextView::OnFocusChanged(focused, direction, previouslyFocusedRect);
    mFromFocusChange = TRUE;
}

//@Override
void WebTextView::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    AutoCompleteTextView::OnScrollChanged(l, t, oldl, oldt);
    LineUpScroll();
}

//@Override
void WebTextView::OnSelectionChanged(
    /* [in] */ Int32 selStart,
    /* [in] */ Int32 selEnd)
{
    if (mInSetTextAndKeepSelection) {
        return;
    }
    // This code is copied from TextView.onDraw().  That code does not get
    // executed, however, because the WebTextView does not draw, allowing
    // webkit's drawing to show through.
    
    AutoPtr<ILocalInputMethodManager> imm;
    imm = CLocalInputMethodManager::PeekInstance();
    
    Boolean bIsActive = FALSE;
    imm -> IsActive( (IView*)this, &bIsActive);
    if( (imm.Get() == NULL) && bIsActive ){
        AutoPtr<ISpannable> sp = (ISpannable*)( GetText().Get() );
        Int32 candStart = CEditableInputConnection::GetComposingSpanStart(sp.Get());    //widget/EditableInputConnection.java
        Int32 candEnd = CEditableInputConnection::GetComposingSpanEnd(sp.Get());
        imm -> UpdateSelection((IView*)this, selStart, selEnd, candStart, candEnd);
    }
    if (!mFromWebKit && !mFromFocusChange && !mFromSetInputType && (mWebView.Get() != NULL) ) {
        if (DebugFlags::sWEB_TEXT_VIEW) {
            String strOut = String("onSelectionChanged selStart=") + String::FromInt32(selStart) + String(" selEnd=") + String::FromInt32(selEnd) + String("\n");
            Utility::Logging::Logger::V(LOGTAG, strOut);
        }
        ( (CWebView*)(mWebView.Get()) ) -> SetSelection(selStart, selEnd);
        LineUpScroll();
    }
}

//@Override
void WebTextView::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    AutoCompleteTextView::OnTextChanged(s, start, before, count);
    String postChange;
    s -> ToString(&postChange);
    // Prevent calls to setText from invoking onTextChanged (since this will
    // mean we are on a different textfield).  Also prevent the change when
    // going from a textfield with a string of text to one with a smaller
    // limit on text length from registering the onTextChanged event.
    if (mPreChange == NULL || mPreChange.Equals(postChange) ||
                (mMaxLength > -1 && mPreChange.GetLength() > mMaxLength &&
                mPreChange.Substring(0, mMaxLength).Equals(postChange))) {
        return;
    }
    mPreChange = postChange;
    if (0 == count) {
        if (before > 0) {
            // This was simply a delete or a cut, so just delete the
            // selection.
            ( (CWebView*)(mWebView.Get()) ) -> DeleteSelection(start, start + before);
            // For this and all changes to the text, update our cache
            UpdateCachedTextfield();
        }
        // before should never be negative, so whether it was a cut
        // (handled above), or before is 0, in which case nothing has
        // changed, we should return.
        return;
    }
    // Find the last character being replaced.  If it can be represented by
    // events, we will pass them to native (after replacing the beginning
    // of the changed text), so we can see javascript events.
    // Otherwise, replace the text being changed (including the last
    // character) in the textfield.
    TextUtils::GetChars(s, start + count - 1, start + count, mCharacter.Get(), 0);
    /*
    AutoPtr<IKeyCharacterMap> kmap;    //view/KeyCharacterMap
    kmap = KeyCharacterMap::Load(KeyCharacterMap_BUILT_IN_KEYBOARD);
    */
    AutoFree< ArrayOf< AutoPtr<IKeyEvent> > > events;
    //events = kmap -> GetEvents(mCharacter);
    
    Boolean cannotUseKeyEvents = (NULL == events.Get());
    Int32 charactersFromKeyEvents = cannotUseKeyEvents ? 0 : 1;
    if (count > 1 || cannotUseKeyEvents) {
        AutoPtr<ICharSequence> iCharSequenceT;
        s -> SubSequence(start, start + count - charactersFromKeyEvents, (ICharSequence**)&iCharSequenceT);
        String replace;
        iCharSequenceT -> ToString(&replace);
        ( (CWebView*)(mWebView.Get()) ) -> ReplaceTextfieldText(start, start + before, replace,
                    start + count - charactersFromKeyEvents,
                    start + count - charactersFromKeyEvents);
    } 
    else {
        // This corrects the selection which may have been affected by the
        // trackball or auto-correct.
        if (DebugFlags::sWEB_TEXT_VIEW) {
            String strOut = String("onTextChanged start=") + String::FromInt32(start) + String(" start + before=") + String::FromInt32(start + before) + String("\n");
            Utility::Logging::Logger::V(LOGTAG, strOut);
        }
        if (!mInSetTextAndKeepSelection) {
            ( (CWebView*)(mWebView.Get()) ) -> SetSelection(start, start + before);
        }
    }
    if (!cannotUseKeyEvents) {
        Int32 length = events -> GetLength();
        for (Int32 i = 0; i < length; i++) {
            // We never send modifier keys to native code so don't send them
            // here either.
            Int32 keyCodeT = 0;
            ((*events)[i]) -> GetKeyCode(&keyCodeT);
            if (!CKeyEvent::IsModifierKey(keyCodeT)) {
                SendDomEvent((*events)[i].Get());
            }
        }
    }
    UpdateCachedTextfield();
}

//@Override
Boolean WebTextView::OnTouchEvent(
	/* [in] */ IMotionEvent* event)
{
    Int32 action;
    event -> GetAction(&action);
    switch (action) {
        case MotionEvent_ACTION_DOWN:
        {
            AutoCompleteTextView::OnTouchEvent(event);
            // This event may be the start of a drag, so store it to pass to the
            // WebView if it is.
            event -> GetX(&mDragStartX);
            event -> GetY(&mDragStartY);
            event -> GetEventTime(&mDragStartTime);
            mDragSent = FALSE;
            mScrolled = FALSE;
            mGotTouchDown = TRUE;
            mHasPerformedLongClick = FALSE;
            break;
        }
        case MotionEvent_ACTION_MOVE:
        {
            if (mHasPerformedLongClick) {
                mGotTouchDown = FALSE;
                return FALSE;
            }
            Int32 slop = 0;
            /*
            AutoPtr<IViewConfiguration> viewConfigurationT;
            CViewConfiguration::Get(mContext,(IViewConfiguration**)&viewConfigurationT);
            viewConfigurationT -> GetScaledTouchSlop(&slop);
            */
            AutoPtr<ISpannable> buffer = (ISpannable*)( (GetText().Get()) -> Probe(EIID_ISpannable) );
            Int32 initialScrollX = 0;//CTouch::GetInitialScrollX(this, buffer);    //text/method  Touch.java(public class)
            Int32 initialScrollY = 0;//CTouch::GetInitialScrollY(this, buffer);
            AutoCompleteTextView::OnTouchEvent(event);
            Int32 dx = Math::Abs(mScrollX - initialScrollX);
            Int32 dy = Math::Abs(mScrollY - initialScrollY);
            // Use a smaller slop when checking to see if we've moved far enough
            // to scroll the text, because experimentally, slop has shown to be
            // to big for the case of a small textfield.
            Int32 smallerSlop = slop/2;
            if (dx > smallerSlop || dy > smallerSlop) {
                // Scrolling is handled in onScrollChanged.
                mScrolled = TRUE;
                CancelLongPress();
                return TRUE;
            }
            float fX = 0.0, fY = 0.0;
            event -> GetX(&fX);
            event -> GetY(&fY);

            if (Math::Abs( (Int32)(fX - mDragStartX) ) < slop
                    && Math::Abs((Int32)(fY - mDragStartY) ) < slop) {
                // If the user has not scrolled further than slop, we should not
                // send the drag.  Instead, do nothing, and when the user lifts
                // their finger, we will change the selection.
                return TRUE;
            }
            if (mWebView.Get() != NULL) {
                // Only want to set the initial state once.
                if (!mDragSent) {
                    ( (CWebView*)(mWebView.Get()) ) -> InitiateTextFieldDrag(mDragStartX, mDragStartY, mDragStartTime);
                    mDragSent = TRUE;
                }
                Boolean scrolled = ( (CWebView*)(mWebView.Get()) ) -> TextFieldDrag(event);
                if (scrolled) {
                    mScrolled = TRUE;
                    CancelLongPress();
                    return TRUE;
                }
            }
            return FALSE;
        }
        case MotionEvent_ACTION_UP:
        case MotionEvent_ACTION_CANCEL:
        {
            AutoCompleteTextView::OnTouchEvent(event);
            if (mHasPerformedLongClick) {
                mGotTouchDown = FALSE;
                return FALSE;
            }
            if (!mScrolled) {
                // If the page scrolled, or the TextView scrolled, we do not
                // want to change the selection
                CancelLongPress();
                if (mGotTouchDown && (mWebView.Get() != NULL) ) {
                    ( (CWebView*)(mWebView.Get()) ) -> TouchUpOnTextField(event);
                }
            }
            // Necessary for the WebView to reset its state
            if (mWebView.Get() != NULL && mDragSent) {
                Boolean bOnTouchEvent;
                ( (CWebView*)(mWebView.Get()) ) -> OnTouchEvent(event,&bOnTouchEvent);
            }
            mGotTouchDown = FALSE;
            break;
        }
        default:
            break;
    }
    return TRUE;
}

//@Override
Boolean WebTextView::OnTrackballEvent(
	/* [in] */ IMotionEvent* event)
{
    if (IsPopupShowing()) {
        return AutoCompleteTextView::OnTrackballEvent(event);
    }
    Int32 action;
    event -> GetAction(&action);
    if (action != MotionEvent_ACTION_MOVE) {
        return FALSE;
    }
    // If the Cursor is not on the text input, webview should handle the
    // trackball
    Boolean bCursorMatchesFocus = FALSE;
    bCursorMatchesFocus = ( (CWebView*)(mWebView.Get()) ) -> NativeCursorMatchesFocus();

    if (!bCursorMatchesFocus) {
        Boolean bOnTrackballEventT1 = FALSE;
        ( (CWebView*)(mWebView.Get()) ) -> OnTrackballEvent(event,&bOnTrackballEventT1);
        return bOnTrackballEventT1;
    }
    AutoPtr<ISpannable> text = (ISpannable*)( (GetText().Get()) -> Probe(EIID_ISpannable) );
    AutoPtr<IMovementMethod>  move = GetMovementMethod();
    AutoPtr<ILayout> layOutT = GetLayout();
    Boolean bOnTrackballEventT2 = FALSE;
    move -> OnTrackballEvent((ITextView*)this, text.Get(), event,&bOnTrackballEventT2);
    if ( (move.Get() != NULL) && (layOutT.Get() != NULL) &&bOnTrackballEventT2) {
        // Selection is changed in onSelectionChanged
        return TRUE;
    }
    return FALSE;
}

//@Override
Boolean WebTextView::PerformLongClick()
{
    mHasPerformedLongClick = TRUE;
    return AutoCompleteTextView::PerformLongClick();
}

/**
 * Remove this WebTextView from its host WebView, and return
 * focus to the host.
 */
/* package */
void WebTextView::Remove()
{
    // hide the soft keyboard when the edit text is out of focus
    
    AutoPtr<ILocalInputMethodManager> imm;
    imm = CLocalInputMethodManager::GetInstance( mContext);
    
    Boolean bHideSoftInputFromWindow;
    AutoPtr<IBinder> binder;
    binder = GetWindowToken();
    imm -> HideSoftInputFromWindow(binder, 0, &bHideSoftInputFromWindow);
    //( mWebView.Get() ) -> RemoveView(this);
    Boolean bRequestFocus;
    ( mWebView.Get() ) -> RequestFocus(&bRequestFocus);
}

/* package */
void WebTextView::BringIntoView()
{
    AutoPtr<ILayout> layOutT;
    layOutT = GetLayout();
    if ( layOutT.Get() != NULL ) {
        AutoPtr<ICharSequence> charSequenceT = GetText().Get();
        BringPointIntoView(/*CSelection::GetSelectionEnd(charSequenceT.Get())*/0);    //JAVA:    text/Selection.java(public class)
    }
}

/**
 *  Send the DOM events for the specified event.
 *  @param event    KeyEvent to be translated into a DOM event.
 */
void WebTextView::SendDomEvent(
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<ICharSequence> charSequenceT = GetText().Get();
    String currentText;
    charSequenceT -> ToString(&currentText);
    ( (CWebView*)(mWebView.Get()) ) -> PassToJavaScript(currentText, event);
}

/**
 *  Always use this instead of setAdapter, as this has features specific to
 *  the WebTextView.
 */
void WebTextView::SetAdapterCustom(
	/* [in] */ AutoCompleteAdapter* adapter)
{
    if (adapter != NULL) {
        SetInputType(GetInputType() | InputType_TYPE_TEXT_FLAG_AUTO_COMPLETE);
        adapter -> SetTextView((ITextView*)this);
    }
    AutoCompleteTextView::SetAdapter((IListAdapter*)adapter);
}

/**
 * Sets the selection when the user clicks on a textfield or textarea with
 * the trackball or center key, or starts typing into it without clicking on
 * it.
 */
/* package */
void WebTextView::SetDefaultSelection()
{
    AutoPtr<ISpannable> text = (ISpannable*)(GetText().Get());
    Int32 textLen = 0;
    text -> GetLength(&textLen);
    Int32 selection = mSingle ? textLen : 0;
    if (/*CSelection::GetSelectionStart((ICharSequence*)text) == selection*/FALSE
                && /*CSelection::GetSelectionEnd((ICharSequence*)text) == selection*/FALSE) {   //JAVA:  text/Selection
        // The selection of the UI copy is set correctly, but the
        // WebTextView still needs to inform the webkit thread to set the
        // selection.  Normally that is done in onSelectionChanged, but
        // onSelectionChanged will not be called because the UI copy is not
        // changing.  (This can happen when the WebTextView takes focus.
        // That onSelectionChanged was blocked because the selection set
        // when focusing is not necessarily the desirable selection for
        // WebTextView.)
        if (mWebView.Get() != NULL) {
            ( (CWebView*)(mWebView.Get()) ) -> SetSelection(selection, selection);
        }
    } 
    else {
        //CSelection::SetSelection(text, selection, selection);
    }
}

/**
 * Determine whether to use the system-wide password disguising method,
 * or to use none.
 * @param   inPassword  True if the textfield is a password field.
 */
/* package */
void WebTextView::SetInPassword(
    /* [in] */Boolean inPassword)
{
    if (inPassword) {
        SetInputType(InputType_TYPE_CLASS_TEXT | InputType_TYPE_TEXT_VARIATION_PASSWORD);
        CreateBackground();
    }
    // For password fields, draw the WebTextView.  For others, just show
    // webkit's drawing.
    SetWillNotDraw(!inPassword);
    SetBackgroundDrawable(inPassword ? mBackground : NULL);
}

/**
 * Create a background for the WebTextView and set up the paint for drawing
 * the text.  This way, we can see the password transformation of the
 * system, which (optionally) shows the actual text before changing to dots.
 * The background is necessary to hide the webkit-drawn text beneath.
 */
void WebTextView::CreateBackground()
{
    if (mBackground != NULL) {
            return;
    }
    //mBackground = new OutlineDrawable();

    SetGravity(Gravity_CENTER_VERTICAL);
    // Turn on subpixel text, and turn off kerning, so it better matches
    // the text in webkit.
    AutoPtr<ITextPaint> paint = GetPaint();
    Int32 flags = 0;
    paint -> GetFlags(&flags);
    flags = flags | Paint_SUBPIXEL_TEXT_FLAG | Paint_ANTI_ALIAS_FLAG & ~Paint_DEV_KERN_TEXT_FLAG;
    paint -> SetFlags(flags);
    // Set the text color to black, regardless of the theme.  This ensures
    // that other applications that use embedded WebViews will properly
    // display the text in password textfields.
    SetTextColor(Color_BLACK);
}

//@Override
CARAPI WebTextView::SetInputType(
	/* [in] */ Int32 type)
{
    mFromSetInputType = TRUE;
    AutoCompleteTextView::SetInputType(type);
    mFromSetInputType = FALSE;
}

void WebTextView::SetMaxLength(
    /* [in] */ Int32 maxLength)
{
    mMaxLength = maxLength;
    if (-1 == maxLength) {
        //SetFilters(NO_FILTERS);
    }
    else {
        /*
        AutoFree<ArrayOf<IInputFilter> > arrayIInputFilterT;
        ArrayOf<IInputFilter>::Free(arrayIInputFilterT.Get());
        arrayIInputFilterT = ArrayOf<IInputFilter>::Alloc(1);
        (*arrayIInputFilterT)[i] = new InputFilter.LengthFilter(maxLength); //JAVA: text/InputFilter.java(public interface . static class)
        SetFilters(arrayIInputFilterT);
        */
    }
}

/**
 *  Set the pointer for this node so it can be determined which node this
 *  WebTextView represents.
 *  @param  ptr Integer representing the pointer to the node which this
 *          WebTextView represents.
 */
/* package */
void WebTextView::SetNodePointer(
    /* [in] */ Int32 ptr)
{
    mNodePointer = ptr;
}

/**
 * Determine the position and size of WebTextView, and add it to the
 * WebView's view heirarchy.  All parameters are presumed to be in
 * view coordinates.  Also requests Focus and sets the cursor to not
 * request to be in view.
 * @param x         x-position of the textfield.
 * @param y         y-position of the textfield.
 * @param width     width of the textfield.
 * @param height    height of the textfield.
 */
/* package */
void WebTextView::SetRect(
	/* [in] */ Int32 x,
	/* [in] */ Int32 y,
	/* [in] */ Int32 width,
	/* [in] */ Int32 height)
{    
    AutoPtr<IAbsoluteLayoutLayoutParams> lp = (IAbsoluteLayoutLayoutParams*)(GetLayoutParams().Get());
    if (NULL == lp.Get()) {
        CAbsoluteLayoutLayoutParams::New(width, height, x, y,(IAbsoluteLayoutLayoutParams**)&lp);
    } 
    else {
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mX = x;
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mY = y;
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mWidth = width;
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mHeight = height;
    }
    
    if ( (GetParent().Get()) == NULL) {
        ( (CWebView*)(mWebView.Get()) ) -> AddViewEx3((IView*)this, (IViewGroupLayoutParams*)lp);
    } 
    else {
        SetLayoutParams((IViewGroupLayoutParams*)lp);
    }
    // Set up a measure spec so a layout can always be recreated.
    mWidthSpec = MeasureSpec::MakeMeasureSpec(width, MeasureSpec::EXACTLY);
    mHeightSpec = MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY);
}

/**
 * Set the selection, and disable our onSelectionChanged action.
 */
/* package */
void WebTextView::SetSelectionFromWebKit(
	/* [in] */ Int32 start,
	/* [in] */ Int32 end)
{
    if (start < 0 || end < 0){
        return;
    } 
    AutoPtr<ISpannable> text = (ISpannable*)(GetText().Get());
    Int32 length;
    text -> GetLength(&length);
    if (start > length || end > length){
        return;
    }
    mFromWebKit = TRUE;
    //CSelection::SetSelection(text, start, end);   //JAVA:  text/Selection.java
    mFromWebKit = FALSE;
}

/**
 * Set the text to the new string, but use the old selection, making sure
 * to keep it within the new string.
 * @param   text    The new text to place in the textfield.
 */
/* package */
void WebTextView::SetTextAndKeepSelection(
	/* [in] */ const String& text)
{
    mPreChange = text;
    AutoPtr<IEditable> edit = (IEditable*)(GetText().Get());
    Int32 selStart = 0;//CSelection::GetSelectionStart(edit);   //JAVA text/Selection.java(static Method)
    Int32 selEnd = 0;//CSelection::GetSelectionEnd(edit);
    mInSetTextAndKeepSelection = TRUE;
    AutoPtr<IEditable> editT;
    Int32 editLen;
    ((ICharSequence*)(edit.Get())) -> GetLength(&editLen);
    edit -> ReplaceEx(0, editLen, (ICharSequence*)&text, (IEditable**)&editT);
    Int32 newLength;
    ((ICharSequence*)(edit.Get())) -> GetLength(&newLength);
    if (selStart > newLength) {
        selStart = newLength;
    } 
    if (selEnd > newLength) {
        selEnd = newLength;
    } 
    //CSelection::SetSelection(edit, selStart, selEnd);
    mInSetTextAndKeepSelection = FALSE;
    UpdateCachedTextfield();
}

/**
 * Called by WebView.rebuildWebTextView().  Based on the type of the <input>
 * element, set up the WebTextView, its InputType, and IME Options properly.
 * @param type int corresponding to enum "type" defined in WebView.cpp.
 *              Does not correspond to HTMLInputElement::InputType so this
 *              is unaffected if that changes, and also because that has no
 *              type corresponding to textarea (which is its own tag).
 */
/* package */
void WebTextView::SetType(
	/* [in] */ Int32 type)
{
    if (mWebView.Get() == NULL){
        return;
    } 
    Boolean single = TRUE;
    Boolean inPassword = FALSE;
    Int32 maxLength = -1;
    Int32 inputType = InputType_TYPE_CLASS_TEXT;
    Boolean bFocusCandidateHasNextTextfield = ( (CWebView*)(mWebView.Get()) ) -> NativeFocusCandidateHasNextTextfield();
    if ( bFocusCandidateHasNextTextfield ) {
            inputType |= InputType_TYPE_TEXT_VARIATION_WEB_EDIT_TEXT;
    }
    Int32 imeOptions = EditorInfo_IME_FLAG_NO_EXTRACT_UI | EditorInfo_IME_FLAG_NO_FULLSCREEN;
    switch (type) {
        case 0: // NORMAL_TEXT_FIELD
        {
            imeOptions |= EditorInfo_IME_ACTION_GO;
            break;
        }
        case 1: // TEXT_AREA
        {
            single = FALSE;
            inputType = InputType_TYPE_TEXT_FLAG_MULTI_LINE
                        | InputType_TYPE_TEXT_FLAG_CAP_SENTENCES
                        | InputType_TYPE_CLASS_TEXT
                        | InputType_TYPE_TEXT_FLAG_AUTO_CORRECT;
            imeOptions |= EditorInfo_IME_ACTION_NONE;
            break;
        }                
        case 2: // PASSWORD
        {
            inPassword = TRUE;
            imeOptions |= EditorInfo_IME_ACTION_GO;
            break;
        }
        case 3: // SEARCH
        {
            imeOptions |= EditorInfo_IME_ACTION_SEARCH;
            break;            
        }
        case 4: // EMAIL
        {
            // TYPE_TEXT_VARIATION_WEB_EDIT_TEXT prevents EMAIL_ADDRESS
            // from working, so exclude it for now.
            imeOptions |= EditorInfo_IME_ACTION_GO;
            break;            
        }
        case 5: // NUMBER
        {
            inputType |= InputType_TYPE_CLASS_NUMBER;
            // Number and telephone do not have both a Tab key and an
            // action, so set the action to NEXT
            imeOptions |= EditorInfo_IME_ACTION_NEXT;
            break;            
        }
        case 6: // TELEPHONE
        {
            inputType |= InputType_TYPE_CLASS_PHONE;
            imeOptions |= EditorInfo_IME_ACTION_NEXT;
            break;            
        }
        case 7: // URL
        {
            // TYPE_TEXT_VARIATION_URI prevents Tab key from showing, so
            // exclude it for now.
            imeOptions |= EditorInfo_IME_ACTION_GO;
            break;            
        }
        default:
        {
            imeOptions |= EditorInfo_IME_ACTION_GO;
            break;            
        }
    }
    SetHint(NULL);
    if (single) {
        Boolean bFocusCandidateFramePointer = ((CWebView*)(mWebView.Get())) -> NativeFocusCandidateFramePointer();
        ((CWebView*)(mWebView.Get())) -> RequestLabel(bFocusCandidateFramePointer,mNodePointer);
        maxLength = ((CWebView*)(mWebView.Get())) -> NativeFocusCandidateMaxLength();
        if (type != 2 /* PASSWORD */) {
            String name = ((CWebView*)(mWebView.Get())) -> NativeFocusCandidateName();
            if (name != NULL && name.GetLength() > 0) {
                ((CWebView*)(mWebView.Get())) -> RequestFormData(name, mNodePointer);
            }
        }
    }
    mSingle = single;
    SetMaxLength(maxLength);
    SetHorizontallyScrolling(single);
    SetInputType(inputType);
    SetImeOptions(imeOptions);
    SetInPassword(inPassword);
    AutoCompleteAdapter* adapter = NULL;
    SetAdapterCustom(adapter);
}

/**
 *  Update the cache to reflect the current text.
 */
/* package */
void WebTextView::UpdateCachedTextfield()
{
    AutoPtr<ICharSequence> charSequenceT = GetText().Get();
    String strUpdatedText;
    charSequenceT -> ToString(&strUpdatedText);
    ((CWebView*)(mWebView.Get())) -> UpdateCachedTextfield(strUpdatedText);    
}

//@Override
Boolean WebTextView::RequestRectangleOnScreen(
	/* [in] */ IRect* rectangle)
{
    // don't scroll while in zoom animation. When it is done, we will adjust
    // the WebTextView if it is in editing mode.
    Boolean bIsAnimateZoom;
    bIsAnimateZoom = ((CWebView*)(mWebView.Get())) -> InAnimateZoom();
    if (!bIsAnimateZoom) {
        return AutoCompleteTextView::RequestRectangleOnScreen(rectangle);
    }
    return FALSE;
}