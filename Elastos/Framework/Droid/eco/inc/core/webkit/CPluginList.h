
#ifndef __CPLUGINLIST_H__
#define __CPLUGINLIST_H__

#include "_CPluginList.h"
//#include "Plugin.h"
#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>

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

    ~CPluginList();

private:
    List< AutoPtr< IPlugin > > * mPlugins;
};

#endif // __CPLUGINLIST_H__
