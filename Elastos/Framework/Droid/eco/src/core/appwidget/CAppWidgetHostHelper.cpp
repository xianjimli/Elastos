
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetHostHelper.h"
#include "appwidget/CAppWidgetHost.h"

ECode CAppWidgetHostHelper::DeleteAllHosts()
{
    //try {
    assert(CAppWidgetHost::sService);
    return CAppWidgetHost::sService->DeleteAllHosts();
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}
