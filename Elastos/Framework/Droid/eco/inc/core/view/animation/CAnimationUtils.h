
#ifndef __CANIMATIONUTILS_H__
#define __CANIMATIONUTILS_H__

#include "_CAnimationUtils.h"

/**
 * Defines common utilities for working with animations.
 *
 */
CarClass(CAnimationUtils)
{
public:
    CARAPI CurrentAnimationTimeMillis(
        /* [out] */ Int64* time);

    CARAPI LoadAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ IAnimation** animation);

    CARAPI LoadLayoutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ ILayoutAnimationController** controller);

    CARAPI MakeInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Boolean fromLeft,
        /* [out] */ IAnimation** animation);

    CARAPI MakeOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Boolean toRight,
        /* [out] */ IAnimation** animation);

    CARAPI MakeInChildBottomAnimation(
        /* [in] */ IContext* context,
        /* [out] */ IAnimation** animation);

    CARAPI LoadInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ IInterpolator** interpolator);
};
#endif //__CANIMATIONUTILS_H__
