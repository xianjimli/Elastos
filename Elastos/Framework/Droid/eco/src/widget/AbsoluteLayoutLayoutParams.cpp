
#include "widget/AbsoluteLayoutLayoutParams.h"
#include <elastos/AutoPtr.h>

static Int32 R_Styleable_AbsoluteLayout_Layout[] = {
    0x0101017f, 0x01010180
};

static const Int32 R_Styleable_AbsoluteLayout_Layout_layout_x = 0;

static const Int32 R_Styleable_AbsoluteLayout_Layout_layout_y = 1;

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams()
{

}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ Int32 width, 
    /* [in] */ Int32 height, 
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y) : ViewGroupLayoutParams(width, height)
{
    mX = x;
    mY = y;
}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ IContext* c, 
    /* [in] */ IAttributeSet* attrs) : ViewGroupLayoutParams(c, attrs) 
{
    AutoPtr<ITypedArray> a;

    c->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_AbsoluteLayout_Layout, 2), (ITypedArray**)&a);

    a->GetDimensionPixelOffset(
            R_Styleable_AbsoluteLayout_Layout_layout_x, 0, &mX);

    a->GetDimensionPixelOffset(
            R_Styleable_AbsoluteLayout_Layout_layout_y, 0, &mY);
    
    a->Recycle();
}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source) : ViewGroupLayoutParams(source)
{
}

String AbsoluteLayoutLayoutParams::Debug(
    /* [in] */ String output)
{
    return output /*+ "Absolute.LayoutParams={width="
            + sizeToString(width) + ", height=" + sizeToString(height)
            + " x=" + x + " y=" + y + "}"*/;
}

