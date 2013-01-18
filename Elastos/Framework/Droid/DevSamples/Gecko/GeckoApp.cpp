
#include "GeckoApp.h"
#include <elastos/ElRefBase.h>
#include "GeckoRunnable.h"
#include "GeckoAppShell.h"
#include <stdio.h>
//#include "CGeckoConnectivityReceiver.h"

IFrameLayout* GeckoApp::mainLayout = NULL;
CGeckoSurfaceView* GeckoApp::surfaceView = NULL;
GeckoApp* GeckoApp::mAppContext = NULL;
Boolean GeckoApp::mFullscreen = FALSE;
IFile* GeckoApp::sGREDir = NULL;
GeckoApp::LaunchState GeckoApp::sLaunchState = GeckoApp::LaunchState_PreLaunch;
Mutex GeckoApp::sSyncLaunchState;

GeckoApp::GeckoApp()
    : mLibLoadThread(NULL)
    , mMainHandler(NULL)
    , haveKilledZombies(FALSE)
    , mLaunchButton(NULL)
    , mLaunchIntent(NULL)
    , mConnectivityFilter(NULL)
    , mConnectivityReceiver(NULL)
{
}

GeckoApp::~GeckoApp()
{
    if (mainLayout) {
        mainLayout->Release();
        mainLayout = NULL;
    }
    if (surfaceView) {
        surfaceView->Probe(EIID_ISurfaceView)->Release();
        surfaceView = NULL;
    }
    if (sGREDir) {
        sGREDir->Release();
        sGREDir = NULL;
    }
    if (mMainHandler) {
        mMainHandler->Release();
        mMainHandler = NULL;
    }
    if (mLibLoadThread) {
        mLibLoadThread->Release();
        mLibLoadThread = NULL;
    }
    if (mLaunchButton) {
        mLaunchButton->Release();
        mLaunchButton = NULL;
    }
    if (mLaunchIntent) {
        mLaunchIntent->Release();
        mLaunchIntent = NULL;
    }
    if (mConnectivityFilter) {
        mConnectivityFilter->Release();
        mConnectivityFilter = NULL;
    }
    if (mConnectivityReceiver) {
        mConnectivityReceiver->Release();
        mConnectivityReceiver = NULL;
    }
}

ECode GeckoApp::CheckLaunchState(
    /* [in] */ LaunchState checkState,
    /* [out] */ Boolean* pResult)
{
    if (!pResult) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(sSyncLaunchState);
    *pResult = (sLaunchState == checkState);
    return NOERROR;
}

ECode GeckoApp::SetLaunchState(
    /* [in] */ LaunchState setState)
{
    Mutex::Autolock lock(sSyncLaunchState);
    sLaunchState = setState;
    return NOERROR;
}

// if mLaunchState is equal to checkState this sets mLaunchState to setState
// and return true. Otherwise we return false.
ECode GeckoApp::CheckAndSetLaunchState(
    /* [in] */ LaunchState checkState,
    /* [in] */ LaunchState setState,
    /* [out] */ Boolean* pResult)
{
    if (!pResult) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(sSyncLaunchState);
    if (sLaunchState != checkState) {
        *pResult = FALSE;
    }
    sLaunchState = setState;
    *pResult = TRUE;
    return NOERROR;
}

ECode GeckoApp::ShowErrorDialog(
    /* [in] */ const String& message)
{
    /*new AlertDialog.Builder(this)
            .setMessage(message)
            .setCancelable(false)
            .setPositiveButton(0x7f05000d, //R.string.exit_label,
                               new DialogInterface.OnClickListener() {
                                   public void onClick(DialogInterface dialog,
                                                       int id)
                                   {
                                       GeckoApp.this.finish();
                                       System.exit(0);
                                   }
                               }).show();*/
     return E_NOT_IMPLEMENTED;
}

class LaunchRunnable : public GeckoRunnable
{
public:
    LaunchRunnable(
        /* [in] */ GeckoApp* pHost)
        : GeckoRunnable((Void*)pHost)    
    {
    }

    ECode Run()
    {
        Int64 startup_time;
        ECode ec;// = System::CurrentTimeMillis(&startup_time);
        //if (FAILED(ec)) return ec;

        GeckoApp* pHost = (GeckoApp*)mHost;
        //try {
            if (!pHost->mLibLoadThread) {
                pHost->mLibLoadThread->Join();
            }
        //} catch (InterruptedException ie) {}
        IResources* pIRes;
        ec = pHost->GetResources(&pIRes);
        if (FAILED(ec)) return ec;
        ec = pIRes->GetString(0x7f050000, //R.string.splash_screen_loading,
                         &GeckoApp::surfaceView->sSplashStatusMsg);
        pIRes->Release();
        if (FAILED(ec)) return ec;
        GeckoApp::surfaceView->DrawSplashScreen();
        // unpack files in the components directory
        //try {
        ec = pHost->UnpackComponents();
        if (ec == E_FILE_NOT_FOUND_EXCEPTION) {
            printf("GeckoApp error unpacking components: file not found\n");
            //Looper:Prepare();
            String error;
            pHost->GetString(0x7f050004, &error); //R.string.error_loading_file
            pHost->ShowErrorDialog(error);
            //Looper::Loop();
            return NOERROR;
        }
        else if (ec == E_IO_EXCEPTION){
            printf("GeckoApp error unpacking components: io error\n");
            String msg("io error");
            //Looper::Prepare();
            /*if (msg != null && msg.equalsIgnoreCase("No space left on device"))
                showErrorDialog(getString(0x7f050003)); //R.string.no_space_to_start_error
            else
                showErrorDialog(getString(0x7f050004)); //R.string.error_loading_file*/
            //Looper::Loop();
            return NOERROR;
        }
        /*} catch (FileNotFoundException fnfe) {
            Log.e("GeckoApp", "error unpacking components", fnfe);
            Looper.prepare();
            showErrorDialog(getString(0x7f050004));//R.string.error_loading_file
            Looper.loop();
            return;
        } catch (IOException ie) {
            Log.e("GeckoApp", "error unpacking components", ie);
            String msg = ie.getMessage();
            Looper.prepare();
            if (msg != null && msg.equalsIgnoreCase("No space left on device"))
                showErrorDialog(getString(R.string.no_space_to_start_error));
            else
                showErrorDialog(getString(R.string.error_loading_file));
            Looper.loop();
            return;
        }*/

        // and then fire us up
        //try {
            String env;
            ec = pHost->mLaunchIntent->GetStringExtra(String("env0"), &env);
            if (FAILED(ec)) return ec;

            IApplication* pApp;
            ec = pHost->GetApplication(&pApp);
            if (FAILED(ec)) return ec;
            String resPath;
            //ec = pApp->GetPackageResourcePath(resPath&);
            pApp->Release();
            if (FAILED(ec)) return ec;

            String args, data;
            ec = pHost->mLaunchIntent->GetStringExtra(String("args"), &args);
            if (FAILED(ec)) return ec;
            //ec = pHost->mLaunchIntent->GetDataString(&data);
            if (FAILED(ec)) return ec;
            ec = GeckoAppShell::RunGecko(resPath, args, data);
            if (FAILED(ec)) {
                printf("GeckoApp top level exception RunGecko %x\n", ec);
                String error("run gecko failed");
                GeckoAppShell::ReportJavaCrash(error);
            }
        /*} catch (Exception e) {
            Log.e("GeckoApp", "top level exception", e);
            StringWriter sw = new StringWriter();
            e.printStackTrace(new PrintWriter(sw));
            GeckoAppShell.reportJavaCrash(sw.toString());
        }*/
        return NOERROR;
    }

};

// Returns true when the intent is going to be handled by gecko launch
ECode GeckoApp::Launch(
    /* [in] */ IIntent* pIntent,
    /* [out] */ Boolean* pResult)
{
    if (!pResult) return E_INVALID_ARGUMENT;

    Boolean result;
    CheckAndSetLaunchState(LaunchState_Launching, LaunchState_Launched, &result);
    if (!result) {
        *pResult = FALSE;
        return NOERROR;
    }

    ECode ec;
    if (!pIntent) {
        pIntent = GetIntent();
        if (FAILED(ec)) return ec;
    }
    mLaunchIntent = pIntent;
    mLaunchIntent->AddRef();

    LaunchRunnable* pRunnable = new LaunchRunnable(this);
    if (!pRunnable) return E_OUT_OF_MEMORY;

    IThread* pThread;
    ec = CThread::New(IRunnable::Probe(pRunnable), &pThread);
    if (FAILED(ec)) {
        delete pRunnable;
        return ec;
    }
    pThread->Start();
    pThread->Release();

    *pResult = TRUE;
    return NOERROR;
}

class LoopRunnable : public GeckoRunnable
{
public:
    LoopRunnable(
        /* [in] */ GeckoApp* pHost)
        : GeckoRunnable((Void*)pHost)    
    {
    }

    ECode Run()
    {
        /*ECode ec = Looper::Loop();
        if (FAILED(ec)) {
            printf("GeckoApp top level exception Looper::Loop %x\n", ec);
            GeckoAppShell::ReportJavaCrash(String("Looper::Loop error"));
        }*/

        return NOERROR;
    }
};

class LibLoadRunnable : public GeckoRunnable
{
public:
    LibLoadRunnable(
        /* [in] */ GeckoApp* pHost)
        : GeckoRunnable((Void*)pHost)    
    {
    }

    ECode Run()
    {
        // At some point while loading the gecko libs our default locale gets set
        // so just save it to locale here and reset it as default after the join
        ILocale* pLocale;
        ECode ec;
        //ec = Locale::GetDefault(&pLocale);
        //if (FAILED(ec)) return ec;

        GeckoApp* pHost = (GeckoApp*)mHost;
        IApplication* pApp;
        String path;
        ec = pHost->GetApplication(&pApp);
        if (FAILED(ec)) goto exit;
        //ec = pApp->GetPackageResourcePath(&path);
        pApp->Release();
        if (FAILED(ec)) goto exit;
        ec = GeckoAppShell::LoadGeckoLibs(path);
        if (FAILED(ec)) goto exit;

        //ec = Locale::SetDefault(pLocale);
        //if (FAILED(ec)) goto exit;

        IContext* pIContext;
        ec = pHost->GetBaseContext(&pIContext);
        if (FAILED(ec)) goto exit;
        IResources* pRes;
        ec = pIContext->GetResources(&pRes);
        pIContext->Release();
        if (FAILED(ec)) goto exit;

        IConfiguration* pConfig;
        ec = pRes->GetConfiguration(&pConfig);
        if (FAILED(ec)) {
            pRes->Release();
            goto exit;
        }
        /*pConfig->SetLocale(pLocale);
        IDisplayMerics* pIDM;
        ec = pRes->GetDisplayMetrics(&pIDM);
        if (FAILED(ec)) {
            pRes->Release();
            pConfig->Release();
            goto exit;
        }
        pRes->UpdateConfiguration(pConfig, pIDM);
        pIDM->Release();*/
        pRes->Release();
        pConfig->Release();

exit:
        pLocale->Release();
        return NOERROR;
    }
};

ECode GeckoApp::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    mAppContext = this;
    ECode ec;
    /*ec = CHandler::New(&mMainHandler);
    if (FAILED(ec)) return ec;

    LoopRunnable* pRunnable = new LoopRunnable(this);
    if (!pRunnable) return E_OUT_OF_MEMORY;

    ec = mMainHandler->Post(IRunnable::Probe(pRunnable));
    if (FAILED(ec)) return ec;*/

    /*ISharedPreferences* pSettings;
    ec = GetPreferences(Activity_MODE_PRIVATE, &pSettings);
    if (FAILED(ec)) return ec;

    String packageName;
    GetPackageName(&packageName)
    packageName += ".locale";
    String localeCode;
    ec = pSettings->GetString(packageName, "", &localeCode);
    pSettings->Release();
    if (FAILED(ec)) return ec;
    if (!localeCode.IsNullOrEmpty()) {
        GeckoAppShell::SetSelectedLocale(localeCode);
    }*/

    printf("GeckoApp create\n");
    Activity::OnCreate(savedInstanceState);

    if (!sGREDir) {
        String dataDir;
        /*IApplicationInfo* pAppInfo;
        ec = GetApplicationInfo(&pAppInfo);
        if (FAILED(ec)) return ec;

        ec = pAppInfo->GetDataDir(&dataDir);
        pAppInfo->Release();
        if (FAILED(ec)) return ec;*/
        ec = CFile::New(dataDir, &sGREDir);
        if (FAILED(ec)) return ec;
    }

    IWindow* pIWindow;
    ec = GetWindowEx(&pIWindow);
    if (FAILED(ec)) return ec;

    ec = pIWindow->SetFlags(mFullscreen ?
                         WindowManagerLayoutParams_FLAG_FULLSCREEN : 0,
                         WindowManagerLayoutParams_FLAG_FULLSCREEN);
    pIWindow->Release();
    if (FAILED(ec)) return ec;

    if (!surfaceView) {
        ec = CGeckoSurfaceView::NewByFriend(this, &surfaceView);
    }
    else {
        ec = mainLayout->RemoveViewInLayout(surfaceView);
    }
    if (FAILED(ec)) return ec;

    ec = CFrameLayout::New(this, &mainLayout);
    if (FAILED(ec)) return ec;

    IFrameLayoutLayoutParams* pLayoutParams;
    ec = CFrameLayoutLayoutParams::New(ViewGroupLayoutParams_FILL_PARENT,
                        ViewGroupLayoutParams_FILL_PARENT, &pLayoutParams);
    if (FAILED(ec)) return ec;
    ec = mainLayout->AddViewEx3(surfaceView, pLayoutParams);
    pLayoutParams->Release();
    if (FAILED(ec)) return ec;

    IViewGroupLayoutParams* pVGroupLayoutParams;
    ec = CViewGroupLayoutParams::New(ViewGroupLayoutParams_FILL_PARENT,
                        ViewGroupLayoutParams_FILL_PARENT, &pVGroupLayoutParams);
    if (FAILED(ec)) return ec;
    SetContentView(mainLayout, pVGroupLayoutParams);
    pVGroupLayoutParams->Release();
    if (FAILED(ec)) return ec;

    if (!mConnectivityFilter) {
        ec = CIntentFilter::New(&mConnectivityFilter);
        if (FAILED(ec)) return ec;
    }
    mConnectivityFilter->AddAction(
                String(ConnectivityManager_CONNECTIVITY_ACTION));

    if (!mConnectivityReceiver) {
        //ec = CGeckoConnectivityReceiver::New(&mConnectivityReceiver);
        //if (FAILED(ec)) return ec;
    }

    Boolean result;
    GeckoApp::CheckAndSetLaunchState(LaunchState_PreLaunch,
                                LaunchState_Launching, &result);
    if (!result) return NOERROR;

    CheckAndLaunchUpdate();

    LibLoadRunnable* pLibLoadRunnable = new LibLoadRunnable(this);
    if (!pLibLoadRunnable) return E_OUT_OF_MEMORY;
    ec = CThread::New(IRunnable::Probe(pLibLoadRunnable), &mLibLoadThread);
    if (FAILED(ec)) {
        delete pLibLoadRunnable;
        return ec;
    }

    IFile* pCacheFile;
    ec = GeckoAppShell::GetCacheDir(&pCacheFile);
    if (FAILED(ec)) return ec;

    IFile* pLibxulFile;
    ec = CFile::New(pCacheFile, String("libxul.so"), &pLibxulFile);
    pCacheFile->Release();
    if (FAILED(ec)) return ec;

    IApplication* pIApplication;
    ec = GetApplication(&pIApplication);
    if (FAILED(ec)) {
        pLibxulFile->Release();
        return ec;
    }
    String resourcePath;
    //ec = pIApplication->GetPackageResourcePath(&resourcePath);
    pIApplication->Release();
    if (FAILED(ec)) {
        pLibxulFile->Release();
        return ec;
    }
    IFile* pRes;
    ec = CFile::New(resourcePath, &pRes);
    if (FAILED(ec)) {
        pLibxulFile->Release();
        return ec;
    }

    Int64 resModifyTime, xulModifyTime;
    pRes->LastModified(&resModifyTime);
    pRes->Release();
    pLibxulFile->LastModified(&xulModifyTime);

    Int32 freeSpace = 0;
    Boolean exist = FALSE;
    GeckoAppShell::GetFreeSpace(&freeSpace);
    pLibxulFile->Exists(&exist);
    pLibxulFile->Release();

    IResources* pIResources;
    ec = GetResources(&pIResources);
    if (FAILED(ec)) return ec;
    if (freeSpace > GeckoAppShell::kFreeSpaceThreshold &&
        (!exist || resModifyTime >= xulModifyTime)) {
        ec = pIResources->GetString(
                    0x7f050001, //R.string.splash_screen_installing_libs,
                    &surfaceView->sSplashStatusMsg);
    }
    else {
        ec = pIResources->GetString(
                    0x7f050000, //R.string.splash_screen_loading,
                    &surfaceView->sSplashStatusMsg);
    }
    pIResources->Release();
    if (FAILED(ec)) return ec;

    mLibLoadThread->Start();
    return NOERROR;
}

ECode GeckoApp::OnStart()
{
    printf("GeckoApp onstart\n");
    return Activity::OnStart();
}

ECode GeckoApp::OnResume()
{
    printf("GeckoApp resume\n");
    Boolean result, result2;
    CheckLaunchState(LaunchState_GeckoRunning, &result);
    if (result) {
        GeckoAppShell::OnResume();
    }
    // After an onPause, the activity is back in the foreground.
    // Undo whatever we did in onPause.
    Activity::OnResume();

    // Just in case. Normally we start in onNewIntent
    CheckLaunchState(LaunchState_PreLaunch, &result);
    CheckLaunchState(LaunchState_Launching, &result2);
    if (result || result2) {
        AutoPtr<IIntent> pIIntent = GetIntent();
        OnNewIntent(pIIntent);
    }

    //RegisterReceiver(mConnectivityReceiver, mConnectivityFilter);
    return NOERROR;
}

ECode GeckoApp::OnPause()
{
    printf("GeckoApp pause\n");
    GeckoEvent* pEvent = new GeckoEvent(GeckoEvent::ACTIVITY_PAUSING);
    if (!pEvent) return E_OUT_OF_MEMORY;
    GeckoAppShell::SendEventToGecko(pEvent);
    // The user is navigating away from this activity, but nothing
    // has come to the foreground yet; for Gecko, we may want to
    // stop repainting, for example.

    // Whatever we do here should be fast, because we're blocking
    // the next activity from showing up until we finish.

    // onPause will be followed by either onResume or onStop.
    Activity::OnPause();

    //UnregisterReceiver(mConnectivityReceiver);
    return NOERROR;
}

ECode GeckoApp::OnRestart()
{
    printf("GeckoApp restart\n");
    return Activity::OnRestart();
}

ECode GeckoApp::OnStop()
{
    printf("GeckoApp stop\n");
    // We're about to be stopped, potentially in preparation for
    // being destroyed.  We're killable after this point -- as I
    // understand it, in extreme cases the process can be terminated
    // without going through onDestroy.
    //
    // We might also get an onRestart after this; not sure what
    // that would mean for Gecko if we were to kill it here.
    // Instead, what we should do here is save prefs, session,
    // etc., and generally mark the profile as 'clean', and then
    // dirty it again if we get an onResume.
    GeckoEvent* pEvent = new GeckoEvent(GeckoEvent::ACTIVITY_STOPPING);
    if (!pEvent) return E_OUT_OF_MEMORY;
    GeckoAppShell::SendEventToGecko(pEvent);
    return Activity::OnStop();
}

ECode GeckoApp::OnDestroy()
{
    printf("GeckoApp destroy\n");
    // Tell Gecko to shutting down; we'll end up calling System.exit()
    // in onXreExit.
    Boolean isFinishing;
    //IsFinishing(&isFinishing)
    if (isFinishing) {
        GeckoEvent* pEvent = new GeckoEvent(GeckoEvent::ACTIVITY_SHUTDOWN);
        if (!pEvent) return E_OUT_OF_MEMORY;
        GeckoAppShell::SendEventToGecko(pEvent);
    }

    return Activity::OnDestroy();
}

ECode GeckoApp::OnConfigurationChanged(
    /* [in] */ IConfiguration* pNewConfig)
{
    printf("GeckoApp configuration changed\n");
    // nothing, just ignore
    //return Activity::OnConfigurationChanged(pNewConfig);
    return NOERROR;
}

ECode GeckoApp::OnLowMemory()
{
    printf("GeckoApp low memory\n");
    Boolean result;
    CheckLaunchState(LaunchState_GeckoRunning, &result);
    if (result) {
        GeckoAppShell::OnLowMemory();
    }

    //return Activity::OnLowMemory();
    return NOERROR;
}

ECode GeckoApp::RemoveFiles()
{
    IFile* pIFile;
    ECode ec = CFile::New(sGREDir, String("removed-files"), &pIFile);
    if (FAILED(ec)) return ec;
    IInputStreamReader* pIFileReader;
    ec = CFileReader::New(pIFile, &pIFileReader);
    pIFile->Release();
    if (FAILED(ec)) return ec;
    IBufferedReader* pReader;
    ec = CBufferedReader::New(pIFileReader, &pReader);
    pIFileReader->Release();
    if (FAILED(ec)) return ec;

    //try {
        String removedFileName;
        IFile* pRemovedFile;
        Boolean exist, isDeleted;
        ec = pReader->ReadLine(&removedFileName);
        while(SUCCEEDED(ec) && !removedFileName.IsNullOrEmpty()) {
            ec = CFile::New(sGREDir, removedFileName, &pRemovedFile);
            if (FAILED(ec)) goto exit;
            pRemovedFile->Exists(&exist);
            if (exist) {
                pRemovedFile->Delete(&isDeleted);
            }
            pRemovedFile->Release();
            ec = pReader->ReadLine(&removedFileName);
        }
    //} finally {
    //    reader.close();
    //}
exit:
    pReader->Close();
    pReader->Release();
    return ec;
}

ECode GeckoApp::AddEnvToIntent(
    /* [in] */ IIntent* pIntent)
{
    /*Map<String,String> envMap = System.getenv();
    Set<Map.Entry<String,String>> envSet = envMap.entrySet();
    Iterator<Map.Entry<String,String>> envIter = envSet.iterator();
    StringBuffer envstr = new StringBuffer();
    int c = 0;
    while (envIter.hasNext()) {
        Map.Entry<String,String> entry = envIter.next();
        intent.putExtra("env" + c, entry.getKey() + "="
                        + entry.getValue());
        c++;
    }*/

    return E_NOT_IMPLEMENTED;
}

ECode GeckoApp::DoRestart()
{
    //try {
        String action("org.mozilla.gecko.restart");
        IIntent* pIIntent;
        ECode ec = CIntent::New(action, &pIIntent);
        if (FAILED(ec)) return ec;
        String packageName;
        GetPackageName(&packageName);
        pIIntent->SetClassName(packageName, packageName + ".Restarter");

        ec = AddEnvToIntent(pIIntent);
        if (FAILED(ec)) {
            pIIntent->Release();
            return ec;
        }

        pIIntent->SetFlags(Intent_FLAG_ACTIVITY_NEW_TASK |
                        Intent_FLAG_ACTIVITY_MULTIPLE_TASK);
        //printf("GeckoAppJava %s\n", pIIntent->ToString());
        GeckoAppShell::KillAnyZombies();
        ec = StartActivity(pIIntent);
        pIIntent->Release();
        if (FAILED(ec)) return ec;

    //} catch (Exception e) {
    //    Log.i("GeckoAppJava", "error doing restart", e);
    //}
    Finish();
    // Give the restart process time to start before we die
    GeckoAppShell::WaitForAnotherGeckoProc();
    return NOERROR;
}

ECode GeckoApp::HandleNotification(
    /* [in] */ const String& action,
    /* [in] */ const String& alertName,
    /* [in] */ const String& alertCookie)
{
    return GeckoAppShell::HandleNotification(action, alertName, alertCookie);
}

ECode GeckoApp::ShowFilePicker(
    /* [in] */ const String& aMimeType,
    /* [out] */ String* pFilePickerResult)
{
    /*IIntent* pIIntent;
    ECode ec = CIntent::New(Intent_ACTION_GET_CONTENT, &pIIntent);
    if (FAILED(ec)) return ec;

    pIIntent->AddCategory(Intent_CATEGORY_OPENABLE);
    pIIntent->SetType(aMimeType);
    IIntent* pChooser;
    ec = Intent::CreateChooser(pIIntent, "choose a file", &pChooser);
    pIIntent->Release();
    if (FAILED(ec)) return ec;

    ec = StartActivityForResult(pChooser, GeckoApp_FILE_PICKER_REQUEST);
    pChooser->Release();
    if (FAILED(ec)) return ec;*/

    //try {
        //mFilePickerResult.Take(pFilePickerResult);
    //} catch (InterruptedException e) {
    //    Log.i("GeckoApp", "showing file picker ",  e);
    //}

    return NOERROR;
}

class CButtonClickListener : public ElRefBase
                           , public IViewOnClickListener
{
public:
    CButtonClickListener(
        /* [in] */ GeckoApp* pHost)
        : mHost(pHost)
    {
    }

    UInt32 AddRef()
    {
        return ElRefBase::AddRef();
    }

    UInt32 Release()
    {
        return ElRefBase::Release();
    }

    PInterface Probe(
        /* [in] */ REIID riid)
    {
        if (riid == EIID_IInterface) {
            return (PInterface)(IViewOnClickListener*)this;
        }
        else if (riid == EIID_IViewOnClickListener) {
            return (IViewOnClickListener*)this;
        }

        return NULL;
    }

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* pObject,
        /* [in] */ InterfaceID* pIID)
    {
        return E_NOT_IMPLEMENTED;
    }

    ECode OnClick (
        /* [in] */ IView* v)
    {
        // hide the button so we can't be launched again
        mHost->mainLayout->RemoveViewInLayout(mHost->mLaunchButton);
        mHost->SetLaunchState(GeckoApp::LaunchState_Launching);
        Boolean result;
        mHost->Launch(NULL, &result);

        return NOERROR;
    }

private:
    GeckoApp* mHost;
};

ECode GeckoApp::OnNewIntent(
    /* [in] */ IIntent* pIntent)
{
    Boolean result;
    CheckLaunchState(LaunchState_GeckoExiting, &result);
    if (result) {
        // We're exiting and shouldn't try to do anything else just incase
        // we're hung for some reason we'll force the process to exit
        //System.exit(0);
        return NOERROR;
    }

    String action;
    ECode ec = pIntent->GetAction(&action);
    if (FAILED(ec)) return ec;

    CheckLaunchState(LaunchState_Launching, &result);
    if (action.Equals("org.mozilla.gecko.DEBUG")) {
        SetLaunchState(LaunchState_WaitButton);
        if (result) {
            ec = CButton::New(this, &mLaunchButton);
            if (FAILED(ec)) return ec;

            ICharSequence* pText;
            ec = CStringWrapper::New(String("Launch"), &pText);
            if (FAILED(ec)) return ec;

            mLaunchButton->SetText(pText); // don't need to localize
            pText->Release();

            CButtonClickListener* listener = new CButtonClickListener(this);
            if (!listener) return E_OUT_OF_MEMORY;
            mLaunchButton->SetOnClickListener(IViewOnClickListener::Probe(listener));
            mainLayout->AddViewEx2(mLaunchButton, 300, 200);

            return NOERROR;
        }
    }

    CheckLaunchState(LaunchState_WaitButton, &result);
    if (result) return NOERROR;

    ec = Launch(pIntent, &result);
    if (result) return NOERROR;

    if (1) {//action.Equals(Intent_ACTION_VIEW)) {
        String uri;
        //ec = pIntent->GetDataString(&uri);
        //if (FAILED(ec)) return ec;
        GeckoEvent* pEvent = new GeckoEvent(uri);
        if (!pEvent) return E_OUT_OF_MEMORY;        
        GeckoAppShell::SendEventToGecko(pEvent);
        printf("GeckoApp onNewIntent: %s\n", uri.string());
    }
    else if (action.Equals(Intent_ACTION_MAIN)) {
        printf("GeckoApp Intent : ACTION_MAIN\n");
        GeckoEvent* pEvent = new GeckoEvent(String(""));
        if (!pEvent) return E_OUT_OF_MEMORY;
        GeckoAppShell::SendEventToGecko(pEvent);
    }
    else if (action.Equals("org.mozilla.fennec.WEBAPP")) {
        String uri;
        ec = pIntent->GetStringExtra(String("args"), &uri);
        if (FAILED(ec)) return ec;
        GeckoEvent* pEvent = new GeckoEvent(uri);
        if (!pEvent) return E_OUT_OF_MEMORY;
        GeckoAppShell::SendEventToGecko(pEvent);
        printf("GeckoApp Intent : WEBAPP - %s\n", uri.string());
    }

    return NOERROR;
}

ECode GeckoApp::UnpackComponents()
{
    IZipFile* pZip;
    IInputStream* pListStream;

    IFile* pComponentsDir;
    ECode ec = CFile::New(sGREDir, String("components"), &pComponentsDir);
    if (FAILED(ec)) return ec;
    Boolean succeeded;
    pComponentsDir->Mkdir(&succeeded);
    pComponentsDir->Release();

    IApplication* pIApplication;
    ec = GetApplication(&pIApplication);
    if (FAILED(ec)) return ec;
    String resPath;
    //ec = pIApplication->GetPackageResourcePath(&resPath);
    pIApplication->Release();
    if (FAILED(ec)) return ec;

    ec = CZipFile::New(resPath, &pZip);
    if (FAILED(ec)) return ec;

    ArrayOf_<Byte, 8192> buf;
    IZipEntry* pEntry;
    String name;
    String processName;
    //try {
    ec = UnpackFile(pZip, &buf, NULL, String("removed-files"), &succeeded);
    if (FAILED(ec)) {
         // This file may not be there, so just log any errors and move on
        printf("GeckoApp error removing files\n");
        goto exit;
    }
    if (succeeded) {
        RemoveFiles();
    }
    //} catch (Exception ex) {
        // This file may not be there, so just log any errors and move on
    ///    Log.w("GeckoApp", "error removing files", ex);
    //}

    ec = UnpackFile(pZip, &buf, NULL, String("application.ini"), &succeeded);
    if (FAILED(ec)) goto exit;

    GetContentProcessName(&processName);
    ec = UnpackFile(pZip, &buf, NULL, processName, &succeeded);
    if (FAILED(ec)) goto exit;

    //try {
        ec = UnpackFile(pZip, &buf, NULL, String("update.locale"), &succeeded);
        if (FAILED(ec)) goto exit;
    //} catch (Exception e) {/* this is non-fatal */}

    // copy any .xpi file into an extensions/ directory
    IObjectContainer* pContainer;
    ec = pZip->GetEntries(&pContainer);
    if (FAILED(ec)) goto exit;

    IObjectEnumerator* pEnum;
    ec = pContainer->GetObjectEnumerator(&pEnum);
    pContainer->Release();
    if (FAILED(ec)) goto exit;

    ec = pEnum->MoveNext(&succeeded);
    while (SUCCEEDED(ec) && succeeded) {
        ec = pEnum->Current((IInterface**)&pEntry);
        if (FAILED(ec)) {
            pEnum->Release();
            goto exit;
        }
        pEntry->GetName(&name);
        if (name.StartWith("extensions/") && name.EndWith(".xpi")) {
            printf("GeckoAppJava installing extension : %s\n", name.string());
            ec = UnpackFile(pZip, &buf, pEntry, name, &succeeded);
            if (FAILED(ec)) {
                pEnum->Release();
                goto exit;
            }
        }
        pEntry->Release();
        ec = pEnum->MoveNext(&succeeded);
    }

    // copy any hyphenation dictionaries file into a hyphenation/ directory
    pEnum->Reset();
    ec = pEnum->MoveNext(&succeeded);
    while (SUCCEEDED(ec) && succeeded) {
        ec = pEnum->Current((IInterface**)&pEntry);
        if (FAILED(ec)) {
            pEnum->Release();
            goto exit;
        }
        pEntry->GetName(&name);
        if (name.StartWith("hyphenation/")) {
            printf("GeckoAppJava installing hyphenation : %s\n", name.string());
            UnpackFile(pZip, &buf, pEntry, name, &succeeded);
            if (FAILED(ec)) {
                pEnum->Release();
                goto exit;
            }
        }
        pEntry->Release();
        ec = pEnum->MoveNext(&succeeded);
    }
    pEnum->Release();

exit:
    pZip->Release();
    return ec;
}

ECode GeckoApp::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* pData)
{
    ECode ec = NOERROR;
    String filePickerResult("");
    if (pData && resultCode == Activity_RESULT_OK) {
        //try {
            IContentResolver* pCr;
            IUri* pUri = NULL;
            IFile* pIFile = NULL;
            IFileOutputStream* pFos = NULL;
            IInputStream* pIs = NULL;
            
            String fileName("tmp_");
            String fileExt;
            Int32 period;
            String name;
            ArrayOf_<String, 1> queryColumns;
            ArrayOf_<Byte, 4096> buf;
            Int32 len;

            ECode ec = GetContentResolver(&pCr);
            if (FAILED(ec)) return ec;

            ec = pData->GetData(&pUri);
            if (FAILED(ec)) goto exit;

            ICursor* pCursor;
            //queryColumns[0] = OpenableColumns_DISPLAY_NAME;
            ec = pCr->Query(pUri, queryColumns,
                        String(""), NULL_ARRAYOF(String), String(""), &pCursor);
            if (FAILED(ec)) goto exit;

            if (pCursor) {
                //try {
                    Boolean succeeded;
                    pCursor->MoveToNext(&succeeded);
                    if (succeeded) {
                        pCursor->GetString(0, &name);
                    }
                    pCursor->Close();
                    pCursor->Release();
                //} finally {
                //    cursor.close();
                //}
            }
            if (name.IsNull() || (period = name.LastIndexOf('.')) == -1) {
                String mimeType;
                ec = pCr->GetType(pUri, &mimeType);
                if (FAILED(ec)) goto exit;

                String extension;
                GeckoAppShell::GetExtensionFromMimeType(mimeType, &extension);
                fileExt = ".";
                fileExt += extension;
            } else {
                fileExt = name.Substring(period);
                fileName = name.Substring(0, period);
            }


            //ec = File::CreateTempFile(fileName, fileExt, sGREDir, &pIFile);
            //if (FAILED(ec)) goto exit;

            ec = CFileOutputStream::New(pIFile, &pFos);
            if (FAILED(ec)) goto exit;

            ec = pCr->OpenInputStream(pUri, &pIs);
            if (FAILED(ec)) {
                pFos->Release();
                goto exit;
            }

            ec = pIs->ReadBuffer(&buf, &len);
            if (FAILED(ec)) goto exit;
            while (len != -1) {
                ec = pFos->WriteBufferEx(0, len, buf);
                if (FAILED(ec)) goto exit;
                ec = pIs->ReadBuffer(&buf, &len);
                if (FAILED(ec)) goto exit;
            }
            pIFile->GetAbsolutePath(&filePickerResult);
exit:
            if (pFos) {
                pFos->Close();
                pFos->Release();
            }
            if (pIs) {
                pIs->Close();
                pIs->Release();
            }
            if (pIFile) {
                pIFile->Release();
            }
            if (pUri) {
                pUri->Release();
            }
            if (pCr) {
                pCr->Release();
            }
        //}catch (Exception e) {
        //    Log.e("GeckoApp", "showing file picker", e);
        //}
    }

    //try {
        //mFilePickerResult.Put(filePickerResult);
    //} catch (InterruptedException e) {
    //    Log.i("GeckoApp", "error returning file picker result", e);
    //}
    return ec;
}

ECode GeckoApp::UnpackFile(
    /* [in] */ IZipFile* pZip,
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ IZipEntry* pFileEntry,
    /* [in] */ const String& name,
    /* [out] */ Boolean* pResult)
{
    if (!pZip || pResult) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec;
    if (!pFileEntry) {
        ec = pZip->GetEntry(name, &pFileEntry);
        if (FAILED(ec)) return ec;
    }
    else {
        pFileEntry->AddRef();
    }
    if (!pFileEntry) {
        String zipName;
        pZip->GetName(&zipName);
        printf("Can't find %s in %s\n", name.string(), zipName.string());
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    IFile* pOutFile = NULL;
    IFileOutputStream* pOutStream = NULL;
    ec = CFile::New(sGREDir, name, &pOutFile);
    if (FAILED(ec)) goto exit;

    Boolean exist;
    pOutFile->Exists(&exist);
    Int64 modified, entryTime;
    pOutFile->LastModified(&modified);
    pFileEntry->GetTime(&entryTime);
    Int64 length, size;
    pOutFile->GetLength(&length);
    pFileEntry->GetSize(&size);
    if (exist && modified == entryTime && length == size) {
        *pResult = FALSE;
        goto exit;
    }

    IResources* pIRes;
    ec = GetResources(&pIRes);
    if (FAILED(ec)) goto exit;
    ec = pIRes->GetString(0x7f050002, //R.string.splash_firstrun,
                    &surfaceView->sSplashStatusMsg);
    pIRes->Release();
    if (FAILED(ec)) goto exit;
    surfaceView->DrawSplashScreen();

    if (!haveKilledZombies) {
        haveKilledZombies = TRUE;
        GeckoAppShell::KillAnyZombies();
    }

    IFile* pDir;
    ec = pOutFile->GetParentFile(&pDir);
    if (FAILED(ec)) goto exit;
    pOutFile->Exists(&exist);
    if (!exist) {
        Boolean succeeded;
        pDir->Mkdirs(&succeeded);
    }
    pDir->Release();

    IInputStream* pFileStream;
    ec = pZip->GetInputStream(pFileEntry, &pFileStream);
    if (FAILED(ec)) goto exit;

    ec = CFileOutputStream::New(pOutFile, &pOutStream);
    if (FAILED(ec)) goto exit2;

    Int32 number, readCount;
    ec = pFileStream->Available(&number);
    while (SUCCEEDED(ec) && number > 0) {
        ec = pFileStream->ReadBufferEx(0, buf->GetLength(), buf, &readCount);
        if (FAILED(ec)) goto exit2;

        ec = pOutStream->WriteBufferEx(0, readCount, *buf);
        if (FAILED(ec)) goto exit2;

        ec = pFileStream->Available(&number);
    }
    Boolean result;
    pOutFile->SetLastModified(entryTime, &result);
    *pResult = TRUE;

exit2:
    pFileStream->Close();
    pFileStream->Release();
    if (pOutStream) {
        pOutStream->Close();
        pOutStream->Release();
    }

exit:
    if (pOutFile) pOutFile->Release();
    pFileEntry->Release();
    return ec;
}

ECode GeckoApp::CheckAndLaunchUpdate()
{
    printf("GeckoAppJava Checking for an update\n");

    ECode ec;
    Int32 statusCode = 8; // UNEXPECTED_ERROR
    IFile* pBaseUpdateDir = NULL;
    /*if (Build_VERSION.SDK_INT >= 8) {
        ec = GetExternalFilesDir(
                Environment_DIRECTORY_DOWNLOADS, &pBaseUpdateDir);
    }
    else {
        IFile* pIFile;
        ec = Environment::GetExternalStorageDirectory(&pIFile);
        if (FAILED(ec)) return ec;

        String path;
        pIFile->GetPath(&path);
        pIFile->Release();
        ec = CFile::New(path, String("download"), &pBaseUpdateDir);
    }
    if (FAILED(ec)) return ec;*/

    IFile* pTemp;
    IFile* pUpdateDir;
    ec = CFile::New(pBaseUpdateDir, String("updates"), &pTemp);
    pBaseUpdateDir->Release();
    if (FAILED(ec)) return ec;
    ec = CFile::New(pTemp, String("0"), &pUpdateDir);
    pTemp->Release();
    if (FAILED(ec)) return ec;

    IFile *pUpdateFile = NULL, *pStatusFile = NULL;
    Boolean exist;
    String updateStatus;
    String status;
    String packageName;
    ec = CFile::New(pUpdateDir, String("update.apk"), &pUpdateFile);
    if (FAILED(ec)) goto exit;
    ec = CFile::New(pUpdateDir, String("update.status"), &pStatusFile);
    if (FAILED(ec)) goto exit;

    pStatusFile->Exists(&exist);
    ReadUpdateStatus(pStatusFile, &updateStatus);
    if (!exist || !updateStatus.Equals("pending")) {
        goto exit;
    }

    pUpdateFile->Exists(&exist);
    if (!exist) goto exit;

    printf("GeckoAppJava Update is available!\n");

    // Launch APK
    GetPackageName(&packageName);
    IFile* pUpdateFileToRun;
    ec = CFile::New(pUpdateDir, packageName + "-update.apk", &pUpdateFileToRun);
    if (FAILED(ec)) goto exit;
    //try {
        Boolean succeeded;
        ec = pUpdateFile->RenameTo(pUpdateFileToRun, &succeeded);
        if (FAILED(ec)) {
            pUpdateFileToRun->Release();
            goto exit;
        }
        if (succeeded) {
            String path;
            pUpdateFileToRun->GetPath(&path);
            String amCmd("/system/bin/am start -a android.intent.action.VIEW ");
            amCmd += "-n com.android.packageinstaller/.PackageInstallerActivity -d file://";
            amCmd += path;
            printf("GeckoAppJava %s\n", amCmd.string());
            /*IRuntime* pIRuntime;
            ec = CRuntime::GetRuntime(&pIRuntime);
            if (FAILED(ec)) {
                pUpdateFileToRun->Release();
                goto exit;
            }
            pIRuntime->Exec(amCmd);
            pIRuntime->Release();*/
            statusCode = 0; // OK
        }
        else {
            printf("GeckoAppJava Cannot rename the update file!\n");
            statusCode = 7; // WRITE_ERROR
        }
        pUpdateFileToRun->Release();
    //} catch (Exception e) {
    //    Log.i("GeckoAppJava", "error launching installer to update", e);
    //}

    // Update the status file
    if (statusCode == 0) {
        status = "succeeded\n";
    }
    else {
        status = "failed: ";
        status += + statusCode + "\n";
    }

    IFileOutputStream* pIOutStream;
    //try {
    {
        ArrayOf<Byte> buf((Byte*)status.string(), status.GetLength());

        ec = CFileOutputStream::New(pStatusFile, &pIOutStream);
        if (FAILED(ec)) goto exit;

        ec = pIOutStream->WriteBufferEx(0, buf.GetLength(), buf);
        pIOutStream->Close();
        pIOutStream->Release();
        if (FAILED(ec)) goto exit;
    }
    //} catch (Exception e) {
    //    Log.i("GeckoAppJava", "error writing status file", e);
    //}

    if (statusCode == 0) {
    //    System::Exit(0);
    }

exit:
    if (pUpdateFile) {
        pUpdateFile->Release();
    }
    if (pStatusFile) {
        pStatusFile->Release();
    }
    pUpdateDir->Release();
    return ec;
}

ECode GeckoApp::ReadUpdateStatus(
    /* [in] */ IFile* pStatusFile,
    /* [out] */ String* pStatus)
{
    //try {
        IInputStreamReader* pIFileReader;
        ECode ec = CFileReader::New(pStatusFile, &pIFileReader);
        if (FAILED(ec)) return ec;

        IBufferedReader* pReader;
        ec = CBufferedReader::New(pIFileReader, &pReader);
        pIFileReader->Release();
        if (FAILED(ec)) return ec;

        ec = pReader->ReadLine(pStatus);
        pReader->Close();
        pReader->Release();
    //} catch (Exception e) {
    //    Log.i("GeckoAppJava", "error reading update status", e);
    //}
    return ec;
}

