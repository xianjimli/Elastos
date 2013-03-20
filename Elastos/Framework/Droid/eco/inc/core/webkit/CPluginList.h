
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
        /* [out] */ IObjectContainer ** list);

    CARAPI AddPlugin(
        /* [in] */ IPlugin * plugin);

    CARAPI RemovePlugin(
        /* [in] */ IPlugin * plugin);

    CARAPI Clear();

    CARAPI PluginClicked(
        /* [in] */ IContext * context,
        /* [in] */ Int32 position);

    CARAPI constructor();

    ~CPluginList();

private:
    List< AutoPtr< IPlugin > > * mPlugins;
};

#endif // __CPLUGINLIST_H__
