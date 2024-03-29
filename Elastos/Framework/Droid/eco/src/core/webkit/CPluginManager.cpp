#include <Logger.h>

#include "webkit/CPluginManager.h"
#include <ext/frameworkdef.h>
#include "content/Context.h"
#include "content/cm/CSignature.h"
#include "webkit/CBrowserFrame.h"
#include "webkit/JWebCoreJavaBridge.h"
#include "os/SystemProperties.h"
#include "content/CIntent.h"
#include "capsule/CServiceInfo.h"
#include "content/CResolveInfo.h"
#include "capsule/CCapsuleInfo.h"
#include "content/CApplicationInfo.h"
#include "os/SystemProperties.h"

const CString CPluginManager::LOGTAG = "PluginManager";

const CString CPluginManager::PLUGIN_SYSTEM_LIB = "/system/lib/plugins/";

const CString CPluginManager::PLUGIN_TYPE = "type";

const CString CPluginManager::TYPE_NATIVE = "native";

IPluginManager* CPluginManager::sInstance = NULL;

// Only plugin matches one of the signatures in the list can be loaded
// inside the WebView process
const CString CPluginManager::SIGNATURE_1 = "308204c5308203ada003020102020900d7cb412f75f4887e300d06092a864886f70d010105050030819d310b3009060355040613025553311330110603550408130a43616c69666f726e69613111300f0603550407130853616e204a6f736531233021060355040a131a41646f62652053797374656d7320496e636f72706f7261746564311c301a060355040b1313496e666f726d6174696f6e2053797374656d73312330210603550403131a41646f62652053797374656d7320496e636f72706f7261746564301e170d3039313030313030323331345a170d3337303231363030323331345a30819d310b3009060355040613025553311330110603550408130a43616c69666f726e69613111300f0603550407130853616e204a6f736531233021060355040a131a41646f62652053797374656d7320496e636f72706f7261746564311c301a060355040b1313496e666f726d6174696f6e2053797374656d73312330210603550403131a41646f62652053797374656d7320496e636f72706f726174656430820120300d06092a864886f70d01010105000382010d0030820108028201010099724f3e05bbd78843794f357776e04b340e13cb1c9ccb3044865180d7d8fec8166c5bbd876da8b80aa71eb6ba3d4d3455c9a8de162d24a25c4c1cd04c9523affd06a279fc8f0d018f242486bdbb2dbfbf6fcb21ed567879091928b876f7ccebc7bccef157366ebe74e33ae1d7e9373091adab8327482154afc0693a549522f8c796dd84d16e24bb221f5dbb809ca56dd2b6e799c5fa06b6d9c5c09ada54ea4c5db1523a9794ed22a3889e5e05b29f8ee0a8d61efe07ae28f65dece2ff7edc5b1416d7c7aad7f0d35e8f4a4b964dbf50ae9aa6d620157770d974131b3e7e3abd6d163d65758e2f0822db9c88598b9db6263d963d13942c91fc5efe34fc1e06e3020103a382010630820102301d0603551d0e041604145af418e419a639e1657db960996364a37ef20d403081d20603551d230481ca3081c780145af418e419a639e1657db960996364a37ef20d40a181a3a481a030819d310b3009060355040613025553311330110603550408130a43616c69666f726e69613111300f0603550407130853616e204a6f736531233021060355040a131a41646f62652053797374656d7320496e636f72706f7261746564311c301a060355040b1313496e666f726d6174696f6e2053797374656d73312330210603550403131a41646f62652053797374656d7320496e636f72706f7261746564820900d7cb412f75f4887e300c0603551d13040530030101ff300d06092a864886f70d0101050500038201010076c2a11fe303359689c2ebc7b2c398eff8c3f9ad545cdbac75df63bf7b5395b6988d1842d6aa1556d595b5692e08224d667a4c9c438f05e74906c53dd8016dde7004068866f01846365efd146e9bfaa48c9ecf657f87b97c757da11f225c4a24177bf2d7188e6cce2a70a1e8a841a14471eb51457398b8a0addd8b6c8c1538ca8f1e40b4d8b960009ea22c188d28924813d2c0b4a4d334b7cf05507e1fcf0a06fe946c7ffc435e173af6fc3e3400643710acc806f830a14788291d46f2feed9fb5c70423ca747ed1572d752894ac1f19f93989766308579393fabb43649aa8806a313b1ab9a50922a44c2467b9062037f2da0d484d9ffd8fe628eeea629ba637";


const AutoFree < ArrayOf < AutoPtr <ISignature> > > CPluginManager::SIGNATURES = ArrayOf< AutoPtr <ISignature> >::Alloc(1);

Core::Threading::Mutex CPluginManager::mMutexClass;

IPluginManager* CPluginManager::GetInstance(
    /* [in] */ IContext * context)
{
    Mutex::Autolock lock(mMutexClass);
    
    if(sInstance == NULL) {
        if(context == NULL) {
            //throw new IllegalStateException("First call to PluginManager need a valid context.");
            //return E_ILLEGAL_STATE_EXCEPTION;
            return NULL;
        }
        AutoPtr<IContext> contextT;
        ECode ec = context->GetApplicationContext((IContext**)&contextT);
        if(FAILED(ec)) {
            //return ec;
            return NULL;
        }

        //sInstance = new CPluginManager();
        AcquireSingletonByFriend( (CPluginManager**)&sInstance );
        ((CPluginManager *)sInstance) -> InitInstance(contextT.Get());    //CPluginManager::New(contextT, (CPluginManager**)&instance);     //CPluginManager::NewByFriend((CPluginManager**)&sInstance);   

        return sInstance;        
    }
    else {
        return sInstance;        
    }
    
    return NULL;
}

ECode CPluginManager::InitInstance(
        /* [in] */ IContext * context)
{
    mContext = context;
    return NOERROR;
}

ECode CPluginManager::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(reloadOpenPages);
    (CBrowserFrame::sJavaBridge)->SendMessage(JWebCoreJavaBridge::REFRESH_PLUGINS, params);
    return NOERROR;
}

ECode CPluginManager::GetPluginDirectories(
    /* [out] */ ArrayOf<String> * pluginDirectories)
{
    VALIDATE_NOT_NULL(pluginDirectories);
    List<String>* directories = new List<String>;
    
    AutoPtr<ILocalCapsuleManager> pm;
    ECode ec = mContext -> GetCapsuleManager((ILocalCapsuleManager**)&pm);
    AutoPtr<IIntent> pIntent;
    ec = CIntent::New(String(PLUGIN_ACTION),(IIntent**)&pIntent);
    List<AutoPtr<IResolveInfo> >* plugins;
    ec = pm -> QueryIntentServices(pIntent.Get(),
        CapsuleManager_GET_SERVICES | CapsuleManager_GET_META_DATA, (IObjectContainer**)&plugins);

    Mutex::Autolock lock(mMutexPackageInfoCache);
    
    // clear the list of existing packageInfo objects
    mPackageInfoCache.Clear();

    AutoPtr<IResolveInfo> info;
    for (List<AutoPtr<IResolveInfo> >::Iterator it = plugins -> Begin(); it != (plugins -> End()); it++)  {
        info = (*it);

        // retrieve the plugin's service information
        AutoPtr<CServiceInfo> serviceInfo = ( ((CResolveInfo*)(info.Get())) -> mServiceInfo );
        if (serviceInfo == NULL)  {
            Utility::Logging::Logger::W(LOGTAG, "Ignore bad plugin\n");
            continue;
        }

        // retrieve information from the plugin's manifest
        AutoPtr<ICapsuleInfo> pkgInfo;
        String strPackageName;
        serviceInfo -> GetCapsuleName(&strPackageName);//serviceInfo -> packageName
        ec = pm -> GetCapsuleInfo(strPackageName,
                            CapsuleManager_GET_PERMISSIONS
                            | CapsuleManager_GET_SIGNATURES,(ICapsuleInfo**)&pkgInfo);
        if(FAILED(ec)) {
            if(FALSE){
                //PackageManager::NameNotFoundException                 
                Utility::Logging::Logger::W(LOGTAG, String("Can't find plugin: ") + strPackageName + "\n");
                continue;    
            }            
        } 

        if (pkgInfo.Get() == NULL)  {
            continue;
        }
        
        // find the location of the plugin's shared library. The default
        // is to assume the app is either a user installed app or an
        // updated system app. In both of these cases the library is
        // stored in the app's data directory.

        CCapsuleInfo * pPI = (CCapsuleInfo*)(pkgInfo.Get());
        AutoPtr<CApplicationInfo> pAI = (CApplicationInfo*)((pPI -> mApplicationInfo).Get());
        String directory = pAI -> mDataDir + "/lib";
        const Int32 appFlags = pAI -> mFlags;
        const Int32 updatedSystemFlags = ApplicationInfo_FLAG_SYSTEM |
                                       ApplicationInfo_FLAG_UPDATED_SYSTEM_APP;
        // preloaded system app with no user updates
        if ((appFlags & updatedSystemFlags) == ApplicationInfo_FLAG_SYSTEM)  {
            String strCapsuleName;
            pPI->GetCapsuleName(&strCapsuleName);
            directory = String(PLUGIN_SYSTEM_LIB) + strCapsuleName;
        }

        // check if the plugin has the required permissions
        //JAVA:String permissions[] = pkgInfo.requestedPermissions;
        List<String> permissions=pPI->mRequestedPermissions;
        if (permissions.IsEmpty())  {
           continue;
        }

        Boolean permissionOk = FALSE;
        Int32 nArrayLen=permissions.GetSize();
        for(Int32 i = 0; i < nArrayLen; i++) {
            if( String(PLUGIN_PERMISSION).Equals( (pPI->mRequestedPermissions)[i] ) )  {
                permissionOk = TRUE;
                break;
            }
        }        
        if (!permissionOk) {
            continue;
        }

        // check to ensure the plugin is properly signed
        List< AutoPtr<ISignature> > Signatures = pPI->mSignatures;
        nArrayLen = (pPI -> mSignatures).GetSize();
        if( Signatures.IsEmpty() ) {
            continue;
        }

        if (SystemProperties::GetBoolean("ro.secure", FALSE))  {
            Boolean signatureMatch = FALSE;
            AutoPtr<ISignature> pCSignatureT = NULL;
            for(Int32 j = 0; j < nArrayLen; j++) {
                pCSignatureT = (((pPI->mSignatures)[j]).Get());
                for(Int32 i = 0; i < (SIGNATURES -> GetLength()); i++) {
                    Boolean isEqual=FALSE;
                    ((CSignature*)(((*SIGNATURES)[i]).Get()))->Equals(pCSignatureT, &isEqual);
                    if( isEqual ) {
                        signatureMatch = TRUE;
                        break;
                    }
                }
            }
            if (!signatureMatch)  {
                continue;
            }
        }

        // determine the type of plugin from the manifest
        AutoPtr<IBundle> metaDataSI;
        serviceInfo->GetMetaData((IBundle**)&metaDataSI);
        
        String nameSI;
        serviceInfo->GetName(&nameSI);
        if( metaDataSI.Get() == NULL ) {
            Utility::Logging::Logger::E(LOGTAG, String("The plugin '") + nameSI + String("' has no type defined\n") );
            continue;
        }

        String pluginTypeSI;
        metaDataSI -> GetString(String(PLUGIN_TYPE),&pluginTypeSI);
        if (!TYPE_NATIVE.Compare(pluginTypeSI))  {
            Utility::Logging::Logger::E(LOGTAG, String("Unrecognized plugin type: ") + pluginTypeSI+ String("\n") );
            continue;
        }

        Handle32 cls;
        String capsuleNameSI;
        serviceInfo->GetCapsuleName(&capsuleNameSI);
        ec = GetPluginClass( capsuleNameSI,nameSI,&cls);
        if(FAILED(ec)) {
            if(FALSE) {
                //JAVA: catch(NameNotFoundException e)
                Utility::Logging::Logger::E(LOGTAG, String("Can't find plugin: ") + capsuleNameSI + String("\n") );
                continue;    
            }            

            if(FALSE) {
                //JAVA: catch (ClassNotFoundException e)
                Utility::Logging::Logger::E(LOGTAG, String("Can't find plugin's class: ") + nameSI + String("\n") );
                continue;    
            }
        }

        //TODO implement any requirements of the plugin class here!
        Boolean classFound = TRUE;
        if (!classFound)  {
            Utility::Logging::Logger::E(LOGTAG, String("The plugin's class' ") + nameSI + String("' does not extend the appropriate class.\n") );
            continue;
        }

        // if all checks have passed then make the plugin available
        mPackageInfoCache.PushBack(pkgInfo);
        directories -> PushBack(directory);
    }

    //Convert List To ArrayOf
    List<String>::Iterator iterT;
    List<String>::Iterator iterE;
    iterT = directories -> Begin();
    iterE = directories -> End();
    int nListLen = directories -> GetSize();
    ArrayOf<String>::Free(pluginDirectories);
    pluginDirectories = ArrayOf<String>::Alloc(nListLen);
    for(int n = 0; n < nListLen ; n ++ ) {
        (*pluginDirectories)[n] = *iterT;
        iterT ++;
    }

    directories -> Clear();
    delete directories;

    return NOERROR;
}

/* package */
ECode CPluginManager::GetPluginsAPKName(
        /* [in] */ const String& pluginLib,
        /* [out] */ String * pluginsAPKName)
{
    VALIDATE_NOT_NULL(pluginsAPKName);
    // basic error checking on input params
    if (pluginLib == NULL || pluginLib.GetSize() == 0) {
        return E_NOT_IMPLEMENTED;
    }
    
    // must be synchronized to ensure the consistency of the cache
    Mutex::Autolock lock(mMutexPackageInfoCache);  //synchronized(mPackageInfoCache)
    
    for (List<AutoPtr<ICapsuleInfo> >::Iterator it = mPackageInfoCache.Begin(); it != (mPackageInfoCache.End()); it++)  {
        AutoPtr<CCapsuleInfo> pkgInfo = (CCapsuleInfo *)((*it).Get());
        if(pluginLib.Contains(pkgInfo -> mCapsuleName)) {
            *pluginsAPKName = pkgInfo -> mCapsuleName;
        }
    }    

    // if no apk was found then return null
    return NOERROR;
}

ECode CPluginManager::GetPluginSharedDataDirectory(
        /* [out] */ String * pluginSharedDataDirectory)
{
    VALIDATE_NOT_NULL(pluginSharedDataDirectory);    
    AutoPtr<IFile> tFile;
    ((Context *)(mContext.Get()))->GetDir(String("plugins"), 0, (IFile**)&tFile);
    tFile -> GetPath(pluginSharedDataDirectory);    
    return NOERROR;
}

/* package */
ECode CPluginManager::GetPluginClass(
        /* [in] */ const String& packageName, 
        /* [in] */ const String& className,
        /* [out] */ Handle32 * pluginClass)
{
    VALIDATE_NOT_NULL(pluginClass);
    AutoPtr<IContext> pluginContext;
    mContext -> CreateCapsuleContext(packageName,
            Context_CONTEXT_INCLUDE_CODE | Context_CONTEXT_IGNORE_SECURITY,
            (IContext**)&pluginContext);
    AutoPtr<IClassLoader> pluginCL;    //IClassLoader appreared in Elastos.Framework.Core.tmp   But it is not declared in the library of the Framework
    pluginContext -> GetClassLoader( (IClassLoader**)&pluginCL );
    pluginCL -> LoadClass(className,pluginClass);
    return NOERROR;
}

ECode CPluginManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;

    AutoPtr<ISignature> pSignatureT;    
    ECode ec = CSignature::New(SIGNATURE_1,(ISignature**)&pSignatureT);
    if(FAILED(ec)) {
        return ec;
    }
    (*(SIGNATURES.Get()))[0] = pSignatureT;

    return NOERROR;
}

