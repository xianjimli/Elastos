
#include "view/animation/AnimationUtils.h"
#include "os/SystemClock.h"

Int64 AnimationUtils::CurrentAnimationTimeMillis()
{
    return SystemClock::GetUptimeMillis();
}

ECode AnimationUtils::LoadAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IAnimation** animation)
{
    return E_NOT_IMPLEMENTED;
}

ECode AnimationUtils::MakeInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean fromLeft,
    /* [out] */ IAnimation** animation)
{
    return E_NOT_IMPLEMENTED;
}

ECode AnimationUtils::MakeOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean toRight,
    /* [out] */ IAnimation** animation)
{
    return E_NOT_IMPLEMENTED;
}

ECode AnimationUtils::MakeInChildBottomAnimation(
    /* [in] */ IContext* context,
    /* [out] */ IAnimation** animation)
{
    return E_NOT_IMPLEMENTED;
}

ECode AnimationUtils::LoadInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IInterpolator** interpolator)
{
    return E_NOT_IMPLEMENTED;
}

ECode AnimationUtils::CreateAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IAnimation** animation)
{
    return E_NOT_IMPLEMENTED;
}

ECode AnimationUtils::CreateInterpolatorFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IInterpolator** interpolator)
{
    return E_NOT_IMPLEMENTED;
}
