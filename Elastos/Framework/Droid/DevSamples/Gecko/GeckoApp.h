
#ifndef __GECKOAPP_H__
#define __GECKOAPP_H__

#include "Activity.h"
#include "CGeckoSurfaceView.h"
#include <Elastos.Utility.Zip.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

#define GeckoApp_ACTION_ALERT_CLICK  \
        "org.mozilla.gecko.ACTION_ALERT_CLICK"

#define GeckoApp_ACTION_ALERT_CLEAR \
         "org.mozilla.gecko.ACTION_ALERT_CLEAR"

#define GeckoApp_FILE_PICKER_REQUEST 1

class GeckoApp : public Activity
{
public:
    enum LaunchState {
        LaunchState_PreLaunch,
        LaunchState_Launching,
        LaunchState_WaitButton,
        LaunchState_Launched,
        LaunchState_GeckoRunning,
        LaunchState_GeckoExiting
     };

public:
    GeckoApp();

    ~GeckoApp();

    static CARAPI_(Boolean) CheckLaunchState(
        /* [in] */ LaunchState checkState);

    static CARAPI_(void) SetLaunchState(
        /* [in] */ LaunchState setState);

    // if mLaunchState is equal to checkState this sets mLaunchState to setState
    // and return true. Otherwise we return false.
    static CARAPI_(Boolean) CheckAndSetLaunchState(
        /* [in] */ LaunchState checkState,
        /* [in] */ LaunchState setState);

    CARAPI ShowErrorDialog(
        /* [in] */ const String& message);

    // Returns true when the intent is going to be handled by gecko launch
    CARAPI_(Boolean) Launch(
        /* [in] */ IIntent* intent);

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnRestart();

    CARAPI OnStop();

    CARAPI OnDestroy();

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI OnLowMemory();

    ECode RemoveFiles();

    ECode AddEnvToIntent(
        /* [in] */ IIntent* intent);

    ECode DoRestart();

    ECode HandleNotification(
        /* [in] */ const String& action,
        /* [in] */ const String& alertName,
        /* [in] */ const String& alertCookie);

    ECode ShowFilePicker(
        /* [in] */ const String& aMimeType,
        /* [out] */ String* filePickerResult);

    virtual ECode GetPackageName(
        /* [out] */ String* packageName) = 0;

    virtual ECode GetContentProcessName(
        /* [out] */ String* processName) = 0;

    ECode UnpackComponents();

protected:
    CARAPI OnNewIntent(
        /* [in] */ IIntent* intent);

    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

private:
    ECode UnpackFile(
        /* [in] */ IZipFile* zip,
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ IZipEntry* fileEntry,
        /* [in] */ const String& name,
        /* [out] */ Boolean* result);

    ECode CheckAndLaunchUpdate();

    ECode ReadUpdateStatus(
        /* [in] */ IFile* statusFile,
        /* [out] */ String* status);

    static void* LaunchEntryRoutine(void *arg);

public:

    static AutoPtr<IFrameLayout> sMainLayout;
    static AutoPtr<CGeckoSurfaceView> sSurfaceView;
    static AutoPtr<GeckoApp> sAppContext;
    static Boolean sFullscreen;
    static AutoPtr<IFile> sGREDir;

    AutoPtr<IThread> mLibLoadThread;
    AutoPtr<IHandler> mMainHandler;
    Boolean mHaveKilledZombies;
    AutoPtr<IButton> mLaunchButton;
    AutoPtr<IIntent> mLaunchIntent;

private:
    static LaunchState sLaunchState;
    static Mutex sSyncLaunchState;

    AutoPtr<IIntentFilter> mConnectivityFilter;
    AutoPtr<IBroadcastReceiver> mConnectivityReceiver;
    //SynchronousQueue<String> mFilePickerResult;
};

#endif //__GECKOAPP_H__

