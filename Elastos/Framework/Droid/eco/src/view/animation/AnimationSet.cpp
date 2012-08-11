
#include "view/animation/AnimationSet.h"
#include "view/animation/CTransformation.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const Int32 AnimationSet::PROPERTY_FILL_AFTER_MASK;
const Int32 AnimationSet::PROPERTY_FILL_BEFORE_MASK;
const Int32 AnimationSet::PROPERTY_REPEAT_MODE_MASK;
const Int32 AnimationSet::PROPERTY_START_OFFSET_MASK;
const Int32 AnimationSet::PROPERTY_SHARE_INTERPOLATOR_MASK;
const Int32 AnimationSet::PROPERTY_DURATION_MASK;
const Int32 AnimationSet::PROPERTY_MORPH_MATRIX_MASK;
const Int32 AnimationSet::PROPERTY_CHANGE_BOUNDS_MASK;

AnimationSet::AnimationSet()
    : mFlags(0)
    , mStoredOffsets(NULL)
{
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTempTransformation));
}

/**
 * Constructor used when an AnimationSet is loaded from a resource.
 *
 * @param context Application context to use
 * @param attrs Attribute set from which to read values
 */
AnimationSet::AnimationSet(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mFlags(0)
    , mStoredOffsets(NULL)
{
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTempTransformation));
    Init(context, attrs);
}

/**
 * Constructor to use when building an AnimationSet from code
 *
 * @param shareInterpolator Pass TRUE if all of the animations in this set
 *        should use the interpolator assocciated with this AnimationSet.
 *        Pass FALSE if each animation should use its own interpolator.
 */
AnimationSet::AnimationSet(
    /* [in] */ Boolean shareInterpolator)
    : mFlags(0)
    , mStoredOffsets(NULL)
{
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTempTransformation));
    Init(shareInterpolator);
}

//@Override
//protected AnimationSet clone() throws CloneNotSupportedException {
//    final AnimationSet animation = (AnimationSet) super.clone();
//    animation.mTempTransformation = new Transformation();
//    animation.mAnimations = new ArrayList<Animation>();

//    final Int32 count = mAnimations.size();
//    final ArrayList<Animation> animations = mAnimations;

//    for (Int32 i = 0; i < count; i++) {
//        animation.mAnimations.add(animations.get(i).clone());
//    }

//    return animation;
//}

void AnimationSet::SetFlag(
    /* [in] */ Int32 mask,
    /* [in] */ Boolean value)
{
    if (value) {
        mFlags |= mask;
    }
    else {
        mFlags &= ~mask;
    }
}

void AnimationSet::InitInternal()
{
    mStartTime = 0;
    mDuration = 0;
}

//@Override
ECode AnimationSet::SetFillAfter(
    /* [in] */ Boolean fillAfter)
{
    mFlags |= PROPERTY_FILL_AFTER_MASK;
    return Animation::SetFillAfter(fillAfter);
}

//@Override
ECode AnimationSet::SetFillBefore(
    /* [in] */ Boolean fillBefore)
{
    mFlags |= PROPERTY_FILL_BEFORE_MASK;
    return Animation::SetFillBefore(fillBefore);
}

//@Override
ECode AnimationSet::SetRepeatMode(
    /* [in] */ Int32 repeatMode)
{
    mFlags |= PROPERTY_REPEAT_MODE_MASK;
    return Animation::SetRepeatMode(repeatMode);
}

//@Override
ECode AnimationSet::SetStartOffset(
    /* [in] */ Int64 startOffset)
{
    mFlags |= PROPERTY_START_OFFSET_MASK;
    return Animation::SetStartOffset(startOffset);
}

/**
 * <p>Sets the duration of every child animation.</p>
 *
 * @param durationMillis the duration of the animation, in milliseconds, for
 *        every child in this set
 */
//@Override
ECode AnimationSet::SetDuration(
    /* [in] */ Int64 durationMillis)
{
    mFlags |= PROPERTY_DURATION_MASK;
    return Animation::SetDuration(durationMillis);
}

/**
 * Add a child animation to this animation set.
 * The transforms of the child animations are applied in the order
 * that they were added
 * @param a Animation to add.
 */
ECode AnimationSet::AddAnimation(
    /* [in] */ IAnimation* a)
{
    mAnimations.PushBack(a);

    Boolean noMatrix = (mFlags & PROPERTY_MORPH_MATRIX_MASK) == 0;
    Boolean willChange;
    a->WillChangeTransformationMatrix(&willChange);
    if (noMatrix && willChange) {
        mFlags |= PROPERTY_MORPH_MATRIX_MASK;
    }

    Boolean changeBounds = (mFlags & PROPERTY_CHANGE_BOUNDS_MASK) == 0;
    if (changeBounds && willChange) {
        mFlags |= PROPERTY_CHANGE_BOUNDS_MASK;
    }

    Int64 startOffset, duration;
    a->GetStartOffset(&startOffset);
    a->GetDuration(&duration);
    if (mAnimations.GetSize() == 1) {
        mDuration = startOffset + duration;
        mLastEnd = mStartOffset + mDuration;
    }
    else {
        mLastEnd = Math::Max(mLastEnd, startOffset + duration);
        mDuration = mLastEnd - mStartOffset;
    }

    return NOERROR;
}

/**
 * Sets the start time of this animation and all child animations
 *
 * @see android.view.animation.Animation#setStartTime(Int64)
 */
//@Override
ECode AnimationSet::SetStartTime(
    /* [in] */ Int64 startTimeMillis)
{
    Animation::SetStartTime(startTimeMillis);

    List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
    for (; iter != mAnimations.End(); ++iter) {
        (*iter)->SetStartOffset(startTimeMillis);
    }

    return NOERROR;
}

//@Override
Int64 AnimationSet::GetStartTime()
{
    Int64 startTime = Math::INT64_MAX_VALUE;

    List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
    for (; iter != mAnimations.End(); ++iter) {
        Int64 temp;
        (*iter)->GetStartTime(&temp);
        startTime = Math::Min(startTime, temp);
    }

    return startTime;
}

//@Override
ECode AnimationSet::RestrictDuration(
    /* [in] */ Int64 durationMillis)
{
    Animation::RestrictDuration(durationMillis);

    List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
    for (; iter != mAnimations.End(); ++iter) {
        (*iter)->RestrictDuration(durationMillis);
    }

    return NOERROR;
}

/**
 * The duration of an AnimationSet is defined to be the
 * duration of the longest child animation.
 *
 * @see android.view.animation.Animation#getDuration()
 */
//@Override
Int64 AnimationSet::GetDuration()
{
    Int64 duration = 0;

    Boolean durationSet = (mFlags & PROPERTY_DURATION_MASK) == PROPERTY_DURATION_MASK;
    if (durationSet) {
        duration = mDuration;
    }
    else {
        List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
        for (; iter != mAnimations.End(); ++iter) {
            Int64 temp;
            (*iter)->GetDuration(&temp);
            duration = Math::Max(duration, temp);
        }
    }

    return duration;
}

/**
 * The duration hint of an animation set is the maximum of the duration
 * hints of all of its component animations.
 *
 * @see android.view.animation.Animation#computeDurationHint
 */
Int64 AnimationSet::ComputeDurationHint()
{
    Int64 duration = 0;
    List<AutoPtr<IAnimation> >::ReverseIterator iter = mAnimations.RBegin();
    for (; iter != mAnimations.REnd(); ++iter) {
        Int64 d;
        (*iter)->ComputeDurationHint(&d);
        if (d > duration) {
            duration = d;
        }
    }
    return duration;
}

/**
 * @hide
 */
ECode AnimationSet::InitializeInvalidateRegion(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    IRectF* region = mPreviousRegion;
    region->Set(left, top, right, bottom);
    region->Inset(-1.0f, -1.0f);

    if (mFillBefore) {
        List<AutoPtr<IAnimation> >::ReverseIterator iter = mAnimations.RBegin();
        for (; iter != mAnimations.REnd(); ++iter) {
            Animation* a = (Animation*)(*iter)->Probe(EIID_Animation);
            mTempTransformation->Clear();
            IInterpolator* interpolator = a->mInterpolator;
            Float interpolation = 0.0f;
            if (interpolator != NULL) {
                interpolator->GetInterpolation(0.0f, &interpolation);
            }
            a->ApplyTransformation(interpolation, mTempTransformation);
            mPreviousTransformation->Compose(mTempTransformation);
        }
    }

    return NOERROR;
}

/**
 * The transformation of an animation set is the concatenation of all of its
 * component animations.
 *
 * @see android.view.animation.Animation#getTransformation
 */
//@Override
Boolean AnimationSet::GetTransformation(
    /* [in] */ Int64 currentTime,
    /* [in] */ ITransformation* t)
{
    Boolean more = FALSE;
    Boolean started = FALSE;
    Boolean ended = TRUE;

    t->Clear();

    List<AutoPtr<IAnimation> >::ReverseIterator iter = mAnimations.RBegin();
    for (; iter != mAnimations.REnd(); ++iter) {
        Animation* a = (Animation*)(*iter)->Probe(EIID_Animation);

        mTempTransformation->Clear();
        more = a->GetTransformation(currentTime, mTempTransformation) || more;
        t->Compose(mTempTransformation);

        started = started || a->HasStarted();
        ended = a->HasEnded() && ended;
    }

    if (started && !mStarted) {
        if (mListener != NULL) {
            mListener->OnAnimationStart((IAnimation*)this->Probe(EIID_IAnimation));
        }
        mStarted = TRUE;
    }

    if (ended != mEnded) {
        if (mListener != NULL) {
            mListener->OnAnimationEnd((IAnimation*)this->Probe(EIID_IAnimation));
        }
        mEnded = ended;
    }

    return more;
}

/**
 * @see android.view.animation.Animation#scaleCurrentDuration(Float)
 */
//@Override
ECode AnimationSet::ScaleCurrentDuration(
    /* [in] */  Float scale)
{
    List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
    for (; iter != mAnimations.End(); ++iter) {
        (*iter)->ScaleCurrentDuration(scale);
    }

    return NOERROR;
}

/**
 * @see android.view.animation.Animation#initialize(Int32, Int32, Int32, Int32)
 */
//@Override
ECode AnimationSet::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Animation::Initialize(width, height, parentWidth, parentHeight);

    Boolean durationSet = (mFlags & PROPERTY_DURATION_MASK) == PROPERTY_DURATION_MASK;
    Boolean fillAfterSet = (mFlags & PROPERTY_FILL_AFTER_MASK) == PROPERTY_FILL_AFTER_MASK;
    Boolean fillBeforeSet = (mFlags & PROPERTY_FILL_BEFORE_MASK) == PROPERTY_FILL_BEFORE_MASK;
    Boolean repeatModeSet = (mFlags & PROPERTY_REPEAT_MODE_MASK) == PROPERTY_REPEAT_MODE_MASK;
    Boolean shareInterpolator = (mFlags & PROPERTY_SHARE_INTERPOLATOR_MASK)
            == PROPERTY_SHARE_INTERPOLATOR_MASK;
    Boolean startOffsetSet = (mFlags & PROPERTY_START_OFFSET_MASK)
            == PROPERTY_START_OFFSET_MASK;

    if (shareInterpolator) {
        EnsureInterpolator();
    }

    if (startOffsetSet) {
        Int32 count = mAnimations.GetSize();
        if (mStoredOffsets == NULL || mStoredOffsets->GetLength() != count) {
            ArrayOf<Int64>::Free(mStoredOffsets);
            mStoredOffsets = ArrayOf<Int64>::Alloc(count);
        }
    }
    else if (mStoredOffsets != NULL) {
        ArrayOf<Int64>::Free(mStoredOffsets);
        mStoredOffsets = NULL;
    }

    List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
    for (Int32 i = 0; iter != mAnimations.End(); ++iter, ++i) {
        if (durationSet) {
            (*iter)->SetDuration(mDuration);
        }
        if (fillAfterSet) {
            (*iter)->SetFillAfter(mFillAfter);
        }
        if (fillBeforeSet) {
            (*iter)->SetFillBefore(mFillBefore);
        }
        if (repeatModeSet) {
            (*iter)->SetRepeatMode(mRepeatMode);
        }
        if (shareInterpolator) {
            (*iter)->SetInterpolatorEx(mInterpolator);
        }
        if (startOffsetSet) {
            Int64 offset;
            (*iter)->GetStartOffset(&offset);
            (*iter)->SetStartOffset(offset + mStartOffset);
            (*mStoredOffsets)[i] = offset;
        }
        (*iter)->Initialize(width, height, parentWidth, parentHeight);
    }

    return NOERROR;
}

//@Override
ECode AnimationSet::Reset()
{
    Animation::Reset();
    return RestoreChildrenStartOffset();
}

/**
 * @hide
 */
ECode AnimationSet::RestoreChildrenStartOffset()
{
    if (mStoredOffsets == NULL) {
        return NOERROR;
    }

    List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
    for (Int32 i = 0; iter != mAnimations.End(); ++iter, ++i) {
        (*iter)->SetStartOffset((*mStoredOffsets)[i]);
    }

    return NOERROR;
}

/**
 * @return All the child animations in this AnimationSet. Note that
 * this may include other AnimationSets, which are not expanded.
 */
ECode AnimationSet::GetAnimations(
    /* [out] */ IObjectContainer** animations)
{
    VALIDATE_NOT_NULL(animations);

    CObjectContainer::New(animations);
    List<AutoPtr<IAnimation> >::Iterator iter = mAnimations.Begin();
    for (; iter != mAnimations.End(); ++iter) {
        (*animations)->Add((*iter).Get());;
    }

    return NOERROR;
}

//@Override
Boolean AnimationSet::WillChangeTransformationMatrix()
{
    return (mFlags & PROPERTY_MORPH_MATRIX_MASK) == PROPERTY_MORPH_MATRIX_MASK;
}

//@Override
Boolean AnimationSet::WillChangeBounds()
{
    return (mFlags & PROPERTY_CHANGE_BOUNDS_MASK) == PROPERTY_CHANGE_BOUNDS_MASK;
}

static Int32 R_Styleable_AnimationSet[] = {
    0x010101bb
};

ECode AnimationSet::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_AnimationSet,
        sizeof(R_Styleable_AnimationSet) / sizeof(Int32))
        /*com.android.internal.R.styleable.AnimationSet*/, (ITypedArray**)&a);

    Boolean share;
    a->GetBoolean(
        0/*com.android.internal.R.styleable.AnimationSet_shareInterpolator*/,
        TRUE, &share);

    SetFlag(PROPERTY_SHARE_INTERPOLATOR_MASK, share);
    InitInternal();

    a->Recycle();

    return NOERROR;
}

ECode AnimationSet::Init(
    /* [in] */ Boolean shareInterpolator)
{
    SetFlag(PROPERTY_SHARE_INTERPOLATOR_MASK, shareInterpolator);
    InitInternal();

    return NOERROR;
}

