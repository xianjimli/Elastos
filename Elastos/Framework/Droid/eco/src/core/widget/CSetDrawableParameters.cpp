
#include "widget/CSetDrawableParameters.h"

CSetDrawableParameters::CSetDrawableParameters()
    : mViewId(0)
    , mTargetBackground(FALSE)
    , mAlpha(0)
    , mColorFilter(0)
    , mLevel(0)
{
}

ECode CSetDrawableParameters::Apply(
    /* [in] */ IView* root)
{
    AutoPtr<IView> target;
    FAIL_RETURN(root->FindViewById(mViewId, (IView**)&target));
    if (target == NULL) {
        return NOERROR;
    }

    // Pick the correct drawable to modify for this view
    AutoPtr<IDrawable> targetDrawable;
    if (mTargetBackground) {
        FAIL_RETURN(target->GetBackground((IDrawable**)&targetDrawable));
    } else if (target->Probe(EIID_IImageView) != NULL) {
        AutoPtr<IImageView> imageView = (IImageView*)target->Probe(EIID_IImageView);
        imageView->GetDrawable((IDrawable**)&targetDrawable);
    }

    if (targetDrawable != NULL) {
        // Perform modifications only if values are set correctly
        if (mAlpha != -1) {
            FAIL_RETURN(targetDrawable->SetAlpha(mAlpha));
        }
        if (mColorFilter != -1 && mFilterMode != NULL) {
            FAIL_RETURN(targetDrawable->SetColorFilterEx(
                mColorFilter, mFilterMode));
        }
        if (mLevel != -1) {
            Boolean changed;
            FAIL_RETURN(targetDrawable->SetLevel(mLevel, &changed));
        }
    }

    return NOERROR;
}

ECode CSetDrawableParameters::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSetDrawableParameters::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(SetDrawableParameters_TAG));
    FAIL_RETURN(dest->WriteInt32(mViewId));
    FAIL_RETURN(dest->WriteInt32(mTargetBackground ? 1 : 0));
    FAIL_RETURN(dest->WriteInt32(mAlpha));
    FAIL_RETURN(dest->WriteInt32(mColorFilter));

    // if (mFilterMode != NULL) {
        FAIL_RETURN(dest->WriteInt32(1));
        // dest.writeString(filterMode.toString());
        FAIL_RETURN(dest->WriteInt32((Int32)mFilterMode));
    // } else {
    //     dest.writeInt(0);
    // }

    FAIL_RETURN(dest->WriteInt32(mLevel));

    return NOERROR;
}

ECode CSetDrawableParameters::constructor(
    /* [in] */ Int32 id,
    /* [in] */ Boolean targetBackground,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 colorFilter,
    /* [in] */ PorterDuffMode mode,
    /* [in] */ Int32 level)
{
    mViewId = id;
    mTargetBackground = targetBackground;
    mAlpha = alpha;
    mColorFilter = colorFilter;
    mFilterMode = mode;
    mLevel = level;

    return NOERROR;
}

ECode CSetDrawableParameters::constructor(
    /* [in] */ IParcel* parcel)
{
    FAIL_RETURN(parcel->ReadInt32(&mViewId));

    Int32 temp;
    FAIL_RETURN(parcel->ReadInt32(&temp));
    mTargetBackground = temp != 0;

    FAIL_RETURN(parcel->ReadInt32(&mAlpha));
    FAIL_RETURN(parcel->ReadInt32(&mColorFilter));

    FAIL_RETURN(parcel->ReadInt32(&temp));
    Boolean hasMode = temp != 0;
    if (hasMode) {
        // mFilterMode = PorterDuffMode.valueOf(parcel.readString());
        FAIL_RETURN(parcel->ReadInt32(&temp));
        mFilterMode = (PorterDuffMode)temp;
    } else {
        // TODO: ALEX ?
        // mFilterMode = null;
    }

    FAIL_RETURN(parcel->ReadInt32(&mLevel));

    return NOERROR;
}

