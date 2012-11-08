
#include "view/menu/IconMenuViewLayoutParams.h"

IconMenuViewLayoutParams::IconMenuViewLayoutParams()
{}

IconMenuViewLayoutParams::IconMenuViewLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupMarginLayoutParams(c, attrs)
{}

IconMenuViewLayoutParams::IconMenuViewLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupMarginLayoutParams(width, height)
{}

ECode IconMenuViewLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupMarginLayoutParams::Init(c, attrs);
}

ECode IconMenuViewLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupMarginLayoutParams::Init(width, height);
}
