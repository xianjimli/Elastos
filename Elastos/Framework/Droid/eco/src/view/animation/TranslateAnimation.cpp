
#include "view/animation/TranslateAnimation.h"


TranslateAnimation::TranslateAnimation()
    : mFromXType(Animation_ABSOLUTE)
    , mToXType(Animation_ABSOLUTE)
    , mFromYType(Animation_ABSOLUTE)
    , mToYType(Animation_ABSOLUTE)
    , mFromXValue(0.0f)
    , mToXValue(0.0f)
    , mFromYValue(0.0f)
    , mToYValue(0.0f)
{}

TranslateAnimation::TranslateAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

TranslateAnimation::TranslateAnimation(
    /* [in] */ Float fromXDelta,
    /* [in] */ Float toXDelta,
    /* [in] */ Float fromYDelta,
    /* [in] */ Float toYDelta)
{
    Init(fromXDelta, toXDelta, fromYDelta, toYDelta);
}

TranslateAnimation::TranslateAnimation(
    /* [in] */ Float fromXType,
    /* [in] */ Float fromXValue,
    /* [in] */ Float toXType,
    /* [in] */ Float toXValue,
    /* [in] */ Float fromYType,
    /* [in] */ Float fromYValue,
    /* [in] */ Float toYType,
    /* [in] */ Float toYValue)
{
    Init(fromXType, fromXValue, toXType, toXValue,
        fromYType, fromYValue, toYType, toYValue);
}

void TranslateAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    float dx = mFromXDelta;
    float dy = mFromYDelta;
    if (mFromXDelta != mToXDelta) {
        dx = mFromXDelta + ((mToXDelta - mFromXDelta) * interpolatedTime);
    }
    if (mFromYDelta != mToYDelta) {
        dy = mFromYDelta + ((mToYDelta - mFromYDelta) * interpolatedTime);
    }

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    matrix->SetTranslate(dx, dy);
}

ECode TranslateAnimation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Animation::Initialize(width, height, parentWidth, parentHeight);
    mFromXDelta = ResolveSize(mFromXType, mFromXValue, width, parentWidth);
    mToXDelta = ResolveSize(mToXType, mToXValue, width, parentWidth);
    mFromYDelta = ResolveSize(mFromYType, mFromYValue, height, parentHeight);
    mToYDelta = ResolveSize(mToYType, mToYValue, height, parentHeight);

    return NOERROR;
}

static Int32 R_Styleable_TranslateAnimation[] = {
    0x010101c6, 0x010101c7, 0x010101c8, 0x010101c9
};

ECode TranslateAnimation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_TranslateAnimation,
        sizeof(R_Styleable_TranslateAnimation) / sizeof(Int32))
        /*com.android.internal.R.styleable.TranslateAnimation*/, (ITypedArray**)&a);

    AutoPtr<ITypedValue> value;
    a->PeekValue(
        0/*com.android.internal.R.styleable.TranslateAnimation_fromXDelta*/,
        (ITypedValue**)&value);
    AutoPtr<Description> d = Description::ParseValue(value);
    mFromXType = d->mType;
    mFromXValue = d->mValue;

    a->PeekValue(
        1/*com.android.internal.R.styleable.TranslateAnimation_toXDelta*/,
        (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mToXType = d->mType;
    mToXValue = d->mValue;

    a->PeekValue(
        2/*com.android.internal.R.styleable.TranslateAnimation_fromYDelta*/,
        (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mFromYType = d->mType;
    mFromYValue = d->mValue;

    a->PeekValue(
        3/*com.android.internal.R.styleable.TranslateAnimation_toYDelta*/,
        (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mToYType = d->mType;
    mToYValue = d->mValue;

    a->Recycle();

    return NOERROR;
}

ECode TranslateAnimation::Init(
    /* [in] */ Float fromXDelta,
    /* [in] */ Float toXDelta,
    /* [in] */ Float fromYDelta,
    /* [in] */ Float toYDelta)
{
    mFromXValue = fromXDelta;
    mToXValue = toXDelta;
    mFromYValue = fromYDelta;
    mToYValue = toYDelta;

    mFromXType = Animation_ABSOLUTE;
    mToXType = Animation_ABSOLUTE;
    mFromYType = Animation_ABSOLUTE;
    mToYType = Animation_ABSOLUTE;

    return NOERROR;
}

ECode TranslateAnimation::Init(
   /* [in] */ Float fromXType,
   /* [in] */ Float fromXValue,
   /* [in] */ Float toXType,
   /* [in] */ Float toXValue,
   /* [in] */ Float fromYType,
   /* [in] */ Float fromYValue,
   /* [in] */ Float toYType,
   /* [in] */ Float toYValue)
{
    mFromXValue = fromXValue;
    mToXValue = toXValue;
    mFromYValue = fromYValue;
    mToYValue = toYValue;

    mFromXType = fromXType;
    mToXType = toXType;
    mFromYType = fromYType;
    mToYType = toYType;

    return NOERROR;
}
