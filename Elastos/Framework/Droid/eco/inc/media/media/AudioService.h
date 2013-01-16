
#ifndef __AUDIOSERVICE_H__
#define __AUDIOSERVICE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>

/**
 * The implementation of the volume manager service.
 * <p>
 * This implementation focuses on delivering a responsive UI. Most methods are
 * asynchronous to external calls. For example, the task of setting a volume
 * will update our internal state, but in a separate thread will set the system
 * volume and later persist to the database. Similarly, setting the ringer mode
 * will update the state and broadcast a change and in a separate thread later
 * persist the ringer mode.
 *
 * @hide
 */
class AudioService
{
public:
    ///////////////////////////////////////////////////////////////////////////
    // Construction
    ///////////////////////////////////////////////////////////////////////////

    AudioService();

    /** @hide */
    AudioService(
        /* [in] */ IContext* context);

private:
    CARAPI_(void) CreateAudioSystemThread();

    /** Waits for the volume handler to be created by the other thread. */
    CARAPI_(void) WaitForAudioHandlerCreation();

    CARAPI_(void) CreateStreamStates();

    CARAPI_(void) ReadPersistedSettings();

    CARAPI_(void) SetStreamVolumeIndex(
        /* [in] */ Int32 stream, 
        /* [in] */ Int32 index);

    CARAPI_(Int32) RescaleIndex(
        /* [in] */ Int32 index, 
        /* [in] */ Int32 srcStream, 
        /* [in] */ Int32 dstStream);

    CARAPI_(void) SendVolumeUpdate(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 oldIndex, 
        /* [in] */ Int32 index);

    /**
     * Sets the stream state's index, and posts a message to set system volume.
     * This will not call out to the UI. Assumes a valid stream type.
     *
     * @param streamType Type of the stream
     * @param index Desired volume index of the stream
     * @param force If TRUE, set the volume even if the desired volume is same
     * as the current volume.
     * @param lastAudible If TRUE, stores new index as last audible one
     */
    CARAPI_(void) SetStreamVolumeInt(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 index, 
        /* [in] */ Boolean force, 
        /* [in] */ Boolean lastAudible);

    CARAPI_(void) SetRingerModeInt(
        /* [in] */ Int32 ringerMode, 
        /* [in] */ Boolean persist);

public:
    ///////////////////////////////////////////////////////////////////////////
    // IPC methods
    ///////////////////////////////////////////////////////////////////////////

    /** @see AudioManager#adjustVolume(Int32, Int32) */
    virtual CARAPI AdjustVolume(
        /* [in] */ Int32 direction, 
        /* [in] */ Int32 flags);

    /** @see AudioManager#adjustVolume(Int32, Int32, Int32) */
    virtual CARAPI AdjustSuggestedStreamVolume(
        /* [in] */ Int32 direction, 
        /* [in] */ Int32 suggestedStreamType, 
        /* [in] */ Int32 flags);

    /** @see AudioManager#adjustStreamVolume(Int32, Int32, Int32) */
    virtual CARAPI AdjustStreamVolume(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 direction, 
        /* [in] */ Int32 flags);

    /** @see AudioManager#setStreamVolume(Int32, Int32, Int32) */
    virtual CARAPI SetStreamVolume(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 index, 
        /* [in] */ Int32 flags);

    /** @see AudioManager#setStreamSolo(Int32, Boolean) */
    virtual CARAPI SetStreamSolo(
        /* [in] */ Int32 streamType, 
        /* [in] */ Boolean state, 
        /* [in] */ IBinder* cb);

    /** @see AudioManager#setStreamMute(Int32, Boolean) */
    virtual CARAPI SetStreamMute(
        /* [in] */ Int32 streamType, 
        /* [in] */ Boolean state, 
        /* [in] */ IBinder* cb);

    /** @see AudioManager#getStreamVolume(Int32) */
    virtual CARAPI_(Int32) GetStreamVolume(
        /* [in] */ Int32 streamType);

    /** @see AudioManager#getStreamMaxVolume(Int32) */
    virtual CARAPI_(Int32) GetStreamMaxVolume(
        /* [in] */ Int32 streamType);

    /** @see AudioManager#getRingerMode() */
    virtual CARAPI_(Int32) GetRingerMode();

    /** @see AudioManager#setRingerMode(Int32) */
    virtual CARAPI SetRingerMode(
        /* [in] */ Int32 ringerMode);

    /** @see AudioManager#shouldVibrate(Int32) */
    virtual CARAPI_(Boolean) ShouldVibrate(
        /* [in] */ Int32 vibrateType);

    /** @see AudioManager#getVibrateSetting(Int32) */
    virtual CARAPI_(Int32) GetVibrateSetting(
        /* [in] */ Int32 vibrateType);

    /** @see AudioManager#setVibrateSetting(Int32, Int32) */
    virtual CARAPI SetVibrateSetting(
        /* [in] */ Int32 vibrateType, 
        /* [in] */ Int32 vibrateSetting);

    /**
     * @see #setVibrateSetting(Int32, Int32)
     */
    static CARAPI_(Int32) GetValueForVibrateSetting(
        /* [in] */ Int32 existingValue, 
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting);

    class SetModeDeathHandler : public IBinderDeathRecipient, public ElRefBase
    {
    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        Int32 mMode;// = AudioSystem.MODE_NORMAL; // Current mode set by this client

    public:
        SetModeDeathHandler(
            /* [in] */ IBinder* cb);

        CARAPI BinderDied();

        CARAPI SetMode(
            /* [in] */ Int32 mode);

        CARAPI_(Int32) GetMode();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    class ScoClient : public IBinderDeathRecipient, public ElRefBase 
    {
    public:
        ScoClient(
            /* [in] */ IBinder* cb);

        CARAPI BinderDied();

        CARAPI_(void) IncCount();

        CARAPI_(void) DecCount();

        CARAPI_(void) ClearCount(
            /* [in] */ Boolean stopSco);

        CARAPI_(Int32) GetCount();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

        CARAPI_(Int32) TotalCount();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        Int32 mStartcount; // number of SCO connections started by this client

        CARAPI_(void) RequestScoState(
            /* [in] */ Int32 state);
    };

    /** @see AudioManager#setMode(Int32) */
    virtual CARAPI SetMode(
        /* [in] */ Int32 mode, 
        /* [in] */ IBinder* cb);

    /** @see AudioManager#getMode() */
    virtual CARAPI_(Int32) GetMode();

    /** @see AudioManager#playSoundEffect(Int32) */
    virtual CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType);

    /** @see AudioManager#playSoundEffect(Int32, Float) */
    virtual CARAPI PlaySoundEffectVolume(
        /* [in] */ Int32 effectType, 
        /* [in] */ Float volume);

    /**
     * Loads samples into the soundpool.
     * This method must be called at when sound effects are enabled
     */
    virtual CARAPI_(Boolean) LoadSoundEffects();

    /**
     *  Unloads samples from the sound pool.
     *  This method can be called to free some memory when
     *  sound effects are disabled.
     */
    virtual CARAPI UnloadSoundEffects();

    /** @see AudioManager#reloadAudioSettings() */
    virtual CARAPI ReloadAudioSettings();

    /** @see AudioManager#setSpeakerphoneOn() */
    virtual CARAPI SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    /** @see AudioManager#isSpeakerphoneOn() */
    virtual CARAPI_(Boolean) IsSpeakerphoneOn();

    /** @see AudioManager#setBluetoothScoOn() */
    virtual CARAPI SetBluetoothScoOn(
        /* [in] */ Boolean on);

    /** @see AudioManager#isBluetoothScoOn() */
    virtual CARAPI_(Boolean) IsBluetoothScoOn();

    /** @see AudioManager#startBluetoothSco() */
    virtual CARAPI StartBluetoothSco(
        /* [in] */ IBinder* cb);

    /** @see AudioManager#stopBluetoothSco() */
    virtual CARAPI StopBluetoothSco(
        /* [in] */ IBinder* cb);

    virtual CARAPI_(ScoClient*) GetScoClient(
        /* [in] */ IBinder* cb);

    virtual CARAPI ClearAllScoClients();

    virtual CARAPI_(Boolean) IsStreamAffectedByRingerMode(
        /* [in] */ Int32 streamType);

    virtual CARAPI_(Boolean) IsStreamAffectedByMute(
        /* [in] */ Int32 streamType);

private:

    //BluetoothHeadset.ServiceListener mBluetoothHeadsetServiceListener;

    ///////////////////////////////////////////////////////////////////////////
    // Internal methods
    ///////////////////////////////////////////////////////////////////////////

    /**
     * Checks if the adjustment should change ringer mode instead of just
     * adjusting volume. If so, this will set the proper ringer mode and volume
     * indices on the stream states.
     */
    CARAPI_(Boolean) CheckForRingerModeChange(
        /* [in] */ Int32 oldIndex, 
        /* [in] */ Int32 direction);

    CARAPI_(Boolean) IsStreamMutedByRingerMode(
        /* [in] */ Int32 streamType);

    CARAPI_(void) EnsureValidDirection(
        /* [in] */ Int32 direction);

    CARAPI_(void) EnsureValidStreamType(
        /* [in] */ Int32 streamType);

    CARAPI_(Int32) GetActiveStreamType(
        /* [in] */ Int32 suggestedStreamType);

    CARAPI_(void) BroadcastRingerMode();

    CARAPI_(void) BroadcastVibrateSetting(
        /* [in] */ Int32 vibrateType);

    // Message helper methods
    static CARAPI_(Int32) GetMsg(
        /* [in] */ Int32 baseMsg, 
        /* [in] */ Int32 streamType);

    static CARAPI_(Int32) GetMsgBase(
        /* [in] */ Int32 msg);

    static CARAPI_(void) SendMsg(
        /* [in] */ IHandler* handler, 
        /* [in] */ Int32 baseMsg, 
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 existingMsgPolicy, 
        /* [in] */ Int32 arg1, 
        /* [in] */ Int32 arg2, 
        /* [in] */ IInterface* obj, 
        /* [in] */ Int32 delay);

    CARAPI_(Boolean) CheckAudioSettingsPermission(
        /* [in] */ String method);


    ///////////////////////////////////////////////////////////////////////////
    // Inner classes
    ///////////////////////////////////////////////////////////////////////////

    class VolumeStreamState
    {
    public:
        VolumeStreamState(
            /* [in] */ String settingName, 
            /* [in] */ Int32 streamType);

        virtual CARAPI_(void) SetVolumeIndexSettingName(
            /* [in] */ String settingName);

        virtual CARAPI_(Boolean) AdjustIndex(
            /* [in] */ Int32 deltaIndex);

        virtual CARAPI_(Boolean) SetIndex(
            /* [in] */ Int32 index, 
            /* [in] */ Boolean lastAudible);

        virtual CARAPI_(void) SetLastAudibleIndex(
            /* [in] */ Int32 index);

        virtual CARAPI_(void) AdjustLastAudibleIndex(
            /* [in] */ Int32 deltaIndex);

        virtual CARAPI_(Int32) GetMaxIndex();

        virtual CARAPI_(void) Mute(
            /* [in] */ IBinder* cb, 
            /* [in] */ Boolean state);

        virtual CARAPI_(Int32) GetValidIndex(
            /* [in] */ Int32 index);

        virtual CARAPI_(Int32) MuteCount();

    //private:

        class VolumeDeathHandler : public IBinderDeathRecipient, public ElRefBase
        {
        public:
            CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            VolumeDeathHandler(
                /* [in] */ IBinder* cb);

            virtual CARAPI_(void) Mute(
                /* [in] */ Boolean state);

            virtual CARAPI BinderDied();
            
            Int32 mMuteCount; // Number of active mutes for this client
        
            AutoPtr<IBinder> mICallback; // To be notified of client's death
            
        };

        CARAPI_(VolumeDeathHandler*) GetDeathHandler(
            /* [in] */ IBinder* cb, 
            /* [in] */ Boolean state);

    private:
        

        String mVolumeIndexSettingName;
        String mLastAudibleVolumeIndexSettingName;
        Int32 mIndexMax;
        
    public:
        Int32 mStreamType;
        Int32 mIndex;
        Int32 mLastAudibleIndex;
        ArrayOf<VolumeDeathHandler*>* mDeathHandlers; //handles mute/solo requests client death
    };

    /** Thread that handles native AudioSystem control. */
    class AudioSystemThread : public IThread, public ElRefBase
    {
    public:
        AudioSystemThread();

        virtual CARAPI Run();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    /** Handles internal volume messages in separate volume thread. */
    class AudioHandler : public IHandler, public ElRefBase
    {
    private:
        CARAPI_(void) SetSystemVolume(
            /* [in] */ VolumeStreamState* streamState);

        CARAPI_(void) PersistVolume(
            /* [in] */ VolumeStreamState* streamState, 
            /* [in] */ Boolean current, 
            /* [in] */ Boolean lastAudible);

        CARAPI_(void) PersistRingerMode();

        CARAPI_(void) PersistVibrateSetting();

        CARAPI_(void) PlaySoundEffect(
            /* [in] */ Int32 effectType, 
            /* [in] */ Int32 volume);

        //CARAPI_(void) CleanupPlayer(
        //    /* [in] */ MediaPlayer mp);
    
    public:
        //virtual CARAPI HandleMessage(
        //    /* [in] */ IMessage* msg);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    class SettingsObserver : public IContentObserver, public ElRefBase
    {
    public:
        SettingsObserver();

        virtual CARAPI OnChange(
            /* [in] */ Boolean selfChange);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    CARAPI_(void) MakeA2dpDeviceAvailable(
        /* [in] */ String address);

    CARAPI_(void) MakeA2dpDeviceUnavailableNow(
        /* [in] */ String address);

    CARAPI_(void) MakeA2dpDeviceUnavailableLater(
        /* [in] */ String address);

    CARAPI_(void) CancelA2dpDeviceTimeout();

    CARAPI_(Boolean) HasScheduledA2dpDockTimeout();

    /* cache of the address of the last dock the device was connected to */
    String mDockAddress;

    /**
     * Receiver for misc intent broadcasts the Phone app cares about.
     */
    class AudioServiceBroadcastReceiver : public IBroadcastReceiver, public ElRefBase 
    {
    public:
        virtual CARAPI OnReceive(
            /* [in] */ IContext* context, 
            /* [in] */ IIntent* intent);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    //==========================================================================================
    // AudioFocus
    //==========================================================================================

    /* constant to identify focus stack entry that is used to hold the focus while the phone
     * is ringing or during a call
     */
    static String IN_VOICE_COMM_FOCUS_ID;// = "AudioFocus_For_Phone_Ring_And_Calls";

    static AutoPtr<IInterface> mAudioFocusLock;// = new Object();

    static AutoPtr<IInterface> mRingingLock;// = new Object();

    //PhoneStateListener* mPhoneStateListener;// = new PhoneStateListener();

    CARAPI_(void) NotifyTopOfAudioFocusStack();

    class FocusStackEntry 
    {
    public:
        Int32 mStreamType;// = -1;// no stream type
        Boolean mIsTransportControlReceiver;// = FALSE;
        AutoPtr<IAudioFocusDispatcher> mFocusDispatcher;// = NULL;
        AutoPtr<IBinder> mSourceRef;// = NULL;
        String mClientId;
        Int32 mFocusChangeType;

        FocusStackEntry();

        FocusStackEntry(
            /* [in] */ Int32 streamType, 
            /* [in] */ Int32 duration, 
            /* [in] */ Boolean isTransportControlReceiver,
            /* [in] */ IAudioFocusDispatcher* afl, 
            /* [in] */ IBinder* source, 
            /* [in] */ String id);
    };

    //Stack<FocusStackEntry> mFocusStack = new Stack<FocusStackEntry>();

    /**
     * Helper function:
     * Display in the log the current entries in the audio focus stack
     */
    //CARAPI_(void) DumpFocusStack(
    //    /* [in] */ PrintWriter pw);

    /**
     * Helper function:
     * Remove a focus listener from the focus stack.
     * @param focusListenerToRemove the focus listener
     * @param signal if TRUE and the listener was at the top of the focus stack, i.e. it was holding
     *   focus, notify the next item in the stack it gained focus.
     */
    CARAPI_(void) RemoveFocusStackEntry(
        /* [in] */ String clientToRemove, 
        /* [in] */ Boolean signal);

    /**
     * Helper function:
     * Remove focus listeners from the focus stack for a particular client.
     */
    CARAPI_(void) RemoveFocusStackEntryForClient(
        /* [in] */ IBinder* cb);

    /**
     * Helper function:
     * Returns TRUE if the system is in a state where the focus can be reevaluated, FALSE otherwise.
     */
    CARAPI_(Boolean) CanReassignAudioFocus();

    /**
     * Inner class to monitor audio focus client deaths, and remove them from the audio focus
     * stack if necessary.
     */
    class AudioFocusDeathHandler : public IBinderDeathRecipient, public ElRefBase
    {
    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death

    public:
        AudioFocusDeathHandler(
            /* [in] */ IBinder* cb);

        virtual CARAPI BinderDied();

        virtual CARAPI_(AutoPtr<IBinder>) GetBinder();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };


public:
    /** @see AudioManager#requestAudioFocus(IAudioFocusDispatcher, Int32, Int32) */
    virtual CARAPI_(Int32) RequestAudioFocus(
        /* [in] */ Int32 mainStreamType, 
        /* [in] */ Int32 focusChangeHint, 
        /* [in] */ IBinder* cb,
        /* [in] */ IAudioFocusDispatcher* fd, 
        /* [in] */ String clientId);

    /** @see AudioManager#abandonAudioFocus(IAudioFocusDispatcher) */
    virtual CARAPI_(Int32) AbandonAudioFocus(
        /* [in] */ IAudioFocusDispatcher* fl, 
        /* [in] */ String clientId);

    virtual CARAPI UnregisterAudioFocusClient(
        /* [in] */ String clientId);

    //==========================================================================================
    // RemoteControl
    //==========================================================================================
    /**
     * Receiver for media button intents. Handles the dispatching of the media button event
     * to one of the registered listeners, or if there was none, resumes the intent broadcast
     * to the rest of the system.
     */
    class MediaButtonBroadcastReceiver : public IBroadcastReceiver, public ElRefBase 
    {
    public:
        virtual CARAPI OnReceive(
            /* [in] */ IContext* context, 
            /* [in] */ IIntent* intent);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

private:
    class RemoteControlStackEntry
    {
    public:
        AutoPtr<IComponentName> mReceiverComponent;// always non NULL
        // TODO implement registration expiration?
        //public Int32 mRegistrationTime;

        RemoteControlStackEntry();

        RemoteControlStackEntry(
            /* [in] */ IComponentName* r);
    };

    //Stack<RemoteControlStackEntry> mRCStack;// = new Stack<RemoteControlStackEntry>();

    /**
     * Helper function:
     * Display in the log the current entries in the remote control focus stack
     */
    //CARAPI_(void) DumpRCStack(
    //    /* [in] */ PrintWriter pw);

    /**
     * Helper function:
     * Set the new remote control receiver at the top of the RC focus stack
     */
    CARAPI_(void) PushMediaButtonReceiver(
        /* [in] */ IComponentName* newReceiver);

    /**
     * Helper function:
     * Remove the remote control receiver from the RC focus stack
     */
    CARAPI_(void) RemoveMediaButtonReceiver(
        /* [in] */ IComponentName* newReceiver);

public:
    /** see AudioManager.registerMediaButtonEventReceiver(ComponentName eventReceiver) */
    virtual CARAPI RegisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

    /** see AudioManager.unregisterMediaButtonEventReceiver(ComponentName eventReceiver) */
    virtual CARAPI UnregisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

protected:
    //virtual CARAPI_(void) Dump(
    //    /* [in] */ FileDescriptor fd, 
    //    /* [in] */ PrintWriter pw, 
    //    /* [in] */ String[] args);

private:
    static String TAG;// = "AudioService";

    /** How long to delay before persisting a change in volume/ringer mode. */
    static const Int32 PERSIST_DELAY = 3000;

    AutoPtr<IContext> mContext;
    AutoPtr<IContentResolver> mContentResolver;


    /** The UI */
    //VolumePanel mVolumePanel;

    // sendMsg() flags
    /** Used when a message should be shared across all stream types. */
    static const Int32 SHARED_MSG = -1;
    /** If the msg is already queued, replace it with this one. */
    static const Int32 SENDMSG_REPLACE = 0;
    /** If the msg is already queued, ignore this one and leave the old. */
    static const Int32 SENDMSG_NOOP = 1;
    /** If the msg is already queued, queue this one and leave the old. */
    static const Int32 SENDMSG_QUEUE = 2;

    // AudioHandler message.whats
    static const Int32 MSG_SET_SYSTEM_VOLUME = 0;
    static const Int32 MSG_PERSIST_VOLUME = 1;
    static const Int32 MSG_PERSIST_RINGER_MODE = 3;
    static const Int32 MSG_PERSIST_VIBRATE_SETTING = 4;
    static const Int32 MSG_MEDIA_SERVER_DIED = 5;
    static const Int32 MSG_MEDIA_SERVER_STARTED = 6;
    static const Int32 MSG_PLAY_SOUND_EFFECT = 7;
    static const Int32 MSG_BTA2DP_DOCK_TIMEOUT = 8;

    static const Int32 BTA2DP_DOCK_TIMEOUT_MILLIS = 8000;

    /** @see AudioSystemThread */
    AudioSystemThread* mAudioSystemThread;
    /** @see AudioHandler */
    AudioHandler* mAudioHandler;
    /** @see VolumeStreamState */
    ArrayOf<VolumeStreamState*>* mStreamStates;
    SettingsObserver* mSettingsObserver;

    Int32 mMode;
    AutoPtr<IInterface> mSettingsLock;// = new Object();
    Boolean mMediaServerOk;

    //SoundPool mSoundPool;
    AutoPtr<IInterface> mSoundEffectsLock;// = new Object();
    static const Int32 NUM_SOUNDPOOL_CHANNELS = 4;
    static const Int32 SOUND_EFFECT_VOLUME = 1000;

    /* Sound effect file names  */
    static String SOUND_EFFECTS_PATH;// = "/media/audio/ui/";
    static ArrayOf<String>* SOUND_EFFECT_FILES;// = new String[] {
        /*"Effect_Tick.ogg",
        "KeypressStandard.ogg",
        "KeypressSpacebar.ogg",
        "KeypressDelete.ogg",
        "KeypressReturn.ogg"
    };*/

    /* Sound effect file name mapping sound effect id (AudioManager.FX_xxx) to
     * file index in SOUND_EFFECT_FILES[] (first column) and indicating if effect
     * uses soundpool (second column) */
    ArrayOf<ArrayOf<Int32>*>* SOUND_EFFECT_FILES_MAP;// = new Int32[][] {
    //    {0, -1},  // FX_KEY_CLICK
    //    {0, -1},  // FX_FOCUS_NAVIGATION_UP
    //    {0, -1},  // FX_FOCUS_NAVIGATION_DOWN
    //    {0, -1},  // FX_FOCUS_NAVIGATION_LEFT
    //    {0, -1},  // FX_FOCUS_NAVIGATION_RIGHT
    //    {1, -1},  // FX_KEYPRESS_STANDARD
    //    {2, -1},  // FX_KEYPRESS_SPACEBAR
    //    {3, -1},  // FX_FOCUS_DELETE
    //    {4, -1}   // FX_FOCUS_RETURN
    //};

   /** @hide Maximum volume index values for audio streams */
    ArrayOf<Int32>* MAX_STREAM_VOLUME;// = new Int32[] {
    //    5,  // STREAM_VOICE_CALL
    //    7,  // STREAM_SYSTEM
    //    7,  // STREAM_RING
    //    15, // STREAM_MUSIC
    //    7,  // STREAM_ALARM
    //    7,  // STREAM_NOTIFICATION
    //    15, // STREAM_BLUETOOTH_SCO
    //    7,  // STREAM_SYSTEM_ENFORCED
    //    15, // STREAM_DTMF
    //    15  // STREAM_TTS
    //};

    /* STREAM_VOLUME_ALIAS[] indicates for each stream if it uses the volume settings
     * of another stream: This avoids multiplying the volume settings for hidden
     * stream types that follow other stream behavior for volume settings
     * NOTE: do not create loops in aliases! */
    ArrayOf<Int32>* STREAM_VOLUME_ALIAS;// = new Int32[] {
    //    AudioSystem.STREAM_VOICE_CALL,  // STREAM_VOICE_CALL
    //    AudioSystem.STREAM_SYSTEM,  // STREAM_SYSTEM
    //    AudioSystem.STREAM_RING,  // STREAM_RING
    //    AudioSystem.STREAM_MUSIC, // STREAM_MUSIC
    //    AudioSystem.STREAM_ALARM,  // STREAM_ALARM
    //    AudioSystem.STREAM_NOTIFICATION,  // STREAM_NOTIFICATION
    //    AudioSystem.STREAM_BLUETOOTH_SCO, // STREAM_BLUETOOTH_SCO
    //    AudioSystem.STREAM_SYSTEM,  // STREAM_SYSTEM_ENFORCED
    //    AudioSystem.STREAM_VOICE_CALL, // STREAM_DTMF
    //    AudioSystem.STREAM_MUSIC  // STREAM_TTS
    //};

    /*AudioSystem.ErrorCallback mAudioSystemCallback; = new AudioSystem.ErrorCallback() {
        public void onError(Int32 error) {
            switch (error) {
            case AudioSystem.AUDIO_STATUS_SERVER_DIED:
                if (mMediaServerOk) {
                    sendMsg(mAudioHandler, MSG_MEDIA_SERVER_DIED, SHARED_MSG, SENDMSG_NOOP, 0, 0,
                            NULL, 1500);
                    mMediaServerOk = FALSE;
                }
                break;
            case AudioSystem.AUDIO_STATUS_OK:
                if (!mMediaServerOk) {
                    sendMsg(mAudioHandler, MSG_MEDIA_SERVER_STARTED, SHARED_MSG, SENDMSG_NOOP, 0, 0,
                            NULL, 0);
                    mMediaServerOk = TRUE;
                }
                break;
            default:
                break;
            }
       }
    };*/

    /**
     * Current ringer mode from one of {@link AudioManager#RINGER_MODE_NORMAL},
     * {@link AudioManager#RINGER_MODE_SILENT}, or
     * {@link AudioManager#RINGER_MODE_VIBRATE}.
     */
    Int32 mRingerMode;

    /** @see System#MODE_RINGER_STREAMS_AFFECTED */
    Int32 mRingerModeAffectedStreams;

    // Streams currently muted by ringer mode
    Int32 mRingerModeMutedStreams;

    /** @see System#MUTE_STREAMS_AFFECTED */
    Int32 mMuteAffectedStreams;

    /**
     * Has multiple bits per vibrate type to indicate the type's vibrate
     * setting. See {@link #setVibrateSetting(Int32, Int32)}.
     * <p>
     * NOTE: This is not the final decision of whether vibrate is on/off for the
     * type since it depends on the ringer mode. See {@link #shouldVibrate(Int32)}.
     */
    Int32 mVibrateSetting;

    /** @see System#NOTIFICATIONS_USE_RING_VOLUME */
    Int32 mNotificationsUseRingVolume;

    // Broadcast receiver for device connections intent broadcasts
    AutoPtr<IBroadcastReceiver> mReceiver;// = new AudioServiceBroadcastReceiver();

    //  Broadcast receiver for media button broadcasts (separate from mReceiver to
    //  independently change its priority)
    AutoPtr<IBroadcastReceiver> mMediaButtonReceiver;// = new MediaButtonBroadcastReceiver();

    // Used to alter media button redirection when the phone is ringing.
    Boolean mIsRinging;// = FALSE;

    // Devices currently connected
    HashMap <Int32, String> mConnectedDevices;// = new HashMap <Integer, String>();

    // Forced device usage for communications
    Int32 mForcedUseForComm;

    // List of binder death handlers for setMode() client processes.
    // The last process to have called setMode() is at the top of the list.
    ArrayOf<AudioService::SetModeDeathHandler*>* mSetModeDeathHandlers;// = new ArrayList <SetModeDeathHandler>();

    // List of clients having issued a SCO start request
    ArrayOf<AudioService::ScoClient*>* mScoClients;// = new ArrayList <ScoClient>();

    // BluetoothHeadset API to control SCO connection
    //BluetoothHeadset mBluetoothHeadset;

    // Bluetooth headset connection state
    Boolean mBluetoothHeadsetConnected;

};

#endif
