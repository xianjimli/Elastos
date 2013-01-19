
#include "cmdef.h"
#include "GeckoEvent.h"

const Int32 GeckoEvent::INVALID = -1;
const Int32 GeckoEvent::NATIVE_POKE = 0;
const Int32 GeckoEvent::KEY_EVENT = 1;
const Int32 GeckoEvent::MOTION_EVENT = 2;
const Int32 GeckoEvent::ORIENTATION_EVENT = 3;
const Int32 GeckoEvent::ACCELERATION_EVENT = 4;
const Int32 GeckoEvent::LOCATION_EVENT = 5;
const Int32 GeckoEvent::IME_EVENT = 6;
const Int32 GeckoEvent::DRAW = 7;
const Int32 GeckoEvent::SIZE_CHANGED = 8;
const Int32 GeckoEvent::ACTIVITY_STOPPING = 9;
const Int32 GeckoEvent::ACTIVITY_PAUSING = 10;
const Int32 GeckoEvent::ACTIVITY_SHUTDOWN = 11;
const Int32 GeckoEvent::LOAD_URI = 12;
const Int32 GeckoEvent::SURFACE_CREATED = 13;
const Int32 GeckoEvent::SURFACE_DESTROYED = 14;
const Int32 GeckoEvent::GECKO_EVENT_SYNC = 15;

const Int32 GeckoEvent::IME_COMPOSITION_END = 0;
const Int32 GeckoEvent::IME_COMPOSITION_BEGIN = 1;
const Int32 GeckoEvent::IME_SET_TEXT = 2;
const Int32 GeckoEvent::IME_GET_TEXT = 3;
const Int32 GeckoEvent::IME_DELETE_TEXT = 4;
const Int32 GeckoEvent::IME_SET_SELECTION = 5;
const Int32 GeckoEvent::IME_GET_SELECTION = 6;
const Int32 GeckoEvent::IME_ADD_RANGE = 7;

const Int32 GeckoEvent::IME_RANGE_CARETPOSITION = 1;
const Int32 GeckoEvent::IME_RANGE_RAWINPUT = 2;
const Int32 GeckoEvent::IME_RANGE_SELECTEDRAWTEXT = 3;
const Int32 GeckoEvent::IME_RANGE_CONVERTEDTEXT = 4;
const Int32 GeckoEvent::IME_RANGE_SELECTEDCONVERTEDTEXT = 5;

const Int32 GeckoEvent::IME_RANGE_UNDERLINE = 1;
const Int32 GeckoEvent::IME_RANGE_FORECOLOR = 2;
const Int32 GeckoEvent::IME_RANGE_BACKCOLOR = 4;

GeckoEvent::~GeckoEvent ()
{}

GeckoEvent::GeckoEvent()
    : mType(NATIVE_POKE)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 evType)
    : mType(evType)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{}

GeckoEvent::GeckoEvent(
    /* [in] */ IKeyEvent* k)
    : mType(KEY_EVENT)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{
    ASSERT_SUCCEEDED(k->GetAction(&mAction));
    ASSERT_SUCCEEDED(k->GetEventTime(&mTime));
    ASSERT_SUCCEEDED(k->GetMetaState(&mMetaState));
    ASSERT_SUCCEEDED(k->GetFlags(&mFlags));
    ASSERT_SUCCEEDED(k->GetKeyCode(&mKeyCode));
    ASSERT_SUCCEEDED(k->GetUnicodeChar(&mUnicodeChar));
    ASSERT_SUCCEEDED(k->GetCharacters(&mCharacters));
}

GeckoEvent::GeckoEvent(
    /* [in] */ IMotionEvent* m)
    : mType(MOTION_EVENT)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{
    ASSERT_SUCCEEDED(m->GetAction(&mAction));
    ASSERT_SUCCEEDED(m->GetEventTime(&mTime));
    ASSERT_SUCCEEDED(m->GetMetaState(&mMetaState));
    Float x = 0, y = 0;
    ASSERT_SUCCEEDED(m->GetXEx(0, &x));
    ASSERT_SUCCEEDED(m->GetYEx(0, &y));
    ASSERT_SUCCEEDED(CPoint::New((Int32)x, (Int32)y, (IPoint**)&mP0));

    ASSERT_SUCCEEDED(m->GetPointerCount(&mCount));
    if (mCount > 1) {
        ASSERT_SUCCEEDED(m->GetXEx(1, &x));
        ASSERT_SUCCEEDED(m->GetYEx(1, &y));
        ASSERT_SUCCEEDED(CPoint::New((Int32)x, (Int32)y,(IPoint**)&mP1));
    }
}

GeckoEvent::GeckoEvent(
    /* [in] */ ILocation* l,
    /* [in] */ IAddress* a)
    : mType(LOCATION_EVENT)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mLocation(l)
    , mAddress(a)
    , mNativeWindow(0)
{}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 imeAction,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
    : mType(IME_EVENT)
    , mAction(imeAction)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(offset)
    , mCount(count)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int32 rangeType,
    /* [in] */ Int32 rangeStyles,
    /* [in] */ Int32 rangeForeColor,
    /* [in] */ Int32 rangeBackColor,
    /* [in] */ const String& text)
    : mType(IME_EVENT)
    , mAction(IME_SET_TEXT)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(offset)
    , mCount(count)
    , mCharacters(text)
    , mRangeType(rangeType)
    , mRangeStyles(rangeStyles)
    , mRangeForeColor(rangeForeColor)
    , mRangeBackColor(rangeBackColor)
    , mNativeWindow(0)
{}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int32 rangeType,
    /* [in] */ Int32 rangeStyles,
    /* [in] */ Int32 rangeForeColor,
    /* [in] */ Int32 rangeBackColor)
    : mType(IME_EVENT)
    , mAction(IME_ADD_RANGE)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(offset)
    , mCount(count)
    , mRangeType(rangeType)
    , mRangeStyles(rangeStyles)
    , mRangeForeColor(rangeForeColor)
    , mRangeBackColor(rangeBackColor)
    , mNativeWindow(0)
{}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 etype,
    /* [in] */ IRect* dirty)
    : mType(0)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{
    if (etype != DRAW) {
        mType = INVALID;
        return;
    }

    mType = etype;
    mRect = dirty;
}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 etype,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 screenw,
    /* [in] */ Int32 screenh)
    : mType(0)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{
    if (etype != SIZE_CHANGED) {
        mType = INVALID;
        return;
    }

    mType = etype;

    ASSERT_SUCCEEDED(CPoint::New(w, h, (IPoint**)&mP0));
    ASSERT_SUCCEEDED(CPoint::New(screenw, screenh, (IPoint**)&mP1));
}

GeckoEvent::GeckoEvent(
    /* [in] */ const String& uri)
    : mType(LOAD_URI)
    , mAction(0)
    , mTime(0)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mAlpha(0)
    , mBeta(0)
    , mGamma(0)
    , mMetaState(0)
    , mFlags(0)
    , mKeyCode(0)
    , mUnicodeChar(0)
    , mOffset(0)
    , mCount(0)
    , mCharacters(uri)
    , mRangeType(0)
    , mRangeStyles(0)
    , mRangeForeColor(0)
    , mRangeBackColor(0)
    , mNativeWindow(0)
{}

UInt32 GeckoEvent::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GeckoEvent::Release()
{
    return ElRefBase::Release();
}

