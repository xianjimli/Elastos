
#include "widget/AbsoluteLayoutLayoutParams.h"
#include <elastos/AutoPtr.h>

static Int32 R_Styleable_AbsoluteLayout_Layout[] = {
    0x0101017f, 0x01010180
};

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams()
{}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
    : ViewGroupLayoutParams(width, height)
    , mX(x)
    , mY(y)
{}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupLayoutParams(c, attrs)
{
    InitFromAttributes(c, attrs);
}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
{
}

ECode AbsoluteLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    ViewGroupLayoutParams::Init(width, height);
    mX = x;
    mY = y;
    return NOERROR;
}

ECode AbsoluteLayoutLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroupLayoutParams::Init(c, attrs);
    return InitFromAttributes(c, attrs);
}

ECode AbsoluteLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}

ECode AbsoluteLayoutLayoutParams::InitFromAttributes(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;

    c->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_AbsoluteLayout_Layout,
                    sizeof(R_Styleable_AbsoluteLayout_Layout) / sizeof(Int32)),
            (ITypedArray**)&a);

    a->GetDimensionPixelOffset(
            0/*com.android.internal.R.styleable.AbsoluteLayout_Layout_layout_x*/, 0, &mX);
    a->GetDimensionPixelOffset(
            1/*com.android.internal.R.styleable.AbsoluteLayout_Layout_layout_y*/, 0, &mY);

    a->Recycle();

    return NOERROR;
}
