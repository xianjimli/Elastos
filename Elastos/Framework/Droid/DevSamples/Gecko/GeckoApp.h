
#ifndef __GECKOAPP_H__
#define __GECKOAPP_H__

#include "Activity.h"
#include "CGeckoSurfaceView.h"
#include <Elastos.Utility.Zip.h>

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

    static ECode CheckLaunchState(
        /* [in] */ LaunchState checkState,
        /* [out] */ Boolean* pResult);

    static ECode SetLaunchState(
        /* [in] */ LaunchState setState);

    // if mLaunchState is equal to checkState this sets mLaunchState to setState
    // and return true. Otherwise we return false.
    static ECode CheckAndSetLaunchState(
        /* [in] */ LaunchState checkState,
        /* [in] */ LaunchState setState,
        /* [out] */ Boolean* pResult);

    ECode ShowErrorDialog(
        /* [in] */ const String& message);

    // Returns true when the intent is going to be handled by gecko launch
    ECode Launch(
        /* [in] */ IIntent* pIntent,
        /* [out] */ Boolean* pResult);

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnRestart();

    CARAPI OnStop();

    CARAPI OnDestroy();

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* pNewConfig);

    CARAPI OnLowMemory();

    ECode RemoveFiles();

    ECode AddEnvToIntent(
        /* [in] */ IIntent* pIntent);

    ECode DoRestart();

    ECode HandleNotification(
        /* [in] */ const String& action,
        /* [in] */ const String& alertName,
        /* [in] */ const String& alertCookie);

    ECode ShowFilePicker(
        /* [in] */ const String& aMimeType,
        /* [out] */ String* pFilePickerResult);

    virtual ECode GetPackageName(
        /* [out] */ String* pPackageName) = 0;

    virtual ECode GetContentProcessName(
        /* [out] */ String* pProcessName) = 0;

    ECode UnpackComponents();

protected:
    CARAPI OnNewIntent(
        /* [in] */ IIntent* pIntent);

    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* pData);

private:
    ECode UnpackFile(
        /* [in] */ IZipFile* pZip,
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ IZipEntry* pFileEntry,
        /* [in] */ const String& name,
        /* [out] */ Boolean* pResult);

    ECode CheckAndLaunchUpdate();

    ECode ReadUpdateStatus(
        /* [in] */ IFile* pStatusFile,
        /* [out] */ String* pStatus);

public:

    static IFrameLayout* mainLayout;
    static CGeckoSurfaceView* surfaceView;
    static GeckoApp* mAppContext;
    static Boolean mFullscreen;
    static IFile* sGREDir;

    IThread* mLibLoadThread;
    IHandler* mMainHandler;
    Boolean haveKilledZombies;
    IButton* mLaunchButton;
    IIntent* mLaunchIntent;

private:
    static LaunchState sLaunchState;
    static Mutex sSyncLaunchState;

    IIntentFilter* mConnectivityFilter;
    IBroadcastReceiver* mConnectivityReceiver;
    //SynchronousQueue<String> mFilePickerResult;
};

#endif //__GECKOAPP_H__

