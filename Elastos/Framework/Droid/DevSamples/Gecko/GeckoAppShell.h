
#ifndef __GECKOAPPSHELL_H__
#define __GECKOAPPSHELL_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "GeckoEvent.h"
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/Thread.h>
#include "CAlertNotification.h"
#include "CGeckoSurfaceView.h"


class GeckoAppShell
{
private:
    // A looper thread, accessed by GeckoAppShell.getHandler
    class LooperThread : public IThread
                       , public Thread
                       , public ElRefBase
    {
    public:
        LooperThread();

        ECode Run();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(Mutex*) GetSelfLock();

        UInt32 AddRef();

        UInt32 Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI CheckAccess();

        CARAPI CountStackFrames(
            /* [out] */ Int32* number);

        CARAPI Destroy();

        CARAPI GetId(
            /* [out] */ Int64* id);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI GetPriority(
            /* [out] */ Int32* priority);

        CARAPI GetState(
            /* [out] */ ThreadState* state);

        CARAPI GetThreadGroup(
            /* [out] */ IThreadGroup** group);

        CARAPI Interrupt();

        CARAPI IsAlive(
            /* [out] */ Boolean* isAlive);

        CARAPI IsDaemon(
            /* [out] */ Boolean* isDaemon);

        CARAPI IsInterrupted(
            /* [out] */ Boolean* isInterrupted);

        CARAPI Join();

        CARAPI JoinEx(
            /* [in] */ Int64 millis);

        CARAPI JoinEx2(
            /* [in] */ Int64 millis,
            /* [in] */ Int32 nanos);

        CARAPI Resume();

        CARAPI SetDaemon(
            /* [in] */ Boolean isDaemon);

        CARAPI SetName(
            /* [in] */ const String& threadName);

        CARAPI SetPriority(
            /* [in] */ Int32 priority);

        CARAPI Start();

        CARAPI Stop();

        CARAPI Suspend();

    public:
         //SynchronousQueue<Handler> mHandlerQueue;
    private:
        Mutex mSync;
    };

    /*private static class GeckoMediaScannerClient implements MediaScannerConnectionClient {
        private String mFile = "";
        private String mMimeType = "";
        private MediaScannerConnection mScanner = null;

        public GeckoMediaScannerClient(Context aContext, String aFile, String aMimeType) {
            mFile = aFile;
            mMimeType = aMimeType;
            mScanner = new MediaScannerConnection(aContext, this);
            if (mScanner != null)
                mScanner.connect();
        }

        public void onMediaScannerConnected() {
            mScanner.scanFile(mFile, mMimeType);
        }

        public void onScanCompleted(String path, Uri uri) {
            if(path.equals(mFile)) {
                mScanner.disconnect();
                mScanner = null;
            }
        }
    }*/

    /* Delay updating IME states (see bug 573800) */
    /*private static final class IMEStateUpdater extends TimerTask
    {
        static private IMEStateUpdater instance;
        private boolean mEnable, mReset;

        static private IMEStateUpdater getInstance() {
            if (instance == null) {
                instance = new IMEStateUpdater();
                mIMETimer.schedule(instance, 200);
            }
            return instance;
        }

        static public synchronized void enableIME() {
            getInstance().mEnable = true;
        }

        static public synchronized void resetIME() {
            getInstance().mReset = true;
        }

        public void run() {
            synchronized(IMEStateUpdater.class) {
                instance = null;
            }

            InputMethodManager imm = (InputMethodManager)
                GeckoApp.surfaceView.getContext().getSystemService(
                    Context.INPUT_METHOD_SERVICE);
            if (imm == null)
                return;

            if (mReset)
                imm.restartInput(GeckoApp.surfaceView);

            if (!mEnable)
                return;

            int state = GeckoApp.surfaceView.mIMEState;
            if (state != GeckoSurfaceView.IME_STATE_DISABLED &&
                state != GeckoSurfaceView.IME_STATE_PLUGIN)
                imm.showSoftInput(GeckoApp.surfaceView, 0);
            else
                imm.hideSoftInputFromWindow(
                    GeckoApp.surfaceView.getWindowToken(), 0);
        }
    }*/

public:
    ~GeckoAppShell();

    class GeckoProcessesVisitor {
        virtual Boolean Callback(
            /* [in] */ Int32 pid) = 0;
    };

    // Get a Handler for the main java thread
    static ECode GetMainHandler(
        /* [out] */ IHandler** ppHandler);

    static ECode GetHandler(
        /* [out] */ IHandler** ppHandler);

    static ECode GetCacheDir(
        /* [out] */ IFile** ppFile);

    static ECode GetFreeSpace(
        /* [out] */ Int32* pFreeSpace);

    static ECode MoveFile(
        /* [in] */ IFile* pInFile,
        /* [in] */ IFile* pOutFile,
        /* [out] */ Boolean* pResult);

    static ECode MoveDir(
        /* [in] */ IFile* pFrom,
        /* [in] */ IFile* pTo,
        /* [out] */ Boolean* pResult);

    static ECode LoadGeckoLibs(
        /* [in] */ const String& apkName);

    static ECode RunGecko(
        /* [in] */ const String& apkPath,
        /* [in] */ const String& args,
        /* [in] */ const String& url);

    static ECode SendEventToGecko(
        /* [in] */ GeckoEvent* event);

    static ECode SendEventToGeckoSync(
        /* [in] */ GeckoEvent* event);

    //The Gecko-side API: API methods that Gecko calls
    static ECode ScheduleRedraw();

    static ECode ScheduleRedraw(
        /* [in] */ Int32 nativeWindow,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    static ECode NotifyIME(
        /* [in] */ Int32 type,
        /* [in] */ Int32 state);

    static ECode NotifyIMEEnabled(
        /* [in] */ Int32 state,
        /* [in] */ const String& typeHint,
        /* [in] */ const String& actionHint,
        /* [in] */ Boolean landscapeFS);

    static ECode NotifyIMEChange(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 newEnd);

    // Block the current thread until the Gecko event loop is caught up
    static ECode GeckoEventSync();

    // Signal the Java thread that it's time to wake up
    static ECode AcknowledgeEventSync();

    static ECode EnableDeviceMotion(
        /* [in] */ Boolean enable);

    static ECode EnableLocation(
        /* [in] */ Boolean enable);

    static ECode MoveTaskToBack();

    static ECode ReturnIMEQueryResult(
        /* [in] */ const String& result,
        /* [in] */ Int32 selectionStart,
        /* [in] */ Int32 selectionLength);

    static ECode OnAppShellReady();

    static ECode OnXreExit();

    static ECode ScheduleRestart();

    // "Installs" an application by creating a shortcut
    static ECode InstallWebApplication(
        /* [in] */ const String& aURI,
        /* [in] */ const String& aTitle,
        /* [in] */ const String& aIconData);

    static ECode GetHandlersForMimeType(
        /* [in] */ const String& aMimeType,
        /* [in] */ const String& aAction,
        /* [out, callee] */ ArrayOf<String>** ppHandlers);

    static ECode GetHandlersForURL(
        /* [in] */ const String& aURL,
        /* [in] */ const String& aAction,
        /* [out, callee] */ ArrayOf<String>** ppHandlers);

    static ECode GetHandlersForIntent(
        /* [in] */ IIntent* pIntent,
        /* [out, callee] */ ArrayOf<String>** ppHandlers);

    static ECode GetIntentForActionString(
        /* [in] */ const String& aAction,
        /* [out] */ IIntent** ppIntent);

    static ECode GetExtensionFromMimeType(
        /* [in] */ const String& aMimeType,
        /* [out] */ String* ext);

    static ECode GetMimeTypeFromExtensions(
        /* [in] */ const String& aFileExt,
        /* [out] */ String* mimeType);

    static ECode OpenUriExternal(
        /* [in] */ const String& aUriSpec,
        /* [in] */ const String& aMimeType,
        /* [in] */ const String& aPackageName,
        /* [in] */ const String& aClassName,
        /* [in] */ const String& aAction,
        /* [in] */ const String& aTitle);

    // On some devices, access to the clipboard service needs to happen
    // on a thread with a looper, so dispatch this to our looper thread
    // Note: the main looper won't work because it may be blocked on the
    // gecko thread, which is most likely this thread
    static ECode GetClipboardText(
        /* [out] */ String* text);

    static ECode SetClipboardText(
        /* [in] */ const String& text);

    static ECode ShowAlertNotification(
        /* [in] */ const String& aImageUrl,
        /* [in] */ const String& aAlertTitle,
        /* [in] */ const String& aAlertText,
        /* [in] */ const String& aAlertCookie,
        /* [in] */ String aAlertName);

    static ECode AlertsProgressListener_OnProgress(
        /* [in] */ String aAlertName,
        /* [in] */ Int32 aProgress,
        /* [in] */ Int32 aProgressMax,
        /* [in] */ const String& aAlertText);

    static ECode AlertsProgressListener_OnCancel(
        /* [in] */ String aAlertName);

    static ECode HandleNotification(
        /* [in] */ const String& aAction,
        /* [in] */ String aAlertName,
        /* [in] */ const String& aAlertCookie);

    static ECode RemoveNotification(
        /* [in] */ Int32 notificationID);

    static ECode GetDpi(
        /* [out] */ Int32* pDpi);

    static ECode SetFullScreen(
        /* [in] */ Boolean fullscreen);

    static ECode ShowFilePicker(
        /* [in] */ const String& aFilters,
        /* [out] */ String* pPicker);

    static ECode PerformHapticFeedback(
        /* [in] */ Boolean aIsLongPress);

    static ECode ShowInputMethodPicker();

    static ECode HideProgressDialog();

    static ECode SetKeepScreenOn(
        /* [in] */ Boolean on);

    static ECode IsNetworkLinkUp(
        /* [out] */ Boolean* isLinkUp);

    static ECode IsNetworkLinkKnown(
        /* [out] */ Boolean* isLinkKnown);

    static ECode SetSelectedLocale(
        /* [in] */ const String& localeCode);

    static ECode GetSystemColors(
        /* [out] */ ArrayOf<Int32>** ppColors);

    static ECode KillAnyZombies();

    static ECode CheckForGeckoProcs(
        /* [out] */ Boolean* pResult);

    static ECode WaitForAnotherGeckoProc();

    static ECode ScanMedia(
        /* [in] */ const String& aFile,
        /* [in] */ const String& aMimeType);

    static ECode GetIconForExtension(
        /* [in] */ String& aExt,
        /* [in] */ Int32 iconSize,
        /* [out] */ ArrayOf<Byte>** ppIcons);

//native function
public:
    // Tell the Gecko event loop that an event is available.
    static Void NotifyGeckoOfEvent(
        /* [in] */ GeckoEvent* event);

    /* The Android-side API: API methods that Android calls */

    // Initialization methods
    static Void NativeInit();

    static Void NativeRun(
        /* [in] */ const String& args);

    // helper methods
    static Void SetSurfaceView(
        /* [in] */ CGeckoSurfaceView* pSurfaceView);

    static Void PutEnv(
        /* [in] */ const String& map);

    static Void OnResume();

    static Void OnLowMemory();

    static Void CallObserver(
        /* [in] */ const String& observerKey,
        /* [in] */ const String& topic,
        /* [in] */ const String& data);

    static Void RemoveObserver(
        /* [in] */ const String& observerKey);

    static Void LoadLibs(
        /* [in] */ const String& apkName,
        /* [in] */ Boolean shouldExtract);

    static Void OnChangeNetworkLinkStatus(
        /* [in] */ const String& status);

    static Void ReportJavaCrash(
        /* [in] */ const String& stack);

private:
    // static members only
    GeckoAppShell();

    static ECode PutLocaleEnv();

    static ECode SendPendingEventsToGecko();

    static ECode EnumerateGeckoProcesses(
        /* [in] */ GeckoProcessesVisitor* pVisiter);

    static ECode GetDrawableForExtension(
        ///* [in] */ IPackageManager* pPm,
        /* [in] */ const String& aExt,
        /* [out] */ IDrawable** ppDrawable);

public:
    static const Int32 kFreeSpaceThreshold = 157286400L; // 150MB
    static Int32 sPidColumn;
    static Int32 sUserColumn;

private:
    static List< AutoPtr<GeckoEvent> > gPendingEvents;

    static Boolean gRestartScheduled;

    //static const ITimer* mIMETimer;
    static HashMap<Int32, IAlertNotification*> mAlertNotifications;

#define NOTIFY_IME_RESETINPUTSTATE      0
#define NOTIFY_IME_SETOPENSTATE         1
#define NOTIFY_IME_CANCELCOMPOSITION    2
#define NOTIFY_IME_FOCUSCHANGE          3

    static const Int32 kLibFreeSpaceBuffer = 20971520L; // 29MB
    static IFile* sCacheFile;
    static Int32 sFreeSpace;
    static IHandler* sHandler;
    static GeckoEvent* mLastDrawEvent;
    //static ICountDownLatch* sGeckoPendingAcks;
    //static ISensor* gAccelerometerSensor;
    //static ISensor* gOrientationSensor;
    //static SynchronousQueue<String> sClipboardQueue;

    static Mutex mGeckoAppShell;
};

#endif //__GECKOAPPSHELL_H__

