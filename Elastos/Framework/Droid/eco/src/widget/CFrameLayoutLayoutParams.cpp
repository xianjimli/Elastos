
#include "widget/CFrameLayoutLayoutParams.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewGroupMarginLayoutParams.h"
#include "ext/frameworkdef.h"

ECode CFrameLayoutLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFrameLayoutLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CFrameLayoutLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CFrameLayoutLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CFrameLayoutLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CFrameLayoutLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return FrameLayoutLayoutParams::SetMargins(left, top, right, bottom);
}

ECode CFrameLayoutLayoutParams::GetMargins(
    /* [out] */ Int32* left,
    /* [out] */ Int32* top,
    /* [out] */ Int32* right,
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(left);
    VALIDATE_NOT_NULL(top);
    VALIDATE_NOT_NULL(right);
    VALIDATE_NOT_NULL(bottom);

    *left = mLeftMargin;
    *top = mTopMargin;
    *right = mRightMargin;
    *bottom = mBottomMargin;

    return NOERROR;
}

ECode CFrameLayoutLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = mGravity;

    return NOERROR;
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return FrameLayoutLayoutParams::Init(c, attrs);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return FrameLayoutLayoutParams::Init(width, height);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 gravity)
{
    return FrameLayoutLayoutParams::Init(width, height, gravity);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return FrameLayoutLayoutParams::Init(source);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return FrameLayoutLayoutParams::Init(source);
}
