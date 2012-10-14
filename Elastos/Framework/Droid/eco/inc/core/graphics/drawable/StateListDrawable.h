
#ifndef __STATELISTDRAWABLE_H__
#define __STATELISTDRAWABLE_H__

#include "graphics/drawable/DrawableContainer.h"
#include "utils/StateSet.h"

class StateListDrawable : public DrawableContainer
{
public:
    class StateListState : public DrawableContainer::DrawableContainerState
    {
        friend class StateListDrawable;

    public:
        StateListState(
            /* [in] */ StateListState* orig,
            /* [in] */ StateListDrawable* owner,
            /* [in] */ IResources* res);

        ~StateListState();

        CARAPI_(Int32) AddStateSet(
            /* [in] */ const ArrayOf<Int32>* stateSet,
            /* [in] */ IDrawable* drawable);

        CARAPI_(Int32) IndexOfStateSet(
            /* [in] */ const ArrayOf<Int32>* stateSet);

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI_(void) GrowArray(
            /* [in] */ Int32 oldSize,
            /* [in] */ Int32 newSize);

    private:
        ArrayOf<ArrayOf<Int32>*>* mStateSets;
    };

public:
    StateListDrawable();

    StateListDrawable(
        /* [in] */ StateListState* state,
        /* [in] */ IResources* res);

    /**
     * Add a new image/string ID to the set of images.
     *
     * @param stateSet - An array of resource Ids to associate with the image.
     *                 Switch to this image by calling setState().
     * @param drawable -The image to show.
     */
    virtual CARAPI AddState(
        /* [in] */ const ArrayOf<Int32>& stateSet,
        /* [in] */ IDrawable* drawable);

    virtual CARAPI_(Boolean) IsStateful();

    virtual CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(StateListState*) GetStateListState();

    /**
     * Gets the number of states contained in this drawable.
     *
     * @return The number of states contained in this drawable.
     * @hide pending API council
     * @see #getStateSet(int)
     * @see #getStateDrawable(int)
     */
    virtual CARAPI_(Int32) GetStateCount();

    /**
     * Gets the state set at an index.
     *
     * @param index The index of the state set.
     * @return The state set at the index.
     * @hide pending API council
     * @see #getStateCount()
     * @see #getStateDrawable(int)
     */
    virtual CARAPI_(ArrayOf<Int32>*) GetStateSet(
        /* [in] */ Int32 index);

    /**
     * Gets the drawable at an index.
     *
     * @param index The index of the drawable.
     * @return The drawable at the index.
     * @hide pending API council
     * @see #getStateCount()
     * @see #getStateSet(int)
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetStateDrawable(
        /* [in] */ Int32 index);

    /**
     * Gets the index of the drawable with the provided state set.
     *
     * @param stateSet the state set to look up
     * @return the index of the provided state set, or -1 if not found
     * @hide pending API council
     * @see #getStateDrawable(int)
     * @see #getStateSet(int)
     */
    virtual CARAPI_(Int32) GetStateDrawableIndex(
        /* [in] */ const ArrayOf<Int32>& stateSet);

    virtual CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    virtual CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* stateSet);

    CARAPI Init(
        /* [in] */ StateListState* state,
        /* [in] */ IResources* res);

private:
    AutoPtr<StateListState> mStateListState;

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
    Boolean mMutated;
};

#endif //__STATELISTDRAWABLE_H__

