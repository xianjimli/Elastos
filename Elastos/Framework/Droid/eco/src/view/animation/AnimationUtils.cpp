
#include "view/animation/AnimationUtils.h"
#include "view/animation/CAccelerateDecelerateInterpolator.h"
#include "view/animation/CAccelerateInterpolator.h"
#include "view/animation/CAnticipateInterpolator.h"
#include "view/animation/CAnticipateOvershootInterpolator.h"
#include "view/animation/CBounceInterpolator.h"
#include "view/animation/CCycleInterpolator.h"
#include "view/animation/CDecelerateInterpolator.h"
#include "view/animation/CLinearInterpolator.h"
#include "view/animation/COvershootInterpolator.h"
#include "view/animation/CAnimationSet.h"
#include "view/animation/CAlphaAnimation.h"
#include "view/animation/CRotateAnimation.h"
#include "view/animation/CScaleAnimation.h"
#include "view/animation/CTranslateAnimation.h"
#include "view/animation/CLayoutAnimationController.h"
#include "view/animation/CGridLayoutAnimationController.h"
#include "os/SystemClock.h"
#include "utils/Xml.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

Int64 AnimationUtils::CurrentAnimationTimeMillis()
{
    return SystemClock::GetUptimeMillis();
}

ECode AnimationUtils::LoadAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IXmlResourceParser> parser;
    res->GetAnimation(id, (IXmlResourceParser**)&parser);
    CreateAnimationFromXml(context, parser, animation);

    if (parser != NULL)
        parser->Close();

    return NOERROR;
}

ECode AnimationUtils::CreateAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IAnimation** animation)
{
    return CreateAnimationFromXml(
        c, parser, NULL, Xml::AsAttributeSet(parser), animation);
}

ECode AnimationUtils::CreateAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAnimationSet* parent,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IAnimation** animation)
{
    // Make sure we are on a start tag.
    Int32 type;
    Int32 depth, curDepth;
    parser->GetDepth(&depth);

    while (((parser->Next(&type), type) != IXmlPullParser_END_TAG
        || (parser->GetDepth(&curDepth), curDepth) > depth)
        && type != IXmlPullParser_END_DOCUMENT) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);

        if (name.Equals("set")) {
            FAIL_RETURN(CAnimationSet::New(c, attrs, (IAnimationSet**)animation));
            AutoPtr<IAnimation> temp;
            CreateAnimationFromXml(
                c, parser, IAnimationSet::Probe(*animation), attrs, (IAnimation**)&temp);
        }
        else if (name.Equals("alpha")) {
            FAIL_RETURN(CAlphaAnimation::New(c, attrs, animation));
        }
        else if (name.Equals("scale")) {
            FAIL_RETURN(CScaleAnimation::New(c, attrs, animation));
        }
        else if (name.Equals("rotate")) {
            FAIL_RETURN(CRotateAnimation::New(c, attrs, animation));
        }
        else if (name.Equals("translate")) {
            FAIL_RETURN(CTranslateAnimation::New(c, attrs, animation));
        }
        else {
            Logger::E("AnimationUtils", String("Unknown animation name: ") + name);
            return E_RUNTIME_EXCEPTION;
        }

        if (parent != NULL) {
            parent->AddAnimation(*animation);
        }
    }

    return NOERROR;
}

ECode AnimationUtils::LoadLayoutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ ILayoutAnimationController** controller)
{
    VALIDATE_NOT_NULL(controller);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IXmlResourceParser> parser;
    res->GetAnimation(id, (IXmlResourceParser**)&parser);
    CreateLayoutAnimationFromXml(context, parser, controller);

    if (parser != NULL)
        parser->Close();

    return NOERROR;
}

ECode AnimationUtils::CreateLayoutAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ ILayoutAnimationController** controller)
{
    return CreateLayoutAnimationFromXml(
        c, parser, Xml::AsAttributeSet(parser), controller);
}

ECode AnimationUtils::CreateLayoutAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ ILayoutAnimationController** controller)
{
    VALIDATE_NOT_NULL(controller);

    // Make sure we are on a start tag.
    Int32 type;
    Int32 depth, curDepth;
    parser->GetDepth(&depth);

    while (((parser->Next(&type), type) != IXmlPullParser_END_TAG
        || (parser->GetDepth(&curDepth), curDepth) > depth)
        && type != IXmlPullParser_END_DOCUMENT) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);

        if (name.Equals("layoutAnimation")) {
            FAIL_RETURN(CLayoutAnimationController::New(c, attrs, controller));
        }
        else if (name.Equals("gridLayoutAnimation")) {
            FAIL_RETURN(CGridLayoutAnimationController::New(
                c, attrs, (IGridLayoutAnimationController**)controller));
        }
        else {
            Logger::E("AnimationUtils", String("Unknown layout animation name: ") + name);

            return E_RUNTIME_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode AnimationUtils::MakeInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean fromLeft,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);

    if (fromLeft) {
        AnimationUtils::LoadAnimation(
            context, 0x010a0002/*com.android.internal.R.anim.slide_in_left*/,
            animation);
    }
    else {
        AnimationUtils::LoadAnimation(
            context, 0x010a0030/*com.android.internal.R.anim.slide_in_right*/,
            animation);
    }

    AutoPtr<IInterpolator> interpolator;
    ASSERT_SUCCEEDED(CDecelerateInterpolator::New((IInterpolator**)&interpolator));
    (*animation)->SetInterpolatorEx(interpolator);
    (*animation)->SetStartTime(CurrentAnimationTimeMillis());

    return NOERROR;
}

ECode AnimationUtils::MakeOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean toRight,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);

    if (toRight) {
        AnimationUtils::LoadAnimation(
            context, 0x010a0003/*com.android.internal.R.anim.slide_out_right*/,
            animation);
    }
    else {
        AnimationUtils::LoadAnimation(
            context, 0x010a0033/*com.android.internal.R.anim.slide_out_left*/,
            animation);
    }

    AutoPtr<IInterpolator> interpolator;
    ASSERT_SUCCEEDED(CAccelerateInterpolator::New((IInterpolator**)&interpolator));
    (*animation)->SetInterpolatorEx(interpolator);
    (*animation)->SetStartTime(CurrentAnimationTimeMillis());

    return NOERROR;
}

ECode AnimationUtils::MakeInChildBottomAnimation(
    /* [in] */ IContext* context,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);

    AnimationUtils::LoadAnimation(
        context, 0x010a002f/*com.android.internal.R.anim.slide_in_left*/,
        animation);

    AutoPtr<IInterpolator> interpolator;
    ASSERT_SUCCEEDED(CAccelerateInterpolator::New((IInterpolator**)&interpolator));
    (*animation)->SetInterpolatorEx(interpolator);
    (*animation)->SetStartTime(CurrentAnimationTimeMillis());

    return NOERROR;
}

ECode AnimationUtils::LoadInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IXmlResourceParser> parser;
    res->GetAnimation(id, (IXmlResourceParser**)&parser);
    CreateInterpolatorFromXml(context, parser, interpolator);

    if (parser != NULL)
        parser->Close();

    return NOERROR;
}

ECode AnimationUtils::CreateInterpolatorFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);

    // Make sure we are on a start tag.
    Int32 type;
    Int32 depth, curDepth;
    parser->GetDepth(&depth);

    while (((parser->Next(&type), type) != IXmlPullParser_END_TAG
        || (parser->GetDepth(&curDepth), curDepth) > depth)
        && type != IXmlPullParser_END_DOCUMENT) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
        String name;
        parser->GetName(&name);

        if (name.Equals("linearInterpolator")) {
            FAIL_RETURN(CLinearInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("accelerateInterpolator")) {
            FAIL_RETURN(CAccelerateInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("decelerateInterpolator")) {
            FAIL_RETURN(CDecelerateInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("accelerateDecelerateInterpolator")) {
            FAIL_RETURN(CAccelerateDecelerateInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("cycleInterpolator")) {
            FAIL_RETURN(CCycleInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("anticipateInterpolator")) {
            FAIL_RETURN(CAnticipateInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("overshootInterpolator")) {
            FAIL_RETURN(COvershootInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("anticipateOvershootInterpolator")) {
            FAIL_RETURN(CAnticipateOvershootInterpolator::New(c, attrs, interpolator));
        }
        else if (name.Equals("bounceInterpolator")) {
            FAIL_RETURN(CBounceInterpolator::New(c, attrs, interpolator));
        }
        else {
            Logger::E("AnimationUtils", String("Unknown interpolator name: ") + name);

            return E_RUNTIME_EXCEPTION;
        }
    }

    return NOERROR;
}
