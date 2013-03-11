#include <Logger.h>

#include "webkit/CPluginManager.h"
#include <ext/frameworkdef.h>
#include "content/Context.h"
#include "content/cm/CSignature.h"


const char* CPluginManager::LOGTAG = "PluginManager";

const char* CPluginManager::PLUGIN_SYSTEM_LIB = "/system/lib/plugins/";

const char* CPluginManager::PLUGIN_TYPE = "type";

const char* CPluginManager::TYPE_NATIVE = "native";

IPluginManager * CPluginManager::sInstance = NULL;

// Only plugin matches one of the signatures in the list can be loaded
// inside the WebView process
const char* CPluginManager::SIGNATURE_1 = "308204c5308203ada003020102020900d7cb412f75f4887e300d06092a864886f70d010105050030819d310b3009060355040613025553311330110603550408130a43616c69666f726e69613111300f0603550407130853616e204a6f736531233021060355040a131a41646f62652053797374656d7320496e636f72706f7261746564311c301a060355040b1313496e666f726d6174696f6e2053797374656d73312330210603550403131a41646f62652053797374656d7320496e636f72706f7261746564301e170d3039313030313030323331345a170d3337303231363030323331345a30819d310b3009060355040613025553311330110603550408130a43616c69666f726e69613111300f0603550407130853616e204a6f736531233021060355040a131a41646f62652053797374656d7320496e636f72706f7261746564311c301a060355040b1313496e666f726d6174696f6e2053797374656d73312330210603550403131a41646f62652053797374656d7320496e636f72706f726174656430820120300d06092a864886f70d01010105000382010d0030820108028201010099724f3e05bbd78843794f357776e04b340e13cb1c9ccb3044865180d7d8fec8166c5bbd876da8b80aa71eb6ba3d4d3455c9a8de162d24a25c4c1cd04c9523affd06a279fc8f0d018f242486bdbb2dbfbf6fcb21ed567879091928b876f7ccebc7bccef157366ebe74e33ae1d7e9373091adab8327482154afc0693a549522f8c796dd84d16e24bb221f5dbb809ca56dd2b6e799c5fa06b6d9c5c09ada54ea4c5db1523a9794ed22a3889e5e05b29f8ee0a8d61efe07ae28f65dece2ff7edc5b1416d7c7aad7f0d35e8f4a4b964dbf50ae9aa6d620157770d974131b3e7e3abd6d163d65758e2f0822db9c88598b9db6263d963d13942c91fc5efe34fc1e06e3020103a382010630820102301d0603551d0e041604145af418e419a639e1657db960996364a37ef20d403081d20603551d230481ca3081c780145af418e419a639e1657db960996364a37ef20d40a181a3a481a030819d310b3009060355040613025553311330110603550408130a43616c69666f726e69613111300f0603550407130853616e204a6f736531233021060355040a131a41646f62652053797374656d7320496e636f72706f7261746564311c301a060355040b1313496e666f726d6174696f6e2053797374656d73312330210603550403131a41646f62652053797374656d7320496e636f72706f7261746564820900d7cb412f75f4887e300c0603551d13040530030101ff300d06092a864886f70d0101050500038201010076c2a11fe303359689c2ebc7b2c398eff8c3f9ad545cdbac75df63bf7b5395b6988d1842d6aa1556d595b5692e08224d667a4c9c438f05e74906c53dd8016dde7004068866f01846365efd146e9bfaa48c9ecf657f87b97c757da11f225c4a24177bf2d7188e6cce2a70a1e8a841a14471eb51457398b8a0addd8b6c8c1538ca8f1e40b4d8b960009ea22c188d28924813d2c0b4a4d334b7cf05507e1fcf0a06fe946c7ffc435e173af6fc3e3400643710acc806f830a14788291d46f2feed9fb5c70423ca747ed1572d752894ac1f19f93989766308579393fabb43649aa8806a313b1ab9a50922a44c2467b9062037f2da0d484d9ffd8fe628eeea629ba637";


const AutoFree < ArrayOf < AutoPtr <ISignature> > > CPluginManager::SIGNATURES = ArrayOf< AutoPtr <ISignature> >::Alloc(1);


ECode CPluginManager::GetInstance(
    /* [in] */ IContext * pContext,
    /* [out] */ IPluginManager ** ppInstance)
{
    Mutex::Autolock lock(_m_syncLock);
    
    if(sInstance == NULL)
    {
        if(pContext == NULL)
        {
            //throw new IllegalStateException("First call to PluginManager need a valid context.");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        IContext * pContextT;
        ECode ec = pContext->GetApplicationContext(&pContextT);
        if(FAILED(ec))
        {
            return ec;
        }

        sInstance = new CPluginManager();
        ((CPluginManager *)sInstance) -> InitInstance(pContextT);   //CPluginManager::New(pContextT, ppInstance);     //CPluginManager::NewByFriend((CPluginManager**)&sInstance);   

        *ppInstance = sInstance;
        pContextT -> Release();
        
    }
    else
    {
        sInstance -> AddRef();
        *ppInstance = sInstance;        
    }

    return NOERROR;
}

ECode CPluginManager::InitInstance(
        /* [in] */ IContext * pContext)
{
    mContext = pContext;
    return NOERROR;
}

ECode CPluginManager::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    /*
    IMessage * pMessage;    //frameworks/base/core/java/android/os/Message.java
    IBrowserFrame -> mSJavaBridge -> ObtainMessage(IJWebCoreJavaBridge -> REFRESH_PLUGINS,reloadOpenPages,&pMessage);
    pMessage -> SendToTarget();
    pMessage -> Release();
    */
    return NOERROR;
}

ECode CPluginManager::GetPluginDirectories(
    /* [out] */ ArrayOf<String> * pPluginDirectories)
{    
    List<String>* directories = new List<String>;
    /*
    IPackageManager* pm;
    ECode ec = mContext -> GetPackageManager(&pm);
    IIntent * pIntent;
    ec = Intent::New(PLUGIN_ACTION,&pIntent);
    List<AutoPtr<IResolveInfo> >* plugins;
    ec = pm -> queryIntentServices(pIntent,
        IPackageManager->GET_SERVICES | IPackageManager->GET_META_DATA,&plugins);
    pIntent -> Release();

    Mutex::Autolock lock(_m_syncLock);
    
    // clear the list of existing packageInfo objects
    mPackageInfoCache -> clear();

    AutoPtr<IResolveInfo> info = NULL;
    for (List<AutoPtr<IResolveInfo> >::Iterator it = plugins -> begin(); it != (plugins -> end()); it++) 
    {
        info = (*it);

        // retrieve the plugin's service information
        CServiceInfo* serviceInfo = ( ((CResolveInfo*)info.Get()) -> mServiceInfo );
        if (serviceInfo == NULL) 
        {
            Utility::Logging::Logger::W(LOGTAG, "Ignore bad plugin\n");
            continue;
        }

        // retrieve information from the plugin's manifest
        IPackageInfo * pkgInfo;
        ec = pm -> getPackageInfo(serviceInfo -> packageName,
                            IPackageManager::GET_PERMISSIONS
                            | IPackageManager::GET_SIGNATURES,&pkgInfo);
        if(FAILED(ec))
        {
            if(FALSE)//PackageManager::NameNotFoundException
            {
                Utility::Logging::Logger::W(LOGTAG, String("Can't find plugin: ") + (serviceInfo -> packageName) + "\n");
                continue;    
            }            
        } 

        if (pkgInfo == NULL) 
        {
            continue;
        }
        
        // find the location of the plugin's shared library. The default
        // is to assume the app is either a user installed app or an
        // updated system app. In both of these cases the library is
        // stored in the app's data directory.

        CPackageInfo * pPI = (CPackageInfo*)pkgInfo;
        CApplicationInfo * pAI = pPI -> mApplicationInfo;
        String directory = pAI -> dataDir + "/lib";
        const Int32 appFlags = pAI -> flags;
        const Int32 updatedSystemFlags = (IApplicationInfo::FLAG_SYSTEM) |
                                       (IApplicationInfo::FLAG_UPDATED_SYSTEM_APP);
        // preloaded system app with no user updates
        if ((appFlags & updatedSystemFlags) == (IApplicationInfo::FLAG_SYSTEM)) 
        {
            directory = PLUGIN_SYSTEM_LIB + (pPI -> packageName);
        }

        // check if the plugin has the required permissions
        //JAVA:String permissions[] = pPI ->requestedPermissions;
        //AutoFree<ArrayOf<String> > permissions;
        //ArrayOf<String>::Free(permissions);
        Int32 nArrayLen = (pPI -> mRequestedPermissions) -> GetLength();
        //permissions = ArrayOf<String>::Alloc(nArrayLen);
        //for (Int32 i = 0; i < nArrayLen; ++i) 
        //{
        //    (*permissions)[i] = (*(pPI->mRequestedPermissions))[i];
        //}        
        //if (permissions == NULL) 
        //{
        //    continue;
        //}

        boolean permissionOk = FALSE;
        if ( (pPI -> mRequestedPermissions) == NULL || nArrayLen == 0 )
        {
            continue;
        }
        for(int i = 0; i < nArrayLen; i++)
        {
            if( PLUGIN_PERMISSION.Compare( (*(pPI->mRequestedPermissions))[i] ) )
            {
                permissionOk = TRUE;
                break;
            }
        }        
        if (!permissionOk) {
            continue;
        }

        // check to ensure the plugin is properly signed
        nArrayLen = (pPI -> mSignatures) ->  GetLength();
        if( (pPI -> mSignatures) == NULL || nArrayLen == 0 )
        {
            continue;
        }

        if (SystemProperties::GetBoolean("ro.secure", FALSE)) 
        {
            boolean signatureMatch = FALSE;
            CSignature * pCSignatureT = NULL;
            for(int j = 0; j < nArrayLen; j++)
            {
                pCSignatureT = (CSignature *)(*(pPI->mSignatures))[j];
                for(int i = 0; i < (SIGNATURES -> GetLength()); i++)
                {
                    if( (SIGNATURES[i]) -> Equals(pCSignatureT) )
                    {
                        signatureMatch = TRUE;
                        break;
                    }
                }

            }
            if (!signatureMatch) 
            {
                continue;
            }
        }

        // determine the type of plugin from the manifest
        if( (serviceInfo -> mMetaData) == NULL )
        {
            Utility::Logging::Logger::E(LOGTAG, String("The plugin '") + (serviceInfo -> name) + String("' has no type defined\n") );
            continue;
        }

        String pluginType;
        (serviceInfo -> metaData) -> GetString(PLUGIN_TYPE,&pluginType);
        if (!TYPE_NATIVE.Compare(pluginType)) 
        {
            Utility::Logging::Logger::E(LOGTAG, String("Unrecognized plugin type: ") + pluginType+ String("\n") );
            continue;
        }

        Handle32 cls;
        ec = GetPluginClass( (serviceInfo -> mPackageName),(serviceInfo -> mName),&cls);
        if(FAILED(ec))
        {
            if(FALSE)    //JAVA: catch(NameNotFoundException e)
            {
                Utility::Logging::Logger::E(LOGTAG, String("Can't find plugin: ") + (serviceInfo -> mPackageName) + String("\n") );
                continue;    
            }            

            if(FALSE)    //JAVA: catch (ClassNotFoundException e)
            {
                Utility::Logging::Logger::E(LOGTAG, String("Can't find plugin's class: ") + (serviceInfo -> mName) + String("\n") );
                continue;    
            }
        }

        //TODO implement any requirements of the plugin class here!
        boolean classFound = true;
        if (!classFound) 
        {
            Utility::Logging::Logger::E(LOGTAG, String("The plugin's class' ") + (serviceInfo -> mName) + String("' does not extend the appropriate class.\n") );
            continue;
        }

        // if all checks have passed then make the plugin available
        mPackageInfoCache -> add(pkgInfo);
        pkgInfo -> Release();
        directories -> Push_back(directory);
    }
    pm -> Release();
    */

    //Convert List To ArrayOf
    List<String>::Iterator iterT;
    List<String>::Iterator iterE;
    iterT = directories -> Begin();
    iterE = directories -> End();
    int nListLen = directories -> GetSize();
    ArrayOf<String>::Free(pPluginDirectories);
    pPluginDirectories = ArrayOf<String>::Alloc(nListLen);
    for(int n = 0; n < nListLen ; n ++ )
    {
        (*pPluginDirectories)[n] = *iterT;
        iterT ++;
    }

    directories -> Clear();
    delete directories;

    return NOERROR;
}

/* package */
ECode CPluginManager::GetPluginsAPKName(
        /* [in] */ String pluginLib,
        /* [out] */ String * pPluginsAPKName)
{
    // basic error checking on input params
    if (pluginLib == NULL || pluginLib.GetSize() == 0) {
        return E_NOT_IMPLEMENTED;
    }
    
    // must be synchronized to ensure the consistency of the cache
    Mutex::Autolock lock(_m_syncLock);  //synchronized(mPackageInfoCache)
    /*
    for (List<AutoPtr<IPackageInfo> >::Iterator it = mPackageInfoCache -> begin(); it != (mPackageInfoCache -> end()); it++) 
    {
        CPackageInfo * pkgInfo = (CPackageInfo *)((*it).Get());
        if(pluginLib.Contains(pkgInfo -> mPackageName))
        {
            *pPluginsAPKName = pkgInfo -> mPackageName;
        }
    }
    */

    // if no apk was found then return null
    return NOERROR;
}

ECode CPluginManager::GetPluginSharedDataDirectory(
        /* [out] */ String * pPluginSharedDataDirectory)
{
    /*
    IFile * tFile;
    ((Context *)mContext) -> GetDir("plugins", 0, &tFile);
    tFile -> GetPath(pPluginSharedDataDirectory);
    tFile -> Release();
    */
    return NOERROR;
}

/* package */
ECode CPluginManager::GetPluginClass(
        /* [in] */ String packageName, 
        /* [in] */ String className,
        /* [out] */ Handle32 * hPluginClass)
{
    /*
    IContext * pluginContext;    
    mContext -> CreatePackageContext(packageName,
            IContext::CONTEXT_INCLUDE_CODE | IContext::CONTEXT_IGNORE_SECURITY,
            &pluginContext);
    IClassLoader * pluginCL;    //IClassLoader appreared in Elastos.Framework.Core.tmp   But it is not declared in the library of the Framework
    pluginContext -> GetClassLoader(&pluginCL);
    pluginContext -> Release();
    pluginCL -> LoadClass(className,hPluginClass);
    pluginCL -> Release();
    */
    return NOERROR;
}

ECode CPluginManager::constructor(
    /* [in] */ IContext* pContext)
{
    mContext = pContext;
    /*mPackageInfoCache = new List<IPackageInfo *>; */

    ISignature * pSignatureT;    
    ECode ec = CSignature::New(SIGNATURE_1,&pSignatureT);
    if(FAILED(ec))
    {
        return ec;
    }
    (*(SIGNATURES.Get()))[0] = pSignatureT;
    pSignatureT -> Release();

    return NOERROR;
}

#if 0
ECode CPluginManager::CreateInstance(
    /* [in] */ IContext * pContext,
    /* [out] */ IPluginManager ** ppInstance)
{}
#endif
