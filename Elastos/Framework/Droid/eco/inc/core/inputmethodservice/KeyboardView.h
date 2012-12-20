
#ifndef  _KEYBOARDVIEW_H__
#define  _KEYBOARDVIEW_H__

#include "ext/frameworkext.h"
#include <elastos/Map.h>
#include "view/GestureDetector.h"
#include "view/View.h"
#include "inputmethodservice/Keyboard.h"

class KeyboardView: public View
{
public:
    class _OnKeyboardActionListener:
        public ElRefBase,
        public IOnKeyboardActionListener
    {
    public:
        _OnKeyboardActionListener(
            /* [in] */ KeyboardView* host);

        ~_OnKeyboardActionListener();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
           /* [in] */ REIID riid);

        CARAPI GetInterfaceID(
           /* [in] */ IInterface *pObject,
           /* [out] */ InterfaceID *pIID);

        CARAPI OnPress(
            /* [in] */ Int32 primaryCode);

        CARAPI OnRelease(
            /* [in] */ Int32 primaryCode);

        CARAPI OnKey(
            /* [in] */ Int32 primaryCode,
            /* [in] */ ArrayOf<Int32>* keyCodes);

        CARAPI OnText(
            /* [in] */ ICharSequence* text);

        CARAPI SwipeLeft();

        CARAPI SwipeRight();

        CARAPI SwipeDown();

        CARAPI SwipeUp();

    private:
        KeyboardView*      mHost;
    };

private:
    class SwipeTracker: public ElRefBase
    {
    public:
        SwipeTracker();

        ~SwipeTracker();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(void) Clear();

        CARAPI_(void) AddMovement(
            /* [in] */ IMotionEvent* ev);

        CARAPI_(void) ComputeCurrentVelocity(
            /* [in] */ Int32 units);

        CARAPI_(void) ComputeCurrentVelocity(
            /* [in] */ Int32 units,
            /* [in] */ Float maxVelocity);

        CARAPI_(Float) GetXVelocity();

        CARAPI_(Float) GetYVelocity();

    private:
        CARAPI_(void) AddPoint(
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ Int64 time);

    private:
        static const Int32 NUM_PAST = 4;
        static const Int32 LONGEST_PAST_TIME = 200;

        ArrayOf<Float>* mPastX;
        ArrayOf<Float>* mPastY;
        ArrayOf<Int64>* mPastTime;
        Float mYVelocity;
        Float mXVelocity;
    };

    class _SimpleOnGestureListener:
        public ElRefBase,
        public GestureDetector::SimpleOnGestureListener
    {
    public:
        _SimpleOnGestureListener(
            /* [in] */ KeyboardView* host);

        ~_SimpleOnGestureListener();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        virtual Boolean OnFling(
            /* [in] */ IMotionEvent* e1,
            /* [in] */ IMotionEvent* e2,
            /* [in] */ Float velocityX,
            /* [in] */ Float velocityY);

    private:
        KeyboardView*   mHost;
    };

public:
    KeyboardView();

    ~KeyboardView();


    CARAPI SetOnKeyboardActionListener(
        /* [in] */ IOnKeyboardActionListener* listener);

    /**
     * Attaches a keyboard to this view. The keyboard can be switched at any time and the
     * view will re-layout itself to accommodate the keyboard.
     * @see Keyboard
     * @see #getKeyboard()
     * @param keyboard the keyboard to display in this view
     */
    CARAPI_(void) SetKeyboard(
        /* [in] */ Keyboard* keyboard);

    /**
     * Returns the current keyboard being displayed by this view.
     * @return the currently attached keyboard
     * @see #setKeyboard(Keyboard)
     */
    CARAPI_(AutoPtr<Keyboard>) GetKeyboard();

    /**
     * Sets the state of the shift key of the keyboard, if any.
     * @param shifted whether or not to enable the state of the shift key
     * @return TRUE if the shift key state changed, FALSE if there was no change
     * @see KeyboardView#isShifted()
     */
    CARAPI_(Boolean) SetShifted(
        /* [in] */ Boolean shifted);

    /**
     * Returns the state of the shift key of the keyboard, if any.
     * @return TRUE if the shift is in a pressed state, FALSE otherwise. If there is
     * no shift key on the keyboard or there is no keyboard attached, it returns FALSE.
     * @see KeyboardView#setShifted(Boolean)
     */
    CARAPI_(Boolean) IsShifted();

    /**
     * Enables or disables the key feedback popup. This is a popup that shows a magnified
     * version of the depressed key. By default the preview is enabled.
     * @param previewEnabled whether or not to enable the key feedback popup
     * @see #isPreviewEnabled()
     */
    CARAPI_(void) SetPreviewEnabled(
        /* [in] */ Boolean previewEnabled);

    /**
     * Returns the enabled state of the key feedback popup.
     * @return whether or not the key feedback popup is enabled
     * @see #setPreviewEnabled(Boolean)
     */
    CARAPI_(Boolean) IsPreviewEnabled();

    CARAPI_(void) SetVerticalCorrection(
        /* [in] */ Int32 verticalOffset);

    CARAPI_(void) SetPopupParent(
        /* [in] */ IView* v);

    CARAPI_(void) SetPopupOffset(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * When enabled, calls to {@link IOnKeyboardActionListener#onKey} will include key
     * codes for adjacent keys.  When disabled, only the primary key code will be
     * reported.
     * @param enabled whether or not the proximity correction is enabled
     */
    CARAPI_(void) SetProximityCorrectionEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Returns TRUE if proximity correction is enabled.
     */
    CARAPI_(Boolean) IsProximityCorrectionEnabled();

    /**
     * Popup keyboard close button clicked.
     * @hide
     */
    CARAPI OnClick(
        /* [in] */ IView* v);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    //@Override
    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    /**
     * Requests a redraw of the entire keyboard. Calling {@link #invalidate} is not sufficient
     * because the keyboard renders the keys to an off-screen buffer and an invalidate() only
     * draws the cached buffer.
     * @see #invalidateKey(Int32)
     */
    CARAPI_(void) InvalidateAllKeys();

    /**
     * Invalidates a key so that it will be redrawn on the next repaint. Use this method if only
     * one key is changing it's content. Any changes that affect the position or size of the key
     * may not be honored.
     * @param keyIndex the index of the key in the attached {@link Keyboard}.
     * @see #invalidateAllKeys
     */
    CARAPI_(void) InvalidateKey(
        /* [in] */ Int32 keyIndex);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* me);

    CARAPI_(void) Closing();

    //@Override
    CARAPI_(void) OnDetachedFromWindow();

    CARAPI_(Boolean) HandleBack();

protected:
    /**
     * Returns the {@link IOnKeyboardActionListener} object.
     * @return the listener attached to this keyboard
     */
    CARAPI_(AutoPtr<IOnKeyboardActionListener>) GetOnKeyboardActionListener();

    /**
     * Called when a key is Int64 pressed. By default this will open any popup keyboard associated
     * with this key through the attributes popupLayout and popupCharacters.
     * @param popupKey the key that was Int64 pressed
     * @return TRUE if the Int64 press is handled, FALSE otherwise. Subclasses should call the
     * method on the base class if the subclass doesn't wish to handle the call.
     */
    CARAPI_(Boolean) OnLongPress(
        /* [in] */ Keyboard::Key* popupKey);

    CARAPI_(void) SwipeRight();

    CARAPI_(void) SwipeLeft();

    CARAPI_(void) SwipeUp();

    CARAPI_(void) SwipeDown();


protected:
    CARAPI_(void) InitGestureDetector();

    CARAPI_(AutoPtr<ICharSequence>) AdjustCase(
        /* [in] */ ICharSequence* label);

    /**
     * Compute the average distance between adjacent keys (horizontally and vertically)
     * and square it to get the proximity threshold. We use a square here and in computing
     * the touch distance from a key's center to avoid taking a square root.
     * @param keyboard
     */
    CARAPI_(void) ComputeProximityThreshold(
        /* [in] */ Keyboard* keyboard);

    CARAPI_(void) OnBufferDraw();

    CARAPI_(Int32) GetKeyIndices(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ ArrayOf<Int32>* allKeys);

    CARAPI_(void) DetectAndSendKey(
        /* [in] */ Int32 index,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int64 eventTime);

    /**
     * Handle multi-tap keys by producing the key label for the current multi-tap state.
     */
    CARAPI_(AutoPtr<ICharSequence>) GetPreviewText(
        /* [in] */ Keyboard::Key* key);

    CARAPI_(void) ShowPreview(
        Int32 keyIndex);

    CARAPI_(void) ShowKey(
        /* [in] */ Int32 keyIndex);

    CARAPI_(Boolean) OpenPopupIfRequired(
        /* [in] */ IMotionEvent* me);

    CARAPI_(Boolean) OnModifiedTouchEvent(
        /* [in] */ IMotionEvent* me,
        /* [in] */ Boolean possiblePoly);

    CARAPI_(Boolean) RepeatKey();

    CARAPI_(void) RemoveMessages();

    CARAPI_(void) DismissPopupKeyboard();

    CARAPI_(void) ResetMultiTap();

    CARAPI_(void) CheckMultiTap(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 keyIndex);

    CARAPI_(void) HandleShowPreView(
        /* [in] */ Int32 keyIndex);

    CARAPI_(void) HandleRemovePreView();

    CARAPI_(void) HandleRepeat();

    CARAPI_(void) HandleLongPress(
        /* [in] */ IMotionEvent* obj);


protected:
    static const Boolean DEBUG = FALSE;
    static const Int32 NOT_A_KEY = -1;
    static Int32 KEY_DELETE[];// = { Keyboard::KEYCODE_DELETE };
    static Int32 LONG_PRESSABLE_STATE_SET[];// = { 0x0101023c /*R.attr.state_long_pressable*/ };

    AutoPtr<Keyboard> mKeyboard;
    Int32 mCurrentKeyIndex;
    Int32 mLabelTextSize;
    Int32 mKeyTextSize;
    Int32 mKeyTextColor;
    Float mShadowRadius;
    Int32 mShadowColor;
    Float mBackgroundDimAmount;

    AutoPtr<ITextView> mPreviewText;
    AutoPtr<IPopupWindow> mPreviewPopup;
    Int32 mPreviewTextSizeLarge;
    Int32 mPreviewOffset;
    Int32 mPreviewHeight;
    ArrayOf<Int32>* mOffsetInWindow;

    AutoPtr<IPopupWindow> mPopupKeyboard;
    AutoPtr<IView> mMiniKeyboardContainer;
    AutoPtr<IKeyboardView> mMiniKeyboard;
    Boolean mMiniKeyboardOnScreen;
    AutoPtr<IView> mPopupParent;
    Int32 mMiniKeyboardOffsetX;
    Int32 mMiniKeyboardOffsetY;
    Map<AutoPtr<Keyboard::Key>, AutoPtr<IView> > mMiniKeyboardCache;
    ArrayOf<Int32>* mWindowOffset;
    ArrayOf<AutoPtr<Keyboard::Key> >* mKeys;

    /** Listener for {@link IOnKeyboardActionListener}. */
    AutoPtr<IOnKeyboardActionListener> mKeyboardActionListener;

    static const Int32 MSG_SHOW_PREVIEW = 1;
    static const Int32 MSG_REMOVE_PREVIEW = 2;
    static const Int32 MSG_REPEAT = 3;
    static const Int32 MSG_LONGPRESS = 4;

    static const Int32 DELAY_BEFORE_PREVIEW = 0;
    static const Int32 DELAY_AFTER_PREVIEW = 70;
    static const Int32 DEBOUNCE_TIME = 70;

    Int32 mVerticalCorrection;
    Int32 mProximityThreshold;

    Boolean mPreviewCentered;
    Boolean mShowPreview;
    Boolean mShowTouchPoints;
    Int32 mPopupPreviewX;
    Int32 mPopupPreviewY;
    Int32 mWindowY;

    Int32 mLastX;
    Int32 mLastY;
    Int32 mStartX;
    Int32 mStartY;

    Boolean mProximityCorrectOn;

    AutoPtr<IPaint> mPaint;
    AutoPtr<IRect> mPadding;

    Int64 mDownTime;
    Int64 mLastMoveTime;
    Int32 mLastKey;
    Int32 mLastCodeX;
    Int32 mLastCodeY;
    Int32 mCurrentKey;
    Int32 mDownKey;
    Int64 mLastKeyTime;
    Int64 mCurrentKeyTime;
    ArrayOf<Int32>* mKeyIndices;
    GestureDetector* mGestureDetector;
    Int32 mPopupX;
    Int32 mPopupY;
    Int32 mRepeatKeyIndex;
    Int32 mPopupLayout;
    Boolean mAbortKey;
    AutoPtr<Keyboard::Key> mInvalidatedKey;
    AutoPtr<IRect> mClipRegion;
    Boolean mPossiblePoly;
    AutoPtr<SwipeTracker> mSwipeTracker;;
    Int32 mSwipeThreshold;
    Boolean mDisambiguateSwipe;

    // Variables for dealing with multiple pointers
    Int32 mOldPointerCount;
    Float mOldPointerX;
    Float mOldPointerY;

    AutoPtr<IDrawable> mKeyBackground;

    static const Int32 REPEAT_INTERVAL = 50; // ~20 keys per second
    static const Int32 REPEAT_START_DELAY = 400;
    static const Int32 LONGPRESS_TIMEOUT;

    static Int32 MAX_NEARBY_KEYS;
    ArrayOf<Int32>* mDistances;

    // For multi-tap
    Int32 mLastSentIndex;
    Int32 mTapCount;
    Int64 mLastTapTime;
    Boolean mInMultiTap;
    static const Int32 MULTITAP_INTERVAL = 800; // milliseconds

    //TODO
    //StringBuilder mPreviewLabel = new StringBuilder(1);
    StringBuf_<1> mPreviewLabel;// = new StringBuilder(1);

    /** Whether the keyboard bitmap needs to be redrawn before it's blitted. **/
    Boolean mDrawPending;
    /** The dirty region in the keyboard bitmap */
    AutoPtr<IRectF> mDirtyRect;
    /** The keyboard bitmap for faster updates */
    AutoPtr<IBitmap> mBuffer;
    /** Notes if the keyboard just changed, so that we could possibly reallocate the mBuffer. */
    Boolean mKeyboardChanged;
    /** The canvas for the above mutable keyboard bitmap */
    AutoPtr<ICanvas> mCanvas;

    AutoPtr<IApartment> mHandler;
    // Handler mHandler = new Handler() {
    //     @Override
    //     public void handleMessage(Message msg) {
    //         switch (msg.what) {
    //             case MSG_SHOW_PREVIEW:
    //                 showKey(msg.arg1);
    //                 break;
    //             case MSG_REMOVE_PREVIEW:
    //                 mPreviewText.setVisibility(INVISIBLE);
    //                 break;
    //             case MSG_REPEAT:
    //                 if (repeatKey()) {
    //                     Message repeat = Message.obtain(this, MSG_REPEAT);
    //                     sendMessageDelayed(repeat, REPEAT_INTERVAL);
    //                 }
    //                 break;
    //             case MSG_LONGPRESS:
    //                 openPopupIfRequired((MotionEvent) msg.obj);
    //                 break;
    //         }
    //     }
    // };

    Int64 mOldEventTime;
    Boolean mUsedVelocity;

friend class _OnKeyboardActionListener;
friend class _SimpleOnGestureListener;
};

#endif  //_KEYBOARDVIEW_H__
