
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
        /* [in] */ IContext * context,
        /* [out] */ IPluginManager ** instance);

    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    CARAPI GetPluginDirectories(
        /* [out] */ ArrayOf<String> * pluginDirectories);

    /* package */
    CARAPI GetPluginsAPKName(
        /* [in] */ String pluginLib,
        /* [out] */ String * pluginsAPKName);

    CARAPI GetPluginSharedDataDirectory(
        /* [out] */ String * pluginSharedDataDirectory);

    /* package */
    CARAPI GetPluginClass(
        /* [in] */ String packageName, 
        /* [in] */ String className,
        /* [out] */ Handle32 * pluginClass);

    /*added for function GetInstance to init 'mInstance'  */
    CARAPI InitInstance(
        /* [in] */ IContext * context);

private:
    CARAPI constructor(
        /* [in] */ IContext * context); 

private:
	static const CString sLOGTAG;

    static const CString sPLUGIN_SYSTEM_LIB;

    static const CString sPLUGIN_TYPE;
    
    static const CString sTYPE_NATIVE;

    static IPluginManager * sInstance;

    AutoPtr<IContext> mContext;

    /*List<AutoPtr<IPackageInfo> >* mPackageInfoCache;*/

    // Only plugin matches one of the signatures in the list can be loaded
    // inside the WebView process
    static const CString sSIGNATURE_1;

    static const AutoFree < ArrayOf < AutoPtr <ISignature> > > sSIGNATURES;
};

#endif // __CPLUGINMANAGER_H__
