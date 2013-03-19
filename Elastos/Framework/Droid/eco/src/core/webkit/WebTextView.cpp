
#include "webkit/WebTextView.h"

const CString WebTextView::LOGTAG = "webtextview";

/**
 * Create a new WebTextView.
 * @param   context The Context for this WebTextView.
 * @param   webView The WebView that created this.
 */
/* package */
WebTextView::WebTextView(
	/* [in] */ IContext* context,
	/* [in] */ IWebView* webView)
{}

//@Override
CARAPI_(Boolean) WebTextView::DispatchKeyEvent(
	/* [in] */ IKeyEvent* event)
{
#if 0
	assert(event != NULL);

	Boolean bSystem = FALSE;
	event->IsSystem(&bSystem);
	if (bSystem) {
        return AutoCompleteTextView::DispatchKeyEvent(event);
    }
    // Treat ACTION_DOWN and ACTION MULTIPLE the same
    Int32 action;
    event->GetAction(&action);
    Boolean down = action != KeyEvent_ACTION_UP;

    Int32 keyCode = FALSE;
    event->GetKeyCode(&keyCode);

    Boolean isArrowKey = FALSE;
    switch(keyCode) {
        case KeyEvent_KEYCODE_DPAD_LEFT:
        case KeyEvent_KEYCODE_DPAD_RIGHT:
        case KeyEvent_KEYCODE_DPAD_UP:
        case KeyEvent_KEYCODE_DPAD_DOWN:
            if (!((CWebView*)mWebView->Get())->NativeCursorMatchesFocus()) {
                return down ? mWebView->OnKeyDown(keyCode, event) : mWebView->OnKeyUp(keyCode, event);

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
    AutoPtr<ISpannable> text;// = (Spannable) GetText();
    Int32 oldLength;// = text.length();
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
            InputMethodManager.getInstance(mContext)
                    .hideSoftInputFromWindow(getWindowToken(), 0);
            sendDomEvent(new KeyEvent(KeyEvent.ACTION_DOWN, keyCode));
            sendDomEvent(event);
        }
        return super.dispatchKeyEvent(event);
    } else if (KeyEvent.KEYCODE_DPAD_CENTER == keyCode) {
        // Note that this handles center key and trackball.
        if (isPopupShowing()) {
            return super.dispatchKeyEvent(event);
        }
        if (!mWebView.nativeCursorMatchesFocus()) {
            return down ? mWebView.onKeyDown(keyCode, event) : mWebView
                    .onKeyUp(keyCode, event);
        }
        // Center key should be passed to a potential onClick
        if (!down) {
            mWebView.centerKeyPressOnTextField();
        }
        // Pass to super to handle longpress.
        return super.dispatchKeyEvent(event);
    }

    // Ensure there is a layout so arrow keys are handled properly.
    if (getLayout() == null) {
        measure(mWidthSpec, mHeightSpec);
    }
    int oldStart = Selection.getSelectionStart(text);
    int oldEnd = Selection.getSelectionEnd(text);

    boolean maxedOut = mMaxLength != -1 && oldLength == mMaxLength;
    // If we are at max length, and there is a selection rather than a
    // cursor, we need to store the text to compare later, since the key
    // may have changed the string.
    String oldText;
    if (maxedOut && oldEnd != oldStart) {
        oldText = text.toString();
    } else {
        oldText = "";
    }
    if (super.dispatchKeyEvent(event)) {
        // If the WebTextView handled the key it was either an alphanumeric
        // key, a delete, or a movement within the text. All of those are
        // ok to pass to javascript.

        // UNLESS there is a max length determined by the html.  In that
        // case, if the string was already at the max length, an
        // alphanumeric key will be erased by the LengthFilter,
        // so do not pass down to javascript, and instead
        // return true.  If it is an arrow key or a delete key, we can go
        // ahead and pass it down.
        if (KeyEvent.KEYCODE_ENTER == keyCode) {
            // For multi-line text boxes, newlines will
            // trigger onTextChanged for key down (which will send both
            // key up and key down) but not key up.
            mGotEnterDown = true;
        }
        if (maxedOut && !isArrowKey && keyCode != KeyEvent.KEYCODE_DEL) {
            if (oldEnd == oldStart) {
                // Return true so the key gets dropped.
                return true;
            } else if (!oldText.equals(getText().toString())) {
                // FIXME: This makes the text work properly, but it
                // does not pass down the key event, so it may not
                // work for a textfield that has the type of
                // behavior of GoogleSuggest.  That said, it is
                // unlikely that a site would combine the two in
                // one textfield.
                Spannable span = (Spannable) getText();
                int newStart = Selection.getSelectionStart(span);
                int newEnd = Selection.getSelectionEnd(span);
                mWebView.replaceTextfieldText(0, oldLength, span.toString(),
                        newStart, newEnd);
                return true;
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
        return true;
    }
    // Ignore the key up event for newlines. This prevents
    // multiple newlines in the native textarea.
    if (mGotEnterDown && !down) {
        return true;
    }
    // if it is a navigation key, pass it to WebView
    if (isArrowKey) {
        // WebView check the trackballtime in onKeyDown to avoid calling
        // native from both trackball and key handling. As this is called
        // from WebTextView, we always want WebView to check with native.
        // Reset trackballtime to ensure it.
        mWebView.resetTrackballTime();
        return down ? mWebView.onKeyDown(keyCode, event) : mWebView
                .onKeyUp(keyCode, event);
    }
#endif
    return false;
}

/**
 *  Determine whether this WebTextView currently represents the node
 *  represented by ptr.
 *  @param  ptr Pointer to a node to compare to.
 *  @return boolean Whether this WebTextView already represents the node
 *          pointed to by ptr.
 */
/* package */
CARAPI_(Boolean) WebTextView::IsSameTextField(
	/* [in] */ Int32 ptr)
{}

//@Override
CARAPI_(AutoPtr<IInputConnection>) WebTextView::OnCreateInputConnection(
	/* [in] */ IEditorInfo* outAttrs)
{}

CARAPI_(void) WebTextView::OnDrawSubstitute()
{}

//@Override
CARAPI WebTextView::OnEditorAction(
	/* [in] */ Int32 actionCode)
{}

//@Override
CARAPI_(Boolean) WebTextView::OnTouchEvent(
	/* [in] */ IMotionEvent* event)
{}

//@Override
CARAPI_(Boolean) WebTextView::OnTrackballEvent(
	/* [in] */ IMotionEvent* event)
{}

//@Override
CARAPI_(Boolean) WebTextView::PerformLongClick()
{}

/**
 * Remove this WebTextView from its host WebView, and return
 * focus to the host.
 */
/* package */
CARAPI_(void) WebTextView::Remove()
{}

/* package */
CARAPI_(void) WebTextView::BringIntoView()
{}

/**
 *  Always use this instead of setAdapter, as this has features specific to
 *  the WebTextView.
 */
CARAPI_(void) WebTextView::SetAdapterCustom(
	/* [in] */ AutoCompleteAdapter* adapter)
{}

/**
 * Sets the selection when the user clicks on a textfield or textarea with
 * the trackball or center key, or starts typing into it without clicking on
 * it.
 */
/* package */
CARAPI_(void) WebTextView::SetDefaultSelection()
{}

/**
 * Determine whether to use the system-wide password disguising method,
 * or to use none.
 * @param   inPassword  True if the textfield is a password field.
 */
/* package */
CARAPI_(void) WebTextView::SetInPassword(
	CARAPI_(Boolean) inPassword)
{}

//@Override
CARAPI WebTextView::SetInputType(
	/* [in] */ Int32 type)
{}

/**
 *  Set the pointer for this node so it can be determined which node this
 *  WebTextView represents.
 *  @param  ptr Integer representing the pointer to the node which this
 *          WebTextView represents.
 */
/* package */
CARAPI_(void) WebTextView::setNodePointer(
    /* [in] */ Int32 ptr)
{}

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
CARAPI_(void) WebTextView::SetRect(
	/* [in] */ Int32 x,
	/* [in] */ Int32 y,
	/* [in] */ Int32 width,
	/* [in] */ Int32 height)
{}

/**
 * Set the selection, and disable our onSelectionChanged action.
 */
/* package */
CARAPI_(void) WebTextView::SetSelectionFromWebKit(
	/* [in] */ Int32 start,
	/* [in] */ Int32 end)
{}

/**
 * Set the text to the new string, but use the old selection, making sure
 * to keep it within the new string.
 * @param   text    The new text to place in the textfield.
 */
/* package */
CARAPI_(void) WebTextView::SetTextAndKeepSelection(
	/* [in] */ const String& text)
{}

/**
 * Called by WebView.rebuildWebTextView().  Based on the type of the <input>
 * element, set up the WebTextView, its InputType, and IME Options properly.
 * @param type int corresponding to enum "type" defined in WebView.cpp.
 *              Does not correspond to HTMLInputElement::InputType so this
 *              is unaffected if that changes, and also because that has no
 *              type corresponding to textarea (which is its own tag).
 */
/* package */
CARAPI_(void) WebTextView::SetType(
	/* [in] */ Int32 type)
{}

/**
 *  Update the cache to reflect the current text.
 */
/* package */
CARAPI_(void) WebTextView::UpdateCachedTextfield()
{}

//@Override
CARAPI_(Boolean) WebTextView::RequestRectangleOnScreen(
	/* [in] */ IRect* rectangle)
{}

//@Override
CARAPI_(void) WebTextView::OnDraw(
	/* [in] */ ICanvas* canvas)
{}

//@Override
CARAPI_(void) WebTextView::OnFocusChanged(
	/* [in] */ Boolean focused,
	/* [in] */ Int32 direction,
	/* [in] */ IRect* previouslyFocusedRect)
{}

//@Override
CARAPI_(void) WebTextView::OnScrollChanged(
	/* [in] */ Int32 l,
	/* [in] */ Int32 t,
	/* [in] */ Int32 oldl,
	/* [in] */ Int32 oldt)
{}

//@Override
CARAPI_(void) WebTextView::OnSelectionChanged(
	/* [in] */ Int32 selStart,
	/* [in] */ Int32 selEnd)
{}

//@Override
CARAPI_(void) WebTextView::OnTextChanged(
	/* [in] */ ICharSequence* s,
	/* [in] */ Int32 start,
	/* [in] */ Int32 before,
	/* [in] */ Int32 count)
{}

/**
 * Ensure that the underlying textfield is lined up with the WebTextView.
 */
CARAPI_(void) WebTextView::LineUpScroll()
{}

/**
 *  Send the DOM events for the specified event.
 *  @param event    KeyEvent to be translated into a DOM event.
 */
CARAPI_(void) WebTextView::SendDomEvent(
	/* [in] */ IKeyEvent* event)
{}

/**
 * Create a background for the WebTextView and set up the paint for drawing
 * the text.  This way, we can see the password transformation of the
 * system, which (optionally) shows the actual text before changing to dots.
 * The background is necessary to hide the webkit-drawn text beneath.
 */
CARAPI_(void) WebTextView::CreateBackground()
{}

CARAPI_(void) WebTextView::SetMaxLength(
	/* [in] */ Int32 maxLength)
{}


/******************************AutoCompleteAdapter*************************/
WebTextView::AutoCompleteAdapter::AutoCompleteAdapter(
    /* [in] */ IContext* context//,
    /* [in] */ /*ArrayList<String> entries*/)
{}

/**
 * {@inheritDoc}
 */
CARAPI_(AutoPtr<IView>) WebTextView::AutoCompleteAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{}

/**
 * Set the TextView so we can match its text size.
 */
CARAPI_(void) WebTextView::AutoCompleteAdapter::SetTextView(
    /* [in] */ ITextView* tv)
{}

/*******************************OutlineDrawable*********************************/
CARAPI_(void) WebTextView::OutlineDrawable::Draw(
	/* [in] */ ICanvas* canvas)
{}

// Always want it to be opaque.
CARAPI_(Int32) WebTextView::OutlineDrawable::GetOpacity()
{}

// These are needed because they are abstract in Drawable.
CARAPI_(void) WebTextView::OutlineDrawable::SetAlpha(
	/* [in] */ Int32 alpha)
{}

CARAPI_(void) WebTextView::OutlineDrawable::SetColorFilter(
	/* [in] */ IColorFilter* cf)
{}
