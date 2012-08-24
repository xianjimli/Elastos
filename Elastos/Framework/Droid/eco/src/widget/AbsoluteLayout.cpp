
#include "widget/AbsoluteLayout.h"
#include <elastos/Math.h>
#include "widget/CAbsoluteLayoutLayoutParams.h"

AbsoluteLayout::AbsoluteLayout()
{

}

AbsoluteLayout::AbsoluteLayout(
    /* [in] */ IContext* context) : ViewGroup(context)
{
}

AbsoluteLayout::AbsoluteLayout(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : ViewGroup(context, attrs)
{
}

AbsoluteLayout::AbsoluteLayout(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) : ViewGroup(context, attrs, defStyle)
{
}

void AbsoluteLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec, 
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 count = GetChildCount();

    Int32 maxHeight = 0;
    Int32 maxWidth = 0;

    // Find out how big everyone wants to be
    MeasureChildren(widthMeasureSpec, heightMeasureSpec);

    // Find rightmost and bottom-most child
    for (Int32 i = 0; i < count; i++)
    {
        AutoPtr<IView> child = GetChildAt(i);
        
        Int32 visible;
        child->GetVisibility(&visible);
        if (visible != GONE) {
            Int32 childRight;
            Int32 childBottom;

            AutoPtr<IViewGroupLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            Int32 w, h;
            child->GetMeasuredWidth(&w);
            child->GetMeasuredHeight(&h);

            childRight = ((CAbsoluteLayoutLayoutParams*)lp.Get())->mX + w;
            childBottom = ((CAbsoluteLayoutLayoutParams*)lp.Get())->mY + h;

            maxWidth = Math::Max(maxWidth, childRight);
            maxHeight = Math::Max(maxHeight, childBottom);
        }
    }

    // Account for padding too
    maxWidth += mPaddingLeft + mPaddingRight;
    maxHeight += mPaddingTop + mPaddingBottom;

    // Check against minimum height and width
    maxHeight = Math::Max(maxHeight, GetSuggestedMinimumHeight());
    maxWidth = Math::Max(maxWidth, GetSuggestedMinimumWidth());
    
    SetMeasuredDimension(ResolveSize(maxWidth, widthMeasureSpec),
            ResolveSize(maxHeight, heightMeasureSpec));
}

/**
 * Returns a set of layout parameters with a width of
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT},
 * a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}
 * and with the coordinates (0, 0).
 */
ECode AbsoluteLayout::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** lp) 
{
    return CAbsoluteLayoutLayoutParams::New(
        ViewGroupLayoutParams::WRAP_CONTENT, ViewGroupLayoutParams::WRAP_CONTENT, 0, 0, (IAbsoluteLayoutLayoutParams**)lp);
}

void AbsoluteLayout::OnLayout(
    /* [in] */ Boolean changed, 
    /* [in] */ Int32 l, 
    /* [in] */ Int32 t,
    /* [in] */ Int32 r, 
    /* [in] */ Int32 b) 
{
    Int32 count = GetChildCount();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);

        Int32 v;
        child->GetVisibility(&v);
        if (v != GONE) {

            AutoPtr<IAbsoluteLayoutLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            Int32 childLeft = mPaddingLeft + ((CAbsoluteLayoutLayoutParams*)lp.Get())->mX;
            Int32 childTop = mPaddingTop + ((CAbsoluteLayoutLayoutParams*)lp.Get())->mY;

            Int32 w, h;
            child->GetMeasuredWidth(&w);
            child->GetMeasuredHeight(&h);
            child->Layout(childLeft, childTop,
                    childLeft + w,
                    childTop + h);

        }
    }
}

AutoPtr<IViewGroupLayoutParams> AbsoluteLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs) 
{
    AutoPtr<IViewGroupLayoutParams> lp;
    
    CAbsoluteLayoutLayoutParams::New(GetContext(), attrs, (IAbsoluteLayoutLayoutParams**)&lp);

    return lp;
}

// Override to allow type-checking of LayoutParams. 
Boolean AbsoluteLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p) 
{
    return p->Probe(EIID_IAbsoluteLayoutLayoutParams) != NULL;
}

AutoPtr<IViewGroupLayoutParams> AbsoluteLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p) 
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CAbsoluteLayoutLayoutParams::New(p, (IAbsoluteLayoutLayoutParams**)&lp);
    
    return lp;
}

