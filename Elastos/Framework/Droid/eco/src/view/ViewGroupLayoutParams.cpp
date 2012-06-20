
#include "view/ViewGroupLayoutParams.h"
#include <elastos/AutoPtr.h>

const Int32 ViewGroupLayoutParams::FILL_PARENT;
const Int32 ViewGroupLayoutParams::MATCH_PARENT;
const Int32 ViewGroupLayoutParams::WRAP_CONTENT;


ViewGroupLayoutParams::ViewGroupLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

/**
 * Creates a new set of layout parameters with the specified width
 * and height.
 *
 * @param width the width, either {@link #WRAP_CONTENT},
 *        {@link #FILL_PARENT} (replaced by {@link #MATCH_PARENT} in
 *        API Level 8), or a fixed size in pixels
 * @param height the height, either {@link #WRAP_CONTENT},
 *        {@link #FILL_PARENT} (replaced by {@link #MATCH_PARENT} in
 *        API Level 8), or a fixed size in pixels
 */
ViewGroupLayoutParams::ViewGroupLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : mWidth(width)
    , mHeight(height)
{}

/**
 * Copy constructor. Clones the width and height values of the source.
 *
 * @param source The layout params to copy from.
 */
ViewGroupLayoutParams::ViewGroupLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : mWidth(source->mWidth)
    , mHeight(source->mHeight)
{}

/**
 * Used internally by MarginLayoutParams.
 * @hide
 */
ViewGroupLayoutParams::ViewGroupLayoutParams()
{}

void ViewGroupLayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 widthAttr,
    /* [in] */ Int32 heightAttr)
{
    a->GetLayoutDimension(widthAttr, "layout_width", &mWidth);
    a->GetLayoutDimension(heightAttr, "layout_height", &mHeight);
}

ECode ViewGroupLayoutParams::SizeToString(
    /* [in] */ Int32 size,
    /* [out] */ String* des)
{
    VALIDATE_NOT_NULL(des);

    if (size == WRAP_CONTENT) {
        *des = String::Duplicate("wrap-content");
        return NOERROR;
    }
    if (size == MATCH_PARENT) {
        *des = String::Duplicate("match-parent");
        return NOERROR;
    }
//    return String.valueOf(size);
    return E_NOT_IMPLEMENTED;
}

static Int32 R_Styleable_ViewGroup_Layout[] = {
    0x010100f4, 0x010100f5
};

ECode ViewGroupLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_ViewGroup_Layout, 2)/*R.styleable.ViewGroup_Layout*/,
            (ITypedArray**)&a));
    SetBaseAttributes(a,
            0/*R.styleable.ViewGroup_Layout_layout_width*/,
            1/*R.styleable.ViewGroup_Layout_layout_height*/);
    a->Recycle();
    return NOERROR;
}

ECode ViewGroupLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;
    return NOERROR;
}

ECode ViewGroupLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    source->GetWidth(&mWidth);
    source->GetHeight(&mHeight);
    return NOERROR;
}
