#ifndef __WEBTEXTVIEW_H__
#define __WEBTEXTVIEW_H__

#include "widget/AutoCompleteTextView.h"

#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <elastos/ElRefBase.h>
#include <elastos/List.h>
#include "widget/ArrayAdapter.h"
#include "graphics/drawable/Drawable.h"

/**
 * WebTextView is a specialized version of EditText used by WebView
 * to overlay html textfields (and textareas) to use our standard
 * text editing.
 */
/* package */
 class WebTextView : public ElRefBase, AutoCompleteTextView {

 public:
    static const CString LOGTAG;// = "webtextview";

    /**
     *  This is a special version of ArrayAdapter which changes its text size
     *  to match the text size of its host TextView.
     */
    class AutoCompleteAdapter : public ArrayAdapter
    {
    public:
        friend class WebTextView;

        //public
        AutoCompleteAdapter(
            /* [in] */ IContext* context,
            /* [in] */ IObjectContainer* entries);

        /**
         * {@inheritDoc}
         */
        //public
        virtual CARAPI_(AutoPtr<IView>) GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent);

    private:
        /**
         * Set the TextView so we can match its text size.
         */
        CARAPI_(void) SetTextView(
            /* [in] */ ITextView* tv);

        AutoPtr<ITextView> mTextView;
    };

    /**
     * Private class used for the background of a password textfield.
     */
    class OutlineDrawable : public ElRefBase, public Drawable//, public IDrawable
    {
    public:
        virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

        //public
        virtual /*CARAPI_(void)*/CARAPI Draw(
            /* [in] */ ICanvas* canvas);

        // Always want it to be opaque.
        //public
        virtual CARAPI_(Int32) GetOpacity();

        // These are needed because they are abstract in Drawable.
        //public
        virtual /*CARAPI_(void)*/CARAPI SetAlpha(
            /* [in] */ Int32 alpha);

        //public
        virtual /*CARAPI_(void)*/CARAPI SetColorFilter(
            /* [in] */ IColorFilter* cf);
    };

    /**
     * Create a new WebTextView.
     * @param   context The Context for this WebTextView.
     * @param   webView The WebView that created this.
     */
    /* package */
    WebTextView(
        /* [in] */ IContext* context,
        /* [in] */ IWebView* webView);

    //@Override
    //public
    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     *  Determine whether this WebTextView currently represents the node
     *  represented by ptr.
     *  @param  ptr Pointer to a node to compare to.
     *  @return boolean Whether this WebTextView already represents the node
     *          pointed to by ptr.
     */
    /* package */
    virtual CARAPI_(Boolean) IsSameTextField(
        /* [in] */ Int32 ptr);

    //@Override
    //public
    virtual CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs);

    //public
    virtual CARAPI_(void) OnDrawSubstitute();

    //@Override
    //public
    virtual CARAPI OnEditorAction(
        /* [in] */ Int32 actionCode);

    //@Override
    //public
    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    //public
    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    //public
    virtual CARAPI_(Boolean) PerformLongClick();

    /**
     * Remove this WebTextView from its host WebView, and return
     * focus to the host.
     */
    /* package */
    virtual CARAPI_(void) Remove();

    /* package */
    virtual CARAPI_(void) BringIntoView();

    /**
     *  Always use this instead of setAdapter, as this has features specific to
     *  the WebTextView.
     */
    //public
    virtual CARAPI_(void) SetAdapterCustom(
        /* [in] */ AutoCompleteAdapter* adapter);

    /**
     * Sets the selection when the user clicks on a textfield or textarea with
     * the trackball or center key, or starts typing into it without clicking on
     * it.
     */
    /* package */
    virtual CARAPI_(void) SetDefaultSelection();

    /**
     * Determine whether to use the system-wide password disguising method,
     * or to use none.
     * @param   inPassword  True if the textfield is a password field.
     */
    /* package */
    virtual CARAPI_(void) SetInPassword(
        /* [in] */ Boolean inPassword);

    //@Override
    //public
    virtual CARAPI SetInputType(
        /* [in] */ Int32 type);



    /**
     *  Set the pointer for this node so it can be determined which node this
     *  WebTextView represents.
     *  @param  ptr Integer representing the pointer to the node which this
     *          WebTextView represents.
     */
    /* package */
    virtual CARAPI_(void) SetNodePointer(
        /* [in] */ Int32 ptr);

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
    virtual CARAPI_(void) SetRect(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Set the selection, and disable our onSelectionChanged action.
     */
    /* package */
    virtual CARAPI_(void) SetSelectionFromWebKit(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Set the text to the new string, but use the old selection, making sure
     * to keep it within the new string.
     * @param   text    The new text to place in the textfield.
     */
    /* package */
    virtual CARAPI_(void) SetTextAndKeepSelection(
        /* [in] */ const String& text);

    /**
     * Called by WebView.rebuildWebTextView().  Based on the type of the <input>
     * element, set up the WebTextView, its InputType, and IME Options properly.
     * @param type int corresponding to enum "type" defined in WebView.cpp.
     *              Does not correspond to HTMLInputElement::InputType so this
     *              is unaffected if that changes, and also because that has no
     *              type corresponding to textarea (which is its own tag).
     */
    /* package */
    virtual CARAPI_(void) SetType(
        /* [in] */ Int32 type);

    /**
     *  Update the cache to reflect the current text.
     */
    /* package */
    virtual CARAPI_(void) UpdateCachedTextfield();

    //@Override
    //public
    virtual CARAPI_(Boolean) RequestRectangleOnScreen(
        /* [in] */ IRect* rectangle);

protected:
    //@Override
    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    virtual CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    virtual CARAPI_(void) OnScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

    //@Override
    virtual CARAPI_(void) OnSelectionChanged(
        /* [in] */ Int32 selStart,
        /* [in] */ Int32 selEnd);

    //@Override
    virtual CARAPI_(void) OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

private:
    /**
     * Ensure that the underlying textfield is lined up with the WebTextView.
     */
    CARAPI_(void) LineUpScroll();

    /**
     *  Send the DOM events for the specified event.
     *  @param event    KeyEvent to be translated into a DOM event.
     */
    CARAPI_(void) SendDomEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * Create a background for the WebTextView and set up the paint for drawing
     * the text.  This way, we can see the password transformation of the
     * system, which (optionally) shows the actual text before changing to dots.
     * The background is necessary to hide the webkit-drawn text beneath.
     */
    CARAPI_(void) CreateBackground();

    CARAPI_(void) SetMaxLength(
        /* [in] */ Int32 maxLength);

private:
    AutoPtr<IWebView>       mWebView;
    Boolean         mSingle;
    Int32           mWidthSpec;
    Int32           mHeightSpec;
    Int32           mNodePointer;
    // FIXME: This is a hack for blocking unmatched key ups, in particular
    // on the enter key.  The method for blocking unmatched key ups prevents
    // the shift key from working properly.
    Boolean         mGotEnterDown;
    Int32           mMaxLength;
    // Keep track of the text before the change so we know whether we actually
    // need to send down the DOM events.
    String          mPreChange;
    AutoPtr<IDrawable>        mBackground;
    // Variables for keeping track of the touch down, to send to the WebView
    // when a drag starts
    Float           mDragStartX;
    Float           mDragStartY;
    Int64           mDragStartTime;
    Boolean         mDragSent;
    // True if the most recent drag event has caused either the TextView to
    // scroll or the web page to scroll.  Gets reset after a touch down.
    Boolean         mScrolled;
    // Whether or not a selection change was generated from webkit.  If it was,
    // we do not need to pass the selection back to webkit.
    Boolean         mFromWebKit;
    // Whether or not a selection change was generated from the WebTextView
    // gaining focus.  If it is, we do not want to pass it to webkit.  This
    // selection comes from the MovementMethod, but we behave differently.  If
    // WebTextView gained focus from a touch, webkit will determine the
    // selection.
    Boolean         mFromFocusChange;
    // Whether or not a selection change was generated from setInputType.  We
    // do not want to pass this change to webkit.
    Boolean         mFromSetInputType;
    Boolean         mGotTouchDown;
    // Keep track of whether a long press has happened.  Only meaningful after
    // an ACTION_DOWN MotionEvent
    Boolean         mHasPerformedLongClick;
    Boolean         mInSetTextAndKeepSelection;
    // Array to store the final character added in onTextChanged, so that its
    // KeyEvents may be determined.
    AutoFree<ArrayOf<char> >      mCharacter;
    // This is used to reset the length filter when on a textfield
    // with no max length.
    // FIXME: This can be replaced with TextView.NO_FILTERS if that
    // is made public/protected.
    /*
    static const AutoFree<ArrayOf< AutoPtr<IInputFilter> > > NO_FILTERS;
    */
};

#endif //__WEBTEXTVIEW_H__