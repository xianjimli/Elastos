
#ifndef __CPLUGINMANAGERHELPER_H__
#define __CPLUGINMANAGERHELPER_H__

#include "_CPluginManagerHelper.h"

CarClass(CPluginManagerHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ IContext * context,
        /* [out] */ IPluginManager ** instance);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPLUGINMANAGERHELPER_H__
