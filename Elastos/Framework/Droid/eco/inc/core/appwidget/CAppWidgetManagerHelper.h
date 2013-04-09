
#ifndef __CAPPWIDGETMANAGERHELPER_H__
#define __CAPPWIDGETMANAGERHELPER_H__

#include "_CAppWidgetManagerHelper.h"

CarClass(CAppWidgetManagerHelper)
{
public:
    /**
     * Get the AppWidgetManager instance to use for the supplied {@link android.content.Context
     * Context} object.
     */
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IAppWidgetManager** appWidgetManager);
};

#endif //__CAPPWIDGETMANAGERHELPER_H__
