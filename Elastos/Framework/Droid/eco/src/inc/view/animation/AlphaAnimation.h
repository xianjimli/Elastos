
#ifndef __ALPHAANIMATION_H__
#define __ALPHAANIMATION_H__

#include "view/animation/Animation.h"

class AlphaAnimation : public Animation
{
public:
    AlphaAnimation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AlphaAnimation(
        /* [in] */ Float fromAlpha,
        /* [in] */ Float toAlpha);

    CARAPI_(Boolean) WillChangeTransformationMatrix();

    CARAPI_(Boolean) WillChangeBounds();

protected:
    AlphaAnimation();

    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Float fromAlpha,
        /* [in] */ Float toAlpha);

private:
    Float mFromAlpha;
    Float mToAlpha;
};
#endif //__ALPHAANIMATION_H__
