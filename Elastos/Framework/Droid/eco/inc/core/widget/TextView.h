#ifndef __TEXTVIEW_H__
#define __TEXTVIEW_H__

#include "view/View.h"
#include "graphics/CRectF.h"
#include "text/CTextPaint.h"
#include "view/ViewMacro.h"
#include "view/inputmethod/CExtractedText.h"
#include "widget/Scroller.h"

using namespace Elastos;

class TextView : public View
{
protected:
    class InputContentType : public ElRefBase
    {
    public:
        InputContentType() : mImeOptions(EditorInfo_IME_NULL)
        {}

        Int32 mImeOptions;
        String mPrivateImeOptions;
        AutoPtr<ICharSequence> mImeActionLabel;
        Int32 mImeActionId;
        AutoPtr<IBundle> mExtras;
        AutoPtr<IOnEditorActionListener> mOnEditorActionListener;
        Boolean enterDown;
    };

    class InputMethodState : public ElRefBase
    {
    public:
        InputMethodState()
            : mCursorChanged(FALSE)
            , mSelectionModeChanged(FALSE)
            , mContentChanged(FALSE)
        {
            ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mCursorRectInWindow));
            ASSERT_SUCCEEDED(CRectF::NewByFriend((CRectF**)&mTmpRectF));
            ASSERT_SUCCEEDED(CExtractedText::New((IExtractedText**)&mTmpExtracted));
        }

        AutoPtr<IRect> mCursorRectInWindow;
        AutoPtr<IRectF> mTmpRectF;
        Float mTmpOffset[2];
        AutoPtr<IExtractedTextRequest> mExtracting;
        AutoPtr<IExtractedText> mTmpExtracted;
        Int32 mBatchEditNesting;
        Boolean mCursorChanged;
        Boolean mSelectionModeChanged;
        Boolean mContentChanged;
        Int32 mChangedStart;
        Int32 mChangedEnd;
        Int32 mChangedDelta;
    };
private:
    class CharWrapper:
        public ElRefBase,
        public IGetChars
    {
        friend class TextView;

    public:
        CharWrapper(
            /* [in] */ const ArrayOf<Char8>& chars,
            /* [in] */ Int32 start,
            /* [in] */ Int32 len);

        ~CharWrapper();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) Set(
            /* [in] */ const ArrayOf<Char8>& chars,
            /* [in] */ Int32 start,
            /* [in] */ Int32 len);

        CARAPI GetLength(
            /* [out] */ Int32* len);

        CARAPI GetCharAt(
            /* [in] */ Int32 off,
            /* [out] */ Char32* c);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** csq);

        CARAPI GetChars(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ArrayOf<Char8>* dest,
            /* [in] */ Int32 destoff);

        CARAPI DrawText(
            /* [in] */ ICanvas* c,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ IPaint* p);

        CARAPI MeasureText(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ IPaint* p,
            /* [out] */ Float* width);

        CARAPI GetTextWidths(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ArrayOf<Float>* widths,
            /* [in] */ IPaint* p,
            /* [out] */ Int32* count);

    private:
        ArrayOf<Char8>* mChars;
        Int32 mStart, mLength;
    };

    class HandleView;

    /**
     * A CursorController instance can be used to control a cursor in the text.
     * It is not used outside of {@link TextView}.
     * @hide
     */
    class CursorController : public ElRefBase, public IOnTouchModeChangeListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /**
         * Makes the cursor controller visible on screen. Will be drawn by {@link #draw(Canvas)}.
         * See also {@link #hide()}.
         */
        virtual CARAPI_(void) Show() = 0;

        /**
         * Hide the cursor controller from screen.
         * See also {@link #show()}.
         */
        virtual CARAPI_(void) Hide() = 0;

        /**
         * @return TRUE if the CursorController is currently visible
         */
        virtual CARAPI_(Boolean) IsShowing() = 0;

        /**
         * Update the controller's position.
         */
        virtual CARAPI_(void) UpdatePosition(
            /* [in] */ HandleView* handle,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y) = 0;

        virtual CARAPI_(void) UpdatePosition() = 0;

        /**
         * This method is called by {@link #onTouchEvent(MotionEvent)} and gives the controller
         * a chance to become active and/or visible.
         * @param event The touch event
         */
        virtual CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* event) = 0;

        /**
         * Called when the view is detached from window. Perform house keeping task, such as
         * stopping Runnable thread that would otherwise keep a reference on the context, thus
         * preventing the activity to be recycled.
         */
        virtual CARAPI_(void) OnDetached() = 0;
    };

    class _HandleView : public View
    {
    public:
        static const Int32 LEFT = 0;
        static const Int32 CENTER = 1;
        static const Int32 RIGHT = 2;

    public:
        _HandleView(
            /* [in] */ CursorController* controller,
            /* [in] */ Int32 pos,
            /* [in] */ TextView* host);

        CARAPI_(void) SetOrientation(
            /* [in] */ Int32 pos);

        CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);

        CARAPI_(void) Show();

        CARAPI_(void) Hide();

        CARAPI_(Boolean) IsShowing();

        CARAPI_(void) OnDraw(
            /* [in] */ ICanvas* c);

        CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* ev);

        CARAPI_(Boolean) IsDragging();

        CARAPI_(void) PositionAtCursor(
            /* [in] */ Int32 offset,
            /* [in] */ Boolean bottom);

    private:
        CARAPI_(Boolean) IsPositionVisible();

        CARAPI_(void) MoveTo(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

    private:
        Boolean mPositionOnTop;
        AutoPtr<IDrawable> mDrawable;
        AutoPtr<IPopupWindow> mContainer;
        Int32 mPositionX;
        Int32 mPositionY;
        AutoPtr<CursorController> mController;
        Boolean mIsDragging;
        Float mTouchToWindowOffsetX;
        Float mTouchToWindowOffsetY;
        Float mHotspotX;
        Float mHotspotY;
        Int32 mHeight;
        Float mTouchOffsetY;
        Int32 mLastParentX;
        Int32 mLastParentY;

        TextView* mHost;
    };

    class HandleView
        : public ElRefBase
        , public _HandleView
        , public IView
        , public IDrawableCallback
        , public IKeyEventCallback
        , public IAccessibilityEventSource
    {
    public:
        IVIEW_METHODS_DECL();
        IDrawableCallback_METHODS_DECL();
        IKeyEventCallback_METHODS_DECL();
        IAccessibilityEventSource_METHODS_DECL();

        HandleView(
            /* [in] */ CursorController* controller,
            /* [in] */ Int32 pos,
            /* [in] */ TextView* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    class InsertionPointCursorController : public CursorController
    {
    private:
        class MyRunnable : public Runnable
        {
        public:
            MyRunnable(
                /* [in] */ InsertionPointCursorController* host)
                : mHost(host)
            {}

            CARAPI Run()
            {
                mHost->Hide();
                return NOERROR;
            }

        private:
            InsertionPointCursorController* mHost;
        };

    public:
        InsertionPointCursorController(
            /* [in] */ TextView* host);

        CARAPI_(void) Show();

        CARAPI_(void) Hide();

        CARAPI_(Boolean) IsShowing();

        CARAPI_(void) UpdatePosition(
            /* [in] */ HandleView* handle,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

        CARAPI_(void) UpdatePosition();

        CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* ev);

        CARAPI OnTouchModeChanged(
            /* [in] */ Boolean isInTouchMode);

        CARAPI_(void) OnDetached();

    private:
        CARAPI_(void) HideDelayed(
            /* [in] */ Int32 msec);

    private:
        static const Int32 DELAY_BEFORE_FADE_OUT = 4100;

        // The cursor controller image
        AutoPtr<HandleView> mHandle;
        TextView* mHost;
        AutoPtr<Runnable> mHider;
    };

    class SelectionModifierCursorController : public CursorController
    {
    public:
        SelectionModifierCursorController(
            /* [in] */ TextView* host);

        CARAPI_(void) Show();

        CARAPI_(void) Hide();

        CARAPI_(Boolean) IsShowing();

        CARAPI_(void) UpdatePosition(
            /* [in] */ HandleView* handle,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

        CARAPI_(void) UpdatePosition();

        CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(Int32) GetMinTouchOffset();

        CARAPI_(Int32) GetMaxTouchOffset();

        CARAPI_(void) ResetTouchOffsets();

        CARAPI_(Boolean) IsSelectionStartDragged();

        CARAPI OnTouchModeChanged(
            /* [in] */ Boolean isInTouchMode);

        CARAPI_(void) OnDetached();

    private:
        CARAPI_(void) UpdateMinAndMaxOffsets(
            /* [in] */ IMotionEvent* event);

    private:
        // The cursor controller images
        AutoPtr<HandleView> mStartHandle;
        AutoPtr<HandleView> mEndHandle;
        // The offsets of that last touch down event. Remembered to start selection there.
        Int32 mMinTouchOffset;
        Int32 mMaxTouchOffset;
        // Whether selection anchors are active
        Boolean mIsShowing;
        // Double tap detection
        Int64 mPreviousTapUpTime;
        Int32 mPreviousTapPositionX;
        Int32 mPreviousTapPositionY;
        TextView* mHost;
    };

    class ChangeWatcher : public ElRefBase, public ITextWatcher, public ISpanWatcher
    {
    public:
        ChangeWatcher(
            /* [in] */ TextView* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI BeforeTextChanged(
            /* [in] */ ICharSequence* buffer,
            /* [in] */ Int32 start,
            /* [in] */ Int32 before,
            /* [in] */ Int32 after);

        CARAPI OnTextChanged(
            /* [in] */ ICharSequence* buffer,
            /* [in] */ Int32 start,
            /* [in] */ Int32 before,
            /* [in] */ Int32 after);

        CARAPI AfterTextChanged(
            /* [in] */ IEditable* buffer);

        CARAPI OnSpanChanged(
            /* [in] */ ISpannable* buf,
            /* [in] */ IInterface* what,
            /* [in] */ Int32 s,
            /* [in] */ Int32 e,
            /* [in] */ Int32 st,
            /* [in] */ Int32 en);

        CARAPI OnSpanAdded(
            /* [in] */ ISpannable* buf,
            /* [in] */ IInterface* what,
            /* [in] */ Int32 s,
            /* [in] */ Int32 e);

        CARAPI OnSpanRemoved(
            /* [in] */ ISpannable* buf,
            /* [in] */ IInterface* what,
            /* [in] */ Int32 s,
            /* [in] */ Int32 e);

    private:
        AutoPtr<ICharSequence> mBeforeText;
        TextView* mHost;
    };

    class Blink : public Runnable
    {
    public:
        Blink(
            /* [in] */ TextView* v);

        CARAPI Run();

        CARAPI_(void) Cancel();

        CARAPI_(void) Uncancel();

        CARAPI_(void) PostAtTime(
            /* [in] */ IRunnable* action,
            /* [in] */ Int64 millis);

        CARAPI_(void) RemoveCallbacks(
            /* [in] */ IRunnable* action);

    private:
        //final WeakReference<TextView> mView;
        TextView* mView;
        Boolean mCancelled;
        AutoPtr<IApartment> mApartment;
    };

    class MenuHandler
        : public ElRefBase
        , public IOnMenuItemClickListener
    {
    public:
        MenuHandler(
            /* [in] */ TextView* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI OnMenuItemClick(
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* isConsumed);

    private:
        TextView* mHost;
    };

public:
    TextView();

    TextView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010084/*com.android.internal.R.attr.textViewStyle*/);

    /**
     * Sets the typeface and style in which the text should be displayed,
     * and turns on the fake bold and italic bits in the Paint if the
     * Typeface that you provided does not have all the bits in the
     * style that you specified.
     *
     * @attr ref android.R.styleable#TextView_typeface
     * @attr ref android.R.styleable#TextView_textStyle
     */
    virtual CARAPI SetTypeface(
        /* [in] */ ITypeface* tf,
        /* [in] */ Int32 style);

    /**
     * Return the text the TextView is displaying. If setText() was called with
     * an argument of BufferType.SPANNABLE or BufferType.EDITABLE, you can cast
     * the return value from this method to Spannable or Editable, respectively.
     *
     * Note: The content of the return value should not be modified. If you want
     * a modifiable one, you should make your own copy first.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetText();

    /**
     * Returns the length, in characters, of the text managed by this TextView
     */
    virtual CARAPI_(Int32) GetLength();

    /**
     * Return the text the TextView is displaying as an Editable object.  If
     * the text is not editable, null is returned.
     *
     * @see #getText
     */
    virtual CARAPI_(AutoPtr<IEditable>) GetEditableText();

    /**
     * @return the height of one standard line in pixels.  Note that markup
     * within the text can cause individual lines to be taller or shorter
     * than this height, and the layout may contain additional first-
     * or last-line padding.
     */
    virtual CARAPI_(Int32) GetLineHeight();

    /**
     * @return the Layout that is currently being used to display the text.
     * This can be null if the text or width has recently changes.
     */
    CARAPI_(AutoPtr<ILayout>) GetLayout();

    /**
     * @return the current key listener for this TextView.
     * This will frequently be null for non-EditText TextViews.
     */
    CARAPI_(AutoPtr<IKeyListener>) GetKeyListener();

    /**
     * Sets the key listener to be used with this TextView.  This can be null
     * to disallow user input.  Note that this method has significant and
     * subtle interactions with soft keyboards and other input method:
     * see {@link KeyListener#getInputType() KeyListener.getContentType()}
     * for important details.  Calling this method will replace the current
     * content type of the text view with the content type returned by the
     * key listener.
     * <p>
     * Be warned that if you want a TextView with a key listener or movement
     * method not to be focusable, or if you want a TextView without a
     * key listener or movement method to be focusable, you must call
     * {@link #setFocusable} again after calling this to get the focusability
     * back the way you want it.
     *
     * @attr ref android.R.styleable#TextView_numeric
     * @attr ref android.R.styleable#TextView_digits
     * @attr ref android.R.styleable#TextView_phoneNumber
     * @attr ref android.R.styleable#TextView_inputMethod
     * @attr ref android.R.styleable#TextView_capitalize
     * @attr ref android.R.styleable#TextView_autoText
     */
    virtual CARAPI SetKeyListener(
        /* [in] */ IKeyListener* input);

    /**
     * @return the movement method being used for this TextView.
     * This will frequently be null for non-EditText TextViews.
     */
    CARAPI_(AutoPtr<IMovementMethod>) GetMovementMethod();

    /**
     * Sets the movement method (arrow key handler) to be used for
     * this TextView.  This can be null to disallow using the arrow keys
     * to move the cursor or scroll the view.
     * <p>
     * Be warned that if you want a TextView with a key listener or movement
     * method not to be focusable, or if you want a TextView without a
     * key listener or movement method to be focusable, you must call
     * {@link #setFocusable} again after calling this to get the focusability
     * back the way you want it.
     */
    CARAPI SetMovementMethod(
        /* [in] */ IMovementMethod* movement);

    /**
     * @return the current transformation method for this TextView.
     * This will frequently be null except for single-line and password
     * fields.
     */
    //CARAPI_(TransformationMethod) GetTransformationMethod();

    /**
     * Sets the transformation that is applied to the text that this
     * TextView is displaying.
     *
     * @attr ref android.R.styleable#TextView_password
     * @attr ref android.R.styleable#TextView_singleLine
     */
    CARAPI SetTransformationMethod(
        /* [in] */ ITransformationMethod* method);

    /**
     * Returns the top padding of the view, plus space for the top
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingTop();

    /**
     * Returns the bottom padding of the view, plus space for the bottom
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingBottom();

    /**
     * Returns the left padding of the view, plus space for the left
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingLeft();

    /**
     * Returns the right padding of the view, plus space for the right
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingRight();


    /**
     * Returns the extended top padding of the view, including both the
     * top Drawable if any and any extra space to keep more than maxLines
     * of text from showing.  It is only valid to call this after measuring.
     */
    virtual CARAPI_(Int32) GetExtendedPaddingTop();

    /**
     * Returns the extended bottom padding of the view, including both the
     * bottom Drawable if any and any extra space to keep more than maxLines
     * of text from showing.  It is only valid to call this after measuring.
     */
    virtual CARAPI_(Int32) GetExtendedPaddingBottom();

    /**
     * Returns the total left padding of the view, including the left
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingLeft();

    /**
     * Returns the total right padding of the view, including the right
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingRight();

    /**
     * Returns the total top padding of the view, including the top
     * Drawable if any, the extra space to keep more than maxLines
     * from showing, and the vertical offset for gravity, if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingTop();

    /**
     * Returns the total bottom padding of the view, including the bottom
     * Drawable if any, the extra space to keep more than maxLines
     * from showing, and the vertical offset for gravity, if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingBottom();

    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use null if you do not
     * want a Drawable there.  The Drawables must already have had
     * {@link Drawable#setBounds} called.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawables(
        /* [in] */ IDrawable* left,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* right,
        /* [in] */ IDrawable* bottom);


    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use 0 if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @param left Resource identifier of the left Drawable.
     * @param top Resource identifier of the top Drawable.
     * @param right Resource identifier of the right Drawable.
     * @param bottom Resource identifier of the bottom Drawable.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawablesWithIntrinsicBounds(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);


    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use null if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawablesWithIntrinsicBounds(
        /* [in] */ IDrawable* left,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* right,
        /* [in] */ IDrawable* bottom);


    /**
     * Returns drawables for the left, top, right, and bottom borders.
     */
    virtual CARAPI GetCompoundDrawables(
        /* [out, callee] */ ArrayOf<IDrawable*>** drawables);

    /**
     * Sets the size of the padding between the compound drawables and
     * the text.
     *
     * @attr ref android.R.styleable#TextView_drawablePadding
     */
    virtual CARAPI SetCompoundDrawablePadding(
        /* [in] */ Int32 pad);

    /**
     * Returns the padding between the compound drawables and the text.
     */
    virtual CARAPI_(Int32) GetCompoundDrawablePadding();

    virtual CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Gets the autolink mask of the text.  See {@link
     * android.text.util.Linkify#ALL Linkify.ALL} and peers for
     * possible values.
     *
     * @attr ref android.R.styleable#TextView_autoLink
     */
    CARAPI_(Int32) GetAutoLinkMask();

    /**
     * Sets the text color, size, style, hint color, and highlight color
     * from the specified TextAppearance resource.
     */
    virtual CARAPI SetTextAppearance(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resid);

    /**
     * @return the size (in pixels) of the default text size in this TextView.
     */
    virtual CARAPI_(Float) GetTextSize();

    /**
     * Set the default text size to the given value, interpreted as "scaled
     * pixel" units.  This size is adjusted based on the current density and
     * user font size preference.
     *
     * @param size The scaled pixel size.
     *
     * @attr ref android.R.styleable#TextView_textSize
     */
    virtual CARAPI SetTextSize(
        /* [in] */ Float size);

    /**
     * Set the default text size to a given unit and value.  See {@link
     * TypedValue} for the possible dimension units.
     *
     * @param unit The desired dimension unit.
     * @param size The desired size in the given units.
     *
     * @attr ref android.R.styleable#TextView_textSize
     */
    virtual CARAPI SetTextSize(
        /* [in] */ Int32 unit,
        /* [in] */ Float size);

    /**
     * @return the extent by which text is currently being stretched
     * horizontally.  This will usually be 1.
     */
    virtual CARAPI_(Float) GetTextScaleX();

    /**
     * Sets the extent by which text should be stretched horizontally.
     *
     * @attr ref android.R.styleable#TextView_textScaleX
     */
    virtual CARAPI SetTextScaleX(
        /* [in] */ Float size);

    /**
     * Sets the typeface and style in which the text should be displayed.
     * Note that not all Typeface families actually have bold and italic
     * variants, so you may need to use
     * {@link #setTypeface(Typeface, Int32)} to get the appearance
     * that you actually want.
     *
     * @attr ref android.R.styleable#TextView_typeface
     * @attr ref android.R.styleable#TextView_textStyle
     */
    virtual CARAPI SetTypeface(
        /* [in] */ ITypeface* tf);

    /**
     * @return the current typeface and style in which the text is being
     * displayed.
     */
    virtual CARAPI GetTypeface(
        /* [out] */ ITypeface** tf);

    /**
     * Sets the text color for all the states (normal, selected,
     * focused) to be this color.
     *
     * @attr ref android.R.styleable#TextView_textColor
     */
    virtual CARAPI SetTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the text color.
     *
     * @attr ref android.R.styleable#TextView_textColor
     */
    virtual CARAPI SetTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * Return the set of text colors.
     *
     * @return Returns the set of text colors.
     */
    CARAPI GetTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * <p>Return the current color selected for normal text.</p>
     *
     * @return Returns the current text color.
     */
    CARAPI_(Int32) GetCurrentTextColor();

    /**
     * Sets the color used to display the selection highlight.
     *
     * @attr ref android.R.styleable#TextView_textColorHighlight
     */
    virtual CARAPI SetHighlightColor(
        /* [in] */ Int32 color);

    /**
     * Gives the text a shadow of the specified radius and color, the specified
     * distance from its normal position.
     *
     * @attr ref android.R.styleable#TextView_shadowColor
     * @attr ref android.R.styleable#TextView_shadowDx
     * @attr ref android.R.styleable#TextView_shadowDy
     * @attr ref android.R.styleable#TextView_shadowRadius
     */
    virtual CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ Int32 color);

    /**
     * @return the base paint used for the text.  Please use this only to
     * consult the Paint's properties and not to change them.
     */
    virtual CARAPI_(AutoPtr<ITextPaint>) GetPaint();

    /**
     * Sets the autolink mask of the text.  See {@link
     * android.text.util.Linkify#ALL Linkify.ALL} and peers for
     * possible values.
     *
     * @attr ref android.R.styleable#TextView_autoLink
     */
    CARAPI SetAutoLinkMask(
        /* [in] */ Int32 mask);

    /**
     * Sets whether the movement method will automatically be set to
     * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
     * set to nonzero and links are detected in {@link #setText}.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_linksClickable
     */
    CARAPI SetLinksClickable(
        /* [in] */ Boolean whether);

    /**
     * Returns whether the movement method will automatically be set to
     * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
     * set to nonzero and links are detected in {@link #setText}.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_linksClickable
     */
    CARAPI_(Boolean) GetLinksClickable();

    /**
     * Returns the list of URLSpans attached to the text
     * (by {@link Linkify} or otherwise) if any.  You can call
     * {@link URLSpan#getURL} on them to find where they link to
     * or use {@link Spanned#getSpanStart} and {@link Spanned#getSpanEnd}
     * to find the region of the text they are attached to.
     */
    //virtual CARAPI_(URLSpan[]) GetUrls();

    /**
     * Sets the color of the hint text.
     *
     * @attr ref android.R.styleable#TextView_textColorHint
     */
    CARAPI SetHintTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the color of the hint text.
     *
     * @attr ref android.R.styleable#TextView_textColorHint
     */
    CARAPI SetHintTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * <p>Return the color used to paint the hint text.</p>
     *
     * @return Returns the list of hint text colors.
     */
    CARAPI GetHintTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * <p>Return the current color selected to paint the hint text.</p>
     *
     * @return Returns the current hint text color.
     */
    CARAPI_(Int32) GetCurrentHintTextColor();

    /**
     * Sets the color of links in the text.
     *
     * @attr ref android.R.styleable#TextView_textColorLink
     */
    CARAPI SetLinkTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the color of links in the text.
     *
     * @attr ref android.R.styleable#TextView_textColorLink
     */
    CARAPI SetLinkTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * <p>Returns the color used to paint links in the text.</p>
     *
     * @return Returns the list of link text colors.
     */
    CARAPI GetLinkTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * Sets the horizontal alignment of the text and the
     * vertical gravity that will be used when there is extra space
     * in the TextView beyond what is required for the text itself.
     *
     * @see android.view.Gravity
     * @attr ref android.R.styleable#TextView_gravity
     */
    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    /**
     * Returns the horizontal and vertical alignment of this TextView.
     *
     * @see android.view.Gravity
     * @attr ref android.R.styleable#TextView_gravity
     */
    virtual CARAPI_(Int32) GetGravity();

    /**
     * @return the flags on the Paint being used to display the text.
     * @see Paint#getFlags
     */
    virtual CARAPI_(Int32) GetPaintFlags();

    /**
     * Sets flags on the Paint being used to display the text and
     * reflows the text if they are different from the old flags.
     * @see Paint#setFlags
     */
    virtual CARAPI SetPaintFlags(
        /* [in] */ Int32 flags);

    /**
     * Sets whether the text should be allowed to be wider than the
     * View is.  If false, it will be wrapped to the width of the View.
     *
     * @attr ref android.R.styleable#TextView_scrollHorizontally
     */
    virtual CARAPI SetHorizontallyScrolling(
        /* [in] */ Boolean whether);

    /**
     * Makes the TextView at least this many lines tall
     *
     * @attr ref android.R.styleable#TextView_minLines
     */
    virtual CARAPI SetMinLines(
        /* [in] */ Int32 minlines);

    /**
     * Makes the TextView at least this many pixels tall
     *
     * @attr ref android.R.styleable#TextView_minHeight
     */
    virtual CARAPI SetMinHeight(
        /* [in] */ Int32 minHeight);

    /**
     * Makes the TextView at most this many lines tall
     *
     * @attr ref android.R.styleable#TextView_maxLines
     */
    virtual CARAPI SetMaxLines(
        /* [in] */ Int32 maxlines);

    /**
     * Makes the TextView at most this many pixels tall
     *
     * @attr ref android.R.styleable#TextView_maxHeight
     */
    virtual CARAPI SetMaxHeight(
        /* [in] */ Int32 maxHeight);

    /**
     * Makes the TextView exactly this many lines tall
     *
     * @attr ref android.R.styleable#TextView_lines
     */
    virtual CARAPI SetLines(
        /* [in] */ Int32 lines);

    /**
     * Makes the TextView exactly this many pixels tall.
     * You could do the same thing by specifying this number in the
     * LayoutParams.
     *
     * @attr ref android.R.styleable#TextView_height
     */
    virtual CARAPI SetHeight(
        /* [in] */ Int32 pixels);

    /**
     * Makes the TextView at least this many ems wide
     *
     * @attr ref android.R.styleable#TextView_minEms
     */
    virtual CARAPI SetMinEms(
        /* [in] */ Int32 minems);

    /**
     * Makes the TextView at least this many pixels wide
     *
     * @attr ref android.R.styleable#TextView_minWidth
     */
    virtual CARAPI SetMinWidth(
        /* [in] */ Int32 minpixels);

    /**
     * Makes the TextView at most this many ems wide
     *
     * @attr ref android.R.styleable#TextView_maxEms
     */
    virtual CARAPI SetMaxEms(
        /* [in] */ Int32 maxems);

    /**
     * Makes the TextView at most this many pixels wide
     *
     * @attr ref android.R.styleable#TextView_maxWidth
     */
    virtual CARAPI SetMaxWidth(
        /* [in] */ Int32 maxpixels);

    /**
     * Makes the TextView exactly this many ems wide
     *
     * @attr ref android.R.styleable#TextView_ems
     */
    virtual CARAPI SetEms(
        /* [in] */ Int32 ems);

    /**
     * Makes the TextView exactly this many pixels wide.
     * You could do the same thing by specifying this number in the
     * LayoutParams.
     *
     * @attr ref android.R.styleable#TextView_width
     */
    virtual CARAPI SetWidth(
        /* [in] */ Int32 pixels);

    /**
     * Sets line spacing for this TextView.  Each line will have its height
     * multiplied by <code>mult</code> and have <code>add</code> added to it.
     *
     * @attr ref android.R.styleable#TextView_lineSpacingExtra
     * @attr ref android.R.styleable#TextView_lineSpacingMultiplier
     */
    virtual CARAPI SetLineSpacing(
        /* [in] */ Float add,
        /* [in] */ Float mult);

    /**
     * Convenience method: Append the specified text to the TextView's
     * display buffer, upgrading it to BufferType.EDITABLE if it was
     * not already editable.
     */
    CARAPI Append(
        /* [in] */ ICharSequence* text);

    /**
     * Convenience method: Append the specified text slice to the TextView's
     * display buffer, upgrading it to BufferType.EDITABLE if it was
     * not already editable.
     */
    virtual CARAPI Append(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    /**
     * Control whether this text view saves its entire text contents when
     * freezing to an icicle, in addition to dynamic state such as cursor
     * position.  By default this is false, not saving the text.  Set to true
     * if the text in the text view is not being saved somewhere else in
     * persistent storage (such as in a content provider) so that if the
     * view is later thawed the user will not lose their data.
     *
     * @param freezesText Controls whether a frozen icicle should include the
     * entire text data: true to include it, false to not.
     *
     * @attr ref android.R.styleable#TextView_freezesText
     */
    virtual CARAPI SetFreezesText(
        /* [in] */ Boolean freezesText);

    /**
     * Return whether this text view is including its entire text contents
     * in frozen icicles.
     *
     * @return Returns true if text is included, false if it isn't.
     *
     * @see #setFreezesText
     */
    virtual CARAPI_(Boolean) GetFreezesText();

    /**
     * Sets the Factory used to create new Editables.
     */
    CARAPI SetEditableFactory(
        /* [in] */ IEditableFactory* factory);

    /**
     * Sets the Factory used to create new Spannables.
     */
    CARAPI SetSpannableFactory(
        /* [in] */ ISpannableFactory* factory);

    /**
     * Sets the string value of the TextView. TextView <em>does not</em> accept
     * HTML-like formatting, which you can do with text strings in XML resource files.
     * To style your strings, attach android.text.style.* objects to a
     * {@link android.text.SpannableString SpannableString}, or see the
     * <a href="{@docRoot}guide/topics/resources/available-resources.html#stringresources">
     * Available Resource Types</a> documentation for an example of setting
     * formatted text in the XML resource file.
     *
     * @attr ref android.R.styleable#TextView_text
     */
    CARAPI SetText(
        /* [in] */ ICharSequence* text);

    /**
     * Like {@link #setText(CharSequence)},
     * except that the cursor position (if any) is retained in the new text.
     *
     * @param text The new text to place in the text view.
     *
     * @see #setText(CharSequence)
     */
    CARAPI SetTextKeepState(
        /* [in] */ ICharSequence* text);

    /**
     * Sets the text that this TextView is to display (see
     * {@link #setText(CharSequence)}) and also sets whether it is stored
     * in a styleable/spannable buffer and whether it is editable.
     *
     * @attr ref android.R.styleable#TextView_text
     * @attr ref android.R.styleable#TextView_bufferType
     */
    virtual CARAPI SetText(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type);

    /**
     * Sets the TextView to display the specified slice of the specified
     * char array.  You must promise that you will not change the contents
     * of the array except for right before another call to setText(),
     * since the TextView has no way to know that the text
     * has changed and that it needs to invalidate and re-layout.
     */
    CARAPI SetText(
        /* [in] */ const ArrayOf<Char8>& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len);

    /**
     * Like {@link #setText(CharSequence, android.widget.TextView.BufferType)},
     * except that the cursor position (if any) is retained in the new text.
     *
     * @see #setText(CharSequence, android.widget.TextView.BufferType)
     */
    CARAPI SetTextKeepState(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type);

    CARAPI SetText(
        /* [in] */ Int32 resid);

    CARAPI SetText(
        /* [in] */ Int32 resid,
        /* [in] */ BufferType type);

    /**
     * Sets the text to be displayed when the text of the TextView is empty.
     * Null means to use the normal empty text. The hint does not currently
     * participate in determining the size of the view.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    CARAPI SetHint(
        /* [in] */ ICharSequence* hint);

    /**
     * Sets the text to be displayed when the text of the TextView is empty,
     * from a resource.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    CARAPI SetHint(
        /* [in] */ Int32 resid);

    /**
     * Returns the hint that is displayed when the text of the TextView
     * is empty.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetHint();

    /**
     * Set the type of the content with a constant as defined for
     * {@link EditorInfo#inputType}.  This will take care of changing
     * the key listener, by calling {@link #setKeyListener(KeyListener)}, to
     * match the given content type.  If the given content type is
     * {@link EditorInfo#TYPE_NULL} then a soft keyboard will
     * not be displayed for this text view.
     *
     * @see #getInputType()
     * @see #setRawInputType(Int32)
     * @see android.text.InputType
     * @attr ref android.R.styleable#TextView_inputType
     */
    virtual CARAPI SetInputType(
        /* [in] */ Int32 type);

    /**
     * Directly change the content type integer of the text view, without
     * modifying any other state.
     * @see #setInputType(Int32)
     * @see android.text.InputType
     * @attr ref android.R.styleable#TextView_inputType
     */
    virtual CARAPI SetRawInputType(
        /* [in] */ Int32 type);

    /**
     * Get the type of the content.
     *
     * @see #setInputType(Int32)
     * @see android.text.InputType
     */
    virtual CARAPI_(Int32) GetInputType();

    /**
     * Change the editor type integer associated with the text view, which
     * will be reported to an IME with {@link EditorInfo#imeOptions} when it
     * has focus.
     * @see #getImeOptions
     * @see android.view.inputmethod.EditorInfo
     * @attr ref android.R.styleable#TextView_imeOptions
     */
    virtual CARAPI SetImeOptions(
        /* [in] */ Int32 imeOptions);

    /**
     * Get the type of the IME editor.
     *
     * @see #setImeOptions(Int32)
     * @see android.view.inputmethod.EditorInfo
     */
    virtual CARAPI_(Int32) GetImeOptions();

    /**
     * Change the custom IME action associated with the text view, which
     * will be reported to an IME with {@link EditorInfo#actionLabel}
     * and {@link EditorInfo#actionId} when it has focus.
     * @see #getImeActionLabel
     * @see #getImeActionId
     * @see android.view.inputmethod.EditorInfo
     * @attr ref android.R.styleable#TextView_imeActionLabel
     * @attr ref android.R.styleable#TextView_imeActionId
     */
    virtual CARAPI SetImeActionLabel(
        /* [in] */ ICharSequence* label,
        /* [in] */ Int32 actionId);

    /**
     * Get the IME action label previous set with {@link #setImeActionLabel}.
     *
     * @see #setImeActionLabel
     * @see android.view.inputmethod.EditorInfo
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetImeActionLabel();

    /**
     * Get the IME action ID previous set with {@link #setImeActionLabel}.
     *
     * @see #setImeActionLabel
     * @see android.view.inputmethod.EditorInfo
     */
    virtual CARAPI_(Int32) GetImeActionId();

    /**
     * Set a special listener to be called when an action is performed
     * on the text view.  This will be called when the enter key is pressed,
     * or when an action supplied to the IME is selected by the user.  Setting
     * this means that the normal hard key event will not insert a newline
     * into the text view, even if it is multi-line; holding down the ALT
     * modifier will, however, allow the user to insert a newline character.
     */
    //virtual CARAPI_(void) SetOnEditorActionListener(
    //    /* [in] */ OnEditorActionListener l);

    /**
     * Called when an attached input method calls
     * {@link InputConnection#performEditorAction(Int32)
     * InputConnection.performEditorAction()}
     * for this text view.  The default implementation will call your action
     * listener supplied to {@link #setOnEditorActionListener}, or perform
     * a standard operation for {@link EditorInfo#IME_ACTION_NEXT
     * EditorInfo.IME_ACTION_NEXT} or {@link EditorInfo#IME_ACTION_DONE
     * EditorInfo.IME_ACTION_DONE}.
     *
     * <p>For backwards compatibility, if no IME options have been set and the
     * text view would not normally advance focus on enter, then
     * the NEXT and DONE actions received here will be turned into an enter
     * key down/up pair to go through the normal key handling.
     *
     * @param actionCode The code of the action being performed.
     *
     * @see #setOnEditorActionListener
     */
    virtual CARAPI OnEditorAction(
        /* [in] */ Int32 actionCode);

    /**
     * Set the private content type of the text, which is the
     * {@link EditorInfo#privateImeOptions EditorInfo.privateImeOptions}
     * field that will be filled in when creating an input connection.
     *
     * @see #getPrivateImeOptions()
     * @see EditorInfo#privateImeOptions
     * @attr ref android.R.styleable#TextView_privateImeOptions
     */
    virtual CARAPI SetPrivateImeOptions(
        /* [in] */ const String& type);

    /**
     * Get the private type of the content.
     *
     * @see #setPrivateImeOptions(String)
     * @see EditorInfo#privateImeOptions
     */
    virtual CARAPI_(String) GetPrivateImeOptions();

    /**
     * Set the extra input data of the text, which is the
     * {@link EditorInfo#extras TextBoxAttribute.extras}
     * Bundle that will be filled in when creating an input connection.  The
     * given integer is the resource ID of an XML resource holding an
     * {@link android.R.styleable#InputExtras &lt;input-extras&gt;} XML tree.
     *
     * @see #getInputExtras(Boolean)
     * @see EditorInfo#extras
     * @attr ref android.R.styleable#TextView_editorExtras
     */
    virtual CARAPI SetInputExtras(
        /* [in] */ Int32 xmlResId);

    /**
     * Retrieve the input extras currently associated with the text view, which
     * can be viewed as well as modified.
     *
     * @param create If true, the extras will be created if they don't already
     * exist.  Otherwise, null will be returned if none have been created.
     * @see #setInputExtras(Int32)
     * @see EditorInfo#extras
     * @attr ref android.R.styleable#TextView_editorExtras
     */
    virtual CARAPI_(AutoPtr<IBundle>) GetInputExtras(
        /* [in] */ Boolean create);

    /**
     * Returns the error message that was set to be displayed with
     * {@link #setError}, or <code>null</code> if no error was set
     * or if it the error was cleared by the widget after user input.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetError();

    /**
     * Sets the right-hand compound drawable of the TextView to the "error"
     * icon and sets an error message that will be displayed in a popup when
     * the TextView has focus.  The icon and error message will be reset to
     * null when any key events cause changes to the TextView's text.  If the
     * <code>error</code> is <code>null</code>, the error message and icon
     * will be cleared.
     */
    virtual CARAPI SetError(
        /* [in] */ ICharSequence* error);

    /**
     * Sets the right-hand compound drawable of the TextView to the specified
     * icon and sets an error message that will be displayed in a popup when
     * the TextView has focus.  The icon and error message will be reset to
     * null when any key events cause changes to the TextView's text.  The
     * drawable must already have had {@link Drawable#setBounds} set on it.
     * If the <code>error</code> is <code>null</code>, the error message will
     * be cleared (and you should provide a <code>null</code> icon as well).
     */
    virtual CARAPI SetError(
        /* [in] */ ICharSequence* error,
        /* [in] */ IDrawable* icon);

    /**
     * Sets the list of input filters that will be used if the buffer is
     * Editable.  Has no effect otherwise.
     *
     * @attr ref android.R.styleable#TextView_maxLength
     */
    //virtual CARAPI_(void) SetFilters(
    //    /* [in] */ InputFilter[] filters);

    /**
     * Returns the current list of input filters.
     */
    //virtual CARAPI_(InputFilter[]) GetFilters();

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(Boolean) OnPreDraw();

    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI GetFocusedRect(
        /* [in] */ IRect* r);

    /**
     * Return the number of lines of text, or 0 if the internal Layout has not
     * been built.
     */
    virtual CARAPI_(Int32) GetLineCount();

    /**
     * Return the baseline for the specified line (0...getLineCount() - 1)
     * If bounds is not null, return the top, left, right, bottom extents
     * of the specified line in it. If the internal Layout has not been built,
     * return 0 and set bounds to (0, 0, 0, 0)
     * @param line which line to examine (0..getLineCount() - 1)
     * @param bounds Optional. If not null, it returns the extent of the line
     * @return the Y-coordinate of the baseline
     */
    virtual CARAPI_(Int32) GetLineBounds(
        /* [in] */ Int32 line,
        /* [in] */ IRect* bounds);

    CARAPI GetBaseline(
        /* [out] */ Int32* baseline);

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnCheckIsTextEditor();

    virtual CARAPI_(IInputConnection*) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs);

    /**
     * If this TextView contains editable content, extract a portion of it
     * based on the information in <var>request</var> in to <var>outText</var>.
     * @return Returns true if the text was successfully extracted, else false.
     */
    virtual CARAPI_(Boolean) ExtractText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ IExtractedText* outText);


    virtual CARAPI_(Boolean) ExtractTextInternal(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 partialStartOffset,
        /* [in] */ Int32 partialEndOffset,
        /* [in] */ Int32 delta,
        /* [in] */ IExtractedText* outText);

    virtual CARAPI_(Boolean) ReportExtractedText();

    /**
     * This is used to remove all style-impacting spans from text before new
     * extracted text is being replaced into it, so that we don't have any
     * lingering spans applied during the replace.
     */
    //static CARAPI_(void) RemoveParcelableSpans(
    //    /* [in] */ Spannable spannable,
    //    /* [in] */ Int32 start,
    //    /* [in] */ Int32 end);

    /**
     * Apply to this text view the given extracted text, as previously
     * returned by {@link #extractText(ExtractedTextRequest, ExtractedText)}.
     */
    virtual CARAPI SetExtractedText(
        /* [in] */ IExtractedText* text);

    /**
     * @hide
     */
    virtual CARAPI SetExtracting(
        /* [in] */ IExtractedTextRequest* req);

    /**
     * Called by the framework in response to a text completion from
     * the current input method, provided by it calling
     * {@link InputConnection#commitCompletion
     * InputConnection.commitCompletion()}.  The default implementation does
     * nothing; text views that are supporting auto-completion should override
     * this to do their desired behavior.
     *
     * @param text The auto complete text the user has selected.
     */
    virtual CARAPI OnCommitCompletion(
        /* [in] */ ICompletionInfo* text);

    virtual CARAPI BeginBatchEdit();

    virtual CARAPI EndBatchEdit();

    virtual CARAPI_(void) EnsureEndedBatchEdit();

    virtual CARAPI_(void) FinishBatchEdit(
        /* [in] */ InputMethodState* ims);

    virtual CARAPI_(void) UpdateAfterEdit();

    /**
     * Called by the framework in response to a request to begin a batch
     * of edit operations through a call to link {@link #beginBatchEdit()}.
     */
    virtual CARAPI OnBeginBatchEdit();

    /**
     * Called by the framework in response to a request to end a batch
     * of edit operations through a call to link {@link #endBatchEdit}.
     */
    virtual CARAPI OnEndBatchEdit();

    /**
     * Called by the framework in response to a private command from the
     * current method, provided by it calling
     * {@link InputConnection#performPrivateCommand
     * InputConnection.performPrivateCommand()}.
     *
     * @param action The action name of the command.
     * @param data Any additional data for the command.  This may be null.
     * @return Return true if you handled the command, else false.
     */
    virtual CARAPI_(Boolean) OnPrivateIMECommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    /**
     * Set whether the TextView includes extra top and bottom padding to make
     * room for accents that go above the normal ascent and descent.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_includeFontPadding
     */
    virtual CARAPI SetIncludeFontPadding(
        /* [in] */ Boolean includepad);

    /**
     * Move the point, specified by the offset, into the view if it is needed.
     * This has to be called after layout. Returns true if anything changed.
     */
    virtual CARAPI_(Boolean) BringPointIntoView(
        /* [in] */ Int32 offset);

    /**
     * Move the cursor, if needed, so that it is at an offset that is visible
     * to the user.  This will not move the cursor if it represents more than
     * one character (a selection range).  This will only work if the
     * TextView contains spannable text; otherwise it will do nothing.
     *
     * @return True if the cursor was actually moved, false otherwise.
     */
    virtual CARAPI_(Boolean) MoveCursorToVisibleOffset();

    CARAPI ComputeScroll();

    CARAPI_(void) Debug(
        /* [in] */ Int32 depth);

    /**
     * Convenience for {@link Selection#getSelectionStart}.
     */
    virtual CARAPI_(Int32) GetSelectionStart();

    /**
     * Convenience for {@link Selection#getSelectionEnd}.
     */
    virtual CARAPI_(Int32) GetSelectionEnd();

    /**
     * Return true iff there is a selection inside this text view.
     */
    virtual CARAPI_(Boolean) HasSelection();

    /**
     * Sets the properties of this field (lines, horizontally scrolling,
     * transformation method) to be for a single-line input.
     *
     * @attr ref android.R.styleable#TextView_singleLine
     */
    virtual CARAPI SetSingleLine();

    /**
     * If true, sets the properties of this field (lines, horizontally
     * scrolling, transformation method) to be for a single-line input;
     * if false, restores these to the default conditions.
     * Note that calling this with false restores default conditions,
     * not necessarily those that were in effect prior to calling
     * it with true.
     *
     * @attr ref android.R.styleable#TextView_singleLine
     */
    virtual CARAPI SetSingleLine(
        /* [in] */ Boolean singleLine);

    /**
     * Causes words in the text that are longer than the view is wide
     * to be ellipsized instead of broken in the middle.  You may also
     * want to {@link #setSingleLine} or {@link #setHorizontallyScrolling}
     * to constrain the text to a single line.  Use <code>null</code>
     * to turn off ellipsizing.
     *
     * @attr ref android.R.styleable#TextView_ellipsize
     */
    virtual CARAPI SetEllipsize(
        /* [in] */ TextUtilsTruncateAt where);

    /**
     * Sets how many times to repeat the marquee animation. Only applied if the
     * TextView has marquee enabled. Set to -1 to repeat indefinitely.
     *
     * @attr ref android.R.styleable#TextView_marqueeRepeatLimit
     */
    virtual CARAPI SetMarqueeRepeatLimit(
        /* [in] */ Int32 marqueeLimit);

    /**
     * Returns where, if anywhere, words that are longer than the view
     * is wide should be ellipsized.
     */
    virtual CARAPI_(TextUtilsTruncateAt) GetEllipsize();

    /**
     * Set the TextView so that when it takes focus, all the text is
     * selected.
     *
     * @attr ref android.R.styleable#TextView_selectAllOnFocus
     */
    virtual CARAPI SetSelectAllOnFocus(
        /* [in] */ Boolean selectAllOnFocus);

    /**
     * Set whether the cursor is visible.  The default is true.
     *
     * @attr ref android.R.styleable#TextView_cursorVisible
     */
    virtual CARAPI SetCursorVisible(
        /* [in] */ Boolean visible);

    /**
     * Adds a TextWatcher to the list of those whose methods are called
     * whenever this TextView's text changes.
     * <p>
     * In 1.0, the {@link TextWatcher#afterTextChanged} method was erroneously
     * not called after {@link #setText} calls.  Now, doing {@link #setText}
     * if there are any text changed listeners forces the buffer type to
     * Editable if it would not otherwise be and does call this method.
     */
    virtual CARAPI_(void) AddTextChangedListener(
        /* [in] */ ITextWatcher* watcher);


    /**
     * Removes the specified TextWatcher from the list of those whose
     * methods are called
     * whenever this TextView's text changes.
     */
    virtual CARAPI_(void) RemoveTextChangedListener(
        /* [in] */ ITextWatcher* watcher);

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI_(void) SendOnTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI_(void) SendAfterTextChanged(
        /* [in] */ IEditable* text);

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI_(void) HandleTextChanged(
        /* [in] */ ICharSequence* buffer,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI SpanChange(
        /* [in] */ ISpanned* buf,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 oldStart,
        /* [in] */ Int32 newStart,
        /* [in] */ Int32 oldEnd,
        /* [in] */ Int32 newEnd);

    /**
     * @hide
     */
    CARAPI DispatchFinishTemporaryDetach();

    CARAPI OnStartTemporaryDetach();

    CARAPI OnFinishTemporaryDetach();

    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    /**
     * Use {@link BaseInputConnection#removeComposingSpans
     * BaseInputConnection.removeComposingSpans()} to remove any IME composing
     * state from this text view.
     */
    virtual CARAPI ClearComposingText();

    CARAPI SetSelected(
        /* [in] */ Boolean selected);

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * Returns true, only while processing a touch gesture, if the initial
     * touch down event caused focus to move to the text view and as a result
     * its selection changed.  Only valid while processing the touch gesture
     * of interest.
     */
    virtual CARAPI_(Boolean) DidTouchFocusSelect();

    CARAPI CancelLongPress();

    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(void) SetScroller(
        /* [in] */ Scroller* s);

    /**
     * Returns the TextView_textColor attribute from the
     * Resources.StyledAttributes, if set, or the TextAppearance_textColor
     * from the TextView_textAppearance attribute, if TextView_textColor
     * was not set directly.
     */
    static CARAPI_(AutoPtr<IColorStateList>) GetTextColors(
        /* [in] */ IContext* context,
        /* [in] */ ITypedArray* attrs);

    /**
     * Returns the default color from the TextView_textColor attribute
     * from the AttributeSet, if set, or the default color from the
     * TextAppearance_textColor from the TextView_textAppearance attribute,
     * if TextView_textColor was not set directly.
     */
    static CARAPI_(Int32) GetTextColor(
        /* [in] */ IContext* context,
        /* [in] */ ITypedArray* attrs,
        /* [in] */ Int32 def);

    CARAPI_(Boolean) OnKeyShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI_(void) SendAccessibilityEventTypeViewTextChanged(
        /* [in] */ ICharSequence* beforeText,
        /* [in] */ Int32 fromIndex,
        /* [in] */ Int32 removedCount,
        /* [in] */ Int32 addedCount);

    /**
     * Returns whether this text view is a current input method target.  The
     * default implementation just checks with {@link InputMethodManager}.
     */
    virtual CARAPI_(Boolean) IsInputMethodTarget();

    /**
     * Called when a context menu option for the text view is selected.  Currently
     * this will be one of: {@link android.R.id#selectAll},
     * {@link android.R.id#startSelectingText},
     * {@link android.R.id#cut}, {@link android.R.id#copy},
     * {@link android.R.id#paste}, {@link android.R.id#copyUrl},
     * or {@link android.R.id#switchInputMethod}.
     */
    virtual CARAPI_(Boolean) OnTextContextMenuItem(
        /* [in] */ Int32 id);

    CARAPI_(Boolean) PerformLongClick();

    /**
     * Get the offset character closest to the specified absolute position.
     *
     * @param x The horizontal absolute position of a point on screen
     * @param y The vertical absolute position of a point on screen
     * @return the character offset for the character whose position is closest to the specified
     *  position. Returns -1 if there is no layout.
     *
     * @hide
     */
    virtual CARAPI_(Int32) GetOffset(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);


    virtual CARAPI_(Int32) GetHysteresisOffset(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 previousOffset);

    /**
     * @return True if this view supports insertion handles.
     */
    virtual CARAPI_(Boolean) HasInsertionController();

    /**
     * @return True if this view supports selection handles.
     */
    virtual CARAPI_(Boolean) HasSelectionController();

    virtual CARAPI_(AutoPtr<CursorController>) GetInsertionController();

    virtual CARAPI_(AutoPtr<CursorController>) GetSelectionController();

    virtual CARAPI_(Boolean) IsInBatchEditMode();

protected:
    /**
     * Subclasses override this to specify that they have a KeyListener
     * by default even if not specifically called for in the XML options.
     */
    virtual CARAPI_(Boolean) GetDefaultEditable();

    /**
     * Subclasses override this to specify a default movement method.
     */
    virtual CARAPI_(AutoPtr<IMovementMethod>) GetDefaultMovementMethod();

    CARAPI DrawableStateChanged();

    CARAPI SetText(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type,
        /* [in] */ Boolean notifyBefore,
        /* [in] */ Int32 oldlen);

    CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI_(void) OnAttachedToWindow();

    CARAPI_(void) OnDetachedFromWindow();

    CARAPI_(Boolean) IsPaddingOffsetRequired();

    CARAPI_(Int32) GetLeftPaddingOffset();

    CARAPI_(Int32) GetTopPaddingOffset();

    CARAPI_(Int32) GetBottomPaddingOffset();

    CARAPI_(Int32) GetRightPaddingOffset();

    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    /**
     * Update the positions of the CursorControllers.  Needed by WebTextView,
     * which does not draw.
     * @hide
     */
    virtual CARAPI_(void) UpdateCursorControllerPositions();

    /**
     * The width passed in is now the desired layout width,
     * not the full view width with padding.
     * {@hide}
     */
    virtual CARAPI_(void) MakeNewLayout(
        /* [in] */ Int32 w,
        /* [in] */ Int32 hintWidth,
        /* [in] */ IBoringLayoutMetrics* boring,
        /* [in] */ IBoringLayoutMetrics* hintBoring,
        /* [in] */ Int32 ellipsisWidth,
        /* [in] */ Boolean bringIntoView);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    /**
     * This method is called when the text is changed, in case any
     * subclasses would like to know.
     *
     * @param text The text the TextView is displaying.
     * @param start The offset of the start of the range of the text
     *              that was modified.
     * @param before The offset of the former end of the range of the
     *               text that was modified.  If text was simply inserted,
     *               this will be the same as <code>start</code>.
     *               If text was replaced with new text or deleted, the
     *               length of the old text was <code>before-start</code>.
     * @param after The offset of the end of the range of the text
     *              that was modified.  If text was simply deleted,
     *              this will be the same as <code>start</code>.
     *              If text was replaced with new text or inserted,
     *              the length of the new text is <code>after-start</code>.
     */
    virtual CARAPI_(void) OnTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    /**
     * This method is called when the selection has changed, in case any
     * subclasses would like to know.
     *
     * @param selStart The new selection start location.
     * @param selEnd The new selection end location.
     */
    virtual CARAPI_(void) OnSelectionChanged(
        /* [in] */ Int32 selStart,
        /* [in] */ Int32 selEnd);

    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    CARAPI_(Float) GetLeftFadingEdgeStrength();

    CARAPI_(Float) GetRightFadingEdgeStrength();

    CARAPI_(Int32) ComputeHorizontalScrollRange();

    CARAPI_(Int32) ComputeVerticalScrollRange();

    CARAPI_(Int32) ComputeVerticalScrollExtent();

    CARAPI_(void) OnCreateContextMenu(
        /* [in] */ IContextMenu* menu);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010084/*com.android.internal.R.attr.textViewStyle*/);

private:
    CARAPI_(void) SetTypefaceByIndex(
        /* [in] */ Int32 typefaceIndex,
        /* [in] */ Int32 styleIndex);

    CARAPI_(void) SetKeyListenerOnly(
        /* [in] */ IKeyListener* input);

    CARAPI_(void) FixFocusableAndClickableSettings();

    CARAPI_(void) SetRawTextSize(
        /* [in] */ Float size);

    CARAPI_(void) UpdateTextColors();

    /**
     * It would be better to rely on the input type for everything. A password inputType should have
     * a password transformation. We should hence use isPasswordInputType instead of this method.
     *
     * We should:
     * - Call setInputType in setKeyListener instead of changing the input type directly (which
     * would install the correct transformation).
     * - Refuse the installation of a non-password transformation in setTransformation if the input
     * type is password.
     *
     * However, this is like this for legacy reasons and we cannot break existing apps. This method
     * is useful since it matches what the user can see (obfuscated text or not).
     *
     * @return true if the current transformation method is of the password type.
     */
    CARAPI_(Boolean) HasPasswordTransformationMethod();

    CARAPI_(Boolean) IsPasswordInputType(
        /* [in] */ Int32 inputType);

    CARAPI_(Boolean) IsVisiblePasswordInputType(
        /* [in] */ Int32 inputType);

    CARAPI_(void) SetInputType(
        /* [in] */ Int32 type,
        /* [in] */ Boolean direct);

    CARAPI_(void) ShowError();

    /**
     * Returns the Y offset to make the pointy top of the error point
     * at the middle of the error icon.
     */
    CARAPI_(Int32) GetErrorX();

    /**
     * Returns the Y offset to make the pointy top of the error point
     * at the bottom of the error icon.
     */
    CARAPI_(Int32) GetErrorY();

    CARAPI_(void) HideError();

    //CARAPI_(void) ChooseSize(
    //    /* [in] */ PopupWindow pop,
    //    /* [in] */ const ArrayOf<Char8>& text,
    //    /* [in] */ ITextView* tv);

    CARAPI_(void) RestartMarqueeIfNeeded();

    /**
     * Sets the list of input filters on the specified Editable,
     * and includes mInput in the list if it is an InputFilter.
     */
    //CARAPI_(void) SetFilters(
    //    /* [in] */ Editable e,
    //    /* [in] */ InputFilter[] filters);

    CARAPI_(Int32) GetVerticalOffset(
        /* [in] */ Boolean forceNormal);

    CARAPI_(Int32) GetBottomVerticalOffset(
        /* [in] */ Boolean forceNormal);

    CARAPI_(void) InvalidateCursorPath();

    CARAPI_(void) InvalidateCursor();

    CARAPI_(void) InvalidateCursor(
        /* [in] */ Int32 a,
        /* [in] */ Int32 b,
        /* [in] */ Int32 c);

    CARAPI_(void) RegisterForPreDraw();

    /**
     * Returns true if pressing ENTER in this field advances focus instead
     * of inserting the character.  This is true mostly in single-line fields,
     * but also in mail addresses and subjects which will display on multiple
     * lines but where it doesn't make sense to insert newlines.
     */
    CARAPI_(Boolean) ShouldAdvanceFocusOnEnter();

    CARAPI_(Int32) DoKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ IKeyEvent* otherEvent);

    CARAPI_(void) NullLayouts();

    /**
     * Make a new Layout based on the already-measured size of the view,
     * on the assumption that it was measured correctly at some point.
     */
    CARAPI_(void) AssumeLayout();

    CARAPI_(Boolean) CompressText(
        /* [in] */ Float width);

    static CARAPI_(Int32) Desired(
        /* [in] */ ILayout* layout);

    CARAPI_(Int32) GetDesiredHeight();

    CARAPI_(Int32) GetDesiredHeight(
        /* [in] */ ILayout* layout,
        /* [in] */ Boolean cap);

    /**
     * Check whether a change to the existing text layout requires a
     * new view layout.
     */
    CARAPI_(void) CheckForResize();

    /**
     * Check whether entirely new text requires a new view layout
     * or merely a new text layout.
     */
    CARAPI_(void) CheckForRelayout();

    /**
     * Returns true if anything changed.
     */
    CARAPI_(Boolean) BringTextIntoView();

    CARAPI_(void) GetInterestingRect(
        /* [in] */ IRect* r,
        /* [in] */ Int32 line);

    CARAPI_(void) ConvertFromViewportToContentCoordinates(
        /* [in] */ IRect* r);

    CARAPI_(Int32) ViewportToContentHorizontalOffset();

    CARAPI_(Int32) ViewportToContentVerticalOffset();

    CARAPI ApplySingleLine(
        /* [in] */ Boolean singleLine,
        /* [in] */ Boolean applyTransformation);

    CARAPI_(Boolean) CanMarquee();

    CARAPI_(void) StartMarquee();

    CARAPI_(void) StopMarquee();

    CARAPI_(void) StartStopMarquee(
        /* [in] */ Boolean start);

    CARAPI_(void) SendBeforeTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI_(void) MakeBlink();

    CARAPI_(Int32) GetLastTapPosition();

    CARAPI_(void) OnTapUpEvent(
        /* [in] */ Int32 prevStart,
        /* [in] */ Int32 prevEnd);

    CARAPI_(void) PrepareCursorControllers();

    /**
     * @return True iff this TextView contains a text that can be edited.
     */
    CARAPI_(Boolean) IsTextEditable();

    CARAPI_(Boolean) CanSelectText();

    CARAPI_(Boolean) TextCanBeSelected();

    CARAPI_(Boolean) CanCut();

    CARAPI_(Boolean) CanCopy();

    CARAPI_(Boolean) CanPaste();

    /**
     * Returns the offsets delimiting the 'word' located at position offset.
     *
     * @param offset An offset in the text.
     * @return The offsets for the start and end of the word located at <code>offset</code>.
     * The two ints offsets are packed in a Int64, with the starting offset shifted by 32 bits.
     * Returns a negative value if no valid word was found.
     */
    CARAPI_(Int64) GetWordLimitsAt(
        /* [in] */ Int32 offset);

    static CARAPI_(Int64) PackRangeInLong(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(Int32) ExtractRangeStartFromLong(
        /* [in] */ Int64 range);

    static CARAPI_(Int32) ExtractRangeEndFromLong(
        /* [in] */ Int64 range);

    CARAPI_(void) SelectCurrentWord();

    CARAPI_(void) GetWordForDictionary(
        /* [out] */ String* word);

    /**
     * Prepare text so that there are not zero or two spaces at beginning and end of region defined
     * by [min, max] when replacing this region by paste.
     */
    CARAPI_(Int64) PrepareSpacesAroundPaste(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max,
        /* [in] */ ICharSequence* paste);

    CARAPI_(void) StartTextSelectionMode();

    CARAPI_(void) StopTextSelectionMode();

    CARAPI_(void) HideInsertionPointCursorController();

    CARAPI_(void) HideSelectionModifierCursorController();

    CARAPI_(void) HideControllers();

    CARAPI_(Int32) GetOffsetForHorizontal(
        /* [in] */ Int32 line,
        /* [in] */ Int32 x);

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:
    static const char* TEXT_VIEW_LOG_TAG;
    static const Boolean DEBUG_EXTRACT;
    Int32 mTempCoords[2];
    AutoPtr<CRect> mTempRect;

    AutoPtr<InputContentType> mInputContentType;
    AutoPtr<InputMethodState> mInputMethodState;

    Int32 mTextSelectHandleLeftRes;
    Int32 mTextSelectHandleRightRes;
    Int32 mTextSelectHandleRes;

    AutoPtr<IDrawable> mSelectHandleLeft;
    AutoPtr<IDrawable> mSelectHandleRight;
    AutoPtr<IDrawable> mSelectHandleCenter;

private:
    static const Int32 PRIORITY;

    //// Context menu entries
    static const Int32 ID_SELECT_ALL = 0x0102001f/*android.R.id.selectAll*/;
    static const Int32 ID_START_SELECTING_TEXT = 0x01020028 /*android.R.id.startSelectingText*/;
    static const Int32 ID_CUT = 0x01020020 /*android.R.id.cut*/;
    static const Int32 ID_COPY = 0x01020021 /*android.R.id.copy*/;
    static const Int32 ID_PASTE = 0x01020022 /*android.R.id.paste*/;
    static const Int32 ID_COPY_URL = 0x01020023 /*android.R.id.copyUrl*/;
    static const Int32 ID_SWITCH_INPUT_METHOD = 0x01020024 /*android.R.id.switchInputMethod*/;
    static const Int32 ID_ADD_TO_DICTIONARY = 0x0102002a /*android.R.id.addToDictionary*/;

    AutoPtr<IColorStateList> mTextColor;
    Int32 mCurTextColor;
    AutoPtr<IColorStateList> mHintTextColor;
    AutoPtr<IColorStateList> mLinkTextColor;
    Int32 mCurHintTextColor;
    Boolean mFreezesText;
    Boolean mFrozenWithFocus;
    Boolean mTemporaryDetach;
    Boolean mDispatchTemporaryDetach;

    Boolean mEatTouchRelease;
    Boolean mScrolled;

    AutoPtr<IEditableFactory> mEditableFactory;
    AutoPtr<ISpannableFactory> mSpannableFactory;

    Float mShadowRadius, mShadowDx, mShadowDy;

    static const Int32 PREDRAW_NOT_REGISTERED;
    static const Int32 PREDRAW_PENDING;
    static const Int32 PREDRAW_DONE;
    Int32 mPreDrawState;

    TextUtilsTruncateAt mEllipsize;

    // Enum for the "typeface" XML parameter.
    // TODO: How can we get this from the XML instead of hardcoding it here?
    static const Int32 SANS;
    static const Int32 SERIF;
    static const Int32 MONOSPACE;

    // Bitfield for the "numeric" XML parameter.
    // TODO: How can we get this from the XML instead of hardcoding it here?
    static const Int32 SIGNED;
    static const Int32 DECIMAL;

    class Drawables
    {
    public:
        AutoPtr<CRect> mCompoundRect;
        AutoPtr<IDrawable> mDrawableTop, mDrawableBottom, mDrawableLeft, mDrawableRight;
        Int32 mDrawableSizeTop, mDrawableSizeBottom, mDrawableSizeLeft, mDrawableSizeRight;
        Int32 mDrawableWidthTop, mDrawableWidthBottom, mDrawableHeightLeft, mDrawableHeightRight;
        Int32 mDrawablePadding;
    };

    Drawables* mDrawables;

    AutoPtr<ICharSequence> mError;
    Boolean mErrorWasChanged;

    /*private static class ErrorPopup extends PopupWindow {
        private Boolean mAbove = FALSE;
        private final TextView mView;

        ErrorPopup(TextView v, Int32 width, Int32 height) {
            super(v, width, height);
            mView = v;
        }

        void fixDirection(Boolean above) {
            mAbove = above;

            if (above) {
                mView.setBackgroundResource(com.android.internal.R.drawable.popup_inline_error_above);
            } else {
                mView.setBackgroundResource(com.android.internal.R.drawable.popup_inline_error);
            }
        }

        @Override
        public void update(Int32 x, Int32 y, Int32 w, Int32 h, Boolean force) {
            super.update(x, y, w, h, force);

            Boolean above = isAboveAnchor();
            if (above != mAbove) {
                fixDirection(above);
            }
        }
    }*/
    //ErrorPopup mPopup;
    /**
     * This flag is set if the TextView tries to display an error before it
     * is attached to the window (so its position is still unknown).
     * It causes the error to be shown later, when onAttachedToWindow()
     * is called.
     */
    Boolean mShowErrorAfterAttach;

    AutoPtr<CharWrapper> mCharWrapper;

    Boolean mSelectionMoved;
    Boolean mTouchFocusSelected;

    //Marquee mMarquee;
    Boolean mRestartMarquee;

    Int32 mMarqueeRepeatLimit;

    // Set when this TextView gained focus with some text selected. Will start selection mode.
    Boolean mCreatedWithASelection;

    Boolean mNoContextMenuOnUp;

    /*
     * Kick-start the font cache for the zygote process (to pay the cost of
     * initializing freetype for our default font only once).
     */
    //static {
    //    Paint p = new Paint();
    //    p.setAntiAlias(true);
    //    // We don't care about the result, just the side-effect of measuring.
    //    p.measureText("H");
    //}

    // Context menu entries
    //static const Int32 ID_SELECT_ALL = android.R.id.selectAll;
    //static const Int32 ID_START_SELECTING_TEXT = android.R.id.startSelectingText;
    //static const Int32 ID_CUT = android.R.id.cut;
    //static const Int32 ID_COPY = android.R.id.copy;
    //static const Int32 ID_PASTE = android.R.id.paste;
    //static const Int32 ID_COPY_URL = android.R.id.copyUrl;
    //static const Int32 ID_SWITCH_INPUT_METHOD = android.R.id.switchInputMethod;
    //static const Int32 ID_ADD_TO_DICTIONARY = android.R.id.addToDictionary;

    AutoPtr<ICharSequence>  mText;
    AutoPtr<ICharSequence>  mTransformed;
    BufferType              mBufferType;

    Int32                   mInputType;// = EditorInfo.TYPE_NULL;
    AutoPtr<ICharSequence>  mHint;
    AutoPtr<ILayout>        mHintLayout;

    AutoPtr<IKeyListener>   mInput;

    AutoPtr<IMovementMethod> mMovement;
    AutoPtr<ITransformationMethod>    mTransformation;
    AutoPtr<ChangeWatcher>           mChangeWatcher;

    List<AutoPtr<ITextWatcher> >  mListeners;

    // display attributes
    AutoPtr<CTextPaint>     mTextPaint;
    Boolean                 mUserSetTextScaleX;
    AutoPtr<IPaint>         mHighlightPaint;
    Int32                   mHighlightColor;
    AutoPtr<ILayout>        mLayout;

    Int64                   mShowCursor;
    AutoPtr<Blink>          mBlink;
    Boolean                 mCursorVisible;

    // Cursor Controllers. Null when disabled.
    AutoPtr<CursorController> mInsertionPointCursorController;
    AutoPtr<CursorController> mSelectionModifierCursorController;
    Boolean                 mInsertionControllerEnabled;
    Boolean                 mSelectionControllerEnabled;
    Boolean                 mInBatchEditControllers;
    Boolean                 mIsInTextSelectionMode;
    // These are needed to desambiguate a long click. If the long click comes from ones of these, we
    // select from the current cursor position. Otherwise, select from long pressed position.
    Boolean                 mDPadCenterIsDown;
    Boolean                 mEnterKeyIsDown;
    Boolean                 mContextMenuTriggeredByKey;
    // Created once and shared by different CursorController helper methods.
    // Only one cursor controller is active at any time which prevent race conditions.
    static AutoPtr<CRect>   sCursorControllerTempRect;

    Boolean                 mSelectAllOnFocus;

    Int32                   mGravity;
    Boolean                 mHorizontallyScrolling;

    Int32                   mAutoLinkMask;
    Boolean                 mLinksClickable;

    Float                   mSpacingMult;
    Float                   mSpacingAdd;

    static const Int32      LINES;
    static const Int32      EMS;
    static const Int32      PIXELS;

    Int32                   mMaximum;
    Int32                   mMaxMode;
    Int32                   mMinimum;
    Int32                   mMinMode;

    Int32                   mMaxWidth;
    Int32                   mMaxWidthMode;
    Int32                   mMinWidth;
    Int32                   mMinWidthMode;

    Boolean                 mSingleLine;
    Int32                   mDesiredHeightAtMeasure;
    Boolean                 mIncludePad;

    // tmp primitives, so we don't alloc them on each draw
    AutoPtr<IPath>          mHighlightPath;
    Boolean                 mHighlightPathBogus;

    static const AutoPtr<CRectF> sTempRect;
    static Mutex sTempRectLock;

    // XXX should be much larger
    static const Int32 VERY_WIDE;

    static const Int32 BLINK;

    static const Int32 ANIMATED_SCROLL_GAP;
    Int64 mLastScroll;
    Scroller* mScroller;

    AutoPtr<IBoringLayoutMetrics> mBoring;
    AutoPtr<IBoringLayoutMetrics> mHintBoring;

    AutoPtr<IBoringLayout> mSavedLayout, mSavedHintLayout;

    /*static final InputFilter[] NO_FILTERS = new InputFilter[0];
    InputFilter[] mFilters = NO_FILTERS;
    static final Spanned EMPTY_SPANNED = new SpannedString("");*/

    static const Int32 EXTRACT_NOTHING;
    static const Int32 EXTRACT_UNKNOWN;

    static const AutoPtr<IBoringLayoutMetrics> UNKNOWN_BORING;

    //private static final class Marquee extends Handler {
    //    // TODO: Add an option to configure this
    //    private static final Float MARQUEE_DELTA_MAX = 0.07f;
    //    private static final Int32 MARQUEE_DELAY = 1200;
    //    private static final Int32 MARQUEE_RESTART_DELAY = 1200;
    //    private static final Int32 MARQUEE_RESOLUTION = 1000 / 30;
    //    private static final Int32 MARQUEE_PIXELS_PER_SECOND = 30;

    //    private static final byte MARQUEE_STOPPED = 0x0;
    //    private static final byte MARQUEE_STARTING = 0x1;
    //    private static final byte MARQUEE_RUNNING = 0x2;

    //    private static final Int32 MESSAGE_START = 0x1;
    //    private static final Int32 MESSAGE_TICK = 0x2;
    //    private static final Int32 MESSAGE_RESTART = 0x3;

    //    private final WeakReference<TextView> mView;

    //    private byte mStatus = MARQUEE_STOPPED;
    //    private final Float mScrollUnit;
    //    private Float mMaxScroll;
    //    Float mMaxFadeScroll;
    //    private Float mGhostStart;
    //    private Float mGhostOffset;
    //    private Float mFadeStop;
    //    private Int32 mRepeatLimit;

    //    Float mScroll;

    //    Marquee(TextView v) {
    //        final Float density = v.getContext().getResources().getDisplayMetrics().density;
    //        mScrollUnit = (MARQUEE_PIXELS_PER_SECOND * density) / MARQUEE_RESOLUTION;
    //        mView = new WeakReference<TextView>(v);
    //    }

    //    @Override
    //    public void handleMessage(Message msg) {
    //        switch (msg.what) {
    //            case MESSAGE_START:
    //                mStatus = MARQUEE_RUNNING;
    //                tick();
    //                break;
    //            case MESSAGE_TICK:
    //                tick();
    //                break;
    //            case MESSAGE_RESTART:
    //                if (mStatus == MARQUEE_RUNNING) {
    //                    if (mRepeatLimit >= 0) {
    //                        mRepeatLimit--;
    //                    }
    //                    start(mRepeatLimit);
    //                }
    //                break;
    //        }
    //    }

    //    void tick() {
    //        if (mStatus != MARQUEE_RUNNING) {
    //            return;
    //        }

    //        removeMessages(MESSAGE_TICK);

    //        final TextView textView = mView.get();
    //        if (textView != NULL && (textView.isFocused() || textView.isSelected())) {
    //            mScroll += mScrollUnit;
    //            if (mScroll > mMaxScroll) {
    //                mScroll = mMaxScroll;
    //                sendEmptyMessageDelayed(MESSAGE_RESTART, MARQUEE_RESTART_DELAY);
    //            } else {
    //                sendEmptyMessageDelayed(MESSAGE_TICK, MARQUEE_RESOLUTION);
    //            }
    //            textView.invalidate();
    //        }
    //    }

    //    void stop() {
    //        mStatus = MARQUEE_STOPPED;
    //        removeMessages(MESSAGE_START);
    //        removeMessages(MESSAGE_RESTART);
    //        removeMessages(MESSAGE_TICK);
    //        resetScroll();
    //    }

    //    private void resetScroll() {
    //        mScroll = 0.0f;
    //        final TextView textView = mView.get();
    //        if (textView != NULL) textView.invalidate();
    //    }

    //    void start(Int32 repeatLimit) {
    //        if (repeatLimit == 0) {
    //            stop();
    //            return;
    //        }
    //        mRepeatLimit = repeatLimit;
    //        final TextView textView = mView.get();
    //        if (textView != NULL && textView.mLayout != NULL) {
    //            mStatus = MARQUEE_STARTING;
    //            mScroll = 0.0f;
    //            final Int32 textWidth = textView.getWidth() - textView.getCompoundPaddingLeft() -
    //                    textView.getCompoundPaddingRight();
    //            final Float lineWidth = textView.mLayout.getLineWidth(0);
    //            final Float gap = textWidth / 3.0f;
    //            mGhostStart = lineWidth - textWidth + gap;
    //            mMaxScroll = mGhostStart + textWidth;
    //            mGhostOffset = lineWidth + gap;
    //            mFadeStop = lineWidth + textWidth / 6.0f;
    //            mMaxFadeScroll = mGhostStart + lineWidth + lineWidth;

    //            textView.invalidate();
    //            sendEmptyMessageDelayed(MESSAGE_START, MARQUEE_DELAY);
    //        }
    //    }

    //    Float getGhostOffset() {
    //        return mGhostOffset;
    //    }

    //    Boolean shouldDrawLeftFade() {
    //        return mScroll <= mFadeStop;
    //    }

    //    Boolean shouldDrawGhost() {
    //        return mStatus == MARQUEE_RUNNING && mScroll > mGhostStart;
    //    }

    //    Boolean isRunning() {
    //        return mStatus == MARQUEE_RUNNING;
    //    }

    //    Boolean isStopped() {
    //        return mStatus == MARQUEE_STOPPED;
    //    }
    //}

    //class CommitSelectionReceiver extends ResultReceiver {
    //    private final Int32 mPrevStart, mPrevEnd;
    //
    //    public CommitSelectionReceiver(Int32 prevStart, Int32 prevEnd) {
    //        super(getHandler());
    //        mPrevStart = prevStart;
    //        mPrevEnd = prevEnd;
    //    }
    //
    //    @Override
    //    protected void onReceiveResult(Int32 resultCode, Bundle resultData) {
    //        // If this tap was actually used to show the IMM, leave cursor or selection unchanged
    //        // by restoring its previous position.
    //        if (resultCode == InputMethodManager.RESULT_SHOWN) {
    //            final Int32 len = mText.length();
    //            Int32 start = Math.min(len, mPrevStart);
    //            Int32 end = Math.min(len, mPrevEnd);
    //            Selection.setSelection((Spannable)mText, start, end);

    //            Boolean selectAllGotFocus = mSelectAllOnFocus && mTouchFocusSelected;
    //            if (hasSelection() && !selectAllGotFocus) {
    //                startTextSelectionMode();
    //            }
    //        }
    //    }
    //}
};

#endif //__TEXTVIEW_H__
