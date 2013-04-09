
#ifndef __CAPPWIDGETHOSTCALLBACKS_H__
#define __CAPPWIDGETHOSTCALLBACKS_H__

#include "_CAppWidgetHostCallbacks.h"
#include "appwidget/CAppWidgetHost.h"
#include <elastos/AutoPtr.h>

CarClass(CAppWidgetHostCallbacks)
{
public:
    CARAPI UpdateAppWidget(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IRemoteViews* views);

    CARAPI ProviderChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* info);

    CARAPI constructor(
        /* [in] */ ILocalAppWidgetHost* host);

private:
    AutoPtr<CAppWidgetHost> mHost;
};

#endif //__CAPPWIDGETHOSTCALLBACKS_H__
