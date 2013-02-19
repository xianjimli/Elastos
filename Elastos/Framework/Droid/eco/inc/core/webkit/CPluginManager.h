
#ifndef __CPLUGINMANAGER_H__
#define __CPLUGINMANAGER_H__

#include "_CPluginManager.h"

CarClass(CPluginManager)
{
public:
    CARAPI GetInstance(
        /* [in] */ IContext * pContext,
        /* [out] */ IPluginManager ** ppInstance);

    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    CARAPI GetPluginDirectories(
        /* [out] */ ArrayOf<String> * pPluginDirectories);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPLUGINMANAGER_H__
