
#ifndef __LEVELLISTDRAWABLE_H__
#define __LEVELLISTDRAWABLE_H__

#include "graphics/drawable/DrawableContainer.h"
#include <elastos/AutoPtr.h>


class LevelListDrawable : public DrawableContainer
{
    friend class LevelListState;

protected:
    class LevelListState : public DrawableContainerState
    {
        friend class LevelListDrawable;

    public:
        LevelListState(
            /* [in] */ LevelListState* orig,
            /* [in] */ LevelListDrawable* owner,
            /* [in] */ IResources* res);

        virtual ~LevelListState();

        virtual CARAPI_(void) AddLevel(
            /* [in] */ Int32 low,
            /* [in] */ Int32 high,
            /* [in] */ IDrawable* drawable);

        virtual CARAPI_(Int32) IndexOfLevel(
            /* [in] */ Int32 level);

        //@Override
        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI_(void) GrowArray(
            /* [in] */ Int32 oldSize,
            /* [in] */ Int32 newSize);

    private:
        ArrayOf<Int32>* mLows;
        ArrayOf<Int32>* mHighs;
    };

public:
    LevelListDrawable();

    virtual CARAPI AddLevel(
        /* [in] */ Int32 low,
        /* [in] */ Int32 high,
        /* [in] */ IDrawable* drawable);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ LevelListState* state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

private:
    LevelListDrawable(
        /* [in] */ LevelListState* state,
        /* [in] */ IResources* res);

private:
    AutoPtr<LevelListState> mLevelListState;
    Boolean mMutated;
};

#endif //__LEVELLISTDRAWABLE_H__
