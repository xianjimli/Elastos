
#include "widget/CSetOnClickPendingIntent.h"
#include "content/CompatibilityInfo.h"
#include "graphics/CRect.h"
#include "content/CIntent.h"
#include "app/CPendingIntent.h"
#include "app/CPendingIntentHelper.h"

CSetOnClickPendingIntent::COnClickLintener::COnClickLintener(
    /* [in] */ IRemoteViewsAction* remoteViewsAction)
{
    mRemoteViewsAction = remoteViewsAction;
}

PInterface CSetOnClickPendingIntent::COnClickLintener::Probe(
    /* [in] */ REIID riid)
{
    // TODO: ALEX need EIID_IViewOnClickLintener
    /*
    if (riid == EIID_IViewOnClickLintener) {
        return (PInterface)(IViewOnClickLintener*)this;
    }
    else {
        return NULL;
    }
    */
    return NULL;
}

UInt32 CSetOnClickPendingIntent::COnClickLintener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSetOnClickPendingIntent::COnClickLintener::Release()
{
    return ElRefBase::Release();
}

ECode CSetOnClickPendingIntent::COnClickLintener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSetOnClickPendingIntent::COnClickLintener::OnClick(
    /* [in] */ IView* v)
{
    // Find target view location in screen coordinates and
    // fill into PendingIntent before sending.
    AutoPtr<IContext> context;
    FAIL_RETURN(v->GetContext((IContext**)&context));

    AutoPtr<IResources> resources;
    FAIL_RETURN(context->GetResources((IResources**)&resources));

    AutoPtr<ICompatibilityInfo> compatibilityInfo;
    FAIL_RETURN(resources->GetCompatibilityInfo(
        (ICompatibilityInfo**)&compatibilityInfo));

    Float appScale = ((CompatibilityInfo*)(ICompatibilityInfo*)compatibilityInfo)->mApplicationScale;

    Int32 pos[2];
    FAIL_RETURN(v->GetLocationOnScreen(&pos[0], &pos[1]));

    AutoPtr<IRect> rect;
    FAIL_RETURN(CRect::New((IRect**)&rect));

    Int32 width, height;
    FAIL_RETURN(v->GetWidth(&width));
    FAIL_RETURN(v->GetHeight(&height));

    rect->Set((Int32) (pos[0] * appScale + 0.5f),
        (Int32) (pos[1] * appScale + 0.5f),
        (Int32) ((pos[0] + width) * appScale + 0.5f),
        (Int32) ((pos[1] + height) * appScale + 0.5f));

    AutoPtr<IIntent> intent;
    FAIL_RETURN(CIntent::New((IIntent**)&intent));
    intent->SetSourceBounds(rect);

    // try {
        // TODO: Unregister this handler if PendingIntent.FLAG_ONE_SHOT?
        AutoPtr<IIntentSender> intentSender;
        FAIL_RETURN(((CSetOnClickPendingIntent*)(IRemoteViewsAction*)mRemoteViewsAction)->
            mPendingIntent->GetIntentSender(
            (IIntentSender**)&intentSender));

        FAIL_RETURN(context->StartIntentSender(
            intentSender, intent,
            Intent_FLAG_ACTIVITY_NEW_TASK,
            Intent_FLAG_ACTIVITY_NEW_TASK, 0));
    // } catch (IntentSender.SendIntentException e) {
    //     android.util.Log.e(LOG_TAG, "Cannot send pending intent: ", e);
    // }

    return NOERROR;
}

CSetOnClickPendingIntent::CSetOnClickPendingIntent()
    : mViewId(0)
{
}

ECode CSetOnClickPendingIntent::Apply(
    /* [in] */ IView* root)
{
    AutoPtr<IView> target;
    FAIL_RETURN(root->FindViewById(mViewId, (IView**)&target));

    if (target != NULL && mPendingIntent != NULL) {
        AutoPtr<IViewOnClickListener> listener =
            new CSetOnClickPendingIntent::COnClickLintener(this);
        if (listener == NULL) {
            return E_OUT_OF_MEMORY;
        }
        target->SetOnClickListener(listener);
    }

    return NOERROR;
}

ECode CSetOnClickPendingIntent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSetOnClickPendingIntent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(SetOnClickPendingIntent_TAG));
    FAIL_RETURN(dest->WriteInt32(mViewId));

    IParcelable* parcelable = (IParcelable*)mPendingIntent->Probe(
        EIID_IParcelable);
    if (!parcelable) return E_NO_INTERFACE;
    FAIL_RETURN(parcelable->WriteToParcel(dest));

    return NOERROR;
}

ECode CSetOnClickPendingIntent::constructor(
    /* [in] */ Int32 id,
    /* [in] */ IPendingIntent* pendingIntent)
{
    mViewId = id;
    mPendingIntent = pendingIntent;

    return NOERROR;
}

ECode CSetOnClickPendingIntent::constructor(
    /* [in] */ IParcel* parcel)
{
    FAIL_RETURN(parcel->ReadInt32(&mViewId));

    AutoPtr<IPendingIntentHelper> helper;
    FAIL_RETURN(CPendingIntentHelper::AcquireSingleton(
        (IPendingIntentHelper**)&helper));
    FAIL_RETURN(helper->ReadPendingIntentOrNullFromParcel(
        parcel, (IPendingIntent**)&mPendingIntent));

    return NOERROR;
}

