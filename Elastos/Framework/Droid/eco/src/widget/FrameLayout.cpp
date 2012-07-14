
#include "widget/FrameLayout.h"
#include "view/CGravity.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

#define UNUSED(x) ((void)x)

FrameLayout::FrameLayout()
    : mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(Gravity_FILL)
    , mForegroundBoundsChanged(FALSE)
{
    CRect::NewByFriend((CRect**)&mSelfBounds);
    CRect::NewByFriend((CRect**)&mOverlayBounds);
}

FrameLayout::FrameLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
    , mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(Gravity_FILL)
    , mForegroundBoundsChanged(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSelfBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverlayBounds));
}

FrameLayout::FrameLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs, 0)
    , mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(Gravity_FILL)
    , mForegroundBoundsChanged(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSelfBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverlayBounds));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, 0));
}

FrameLayout::FrameLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
    , mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(Gravity_FILL)
    , mForegroundBoundsChanged(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSelfBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverlayBounds));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

/**
  * Describes how the foreground is positioned. Defaults to FILL.
  *
  * @param foregroundGravity See {@link android.view.Gravity}
  *
  * @attr ref android.R.styleable#FrameLayout_foregroundGravity
  */
ECode FrameLayout::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    if (mForegroundGravity != foregroundGravity) {
        if ((foregroundGravity & Gravity_HORIZONTAL_GRAVITY_MASK) == 0) {
            foregroundGravity |= Gravity_LEFT;
        }

        if ((foregroundGravity & Gravity_VERTICAL_GRAVITY_MASK) == 0) {
            foregroundGravity |= Gravity_TOP;
        }

        mForegroundGravity = foregroundGravity;


        if (mForegroundGravity == Gravity_FILL && mForeground != NULL) {
            IRect* padding;
            CRect::New(&padding);
            Boolean isPadding;

            if (mForeground->GetPadding(padding, &isPadding)) {
                mForegroundPaddingLeft = ((CRect*)padding)->mLeft;
                mForegroundPaddingTop = ((CRect*)padding)->mTop;
                mForegroundPaddingRight = ((CRect*)padding)->mRight;
                mForegroundPaddingBottom = ((CRect*)padding)->mBottom;
            }
        } else {
            mForegroundPaddingLeft = 0;
            mForegroundPaddingTop = 0;
            mForegroundPaddingRight = 0;
            mForegroundPaddingBottom = 0;
        }

        RequestLayout();
    }
    return NOERROR;
}

Boolean FrameLayout::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return TRUE;
    //return super.verifyDrawable(who) || (who == mForeground);
}

ECode FrameLayout::DrawableStateChanged()
{
    FAIL_RETURN(ViewGroup::DrawableStateChanged());
    if (mForeground != NULL /*&& mForeground->IsStateful()*/) {
        //mForeground->SetState(GetDrawableState());
    }
    return NOERROR;
}

/**
  * Returns a set of layout parameters with a width of
  * {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT},
  * and a height of {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT}.
  */
ECode FrameLayout::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** params)
{
    return NOERROR;
    //return new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
}

/**
  * Supply a Drawable that is to be rendered on top of all of the child
  * views in the frame layout.  Any padding in the Drawable will be taken
  * into account by ensuring that the children are inset to be placed
  * inside of the padding area.
  *
  * @param drawable The Drawable to be drawn on top of the children.
  *
  * @attr ref android.R.styleable#FrameLayout_foreground
  */
ECode FrameLayout::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    if (mForeground.Get() != drawable) {
        if (mForeground != NULL) {
            mForeground->SetCallback(NULL);
            UnscheduleDrawable(mForeground);
        }

        mForeground = drawable;
        mForegroundPaddingLeft = 0;
        mForegroundPaddingTop = 0;
        mForegroundPaddingRight = 0;
        mForegroundPaddingBottom = 0;

        if (drawable != NULL) {
            //SetWillNotDraw(FALSE);
            //drawable->SetCallback(this);

            Boolean isStateful;
            drawable->IsStateful(&isStateful);
            if (isStateful) {
                //drawable->SetState(GetDrawableState());
            }
            if (mForegroundGravity == Gravity_FILL) {
                IRect* padding;
                CRect::New(&padding);
                Boolean isPadding;
                if (drawable->GetPadding(padding, &isPadding)) {
                    mForegroundPaddingLeft = ((CRect*)padding)->mLeft;
                    mForegroundPaddingTop = ((CRect*)padding)->mTop;
                    mForegroundPaddingRight = ((CRect*)padding)->mRight;
                    mForegroundPaddingBottom = ((CRect*)padding)->mBottom;
                }
            }
        }  else {
            //SetWillNotDraw(TRUE);
        }
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

/**
  * Returns the drawable used as the foreground of this FrameLayout. The
  * foreground drawable, if non-null, is always drawn on top of the children.
  *
  * @return A Drawable or null if no foreground was set.
  */
AutoPtr<IDrawable> FrameLayout::GetForeground()
{
    return mForeground;
}

void FrameLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    const Int32 count = GetChildCount();

    Int32 maxHeight = 0;
    Int32 maxWidth = 0;

    // Find rightmost and bottommost child
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> temp = GetChildAt(i);
        View* child = (View*)temp->Probe(EIID_View);
        if (mMeasureAllChildren || child->GetVisibility() != GONE) {
            MeasureChildWithMargins(temp, widthMeasureSpec, 0, heightMeasureSpec, 0);
            maxWidth = Math::Max(maxWidth, child->GetMeasuredWidth());
            maxHeight = Math::Max(maxHeight, child->GetMeasuredHeight());
        }
    }

    // Account for padding too
    maxWidth += mPaddingLeft + mPaddingRight + mForegroundPaddingLeft + mForegroundPaddingRight;
    maxHeight += mPaddingTop + mPaddingBottom + mForegroundPaddingTop + mForegroundPaddingBottom;

    // Check against our minimum height and width
    maxHeight = Math::Max(maxHeight, GetSuggestedMinimumHeight());
    maxWidth = Math::Max(maxWidth, GetSuggestedMinimumWidth());

    // Check against our foreground's minimum height and width
    AutoPtr<IDrawable> drawable = GetForeground();
    if (drawable != NULL) {
        Int32 drawMinHeight, drawMinWidth;
        drawable->GetMinimumHeight(&drawMinHeight);
        drawable->GetMinimumWidth(&drawMinWidth);

        maxHeight = Math::Max(maxHeight, drawMinHeight);
        maxWidth = Math::Max(maxWidth, drawMinWidth);
    }

    SetMeasuredDimension(ResolveSize(maxWidth, widthMeasureSpec),
            ResolveSize(maxHeight, heightMeasureSpec));
}

void FrameLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    const Int32 count = GetChildCount();

    const Int32 parentLeft = mPaddingLeft + mForegroundPaddingLeft;
    const Int32 parentRight = right - left - mPaddingRight - mForegroundPaddingRight;

    const Int32 parentTop = mPaddingTop + mForegroundPaddingTop;
    const Int32 parentBottom = bottom - top - mPaddingBottom - mForegroundPaddingBottom;

    mForegroundBoundsChanged = TRUE;

    for (Int32 i = 0; i < count; i++) {
        View* child = (View*)GetChildAt(i)->Probe(EIID_View);
        assert(child);
        if (child->GetVisibility() != GONE) {
            AutoPtr<IViewGroupLayoutParams> temp = child->GetLayoutParams();
            CFrameLayoutLayoutParams* lp = (CFrameLayoutLayoutParams*)temp.Get();
            assert(lp);

            Int32 width = child->GetMeasuredWidth();
            Int32 height = child->GetMeasuredHeight();
            Int32 childLeft = parentLeft;
            Int32 childTop = parentTop;
            Int32 gravity = lp->mGravity;

            //printf("child index = %d, width = %d, height = %d\n",
            //    i, width, height);
            //printf("childLeft = %d, childTop = %d, gravity = %d\n",
            //    childLeft, childTop, gravity);

            if (gravity != -1) {
                Int32 horizontalGravity = gravity & Gravity_HORIZONTAL_GRAVITY_MASK;
                Int32 verticalGravity = gravity & Gravity_VERTICAL_GRAVITY_MASK;

                switch (horizontalGravity) {
                    case Gravity_LEFT:
                        childLeft = parentLeft + lp->mLeftMargin;
                        break;
                    case Gravity_CENTER_HORIZONTAL:
                        childLeft = parentLeft + (parentRight - parentLeft - width) / 2 +
                                lp->mLeftMargin - lp->mRightMargin;
                        break;
                    case Gravity_RIGHT:
                        childLeft = parentRight - width - lp->mRightMargin;
                        break;
                    default:
                        childLeft = parentLeft + lp->mLeftMargin;
                }

                switch (verticalGravity) {
                    case Gravity_TOP:
                        childTop = parentTop + lp->mTopMargin;
                        break;
                    case Gravity_CENTER_VERTICAL:
                        childTop = parentTop + (parentBottom - parentTop - height) / 2 +
                                lp->mTopMargin - lp->mBottomMargin;
                        break;
                    case Gravity_BOTTOM:
                        childTop = parentBottom - height - lp->mBottomMargin;
                        break;
                    default:
                        childTop = parentTop + lp->mTopMargin;
                }
            }

            child->Layout(childLeft, childTop, childLeft + width, childTop + height);
        }
    }
}

void FrameLayout::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    ViewGroup::OnSizeChanged(w, h, oldw, oldh);
    mForegroundBoundsChanged = TRUE;
}

ECode FrameLayout::Draw(
    /* [in] */ ICanvas* canvas)
{
    ViewGroup::Draw(canvas);

    if (mForeground != NULL) {
        AutoPtr<IDrawable> foreground = mForeground;

        if (mForegroundBoundsChanged) {
            mForegroundBoundsChanged = FALSE;
            AutoPtr<IRect> selfBounds = (IRect*)mSelfBounds.Get();
            AutoPtr<IRect> overlayBounds = (IRect*)mOverlayBounds.Get();

            Int32 w = mRight - mLeft;
            Int32 h = mBottom - mTop;

            if (mForegroundInPadding) {
                selfBounds->Set(0, 0, w, h);
            } else {
                selfBounds->Set(mPaddingLeft, mPaddingTop, w - mPaddingRight,
                        h - mPaddingBottom);
            }

            Int32 iWidth, iHeight;
            foreground->GetIntrinsicWidth(&iWidth);
            foreground->GetIntrinsicHeight(&iHeight);
            AutoPtr<IGravity> gravity;
            CGravity::AcquireSingleton((IGravity**)&gravity);
            gravity->Apply(mForegroundGravity, iWidth, iHeight,
                    selfBounds, overlayBounds);
            foreground->SetBoundsEx(overlayBounds);
        }

        foreground->Draw(canvas);
    }

    return NOERROR;
}

Boolean FrameLayout::GatherTransparentRegion(
    /* [in] */ IRegion* region)
{
    //Boolean opaque = ViewGroup::GatherTransparentRegion(region);
    if (region != NULL && mForeground != NULL) {
        //ApplyDrawableToTransparentRegion(mForeground, region);
    }
    //return opaque;
    return TRUE;
}

/**
  * Determines whether to measure all children or just those in
  * the VISIBLE or INVISIBLE state when measuring. Defaults to FALSE.
  * @param measureAll TRUE to consider children marked GONE, FALSE otherwise.
  * Default value is FALSE.
  *
  * @attr ref android.R.styleable#FrameLayout_measureAllChildren
  */
ECode FrameLayout::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    mMeasureAllChildren = measureAll;
    return NOERROR;
}

/**
  * Determines whether to measure all children or just those in
  * the VISIBLE or INVISIBLE state when measuring.
  */
Boolean FrameLayout::GetConsiderGoneChildrenWhenMeasuring()
{
    return mMeasureAllChildren;
}

AutoPtr<IViewGroupLayoutParams> FrameLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IViewGroupLayoutParams> params;
    assert(SUCCEEDED(CFrameLayoutLayoutParams::New(GetContext(), attrs,
        (IFrameLayoutLayoutParams**)&params)));

    return params;
}

Boolean FrameLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p && (p->Probe(EIID_IFrameLayoutLayoutParams) != NULL);
}

AutoPtr<IViewGroupLayoutParams> FrameLayout::GenerateLayoutParams(
    /* [int] */ IViewGroupLayoutParams* p)
{
    assert(p);
    AutoPtr<IViewGroupLayoutParams> params;
    assert(SUCCEEDED(CFrameLayoutLayoutParams::New(p,
        (IFrameLayoutLayoutParams**)&params)));

    return params;
}

ECode FrameLayout::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context));
    return NOERROR;
}

ECode FrameLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(Init(context, attrs, 0));
    return NOERROR;
}

ECode FrameLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs, defStyle));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
    return NOERROR;
}

static Int32 R_Styleable_FrameLayout[] = {
    0x01010109, 0x0101010a, 0x01010200, 0x010102d1
};

ECode FrameLayout::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_FrameLayout, 4)/*com.android.internal.R.styleable.FrameLayout*/,
            defStyle, 0, (ITypedArray**)&a));

    a->GetInt32(2/*com.android.internal.R.styleable.FrameLayout_foregroundGravity*/,
            mForegroundGravity, &mForegroundGravity);

    AutoPtr<IDrawable> d;
    a->GetDrawable(0/*com.android.internal.R.styleable.FrameLayout_foreground*/,
            (IDrawable**)&d);
    if (d != NULL) {
        SetForeground(d);
    }

    Boolean value;
    a->GetBoolean(1/*com.android.internal.R.styleable.FrameLayout_measureAllChildren*/,
            FALSE, &value);
    if (value) {
        SetMeasureAllChildren(TRUE);
    }

    a->GetBoolean(3/*com.android.internal.R.styleable.FrameLayout_foregroundInsidePadding*/,
            TRUE, &mForegroundInPadding);

    a->Recycle();

    return NOERROR;
}
