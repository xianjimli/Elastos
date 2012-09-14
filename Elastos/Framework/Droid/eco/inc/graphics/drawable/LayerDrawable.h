
#ifndef __LAYERDRAWABLE_H__
#define __LAYERDRAWABLE_H__

#include "graphics/drawable/Drawable.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class LayerDrawable : public Drawable
{
public:
    class ChildDrawable : public ElRefBase
    {
    public:
        AutoPtr<IDrawable> mDrawable;
        Int32 mInsetL;
        Int32 mInsetT;
        Int32 mInsetR;
        Int32 mInsetB;
        Int32 mId;
    };

    class LayerState : public ElRefBase, public IDrawableConstantState
    {
    public:
        LayerState(
            /* [in] */ LayerState* orig,
            /* [in] */ LayerDrawable* owner,
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

        CARAPI_(Int32) GetOpacity();

        CARAPI_(Boolean) IsStateful();

        virtual CARAPI_(Boolean) CanConstantState();

    public:
        Int32 mNum;
        ArrayOf<AutoPtr<ChildDrawable> >* mChildren;

        Int32 mChangingConfigurations;
        Int32 mChildrenChangingConfigurations;

    private:
        Boolean mHaveOpacity;
        Int32 mOpacity;

        Boolean mHaveStateful;
        Boolean mStateful;

        Boolean mCheckedConstantState;
        Boolean mCanConstantState;

        Mutex m_syncLock;
    };

public:
    LayerDrawable();

    /**
     * Create a new layer drawable with the specified list of layers and the specified
     * constant state.
     *
     * @param layers The list of layers to add to this drawable.
     * @param state The constant drawable state.
     */
    LayerDrawable(
        /* [in] */ IObjectContainer* layers,
        /* [in] */ LayerState* state = NULL);

    LayerDrawable(
        /* [in] */ LayerState* state,
        /* [in] */ IResources* res);

    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Look for a layer with the given id, and returns its {@link Drawable}.
     *
     * @param id The layer ID to search for->
     * @return The {@link Drawable} of the layer that has the given id in the hierarchy or NULL.
     */
    CARAPI_(AutoPtr<IDrawable>) FindDrawableByLayerId(
        /* [in] */ Int32 id);

    /**
     * Sets the ID of a layer->
     *
     * @param index The index of the layer which will received the ID.
     * @param id The ID to assign to the layer->
     */
    CARAPI SetId(
        /* [in] */ Int32 index,
        /* [in] */ Int32 id);

    /**
     * Returns the number of layers contained within this.
     * @return The number of layers.
     */
    CARAPI_(Int32) GetNumberOfLayers();

    /**
     * Returns the drawable at the specified layer index.
     *
     * @param index The layer index of the drawable to retrieve.
     *
     * @return The {@link android.graphics.drawable.Drawable} at the specified layer index.
     */
    CARAPI_(AutoPtr<IDrawable>) GetDrawable(
        /* [in] */ Int32 index);

    /**
     * Returns the id of the specified layer.
     *
     * @param index The index of the layer.
     *
     * @return The id of the layer or {@link android.view.View#NO_ID} if the layer has no id.
     */
    CARAPI_(Int32) GetId(
        /* [in] */ Int32 index);

    /**
     * Sets (or replaces) the {@link Drawable} for the layer with the given id.
     *
     * @param id The layer ID to search for->
     * @param drawable The replacement {@link Drawable}.
     * @return Whether the {@link Drawable} was replaced (could return FALSE if
     *         the id was not found).
     */
    CARAPI_(Boolean) SetDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [in] */ IDrawable* drawable);

    /** Specify modifiers to the bounds for the drawable[index]->
        left += l
        top += t;
        right -= r;
        bottom -= b;
    */
    CARAPI SetLayerInset(
        /* [in] */ Int32 index,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* who);

    CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Int32) GetChangingConfigurations();

    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    CARAPI SetDither(
        /* [in] */ Boolean dither);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI_(Int32) GetOpacity();

    CARAPI_(Boolean) IsStateful();

    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI_(AutoPtr<LayerState>) CreateConstantState(
        /* [in] */ LayerState* state,
        /* [in] */ IResources* res);

    CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    CARAPI_(Int32) GetIntrinsicWidth();

    CARAPI_(Int32) GetIntrinsicHeight();

    CARAPI Init(
        /* [in] */ IObjectContainer* layers,
        /* [in] */ LayerState* state = NULL);

    CARAPI Init(
        /* [in] */ LayerState* state,
        /* [in] */ IResources* res);

private:
    CARAPI_(void) AddLayer(
        /* [in] */ IDrawable* layer,
        /* [in] */ Int32 id,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(Boolean) ReapplyPadding(
        /* [in] */ Int32 i,
        /* [in] */ ChildDrawable* r);

    CARAPI_(void) EnsurePadding();

protected:
    AutoPtr<LayerState> mLayerState;

private:
    ArrayOf<Int32>* mPaddingL;
    ArrayOf<Int32>* mPaddingT;
    ArrayOf<Int32>* mPaddingR;
    ArrayOf<Int32>* mPaddingB;

    AutoPtr<IRect> mTmpRect;
    Boolean mMutated;
};

#endif // __LAYERDRAWABLE_H__

