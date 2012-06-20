
#include "ext/frameworkdef.h"
#include "view/CWindowManagerLayoutParams.h"
#include "capsule/CActivityInfo.h"
#include "graphics/ElPixelFormat.h"

CWindowManagerLayoutParams::CWindowManagerLayoutParams()
    : mAlpha(1.0f)
    , mDimAmount(1.0f)
    , mCompatibilityParamsBackup(NULL)
{
    mTitle = ArrayOf<Char8>::Alloc(1);

    mScreenBrightness = WindowManagerLayoutParams_BRIGHTNESS_OVERRIDE_NONE;
    mButtonBrightness = WindowManagerLayoutParams_BRIGHTNESS_OVERRIDE_NONE;
    mScreenOrientation = CActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED;

}

CWindowManagerLayoutParams::~CWindowManagerLayoutParams()
{
    ArrayOf<Char8>::Free(mTitle);
    delete[] mCompatibilityParamsBackup;
}

Boolean CWindowManagerLayoutParams::MayUseInputMethod(
    /* [in] */ Int32 flags)
{
    switch (flags & (WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE |
        WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM)) {
    case 0:
    case WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE |
        WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM:
        return TRUE;
    }

    return FALSE;
}

ECode CWindowManagerLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowManagerLayoutParams::SetTitle(
    /* [in] */ const ArrayOf<Char8>& title)
{
//    if (null == title)
//        title = "";
//
//    mTitle = TextUtils.stringOrSpannedString(title);

    ArrayOf<Char8>::Free(mTitle);
    mTitle = title.Clone();

    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetTitle(
    /* [out, callee] */ ArrayOf<Char8>** title)
{
    if (title == NULL) return E_INVALID_ARGUMENT;

    *title = mTitle->Clone();

    return NOERROR;
}

ECode CWindowManagerLayoutParams::CopyFrom(
    /* [in] */ IWindowManagerLayoutParams* source)
{
    if (!source) {
        return E_INVALID_ARGUMENT;
    }

    CWindowManagerLayoutParams* src = (CWindowManagerLayoutParams*)source;
    Int32 changes = 0;

    if (mWidth != src->mWidth) {
        mWidth = src->mWidth;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mHeight != src->mHeight) {
        mHeight = src->mHeight;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mX != src->mX) {
        mX = src->mX;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mY != src->mY) {
        mY = src->mY;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mHorizontalWeight != src->mHorizontalWeight) {
        mHorizontalWeight = src->mHorizontalWeight;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mVerticalWeight != src->mVerticalWeight) {
        mVerticalWeight = src->mVerticalWeight;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mType != src->mType) {
        mType = src->mType;
        changes |= WindowManagerLayoutParams_TYPE_CHANGED;
    }

    if (mMemoryType != src->mMemoryType) {
        mMemoryType = src->mMemoryType;
        changes |= WindowManagerLayoutParams_MEMORY_TYPE_CHANGED;
    }

    if (mFlags != src->mFlags) {
        mFlags = src->mFlags;
        changes |= WindowManagerLayoutParams_FLAGS_CHANGED;
    }

    if (mSoftInputMode != src->mSoftInputMode) {
        mSoftInputMode = src->mSoftInputMode;
        changes |= WindowManagerLayoutParams_SOFT_INPUT_MODE_CHANGED;
    }
    if (mGravity != src->mGravity) {
        mGravity = src->mGravity;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mHorizontalMargin != src->mHorizontalMargin) {
        mHorizontalMargin = src->mHorizontalMargin;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mVerticalMargin != src->mVerticalMargin) {
        mVerticalMargin = src->mVerticalMargin;
        changes |= WindowManagerLayoutParams_LAYOUT_CHANGED;
    }

    if (mFormat != src->mFormat) {
        mFormat = src->mFormat;
        changes |= WindowManagerLayoutParams_FORMAT_CHANGED;
    }

    if (mWindowAnimations != src->mWindowAnimations) {
        mWindowAnimations = src->mWindowAnimations;
        changes |= WindowManagerLayoutParams_ANIMATION_CHANGED;
    }
    if (mToken == NULL) {
        // NOTE: mToken only copied if the recipient doesn't
        // already have one.
        mToken = src->mToken;
    }
    if (mCapsuleName.IsNull()) {
        // NOTE: packageName only copied if the recipient doesn't
        // already have one.
        mCapsuleName = String::Duplicate(src->mCapsuleName);
    }
    if (strcmp((const char*)mTitle, (const char*)src->mTitle) != 0) {
        ArrayOf<Char8>::Free(mTitle);
        mTitle = src->mTitle->Clone();
        changes |= WindowManagerLayoutParams_TITLE_CHANGED;
    }

    if (mAlpha != src->mAlpha) {
        mAlpha = src->mAlpha;
        changes |= WindowManagerLayoutParams_ALPHA_CHANGED;
    }
    if (mDimAmount != src->mDimAmount) {
        mDimAmount = src->mDimAmount;
        changes |= WindowManagerLayoutParams_DIM_AMOUNT_CHANGED;
    }
    if (mScreenBrightness != src->mScreenBrightness) {
        mScreenBrightness = src->mScreenBrightness;
        changes |= WindowManagerLayoutParams_SCREEN_BRIGHTNESS_CHANGED;
    }
    if (mButtonBrightness != src->mButtonBrightness) {
        mButtonBrightness = src->mButtonBrightness;
        changes |= WindowManagerLayoutParams_BUTTON_BRIGHTNESS_CHANGED;
    }

    if (mScreenOrientation != src->mScreenOrientation) {
        mScreenOrientation = src->mScreenOrientation;
        changes |= WindowManagerLayoutParams_SCREEN_ORIENTATION_CHANGED;
    }

    return NOERROR;
}

/**
 * Scale the layout params' coordinates and size.
 * @hide
 */
ECode CWindowManagerLayoutParams::Scale(
    /* [in] */ Float scale)
{
    mX = (Int32)(mX * scale + 0.5f);
    mY = (Int32)(mY * scale + 0.5f);

    if (mWidth > 0) {
        mWidth = (Int32)(mWidth * scale + 0.5f);
    }

    if (mHeight > 0) {
        mHeight = (Int32)(mHeight * scale + 0.5f);
    }

    return NOERROR;
}

/**
 * Backup the layout parameters used in compatibility mode.
 * @see LayoutParams#restore()
 */
ECode CWindowManagerLayoutParams::Backup()
{
    if (mCompatibilityParamsBackup == NULL) {
        // we backup 4 elements, x, y, mWidth, mHeight
        mCompatibilityParamsBackup = new int[4];
    }
    mCompatibilityParamsBackup[0] = mX;
    mCompatibilityParamsBackup[1] = mY;
    mCompatibilityParamsBackup[2] = mWidth;
    mCompatibilityParamsBackup[3] = mHeight;

    return NOERROR;
}

/**
 * Restore the layout params' coordinates, size and gravity
 * @see LayoutParams#backup()
 */
ECode CWindowManagerLayoutParams::Restore()
{
    if (mCompatibilityParamsBackup != NULL) {
        mX = mCompatibilityParamsBackup[0];
        mY = mCompatibilityParamsBackup[1];
        mWidth = mCompatibilityParamsBackup[2];
        mHeight = mCompatibilityParamsBackup[3];
    }

    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    if (!source) {
        return E_INVALID_ARGUMENT;
    }

    source->ReadInt32(&mWidth);
    source->ReadInt32(&mHeight);
    source->ReadInt32(&mX);
    source->ReadInt32(&mY);
    source->ReadInt32(&mType);
    source->ReadInt32(&mMemoryType);
    source->ReadInt32(&mFlags);
    source->ReadInt32(&mSoftInputMode);
    source->ReadInt32(&mGravity);
    source->ReadFloat(&mHorizontalMargin);
    source->ReadFloat(&mVerticalMargin);
    source->ReadInt32(&mFormat);
    source->ReadInt32(&mWindowAnimations);
    source->ReadFloat(&mAlpha);
    source->ReadFloat(&mDimAmount);
    source->ReadFloat(&mScreenBrightness);
    source->ReadFloat(&mButtonBrightness);
    source->ReadInterfacePtr((Handle32*)&mToken);
    source->ReadString(&mCapsuleName);

    String title;
    source->ReadString(&title);
    mTitle->Copy((const char*)title, title.GetLength());

    source->ReadInt32(&mScreenOrientation);

    return NOERROR;
}

ECode CWindowManagerLayoutParams::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (!dest) {
        return E_INVALID_ARGUMENT;
    }

    dest->WriteInt32(mWidth);
    dest->WriteInt32(mHeight);
    dest->WriteInt32(mX);
    dest->WriteInt32(mY);
    dest->WriteInt32(mType);
    dest->WriteInt32(mMemoryType);
    dest->WriteInt32(mFlags);
    dest->WriteInt32(mSoftInputMode);
    dest->WriteInt32(mGravity);
    dest->WriteFloat(mHorizontalMargin);
    dest->WriteFloat(mVerticalMargin);
    dest->WriteInt32(mFormat);
    dest->WriteInt32(mWindowAnimations);
    dest->WriteFloat(mAlpha);
    dest->WriteFloat(mDimAmount);
    dest->WriteFloat(mScreenBrightness);
    dest->WriteFloat(mButtonBrightness);
    dest->WriteInterfacePtr((IInterface*)mToken.Get());
    dest->WriteString(mCapsuleName);
    dest->WriteString((const char*)mTitle);
    dest->WriteInt32(mScreenOrientation);

    return NOERROR;
}

ECode CWindowManagerLayoutParams::constructor()
{
    ViewGroupLayoutParams::Init(
            ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_MATCH_PARENT);

    mType = WindowManagerLayoutParams_TYPE_APPLICATION;
    mFormat = ElPixelFormat::OPAQUE;
    return NOERROR;
}
