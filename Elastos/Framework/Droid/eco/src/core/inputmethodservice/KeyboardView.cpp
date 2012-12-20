
#include "inputmethodservice/KeyboardView.h"
#include <elastos/Character.h>
#include "graphics/CBitmapFactory.h"
#include <elastos/Math.h>
#include "view/ViewConfiguration.h"
#include "os/CApartment.h"
#include "view/CMotionEvent.h"
#include "graphics/CCanvas.h"
#include "graphics/CRectF.h"
#include "graphics/Typeface.h"
#include "view/CViewGroupLayoutParams.h"

using namespace Elastos::Core;


const Int32 KeyboardView::SwipeTracker::NUM_PAST;
const Int32 KeyboardView::SwipeTracker::LONGEST_PAST_TIME;
const Boolean KeyboardView::DEBUG;
const Int32 KeyboardView::NOT_A_KEY;
Int32 KeyboardView::KEY_DELETE[] = { Keyboard::KEYCODE_DELETE };
Int32 KeyboardView::LONG_PRESSABLE_STATE_SET[] = { 0x0101023c /*R.attr.state_long_pressable*/ };

const Int32 KeyboardView::MSG_SHOW_PREVIEW;
const Int32 KeyboardView::MSG_REMOVE_PREVIEW;
const Int32 KeyboardView::MSG_REPEAT;
const Int32 KeyboardView::MSG_LONGPRESS;

const Int32 KeyboardView::DELAY_BEFORE_PREVIEW;
const Int32 KeyboardView::DELAY_AFTER_PREVIEW;
const Int32 KeyboardView::DEBOUNCE_TIME;

const Int32 KeyboardView::REPEAT_INTERVAL; // ~20 keys per second
const Int32 KeyboardView::REPEAT_START_DELAY;
const Int32 KeyboardView::LONGPRESS_TIMEOUT = ViewConfiguration::GetLongPressTimeout();
Int32 KeyboardView::MAX_NEARBY_KEYS = 12;
const Int32 KeyboardView::MULTITAP_INTERVAL;


KeyboardView::_OnKeyboardActionListener::_OnKeyboardActionListener(
	/* [in] */ KeyboardView* host):
	mHost(host)
{

}

KeyboardView::_OnKeyboardActionListener::~_OnKeyboardActionListener()
{

}

UInt32 KeyboardView::_OnKeyboardActionListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 KeyboardView::_OnKeyboardActionListener::Release()
{
    return ElRefBase::Release();
}

PInterface KeyboardView::_OnKeyboardActionListener::Probe(
   /* [in] */ REIID riid)
{
   if (riid == EIID_IOnKeyboardActionListener) {
       return (IOnKeyboardActionListener*)this;
   }

   return NULL;
}

ECode KeyboardView::_OnKeyboardActionListener::GetInterfaceID(
   /* [in] */ IInterface *pObject,
   /* [out] */ InterfaceID *pIID)
{
   VALIDATE_NOT_NULL(pIID);

   if (pObject == (IInterface*)(IOnKeyboardActionListener*)this) {
       *pIID = EIID_IOnKeyboardActionListener;
   }
   else {
       return E_INVALID_ARGUMENT;
   }

   return NOERROR;
}

ECode KeyboardView::_OnKeyboardActionListener::OnPress(
    /* [in] */ Int32 primaryCode)
{
	assert(mHost->mKeyboardActionListener != NULL);
	return mHost->mKeyboardActionListener->OnPress(primaryCode);
}

ECode KeyboardView::_OnKeyboardActionListener::OnRelease(
    /* [in] */ Int32 primaryCode)
{
	assert(mHost->mKeyboardActionListener != NULL);
	return mHost->mKeyboardActionListener->OnRelease(primaryCode);
}

ECode KeyboardView::_OnKeyboardActionListener::OnKey(
    /* [in] */ Int32 primaryCode,
    /* [in] */ ArrayOf<Int32>* keyCodes)
{
	assert(mHost->mKeyboardActionListener != NULL && mHost != NULL);
    mHost->mKeyboardActionListener->OnKey(primaryCode, keyCodes);
    mHost->DismissPopupKeyboard();
    return NOERROR;
}

ECode KeyboardView::_OnKeyboardActionListener::OnText(
    /* [in] */ ICharSequence* text)
{
	assert(mHost->mKeyboardActionListener != NULL && mHost != NULL);
    mHost->mKeyboardActionListener->OnText(text);
    mHost->DismissPopupKeyboard();
    return NOERROR;
}

ECode KeyboardView::_OnKeyboardActionListener::SwipeLeft()
{
    return NOERROR;
}

ECode KeyboardView::_OnKeyboardActionListener::SwipeRight()
{
    return NOERROR;
}

ECode KeyboardView::_OnKeyboardActionListener::SwipeDown()
{
    return NOERROR;
}

ECode KeyboardView::_OnKeyboardActionListener::SwipeUp()
{
    return NOERROR;
}

KeyboardView::SwipeTracker::SwipeTracker()
{
    mPastX = ArrayOf<Float>::Alloc(NUM_PAST);
    mPastY = ArrayOf<Float>::Alloc(NUM_PAST);
    mPastTime = ArrayOf<Int64>::Alloc(NUM_PAST);
    mYVelocity = 0.0;
    mXVelocity = 0.0;
}

KeyboardView::SwipeTracker::~SwipeTracker()
{
	if (mPastX != NULL) {
		ArrayOf<Float>::Free(mPastX);
	}

	if (mPastY != NULL) {
		ArrayOf<Float>::Free(mPastY);
	}

	if (mPastTime != NULL) {
		ArrayOf<Int64>::Free(mPastTime);
	}
}

UInt32 KeyboardView::SwipeTracker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 KeyboardView::SwipeTracker::Release()
{
    return ElRefBase::Release();
}

void KeyboardView::SwipeTracker::Clear()
{
    (*mPastTime)[0] = 0;
}

void KeyboardView::SwipeTracker::AddMovement(
    /* [in] */ IMotionEvent* ev)
{
    Int64 time = 0;
    ev->GetEventTime(&time);
    Int32 N = 0;
    ev->GetHistorySize(&N);

    Float x = 0.0, y = 0.0;
    Int64 tmpTime = 0;
    for (Int32 i=0; i<N; i++) {
    	ev->GetHistoricalX(i, &x);
    	ev->GetHistoricalY(i, &y);
    	ev->GetHistoricalEventTime(i, &tmpTime);
        AddPoint(x, y, tmpTime);
    }

    ev->GetX(&x);
    ev->GetY(&y);
    AddPoint(x, y, time);
}

void KeyboardView::SwipeTracker::ComputeCurrentVelocity(
    /* [in] */ Int32 units)
{
    ComputeCurrentVelocity(units, Math::FLOAT_MAX_VALUE);
}

void KeyboardView::SwipeTracker::ComputeCurrentVelocity(
    /* [in] */ Int32 units,
    /* [in] */ Float maxVelocity)
{
    Float oldestX = (*mPastX)[0];
    Float oldestY = (*mPastY)[0];
    Int64 oldestTime = (*mPastTime)[0];
    Float accumX = 0;
    Float accumY = 0;
    Int32 N=0;
    while (N < NUM_PAST) {
        if ((*mPastTime)[N] == 0) {
            break;
        }
        N++;
    }

    for (Int32 i=1; i < N; i++) {
        Int32 dur = (Int32)((*mPastTime)[i] - oldestTime);
        if (dur == 0) {
        	continue;
        }

        Float dist = (*mPastX)[i] - oldestX;
        Float vel = (dist / dur) * units;   // pixels/frame.
        if (accumX == 0) {
        	accumX = vel;
        }
        else {
        	accumX = (accumX + vel) * .5f;
        }

        dist = (*mPastY)[i] - oldestY;
        vel = (dist / dur) * units;   // pixels/frame.
        if (accumY == 0) {
        	accumY = vel;
        }
        else {
        	accumY = (accumY + vel) * .5f;
        }
    }
    mXVelocity = accumX < 0.0f ? Math::Max(accumX, -maxVelocity)
            : Math::Min(accumX, maxVelocity);
    mYVelocity = accumY < 0.0f ? Math::Max(accumY, -maxVelocity)
            : Math::Min(accumY, maxVelocity);
}

Float KeyboardView::SwipeTracker::GetXVelocity()
{
    return mXVelocity;
}

Float KeyboardView::SwipeTracker::GetYVelocity()
{
    return mYVelocity;
}

void KeyboardView::SwipeTracker::AddPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 time)
{
    Int32 drop = -1;
    Int32 i = 0;
    assert(mPastTime != NULL);

    for (i=0; i<NUM_PAST; i++) {
        if ((*mPastTime)[i] == 0) {
            break;
        } else if ((*mPastTime)[i] < time - LONGEST_PAST_TIME) {
            drop = i;
        }
    }

    if (i == NUM_PAST && drop < 0) {
        drop = 0;
    }

    if (drop == i) {
    	drop--;
    }

    if (drop >= 0) {
        const Int32 start = drop + 1;
        const Int32 count = NUM_PAST - drop - 1;
        // System.arraycopy(mPastX, start, mPastX, 0, count);
        memcpy(mPastX->GetPayload() + start, mPastX->GetPayload(), count * sizeof(Float));
        // System.arraycopy(mPastY, start, mPastY, 0, count);
        memcpy(mPastY->GetPayload() + start, mPastY->GetPayload(), count * sizeof(Float));
        // System.arraycopy(mPastTime, start, mPastTime, 0, count);
        memcpy(mPastTime->GetPayload() + start, mPastTime->GetPayload(), count * sizeof(Int64));

        i -= (drop+1);
    }

    (*mPastX)[i] = x;
    (*mPastY)[i] = y;
    (*mPastTime)[i] = time;
    i++;

    if (i < NUM_PAST) {
        (*mPastTime)[i] = 0;
    }
}

KeyboardView::_SimpleOnGestureListener::_SimpleOnGestureListener(
	/* [in] */ KeyboardView* host):
	mHost(host)
{

}

KeyboardView::_SimpleOnGestureListener::~_SimpleOnGestureListener()
{

}

UInt32 KeyboardView::_SimpleOnGestureListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 KeyboardView::_SimpleOnGestureListener::Release()
{
    return ElRefBase::Release();
}

Boolean KeyboardView::_SimpleOnGestureListener::OnFling(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY)
{
    if (mHost->mPossiblePoly) {
    	return FALSE;
    }

    const Float absX = Math::Abs(velocityX);
    const Float absY = Math::Abs(velocityY);

    Float tm1 = 0.0, tm2 = 0.0;
    e2->GetX(&tm2);
    e1->GetX(&tm1);
    Float deltaX = tm2 - tm1;

    e2->GetY(&tm2);
    e1->GetY(&tm1);
    Float deltaY = tm2 - tm1;
    Int32 travelX = mHost->GetWidth() / 2; // Half the keyboard width
    Int32 travelY = mHost->GetHeight() / 2; // Half the keyboard height

    mHost->mSwipeTracker->ComputeCurrentVelocity(1000);
    const Float endingVelocityX = mHost->mSwipeTracker->GetXVelocity();
    const Float endingVelocityY = mHost->mSwipeTracker->GetYVelocity();
    Boolean sendDownKey = FALSE;

    if (velocityX > mHost->mSwipeThreshold && absY < absX && deltaX > travelX) {
        if (mHost->mDisambiguateSwipe && endingVelocityX < velocityX / 4) {
            sendDownKey = TRUE;
        } else {
            mHost->SwipeRight();
            return TRUE;
        }
    } else if (velocityX < - mHost->mSwipeThreshold && absY < absX && deltaX < -travelX) {
        if (mHost->mDisambiguateSwipe && endingVelocityX > velocityX / 4) {
            sendDownKey = TRUE;
        } else {
            mHost->SwipeLeft();
            return TRUE;
        }
    } else if (velocityY < - mHost->mSwipeThreshold && absX < absY && deltaY < -travelY) {
        if (mHost->mDisambiguateSwipe && endingVelocityY > velocityY / 4) {
            sendDownKey = TRUE;
        } else {
            mHost->SwipeUp();
            return TRUE;
        }
    } else if (velocityY > mHost->mSwipeThreshold && absX < absY / 2 && deltaY > travelY) {
        if (mHost->mDisambiguateSwipe && endingVelocityY < velocityY / 4) {
            sendDownKey = TRUE;
        } else {
            mHost->SwipeDown();
            return TRUE;
        }
    }

    if (sendDownKey) {
    	Int64 time = 0;
    	e1->GetEventTime(&time);
        mHost->DetectAndSendKey(mHost->mDownKey, mHost->mStartX, mHost->mStartY, time);
    }

    return FALSE;
}

KeyboardView::KeyboardView()
{
	mCurrentKeyIndex = NOT_A_KEY;
	mLabelTextSize = 0;
	mKeyTextSize = 0;
	mKeyTextColor = 0;
	mShadowRadius = 0.0;
	mShadowColor = 0;
	mBackgroundDimAmount = 0.0;
	mPreviewTextSizeLarge = 0;
	mPreviewOffset = 0;
	mPreviewHeight = 0;
	mOffsetInWindow = NULL;
	mMiniKeyboardOnScreen = FALSE;
	mMiniKeyboardOffsetX  = 0;
	mMiniKeyboardOffsetY = 0;
	mWindowOffset = NULL;
	mKeys = NULL;
	mVerticalCorrection = 0;
	mProximityThreshold = 0;
	mPreviewCentered = FALSE;
	mShowPreview = TRUE;
	mShowTouchPoints = TRUE;
	mPopupPreviewX = 0;
	mPopupPreviewY = 0;
	mWindowY = 0;
	mLastX = 0;
	mLastY = 0;
	mStartX = 0;
	mStartY = 0;
	mProximityCorrectOn = FALSE;
	mDownTime = 0;
	mLastMoveTime = 0;
	mLastKey = 0;
	mLastCodeX = 0;
	mLastCodeY = 0;
	mCurrentKey = NOT_A_KEY;
	mDownKey = NOT_A_KEY;
	mLastKeyTime = 0;
	mCurrentKeyTime = 0;
	mKeyIndices = ArrayOf<Int32>::Alloc(12);
	mGestureDetector = NULL;
	mPopupX = 0;
	mPopupY = 0;
	mRepeatKeyIndex = NOT_A_KEY;
	mPopupLayout = 0;
	mAbortKey = FALSE;
	CRect::New(0, 0, 0, 0, (IRect**) &mClipRegion);
	mPossiblePoly = FALSE;
	mSwipeTracker = new SwipeTracker();
	mSwipeThreshold = 0;
	mDisambiguateSwipe = FALSE;
	mOldPointerCount = 1;
	mOldPointerX = 0.0;
	mOldPointerY = 0.0;
	mDistances = ArrayOf<Int32>::Alloc(MAX_NEARBY_KEYS);
	mLastSentIndex = 0;
	mTapCount = 0;
	mLastTapTime = 0;
	mInMultiTap = FALSE;
	mDrawPending = FALSE;
	CRectF::New((IRectF**) &mDirtyRect);
	mKeyboardChanged = FALSE;
	CApartment::New(TRUE, (IApartment**)&mHandler);
    mHandler->Start(ApartmentAttr_New);
    mOldEventTime = 0;
    mUsedVelocity = FALSE;
}

KeyboardView::~KeyboardView()
{
	if (mKeyIndices != NULL) {
		ArrayOf<Int32>::Free(mKeyIndices);
	}

	if (mOffsetInWindow != NULL) {
		ArrayOf<Int32>::Free(mOffsetInWindow);
	}

	if (mWindowOffset != NULL) {
		ArrayOf<Int32>::Free(mWindowOffset);
	}

	if (mKeys != NULL) {
		ArrayOf<AutoPtr<Keyboard::Key> >::Free(mKeys);
	}

	if (mKeyIndices != NULL) {
		ArrayOf<Int32>::Free(mKeyIndices);
	}

	if (mDistances != NULL) {
		ArrayOf<Int32>::Free(mDistances);
	}
}

ECode KeyboardView::SetOnKeyboardActionListener(
    /* [in] */ IOnKeyboardActionListener* listener)
{
    mKeyboardActionListener = listener;
    return NOERROR;
}

void KeyboardView::SetKeyboard(
    /* [in] */ Keyboard* keyboard)
{
    if (mKeyboard != NULL) {
        ShowPreview(NOT_A_KEY);
    }

    // Remove any pending messages
    RemoveMessages();
    mKeyboard = keyboard;
    List<AutoPtr<Keyboard::Key> > keys = mKeyboard->GetKeys();

    //TODO
    //mKeys = keys.toArray(new Key[keys.size()]);
    mKeys = ArrayOf<AutoPtr<Keyboard::Key> >::Alloc(keys.GetSize());
    List< AutoPtr<Keyboard::Key> >::Iterator it;
    Int32 pos = 0;
	for (it = keys.Begin(); it != keys.End() && pos < (Int32)keys.GetSize(); it++, pos ++) {
		(*mKeys)[pos] = *it;
	}

    RequestLayout();
    // Hint to reallocate the buffer if the size changed
    mKeyboardChanged = TRUE;
    InvalidateAllKeys();
    ComputeProximityThreshold(keyboard);
    mMiniKeyboardCache.Clear(); // Not really necessary to do every time, but will free up views
    // Switching to a different keyboard should abort any pending keys so that the key up
    // doesn't get delivered to the old or new keyboard
    mAbortKey = TRUE; // Until the next ACTION_DOWN
}

AutoPtr<Keyboard> KeyboardView::GetKeyboard()
{
    return mKeyboard;
}

/**
 * Sets the state of the shift key of the keyboard, if any.
 * @param shifted whether or not to enable the state of the shift key
 * @return TRUE if the shift key state changed, FALSE if there was no change
 * @see KeyboardView#isShifted()
 */
Boolean KeyboardView::SetShifted(
    /* [in] */ Boolean shifted)
{
    if (mKeyboard != NULL) {
        if (mKeyboard->SetShifted(shifted)) {
            // The whole keyboard probably needs to be redrawn
            InvalidateAllKeys();
            return TRUE;
        }
    }

    return FALSE;
}

Boolean KeyboardView::IsShifted()
{
    if (mKeyboard != NULL) {
        return mKeyboard->IsShifted();
    }

    return FALSE;
}

void KeyboardView::SetPreviewEnabled(
    /* [in] */ Boolean previewEnabled)
{
    mShowPreview = previewEnabled;
}

Boolean KeyboardView::IsPreviewEnabled()
{
    return mShowPreview;
}

void KeyboardView::SetVerticalCorrection(
    /* [in] */ Int32 verticalOffset)
{

}

void KeyboardView::SetPopupParent(
    /* [in] */ IView* v)
{
    mPopupParent = v;
}

void KeyboardView::SetPopupOffset(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mMiniKeyboardOffsetX = x;
    mMiniKeyboardOffsetY = y;

    Boolean showing = FALSE;
    mPreviewPopup->IsShowing(&showing);
    if (showing) {
        mPreviewPopup->Dismiss();
    }
}

void KeyboardView::SetProximityCorrectionEnabled(
    /* [in] */ Boolean enabled)
{
    mProximityCorrectOn = enabled;
}

/**
 * Returns TRUE if proximity correction is enabled.
 */
Boolean KeyboardView::IsProximityCorrectionEnabled()
{
    return mProximityCorrectOn;
}

/**
 * Popup keyboard close button clicked.
 * @hide
 */
ECode KeyboardView::OnClick(
    /* [in] */ IView* v)
{
    DismissPopupKeyboard();
    return NOERROR;
}

void KeyboardView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // Round up a little
    if (mKeyboard == NULL) {
        SetMeasuredDimension(mPaddingLeft + mPaddingRight, mPaddingTop + mPaddingBottom);
    } else {
        Int32 width = mKeyboard->GetMinWidth() + mPaddingLeft + mPaddingRight;
        if (View::MeasureSpec::GetSize(widthMeasureSpec) < width + 10) {
            width = View::MeasureSpec::GetSize(widthMeasureSpec);
        }

        SetMeasuredDimension(width, mKeyboard->GetHeight() + mPaddingTop + mPaddingBottom);
    }
}

void KeyboardView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    View::OnSizeChanged(w, h, oldw, oldh);
    // Release the buffer, if any and it will be reallocated on the next draw
    mBuffer = NULL;
}

//@Override
void KeyboardView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    View::OnDraw(canvas);

    if (mDrawPending || mBuffer == NULL || mKeyboardChanged) {
        OnBufferDraw();
    }

    canvas->DrawBitmap(mBuffer, 0, 0, NULL);
}

void KeyboardView::InvalidateAllKeys()
{
    mDirtyRect->Union(0, 0, GetWidth(), GetHeight());
    mDrawPending = TRUE;
    Invalidate();
}

void KeyboardView::InvalidateKey(
    /* [in] */ Int32 keyIndex)
{
    if (mKeys == NULL) return;
    if (keyIndex < 0 || keyIndex >= mKeys->GetLength()) {
        return;
    }

    AutoPtr<Keyboard::Key> key = (*mKeys)[keyIndex];
    mInvalidatedKey = key;
    mDirtyRect->Union(key->mX + mPaddingLeft, key->mY + mPaddingTop,
            key->mX + key->mWidth + mPaddingLeft, key->mY + key->mHeight + mPaddingTop);

    OnBufferDraw();
    Invalidate(key->mX + mPaddingLeft, key->mY + mPaddingTop,
            key->mX + key->mWidth + mPaddingLeft, key->mY + key->mHeight + mPaddingTop);
}

Boolean KeyboardView::OnTouchEvent(
    /* [in] */ IMotionEvent* me)
{
    // Convert multi-pointer up/down events to single up/down events to
    // deal with the typical multi-pointer behavior of two-thumb typing
    Int32 pointerCount = 0;
    me->GetPointerCount(&pointerCount);

    Int32 action = 0;
    me->GetAction(&action);

    Boolean result = FALSE;
    Int64 now = 0;
    me->GetEventTime(&now);

    if (pointerCount != mOldPointerCount) {
        if (pointerCount == 1) {
            // Send a down event for the latest pointer
            AutoPtr<CMotionEvent> down;
            Float tX = 0.0, tY = 0.0;
            me->GetX(&tX);
            me->GetY(&tY);

            Int32 tState = 0;
            me->GetMetaState(&tState);
            CMotionEvent::Obtain(now, now, KeyEvent_ACTION_DOWN,
                    tX, tY, tState, (CMotionEvent**) &down);

            result = OnModifiedTouchEvent(down, FALSE);
            down->Recycle();

            // If it's an up action, then deliver the up as well.
            if (action == KeyEvent_ACTION_UP) {
                result = OnModifiedTouchEvent(me, TRUE);
            }
        } else {
            Int32 tState = 0;
            me->GetMetaState(&tState);
            // Send an up event for the last pointer
            AutoPtr<CMotionEvent> up;
            CMotionEvent::Obtain(now, now, KeyEvent_ACTION_UP,
                    mOldPointerX, mOldPointerY, tState, (CMotionEvent**) &up);

            result = OnModifiedTouchEvent(up, TRUE);
            up->Recycle();
        }
    } else {
        if (pointerCount == 1) {
            result = OnModifiedTouchEvent(me, FALSE);
            me->GetX(&mOldPointerX);
            me->GetY(&mOldPointerY);
        } else {
            // Don't do anything when 2 pointers are down and moving.
            result = TRUE;
        }
    }

    mOldPointerCount = pointerCount;
    return result;
}

void KeyboardView::Closing()
{
	assert(mPreviewPopup != NULL);
	Boolean showing = FALSE;
	mPreviewPopup->IsShowing(&showing);
    if (showing) {
        mPreviewPopup->Dismiss();
    }

    RemoveMessages();

    DismissPopupKeyboard();
    mBuffer = NULL;
    mCanvas = NULL;
    mMiniKeyboardCache.Clear();
}

void KeyboardView::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();
    Closing();
}

Boolean KeyboardView::HandleBack()
{
	assert(mPopupKeyboard != NULL);
	Boolean showing = FALSE;
	mPopupKeyboard->IsShowing(&showing);

    if (showing) {
        DismissPopupKeyboard();
        return TRUE;
    }

    return FALSE;
}

AutoPtr<IOnKeyboardActionListener> KeyboardView::GetOnKeyboardActionListener()
{
    return mKeyboardActionListener;
}

Boolean KeyboardView::OnLongPress(
    /* [in] */ Keyboard::Key* popupKey)
{
    Int32 popupKeyboardId = popupKey->mPopupResId;

    if (popupKeyboardId != 0) {
        mMiniKeyboardContainer = mMiniKeyboardCache[popupKey];
        if (mMiniKeyboardContainer == NULL) {
            AutoPtr<ILayoutInflater> inflater;
            GetContext()->GetSystemService(
                    Context_LAYOUT_INFLATER_SERVICE, (IInterface**) &inflater);

            inflater->Inflate(mPopupLayout, NULL, (IView**) &mMiniKeyboardContainer);

            mMiniKeyboardContainer->FindViewById(0x01020026 /*com.android.internal.R.id.keyboardView*/,
            		(IView**) &mMiniKeyboard);

            AutoPtr<IView> closeButton;
            mMiniKeyboardContainer->FindViewById(0x01020027 /*com.android.internal.R.id.closeButton*/,
            		(IView**) &closeButton);

            if (closeButton != NULL) {
            	closeButton->SetOnClickListener((IViewOnClickListener*)this->Probe(EIID_IViewOnClickListener));
            }

            mMiniKeyboard->SetOnKeyboardActionListener(new _OnKeyboardActionListener(this));
            //mInputView.setSuggest(mSuggest);
            AutoPtr<Keyboard> keyboard;
            if (popupKey->mPopupCharacters != NULL) {
                keyboard = new Keyboard(GetContext(), popupKeyboardId,
                        popupKey->mPopupCharacters, -1, GetPaddingLeft() + GetPaddingRight());
            } else {
                keyboard = new Keyboard(GetContext(), popupKeyboardId);
            }

            ((KeyboardView*)mMiniKeyboard.Get())->SetKeyboard(keyboard);
            ((KeyboardView*)mMiniKeyboard.Get())->SetPopupParent((IView*)this->Probe(EIID_IView));

            mMiniKeyboardContainer->Measure(
                    View::MeasureSpec::MakeMeasureSpec(GetWidth(), View::MeasureSpec::AT_MOST),
                    View::MeasureSpec::MakeMeasureSpec(GetHeight(), View::MeasureSpec::AT_MOST));

            mMiniKeyboardCache[popupKey] = mMiniKeyboardContainer;
        } else {
            mMiniKeyboardContainer->FindViewById(0x01020026 /*com.android.internal.R.id.keyboardView*/,
            		(IView**) &mMiniKeyboard);
        }

        if (mWindowOffset == NULL) {
            mWindowOffset = ArrayOf<Int32>::Alloc(2);
            Int32 tmpX = 0, tmpY = 0;
            GetLocationInWindow(&tmpX, &tmpY);
            (*mWindowOffset)[0] = tmpX;
            (*mWindowOffset)[1] = tmpY;
        }

        mPopupX = popupKey->mX + mPaddingLeft;
        mPopupY = popupKey->mY + mPaddingTop;

        Int32 meauseredW = 0, meauseredH = 0;
        mMiniKeyboardContainer->GetMeasuredWidth(&meauseredW);
        mPopupX = mPopupX + popupKey->mWidth - meauseredW;

        mMiniKeyboardContainer->GetMeasuredHeight(&meauseredH);
        mPopupY = mPopupY - meauseredH;

        Int32 paddingRight = 0, paddingBottom = 0;
        mMiniKeyboardContainer->GetPaddingRight(&paddingRight);
        Int32 x = mPopupX + paddingRight + (*mWindowOffset)[0];

        mMiniKeyboardContainer->GetPaddingBottom(&paddingBottom);
        Int32 y = mPopupY + paddingBottom + (*mWindowOffset)[1];

        ((KeyboardView*)mMiniKeyboard.Get())->SetPopupOffset(x < 0 ? 0 : x, y);
        ((KeyboardView*)mMiniKeyboard.Get())->SetShifted(IsShifted());
        mPopupKeyboard->SetContentView(mMiniKeyboardContainer);
        mPopupKeyboard->SetWidth(meauseredW);
        mPopupKeyboard->SetHeight(meauseredH);
        mPopupKeyboard->ShowAtLocation((IView*)this->Probe(EIID_IView), Gravity_NO_GRAVITY, x, y);
        mMiniKeyboardOnScreen = TRUE;
        //mMiniKeyboard.onTouchEvent(getTranslatedEvent(me));
        InvalidateAllKeys();
        return TRUE;
    }
    return FALSE;
}

void KeyboardView::SwipeRight()
{
	assert(mKeyboardActionListener != NULL);
    mKeyboardActionListener->SwipeRight();
}

void KeyboardView::SwipeLeft()
{
	assert(mKeyboardActionListener != NULL);
    mKeyboardActionListener->SwipeLeft();
}

void KeyboardView::SwipeUp()
{
	assert(mKeyboardActionListener != NULL);
    mKeyboardActionListener->SwipeUp();
}

void KeyboardView::SwipeDown()
{
	assert(mKeyboardActionListener != NULL);
    mKeyboardActionListener->SwipeDown();
}

void KeyboardView::InitGestureDetector() {
    mGestureDetector = new GestureDetector(GetContext(), new _SimpleOnGestureListener(this));
    mGestureDetector->SetIsLongpressEnabled(FALSE);
}

AutoPtr<ICharSequence> KeyboardView::AdjustCase(
    /* [in] */ ICharSequence* label)
{
	Int32 len = 0;
	Char32 c = 0;
	if (label != NULL) {
		label->GetLength(&len);
		label->GetCharAt(0, &c);
	}

    if (mKeyboard->IsShifted() && label != NULL && len < 3
            && Character::IsLowerCase(c)) {
    	String str;
    	label->ToString(&str);

    	str.ToUpperCase();

    	//TODO: need to release label ?
    	CStringWrapper::New(str, &label);
    }

    return label;
}

void KeyboardView::ComputeProximityThreshold(
    /* [in] */ Keyboard* keyboard)
{
    if (keyboard == NULL) return;
    if (mKeys == NULL) return;

    Int32 length = mKeys->GetLength();
    Int32 dimensionSum = 0;
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<Keyboard::Key> key = (*mKeys)[i];
        dimensionSum += Math::Min(key->mWidth, key->mHeight) + key->mGap;
    }

    if (dimensionSum < 0 || length == 0) return;

    mProximityThreshold = (Int32) (dimensionSum * 1.4f / length);
    mProximityThreshold *= mProximityThreshold; // Square it
}

void KeyboardView::OnBufferDraw()
{
    if (mBuffer == NULL || mKeyboardChanged) {
    	Int32 bufferW = 0, bufferH = 0;
    	if (mBuffer != NULL) {
    		mBuffer->GetWidth(&bufferW);
    		mBuffer->GetHeight(&bufferH);
    	}

        if (mBuffer == NULL || (mKeyboardChanged &&
                (bufferW != GetWidth() || bufferH != GetHeight())))
        {
            // Make sure our bitmap is at least 1x1
            const Int32 width = Math::Max(1, GetWidth());
            const Int32 height = Math::Max(1, GetHeight());

			AutoPtr<IBitmapFactory> bmFactory;
			CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmFactory);
            bmFactory->CreateBitmapEx3(width, height,
            		BitmapConfig_ARGB_8888, (IBitmap**) &mBuffer);

            CCanvas::New(mBuffer, (ICanvas**) &mCanvas);
        }

        InvalidateAllKeys();
        mKeyboardChanged = FALSE;
    }

    assert(mCanvas != NULL);
    Boolean isNotEmpty = FALSE;
    mCanvas->ClipRect(mDirtyRect.Get(), RegionOp_REPLACE, &isNotEmpty);

    if (mKeyboard == NULL) return;

    assert(mPadding != NULL);
    CRect* padding = (CRect*)mPadding.Get();
    const Int32 kbdPaddingLeft = mPaddingLeft;
    const Int32 kbdPaddingTop = mPaddingTop;

    assert(mPaint != NULL);
    mPaint->SetColor(mKeyTextColor);
    Boolean drawSingleKey = FALSE;
    Boolean isNonEmpty = FALSE;
    mCanvas->GetClipBounds(mClipRegion, &isNonEmpty);
    CRect* clipRegion = (CRect*)mClipRegion.Get();

    if (mInvalidatedKey != NULL && isNonEmpty) {
      // Is clipRegion completely contained within the invalidated key?
      if (mInvalidatedKey->mX + kbdPaddingLeft - 1 <= clipRegion->mLeft &&
              mInvalidatedKey->mY + kbdPaddingTop - 1 <= clipRegion->mTop &&
              mInvalidatedKey->mX + mInvalidatedKey->mWidth + kbdPaddingLeft + 1 >= clipRegion->mRight &&
              mInvalidatedKey->mY + mInvalidatedKey->mHeight + kbdPaddingTop + 1 >= clipRegion->mBottom) {
          drawSingleKey = TRUE;
      }
    }

    mCanvas->DrawColorEx(0x00000000, PorterDuffMode_CLEAR);
    const Int32 keyCount = mKeys->GetLength();
    for (Int32 i = 0; i < keyCount; i++) {
        AutoPtr<Keyboard::Key> key = (*mKeys)[i];
        if (drawSingleKey && mInvalidatedKey != key) {
            continue;
        }

        ArrayOf<Int32>* drawableState = key->GetCurrentDrawableState();
        Boolean isStateful = FALSE;
        mKeyBackground->SetState(drawableState, &isStateful);

        // Switch the character to uppercase if shift is pressed
        String label;// = key->mLabel == NULL? NULL : AdjustCase(key->mLabel).toString();
        if (key->mLabel == NULL) {
        	//NULL
        }
        else {
        	AdjustCase(key->mLabel)->ToString(&label);
        }


        AutoPtr<IRect> bounds;
        mKeyBackground->GetBounds((IRect**) &bounds);
        if (key->mWidth != ((CRect*)bounds.Get())->mRight ||
                key->mHeight != ((CRect*)bounds.Get())->mBottom)
        {
            mKeyBackground->SetBounds(0, 0, key->mWidth, key->mHeight);
        }

        mCanvas->Translate(key->mX + kbdPaddingLeft, key->mY + kbdPaddingTop);
        mKeyBackground->Draw(mCanvas);

        if (label != NULL) {
            // For characters, use large font. For labels like "Done", use small font.
            if (label.GetLength() > 1 && key->mCodes->GetLength() < 2) {
                mPaint->SetTextSize(mLabelTextSize);
                mPaint->SetTypeface(Typeface::DEFAULT_BOLD);
            } else {
                mPaint->SetTextSize(mKeyTextSize);
                mPaint->SetTypeface(Typeface::DEFAULT);
            }

            // Draw a drop shadow for the text
            mPaint->SetShadowLayer(mShadowRadius, 0, 0, mShadowColor);

            Float textSize = 0.0, tmpDescent = 0.0;
            mPaint->GetTextSize(&textSize);
            mPaint->Descent(&tmpDescent);

            // Draw the text
            mCanvas->DrawTextInString(label,
                (key->mWidth - padding->mLeft - padding->mRight) / 2
                        + padding->mLeft,
                (key->mHeight - padding->mTop - padding->mBottom) / 2
                        + (textSize - tmpDescent) / 2 + padding->mTop,
                mPaint);

            // Turn off drop shadow
            mPaint->SetShadowLayer(0, 0, 0, 0);
        } else if (key->mIcon != NULL) {
        	Int32 intrinsicWidth = 0, intrinsicHeight = 0;
        	key->mIcon->GetIntrinsicWidth(&intrinsicWidth);
        	key->mIcon->GetIntrinsicHeight(&intrinsicHeight);

            const Int32 drawableX = (key->mWidth - padding->mLeft - padding->mRight
                            - intrinsicWidth) / 2 + padding->mLeft;

            const Int32 drawableY = (key->mHeight - padding->mTop - padding->mBottom
                    - intrinsicHeight) / 2 + padding->mTop;

            mCanvas->Translate(drawableX, drawableY);
            key->mIcon->SetBounds(0, 0, intrinsicWidth, intrinsicHeight);

            key->mIcon->Draw(mCanvas);
            mCanvas->Translate(-drawableX, -drawableY);
        }

        mCanvas->Translate(-key->mX - kbdPaddingLeft, -key->mY - kbdPaddingTop);
    }

    mInvalidatedKey = NULL;
    // Overlay a dark rectangle to dim the keyboard
    if (mMiniKeyboardOnScreen) {
        mPaint->SetColor((Int32) (mBackgroundDimAmount * 0xFF) << 24);
        mCanvas->DrawRectEx2(0, 0, GetWidth(), GetHeight(), mPaint);
    }

    if (DEBUG && mShowTouchPoints) {
        mPaint->SetAlpha(128);
        mPaint->SetColor(0xFFFF0000);
        mCanvas->DrawCircle(mStartX, mStartY, 3, mPaint);
        mCanvas->DrawLine(mStartX, mStartY, mLastX, mLastY, mPaint);
        mPaint->SetColor(0xFF0000FF);
        mCanvas->DrawCircle(mLastX, mLastY, 3, mPaint);
        mPaint->SetColor(0xFF00FF00);
        mCanvas->DrawCircle((mStartX + mLastX) / 2, (mStartY + mLastY) / 2, 2, mPaint);
    }

    mDrawPending = FALSE;
    mDirtyRect->SetEmpty();
}

Int32 KeyboardView::GetKeyIndices(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ ArrayOf<Int32>* allKeys)
{
	assert(allKeys != NULL);
    Int32 primaryIndex = NOT_A_KEY;
    Int32 closestKey = NOT_A_KEY;
    Int32 closestKeyDist = mProximityThreshold + 1;

    //java.util.Arrays.fill(mDistances, Integer.MAX_VALUE);
    assert(mDistances != NULL);
    for (Int32 i = 0; i < mDistances->GetLength(); i++) {
    	(*mDistances)[i] = Math::INT32_MAX_VALUE;
    }

    ArrayOf<Int32>* nearestKeyIndices = mKeyboard->GetNearestKeys(x, y);
    const Int32 keyCount = nearestKeyIndices->GetLength();
    for (Int32 i = 0; i < keyCount; i++) {
        AutoPtr<Keyboard::Key> key = (*mKeys)[(*nearestKeyIndices)[i]];
        Int32 dist = 0;
        Boolean isInside = key->IsInside(x,y);
        if (isInside) {
            primaryIndex = (*nearestKeyIndices)[i];
        }

        if (((mProximityCorrectOn
                && (dist = key->SquaredDistanceFrom(x, y)) < mProximityThreshold)
                || isInside)
                && (*key->mCodes)[0] > 32)
        {
            // Find insertion point
            const Int32 nCodes = key->mCodes->GetLength();
            if (dist < closestKeyDist) {
                closestKeyDist = dist;
                closestKey = (*nearestKeyIndices)[i];
            }

            if (allKeys == NULL) continue;

            for (Int32 j = 0; j < mDistances->GetLength(); j++) {
                if ((*mDistances)[j] > dist) {
                    // Make space for nCodes codes
                    // System.arraycopy(mDistances, j, mDistances, j + nCodes,
                    //         mDistances.length - j - nCodes);
                    memcpy(mDistances->GetPayload() + j, mDistances->GetPayload() + j + nCodes,
                            (mDistances->GetLength() - j - nCodes) * sizeof(Int32));

                    // System.arraycopy(allKeys, j, allKeys, j + nCodes,
                    //         allKeys.length - j - nCodes);
                    memcpy(allKeys->GetPayload() + j, allKeys->GetPayload() + j + nCodes,
                            (allKeys->GetLength() - j - nCodes) * sizeof(Int32));

                    for (Int32 c = 0; c < nCodes; c++) {
                        (*allKeys)[j + c] = (*key->mCodes)[c];
                        (*mDistances)[j + c] = dist;
                    }

                    break;
                }
            }
        }
    }

    if (primaryIndex == NOT_A_KEY) {
        primaryIndex = closestKey;
    }

    return primaryIndex;
}

void KeyboardView::DetectAndSendKey(
	/* [in] */ Int32 index,
	/* [in] */ Int32 x,
	/* [in] */ Int32 y,
	/* [in] */ Int64 eventTime)
{
	assert(mKeys != NULL && mKeyboardActionListener != NULL);
    if (index != NOT_A_KEY && index < mKeys->GetLength()) {
        AutoPtr<Keyboard::Key> key = (*mKeys)[index];
        if (key->mText != NULL) {
            mKeyboardActionListener->OnText(key->mText);
            mKeyboardActionListener->OnRelease(NOT_A_KEY);
        } else {
            Int32 code = (*key->mCodes)[0];
            //TextEntryState.keyPressedAt(key, x, y);
            ArrayOf<Int32>* codes = ArrayOf<Int32>::Alloc(MAX_NEARBY_KEYS);

            //Arrays.fill(codes, NOT_A_KEY);
            for (Int32 i = 0; i < MAX_NEARBY_KEYS; ++i)
            {
            	(*codes)[i] = NOT_A_KEY;
            }

            GetKeyIndices(x, y, codes);
            // Multi-tap
            if (mInMultiTap) {
                if (mTapCount != -1) {
                    ArrayOf<Int32>* tmpArr = ArrayOf<Int32>::Alloc(1);
                    (*tmpArr)[0] = KEY_DELETE[0];
                    mKeyboardActionListener->OnKey(Keyboard::KEYCODE_DELETE, tmpArr);

                    ArrayOf<Int32>::Free(tmpArr);
                } else {
                    mTapCount = 0;
                }

                code = (*key->mCodes)[mTapCount];
            }

            mKeyboardActionListener->OnKey(code, codes);
            mKeyboardActionListener->OnRelease(code);

            ArrayOf<Int32>::Free(codes);
        }

        mLastSentIndex = index;
        mLastTapTime = eventTime;
    }
}

/**
 * Handle multi-tap keys by producing the key label for the current multi-tap state.
 */
AutoPtr<ICharSequence> KeyboardView::GetPreviewText(
	/* [in] */ Keyboard::Key* key)
{
    if (mInMultiTap) {
        // Multi-tap
        mPreviewLabel.SetLength(0);
        mPreviewLabel.Append((Char16) (*key->mCodes)[mTapCount < 0 ? 0 : mTapCount]);

        AutoPtr<ICharSequence> text;
        CStringWrapper::New(String(mPreviewLabel.GetPayload()), (ICharSequence**) &text);
        return AdjustCase(text);
    } else {
        return AdjustCase(key->mLabel);
    }
}

void KeyboardView::ShowPreview(
	/* [in] */ Int32 keyIndex)
{
	assert(mPreviewPopup != NULL && mKeys != NULL && mHandler != NULL);
    Int32 oldKeyIndex = mCurrentKeyIndex;

    mCurrentKeyIndex = keyIndex;
    // Release the old key and press the new key
    if (oldKeyIndex != mCurrentKeyIndex) {
        if (oldKeyIndex != NOT_A_KEY && mKeys->GetLength() > oldKeyIndex) {
            (*mKeys)[oldKeyIndex]->OnReleased(mCurrentKeyIndex == NOT_A_KEY);
            InvalidateKey(oldKeyIndex);
        }
        if (mCurrentKeyIndex != NOT_A_KEY && mKeys->GetLength() > mCurrentKeyIndex) {
            (*mKeys)[mCurrentKeyIndex]->OnPressed();

            InvalidateKey(mCurrentKeyIndex);
        }
    }
    // If key changed and preview is on ...
    if (oldKeyIndex != mCurrentKeyIndex && mShowPreview) {
        void (STDCALL KeyboardView::*pHandlerFunc)(Int32);
        pHandlerFunc = &KeyboardView::HandleShowPreView;
        //mHandler->RemoveMessages(MSG_SHOW_PREVIEW);
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

        Boolean showing = FALSE;
        mPreviewPopup->IsShowing(&showing);
        if (showing) {
            if (keyIndex == NOT_A_KEY) {
                void (STDCALL KeyboardView::*pHandlerFunc)();
                pHandlerFunc = &KeyboardView::HandleRemovePreView;

                mHandler->PostCppCallbackDelayed(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    NULL, 0, DELAY_AFTER_PREVIEW);
            }
        }

        if (keyIndex != NOT_A_KEY) {
        	mPreviewPopup->IsShowing(&showing);
        	Int32 v = 0;
        	mPreviewText->GetVisibility(&v);
            if (showing && v == VISIBLE) {
                // Show right away, if it's already visible and finger is moving around
                ShowKey(keyIndex);
            } else {
		        AutoPtr<IParcel> params;
		        CCallbackParcel::New((IParcel**)&params);
		        params->WriteInt32(keyIndex);

                mHandler->PostCppCallbackDelayed(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    params, 0, DELAY_BEFORE_PREVIEW);
            }
        }
    }
}

void KeyboardView::ShowKey(
	/* [in] */ Int32 keyIndex)
{
	assert(mPreviewPopup != NULL && mKeys != NULL);
    if (keyIndex < 0 || keyIndex >= mKeys->GetLength()) return;

    AutoPtr<Keyboard::Key> key = (*mKeys)[keyIndex];
    if (key->mIcon != NULL) {
        mPreviewText->SetCompoundDrawables(NULL, NULL, NULL,
                key->mIconPreview != NULL ? key->mIconPreview : key->mIcon);

        mPreviewText->SetText(NULL);
    } else {
        mPreviewText->SetCompoundDrawables(NULL, NULL, NULL, NULL);
        mPreviewText->SetText(GetPreviewText(key));

        Int32 len = 0;
        key->mLabel->GetLength(&len);
        if (len > 1 && key->mCodes->GetLength() < 2) {
            mPreviewText->SetTextSizeEx(TypedValue_COMPLEX_UNIT_PX, mKeyTextSize);
            mPreviewText->SetTypefaceEx(Typeface::DEFAULT_BOLD);
        } else {
            mPreviewText->SetTextSizeEx(TypedValue_COMPLEX_UNIT_PX, mPreviewTextSizeLarge);
            mPreviewText->SetTypefaceEx(Typeface::DEFAULT);
        }
    }

    mPreviewText->Measure(MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED),
            MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED));

    Int32 tmp = 0, tmpL = 0, tmpR = 0;
    mPreviewText->GetMeasuredWidth(&tmp);
    mPreviewText->GetPaddingLeft(&tmpL);
    mPreviewText->GetPaddingRight(&tmpR);

    Int32 popupWidth = Math::Max(tmp, key->mWidth + tmpL + tmpR);

    const Int32 popupHeight = mPreviewHeight;
    AutoPtr<IViewGroupLayoutParams> lp;
    mPreviewText->GetLayoutParams((IViewGroupLayoutParams**) &lp);

    if (lp != NULL) {
        ((CViewGroupLayoutParams*)lp.Get())->mWidth = popupWidth;
        ((CViewGroupLayoutParams*)lp.Get())->mHeight = popupHeight;
    }

    if (!mPreviewCentered) {
    	mPreviewText->GetPaddingLeft(&tmpL);
        mPopupPreviewX = key->mX - tmpL + mPaddingLeft;
        mPopupPreviewY = key->mY - popupHeight + mPreviewOffset;
    } else {
    	mPreviewText->GetMeasuredWidth(&tmp);
        // TODO: Fix this if centering is brought back
        mPopupPreviewX = 160 - tmp / 2;

        mPreviewText->GetMeasuredHeight(&tmp);
        mPopupPreviewY = - tmp;
    }

    void (STDCALL KeyboardView::*pHandlerFunc)();
    pHandlerFunc = &KeyboardView::HandleRemovePreView;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
    //mHandler->RemoveMessages(MSG_REMOVE_PREVIEW);

    if (mOffsetInWindow == NULL) {
        mOffsetInWindow = ArrayOf<Int32>::Alloc(2);
        Int32 tmpX = 0, tmpY = 0;
        GetLocationInWindow(&tmpX, &tmpY);
        (*mOffsetInWindow)[0] = tmpX;
        (*mOffsetInWindow)[1] = tmpY;

        (*mOffsetInWindow)[0] += mMiniKeyboardOffsetX; // Offset may be zero
        (*mOffsetInWindow)[1] += mMiniKeyboardOffsetY; // Offset may be zero

        ArrayOf<Int32>* mWindowLocation = ArrayOf<Int32>::Alloc(2);
        GetLocationInWindow(&tmpX, &tmpY);
        (*mWindowLocation)[0] = tmpX;
        (*mWindowLocation)[1] = tmpY;

        mWindowY = (*mWindowLocation)[1];
        ArrayOf<Int32>::Free(mWindowLocation);
    }

    // Set the preview background state
    AutoPtr<IDrawable> drawable;
    mPreviewText->GetBackground((IDrawable**) &drawable);
    assert(drawable != NULL);

    ArrayOf<Int32>* tmpArr = NULL;
    if (key->mPopupResId != 0) {
        tmpArr = ArrayOf<Int32>::Alloc(1);
        (*tmpArr)[0] = LONG_PRESSABLE_STATE_SET[0];
    }
    else {
        //TODO EMPTY_STATE_SET
        //Empty is equal NULL?
    }

    Boolean isStateful = FALSE;
    drawable->SetState(tmpArr, &isStateful);
    if (tmpArr != NULL) {
        ArrayOf<Int32>::Free(tmpArr);
        tmpArr = NULL;
    }

    // drawable->SetState(
    //         key->mPopupResId != 0 ? LONG_PRESSABLE_STATE_SET : EMPTY_STATE_SET);

    mPopupPreviewX += (*mOffsetInWindow)[0];
    mPopupPreviewY += (*mOffsetInWindow)[1];

    // If the popup cannot be shown above the key, put it on the side
    if (mPopupPreviewY + mWindowY < 0) {
        // If the key you're pressing is on the left side of the keyboard, show the popup on
        // the right, offset by enough to see at least one key to the left/right.
        if (key->mX + key->mWidth <= GetWidth() / 2) {
            mPopupPreviewX += (Int32) (key->mWidth * 2.5);
        } else {
            mPopupPreviewX -= (Int32) (key->mWidth * 2.5);
        }

        mPopupPreviewY += popupHeight;
    }

    Boolean showing = FALSE;
    mPreviewPopup->IsShowing(&showing);
    if (showing) {
        mPreviewPopup->UpdateEx2(mPopupPreviewX, mPopupPreviewY,
                popupWidth, popupHeight);
    } else {
        mPreviewPopup->SetWidth(popupWidth);
        mPreviewPopup->SetHeight(popupHeight);
        mPreviewPopup->ShowAtLocation(mPopupParent, Gravity_NO_GRAVITY,
                mPopupPreviewX, mPopupPreviewY);
    }

    mPreviewText->SetVisibility(VISIBLE);
}

Boolean KeyboardView::OpenPopupIfRequired(
    /* [in] */ IMotionEvent* me)
{
    // Check if we have a popup layout specified first.
    if (mPopupLayout == 0) {
        return FALSE;
    }

    assert(mKeys != NULL);
    if (mCurrentKey < 0 || mCurrentKey >= mKeys->GetLength()) {
        return FALSE;
    }

    AutoPtr<Keyboard::Key> popupKey = (*mKeys)[mCurrentKey];
    Boolean result = OnLongPress(popupKey);
    if (result) {
        mAbortKey = TRUE;
        ShowPreview(NOT_A_KEY);
    }

    return result;
}

Boolean KeyboardView::OnModifiedTouchEvent(
    /* [in] */ IMotionEvent* me,
    /* [in] */ Boolean possiblePoly)
{
	assert(me != NULL);
	Float tmpX = 0, tmpY = 0;
	me->GetX(&tmpX);
	me->GetY(&tmpY);

    Int32 touchX = (Int32) tmpX - mPaddingLeft;
    Int32 touchY = (Int32) tmpY + mVerticalCorrection - mPaddingTop;

    Int32 action = 0;
    me->GetAction(&action);

    Int64 eventTime = 0;
    me->GetEventTime(&eventTime);

    mOldEventTime = eventTime;
    Int32 keyIndex = GetKeyIndices(touchX, touchY, NULL);
    mPossiblePoly = possiblePoly;

    // Track the last few movements to look for spurious swipes.
    if (action == KeyEvent_ACTION_DOWN) mSwipeTracker.Clear();
    mSwipeTracker->AddMovement(me);

    // Ignore all motion events until a DOWN.
    if (mAbortKey
            && action != KeyEvent_ACTION_DOWN && action != MotionEvent_ACTION_CANCEL) {
        return TRUE;
    }

    if (mGestureDetector->OnTouchEvent(me)) {
        ShowPreview(NOT_A_KEY);

        void (STDCALL KeyboardView::*pHandlerFunc)();
        pHandlerFunc = &KeyboardView::HandleRepeat;
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        //mHandler->RemoveMessages(MSG_REPEAT);

        void (STDCALL KeyboardView::*pHandlerFunc2)(IMotionEvent*);
        pHandlerFunc2 = &KeyboardView::HandleLongPress;
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc2);
        //mHandler->RemoveMessages(MSG_LONGPRESS);

        return TRUE;
    }

    // Needs to be called after the gesture detector gets a turn, as it may have
    // displayed the mini keyboard
    if (mMiniKeyboardOnScreen && action != MotionEvent_ACTION_CANCEL) {
        return TRUE;
    }

    switch (action) {
        case KeyEvent_ACTION_DOWN: {
            mAbortKey = FALSE;
            mStartX = touchX;
            mStartY = touchY;
            mLastCodeX = touchX;
            mLastCodeY = touchY;
            mLastKeyTime = 0;
            mCurrentKeyTime = 0;
            mLastKey = NOT_A_KEY;
            mCurrentKey = keyIndex;
            mDownKey = keyIndex;
            me->GetEventTime(&mDownTime);
            mLastMoveTime = mDownTime;
            CheckMultiTap(eventTime, keyIndex);
            mKeyboardActionListener->OnPress(keyIndex != NOT_A_KEY ?
                    ((*(*mKeys)[keyIndex]->mCodes))[0] : 0);

            if (mCurrentKey >= 0 && (*mKeys)[mCurrentKey]->mRepeatable) {
                mRepeatKeyIndex = mCurrentKey;

                void (STDCALL KeyboardView::*pHandlerFunc)();
                pHandlerFunc = &KeyboardView::HandleRepeat;

                mHandler->PostCppCallbackDelayed(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    NULL, 0, REPEAT_START_DELAY);

                RepeatKey();
                // Delivering the key could have caused an abort
                if (mAbortKey) {
                    mRepeatKeyIndex = NOT_A_KEY;
                    break;
                }
            }

            if (mCurrentKey != NOT_A_KEY) {
                void (STDCALL KeyboardView::*pHandlerFunc)(IMotionEvent*);
                pHandlerFunc = &KeyboardView::HandleLongPress;

		        AutoPtr<IParcel> params;
		        CCallbackParcel::New((IParcel**)&params);
		        params->WriteInterfacePtr(me);

                mHandler->PostCppCallbackDelayed(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    NULL, 0, LONGPRESS_TIMEOUT);
            }

            ShowPreview(keyIndex);
            break;
        }

        case MotionEvent_ACTION_MOVE: {
            Boolean continueLongPress = FALSE;
            if (keyIndex != NOT_A_KEY) {
                if (mCurrentKey == NOT_A_KEY) {
                    mCurrentKey = keyIndex;
                    mCurrentKeyTime = eventTime - mDownTime;
                } else {
                    if (keyIndex == mCurrentKey) {
                        mCurrentKeyTime += eventTime - mLastMoveTime;
                        continueLongPress = TRUE;
                    } else if (mRepeatKeyIndex == NOT_A_KEY) {
                        ResetMultiTap();
                        mLastKey = mCurrentKey;
                        mLastCodeX = mLastX;
                        mLastCodeY = mLastY;
                        mLastKeyTime = mCurrentKeyTime + eventTime - mLastMoveTime;
                        mCurrentKey = keyIndex;
                        mCurrentKeyTime = 0;
                    }
                }
            }

            if (!continueLongPress) {
                // Cancel old longpress
                void (STDCALL KeyboardView::*pHandlerFunc2)(IMotionEvent*);
                pHandlerFunc2 = &KeyboardView::HandleLongPress;
                mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc2);
                //mHandler->RemoveMessages(MSG_LONGPRESS);

                // Start new longpress if key has changed
                if (keyIndex != NOT_A_KEY) {
                void (STDCALL KeyboardView::*pHandlerFunc)(IMotionEvent*);
                pHandlerFunc = &KeyboardView::HandleLongPress;

		        AutoPtr<IParcel> params;
		        CCallbackParcel::New((IParcel**)&params);
		        params->WriteInterfacePtr(me);

                mHandler->PostCppCallbackDelayed(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    params, 0, LONGPRESS_TIMEOUT);
                }
            }

            ShowPreview(mCurrentKey);
            mLastMoveTime = eventTime;
            break;
        }

        case KeyEvent_ACTION_UP: {
            RemoveMessages();
            if (keyIndex == mCurrentKey) {
                mCurrentKeyTime += eventTime - mLastMoveTime;
            } else {
                ResetMultiTap();

                mLastKey = mCurrentKey;
                mLastKeyTime = mCurrentKeyTime + eventTime - mLastMoveTime;
                mCurrentKey = keyIndex;
                mCurrentKeyTime = 0;
            }

            if (mCurrentKeyTime < mLastKeyTime && mCurrentKeyTime < DEBOUNCE_TIME
                    && mLastKey != NOT_A_KEY) {
                mCurrentKey = mLastKey;
                touchX = mLastCodeX;
                touchY = mLastCodeY;
            }

            ShowPreview(NOT_A_KEY);
            //Arrays.fill(mKeyIndices, NOT_A_KEY);
            for (Int32 i = 0; i < mKeyIndices->GetLength(); ++i)
            {
            	(*mKeyIndices)[i] = NOT_A_KEY;
            }

            // If we're not on a repeating key (which sends on a DOWN event)
            if (mRepeatKeyIndex == NOT_A_KEY && !mMiniKeyboardOnScreen && !mAbortKey) {
                DetectAndSendKey(mCurrentKey, touchX, touchY, eventTime);
            }

            InvalidateKey(keyIndex);
            mRepeatKeyIndex = NOT_A_KEY;
            break;
        }

        case MotionEvent_ACTION_CANCEL: {
            RemoveMessages();
            DismissPopupKeyboard();
            mAbortKey = TRUE;
            ShowPreview(NOT_A_KEY);
            InvalidateKey(mCurrentKey);
            break;
        }
    }

    mLastX = touchX;
    mLastY = touchY;
    return TRUE;
}

Boolean KeyboardView::RepeatKey()
{
    AutoPtr<Keyboard::Key> key = (*mKeys)[mRepeatKeyIndex];
    DetectAndSendKey(mCurrentKey, key->mX, key->mY, mLastTapTime);
    return TRUE;
}

void KeyboardView::RemoveMessages()
{
    void (STDCALL KeyboardView::*pHandlerFunc)();
    pHandlerFunc = &KeyboardView::HandleRepeat;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
    //mHandler->RemoveMessages(MSG_REPEAT);

    void (STDCALL KeyboardView::*pHandlerFunc2)(IMotionEvent*);
    pHandlerFunc2 = &KeyboardView::HandleLongPress;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc2);
    //mHandler->RemoveMessages(MSG_LONGPRESS);

    void (STDCALL KeyboardView::*pHandlerFunc3)(Int32);
    pHandlerFunc3 = &KeyboardView::HandleShowPreView;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc3);
    //mHandler->RemoveMessages(MSG_SHOW_PREVIEW);
}

void KeyboardView::DismissPopupKeyboard()
{
	assert(mPopupKeyboard != NULL);
	Boolean showing = FALSE;
	mPopupKeyboard->IsShowing(&showing);

    if (showing) {
        mPopupKeyboard->Dismiss();
        mMiniKeyboardOnScreen = FALSE;
        InvalidateAllKeys();
    }
}

void KeyboardView::ResetMultiTap()
{
    mLastSentIndex = NOT_A_KEY;
    mTapCount = 0;
    mLastTapTime = -1;
    mInMultiTap = FALSE;
}

void KeyboardView::CheckMultiTap(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 keyIndex)
{
    if (keyIndex == NOT_A_KEY) return;

    AutoPtr<Keyboard::Key> key = (*mKeys)[keyIndex];
    if (key->mCodes->GetLength() > 1) {
        mInMultiTap = TRUE;

        if (eventTime < mLastTapTime + MULTITAP_INTERVAL
                && keyIndex == mLastSentIndex)
        {
            mTapCount = (mTapCount + 1) % key->mCodes->GetLength();

            return;
        } else {
            mTapCount = -1;
            return;
        }
    }

    if (eventTime > mLastTapTime + MULTITAP_INTERVAL || keyIndex != mLastSentIndex) {
        ResetMultiTap();
    }
}

void KeyboardView::HandleShowPreView(
    /* [in] */ Int32 keyIndex)
{
	ShowKey(keyIndex);
}

void KeyboardView::HandleRemovePreView()
{
	assert(mPreviewText != NULL);
	mPreviewText->SetVisibility(INVISIBLE);
}

void KeyboardView::HandleRepeat()
{
    if (RepeatKey()) {
		assert(0);

    	//TODO: in android's java, no code to process this message?
        // Message repeat = Message.obtain(this, MSG_REPEAT);
        // sendMessageDelayed(repeat, REPEAT_INTERVAL);
    }
}

void KeyboardView::HandleLongPress(
    /* [in] */ IMotionEvent* obj)
{
	OpenPopupIfRequired(obj);
}
