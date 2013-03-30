
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

ECode CConfiguration::constructor()
{
    return SetToDefaults();
}

ECode CConfiguration::constructor(
    /* [in] */ IConfiguration* o)
{
    assert(o != NULL);

    return SetTo(o);
}

ECode CConfiguration::SetTo(
    /* [in] */ IConfiguration* o)
{
    assert(o != NULL);

    CConfiguration* c = (CConfiguration*)o;
    mFontScale = c->mFontScale;
    mMcc = c->mMcc;
    mMnc = c->mMnc;
    if (c->mLocale != NULL) {
        // TODO: ALEX
        // mLocale = c->mLocale->Clone();
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
    sb.Append((Int32)((ILocale*)mLocale));
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
    assert(delta != NULL);
    VALIDATE_NOT_NULL(changes);

    CConfiguration* c = (CConfiguration*)delta;
    Int32 changed = 0;

    if (c->mFontScale > 0 && mFontScale != c->mFontScale) {
        changed |= ActivityInfo_CONFIG_FONT_SCALE;
        mFontScale = c->mFontScale;
    }
    if (c->mMcc != 0 && mMcc  != c->mMcc ) {
        changed |= ActivityInfo_CONFIG_MCC;
        mMcc  = c->mMcc ;
    }
    if (c->mMnc != 0 && mMnc!= c->mMnc) {
        changed |= ActivityInfo_CONFIG_MNC;
        mMnc = c->mMnc;
    }
    if (c->mLocale != NULL
            && (mLocale == NULL || (ILocale*)mLocale != (ILocale*)c->mLocale)) {
        changed |= ActivityInfo_CONFIG_LOCALE;
        // TODO: ALEX
        // mLocale = c->mLocale != NULL
        //         ? c->mLocale->Clone() : NULL;
    }
    if (c->mUserSetLocale && (!mUserSetLocale
        || ((changed & ActivityInfo_CONFIG_LOCALE) != 0)))
    {
        mUserSetLocale = true;
        changed |= ActivityInfo_CONFIG_LOCALE;
    }
    if (c->mTouchscreen != Configuration_TOUCHSCREEN_UNDEFINED
            && mTouchscreen != c->mTouchscreen) {
        changed |= ActivityInfo_CONFIG_TOUCHSCREEN;
        mTouchscreen = c->mTouchscreen;
    }
    if (c->mKeyboard != Configuration_KEYBOARD_UNDEFINED
            && mKeyboard != c->mKeyboard) {
        changed |= ActivityInfo_CONFIG_KEYBOARD;
        mKeyboard = c->mKeyboard;
    }
    if (c->mKeyboardHidden != Configuration_KEYBOARDHIDDEN_UNDEFINED
            && mKeyboardHidden != c->mKeyboardHidden) {
        changed |= ActivityInfo_CONFIG_KEYBOARD_HIDDEN;
        mKeyboardHidden = c->mKeyboardHidden;
    }
    if (c->mHardKeyboardHidden != Configuration_HARDKEYBOARDHIDDEN_UNDEFINED
            && mHardKeyboardHidden != c->mHardKeyboardHidden) {
        changed |= ActivityInfo_CONFIG_KEYBOARD_HIDDEN;
        mHardKeyboardHidden = c->mHardKeyboardHidden;
    }
    if (c->mNavigation != Configuration_NAVIGATION_UNDEFINED
            && mNavigation != c->mNavigation) {
        changed |= ActivityInfo_CONFIG_NAVIGATION;
        mNavigation = c->mNavigation;
    }
    if (c->mNavigationHidden != Configuration_NAVIGATIONHIDDEN_UNDEFINED
            && mNavigationHidden != c->mNavigationHidden) {
        changed |= ActivityInfo_CONFIG_KEYBOARD_HIDDEN;
        mNavigationHidden = c->mNavigationHidden;
    }
    if (c->mOrientation != Configuration_ORIENTATION_UNDEFINED
            && mOrientation != c->mOrientation) {
        changed |= ActivityInfo_CONFIG_ORIENTATION;
        mOrientation = c->mOrientation;
    }
    if (c->mScreenLayout != Configuration_SCREENLAYOUT_SIZE_UNDEFINED
            && mScreenLayout != c->mScreenLayout) {
        changed |= ActivityInfo_CONFIG_SCREEN_LAYOUT;
        mScreenLayout = c->mScreenLayout;
    }
    if (c->mUiMode != (Configuration_UI_MODE_TYPE_UNDEFINED
        | Configuration_UI_MODE_NIGHT_UNDEFINED)
            && mUiMode != c->mUiMode) {
        changed |= ActivityInfo_CONFIG_UI_MODE;
        if ((c->mUiMode & Configuration_UI_MODE_TYPE_MASK)
            != Configuration_UI_MODE_TYPE_UNDEFINED) {
            mUiMode = (mUiMode & ~Configuration_UI_MODE_TYPE_MASK)
                    | (c->mUiMode & Configuration_UI_MODE_TYPE_MASK);
        }
        if ((c->mUiMode & Configuration_UI_MODE_NIGHT_MASK)
            != Configuration_UI_MODE_NIGHT_UNDEFINED) {
            mUiMode = (mUiMode & ~Configuration_UI_MODE_NIGHT_MASK)
                    | (c->mUiMode & Configuration_UI_MODE_NIGHT_MASK);
        }
    }

    if (c->mSeq != 0) {
        mSeq = c->mSeq;
    }

    *changes = changed;

    return NOERROR;
}

ECode CConfiguration::Diff(
    /* [in] */ IConfiguration* delta,
    /* [out] */ Int32* result)
{
    assert(delta != NULL);
    VALIDATE_NOT_NULL(result);

    CConfiguration* c = (CConfiguration*)delta;
    Int32 changed = 0;

    if (c->mFontScale > 0 && mFontScale != c->mFontScale) {
        changed |= ActivityInfo_CONFIG_FONT_SCALE;
    }
    if (c->mMcc != 0 && mMcc != c->mMcc) {
        changed |= ActivityInfo_CONFIG_MCC;
    }
    if (c->mMnc != 0 && mMnc != c->mMnc) {
        changed |= ActivityInfo_CONFIG_MNC;
    }
    if (c->mLocale != NULL
        && (mLocale == NULL || (ILocale*)mLocale != (ILocale*)c->mLocale)) {
        changed |= ActivityInfo_CONFIG_LOCALE;
    }
    if (c->mTouchscreen != Configuration_TOUCHSCREEN_UNDEFINED
            && mTouchscreen != c->mTouchscreen) {
        changed |= ActivityInfo_CONFIG_TOUCHSCREEN;
    }
    if (c->mKeyboard != Configuration_KEYBOARD_UNDEFINED
            && mKeyboard != c->mKeyboard) {
        changed |= ActivityInfo_CONFIG_KEYBOARD;
    }
    if (c->mKeyboardHidden != Configuration_KEYBOARDHIDDEN_UNDEFINED
            && mKeyboardHidden != c->mKeyboardHidden) {
        changed |= ActivityInfo_CONFIG_KEYBOARD_HIDDEN;
    }
    if (c->mHardKeyboardHidden != Configuration_HARDKEYBOARDHIDDEN_UNDEFINED
            && mHardKeyboardHidden != c->mHardKeyboardHidden) {
        changed |= ActivityInfo_CONFIG_KEYBOARD_HIDDEN;
    }
    if (c->mNavigation != Configuration_NAVIGATION_UNDEFINED
            && mNavigation != c->mNavigation) {
        changed |= ActivityInfo_CONFIG_NAVIGATION;
    }
    if (c->mNavigationHidden != Configuration_NAVIGATIONHIDDEN_UNDEFINED
            && mNavigationHidden != c->mNavigationHidden) {
        changed |= ActivityInfo_CONFIG_KEYBOARD_HIDDEN;
    }
    if (c->mOrientation != Configuration_ORIENTATION_UNDEFINED
            && mOrientation != c->mOrientation) {
        changed |= ActivityInfo_CONFIG_ORIENTATION;
    }
    if (c->mScreenLayout != Configuration_SCREENLAYOUT_SIZE_UNDEFINED
            && mScreenLayout != c->mScreenLayout) {
        changed |= ActivityInfo_CONFIG_SCREEN_LAYOUT;
    }
    if (c->mUiMode != (Configuration_UI_MODE_TYPE_UNDEFINED
        | Configuration_UI_MODE_NIGHT_UNDEFINED) && mUiMode != c->mUiMode) {
        changed |= ActivityInfo_CONFIG_UI_MODE;
    }

    *result = changed;

    return NOERROR;
}

ECode CConfiguration::IsOtherSeqNewer(
    /* [in] */ IConfiguration* other,
    /* [out] */ Boolean* isBetter)
{
    VALIDATE_NOT_NULL(isBetter);
    CConfiguration* c = (CConfiguration*)other;

    if (other == NULL) {
        // Sanity check.
        *isBetter = FALSE;
        return NOERROR;
    }
    if (c->mSeq == 0) {
        // If the other sequence is not specified, then we must assume
        // it is newer since we don't know any better.
        *isBetter = TRUE;
        return NOERROR;
    }
    if (mSeq == 0) {
        // If this sequence is not specified, then we also consider the
        // other is better.  Yes we have a preference for other.  Sue us.
        *isBetter = TRUE;
        return NOERROR;
    }
    int diff = c->mSeq - mSeq;
    if (diff > 0x10000) {
        // If there has been a sufficiently large jump, assume the
        // sequence has wrapped around.
        *isBetter = FALSE;
        return NOERROR;
    }

    *isBetter = (diff > 0);

    return NOERROR;
}

ECode CConfiguration::Equals(
    /* [in] */ IConfiguration* that,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

    if (that == NULL) {
        *isEqual = FALSE;
        return NOERROR;
    }

    if (that == this) {
        *isEqual = TRUE;
        return NOERROR;
    }

    Int32 result;
    FAIL_RETURN(CompareTo(that, &result));

    return (result == 0);
}

ECode CConfiguration::EqualsEx(
    /* [in] */ IInterface* that,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

    if (!that) {
        *isEqual = FALSE;
        return NOERROR;
    }

    // try {
    IConfiguration * object = IConfiguration::Probe(that);
    if (!object) {
        *isEqual = FALSE;
        return NOERROR;
    }

    return Equals(object, isEqual);
    // } catch (ClassCastException e) {
    // }
    // return false;
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

ECode CConfiguration::GetMcc(
    /* [out] */ Int32* mcc)
{
    VALIDATE_NOT_NULL(mcc);
    *mcc = mMcc;

    return NOERROR;
}

ECode CConfiguration::SetMcc(
    /* [in] */ Int32 mcc)
{
    mMcc = mcc;

    return NOERROR;
}

ECode CConfiguration::GetMnc(
    /* [out] */ Int32* mnc)
{
    VALIDATE_NOT_NULL(mnc);
    *mnc = mMnc;

    return NOERROR;
}

ECode CConfiguration::SetMnc(
    /* [in] */ Int32 mnc)
{
    mMnc = mnc;

    return NOERROR;
}

ECode CConfiguration::GetUiMode(
    /* [out] */ Int32* uiMode)
{
    VALIDATE_NOT_NULL(uiMode);
    *uiMode = mUiMode;

    return NOERROR;
}

ECode CConfiguration::SetUiMode(
    /* [in] */ Int32 uiMode)
{
    mUiMode = uiMode;

    return NOERROR;
}

ECode CConfiguration::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = mLocale;
    mLocale->AddRef();

    return NOERROR;
}

ECode CConfiguration::SetLocale(
    /* [in] */ ILocale* locale)
{
    mLocale = locale;

    return NOERROR;
}

ECode CConfiguration::CompareTo(
    /* [in] */ IConfiguration* object,
    /* [out] */ Int32* result)
{
    assert(object != NULL);
    VALIDATE_NOT_NULL(result);

    CConfiguration* c = (CConfiguration*)object;
    Int32 n;
    Float a = mFontScale;
    Float b = c->mFontScale;
    if (a < b) {
        *result = -1;
        return NOERROR;
    }
    if (a > b) {
        *result = 1;
        return NOERROR;
    }
    n = mMcc - c->mMcc;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mMnc - c->mMnc;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    if (mLocale == NULL) {
        if (c->mLocale != NULL) {
            *result = 1;
            return NOERROR;
        }
    } else if (c->mLocale == NULL) {
        *result = -1;
        return NOERROR;
    } else {
        String str1;
        String str2;

        FAIL_RETURN(mLocale->GetLanguage(&str1));
        FAIL_RETURN(c->mLocale->GetLanguage(&str2));
        n = str1.Compare(str2);
        if (n != 0) {
            *result = n;
            return NOERROR;
        }

        FAIL_RETURN(mLocale->GetCountry(&str1));
        FAIL_RETURN(c->mLocale->GetCountry(&str2));
        n = str1.Compare(str2);
        if (n != 0) {
            *result = n;
            return NOERROR;
        }

        FAIL_RETURN(mLocale->GetVariant(&str1));
        FAIL_RETURN(c->mLocale->GetVariant(&str2));
        n = str1.Compare(str2);
        if (n != 0) {
            *result = n;
            return NOERROR;
        }
    }
    n = mTouchscreen - c->mTouchscreen;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mKeyboard - c->mKeyboard;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mKeyboardHidden - c->mKeyboardHidden;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mHardKeyboardHidden - c->mHardKeyboardHidden;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mNavigation - c->mNavigation;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mNavigationHidden - c->mNavigationHidden;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mOrientation - c->mOrientation;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mScreenLayout - c->mScreenLayout;
    if (n != 0) {
        *result = n;
        return NOERROR;
    }
    n = mUiMode - c->mUiMode;
    //if (n != 0) return n;
    *result = n;

    return NOERROR;
}

ECode CConfiguration::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    FAIL_RETURN(source->ReadFloat(&mFontScale));
    FAIL_RETURN(source->ReadInt32(&mMcc));
    FAIL_RETURN(source->ReadInt32(&mMnc));

    Int32 value;
    FAIL_RETURN(source->ReadInt32(&value));
    if (value != 0) {
        String language;
        String country;
        String variant;
        FAIL_RETURN(source->ReadString(&language));
        FAIL_RETURN(source->ReadString(&country));
        FAIL_RETURN(source->ReadString(&variant));
        FAIL_RETURN(CLocale::New(
            language, country, variant, (ILocale**)&mLocale));
    }

    FAIL_RETURN(source->ReadInt32(&value));
    mUserSetLocale = (value == 1);
    FAIL_RETURN(source->ReadInt32(&value));
    mTouchscreen = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mKeyboard = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mKeyboardHidden = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mHardKeyboardHidden = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mNavigation = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mNavigationHidden = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mOrientation = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mScreenLayout = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mUiMode = value;
    FAIL_RETURN(source->ReadInt32(&value));
    mSeq = value;

    return NOERROR;
}

ECode CConfiguration::constructor(
        /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    return ReadFromParcel(source);
}

ECode CConfiguration::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    dest->WriteFloat(mFontScale);
    dest->WriteInt32(mMcc);
    dest->WriteInt32(mMnc);
    if (mLocale == NULL) {
        dest->WriteInt32(0);
    } else {
        dest->WriteInt32(1);
        String language;
        String country;
        String variant;
        FAIL_RETURN(mLocale->GetLanguage(&language));
        FAIL_RETURN(mLocale->GetCountry(&country));
        FAIL_RETURN(mLocale->GetVariant(&variant));
        dest->WriteString(language);
        dest->WriteString(country);
        dest->WriteString(variant);
    }
    if (mUserSetLocale) {
        dest->WriteInt32(1);
    } else {
        dest->WriteInt32(0);
    }
    dest->WriteInt32(mTouchscreen);
    dest->WriteInt32(mKeyboard);
    dest->WriteInt32(mKeyboardHidden);
    dest->WriteInt32(mHardKeyboardHidden);
    dest->WriteInt32(mNavigation);
    dest->WriteInt32(mNavigationHidden);
    dest->WriteInt32(mOrientation);
    dest->WriteInt32(mScreenLayout);
    dest->WriteInt32(mUiMode);
    dest->WriteInt32(mSeq);

    return NOERROR;
}

ECode CConfiguration::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    // TODO: ALEX need mLocale.hashCode()
    /*
    *hashCode = ((int)mFontScale) + mMcc + mMnc
                + (mLocale != NULL ? mLocale.hashCode() : 0)
                + mTouchscreen
                + mKeyboard + mKeyboardHidden + mHardKeyboardHidden
                + mNavigation + mNavigationHidden
                + mOrientation + mScreenLayout + mUiMode;

    return NOERROR;
    */
    return E_NOT_IMPLEMENTED;
}

