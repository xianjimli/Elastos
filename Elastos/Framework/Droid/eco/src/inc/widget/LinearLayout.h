#ifndef __LINEARLAYOUT_H__
#define __LINEARLAYOUT_H__

#include "view/ViewGroup.h"
#include "widget/CLinearLayoutLayoutParams.h"

using namespace Elastos;

class LinearLayout : public ViewGroup
{
public:
    LinearLayout(){};

    LinearLayout(
        /* [in] */ IContext* context);

    LinearLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(Boolean) IsBaselineAligned();

    virtual CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    CARAPI GetBaseline(
        /* [out] */ Int32* baseline);

    virtual CARAPI_(Int32) GetBaselineAlignedChildIndex();

    virtual CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 i);

    virtual CARAPI_(AutoPtr<IView>) GetVirtualChildAt(
        /* [in] */ Int32 index);

    virtual CARAPI_(Int32) GetVirtualChildCount();

    virtual CARAPI_(Float) GetWeightSum();

    virtual CARAPI SetWeightSum(
        /* [in] */ Float weightSum);

    virtual CARAPI MeasureVertical(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI MeasureHorizontal(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(Int32) GetChildrenSkipCount(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    virtual CARAPI_(Int32) MeasureNullChild(
        /* [in] */ Int32 childIndex);

    virtual CARAPI_(void) MeasureChildBeforeLayout(
        /* [in] */ IView* child,
        /* [in] */ Int32 childIndex,
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 totalWidth,
        /* [in] */ Int32 heightMeasureSpec,
        /* [in] */ Int32 totalHeight);

    virtual CARAPI_(Int32) GetLocationOffset(
        /* [in] */ IView* child);

    virtual CARAPI_(Int32) GetNextLocationOffset(
        /* [in] */ IView* child);

    virtual CARAPI_(void) LayoutVertical();

    virtual CARAPI_(void) LayoutHorizontal();

    virtual CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    virtual CARAPI GetOrientation();

    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    virtual CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    virtual CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

protected:
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ ILinearLayoutLayoutParams** lp);

    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    CARAPI_(void) ForceUniformWidth(
        /* [in] */ Int32 count,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) ForceUniformHeight(
        /* [in] */ Int32 count,
        /* [in] */ Int32 widthMeasureSpec);

    CARAPI_(void) SetChildFrame(
        /* [in] */ IView* child,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

public:
    static const Int32 HORIZONTAL = 0;
    static const Int32 VERTICAL = 1;

private:
    Boolean mBaselineAligned;

    Int32 mBaselineAlignedChildIndex;

    Int32 mBaselineChildTop;

    Int32 mOrientation;

    Int32 mGravity;

    Int32 mTotalLength;

    Float mWeightSum;

    Boolean mUseLargestChild;

    Int32* mMaxAscent;

    Int32* mMaxDescent;

    static const Int32 VERTICAL_GRAVITY_COUNT = 4;

    static const Int32 INDEX_CENTER_VERTICAL = 0;
    static const Int32 INDEX_TOP = 1;
    static const Int32 INDEX_BOTTOM = 2;
    static const Int32 INDEX_FILL = 3;

};

#endif //__LINEARLAYOUT_H__

