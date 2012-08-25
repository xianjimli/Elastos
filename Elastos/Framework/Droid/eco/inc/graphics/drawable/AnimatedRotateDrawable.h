
#ifndef __ANIMATEDROTATEDRAWABLE_H__
#define __ANIMATEDROTATEDRAWABLE_H__

#include "graphics/drawable/Drawable.h"
#include <elastos/ElRefBase.h>

class AnimatedRotateDrawable : public Drawable
{
public:
    class AnimatedRotateState : public ElRefBase, public IDrawableConstantState
    {
    public:
        AnimatedRotateState(
            /* [in] */ AnimatedRotateState* source,
            /* [in] */ AnimatedRotateDrawable* owner,
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

        CARAPI_(Boolean) CanConstantState();

    public:
        AutoPtr<IDrawable> mDrawable;

        Int32 mChangingConfigurations;

        Boolean mPivotXRel;
        Float mPivotX;
        Boolean mPivotYRel;
        Float mPivotY;
        Int32 mFrameDuration;
        Int32 mFramesCount;

    private:
        Boolean mCanConstantState;
        Boolean mCheckedConstantState;
    };

public:
    AnimatedRotateDrawable(
        /* [in] */ AnimatedRotateState* rotateState = NULL,
        /* [in] */ IResources* res = NULL);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    CARAPI Start();

    CARAPI Stop();

    CARAPI_(Boolean) IsRunning();

    CARAPI Run();

    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    CARAPI_(AutoPtr<IDrawable>) GetDrawable();

    CARAPI_(Int32) GetChangingConfigurations();

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI_(Int32) GetOpacity();

    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* who);

    CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    CARAPI_(Boolean) IsStateful();

    CARAPI_(Int32) GetIntrinsicWidth();

    CARAPI_(Int32) GetIntrinsicHeight();

    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    AutoPtr<IDrawable> Mutate();

protected:
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    CARAPI Init(
        /* [in] */ AnimatedRotateState* rotateState,
        /* [in] */ IResources*);

private:
    CARAPI_(void) InitInternal();

    CARAPI_(void) NextFrame();

private:
    AutoPtr<AnimatedRotateState> mState;
    Boolean mMutated;
    Float mCurrentDegrees;
    Float mIncrement;
    Boolean mRunning;
};

#endif // __ANIMATEDROTATEDRAWABLE_H__

