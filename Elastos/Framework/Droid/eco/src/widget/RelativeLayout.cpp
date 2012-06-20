
#include "widget/RelativeLayout.h"
#include "view/CGravity.h"
#include "view/ViewGroup.h"
#include <elastos/Math.h>
#include <Logger.h>

using namespace Elastos::System;
using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;

const Int32 RelativeLayout::DependencyGraph::Node::POOL_LIMIT;

RelativeLayout::RelativeLayout() :
    mHasBaselineAlignedChild(FALSE),
    mGravity(Gravity_LEFT | Gravity_TOP),
    mIgnoreGravity(0),
    mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
{
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context));

    return NOERROR;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)

{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs));

    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));

    return NOERROR;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs, defStyle));

    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));

    return NOERROR;
}

static Int32 R_Styleable_RelativeLayout[] = {
    0x010100af, 0x010101ff
};

ECode RelativeLayout::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    assert(context != NULL);

    AutoPtr<ITypedArray> a;

    FAIL_RETURN(context->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_RelativeLayout, 6)/*com.android.internal.R.styleable.RelativeLayout*/,
            (ITypedArray**)&a));

    a->GetResourceId(1/*R.styleable.RelativeLayout_ignoreGravity*/, View_NO_ID, &mIgnoreGravity);
    a->GetInt32(0/*R.styleable.RelativeLayout_gravity*/, mGravity, &mGravity);
    a->Recycle();

    return NOERROR;
}

/**
 * Defines which View is ignored when the gravity is applied. This setting has no
 * effect if the gravity is <code>Gravity.LEFT | Gravity.TOP</code>.
 *
 * @param viewId The id of the View to be ignored by gravity, or 0 if no View
 *        should be ignored.
 *
 * @see #setGravity(int)
 *
 * @attr ref android.R.styleable#RelativeLayout_ignoreGravity
 */
ECode RelativeLayout::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    mIgnoreGravity = viewId;

    return NOERROR;
}

ECode RelativeLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mGravity != gravity) {
        if ((gravity & Gravity_HORIZONTAL_GRAVITY_MASK) == 0) {
            gravity |= Gravity_LEFT;
        }

        if ((gravity & Gravity_VERTICAL_GRAVITY_MASK) == 0) {
            gravity |= Gravity_TOP;
        }

        mGravity = gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    Int32 gravity = horizontalGravity & Gravity_HORIZONTAL_GRAVITY_MASK;
    if ((mGravity & Gravity_HORIZONTAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~Gravity_HORIZONTAL_GRAVITY_MASK) | gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    Int32 gravity = verticalGravity & Gravity_VERTICAL_GRAVITY_MASK;
    if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~Gravity_VERTICAL_GRAVITY_MASK) | gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::GetBaseline(
    /* [out] */ Int32* baseline)
{
    assert(baseline != NULL);

    mBaselineView != NULL ?
            mBaselineView->GetBaseline(baseline) : ViewGroup::GetBaseline(baseline);

    return NOERROR;
}

ECode RelativeLayout::RequestLayout()
{
    ASSERT_SUCCEEDED(ViewGroup::RequestLayout());

    mDirtyHierarchy = TRUE;

    mGraph = new DependencyGraph();

    return NOERROR;
}

void RelativeLayout::SortChildren()
{
    Int32 count = GetChildCount();

    if (mSortedVerticalChildren == NULL ||
            (Int32)mSortedVerticalChildren->GetSize() != count) {
        mSortedVerticalChildren = new Vector<AutoPtr<IView> >;
    }
    if (mSortedVerticalChildren == NULL ||
            (Int32)mSortedVerticalChildren->GetSize() != count) {
        mSortedHorizontalChildren = new Vector<AutoPtr<IView> >;
    }

    DependencyGraph* graph = mGraph;
    graph->Clear();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        graph->Add(child);
    }

//    if (DEBUG_GRAPH) {
//        d(LOG_TAG, "=== Sorted vertical children");
//        graph.log(getResources(), ABOVE, BELOW, ALIGN_BASELINE, ALIGN_TOP, ALIGN_BOTTOM);
//        d(LOG_TAG, "=== Sorted horizontal children");
//        graph.log(getResources(), LEFT_OF, RIGHT_OF, ALIGN_LEFT, ALIGN_RIGHT);
//    }

    Int32 rulesInt[] = {RelativeLayout_ABOVE, RelativeLayout_BELOW, RelativeLayout_ALIGN_BASELINE,
            RelativeLayout_ALIGN_TOP, RelativeLayout_ALIGN_BOTTOM};
    ArrayOf<Int32> rules(rulesInt, 5);
    graph->GetSortedViews(mSortedVerticalChildren, &rules);
    rulesInt = {RelativeLayout_LEFT_OF, RelativeLayout_RIGHT_OF, RelativeLayout_ALIGN_LEFT,
        RelativeLayout_ALIGN_RIGHT};
    rules.Copy(rulesInt, 5);
    graph->GetSortedViews(mSortedHorizontalChildren, &rules);
    ArrayOf<Int32>::Free(&rules);

//    if (DEBUG_GRAPH) {
//        d(LOG_TAG, "=== Ordered list of vertical children");
//        for (View view : mSortedVerticalChildren) {
//            DependencyGraph.printViewId(getResources(), view);
//        }
//        d(LOG_TAG, "=== Ordered list of horizontal children");
//        for (View view : mSortedHorizontalChildren) {
//            DependencyGraph.printViewId(getResources(), view);
//        }
//    }
}

//TODO: we need to find another way to implement RelativeLayout
// This implementation cannot handle every case
void RelativeLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mDirtyHierarchy) {
        mDirtyHierarchy = FALSE;
        SortChildren();
    }

    Int32 myWidth = -1;
    Int32 myHeight = -1;

    Int32 width = 0;
    Int32 height = 0;

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    // Record our dimensions if they are known;
    if (widthMode != MeasureSpec::UNSPECIFIED) {
        myWidth = widthSize;
    }

    if (heightMode != MeasureSpec::UNSPECIFIED) {
        myHeight = heightSize;
    }

    if (widthMode == MeasureSpec::EXACTLY) {
        width = myWidth;
    }

    if (heightMode == MeasureSpec::EXACTLY) {
        height = myHeight;
    }

    mHasBaselineAlignedChild = FALSE;

    AutoPtr<IView> ignore;
    Int32 gravity = mGravity & Gravity_HORIZONTAL_GRAVITY_MASK;
    Boolean horizontalGravity = gravity != Gravity_LEFT && gravity != 0;
    gravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;
    Boolean verticalGravity = gravity != Gravity_TOP && gravity != 0;

    Int32 left = Math::INT32_MAX_VALUE;
    Int32 top = Math::INT32_MAX_VALUE;
    Int32 right = Math::INT32_MIN_VALUE;
    Int32 bottom = Math::INT32_MIN_VALUE;

    Boolean offsetHorizontalAxis = FALSE;
    Boolean offsetVerticalAxis = FALSE;

    if ((horizontalGravity || verticalGravity) && mIgnoreGravity != View_NO_ID) {
        ignore = FindViewById(mIgnoreGravity);
    }

    Boolean isWrapContentWidth = widthMode != MeasureSpec::EXACTLY;
    Boolean isWrapContentHeight = heightMode != MeasureSpec::EXACTLY;

    Vector<AutoPtr<IView> >* views = mSortedHorizontalChildren;
    Vector<AutoPtr<IView> >::Iterator it;
    for (it = views->Begin(); it != views->End(); ++it) {
        AutoPtr<IView> child = *it;
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != View_GONE) {
            AutoPtr<IViewGroupLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(params.Get());

            ApplyHorizontalSizeRules(lp, myWidth);
            MeasureChildHorizontal(child, lp, myWidth, myHeight);
            if (PositionChildHorizontal(child, lp, myWidth, isWrapContentWidth)) {
                offsetHorizontalAxis = TRUE;
            }
        }
    }

    views = mSortedVerticalChildren;

    for (it = views->Begin(); it != views->End(); ++it) {
        AutoPtr<IView> child = *it;
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != View_GONE) {
            AutoPtr<IViewGroupLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(params.Get());

            ApplyVerticalSizeRules(lp, myHeight);
            MeasureChild(child, lp, myWidth, myHeight);
            if (PositionChildVertical(child, lp, myHeight, isWrapContentHeight)) {
                offsetVerticalAxis = TRUE;
            }

            if (isWrapContentWidth) {
                width = Math::Max(width, ((CRelativeLayoutLayoutParams*)lp)->mRight);
            }

            if (isWrapContentHeight) {
                height = Math::Max(height, ((CRelativeLayoutLayoutParams*)lp)->mBottom);
            }

            if (child != ignore || verticalGravity) {
                left = Math::Min(left,
                        ((CRelativeLayoutLayoutParams*)lp)->mLeft - ((CRelativeLayoutLayoutParams*)lp)->mLeftMargin);
                top = Math::Min(top,
                        ((CRelativeLayoutLayoutParams*)lp)->mTop - ((CRelativeLayoutLayoutParams*)lp)->mTopMargin);
            }

            if (child != ignore || horizontalGravity) {
                right = Math::Max(right,
                        ((CRelativeLayoutLayoutParams*)lp)->mRight + ((CRelativeLayoutLayoutParams*)lp)->mRightMargin);
                bottom = Math::Max(bottom,
                        ((CRelativeLayoutLayoutParams*)lp)->mBottom + ((CRelativeLayoutLayoutParams*)lp)->mBottomMargin);
            }
        }
    }

    if (mHasBaselineAlignedChild) {
        for (it = views->Begin(); it != views->End(); ++it) {
            AutoPtr<IView> child = *it;
            Int32 visibility;
            child->GetVisibility(&visibility);
            if (visibility != View_GONE) {
                AutoPtr<IViewGroupLayoutParams> params;
                child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(params.Get());

                AlignBaseline(child, lp);

                if (child != ignore || verticalGravity) {
                    left = Math::Min(left,
                            ((CRelativeLayoutLayoutParams*)lp)->mLeft -
                            ((CRelativeLayoutLayoutParams*)lp)->mLeftMargin);
                    top = Math::Min(top,
                            ((CRelativeLayoutLayoutParams*)lp)->mTop -
                            ((CRelativeLayoutLayoutParams*)lp)->mTopMargin);
                }

                if (child != ignore || horizontalGravity) {
                    right = Math::Max(right,
                            ((CRelativeLayoutLayoutParams*)lp)->mRight +
                            ((CRelativeLayoutLayoutParams*)lp)->mRightMargin);
                    bottom = Math::Max(bottom,
                            ((CRelativeLayoutLayoutParams*)lp)->mBottom +
                            ((CRelativeLayoutLayoutParams*)lp)->mBottomMargin);
                }
            }
        }
    }

    if (isWrapContentWidth) {
        // Width already has left padding in it since it was calculated by looking at
        // the right of each child view
        width += mPaddingRight;

        IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(mLayoutParams.Get());

        if (((CRelativeLayoutLayoutParams*)lp)->mWidth >= 0) {
            width = Math::Max(width, ((CRelativeLayoutLayoutParams*)lp)->mWidth);
        }

        width = Math::Max(width, GetSuggestedMinimumWidth());
        width = ResolveSize(width, widthMeasureSpec);

        if (offsetHorizontalAxis) {
            Int32 count = views->GetSize();
            for (Int32 i = 0; i < count; ++i) {
                AutoPtr<IView> child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != View_GONE) {
                    AutoPtr<IViewGroupLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                    IRelativeLayoutLayoutParams* lp =
                    (IRelativeLayoutLayoutParams*)(mLayoutParams.Get());
                    ArrayOf<Int32>* rules; lp->GetRules(&rules);
                    if (rules[RelativeLayout_CENTER_IN_PARENT] != 0 ||
                            rules[RelativeLayout_CENTER_HORIZONTAL] != 0) {
                        CenterHorizontal(child, lp, width);
                    }
                    else if (rules[RelativeLayout_ALIGN_PARENT_RIGHT] != 0) {
                        Int32 childWidth;
                        child->GetMeasuredWidth(&childWidth);
                        ((CRelativeLayoutLayoutParams*)lp)->mLeft = width -
                                mPaddingRight - childWidth;
                        ((CRelativeLayoutLayoutParams*)lp)->mRight =
                                ((CRelativeLayoutLayoutParams*)lp)->mLeft + childWidth;
                    }
                }
            }
        }
    }

    if (isWrapContentHeight) {
        // Height already has top padding in it since it was calculated by looking at
        // the bottom of each child view
        height += mPaddingBottom;

        IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(mLayoutParams.Get());
        if (((CRelativeLayoutLayoutParams*)lp)->mHeight >= 0) {
            height = Math::Max(height, ((CRelativeLayoutLayoutParams*)lp)->mHeight);
        }

        height = Math::Max(height, GetSuggestedMinimumHeight());
        height = ResolveSize(height, heightMeasureSpec);

        if (offsetVerticalAxis) {
            Int32 count = views->GetSize();
            for (Int32 i = 0; i < count; i++) {
                AutoPtr<IView> child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != View_GONE) {
                    AutoPtr<IViewGroupLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                    lp = (IRelativeLayoutLayoutParams*)(params.Get());

                    ArrayOf<Int32>* rules;
                    lp->GetRules(&rules);
                    if (rules[RelativeLayout_CENTER_IN_PARENT] != 0 ||
                            rules[RelativeLayout_CENTER_VERTICAL] != 0) {
                        CenterVertical(child, lp, height);
                    }
                    else if (rules[RelativeLayout_ALIGN_PARENT_BOTTOM] != 0) {
                        Int32 childHeight;
                        child->GetMeasuredHeight(&childHeight);
                        ((CRelativeLayoutLayoutParams*)lp)->mTop = height - mPaddingBottom - childHeight;
                        ((CRelativeLayoutLayoutParams*)lp)->mBottom =
                                ((CRelativeLayoutLayoutParams*)lp)->mTop + childHeight;
                    }
                }
            }
        }
    }

    if (horizontalGravity || verticalGravity) {
        AutoPtr<IRect> selfBounds = mSelfBounds;
        selfBounds->Set(mPaddingLeft, mPaddingTop, width - mPaddingRight,
                height - mPaddingBottom);

        AutoPtr<IRect> contentBounds = mContentBounds;
        AutoPtr<IGravity> gravity;
        CGravity::AcquireSingleton((IGravity**)&gravity);
        gravity->Apply(mGravity, right - left, bottom - top,
                selfBounds, contentBounds);

        Int32 horizontalOffset = ((CRect*)contentBounds.Get())->mLeft - left;
        Int32 verticalOffset = ((CRect*)contentBounds.Get())->mTop - top;
        if (horizontalOffset != 0 || verticalOffset != 0) {
            Int32 count = views->GetSize();
            for (Int32 i = 0; i < count; ++i) {
                AutoPtr<IView> child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != View_GONE && child != ignore) {
                    AutoPtr<IViewGroupLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
                    IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(mLayoutParams.Get());
                    if (horizontalGravity) {
                        ((CRelativeLayoutLayoutParams*)lp)->mLeft += horizontalOffset;
                        ((CRelativeLayoutLayoutParams*)lp)->mRight += horizontalOffset;
                    }
                    if (verticalGravity) {
                        ((CRelativeLayoutLayoutParams*)lp)->mTop += verticalOffset;
                        ((CRelativeLayoutLayoutParams*)lp)->mBottom += verticalOffset;
                    }
                }
            }
        }
    }

    SetMeasuredDimension(width, height);
}

void RelativeLayout::AlignBaseline(
    /* [in] */ IView* child,
    /* [in] */ IRelativeLayoutLayoutParams* params)
{
    assert(child != NULL && params != NULL);

    ArrayOf<Int32>* rules;
    params->GetRules(&rules);
    Int32 anchorBaseline = GetRelatedViewBaseline(rules, RelativeLayout_ALIGN_BASELINE);

    if (anchorBaseline != -1) {
        AutoPtr<IRelativeLayoutLayoutParams> anchorParams =
                GetRelatedViewParams(rules, RelativeLayout_ALIGN_BASELINE);

        if (anchorParams != NULL) {
            Int32 offset = ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mTop + anchorBaseline;
            Int32 baseline;
            child->GetBaseline(&baseline);
            if (baseline != -1) {
                offset -= baseline;
            }
            Int32 height = ((CRelativeLayoutLayoutParams*)params)->mBottom -
                    ((CRelativeLayoutLayoutParams*)params)->mTop;
            ((CRelativeLayoutLayoutParams*)params)->mTop = offset;
            ((CRelativeLayoutLayoutParams*)params)->mBottom =
                    ((CRelativeLayoutLayoutParams*)params)->mTop + height;
        }
    }

    if (mBaselineView == NULL) {
        mBaselineView = child;
    }
    else {
        AutoPtr<IViewGroupLayoutParams> layoutParams;
        mBaselineView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(mLayoutParams.Get());
        if (((CRelativeLayoutLayoutParams*)params)->mTop < ((CRelativeLayoutLayoutParams*)lp)->mTop
                || ((CRelativeLayoutLayoutParams*)params)->mTop == ((CRelativeLayoutLayoutParams*)lp)->mTop
                && ((CRelativeLayoutLayoutParams*)params)->mLeft < ((CRelativeLayoutLayoutParams*)lp)->mLeft) {
            mBaselineView = child;
        }
    }
}

/**
 * Measure a child. The child should have left, top, right and bottom information
 * stored in its LayoutParams. If any of these values is -1 it means that the view
 * can extend up to the corresponding edge.
 *
 * @param child Child to measure
 * @param params LayoutParams associated with child
 * @param myWidth Width of the the RelativeLayout
 * @param myHeight Height of the RelativeLayout
 */
void RelativeLayout::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ IRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(((CRelativeLayoutLayoutParams*)params)->mLeft,
            ((CRelativeLayoutLayoutParams*)params)->mRight, ((CRelativeLayoutLayoutParams*)params)->mWidth,
            ((CRelativeLayoutLayoutParams*)params)->mLeftMargin, ((CRelativeLayoutLayoutParams*)params)->mRightMargin,
            mPaddingLeft, mPaddingRight,
            myWidth);
    Int32 childHeightMeasureSpec = GetChildMeasureSpec(((CRelativeLayoutLayoutParams*)params)->mTop,
            ((CRelativeLayoutLayoutParams*)params)->mBottom, ((CRelativeLayoutLayoutParams*)params)->mHeight,
            ((CRelativeLayoutLayoutParams*)params)->mTopMargin, ((CRelativeLayoutLayoutParams*)params)->mBottomMargin,
            mPaddingTop, mPaddingBottom,
            myHeight);
    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

void RelativeLayout::MeasureChildHorizontal(
    /* [in] */ IView* child,
    /* [in] */ IRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(((CRelativeLayoutLayoutParams*)params)->mLeft,
            ((CRelativeLayoutLayoutParams*)params)->mRight, ((CRelativeLayoutLayoutParams*)params)->mWidth,
            ((CRelativeLayoutLayoutParams*)params)->mLeftMargin, ((CRelativeLayoutLayoutParams*)params)->mRightMargin,
            mPaddingLeft, mPaddingRight,
            myWidth);
    Int32 childHeightMeasureSpec;
    if (((CRelativeLayoutLayoutParams*)params)->mWidth == ViewGroupLayoutParams_MATCH_PARENT) {
        childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::EXACTLY);
    }
    else {
        childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::AT_MOST);
    }
    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

/**
 * Get a measure spec that accounts for all of the constraints on this view.
 * This includes size contstraints imposed by the RelativeLayout as well as
 * the View's desired dimension.
 *
 * @param childStart The left or top field of the child's layout params
 * @param childEnd The right or bottom field of the child's layout params
 * @param childSize The child's desired size (the width or height field of
 *        the child's layout params)
 * @param startMargin The left or top margin
 * @param endMargin The right or bottom margin
 * @param startPadding mPaddingLeft or mPaddingTop
 * @param endPadding mPaddingRight or mPaddingBottom
 * @param mySize The width or height of this view (the RelativeLayout)
 * @return MeasureSpec for the child
 */
Int32 RelativeLayout::GetChildMeasureSpec(
    /* [in] */ Int32 childStart,
    /* [in] */ Int32 childEnd,
    /* [in] */ Int32 childSize,
    /* [in] */ Int32 startMargin,
    /* [in] */ Int32 endMargin,
    /* [in] */ Int32 startPadding,
    /* [in] */ Int32 endPadding,
    /* [in] */ Int32 mySize)
{
    Int32 childSpecMode = 0;
    Int32 childSpecSize = 0;

    // Figure out start and end bounds.
    Int32 tempStart = childStart;
    Int32 tempEnd = childEnd;

    // If the view did not express a layout constraint for an edge, use
    // view's margins and our padding
    if (tempStart < 0) {
        tempStart = startPadding + startMargin;
    }
    if (tempEnd < 0) {
        tempEnd = mySize - endPadding - endMargin;
    }

    // Figure out maximum size available to this view
    Int32 maxAvailable = tempEnd - tempStart;

    if (childStart >= 0 && childEnd >= 0) {
        // Constraints fixed both edges, so child must be an exact size
        childSpecMode = MeasureSpec::EXACTLY;
        childSpecSize = maxAvailable;
    }
    else {
        if (childSize >= 0) {
            // Child wanted an exact size. Give as much as possible
            childSpecMode = MeasureSpec::EXACTLY;

            if (maxAvailable >= 0) {
                // We have a maxmum size in this dimension.
                childSpecSize = Math::Min(maxAvailable, childSize);
            }
            else {
                // We can grow in this dimension.
                childSpecSize = childSize;
            }
        }
        else if (childSize == ViewGroupLayoutParams_MATCH_PARENT) {
            // Child wanted to be as big as possible. Give all availble
            // space
            childSpecMode = MeasureSpec::EXACTLY;
            childSpecSize = maxAvailable;
        }
        else if (childSize == ViewGroupLayoutParams_WRAP_CONTENT) {
            // Child wants to wrap content. Use AT_MOST
            // to communicate available space if we know
            // our max size
            if (maxAvailable >= 0) {
                // We have a maxmum size in this dimension.
                childSpecMode = MeasureSpec::AT_MOST;
                childSpecSize = maxAvailable;
            }
            else {
                // We can grow in this dimension. Child can be as big as it
                // wants
                childSpecMode = MeasureSpec::UNSPECIFIED;
                childSpecSize = 0;
            }
        }
    }

    return MeasureSpec::MakeMeasureSpec(childSpecSize, childSpecMode);
}

Boolean RelativeLayout::PositionChildHorizontal(
    /* [in] */ IView* child,
    /* [in] */ IRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Boolean wrapContent)
{
    assert(child != NULL && params != NULL);

    ArrayOf<Int32>* rules;
    params->GetRules(&rules);

    if (((CRelativeLayoutLayoutParams*)params)->mLeft < 0 &&
            ((CRelativeLayoutLayoutParams*)params)->mRight >= 0) {
        // Right is fixed, but left varies
        Int32 width;
        child->GetMeasuredWidth(&width);
        ((CRelativeLayoutLayoutParams*)params)->mLeft =
                ((CRelativeLayoutLayoutParams*)params)->mRight - width;
    }
    else if (((CRelativeLayoutLayoutParams*)params)->mLeft >= 0 &&
            ((CRelativeLayoutLayoutParams*)params)->mRight < 0) {
        // Left is fixed, but right varies
        Int32 width;
        child->GetMeasuredWidth(&width);
        ((CRelativeLayoutLayoutParams*)params)->mRight =
                ((CRelativeLayoutLayoutParams*)params)->mLeft + width;
    }
    else if (((CRelativeLayoutLayoutParams*)params)->mLeft < 0 &&
            ((CRelativeLayoutLayoutParams*)params)->mRight < 0) {
        // Both left and right vary
        if (rules[RelativeLayout_CENTER_IN_PARENT] != 0 || rules[RelativeLayout_CENTER_HORIZONTAL] != 0) {
            if (!wrapContent) {
                CenterHorizontal(child, params, myWidth);
            }
            else {
                ((CRelativeLayoutLayoutParams*)params)->mLeft =
                        mPaddingLeft + ((CRelativeLayoutLayoutParams*)params)->mLeftMargin;

                Int32 width;
                child->GetMeasuredWidth(&width);
                ((CRelativeLayoutLayoutParams*)params)->mRight =
                        ((CRelativeLayoutLayoutParams*)params)->mLeft + width;
            }
            return TRUE;
        }
        else {
            ((CRelativeLayoutLayoutParams*)params)->mLeft =
                   mPaddingLeft + ((CRelativeLayoutLayoutParams*)params)->mLeftMargin;

            Int32 width;
            child->GetMeasuredWidth(&width);
            ((CRelativeLayoutLayoutParams*)params)->mRight =
                   ((CRelativeLayoutLayoutParams*)params)->mLeft + width;
        }
    }
    return rules[RelativeLayout_ALIGN_PARENT_RIGHT] != 0;
}

Boolean RelativeLayout::PositionChildVertical(
    /* [in] */ IView* child,
    /* [in] */ IRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myHeight,
    /* [in] */ Boolean wrapContent)
{
    assert(child != NULL && params != NULL);

    ArrayOf<Int32>* rules;
    params->GetRules(&rules);

    if (((CRelativeLayoutLayoutParams*)params)->mTop < 0 &&
            ((CRelativeLayoutLayoutParams*)params)->mBottom >= 0) {
        // Bottom is fixed, but top varies
        Int32 height;
        child->GetMeasuredHeight(&height);
        ((CRelativeLayoutLayoutParams*)params)->mTop =
                ((CRelativeLayoutLayoutParams*)params)->mBottom - height;
    }
    else if (((CRelativeLayoutLayoutParams*)params)->mTop >= 0 &&
            ((CRelativeLayoutLayoutParams*)params)->mBottom < 0) {
        // Top is fixed, but bottom varies
        Int32 height;
        child->GetMeasuredHeight(&height);
        ((CRelativeLayoutLayoutParams*)params)->mBottom =
            ((CRelativeLayoutLayoutParams*)params)->mTop + height;
    }
    else if (((CRelativeLayoutLayoutParams*)params)->mTop < 0 &&
            ((CRelativeLayoutLayoutParams*)params)->mBottom < 0) {
        // Both top and bottom vary
        if (rules[RelativeLayout_CENTER_IN_PARENT] != 0 || rules[RelativeLayout_CENTER_VERTICAL] != 0) {
            if (!wrapContent) {
                CenterVertical(child, params, myHeight);
            }
            else {
                ((CRelativeLayoutLayoutParams*)params)->mTop =
                        mPaddingTop + ((CRelativeLayoutLayoutParams*)params)->mTopMargin;

                Int32 height;
                child->GetMeasuredHeight(&height);
                ((CRelativeLayoutLayoutParams*)params)->mBottom =
                        ((CRelativeLayoutLayoutParams*)params)->mTop + height;
            }
            return TRUE;
        }
        else {
            ((CRelativeLayoutLayoutParams*)params)->mTop =
                    mPaddingTop + ((CRelativeLayoutLayoutParams*)params)->mTopMargin;

            Int32 height;
            child->GetMeasuredHeight(&height);
            ((CRelativeLayoutLayoutParams*)params)->mBottom =
                    ((CRelativeLayoutLayoutParams*)params)->mTop + height;
        }
    }
    return rules[RelativeLayout_ALIGN_PARENT_BOTTOM] != 0;
}

void RelativeLayout::ApplyHorizontalSizeRules(
    /* [in] */ IRelativeLayoutLayoutParams* childParams,
    /* [in] */ Int32 myWidth)
{
    assert(childParams != NULL);

    ArrayOf<Int32>* rules;
    childParams->GetRules(&rules);
    AutoPtr<IRelativeLayoutLayoutParams> anchorParams;

    // -1 indicated a "soft requirement" in that direction. For example:
    // left=10, right=-1 means the view must start at 10, but can go as far as it wants to the right
    // left =-1, right=10 means the view must end at 10, but can go as far as it wants to the left
    // left=10, right=20 means the left and right ends are both fixed
    ((CRelativeLayoutLayoutParams*)childParams)->mLeft = -1;
    ((CRelativeLayoutLayoutParams*)childParams)->mRight = -1;

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_LEFT_OF);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mRight =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mLeft -
                (((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mLeftMargin +
                ((CRelativeLayoutLayoutParams*)childParams)->mRightMargin);
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent && rules[RelativeLayout_LEFT_OF] != 0) {
        if (myWidth >= 0) {
            ((CRelativeLayoutLayoutParams*)childParams)->mRight = myWidth
                    - mPaddingRight - ((CRelativeLayoutLayoutParams*)childParams)->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_RIGHT_OF);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mLeft =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mRight +
                (((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mRightMargin +
                ((CRelativeLayoutLayoutParams*)childParams)->mLeftMargin);
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent && rules[RelativeLayout_RIGHT_OF] != 0) {
        ((CRelativeLayoutLayoutParams*)childParams)->mLeft = mPaddingLeft +
                ((CRelativeLayoutLayoutParams*)childParams)->mLeftMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_LEFT);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mLeft =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mLeft +
                ((CRelativeLayoutLayoutParams*)childParams)->mLeftMargin;
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent && rules[RelativeLayout_ALIGN_LEFT] != 0) {
        ((CRelativeLayoutLayoutParams*)childParams)->mLeft = mPaddingLeft +
                ((CRelativeLayoutLayoutParams*)childParams)->mLeftMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_RIGHT);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mRight =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mRight -
                ((CRelativeLayoutLayoutParams*)childParams)->mRightMargin;
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent && rules[RelativeLayout_ALIGN_RIGHT] != 0) {
        if (myWidth >= 0) {
            ((CRelativeLayoutLayoutParams*)childParams)->mRight = myWidth -
                    mPaddingRight - ((CRelativeLayoutLayoutParams*)childParams)->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if (0 != rules[RelativeLayout_ALIGN_PARENT_LEFT]) {
        ((CRelativeLayoutLayoutParams*)childParams)->mLeft = mPaddingLeft +
                ((CRelativeLayoutLayoutParams*)childParams)->mLeftMargin;
    }

    if (0 != rules[RelativeLayout_ALIGN_PARENT_RIGHT]) {
        if (myWidth >= 0) {
            ((CRelativeLayoutLayoutParams*)childParams)->mRight = myWidth -
                    mPaddingRight - ((CRelativeLayoutLayoutParams*)childParams)->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }
}

void RelativeLayout::ApplyVerticalSizeRules(
    /* [in] */ IRelativeLayoutLayoutParams* childParams,
    /* [in] */ Int32 myHeight)
{
    assert(childParams != NULL);

    ArrayOf<Int32>* rules;
    childParams->GetRules(&rules);
    AutoPtr<IRelativeLayoutLayoutParams> anchorParams;

    ((CRelativeLayoutLayoutParams*)childParams)->mTop = -1;
    ((CRelativeLayoutLayoutParams*)childParams)->mBottom = -1;

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ABOVE);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mBottom =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mTop -
                (((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mTopMargin +
                ((CRelativeLayoutLayoutParams*)childParams)->mBottomMargin);
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent &&
            rules[RelativeLayout_ABOVE] != 0) {
        if (myHeight >= 0) {
            ((CRelativeLayoutLayoutParams*)childParams)->mBottom =
                    myHeight - mPaddingBottom - ((CRelativeLayoutLayoutParams*)childParams)->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_BELOW);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mTop =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mBottom +
                (((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mBottomMargin +
                ((CRelativeLayoutLayoutParams*)childParams)->mTopMargin);
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent &&
            rules[RelativeLayout_BELOW] != 0) {
        ((CRelativeLayoutLayoutParams*)childParams)->mTop =
                mPaddingTop + ((CRelativeLayoutLayoutParams*)childParams)->mTopMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_TOP);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mTop =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mTop + ((CRelativeLayoutLayoutParams*)childParams)->mTopMargin;
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent &&
            rules[RelativeLayout_ALIGN_TOP] != 0) {
        ((CRelativeLayoutLayoutParams*)childParams)->mTop =
                mPaddingTop + ((CRelativeLayoutLayoutParams*)childParams)->mTopMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_BOTTOM);
    if (anchorParams != NULL) {
        ((CRelativeLayoutLayoutParams*)childParams)->mBottom =
                ((CRelativeLayoutLayoutParams*)(anchorParams.Get()))->mBottom - ((CRelativeLayoutLayoutParams*)childParams)->mBottomMargin;
    }
    else if (((CRelativeLayoutLayoutParams*)childParams)->mAlignWithParent &&
            rules[RelativeLayout_ALIGN_BOTTOM] != 0) {
        if (myHeight >= 0) {
            ((CRelativeLayoutLayoutParams*)childParams)->mBottom =
                    myHeight - mPaddingBottom - ((CRelativeLayoutLayoutParams*)childParams)->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if (0 != rules[RelativeLayout_ALIGN_PARENT_TOP]) {
        ((CRelativeLayoutLayoutParams*)childParams)->mTop = mPaddingTop + ((CRelativeLayoutLayoutParams*)childParams)->mTopMargin;
    }

    if (0 != rules[RelativeLayout_ALIGN_PARENT_BOTTOM]) {
        if (myHeight >= 0) {
            ((CRelativeLayoutLayoutParams*)childParams)->mBottom = myHeight - mPaddingBottom - ((CRelativeLayoutLayoutParams*)childParams)->mBottomMargin;
        } else {
            // FIXME uh oh...
        }
    }

    if (rules[RelativeLayout_ALIGN_BASELINE] != 0) {
        mHasBaselineAlignedChild = TRUE;
    }
}

AutoPtr<IView> RelativeLayout::GetRelatedView(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    assert(rules != NULL);

    Int32 id = (*rules)[relation];
    if (id != 0) {
        RelativeLayout::DependencyGraph::Node* node = mGraph->mKeyNodes[id];
        if (node == NULL) return NULL;
        AutoPtr<IView> v = node->mView;

        // Find the first non-GONE view up the chain
        Int32 visibility;
        v->GetVisibility(&visibility);
        while (visibility == View_GONE) {
            AutoPtr<IViewGroupLayoutParams> params;
            v->GetLayoutParams((IViewGroupLayoutParams**)&params);
            IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)(params.Get());
            lp->GetRules(&rules);
            node = mGraph->mKeyNodes[(*rules)[relation]];
            if (node == NULL) return NULL;
            v = node->mView;
        }

        return v;
    }

    return NULL;
}

AutoPtr<IRelativeLayoutLayoutParams> RelativeLayout::GetRelatedViewParams(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    AutoPtr<IView> v = GetRelatedView(rules, relation);
    if (v != NULL) {
        AutoPtr<IViewGroupLayoutParams> params;
        v->GetLayoutParams((IViewGroupLayoutParams**)&params);

        if (params && params->Probe(EIID_IRelativeLayoutLayoutParams) != NULL)
            return AutoPtr<IRelativeLayoutLayoutParams>(
                    (IRelativeLayoutLayoutParams*)params->Probe(EIID_IRelativeLayoutLayoutParams));
    }

    return NULL;
}

Int32 RelativeLayout::GetRelatedViewBaseline(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    assert(rules != NULL);

    AutoPtr<IView> v = GetRelatedView(rules, relation);
    if (v != NULL) {
        Int32 baseline;
        v->GetBaseline(&baseline);
        return baseline;
    }
    return -1;
}

void RelativeLayout::CenterHorizontal(
    /* [in] */ IView* child,
    /* [in] */ IRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth)
{
    assert(child != NULL);

    Int32 childWidth;
    child->GetMeasuredWidth(&childWidth);
    Int32 left = (myWidth - childWidth) / 2;

    ((CRelativeLayoutLayoutParams*)params)->mLeft = left;
    ((CRelativeLayoutLayoutParams*)params)->mRight = left + childWidth;
}

void RelativeLayout::CenterVertical(
    /* [in] */ IView* child,
    /* [in] */ IRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childHeight;
    child->GetMeasuredHeight(&childHeight);
    Int32 top = (myHeight - childHeight) / 2;

    ((CRelativeLayoutLayoutParams*)params)->mTop = top;
    ((CRelativeLayoutLayoutParams*)params)->mBottom = top + childHeight;
}

void RelativeLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    //  The layout has actually already been performed and the positions
    //  cached.  Apply the cached values to the children.
    Int32 count = GetChildCount();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != View_GONE) {
            AutoPtr<IViewGroupLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);
            CRelativeLayoutLayoutParams* st = (CRelativeLayoutLayoutParams*)(IRelativeLayoutLayoutParams*)params.Get();
            child->Layout(st->mLeft, st->mTop, st->mRight, st->mBottom);

        }
    }
}

AutoPtr<IViewGroupLayoutParams> RelativeLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    assert(attrs != NULL);
    AutoPtr<IRelativeLayoutLayoutParams> lp;
    assert(SUCCEEDED(CRelativeLayoutLayoutParams::New(GetContext(), attrs,
        (IRelativeLayoutLayoutParams**)&lp)));

    return AutoPtr<IViewGroupLayoutParams>(
            (IViewGroupLayoutParams*)lp->Probe(EIID_IViewGroupLayoutParams));
}

ECode RelativeLayout::GenerateDefaultLayoutParams(
    /* [out] */ IRelativeLayoutLayoutParams** lp)
{
    assert(lp != NULL);

    return CRelativeLayoutLayoutParams::New(
                ViewGroupLayoutParams_WRAP_CONTENT,
                ViewGroupLayoutParams_WRAP_CONTENT,
                lp);
}

// Override to allow type-checking of LayoutParams.
Boolean RelativeLayout::CheckLayoutParams(
    /* [in] */ IRelativeLayoutLayoutParams* p)
{
    return p && p->Probe(EIID_IRelativeLayoutLayoutParams) != NULL;
}

AutoPtr<IViewGroupLayoutParams> RelativeLayout::GenerateLayoutParams(
    /* [in] */ IRelativeLayoutLayoutParams* p)
{
    assert(p != NULL);

    AutoPtr<IRelativeLayoutLayoutParams> lp;
    assert(SUCCEEDED(CRelativeLayoutLayoutParams::New(p,
        (IRelativeLayoutLayoutParams**)&lp)));

    return AutoPtr<IViewGroupLayoutParams>(
        (IViewGroupLayoutParams*)lp->Probe(EIID_IViewGroupLayoutParams));
}

Boolean RelativeLayout::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* p)
{
    if (mTopToBottomLeftToRightSet == NULL) {
//        mTopToBottomLeftToRightSet = new TreeSet<View>(new TopToBottomLeftToRightComparator());
        mTopToBottomLeftToRightSet = new Set<AutoPtr<IView> >;
    }

    // sort children top-to-bottom and left-to-right
    for (Int32 i = 0, count = GetChildCount(); i < count; i++) {
        mTopToBottomLeftToRightSet->Insert(GetChildAt(i));
    }

    Set<AutoPtr<IView> >::Iterator it;
    for (it = mTopToBottomLeftToRightSet->Begin(); it != mTopToBottomLeftToRightSet->End(); ++it) {
        View* v = (View*)(*it)->Probe(EIID_View);
        if (v->DispatchPopulateAccessibilityEvent(p)) {
            mTopToBottomLeftToRightSet->Clear();
            return TRUE;
        }
    }

    mTopToBottomLeftToRightSet->Clear();

    return FALSE;
}

/**
 * Clears the graph.
 */
void RelativeLayout::DependencyGraph::Clear()
{
    List<Node*> nodes = mNodes;
    List<Node*>::Iterator it;

    for (it = nodes.Begin(); it != nodes.End(); ++it) {
        (*it)->Release();
    }

    nodes.Clear();

    mKeyNodes.Clear();
    mRoots.Clear();
}

/**
 * Adds a view to the graph.
 *
 * @param view The view to be added as a node to the graph.
 */
void RelativeLayout::DependencyGraph::Add(
    /* [in] */ IView* view)
{
    assert(view != NULL);

    Int32 id = ((View*)view)->GetId();
    Node* node = Node::Acquire(view);

    if (id != View_NO_ID) {
        mKeyNodes.Insert(id, node);
    }

    mNodes.PushBack(node);
}

/**
 * Builds a sorted list of views. The sorting order depends on the dependencies
 * between the view. For instance, if view C needs view A to be processed first
 * and view A needs view B to be processed first, the dependency graph
 * is: B -> A -> C. The sorted array will contain view s B, A and C in this order.
 *
 * @param sorted The sorted list of views. The length of this array must
 *        be equal to getChildCount().
 * @param rules The list of rules to take into account.
 */
void RelativeLayout::DependencyGraph::GetSortedViews(
    /* [in] */ Vector<AutoPtr<IView> >* sorted,
    /* [in] */ ArrayOf<Int32>* rules)
{
    assert(sorted != NULL);

    List<RelativeLayout::DependencyGraph::Node*>& roots = FindRoots(rules);

    Vector<AutoPtr<IView> >::Iterator it = sorted->Begin();
    while (roots.Begin() != roots.End()) {
        Node* node = *(roots.Begin());
        roots.Erase(roots.Begin());

        IView* view = node->mView.Get();
        Int32 key = ((View*)view)->GetId();

        sorted->Insert(it++, node->mView);

        Set<RelativeLayout::DependencyGraph::Node*>& dependents = node->mDependents;
        Set<RelativeLayout::DependencyGraph::Node*>::Iterator it;
        for (it = dependents.Begin(); it != dependents.End(); ++it) {
            Node* dependent = *it;
            List<RelativeLayout::DependencyGraph::Node*> dependencies = dependent->mDependencies;
            dependencies.Remove(key);
            if (dependencies.Begin() == dependencies.End()) {
                roots.PushBack(dependent);
            }
        }
    }

    if (it != sorted->End()) {
//        throw new IllegalStateException("Circular dependencies cannot exist"
//                + " in RelativeLayout");
    }
}

/**
 * Finds the roots of the graph. A root is a node with no dependency and
 * with [0..n] dependents.
 *
 * @param rulesFilter The list of rules to consider when building the
 *        dependencies
 *
 * @return A list of node, each being a root of the graph
 */
List<RelativeLayout::DependencyGraph::Node*>&
RelativeLayout::DependencyGraph::FindRoots(
    /* [in] */ ArrayOf<Int32>* rulesFilter)
{
    assert(rulesFilter != NULL);

    List<Node*>& keyNodes = mKeyNodes;
    List<Node*>& nodes = mNodes;
    List<Node*>::Iterator it;

    // Find roots can be invoked several times, so make sure to clear
    // all dependents and dependencies before running the algorithm
    for (it = nodes.Begin(); it != nodes.End(); ++it) {
        Node* node = *it;
        node->mDependents.Clear();
        node->mDependencies.Clear();
    }

    // Builds up the dependents and dependencies for each node of the graph
    for (it = nodes.Begin(); it != nodes.End(); ++it) {
        Node* node = *it;

        AutoPtr<IViewGroupLayoutParams> layoutParams;
        node->mView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        IRelativeLayoutLayoutParams* lp = (IRelativeLayoutLayoutParams*)layoutParams.Get();
        ArrayOf<Int32>* rules = ((CRelativeLayoutLayoutParams*)lp)->mRules.Clone();
        Int32 rulesCount = rulesFilter->GetLength();
        // Look only the the rules passed in parameter, this way we build only the
        // dependencies for a specific set of rules
        for (Int32 j = 0; j < rulesCount; j++) {
            Int32 rule = (*rules)[(*rulesFilter)[j]];
            if (rule > 0) {
                // The node this node depends on
                Node* dependency = keyNodes[rule];
                // Skip unknowns and self dependencies
                if (dependency == NULL || dependency == node) {
                    continue;
                }
                // Add the current node as a dependent
//                dependency->mDependents.Insert(node);
                // Add a dependency to the current node
                node->mDependencies.Insert(rule, dependency);
            }
        }
    }

    List<Node*>& roots = mRoots;
    roots.Clear();

    // Finds all the roots in the graph: all nodes with no dependencies
    for (it = nodes.Begin(); it != nodes.End(); ++it) {
        Node* node = *it;
        if (node->mDependencies.Begin() == node->mDependencies.End()) {
            roots.PushBack(node);
        }
    }

    return roots;
}

/**
 * Prints the dependency graph for the specified rules.
 *
 * @param resources The context's resources to print the ids.
 * @param rules The list of rules to take into account.
 */
void RelativeLayout::DependencyGraph::Log(
    /* [in] */ IResources* resources,
    /* [in] */ ArrayOf<Int32>* rules)
{
}

void RelativeLayout::DependencyGraph::PrintViewId(
    /* [in] */ IResources* resources,
    /* [in] */ IView* view)
{
}

void RelativeLayout::DependencyGraph::AppendViewId(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node,
    /* [in] */ StringBuffer& buffer)
{
    Int32 id = ((View*)node->mView.Get())->GetId();
    if (id != View_NO_ID) {
        String name;
        resources->GetResourceEntryName(id, &name);
        buffer += name;
        String::Free(name);
    }
    else {
        buffer += "NO_ID";
    }
}

void RelativeLayout::DependencyGraph::PrintNode(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node)
{
}

void RelativeLayout::DependencyGraph::Printdependents(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node,
    /* [in] */ StringBuffer& buffer)
{
}

void RelativeLayout::DependencyGraph::Node::SetNextPoolable(
    /* [in] */ RelativeLayout::DependencyGraph::Node* element)
{
    mNext = element;
}

RelativeLayout::DependencyGraph::Node*
RelativeLayout::DependencyGraph::Node::GetNextPoolable()
{
    return mNext;
}

RelativeLayout::DependencyGraph::Node*
RelativeLayout::DependencyGraph::Node::Acquire(
    /* [in] */ IView* view)
{
//    Node* node = sPool.acquire();
    Node* node = new Node();
    node->mView = view;
//
//    return node;

    return node;
}

void RelativeLayout::DependencyGraph::Node::Release()
{
    mView = NULL;
    mDependents.Clear();
    mDependencies.Clear();

//    sPool.release(this);
}

