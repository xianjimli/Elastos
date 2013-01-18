
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
{
    InitMembers();
}

GeckoEvent::GeckoEvent()
{
    InitMembers();
    mType = NATIVE_POKE;
}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 evType)
{
    InitMembers();
    mType = evType;
}

GeckoEvent::GeckoEvent(
    /* [in] */ IKeyEvent * pK)
{
    InitMembers();
    ECode ec = NOERROR;

    mType = KEY_EVENT;
    ec = pK->GetAction(&mAction);
    if (FAILED(ec)) return;
    ec = pK->GetEventTime(&mTime);
    if (FAILED(ec)) return;
    ec = pK->GetMetaState(&mMetaState);
    if (FAILED(ec)) return;
    ec = pK->GetFlags(&mFlags);
    if (FAILED(ec)) return;
    ec = pK->GetKeyCode(&mKeyCode);
    if (FAILED(ec)) return;
    ec = pK->GetUnicodeChar(&mUnicodeChar);
    if (FAILED(ec)) return;
    ec = pK->GetCharacters(&mCharacters);
}

GeckoEvent::GeckoEvent(
    /* [in] */ IMotionEvent * pM)
{
    InitMembers();
    mType = MOTION_EVENT;
    ECode ec = pM->GetAction(&mAction);
    ec = pM->GetEventTime(&mTime);
    if (FAILED(ec)) return;
    ec = pM->GetMetaState(&mMetaState);
    if (FAILED(ec)) return;
    Float x = 0, y = 0;
    ec = pM->GetXEx(0, &x);
    if (FAILED(ec)) return;
    ec = pM->GetYEx(0, &y);
    if (FAILED(ec)) return;

    ec = CPoint::New((Int32)x, (Int32)y, (IPoint**)&mP0);
    if (!mP0) return;

    ec = pM->GetPointerCount(&mCount);
    if (mCount > 1) {
        ec = pM->GetXEx(1, &x);
        if (FAILED(ec)) return;
        ec = pM->GetYEx(1, &y);
        if (FAILED(ec)) return;
        ec = CPoint::New((Int32)x, (Int32)y,(IPoint**)&mP1);;
        if (!mP1) return;
    }
}

GeckoEvent::GeckoEvent(
    /* [in] */ ILocation * pL,
    /* [in] */ IAddress* pA)
{
    InitMembers();
    mType = LOCATION_EVENT;
    mLocation = pL;
    mAddress  = pA;
}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 imeAction,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    InitMembers();
    mType = IME_EVENT;
    mAction = imeAction;
    mOffset = offset;
    mCount = count;
}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int32 rangeType,
    /* [in] */ Int32 rangeStyles,
    /* [in] */ Int32 rangeForeColor,
    /* [in] */ Int32 rangeBackColor,
    /* [in] */ const String& text)
{
    InitMembers();
    InitIMERange(IME_SET_TEXT, offset, count, rangeType, rangeStyles,
                     rangeForeColor, rangeBackColor);
    mCharacters = text;
}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int32 rangeType,
    /* [in] */ Int32 rangeStyles,
    /* [in] */ Int32 rangeForeColor,
    /* [in] */ Int32 rangeBackColor)
{
    InitMembers();
    InitIMERange(IME_ADD_RANGE, offset, count, rangeType, rangeStyles,
                 rangeForeColor, rangeBackColor);
}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 etype,
    /* [in] */ IRect * pDirty)
{
    InitMembers();
    if (etype != DRAW) {
        mType = INVALID;
        return;
    }

    mType = etype;
    mRect = pDirty;
}

GeckoEvent::GeckoEvent(
    /* [in] */ Int32 etype,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 screenw,
    /* [in] */ Int32 screenh)
{
    InitMembers();
    if (etype != SIZE_CHANGED) {
        mType = INVALID;
        return;
    }

    mType = etype;

    CPoint::New(w, h, (IPoint**)&mP0);
    CPoint::New(screenw, screenh, (IPoint**)&mP1);
}

GeckoEvent::GeckoEvent(
    /* [in] */ const String& uri)
{
    InitMembers();
    mType = LOAD_URI;
    mCharacters = uri;
}

