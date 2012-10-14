
#ifndef __DRAWABLECONTAINER_H__
#define __DRAWABLECONTAINER_H__

#include "graphics/ElPixelFormat.h"
#include "graphics/CRect.h"
#include "graphics/drawable/Drawable.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class DrawableContainer : public Drawable
{
public:
    class DrawableContainerState : public ElRefBase, public IDrawableConstantState
    {
        friend class DrawableContainer;

    public:
        DrawableContainerState(
            /* [in] */ DrawableContainerState* orig,
            /* [in] */ DrawableContainer* owner,
            /* [in] */ IResources* res);

        virtual ~DrawableContainerState();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* configurations);

        CARAPI_(Int32) AddChild(
            /* [in] */ IDrawable* dr);

        CARAPI_(Int32) GetChildCount();

        CARAPI_(ArrayOf<IDrawable*>*) GetChildren();

        /** A Boolean value indicating whether to use the maximum padding value of
         * all frames in the set (FALSE), or to use the padding value of the frame
         * being shown (TRUE). Default value is FALSE.
         */
        CARAPI_(void) SetVariablePadding(
            /* [in] */ Boolean variable);

        CARAPI_(AutoPtr<IRect>) GetConstantPadding();

        CARAPI_(void) SetConstantSize(
            /* [in] */ Boolean constant);

        CARAPI_(Boolean) IsConstantSize();

        CARAPI_(Int32) GetConstantWidth();

        CARAPI_(Int32) GetConstantHeight();

        CARAPI_(Int32) GetConstantMinimumWidth();

        CARAPI_(Int32) GetConstantMinimumHeight();

        CARAPI_(Int32) GetOpacity();

        CARAPI_(Boolean) IsStateful();

        virtual CARAPI_(void) GrowArray(
            /* [in] */ Int32 oldSize,
            /* [in] */ Int32 newSize);

        //synchronized
        virtual CARAPI_(Boolean) CanConstantState();

    private:
        CARAPI_(void) ComputeConstantSize();

    protected:
        DrawableContainer* mOwner;

        Int32 mChangingConfigurations;
        Int32 mChildrenChangingConfigurations;

        ArrayOf<IDrawable*>* mDrawables;
        Int32 mNumChildren;

        Boolean mVariablePadding;
        AutoPtr<IRect> mConstantPadding;

        Boolean mConstantSize;
        Boolean mComputedConstantSize;
        Int32 mConstantWidth;
        Int32 mConstantHeight;
        Int32 mConstantMinimumWidth;
        Int32 mConstantMinimumHeight;

        Boolean mHaveOpacity;
        Int32 mOpacity;

        Boolean mHaveStateful;
        Boolean mStateful;

        Boolean mCheckedConstantState;
        Boolean mCanConstantState;

        Boolean mPaddingChecked;

        Boolean mDither;

        Mutex mLock;
    };

public:
    DrawableContainer();

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Int32) GetChangingConfigurations();

    virtual CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    virtual CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    virtual CARAPI SetDither(
        /* [in] */ Boolean dither);

    virtual CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    virtual CARAPI_(Boolean) IsStateful();

    virtual CARAPI_(Int32) GetIntrinsicWidth();

    virtual CARAPI_(Int32) GetIntrinsicHeight();

    virtual CARAPI_(Int32) GetMinimumWidth();

    virtual CARAPI_(Int32) GetMinimumHeight();

    virtual CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* who);

    virtual CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    virtual CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    virtual CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    virtual CARAPI_(Int32) GetOpacity();

    virtual CARAPI_(Boolean) SelectDrawable(
        /* [in] */ Int32 idx);

    virtual CARAPI_(AutoPtr<IDrawable>) GetCurrent();

    virtual CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    virtual CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    virtual CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    virtual CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    virtual CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    virtual CARAPI_(void) SetConstantState(
        /* [in] */ DrawableContainerState* state);

private:
    /**
     * To be proper, we should have a getter for dither (and alpha, etc.)
     * so that proxy classes like this can save/restore their delegates'
     * values, but we don't have getters. Since we do have setters
     * (e.g. setDither), which this proxy forwards on, we have to have some
     * default/initial setting.
     *
     * The initial setting for dither is now true, since it almost always seems
     * to improve the quality at negligible cost.
     */
    static const Boolean DEFAULT_DITHER = TRUE;
    AutoPtr<DrawableContainerState> mDrawableContainerState;
    AutoPtr<IDrawable> mCurrDrawable;
    Int32 mAlpha;// = 0xFF;
    AutoPtr<IColorFilter> mColorFilter;

    Int32 mCurIndex;// = -1;
    Boolean mMutated;
};



#endif //__DRAWABLECONTAINER_H__
