
#ifndef __CPLUGINLIST_H__
#define __CPLUGINLIST_H__

#include "_CPluginList.h"

CarClass(CPluginList)
{
public:
    CARAPI GetList(
        /* [out] */ IObjectContainer ** ppList);

    CARAPI AddPlugin(
        /* [in] */ IPlugin * pPlugin);

    CARAPI RemovePlugin(
        /* [in] */ IPlugin * pPlugin);

    CARAPI Clear();

    CARAPI PluginClicked(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 position);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CPLUGINLIST_H__
