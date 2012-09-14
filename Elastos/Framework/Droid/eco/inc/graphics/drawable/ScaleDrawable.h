
#ifndef __SCALEDRAWABLE_H__
#define __SCALEDRAWABLE_H__


#include "ext/frameworkext.h"
#include "graphics/drawable/Drawable.h"
#include "graphics/CRect.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


class ScaleDrawable : public Drawable
{
public:
    class ScaleState : public ElRefBase, public IDrawableConstantState
    {
    public:
        ScaleState(
            /* [in] */ ScaleState* orig,
            /* [in] */ ScaleDrawable* owner,
            /* [in] */ IResources* res);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);

        virtual CARAPI_(Boolean) CanConstantState();

    public:
        AutoPtr<IDrawable> mDrawable;
        Int32 mChangingConfigurations;
        Float mScaleWidth;
        Float mScaleHeight;
        Int32 mGravity;

    private:
        Boolean mCheckedConstantState;
        Boolean mCanConstantState;
    };

public:
    ScaleDrawable();

    ScaleDrawable(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 gravity,
        /* [in] */ Float scaleWidth,
        /* [in] */ Float scaleHeight);

    /**
     * Returns the drawable scaled by this ScaleDrawable.
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetDrawable();

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* who);

    CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    //@Override
    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI_(Boolean) IsStateful();

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 gravity,
        /* [in] */ Float scaleWidth,
        /* [in] */ Float scaleHeight);

    CARAPI Init(
        /* [in] */ ScaleState* state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    //@Override
    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

private:
    static CARAPI_(Float) GetPercent(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 name);

    ScaleDrawable(
        /* [in] */ ScaleState* state,
        /* [in] */ IResources* res);

private:
    AutoPtr<ScaleState> mScaleState;
    Boolean mMutated;
    AutoPtr<CRect> mTmpRect;
};

#endif //__SCALEDRAWABLE_H__
