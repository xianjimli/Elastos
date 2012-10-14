
#include "view/animation/AlphaAnimation.h"

AlphaAnimation::AlphaAnimation()
{}

/**
 * Constructor used when an AlphaAnimation is loaded from a resource.
 *
 * @param context Application context to use
 * @param attrs Attribute set from which to read values
 */
AlphaAnimation::AlphaAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

/**
 * Constructor to use when building an AlphaAnimation from code
 *
 * @param fromAlpha Starting alpha value for the animation, where 1.0 means
 *        fully opaque and 0.0 means fully transparent.
 * @param toAlpha Ending alpha value for the animation.
 */
AlphaAnimation::AlphaAnimation(
    /* [in] */ Float fromAlpha,
    /* [in] */ Float toAlpha)
{
    Init(fromAlpha, toAlpha);
}

/**
 * Changes the alpha property of the supplied {@link Transformation}
 */
//@Override
void AlphaAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    t->SetAlpha(mFromAlpha + ((mToAlpha - mFromAlpha) * interpolatedTime));
}

//@Override
Boolean AlphaAnimation::WillChangeTransformationMatrix()
{
    return FALSE;
}

//@Override
Boolean AlphaAnimation::WillChangeBounds()
{
    return FALSE;
}

static Int32 R_Styleable_AlphaAnimation[] = {
    0x010101ca, 0x010101cb
};

ECode AlphaAnimation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_AlphaAnimation,
        sizeof(R_Styleable_AlphaAnimation) / sizeof(Int32))
        /*com.android.internal.R.styleable.AlphaAnimation*/, (ITypedArray**)&a);

    a->GetFloat(
        0/*com.android.internal.R.styleable.AlphaAnimation_fromAlpha*/,
        1.0f, &mFromAlpha);

    a->GetFloat(
        1/*com.android.internal.R.styleable.AlphaAnimation_toAlpha*/,
        1.0f, &mToAlpha);

    a->Recycle();

    return NOERROR;
}

/**
 * Constructor to use when building an AlphaAnimation from code
 *
 * @param fromAlpha Starting alpha value for the animation, where 1.0 means
 *        fully opaque and 0.0 means fully transparent.
 * @param toAlpha Ending alpha value for the animation.
 */
ECode AlphaAnimation::Init(
    /* [in] */ Float fromAlpha,
    /* [in] */ Float toAlpha)
{
    mFromAlpha = fromAlpha;
    mToAlpha = toAlpha;

    return NOERROR;
}
