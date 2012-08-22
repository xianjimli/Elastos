
#include "graphics/drawable/AnimationDrawable.h"
#include "graphics/drawable/CAnimationDrawable.h"
#include "os/SystemClock.h"

AnimationDrawable::AnimationState::AnimationState(
    /* [in] */ AnimationState* orig,
    /* [in] */ IAnimationDrawable* owner,
    /* [in] */ IResources* res)
    : DrawableContainerState(orig, owner, res)
{
    if (orig != NULL) {
        mDurations = orig->mDurations->Clone();
        mOneShot = orig->mOneShot;
    }
    else {
        mDurations = ArrayOf<Int32>::Alloc(GetChildren()->GetLength());
        mOneShot = TRUE;
    }
}

AnimationDrawable::AnimationState::~AnimationState()
{
    ArrayOf<Int32>::Free(mDurations);
}

//@Override
ECode AnimationDrawable::AnimationState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CAnimationDrawable::New((Handle32)this, NULL, (IAnimationDrawable**)drawable);
}

//@Override
ECode AnimationDrawable::AnimationState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CAnimationDrawable::New((Handle32)this, res, (IAnimationDrawable**)drawable);
}

void AnimationDrawable::AnimationState::AddFrame(
    /* [in] */ IDrawable* dr,
    /* [in] */ Int32 dur)
{
    // Do not combine the following. The array index must be evaluated before
    // the array is accessed because super.addChild(dr) has a side effect on mDurations.
    Int32 pos = DrawableContainerState::AddChild(dr);
    (*mDurations)[pos] = dur;
}

//@Override
void AnimationDrawable::AnimationState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    DrawableContainerState::GrowArray(oldSize, newSize);
    ArrayOf<Int32>* newDurations = ArrayOf<Int32>::Alloc(newSize);
    memcpy(newDurations->GetPayload(), mDurations->GetPayload(), oldSize * sizeof(Int32));
    ArrayOf<Int32>::Free(mDurations);
    mDurations = newDurations;
}


AnimationDrawable::AnimationDrawable()
    : mCurFrame(-1)
    , mMutated(FALSE)
{
//    this(null, null);
}

//@Override
Boolean AnimationDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = DrawableContainer::SetVisible(visible, restart);
    if (visible) {
        if (changed || restart) {
            SetFrame(0, TRUE, TRUE);
        }
    }
    else {
        UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    }
    return changed;
}

/**
 * <p>Starts the animation, looping if necessary. This method has no effect
 * if the animation is running.</p>
 *
 * @see #isRunning()
 * @see #stop()
 */
ECode AnimationDrawable::Start()
{
    if (!IsRunning()) {
        return Run();
    }
    return NOERROR;
}

/**
 * <p>Stops the animation. This method has no effect if the animation is
 * not running.</p>
 *
 * @see #isRunning()
 * @see #start()
 */
ECode AnimationDrawable::Stop()
{
    if (IsRunning()) {
        return UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    }
    return NOERROR;
}

/**
 * <p>Indicates whether the animation is currently running or not.</p>
 *
 * @return true if the animation is running, false otherwise
 */
Boolean AnimationDrawable::IsRunning()
{
    return mCurFrame > -1;
}

/**
 * <p>This method exists for implementation purpose only and should not be
 * called directly. Invoke {@link #start()} instead.</p>
 *
 * @see #start()
 */
ECode AnimationDrawable::Run()
{
    return NextFrame(FALSE);
}

//@Override
ECode AnimationDrawable::UnscheduleSelf(
    /* [in] */ IRunnable* what)
{
    mCurFrame = -1;
    return DrawableContainer::UnscheduleSelf(what);
}

/**
 * @return The number of frames in the animation
 */
Int32 AnimationDrawable::GetNumberOfFrames()
{
    return mAnimationState->GetChildCount();
}

/**
 * @return The Drawable at the specified frame index
 */
AutoPtr<IDrawable> AnimationDrawable::GetFrame(
    /* [in] */ Int32 index)
{
    return (*mAnimationState->GetChildren())[index];
}

/**
 * @return The duration in milliseconds of the frame at the
 * specified index
 */
Int32 AnimationDrawable::GetDuration(
    /* [in] */ Int32 i)
{
    return (*mAnimationState->mDurations)[i];
}

/**
 * @return True of the animation will play once, false otherwise
 */
Boolean AnimationDrawable::IsOneShot()
{
    return mAnimationState->mOneShot;
}

/**
 * Sets whether the animation should play once or repeat.
 *
 * @param oneShot Pass true if the animation should only play once
 */
ECode AnimationDrawable::SetOneShot(
    /* [in] */ Boolean oneShot)
{
    mAnimationState->mOneShot = oneShot;
    return NOERROR;
}

/**
 * Add a frame to the animation
 *
 * @param frame The frame to add
 * @param duration How long in milliseconds the frame should appear
 */
ECode AnimationDrawable::AddFrame(
    /* [in] */ IDrawable* frame,
    /* [in] */ Int32 duration)
{
    mAnimationState->AddFrame(frame, duration);
    return NOERROR;
}

ECode AnimationDrawable::NextFrame(
    /* [in] */ Boolean unschedule)
{
    Int32 next = mCurFrame + 1;
    const Int32 N = mAnimationState->GetChildCount();
    if (next >= N) {
        next = 0;
    }
    return SetFrame(next, unschedule, !mAnimationState->mOneShot || next < (N - 1));
}

ECode AnimationDrawable::SetFrame(
    /* [in] */ Int32 frame,
    /* [in] */ Boolean unschedule,
    /* [in] */ Boolean animate)
{
    if (frame >= mAnimationState->GetChildCount()) {
        return NOERROR;
    }
    mCurFrame = frame;
    SelectDrawable(frame);
    if (unschedule) {
        FAIL_RETURN(UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable)));
    }
    if (animate) {
        FAIL_RETURN(ScheduleSelf((IRunnable*)this->Probe(EIID_IRunnable),
                SystemClock::GetUptimeMillis() + (*mAnimationState->mDurations)[frame]));
    }
    return NOERROR;
}


static Int32 R_Styleable_AnimationDrawable[] = {
    0x01010194, 0x01010195, 0x01010197
};

static Int32 R_Styleable_AnimationDrawableItem[] = {
    0x01010198, 0x01010199
};

//@Override
ECode AnimationDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_AnimationDrawable,
                    sizeof(R_Styleable_AnimationDrawable) / sizeof(Int32)),
            (ITypedArray**)&a);

    FAIL_RETURN(DrawableContainer::InflateWithAttributes(r, parser, a,
            0/*com.android.internal.R.styleable.AnimationDrawable_visible*/));

    Boolean padding;
    a->GetBoolean(
            1/*com.android.internal.R.styleable.AnimationDrawable_variablePadding*/,
            FALSE, &padding);
    mAnimationState->SetVariablePadding(padding);

    a->GetBoolean(
            2/*com.android.internal.R.styleable.AnimationDrawable_oneshot*/,
            FALSE, &mAnimationState->mOneShot);

    a->Recycle();

    Int32 type;

    Int32 innerDepth, depth;
    parser->GetDepth(&depth);
    innerDepth = depth + 1;
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT) &&
            ((parser->GetDepth(&depth), depth >= innerDepth) || type != IXmlPullParser_END_TAG)) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        String name;
        if (depth > innerDepth || (parser->GetName(&name), !name.Equals("item"))) {
            continue;
        }

        a = NULL;
        r->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AnimationDrawableItem,
                        sizeof(R_Styleable_AnimationDrawableItem) / sizeof(Int32)),
                (ITypedArray**)&a);
        Int32 duration;
        a->GetInt32(
                0/*com.android.internal.R.styleable.AnimationDrawableItem_duration*/,
                -1, &duration);
        if (duration < 0) {
//            throw new XmlPullParserException(
//                    parser.getPositionDescription()
//                    + ": <item> tag requires a 'duration' attribute");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        Int32 drawableRes;
        a->GetResourceId(
                1/*com.android.internal.R.styleable.AnimationDrawableItem_drawable*/,
                0, &drawableRes);

        a->Recycle();

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
        }
        else {
            while (parser->Next(&type), type == IXmlPullParser_TEXT) {
                // Empty
            }
            if (type != IXmlPullParser_START_TAG) {
//                throw new XmlPullParserException(parser.getPositionDescription() +
//                        ": <item> tag requires a 'drawable' attribute or child tag" +
//                        " defining a drawable");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
        }

        mAnimationState->AddFrame(dr, duration);
        if (dr != NULL) {
            dr->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        }
    }

    return SetFrame(0, TRUE, FALSE);
}

//@Override
AutoPtr<IDrawable> AnimationDrawable::Mutate()
{
    if (!mMutated &&
            DrawableContainer::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mAnimationState->mDurations = mAnimationState->mDurations->Clone();
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

AnimationDrawable::AnimationDrawable(
    /* [in] */ AnimationState* state,
    /* [in] */ IResources* res)
{
    ASSERT_SUCCEEDED(Init(state, res));
}

ECode AnimationDrawable::Init(
    /* [in] */ AnimationState* state,
    /* [in] */ IResources* res)
{
    mAnimationState = new AnimationState(state,
            (IAnimationDrawable*)this->Probe(EIID_IAnimationDrawable), res);
    SetConstantState(mAnimationState);
    if (state != NULL) {
        SetFrame(0, TRUE, FALSE);
    }
    return NOERROR;
}
