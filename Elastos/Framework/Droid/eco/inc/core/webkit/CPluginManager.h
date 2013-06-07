
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
    /* static */
    static CARAPI_(IPluginManager*) GetInstance(
        /* [in] */ IContext * context);

    /* public */
    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    /* package */
    CARAPI GetPluginDirectories(
        /* [out] */ ArrayOf<String> * pluginDirectories);

    /* package */
    CARAPI GetPluginsAPKName(
        /* [in] */ const String& pluginLib,
        /* [out] */ String * pluginsAPKName);

    /* package */
    CARAPI GetPluginSharedDataDirectory(
        /* [out] */ String * pluginSharedDataDirectory);

    /* package */
    CARAPI GetPluginClass(
        /* [in] */ const String& packageName, 
        /* [in] */ const String& className,
        /* [out] */ Handle32 * pluginClass);

    /*added for function GetInstance to init 'mInstance'  */
    CARAPI InitInstance(
        /* [in] */ IContext * context);

private:
    CARAPI constructor(
        /* [in] */ IContext * context); 

private:
    static const CString LOGTAG;

    static const CString PLUGIN_SYSTEM_LIB;

    static const CString PLUGIN_TYPE;
    
    static const CString TYPE_NATIVE;

    static IPluginManager * sInstance;

    AutoPtr<IContext> mContext;

    List<AutoPtr<ICapsuleInfo> > mPackageInfoCache;

    // Only plugin matches one of the signatures in the list can be loaded
    // inside the WebView process
    static const CString SIGNATURE_1;

    static const AutoFree < ArrayOf < AutoPtr <ISignature> > > SIGNATURES;

    static Core::Threading::Mutex mMutexClass;
    Core::Threading::Mutex mMutexPackageInfoCache;
};

#endif // __CPLUGINMANAGER_H__
