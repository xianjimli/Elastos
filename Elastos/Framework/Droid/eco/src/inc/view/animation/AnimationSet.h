
#ifndef __ANIMATIONSET_H__
#define __ANIMATIONSET_H__

#include "view/animation/Animation.h"
#include <elastos/List.h>

class AnimationSet : public Animation
{
public:
    AnimationSet(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AnimationSet(
        /* [in] */ Boolean shareInterpolator);

    CARAPI SetFillAfter(
        /* [in] */ Boolean fillAfter);

    CARAPI SetFillBefore(
        /* [in] */ Boolean fillBefore);

    CARAPI SetRepeatMode(
        /* [in] */ Int32 repeatMode);

    CARAPI SetStartOffset(
        /* [in] */ Int64 startOffset);

    CARAPI SetDuration(
        /* [in] */ Int64 durationMillis);

    CARAPI AddAnimation(
        /* [in] */ IAnimation* a);

    CARAPI SetStartTime(
        /* [in] */ Int64 startTimeMillis);

    Int64 GetStartTime();

    CARAPI RestrictDuration(
        /* [in] */ Int64 durationMillis);

    Int64 GetDuration();

    Int64 ComputeDurationHint();

    CARAPI InitializeInvalidateRegion(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    Boolean GetTransformation(
        /* [in] */ Int64 currentTime,
        /* [in] */ ITransformation* t);

    CARAPI ScaleCurrentDuration(
        /* [in] */  Float scale);

    CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

    CARAPI Reset();

    CARAPI RestoreChildrenStartOffset();

    CARAPI GetAnimations(
        /* [out] */ IObjectContainer** animations);

    Boolean WillChangeTransformationMatrix();

    Boolean WillChangeBounds();

protected:
    AnimationSet();

    //@Override
    //protected AnimationSet clone();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Boolean shareInterpolator);

private:
    void SetFlag(
        /* [in] */ Int32 mask,
        /* [in] */ Boolean value);

    void InitInternal();

private:
    static const Int32 PROPERTY_FILL_AFTER_MASK         = 0x1;
    static const Int32 PROPERTY_FILL_BEFORE_MASK        = 0x2;
    static const Int32 PROPERTY_REPEAT_MODE_MASK        = 0x4;
    static const Int32 PROPERTY_START_OFFSET_MASK       = 0x8;
    static const Int32 PROPERTY_SHARE_INTERPOLATOR_MASK = 0x10;
    static const Int32 PROPERTY_DURATION_MASK           = 0x20;
    static const Int32 PROPERTY_MORPH_MATRIX_MASK       = 0x40;
    static const Int32 PROPERTY_CHANGE_BOUNDS_MASK      = 0x80;

private:
    Int32 mFlags;
    List<AutoPtr<IAnimation> > mAnimations;
    AutoPtr<ITransformation> mTempTransformation;
    Int64 mLastEnd;
    ArrayOf<Int64>* mStoredOffsets;
};
#endif //__ANIMATIONSET_H__
