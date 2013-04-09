
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetHostCallbacks.h"

ECode CAppWidgetHostCallbacks::UpdateAppWidget(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IRemoteViews* views)
{
    void (STDCALL CAppWidgetHost::*pHandlerFunc)(Int32, IRemoteViews*);
    pHandlerFunc = &CAppWidgetHost::HandleUpdate;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(appWidgetId);
    params->WriteInterfacePtr(views);
    
    return mHost->mHandler->PostCppCallback(
        (Handle32)mHost.Get(), *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CAppWidgetHostCallbacks::ProviderChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* info)
{
    void (STDCALL CAppWidgetHost::*pHandlerFunc)(Int32, IAppWidgetProviderInfo*);
    pHandlerFunc = &CAppWidgetHost::HandleProviderChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(appWidgetId);
    params->WriteInterfacePtr(info);
    
    return mHost->mHandler->PostCppCallback(
        (Handle32)mHost.Get(), *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CAppWidgetHostCallbacks::constructor(
    /* [in] */ ILocalAppWidgetHost* host)
{
    mHost = (CAppWidgetHost*)host;
    return NOERROR;
}
