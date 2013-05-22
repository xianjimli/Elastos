
#include "os/Environment.h"
#include "os/Binder.h"
#include "os/Process.h"
//#include "server/CActivityManagerService.h"
#include "utils/Xml.h"
#include "utils/XmlUtils.h"
#include "text/TextUtils.h"

#include "server/CMountService.h"
#include <Logger.h>
#include <StringBuffer.h>
#include <eltypes.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <utils/String8.h>
#include <cutils/config_utils.h>
#include <cutils/sockets.h>
#include <cutils/log.h>
#include <linux/netlink.h>



#define VOLD_SOCKET "vold"

using namespace Elastos::Utility::Logging;

const char* CMountService::TAG = "MountService";

static int door_sock = -1;
static CMountService* gMountService = NULL;
CMountService::MountServiceHandler* CMountService::mHandler = new CMountService::MountServiceHandler();
AutoPtr<CCapsuleManagerService> CMountService::mPms;
const String CMountService::EXTERNAL_STORAGE_PATH = String("/storage/sdcard0"); //Environment.getExternalStorageDirectory().toString();
const String CMountService::EXTERNAL_STORAGE2_PATH = String("/storage/sdcard0/external"); //Environment.getExternalStorage2Directory().toString();

Boolean CMountService::OnMessage(
    /* [in] */ const String& message)
{
 //   if (DEBUG_EVENTS) {
 //       LOGD("%s: Receive message: %s", TAG, (const char*)message);
 //  }

    String cooked[25];
    String raw = message;

    int i = 0;
    ssize_t idx = raw.IndexOf(' ');
    while(idx != -1 && idx < (ssize_t)raw.GetLength()) {
        cooked[i] = String(raw, idx);
        raw = String(raw, idx + 1);
        idx = raw.IndexOf(' ');
        i++;
        assert(i <= 24);
    }
    cooked[i] = raw;
    int code = atoi(cooked[0]);
    if (code >= ResponseCode::UnsolicitedInformational) {
        if (!OnEvent(code, message, cooked)) {
            //LOGD("%s: Unhandled event (%s)", TAG, (const char*)message);
            return false;
        }
        return true;
    }
    else {
        //mResponseQueue.add(event.getCmdNumber(), event);
        return true;
    }
}

void* threadRoutine(void *arg)
{
    if (arg == NULL) {
        pthread_exit((void*)-1);
    }

    String* message = (String*)arg;
    CMountService::GetDefault()->OnMessage(*message);
    delete message;

    return (void*)0;
}

/*
  * ObbState
  */
CMountService::ObbState::ObbState(
    /* [in] */ String filename,
    /* [in] */ Int32 callerUid,
    /* [in] */ IObbActionListener* token,
    /* [in] */ Int32 nonce)
    :mFilename(filename),
    mCallerUid(callerUid),
    mToken(token),
    mNonce(nonce)
{ }
AutoPtr<IBinder> CMountService::ObbState::GetBinder(){}
void CMountService::ObbState::Link(){}
void CMountService::ObbState::Unlink() {}
String CMountService::ObbState::ToString() {}
ECode CMountService::ObbState::BinderDied()
{
//    ObbAction* action = new UnmountObbAction(this, true);
//    mObbActionHandler.sendMessage(mObbActionHandler.obtainMessage(OBB_RUN_ACTION, action));
    return NOERROR;
}

/*
  * ObbAction
  */
CMountService::ObbAction::ObbAction(
    /* [in] */ ObbState* obbState)
{
//    mObbState = obbState;
}
void CMountService::ObbAction::HandleExecute() {}
void CMountService::ObbAction::HandleError() {}
void CMountService::ObbAction::SendNewStatusOrIgnore(
    /* [in] */ Int32 status)
{ }

/*
  * MountObbAction
  */
CMountService::MountObbAction::MountObbAction(
    /* [in] */ ObbState* obbState,
    /* [in] */ String key)
    :ObbAction(obbState)
{
}
void CMountService::MountObbAction::HandleExecute() {}
void CMountService::MountObbAction::HandleError() {}
String CMountService::MountObbAction::ToString() {}

/*
  * UnmountObbAction
  */
CMountService::UnmountObbAction::UnmountObbAction(
    /* [in] */ ObbState* obbState,
    /* [in] */ Boolean force)
    :ObbAction(obbState)
{}
void CMountService::UnmountObbAction::HandleExecute(){}
void CMountService::UnmountObbAction::HandleError(){}
String CMountService::UnmountObbAction::ToString(){}

/*
  * UnmountCallBack
  */
CMountService::UnmountCallBack::UnmountCallBack(
    /* [in] */ String path,
    /* [in] */ Boolean force,
    /* [in] */ Boolean removeEncryption)
{
    mRetries = 0;
    mPath = path;
    mForce = force;
    mRemoveEncryption = removeEncryption;
}
void CMountService::UnmountCallBack::handleFinished()
{
    if (DEBUG_UNMOUNT) LOGD("%s:Unmounting %s", TAG, (const char*)mPath);
    CMountService::GetDefault()->DoUnmountVolume(mPath, TRUE, mRemoveEncryption);
}

/*
  * UmsEnableCallBack
  */
CMountService::UmsEnableCallBack::UmsEnableCallBack(
    /* [in] */ String path,
    /* [in] */ String method,
    /* [in] */ Boolean force)
    :UnmountCallBack(path, force, FALSE)
{
   mMethod = method;
}
void CMountService::UmsEnableCallBack::handleFinished()
{
    UnmountCallBack::handleFinished();
    CMountService::GetDefault()->DoShareUnshareVolume(mPath, mMethod, TRUE);
}

/*
  * ShutdownCallBack
  */
CMountService::ShutdownCallBack::ShutdownCallBack(
    /* [in] */ String path)
    :UnmountCallBack(path, TRUE, FALSE)
{
}
void CMountService::ShutdownCallBack::handleFinished()
{
    CMountService::GetDefault()->DoUnmountVolume(mPath, TRUE, mRemoveEncryption);
}

/*
  * MountServiceHandler
  */
CMountService::MountServiceHandler::MountServiceHandler()
{
    mForceUnmounts = new List<UnmountCallBack* >();
    mUpdatingStatus = FALSE;
}
void CMountService::MountServiceHandler::sendMessage(
    /* [in] */ MsgData* msg)
{
    void (STDCALL CMountService::MountServiceHandler::*pHandlerFunc)(MsgData* msg);
    pHandlerFunc = &CMountService::MountServiceHandler::handleMessage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)msg);
    CMountService::GetDefault()->SendMessage(*(Handle32*)&pHandlerFunc, params);
}
void CMountService::MountServiceHandler::sendMessageDelayed(
    /* [in] */ MsgData* msg,
    /* [in] */ Millisecond64 delay)
{
    void (STDCALL CMountService::MountServiceHandler::*pHandlerFunc)(MsgData* msg);
    pHandlerFunc = &CMountService::MountServiceHandler::handleMessage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)msg);
    CMountService::GetDefault()->SendMessageAtTime(*(Handle32*)&pHandlerFunc, params, delay);
}
void CMountService::MountServiceHandler::obtainMessage() {}
void CMountService::MountServiceHandler::handleMessage(
    /* [in] */ MsgData* msg)
{
    switch (msg->msg) {
        case H_UNMOUNT_PM_UPDATE: {
            if (DEBUG_UNMOUNT) LOGD("%s:H_UNMOUNT_PM_UPDATE", TAG);
            mForceUnmounts->PushBack(msg->ucb);
            if (DEBUG_UNMOUNT) LOGD("%s:registered = %d", TAG, mUpdatingStatus);
            // Register only if needed.
            if (!mUpdatingStatus) {
                if (DEBUG_UNMOUNT) LOGD("%s:Updating external media status on PackageManager", TAG);
                mUpdatingStatus = TRUE;
                mPms->UpdateExternalMediaStatus(FALSE, TRUE);
            }
            break;
        }
        case H_UNMOUNT_PM_DONE: {
            if (DEBUG_UNMOUNT) LOGD("%s:H_UNMOUNT_PM_DONE", TAG);
            if (DEBUG_UNMOUNT) LOGD("%s:Updated status. Processing requests", TAG);
            mUpdatingStatus = FALSE;
            int size = mForceUnmounts->GetSize();
            int sizeArr[size];
            int sizeArrN = 0;
            // Kill processes holding references first
//            ActivityManagerService ams = (ActivityManagerService)
//            ServiceManager.getService("activity");
//            AutoPtr<CActivityManagerService> ams;
            for (int i = 0; i < size; i++) {
                UnmountCallBack* ucb = (UnmountCallBack*) mForceUnmounts->GetFront();
                String path = ucb->mPath;
                Boolean done = FALSE;
                if (!ucb->mForce) {
                    done = TRUE;
                } else {
//                    int pids[] = getStorageUsers(path);
//                    if (pids == null || pids.length == 0) {
//                        done = true;
//                    } else {
                        // Eliminate system process here?
//                        ams->killPids(pids, "unmount media", true);
                        // Confirm if file references have been freed.
//                        pids = getStorageUsers(path);
//                        if (pids == null || pids.length == 0) {
//                            done = true;
//                        }
//                    }
                }
                if (!done && (ucb->mRetries < MAX_UNMOUNT_RETRIES)) {
                    // Retry again
                    LOGI("%s:Retrying to kill storage users again", TAG);
                    MsgData* msgSend = new MsgData();
                    msgSend->msg = H_UNMOUNT_PM_DONE;
                    msg->ucb->mRetries++;
                    msgSend->ucb = ucb;
                    mHandler->sendMessageDelayed(msgSend, RETRY_UNMOUNT_DELAY);
                } else {
                    if (ucb->mRetries >= MAX_UNMOUNT_RETRIES) {
                            LOGI("%s:Failed to unmount media inspite of %d retries. Forcibly killing processes now", TAG, MAX_UNMOUNT_RETRIES);
                    }
                    sizeArr[sizeArrN++] = i;
                    MsgData* msgSend = new MsgData();
                    msgSend->msg = H_UNMOUNT_MS;
                    msgSend->ucb = ucb;
                    mHandler->sendMessage(msgSend);
                }
            }
            // Remove already processed elements from list.
            for (int i = (sizeArrN-1); i >= 0; i--) {
//                mForceUnmounts.remove(sizeArr[i]);
            }
            break;
        }
        case H_UNMOUNT_MS : {
            if (DEBUG_UNMOUNT) LOGD("%s:H_UNMOUNT_MS", TAG);
            msg->ucb->handleFinished();
            break;
        }
    }
}

CMountService::CMountService()
    :mExternalStoragePath("/storage/sdcard0"),
    mUmsEnabling(FALSE),
    mUmsAvailable(FALSE),
    mBooted(FALSE),
    mSendUmsConnectedOnBoot(FALSE),
    mEmulateExternalStorage(FALSE),
    mVolumeStates(6)
{
    AutoPtr<IApartmentHelper> helper;
    assert(SUCCEEDED(CApartmentHelper::AcquireSingleton((IApartmentHelper**)&helper)));
    assert(SUCCEEDED(helper->GetMainApartment((IApartment**)&mApartment))
            && (mApartment != NULL));

    mHandler = new MountServiceHandler();

    mVolumeStates[mExternalStoragePath] = String("removed");
    HashMap<String, String>::Iterator iter = mVolumeStates.Find(mExternalStoragePath);
    if (iter != mVolumeStates.End())
	 Logger::E(TAG, StringBuffer("MS state(")+ iter->mSecond+")");

    AutoPtr<IServiceManager> serviceManager;
//    Elastos::GetIServiceManager((CCapsuleManagerService**)&serviceManager);
}
CMountService::~CMountService() { }

CMountService* CMountService::GetDefault()
{
    if (gMountService != NULL) {
        return gMountService;
    }

    gMountService = new CMountService();
    return gMountService;
}

Boolean CMountService::OnEvent(
        /* [in] */ Int32 code,
        /* [in] */ const String& raw,
        /* [in] */ String* cooked)
{
    String path = cooked[3];

    if (DEBUG_EVENTS) {
        StringBuffer builder("OnEvent::");
        builder += String(" raw= ");
        builder += raw;
        if (cooked != NULL) {
            builder += String(" cooked = ");
            for (int i = 0; i < 11; i++) {
                if (cooked[i].GetSize() != 0) {
                    builder += cooked[i];
                }
            }
        }
        LOGD("%s: %s", TAG, (const char*)builder);
    }

    if (code == VoldResponseCode::VolumeStateChange) {
        /*
          * One of the volumes we're managing has changed state.
          * Format: "NNN Volume <label> <path> state changed
          * from <old_#> (<old_str>) to <new_#> (<new_str>)"
          */
        LOGD("%s:VolumeStateChange !!!", TAG);
        NotifyVolumeStateChange(
                 cooked[2], cooked[3], atoi(cooked[7]), atoi(cooked[10]));

        if ( VolumeState::Mounted == atoi(cooked[10]) )
        {
            int devtype = atoi(cooked[12]);
            int devnode = atoi(cooked[14]);

            Mutex::Autolock lock(mStorageLock);
                mStorage->PushBack(cooked[3]);
                mStorageDevNode[cooked[3]] = devnode;
                mStorageDevType[cooked[3]] = devtype;
            Mutex::Autolock unlock(mStorageLock);
        }
    } else if ((code == VoldResponseCode::VolumeDiskInserted) ||
        (code == VoldResponseCode::VolumeDiskRemoved) ||
        (code == VoldResponseCode::VolumeBadRemoval)) {
        // FMT: NNN Volume <label> <mountpoint> disk inserted (<major>:<minor>)
        // FMT: NNN Volume <label> <mountpoint> disk removed (<major>:<minor>)
        // FMT: NNN Volume <label> <mountpoint> bad removal (<major>:<minor>)
        String action;
        const String label = cooked[2];
        const String path = cooked[3];
        int major = -1;
        int minor = -1;
//        String devComp = cooked[6].substring(1, cooked[6]->GetLength() -1);
//        String[] devTok = devComp.split(":");
//        major = Integer.parseInt(devTok[0]);
//        minor = Integer.parseInt(devTok[1]);
        if (code == VoldResponseCode::VolumeDiskInserted) {
            Boolean enable;
            if (IsUsbMassStorageEnabled(&enable)) {
                LOGD("%s:Another volume is already shared, sharing %s", TAG, (const char*)path);
                DoShareUnshareVolume(path, String("ums"), true);
            } else {
                int rc;
                if ((rc = DoMountVolume(path)) != StorageResultCode::OperationSucceeded) {
                    LOGW("%s:Insertion mount failed (%d)", TAG, rc);
                }
            }
        } else if (code == VoldResponseCode::VolumeDiskRemoved) {
           /*
             * This event gets trumped if we're already in BAD_REMOVAL state
             */
            String vs;
            GetVolumeState(path, &vs);
            if (vs.Equals(Environment::MEDIA_BAD_REMOVAL)) {
                return true;
            }
            /* Send the media unmounted event first */
            if (DEBUG_EVENTS) LOGI("%s:Sending unmounted event first", TAG);
            UpdatePublicVolumeState(path, Environment::MEDIA_UNMOUNTED);
            SendStorageIntent(Environment::MEDIA_UNMOUNTED, path);

            if (DEBUG_EVENTS) LOGI("%s:Sending media removed", TAG);
            UpdatePublicVolumeState(path, Environment::MEDIA_REMOVED);
            Mutex::Autolock lock(mStorageLock);
                mStorage->Remove(path);
                mStorageDevNode.Erase(path);
                mStorageDevType.Erase(path);
            Mutex::Autolock unlock(mStorageLock);
	    action = String(Intent_ACTION_MEDIA_REMOVED);
        } else if (code == VoldResponseCode::VolumeBadRemoval) {
            if (DEBUG_EVENTS) LOGI("%s:Sending unmounted event first", TAG);
            /* Send the media unmounted event first */
            UpdatePublicVolumeState(path, Environment::MEDIA_UNMOUNTED);
            action = String(Intent_ACTION_MEDIA_UNMOUNTED);

            if (DEBUG_EVENTS) LOGI("%s:Sending media bad removal", TAG);
            UpdatePublicVolumeState(path, Environment::MEDIA_BAD_REMOVAL);
            action = String(Intent_ACTION_MEDIA_BAD_REMOVAL);
        } else {
            LOGE("%s:Unknown code {%d}", TAG, code);
        }

        if (action != NULL) {
            SendStorageIntent(action, path);
        }
    }else {
        return FALSE;
    }
    return TRUE;
}

void CMountService::NotifyVolumeStateChange(
        /* [in] */ const String& label,
        /* [in] */ const String& path,
        /* [in] */ Int32 oldState,
        /* [in] */ Int32 newState)
{
    String vs;
    GetVolumeState(path, &vs);
    if (DEBUG_EVENTS) LOGD("%s:notifyVolumeStateChanged::%s", TAG, (const char*)vs);
    String action;

    if (oldState == VolumeState::Shared && newState != oldState) {
        if (DEBUG_EVENTS) LOGD("MountService:Sending ACTION_MEDIA_UNSHARED intent");
        SendStorageIntent(String(Intent_ACTION_MEDIA_UNSHARED),  path);
    }

    if (newState == VolumeState::Init) {
    } else if (newState == VolumeState::NoMedia) {
         // NoMedia is handled via Disk Remove events
    } else if (newState == VolumeState::Idle) {
         /*
          * Don't notify if we're in BAD_REMOVAL, NOFS, UNMOUNTABLE, or
          * if we're in the process of enabling UMS
          */
         if (!vs.Equals(Environment::MEDIA_BAD_REMOVAL) &&
                !vs.Equals(Environment::MEDIA_NOFS) &&
                !vs.Equals(Environment::MEDIA_UNMOUNTABLE) && !GetUmsEnabling()) {
            if (DEBUG_EVENTS) LOGI("%s:updating volume state for media bad removal nofs and unmountable", TAG);
            UpdatePublicVolumeState(path, Environment::MEDIA_UNMOUNTED);
            action = String(Intent_ACTION_MEDIA_UNMOUNTED);
         }
    } else if (newState == VolumeState::Pending) {
    } else if (newState == VolumeState::Checking) {
        if (DEBUG_EVENTS) LOGD("%s:updating volume state checking", TAG);
        UpdatePublicVolumeState(path, Environment::MEDIA_CHECKING);
        action = String(Intent_ACTION_MEDIA_CHECKING);
    } else if (newState == VolumeState::Mounted) {
        if (DEBUG_EVENTS) LOGD("%s:updating volume state mounted", TAG);
        UpdatePublicVolumeState(path, Environment::MEDIA_MOUNTED);
        action = String(Intent_ACTION_MEDIA_MOUNTED);
    } else if (newState == VolumeState::Unmounting) {
        action = String(Intent_ACTION_MEDIA_EJECT);
    } else if (newState == VolumeState::Formatting) {
    } else if (newState == VolumeState::Shared) {
        if (DEBUG_EVENTS) LOGD("%s:Updating volume state media mounted", TAG);
    //     /* Send the media unmounted event first */
        UpdatePublicVolumeState(path, Environment::MEDIA_UNMOUNTED);
        SendStorageIntent(String(Intent_ACTION_MEDIA_UNMOUNTED), path);

        if (DEBUG_EVENTS) LOGD("%s:Updating media shared", TAG);
        UpdatePublicVolumeState(path, Environment::MEDIA_SHARED);
        action = String(Intent_ACTION_MEDIA_SHARED);
         if (DEBUG_EVENTS) LOGD("%s:Sending ACTION_MEDIA_SHARED intent", TAG);

         if (GetUmsEnabling()) {
            Mutex::Autolock lock(mUmsEnablingVolumesLock);
                int index;
//                if ((index = mUmsEnablingVolumes.indexOf(path)) >= 0) {
//                    mUmsEnablingVolumes.Remove(index);
//                    if (DEBUG_EVENTS) LOGD("%s:mUmsEnablingVolumes.remove %s", TAG, (const char*)path);
//                }
                if (mUmsEnablingVolumes->IsEmpty()) {
                    if (DEBUG_EVENTS) LOGD("%s:mUmsEnablingVolumes.isEmpty, notifying", TAG);
                    SetUmsEnabling(false);
//                    mUmsEnablingVolumes.notifyAll();
                }
            Mutex::Autolock unlock(mUmsEnablingVolumesLock);
        }
    } else if (newState == VolumeState::SharedMnt) {
        LOGE("%s: Live shared mounts not supported yet!", TAG);
        return;
    } else {
         LOGE("MountService:Unhandled VolumeState {%d}", newState);
    }
    if (action != NULL) {
        SendStorageIntent(action, path);
    }
}

Int32 CMountService::DoMountVolume(
        /* [in] */ const String& path)
{
    int rc = StorageResultCode::OperationSucceeded;

    if (DEBUG_EVENTS) LOGD("%s:DoMountVolume: Mouting %s", TAG, (const char*)path);
    DoCommand(String(StringBuffer("0 volume mount ")+path+" ums'\0'"));
//} catch (NativeDaemonConnectorException e) {
       /*
         * Mount failed for some reason
         */
//        String action = null;
//        int code = e.getCode();
//        if (code == VoldResponseCode.OpFailedNoMedia) {
            /*
             * Attempt to mount but no media inserted
             */
//            rc = StorageResultCode.OperationFailedNoMedia;
//        } else if (code == VoldResponseCode.OpFailedMediaBlank) {
//            if (DEBUG_EVENTS) Slog.i(TAG, " updating volume state :: media nofs");
            /*
             * Media is blank or does not contain a supported filesystem
             */
//            updatePublicVolumeState(path, Environment.MEDIA_NOFS);
//            action = Intent.ACTION_MEDIA_NOFS;
//            rc = StorageResultCode.OperationFailedMediaBlank;
//        } else if (code == VoldResponseCode.OpFailedMediaCorrupt) {
//            if (DEBUG_EVENTS) Slog.i(TAG, "updating volume state media corrupt");
            /*
             * Volume consistency check failed
             */
//            updatePublicVolumeState(path, Environment.MEDIA_UNMOUNTABLE);
//            action = Intent.ACTION_MEDIA_UNMOUNTABLE;
//            rc = StorageResultCode.OperationFailedMediaCorrupt;
//        } else {
//            rc = StorageResultCode.OperationFailedInternalError;
//        }

        /*
         * Send broadcast intent (if required for the failure)
         */
//        if (action != null) {
//            SendStorageIntent(action, path);
//        }
//}
    return rc;
}

void CMountService::DoCommand(
        /* [in] */ const String& cmd)
{
     //LOGD("MountService: Send Command %s to vold.", (const char*)cmd);
    send(door_sock, cmd, cmd.GetLength() + 1, 0);
}

void CMountService::DoShareUnshareVolume(
        /* [in] */ const String& path,
        /* [in] */ const String& method,
        /* [in] */ Boolean enable)
{
    // TODO: Add support for multiple share methods
    if (!method.Equals("ums")) {
        LOGE("%s: Method %s not supported", TAG, (const char*)method);
        //return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException(String.format("Method %s not supported", method));
    }
    DoCommand(String(StringBuffer("0 volume ")+(enable ? "" : "un")+"share"+path+" "+method));
//    } catch (NativeDaemonConnectorException e) {
//            Slog.e(TAG, "Failed to share/unshare", e);
//            if (GetUmsEnabling()) {
//                Mutex::Autolock lock(mUmsEnablingVolumesLock);
//                    int index;
//                    if ((index = mUmsEnablingVolumes.indexOf(path)) >= 0) {
//                        mUmsEnablingVolumes.remove(index);
//                        if (LOCAL_LOGD) Slog.d(TAG, "mUmsEnablingVolumes.remove" + path);
//                    }
//                    if (mUmsEnablingVolumes.isEmpty()) {
//                        if (LOCAL_LOGD) Slog.d(TAG, "mUmsEnablingVolumes.isEmpty, notifying");
//                        setUmsEnabling(false);
//                        mUmsEnablingVolumes.notifyAll();
//                    }
//                Mutex::Autolock unlock(mUmsEnablingVolumesLock);
//            }
//    }
}

void CMountService::UpdatePublicVolumeState(
        /* [in] */ const String& path,
        /* [in] */ const String& state)
{
    String oldState;
    Mutex::Autolock lock(mVolumeStatesLock);
        HashMap<String, String>::Iterator iter = mVolumeStates.Find(path);
        if (iter != mVolumeStates.End()) {
        Logger::D(TAG, StringBuffer("UpdatePublicVolumeState state(")+iter->mSecond+")");
        oldState = iter->mSecond;
            mVolumeStates[path] = state;
        } else {
        Logger::D(TAG, StringBuffer("UpdatePublicVolumeState(")+path+"): Unknown volume");
            mVolumeStates[path] = Environment::MEDIA_REMOVED;
        }
//    Mutex::Autolock unlock(mVolumeStatesLock);

    if (state.Equals(oldState)) {
        Logger::W(TAG, StringBuffer("Duplicate state transition (")+oldState+")->("+state+") for"+path);
        return;
    }

    Logger::D(TAG, StringBuffer("volume state changed for ")+path+" ("+oldState+" -> "+state+" )");

    if (path.Equals(mExternalStoragePath)) {
        // Update state on PackageManager, but only of real events
        if (!mEmulateExternalStorage) {
            if (state.Equals(Environment::MEDIA_UNMOUNTED)) {
                mPms->UpdateExternalMediaStatus(FALSE, FALSE);

                /*
                 * Some OBBs might have been unmounted when this volume was
                 * unmounted, so send a message to the handler to let it know to
                 * remove those from the list of mounted OBBS.
                 */
                //mObbActionHandler.sendMessage(mObbActionHandler.obtainMessage(OBB_FLUSH_MOUNT_STATE, path));
            } else if (Environment::MEDIA_MOUNTED.Equals(state)) {
                mPms->UpdateExternalMediaStatus(TRUE, FALSE);
            }
        }
    }
//    Mutex::Autolock lock(mListenersLock);
//        for (int i = mListeners.size() -1; i >= 0; i--) {
//            MountServiceBinderListener bl = mListeners.get(i);
//            try {
//                bl.mListener.onStorageStateChanged(path, oldState, state);
//            } catch (RemoteException rex) {
//                Slog.e(TAG, "Listener dead");
//                mListeners.remove(i);
//            } catch (Exception ex) {
//                Slog.e(TAG, "Listener failed", ex);
//            }
//        }
//    Mutex::Autolock unlock(mListenersLock);
}

Int32 CMountService::DoUnmountVolume(
        /* [in] */ const String& path,
        /* [in] */ Boolean force,
        /* [in] */ Boolean removeEncryption)
{
    String vs;
    GetVolumeState(path, &vs);
    if (!vs.Equals(Environment::MEDIA_MOUNTED))
    	return VoldResponseCode::OpFailedVolNotMounted;

    /*
     * Force a GC to make sure AssetManagers in other threads of the
     * system_server are cleaned up. We have to do this since AssetManager
     * instances are kept as a WeakReference and it's possible we have files
     * open on the external storage.
     */
    //Runtime.getRuntime().gc();

    // Redundant probably. But no harm in updating state again.
    mPms->UpdateExternalMediaStatus(FALSE, FALSE);

    StringBuffer cmd("0 volume unmount ");
    cmd += path;
    if (removeEncryption)
    	cmd += " force_and_revert";
    else if (force)
    	cmd += " force";
    else
    	cmd += " ";
    LOGD("%s:DoUnmountVolume %s", TAG, (const char*)cmd);
    DoCommand(String(cmd));
    if (path.Equals(EXTERNAL_STORAGE_PATH)) {
//    // We unmounted the volume. None of the asec containers are available now.
        Mutex::Autolock lock(mAsecMountSetLock);
//            if (DEBUG_EVENTS) {
//                LOGD("%s:mAsecMountSet.clear, size=%d", TAG, mAsecMountSet.GetSize());
//                Iterator it = mAsecMountSet.iterator();
//                while (it.hasNext())
//                    Slog.d(TAG, "  " + it.next());
//            }
//            mPrevAsecMountSet = mAsecMountSet.toArray(new String[0]);
//            mAsecMountSet.Clear();
        Mutex::Autolock unlock(mAsecMountSetLock);
    }
    return StorageResultCode::OperationSucceeded;
}

Int32 CMountService::DoFormatVolume(
        /* [in] */ const String& path)
{
    String cmd = String(StringBuffer("volume format ")+path);
    DoCommand(cmd);
    return StorageResultCode::OperationSucceeded;
}

Boolean CMountService::DoGetVolumeShared(
        /* [in] */ const String& path,
        /* [in] */ const String& method)
{
//    final NativeDaemonEvent event;
    String cmd = String(StringBuffer("volume shared ")+path+" "+method);
    DoCommand(cmd);

//    if (event.getCode() == VoldResponseCode.ShareEnabledResult) {
//        return event.getMessage().endsWith("enabled");
//    } else {
//        return false;
//    }
    return FALSE;
}

void CMountService::NotifyShareAvailabilityChange(
        /* [in] */ const Boolean avail)
{
    Mutex::Autolock lock(mListenersLock);
        mUmsAvailable = avail;
//        for (int i = mListeners.size() -1; i >= 0; i--) {
//            MountServiceBinderListener bl = mListeners.get(i);
//            bl.mListener.onUsbMassStorageConnectionChanged(avail);
//        }
//    Mutex::Autolock unlock(mListenersLock);

    if (mBooted == true) {
        SendUmsIntent(avail);
    } else {
        mSendUmsConnectedOnBoot = avail;
    }

        if (avail == false) {
            /*
             * USB mass storage disconnected while enabled
             */
            int timeouts = 0;
            while (GetUmsEnabling()) {
                LOGD("%s:UMS disconnected. Waiting until done sharing before unsharing...", TAG);
//                Mutex::Autolock lock(mUmsEnablingVolumesLock);
                    usleep(1000*1000); //mUmsEnablingVolumes.wait(10000);
                    if (!GetUmsEnabling()) {
                        LOGD("%s:done waiting for all volumes to become shared", TAG);
                        break;
                    } else if (++timeouts > 8) {
                        LOGE("%s:timed out waiting for all volumes to become shared. continuing", TAG);
                        break;
                    }
//                Mutex::Autolock unlock(mUmsEnablingVolumesLock);
            }
//            Mutex::Autolock lock(mUmsEnablingVolumesLock);
                if (LOCAL_LOGD) LOGD("%s:mUmsEnablingVolumes.clear", TAG);
                mUmsEnablingVolumes->Clear();
//            Mutex::Autolock unlock(mUmsEnablingVolumesLock);

            ArrayOf<String>* paths; //String[] paths;
            ArrayOf<String>* states; //String[] states;
            int count;
//            Mutex::Autolock lock(mVolumeStatesLock);
                Set<String> keys;//Set<String> keys = mVolumeStates.keySet();
                HashMap<String, String>::Iterator iter;
                for (iter =  mVolumeStates.Begin(); iter != mVolumeStates.End(); ++iter) {
                    keys.Insert(iter->mFirst);
                }
                count = keys.GetSize(); //count = keys.size();
                paths = ArrayOf<String>::Alloc(count);//paths = keys.toArray(new String[count]);
                states = ArrayOf<String>::Alloc(count);//states = new String[count];
                for (int i = 0; i < count; i++) {
                    (*states)[i] = mVolumeStates[ (*paths)[i] ];//mVolumeStates.get(paths[i]);
                }
//            Mutex::Autolock unlock(mVolumeStatesLock);

            for (int i = 0; i < count; i++) {
                String path = (*paths)[i];
                String state = (*states)[i];
                if(!path.Equals(EXTERNAL_STORAGE_PATH))//unshare EXTERNAL_STORAGE_PATH first
                    continue;
                if (state.Equals(Environment::MEDIA_SHARED)) {
                    int rc;
                    LOGD("%s:Disabling UMS after cable disconnect", TAG);
                    DoShareUnshareVolume(path, String("ums"), FALSE);
                    if ((rc = DoMountVolume(path)) != StorageResultCode::OperationSucceeded) {
                        LOGE("%s:Failed to remount {%s} on UMS enabled-disconnect (%d)",TAG, (const char*)path, rc);
                    }
                }
            }
            for (int i = 0; i < count; i++) {
                String path = (*paths)[i];
                String state = (*states)[i];
                if(path.Equals(EXTERNAL_STORAGE_PATH))//unshare others
                    continue;
                if (state.Equals(Environment::MEDIA_SHARED)) {
                    int rc;
                    LOGD("%s:Disabling UMS after cable disconnect", TAG);
                    DoShareUnshareVolume(path, String("ums"), FALSE);
                    if ((rc = DoMountVolume(path)) != StorageResultCode::OperationSucceeded) {
                        LOGE("%s:Failed to remount {%s} on UMS enabled-disconnect (%d)", TAG, (const char*)path, rc);
                    }
                }
            }
        }
}

void CMountService::SendStorageIntent(
        /* [in] */ const String& action,
        /* [in] */ const String& path)
{
    AutoPtr<IIntent> in = NULL;
    AutoPtr<IUri> uri;
    Uri::Parse(String(StringBuffer("file://") + path), (IUri **) &uri);
    CIntent::New(action, (IUri*)uri, (IIntent**)&in);
//    // add StorageVolume extra
//    intent.putExtra(StorageVolume.EXTRA_STORAGE_VOLUME, mVolumeMap.get(path));
//    if (action.Equals(String(Intent_ACTION_MEDIA_MOUNTED)))
//        in.PutExtra("system-send", true);
//    LOGD("%s:SendStorageIntent ", TAG);// + intent);
    mContext->SendBroadcast(in);
}

void CMountService::SendUmsIntent(
        /* [in] */ Boolean c)
{
    AutoPtr<IIntent> in = NULL;
    CIntent::New(String(c ? Intent_ACTION_UMS_CONNECTED : Intent_ACTION_UMS_DISCONNECTED), (IIntent**)&in);
    mContext->SendBroadcast(in);
}

Boolean CMountService::GetUmsEnabling() {
    Mutex::Autolock lock(mListenersLock);
    return mUmsEnabling;
}
/*
void CMountService::WaitForLatch(CountDownLatch latch) {
    if (latch == null) {
        return;
    }

    for (;;) {
        try {
            if (latch.await(5000, TimeUnit.MILLISECONDS)) {
                return;
            } else {
                Slog.w(TAG, "Thread " + Thread.currentThread().getName()
                        + " still waiting for MountService ready...");
            }
        } catch (InterruptedException e) {
            Slog.w(TAG, "Interrupt while waiting for MountService to be ready.");
        }
    }
}
*/
void CMountService::WaitForReady()
{
//        WaitForLatch(mConnectedSignal);
}

void CMountService::ValidatePermission(
    /* [in] */ const String& perm)
{
    Int32 val;
    mContext->CheckCallingOrSelfPermission(perm, &val);
    if (val != CapsuleManager_PERMISSION_GRANTED) {
        LOGE("%s: Requires %s permission", TAG, (const char*)perm);
        //return E_SECURITY_EXCEPTION;
        //throw new SecurityException(String.format("Requires %s permission", perm));
    }
}

Boolean CMountService::WarnOnNotMounted()
{
    if (!(Environment::GetExternalStorageState().Equals(Environment::MEDIA_MOUNTED))) {
        LOGW("%s:getSecureContainerList() called when storage not mounted", TAG);
        return true;
    }
    return false;
}

// Storage list XML tags
static const String TAG_STORAGE_LIST = String("StorageList");
static const String TAG_STORAGE = String("storage");
static Int32 R_Styleable_Storage[] = {
    0x01010001,
    0x01010002
};
void CMountService::ReadStorageList() {
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);

    int id = 1; // com.android.internal.R.xml.storage_list
    AutoPtr<IXmlResourceParser> parser;
    resources->GetXml(id, (IXmlResourceParser**)&parser);
    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

//    try{
    XmlUtils::BeginDocument(parser, TAG_STORAGE_LIST);
    while (true) {
        XmlUtils::NextElement(parser);

        String element;
        parser->GetName(&element);
        if (element == NULL) break;

        if (TAG_STORAGE.Equals(element)) {
            AutoPtr<ITypedArray> a;
            resources->ObtainAttributes(
                attrs, ArrayOf<Int32>(R_Styleable_Storage, 2)/*com.android.internal.R.styleable.Storage*/,
                (ITypedArray**)&a);

            AutoPtr<ICharSequence> path;
            a->GetText(0/*com.android.internal.R.styleable.Storage_mountPoint*/, (ICharSequence**)&path);
            Int32 descriptionId;
            a->GetResourceId(1/*com.android.internal.R.styleable.Storage_storageDescription*/, -1, &descriptionId);
            AutoPtr<ICharSequence> description;
            a->GetText(2/*com.android.internal.R.styleable.Storage_storageDescription*/, (ICharSequence**)&description);
            Boolean primary;
            a->GetBoolean(3/*com.android.internal.R.styleable.Storage_primary*/, FALSE, &primary);
            Boolean removable;
            a->GetBoolean(4/*com.android.internal.R.styleable.Storage_removable*/, FALSE, &removable);
            Boolean emulated;
            a->GetBoolean(5/*com.android.internal.R.styleable.Storage_emulated*/, FALSE, &emulated);
            Int32 mtpReserve;
            a->GetInt32(6/*com.android.internal.R.styleable.Storage_mtpReserve*/, 0, &mtpReserve);
            Boolean allowMassStorage;
            a->GetBoolean(7/*com.android.internal.R.styleable.Storage_allowMassStorage*/, FALSE, &allowMassStorage);
            // resource parser does not support longs, so XML value is in megabytes
            Int32 maxFileSize32;
            a->GetInt32(8/*com.android.internal.R.styleable.Storage_maxFileSize*/, 0, &maxFileSize32);
            Int64 maxFileSize = maxFileSize32*1024L*1024L;
//            Slog.d(TAG, "got storage path: " + path + " description: " + description +
//                    " primary: " + primary + " removable: " + removable +
//                    " emulated: " + emulated +  " mtpReserve: " + mtpReserve +
//                    " allowMassStorage: " + allowMassStorage +
//                    " maxFileSize: " + maxFileSize);
            if (path == NULL || description == NULL) {
                LOGE("%s:path or description is null in readStorageList", TAG);
            } else {
                String* pathString;
                path->ToString(pathString);
//                AutoPtr<IStorageVolume>* volume = new IStorageVolume(pathString,
//                        descriptionId, removable, emulated,
//                        mtpReserve, allowMassStorage, maxFileSize);
//                if (primary) {
//                    if (mPrimaryVolume == NULL) {
//                        mPrimaryVolume = volume;
//                    } else {
//                        LOGE("%s:multiple primary volumes in storage list", TAG);
//                    }
//                }
//                if (mPrimaryVolume == volume) {
                    // primay volume must be first
//                    mVolumes.add(0, volume);
//                } else {
//                    mVolumes.add(volume);
//                }
//                mVolumeMap.put(pathString, volume);
            }
//            a.recycle();
        }
    } //while(true)
//        } catch (XmlPullParserException e) {
//            throw new RuntimeException(e);
//        } catch (IOException e) {
//            throw new RuntimeException(e);
//        } finally {
//            // compute storage ID for each volume
//            int length = mVolumes.size();
//            for (int i = 0; i < length; i++) {
//                mVolumes.get(i).setStorageId(i);
//            }
//            parser.close();
//        }

}
void CMountService::SetUmsEnabling(Boolean ena) {
    Mutex::Autolock lock(mListenersLock);
        mUmsEnabling = ena;
    Mutex::Autolock unlock(mListenersLock);
}

Boolean CMountService::IsUsbMassStorageAllEnabled() {
    WaitForReady();
    Mutex::Autolock lock(mSharedVolumesLock);
//        for (String path : mSharedVolumes) {
//            if (!DoGetVolumeShared(path, String("ums"))) {
                return false;
//            }
//        }
    Mutex::Autolock unlock(mSharedVolumesLock);
    return true;
}

/**
 * @return label of the volume at the specified mount point
 */
String CMountService::GetVolumeLabel(
    /* [in] */ const String& mountPoint) {
    String vlabel;
    HashMap<String, String>::Iterator iter = mVolumeLabel.Find(mountPoint);
    if (iter != mVolumeLabel.End()) {
        vlabel = iter->mSecond;
    } else {
        vlabel = String("Unknown");
    }
    return vlabel;
}

String CMountService::GetVolumeFSLabel(
    /* [in] */ const String& mountPoint) {
//    String[] label = mConnector.doListCommand(
//        "volume label " + mountPoint, VoldResponseCode.VolumeLabelResult);
//    if (LOCAL_LOGD)
//        Slog.d(TAG, "label.length=" + label.length +
//            " label[0] = " + (label.length != 0 ? label[0] : ""));
//    if (label.length != 1)
        return String(NULL);
//    return label[0];
}

List<String>* CMountService::GetAllVolume() {
    List<String>* storagelist;

    Mutex::Autolock lock(mStorageLock);
        storagelist = mStorage;
    Mutex::Autolock unlock(mStorageLock);
    return storagelist;
}

Int32 CMountService::GetVolumedevtype(
    /* [in] */ const String& mountPoint) {
    Int32 devtype;

    Mutex::Autolock lock(mStorageLock);
        HashMap<String, Int32>::Iterator iter = mStorageDevType.Find(mountPoint);
        if (iter != mStorageDevType.End()) {
            devtype = iter->mSecond;
        } else {
            devtype = -1;
        }
    Mutex::Autolock unlock(mStorageLock);
    return devtype;
}

Int32 CMountService::GetVolumedevnode(
    /* [in] */ const String& mountPoint) {
    Int32 devnode;
    Mutex::Autolock lock(mStorageLock);
        HashMap<String, Int32>::Iterator iter = mStorageDevNode.Find(mountPoint);
        if (iter != mStorageDevNode.End()) {
            devnode = iter->mSecond;
        } else {
            devnode = -1;
        }
    Mutex::Autolock unlock(mStorageLock);
    return devnode;
}

String CMountService::GetVolumeUUID(
    /* [in] */ const String& mountPoint) {
//        String[] UUID = mConnector.doListCommand(
//            "volume UUID " + mountPoint, VoldResponseCode.VolumeLabelResult);
//        if (LOCAL_LOGD)
//            Slog.d(TAG, "UUID.length=" + UUID.length +
//                " UUID[0] = " + (UUID.length != 0 ? UUID[0] : ""));
//        if (UUID.length != 1)
            return String(NULL);
//        return UUID[0];
}

Int32 CMountService::FixPermissionsSecureContainer(
    /* [in] */ const String& id,
    /* [in] */ Int32 gid,
    /* [in] */ const String&filename)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_CREATE")
        /*android.Manifest.permission.ASEC_CREATE*/);
    WarnOnNotMounted();

    int rc = StorageResultCode::OperationSucceeded;

    StringBuffer cmd("asec fixperms");
    cmd += StringBuffer(" ")+id;
    cmd += StringBuffer(" ")+gid;
    cmd += StringBuffer(" ")+filename;
    DoCommand(String(cmd));

    return rc;
}

Boolean CMountService::IsExternalStorageEmulated()
{
    return mEmulateExternalStorage;
}

String CMountService::GetSecureContainerFilesystemPath(
    /* [in] */ const String& id)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_ACCESS"));
    WaitForReady();
    WarnOnNotMounted();

//    final NativeDaemonEvent event;
//    try {
//        event = mConnector.execute("asec", "fspath", id);
//        event.checkCode(VoldResponseCode.AsecPathResult);
//        return event.getMessage();
//    } catch (NativeDaemonConnectorException e) {
//        int code = e.getCode();
//        if (code == VoldResponseCode.OpFailedStorageNotFound) {
//            Slog.i(TAG, String.format("Container '%s' not found", id));
//            return null;
//        } else {
//            throw new IllegalStateException(String.format("Unexpected response code %d", code));
//        }
//    }
}

Boolean CMountService::IsUidOwnerOfPackageOrSystem(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 callerUid)
{
    if (callerUid == Process::SYSTEM_UID) {
        return TRUE;
    }
    if (packageName == NULL) {
        return FALSE;
    }

//    const Int32 packageUid = mPms->GetPackageUid(packageName, UserId.getUserId(callerUid));

//    if (DEBUG_OBB) {
//        Slog.d(TAG, "packageName = " + packageName + ", packageUid = " +
//                packageUid + ", callerUid = " + callerUid);
//    }

//    return callerUid == packageUid;
}

Int32 CMountService::GetEncryptionState()
{
    mContext->EnforceCallingOrSelfPermission(
         String("") /*Manifest.permission.CRYPT_KEEPER*/, String("no permission to access the crypt keeper"));

    WaitForReady();

//    final NativeDaemonEvent event;
//    try {
//        event = mConnector.execute("cryptfs", "cryptocomplete");
//        return Integer.parseInt(event.getMessage());
//    } catch (NumberFormatException e) {
//        // Bad result - unexpected.
//        Slog.w(TAG, "Unable to parse result from cryptfs cryptocomplete");
//        return ENCRYPTION_STATE_ERROR_UNKNOWN;
//    } catch (NativeDaemonConnectorException e) {
//        // Something bad happened.
//        Slog.w(TAG, "Error in communicating with cryptfs in validating");
//        return ENCRYPTION_STATE_ERROR_UNKNOWN;
//    }
}

Int32 CMountService::DecryptStorage(
    /* [in] */ ICharSequence* password)
{
    if (TextUtils::IsEmpty(password)) {
        LOGE("%s: password cannot be empty", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("password cannot be empty");
    }

    mContext->EnforceCallingOrSelfPermission(
             String("") /*Manifest.permission.CRYPT_KEEPER*/, String("no permission to access the crypt keeper"));

    WaitForReady();

    if (DEBUG_EVENTS) {
        LOGD("%s: decrypting storage...", TAG);
    }

//    final NativeDaemonEvent event;
//    try {
//        event = mConnector.execute("cryptfs", "checkpw", password);

//        final int code = Integer.parseInt(event.getMessage());
//        if (code == 0) {
//            // Decrypt was successful. Post a delayed message before restarting in order
//            // to let the UI to clear itself
//            mHandler.postDelayed(new Runnable() {
//                public void run() {
//                    try {
//                        mConnector.execute("cryptfs", "restart");
//                    } catch (NativeDaemonConnectorException e) {
//                        Slog.e(TAG, "problem executing in background", e);
//                    }
//                }
//            }, 1000); // 1 second
//        }

//        return code;
//    } catch (NativeDaemonConnectorException e) {
//        // Decryption failed
//        return e.getCode();
//    }
}

Int32 CMountService::EncryptStorage(
    /* [in] */ ICharSequence* password)
{
    if (TextUtils::IsEmpty(password)) {
        LOGE("%s: password cannot be empty", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mContext->EnforceCallingOrSelfPermission(
             String("") /*Manifest.permission.CRYPT_KEEPER*/, String("no permission to access the crypt keeper"));

    WaitForReady();

    if (DEBUG_EVENTS) {
        LOGD("%s: encrypting storage...", TAG);
    }
    DoCommand(String(StringBuffer("cryptfs enablecrypto inplace ")+password));
    return 0;
}

Int32 CMountService::ChangeEncryptionPassword(
    /* [in] */ ICharSequence* password)
{
    if (TextUtils::IsEmpty(password)) {
        LOGE("%s: password cannot be empty", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mContext->EnforceCallingOrSelfPermission(
             String("") /*Manifest.permission.CRYPT_KEEPER*/, String("no permission to access the crypt keeper"));

    WaitForReady();

    if (DEBUG_EVENTS) {
        LOGD("%s:changing encryption password...", TAG);
    }

//    final NativeDaemonEvent event;
//    try {
//        event = mConnector.execute("cryptfs", "changepw", password);
//        return Integer.parseInt(event.getMessage());
//    } catch (NativeDaemonConnectorException e) {
//        // Encryption failed
//        return e.getCode();
//    }
}

/**
 * Validate a user-supplied password string with cryptfs
 */
Int32 CMountService::VerifyEncryptionPassword(
    /* [in] */ ICharSequence* password)
{
//    // Only the system process is permitted to validate passwords
    if (Binder::GetCallingPid() != Process::SYSTEM_UID) {
        LOGE("%s: no permission to access the crypt keeper", TAG);
        return E_SECURITY_EXCEPTION;
        //throw new SecurityException("no permission to access the crypt keeper");
    }

    mContext->EnforceCallingOrSelfPermission(
         String("") /*Manifest.permission.CRYPT_KEEPER*/, String("no permission to access the crypt keeper"));

    if (TextUtils::IsEmpty(password)) {
        LOGE("%s: password cannot be empty", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    WaitForReady();

    if (DEBUG_EVENTS) {
        LOGD("%s:validating encryption password...", TAG);
    }

//    final NativeDaemonEvent event;
//    try {
//        event = mConnector.execute("cryptfs", "verifypw", password);
//        Slog.i(TAG, "cryptfs verifypw => " + event.getMessage());
//        return Integer.parseInt(event.getMessage());
//    } catch (NativeDaemonConnectorException e) {
//        // Encryption failed
//        return e.getCode();
//    }
}

ECode CMountService::GetVolumeList(
    /* [out, callee] */ ArrayOf<IParcelable*>** volList)
{
    Mutex::Autolock lock(mVolumesLock);
        int size;// = mVolumes.GetSize();
        ArrayOf<IParcelable*>* result = ArrayOf<IParcelable*>::Alloc(size);
        for (int i = 0; i < size; i++) {
            //result[i] = mVolumes.get(i);
        }
        *volList = result;
    Mutex::Autolock unlock(mVolumesLock);
}

ECode CMountService::CreateSecureContainer(
    /* [in] */ const String& id,
    /* [in] */ Int32 sizeMb,
    /* [in] */ const String& fstype,
    /* [in] */ const String& key,
    /* [in] */ Int32 ownerUid,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_CREATE"));
    WaitForReady();
    WarnOnNotMounted();

    int rc = StorageResultCode::OperationSucceeded;
    String cmd = String(StringBuffer("asec create ")+id+" "+sizeMb+" "+fstype+" "+key+" "+ownerUid);
    DoCommand(cmd);

    if (rc == StorageResultCode::OperationSucceeded) {
        Mutex::Autolock lock(mAsecMountSetLock);
            mAsecMountSet.Insert(id);
        Mutex::Autolock unlock(mAsecMountSetLock);
    }
    return rc;
}

ECode CMountService::DestroySecureContainer(
    /* [in] */ CString id,
    /* [in] */ Boolean force,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_DESTROY"));
    WaitForReady();
    WarnOnNotMounted();

    /*
     * Force a GC to make sure AssetManagers in other threads of the
     * system_server are cleaned up. We have to do this since AssetManager
     * instances are kept as a WeakReference and it's possible we have files
     * open on the external storage.
     */
    //Runtime.getRuntime().gc();

    Int32 rc = StorageResultCode::OperationSucceeded;
    DoCommand(String(StringBuffer("asec destroy ")+id+(force ? " force" : "")));

    if (rc == StorageResultCode::OperationSucceeded) {
        Mutex::Autolock lock(mAsecMountSetLock);
        HashSet<String>::Iterator iter = mAsecMountSet.Find(String(id));
        if (iter != mAsecMountSet.End()) {
            mAsecMountSet.Erase(String(id));
        }
        Mutex::Autolock unlock(mAsecMountSetLock);
    }
    *pResult = rc;
}

ECode CMountService::FinalizeSecureContainer(
    /* [in] */ CString id,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_CREATE"));
    WarnOnNotMounted();
    pResult = StorageResultCode::OperationSucceeded;

    DoCommand(String(StringBuffer("asec finalize ")+id));
}

/* Called by PackageManagerService after it is done processing SD card unmount.
 * H_UNMOUNT_PM_UPDATE -> mPms.updateExternalMediaStatus() -> finishMediaUpdate()
 */
ECode CMountService::FinishMediaUpdate()
{
    MsgData* msgSend = new MsgData();
    msgSend->msg = H_UNMOUNT_PM_DONE;
    mHandler->sendMessage(msgSend);//mHandler.sendEmptyMessage(H_UNMOUNT_PM_DONE);
}

ECode CMountService::FormatVolume(
    /* [in] */ CString mountPoint,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.MOUNT_FORMAT_FILESYSTEMS"));
    WaitForReady();

    *pResult = DoFormatVolume(String(mountPoint));
}

ECode CMountService::GetMountedObbPath(
    /* [in] */ CString filename,
    /* [out] */ String * pObbPath)
{
    if (filename.IsNull()){
        LOGE("%s: filename cannot be null", TAG);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
    //throw new IllegalArgumentException("filename cannot be null");
    }

    WaitForReady();
    WarnOnNotMounted();

    //ArrayOf<String> rsp = DoCommand(String(StringBuffer("obb path ")+filename));
    //String []tok = rsp.get(0).split(" ");
    //int code = Integer.parseInt(tok[0]);
    //if (code != VoldResponseCode.AsecPathResult) {
    //    throw new IllegalStateException(String.format("Unexpected response code %d", code));

   // *pObbPath = tok[1];

}

ECode CMountService::GetSecureContainerList(
    /* [out, callee] */ ArrayOf<String> ** ppIds)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_ACCESS"));
    WaitForReady();
    WarnOnNotMounted();
//    ppIds = NativeDaemonEvent.filterMessageList(
//                    mConnector.executeForList("asec", "list"), VoldResponseCode.AsecListResult);
}

ECode CMountService::GetSecureContainerPath(
    /* [in] */ CString id,
    /* [out] */ String * pPath)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_ACCESS"));
    WaitForReady();
    WarnOnNotMounted();

    //ArrayList<String> rsp = mConnector.doCommand(String.format("asec path %s", id));
    //ArrayOf<String> rsp = DoCommand(String(StringBuffer("asec path ")+id));
    //String []tok = rsp.get(0).split(" ");
    //int code = Integer.parseInt(tok[0]);
    //if (code != VoldResponseCode.AsecPathResult) {
    //    throw new IllegalStateException(String.format("Unexpected response code %d", code));

    //return tok[1];
}

ECode CMountService::GetStorageUsers(
    /* [in] */ CString path,
    /* [out, callee] */ ArrayOf<Int32> ** ppPids)
{
    ValidatePermission(String("elastos.Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS"));
    WaitForReady();
//     final String[] r = NativeDaemonEvent.filterMessageList(
//                    mConnector.executeForList("storage", "users", path),
//                    VoldResponseCode.StorageUsersListResult);
    // FMT: <pid> <process name>
    //int[] data = new int[r.length];
    //for (int i = 0; i < r.length; i++) {
    //    String []tok = r[i].split(" ");
    //    data[i] = Integer.parseInt(tok[0]);
    //}
    //return data;

}

/**
 * @return state of the volume at the specified mount point
 */
ECode CMountService::GetVolumeState(
    /* [in] */ CString mountPoint,
    /* [out] */ String * pState)
{
    Mutex::Autolock lock(mVolumeStatesLock);
    Logger::D(TAG, StringBuffer("GetVolumeState( ")+String(mountPoint)+" )");

    HashMap<String, String>::Iterator iter = mVolumeStates.Find(mExternalStoragePath);
    if (iter != mVolumeStates.End()) {
	 Logger::D(TAG, StringBuffer("state(")+iter->mSecond+")");
        *pState = iter->mSecond;
    } else {
        Logger::E(TAG, StringBuffer("GetVolumeState(")+String(mountPoint)+"): Unknown volume");
        *pState = NULL;
    }
//    if (*pState == NULL) {
//        Slog.w(TAG, "getVolumeState(" + mountPoint + "): Unknown volume");
//        // don't throw on dynamically created usb mount paths. eg /mnt/sda1
//        if (mountPoint.startsWith("/storage/sd") ||mountPoint.startsWith("/mnt/sd")||
//                SystemProperties.get("vold.encrypt_progress").length() != 0) {
//            state = Environment.MEDIA_REMOVED;
//        } else {
//            throw new IllegalArgumentException();
//        }
//    }
    Mutex::Autolock unlock(mVolumeStatesLock);
}

ECode CMountService::IsObbMounted(
    /* [in] */ CString filename,
    /* [out] */ Boolean * pIsMounted)
{
    if (filename.IsNull()) {
        LOGE("%s: filename cannot be null", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException("filename cannot be null");
    }

    Mutex::Autolock lock(mObbMountsLock);
//        return mObbPathToStateMap.containsKey(filename);
}

ECode CMountService::IsSecureContainerMounted(
    /* [in] */ CString id,
    /* [out] */ Boolean * pIsMounted)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_ACCESS"));
    WaitForReady();
    WarnOnNotMounted();

    Mutex::Autolock lock(mAsecMountSetLock);
        HashSet<String>::Iterator iter = mAsecMountSet.Find(String(id));
        if (iter != mAsecMountSet.End()) {
            *pIsMounted = TRUE;
        } else {
            *pIsMounted = FALSE;
        }
    Mutex::Autolock unlock(mAsecMountSetLock);
}

ECode CMountService::IsUsbMassStorageConnected(
    /* [out] */ Boolean * pIsConnected)
{
    WaitForReady();

    if (GetUmsEnabling()) {
        return true;
    }
    Mutex::Autolock lock(mListenersLock);
    return mUmsAvailable;
}

ECode CMountService::IsUsbMassStorageEnabled(
    /* [out] */ Boolean * pIsEnabled)
{
   WaitForReady();
    ArrayOf<String>* paths;//String[] paths;
    int count;
    Mutex::Autolock lock(mVolumeStatesLock);
        Set<String> keys;//Set<String> keys = mVolumeStates.keySet();
        HashMap<String, String>::Iterator iter;
        for (iter =  mVolumeStates.Begin(); iter != mVolumeStates.End(); ++iter) {
            keys.Insert(iter->mFirst);
        }
        count = keys.GetSize(); //count = keys.size();
        paths = ArrayOf<String>::Alloc(count);
    Mutex::Autolock unlock(mVolumeStatesLock);
    for (int i = 0; i < count; i++) {
        String path = (*paths)[i];
        if (DoGetVolumeShared(path, String("ums")))
            *pIsEnabled = TRUE;
    }
    *pIsEnabled = FALSE;
}

ECode CMountService::MountObb(
    /* [in] */ const String& filename,
    /* [in] */ const String& key,
    /* [in] */ IObbActionListener * pToken,
    /* [in] */ Int32 nonce)
{
    if (filename == NULL) {
        LOGE("%s: filename cannot be null", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException("filename cannot be null");
    }
    if (pToken == NULL) {
        LOGE("%s: token cannot be null", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 callerUid = Binder::GetCallingUid();
    ObbState* obbState = new ObbState(filename, callerUid, pToken, nonce);
    ObbAction* action = new MountObbAction(obbState, key);
//    mObbActionHandler.sendMessage(mObbActionHandler.obtainMessage(OBB_RUN_ACTION, action));

//    if (DEBUG_OBB)
//        Slog.i(TAG, "Send to OBB handler: " + action.toString());
}

ECode CMountService::MountSecureContainer(
    /* [in] */ const String& id,
    /* [in] */ const String& key,
    /* [in] */ Int32 ownerUid,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_MOUNT_UNMOUNT"));
    WaitForReady();
    WarnOnNotMounted();

    Mutex::Autolock lock(mAsecMountSetLock);
    HashSet<String>::Iterator iter = mAsecMountSet.Find(id);
        if (iter != mAsecMountSet.End()) {
        *pResult = StorageResultCode::OperationFailedStorageMounted;
        return 0;
    }

    int rc = StorageResultCode::OperationSucceeded;
    String cmd = String(StringBuffer("asec mount ")+id+" "+key+" "+ownerUid);

    DoCommand(cmd);

    if (rc == StorageResultCode::OperationSucceeded) {
        Mutex::Autolock lock(mAsecMountSetLock);
        mAsecMountSet.Insert(id);
    }
    *pResult = rc; //return rc;
}

ECode CMountService::MountVolume(
    /* [in] */ const String& mountPoint,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS"));
    WaitForReady();

    *pResult = DoMountVolume(mountPoint);
}

ECode CMountService::RegisterListener(
    /* [in] */ IMountServiceListener * pListener)
{
    Mutex::Autolock lock(mListenersLock);
//            MountServiceBinderListener bl = new MountServiceBinderListener(listener);
//            listener.asBinder().linkToDeath(bl, 0);
//            mListeners.add(bl);
}

ECode CMountService::RenameSecureContainer(
    /* [in] */ const String& oldId,
    /* [in] */ const String& newId,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_RENAME"));
    WaitForReady();
    WarnOnNotMounted();

    Mutex::Autolock lock(mAsecMountSetLock);
    /*
     * Because a mounted container has active internal state which cannot be
     * changed while active, we must ensure both ids are not currently mounted.
     */
    HashSet<String>::Iterator iter = mAsecMountSet.Find(oldId);
    HashSet<String>::Iterator iter2= mAsecMountSet.Find(newId);
    if (iter != mAsecMountSet.End() && iter2 != mAsecMountSet.End() ) {
        *pResult = StorageResultCode::OperationFailedStorageMounted;
        return 0;
    }

    int rc = StorageResultCode::OperationSucceeded;
    String cmd = String(StringBuffer("asec rename ")+oldId+" "+newId);
    DoCommand(cmd);
    return rc;
}

ECode CMountService::SetUsbMassStorageEnabled(
    /* [in] */ Boolean enable)
{
//    if (LOCAL_LOGD) LOGD(TAG, "setUsbMassStorageEnabled " + Boolean.toString(enable));
    WaitForReady();
    ValidatePermission(String("elastos.Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS"));

    // TODO: Add support for multiple share methods
    Mutex::Autolock lock(mUmsEnablingVolumesLock);
    mUmsEnablingVolumes->Clear();
    if (LOCAL_LOGD) LOGD("%s:mUmsEnablingVolumes.clear", TAG);

    ArrayOf<String>* paths;
    ArrayOf<String>* states;
    int count;
    //Mutex::Autolock lock(mVolumeStatesLock);
        Set<String> keys;
        HashMap<String, String>::Iterator iter;
        for (iter =  mVolumeStates.Begin(); iter != mVolumeStates.End(); ++iter) {
            keys.Insert(iter->mFirst);
        }
        count = keys.GetSize();
        paths = ArrayOf<String>::Alloc(count);
        states = ArrayOf<String>::Alloc(count);
        for (int i = 0; i < count; i++) {
            (*states)[i] = mVolumeStates[ (*paths)[i] ];
        }

    if (enable) {
//        mSharedVolumes.clear();
        // create a list of volumes that will be enabled
        for (int i = 0; i < count; i++) {
            String path = (*paths)[i];
            String vs = (*states)[i];
            if (vs.Equals(Environment::MEDIA_MOUNTED)) {
                // Override for isUsbMassStorageEnabled()
                SetUmsEnabling(enable);
                //Mutex::Autolock lock(mUmsEnablingVolumesLock);
                    mUmsEnablingVolumes->PushBack(path);
                    mSharedVolumes->PushBack(path);
                if (LOCAL_LOGD) LOGD("%s: mUmsEnablingVolumes.add %s", TAG, (const char*)path);
            } else {
                LOGW("%s: not adding?? %s", TAG, (const char*)path);
            }
        }
    }

    for (int i = 0; i < count; i++) {
        String path = (*paths)[i];
        String vs = (*states)[i];
        const String method = String("ums");

        /*
         * If the volume is mounted and we're enabling then unmount it
         */
        if (enable && vs.Equals(Environment::MEDIA_MOUNTED)) {
            if(path.Equals(EXTERNAL_STORAGE_PATH))//share others first
                continue;
//            UmsEnableCallBack umscb = new UmsEnableCallBack(path, method, true);
//            mHandler.sendMessage(mHandler.obtainMessage(H_UNMOUNT_PM_UPDATE, umscb));
        }
        /*
         * If we disabled UMS then mount the volume
         */
        if (!enable) {
            if(!path.Equals(EXTERNAL_STORAGE_PATH))//unshare EXTERNAL_STORAGE_PATH first
                continue;
                DoShareUnshareVolume(path, method, enable);
            if (DoMountVolume(path) != StorageResultCode::OperationSucceeded) {
//                Slog.e(TAG, "Failed to remount " + path +
//                        " after disabling share method " + method);
                /*
                 * Even though the mount failed, the unshare didn't so don't indicate an error.
                 * The mountVolume() call will have set the storage state and sent the necessary
                 * broadcasts.
                 */
            }
        }
    }

    for (int i = 0; i < count; i++) {
        String path = (*paths)[i];
        String vs = (*states)[i];
        const String method = String("ums");

        /*
         * If the volume is mounted and we're enabling then unmount it
         */
        if (enable && vs.Equals(Environment::MEDIA_MOUNTED)) {
            if(!path.Equals(EXTERNAL_STORAGE_PATH))//share EXTERNAL_STORAGE_PATH at last
                continue;
//                UmsEnableCallBack umscb = new UmsEnableCallBack(path, method, true);
//                mHandler.sendMessage(mHandler.obtainMessage(H_UNMOUNT_PM_UPDATE, umscb));
        }
        /*
         * If we disabled UMS then mount the volume
         */
        if (!enable) {
            if(path.Equals(EXTERNAL_STORAGE_PATH))//unshare others later
                continue;
            DoShareUnshareVolume(path, method, enable);
            if (DoMountVolume(path) != StorageResultCode::OperationSucceeded) {
//                    Slog.e(TAG, "Failed to remount " + path +
//                            " after disabling share method " + method);
                /*
                 * Even though the mount failed, the unshare didn't so don't indicate an error.
                 * The mountVolume() call will have set the storage state and sent the necessary
                 * broadcasts.
                 */
            }
        }
    }
}

ECode CMountService::Shutdown(
    /* [in] */ IMountShutdownObserver * pObserver)
{
    ValidatePermission(String("elastos.Manifest.permission.SHUTDOWN"));

    LOGI("%s:Shutting down", TAG);
    Mutex::Autolock lock(mVolumeStatesLock);
//        for (String path : mVolumeStates.keySet()) {
//              String state = mVolumeStates.get(path);
            String state;
            HashMap<String, String>::Iterator iter = mVolumeStates.Find(mExternalStoragePath);
            if (iter != mVolumeStates.End())
                Logger::E(TAG, StringBuffer("MS state(")+ iter->mSecond+")");
            else
                state = iter->mSecond;

            if (state.Equals(Environment::MEDIA_SHARED)) {
                /*
                 * If the media is currently shared, unshare it.
                 * XXX: This is still dangerous!. We should not
                 * be rebooting at *all* if UMS is enabled, since
                 * the UMS host could have dirty FAT cache entries
                 * yet to flush.
                 */
                SetUsbMassStorageEnabled(FALSE);
            } else if (state.Equals(Environment::MEDIA_CHECKING)) {
                /*
                 * If the media is being checked, then we need to wait for
                 * it to complete before being able to proceed.
                 */
                // XXX: @hackbod - Should we disable the ANR timer here?
                int retries = 30;
                while (state.Equals(Environment::MEDIA_CHECKING) && (retries-- >=0)) {
//                    try {
//                        Thread.sleep(1000);
                            usleep(1000*1000);
//                    } catch (InterruptedException iex) {
//                        Slog.e(TAG, "Interrupted while waiting for media", iex);
//                        break;
//                    }
                        state = Environment::GetExternalStorageState();
                }
                if (retries == 0) {
                    LOGE("%s:Timed out waiting for media to check", TAG);
                }
            }

            if (state.Equals(Environment::MEDIA_MOUNTED)) {
                // Post a unmount message.
//                ShutdownCallBack ucb = new ShutdownCallBack(path, observer);
//                mHandler.sendMessage(mHandler.obtainMessage(H_UNMOUNT_PM_UPDATE, ucb));
            } else if (pObserver != NULL) {
                /*
                 * Observer is waiting for onShutDownComplete when we are done.
                 * Since nothing will be done send notification directly so shutdown
                 * sequence can continue.
                 */
//                try {
//                    observer.onShutDownComplete(StorageResultCode.OperationSucceeded);
//                } catch (RemoteException e) {
//                    Slog.w(TAG, "RemoteException when shutting down");
//                }
            }
//        }
}

ECode CMountService::UnmountObb(
    /* [in] */ CString filename,
    /* [in] */ Boolean force,
    /* [in] */ IObbActionListener * pToken,
    /* [in] */ Int32 nonce)
{
    if (filename.IsNull()) {
        LOGE("%s: filename cannot be null", TAG);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException("filename cannot be null");
    }

    Int32 callerUid = Binder::GetCallingUid();
    ObbState* obbState = new ObbState(String(filename), callerUid, pToken, nonce);
    ObbAction* action = new UnmountObbAction(obbState, force);
//    mObbActionHandler.sendMessage(mObbActionHandler.obtainMessage(OBB_RUN_ACTION, action));
}

ECode CMountService::UnmountSecureContainer(
    /* [in] */ CString id,
    /* [in] */ Boolean force,
    /* [out] */ Int32 * pResult)
{
    ValidatePermission(String("elastos.Manifest.permission.ASEC_MOUNT_UNMOUNT"));
    WaitForReady();
    WarnOnNotMounted();

    Mutex::Autolock lock(mAsecMountSetLock);
    HashSet<String>::Iterator iter = mAsecMountSet.Find(String(id));
    if (iter == mAsecMountSet.End()) {
        *pResult = StorageResultCode::OperationFailedStorageMounted;
        return 0;
    }

    /*
     * Force a GC to make sure AssetManagers in other threads of the
     * system_server are cleaned up. We have to do this since AssetManager
     * instances are kept as a WeakReference and it's possible we have files
     * open on the external storage.
     */
//    Runtime.getRuntime().gc();

    int rc = StorageResultCode::OperationSucceeded;
    String cmd = String(StringBuffer("asec unmount ")+id+(force ? " force" : ""));
    DoCommand(cmd);

    if (rc == StorageResultCode::OperationSucceeded) {
        Mutex::Autolock lock(mAsecMountSetLock);
        mAsecMountSet.Erase(String(id));
    }
    *pResult = rc; //return rc;
}

ECode CMountService::UnmountVolume(
    /* [in] */ CString mountPoint,
    /* [in] */ Boolean force)
{
    ValidatePermission(String("elastos.Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS")
        /*android.Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS*/);
    WaitForReady();

    String volState;
    GetVolumeState(mountPoint, &volState);
//    if (DEBUG_UNMOUNT) {
//        Slog.i(TAG, "Unmounting " + path
//                + " force = " + force
//                + " removeEncryption = " + removeEncryption);
//    }
    if(volState.Equals(Environment::MEDIA_UNMOUNTED) ||
        volState.Equals(Environment::MEDIA_REMOVED) ||
        volState.Equals(Environment::MEDIA_SHARED) ||
        volState.Equals(Environment::MEDIA_UNMOUNTABLE)) {
        // Media already unmounted or cannot be unmounted.
        // TODO return valid return code when adding observer call back.
        return 0;
    }
    MsgData* msgSend = new MsgData();
    msgSend->msg = H_UNMOUNT_PM_UPDATE;
    msgSend->ucb = new UnmountCallBack(String(mountPoint), force, FALSE);
    mHandler->sendMessage(msgSend);
}

ECode CMountService::UnregisterListener(
    /* [in] */ IMountServiceListener * pListener)
{
     Mutex::Autolock lock(mListenersLock);
//        for(MountServiceBinderListener bl : mListeners) {
//            if (bl.mListener == listener) {
//                mListeners.remove(mListeners.indexOf(bl));
//                return;
//            }
//        }
}

ECode CMountService::constructor(
    /* [in] */ IContext * pCtx)
{
    mContext = pCtx;
    struct sockaddr_un addr;
    int door_sock = -1;
    int len;
    char buf[BUFSIZ];
    LOGD("Elastos Mount Service version 0.2\n");

    /*
     * Create all the various sockets we'll need
     */
    door_sock = socket(PF_UNIX, SOCK_STREAM, 0);
    if (door_sock < 0) {
        LOGD("Failed to open socket '%s': %s",
             VOLD_SOCKET, strerror(errno));
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "/dev/socket/%s", VOLD_SOCKET);

    int ret = connect(door_sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret) {
        LOGD("Unable to connect on fd '%d' for socket '%s': %s",
             door_sock, VOLD_SOCKET, strerror(errno));
        exit(1);
    }

    LOGD("Listen to %s\n", VOLD_SOCKET);

    while(true)
    {
        pthread_t thread;
        String* message;

        len = recv(door_sock, buf, BUFSIZ, 0);
        buf[len]='\0';
        if (!strcmp(buf, "quit")) break;

        message = new String(buf);
        pthread_create(&thread, NULL, threadRoutine, (void*)message);
    }

    close(door_sock);
}

ECode CMountService::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode CMountService::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode CMountService::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

