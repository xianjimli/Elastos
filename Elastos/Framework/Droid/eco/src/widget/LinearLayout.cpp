
#include "widget/LinearLayout.h"
#include <elastos/Math.h>

using namespace Elastos::System;

const Int32 LinearLayout::HORIZONTAL;
const Int32 LinearLayout::VERTICAL;
const Int32 LinearLayout::VERTICAL_GRAVITY_COUNT;
const Int32 LinearLayout::INDEX_CENTER_VERTICAL;
const Int32 LinearLayout::INDEX_TOP;
const Int32 LinearLayout::INDEX_BOTTOM;
const Int32 LinearLayout::INDEX_FILL;

LinearLayout::LinearLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
{
}

LinearLayout::LinearLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
}

/**
 * <p>Indicates whether widgets contained within this layout are aligned
 * on their baseline or not.</p>
 *
 * @return TRUE when widgets are baseline-aligned, FALSE otherwise
 */
Boolean LinearLayout::IsBaselineAligned()
{
    return mBaselineAligned;
}

/**
 * <p>Defines whether widgets contained in this layout are
 * baseline-aligned or not.</p>
 *
 * @param baselineAligned TRUE to align widgets on their baseline,
 *         FALSE otherwise
 *
 * @attr ref android.R.styleable#LinearLayout_baselineAligned
 */
ECode LinearLayout::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    mBaselineAligned = baselineAligned;
    return NOERROR;
}

ECode LinearLayout::GetBaseline(
    /* [out] */ Int32* baseline)
{
    assert(baseline != NULL);

    if (mBaselineAlignedChildIndex < 0) {
        return ViewGroup::GetBaseline(baseline);
    }

    if (GetChildCount() <= mBaselineAlignedChildIndex) {
//        throw new RuntimeException("mBaselineAlignedChildIndex of LinearLayout "
//                + "set to an index that is out of bounds.");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IView> child = GetChildAt(mBaselineAlignedChildIndex);
    Int32 childBaseline;
    child->GetBaseline(&childBaseline);

    if (childBaseline == -1) {
        if (mBaselineAlignedChildIndex == 0) {
            // this is just the default case, safe to return -1
            *baseline = -1;
            return NOERROR;
        }
        // the user picked an index that points to something that doesn't
        // know how to calculate its baseline.
//        throw new RuntimeException("mBaselineAlignedChildIndex of LinearLayout "
//                + "points to a View that doesn't know how to get its baseline.");
        return E_RUNTIME_EXCEPTION;
    }

    // TODO: This should try to take into account the virtual offsets
    // (See getNextLocationOffset and getLocationOffset)
    // We should add to childTop:
    // sum([getNextLocationOffset(getChildAt(i)) / i < mBaselineAlignedChildIndex])
    // and also add:
    // getLocationOffset(child)
    Int32 childTop = mBaselineChildTop;

    if (mOrientation == VERTICAL) {
        Int32 majorGravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;
        if (majorGravity != Gravity_TOP) {
            switch (majorGravity) {
                case Gravity_BOTTOM:
                    childTop = mBottom - mTop - mPaddingBottom - mTotalLength;
                    break;

                case Gravity_CENTER_VERTICAL:
                    childTop += ((mBottom - mTop - mPaddingTop - mPaddingBottom) -
                            mTotalLength) / 2;
                    break;
            }
        }
    }

    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    *baseline = childTop + /*((CViewGroupLayoutParams*)lp)->topMargin +*/ childBaseline;
    return NOERROR;
}

/**
 * @return The index of the child that will be used if this layout is
 *   part of a larger layout that is baseline aligned, or -1 if none has
 *   been set.
 */
Int32 LinearLayout::GetBaselineAlignedChildIndex()
{
    return mBaselineAlignedChildIndex;
}

/**
 * @param i The index of the child that will be used if this layout is
 *          part of a larger layout that is baseline aligned.
 *
 * @attr ref android.R.styleable#LinearLayout_baselineAlignedChildIndex
 */
ECode LinearLayout::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 i)
{
    if ((i < 0) || (i >= GetChildCount())) {
//        throw new IllegalArgumentException("base aligned child index out "
//                + "of range (0, " + getChildCount() + ")");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBaselineAlignedChildIndex = i;

    return NOERROR;
}

/**
 * <p>Returns the view at the specified index. This method can be overriden
 * to take into account virtual children. Refer to
 * {@link android.widget.TableLayout} and {@link android.widget.TableRow}
 * for an example.</p>
 *
 * @param index the child's index
 * @return the child at the specified index
 */
AutoPtr<IView> LinearLayout::GetVirtualChildAt(
    /* [in] */ Int32 index)
{
    return GetChildAt(index);
}

/**
 * <p>Returns the virtual number of children. This number might be different
 * than the actual number of children if the layout can hold virtual
 * children. Refer to
 * {@link android.widget.TableLayout} and {@link android.widget.TableRow}
 * for an example.</p>
 *
 * @return the virtual number of children
 */
Int32 LinearLayout::GetVirtualChildCount()
{
    return GetChildCount();
}

/**
 * Returns the desired weights sum.
 *
 * @return A number greater than 0.0f if the weight sum is defined, or
 *         a number lower than or equals to 0.0f if not weight sum is
 *         to be used.
 */
Float LinearLayout::GetWeightSum()
{
    return mWeightSum;
}

/**
 * Defines the desired weights sum. If unspecified the weights sum is computed
 * at layout time by adding the layout_weight of each child.
 *
 * This can be used for instance to give a single child 50% of the total
 * available space by giving it a layout_weight of 0.5 and setting the
 * weightSum to 1.0.
 *
 * @param weightSum a number greater than 0.0f, or a number lower than or equals
 *        to 0.0f if the weight sum should be computed from the children's
 *        layout_weight
 */
ECode LinearLayout::SetWeightSum(
    /* [in] */ Float weightSum)
{
    mWeightSum = Math::Max(0.0f, weightSum);
    return NOERROR;
}

void LinearLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mOrientation == VERTICAL) {
        MeasureVertical(widthMeasureSpec, heightMeasureSpec);
    }
    else {
        MeasureHorizontal(widthMeasureSpec, heightMeasureSpec);
    }
}

/**
 * Measures the children when the orientation of this LinearLayout is set
 * to {@link #VERTICAL}.
 *
 * @param widthMeasureSpec Horizontal space requirements as imposed by the parent.
 * @param heightMeasureSpec Vertical space requirements as imposed by the parent.
 *
 * @see #getOrientation()
 * @see #setOrientation(Int32)
 * @see #onMeasure(Int32, Int32)
 */
ECode LinearLayout::MeasureVertical(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    mTotalLength = 0;
    Int32 maxWidth = 0;
    Int32 alternativeMaxWidth = 0;
    Int32 weightedMaxWidth = 0;
    Boolean allFillParent = TRUE;
    Float totalWeight = 0;

    Int32 count = GetVirtualChildCount();

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);

    Boolean matchWidth = FALSE;

    Int32 baselineChildIndex = mBaselineAlignedChildIndex;
    Boolean useLargestChild = mUseLargestChild;

    Int32 largestChildHeight = Math::INT32_MIN_VALUE;

    // See how tall everyone is. Also remember max width.
    for (Int32 i = 0; i < count; ++i) {
        AutoPtr<IView> child = GetVirtualChildAt(i);

        if (child == NULL) {
            mTotalLength += MeasureNullChild(i);
            continue;
        }

        Int32 visibility;
        child->GetVisibility(&visibility);

        if (visibility == View_GONE) {
            i += GetChildrenSkipCount(child, i);
            continue;
        }

        AutoPtr<IViewGroupLayoutParams> lv;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

        ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();

        Float weight;
        lp->GetWeight(&weight);

        Int32 width, height;
        lp->GetWidth(&width);
        lp->GetHeight(&height);

        Int32 top, left, right, bottom;
        lp->GetMargins(&top, &left, &right, &bottom);

        totalWeight += weight;

        if (heightMode == MeasureSpec::EXACTLY && height == 0 && weight > 0) {
            // Optimization: don't bother measuring children who are going to use
            // leftover space. These views will get measured again down below if
            // there is any leftover space.
            Int32 totalLength = mTotalLength;
            mTotalLength = Math::Max(totalLength, totalLength + top + bottom);
        }
        else {
            Int32 oldHeight = Math::INT32_MIN_VALUE;

            if (height == 0 && weight > 0) {
                // heightMode is either UNSPECIFIED or AT_MOST, and this
                // child wanted to stretch to fill available space.
                // Translate that to WRAP_CONTENT so that it does not end up
                // with a height of 0
                oldHeight = 0;
                height = LinearLayoutLayoutParams::WRAP_CONTENT;
            }

            // Determine how big this child would like to be. If this or
            // previous children have given a weight, then we allow it to
            // use all available space (and we will shrink things later
            // if needed).
            MeasureChildBeforeLayout(
                    child, i, widthMeasureSpec, 0, heightMeasureSpec,
                    totalWeight == 0 ? mTotalLength : 0);

            if (oldHeight != Math::INT32_MIN_VALUE) {
                height = oldHeight;
            }

            Int32 childHeight;
            child->GetMeasuredHeight(&childHeight);

            Int32 totalLength = mTotalLength;
            mTotalLength = Math::Max(totalLength, totalLength + childHeight + top +
                    bottom + GetNextLocationOffset(child));

            if (useLargestChild) {
                largestChildHeight = Math::Max(childHeight, largestChildHeight);
            }
        }

        /**
          * If applicable, compute the additional offset to the child's baseline
          * we'll need later when asked {@link #getBaseline}.
          */
        if ((baselineChildIndex >= 0) && (baselineChildIndex == i + 1)) {
            mBaselineChildTop = mTotalLength;
        }

        // if we are trying to use a child index for our baseline, the above
        // book keeping only works if there are no children above it with
        // weight.  fail fast to aid the developer.
        if (i < baselineChildIndex && weight > 0) {
            /*throw new RuntimeException("A child of LinearLayout with index "
                    + "less than mBaselineAlignedChildIndex has weight > 0, which "
                    + "won't work.  Either remove the weight, or don't set "
                    + "mBaselineAlignedChildIndex.");*/
        }

        Boolean matchWidthLocally = FALSE;
        if (widthMode != MeasureSpec::EXACTLY && width == LinearLayoutLayoutParams::MATCH_PARENT) {
            // The width of the linear layout will scale, and at least one
            // child said it wanted to match our width. Set a flag
            // indicating that we need to remeasure at least that view when
            // we know our width.
            matchWidth = TRUE;
            matchWidthLocally = TRUE;
        }

        Int32 margin = left + right;
        Int32 measuredWidth;
        child->GetMeasuredWidth(&measuredWidth);
        measuredWidth += margin;

        maxWidth = Math::Max(maxWidth, measuredWidth);

        allFillParent = allFillParent && width == LinearLayoutLayoutParams::MATCH_PARENT;
        if (weight > 0) {
            /*
             * Widths of weighted Views are bogus if we end up
             * remeasuring, so keep them separate.
             */
            weightedMaxWidth = Math::Max(weightedMaxWidth,
                    matchWidthLocally ? margin : measuredWidth);
        }
        else {
            alternativeMaxWidth = Math::Max(alternativeMaxWidth,
                    matchWidthLocally ? margin : measuredWidth);
        }

        i += GetChildrenSkipCount(child, i);
    }

    if (useLargestChild && heightMode == MeasureSpec::AT_MOST) {
        mTotalLength = 0;

        for (Int32 i = 0; i < count; ++i) {
            AutoPtr<IView> child = GetVirtualChildAt(i);

            if (child == NULL) {
                mTotalLength += MeasureNullChild(i);
                continue;
            }

            Int32 v;
            child->GetVisibility(&v);
            if (v == View::GONE) {
                i += GetChildrenSkipCount(child, i);
                continue;
            }

            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();
            Int32 top, left, right, bottom;
            lp->GetMargins(&top, &left, &right, &bottom);

            // Account for negative margins
            Int32 totalLength = mTotalLength;
            mTotalLength = Math::Max(totalLength, totalLength + largestChildHeight +
                    top + bottom + GetNextLocationOffset(child));
        }
    }

    // Add in our padding
    mTotalLength += mPaddingTop + mPaddingBottom;

    Int32 heightSize = mTotalLength;

    // Check against our minimum height
    heightSize = Math::Max(heightSize, GetSuggestedMinimumHeight());

    // Reconcile our calculated size with the heightMeasureSpec
    heightSize = ResolveSize(heightSize, heightMeasureSpec);

    // Either expand children with weight to take up available space or
    // shrink them if they extend beyond our current bounds
    Int32 delta = heightSize - mTotalLength;
    if (delta != 0 && totalWeight > 0.0f) {
        Float weightSum = mWeightSum > 0.0f ? mWeightSum : totalWeight;

        mTotalLength = 0;

        for (Int32 i = 0; i < count; ++i) {
            AutoPtr<IView> child = GetVirtualChildAt(i);

            Int32 v;
            child->GetVisibility(&v);
            if (v == View::GONE) {
                continue;
            }

            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();
            Float weight;
            lp->GetWeight(&weight);
            Int32 left, top, right, bottom;
            lp->GetMargins(&left, &top, &right, &bottom);
            Int32 width, height;
            lp->GetWidth(&width);
            lp->GetHeight(&height);

            Float childExtra = weight;
            if (childExtra > 0) {
                // Child said it could absorb extra space -- give him his share
                Int32 share = (Int32) (childExtra * delta / weightSum);
                weightSum -= childExtra;
                delta -= share;

                Int32 childWidthMeasureSpec = GetChildMeasureSpec(widthMeasureSpec,
                        mPaddingLeft + mPaddingRight +
                                left + right, width);

                // TODO: Use a field like lp.isMeasured to figure out if this
                // child has been previously measured
                if ((height != 0) || (heightMode != MeasureSpec::EXACTLY)) {
                    // child was measured once already above...
                    // base new measurement on stored values
                    Int32 childHeight;
                    child->GetMeasuredHeight(&childHeight);
                    childHeight += share;
                    if (childHeight < 0) {
                        childHeight = 0;
                    }

                    child->Measure(childWidthMeasureSpec,
                            MeasureSpec::MakeMeasureSpec(childHeight, MeasureSpec::EXACTLY));
                }
                else {
                    // child was skipped in the loop above.
                    // Measure for this first time here
                    child->Measure(childWidthMeasureSpec,
                            MeasureSpec::MakeMeasureSpec(share > 0 ? share : 0,
                                    MeasureSpec::EXACTLY));
                }
            }

            Int32 margin =  left + right;
            Int32 measuredWidth;
            child->GetMeasuredWidth(&measuredWidth);
            measuredWidth += margin;
            maxWidth = Math::Max(maxWidth, measuredWidth);

            Boolean matchWidthLocally = widthMode != MeasureSpec::EXACTLY &&
                    width == LinearLayoutLayoutParams::MATCH_PARENT;

            alternativeMaxWidth = Math::Max(alternativeMaxWidth,
                    matchWidthLocally ? margin : measuredWidth);

            allFillParent = allFillParent && width == LinearLayoutLayoutParams::MATCH_PARENT;

            Int32 totalLength = mTotalLength;

            Int32 h;
            child->GetMeasuredHeight(&h);
            mTotalLength = Math::Max(totalLength, totalLength + h +
                    top + bottom + GetNextLocationOffset(child));
        }

        // Add in our padding
        mTotalLength += mPaddingTop + mPaddingBottom;
        // TODO: Should we recompute the heightSpec based on the new total length?
    }
    else {
        alternativeMaxWidth = Math::Max(alternativeMaxWidth,
                                        weightedMaxWidth);
    }

    if (!allFillParent && widthMode != MeasureSpec::EXACTLY) {
        maxWidth = alternativeMaxWidth;
    }

    maxWidth += mPaddingLeft + mPaddingRight;

    // Check against our minimum width
    maxWidth = Math::Max(maxWidth, GetSuggestedMinimumWidth());

    SetMeasuredDimension(ResolveSize(maxWidth, widthMeasureSpec), heightSize);

    if (matchWidth) {
        ForceUniformWidth(count, heightMeasureSpec);
    }

    return NOERROR;
}

void LinearLayout::ForceUniformWidth(
    /* [in] */ Int32 count,
    /* [in] */ Int32 heightMeasureSpec)
{
    // Pretend that the linear layout has an exact size.
    Int32 uniformMeasureSpec = MeasureSpec::MakeMeasureSpec(GetMeasuredWidth(),
            MeasureSpec::EXACTLY);
    for (Int32 i = 0; i< count; ++i) {
        AutoPtr<IView> child = GetVirtualChildAt(i);
        Int32 v;
        child->GetVisibility(&v);
        if (v != GONE) {

            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();

            Int32 width, height;
            lp->GetWidth(&width);
            lp->GetHeight(&height);
            if (width == LinearLayoutLayoutParams::MATCH_PARENT) {
                // Temporarily force children to reuse their old measured height
                // FIXME: this may not be right for something like wrapping text?
                Int32 oldHeight = height;
                child->GetMeasuredHeight(&height);

                // Remeasue with new dimensions
                MeasureChildWithMargins(child, uniformMeasureSpec, 0, heightMeasureSpec, 0);
                height = oldHeight;
            }
        }
    }
}

/**
 * Measures the children when the orientation of this LinearLayout is set
 * to {@link #HORIZONTAL}.
 *
 * @param widthMeasureSpec Horizontal space requirements as imposed by the parent.
 * @param heightMeasureSpec Vertical space requirements as imposed by the parent.
 *
 * @see #getOrientation()
 * @see #setOrientation(Int32)
 * @see #onMeasure(Int32, Int32)
 */
ECode LinearLayout::MeasureHorizontal(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    mTotalLength = 0;
    Int32 maxHeight = 0;
    Int32 alternativeMaxHeight = 0;
    Int32 weightedMaxHeight = 0;
    Boolean allFillParent = TRUE;
    Float totalWeight = 0;

    Int32 count = GetVirtualChildCount();

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);

    Boolean matchHeight = FALSE;

    if (mMaxAscent == NULL || mMaxDescent == NULL) {
        mMaxAscent = new Int32[VERTICAL_GRAVITY_COUNT];
        mMaxDescent = new Int32[VERTICAL_GRAVITY_COUNT];
    }

    Int32* maxAscent = mMaxAscent;
    Int32* maxDescent = mMaxDescent;

    maxAscent[0] = maxAscent[1] = maxAscent[2] = maxAscent[3] = -1;
    maxDescent[0] = maxDescent[1] = maxDescent[2] = maxDescent[3] = -1;

    Boolean baselineAligned = mBaselineAligned;
    Boolean useLargestChild = mUseLargestChild;

    Boolean isExactly = widthMode == MeasureSpec::EXACTLY;

    Int32 largestChildWidth = Math::INT32_MIN_VALUE;

    // See how wide everyone is. Also remember max height.
    for (Int32 i = 0; i < count; ++i) {
        AutoPtr<IView> child = GetVirtualChildAt(i);

        if (child == NULL) {
            mTotalLength += MeasureNullChild(i);
            continue;
        }

        Int32 v;
        child->GetVisibility(&v);
        if (v == GONE) {
            i += GetChildrenSkipCount(child, i);
            continue;
        }

        AutoPtr<IViewGroupLayoutParams> lv;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

        ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();

        Int32 width, height;
        lp->GetWidth(&width);
        lp->GetHeight(&height);

        Float weight;
        lp->GetWeight(&weight);
        totalWeight += weight;

        Int32 left, top, right, bottom;
        lp->GetMargins(&left, &top, &right, &bottom);

        Int32 gravity;
        lp->GetGravity(&gravity);

        if (widthMode == MeasureSpec::EXACTLY && width == 0 && weight > 0) {
            // Optimization: don't bother measuring children who are going to use
            // leftover space. These views will get measured again down below if
            // there is any leftover space.
            if (isExactly) {
                mTotalLength += left + right;
            } else {
                Int32 totalLength = mTotalLength;
                mTotalLength = Math::Max(totalLength, totalLength +
                        left + right);
            }

            // Baseline alignment requires to measure widgets to obtain the
            // baseline offset (in particular for TextViews). The following
            // defeats the optimization mentioned above. Allow the child to
            // use as much space as it wants because we can shrink things
            // later (and re-measure).
            if (baselineAligned) {
                Int32 freeSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);
                child->Measure(freeSpec, freeSpec);
            }
        } else {
            Int32 oldWidth = Math::INT32_MIN_VALUE;

            if (width == 0 && weight > 0) {
                // widthMode is either UNSPECIFIED or AT_MOST, and this
                // child
                // wanted to stretch to fill available space. Translate that to
                // WRAP_CONTENT so that it does not end up with a width of 0
                oldWidth = 0;
                width = LinearLayoutLayoutParams::WRAP_CONTENT;
            }

            // Determine how big this child would like to be. If this or
            // previous children have given a weight, then we allow it to
            // use all available space (and we will shrink things later
            // if needed).
            MeasureChildBeforeLayout(child, i, widthMeasureSpec,
                    totalWeight == 0 ? mTotalLength : 0,
                    heightMeasureSpec, 0);

            if (oldWidth != Math::INT32_MIN_VALUE) {
                width = oldWidth;
            }

            Int32 childWidth;
            child->GetMeasuredWidth(&childWidth);
            if (isExactly) {
                mTotalLength += childWidth + left + right +
                        GetNextLocationOffset(child);
            } else {
                Int32 totalLength = mTotalLength;
                mTotalLength = Math::Max(totalLength, totalLength + childWidth + left +
                        right + GetNextLocationOffset(child));
            }

            if (useLargestChild) {
                largestChildWidth = Math::Max(childWidth, largestChildWidth);
            }
        }

        Boolean matchHeightLocally = FALSE;
        if (heightMode != MeasureSpec::EXACTLY && height == LinearLayoutLayoutParams::MATCH_PARENT) {
            // The height of the linear layout will scale, and at least one
            // child said it wanted to match our height. Set a flag indicating that
            // we need to remeasure at least that view when we know our height.
            matchHeight = TRUE;
            matchHeightLocally = TRUE;
        }

        Int32 margin = top + bottom;
        Int32 childHeight;
        child->GetMeasuredHeight(&childHeight);
        childHeight += margin;

        if (baselineAligned) {
            Int32 childBaseline;
            child->GetBaseline(&childBaseline);
            if (childBaseline != -1) {
                // Translates the child's vertical gravity into an index
                // in the range 0..VERTICAL_GRAVITY_COUNT
                Int32 gravity = (gravity < 0 ? mGravity : gravity)
                        & Gravity_VERTICAL_GRAVITY_MASK;
                Int32 index = ((gravity >> Gravity_AXIS_Y_SHIFT)
                        & ~Gravity_AXIS_SPECIFIED) >> 1;

                maxAscent[index] = Math::Max(maxAscent[index], childBaseline);
                maxDescent[index] = Math::Max(maxDescent[index], childHeight - childBaseline);
            }
        }

        maxHeight = Math::Max(maxHeight, childHeight);

        allFillParent = allFillParent && height == LinearLayoutLayoutParams::MATCH_PARENT;
        if (weight > 0) {
            /*
                * Heights of weighted Views are bogus if we end up
                * remeasuring, so keep them separate.
                */
            weightedMaxHeight = Math::Max(weightedMaxHeight,
                    matchHeightLocally ? margin : childHeight);
        } else {
            alternativeMaxHeight = Math::Max(alternativeMaxHeight,
                    matchHeightLocally ? margin : childHeight);
        }

        i += GetChildrenSkipCount(child, i);
    }

    // Check mMaxAscent[INDEX_TOP] first because it maps to Gravity.TOP,
    // the most common case
    if (maxAscent[INDEX_TOP] != -1 ||
            maxAscent[INDEX_CENTER_VERTICAL] != -1 ||
            maxAscent[INDEX_BOTTOM] != -1 ||
            maxAscent[INDEX_FILL] != -1) {
        Int32 ascent = Math::Max(maxAscent[INDEX_FILL],
                Math::Max(maxAscent[INDEX_CENTER_VERTICAL],
                Math::Max(maxAscent[INDEX_TOP], maxAscent[INDEX_BOTTOM])));
        Int32 descent = Math::Max(maxDescent[INDEX_FILL],
                Math::Max(maxDescent[INDEX_CENTER_VERTICAL],
                Math::Max(maxDescent[INDEX_TOP], maxDescent[INDEX_BOTTOM])));
        maxHeight = Math::Max(maxHeight, ascent + descent);
    }

    if (useLargestChild && widthMode == MeasureSpec::AT_MOST) {
        mTotalLength = 0;

        for (Int32 i = 0; i < count; ++i) {
            AutoPtr<IView> child = GetVirtualChildAt(i);

            if (child == NULL) {
                mTotalLength += MeasureNullChild(i);
                continue;
            }

            Int32 visibility;
            child->GetVisibility(&visibility);
            if (visibility == GONE) {
                i += GetChildrenSkipCount(child, i);
                continue;
            }

            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();


            Int32 left, top, right, bottom;
            lp->GetMargins(&left, &top, &right, &bottom);

            if (isExactly) {
                mTotalLength += largestChildWidth + left + right +
                    GetNextLocationOffset(child);
            } else {
                Int32 totalLength = mTotalLength;
                mTotalLength = Math::Max(totalLength, totalLength + largestChildWidth +
                        left + right + GetNextLocationOffset(child));
            }
        }
    }

    // Add in our padding
    mTotalLength += mPaddingLeft + mPaddingRight;

    Int32 widthSize = mTotalLength;

    // Check against our minimum width
    widthSize = Math::Max(widthSize, GetSuggestedMinimumWidth());

    // Reconcile our calculated size with the widthMeasureSpec
    widthSize = ResolveSize(widthSize, widthMeasureSpec);

    // Either expand children with weight to take up available space or
    // shrink them if they extend beyond our current bounds
    Int32 delta = widthSize - mTotalLength;
    if (delta != 0 && totalWeight > 0.0f) {
        Float weightSum = mWeightSum > 0.0f ? mWeightSum : totalWeight;

        maxAscent[0] = maxAscent[1] = maxAscent[2] = maxAscent[3] = -1;
        maxDescent[0] = maxDescent[1] = maxDescent[2] = maxDescent[3] = -1;
        maxHeight = -1;

        mTotalLength = 0;

        for (Int32 i = 0; i < count; ++i) {
            AutoPtr<IView> child = GetVirtualChildAt(i);

            Int32 visibility;
            child->GetVisibility(&visibility);
            if (child == NULL || visibility == View::GONE) {
                continue;
            }

            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);
            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();

            Int32 width, height;
            lp->GetWidth(&width);
            lp->GetHeight(&height);

            Float weight;
            lp->GetWeight(&weight);
            totalWeight += weight;

            Int32 left, top, right, bottom;
            lp->GetMargins(&left, &top, &right, &bottom);

            Int32 gravity;
            lp->GetGravity(&gravity);

            Float childExtra = weight;
            if (childExtra > 0) {
                // Child said it could absorb extra space -- give him his share
                Int32 share = (Int32) (childExtra * delta / weightSum);
                weightSum -= childExtra;
                delta -= share;

                Int32 childHeightMeasureSpec = GetChildMeasureSpec(
                        heightMeasureSpec,
                        mPaddingTop + mPaddingBottom + top + bottom,
                        height);

                // TODO: Use a field like lp.isMeasured to figure out if this
                // child has been previously measured
                if ((width != 0) || (widthMode != MeasureSpec::EXACTLY)) {
                    // child was measured once already above ... base new measurement
                    // on stored values
                    Int32 result;
                    child->GetMeasuredWidth(&result);
                    Int32 childWidth = result + share;
                    if (childWidth < 0) {
                        childWidth = 0;
                    }

                    child->Measure(
                        MeasureSpec::MakeMeasureSpec(childWidth, MeasureSpec::EXACTLY),
                        childHeightMeasureSpec);
                } else {
                    // child was skipped in the loop above. Measure for this first time here
                    child->Measure(MeasureSpec::MakeMeasureSpec(
                            share > 0 ? share : 0, MeasureSpec::EXACTLY),
                            childHeightMeasureSpec);
                }
            }

            Int32 result;
            child->GetMeasuredWidth(&result);
            if (isExactly) {
                mTotalLength += result + left + right +
                        GetNextLocationOffset(child);
            } else {
                Int32 totalLength = mTotalLength;
                mTotalLength = Math::Max(totalLength, totalLength + result +
                        left + right + GetNextLocationOffset(child));
            }

            Boolean matchHeightLocally = heightMode != MeasureSpec::EXACTLY &&
                    height == LinearLayoutLayoutParams::MATCH_PARENT;

            Int32 margin = top + bottom;
            child->GetMeasuredHeight(&result);
            Int32 childHeight = result + margin;
            maxHeight = Math::Max(maxHeight, childHeight);
            alternativeMaxHeight = Math::Max(alternativeMaxHeight,
                    matchHeightLocally ? margin : childHeight);

            allFillParent = allFillParent && height == LinearLayoutLayoutParams::MATCH_PARENT;

            if (baselineAligned) {
                Int32 childBaseline;
                child->GetBaseline(&childBaseline);
                if (childBaseline != -1) {
                    // Translates the child's vertical gravity into an index in the range 0..2
                    Int32 gravity = (gravity < 0 ? mGravity : gravity)
                            & Gravity_VERTICAL_GRAVITY_MASK;
                    Int32 index = ((gravity >> Gravity_AXIS_Y_SHIFT)
                            & ~Gravity_AXIS_SPECIFIED) >> 1;

                    maxAscent[index] = Math::Max(maxAscent[index], childBaseline);
                    maxDescent[index] = Math::Max(maxDescent[index],
                            childHeight - childBaseline);
                }
            }
        }

        // Add in our padding
        mTotalLength += mPaddingLeft + mPaddingRight;
        // TODO: Should we update widthSize with the new total length?

        // Check mMaxAscent[INDEX_TOP] first because it maps to Gravity.TOP,
        // the most common case
        if (maxAscent[INDEX_TOP] != -1 ||
                maxAscent[INDEX_CENTER_VERTICAL] != -1 ||
                maxAscent[INDEX_BOTTOM] != -1 ||
                maxAscent[INDEX_FILL] != -1) {
            Int32 ascent = Math::Max(maxAscent[INDEX_FILL],
                    Math::Max(maxAscent[INDEX_CENTER_VERTICAL],
                    Math::Max(maxAscent[INDEX_TOP], maxAscent[INDEX_BOTTOM])));
            Int32 descent = Math::Max(maxDescent[INDEX_FILL],
                    Math::Max(maxDescent[INDEX_CENTER_VERTICAL],
                    Math::Max(maxDescent[INDEX_TOP], maxDescent[INDEX_BOTTOM])));
            maxHeight = Math::Max(maxHeight, ascent + descent);
        }
    } else {
        alternativeMaxHeight = Math::Max(alternativeMaxHeight, weightedMaxHeight);
    }

    if (!allFillParent && heightMode != MeasureSpec::EXACTLY) {
        maxHeight = alternativeMaxHeight;
    }

    maxHeight += mPaddingTop + mPaddingBottom;

    // Check against our minimum height
    maxHeight = Math::Max(maxHeight, GetSuggestedMinimumHeight());

    SetMeasuredDimension(widthSize, ResolveSize(maxHeight, heightMeasureSpec));

    if (matchHeight) {
        ForceUniformHeight(count, widthMeasureSpec);
    }

    return NOERROR;
}

void LinearLayout::ForceUniformHeight(
    /* [in] */ Int32 count,
    /* [in] */ Int32 widthMeasureSpec)
{
    // Pretend that the linear layout has an exact size. This is the measured height of
    // ourselves. The measured height should be the max height of the children, changed
    // to accomodate the heightMesureSpec from the parent
    Int32 uniformMeasureSpec = MeasureSpec::MakeMeasureSpec(GetMeasuredHeight(),
            MeasureSpec::EXACTLY);
    for (Int32 i = 0; i < count; ++i) {
        AutoPtr<IView> child = GetVirtualChildAt(i);

        Int32 visibility;
        child->GetVisibility(&visibility);

        if (visibility != GONE) {
            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);
            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();

            Int32 width, height;
            lp->GetWidth(&width);
            lp->GetHeight(&height);

            if (height == LinearLayoutLayoutParams::MATCH_PARENT) {
                // Temporarily force children to reuse their old measured width
                // FIXME: this may not be right for something like wrapping text?
                Int32 oldWidth = width;
                child->GetMeasuredWidth(&width);

                // Remeasure with new dimensions
                MeasureChildWithMargins(child, widthMeasureSpec, 0, uniformMeasureSpec, 0);
                width = oldWidth;
            }
        }
    }
}

/**
 * <p>Returns the number of children to skip after measuring/laying out
 * the specified child.</p>
 *
 * @param child the child after which we want to skip children
 * @param index the index of the child after which we want to skip children
 * @return the number of children to skip, 0 by default
 */
Int32 LinearLayout::GetChildrenSkipCount(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    return 0;
}

/**
 * <p>Returns the size (width or height) that should be occupied by a NULL
 * child.</p>
 *
 * @param childIndex the index of the NULL child
 * @return the width or height of the child depending on the orientation
 */
Int32 LinearLayout::MeasureNullChild(
    /* [in] */ Int32 childIndex)
{
    return 0;
}

/**
 * <p>Measure the child according to the parent's measure specs. This
 * method should be overriden by subclasses to force the sizing of
 * children. This method is called by {@link #measureVertical(Int32, Int32)} and
 * {@link #measureHorizontal(Int32, Int32)}.</p>
 *
 * @param child the child to measure
 * @param childIndex the index of the child in this view
 * @param widthMeasureSpec horizontal space requirements as imposed by the parent
 * @param totalWidth extra space that has been used up by the parent horizontally
 * @param heightMeasureSpec vertical space requirements as imposed by the parent
 * @param totalHeight extra space that has been used up by the parent vertically
 */
void LinearLayout::MeasureChildBeforeLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 totalWidth,
    /* [in] */ Int32 heightMeasureSpec,
    /* [in] */ Int32 totalHeight)
{
    MeasureChildWithMargins(child, widthMeasureSpec, totalWidth,
            heightMeasureSpec, totalHeight);
}

/**
 * <p>Return the location offset of the specified child. This can be used
 * by subclasses to change the location of a given widget.</p>
 *
 * @param child the child for which to obtain the location offset
 * @return the location offset in pixels
 */
Int32 LinearLayout::GetLocationOffset(
    /* [in] */ IView* child)
{
    return 0;
}

/**
 * <p>Return the size offset of the next sibling of the specified child.
 * This can be used by subclasses to change the location of the widget
 * following <code>child</code>.</p>
 *
 * @param child the child whose next sibling will be moved
 * @return the location offset of the next child in pixels
 */
Int32 LinearLayout::GetNextLocationOffset(
    /* [in] */ IView* child)
{
    return 0;
}

void LinearLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    if (mOrientation == VERTICAL) {
        LayoutVertical();
    } else {
        LayoutHorizontal();
    }
}

/**
 * Position the children during a layout pass if the orientation of this
 * LinearLayout is set to {@link #VERTICAL}.
 *
 * @see #getOrientation()
 * @see #setOrientation(Int32)
 * @see #onLayout(Boolean, Int32, Int32, Int32, Int32)
 */
void LinearLayout::LayoutVertical()
{
    Int32 paddingLeft = mPaddingLeft;

    Int32 childTop = mPaddingTop;
    Int32 childLeft;

    // Where right end of child should go
    Int32 width = mRight - mLeft;
    Int32 childRight = width - mPaddingRight;

    // Space available for child
    Int32 childSpace = width - paddingLeft - mPaddingRight;

    Int32 count = GetVirtualChildCount();

    Int32 majorGravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;
    Int32 minorGravity = mGravity & Gravity_HORIZONTAL_GRAVITY_MASK;

    if (majorGravity != Gravity_TOP) {
        switch (majorGravity) {
            case Gravity_BOTTOM:
                // mTotalLength contains the padding already, we add the top
                // padding to compensate
                childTop = mBottom - mTop + mPaddingTop - mTotalLength;
                break;

            case Gravity_CENTER_VERTICAL:
                childTop += ((mBottom - mTop)  - mTotalLength) / 2;
                break;
        }

    }

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetVirtualChildAt(i);
        Int32 visibility;
        child->GetVisibility(&visibility);

        if (child == NULL) {
            childTop += MeasureNullChild(i);
        } else if (visibility != GONE) {
            Int32 childWidth;
            child->GetMeasuredWidth(&childWidth);
            Int32 childHeight;
            child->GetMeasuredHeight(&childHeight);

            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);
            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();

            Int32 width, height;
            lp->GetWidth(&width);
            lp->GetHeight(&height);

            Int32 left, top, right, bottom;
            lp->GetMargins(&left, &top, &right, &bottom);

            Int32 gravity;
            lp->GetGravity(&gravity);

            if (gravity < 0) {
                gravity = minorGravity;
            }

            switch (gravity & Gravity_HORIZONTAL_GRAVITY_MASK) {
                case Gravity_LEFT:
                    childLeft = paddingLeft + left;
                    break;

                case Gravity_CENTER_HORIZONTAL:
                    childLeft = paddingLeft + ((childSpace - childWidth) / 2)
                            + left - right;
                    break;

                case Gravity_RIGHT:
                    childLeft = childRight - childWidth - right;
                    break;
                default:
                    childLeft = paddingLeft;
                    break;
            }


            childTop += top;
            SetChildFrame(child, childLeft, childTop + GetLocationOffset(child),
                    childWidth, childHeight);
            childTop += childHeight + bottom + GetNextLocationOffset(child);

            i += GetChildrenSkipCount(child, i);
        }
    }
}

/**
 * Position the children during a layout pass if the orientation of this
 * LinearLayout is set to {@link #HORIZONTAL}.
 *
 * @see #getOrientation()
 * @see #setOrientation(Int32)
 * @see #onLayout(Boolean, Int32, Int32, Int32, Int32)
 */
void LinearLayout::LayoutHorizontal()
{
    Int32 paddingTop = mPaddingTop;

    Int32 childTop;
    Int32 childLeft = mPaddingLeft;

    // Where bottom of child should go
    Int32 height = mBottom - mTop;
    Int32 childBottom = height - mPaddingBottom;

    // Space available for child
    Int32 childSpace = height - paddingTop - mPaddingBottom;

    Int32 count = GetVirtualChildCount();

    Int32 majorGravity = mGravity & Gravity_HORIZONTAL_GRAVITY_MASK;
    Int32 minorGravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;

    Boolean baselineAligned = mBaselineAligned;

    Int32* maxAscent = mMaxAscent;
    Int32* maxDescent = mMaxDescent;

    if (majorGravity != Gravity_LEFT) {
        switch (majorGravity) {
            case Gravity_RIGHT:
                // mTotalLength contains the padding already, we add the left
                // padding to compensate
                childLeft = mRight - mLeft + mPaddingLeft - mTotalLength;
                break;

            case Gravity_CENTER_HORIZONTAL:
                childLeft += ((mRight - mLeft) - mTotalLength) / 2;
                break;
        }
    }

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetVirtualChildAt(i);
        Int32 visibility;
        child->GetVisibility(&visibility);

        if (child == NULL) {
            childLeft += MeasureNullChild(i);
        } else if (visibility != GONE) {
            Int32 childWidth;
            child->GetMeasuredWidth(&childWidth);
            Int32 childHeight;
            child->GetMeasuredHeight(&childHeight);
            Int32 childBaseline = -1;

            AutoPtr<IViewGroupLayoutParams> lv;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lv);
            ILinearLayoutLayoutParams* lp = (ILinearLayoutLayoutParams*)lv.Get();

            Int32 width, height;
            lp->GetWidth(&width);
            lp->GetHeight(&height);

            Int32 left, top, right, bottom;
            lp->GetMargins(&left, &top, &right, &bottom);

            Int32 gravity;
            lp->GetGravity(&gravity);

            if (baselineAligned && height != LinearLayoutLayoutParams::MATCH_PARENT) {
                child->GetBaseline(&childBaseline);
            }

            if (gravity < 0) {
                gravity = minorGravity;
            }

            switch (gravity & Gravity_VERTICAL_GRAVITY_MASK) {
                case Gravity_TOP:
                    childTop = paddingTop + top;
                    if (childBaseline != -1) {
                        childTop += maxAscent[INDEX_TOP] - childBaseline;
                    }
                    break;

                case Gravity_CENTER_VERTICAL:
                    // Removed support for baseline alignment when layout_gravity or
                    // gravity == center_vertical. See bug #1038483.
                    // Keep the code around if we need to re-enable this feature
                    // if (childBaseline != -1) {
                    //     // Align baselines vertically only if the child is smaller than us
                    //     if (childSpace - childHeight > 0) {
                    //         childTop = paddingTop + (childSpace / 2) - childBaseline;
                    //     } else {
                    //         childTop = paddingTop + (childSpace - childHeight) / 2;
                    //     }
                    // } else {
                    childTop = paddingTop + ((childSpace - childHeight) / 2)
                            + top - bottom;
                    break;

                case Gravity_BOTTOM:
                    childTop = childBottom - childHeight - bottom;
                    if (childBaseline != -1) {
                        Int32 descent;
                        child->GetMeasuredHeight(&descent);
                        descent -= childBaseline;
                        childTop -= (maxDescent[INDEX_BOTTOM] - descent);
                    }
                    break;
                default:
                    childTop = paddingTop;
                    break;
            }

            childLeft += left;
            SetChildFrame(child, childLeft + GetLocationOffset(child), childTop,
                    childWidth, childHeight);
            childLeft += childWidth + right +
                    GetNextLocationOffset(child);

            i += GetChildrenSkipCount(child, i);
        }
    }
}

void LinearLayout::SetChildFrame(
    /* [in] */ IView* child,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    child->Layout(left, top, left + width, top + height);
}

/**
 * Should the layout be a column or a row.
 * @param orientation Pass HORIZONTAL or VERTICAL. Default
 * value is HORIZONTAL.
 *
 * @attr ref android.R.styleable#LinearLayout_orientation
 */
ECode LinearLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    if (mOrientation != orientation) {
        mOrientation = orientation;
        RequestLayout();
    }

    return NOERROR;
}

/**
 * Returns the current orientation.
 *
 * @return either {@link #HORIZONTAL} or {@link #VERTICAL}
 */
Int32 LinearLayout::GetOrientation()
{
    return mOrientation;
}

/**
 * Describes how the child views are positioned. Defaults to GRAVITY_TOP. If
 * this layout has a VERTICAL orientation, this controls where all the child
 * views are placed if there is extra vertical space. If this layout has a
 * HORIZONTAL orientation, this controls the alignment of the children.
 *
 * @param gravity See {@link android.view.Gravity}
 *
 * @attr ref android.R.styleable#LinearLayout_gravity
 */
ECode LinearLayout::SetGravity(
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
        RequestLayout();
    }

    return NOERROR;
}

ECode LinearLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    Int32 gravity = horizontalGravity & Gravity_HORIZONTAL_GRAVITY_MASK;
    if ((mGravity & Gravity_HORIZONTAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~Gravity_HORIZONTAL_GRAVITY_MASK) | gravity;
        RequestLayout();
    }

    return NOERROR;
}

ECode LinearLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    Int32 gravity = verticalGravity & Gravity_VERTICAL_GRAVITY_MASK;
    if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~Gravity_VERTICAL_GRAVITY_MASK) | gravity;
        RequestLayout();
    }

    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> LinearLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IViewGroupLayoutParams> params;
    assert(SUCCEEDED(CLinearLayoutLayoutParams::New(GetContext(), attrs,
        (ILinearLayoutLayoutParams**)&params)));

    return params;
}

/**
 * Returns a set of layout parameters with a width of
 * {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT}
 * and a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}
 * when the layout's orientation is {@link #VERTICAL}. When the orientation is
 * {@link #HORIZONTAL}, the width is set to {@link LayoutParams#WRAP_CONTENT}
 * and the height to {@link LayoutParams#WRAP_CONTENT}.
 */
ECode LinearLayout::GenerateDefaultLayoutParams(
    /* [out] */ ILinearLayoutLayoutParams** lp)
{
    if (mOrientation == HORIZONTAL) {
        return CLinearLayoutLayoutParams::New(
                ViewGroupLayoutParams_WRAP_CONTENT,
                ViewGroupLayoutParams_WRAP_CONTENT,
                lp);
    }
    else if (mOrientation == VERTICAL) {
        return CLinearLayoutLayoutParams::New(
                ViewGroupLayoutParams_MATCH_PARENT,
                ViewGroupLayoutParams_MATCH_PARENT,
                lp);
    }
    *lp = NULL;
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

AutoPtr<IViewGroupLayoutParams> LinearLayout::GenerateLayoutParams(
    /* [int] */ IViewGroupLayoutParams* p)
{
    assert(p);
    AutoPtr<IViewGroupLayoutParams> params;
    assert(SUCCEEDED(CLinearLayoutLayoutParams::New(p,
        (ILinearLayoutLayoutParams**)&params)));

    return params;
}

// Override to allow type-checking of LayoutParams.
Boolean LinearLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return (p && p->Probe(EIID_ILinearLayoutLayoutParams) != NULL);
}

ECode LinearLayout::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context));
    return NOERROR;
}

ECode LinearLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)

{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
    return NOERROR;
}

static Int32 R_Styleable_LinearLayout[] = {
    0x010100af, 0x010100c4, 0x01010126, 0x01010127,
    0x01010128, 0x010102d2
};

ECode LinearLayout::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_LinearLayout, 6)/*com.android.internal.R.styleable.LinearLayout*/,
            (ITypedArray**)&a));

    Int32 index;
    a->GetInt32(1/*com.android.internal.R.styleable.LinearLayout_orientation*/,
            -1, &index);
    if (index >= 0) {
        SetOrientation(index);
    }

    a->GetInt32(0/*com.android.internal.R.styleable.LinearLayout_gravity*/,
            -1, &index);
    if (index >= 0) {
        SetGravity(index);
    }

    Boolean baselineAligned;
    a->GetBoolean(2/*R.styleable.LinearLayout_baselineAligned*/, TRUE, &baselineAligned);
    if (!baselineAligned) {
        SetBaselineAligned(baselineAligned);
    }

    a->GetFloat(4/*R.styleable.LinearLayout_weightSum*/, -1.0f, &mWeightSum);

    a->GetInt32(3/*com.android.internal.R.styleable.LinearLayout_baselineAlignedChildIndex*/,
            -1, &mBaselineAlignedChildIndex);

    // TODO: Better name, add Java APIs, make it public
    a->GetBoolean(5/*R.styleable.LinearLayout_useLargestChild*/, FALSE, &mUseLargestChild);

    a->Recycle();
    return NOERROR;
}
