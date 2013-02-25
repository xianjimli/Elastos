
#ifndef __CSYSTEMUISERVICE_H__
#define __CSYSTEMUISERVICE_H__

#include "app/Service.h"
#include "_CSystemUIService.h"
#include <elastos/AutoPtr.h>

class CSystemUIService : public Service
{
protected:
    CARAPI OnCreate();

    CARAPI OnConfigurationChanged(
    	/* [in] */ IConfiguration* newConfig);

    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

protected:
    static const char* TAG;

    // /**
    //  * The class names of the stuff to start.
    //  */
    // final Object[] SERVICES = new Object[] {
    //         0, // system bar or status bar, filled in below.
    //         com.android.systemui.power.PowerUI.class,
    //     };

    /**
     * Hold a reference on the stuff we start.
     */
    //ArrayOf<AutoPtr<ISystemUI>>* mServices;
    //AutoPtr<ISystemUI> mStatusBar;
};
#endif //__CSYSTEMUISERVICE_H__
