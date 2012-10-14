
#include "content/CConfiguration.h"
#include <ext/frameworkdef.h>

CConfiguration::CConfiguration()
    : mFontScale(0.0f)
    , mMcc(0)
    , mMnc(0)
    , mLocale(NULL)
    , mUserSetLocale(FALSE)
    , mScreenLayout(0)
    , mTouchscreen(0)
    , mKeyboard(0)
    , mKeyboardHidden(0)
    , mHardKeyboardHidden(0)
    , mNavigation(0)
    , mNavigationHidden(0)
    , mOrientation(0)
    , mUiMode(0)
    , mSeq(0)
{
}

CConfiguration::~CConfiguration()
{
    if (mLocale) delete mLocale;
}

ECode CConfiguration::constructor()
{
    return SetToDefaults();
}

ECode CConfiguration::constructor(
    /* [in] */ IConfiguration* o)
{
    VALIDATE_NOT_NULL(o);

    return SetTo(o);
}

ECode CConfiguration::SetTo(
    /* [in] */ IConfiguration* o)
{
    VALIDATE_NOT_NULL(o);

    CConfiguration* c = (CConfiguration*)o;
    mFontScale = c->mFontScale;
    mMcc = c->mMcc;
    mMnc = c->mMnc;
    if (c->mLocale != NULL) {
        mLocale = c->mLocale->Clone();
    }
    mUserSetLocale = c->mUserSetLocale;
    mTouchscreen = c->mTouchscreen;
    mKeyboard = c->mKeyboard;
    mKeyboardHidden = c->mKeyboardHidden;
    mHardKeyboardHidden = c->mHardKeyboardHidden;
    mNavigation = c->mNavigation;
    mNavigationHidden = c->mNavigationHidden;
    mOrientation = c->mOrientation;
    mScreenLayout = c->mScreenLayout;
    mUiMode = c->mUiMode;
    mSeq = c->mSeq;
    return NOERROR;
}

ECode CConfiguration::GetDescription(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);

    StringBuf_<128> sb;
    sb.Append("{ scale=");
    sb.Append(mFontScale);
    sb.Append(" imsi=");
    sb.Append(mMcc);
    sb.Append("/");
    sb.Append(mMnc);
    sb.Append(" loc=");
    sb.Append((Int32)mLocale);
    sb.Append(" touch=");
    sb.Append(mTouchscreen);
    sb.Append(" keys=");
    sb.Append(mKeyboard);
    sb.Append("/");
    sb.Append(mKeyboardHidden);
    sb.Append("/");
    sb.Append(mHardKeyboardHidden);
    sb.Append(" nav=");
    sb.Append(mNavigation);
    sb.Append("/");
    sb.Append(mNavigationHidden);
    sb.Append(" orien=");
    sb.Append(mOrientation);
    sb.Append(" layout=");
    sb.Append(mScreenLayout);
    sb.Append(" uiMode=");
    sb.Append(mUiMode);
    if (mSeq != 0) {
        sb.Append(" seq=");
        sb.Append(mSeq);
    }
    sb.Append('}');
    *description = (const char*)sb;
    return NOERROR;
}

ECode CConfiguration::SetToDefaults()
{
    mFontScale = 1;
    mMcc = mMnc = 0;
    mLocale = NULL;
    mUserSetLocale = FALSE;
    mTouchscreen = Configuration_TOUCHSCREEN_UNDEFINED;
    mKeyboard = Configuration_KEYBOARD_UNDEFINED;
    mKeyboardHidden = Configuration_KEYBOARDHIDDEN_UNDEFINED;
    mHardKeyboardHidden = Configuration_HARDKEYBOARDHIDDEN_UNDEFINED;
    mNavigation = Configuration_NAVIGATION_UNDEFINED;
    mNavigationHidden = Configuration_NAVIGATIONHIDDEN_UNDEFINED;
    mOrientation = Configuration_ORIENTATION_UNDEFINED;
    mScreenLayout = Configuration_SCREENLAYOUT_SIZE_UNDEFINED;
    mUiMode = Configuration_UI_MODE_TYPE_UNDEFINED;
    mSeq = 0;
    return NOERROR;
}

ECode CConfiguration::UpdateFrom(
    /* [in] */ IConfiguration* delta,
    /* [out] */ Int32* changes)
{
//	    VALIDATE_NOT_NULL(delta);
//	    VALIDATE_NOT_NULL(changes);
//
//	    Int32 changed = 0;
//	    CConfiguration* d = (CConfiguration*)delta;
//	    if (d->mFontScale > 0 && mFontScale != d->mFontScale) {
//	        changed |= ActivityInfo.CONFIG_FONT_SCALE;
//	        fontScale = delta.fontScale;
//	    }
//	    if (delta.mcc != 0 && mcc != delta.mcc) {
//	        changed |= ActivityInfo.CONFIG_MCC;
//	        mcc = delta.mcc;
//	    }
//	    if (delta.mnc != 0 && mnc != delta.mnc) {
//	        changed |= ActivityInfo.CONFIG_MNC;
//	        mnc = delta.mnc;
//	    }
//	    if (delta.locale != null
//	            && (locale == null || !locale.equals(delta.locale))) {
//	        changed |= ActivityInfo.CONFIG_LOCALE;
//	        locale = delta.locale != null
//	                ? (Locale) delta.locale.clone() : null;
//	    }
//	    if (delta.userSetLocale && (!userSetLocale || ((changed & ActivityInfo.CONFIG_LOCALE) != 0)))
//	    {
//	        userSetLocale = true;
//	        changed |= ActivityInfo.CONFIG_LOCALE;
//	    }
//	    if (delta.touchscreen != TOUCHSCREEN_UNDEFINED
//	            && touchscreen != delta.touchscreen) {
//	        changed |= ActivityInfo.CONFIG_TOUCHSCREEN;
//	        touchscreen = delta.touchscreen;
//	    }
//	    if (delta.keyboard != KEYBOARD_UNDEFINED
//	            && keyboard != delta.keyboard) {
//	        changed |= ActivityInfo.CONFIG_KEYBOARD;
//	        keyboard = delta.keyboard;
//	    }
//	    if (delta.keyboardHidden != KEYBOARDHIDDEN_UNDEFINED
//	            && keyboardHidden != delta.keyboardHidden) {
//	        changed |= ActivityInfo.CONFIG_KEYBOARD_HIDDEN;
//	        keyboardHidden = delta.keyboardHidden;
//	    }
//	    if (delta.hardKeyboardHidden != HARDKEYBOARDHIDDEN_UNDEFINED
//	            && hardKeyboardHidden != delta.hardKeyboardHidden) {
//	        changed |= ActivityInfo.CONFIG_KEYBOARD_HIDDEN;
//	        hardKeyboardHidden = delta.hardKeyboardHidden;
//	    }
//	    if (delta.navigation != NAVIGATION_UNDEFINED
//	            && navigation != delta.navigation) {
//	        changed |= ActivityInfo.CONFIG_NAVIGATION;
//	        navigation = delta.navigation;
//	    }
//	    if (delta.navigationHidden != NAVIGATIONHIDDEN_UNDEFINED
//	            && navigationHidden != delta.navigationHidden) {
//	        changed |= ActivityInfo.CONFIG_KEYBOARD_HIDDEN;
//	        navigationHidden = delta.navigationHidden;
//	    }
//	    if (delta.orientation != ORIENTATION_UNDEFINED
//	            && orientation != delta.orientation) {
//	        changed |= ActivityInfo.CONFIG_ORIENTATION;
//	        orientation = delta.orientation;
//	    }
//	    if (delta.screenLayout != SCREENLAYOUT_SIZE_UNDEFINED
//	            && screenLayout != delta.screenLayout) {
//	        changed |= ActivityInfo.CONFIG_SCREEN_LAYOUT;
//	        screenLayout = delta.screenLayout;
//	    }
//	    if (delta.uiMode != (UI_MODE_TYPE_UNDEFINED|UI_MODE_NIGHT_UNDEFINED)
//	            && uiMode != delta.uiMode) {
//	        changed |= ActivityInfo.CONFIG_UI_MODE;
//	        if ((delta.uiMode&UI_MODE_TYPE_MASK) != UI_MODE_TYPE_UNDEFINED) {
//	            uiMode = (uiMode&~UI_MODE_TYPE_MASK)
//	                    | (delta.uiMode&UI_MODE_TYPE_MASK);
//	        }
//	        if ((delta.uiMode&UI_MODE_NIGHT_MASK) != UI_MODE_NIGHT_UNDEFINED) {
//	            uiMode = (uiMode&~UI_MODE_NIGHT_MASK)
//	                    | (delta.uiMode&UI_MODE_NIGHT_MASK);
//	        }
//	    }
//
//	    if (delta.seq != 0) {
//	        seq = delta.seq;
//	    }
//
//	    return changed;
    return E_NOT_IMPLEMENTED;
}

ECode CConfiguration::Diff(
    /* [in] */ IConfiguration* delta,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CConfiguration::IsOtherSeqNewer(
    /* [in] */ IConfiguration* other,
    /* [out] */ Boolean* isBetter)
{
    return E_NOT_IMPLEMENTED;
}

ECode CConfiguration::Equals(
    /* [in] */ IConfiguration* that,
    /* [out] */ Boolean* isEqual)
{
    return E_NOT_IMPLEMENTED;
}

ECode CConfiguration::EqualsEx(
    /* [in] */ IInterface* that,
    /* [out] */ Boolean* isEqual)
{
    return E_NOT_IMPLEMENTED;
}

ECode CConfiguration::GetFontScale(
    /* [out] */ Float* fontScale)
{
    VALIDATE_NOT_NULL(fontScale);
    *fontScale = mFontScale;
    return NOERROR;
}

ECode CConfiguration::SetFontScale(
    /* [in] */ Float fontScale)
{
    mFontScale = fontScale;
    return NOERROR;
}

ECode CConfiguration::GetScreenLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mScreenLayout;
    return NOERROR;
}

ECode CConfiguration::SetScreenLayout(
    /* [in] */ Int32 layout)
{
    mScreenLayout = layout;
    return NOERROR;
}

ECode CConfiguration::GetTouchscreen(
    /* [out] */ Int32* touchscreen)
{
    VALIDATE_NOT_NULL(touchscreen);
    *touchscreen = mTouchscreen;
    return NOERROR;
}

ECode CConfiguration::SetTouchscreen(
    /* [in] */ Int32 touchscreen)
{
    mTouchscreen = touchscreen;
    return NOERROR;
}

ECode CConfiguration::GetKeyboard(
    /* [out] */ Int32* keyboard)
{
    VALIDATE_NOT_NULL(keyboard);
    *keyboard = mKeyboard;
    return NOERROR;
}

ECode CConfiguration::SetKeyboard(
    /* [in] */ Int32 keyboard)
{
    mKeyboard = keyboard;
    return NOERROR;
}

ECode CConfiguration::GetKeyboardHidden(
    /* [out] */ Int32* keyboardHidden)
{
    VALIDATE_NOT_NULL(keyboardHidden);
    *keyboardHidden = mKeyboardHidden;
    return NOERROR;
}

ECode CConfiguration::SetKeyboardHidden(
    /* [in] */ Int32 keyboardHidden)
{
    mKeyboardHidden = keyboardHidden;
    return NOERROR;
}

ECode CConfiguration::GetHardKeyboardHidden(
    /* [out] */ Int32* hardKeyboardHidden)
{
    VALIDATE_NOT_NULL(hardKeyboardHidden);
    *hardKeyboardHidden = mHardKeyboardHidden;
    return NOERROR;
}

ECode CConfiguration::SetHardKeyboardHidden(
    /* [in] */ Int32 hardKeyboardHidden)
{
    mHardKeyboardHidden = hardKeyboardHidden;
    return NOERROR;
}

ECode CConfiguration::GetNavigation(
    /* [out] */ Int32* navigation)
{
    VALIDATE_NOT_NULL(navigation);
    *navigation = mNavigation;
    return NOERROR;
}

ECode CConfiguration::SetNavigation(
    /* [in] */ Int32 navigation)
{
    mNavigation = navigation;
    return NOERROR;
}

ECode CConfiguration::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mOrientation;
    return NOERROR;
}

ECode CConfiguration::SetOrientation(
    /* [in] */ Int32 orientation)
{
    mOrientation = orientation;
    return NOERROR;
}

ECode CConfiguration::GetSeq(
    /* [out] */ Int32* seq)
{
    VALIDATE_NOT_NULL(seq);
    *seq = mSeq;
    return NOERROR;
}

ECode CConfiguration::SetSeq(
    /* [in] */ Int32 seq)
{
    mSeq = seq;
    return NOERROR;
}

ECode CConfiguration::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CConfiguration::constructor(
        /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    return ReadFromParcel(source);
}

ECode CConfiguration::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CConfiguration::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return E_NOT_IMPLEMENTED;
}

Int32 CConfiguration::Diff(
    /* [in] */ CConfiguration* delta)
{
    return E_NOT_IMPLEMENTED;
}
