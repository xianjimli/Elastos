
#include "view/animation/ScaleAnimation.h"

static Int32 R_Styleable_ScaleAnimation[] = {
    0x010101b5, 0x010101b6, 0x010101c2, 0x010101c3,
    0x010101c4, 0x010101c5
};

ScaleAnimation::ScaleAnimation()
    : mPivotXType(Animation_ABSOLUTE)
    , mPivotYType(Animation_ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
{}

ScaleAnimation::ScaleAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

ScaleAnimation::ScaleAnimation(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY)
    : mPivotXType(Animation_ABSOLUTE)
    , mPivotYType(Animation_ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
{
    Init(fromX, toX, fromY, toY);
}

ScaleAnimation::ScaleAnimation(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    Init(fromX, toX, fromY, toY, pivotX, pivotY);
}

ScaleAnimation::ScaleAnimation(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Float pivotYType,
    /* [in] */ Float pivotYValue)
{
    Init(fromX, toX, fromY, toY, pivotXType,
        pivotXValue, pivotYType, pivotYValue);
}

void ScaleAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    Float sx = 1.0f;
    Float sy = 1.0f;

    if (mFromX != 1.0f || mToX != 1.0f) {
        sx = mFromX + ((mToX - mFromX) * interpolatedTime);
    }
    if (mFromY != 1.0f || mToY != 1.0f) {
        sy = mFromY + ((mToY - mFromY) * interpolatedTime);
    }

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    if (mPivotX == 0.0f && mPivotY == 0.0f) {
        matrix->SetScaleEx(sx, sy);
    }
    else {
        matrix->SetScale(sx, sy, mPivotX, mPivotY);
    }
}

ECode ScaleAnimation::Initialize(
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

ECode ScaleAnimation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_ScaleAnimation,
        sizeof(R_Styleable_ScaleAnimation) / sizeof(Int32))
        /*com.android.internal.R.styleable.ScaleAnimation*/, (ITypedArray**)&a);

    a->GetFloat(
        2/*com.android.internal.R.styleable.ScaleAnimation_fromXScale*/,
        0.0f, &mFromX);

    a->GetFloat(
        3/*com.android.internal.R.styleable.ScaleAnimation_toXScale*/,
        0.0f, &mToX);

    a->GetFloat(
        4/*com.android.internal.R.styleable.ScaleAnimation_fromYScale*/,
        0.0f, &mFromY);

    a->GetFloat(
        5/*com.android.internal.R.styleable.ScaleAnimation_toYScale*/,
        0.0f, &mToY);

    AutoPtr<ITypedValue> value;
    a->PeekValue(
        0/*com.android.internal.R.styleable.ScaleAnimation_pivotX*/,
        (ITypedValue**)&value);
    AutoPtr<Description> d = Description::ParseValue(value);
    mPivotXType = d->mType;
    mPivotXValue = d->mValue;

    a->PeekValue(
        1/*com.android.internal.R.styleable.ScaleAnimation_pivotY*/,
        (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mPivotYType = d->mType;
    mPivotYValue = d->mValue;

    a->Recycle();

    return NOERROR;
}

ECode ScaleAnimation::Init(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY)
{
    mFromX = fromX;
    mToX = toX;
    mFromY = fromY;
    mToY = toY;
    mPivotX = 0.0f;
    mPivotY = 0.0f;

    return NOERROR;
}

ECode ScaleAnimation::Init(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    mFromX = fromX;
    mToX = toX;
    mFromY = fromY;
    mToY = toY;

    mPivotXType = Animation_ABSOLUTE;
    mPivotYType = Animation_ABSOLUTE;
    mPivotXValue = pivotX;
    mPivotYValue = pivotY;

    return NOERROR;
}

ECode ScaleAnimation::Init(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Float pivotYType,
    /* [in] */ Float pivotYValue)
{
    mFromX = fromX;
    mToX = toX;
    mFromY = fromY;
    mToY = toY;

    mPivotXValue = pivotXValue;
    mPivotXType = pivotXType;
    mPivotYValue = pivotYValue;
    mPivotYType = pivotYType;

    return NOERROR;
}
