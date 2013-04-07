
#include "widget/CRemoteViews.h"
#include "widget/CViewGroupAction.h"
#include "widget/CSetOnClickPendingIntent.h"
#include "widget/CSetDrawableParameters.h"
#include "widget/CReflectionAction.h"

static String LOG_TAG("RemoteViews");

CRemoteViews::CRemoteViews()
    : mLayoutId(0)
{
}

ECode CRemoteViews::Clone(
    /* [out] */ IRemoteViews** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IRemoteViews> that;
    FAIL_RETURN(CRemoteViews::New(mPackage, mLayoutId, (IRemoteViews**)&that));
    // if (mActions != null) {
    //     that.mActions = (ArrayList<Action>)mActions.clone();
    // }

    CRemoteViews* obj = (CRemoteViews*)(IRemoteViews*)that;
    List<AutoPtr<IRemoteViewsAction> >::Iterator itor;
    for (itor = mActions.Begin(); itor != mActions.End(); ++itor) {
        obj->mActions.PushBack((IRemoteViewsAction*)*itor);
    }

    *result = that;

    return NOERROR;
}

ECode CRemoteViews::GetPackage(
    /* [out] */ String* package)
{
    VALIDATE_NOT_NULL(package);

    *package = mPackage;

    return NOERROR;
}

ECode CRemoteViews::GetLayoutId(
    /* [out] */ Int32* layoutId)
{
    VALIDATE_NOT_NULL(layoutId);

    *layoutId = mLayoutId;

    return NOERROR;
}

ECode CRemoteViews::AddAction(
    /* [in] */ IRemoteViewsAction* a)
{
    // if (mActions == null) {
    //     mActions = new ArrayList<Action>();
    // }
    mActions.PushBack(a);

    return NOERROR;
}

ECode CRemoteViews::AddView(
    /* [in] */ Int32 viewId,
    /* [in] */ IRemoteViews* nestedView)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CViewGroupAction::New(
        viewId, nestedView, (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::RemoveAllViews(
    /* [in] */ Int32 viewId)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CViewGroupAction::New(
        viewId, NULL, (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetViewVisibility(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 visibility)
{
    return SetInt32(viewId, String("setVisibility"), visibility);
}

ECode CRemoteViews::SetTextViewText(
    /* [in] */ Int32 viewId,
    /* [in] */ ICharSequence* text)
{
    return SetCharSequence(viewId, String("setText"), text);
}

ECode CRemoteViews::SetImageViewResource(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 srcId)
{
    return SetInt32(viewId, String("setImageResource"), srcId);
}

ECode CRemoteViews::SetImageViewUri(
    /* [in] */ Int32 viewId,
    /* [in] */ IUri* uri)
{
    return SetUri(viewId, String("setImageURI"), uri);
}

ECode CRemoteViews::SetImageViewBitmap(
    /* [in] */ Int32 viewId,
    /* [in] */ IBitmap* bitmap)
{
    return SetBitmap(viewId, String("setImageBitmap"), bitmap);
}

ECode CRemoteViews::SetChronometer(
    /* [in] */ Int32 viewId,
    /* [in] */ Int64 base,
    /* [in] */ const String& format,
    /* [in] */ Boolean started)
{
    FAIL_RETURN(SetInt64(viewId, String("setBase"), base));
    FAIL_RETURN(SetString(viewId, String("setFormat"), format));
    FAIL_RETURN(SetBoolean(viewId, String("setStarted"), started));

    return NOERROR;
}

ECode CRemoteViews::SetProgressBar(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 max,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean indeterminate)
{
    FAIL_RETURN(SetBoolean(viewId, String("setIndeterminate"), indeterminate));
    if (!indeterminate) {
        FAIL_RETURN(SetInt32(viewId, String("setMax"), max));
        FAIL_RETURN(SetInt32(viewId, String("setProgress"), progress));
    }

    return NOERROR;
}

ECode CRemoteViews::SetOnClickPendingIntent(
    /* [in] */ Int32 viewId,
    /* [in] */ IPendingIntent* pendingIntent)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CSetOnClickPendingIntent::New(
        viewId, pendingIntent, (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetDrawableParameters(
    /* [in] */ Int32 viewId,
    /* [in] */ Boolean targetBackground,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 colorFilter,
    /* [in] */ PorterDuffMode mode,
    /* [in] */ Int32 level)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CSetDrawableParameters::New(
        viewId, targetBackground, alpha, colorFilter, mode, level,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetTextColor(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 color)
{
    return SetInt32(viewId, String("setTextColor"), color);
}

ECode CRemoteViews::SetBoolean(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Boolean value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_BOOLEAN, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetByte(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Byte value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_BYTE, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetInt16(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int16 value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_SHORT, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetInt32(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int32 value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_INT, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetInt64(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int64 value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_LONG, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetFloat(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Float value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_FLOAT, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetDouble(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Double value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_DOUBLE, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetChar16(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Char16 value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_CHAR, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetString(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ const String& value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_STRING, (IInterface*)&value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetCharSequence(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ ICharSequence* value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_CHAR_SEQUENCE,
        (IInterface*)value, (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetUri(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IUri* value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_URI, (IInterface*)value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetBitmap(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IBitmap* value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_BITMAP, (IInterface*)value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::SetBundle(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IBundle* value)
{
    AutoPtr<IRemoteViewsAction> action;
    FAIL_RETURN(CReflectionAction::New(
        viewId, methodName, CReflectionAction::T_BUNDLE, (IInterface*)value,
        (IRemoteViewsAction**)&action));

    return AddAction(action);
}

ECode CRemoteViews::Apply(
    /* [in] */ IContext* context,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    AutoPtr<IView> result;
    AutoPtr<IContext> c;

    FAIL_RETURN(PrepareContext(context, (IContext**)&c));

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(
        Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

    AutoPtr<ILayoutInflater> inflater2;
    FAIL_RETURN(inflater->CloneInContext(c, (ILayoutInflater**)&inflater2));
    FAIL_RETURN(inflater2->SetFilter(this));

    FAIL_RETURN(inflater2->InflateEx2(
        mLayoutId, parent, FALSE, (IView**)&result));
    FAIL_RETURN(PerformApply(result));

    *view = result;
    (*view)->AddRef();

    return NOERROR;
}

ECode CRemoteViews::Reapply(
    /* [in] */ IContext* context,
    /* [in] */ IView* v)
{
    FAIL_RETURN(PrepareContext(context, NULL));
    FAIL_RETURN(PerformApply(v));

    return NOERROR;
}

ECode CRemoteViews::ReadFromParcel(
    /* [in] */ IParcel * pSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteViews::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mPackage));
    FAIL_RETURN(dest->WriteInt32(mLayoutId));

    Int32 count;
    // if (mActions != null) {
        count = mActions.GetSize();
    // } else {
    //     count = 0;
    // }

    FAIL_RETURN(dest->WriteInt32(count));

    for (Int32 i = 0; i < count; i++) {
        IRemoteViewsAction* a = mActions[i];
        IParcelable* parcel = (IParcelable*)a->Probe(EIID_IParcelable);
        FAIL_RETURN(parcel->WriteToParcel(dest));
    }

    return NOERROR;
}

ECode CRemoteViews::OnLoadClass(
    /* [in] */ Handle32 clazz,
    /* [out] */ Boolean* allowed)
{
    // TODO: ALEX need isAnnotationPresent
    // return clazz.isAnnotationPresent(RemoteView.class);
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteViews::constructor(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 layoutId)
{
    mPackage = packageName;
    mLayoutId = layoutId;

    return NOERROR;
}

ECode CRemoteViews::constructor(
    /* [in] */ IParcel* parcel)
{
    FAIL_RETURN(parcel->ReadString(&mPackage));
    FAIL_RETURN(parcel->ReadInt32(&mLayoutId));

    Int32 count;
    FAIL_RETURN(parcel->ReadInt32(&count));
    if (count > 0) {
        // mActions = new ArrayList<Action>(count);
        for (Int32 i = 0; i < count; i++) {
            Int32 tag;
            FAIL_RETURN(parcel->ReadInt32(&tag));
            switch (tag) {
            case SetOnClickPendingIntent_TAG: {
                AutoPtr<IRemoteViewsAction> action;
                FAIL_RETURN(CSetOnClickPendingIntent::New(
                    parcel, (IRemoteViewsAction**)&action));
                mActions.PushBack(action);
                break;
            }
            case SetDrawableParameters_TAG: {
                AutoPtr<IRemoteViewsAction> action;
                FAIL_RETURN(CSetDrawableParameters::New(
                    parcel, (IRemoteViewsAction**)&action));
                mActions.PushBack(action);
                break;
            }
            case ReflectionAction_TAG: {
                AutoPtr<IRemoteViewsAction> action;
                FAIL_RETURN(CReflectionAction::New(
                    parcel, (IRemoteViewsAction**)&action));
                mActions.PushBack(action);
                break;
            }
            case ViewGroupAction_TAG: {
                AutoPtr<IRemoteViewsAction> action;
                FAIL_RETURN(CViewGroupAction::New(
                    parcel, (IRemoteViewsAction**)&action));
                mActions.PushBack(action);
                break;
            }
            default:
                // throw new ActionException("Tag " + tag + " not found");
                return E_ACTION_EXCEPTION;
            }
        }
    }

    return NOERROR;
}

ECode CRemoteViews::PerformApply(
    /* [in] */ IView* v)
{
    // if (mActions != null) {
        Int32 count = mActions.GetSize();
        for (Int32 i = 0; i < count; i++) {
            IRemoteViewsAction* a = mActions[i];
            FAIL_RETURN(a->Apply(v));
        }
    // }

    return NOERROR;
}

ECode CRemoteViews::PrepareContext(
    /* [in] */ IContext* context,
    /* [out] */ IContext** result)
{
    AutoPtr<IContext> c;
    String packageName = mPackage;

    if (!packageName.IsNull()) {
        // try {
        ECode ec = context->CreateCapsuleContext(
            packageName, Context_CONTEXT_RESTRICTED, (IContext**)&c);
        if (FAILED(ec)) c = context;
        // } catch (NameNotFoundException e) {
        //     Log.e(LOG_TAG, "Package name " + packageName + " not found");
        //     c = context;
        // }
    } else {
        c = context;
    }

    if (result) {
        *result = c;
        (*result)->AddRef();
    }

    return NOERROR;
}
