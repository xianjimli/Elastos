
#ifndef __CAUDIOSERVICE_H__
#define __CAUDIOSERVICE_H__

#include "ext/frameworkext.h"
#include "_CAudioService.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/Stack.h>

#include <elastos/Thread.h>


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
CarClass(CAudioService)
{
public:
    ///////////////////////////////////////////////////////////////////////////
    // Inner classes
    ///////////////////////////////////////////////////////////////////////////

    class VolumeStreamState : public ElRefBase
    {
        friend class CAudioService;

    private:
        class VolumeDeathHandler
            : public ElRefBase
            , public IBinderDeathRecipient
        {
            friend class VolumeStreamState;
            friend class CAudioService;

        public:
            VolumeDeathHandler(
                /* [in] */ IBinder* cb,
                /* [in] */ VolumeStreamState* host);

            CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            CARAPI_(void) Mute(
                /* [in] */ Boolean state);

            CARAPI BinderDied();

        private:
            AutoPtr<IBinder> mICallback; // To be notified of client's death
            Int32 mMuteCount; // Number of active mutes for this client
            VolumeStreamState* mHost;
        };

    public:
        VolumeStreamState(
            /* [in] */ const String& settingName,
            /* [in] */ Int32 streamType,
            /* [in] */ CAudioService* host);

        CARAPI_(void) SetVolumeIndexSettingName(
            /* [in] */ const String& settingName);

        CARAPI_(Boolean) AdjustIndex(
            /* [in] */ Int32 deltaIndex);

        CARAPI_(Boolean) SetIndex(
            /* [in] */ Int32 index,
            /* [in] */ Boolean lastAudible);

        CARAPI_(void) SetLastAudibleIndex(
            /* [in] */ Int32 index);

        CARAPI_(void) AdjustLastAudibleIndex(
            /* [in] */ Int32 deltaIndex);

        CARAPI_(Int32) GetMaxIndex();

        CARAPI_(void) Mute(
            /* [in] */ IBinder* cb,
            /* [in] */ Boolean state);

    private:
        CARAPI_(Int32) GetValidIndex(
            /* [in] */ Int32 index);

        CARAPI_(Int32) MuteCount();

        CARAPI_(AutoPtr<VolumeDeathHandler>) GetDeathHandler(
            /* [in] */ IBinder* cb,
            /* [in] */ Boolean state);

    private:
        Int32 mStreamType;

        String mVolumeIndexSettingName;
        String mLastAudibleVolumeIndexSettingName;
        Int32 mIndexMax;
        Int32 mIndex;
        Int32 mLastAudibleIndex;
        List< AutoPtr<VolumeDeathHandler> > mDeathHandlers; //handles mute/solo requests client death
        Mutex mDeathHandlersLock;
        CAudioService* mHost;
    };

private:
    class SetModeDeathHandler
        : public ElRefBase
        , public IBinderDeathRecipient
    {
    public:
        SetModeDeathHandler(
            /* [in] */ IBinder* cb,
            /* [in] */ CAudioService* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI BinderDied();

        CARAPI_(void) SetMode(
            /* [in] */ Int32 mode);

        CARAPI_(Int32) GetMode();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        Int32 mMode;
        CAudioService* mHost;
    };

    class ScoClient
        : public ElRefBase
        , public IBinderDeathRecipient
    {
    public:
        ScoClient(
            /* [in] */ IBinder* cb,
            /* [in] */ CAudioService* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI BinderDied();

        CARAPI_(void) IncCount();

        CARAPI_(void) DecCount();

        CARAPI_(void) ClearCount(
            /* [in] */ Boolean stopSco);

        CARAPI_(Int32) GetCount();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

        CARAPI_(Int32) TotalCount();

    private:
        CARAPI_(void) RequestScoState(
            /* [in] */ Int32 state);

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        Int32 mStartcount; // number of SCO connections started by this client
        CAudioService* mHost;
    };

    /** Thread that handles native AudioSystem control. */
    class AudioSystemThread
        : public ElRefBase
        , public IThread
        , public Thread
    {
    public:
        AudioSystemThread();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Run();
    };

    /** Handles internal volume messages in separate volume thread. */
    class AudioHandler
        : public ElRefBase
        , public IApartment
    {
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

        CARAPI_(void) CleanupPlayer(
           /* [in] */ IMediaPlayer* mp);
    };

    class SettingsObserver
        : public ElRefBase
        , public IContentObserver
    {
    public:
        SettingsObserver();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);
    };

    /**
     * Receiver for misc intent broadcasts the Phone app cares about.
     */
    class AudioServiceBroadcastReceiver
        : public ElRefBase
        , public IBroadcastReceiver
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    };

    class FocusStackEntry : public ElRefBase
    {
    public:
        FocusStackEntry();

        FocusStackEntry(
            /* [in] */ Int32 streamType,
            /* [in] */ Int32 duration,
            /* [in] */ Boolean isTransportControlReceiver,
            /* [in] */ IAudioFocusDispatcher* afl,
            /* [in] */ IBinder* source,
            /* [in] */ const String& id);

    public:
        Int32 mStreamType;// no stream type
        Boolean mIsTransportControlReceiver;
        AutoPtr<IAudioFocusDispatcher> mFocusDispatcher;
        AutoPtr<IBinder> mSourceRef;
        String mClientId;
        Int32 mFocusChangeType;
    };

    /**
     * Inner class to monitor audio focus client deaths, and remove them from the audio focus
     * stack if necessary.
     */
    class AudioFocusDeathHandler
        : public ElRefBase
        , public IBinderDeathRecipient
    {
    public:
        AudioFocusDeathHandler(
            /* [in] */ IBinder* cb,
            /* [in] */ CAudioService* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI BinderDied();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        CAudioService* mHost;
    };

    //==========================================================================================
    // RemoteControl
    //==========================================================================================
    /**
     * Receiver for media button intents. Handles the dispatching of the media button event
     * to one of the registered listeners, or if there was none, resumes the intent broadcast
     * to the rest of the system.
     */
    class MediaButtonBroadcastReceiver
        : public ElRefBase
        , public IBroadcastReceiver
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    };

    class RemoteControlStackEntry : public ElRefBase
    {
    public:
        RemoteControlStackEntry();

        RemoteControlStackEntry(
            /* [in] */ IComponentName* r);

    public:
        AutoPtr<IComponentName> mReceiverComponent;// always non NULL
        // TODO implement registration expiration?
        //public Int32 mRegistrationTime;
    };

    class AudioSystemErrorCallback
        : public ElRefBase
        , public IAudioSystemErrorCallback
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnError(
            /* [in] */ Int32 error);
    };

public:
    CAudioService();

    ~CAudioService();

    CARAPI constructor(
        /* [in] */ IContext* context);

    ///////////////////////////////////////////////////////////////////////////
    // IPC methods
    ///////////////////////////////////////////////////////////////////////////

    /** @see AudioManager#adjustVolume(Int32, Int32) */
    CARAPI AdjustVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    /** @see AudioManager#adjustVolume(Int32, Int32, Int32) */
    CARAPI AdjustSuggestedStreamVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 suggestedStreamType,
        /* [in] */ Int32 flags);

    /** @see AudioManager#adjustStreamVolume(Int32, Int32, Int32) */
    CARAPI AdjustStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    /** @see AudioManager#setStreamVolume(Int32, Int32, Int32) */
    CARAPI SetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 index,
        /* [in] */ Int32 flags);

    /** @see AudioManager#setStreamSolo(Int32, Boolean) */
    CARAPI SetStreamSolo(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state,
        /* [in] */ IBinder* cb);

    /** @see AudioManager#setStreamMute(Int32, Boolean) */
    CARAPI SetStreamMute(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state,
        /* [in] */ IBinder* cb);

    /** @see AudioManager#getStreamVolume(Int32) */
    CARAPI GetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* volume);

    /** @see AudioManager#getStreamMaxVolume(Int32) */
    CARAPI GetStreamMaxVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* volume);

    /** @see AudioManager#getRingerMode() */
    CARAPI GetRingerMode(
        /* [out] */ Int32* mode);

    /** @see AudioManager#setRingerMode(Int32) */
    CARAPI SetRingerMode(
        /* [in] */ Int32 ringerMode);

    /** @see AudioManager#shouldVibrate(Int32) */
    CARAPI ShouldVibrate(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Boolean* vibrate);

    /** @see AudioManager#getVibrateSetting(Int32) */
    CARAPI GetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Int32* vibrateSetting);

    /** @see AudioManager#setVibrateSetting(Int32, Int32) */
    CARAPI SetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting);

    /**
     * @see #setVibrateSetting(Int32, Int32)
     */
    static CARAPI_(Int32) GetValueForVibrateSetting(
        /* [in] */ Int32 existingValue,
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting);

    /** @see AudioManager#setMode(Int32) */
    CARAPI SetMode(
        /* [in] */ Int32 mode,
        /* [in] */ IBinder* cb);

    /** @see AudioManager#getMode() */
    CARAPI GetMode(
        /* [out] */ Int32* mode);

    /** @see AudioManager#playSoundEffect(Int32) */
    CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType);

    /** @see AudioManager#playSoundEffect(Int32, Float) */
    CARAPI PlaySoundEffectVolume(
        /* [in] */ Int32 effectType,
        /* [in] */ Float volume);

    /**
     * Loads samples into the soundpool.
     * This method must be called at when sound effects are enabled
     */
    CARAPI LoadSoundEffects(
        /* [out] */ Boolean* succeeded);

    /**
     *  Unloads samples from the sound pool.
     *  This method can be called to free some memory when
     *  sound effects are disabled.
     */
    CARAPI UnloadSoundEffects();

    /** @see AudioManager#reloadAudioSettings() */
    CARAPI ReloadAudioSettings();

    /** @see AudioManager#setSpeakerphoneOn() */
    CARAPI SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    /** @see AudioManager#isSpeakerphoneOn() */
    CARAPI IsSpeakerphoneOn(
        /* [out] */ Boolean* isOn);

    /** @see AudioManager#setBluetoothScoOn() */
    CARAPI SetBluetoothScoOn(
        /* [in] */ Boolean on);

    /** @see AudioManager#isBluetoothScoOn() */
    CARAPI IsBluetoothScoOn(
        /* [out] */ Boolean* isOn);

    /** @see AudioManager#startBluetoothSco() */
    CARAPI StartBluetoothSco(
        /* [in] */ IBinder* cb);

    /** @see AudioManager#stopBluetoothSco() */
    CARAPI StopBluetoothSco(
        /* [in] */ IBinder* cb);

    CARAPI_(AutoPtr<ScoClient>) GetScoClient(
        /* [in] */ IBinder* cb);

    CARAPI ClearAllScoClients();

    CARAPI IsStreamAffectedByRingerMode(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean* result);

    CARAPI IsStreamAffectedByMute(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean* result);

    /** @see AudioManager#requestAudioFocus(IAudioFocusDispatcher, Int32, Int32) */
    CARAPI RequestAudioFocus(
        /* [in] */ Int32 mainStreamType,
        /* [in] */ Int32 durationHint,
        /* [in] */ IBinder* cb,
        /* [in] */ IAudioFocusDispatcher* fd,
        /* [in] */ const String& clientId,
        /* [out] */ Int32* result);

    /** @see AudioManager#abandonAudioFocus(IAudioFocusDispatcher) */
    CARAPI AbandonAudioFocus(
        /* [in] */ IAudioFocusDispatcher* fl,
        /* [in] */ const String& clientId,
        /* [out] */ Int32* result);

    CARAPI UnregisterAudioFocusClient(
        /* [in] */ const String& clientId);

    /** see AudioManager.registerMediaButtonEventReceiver(ComponentName eventReceiver) */
    CARAPI RegisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

    /** see AudioManager.unregisterMediaButtonEventReceiver(ComponentName eventReceiver) */
    CARAPI UnregisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

    // CARAPI SetVolumeIndexSettingName(
    //     /* [in] */ const String& settingName);

    // CARAPI AdjustIndex(
    //     /* [in] */ Int32 deltaIndex);

    // CARAPI SetIndex(
    //     /* [in] */ Int32 index,
    //     /* [in] */ Boolean lastAudible);

    // CARAPI SetLastAudibleIndex(
    //     /* [in] */ Int32 index);

    // CARAPI AdjustLastAudibleIndex(
    //     /* [in] */ Int32 deltaIndex);

    // CARAPI GetMaxIndex(
    //     /* [out] */ Int32 * pIndex);

    // CARAPI Mute(
    //     /* [in] */ IBinder * pCb,
    //     /* [in] */ Boolean state);

    CARAPI_(void) Wait();

    CARAPI_(void) NotifyAll();

protected:
    //virtual CARAPI_(void) Dump(
    //    /* [in] */ FileDescriptor fd,
    //    /* [in] */ PrintWriter pw,
    //    /* [in] */ String[] args);

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

    CARAPI EnsureValidDirection(
        /* [in] */ Int32 direction);

    CARAPI EnsureValidStreamType(
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

    static CARAPI SendMsg(
        /* [in] */ IApartment* handler,
        /* [in] */ Int32 baseMsg,
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 existingMsgPolicy,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 delay);

    /* package */ CARAPI_(Boolean) CheckAudioSettingsPermission(
        /* [in] */ const String& method);

    CARAPI_(void) MakeA2dpDeviceAvailable(
        /* [in] */ const String& address);

    CARAPI_(void) MakeA2dpDeviceUnavailableNow(
        /* [in] */ const String& address);

    CARAPI_(void) MakeA2dpDeviceUnavailableLater(
        /* [in] */ const String& address);

    CARAPI_(void) CancelA2dpDeviceTimeout();

    CARAPI_(Boolean) HasScheduledA2dpDockTimeout();

    CARAPI_(void) NotifyTopOfAudioFocusStack();

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

private:
    static CString TAG;

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
    AutoPtr<AudioSystemThread> mAudioSystemThread;
    /** @see AudioHandler */
    AutoPtr<AudioHandler> mAudioHandler;
    /** @see VolumeStreamState */
    ArrayOf< AutoPtr<VolumeStreamState> >* mStreamStates;
    AutoPtr<SettingsObserver> mSettingsObserver;

    Int32 mMode;
    Mutex mSettingsLock;
    Boolean mMediaServerOk;

    AutoPtr<ISoundPool> mSoundPool;
    Mutex mSoundEffectsLock;
    static const Int32 NUM_SOUNDPOOL_CHANNELS = 4;
    static const Int32 SOUND_EFFECT_VOLUME = 1000;

    /* Sound effect file names  */
    static CString SOUND_EFFECTS_PATH;
    static CString SOUND_EFFECT_FILES[];
    static const Int32 SOUND_EFFECT_FILES_LENGTH = 5;

    /* Sound effect file name mapping sound effect id (AudioManager.FX_xxx) to
     * file index in SOUND_EFFECT_FILES[] (first column) and indicating if effect
     * uses soundpool (second column) */
    Int32 SOUND_EFFECT_FILES_MAP[9][2];

    /** @hide Maximum volume index values for audio streams */
    Int32 MAX_STREAM_VOLUME[10];

    /* STREAM_VOLUME_ALIAS[] indicates for each stream if it uses the volume settings
     * of another stream: This avoids multiplying the volume settings for hidden
     * stream types that follow other stream behavior for volume settings
     * NOTE: do not create loops in aliases! */
    Int32 STREAM_VOLUME_ALIAS[10];

    AutoPtr<AudioSystemErrorCallback> mAudioSystemCallback;

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
    Boolean mIsRinging;

    // Devices currently connected
    HashMap<Int32, String> mConnectedDevices;

    // Forced device usage for communications
    Int32 mForcedUseForComm;

    // List of binder death handlers for setMode() client processes.
    // The last process to have called setMode() is at the top of the list.
    List< AutoPtr<SetModeDeathHandler> > mSetModeDeathHandlers;
    Mutex mSetModeDeathHandlersLock;

    // List of clients having issued a SCO start request
    List< AutoPtr<ScoClient> > mScoClients;
    Mutex mScoClientsLock;

    // BluetoothHeadset API to control SCO connection
    //BluetoothHeadset mBluetoothHeadset;

    // Bluetooth headset connection state
    Boolean mBluetoothHeadsetConnected;

    //BluetoothHeadset.ServiceListener mBluetoothHeadsetServiceListener;

    /* cache of the address of the last dock the device was connected to */
    String mDockAddress;

    //==========================================================================================
    // AudioFocus
    //==========================================================================================

    /* constant to identify focus stack entry that is used to hold the focus while the phone
     * is ringing or during a call
     */
    static CString IN_VOICE_COMM_FOCUS_ID;

    static Mutex mAudioFocusLock;

    static Mutex mRingingLock;

    //PhoneStateListener* mPhoneStateListener;// = new PhoneStateListener();

    Stack< AutoPtr<FocusStackEntry> > mFocusStack;

    Stack< AutoPtr<RemoteControlStackEntry> > mRCStack;

    pthread_cond_t mWaitCond;
};

#endif // __CAUDIOSERVICE_H__
