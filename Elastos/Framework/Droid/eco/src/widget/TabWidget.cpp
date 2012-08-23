
#include "widget/TabWidget.h"
#include "content/CApplicationInfo.h"
#include <elastos/Math.h>
#include "widget/CLinearLayoutLayoutParams.h"
#include "widget/CImageView.h"

static Int32 R_Styleable_TabWidget[] = {
    0x01010129, 0x010102bb, 0x010102bc, 0x010102bd
};

static const Int32 R_Attr_TabWidgetStyle = 0x01010083;

static const Int32 R_Styleable_TabWidget_Divider = 0;
static const Int32 R_Styleable_TabWidget_TabStripLeft = 1;
static const Int32 R_Styleable_TabWidget_TabStripRight = 2;
static const Int32 R_Styleable_TabWidget_TabStripEnabled = 3;

static const Int32 R_Drawable_Tab_bottom_left = 0x01080302;
static const Int32 R_Drawable_Tab_bottom_left_v4 = 0x01080303;
static const Int32 R_Drawable_Tab_bottom_right = 0x01080304;
static const Int32 R_Drawable_Tab_bottom_right_v4 = 0x01080305;

TabWidget::TabWidget(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle) : LinearLayout(context, attrs)
{
    AutoPtr<ITypedArray> a;
    
    context->ObtainStyledAttributesEx3(attrs, ArrayOf<Int32>(R_Styleable_TabWidget, 4),
                defStyle, 0, (ITypedArray**)&a);

    a->GetBoolean(R_Styleable_TabWidget_TabStripEnabled, TRUE, &mDrawBottomStrips);
    a->GetDrawable(R_Styleable_TabWidget_Divider, (IDrawable**)&mDividerDrawable);
    a->GetDrawable(R_Styleable_TabWidget_TabStripLeft, (IDrawable**)&mLeftStrip);
    a->GetDrawable(R_Styleable_TabWidget_TabStripRight, (IDrawable**)&mRightStrip);

    a->Recycle();

    InitTabWidget();
}

void TabWidget::OnSizeChanged(
    /* [in] */ Int32 w, 
    /* [in] */ Int32 h, 
    /* [in] */ Int32 oldw, 
    /* [in] */ Int32 oldh)
{
    mStripMoved = TRUE;
    LinearLayout::OnSizeChanged(w, h, oldw, oldh);
}

Int32 TabWidget::GetChildDrawingOrder(
    /* [in] */ Int32 childCount, 
    /* [in] */ Int32 i)
{
    // Always draw the selected tab last, so that drop shadows are drawn
    // in the correct z-order.
    if (i == childCount - 1) {
        return mSelectedTab;
    } else if (i >= mSelectedTab) {
        return i + 1;
    } else {
        return i;
    }
}

void TabWidget::InitTabWidget()
{
    SetOrientation(LinearLayout::HORIZONTAL);
    mGroupFlags |= FLAG_USE_CHILD_DRAWING_ORDER;

    AutoPtr<IContext> context = mContext;
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    
    //AutoPtr<IApplicationInfo> ai;
    //context->GetApplicationInfo((IApplication**)&ai);

    if (/*((CApplicationInfo*)ai.Get())->mTargetSdkVersion <= Build_VERSION_CODES_DONUT*/0) {
        // Donut apps get old color scheme
        if (mLeftStrip.Get() == NULL) {
            resources->GetDrawable(
                    R_Drawable_Tab_bottom_left_v4, (IDrawable**)&mLeftStrip);
        }
        if (mRightStrip.Get() == NULL) {
            resources->GetDrawable(
                    R_Drawable_Tab_bottom_right_v4, (IDrawable**)&mRightStrip);
        }
    } else {
        // Use modern color scheme for Eclair and beyond
        if (mLeftStrip.Get() == NULL) {
            resources->GetDrawable(R_Drawable_Tab_bottom_left, (IDrawable**)&mLeftStrip);
        }
        if (mRightStrip.Get() == NULL) {
            resources->GetDrawable(R_Drawable_Tab_bottom_right, (IDrawable**)&mRightStrip);
        }
    }

    // Deal with focus, as we don't want the focus to go by default
    // to a tab other than the current tab
    SetFocusable(TRUE);
    SetOnFocusChangeListener((IViewOnFocusChangeListener*)this->Probe(EIID_IViewOnFocusChangeListener));
}

/**
 * Returns the tab indicator view at the given index.
 *
 * @param index the zero-based index of the tab indicator view to return
 * @return the tab indicator view at the given index
 */
AutoPtr<IView> TabWidget::GetChildTabViewAt(
    /* [in] */ Int32 index)
{
    // If we are using dividers, then instead of tab views at 0, 1, 2, ...
    // we have tab views at 0, 2, 4, ...
    if (mDividerDrawable != NULL) {
        index *= 2;
    }
    return GetChildAt(index);
}

/**
 * Returns the number of tab indicator views.
 * @return the number of tab indicator views.
 */
Int32 TabWidget::GetTabCount()
{
    Int32 children = GetChildCount();

    // If we have dividers, then we will always have an odd number of
    // children: 1, 3, 5, ... and we want to convert that sequence to
    // this: 1, 2, 3, ...
    if (mDividerDrawable != NULL) {
        children = (children + 1) / 2;
    }
    return children;
}

/**
 * Sets the drawable to use as a divider between the tab indicators.
 * @param drawable the divider drawable
 */
ECode TabWidget::SetDividerDrawable(
    /* [in] */ IDrawable* drawable)
{
    mDividerDrawable = drawable;
    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * Sets the drawable to use as a divider between the tab indicators.
 * @param resId the resource identifier of the drawable to use as a
 * divider.
 */
ECode TabWidget::SetDividerDrawable(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);

    resources->GetDrawable(resId, (IDrawable**)&mDividerDrawable);
    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * Sets the drawable to use as the left part of the strip below the
 * tab indicators.
 * @param drawable the left strip drawable
 */
ECode TabWidget::SetLeftStripDrawable(
    /* [in] */ IDrawable* drawable)
{
    mLeftStrip = drawable;
    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * Sets the drawable to use as the left part of the strip below the
 * tab indicators.
 * @param resId the resource identifier of the drawable to use as the
 * left strip drawable
 */
ECode TabWidget::SetLeftStripDrawable(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);

    resources->GetDrawable(resId, (IDrawable**)&mLeftStrip);
    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * Sets the drawable to use as the right part of the strip below the
 * tab indicators.
 * @param drawable the right strip drawable
 */
ECode TabWidget::SetRightStripDrawable(
    /* [in] */ IDrawable* drawable)
{
    mRightStrip = drawable;
    RequestLayout();
    Invalidate();   

    return NOERROR;
}

/**
 * Sets the drawable to use as the right part of the strip below the
 * tab indicators.
 * @param resId the resource identifier of the drawable to use as the
 * right strip drawable
 */
ECode TabWidget::SetRightStripDrawable(
    /* [in] */ Int32 resId) 
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);

    resources->GetDrawable(resId, (IDrawable**)&mRightStrip);

    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * Controls whether the bottom strips on the tab indicators are drawn or
 * not.  The default is to draw them.  If the user specifies a custom
 * view for the tab indicators, then the TabHost class calls this method
 * to disable drawing of the bottom strips.
 * @param stripEnabled TRUE if the bottom strips should be drawn.
 */
ECode TabWidget::SetStripEnabled(
    /* [in] */ Boolean stripEnabled)
{
    mDrawBottomStrips = stripEnabled;
    Invalidate();

    return NOERROR;
}

/**
 * Indicates whether the bottom strips on the tab indicators are drawn
 * or not.
 */
Boolean TabWidget::IsStripEnabled()
{
    return mDrawBottomStrips;
}

ECode TabWidget::ChildDrawableStateChanged(
    /* [in] */ IView* child)
{
    if (GetTabCount() > 0 && child == GetChildTabViewAt(mSelectedTab)) {
        // To make sure that the bottom strip is redrawn
        Invalidate();
    }
    LinearLayout::ChildDrawableStateChanged(child);

    return NOERROR;
}

void TabWidget::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    LinearLayout::DispatchDraw(canvas);

    // Do nothing if there are no tabs.
    if (GetTabCount() == 0) return;

    // If the user specified a custom view for the tab indicators, then
    // do not draw the bottom strips.
    if (!mDrawBottomStrips) {
        // Skip drawing the bottom strips.
        return;
    }

    AutoPtr<IView> selectedChild = GetChildTabViewAt(mSelectedTab);

    AutoPtr<IDrawable> leftStrip = mLeftStrip;
    AutoPtr<IDrawable> rightStrip = mRightStrip;

    ArrayOf<Int32>* drawableState;
    selectedChild->GetDrawableState(&drawableState);

    Boolean res;
    leftStrip->SetState(drawableState, &res);
    rightStrip->SetState(drawableState, &res);

    if (mStripMoved) {
        AutoPtr<CRect> bounds = (CRect*)mBounds.Get();
        selectedChild->GetLeft(&bounds->mLeft);
        selectedChild->GetRight(&bounds->mRight);
        Int32 myHeight = GetHeight();

        Int32 w, h;
        leftStrip->GetIntrinsicWidth(&w);
        leftStrip->GetIntrinsicHeight(&h);
        leftStrip->SetBounds(Math::Min(0, bounds->mLeft - w),
                myHeight - h, bounds->mLeft, myHeight);

        rightStrip->GetIntrinsicWidth(&w);
        rightStrip->GetIntrinsicHeight(&h);
        rightStrip->SetBounds(bounds->mRight, myHeight - h,
            Math::Max(GetWidth(), bounds->mRight + w), myHeight);
        mStripMoved = FALSE;
    }

    leftStrip->Draw(canvas);
    rightStrip->Draw(canvas);
}

/**
 * Sets the current tab.
 * This method is used to bring a tab to the front of the Widget,
 * and is used to post to the rest of the UI that a different tab
 * has been brought to the foreground.
 *
 * Note, this is separate from the traditional "focus" that is
 * employed from the view logic.
 *
 * For instance, if we have a list in a tabbed view, a user may be
 * navigating up and down the list, moving the UI focus (orange
 * highlighting) through the list items.  The cursor movement does
 * not effect the "selected" tab though, because what is being
 * scrolled through is all on the same tab.  The selected tab only
 * changes when we navigate between tabs (moving from the list view
 * to the next tabbed view, in this example).
 *
 * To move both the focus AND the selected tab at once, please use
 * {@link #setCurrentTab}. Normally, the view logic takes care of
 * adjusting the focus, so unless you're circumventing the UI,
 * you'll probably just focus your interest here.
 *
 *  @param index The tab that you want to indicate as the selected
 *  tab (tab brought to the front of the widget)
 *
 *  @see #focusCurrentTab
 */
ECode TabWidget::SetCurrentTab(
    /* [in] */ Int32 index)
{
    if (index < 0 || index >= GetTabCount()) {
        return NOERROR;
    }

    GetChildTabViewAt(mSelectedTab)->SetSelected(FALSE);
    mSelectedTab = index;
    GetChildTabViewAt(mSelectedTab)->SetSelected(TRUE);
    mStripMoved = TRUE;

    return NOERROR;
}

/**
 * Sets the current tab and focuses the UI on it.
 * This method makes sure that the focused tab matches the selected
 * tab, normally at {@link #setCurrentTab}.  Normally this would not
 * be an issue if we go through the UI, since the UI is responsible
 * for calling TabWidget.onFocusChanged(), but in the case where we
 * are selecting the tab programmatically, we'll need to make sure
 * focus keeps up.
 *
 *  @param index The tab that you want focused (highlighted in orange)
 *  and selected (tab brought to the front of the widget)
 *
 *  @see #setCurrentTab
 */
ECode TabWidget::FocusCurrentTab(
    /* [in] */ Int32 index) 
{
    Int32 oldTab = mSelectedTab;

    // set the tab
    SetCurrentTab(index);

    // change the focus if applicable.
    if (oldTab != index) {
        Boolean res;
        GetChildTabViewAt(index)->RequestFocus(&res);
    }

    return NOERROR;
}

ECode TabWidget::SetEnabled(
    /* [in] */ Boolean enabled)
{
    LinearLayout::SetEnabled(enabled);
    Int32 count = GetTabCount();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildTabViewAt(i);
        child->SetEnabled(enabled);
    }

    return NOERROR;
}

ECode TabWidget::AddView(
    /* [in] */ IView* child)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp.Get() == NULL) {
        AutoPtr<ILinearLayoutLayoutParams> lp;
        CLinearLayoutLayoutParams::New(0, ViewGroupLayoutParams_MATCH_PARENT, 1.0f, (ILinearLayoutLayoutParams**)&lp);
        lp->SetMargins(0, 0, 0, 0);
        child->SetLayoutParams(lp);
    }

    // Ensure you can navigate to the tab with the keyboard, and you can touch it
    child->SetFocusable(TRUE);
    child->SetClickable(TRUE);

    // If we have dividers between the tabs and we already have at least one
    // tab, then add a divider before adding the next tab.
    if (mDividerDrawable != NULL && GetTabCount() > 0) {
        AutoPtr<IImageView> divider;
        CImageView::New(mContext, (IImageView**)&divider);
        AutoPtr<ILinearLayoutLayoutParams> lp;
        Int32 w;
        mDividerDrawable->GetIntrinsicWidth(&w);
        CLinearLayoutLayoutParams::New(w, ViewGroupLayoutParams_MATCH_PARENT, (ILinearLayoutLayoutParams**)&lp);
        lp->SetMargins(0, 0, 0, 0);
        divider->SetLayoutParams(lp);
        divider->SetBackgroundDrawable(mDividerDrawable);
        LinearLayout::AddView(divider);
    }
    LinearLayout::AddView(child);

    // TODO: detect this via geometry with a tabwidget listener rather
    // than potentially interfere with the view's listener
    //child->SetOnClickListener(new TabClickListener(GetTabCount() - 1));
    child->SetOnFocusChangeListener((IViewOnFocusChangeListener*)this->Probe(EIID_IViewOnFocusChangeListener));

    return NOERROR;
}

/**
 * Provides a way for {@link TabHost} to be notified that the user clicked on a tab indicator.
 */
void TabWidget::SetTabSelectionListener(
    /* [in] */ ITabWidgetOnTabSelectionChanged* listener)
{
    mSelectionChangedListener = listener;
}

ECode TabWidget::OnFocusChange(
    /* [in] */ IView* v, 
    /* [in] */ Boolean hasFocus)
{
    if (v == (IView*)this->Probe(EIID_IView) && hasFocus && GetTabCount() > 0) {
        Boolean res;
        GetChildTabViewAt(mSelectedTab)->RequestFocus(&res);
        return NOERROR;
    }

    if (hasFocus) {
        Int32 i = 0;
        Int32 numTabs = GetTabCount();
        while (i < numTabs) {
            if (GetChildTabViewAt(i).Get() == v) {
                SetCurrentTab(i);
                mSelectionChangedListener->OnTabSelectionChanged(i, FALSE);
                break;
            }
            i++;
        }
    }

    return NOERROR;
}

TabWidget::TabClickListener::TabClickListener(
    /* [in] */ Int32 tabIndex,
    /* [in] */ TabWidget* owner)
{
    mTabIndex = tabIndex;
    mOwner = owner;
}

ECode TabWidget::TabClickListener::OnClick(
     /* [in] */ IView* v)
{
    return mOwner->mSelectionChangedListener->OnTabSelectionChanged(mTabIndex, TRUE);
}

