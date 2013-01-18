
#include "GeckoAppShell.h"
#include "GeckoApp.h"
#include "GeckoEvent.h"
#include "GeckoRunnable.h"

#include <stdio.h>
#include <dlfcn.h>

#include <elastos/List.h>
#include <elastos/AutoPtr.h>
//#include <elastos/ByteBuffer.h>

//const ITimer* GeckoAppShell::mIMETimer = NULL;
//ICountDownLatch* GeckoAppShell::sGeckoPendingAcks = NULL;
Int32 GeckoAppShell::sPidColumn = -1;
Int32 GeckoAppShell::sUserColumn = -1;
List<GeckoEvent*> GeckoAppShell::gPendingEvents;
Boolean GeckoAppShell::gRestartScheduled = FALSE;
HashMap<Int32, IAlertNotification*> GeckoAppShell::mAlertNotifications;
IFile* GeckoAppShell::sCacheFile = NULL;
Int32 GeckoAppShell::sFreeSpace = -1;
IHandler* GeckoAppShell::sHandler = NULL;
//ISensor* GeckoAppShell::gAccelerometerSensor = NULL;
//ISensor* GeckoAppShell::gOrientationSensor = NULL;
Mutex GeckoAppShell::mGeckoAppShell;

GeckoAppShell::LooperThread::LooperThread()
    : Thread()
{
}

ECode GeckoAppShell::LooperThread::Run() {
    /*Looper::Prepare();
    //try {
    IHandler* pHandler;
    ECode ec = CHandler::New(&pHandler);
    if (FAILED(ec)) return ec;
    mHandlerQueue.Put(pHandler);
    pHandler->Release();
    //} catch (InterruptedException ie) {}
    Looper::Loop();*/

    return NOERROR;
}

PInterface GeckoAppShell::LooperThread::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRunnable*)this;
    }
    else if (riid == EIID_IThread) {
        return (IThread*)this;
    }

    return NULL;
}

Mutex* GeckoAppShell::LooperThread::GetSelfLock()
{
    return &mSync;
}

UInt32 GeckoAppShell::LooperThread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GeckoAppShell::LooperThread::Release()
{
    return ElRefBase::Release();
}

ECode GeckoAppShell::LooperThread::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::CheckAccess()
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::Destroy()
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::GetId(
    /* [out] */ Int64* id)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::GetName(
    /* [out] */ String* name)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::GetPriority(
    /* [out] */ Int32* priority)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::GetState(
    /* [out] */ ThreadState* state)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::Interrupt()
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::Join()
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::JoinEx(
    /* [in] */ Int64 millis)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::Resume()
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::SetName(
    /* [in] */ const String& threadName)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::Start()
{
    return Thread::Start();
}

ECode GeckoAppShell::LooperThread::Stop()
{
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::LooperThread::Suspend()
{
    return E_NOT_IMPLEMENTED;
}

GeckoAppShell::GeckoAppShell()
{
    //CTimer::New(&mIMETimer);
}

GeckoAppShell::~GeckoAppShell()
{
    /*if (mIMETimer) {
        mIMETimer->Release();
    }*/
    if (sCacheFile) {
        sCacheFile->Release();
    }
    if (sHandler) {
        sHandler->Release();
    }
    /*if (sGeckoPendingAcks) {
        sGeckoPendingAcks>Release();
    }
    if (gAccelerometerSensor) {
        gAccelerometerSensor>Release();
    }
    if (gOrientationSensor) {
        gOrientationSensor>Release();
    }*/
}

ECode GeckoAppShell::GetMainHandler(
    /* [out] */ IHandler** ppHandler)
{
    if (!ppHandler) return E_INVALID_ARGUMENT;

    *ppHandler = GeckoApp::mAppContext->mMainHandler;
    if (*ppHandler) {
        (*ppHandler)->AddRef();
    }
    return NOERROR;
}

ECode GeckoAppShell::GetHandler(
    /* [out] */ IHandler** ppHandler)
{
    if (!ppHandler) return E_INVALID_ARGUMENT;

    if (!sHandler) {
        LooperThread* pLt = new LooperThread();
        if (!pLt) return E_OUT_OF_MEMORY;
        pLt->Start();
        //try {
        //sHandler = pLt->mHandlerQueue.Take();
        //} catch (InterruptedException ie) {}*/
        delete pLt;
    }
    *ppHandler = sHandler;
    if (sHandler) {
        sHandler->AddRef();
    }
    return NOERROR;
}

ECode GeckoAppShell::GetCacheDir(
    /* [out] */ IFile** ppFile)
{
    if (!ppFile) return E_INVALID_ARGUMENT;

    if (!sCacheFile){
        //ECode ec = GeckoApp::mAppContext->GetCacheDir(&sCacheFile);
        //if (FAILED(ec)) return ec;
    }
    *ppFile = sCacheFile;
    if (sCacheFile) {
        sCacheFile->AddRef();
    }
    return NOERROR;
}

ECode GeckoAppShell::GetFreeSpace(
    /* [out] */ Int32* pFreeSpace)
{
    //try {
        if (sFreeSpace == -1) {
            IFile* cacheDir;
            ECode ec = GetCacheDir(&cacheDir);
            if (FAILED(ec)) {
                printf("GeckoAppShell Unable to get cache dir\n");
                *pFreeSpace = -1;
                return NOERROR;
            }
            String path;
            ec = cacheDir->GetPath(&path);
            cacheDir->Release();
            if (FAILED(ec)) return ec;

            /*IStatFs* pCacheStats;
            ec = CStatFs::New(path, &pCacheStats);
            if (FAILED(ec)) return ec;
            Int32 freeBlocks = 0, blockSize = 0;
            pCacheStats->GetFreeBlocks(&freeBlocks);
            pCacheStats->GetBlockSize(&blockSize);
            sFreeSpace = freeBlocks * blockSize;*/
        }
    //} catch (Exception e) {
    //    Log.e("GeckoAppShell", "exception while stating cache dir: ", e);
    //}
    *pFreeSpace = sFreeSpace;
    return NOERROR;
}

ECode GeckoAppShell::MoveFile(
    /* [in] */ IFile* pInFile,
    /* [in] */ IFile* pOutFile,
    /* [out] */ Boolean* pResult)
{
    if (!pResult) return E_INVALID_ARGUMENT;

    //Log.i("GeckoAppShell", "moving " + inFile + " to " + outFile);
    Boolean isDirectory;
    ECode ec = pOutFile->IsDirectory(&isDirectory);
    if (FAILED(ec)) return ec;
    if (isDirectory) {
        String name;
        ec = pInFile->GetName(&name);
        if (FAILED(ec)) return ec;
        ec = CFile::New(pOutFile, name, &pOutFile);
        if (FAILED(ec)) return ec;
    } else {
        pOutFile->AddRef();
    }
    //try {
        Boolean succeeded = FALSE;
        ec = pInFile->RenameTo(pOutFile, &succeeded);
        if (FAILED(ec)) {
            pOutFile->Release();
            return ec;
        }
        if (succeeded) {
            pOutFile->Release();
            *pResult = TRUE;
            return NOERROR;
        }
    //} catch (SecurityException se) {
    //    Log.w("GeckoAppShell", "error trying to rename file", se);
    //}
    //try {
        Int64 lastModified;
        ec = pInFile->LastModified(&lastModified);
        if (FAILED(ec)) {
            pOutFile->Release();
            return ec;
        }

        ec = pOutFile->CreateNewFile(&succeeded);
        if (FAILED(ec)) {
            pOutFile->Release();
            return ec;
        }

        // so copy it instead
        IFileInputStream* pIFileInputStream;
        ec = CFileInputStream::New(pInFile, &pIFileInputStream);
        if (FAILED(ec)) goto exit;
        IFileChannel* pInChannel;
        ec = pIFileInputStream->GetChannel(&pInChannel);
        pIFileInputStream->Release();
        if (FAILED(ec)) goto exit;

        IFileOutputStream* pIFileOutputStream;
        ec = CFileOutputStream::New(pOutFile, &pIFileOutputStream);
        if (FAILED(ec)) {
            pInChannel->Release();
            goto exit;
        }
        IFileChannel* pOutChannel;
        ec = pIFileOutputStream->GetChannel(&pInChannel);
        pIFileOutputStream->Release();
        if (FAILED(ec)) {
            pInChannel->Release();
            goto exit;
        }

        Int64 size, transferred, number;
        ec = pInChannel->Size(&size);
        if (FAILED(ec)) {
            pInChannel->Release();
            pOutChannel->Release();
            goto exit;
        }

        ec = pInChannel->TransferTo(0, size, 
            (IWritableByteChannel*)pOutChannel->Probe(EIID_IWritableByteChannel),
             &number);
        pInChannel->Release();
        pOutChannel->Release();
        if (FAILED(ec)) goto exit;

        ec = pOutFile->SetLastModified(lastModified, &succeeded);
        if (FAILED(ec)) goto exit;

        if (transferred == size) {
            ec = pInFile->Delete(&succeeded);
        }
        else {
            *pResult = FALSE;
        }
    /*} catch (Exception e) {
        Log.e("GeckoAppShell", "exception while moving file: ", e);
        try {
            outFile.delete();
        } catch (SecurityException se) {
            Log.w("GeckoAppShell", "error trying to delete file", se);
        }
        return false;
    }*/
exit:
    if (FAILED(ec)) {
        pOutFile->Delete(&succeeded);
    }
    else {
        *pResult = TRUE;
    }
    pOutFile->Release();
    return ec;
}

ECode GeckoAppShell::MoveDir(
    /* [in] */ IFile* pFrom,
    /* [in] */ IFile* pTo,
    /* [out] */ Boolean* pResult)
{
    if (!pResult) return E_INVALID_ARGUMENT;

    //try {
        Boolean succeeded;
        ECode ec = pTo->Mkdirs(&succeeded);
        if (FAILED(ec)) return ec;
        ec = pFrom->RenameTo(pTo, &succeeded);
        if (FAILED(ec)) return ec;
        if (succeeded) {
            *pResult = TRUE;
            return NOERROR;
        }
    //} catch (SecurityException se) {
    //    Log.w("GeckoAppShell", "error trying to rename file", se);
    //}
    IObjectContainer* pContainer;
    ec = pFrom->ListFiles(&pContainer);
    if (FAILED(ec)) return ec;

    Boolean retVal = TRUE;
    //try {
        IObjectEnumerator* pEnum;
        ec = pContainer->GetObjectEnumerator(&pEnum);
        pContainer->Release();
        if (FAILED(ec)) return ec;

        IFile* pIFile, *pDest;
        String name;
        Boolean isDirectory;
        ec = pEnum->MoveNext(&succeeded);
        while (SUCCEEDED(ec) && succeeded) {
            ec = pEnum->Current((IInterface**)&pIFile);
            if (FAILED(ec)) {
                pEnum->Release();
                return ec;
            }
            pIFile->GetName(&name);
            ec = CFile::New(pTo, name, &pDest);
            if (FAILED(ec)) {
                pIFile->Release();
                pEnum->Release();
                return ec;
            }
            pIFile->IsDirectory(&isDirectory);
            if (isDirectory) {
                ec = MoveDir(pIFile, pDest, &succeeded);
            }
            else {
                ec = MoveFile(pIFile, pDest, &succeeded);
            }
            retVal = succeeded ? retVal : FALSE;
            pIFile->Release();
            pDest->Release();
            if (FAILED(ec)) {
                pEnum->Release();
                return ec;
            }

            ec = pEnum->MoveNext(&succeeded);
        }
        pFrom->Delete(&succeeded);
    //} catch(Exception e) {
    //    Log.e("GeckoAppShell", "error trying to move file", e);
    //}
    *pResult = retVal;
    return ec;
}

ECode GeckoAppShell::LoadGeckoLibs(
    /* [in] */ const String& apkName)
{
    // The package data lib directory isn't placed in ld.so's
    // search path, so we have to manually load libraries that
    // libxul will depend on.  Not ideal.
    dlopen("/data/data/com.elastos.runtime/elastos/org.mozilla.fennec_unofficial/libmozutil.so", RTLD_LAZY);

    GeckoApp* pGeckoApp = GeckoApp::mAppContext;
    String homeDir, resPath;
    IApplication* pIApplication;
    ECode ec = pGeckoApp->GetApplication(&pIApplication);
    if (FAILED(ec)) return ec;
    //ec = pIApplication->GetPackageResourcePath(&resPath);
    pIApplication->Release();
    if (FAILED(ec)) return ec;

    Boolean exist = FALSE;
    Boolean result;
    //if (Build_VERSION.SDK_INT < 8 || resPath.StartWith("/data")) {
    if (resPath.StartWith("/data")) {
        IFile* pHome;
        //ec = pGeckoApp->GetFilesDir(&pHome);
        //if (FAILED(ec)) return ec;
        ec = pHome->GetPath(&homeDir);
        pHome->Release();
        if (FAILED(ec)) return ec;

        // handle the application being moved to phone from sdcard
        IFile* pProfileDir, *pOldHome;
        ec = CFile::New(homeDir, String("mozilla"), &pProfileDir);
        if (FAILED(ec)) return ec;

        String oldHomePath("/data/data/com.elastos.runtime/elastos/");
        String packageName;
        pGeckoApp->GetPackageName(&packageName);
        oldHomePath += packageName + "/mozilla";
        ec = CFile::New(oldHomePath, &pOldHome);
        if (FAILED(ec)) {
            pProfileDir->Release();
            return ec;
        }

        ec = pOldHome->Exists(&exist);
        if (FAILED(ec)) {
            pProfileDir->Release();
            pOldHome->Release();
            return ec;
        }
        if (exist) {
            MoveDir(pOldHome, pProfileDir, &result);
        }
        pOldHome->Release();

        if (1) {//Build_VERSION.SDK_INT >= 8) {
            IFile* pExtHome, *pExtProf;
            /*ec = pGeckoApp->GetExternalFilesDir(NULL, &pExtHome);
            if (FAILED(ec)) {
                pProfileDir->Release();
                return ec;
            }*/
            ec = CFile::New(pExtHome, String("mozilla"), &pExtProf);
            pExtHome->Release();
            if (FAILED(ec)) {
                pProfileDir->Release();
                return ec;
            }
            ec = pExtProf->Exists(&exist);
            if (FAILED(ec)) {
                pProfileDir->Release();
                pExtProf->Release();
                return ec;
            }
            if (exist) {
                MoveDir(pExtProf, pProfileDir, &result);
            }
            pExtProf->Release();
        }
        pProfileDir->Release();
    } 
    else {
        IFile* pHome;
        //ec = pGeckoApp->GetExternalFilesDir(NULL, &pHome);
        //if (FAILED(ec)) return ec;
        ec = pHome->GetPath(&homeDir);
        pHome->Release();
        if (FAILED(ec)) return ec;

        // handle the application being moved to phone from sdcard
        IFile* pProfileDir, *pOldHome;
        ec = CFile::New(homeDir, String("mozilla"), &pProfileDir);
        if (FAILED(ec)) return ec;

        String oldHomePath("/data/data/com.elastos.runtime/elastos/");
        String packageName;
        pGeckoApp->GetPackageName(&packageName);
        oldHomePath += packageName + "/mozilla";
        ec = CFile::New(oldHomePath, &pOldHome);
        if (FAILED(ec)) {
            pProfileDir->Release();
            return ec;
        }

        ec = pOldHome->Exists(&exist);
        if (FAILED(ec)) {
            pProfileDir->Release();
            pOldHome->Release();
            return ec;
        }
        if (exist) {
            MoveDir(pOldHome, pProfileDir, &result);
        }
        pOldHome->Release();

        IFile* pIntHome, *pIntProf;
        /*ec = pGeckoApp->GetFilesDir(&pIntHome);
        if (FAILED(ec)) {

            pProfileDir->Release();
            return ec;
        }*/
        ec = CFile::New(pIntHome, String("mozilla"), &pIntProf);
        pIntHome->Release();
        if (FAILED(ec)) {
            pProfileDir->Release();
            return ec;
        }
        ec = pIntProf->Exists(&exist);
        if (FAILED(ec)) {
            pProfileDir->Release();
            return ec;
        }
        if (exist) {
            MoveDir(pIntProf, pProfileDir, &result);
        }
        pProfileDir->Release();
        pIntProf->Release();
    }

    String home("HOME=");
    home.Append(homeDir);
    String greHome("GRE_HOME=");
    String grePath;
    GeckoApp::sGREDir->GetPath(&grePath);
    greHome.Append(grePath);
    GeckoAppShell::PutEnv(home);
    GeckoAppShell::PutEnv(greHome);

    /*AutoPtr<IIntent> pIIntent = pGeckoApp->GetIntent();
    assert(pIIntent);
    String env;
    ec = pIIntent->GetStringExtra(String("env0"), &env);
    if (FAILED(ec)) return ec;

    printf("GeckoApp env0: %s\n", env);
    for (Int32 c = 1; env.IsNullOrEmpty(); c++) {
        GeckoAppShell::PutEnv(env);
        String temp("env");
        temp += String::FromInt32(c);
        ec = pIIntent->GetStringExtra(temp, &env);
        if (FAILED(ec)) return ec;

        printf("GeckoApp %s : %s\n", temp.string(), env.string());
    }

    IFile* pIFile;
    ec = pGeckoApp->GetDir("tmp", Context_MODE_WORLD_READABLE |
                             Context_MODE_WORLD_WRITEABLE, &pIFile);
    if (FAILED(ec)) return ec;
    ec = pIFile->Exists(&exist);
    if (FAILED(ec)) {
        pIFile->Release();
        return ec;
    }
    if (!exist) {
        Boolean succeeded;
        pIFile->Mkdirs(&succeeded);
    }*/

    String tmpPath;
    //pIFile->GetPath(&tmpPath);
    //pIFile->Release();
    //String tmpDir("TMPDIR=");
    //tmpDir.Append(tmpPath);
    //GeckoAppShell::PutEnv(tmpDir);

    //ec = Environment::GetDownloadCacheDirectory(&pIFile);
    //if (FAILED(ec)) return ec;
    //pIFile->GetPath(&tmpPath);
    //pIFile->Release();
    String extStorage("EXTERNAL_STORAGE=");
    extStorage.Append(tmpPath);
    GeckoAppShell::PutEnv(extStorage);

    IFile* pCacheFile;
    ec = GetCacheDir(&pCacheFile);
    if (FAILED(ec)) return ec;
    pCacheFile->GetPath(&tmpPath);
    String cachePath("CACHE_PATH=");
    cachePath.Append(tmpPath);
    GeckoAppShell::PutEnv(cachePath);

    // gingerbread introduces File.getUsableSpace(). We should use that.
    Int32 freeSpace;
    ec = GetFreeSpace(&freeSpace);
    if (FAILED(ec)) {
        pCacheFile->Release();
        return ec;
    }
    //try {
        IFile *pDownloadDir, *pUpdatesDir;
        if (1) {//Build_VERSION.SDK_INT >= 8) {
            /*ec = Environment.GetExternalStoragePublicDirectory(
                    Environment_DIRECTORY_DOWNLOADS, &pDownloadDir);
            if (FAILED(ec)) {
                pCacheFile->Release();
                return ec;
            }
            ec = pGeckoApp->GetExternalFilesDir(Environment_DIRECTORY_DOWNLOADS, &pUpdatesDirsd);
            if (FAILED(ec)) {
                pCacheFile->Release();
                pDownloadDir->Release();
                return ec;
            }*/
        } 
        else {
            String path;
            /*IFile* pIFile;
            ec = Environment::GetExternalStorageDirectory(&pIFile);

            if (FAILED(ec)) {
                pCacheFile->Release();
                return ec;
            }
            pIFile->GetPath(&path);
            pIFile->Release();*/

            ec = CFile::New(path, String("download"), &pDownloadDir);
            if (FAILED(ec)) {
                pCacheFile->Release();
                return ec;
            }
            pUpdatesDir = pDownloadDir;
            pDownloadDir->AddRef();
        }
        pDownloadDir->GetPath(&tmpPath);
        pDownloadDir->Release();
        String downloadDir("DOWNLOADS_DIRECTORY=");
        downloadDir.Append(tmpPath);
        GeckoAppShell::PutEnv(downloadDir);

        pUpdatesDir->GetPath(&tmpPath);
        pUpdatesDir->Release();
        String updateDir("UPDATES_DIRECTORY=");
        updateDir.Append(tmpPath);
        GeckoAppShell::PutEnv(updateDir);
    //}
    //catch (Exception e) {
    //    Log.i("GeckoApp", "No download directory has been found: " + e);
    //}

    PutLocaleEnv();

    if (freeSpace + kLibFreeSpaceBuffer < kFreeSpaceThreshold) {
        // remove any previously extracted libs since we're apparently low
        IObjectContainer* pContainer;
        ec = pCacheFile->ListFiles(&pContainer);
        pContainer->Release();
        if (FAILED(ec)) return ec;

        IObjectEnumerator* pEnum;
        ec = pContainer->GetObjectEnumerator(&pEnum);
        pContainer->Release();
        if (FAILED(ec)) return ec;

        IFile* pLibFile;
        String name;
        Boolean succeeded;
        ec = pEnum->MoveNext(&succeeded);
        while (SUCCEEDED(ec) && succeeded) {
            ec = pEnum->Current((IInterface**)&pLibFile);
            if (FAILED(ec)) return ec;
            ec = pLibFile->GetName(&name);
            if (FAILED(ec)) {
                pLibFile->Release();
                return ec;
            }
            if (name.EndWith(".so")) {
                pLibFile->Delete(&succeeded);
            }
            pLibFile->Release();
            ec = pEnum->MoveNext(&succeeded);
        }
    }
    LoadLibs(apkName, freeSpace > kFreeSpaceThreshold);
    return NOERROR;
}

ECode GeckoAppShell::RunGecko(
    /* [in] */ const String& apkPath,
    /* [in] */ const String& args,
    /* [in] */ const String& url)
{
    // run gecko -- it will spawn its own thread
    GeckoAppShell::NativeInit();

    // Tell Gecko where the target surface view is for rendering
    GeckoAppShell::SetSurfaceView(GeckoApp::surfaceView);

    // First argument is the .apk path
    String combinedArgs(apkPath);
    combinedArgs.Append(" -greomni ");
    combinedArgs.Append(apkPath);
    if (!args.IsNullOrEmpty()) {
        combinedArgs.Append(" ");
        combinedArgs.Append(args);
    }
    if (!url.IsNullOrEmpty()) {
        combinedArgs.Append(" ");
        combinedArgs.Append(url);
    }
    // and go
    GeckoAppShell::NativeRun(combinedArgs);
    return NOERROR;
}

ECode GeckoAppShell::SendEventToGecko(
    /* [in] */ GeckoEvent* pEvent)
{
    Boolean result;
    GeckoApp::CheckLaunchState(GeckoApp::LaunchState_GeckoRunning, &result);
    if (result) {
        NotifyGeckoOfEvent(pEvent);
        delete pEvent;
    } 
    else {
        gPendingEvents.PushBack(pEvent);
    }
    return NOERROR;
}

ECode GeckoAppShell::SendEventToGeckoSync(
    /* [in] */ GeckoEvent* pEvent)
{
    SendEventToGecko(pEvent);
    GeckoEventSync();
    return NOERROR;
}

//The Gecko-side API: API methods that Gecko calls
ECode GeckoAppShell::ScheduleRedraw()
{
    // Redraw everything
    return ScheduleRedraw(0, -1, -1, -1, -1);
}

ECode GeckoAppShell::ScheduleRedraw(
    /* [in] */ Int32 nativeWindow,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    GeckoEvent* pEvent = NULL;

    if (x == -1) {
        pEvent = new GeckoEvent(GeckoEvent::DRAW, NULL);
    } else {
        IRect* pRect;
        ECode ec = CRect::New(x, y, w, h, &pRect);
        if (FAILED(ec)) return ec;
        pEvent = new GeckoEvent(GeckoEvent::DRAW, pRect);
        pRect->Release();
    }
    if (!pEvent) return E_OUT_OF_MEMORY;

    pEvent->mNativeWindow = nativeWindow;

    SendEventToGecko(pEvent);
}

ECode GeckoAppShell::NotifyIME(
    /* [in] */ Int32 type,
    /* [in] */ Int32 state)
{
    if (!GeckoApp::surfaceView) {
        return NOERROR;
    }
    ECode ec;
    switch (type) {
    case NOTIFY_IME_RESETINPUTSTATE:
        // Composition event is already fired from widget.
        // So reset IME flags.
        GeckoApp::surfaceView->mInputConnection->Reset();

        // Don't use IMEStateUpdater for reset.
        // Because IME may not work showSoftInput()
        // after calling restartInput() immediately.
        // So we have to call showSoftInput() delay.
        IContext* pIContext;
        ec = GeckoApp::surfaceView->GetContext(&pIContext);
        if (FAILED(ec)) return ec;
        ILocalInputMethodManager* pImm;
        ec = pIContext->GetSystemService(
            Context_INPUT_METHOD_SERVICE, (IInterface**)&pImm);
        pIContext->Release();
        if (FAILED(ec)) {
            // no way to reset IME status directly
            //IMEStateUpdater.ResetIME();
        }
        else {
            pImm->RestartInput(GeckoApp::surfaceView);
            pImm->Release();
        }

        // keep current enabled state
        //IMEStateUpdater.EnableIME();
        break;

    case NOTIFY_IME_CANCELCOMPOSITION:
        //IMEStateUpdater.ResetIME();
        break;

    case NOTIFY_IME_FOCUSCHANGE:
        //IMEStateUpdater.ResetIME();
        break;
    }
    return NOERROR;
}

ECode GeckoAppShell::NotifyIMEEnabled(
    /* [in] */ Int32 state,
    /* [in] */ const String& typeHint,
    /* [in] */ const String& actionHint,
    /* [in] */ Boolean landscapeFS)
{
    if (!GeckoApp::surfaceView) {
        return NOERROR;
    }

    /* When IME is 'disabled', IME processing is disabled.
       In addition, the IME UI is hidden */
    /*GeckoApp::surfaceView.mIMEState = state;
    GeckoApp::surfaceView.mIMETypeHint = typeHint;
    GeckoApp::surfaceView.mIMEActionHint = actionHint;
    GeckoApp::surfaceView.mIMELandscapeFS = landscapeFS;
    IMEStateUpdater.EnableIME();*/

    return NOERROR;
}

ECode GeckoAppShell::NotifyIMEChange(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 newEnd)
{
    if (!GeckoApp::surfaceView ||
        !GeckoApp::surfaceView->mInputConnection) {
        return NOERROR;
    }

    IContext* pIContext;
    ECode ec = GeckoApp::surfaceView->GetContext(&pIContext);
    if (FAILED(ec)) return ec;
    ILocalInputMethodManager* pImm;
    ec = pIContext->GetSystemService(
        Context_INPUT_METHOD_SERVICE, (IInterface**)&pImm);
    pIContext->Release();
    if (FAILED(ec)) return ec;

    if (newEnd < 0) {
        ec = GeckoApp::surfaceView->mInputConnection->NotifySelectionChange(
            pImm, start, end);
    }
    else {
        ec = GeckoApp::surfaceView->mInputConnection->NotifyTextChange(
            pImm, text, start, end, newEnd);
    }
    pImm->Release();
    return ec;
}

// Block the current thread until the Gecko event loop is caught up
ECode GeckoAppShell::GeckoEventSync()
{
    Mutex::Autolock lock(mGeckoAppShell);
    ECode ec;
    //ec = CCountDownLatch::New(1, &sGeckoPendingAcks);
    //if (FAILED(ec)) return ec;

    GeckoAppShell::SendEventToGecko(
        new GeckoEvent(GeckoEvent::GECKO_EVENT_SYNC));
    Int32 count;
    /*ec = sGeckoPendingAcks->GetCount(&count);
    if (FAILED(ec)) {
        sGeckoPendingAcks->Release();
        sGeckoPendingAcks = NULL;
        return ec;
    }
    while (count != 0) {
        //try {
            sGeckoPendingAcks->Await();
        //} catch (InterruptedException e) {}
        ec = sGeckoPendingAcks->GetCount(&count);
        if (FAILED(ec)) {
            sGeckoPendingAcks->Release();
            sGeckoPendingAcks = NULL;
            return ec;
        }
    }
    sGeckoPendingAcks->Release();
    sGeckoPendingAcks = NULL;*/
    return NOERROR;
}

// Signal the Java thread that it's time to wake up
ECode GeckoAppShell::AcknowledgeEventSync()
{
    /*ICountDownLatch* pTmp = sGeckoPendingAcks;
    if (pTmp != NULL) {
        pTmp->CountDown();
    }*/
    return NOERROR;
}

ECode GeckoAppShell::EnableDeviceMotion(
    /* [in] */ Boolean enable)
{
   /*SensorManager sm = (SensorManager)
        GeckoApp.surfaceView.getContext().getSystemService(Context.SENSOR_SERVICE);

    if (gAccelerometerSensor == null || gOrientationSensor == null) {
        gAccelerometerSensor = sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        gOrientationSensor   = sm.getDefaultSensor(Sensor.TYPE_ORIENTATION);
    }

    if (enable) {
        if (gAccelerometerSensor != null)
            sm.registerListener(GeckoApp.surfaceView, gAccelerometerSensor, SensorManager.SENSOR_DELAY_GAME);
        if (gOrientationSensor != null)
            sm.registerListener(GeckoApp.surfaceView, gOrientationSensor,   SensorManager.SENSOR_DELAY_GAME);
    } else {
        if (gAccelerometerSensor != null)
            sm.unregisterListener(GeckoApp.surfaceView, gAccelerometerSensor);
        if (gOrientationSensor != null)
            sm.unregisterListener(GeckoApp.surfaceView, gOrientationSensor);
    }*/
    return NOERROR;
}

class LocationRunnable : public GeckoRunnable
{
public:
    LocationRunnable(Boolean enable)
        : GeckoRunnable(NULL)
        , mEnableLocation(enable)
    {
    }

    ECode Run()
    {
        CGeckoSurfaceView* view = GeckoApp::surfaceView;
        IContext* pIContext;
        ECode ec = view->GetContext(&pIContext);
        if (FAILED(ec)) return ec;
        ILocationManager* pLm;
        ec = pIContext->GetSystemService(
                    Context_LOCATION_SERVICE, (IInterface**)&pLm);
        pIContext->Release();
        if (FAILED(ec)) return ec;

        if (mEnableLocation) {
            ICriteria* pCrit;
            ec = CCriteria::New(&pCrit);
            if (FAILED(ec)) goto exit;

            pCrit->SetAccuracy(Criteria_ACCURACY_FINE);
            String provider;
            ec = pLm->GetBestProvider(pCrit, TRUE, &provider);
            pCrit->Release();
            if (FAILED(ec)) goto exit;

            if (provider.IsNull()) {
                goto exit;
            }

            /*ILooper* pLooper;
            ec = Looper::GetMainLooper(&pLooper);
            if (FAILED(ec)) goto exit;*/

            ILocation* pLoc;
            ec = pLm->GetLastKnownLocation(provider, &pLoc);
            if (FAILED(ec)) {
                //pLooper->Release();
                goto exit;
            }
            if (pLoc) {
                //view->OnLocationChanged(pLoc);
                pLoc->Release();
            }
            //pLm.RequestLocationUpdates(provider, 100, (Float).5, view, pLooper);
            //pLooper->Release();
        }
        else {
            //pLm->RemoveUpdates(view);
        }
exit:
        pLm->Release();
        return NOERROR;
    }
private:
    Boolean mEnableLocation;
};

ECode GeckoAppShell::EnableLocation(
    /* [in] */ Boolean enable)
{
    IHandler* pHandler;
    ECode ec = GetMainHandler(&pHandler);
    if (FAILED(ec)) return ec;

    LocationRunnable* pRunnable = new LocationRunnable(enable);
    if (!pRunnable) {
       pHandler->Release();
       return E_OUT_OF_MEMORY;
    }

    //return pHandler->Post(IRunnable::Probe(pRunnable));
    return NOERROR;
}

ECode GeckoAppShell::MoveTaskToBack()
{
    //Boolean result;
    //return GeckoApp::mAppContext->MoveTaskToBack(TRUE, &result);
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::ReturnIMEQueryResult(
    /* [in] */ const String& result,
    /* [in] */ Int32 selectionStart,
    /* [in] */ Int32 selectionLength)
{
    GeckoApp::surfaceView->mInputConnection->SetSelectionStart(selectionStart);
    GeckoApp::surfaceView->mInputConnection->SetSelectionLength(selectionLength);
    //try {
        GeckoApp::surfaceView->mInputConnection->PutResult(result);
    //} catch (InterruptedException e) {
    //}
    return NOERROR;
}

ECode GeckoAppShell::OnAppShellReady()
{
    // mLaunchState can only be Launched at this point
    GeckoApp::SetLaunchState(GeckoApp::LaunchState_GeckoRunning);
    SendPendingEventsToGecko();
    return NOERROR;
}

ECode GeckoAppShell::OnXreExit()
{
    // mLaunchState can only be Launched or GeckoRunning at this point
    GeckoApp::SetLaunchState(GeckoApp::LaunchState_GeckoExiting);
    printf("GeckoAppJava XRE exited\n");
    if (gRestartScheduled) {
        GeckoApp::mAppContext->DoRestart();
    } else {
        printf("GeckoAppJava we're done, good bye\n");
        //GeckoApp::mAppContext->Finish();
    }
    //System::Exit(0);
    return NOERROR;
}

ECode GeckoAppShell::ScheduleRestart()
{
    printf("GeckoAppJava scheduling restart\n");
    gRestartScheduled = TRUE;
    return NOERROR;
}

// "Installs" an application by creating a shortcut
ECode GeckoAppShell::InstallWebApplication(
    /* [in] */ const String& aURI,
    /* [in] */ const String& aTitle,
    /* [in] */ const String& aIconData)
{
    printf("GeckoAppJava installWebApplication for %s [%s]\n",
                     aURI.string(), aTitle.string());

    // the intent to be launched by the shortcut
    IIntent* pShortcutIntent;
    ECode ec = CIntent::New(String("org.mozilla.fennec.WEBAPP"), &pShortcutIntent);
    if (FAILED(ec)) return ec;

    String packageName;
    GeckoApp::mAppContext->GetPackageName(&packageName);
    packageName.Append(".App");
    //ec = pShortcutIntent->SetClassName(GeckoApp::mAppContext, packageName);
    if (FAILED(ec)) {
        pShortcutIntent->Release();
        return ec;
    }
    String args("--webapp=");
    args.Append(aURI);
    ec = pShortcutIntent->PutStringExtra(String("args"), args);
    if (FAILED(ec)) {
        pShortcutIntent->Release();
        return ec;
    }

    IIntent* pIIntent;
    ec = CIntent::New(&pIIntent);
    if (FAILED(ec)) {
        pShortcutIntent->Release();
        return ec;
    }

    //ec = pIIntent->PutExtra(Intent_EXTRA_SHORTCUT_INTENT, pShortcutIntent);
    pShortcutIntent->Release();
    if (FAILED(ec)) goto exit;
    //ec = pIIntent->PutStringExtra(Intent_EXTRA_SHORTCUT_NAME, aTitle);
    //if (FAILED(ec)) goto exit;

    //ArrayOf<Byte> raw = Base64::Decode(aIconData.Substring(22), Base64_DEFAULT);
    IBitmapFactory* pIBitmapFactory;
    ec = CBitmapFactory::AcquireSingleton(&pIBitmapFactory);
    if (FAILED(ec)) goto exit;

    //IBitmap* pIBitmap;
    //ec = pIBitmapFactory->DecodeByteArrayEx(raw, 0, raw.GetLength(), &pIBitmap);
    pIBitmapFactory->Release();
    //if (FAILED(ec)) goto exit;

    //ec = pIIntent->PutExtra(Intent_EXTRA_SHORTCUT_ICON, pIBitmap);
    //pIBitmap->Release();
    //if (FAILED(ec)) goto exit;
    ec = pIIntent->SetAction(String("com.android.launcher.action.INSTALL_SHORTCUT"));
    if (FAILED(ec)) goto exit;

    ec = GeckoApp::mAppContext->SendBroadcast(pIIntent);

exit:
    pIIntent->Release();
    return ec;
}

ECode GeckoAppShell::GetHandlersForMimeType(
    /* [in] */ const String& aMimeType,
    /* [in] */ const String& aAction,
    /* [out, callee] */ ArrayOf<String>** ppHandlers)
{
    if (!ppHandlers) return E_INVALID_ARGUMENT;

    IIntent* pIIntent;
    ECode ec = GetIntentForActionString(aAction, &pIIntent);
    if (FAILED(ec)) return ec;

    if (!aMimeType.IsNullOrEmpty()) {
        pIIntent->SetType(aMimeType);
    }
    ec = GetHandlersForIntent(pIIntent, ppHandlers);
    pIIntent->Release();
    return ec;
}

ECode GeckoAppShell::GetHandlersForURL(
    /* [in] */ const String& aURL,
    /* [in] */ const String& aAction,
    /* [out, callee] */ ArrayOf<String>** ppHandlers)
{
    if (!ppHandlers) return E_INVALID_ARGUMENT;

    // aURL may contain the whole URL or just the protocol
    ECode ec;
    /*IUri* pUri;
    if (aURL.indexOf(':') >= 0) {
        ec = Uri::Parse(aURL, &pUri);
        if (FAILED(ec)) return ec;
    }
    else {
        new Uri.Builder().scheme(aURL).build();
    }*/
    IIntent* pIIntent;
    ec = GetIntentForActionString(aAction, &pIIntent);
    if (FAILED(ec)) {
        //pUri->Release();
        return ec;
    }
    //pIIntent->SetData(pUri);
    //pUri->Release();

    ec = GetHandlersForIntent(pIIntent, ppHandlers);
    pIIntent->Release();
    return ec;
}

ECode GeckoAppShell::GetHandlersForIntent(
    /* [in] */ IIntent* pIntent,
    /* [out, callee] */ ArrayOf<String>** ppHandlers)
{
    if (!ppHandlers) return E_INVALID_ARGUMENT;

    IContext* pIContext;
    ECode ec = GeckoApp::surfaceView->GetContext(&pIContext);
    if (FAILED(ec)) return ec;

    //IPackageManager* pPm;
    //ec = pIContext->GetPackageManager();
    pIContext->Release();
    if (FAILED(ec)) return ec;

    List<IResolveInfo*> list;
    //ec = pPm->QueryIntentActivities(pIntent, 0, &list);
    //if (FAILED(ec)) {
    //    pPm->Release();
   //     return ec;
   // }

    Int32 size = list.GetSize();
    Int32 numAttr = 4;
    ArrayOf<String>* ret = ArrayOf<String>::Alloc(size * numAttr);
    if (!ret) {
        //pPm->Release();
        return E_OUT_OF_MEMORY;
    }

    Boolean isDefault;
    for (int i = 0; i < size; i++) {
        IResolveInfo* pResolveInfo = list[i];
        String lable;
        //pResolveInfo->LoadLabel(pPm, &lable);
        //ret[i * numAttr] = lable;
        pResolveInfo->IsDefault(&isDefault);
        if (isDefault) {
            (*ret)[i * numAttr + 1] = "default";
        }
        else {
            (*ret)[i * numAttr + 1] = "";
        }
        //(*ret)[i * numAttr + 2] = pResolveInfo->ActivityInfo.applicationInfo.packageName;
        //(*ret)[i * numAttr + 3] = pResolveInfo->ActivityInfo.name;
        pResolveInfo->Release();
    }
    *ppHandlers = ret;
    return NOERROR;
}

ECode GeckoAppShell::GetIntentForActionString(
    /* [in] */ const String& aAction,
    /* [out] */ IIntent** ppIntent)
{
    if (!ppIntent) return E_INVALID_ARGUMENT;

    // Default to the view action if no other action as been specified.
    ECode ec;
    if (!aAction.IsNullOrEmpty()) {
        ec = CIntent::New(aAction, ppIntent);
    }
    else {
        //ec = CIntent::New(Intent_ACTION_VIEW, ppIntent);
    }
    return ec;
}

ECode GeckoAppShell::GetExtensionFromMimeType(
    /* [in] */ const String& aMimeType,
    /* [out] */ String* ext)
{
    if (!ext) return E_INVALID_ARGUMENT;

    //return MimeTypeMap.getSingleton().getExtensionFromMimeType(aMimeType);
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::GetMimeTypeFromExtensions(
    /* [in] */ const String& aFileExt,
    /* [out] */ String* mimeType)
{
    if (!mimeType) return E_INVALID_ARGUMENT;

    /*IMimeTypeMap pMtm;
    ECode ec = CMimeTypeMap::AquireSingleton(&pMtm);
    if (FAILED(ec)) return ec;
    StringTokenizer st = new StringTokenizer(aFileExt, "., ");
    String type = null;
    String subType = null;
    while (st.hasMoreElements()) {
        String ext = st.nextToken();
        String mt = mtm.getMimeTypeFromExtension(ext);
        if (mt == null)
            continue;
        int slash = mt.indexOf('/');
        String tmpType = mt.substring(0, slash);
        if (!tmpType.equalsIgnoreCase(type))
            type = type == null ? tmpType : "*";
        String tmpSubType = mt.substring(slash + 1);
        if (!tmpSubType.equalsIgnoreCase(subType))
            subType = subType == null ? tmpSubType : "*";
    }
    if (type == null)
        type = "*";
    if (subType == null)
        subType = "*";
    return type + "/" + subType;*/
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::OpenUriExternal(
    /* [in] */ const String& aUriSpec,
    /* [in] */ const String& aMimeType,
    /* [in] */ const String& aPackageName,
    /* [in] */ const String& aClassName,
    /* [in] */ const String& aAction,
    /* [in] */ const String& aTitle)
{
    IIntent* pIIntent;
    ECode ec = GetIntentForActionString(aAction, &pIIntent);
    if (FAILED(ec)) return ec;

    if (1) {//aAction.EqualsIgnoreCase(Intent_ACTION_SEND)) {
        ec = pIIntent->PutStringExtra(String(Intent_EXTRA_TEXT), aUriSpec);
        if (FAILED(ec)) goto exit;
        ec = pIIntent->PutStringExtra(String(Intent_EXTRA_SUBJECT), aTitle);
        if (FAILED(ec)) goto exit;
        if (!aMimeType.IsNullOrEmpty()) {
            ec = pIIntent->SetType(aMimeType);
            if (FAILED(ec)) goto exit;
        }
    }
    else if (!aMimeType.IsNullOrEmpty()) {
        /*IUri* pIUri;
        ec = Uri::Parse(aUriSpec, &pIUri);
        if (FAILED(ec)) goto exit;

        ec = pIIntent->SetDataAndType(pIUri, aMimeType);
        pIUri->Release();
        if (FAILED(ec)) goto exit;*/
    }
    else {
        String scheme;
        /*IUri* pIUri;
        ec = Uri.Parse(aUriSpec, &pIUri);
        if (FAILED(ec)) goto exit;

        ec = pIUri->GetScheme(&scheme);
        if (FAILED(ec)){
            pIUri->Release();
            goto exit;
        }*/
        if (scheme.Equals("sms")) {
            // Have a apecial handling for the SMS, as the message body
            // is not extracted from the URI automatically
            String query;
            /*ec = pIUri->GetEncodedQuery(&query);
            if (FAILED(ec)){
                pIUri->Release();
                goto exit;
            }*/
            if (!query.IsNullOrEmpty()) {
                StringTokenizer fields(query.string(), "&");
                Boolean foundBody = FALSE;
                String resultQuery("");
                while (fields.HasMoreTokens()) {
                    String field = fields.NextToken();
                    if (field.GetLength() > 5 && field.Substring(0, 5).Equals("body=")) {
                        //String body = Uri::Decode(field.Substring(5));
                        //pIIntent->PutExtra("sms_body", body);
                        foundBody = TRUE;
                    }
                    else {
                        String temp("&");
                        temp.Append(field);
                        resultQuery.Append(resultQuery.GetLength() > 0 ? temp : field);
                    }
                }
                if (foundBody) {
                    // Put the query without the body field back into the URI
                    String temp(aUriSpec.Substring(0, aUriSpec.IndexOf('?')));
                    temp.Append("?");
                    temp.Append(resultQuery);
                    //pIUri->Release();
                    //ec = Uri::Parse(resultQuery.GetLength() > 0 ? temp : prefix, &pIUri);
                    //if (FAILED(ec)) goto exit;
                }
            }
        }
        /*ec = pIIntent->SetData(pIUri);
        pIUri->Release();
        if (FAILED(ec)) goto exit;*/
    }
    if (aPackageName.GetLength() > 0 && aClassName.GetLength() > 0) {
        ec = pIIntent->SetClassName(aPackageName, aClassName);
        if (FAILED(ec)) goto exit;
    }

    ec = pIIntent->SetFlags(Intent_FLAG_ACTIVITY_CLEAR_TOP);
    if (FAILED(ec)) goto exit;
    //try {
        IContext* pIContext;
        ec = GeckoApp::surfaceView->GetContext(&pIContext);
        if (FAILED(ec)) goto exit;

        ec = pIContext->StartActivity(pIIntent);
        pIContext->Release();
    //} catch(ActivityNotFoundException e) {
    //    return false;
    //}

exit:
    pIIntent->Release();
    return ec;
}

class ClipboardQueueRunnable : public GeckoRunnable
{
public:
    ClipboardQueueRunnable()
        : GeckoRunnable(NULL)
    {
    }

    ECode Run()
    {
        IContext* pContext;
        ECode ec = GeckoApp::surfaceView->GetContext(&pContext);
        if (FAILED(ec)) return ec;

        //IClipboardManager* pCm;
        //ec = pContext->GetSystemService(Context_CLIPBOARD_SERVICE, &pCm);
        pContext->Release();
        if (FAILED(ec)) return ec;
        //try {
            /*Boolean hasText;
            pCm->HasText(&hasText);
            sClipboardQueue.Put(hasText ? pCm->GetText().ToString() : "");
            pCm->Release();*/
        //} catch (InterruptedException ie) {}
        return NOERROR;
    }
};

// On some devices, access to the clipboard service needs to happen
// on a thread with a looper, so dispatch this to our looper thread
// Note: the main looper won't work because it may be blocked on the
// gecko thread, which is most likely this thread
ECode GeckoAppShell::GetClipboardText(
    /* [out] */ String* text)
{
    if (!text) return E_INVALID_ARGUMENT;

    IHandler* pIHandler;
    ECode ec = GetHandler(&pIHandler);
    if (FAILED(ec)) return ec;

    ClipboardQueueRunnable* pRunnable = new ClipboardQueueRunnable();
    if (!pRunnable) {
        pIHandler->Release();
        return E_OUT_OF_MEMORY;
    }
    //pIHandler->Post(IRunnable::Probe(pRunnable));
    pIHandler->Release();

    /*try {
        String ret = sClipboardQueue.take();
        return ret == "" ? null : ret;
    } catch (InterruptedException ie) {}
    return null;*/
    return NOERROR;
}

class ClipboardSetRunnable : public GeckoRunnable
{
public:
    ClipboardSetRunnable(
        /* [in] */ const String& text)
        : GeckoRunnable(NULL)
    {
        mClipeBoardText = text;
    }

    ECode Run()
    {
        IContext* pContext;
        ECode ec = GeckoApp::surfaceView->GetContext(&pContext);
        if (FAILED(ec)) return ec;

        /*IClipboardManager* pCm;
        ec = pContext->GetSystemService(Context_CLIPBOARD_SERVICE, &pCm);
        pContext->Release();
        if (FAILED(ec)) return ec;
        pCm->SetText(mClipeBoardText);
        pCm->Release();*/
        return NOERROR;
    }
private:
    String mClipeBoardText;
};

ECode GeckoAppShell::SetClipboardText(
    /* [in] */ const String& text)
{
    IHandler* pIHandler;
    ECode ec = GetHandler(&pIHandler);
    if (FAILED(ec)) return ec;

    ClipboardSetRunnable* pRunnable = new ClipboardSetRunnable(text);
    if (!pRunnable) {
        pIHandler->Release();
        return E_OUT_OF_MEMORY;
    }
    //ec = pIHandler->Post(IRunnalbe::Probe(pRunnable));
    pIHandler->Release();
    return ec;
}

ECode GeckoAppShell::ShowAlertNotification(
    /* [in] */ const String& aImageUrl,
    /* [in] */ const String& aAlertTitle,
    /* [in] */ const String& aAlertText,
    /* [in] */ const String& aAlertCookie,
    /* [in] */ String aAlertName)
{
    printf("GeckoAppJava GeckoAppShell.showAlertNotification - image = '"
        "%s'\n- title = '%s'\n- text = '%s'\n- cookie = '%s'\n- name = '%s'",
         aImageUrl.string(), aAlertTitle.string(), aAlertText.string(),
         aAlertCookie.string(), aAlertName.string());

    Int32 icon = 0x7f020004; //R.drawable.icon; // Just use the app icon by default

    String packageName;
    //IUri* pImageUri = NULL;
    //IUri* pDataUri = NULL;
    IAlertNotification* pNotification = NULL;
    ECode ec;
    //ec = Uri::Parse(aImageUrl, &pImageUri);
    //if (FAILED(ec)) return ec;
    String scheme;
    //ec = pImageUri->GetScheme(&scheme);
    //if (FAILED(ec)) goto exit;

    if (scheme.Equals("drawable")) {
        String resource;
        //ec = pImageUri->GetSchemeSpecificPart(&resource);
        //if (FAILED(ec)) goto exit;

        resource = resource.Substring(resource.LastIndexOf('/') + 1);
        /*try {
            Class drawableClass = R.drawable.class;
            Field f = drawableClass.getField(resource);
            icon = f.getInt(null);
        } catch (Exception e) {} // just means the resource doesn't exist*/
        //pImageUri->Release();
        //pImageUri = NULL;
    }

    Int32 notificationID = aAlertName.GetHashCode();

    // Remove the old notification with the same ID, if any
    RemoveNotification(notificationID);

    Int64 timeMillis;
    //ec = System::CurrentTimeMillis(&timeMillis);
    //if (FAILED(ec)) goto exit;

    ec = CAlertNotification::New(GeckoApp::mAppContext, notificationID,
                     icon, aAlertTitle, aAlertText, timeMillis, &pNotification);
    if (FAILED(ec)) goto exit;

    // The intent to launch when the user clicks the expanded notification
    IIntent* pNotificationIntent;
    ec = CIntent::New(String(GeckoApp_ACTION_ALERT_CLICK), &pNotificationIntent);
    if (FAILED(ec)) goto exit;

    ec = GeckoApp::mAppContext->GetPackageName(&packageName);
    if (FAILED(ec)){
        pNotificationIntent->Release();
        goto exit;
    }
    packageName.Append(".NotificationHandler");
    /*ec = pNotificationIntent->SetClassName(
                    GeckoApp::mAppContext, packageName);
    if (FAILED(ec)){
        pNotificationIntent->Release();
        goto exit;
    }*/

    // Put the strings into the intent as an URI "alert:<name>#<cookie>"
    /*ec = Uri::FromParts("alert", aAlertName, aAlertCookie, &pDataUri);
    if (FAILED(ec)){
        pNotificationIntent->Release();
        goto exit;
    }*/
    //ec = pNotificationIntent->SetData(pDataUri);
    //pDataUri->Release();
    if (FAILED(ec)){
        pNotificationIntent->Release();
        goto exit;
    }

    /*IPendingIntent* pContentIntent;
    ec = PendingIntent::GetBroadcast(GeckoApp::mAppContext,
                        0, pNotificationIntent, 0, &pContentIntent);
    pNotificationIntent->Release();
    if (FAILED(ec)) goto exit;

    ec = pNotification->SetLatestEventInfo(GeckoApp::mAppContext,
                             aAlertTitle, aAlertText, pContentIntent);
    pContentIntent->Release();
    if (FAILED(ec)) goto exit;
    ec = pNotification->SetCustomIcon(pImageUri);
    if (FAILED(ec)) goto exit;*/

    // The intent to execute when the status entry is deleted by the user with the "Clear All Notifications" button
    IIntent* pClearNotificationIntent;
    ec = CIntent::New(String(GeckoApp_ACTION_ALERT_CLEAR), &pClearNotificationIntent);
    if (FAILED(ec)) goto exit;

    GeckoApp::mAppContext->GetPackageName(&packageName);
    packageName += ".NotificationHandler";
    //pClearNotificationIntent->SetClassName(GeckoApp::mAppContext, packageName);
    //pClearNotificationIntent->SetData(pDataUri);

    /*IPendingIntent* pDeleteIntent;
    ec = PendingIntent::GetBroadcast(GeckoApp::mAppContext,
                         0, pClearNotificationIntent, 0, &pDeleteIntent);
    pClearNotificationIntent->Release();
    if (FAILED(ec)) goto exit;
    ec = pNotification->SetDeleteIntent(pDeleteIntent);
    pDeleteIntent->Release();
    if (FAILED(ec)) goto exit;*/

    mAlertNotifications[notificationID] = pNotification;

    pNotification->Show();

    printf("GeckoAppJava Created notification ID %d\n", notificationID);
exit:
    //if (pImageUri) pImageUri->Release();
    //if (pDataUri) pDataUri->Release();
    if (pNotification) pNotification->Release();
    return ec;
}

ECode GeckoAppShell::AlertsProgressListener_OnProgress(
    /* [in] */ String aAlertName,
    /* [in] */ Int32 aProgress,
    /* [in] */ Int32 aProgressMax,
    /* [in] */ const String& aAlertText)
{
    /*Log.i("GeckoAppJava", "GeckoAppShell.alertsProgressListener_OnProgress\n" +
        "- name = '" + aAlertName +"', " +
        "progress = " + aProgress +" / " + aProgressMax + ", text = '" + aAlertText + "'");*/

    Int32 notificationID = aAlertName.GetHashCode();
    HashMap<Int32, IAlertNotification*>::Iterator it = 
                        mAlertNotifications.Find(notificationID);
    if (it == mAlertNotifications.End()) return NOERROR;
    IAlertNotification* pNotification = it->mSecond;
    if (pNotification) {
        pNotification->UpdateProgress(aAlertText, aProgress, aProgressMax);
    }

    if (aProgress == aProgressMax) {
        // Hide the notification at 100%
        RemoveObserver(aAlertName);
        RemoveNotification(notificationID);
    }
    return NOERROR;
}

ECode GeckoAppShell::AlertsProgressListener_OnCancel(
    /* [in] */ String aAlertName)
{
    printf("GeckoAppJava GeckoAppShell.alertsProgressListener_OnCancel('%s')\n",
         aAlertName.string());

    RemoveObserver(aAlertName);

    Int32 notificationID = aAlertName.GetHashCode();
    RemoveNotification(notificationID);
    return NOERROR;
}

ECode GeckoAppShell::HandleNotification(
    /* [in] */ const String& aAction,
    /* [in] */ String aAlertName,
    /* [in] */ const String& aAlertCookie)
{
    Int32 notificationID = aAlertName.GetHashCode();

    if (aAction.Equals(GeckoApp_ACTION_ALERT_CLICK)) {
        printf("GeckoAppJava GeckoAppShell.handleNotification: callObserver(alertclickcallback)\n");
        CallObserver(aAlertName, String("alertclickcallback"), aAlertCookie);
        HashMap<Int32, IAlertNotification*>::Iterator it = 
                    mAlertNotifications.Find(notificationID);
        if (it == mAlertNotifications.End()) return NOERROR;
        IAlertNotification* pNotification = it->mSecond;
        if (pNotification) {
            Boolean isProgressStyle;
            pNotification->IsProgressStyle(&isProgressStyle);
            // When clicked, keep the notification, if it displays a progress
            if (isProgressStyle) return NOERROR;
        }
    }

    CallObserver(aAlertName, String("alertfinished"), aAlertCookie);

    RemoveObserver(aAlertName);

    RemoveNotification(notificationID);
    return NOERROR;
}

ECode GeckoAppShell::RemoveNotification(
    /* [in] */ Int32 notificationID)
{
    mAlertNotifications.Erase(notificationID);

    INotificationManager* pNotificationManager;
    ECode ec = GeckoApp::mAppContext->GetSystemService(
                            Context_NOTIFICATION_SERVICE,
                            (IInterface**)&pNotificationManager);
    if (FAILED(ec)) return ec;
    //ec = pNotificationManager->Cancel(notificationID);
    pNotificationManager->Release();
    return ec;
}

ECode GeckoAppShell::GetDpi(
    /* [out] */ Int32* pDpi)
{
    IDisplayMetrics* pMetrics;
    ECode ec = CDisplayMetrics::New(&pMetrics);
    if (FAILED(ec)) return ec;

    IWindowManager* pIWinManager;
    ec = GeckoApp::mAppContext->GetWindowManagerEx(&pIWinManager);
    if (FAILED(ec)) goto exit;

    IDisplay* pIDisplay;
    ec = pIWinManager->GetDefaultDisplay(&pIDisplay);
    pIWinManager->Release();
    if (FAILED(ec)) goto exit;

    ec = pIDisplay->GetMetrics(pMetrics);
    pIDisplay->Release();
    if (FAILED(ec)) goto exit;

    //ec = pMetrics->GetDensityDpi(pDpi);

exit:
    pMetrics->Release();
    return ec;
}

ECode GeckoAppShell::SetFullScreen(
    /* [in] */ Boolean fullscreen)
{
    GeckoApp::mFullscreen = fullscreen;

    // force a reconfiguration to hide/show the system bar
    /*GeckoApp::mAppContext->SetRequestedOrientation(
                    ActivityInfo_SCREEN_ORIENTATION_PORTRAIT);
    GeckoApp::mAppContext->SetRequestedOrientation(
                    ActivityInfo_SCREEN_ORIENTATION_LANDSCAPE);
    GeckoApp::mAppContext->SetRequestedOrientation(
                    ActivityInfo_SCREEN_ORIENTATION_USER);*/

    return NOERROR;
}

ECode GeckoAppShell::ShowFilePicker(
    /* [in] */ const String& aFilters,
    /* [out] */ String* pPicker)
{
    if (!pPicker) return E_INVALID_ARGUMENT;

    String mimeType;
    ECode ec = GetMimeTypeFromExtensions(aFilters, &mimeType);
    if (FAILED(ec)) return ec;

    return GeckoApp::mAppContext->ShowFilePicker(mimeType, pPicker);
}

ECode GeckoAppShell::PerformHapticFeedback(
    /* [in] */ Boolean aIsLongPress)
{
    Boolean result;
    return GeckoApp::surfaceView->PerformHapticFeedback(aIsLongPress ?
                                  HapticFeedbackConstants_LONG_PRESS :
                                  HapticFeedbackConstants_VIRTUAL_KEY, &result);
}

ECode GeckoAppShell::ShowInputMethodPicker()
{
    IContext* pIContext;
    ECode ec = GeckoApp::surfaceView->GetContext(&pIContext);
    if (FAILED(ec)) return ec;

    ILocalInputMethodManager* pImm;
    ec = pIContext->GetSystemService(
            Context_INPUT_METHOD_SERVICE, (IInterface**)&pImm);
    pIContext->Release();
    if (FAILED(ec)) return ec;

    ec = pImm->ShowInputMethodPicker();
    pImm->Release();
    return ec;
}

ECode GeckoAppShell::HideProgressDialog()
{
    CGeckoSurfaceView::sShowingSplashScreen = FALSE;
    return NOERROR;
}

class SetScreenRunnable : public GeckoRunnable
{
public:
    SetScreenRunnable(
        /* [in] */ Boolean on)
        : GeckoRunnable(NULL)
        , mScreenOn(on)
    {
    }

    ECode Run()
    {
        GeckoApp::surfaceView->SetKeepScreenOn(mScreenOn);
        return NOERROR;
    }
private:
    Boolean mScreenOn;
};

ECode GeckoAppShell::SetKeepScreenOn(
    /* [in] */ Boolean on)
{
    SetScreenRunnable* pRunnable = new SetScreenRunnable(on);
    if (!pRunnable) return E_OUT_OF_MEMORY;
    //return GeckoApp::mAppContext->RunOnUiThread(IRunnable::Probe(pRunnable));
    return NOERROR;
}

ECode GeckoAppShell::IsNetworkLinkUp(
    /* [out] */ Boolean* pIsLinkUp)
{
    if (!pIsLinkUp) return E_INVALID_ARGUMENT;

    IConnectivityManager* pCm;
    ECode ec = GeckoApp::mAppContext->GetSystemService(
                Context_CONNECTIVITY_SERVICE, (IInterface**)&pCm);
    if (FAILED(ec)) return ec;

    INetworkInfo* pINetworkInfo = NULL;
    ec = pCm->GetActiveNetworkInfo(&pINetworkInfo);
    pCm->Release();
    if (FAILED(ec)) return ec;

    if (!pINetworkInfo) {
        *pIsLinkUp = FALSE;
        return NOERROR;
    }

    Boolean isConnected;
    ec = pINetworkInfo->IsConnected(&isConnected);
    pINetworkInfo->Release();
    if (FAILED(ec)) return ec;

    if (!isConnected) {
        *pIsLinkUp = FALSE;
    }
    else {
        *pIsLinkUp = TRUE;
    }
    return NOERROR;
}

ECode GeckoAppShell::IsNetworkLinkKnown(
    /* [out] */ Boolean* pIsLinkKnown)
{
    if (!pIsLinkKnown) return E_INVALID_ARGUMENT;

    IConnectivityManager* pCm;
    ECode ec = GeckoApp::mAppContext->GetSystemService(
                Context_CONNECTIVITY_SERVICE, (IInterface**)&pCm);
    if (FAILED(ec)) return ec;

    INetworkInfo* pINetworkInfo = NULL;
    ec = pCm->GetActiveNetworkInfo(&pINetworkInfo);
    pCm->Release();
    if (FAILED(ec)) return ec;

    if (!pINetworkInfo) {
        *pIsLinkKnown = FALSE;
    }
    else {
        *pIsLinkKnown = TRUE;
    }
    return NOERROR;
}

ECode GeckoAppShell::SetSelectedLocale(
    /* [in] */ const String& localeCode)
{
    ECode ec;
    /*ISharedPreferences* pSettings;
    ec = GeckoApp::mAppContext->GetPreferences(
                        Activity_MODE_PRIVATE, &pSettings);
    if (FAILED(ec)) return ec;
    ISharedPreferencesEditor* pEditor;
    ec = pSettings->Edit(&pEditor);
    pSettings->Release();
    if (FAILED(ec)) return ec;
    String packageName;
    GeckoApp::mAppContext->GetPackageName(&packageName);
    packageName += ".locale";
    pEditor->PutString(packageName,localeCode);
    ec = pEditor->Commit();
    pEditor->Release();
    if (FAILED(ec)) return ec;*/

    ILocale* pLocale;
    Int32 index;
    if ((index = localeCode.IndexOf('-')) != -1) {
        String langCode = localeCode.Substring(0, index);
        String countryCode = localeCode.Substring(index + 1);
        ec = CLocale::New(langCode, countryCode, &pLocale);
    } else {
        ec = CLocale::New(localeCode, &pLocale);
    }
    if (FAILED(ec)) return ec;
    //Locale::SetDefault(pLocale);

    IContext* pIContext;
    ec = GeckoApp::mAppContext->GetBaseContext(&pIContext);
    if (FAILED(ec)) {
        pLocale->Release();
        return ec;
    }
    IResources* pRes;
    ec = pIContext->GetResources(&pRes);
    pIContext->Release();
    if (FAILED(ec)) {
        pLocale->Release();
        return ec;
    }
    IConfiguration* pConfig;
    ec = pRes->GetConfiguration(&pConfig);
    if (FAILED(ec)) {
        pRes->Release();
        pLocale->Release();
        return ec;
    }
    //pConfig->SetLocale(pLocale);
    pLocale->Release();

    /*IDisplayMerics* pIDM;
    ec = pRes->GetDisplayMetrics(&pIDM);
    if (FAILED(ec)) {
        pRes->Release();
        pConfig->Release();
        return ec;
    }
    ec = pRes->UpdateConfiguration(pConfig, pIDM);
    pIDM->Release();*/
    pRes->Release();
    pConfig->Release();
    return ec;
}

ECode GeckoAppShell::GetSystemColors(
    /* [out] */ ArrayOf<Int32>** ppColors)
{
    if (!ppColors) return E_INVALID_ARGUMENT;

    // attrsAppearance[] must correspond to AndroidSystemColors structure in android/AndroidBridge.h
    ArrayOf_<Int32, 11> attrsAppearance;
    attrsAppearance[0] = 0x01010098;//android.R.attr.textColor;
    attrsAppearance[1] = 0x01010036;//android.R.attr.textColorPrimary;
    attrsAppearance[2] = 0x01010039;//android.R.attr.textColorPrimaryInverse;
    attrsAppearance[3] = 0x01010038;//android.R.attr.textColorSecondary;
    attrsAppearance[4] = 0x0101003a;//android.R.attr.textColorSecondaryInverse;
    attrsAppearance[5] = 0x01010212;//android.R.attr.textColorTertiary;
    attrsAppearance[6] = 0x01010213;//android.R.attr.textColorTertiaryInverse;
    attrsAppearance[7] = 0x01010099;//android.R.attr.textColorHighlight;
    attrsAppearance[8] = 0x01010031;//android.R.attr.colorBackground;
    attrsAppearance[9] = 0x01010060;//android.R.attr.panelColorForeground;
    attrsAppearance[10] = 0x0101005f;//android.R.attr.panelColorBackground;

    Int32 length = attrsAppearance.GetLength();
    ArrayOf<Int32>* pResult = ArrayOf<Int32>::Alloc(length);
    if (!pResult) return E_OUT_OF_MEMORY;

    ECode ec;
    IContextThemeWrapper* pContextThemeWrapper;
    ec = CContextThemeWrapper::New(GeckoApp::mAppContext,
             0x0103003e, &pContextThemeWrapper);//android.R.style.TextAppearance
    if (FAILED(ec)) goto exit;

    ITheme* pITheme;
    ec = pContextThemeWrapper->GetTheme(&pITheme);
    pContextThemeWrapper->Release();
    if (FAILED(ec)) goto exit;

    ITypedArray* pAppearance;
    ec = pITheme->ObtainStyledAttributes(attrsAppearance, &pAppearance);
    pITheme->Release();
    if (FAILED(ec)) goto exit;

    for (Int32 i = 0; i < length; i++) {
        Int32 idx, color;
        pAppearance->GetIndex(i, &idx);
        pAppearance->GetColor(idx, 0, &color);
        (*pResult)[idx] = color;
    }
    pAppearance->Recycle();
    pAppearance->Release();

    *ppColors = pResult;

exit:
    if (FAILED(ec)) {
        ArrayOf<Int32>::Free(pResult);
    }
    return ec;
}

class GeckoProcessVisitorChild : public GeckoAppShell::GeckoProcessesVisitor
{
public:
    Boolean Callback(
        /* [in] */ Int32 pid) {
        //if (pid != Process::MyPid()) {
        //   Process::KillProcess(pid);
        //}
        return TRUE;
    }
};

ECode GeckoAppShell::KillAnyZombies()
{
    GeckoProcessVisitorChild* pVisitor = new GeckoProcessVisitorChild();
    if (!pVisitor) return E_OUT_OF_MEMORY;

    ECode ec = EnumerateGeckoProcesses(pVisitor);
    delete pVisitor;
    return ec;
}

class GeckoPidCallback : public GeckoAppShell::GeckoProcessesVisitor
{
public:
    GeckoPidCallback() 
        : otherPidExist(FALSE)
    {
    }

    Boolean Callback(
        /* [in] */ Int32 pid) {
        /*if (pid != Process::MyPid()) {
           otherPidExist = TRUE;
           return FALSE;
        }*/
        return TRUE;
    }

    Boolean otherPidExist;
};

ECode GeckoAppShell::CheckForGeckoProcs(
    /* [out] */ Boolean* pResult)
{
    if (!pResult) return E_INVALID_ARGUMENT;

    GeckoPidCallback* pVisitor = new GeckoPidCallback();
    if (!pVisitor) return E_OUT_OF_MEMORY;

    ECode ec = EnumerateGeckoProcesses(pVisitor);
    if (FAILED(ec)) {
        delete pVisitor;
        return ec;
    }

    *pResult = pVisitor->otherPidExist;
    delete pVisitor;
    return NOERROR;
}

ECode GeckoAppShell::WaitForAnotherGeckoProc()
{
    Int32 countDown = 40;
    Boolean result;
    AutoPtr<IThread> pThread;
    ECode ec;
    CheckForGeckoProcs(&result);
    while (!result &&  --countDown > 0) {
        //try {
        pThread = Thread::GetCurrentThread();
        if (FAILED(ec)) return ec;
        //pThread->Sleep(100);
        //} catch (InterruptedException ie) {}
        CheckForGeckoProcs(&result);
    }
}

ECode GeckoAppShell::ScanMedia(
    /* [in] */ const String& aFile,
    /* [in] */ const String& aMimeType)
{
    /*Context context = GeckoApp.surfaceView.getContext();
    GeckoMediaScannerClient client = new GeckoMediaScannerClient(context, aFile, aMimeType);*/
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::GetIconForExtension(
    /* [in] */ String& aExt,
    /* [in] */ Int32 iconSize,
    /* [out] */ ArrayOf<Byte>** ppIcons)
{
    //try {
        if (!ppIcons) return E_INVALID_ARGUMENT;

        if (iconSize <= 0) {
            iconSize = 16;
        }

        if (aExt.GetLength() > 1 && aExt.GetChar(0) == '.') {
            aExt = aExt.Substring(1);
        }

        /*IContext* pIContext;
        ECode ec = GeckoApp::surfaceView->GetContext(&pIContext);
        if (FAILED(ec)) return ec;

        IPackageManager* pPm;
        ec = pIContext->GetPackageManager(&pPm);
        pIContext->Release();
        if (FAILED(ec)) return ec;

        IDrawable* pIcon;
        ec = GetDrawableForExtension(pPm, aExt, &pIcon);
        if (FAILED(ec)) {
            pPm->Release();
            return ec;
        }

        if (!pIcon) {
            // Use a generic icon
            ec = pPm->GetDefaultActivityIcon(&pIcon);
            if (FAILED(ec)) {
                pPm->Release();
                return ec;
            }
        }
        pPm->Release();

        IBitmapDrawable* pIBDrawable = IBitmapDrawable::Probe(pIcon);
        if (!pIBDrawable) return E_NO_INTERFACE;

        IBitmap* pIBitmap;
        ec = pIBDrawable->GetBitmap(&pIBitmap);
        pIcon->Release();
        if (FAILED(ec)) return ec;

        Int32 width = 0, height = 0;
        pIBitmap->GetWidth(&width);
        pIBitmap->GetHeight(&height);
        if (width != iconSize || height != iconSize) {
            IBitmapFactory* pBFactory;
            ec = CBitmapFactory::AcquireSinglton(&pBFactory);
            if (FAILED(ec)) {
                pIBitmap->Release();
                return ec;
            }
            IBitmap* pTemp;
            ec = CBitmap::CreateScaledBitmap(
                    pBFactory, pIBitmap, iconSize, iconSize, TRUE, &pTemp);
            pBFactory->Release();
            pIBitmap->Release();
            if (FAILED(ec)) return ec;
            pIBitmap = pTemp;
        }

        IByteBuffer* pBuf;
        ec = ByteBuffer::Allocate(iconSize * iconSize * 4, &pBuf);
        if (FAILED(ec)) {
            pIBitmap->Release();
            return ec;
        }
        ec = pIBitmap->CopyPixelsToBuffer(pBuf);
        pIBitmap->Release();
        if (FAILED(ec)) {
            pBuf->Release();
            return ec;
        }

        ec = pBuf->Array(ppIcons);
        pBuf->Release();
        return ec;*/
    //}
    //catch (Exception e) {
    //    Log.i("GeckoAppShell", "getIconForExtension error: ",  e);
    //    return null;
    //}
}

// Tell the Gecko event loop that an event is available.
Void GeckoAppShell::NotifyGeckoOfEvent(
    /* [in] */ GeckoEvent* pEvent)
{
    //call native function.
    //return notifyGeckoOfEvent(pEvent);
    return;
}

/* The Android-side API: API methods that Android calls */

// Initialization methods
Void GeckoAppShell::NativeInit()
{
    //return nativeInit();
    return;
}

Void GeckoAppShell::NativeRun(
    /* [in] */ const String& args)
{
    //return nativeRun(args);
    return;
}

// helper methods
Void GeckoAppShell::SetSurfaceView(
    /* [in] */ CGeckoSurfaceView* pSurfaceView)
{
    //return setSurfaceView(pSurfaceView);
    return;
}

Void GeckoAppShell::PutEnv(
    /* [in] */ const String& map)
{
    //return putEnv(map);
    return;
}

Void GeckoAppShell::OnResume()
{
    //return OnResume();
    return;
}

Void GeckoAppShell::OnLowMemory()
{
    //return onLowMemory();
    return;
}

Void GeckoAppShell::CallObserver(
    /* [in] */ const String& observerKey,
    /* [in] */ const String& topic,
    /* [in] */ const String& data)
{
    //return callObserver(observerKey, topic, data);
    return;
}

Void GeckoAppShell::RemoveObserver(
    /* [in] */ const String& observerKey)
{
    //return removeObserver(observerKey);
    return;
}

Void GeckoAppShell::LoadLibs(
    /* [in] */ const String& apkName,
    /* [in] */ Boolean shouldExtract)
{
    //return loadLibs(apkName, shouldExtract);
    return;
    
}

Void GeckoAppShell::OnChangeNetworkLinkStatus(
    /* [in] */ const String& status)
{
    //return onChangeNetworkLinkStatus(status);
    return;
}

Void GeckoAppShell::ReportJavaCrash(
    /* [in] */ const String& stack)
{
    //return reportJavaCrash(stack);
    return;
}

ECode GeckoAppShell::PutLocaleEnv()
{
   /* GeckoAppShell::PutEnv("LANG=" + Locale.getDefault().toString());
    NumberFormat nf = NumberFormat.getInstance();
    if (nf instanceof DecimalFormat) {
        DecimalFormat df = (DecimalFormat)nf;
        DecimalFormatSymbols dfs = df.getDecimalFormatSymbols();

        GeckoAppShell::PutEnv("LOCALE_DECIMAL_POINT=" + dfs.getDecimalSeparator());
        GeckoAppShell::PutEnv("LOCALE_THOUSANDS_SEP=" + dfs.getGroupingSeparator());
        GeckoAppShell::PutEnv("LOCALE_GROUPING=" + (char)df.getGroupingSize());
    }*/
    return NOERROR;
}

ECode GeckoAppShell::SendPendingEventsToGecko()
{
    //try {
        List<GeckoEvent*>::Iterator it = gPendingEvents.Begin();
        for (; it != gPendingEvents.End(); ++it) {
            GeckoEvent* pEvent = *it;
            NotifyGeckoOfEvent(pEvent);
            delete pEvent;
        }
        gPendingEvents.Clear();
    //} catch (NoSuchElementException e) {}
    return NOERROR;
}

ECode GeckoAppShell::EnumerateGeckoProcesses(
    /* [in] */ GeckoProcessesVisitor* pVisiter)
{
    /*Int32 tokenSoFar, uid, pid;
    Boolean keepGoing;
    String token;
    //try {
        // run ps and parse its output
        IRuntime* pIRuntime;
        ECode ec = Runtime::GetRuntime(&pIRuntime);
        if (FAILED(ec)) return ec;
        java.lang.IProcess* pPs;
        ec = pIRuntime->Exec("ps", &pPs);
        pIRuntime->Release();
        if (FAILED(ec)) return ec;

        IInputStream* pInS;
        ec = pPs->GetInputStream(&pInS);
        if (FAILED(ec)) return ec;
        IInputStreamReader* pInSReader;
        ec = CInputStreamReader::New(pInS, &pInSReader);
        pInS->Release();
        if (FAILED(ec)) return ec;
        IBufferedReader* pIn;
        ec = CBufferedReader::(pInSReader, 2048, &pIn);
        pInSReader->Release();
        if (FAILED(ec)) return ec;

        String headerOutput;
        ec = pIn->ReadLine(&headerOutput);
        if (FAILED(ec)) goto exit;

        // figure out the column offsets.  We only care about the pid and user fields
        if (sPidColumn == -1 || sUserColumn == -1) {
            StringTokenizer st(headerOutput.string());

            tokenSoFar = 0;
            while(st.HasMoreTokens()) {
                next = st.NextToken();
                if (next.EqualsIgnoreCase("PID")) {
                    sPidColumn = tokenSoFar;
                }
                else if (next.EqualsIgnoreCase("USER")) {
                    sUserColumn = tokenSoFar;
                }
                tokenSoFar++;
            }
        }

        // alright, the rest are process entries.
        String psOutput;
        ec = pIn->ReadLine(&psOutput);
        while (SUCCEEDED(ec) && !psOutput.IsNull()) {
            StringTokenizer split(psOutput.string(), "\\s+");
            //String[] split = psOutput.split("\\s+");
            Int32 length = split.GetCount();
            if (length <= sPidColumn || length <= sUserColumn) {
                goto next;
            }
            String uidName, pidName,last;
            for (Int32 i = 0; i < length; i++) {
                String token = split.NextToken();
                if (i == sUserColumn) {
                    uidName = token;
                }
                else if (i == sPidColumn) {
                    pidName = token;
                }
                else if (i == length - 1) {
                    last = token;
                }
            }
            uid = Process::GetUidForName(uidName);
            if (uid == Process::MyUid() && !last.EqualsIgnoreCase("ps")) {
                pid = pidName.ToInt32();
                keepGoing = pVisiter->Callback(pid);
                if (!keepGoing) {
                    break;
                }
            }
        next:
            ec = pIn->ReadLine(&psOutput);
        }
    //}
   // catch (Exception e) {
   //     Log.i("GeckoAppShell", "finding procs throws ",  e);
   // }

exit:
    pIn->Close();
    pIn->Release();
    return ec;*/
    return E_NOT_IMPLEMENTED;
}

ECode GeckoAppShell::GetDrawableForExtension(
    ///* [in] */ IPackageManager* pPm,
    /* [in] */ const String& aExt,
    /* [out] */ IDrawable** ppDrawable)
{
    IIntent* pIIntent;
    ECode ec = CIntent::New(&pIIntent);//Intent_ACTION_VIEW, &pIIntent);
    if (FAILED(ec)) return ec;

    /*IMimeTypeMap* pMtm;
    ec = MimeTypeMap::GetSingleton(&pMtm);
    if (FAILED(ec)) {
        pIIntent->Release();
        return ec;
    }

    String mimeType;
    ec = pMtm->GetMimeTypeFromExtension(aExt, &mimeType);
    pMtm->Release();
    if (FAILED(ec)) {
        pIIntent->Release();
        return ec;
    }
    if (!mimeType.IsNullOrEmpty()) {
        pIIntent->SetType(mimeType);
    }
    else {
        pIIntent->Release();
        *ppDrawable = NULL;
        return NOERROR;
    }*/

    List<IResolveInfo*> list;
    //ec = pPm->QueryIntentActivities(pIIntent, 0, &list);
    pIIntent->Release();
    if (FAILED(ec)) return ec;

    Int32 size = list.GetSize();
    if (size == 0) {
        *ppDrawable = NULL;
        return NOERROR;
    }

    IResolveInfo* pResolveInfo = list[0];
    if (!pResolveInfo) {
        *ppDrawable = NULL;
        goto exit;
    }

    IActivityInfo* pIActivityInfo;
    ec = pResolveInfo->GetActivityInfo(&pIActivityInfo);
    if (FAILED(ec)) goto exit;

    //ec = pIActivityInfo->LoadIcon(pPm, ppDrawable);
    pIActivityInfo->Release();

exit:
    for (Int32 i = 0; i < size; i++) {
        IResolveInfo* pResolveInfo = list[i];
        if (pResolveInfo) pResolveInfo->Release();
    }
    return ec;
}

