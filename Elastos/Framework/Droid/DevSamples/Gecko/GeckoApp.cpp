
#include "cmdef.h"
#include "GeckoApp.h"
#include "GeckoRunnable.h"
#include "GeckoAppShell.h"
#include <stdio.h>
#include <elastos/ElRefBase.h>
//#include "CGeckoConnectivityReceiver.h"

class LaunchRunnable : public GeckoRunnable
{
public:
    LaunchRunnable(
        /* [in] */ GeckoApp* host)
        : GeckoRunnable((Void*)host)
    {}

    ECode Run()
    {
        Int64 startup_time;
        ECode ec;// = System::CurrentTimeMillis(&startup_time);
        //if (FAILED(ec)) return ec;

        GeckoApp* host = (GeckoApp*)mHost;
        //try {
            if (!host->mLibLoadThread) {
                host->mLibLoadThread->Join();
            }
        //} catch (InterruptedException ie) {}
        AutoPtr<IResources> res;
        FAIL_RETURN(host->GetResources((IResources**)&res));
        FAIL_RETURN(res->GetString(0x7f050000, //R.string.splash_screen_loading,
                         &GeckoApp::sSurfaceView->sSplashStatusMsg));
        GeckoApp::sSurfaceView->DrawSplashScreen();
        // unpack files in the components directory
        //try {
        ec = host->UnpackComponents();
        if (ec == E_FILE_NOT_FOUND_EXCEPTION) {
            printf("GeckoApp error unpacking components: file not found\n");
            //Looper:Prepare();
            String error;
            host->GetString(0x7f050004, &error); //R.string.error_loading_file
            host->ShowErrorDialog(error);
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
            FAIL_RETURN(host->mLaunchIntent->GetStringExtra(String("env0"), &env));

            AutoPtr<IApplication> app;
            FAIL_RETURN(host->GetApplication((IApplication**)&app));
            String resPath;
            //ec = pApp->GetPackageResourcePath(resPath&);

            String args, data;
            FAIL_RETURN(host->mLaunchIntent->GetStringExtra(String("args"), &args));
            //FAIL_RETURN(host->mLaunchIntent->GetDataString(&data));
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

class LoopRunnable : public GeckoRunnable
{
public:
    LoopRunnable(
        /* [in] */ GeckoApp* host)
        : GeckoRunnable((Void*)host)
    {}

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
        /* [in] */ GeckoApp* host)
        : GeckoRunnable((Void*)host)
    {
    }

    ECode Run()
    {
        // At some point while loading the gecko libs our default locale gets set
        // so just save it to locale here and reset it as default after the join
        AutoPtr<ILocale> locale;
        //ec = Locale::GetDefault(&pLocale);
        //if (FAILED(ec)) return ec;

        GeckoApp* host = (GeckoApp*)mHost;
        AutoPtr<IApplication> app;
        String path;
        FAIL_RETURN(host->GetApplication((IApplication**)&app));
        //ec = pApp->GetPackageResourcePath(&path);
        FAIL_RETURN(GeckoAppShell::LoadGeckoLibs(path));

        //ec = Locale::SetDefault(pLocale);
        //if (FAILED(ec)) goto exit;

        AutoPtr<IContext> context;
        FAIL_RETURN(host->GetBaseContext((IContext**)&context));
        AutoPtr<IResources> res;
        FAIL_RETURN(context->GetResources((IResources**)&res));

        AutoPtr<IConfiguration> config;
        FAIL_RETURN(res->GetConfiguration((IConfiguration**)&config));
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

        return NOERROR;
    }
};

AutoPtr<IFrameLayout> GeckoApp::sMainLayout;
AutoPtr<CGeckoSurfaceView> GeckoApp::sSurfaceView;
AutoPtr<GeckoApp> GeckoApp::sAppContext;
Boolean GeckoApp::sFullscreen = FALSE;
AutoPtr<IFile> GeckoApp::sGREDir;
GeckoApp::LaunchState GeckoApp::sLaunchState = GeckoApp::LaunchState_PreLaunch;
Mutex GeckoApp::sSyncLaunchState;

GeckoApp::GeckoApp()
    : mHaveKilledZombies(FALSE)
{
}

GeckoApp::~GeckoApp()
{
    // if (sSurfaceView) {
    //     sSurfaceView->Probe(EIID_IsSurfaceView)->Release();
    //     sSurfaceView = NULL;
    // }
}

Boolean GeckoApp::CheckLaunchState(
    /* [in] */ LaunchState checkState)
{
    Mutex::Autolock lock(&sSyncLaunchState);
    return sLaunchState == checkState;
}

void GeckoApp::SetLaunchState(
    /* [in] */ LaunchState setState)
{
    Mutex::Autolock lock(&sSyncLaunchState);
    sLaunchState = setState;
}

// if mLaunchState is equal to checkState this sets mLaunchState to setState
// and return true. Otherwise we return false.
Boolean GeckoApp::CheckAndSetLaunchState(
    /* [in] */ LaunchState checkState,
    /* [in] */ LaunchState setState)
{
    Mutex::Autolock lock(&sSyncLaunchState);
    if (sLaunchState != checkState) {
        return FALSE;
    }
    sLaunchState = setState;
    return TRUE;
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

void* GeckoApp::LaunchEntryRoutine(void *arg)
{
    ECode ec;
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    Int64 startup_time;
    // = System::CurrentTimeMillis(&startup_time);
    //if (FAILED(ec)) return ec;

    GeckoApp* host = (GeckoApp*)arg;
    //try {
        if (!host->mLibLoadThread) {
// printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
            // host->mLibLoadThread->Join();
        }
    //} catch (InterruptedException ie) {}
    AutoPtr<IResources> res;
    ASSERT_SUCCEEDED(host->GetResources((IResources**)&res));
    ASSERT_SUCCEEDED(res->GetString(0x7f050000, //R.string.splash_screen_loading,
                     &GeckoApp::sSurfaceView->sSplashStatusMsg));
    ASSERT_SUCCEEDED(GeckoApp::sSurfaceView->DrawSplashScreen());
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    // unpack files in the components directory
    //try {
//    ec = host->UnpackComponents();
//    if (ec == E_FILE_NOT_FOUND_EXCEPTION) {
//        printf("GeckoApp error unpacking components: file not found\n");
//        //Looper:Prepare();
//        String error;
//        host->GetString(0x7f050004, &error); //R.string.error_loading_file
//        host->ShowErrorDialog(error);
//        //Looper::Loop();
//        return NOERROR;
//    }
//    else if (ec == E_IO_EXCEPTION){
//        printf("GeckoApp error unpacking components: io error\n");
//        String msg("io error");
//        //Looper::Prepare();
//        /*if (msg != null && msg.equalsIgnoreCase("No space left on device"))
//            showErrorDialog(getString(0x7f050003)); //R.string.no_space_to_start_error
//        else
//            showErrorDialog(getString(0x7f050004)); //R.string.error_loading_file*/
//        //Looper::Loop();
//        return NOERROR;
//    }
//    /*} catch (FileNotFoundException fnfe) {
//        Log.e("GeckoApp", "error unpacking components", fnfe);
//        Looper.prepare();
//        showErrorDialog(getString(0x7f050004));//R.string.error_loading_file
//        Looper.loop();
//        return;
//    } catch (IOException ie) {
//        Log.e("GeckoApp", "error unpacking components", ie);
//        String msg = ie.getMessage();
//        Looper.prepare();
//        if (msg != null && msg.equalsIgnoreCase("No space left on device"))
//            showErrorDialog(getString(R.string.no_space_to_start_error));
//        else
//            showErrorDialog(getString(R.string.error_loading_file));
//        Looper.loop();
//        return;
//    }*/

    // and then fire us up
    //try {
       String env;
       // ASSERT_SUCCEEDED(host->mLaunchIntent->GetStringExtra(String("env0"), &env));

       AutoPtr<IApplication> app;
       ASSERT_SUCCEEDED(host->GetApplication((IApplication**)&app));
       String resPath;
       //ec = pApp->GetPackageResourcePath(resPath&);

       String args, data;
       // ASSERT_SUCCEEDED(host->mLaunchIntent->GetStringExtra(String("args"), &args));
       //FAIL_RETURN(host->mLaunchIntent->GetDataString(&data));
       ASSERT_SUCCEEDED(GeckoAppShell::RunGecko(resPath, args, data));
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
    return (void*)ec;
}

// Returns true when the intent is going to be handled by gecko launch
Boolean GeckoApp::Launch(
    /* [in] */ IIntent* _intent)
{
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    if (!CheckAndSetLaunchState(LaunchState_Launching, LaunchState_Launched)) {
        return FALSE;
    }

    AutoPtr<IIntent> intent = _intent;
    if (intent == NULL) {
        intent = GetIntent();
    }

    mLaunchIntent = intent;

    pthread_t thread;
    if (pthread_create(&thread, NULL, LaunchEntryRoutine, (void*)this)) {
        assert(0);
        return E_THREAD_ABORTED;
    }

    // AutoPtr<LaunchRunnable> runnable = new LaunchRunnable(this);
    // assert(runnable != NULL);

    // AutoPtr<IThread> thread;
    // ASSERT_SUCCEEDED(CThread::New(IRunnable::Probe(runnable.Get()), (IThread**)&thread));
    // thread->Start();
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    return TRUE;
}

ECode GeckoApp::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    sAppContext = this;
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

//    if (!sGREDir) {
//        String dataDir;
//        /*IApplicationInfo* pAppInfo;
//        ec = GetApplicationInfo(&pAppInfo);
//        if (FAILED(ec)) return ec;
//
//        ec = pAppInfo->GetDataDir(&dataDir);
//        pAppInfo->Release();
//        if (FAILED(ec)) return ec;*/
//        FAIL_RETURN(CFile::New(dataDir, (IFile**)&sGREDir));
//    }
    AutoPtr<IWindow> window;
    ASSERT_SUCCEEDED(GetWindowEx((IWindow**)&window));

    ASSERT_SUCCEEDED(window->SetFlags(sFullscreen ?
             WindowManagerLayoutParams_FLAG_FULLSCREEN : 0,
             WindowManagerLayoutParams_FLAG_FULLSCREEN));
    if (!sSurfaceView) {
        ASSERT_SUCCEEDED(CGeckoSurfaceView::NewByFriend(this, (CGeckoSurfaceView**)&sSurfaceView));
    }
    else {
        ASSERT_SUCCEEDED(sMainLayout->RemoveViewInLayout(sSurfaceView));
    }
    ASSERT_SUCCEEDED(CFrameLayout::New(this, (IFrameLayout**)&sMainLayout));
    AutoPtr<IFrameLayoutLayoutParams> fLayoutParams;
    ASSERT_SUCCEEDED(CFrameLayoutLayoutParams::New(ViewGroupLayoutParams_FILL_PARENT,
            ViewGroupLayoutParams_FILL_PARENT, (IFrameLayoutLayoutParams**)&fLayoutParams));
    ASSERT_SUCCEEDED(sMainLayout->AddViewEx3(sSurfaceView, fLayoutParams));
    AutoPtr<IViewGroupLayoutParams> vgLayoutParams;
    ASSERT_SUCCEEDED(CViewGroupLayoutParams::New(ViewGroupLayoutParams_FILL_PARENT,
            ViewGroupLayoutParams_FILL_PARENT, (IViewGroupLayoutParams**)&vgLayoutParams));
    SetContentView(sMainLayout, vgLayoutParams);

    // ASSERT_SUCCEEDED((CIntentFilter::New((IIntentFilter**)&mConnectivityFilter)));
    // mConnectivityFilter->AddAction(
    //         String(ConnectivityManager_CONNECTIVITY_ACTION));
    // ASSERT_SUCCEEDED(CGeckoConnectivityReceiver::New(&mConnectivityReceiver));

    if (!GeckoApp::CheckAndSetLaunchState(LaunchState_PreLaunch,
        LaunchState_Launching)) return NOERROR;

    // CheckAndLaunchUpdate();

    // AutoPtr<LibLoadRunnable> libLoadRunnable = new LibLoadRunnable(this);
    // assert(libLoadRunnable != NULL);
    // ASSERT_SUCCEEDED(CThread::New(IRunnable::Probe(libLoadRunnable), (IThread**)&mLibLoadThread));
// printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
//     AutoPtr<IFile> cacheFile;
//     ASSERT_SUCCEEDED(GeckoAppShell::GetCacheDir((IFile**)&cacheFile));
// printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
//     AutoPtr<IFile> libXulFile;
//     ASSERT_SUCCEEDED(CFile::New(cacheFile, String("libxul.so"), (IFile**)&libXulFile));
// printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
//     AutoPtr<IApplication> app;
//     ASSERT_SUCCEEDED(GetApplication((IApplication**)&app));
//     String resourcePath;
    //ec = pIApplication->GetPackageResourcePath(&resourcePath);
//     AutoPtr<IFile> res;
//     ASSERT_SUCCEEDED(CFile::New(resourcePath, (IFile**)&res));
// printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
//     Int64 resModifyTime, xulModifyTime;
//     res->LastModified(&resModifyTime);
//     libXulFile->LastModified(&xulModifyTime);
// printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
//     Int32 freeSpace = 0;
//     Boolean exist = FALSE;
//     GeckoAppShell::GetFreeSpace(&freeSpace);
//     libXulFile->Exists(&exist);

    AutoPtr<IResources> resources;
    ASSERT_SUCCEEDED(GetResources((IResources**)&resources));
//     if (freeSpace > GeckoAppShell::kFreeSpaceThreshold &&
//         (!exist || resModifyTime >= xulModifyTime)) {
//         ASSERT_SUCCEEDED(resources->GetString(
//                 0x7f050001, //R.string.splash_screen_installing_libs,
//                 &sSurfaceView->sSplashStatusMsg));
//     }
//     else {
        ASSERT_SUCCEEDED(resources->GetString(
                0x7f050000, //R.string.splash_screen_loading,
                &sSurfaceView->sSplashStatusMsg));
//    }
printf("==== File: %s, Line: %d, str: %s ====\n", __FILE__, __LINE__, sSurfaceView->sSplashStatusMsg.string());
//    mLibLoadThread->Start();
    return NOERROR;
}

ECode GeckoApp::OnStart()
{
    printf("GeckoApp onstart\n");
    return Activity::OnStart();
}

ECode GeckoApp::OnResume()
{
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    printf("GeckoApp resume\n");
    if (CheckLaunchState(LaunchState_GeckoRunning)) {
        GeckoAppShell::OnResume();
    }
    // After an onPause, the activity is back in the foreground.
    // Undo whatever we did in onPause.
    Activity::OnResume();

    // Just in case. Normally we start in onNewIntent
    if (CheckLaunchState(LaunchState_PreLaunch) ||
        CheckLaunchState(LaunchState_Launching)) {
        OnNewIntent(GetIntent());
    }
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
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
    if (CheckLaunchState(LaunchState_GeckoRunning)) {
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

class ButtonClickListener
    : public ElRefBase
    , public IViewOnClickListener
{
public:
    ButtonClickListener(
        /* [in] */ GeckoApp* host)
        : mHost(host)
    {}

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
        mHost->sMainLayout->RemoveViewInLayout(mHost->mLaunchButton);
        mHost->SetLaunchState(GeckoApp::LaunchState_Launching);
        mHost->Launch(NULL);

        return NOERROR;
    }

private:
    AutoPtr<GeckoApp> mHost;
};

ECode GeckoApp::OnNewIntent(
    /* [in] */ IIntent* intent)
{
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    ECode ec = NOERROR;
    Boolean result;
    if (CheckLaunchState(LaunchState_GeckoExiting)) {
        // We're exiting and shouldn't try to do anything else just incase
        // we're hung for some reason we'll force the process to exit
        exit(0);
        return NOERROR;
    }

    String action;
    intent->GetAction(&action);
printf("==== File: %s, Line: %d, action: %s ====\n", __FILE__, __LINE__, action.string());
    if (action.Equals("org.mozilla.gecko.DEBUG") &&
        CheckAndSetLaunchState(LaunchState_Launching, LaunchState_WaitButton)) {
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
        CButton::New(this, (IButton**)&mLaunchButton);

        AutoPtr<ICharSequence> text;
        CStringWrapper::New(String("Launch"), (ICharSequence**)&text);

        mLaunchButton->SetText(text); // don't need to localize

        AutoPtr<IViewOnClickListener> listener = new ButtonClickListener(this);
        assert(listener != NULL);
        mLaunchButton->SetOnClickListener(listener);
        sMainLayout->AddViewEx2(mLaunchButton, 300, 200);

        return NOERROR;
    }

    if (CheckLaunchState(LaunchState_WaitButton)) return NOERROR;
    if (Launch(intent)) return NOERROR;
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
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
        ec = intent->GetStringExtra(String("args"), &uri);
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
                    &sSurfaceView->sSplashStatusMsg);
    pIRes->Release();
    if (FAILED(ec)) goto exit;
    sSurfaceView->DrawSplashScreen();

    if (!mHaveKilledZombies) {
        mHaveKilledZombies = TRUE;
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
    AutoPtr<IFile> baseUpdateDir;
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

    AutoPtr<IFile> temp, updateDir;
    ASSERT_SUCCEEDED(CFile::New(baseUpdateDir, String("updates"), (IFile**)&temp));
    ASSERT_SUCCEEDED(CFile::New(temp, String("0"), (IFile**)&updateDir));

    AutoPtr<IFile> updateFile, statusFile;
    Boolean exist;
    String updateStatus;
    String status;
    String packageName;
    ASSERT_SUCCEEDED(CFile::New(updateDir, String("update.apk"), (IFile**)&updateFile));
    ASSERT_SUCCEEDED(CFile::New(updateDir, String("update.status"), (IFile**)&statusFile));

    statusFile->Exists(&exist);
    ReadUpdateStatus(statusFile, &updateStatus);
    if (!exist || !updateStatus.Equals("pending")) {
        return NOERROR;
    }

    updateFile->Exists(&exist);
    if (!exist) return NOERROR;

    printf("GeckoAppJava Update is available!\n");

    // Launch APK
    GetPackageName(&packageName);
    AutoPtr<IFile> updateFileToRun;
    ASSERT_SUCCEEDED(CFile::New(updateDir, packageName + "-update.apk", (IFile**)&updateFileToRun));
    //try {
        Boolean succeeded;
        ASSERT_SUCCEEDED(updateFile->RenameTo(updateFileToRun, &succeeded));
        if (succeeded) {
            String path;
            updateFileToRun->GetPath(&path);
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

    AutoPtr<IFileOutputStream> os;
    //try {
    {
        ArrayOf<Byte> buf((Byte*)status.string(), status.GetLength());

        ASSERT_SUCCEEDED(CFileOutputStream::New(statusFile, (IFileOutputStream**)&os));

        os->WriteBufferEx(0, buf.GetLength(), buf);
        os->Close();
    }
    //} catch (Exception e) {
    //    Log.i("GeckoAppJava", "error writing status file", e);
    //}

    if (statusCode == 0) {
    //    System::Exit(0);
    }
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

