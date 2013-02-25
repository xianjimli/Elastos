
#include "systemui/statusbar/phone/CSystemUIService.h"

const char* CSystemUIService::TAG = "SystemUIService";

// private Class chooseClass(Object o) {
//     if (o instanceof Integer) {
//         final String cl = getString((Integer)o);
//         try {
//             return getClassLoader().loadClass(cl);
//         } catch (ClassNotFoundException ex) {
//             throw new RuntimeException(ex);
//         }
//     } else if (o instanceof Class) {
//         return (Class)o;
//     } else {
//         throw new RuntimeException("Unknown system ui service: " + o);
//     }
// }

//@Override
ECode CSystemUIService::OnCreate()
{
    // // Pick status bar or system bar.
    // AutoPtr<IWindowManager> wm = IWindowManager.Stub.asInterface(
    //         ServiceManager.getService(Context.WINDOW_SERVICE));
    // try {
    //     SERVICES[0] = wm.canStatusBarHide()
    //             ? R.string.config_statusBarComponent
    //             : R.string.config_systemBarComponent;
    // } catch (RemoteException e) {
    //     Slog.w(TAG, "Failing checking whether status bar can hide", e);
    // }

    // final int N = SERVICES.length;
    // mServices = new SystemUI[N];
    // for (int i=0; i<N; i++) {
    //     Class cl = chooseClass(SERVICES[i]);
    //     Slog.d(TAG, "loading: " + cl);
    //     try {
    //         mServices[i] = (SystemUI)cl.newInstance();
    //     } catch (IllegalAccessException ex) {
    //         throw new RuntimeException(ex);
    //     } catch (InstantiationException ex) {
    //         throw new RuntimeException(ex);
    //     }
    //     mServices[i].mContext = this;
    //     Slog.d(TAG, "running: " + mServices[i]);
    //     mServices[i].start();
    // }
    //ASSERT_SUCCEEDED(CStatusBar::New((ISystemUI**)&mStatusBar));
    //mStatusBar->Start();
	return NOERROR;
}

//@Override
ECode CSystemUIService::OnConfigurationChanged(
	/* [in] */ IConfiguration* newConfig)
{
    // for (SystemUI ui: mServices) {
    //     ui.onConfigurationChanged(newConfig);
    // }
    //return mStatusBar->OnConfigurationChanged(newConfig);
    return NOERROR;
}

/**
 * Nobody binds to us.
 */
//@Override
ECode CSystemUIService:: OnBind(
	/* [in] */ IIntent* intent,
	/* [out] */ IBinder** binder)
{
	VALIDATE_NOT_NULL(binder);
	*binder = NULL;
    return NOERROR;
}

// @Override
// protected void dump(FileDescriptor fd, PrintWriter pw, String[] args) {
//     if (checkCallingOrSelfPermission(android.Manifest.permission.DUMP)
//             != PackageManager.PERMISSION_GRANTED) {
//         pw.println("Permission Denial: can't dump StatusBar from from pid="
//                 + Binder.getCallingPid()
//                 + ", uid=" + Binder.getCallingUid());
//         return;
//     }

//     if (args == null || args.length == 0) {
//         for (SystemUI ui: mServices) {
//             pw.println("dumping service: " + ui.getClass().getName());
//             ui.dump(fd, pw, args);
//         }
//     } else {
//         String svc = args[0];
//         for (SystemUI ui: mServices) {
//             String name = ui.getClass().getName();
//             if (name.endsWith(svc)) {
//                 ui.dump(fd, pw, args);
//             }
//         }
//     }
// }
