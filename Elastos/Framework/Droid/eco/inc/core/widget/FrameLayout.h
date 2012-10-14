
#ifndef __FrameLayout_h__
#define __FrameLayout_h__

#include "ext/frameworkext.h"
#include "view/ViewGroup.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class FrameLayout : public ViewGroup
{
public:
    FrameLayout();

    FrameLayout(
        /* [in] */ IContext* context);

    FrameLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    FrameLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI_(AutoPtr<IDrawable>) GetForeground();

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Boolean) GatherTransparentRegion(
        /* [in] */ IRegion* region);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI_(Boolean) GetConsiderGoneChildrenWhenMeasuring();

    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

protected:
    virtual CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    CARAPI DrawableStateChanged();

    virtual CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** params);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [int] */ IViewGroupLayoutParams* p);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

protected:
    Boolean mForegroundInPadding;

private:
    Boolean mMeasureAllChildren;

    AutoPtr<IDrawable> mForeground;

    Int32 mForegroundPaddingLeft;

    Int32 mForegroundPaddingTop;

    Int32 mForegroundPaddingRight;

    Int32 mForegroundPaddingBottom;

    AutoPtr<CRect> mSelfBounds;
    AutoPtr<CRect> mOverlayBounds;

    Int32 mForegroundGravity;

    Boolean mForegroundBoundsChanged;
};

#endif //__FrameLayout_h__
