
#ifndef __CSTORAGEMANAGER_H__
#define __CSTORAGEMANAGER_H__

#include "_CStorageManager.h"
#include <elastos/AutoPtr.h>

CarClass(CStorageManager)
{
public:
    class MountServiceBinderListener// extends IMountServiceListener.Stub
    {
    public:
	void OnUsbMassStorageConnectionChanged(Boolean available);
	void OnStorageStateChanged(String path, String oldState, String newState);
    };

    /**
     * Binder listener for OBB action results.
     */
    class ObbActionListener// extends IObbActionListener.Stub
    {
    public:
	void OnObbResult(String filename, Int32 nonce, Int32 status);
	Int32 AddListener(IOnObbStateChangeListener* listener);
    };

    /**
     * Private class containing sender and receiver code for StorageEvents.
     */
    class ObbListenerDelegate
    {
    public:
	ObbListenerDelegate(IOnObbStateChangeListener* listener);
	IOnObbStateChangeListener* getListener();
	void SendObbStateChanged(String path, Int32 state);
    };

    /**
     * Message sent during an OBB status change event.
     */
    class ObbStateChangedStorageEvent// extends StorageEvent
    {
    public:
	const String path;
	const Int32 state;
	ObbStateChangedStorageEvent(String path, Int32 state);
    };

    /**
     * Private base class for messages sent between the callback thread
     * and the target looper handler.
     */
    class StorageEvent
    {
    public:
	static const Int32 EVENT_UMS_CONNECTION_CHANGED = 1;
        static const Int32 EVENT_STORAGE_STATE_CHANGED = 2;
        static const Int32 EVENT_OBB_STATE_CHANGED = 3;

        StorageEvent(Int32 what);
//        public Message getMessage();
    private:
//        private Message mMessage;
    };

    /**
     * Message sent on a USB mass storage connection change.
     */
    class UmsConnectionChangedStorageEvent: public StorageEvent
    {
    public:
	Boolean available;
	UmsConnectionChangedStorageEvent(Boolean a);
    };

    /**
     * Message sent on volume state change.
     */
    class StorageStateChangedStorageEvent: public StorageEvent
    {
    public:
	String path;
	String oldState;
	String newState;
	StorageStateChangedStorageEvent(String p, String oldS, String newS);
    };

    /**
     * Private class containing sender and receiver code for StorageEvents.
     */
    class ListenerDelegate
    {
    public:
//        final StorageEventListener mStorageEventListener;
//        private final Handler mHandler;
//	ListenerDelegate(StorageEventListener listener);
//	StorageEventListener getListener();
	void SendShareAvailabilityChanged(Boolean available);
	void SendStorageStateChanged(String path, String oldState, String newState);
    };

    CARAPI GetNextNonce(
        /* [out] */ Int32 * pPath);

    CARAPI RegisterListener(
        /* [in] */ IStorageEventListener * pListener);

    CARAPI UnregisterListener(
        /* [in] */ IStorageEventListener * pListener);

    CARAPI EnableUsbMassStorage();

    CARAPI DisableUsbMassStorage();

    CARAPI IsUsbMassStorageConnected(
        /* [out] */ Boolean * pConnect);

    CARAPI IsUsbMassStorageEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI IsUsbMassStorageAllEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI MountObb(
        /* [in] */ CString filename,
        /* [in] */ CString key,
        /* [in] */ IOnObbStateChangeListener * pListener,
        /* [out] */ Boolean * pMounted);

    CARAPI UnmountObb(
        /* [in] */ CString filename,
        /* [in] */ Boolean force,
        /* [in] */ IOnObbStateChangeListener * pListener,
        /* [out] */ Boolean * pMounted);

    CARAPI IsObbMounted(
        /* [in] */ CString filename,
        /* [out] */ Boolean * pMounted);

    CARAPI GetMountedObbPath(
        /* [in] */ CString filename,
        /* [out] */ CString * pObbpath);

    CARAPI GetVolumeState(
        /* [in] */ CString mountPoint,
        /* [out] */ CString * pState);

    CARAPI GetVolumeList(
        /* [out, callee] */ ArrayOf<IStorageVolume *> ** ppVolumelists);

    CARAPI GetVolumePaths(
        /* [out, callee] */ ArrayOf<String> ** ppVolumepaths);

    CARAPI GetVolumeFSLabel(
        /* [in] */ CString mountPoint,
        /* [out] */ CString * pLabel);

private:
    static const char* TAG;// = "StorageManager";

    /*
     * Our internal MountService binder reference
     */
    AutoPtr<IMountService> mMountService;

    /*
     * The looper target for callbacks
     */
//    Looper mTgtLooper;

    /*
     * Target listener for binder callbacks
     */
    MountServiceBinderListener mBinderListener;//=null;

    /*
     * List of our listeners
     */
//    private List<ListenerDelegate> mListeners = new ArrayList<ListenerDelegate>();

    /*
     * Next available nonce
     */
//    final private AtomicInteger mNextNonce = new AtomicInteger(0);
};

#endif // __CSTORAGEMANAGER_H__
