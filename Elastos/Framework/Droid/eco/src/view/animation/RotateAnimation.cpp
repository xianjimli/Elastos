
#include "view/animation/RotateAnimation.h"

static Int32 R_Styleable_RotateAnimation[] = {
    0x010101b3, 0x010101b4, 0x010101b5, 0x010101b6
};

RotateAnimation::RotateAnimation()
    : mPivotXType(Animation_ABSOLUTE)
    , mPivotYType(Animation_ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
{}

RotateAnimation::RotateAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

RotateAnimation::RotateAnimation(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees)
    : mPivotXType(Animation_ABSOLUTE)
    , mPivotYType(Animation_ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
{
    Init(fromDegrees, toDegrees);
}

RotateAnimation::RotateAnimation(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    Init(fromDegrees, toDegrees, pivotX, pivotY);
}

RotateAnimation::RotateAnimation(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Float pivotYType,
    /* [in] */ Float pivotYValue)
{
    Init(fromDegrees, toDegrees, pivotXType, pivotXValue, pivotYType, pivotYValue);
}

void RotateAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    Float degrees = mFromDegrees + ((mToDegrees - mFromDegrees) * interpolatedTime);

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    if (mPivotX == 0.0f && mPivotY == 0.0f) {
        matrix->SetRotateEx(degrees);
    }
    else {
        matrix->SetRotate(degrees, mPivotX, mPivotY);
    }
}

ECode RotateAnimation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Animation::Initialize(width, height, parentWidth, parentHeight);
    mPivotX = ResolveSize(mPivotXType, mPivotXValue, width, parentWidth);
    mPivotY = ResolveSize(mPivotYType, mPivotYValue, height, parentHeight);

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_RotateAnimation,
        sizeof(R_Styleable_RotateAnimation) / sizeof(Int32))
        /*com.android.internal.R.styleable.RotateAnimation*/, (ITypedArray**)&a);

    a->GetFloat(
        0/*com.android.internal.R.styleable.RotateAnimation_fromDegrees*/,
        0.0f, &mFromDegrees);

    a->GetFloat(
        1/*com.android.internal.R.styleable.RotateAnimation_toDegrees*/,
        0.0f, &mToDegrees);

    AutoPtr<ITypedValue> value;
    a->PeekValue(
        2/*com.android.internal.R.styleable.RotateAnimation_pivotX*/,
        (ITypedValue**)&value);
    AutoPtr<Description> d = Description::ParseValue(value);
    mPivotXType = d->mType;
    mPivotXValue = d->mValue;

    a->PeekValue(
        3/*com.android.internal.R.styleable.RotateAnimation_pivotY*/,
        (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mPivotYType = d->mType;
    mPivotYValue = d->mValue;

    a->Recycle();

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees)
{
    mFromDegrees = fromDegrees;
    mToDegrees = toDegrees;
    mPivotX = 0.0f;
    mPivotY = 0.0f;

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    mFromDegrees = fromDegrees;
    mToDegrees = toDegrees;

    mPivotXType = Animation_ABSOLUTE;
    mPivotYType = Animation_ABSOLUTE;
    mPivotXValue = pivotX;
    mPivotYValue = pivotY;

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Float pivotYType,
    /* [in] */ Float pivotYValue)
{
    mFromDegrees = fromDegrees;
    mToDegrees = toDegrees;

    mPivotXValue = pivotXValue;
    mPivotXType = pivotXType;
    mPivotYValue = pivotYValue;
    mPivotYType = pivotYType;

    return NOERROR;
}
