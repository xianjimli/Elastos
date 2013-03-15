
#include "inputmethodservice/KeyboardView.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CCanvas.h"
#include "graphics/CRectF.h"
#include "graphics/Typeface.h"
#include "graphics/CPaint.h"
#include "view/ViewConfiguration.h"
#include "view/CMotionEvent.h"
#include "view/CViewGroupLayoutParams.h"
#include "widget/CPopupWindow.h"
#include "utils/CDisplayMetrics.h"
#include "os/CApartment.h"
#include <elastos/Character.h>
#include <elastos/Math.h>


using namespace Elastos::Core;


static Int32 R_Styleable_KeyboardView[] = {
    0x01010161, 0x01010164, 0x01010233, 0x01010234,
    0x01010235, 0x01010236, 0x01010237, 0x01010238,
    0x01010239, 0x0101023a, 0x0101023b, 0x010102d9
};

static Int32 R_Styleable_Theme[] = {
    0x01010030, 0x01010031, 0x01010032, 0x01010033,
    0x01010034, 0x01010035, 0x01010036, 0x01010037,
    0x01010038, 0x01010039, 0x0101003a, 0x0101003b,
    0x0101003c, 0x0101003d, 0x0101003e, 0x0101003f,
    0x01010040, 0x01010041, 0x01010042, 0x01010043,
    0x01010044, 0x01010045, 0x01010046, 0x01010047,
    0x01010048, 0x01010049, 0x0101004a, 0x0101004b,
    0x0101004c, 0x0101004d, 0x0101004e, 0x0101004f,
    0x01010050, 0x01010051, 0x01010052, 0x01010053,
    0x01010054, 0x01010055, 0x01010056, 0x01010057,
    0x01010058, 0x01010059, 0x0101005a, 0x0101005b,
    0x0101005c, 0x0101005d, 0x0101005e, 0x0101005f,
    0x01010060, 0x01010061, 0x01010062, 0x0101006a,
    0x0101006b, 0x0101006c, 0x0101006d, 0x0101006e,
    0x0101006f, 0x01010070, 0x01010071, 0x01010072,
    0x01010073, 0x01010074, 0x01010075, 0x01010076,
    0x01010077, 0x01010078, 0x01010079, 0x0101007a,
    0x0101007b, 0x0101007c, 0x0101007d, 0x0101007e,
    0x01010080, 0x01010081, 0x01010082, 0x01010083,
    0x01010084, 0x01010085, 0x01010086, 0x01010087,
    0x01010088, 0x01010089, 0x0101008a, 0x0101008b,
    0x0101008c, 0x0101008d, 0x0101008e, 0x0101008f,
    0x01010090, 0x01010091, 0x01010092, 0x01010093,
    0x01010094, 0x010100ae, 0x01010206, 0x01010207,
    0x01010208, 0x0101020d, 0x0101020f, 0x01010210,
    0x01010212, 0x01010213, 0x01010214, 0x01010219,
    0x0101021a, 0x0101021e, 0x0101021f, 0x01010222,
    0x0101022b, 0x01010230, 0x01010267, 0x01010287,
    0x01010288, 0x01010289, 0x0101028b, 0x01010292,
    0x010102a0, 0x010102a1, 0x010102ab, 0x010102ae,
    0x010102af, 0x010102b0, 0x010102b1, 0x010102b2,
    0x010102b3, 0x010102b6, 0x010102b9, 0x010102c5,
    0x010102c6, 0x010102c7, 0x010102c8, 0x010102cc,
    0x010102cd, 0x010102ce, 0x010102cf, 0x010102d0
};

const Int32 KeyboardView::SwipeTracker::NUM_PAST;
const Int32 KeyboardView::SwipeTracker::LONGEST_PAST_TIME;
const Boolean KeyboardView::DEBUG;
const Int32 KeyboardView::NOT_A_KEY;
Int32 KeyboardView::KEY_DELETE[] = { Keyboard_KEYCODE_DELETE };
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
const Int32 KeyboardView::MAX_NEARBY_KEYS = 12;
const Int32 KeyboardView::MULTITAP_INTERVAL;


KeyboardView::_OnKeyboardActionListener::_OnKeyboardActionListener(
    /* [in] */ KeyboardView* host)
    : mHost(host)
{}

KeyboardView::_OnKeyboardActionListener::~_OnKeyboardActionListener()
{}

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
    assert(mHost->mKeyboardActionListener != NULL);
    mHost->mKeyboardActionListener->OnKey(primaryCode, keyCodes);
    mHost->DismissPopupKeyboard();
    return NOERROR;
}

ECode KeyboardView::_OnKeyboardActionListener::OnText(
    /* [in] */ ICharSequence* text)
{
    assert(mHost->mKeyboardActionListener != NULL);
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
    for (Int32 i=0; i<N; i++) {
        ev->GetHistoricalX(i, &x);
        ev->GetHistoricalY(i, &y);
        Int64 tmpTime = 0;
        ev->GetHistoricalEventTime(i, &tmpTime);
        AddPoint(x, y, tmpTime);
    }
    ev->GetX(&x);
    ev->GetY(&y);
    AddPoint(x, y, time);
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
        }
        else if ((*mPastTime)[i] < time - LONGEST_PAST_TIME) {
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
        memcpy(mPastX->GetPayload(), mPastX->GetPayload() + start, count * sizeof(Float));
        // System.arraycopy(mPastY, start, mPastY, 0, count);
        memcpy(mPastY->GetPayload(), mPastY->GetPayload() + start, count * sizeof(Float));
        // System.arraycopy(mPastTime, start, mPastTime, 0, count);
        memcpy(mPastTime->GetPayload(), mPastTime->GetPayload() + start, count * sizeof(Int64));

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
    Int32 N = 0;
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



KeyboardView::_SimpleOnGestureListener::_SimpleOnGestureListener(
    /* [in] */ KeyboardView* host)
    : mHost(host)
{}

KeyboardView::_SimpleOnGestureListener::~_SimpleOnGestureListener()
{}

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
    : mCurrentKeyIndex(NOT_A_KEY)
    , mLabelTextSize(0)
    , mKeyTextSize(0)
    , mKeyTextColor(0)
    , mShadowRadius(0.0)
    , mShadowColor(0)
    , mBackgroundDimAmount(0.0)
    , mPreviewTextSizeLarge(0)
    , mPreviewOffset(0)
    , mPreviewHeight(0)
    , mOffsetInWindow(NULL)
    , mMiniKeyboardOnScreen(FALSE)
    , mMiniKeyboardOffsetX(0)
    , mMiniKeyboardOffsetY(0)
    , mWindowOffset(NULL)
    , mKeys(NULL)
    , mVerticalCorrection(0)
    , mProximityThreshold(0)
    , mPreviewCentered(FALSE)
    , mShowPreview(TRUE)
    , mShowTouchPoints(TRUE)
    , mPopupPreviewX(0)
    , mPopupPreviewY(0)
    , mWindowY(0)
    , mLastX(0)
    , mLastY(0)
    , mStartX(0)
    , mStartY(0)
    , mProximityCorrectOn(FALSE)
    , mDownTime(0)
    , mLastMoveTime(0)
    , mLastKey(0)
    , mLastCodeX(0)
    , mLastCodeY(0)
    , mCurrentKey(NOT_A_KEY)
    , mDownKey(NOT_A_KEY)
    , mLastKeyTime(0)
    , mCurrentKeyTime(0)
    , mKeyIndices(ArrayOf<Int32>::Alloc(12))
    , mGestureDetector(NULL)
    , mPopupX(0)
    , mPopupY(0)
    , mRepeatKeyIndex(NOT_A_KEY)
    , mPopupLayout(0)
    , mAbortKey(FALSE)
    , mPossiblePoly(FALSE)
    , mSwipeTracker(new SwipeTracker())
    , mSwipeThreshold(0)
    , mDisambiguateSwipe(FALSE)
    , mOldPointerCount(1)
    , mOldPointerX(0.0)
    , mOldPointerY(0.0)
    , mDistances(ArrayOf<Int32>::Alloc(MAX_NEARBY_KEYS))
    , mLastSentIndex(0)
    , mTapCount(0)
    , mLastTapTime(0)
    , mInMultiTap(FALSE)
    , mDrawPending(FALSE)
    , mKeyboardChanged(FALSE)
    , mOldEventTime(0)
    , mUsedVelocity(FALSE)
{
    CRect::New(0, 0, 0, 0, (IRect**) &mClipRegion);
    CRectF::New((IRectF**)&mDirtyRect);
    CApartment::GetDefaultApartment((IApartment**)&mHandler);
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
        ArrayOf<AutoPtr<IKeyboardKey> >::Free(mKeys);
    }

    if (mKeyIndices != NULL) {
        ArrayOf<Int32>::Free(mKeyIndices);
    }

    if (mDistances != NULL) {
        ArrayOf<Int32>::Free(mDistances);
    }
}

ECode KeyboardView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    View::Init(context, attrs, defStyle);

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_KeyboardView, sizeof(R_Styleable_KeyboardView) / sizeof(Int32)),
            defStyle, 0,
            (ITypedArray**) &a);


    AutoPtr<ILayoutInflater> inflate;
    context->GetSystemService(Context_LAYOUT_INFLATER_SERVICE, (IInterface**) &inflate);

    Int32 previewLayout = 0;
    Int32 keyTextSize = 0;

    Int32 n = 0;
    a->GetIndexCount(&n);

    for (Int32 i = 0; i < n; i++) {
        Int32 attr = 0;
        a->GetIndex(i, &attr);

        switch (attr) {
        case 2 /*com.android.internal.R.styleable.KeyboardView_keyBackground*/:
            a->GetDrawable(attr, (IDrawable**) &mKeyBackground);
            break;
        case 9 /*com.android.internal.R.styleable.KeyboardView_verticalCorrection*/:
            a->GetDimensionPixelOffset(attr, 0, &mVerticalCorrection);
            break;
        case 6 /*com.android.internal.R.styleable.KeyboardView_keyPreviewLayout*/:
            a->GetResourceId(attr, 0, &previewLayout);
            break;
        case 7 /*com.android.internal.R.styleable.KeyboardView_keyPreviewOffset*/:
            a->GetDimensionPixelOffset(attr, 0, &mPreviewOffset);
            break;
        case 8 /*com.android.internal.R.styleable.KeyboardView_keyPreviewHeight*/:
            a->GetDimensionPixelSize(attr, 80, &mPreviewHeight);
            break;
        case 3 /*com.android.internal.R.styleable.KeyboardView_keyTextSize*/:
            a->GetDimensionPixelSize(attr, 18, &mKeyTextSize);
            break;
        case 5 /*com.android.internal.R.styleable.KeyboardView_keyTextColor*/:
            a->GetColor(attr, 0xFF000000, &mKeyTextColor);
            break;
        case 4 /*com.android.internal.R.styleable.KeyboardView_labelTextSize*/:
            a->GetDimensionPixelSize(attr, 14, &mLabelTextSize);
            break;
        case 10 /*com.android.internal.R.styleable.KeyboardView_popupLayout*/:
            a->GetResourceId(attr, 0, &mPopupLayout);
            break;
        case 0 /*com.android.internal.R.styleable.KeyboardView_shadowColor*/:
            a->GetColor(attr, 0, &mShadowColor);
            break;
        case 1 /*com.android.internal.R.styleable.KeyboardView_shadowRadius*/:
            a->GetFloat(attr, 0.0, &mShadowRadius);
            break;
        }
    }

    a->Recycle();

    mContext->ObtainStyledAttributes(
            ArrayOf<Int32>(R_Styleable_Theme, sizeof(R_Styleable_Theme) / sizeof(Int32)),
            (ITypedArray**) &a);

    a->GetFloat(2 /*android.R.styleable.Theme_backgroundDimAmount*/, 0.5f, &mBackgroundDimAmount);
    a->Recycle();

    CPopupWindow::New(context, (IPopupWindow**)&mPreviewPopup);
    if (previewLayout != 0) {
        inflate->Inflate(previewLayout, NULL, (IView**)&mPreviewText);
        Float textSize = 0.0;
        mPreviewText->GetTextSize(&textSize);
        mPreviewTextSizeLarge = (Int32)textSize;
        mPreviewPopup->SetContentView(mPreviewText);
        mPreviewPopup->SetBackgroundDrawable(NULL);
    }
    else {
        mShowPreview = FALSE;
    }

    mPreviewPopup->SetTouchable(FALSE);

    CPopupWindow::New(context, (IPopupWindow**) &mPopupKeyboard);
    mPopupKeyboard->SetBackgroundDrawable(NULL);
    //mPopupKeyboard.setClippingEnabled(FALSE);

    mPopupParent = (IView*)this->Probe(EIID_IView);
    //mPredicting = TRUE;

    CPaint::New((IPaint**) &mPaint);
    mPaint->SetAntiAlias(TRUE);
    mPaint->SetTextSize(keyTextSize);

    mPaint->SetTextAlign(PaintAlign_CENTER /*Align.CENTER*/);
    mPaint->SetAlpha(255);

    CRect::New(0, 0, 0, 0, (IRect**) &mPadding);
    //mMiniKeyboardCache = new HashMap<Key,View>();

    Boolean isPadding = FALSE;
    mKeyBackground->GetPadding(mPadding, &isPadding);

    AutoPtr<IResources> res = GetResources();
    assert(res != NULL);

    AutoPtr<IDisplayMetrics> dis;
    res->GetDisplayMetrics((IDisplayMetrics**) &dis);
    mSwipeThreshold = (Int32) (500 * ((CDisplayMetrics*)dis.Get())->mDensity);

    res->GetBoolean(0x010d000e /*com.android.internal.R.bool.config_swipeDisambiguation*/,
            &mDisambiguateSwipe);

    ResetMultiTap();
    InitGestureDetector();

    return NOERROR;
}

PInterface KeyboardView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IKeyboardView*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IKeyboardView) {
        return (IKeyboardView*)this;
    }
    else if (riid == EIID_IView) {
        return (IView*)this;
    }

    return NULL;
}

void KeyboardView::InitGestureDetector()
{
    mGestureDetector = new GestureDetector(GetContext(),
            new _SimpleOnGestureListener(this));
    mGestureDetector->SetIsLongpressEnabled(FALSE);
}

ECode KeyboardView::SetOnKeyboardActionListener(
    /* [in] */ IOnKeyboardActionListener* listener)
{
    mKeyboardActionListener = listener;
    return NOERROR;
}

ECode KeyboardView::GetOnKeyboardActionListener(
    /* [out] */ IOnKeyboardActionListener** listener)
{
    assert(listener != NULL);
    *listener = mKeyboardActionListener;
    if (*listener != NULL) {
        (*listener)->AddRef();
    }

    return NOERROR;
}

ECode KeyboardView::SetKeyboard(
    /* [in] */ IKeyboard* keyboard)
{
    if (mKeyboard != NULL) {
        ShowPreview(NOT_A_KEY);
    }
    // Remove any pending messages
    RemoveMessages();
    mKeyboard = keyboard;
    AutoPtr<IObjectContainer> keys;
    mKeyboard->GetKeys((IObjectContainer**)&keys);
    Int32 size = 0;
    keys->GetObjectCount(&size);
    mKeys = ArrayOf< AutoPtr<IKeyboardKey> >::Alloc(size);
    AutoPtr<IObjectEnumerator> keyIt;
    keys->GetObjectEnumerator((IObjectEnumerator**)&keyIt);
    Int32 pos = 0;
    Boolean hasNext = FALSE;
    while(keyIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<IKeyboardKey> key;
        keyIt->Current((IInterface**)&key);
        (*mKeys)[pos] = key;
        pos ++;
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
    return NOERROR;
}

ECode KeyboardView::GetKeyboard(
    /* [out] */ IKeyboard** keyboard)
{
    assert(keyboard != NULL);
    *keyboard = mKeyboard;
    if (*keyboard != NULL) {
        (*keyboard)->AddRef();
    }

    return NOERROR;
}

ECode KeyboardView::SetShifted(
    /* [in] */ Boolean shifted,
    /* [out] */ Boolean* res)
{
    assert(res != NULL);
    if (mKeyboard != NULL) {
        Boolean succeeded = FALSE;
        if (mKeyboard->SetShifted(shifted, &succeeded), succeeded) {
            // The whole keyboard probably needs to be redrawn
            InvalidateAllKeys();
            *res = TRUE;
            return NOERROR;
        }
    }
    *res = FALSE;
    return NOERROR;
}

ECode KeyboardView::IsShifted(
    /* [out] */ Boolean* shifted)
{
    assert(shifted != NULL);
    if (mKeyboard != NULL) {
        return mKeyboard->IsShifted(shifted);
    }
    *shifted = FALSE;
    return NOERROR;
}

ECode KeyboardView::SetPreviewEnabled(
    /* [in] */ Boolean previewEnabled)
{
    mShowPreview = previewEnabled;
    return NOERROR;
}

ECode KeyboardView::IsPreviewEnabled(
    /* [out] */ Boolean* res)
{
    assert(res != NULL);
    *res = mShowPreview;
    return NOERROR;
}

ECode KeyboardView::SetVerticalCorrection(
    /* [in] */ Int32 verticalOffset)
{
    return NOERROR;
}

ECode KeyboardView::SetPopupParent(
    /* [in] */ IView* v)
{
    mPopupParent = v;
    return NOERROR;
}

ECode KeyboardView::SetPopupOffset(
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
    return NOERROR;
}

ECode KeyboardView::SetProximityCorrectionEnabled(
    /* [in] */ Boolean enabled)
{
    mProximityCorrectOn = enabled;
    return NOERROR;
}

/**
 * Returns TRUE if proximity correction is enabled.
 */
ECode KeyboardView::IsProximityCorrectionEnabled(
    /* [out] */ Boolean* res)
{
    assert(res != NULL);
    *res = mProximityCorrectOn;
    return NOERROR;
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

AutoPtr<ICharSequence> KeyboardView::AdjustCase(
    /* [in] */ ICharSequence* label)
{
    Boolean shifted;
    if ((mKeyboard->IsShifted(&shifted), shifted) && label != NULL) {
        Int32 len = 0;
        Char32 c = 0;
        label->GetLength(&len);
        label->GetCharAt(0, &c);
        if (len < 3 && Character::IsLowerCase(c)) {
            String str;
            label->ToString(&str);
            str.ToUpperCase();
            AutoPtr<ICharSequence> label2;
            CStringWrapper::New(str, (ICharSequence**)&label2);
            return label2;
        }
    }
    return label;
}

void KeyboardView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // Round up a little
    if (mKeyboard == NULL) {
        SetMeasuredDimension(mPaddingLeft + mPaddingRight, mPaddingTop + mPaddingBottom);
    }
    else {
        Int32 mw = 0;
        mKeyboard->GetMinWidth(&mw);
        Int32 width = mw + mPaddingLeft + mPaddingRight;
        if (View::MeasureSpec::GetSize(widthMeasureSpec) < width + 10) {
            width = View::MeasureSpec::GetSize(widthMeasureSpec);
        }
        Int32 th = 0;
        mKeyboard->GetHeight(&th);
        SetMeasuredDimension(width, th + mPaddingTop + mPaddingBottom);
    }
}

void KeyboardView::ComputeProximityThreshold(
    /* [in] */ IKeyboard* keyboard)
{
    if (keyboard == NULL) return;
    if (mKeys == NULL) return;

    Int32 length = mKeys->GetLength();
    Int32 dimensionSum = 0;
    for (Int32 i = 0; i < length; i++) {
        IKeyboardKey* key = (*mKeys)[i];
        Int32 keyW, keyH, keyG;
        key->GetWidth(&keyW);
        key->GetHeight(&keyH);
        key->GetGap(&keyG);
        dimensionSum += Math::Min(keyW, keyH) + keyG;
    }
    if (dimensionSum < 0 || length == 0) return;
    mProximityThreshold = (Int32) (dimensionSum * 1.4f / length);
    mProximityThreshold *= mProximityThreshold; // Square it
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
    // if (mDrawPending || mBuffer == NULL || mKeyboardChanged) {
        OnBufferDraw(canvas);
    // }
    // canvas->DrawBitmap(mBuffer, 0, 0, NULL);
}

void KeyboardView::OnBufferDraw(
    /* [in] */ ICanvas* canvas)
{
    //todo:
    mCanvas = canvas;
    if (mBuffer == NULL || mKeyboardChanged) {
        Int32 bufferW = 0, bufferH = 0;
        if (mBuffer != NULL) {
            mBuffer->GetWidth(&bufferW);
            mBuffer->GetHeight(&bufferH);
        }
        if (mBuffer == NULL || (mKeyboardChanged &&
                (bufferW != GetWidth() || bufferH != GetHeight()))) {
            // Make sure our bitmap is at least 1x1
            const Int32 width = Math::Max(1, GetWidth());
            const Int32 height = Math::Max(1, GetHeight());
            AutoPtr<IBitmapFactory> bmFactory;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmFactory);
            bmFactory->CreateBitmapEx3(width, height,
                    BitmapConfig_ARGB_8888, (IBitmap**)&mBuffer);
            // CCanvas::New(mBuffer, (ICanvas**)&mCanvas);
        }
        InvalidateAllKeys();
        mKeyboardChanged = FALSE;
    }
    Boolean isNotEmpty = FALSE;
    // mCanvas->ClipRect(mDirtyRect, RegionOp_REPLACE, &isNotEmpty);

    if (mKeyboard == NULL) return;

    const Int32 kbdPaddingLeft = mPaddingLeft;
    const Int32 kbdPaddingTop = mPaddingTop;

    mPaint->SetColor(mKeyTextColor);
    Boolean drawSingleKey = FALSE;
    Boolean isNonEmpty = FALSE;
    if (mInvalidatedKey != NULL && (mCanvas->GetClipBounds(mClipRegion, &isNonEmpty), isNonEmpty)) {
        // Is clipRegion completely contained within the invalidated key?
        Int32 keyX, keyY, keyW, keyH;
        mInvalidatedKey->GetX(&keyX);
        mInvalidatedKey->GetY(&keyY);
        mInvalidatedKey->GetWidth(&keyW);
        mInvalidatedKey->GetHeight(&keyH);
        CRect* clipRegion = (CRect*)mClipRegion.Get();
        if (keyX + kbdPaddingLeft - 1 <= clipRegion->mLeft &&
              keyY + kbdPaddingTop - 1 <= clipRegion->mTop &&
              keyX + keyW + kbdPaddingLeft + 1 >= clipRegion->mRight &&
              keyY + keyH + kbdPaddingTop + 1 >= clipRegion->mBottom) {
            drawSingleKey = TRUE;
        }
    }
    mCanvas->DrawColorEx(0x00000000, PorterDuffMode_CLEAR);
    CRect* padding = (CRect*)mPadding.Get();
    const Int32 keyCount = mKeys->GetLength();
    for (Int32 i = 0; i < keyCount; i++) {
        AutoPtr<IKeyboardKey> key = (*mKeys)[i];
        if (drawSingleKey && mInvalidatedKey != key) {
            continue;
        }
        AutoFree< ArrayOf<Int32> > drawableState;
        key->GetCurrentDrawableState((ArrayOf<Int32>**)&drawableState);
        Boolean isStateful = FALSE;
        mKeyBackground->SetState(drawableState.Get(), &isStateful);

        // Switch the character to uppercase if shift is pressed
        String label;
        AutoPtr<ICharSequence> l;
        key->GetLabel((ICharSequence**)&l);
        if (l != NULL) {
            AdjustCase(l)->ToString(&label);
        }

        Int32 keyW, keyH;
        key->GetWidth(&keyW);
        key->GetHeight(&keyH);
        AutoPtr<IRect> bounds;
        mKeyBackground->GetBounds((IRect**)&bounds);
        if (keyW != ((CRect*)bounds.Get())->mRight ||
                keyH != ((CRect*)bounds.Get())->mBottom) {
            mKeyBackground->SetBounds(0, 0, keyW, keyH);
        }
        Int32 keyX, keyY;
        key->GetX(&keyX);
        key->GetY(&keyY);
        mCanvas->Translate(keyX + kbdPaddingLeft, keyY + kbdPaddingTop);
        mKeyBackground->Draw(mCanvas);

        if (!label.IsNull()) {
            // For characters, use large font. For labels like "Done", use small font.
            if (label.GetCharCount() > 1 /*&& ((CKeyboardKey*)key.Get())->mCodes->GetLength() < 2*/) {
                mPaint->SetTextSize(mLabelTextSize);
                mPaint->SetTypeface(Typeface::DEFAULT_BOLD);
            }
            else {
                mPaint->SetTextSize(mKeyTextSize);
                mPaint->SetTypeface(Typeface::DEFAULT);
            }
            // Draw a drop shadow for the text
            mPaint->SetShadowLayer(mShadowRadius, 0, 0, mShadowColor);
            // Draw the text
            Float textSize = 0.0, tmpDescent = 0.0;
            mPaint->GetTextSize(&textSize);
            mPaint->Descent(&tmpDescent);
            mCanvas->DrawTextInString(label,
                (keyW - padding->mLeft - padding->mRight) / 2 + padding->mLeft,
                (keyH - padding->mTop - padding->mBottom) / 2 + (textSize - tmpDescent) / 2 + padding->mTop,
                mPaint);
            // Turn off drop shadow
            mPaint->SetShadowLayer(0, 0, 0, 0);
        }
        else {
            AutoPtr<IDrawable> icon;
            key->GetIcon((IDrawable**)&icon);
            if (icon != NULL) {
                Int32 intrinsicWidth = 0, intrinsicHeight = 0;
                icon->GetIntrinsicWidth(&intrinsicWidth);
                icon->GetIntrinsicHeight(&intrinsicHeight);
                const Int32 drawableX = (keyW - padding->mLeft - padding->mRight
                        - intrinsicWidth) / 2 + padding->mLeft;
                const Int32 drawableY = (keyH - padding->mTop - padding->mBottom
                        - intrinsicHeight) / 2 + padding->mTop;
                mCanvas->Translate(drawableX, drawableY);
                icon->SetBounds(0, 0, intrinsicWidth, intrinsicHeight);
                icon->Draw(mCanvas);
                mCanvas->Translate(-drawableX, -drawableY);
            }
        }
        mCanvas->Translate(-keyX - kbdPaddingLeft, -keyY - kbdPaddingTop);
    }
    mInvalidatedKey = NULL;
    // Overlay a dark rectangle to dim the keyboard
    if (mMiniKeyboardOnScreen) {
        mPaint->SetColor((Int32)(mBackgroundDimAmount * 0xFF) << 24);
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
    Int32 primaryIndex = NOT_A_KEY;
    Int32 closestKey = NOT_A_KEY;
    Int32 closestKeyDist = mProximityThreshold + 1;
    assert(mDistances != NULL);
    for (Int32 i = 0; i < mDistances->GetLength(); i++) {
        (*mDistances)[i] = Math::INT32_MAX_VALUE;
    }
    AutoFree< ArrayOf<Int32> > nearestKeyIndices;
    Int32 count = 0;
    mKeyboard->GetNearestKeys(x, y, (ArrayOf<Int32>**)&nearestKeyIndices, &count);
    const Int32 keyCount = nearestKeyIndices->GetLength();
    for (Int32 i = 0; i < keyCount; i++) {
        AutoPtr<IKeyboardKey> key = (*mKeys)[(*nearestKeyIndices)[i]];
        Int32 dist = 0;
        Boolean isInside = FALSE;
        key->IsInside(x,y, &isInside);
        if (isInside) {
            primaryIndex = (*nearestKeyIndices)[i];
        }

        key->SquaredDistanceFrom(x, y, &dist);
        AutoFree< ArrayOf<Int32> > codes;
        key->GetCodes((ArrayOf<Int32>**)&codes);
        if (((mProximityCorrectOn
                && dist < mProximityThreshold)
                || isInside)
                && (*codes.Get())[0] > 32) {
            // Find insertion point
            const Int32 nCodes = codes->GetLength();
            if (dist < closestKeyDist) {
                closestKeyDist = dist;
                closestKey = (*nearestKeyIndices)[i];
            }

            if (allKeys == NULL) continue;

            for (Int32 j = 0; j < mDistances->GetLength(); j++) {
                if ((*mDistances)[j] > dist) {
                    // Make space for nCodes codes
                    memcpy(mDistances->GetPayload() + j + nCodes, mDistances->GetPayload() + j,
                            (mDistances->GetLength() - j - nCodes) * sizeof(Int32));
                    memcpy(allKeys->GetPayload() + j + nCodes, allKeys->GetPayload() + j,
                            (allKeys->GetLength() - j - nCodes) * sizeof(Int32));
                    for (Int32 c = 0; c < nCodes; c++) {
                        (*allKeys)[j + c] = (*codes.Get())[c];
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
    if (index != NOT_A_KEY && index < mKeys->GetLength()) {
        AutoPtr<IKeyboardKey> key = (*mKeys)[index];
        AutoPtr<ICharSequence> text;
        key->GetText((ICharSequence**)&text);
        if (text != NULL) {
            mKeyboardActionListener->OnText(text);
            mKeyboardActionListener->OnRelease(NOT_A_KEY);
        }
        else {
            AutoFree< ArrayOf<Int32> > _codes;
            key->GetCodes((ArrayOf<Int32>**)&_codes);
            Int32 code = (*_codes.Get())[0];
            //TextEntryState.keyPressedAt(key, x, y);
            ArrayOf_<Int32, MAX_NEARBY_KEYS> codes;
            for (Int32 i = 0; i < MAX_NEARBY_KEYS; ++i) {
                   codes[i] = NOT_A_KEY;
            }
            GetKeyIndices(x, y, &codes);
            // Multi-tap
            if (mInMultiTap) {
                if (mTapCount != -1) {
                    ArrayOf_<Int32, 1> tmpArr;
                    tmpArr[0] = KEY_DELETE[0];
                    mKeyboardActionListener->OnKey(Keyboard_KEYCODE_DELETE, &tmpArr);
                }
                else {
                    mTapCount = 0;
                }
                code = (*_codes.Get())[mTapCount];
            }
            mKeyboardActionListener->OnKey(code, &codes);
            mKeyboardActionListener->OnRelease(code);
        }
        mLastSentIndex = index;
        mLastTapTime = eventTime;
    }
}

/**
 * Handle multi-tap keys by producing the key label for the current multi-tap state.
 */
AutoPtr<ICharSequence> KeyboardView::GetPreviewText(
    /* [in] */ IKeyboardKey* key)
{
    if (mInMultiTap) {
        // Multi-tap
        AutoFree< ArrayOf<Int32> > codes;
        key->GetCodes((ArrayOf<Int32>**)&codes);
        // mPreviewLabel.SetLength(0);
        mPreviewLabel += (Char32)(*codes.Get())[mTapCount < 0 ? 0 : mTapCount];
        AutoPtr<ICharSequence> text;
        CStringWrapper::New(String(mPreviewLabel), (ICharSequence**)&text);
        return AdjustCase(text);
    }
    else {
        AutoPtr<ICharSequence> label;
        key->GetLabel((ICharSequence**)&label);
        return AdjustCase(label);
    }
}

void KeyboardView::ShowPreview(
    /* [in] */ Int32 keyIndex)
{
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
        pHandlerFunc = &KeyboardView::HandleShowPreview;
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        Boolean showing = FALSE;
        if (mPreviewPopup->IsShowing(&showing), showing) {
            if (keyIndex == NOT_A_KEY) {
                void (STDCALL KeyboardView::*pHandlerFunc)();
                pHandlerFunc = &KeyboardView::HandleRemovePreview;
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
            }
            else {
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
    if (keyIndex < 0 || keyIndex >= mKeys->GetLength()) return;
    AutoPtr<IKeyboardKey> key = (*mKeys)[keyIndex];
    AutoPtr<IDrawable> icon;
    key->GetIcon((IDrawable**)&icon);
    if (icon != NULL) {
        AutoPtr<IDrawable> iconPreview;
        key->GetIconPreview((IDrawable**)&iconPreview);
        mPreviewText->SetCompoundDrawables(NULL, NULL, NULL,
                iconPreview != NULL ? iconPreview : icon);
        mPreviewText->SetText(NULL);
    }
    else {
        mPreviewText->SetCompoundDrawables(NULL, NULL, NULL, NULL);
        mPreviewText->SetText(GetPreviewText(key));

        AutoPtr<ICharSequence> label;
        key->GetLabel((ICharSequence**)&label);
        Int32 len = 0;
        label->GetLength(&len);
        AutoFree< ArrayOf<Int32> > codes;
        key->GetCodes((ArrayOf<Int32>**)&codes);
        if (len > 1 && codes->GetLength() < 2) {
            mPreviewText->SetTextSizeEx(TypedValue_COMPLEX_UNIT_PX, mKeyTextSize);
            mPreviewText->SetTypefaceEx(Typeface::DEFAULT_BOLD);
        }
        else {
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
    Int32 keyW;
    key->GetWidth(&keyW);
    Int32 popupWidth = Math::Max(tmp, keyW + tmpL + tmpR);
    const Int32 popupHeight = mPreviewHeight;
    AutoPtr<IViewGroupLayoutParams> lp;
    mPreviewText->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp != NULL) {
        lp->SetWidth(popupWidth);
        lp->SetHeight(popupHeight);
    }
    if (!mPreviewCentered) {
        mPreviewText->GetPaddingLeft(&tmpL);
        Int32 keyX, keyY;
        key->GetX(&keyX);
        key->GetY(&keyY);
        mPopupPreviewX = keyX - tmpL + mPaddingLeft;
        mPopupPreviewY = keyY - popupHeight + mPreviewOffset;
    }
    else {
        // TODO: Fix this if centering is brought back
        mPreviewText->GetMeasuredWidth(&tmp);
        mPopupPreviewX = 160 - tmp / 2;
        mPreviewText->GetMeasuredHeight(&tmp);
        mPopupPreviewY = - tmp;
    }
    void (STDCALL KeyboardView::*pHandlerFunc)();
    pHandlerFunc = &KeyboardView::HandleRemovePreview;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
    if (mOffsetInWindow == NULL) {
        mOffsetInWindow = ArrayOf<Int32>::Alloc(2);
        Int32 tmpX = 0, tmpY = 0;
        GetLocationInWindow(&tmpX, &tmpY);
        (*mOffsetInWindow)[0] = tmpX + mMiniKeyboardOffsetX; // Offset may be zero
        (*mOffsetInWindow)[1] = tmpY + mMiniKeyboardOffsetY; // Offset may be zero
        GetLocationOnScreen(&tmpX, &tmpY);
        mWindowY = tmpY;
    }
    // Set the preview background state
    AutoPtr<IDrawable> drawable;
    mPreviewText->GetBackground((IDrawable**)&drawable);
    AutoFree< ArrayOf<Int32> >tmpArr;
    Int32 resId;
    key->GetPopupResId(&resId);
    if (resId != 0) {
        tmpArr = ArrayOf<Int32>::Alloc(1);
        (*tmpArr)[0] = LONG_PRESSABLE_STATE_SET[0];
    }
    else {
        tmpArr = ArrayOf<Int32>::Alloc(0);
    }
    Boolean isStateful = FALSE;
    drawable->SetState(tmpArr, &isStateful);
    mPopupPreviewX += (*mOffsetInWindow)[0];
    mPopupPreviewY += (*mOffsetInWindow)[1];

    // If the popup cannot be shown above the key, put it on the side
    if (mPopupPreviewY + mWindowY < 0) {
        // If the key you're pressing is on the left side of the keyboard, show the popup on
        // the right, offset by enough to see at least one key to the left/right.
        Int32 keyX, keyW;
        key->GetX(&keyX);
        key->GetWidth(&keyW);
        if (keyX + keyW <= GetWidth() / 2) {
            mPopupPreviewX += (Int32)(keyW * 2.5);
        }
        else {
            mPopupPreviewX -= (Int32)(keyW * 2.5);
        }
        mPopupPreviewY += popupHeight;
    }

    Boolean showing = FALSE;
    if (mPreviewPopup->IsShowing(&showing), showing) {
        mPreviewPopup->UpdateEx2(mPopupPreviewX, mPopupPreviewY,
                popupWidth, popupHeight);
    }
    else {
        mPreviewPopup->SetWidth(popupWidth);
        mPreviewPopup->SetHeight(popupHeight);
        mPreviewPopup->ShowAtLocation(mPopupParent, Gravity_NO_GRAVITY,
                mPopupPreviewX, mPopupPreviewY);
    }
    mPreviewText->SetVisibility(VISIBLE);
}

ECode KeyboardView::InvalidateAllKeys()
{
    mDirtyRect->Union(0, 0, GetWidth(), GetHeight());
    mDrawPending = TRUE;
    Invalidate();
    return NOERROR;
}

ECode KeyboardView::InvalidateKey(
    /* [in] */ Int32 keyIndex)
{
    if (mKeys == NULL) return NOERROR;
    if (keyIndex < 0 || keyIndex >= mKeys->GetLength()) {
        return NOERROR;
    }
    IKeyboardKey* key = (*mKeys)[keyIndex];
    mInvalidatedKey = key;
    Int32 keyX, keyY, keyW, keyH;
    key->GetX(&keyX);
    key->GetY(&keyY);
    key->GetWidth(&keyW);
    key->GetHeight(&keyH);
    mDirtyRect->Union(keyX + mPaddingLeft, keyY + mPaddingTop,
            keyX + keyW + mPaddingLeft, keyY + keyH + mPaddingTop);
    // OnBufferDraw();
    Invalidate(keyX + mPaddingLeft, keyY + mPaddingTop,
            keyX + keyW + mPaddingLeft, keyY + keyH + mPaddingTop);
    return NOERROR;
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

    AutoPtr<IKeyboardKey> popupKey = (*mKeys)[mCurrentKey];
    Boolean result = FALSE;
    OnLongPress(popupKey, &result);
    if (result) {
        mAbortKey = TRUE;
        ShowPreview(NOT_A_KEY);
    }
    return result;
}

ECode KeyboardView::OnLongPress(
    /* [in] */ IKeyboardKey* popupKey,
    /* [out] */ Boolean* res)
{
    assert(res != NULL && popupKey != NULL);
    Int32 popupKeyboardId;
    popupKey->GetPopupResId(&popupKeyboardId);

    if (popupKeyboardId != 0) {
        mMiniKeyboardContainer = mMiniKeyboardCache[popupKey];
        if (mMiniKeyboardContainer == NULL) {
            AutoPtr<ILayoutInflater> inflater;
            GetContext()->GetSystemService(
                    Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);
            inflater->Inflate(mPopupLayout, NULL, (IView**)&mMiniKeyboardContainer);
            mMiniKeyboardContainer->FindViewById(0x01020026 /*com.android.internal.R.id.keyboardView*/,
                    (IView**)&mMiniKeyboard);
            AutoPtr<IView> closeButton;
            mMiniKeyboardContainer->FindViewById(0x01020027 /*com.android.internal.R.id.closeButton*/,
                    (IView**)&closeButton);
            if (closeButton != NULL) {
                closeButton->SetOnClickListener((IViewOnClickListener*)this->Probe(EIID_IViewOnClickListener));
            }
            mMiniKeyboard->SetOnKeyboardActionListener(new _OnKeyboardActionListener(this));
            //mInputView.setSuggest(mSuggest);
            AutoPtr<IKeyboard> keyboard;
            AutoPtr<ICharSequence> popupCharacters;
            popupKey->GetPopupCharacters((ICharSequence**)&popupCharacters);
            if (popupCharacters != NULL) {
                keyboard = new Keyboard(GetContext(), popupKeyboardId,
                        popupCharacters, -1, GetPaddingLeft() + GetPaddingRight());
            }
            else {
                keyboard = new Keyboard(GetContext(), popupKeyboardId);
            }
            ((KeyboardView*)mMiniKeyboard.Get())->SetKeyboard(keyboard);
            ((KeyboardView*)mMiniKeyboard.Get())->SetPopupParent((IView*)this->Probe(EIID_IView));
            mMiniKeyboardContainer->Measure(
                    View::MeasureSpec::MakeMeasureSpec(GetWidth(), View::MeasureSpec::AT_MOST),
                    View::MeasureSpec::MakeMeasureSpec(GetHeight(), View::MeasureSpec::AT_MOST));

            mMiniKeyboardCache[popupKey] = mMiniKeyboardContainer;
        }
        else {
            mMiniKeyboardContainer->FindViewById(0x01020026 /*com.android.internal.R.id.keyboardView*/,
                    (IView**)&mMiniKeyboard);
        }
        if (mWindowOffset == NULL) {
            mWindowOffset = ArrayOf<Int32>::Alloc(2);
            Int32 tmpX = 0, tmpY = 0;
            GetLocationInWindow(&tmpX, &tmpY);
            (*mWindowOffset)[0] = tmpX;
            (*mWindowOffset)[1] = tmpY;
        }
        Int32 keyX, keyY, keyW;
        popupKey->GetX(&keyX);
        popupKey->GetY(&keyY);
        popupKey->GetWidth(&keyW);
        mPopupX = keyX + mPaddingLeft;
        mPopupY = keyY + mPaddingTop;
        Int32 meauseredW = 0, meauseredH = 0;
        mMiniKeyboardContainer->GetMeasuredWidth(&meauseredW);
        mPopupX = mPopupX + keyW - meauseredW;
        mMiniKeyboardContainer->GetMeasuredHeight(&meauseredH);
        mPopupY = mPopupY - meauseredH;
        Int32 paddingRight = 0, paddingBottom = 0;
        mMiniKeyboardContainer->GetPaddingRight(&paddingRight);
        Int32 x = mPopupX + paddingRight + (*mWindowOffset)[0];
        mMiniKeyboardContainer->GetPaddingBottom(&paddingBottom);
        Int32 y = mPopupY + paddingBottom + (*mWindowOffset)[1];
        mMiniKeyboard->SetPopupOffset(x < 0 ? 0 : x, y);
        Boolean shifted = FALSE;
        IsShifted(&shifted);
        Boolean temp = FALSE;
        mMiniKeyboard->SetShifted(shifted, &temp);
        mPopupKeyboard->SetContentView(mMiniKeyboardContainer);
        mPopupKeyboard->SetWidth(meauseredW);
        mPopupKeyboard->SetHeight(meauseredH);
        mPopupKeyboard->ShowAtLocation((IView*)this->Probe(EIID_IView), Gravity_NO_GRAVITY, x, y);
        mMiniKeyboardOnScreen = TRUE;
        //mMiniKeyboard.onTouchEvent(getTranslatedEvent(me));
        InvalidateAllKeys();
        *res = TRUE;
        return NOERROR;
    }
    *res = FALSE;
    return NOERROR;
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
                    tX, tY, tState, (CMotionEvent**)&down);
            result = OnModifiedTouchEvent(down, FALSE);
            down->Recycle();
            // If it's an up action, then deliver the up as well.
            if (action == KeyEvent_ACTION_UP) {
                result = OnModifiedTouchEvent(me, TRUE);
            }
        }
        else {
            Int32 tState = 0;
            me->GetMetaState(&tState);
            // Send an up event for the last pointer
            AutoPtr<CMotionEvent> up;
            CMotionEvent::Obtain(now, now, KeyEvent_ACTION_UP,
                    mOldPointerX, mOldPointerY, tState, (CMotionEvent**)&up);
            result = OnModifiedTouchEvent(up, TRUE);
            up->Recycle();
        }
    }
    else {
        if (pointerCount == 1) {
            result = OnModifiedTouchEvent(me, FALSE);
            me->GetX(&mOldPointerX);
            me->GetY(&mOldPointerY);
        }
        else {
            // Don't do anything when 2 pointers are down and moving.
            result = TRUE;
        }
    }
    mOldPointerCount = pointerCount;

    return result;
}

Boolean KeyboardView::OnModifiedTouchEvent(
    /* [in] */ IMotionEvent* me,
    /* [in] */ Boolean possiblePoly)
{
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
    if (action == KeyEvent_ACTION_DOWN) mSwipeTracker->Clear();
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
        void (STDCALL KeyboardView::*pHandlerFunc2)(IMotionEvent*);
        pHandlerFunc2 = &KeyboardView::HandleLongPress;
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc2);
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
            if (keyIndex != NOT_A_KEY) {
                AutoFree< ArrayOf<Int32> > codes;
                (*mKeys)[keyIndex]->GetCodes((ArrayOf<Int32>**)&codes);
                mKeyboardActionListener->OnPress((*codes.Get())[0]);
            }
            else {
                mKeyboardActionListener->OnPress(0);
            }
            if (mCurrentKey >= 0) {
                Boolean repeatable;
                (*mKeys)[mCurrentKey]->IsRepeatable(&repeatable);
                if (repeatable) {
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
                }
                else {
                    if (keyIndex == mCurrentKey) {
                        mCurrentKeyTime += eventTime - mLastMoveTime;
                        continueLongPress = TRUE;
                    }
                    else if (mRepeatKeyIndex == NOT_A_KEY) {
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
            }
            else {
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
            for (Int32 i = 0; i < mKeyIndices->GetLength(); ++i) {
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
    IKeyboardKey* key = (*mKeys)[mRepeatKeyIndex];
    Int32 keyX, keyY;
    key->GetX(&keyX);
    key->GetY(&keyY);
    DetectAndSendKey(mCurrentKey, keyX, keyY, mLastTapTime);
    return TRUE;
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

ECode KeyboardView::Closing()
{
    assert(mPreviewPopup != NULL);
    Boolean showing = FALSE;
    if (mPreviewPopup->IsShowing(&showing), showing) {
        mPreviewPopup->Dismiss();
    }
    RemoveMessages();

    DismissPopupKeyboard();
    mBuffer = NULL;
    mCanvas = NULL;
    mMiniKeyboardCache.Clear();
    return NOERROR;
}

void KeyboardView::RemoveMessages()
{
    void (STDCALL KeyboardView::*pHandlerFunc)();
    pHandlerFunc = &KeyboardView::HandleRepeat;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    void (STDCALL KeyboardView::*pHandlerFunc2)(IMotionEvent*);
    pHandlerFunc2 = &KeyboardView::HandleLongPress;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc2);

    void (STDCALL KeyboardView::*pHandlerFunc3)(Int32);
    pHandlerFunc3 = &KeyboardView::HandleShowPreview;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc3);
}

void KeyboardView::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();
    Closing();
}

void KeyboardView::DismissPopupKeyboard()
{
    assert(mPopupKeyboard != NULL);
    Boolean showing = FALSE;
    if (mPopupKeyboard->IsShowing(&showing), showing) {
        mPopupKeyboard->Dismiss();
        mMiniKeyboardOnScreen = FALSE;
        InvalidateAllKeys();
    }
}

ECode KeyboardView::HandleBack(
    /* [out] */ Boolean* res)
{
    assert(res != NULL && mPopupKeyboard != NULL);
    Boolean showing = FALSE;
    if (mPopupKeyboard->IsShowing(&showing), showing) {
        DismissPopupKeyboard();
        *res = TRUE;
        return NOERROR;
    }
    *res = FALSE;
    return NOERROR;
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
    IKeyboardKey* key = (*mKeys)[keyIndex];
    AutoFree< ArrayOf<Int32> > codes;
    key->GetCodes((ArrayOf<Int32>**)&codes);
    if (codes->GetLength() > 1) {
        mInMultiTap = TRUE;
        if (eventTime < mLastTapTime + MULTITAP_INTERVAL
                && keyIndex == mLastSentIndex) {
            mTapCount = (mTapCount + 1) % codes->GetLength();
            return;
        }
        else {
            mTapCount = -1;
            return;
        }
    }
    if (eventTime > mLastTapTime + MULTITAP_INTERVAL || keyIndex != mLastSentIndex) {
        ResetMultiTap();
    }
}

void KeyboardView::HandleShowPreview(
    /* [in] */ Int32 keyIndex)
{
    ShowKey(keyIndex);
}

void KeyboardView::HandleRemovePreview()
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
