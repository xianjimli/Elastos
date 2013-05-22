
#ifndef __CMOUNTSERVICE_H__
#define __CMOUNTSERVICE_H__
#define __USE_MALLOC

#include "_CMountService.h"
#include "server/CCapsuleManagerService.h"
//#include "server/CActivityManagerService.h"
#include "net/Uri.h"
#include <assert.h>
#include <stdlib.h>
#include <cutils/log.h>
//#include <CountDownLatch.h>

#include <elastos.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <elastos/List.h>
#include <elastos/Map.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

CarClass(CMountService)
{
private:
    static const Int32 DEBUG_EVENTS = 1;
    static const Int32 DEBUG_UNMOUNT = 1;
    static const Int32 LOCAL_LOGD = 1;
    static const char* TAG;

    AutoPtr<IApartment>     mApartment;
    AutoPtr<IContext>           mContext;
    //private NativeDaemonConnector                 mConnector;
    List<AutoPtr<IStorageVolume> >*        mVolumes;// = new ArrayList<StorageVolume>();
    AutoPtr<IStorageVolume>                      mPrimaryVolume;
    HashMap<String, String>         mVolumeStates;
    HashMap<String, AutoPtr<IStorageVolume> >  mVolumeMap;// = new HashMap<String, StorageVolume>();
    String                                 mExternalStoragePath;
    HashMap<String, String>             mVolumeLabel;// = new HashMap<String, String>();
    static AutoPtr<CCapsuleManagerService>              mPms; //private PackageManagerService mPms;
    Boolean                               mUmsEnabling;
    List<String>*             mUmsEnablingVolumes; //= new ArrayList<String>();
    List<String>*             mSharedVolumes; // = new ArrayList<String>();
    Boolean                               mUmsAvailable;
    // Used as a lock for methods that register/unregister listeners.
    //final private ArrayList<MountServiceBinderListener> mListeners =
    //        new ArrayList<MountServiceBinderListener>();
    Boolean                               mBooted;
//    private CountDownLatch                        mConnectedSignal = new CountDownLatch(1);
//    private CountDownLatch                        mAsecsScanned = new CountDownLatch(1);
    Boolean                               mSendUmsConnectedOnBoot;
    // true if we should fake MEDIA_MOUNTED state for external storage
    Boolean                               mEmulateExternalStorage;

    List<String>*             mStorage;// = new ArrayList<String>();
    HashMap<String, Int32>     mStorageDevType; //= new HashMap<String, Integer>();
    HashMap<String, Int32>     mStorageDevNode; //= new HashMap<String, Integer>();

    /**
     * Only updating PackageManager when changes happen on this storage path
     */
    static const String EXTERNAL_STORAGE_PATH; //= Environment.getExternalStorageDirectory().toString();
    static const String EXTERNAL_STORAGE2_PATH; //= Environment.getExternalStorage2Directory().toString();

    /**
     * Private hash of currently mounted secure containers.
     * Used as a lock in methods to manipulate secure containers.
     */
    HashSet<String> mAsecMountSet; // = new HashSet<String>();
    List<String>* mPrevAsecMountSet; //private String[] mPrevAsecMountSet = null;

    /**
     * The size of the crypto algorithm key in bits for OBB files. Currently
     * Twofish is used which takes 128-bit keys.
     */
    static const Int32 CRYPTO_ALGORITHM_KEY_SIZE = 128;

    /**
     * The number of times to run SHA1 in the PBKDF2 function for OBB files.
     * 1024 is reasonably secure and not too slow.
     */
    static const Int32 PBKDF2_HASH_ROUNDS = 1024;

    /**
     * Mounted OBB tracking information. Used to track the current state of all
     * OBBs.
     */
    //final private Map<IBinder, List<ObbState>> mObbMounts = new HashMap<IBinder, List<ObbState>>();
    //final private Map<String, ObbState> mObbPathToStateMap = new HashMap<String, ObbState>();

    //Mutex
    Mutex mVolumesLock;
    Mutex mStorageLock;
    Mutex mAsecMountSetLock;
    Mutex mListenersLock;
    Mutex mUmsEnablingVolumesLock;
    Mutex mVolumeStatesLock;
    Mutex mSharedVolumesLock;
    Mutex mObbMountsLock;

    // Handler messages
    static const Int32 H_UNMOUNT_PM_UPDATE = 1;
    static const Int32 H_UNMOUNT_PM_DONE = 2;
    static const Int32 H_UNMOUNT_MS = 3;
    static const Int32 RETRY_UNMOUNT_DELAY = 30; // in ms
    static const Int32 MAX_UNMOUNT_RETRIES = 4;

    friend class MountServiceHandler;

private:

    class VolumeState
    {
    public:
        static const Int32 Init       = -1;
        static const Int32 NoMedia    = 0;
        static const Int32 Idle       = 1;
        static const Int32 Pending    = 2;
        static const Int32 Checking   = 3;
        static const Int32 Mounted    = 4;
        static const Int32 Unmounting = 5;
        static const Int32 Formatting = 6;
        static const Int32 Shared     = 7;
        static const Int32 SharedMnt  = 8;
    };

    /*
     * Internal vold response code constants
     */
    class VoldResponseCode
    {
    public:
        /*
         * 100 series - Requestion action was initiated; expect another reply
         *              before proceeding with a new command.
         */
        static const Int32 VolumeListResult               = 110;
        static const Int32 AsecListResult                 = 111;
        static const Int32 StorageUsersListResult         = 112;

        /*
         * 200 series - Requestion action has been successfully completed.
         */
        static const Int32 ShareStatusResult              = 210;
        static const Int32 AsecPathResult                 = 211;
        static const Int32 ShareEnabledResult             = 212;

        /*
         * 400 series - Command was accepted, but the requested action
         *              did not take place.
         */
        static const Int32 OpFailedNoMedia                = 401;
        static const Int32 OpFailedMediaBlank             = 402;
        static const Int32 OpFailedMediaCorrupt           = 403;
        static const Int32 OpFailedVolNotMounted          = 404;
        static const Int32 OpFailedStorageBusy            = 405;
        static const Int32 OpFailedStorageNotFound        = 406;

        /*
         * 600 series - Unsolicited broadcasts.
         */
        static const Int32 VolumeStateChange              = 605;
        static const Int32 ShareAvailabilityChange        = 620;
        static const Int32 VolumeDiskInserted             = 630;
        static const Int32 VolumeDiskRemoved              = 631;
        static const Int32 VolumeBadRemoval               = 632;
    };

    class ResponseCode
    {
    public:
        static const Int32 ActionInitiated                = 100;
        static const Int32 CommandOkay                    = 200;

        // The range of 400 -> 599 is reserved for cmd failures
        static const Int32 OperationFailed                = 400;
        static const Int32 CommandSyntaxError             = 500;
        static const Int32 CommandParameterError          = 501;
        static const Int32 UnsolicitedInformational       = 600;

        //
        static const Int32 FailedRangeStart               = 400;
        static const Int32 FailedRangeEnd                 = 599;
    };

    /**
     * Class that provides access to constants returned from StorageManager
     * and lower level MountService APIs.
     *
     * @hide
     */
    class StorageResultCode
    {
    public:
        /**
         * Operation succeeded.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationSucceeded               =  0;

        /**
         * Operation failed: internal error.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationFailedInternalError     = -1;

        /**
         * Operation failed: Missing media.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationFailedNoMedia           = -2;

        /**
         * Operation failed: Media is blank.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationFailedMediaBlank        = -3;

        /**
         * Operation failed: Media is corrupt.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationFailedMediaCorrupt      = -4;

        /**
         * Operation failed: Storage not mounted.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationFailedStorageNotMounted  = -5;

        /**
         * Operation failed: Storage is mounted.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationFailedStorageMounted     = -6;

        /**
         * Operation failed: Storage is busy.
         * @see android.os.storage.StorageManager
         */
        static const Int32 OperationFailedStorageBusy        = -7;

    };

    class ObbState// : public IBinderDeathRecipient
    {
        friend class CMountService;
    public:
        String mFilename; // OBB source filename
        Int32 mCallerUid; // Binder.callingUid()
        IObbActionListener* mToken; // Token of remote Binder caller
        Int32 mNonce; // Identifier to pass back to the token

        ObbState(String filename, Int32 callerUid, IObbActionListener* token, Int32 nonce);
        AutoPtr<IBinder> GetBinder();
        void Link();
        void Unlink();
        String ToString();
        CARAPI BinderDied();
    };

    class ObbAction
    {
    private:
        static const Int32 MAX_RETRIES = 3;
        Int32 mRetries;
        ObbState* mObbState;

    public:
        ObbAction(ObbState* obbState);
//        void execute(ObbActionHandler handler);
        virtual void HandleExecute();
        virtual void HandleError();

    protected:
//        protected ObbInfo getObbInfo() throws IOException;
        void SendNewStatusOrIgnore(Int32 status);
    };

    class MountObbAction: public ObbAction
    {
    private:
        String mKey;

    public:
        MountObbAction(ObbState* obbState, String key);
        void HandleExecute();
        void HandleError();
        String ToString();
    };

    class UnmountObbAction: public ObbAction
    {
    private:
        Boolean mForceUnmount;

    public:
        UnmountObbAction(ObbState* obbState, Boolean force);
        void HandleExecute();
        void HandleError();
        String ToString();
    };

    class UnmountCallBack 
    {
    public:
        String mPath;
        Boolean mForce;
        Boolean mRemoveEncryption;
        Int32 mRetries;
        
    public:
        UnmountCallBack(String path, Boolean force, Boolean removeEncryption);
        void handleFinished();
    };

    class UmsEnableCallBack: public UnmountCallBack 
    {
    private:
        String mMethod;

    public:
        UmsEnableCallBack(String path, String method, Boolean force);
        void handleFinished();
    };

    class ShutdownCallBack: public UnmountCallBack 
    {
    private:
        //IMountShutdownObserver observer;

    public:    
        ShutdownCallBack(String path);
        void handleFinished(); 
    };

    class MsgData
    {
    public:
        Int32 msg;
        UnmountCallBack* ucb;
    };

    class MountServiceHandler // :public Handler
    {
    private:
        List<UnmountCallBack*>* mForceUnmounts;
        Boolean mUpdatingStatus;

    public:
        MountServiceHandler();
        void sendMessage(MsgData* msg);
        void sendMessageDelayed(MsgData* msg, Millisecond64 delay);
        void obtainMessage();
        void handleMessage(MsgData* msg);
    };

    static MountServiceHandler* mHandler;

public:
    CMountService();
    ~CMountService();

    static CMountService* GetDefault();
    
    CARAPI_(Boolean) OnMessage(
        /* [in] */ const String& message);
    
    CARAPI_(Boolean) OnEvent(
        /* [in] */ Int32 code,
        /* [in] */ const String& raw,
        /* [in] */ String* cooked);

   CARAPI_(void) NotifyVolumeStateChange(
        /* [in] */ const String& label,
        /* [in] */ const String& path,
        /* [in] */ Int32 oldState,
        /* [in] */ Int32 newState);

    CARAPI_(Int32) DoMountVolume(
        /* [in] */ const String& path);

    CARAPI_(void) DoCommand(
        /* [in] */ const String& cmd);

    CARAPI_(void) DoShareUnshareVolume(
        /* [in] */ const String& path,
        /* [in] */ const String& method,
        /* [in] */ Boolean enable);

    CARAPI_(void) UpdatePublicVolumeState(
        /* [in] */ const String& path,
        /* [in] */ const String& state);

    CARAPI_(Int32) DoUnmountVolume(
        /* [in] */ const String& path,
        /* [in] */ Boolean force,
        /* [in] */ Boolean removeEncryption);

    CARAPI_(Int32) DoFormatVolume(
        /* [in] */ const String& path);

    CARAPI_(Boolean) DoGetVolumeShared(
        /* [in] */ const String& path,
        /* [in] */ const String& method);

    CARAPI_(void) NotifyShareAvailabilityChange(
        /* [in] */ const Boolean avail);

    CARAPI_(void) SendStorageIntent(
        /* [in] */ const String& action,
        /* [in] */ const String& path);

    CARAPI_(void) SendUmsIntent(
        /* [in] */ Boolean c);

    CARAPI_(Boolean) GetUmsEnabling();

    CARAPI_(void) SetUmsEnabling(
        /* [in] */ Boolean ena);

    CARAPI_(void) ValidatePermission(
    /* [in] */ const String& perm);

    CARAPI_(void) ReadStorageList();

//    CARAPI_(void) WaitForLatch(
//    /* [in] */ CountDownLatch latch);

    CARAPI_(void) WaitForReady();

    CARAPI_(Boolean) IsUsbMassStorageAllEnabled();

    CARAPI_(String) GetVolumeLabel(
    /* [in] */ const String& mountPoint);

    CARAPI_(String) GetVolumeFSLabel(
    /* [in] */ const String& mountPoint);

    CARAPI_(List<String>* ) GetAllVolume();

    CARAPI_(Int32) GetVolumedevtype(
    /* [in] */ const String& mountPoint);

    CARAPI_(Int32) GetVolumedevnode(
    /* [in] */ const String& mountPoint);

    CARAPI_(String) GetVolumeUUID(
    /* [in] */ const String& mountPoint);

    CARAPI_(Boolean) IsExternalStorageEmulated();

    CARAPI_(Boolean) WarnOnNotMounted();

    CARAPI_(Int32) FixPermissionsSecureContainer(
    /* [in] */ const String& id,
    /* [in] */ Int32 gid,
    /* [in] */ const String&filename);

    CARAPI_(String) GetSecureContainerFilesystemPath(
    /* [in] */ const String& id);

    CARAPI_(Boolean) IsUidOwnerOfPackageOrSystem(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 callerUid);

    CARAPI_(Int32) GetEncryptionState();

    CARAPI_(Int32) DecryptStorage(
    /* [in] */ ICharSequence* password);

    CARAPI_(Int32) EncryptStorage(
    /* [in] */ ICharSequence* password);

    CARAPI_(Int32) ChangeEncryptionPassword(
    /* [in] */ ICharSequence* password);

    CARAPI_(Int32) VerifyEncryptionPassword(
    /* [in] */ ICharSequence* password);

    CARAPI GetVolumeList(
    /* [out, callee] */ ArrayOf<IParcelable*>** volList);

    CARAPI SendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI SendMessageAtTime(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI RemoveMessage(
        /* [in] */ Handle32 func);
    
    CARAPI CreateSecureContainer(
        /* [in] */ const String& id,
        /* [in] */ Int32 sizeMb,
        /* [in] */ const String& fstype,
        /* [in] */ const String& key,
        /* [in] */ Int32 ownerUid,
        /* [out] */ Int32 * pResult);

    CARAPI DestroySecureContainer(
        /* [in] */ CString id,
        /* [in] */ Boolean force,
        /* [out] */ Int32 * pResult);

    CARAPI FinalizeSecureContainer(
        /* [in] */ CString id,
        /* [out] */ Int32 * pResult);

    CARAPI FinishMediaUpdate();

    CARAPI FormatVolume(
        /* [in] */ CString mountPoint,
        /* [out] */ Int32 * pResult);

    CARAPI GetMountedObbPath(
        /* [in] */ CString filename,
        /* [out] */ String * pObbPath);

    CARAPI GetSecureContainerList(
        /* [out, callee] */ ArrayOf<String> ** ppIds);

    CARAPI GetSecureContainerPath(
        /* [in] */ CString id,
        /* [out] */ String * pPath);

    CARAPI GetStorageUsers(
        /* [in] */ CString path,
        /* [out, callee] */ ArrayOf<Int32> ** ppPids);

    CARAPI GetVolumeState(
        /* [in] */ CString mountPoint,
        /* [out] */ String * pState);

    CARAPI IsObbMounted(
        /* [in] */ CString filename,
        /* [out] */ Boolean * pIsMounted);

    CARAPI IsSecureContainerMounted(
        /* [in] */ CString id,
        /* [out] */ Boolean * pIsMounted);

    CARAPI IsUsbMassStorageConnected(
        /* [out] */ Boolean * pIsConnected);

    CARAPI IsUsbMassStorageEnabled(
        /* [out] */ Boolean * pIsEnabled);

    CARAPI MountObb(
        /* [in] */ const String& filename,
        /* [in] */ const String& key,
        /* [in] */ IObbActionListener * pToken,
        /* [in] */ Int32 nonce);

    CARAPI MountSecureContainer(
        /* [in] */ const String& id,
        /* [in] */ const String& key,
        /* [in] */ Int32 ownerUid,
        /* [out] */ Int32 * pResult);

    CARAPI MountVolume(
        /* [in] */ const String& mountPoint,
        /* [out] */ Int32 * pResult);

    CARAPI RegisterListener(
        /* [in] */ IMountServiceListener * pListener);

    CARAPI RenameSecureContainer(
        /* [in] */ const String& oldId,
        /* [in] */ const String& newId,
        /* [out] */ Int32 * pResult);

    CARAPI SetUsbMassStorageEnabled(
        /* [in] */ Boolean value);

    CARAPI Shutdown(
        /* [in] */ IMountShutdownObserver * pObserver);

    CARAPI UnmountObb(
        /* [in] */ CString filename,
        /* [in] */ Boolean force,
        /* [in] */ IObbActionListener * pToken,
        /* [in] */ Int32 nonce);

    CARAPI UnmountSecureContainer(
        /* [in] */ CString id,
        /* [in] */ Boolean force,
        /* [out] */ Int32 * pResult);

    CARAPI UnmountVolume(
        /* [in] */ CString mountPoint,
        /* [in] */ Boolean force);

    CARAPI UnregisterListener(
        /* [in] */ IMountServiceListener * pListener);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);
};

#endif // __CMOUNTSERVICE_H__
