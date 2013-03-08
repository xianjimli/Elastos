
#ifndef __CPLUGINMANAGER_H__
#define __CPLUGINMANAGER_H__

#include "_CPluginManager.h"
#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos.h>
#include <elastos/Mutex.h>
//#include <Logger.h>
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>

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

    /* package */
    CARAPI GetPluginsAPKName(
        /* [in] */ String pluginLib,
        /* [out] */ String * pPluginsAPKName);

    CARAPI GetPluginSharedDataDirectory(
        /* [out] */ String * pPluginSharedDataDirectory);

    /* package */
    CARAPI GetPluginClass(
        /* [in] */ String packageName, 
        /* [in] */ String className,
        /* [out] */ Handle32 * hPluginClass);

    /*added for function GetInstance to init 'mInstance'  */
    CARAPI InitInstance(
        /* [in] */ IContext * pContext);

private:
    CARAPI constructor(
        /* [in] */ IContext * context); 

private:
	static const char* LOGTAG;

    static const char* PLUGIN_SYSTEM_LIB;

    static const char* PLUGIN_TYPE;
    
    static const char* TYPE_NATIVE;

    static IPluginManager * sInstance;

    AutoPtr<IContext> mContext;

    /*List<AutoPtr<IPackageInfo> >* mPackageInfoCache;*/

    // Only plugin matches one of the signatures in the list can be loaded
    // inside the WebView process
    static const char* SIGNATURE_1;

    static const AutoFree < ArrayOf < AutoPtr <ISignature> > > SIGNATURES;
};

#endif // __CPLUGINMANAGER_H__
