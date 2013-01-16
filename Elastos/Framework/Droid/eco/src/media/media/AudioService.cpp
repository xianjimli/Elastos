
#include "media/AudioService.h"
#include "media/AudioManager.h"
#include "media/AudioSystem.h"

///////////////////////////////////////////////////////////////////////////
// Construction
///////////////////////////////////////////////////////////////////////////

AudioService::AudioService()
{

}

/** @hide */
AudioService::AudioService(
    /* [in] */ IContext* context)
{
    mContext = context;
    context->GetContentResolver((IContentResolver**)&mContentResolver);

    // Intialized volume
    /*MAX_STREAM_VOLUME[AudioSystem::STREAM_VOICE_CALL] = SystemProperties.getInt(
        "ro.config.vc_call_vol_steps",
        MAX_STREAM_VOLUME[AudioSystem::STREAM_VOICE_CALL]);*/

    //mVolumePanel = new VolumePanel(context, this);
    mSettingsObserver = new SettingsObserver();
    mForcedUseForComm = AudioSystem::FORCE_NONE;
    CreateAudioSystemThread();
    ReadPersistedSettings();
    CreateStreamStates();
    // Call setMode() to initialize mSetModeDeathHandlers
    mMode = AudioSystem::MODE_INVALID;
    SetMode(AudioSystem::MODE_NORMAL, NULL);
    mMediaServerOk = TRUE;

    // Call setRingerModeInt() to apply correct mute
    // state on streams affected by ringer mode.
    mRingerModeMutedStreams = 0;
    SetRingerModeInt(GetRingerMode(), FALSE);

    //AudioSystem::SetErrorCallback(mAudioSystemCallback);
    LoadSoundEffects();

    mBluetoothHeadsetConnected = FALSE;
    /*mBluetoothHeadset = new BluetoothHeadset(context,
                                             mBluetoothHeadsetServiceListener);*/

    // Register for device connection intent broadcasts.
    /*IntentFilter intentFilter =
            new IntentFilter(Intent.ACTION_HEADSET_PLUG);
    intentFilter.addAction(BluetoothA2dp.ACTION_SINK_STATE_CHANGED);
    intentFilter.addAction(BluetoothHeadset.ACTION_STATE_CHANGED);
    intentFilter.addAction(Intent.ACTION_DOCK_EVENT);
    intentFilter.addAction(BluetoothHeadset.ACTION_AUDIO_STATE_CHANGED);
    context.registerReceiver(mReceiver, intentFilter);*/

    // Register for media button intent broadcasts.
    /*intentFilter = new IntentFilter(Intent.ACTION_MEDIA_BUTTON);
    intentFilter.setPriority(IntentFilter.SYSTEM_HIGH_PRIORITY);
    context.registerReceiver(mMediaButtonReceiver, intentFilter);*/

    // Register for phone state monitoring
    /*TelephonyManager tmgr = (TelephonyManager)
            context.getSystemService(Context.TELEPHONY_SERVICE);
    tmgr.listen(mPhoneStateListener, PhoneStateListener.LISTEN_CALL_STATE);*/
}

void AudioService::CreateAudioSystemThread()
{
    //mAudioSystemThread = new AudioSystemThread();
    mAudioSystemThread->Start();
    WaitForAudioHandlerCreation();
}

/** Waits for the volume handler to be created by the other thread. */
void AudioService::WaitForAudioHandlerCreation() 
{
    //synchronized(this) {
        while (mAudioHandler == NULL) {
            //try {
                // Wait for mAudioHandler to be set by the other thread
                //Wait();
            /*} catch (InterruptedException e) {
                Log.e(TAG, "Interrupted while waiting on volume handler.");
            }*/
        }
    //}
}

void AudioService::CreateStreamStates()
{
    Int32 numStreamTypes = AudioSystem::GetNumStreamTypes();
    ArrayOf<VolumeStreamState*>* streams = mStreamStates = ArrayOf<VolumeStreamState*>::Alloc(numStreamTypes);

    for (Int32 i = 0; i < numStreamTypes; i++) {
        //streams[i] = new VolumeStreamState(System.VOLUME_SETTINGS[STREAM_VOLUME_ALIAS[i]], i);
    }

    // Correct stream index values for streams with aliases
    for (Int32 i = 0; i < numStreamTypes; i++) {
        if ((*STREAM_VOLUME_ALIAS)[i] != i) {
            Int32 index = RescaleIndex((*streams)[i]->mIndex, (*STREAM_VOLUME_ALIAS)[i], i);
            (*streams)[i]->mIndex = (*streams)[i]->GetValidIndex(index);
            SetStreamVolumeIndex(i, index);
            index = RescaleIndex((*streams)[i]->mLastAudibleIndex, (*STREAM_VOLUME_ALIAS)[i], i);
            (*streams)[i]->mLastAudibleIndex = (*streams)[i]->GetValidIndex(index);
        }
    }
}

void AudioService::ReadPersistedSettings()
{
    AutoPtr<IContentResolver> cr = mContentResolver;

    //mRingerMode = System.getInt(cr, System.MODE_RINGER, AudioManager::RINGER_MODE_NORMAL);

    //mVibrateSetting = System.getInt(cr, System.VIBRATE_ON, 0);

    /*mRingerModeAffectedStreams = Settings.System.getInt(cr,
            Settings.System.MODE_RINGER_STREAMS_AFFECTED,
            ((1 << AudioSystem::STREAM_RING)|(1 << AudioSystem::STREAM_NOTIFICATION)|
             (1 << AudioSystem::STREAM_SYSTEM)|(1 << AudioSystem::STREAM_SYSTEM_ENFORCED)));

    mMuteAffectedStreams = System.getInt(cr,
            System.MUTE_STREAMS_AFFECTED,
            ((1 << AudioSystem::STREAM_MUSIC)|(1 << AudioSystem::STREAM_RING)|(1 << AudioSystem::STREAM_SYSTEM)));

    mNotificationsUseRingVolume = System.getInt(cr,
            Settings.System.NOTIFICATIONS_USE_RING_VOLUME, 1);*/

    if (mNotificationsUseRingVolume == 1) {
        (*STREAM_VOLUME_ALIAS)[AudioSystem::STREAM_NOTIFICATION] = AudioSystem::STREAM_RING;
    }
    // Each stream will read its own persisted settings

    // Broadcast the sticky intent
    BroadcastRingerMode();

    // Broadcast vibrate settings
    BroadcastVibrateSetting(AudioManager::VIBRATE_TYPE_RINGER);
    BroadcastVibrateSetting(AudioManager::VIBRATE_TYPE_NOTIFICATION);
}

void AudioService::SetStreamVolumeIndex(
    /* [in] */ Int32 stream, 
    /* [in] */ Int32 index) 
{
    AudioSystem::SetStreamVolumeIndex(stream, (index + 5)/10);
}

Int32 AudioService::RescaleIndex(
    /* [in] */ Int32 index, 
    /* [in] */ Int32 srcStream, 
    /* [in] */ Int32 dstStream) 
{
    return (index * (*mStreamStates)[dstStream]->GetMaxIndex() + (*mStreamStates)[srcStream]->GetMaxIndex() / 2) / (*mStreamStates)[srcStream]->GetMaxIndex();
}

///////////////////////////////////////////////////////////////////////////
// IPC methods
///////////////////////////////////////////////////////////////////////////

/** @see AudioManager#adjustVolume(Int32, Int32) */
ECode AudioService::AdjustVolume(
    /* [in] */ Int32 direction, 
    /* [in] */ Int32 flags)
{
    AdjustSuggestedStreamVolume(direction, AudioManager::USE_DEFAULT_STREAM_TYPE, flags);

    return NOERROR;
}

/** @see AudioManager#adjustVolume(Int32, Int32, Int32) */
ECode AudioService::AdjustSuggestedStreamVolume(
    /* [in] */ Int32 direction, 
    /* [in] */ Int32 suggestedStreamType, 
    /* [in] */ Int32 flags)
{
    Int32 streamType = GetActiveStreamType(suggestedStreamType);

    // Don't play sound on other streams
    if (streamType != AudioSystem::STREAM_RING && (flags & AudioManager::FLAG_PLAY_SOUND) != 0) {
        flags &= ~AudioManager::FLAG_PLAY_SOUND;
    }

    AdjustStreamVolume(streamType, direction, flags);

    return NOERROR;
}

/** @see AudioManager#adjustStreamVolume(Int32, Int32, Int32) */
ECode AudioService::AdjustStreamVolume(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 direction, 
    /* [in] */ Int32 flags)
{
    EnsureValidDirection(direction);
    EnsureValidStreamType(streamType);


    VolumeStreamState* streamState = (*mStreamStates)[(*STREAM_VOLUME_ALIAS)[streamType]];
    Int32 oldIndex = (streamState->MuteCount() != 0) ? streamState->mLastAudibleIndex : streamState->mIndex;
    Boolean adjustVolume = TRUE;

    // If either the client forces allowing ringer modes for this adjustment,
    // or the stream type is one that is affected by ringer modes
    if ((flags & AudioManager::FLAG_ALLOW_RINGER_MODES) != 0
            || streamType == AudioSystem::STREAM_RING) {
        // Check if the ringer mode changes with this volume adjustment. If
        // it does, it will handle adjusting the volume, so we won't below
        adjustVolume = CheckForRingerModeChange(oldIndex, direction);
    }

    // If stream is muted, adjust last audible index only
    Int32 index;
    if (streamState->MuteCount() != 0) {
        if (adjustVolume) {
            streamState->AdjustLastAudibleIndex(direction);
            // Post a persist volume msg
            SendMsg(mAudioHandler, MSG_PERSIST_VOLUME, streamType,
                    SENDMSG_REPLACE, 0, 1, (IInterface*)streamState, PERSIST_DELAY);
        }
        index = streamState->mLastAudibleIndex;
    } else {
        if (adjustVolume && streamState->AdjustIndex(direction)) {
            // Post message to set system volume (it in turn will post a message
            // to persist). Do not change volume if stream is muted.
            SendMsg(mAudioHandler, MSG_SET_SYSTEM_VOLUME, (*STREAM_VOLUME_ALIAS)[streamType], SENDMSG_NOOP, 0, 0,
                    (IInterface*)streamState, 0);
        }
        index = streamState->mIndex;
    }
    // UI
    //mVolumePanel.postVolumeChanged(streamType, flags);
    // Broadcast Intent
    SendVolumeUpdate(streamType, oldIndex, index);

    return NOERROR;
}

/** @see AudioManager#setStreamVolume(Int32, Int32, Int32) */
ECode AudioService::SetStreamVolume(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 index, 
    /* [in] */ Int32 flags)
{
    EnsureValidStreamType(streamType);
    VolumeStreamState* streamState = (*mStreamStates)[(*STREAM_VOLUME_ALIAS)[streamType]];

    Int32 oldIndex = (streamState->MuteCount() != 0) ? streamState->mLastAudibleIndex : streamState->mIndex;

    index = RescaleIndex(index * 10, streamType, (*STREAM_VOLUME_ALIAS)[streamType]);
    SetStreamVolumeInt((*STREAM_VOLUME_ALIAS)[streamType], index, FALSE, TRUE);

    index = (streamState->MuteCount() != 0) ? streamState->mLastAudibleIndex : streamState->mIndex;

    // UI, etc.
    //mVolumePanel.postVolumeChanged(streamType, flags);
    // Broadcast Intent
    SendVolumeUpdate(streamType, oldIndex, index);

    return NOERROR;
}

void AudioService::SendVolumeUpdate(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 oldIndex, 
    /* [in] */ Int32 index)
{
    oldIndex = (oldIndex + 5) / 10;
    index = (index + 5) / 10;

    AutoPtr<IIntent> intent;
    CIntent::New(AudioManager::VOLUME_CHANGED_ACTION, (IIntent**)&intent);
    intent->PutInt32Extra(AudioManager::EXTRA_VOLUME_STREAM_TYPE, streamType);
    intent->PutInt32Extra(AudioManager::EXTRA_VOLUME_STREAM_VALUE, index);
    intent->PutInt32Extra(AudioManager::EXTRA_PREV_VOLUME_STREAM_VALUE, oldIndex);

    mContext->SendBroadcast(intent);
}

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
void AudioService::SetStreamVolumeInt(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 index, 
    /* [in] */ Boolean force, 
    /* [in] */ Boolean lastAudible)
{
    VolumeStreamState* streamState = (*mStreamStates)[streamType];

    // If stream is muted, set last audible index only
    if (streamState->MuteCount() != 0) {
        // Do not allow last audible index to be 0
        if (index != 0) {
            streamState->SetLastAudibleIndex(index);
            // Post a persist volume msg
            SendMsg(mAudioHandler, MSG_PERSIST_VOLUME, streamType,
                    SENDMSG_REPLACE, 0, 1, (IInterface*)streamState, PERSIST_DELAY);
        }
    } else {
        if (streamState->SetIndex(index, lastAudible) || force) {
            // Post message to set system volume (it in turn will post a message
            // to persist).
            SendMsg(mAudioHandler, MSG_SET_SYSTEM_VOLUME, streamType, SENDMSG_NOOP, 0, 0,
                    (IInterface*)streamState, 0);
        }
    }
}

/** @see AudioManager#setStreamSolo(Int32, Boolean) */
ECode AudioService::SetStreamSolo(
    /* [in] */ Int32 streamType, 
    /* [in] */ Boolean state, 
    /* [in] */ IBinder* cb)
{
    for (Int32 stream = 0; stream < mStreamStates->GetLength(); stream++) {
        if (!IsStreamAffectedByMute(stream) || stream == streamType) continue;
        // Bring back last audible volume
        (*mStreamStates)[stream]->Mute(cb, state);
     }

    return NOERROR;
}

/** @see AudioManager#setStreamMute(Int32, Boolean) */
ECode AudioService::SetStreamMute(
    /* [in] */ Int32 streamType, 
    /* [in] */ Boolean state, 
    /* [in] */ IBinder* cb)
{
    if (IsStreamAffectedByMute(streamType)) {
        (*mStreamStates)[streamType]->Mute(cb, state);
    }

    return NOERROR;
}

/** @see AudioManager#getStreamVolume(Int32) */
Int32 AudioService::GetStreamVolume(
    /* [in] */ Int32 streamType) 
{
    EnsureValidStreamType(streamType);
    return ((*mStreamStates)[streamType]->mIndex + 5) / 10;
}

/** @see AudioManager#getStreamMaxVolume(Int32) */
Int32 AudioService::GetStreamMaxVolume(
    /* [in] */ Int32 streamType) 
{
    EnsureValidStreamType(streamType);
    return ((*mStreamStates)[streamType]->GetMaxIndex() + 5) / 10;
}

/** @see AudioManager#getRingerMode() */
Int32 AudioService::GetRingerMode()
{
    return mRingerMode;
}

/** @see AudioManager#setRingerMode(Int32) */
ECode AudioService::SetRingerMode(
    /* [in] */ Int32 ringerMode)
{
    //synchronized (mSettingsLock) {
        if (ringerMode != mRingerMode) {
            SetRingerModeInt(ringerMode, TRUE);
            // Send sticky broadcast
            BroadcastRingerMode();
        }
    //}

        return NOERROR;
}

void AudioService::SetRingerModeInt(
    /* [in] */ Int32 ringerMode, 
    /* [in] */ Boolean persist) 
{
    mRingerMode = ringerMode;

    // Mute stream if not previously muted by ringer mode and ringer mode
    // is not RINGER_MODE_NORMAL and stream is affected by ringer mode.
    // Unmute stream if previously muted by ringer mode and ringer mode
    // is RINGER_MODE_NORMAL or stream is not affected by ringer mode.
    Int32 numStreamTypes = AudioSystem::GetNumStreamTypes();
    for (Int32 streamType = numStreamTypes - 1; streamType >= 0; streamType--) {
        if (IsStreamMutedByRingerMode(streamType)) {
            if (!IsStreamAffectedByRingerMode(streamType) ||
                mRingerMode == AudioManager::RINGER_MODE_NORMAL) {
                (*mStreamStates)[streamType]->Mute(NULL, FALSE);
                mRingerModeMutedStreams &= ~(1 << streamType);
            }
        } else {
            if (IsStreamAffectedByRingerMode(streamType) &&
                mRingerMode != AudioManager::RINGER_MODE_NORMAL) {
               (*mStreamStates)[streamType]->Mute(NULL, TRUE);
               mRingerModeMutedStreams |= (1 << streamType);
           }
        }
    }

    // Post a persist ringer mode msg
    if (persist) {
        SendMsg(mAudioHandler, MSG_PERSIST_RINGER_MODE, SHARED_MSG,
                SENDMSG_REPLACE, 0, 0, NULL, PERSIST_DELAY);
    }
}

/** @see AudioManager#shouldVibrate(Int32) */
Boolean AudioService::ShouldVibrate(
    /* [in] */ Int32 vibrateType)
{

    switch (GetVibrateSetting(vibrateType)) {

        case AudioManager::VIBRATE_SETTING_ON:
            return mRingerMode != AudioManager::RINGER_MODE_SILENT;

        case AudioManager::VIBRATE_SETTING_ONLY_SILENT:
            return mRingerMode == AudioManager::RINGER_MODE_VIBRATE;

        case AudioManager::VIBRATE_SETTING_OFF:
            // return FALSE, even for incoming calls
            return FALSE;

        default:
            return FALSE;
    }
}

/** @see AudioManager#getVibrateSetting(Int32) */
Int32 AudioService::GetVibrateSetting(
    /* [in] */ Int32 vibrateType) 
{
    return (mVibrateSetting >> (vibrateType * 2)) & 3;
}

/** @see AudioManager#setVibrateSetting(Int32, Int32) */
ECode AudioService::SetVibrateSetting(
    /* [in] */ Int32 vibrateType, 
    /* [in] */ Int32 vibrateSetting)
{

    mVibrateSetting = GetValueForVibrateSetting(mVibrateSetting, vibrateType, vibrateSetting);

    // Broadcast change
    BroadcastVibrateSetting(vibrateType);

    // Post message to set ringer mode (it in turn will post a message
    // to persist)
    SendMsg(mAudioHandler, MSG_PERSIST_VIBRATE_SETTING, SHARED_MSG, SENDMSG_NOOP, 0, 0,
            NULL, 0);

    return NOERROR;
}

/**
 * @see #setVibrateSetting(Int32, Int32)
 */
Int32 AudioService::GetValueForVibrateSetting(
    /* [in] */ Int32 existingValue, 
    /* [in] */ Int32 vibrateType,
    /* [in] */ Int32 vibrateSetting) 
{

    // First clear the existing setting. Each vibrate type has two bits in
    // the value. Note '3' is '11' in binary.
    existingValue &= ~(3 << (vibrateType * 2));

    // Set into the old value
    existingValue |= (vibrateSetting & 3) << (vibrateType * 2);

    return existingValue;
}

AudioService::SetModeDeathHandler::SetModeDeathHandler(
    /* [in] */ IBinder* cb)
{
    mCb = cb;
}

ECode AudioService::SetModeDeathHandler::BinderDied()
{
    //synchronized(mSetModeDeathHandlers) {
        //Log.w(TAG, "setMode() client died");
        Int32 index;// = mSetModeDeathHandlers->IndexOf(this);
        if (index < 0) {
            //Log.w(TAG, "unregistered setMode() client died");
        } else {
            //mSetModeDeathHandlers->Remove(this);
            // If dead client was a the top of client list,
            // apply next mode in the stack
            if (index == 0) {
                // mSetModeDeathHandlers is never empty as the initial entry
                // created when AudioService starts is never removed
                AudioService::SetModeDeathHandler* hdlr;// = (*mSetModeDeathHandlers)[0];
                Int32 mode = hdlr->GetMode();
                //if (AudioService.this.mMode != mode) {
                    if (AudioSystem::SetPhoneState(mode) == AudioSystem::AUDIO_STATUS_OK) {
                        //AudioService.this.mMode = mode;
                    }
                //}
            }
        }
    //}

        return NOERROR;
}

ECode AudioService::SetModeDeathHandler::SetMode(
    /* [in] */ Int32 mode)
{
    mMode = mode;

    return NOERROR;
}

Int32 AudioService::SetModeDeathHandler::GetMode() 
{
    return mMode;
}

AutoPtr<IBinder> AudioService::SetModeDeathHandler::GetBinder()
{
    return mCb;
}

PInterface AudioService::SetModeDeathHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::SetModeDeathHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::SetModeDeathHandler::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::SetModeDeathHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}


/** @see AudioManager#setMode(Int32) */
ECode AudioService::SetMode(
    /* [in] */ Int32 mode, 
    /* [in] */ IBinder* cb)
{
    if (!CheckAudioSettingsPermission(String("setMode()"))) {
        return NOERROR;
    }

    if (mode < AudioSystem::MODE_CURRENT || mode >= AudioSystem::NUM_MODES) {
        return NOERROR;
    }

    //synchronized (mSettingsLock) {
        if (mode == AudioSystem::MODE_CURRENT) {
            mode = mMode;
        }
        if (mode != mMode) {
            if (AudioSystem::SetPhoneState(mode) == AudioSystem::AUDIO_STATUS_OK) {
                mMode = mode;

                //synchronized(mSetModeDeathHandlers) {
                //    SetModeDeathHandler* hdlr = NULL;
                //    Iterator iter = mSetModeDeathHandlers.iterator();
                //    while (iter.hasNext()) {
                //        SetModeDeathHandler* h = (SetModeDeathHandler)iter.next();
                //        if (h.getBinder() == cb) {
                //            hdlr = h;
                //            // Remove from client list so that it is re-inserted at top of list
                //            iter.remove();
                //            break;
                //        }
                //    }
                //    if (hdlr == NULL) {
                //        hdlr = new SetModeDeathHandler(cb);
                //        // cb is NULL when setMode() is called by AudioService constructor
                //        if (cb != NULL) {
                //            // Register for client death notification
                //            //try {
                //                cb->LinkToDeath(hdlr, 0);
                //            //} catch (RemoteException e) {
                //            //    // Client has died!
                //            //    Log.w(TAG, "setMode() could not link to "+cb+" binder death");
                //            //}
                //        }
                //    }
                //    // Last client to call setMode() is always at top of client list
                //    // as required by SetModeDeathHandler.binderDied()
                //    mSetModeDeathHandlers.add(0, hdlr);
                //    hdlr.setMode(mode);
                //}

                if (mode != AudioSystem::MODE_NORMAL) {
                    ClearAllScoClients();
                }
            }
        }
        Int32 streamType = GetActiveStreamType(AudioManager::USE_DEFAULT_STREAM_TYPE);
        Int32 index = (*mStreamStates)[(*STREAM_VOLUME_ALIAS)[streamType]]->mIndex;
        SetStreamVolumeInt((*STREAM_VOLUME_ALIAS)[streamType], index, TRUE, FALSE);
    //}

        return NOERROR;
}

/** @see AudioManager#getMode() */
Int32 AudioService::GetMode()
{
    return mMode;
}

/** @see AudioManager#playSoundEffect(Int32) */
ECode AudioService::PlaySoundEffect(
    /* [in] */ Int32 effectType)
{
    SendMsg(mAudioHandler, MSG_PLAY_SOUND_EFFECT, SHARED_MSG, SENDMSG_NOOP,
            effectType, -1, NULL, 0);

    return NOERROR;
}

/** @see AudioManager#playSoundEffect(Int32, Float) */
ECode AudioService::PlaySoundEffectVolume(
    /* [in] */ Int32 effectType, 
    /* [in] */ Float volume)
{
    LoadSoundEffects();
    SendMsg(mAudioHandler, MSG_PLAY_SOUND_EFFECT, SHARED_MSG, SENDMSG_NOOP,
            effectType, (Int32) (volume * 1000), NULL, 0);
}

/**
 * Loads samples into the soundpool.
 * This method must be called at when sound effects are enabled
 */
Boolean AudioService::LoadSoundEffects()
{
    //synchronized (mSoundEffectsLock) {
    //    if (mSoundPool != NULL) {
    //        return TRUE;
    //    }
    //    mSoundPool = new SoundPool(NUM_SOUNDPOOL_CHANNELS, AudioSystem::STREAM_SYSTEM, 0);
    //    if (mSoundPool == NULL) {
    //        return FALSE;
    //    }
    //    /*
    //     * poolId table: The value -1 in this table indicates that corresponding
    //     * file (same index in SOUND_EFFECT_FILES[] has not been loaded.
    //     * Once loaded, the value in poolId is the sample ID and the same
    //     * sample can be reused for another effect using the same file.
    //     */
    //    Int32[] poolId = new Int32[SOUND_EFFECT_FILES.length];
    //    for (Int32 fileIdx = 0; fileIdx < SOUND_EFFECT_FILES.length; fileIdx++) {
    //        poolId[fileIdx] = -1;
    //    }
    //    /*
    //     * Effects whose value in SOUND_EFFECT_FILES_MAP[effect][1] is -1 must be loaded.
    //     * If load succeeds, value in SOUND_EFFECT_FILES_MAP[effect][1] is > 0:
    //     * this indicates we have a valid sample loaded for this effect.
    //     */
    //    for (Int32 effect = 0; effect < AudioManager::NUM_SOUND_EFFECTS; effect++) {
    //        // Do not load sample if this effect uses the MediaPlayer
    //        if (SOUND_EFFECT_FILES_MAP[effect][1] == 0) {
    //            continue;
    //        }
    //        if (poolId[SOUND_EFFECT_FILES_MAP[effect][0]] == -1) {
    //            String filePath = Environment.getRootDirectory() + SOUND_EFFECTS_PATH + SOUND_EFFECT_FILES[SOUND_EFFECT_FILES_MAP[effect][0]];
    //            Int32 sampleId = mSoundPool.load(filePath, 0);
    //            SOUND_EFFECT_FILES_MAP[effect][1] = sampleId;
    //            poolId[SOUND_EFFECT_FILES_MAP[effect][0]] = sampleId;
    //            if (sampleId <= 0) {
    //                Log.w(TAG, "Soundpool could not load file: "+filePath);
    //            }
    //        } else {
    //            SOUND_EFFECT_FILES_MAP[effect][1] = poolId[SOUND_EFFECT_FILES_MAP[effect][0]];
    //        }
    //    }
    //}

    return TRUE;
}

/**
 *  Unloads samples from the sound pool.
 *  This method can be called to free some memory when
 *  sound effects are disabled.
 */
ECode AudioService::UnloadSoundEffects()
{
    /*synchronized (mSoundEffectsLock) {
        if (mSoundPool == NULL) {
            return;
        }
        Int32[] poolId = new Int32[SOUND_EFFECT_FILES.length];
        for (Int32 fileIdx = 0; fileIdx < SOUND_EFFECT_FILES.length; fileIdx++) {
            poolId[fileIdx] = 0;
        }

        for (Int32 effect = 0; effect < AudioManager::NUM_SOUND_EFFECTS; effect++) {
            if (SOUND_EFFECT_FILES_MAP[effect][1] <= 0) {
                continue;
            }
            if (poolId[SOUND_EFFECT_FILES_MAP[effect][0]] == 0) {
                mSoundPool.unload(SOUND_EFFECT_FILES_MAP[effect][1]);
                SOUND_EFFECT_FILES_MAP[effect][1] = -1;
                poolId[SOUND_EFFECT_FILES_MAP[effect][0]] = -1;
            }
        }
        mSoundPool = NULL;
    }*/

    return NOERROR;
}

/** @see AudioManager#reloadAudioSettings() */
ECode AudioService::ReloadAudioSettings()
{
    // restore ringer mode, ringer mode affected streams, mute affected streams and vibrate settings
    ReadPersistedSettings();

    // restore volume settings
    Int32 numStreamTypes = AudioSystem::GetNumStreamTypes();
    for (Int32 streamType = 0; streamType < numStreamTypes; streamType++) {
        VolumeStreamState* streamState = (*mStreamStates)[streamType];

        /*String settingName = System.VOLUME_SETTINGS[STREAM_VOLUME_ALIAS[streamType]];
        String lastAudibleSettingName = settingName + System.APPEND_FOR_LAST_AUDIBLE;*/
        Int32 index;/* = Settings.System.getInt(mContentResolver,
                                       settingName,
                                       AudioManager::DEFAULT_STREAM_VOLUME[streamType]);*/
        if ((*STREAM_VOLUME_ALIAS)[streamType] != streamType) {
            index = RescaleIndex(index * 10, (*STREAM_VOLUME_ALIAS)[streamType], streamType);
        } else {
            index *= 10;
        }
        streamState->mIndex = streamState->GetValidIndex(index);

        index = (index + 5) / 10;
        /*index = Settings.System.getInt(mContentResolver,
                                        lastAudibleSettingName,
                                        (index > 0) ? index : AudioManager::DEFAULT_STREAM_VOLUME[streamType]);*/
        if ((*STREAM_VOLUME_ALIAS)[streamType] != streamType) {
            index = RescaleIndex(index * 10, (*STREAM_VOLUME_ALIAS)[streamType], streamType);
        } else {
            index *= 10;
        }
        streamState->mLastAudibleIndex = streamState->GetValidIndex(index);

        // unmute stream that was muted but is not affect by mute anymore
        if (streamState->MuteCount() != 0 && !IsStreamAffectedByMute(streamType)) {
            Int32 size = streamState->mDeathHandlers->GetLength();
            for (Int32 i = 0; i < size; i++) {
                (*(streamState->mDeathHandlers))[i]->mMuteCount = 1;
                (*(streamState->mDeathHandlers))[i]->Mute(FALSE);
            }
        }
        // apply stream volume
        if (streamState->MuteCount() == 0) {
            SetStreamVolumeIndex(streamType, streamState->mIndex);
        }
    }

    // apply new ringer mode
    SetRingerModeInt(GetRingerMode(), FALSE);
}

/** @see AudioManager#setSpeakerphoneOn() */
ECode AudioService::SetSpeakerphoneOn(
    /* [in] */ Boolean on)
{
    if (!CheckAudioSettingsPermission(String("setSpeakerphoneOn()"))) {
        return NOERROR;
    }
    if (on) {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_SPEAKER);
        mForcedUseForComm = AudioSystem::FORCE_SPEAKER;
    } else {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_NONE);
        mForcedUseForComm = AudioSystem::FORCE_NONE;
    }

    return NOERROR;
}

/** @see AudioManager#isSpeakerphoneOn() */
Boolean AudioService::IsSpeakerphoneOn()
{
    if (mForcedUseForComm == AudioSystem::FORCE_SPEAKER) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/** @see AudioManager#setBluetoothScoOn() */
ECode AudioService::SetBluetoothScoOn(
    /* [in] */ Boolean on)
{
    if (!CheckAudioSettingsPermission(String("setBluetoothScoOn()"))) {
        return NOERROR;
    }
    if (on) {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_BT_SCO);
        AudioSystem::SetForceUse(AudioSystem::FOR_RECORD, AudioSystem::FORCE_BT_SCO);
        mForcedUseForComm = AudioSystem::FORCE_BT_SCO;
    } else {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_NONE);
        AudioSystem::SetForceUse(AudioSystem::FOR_RECORD, AudioSystem::FORCE_NONE);
        mForcedUseForComm = AudioSystem::FORCE_NONE;
    }

    return NOERROR;
}

/** @see AudioManager#isBluetoothScoOn() */
Boolean AudioService::IsBluetoothScoOn() 
{
    if (mForcedUseForComm == AudioSystem::FORCE_BT_SCO) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/** @see AudioManager#startBluetoothSco() */
ECode AudioService::StartBluetoothSco(
    /* [in] */ IBinder* cb)
{
    if (!CheckAudioSettingsPermission(String("startBluetoothSco()"))) {
        return NOERROR;
    }
    ScoClient* client = GetScoClient(cb);
    client->IncCount();

    return NOERROR;
}

/** @see AudioManager#stopBluetoothSco() */
ECode AudioService::StopBluetoothSco(
    /* [in] */ IBinder* cb)
{
    if (!CheckAudioSettingsPermission(String("stopBluetoothSco()"))) {
        return NOERROR;
    }
    ScoClient* client = GetScoClient(cb);
    client->DecCount();

    return NOERROR;
}

AudioService::ScoClient::ScoClient(
    /* [in] */ IBinder* cb)
{
    mCb = cb;
    mStartcount = 0;
}

ECode AudioService::ScoClient::BinderDied()
{
    /*synchronized(mScoClients) {
        Log.w(TAG, "SCO client died");
        Int32 index = mScoClients.indexOf(this);
        if (index < 0) {
            Log.w(TAG, "unregistered SCO client died");
        } else {
            clearCount(TRUE);
            mScoClients.remove(this);
        }
    }*/

    return NOERROR;
}

void AudioService::ScoClient::IncCount()
{
    //synchronized(mScoClients) {
    //    RequestScoState(BluetoothHeadset.AUDIO_STATE_CONNECTED);
    //    if (mStartcount == 0) {
    //        try {
    //            mCb->LinkToDeath(this, 0);
    //        } catch (RemoteException e) {
    //            // client has already died!
    //            Log.w(TAG, "ScoClient  incCount() could not link to "+mCb+" binder death");
    //        }
    //    }
    //    mStartcount++;
    //}

}

void AudioService::ScoClient::DecCount()
{
    /*synchronized(mScoClients) {
        if (mStartcount == 0) {
            Log.w(TAG, "ScoClient.decCount() already 0");
        } else {
            mStartcount--;
            if (mStartcount == 0) {
                try {
                    mCb.unlinkToDeath(this, 0);
                } catch (NoSuchElementException e) {
                    Log.w(TAG, "decCount() going to 0 but not registered to binder");
                }
            }
            requestScoState(BluetoothHeadset.AUDIO_STATE_DISCONNECTED);
        }
    }*/

    //return NOERROR;
}

void AudioService::ScoClient::ClearCount(
    /* [in] */ Boolean stopSco)
{
    /*synchronized(mScoClients) {
        if (mStartcount != 0) {
            try {
                mCb.unlinkToDeath(this, 0);
            } catch (NoSuchElementException e) {
                Log.w(TAG, "clearCount() mStartcount: "+mStartcount+" != 0 but not registered to binder");
            }
        }
        mStartcount = 0;
        if (stopSco) {
            requestScoState(BluetoothHeadset.AUDIO_STATE_DISCONNECTED);
        }
    }*/

    //return NOERROR;
}

Int32 AudioService::ScoClient::GetCount()
{
    return mStartcount;
}

AutoPtr<IBinder> AudioService::ScoClient::GetBinder()
{
    return mCb;
}

Int32 AudioService::ScoClient::TotalCount()
{
    //synchronized(mScoClients) {
        Int32 count = 0;
        Int32 size;// = mScoClients->GetLength();
        for (Int32 i = 0; i < size; i++) {
            //count += (*mScoClients)[i]->GetCount();
        }
        return count;
    //}
}

void AudioService::ScoClient::RequestScoState(
    /* [in] */ Int32 state)
{
    /*if (totalCount() == 0 &&
        mBluetoothHeadsetConnected &&
        AudioService.this.mMode == AudioSystem::MODE_NORMAL) {
        if (state == BluetoothHeadset.AUDIO_STATE_CONNECTED) {
            mBluetoothHeadset.startVoiceRecognition();
        } else {
            mBluetoothHeadset.stopVoiceRecognition();
        }
    }*/
}

PInterface AudioService::ScoClient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::ScoClient::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::ScoClient::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::ScoClient::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

AudioService::ScoClient* AudioService::GetScoClient(
    /* [in] */ IBinder* cb) 
{
    //synchronized(mScoClients) {
        //ScoClient client;
        //Int32 size = mScoClients.size();
        //for (Int32 i = 0; i < size; i++) {
        //    client = mScoClients.get(i);
        //    if (client.getBinder() == cb)
        //        return client;
        //}
        //client = new ScoClient(cb);
        //mScoClients.add(client);
        //return client;
    //}

    return NULL;
}

ECode AudioService::ClearAllScoClients()
{
    //synchronized(mScoClients) {
        Int32 size = mScoClients->GetLength();
        for (Int32 i = 0; i < size; i++) {
            (*mScoClients)[i]->ClearCount(FALSE);
        }
    //}

        return NOERROR;
}

///////////////////////////////////////////////////////////////////////////
// Internal methods
///////////////////////////////////////////////////////////////////////////

/**
 * Checks if the adjustment should change ringer mode instead of just
 * adjusting volume. If so, this will set the proper ringer mode and volume
 * indices on the stream states.
 */
Boolean AudioService::CheckForRingerModeChange(
    /* [in] */ Int32 oldIndex, 
    /* [in] */ Int32 direction)
{
    Boolean adjustVolumeIndex = TRUE;
    Int32 newRingerMode = mRingerMode;

    if (mRingerMode == AudioManager::RINGER_MODE_NORMAL) {
        // audible mode, at the bottom of the scale
        if (direction == AudioManager::ADJUST_LOWER
                && (oldIndex + 5) / 10 == 1) {
            // "silent mode", but which one?
            /*newRingerMode = System.getInt(mContentResolver, System.VIBRATE_IN_SILENT, 1) == 1
                ? AudioManager::RINGER_MODE_VIBRATE
                : AudioManager::RINGER_MODE_SILENT;*/
        }
    } else {
        if (direction == AudioManager::ADJUST_RAISE) {
            // exiting silent mode
            newRingerMode = AudioManager::RINGER_MODE_NORMAL;
        } else {
            // prevent last audible index to reach 0
            adjustVolumeIndex = FALSE;
        }
    }

    if (newRingerMode != mRingerMode) {
        SetRingerMode(newRingerMode);

        /*
         * If we are changing ringer modes, do not increment/decrement the
         * volume index. Instead, the handler for the message above will
         * take care of changing the index.
         */
        adjustVolumeIndex = FALSE;
    }

    return adjustVolumeIndex;
}

Boolean AudioService::IsStreamAffectedByRingerMode(
    /* [in] */ Int32 streamType)
{
    return (mRingerModeAffectedStreams & (1 << streamType)) != 0;
}

Boolean AudioService::IsStreamMutedByRingerMode(
    /* [in] */ Int32 streamType)
{
    return (mRingerModeMutedStreams & (1 << streamType)) != 0;
}

Boolean AudioService::IsStreamAffectedByMute(
    /* [in] */ Int32 streamType)
{
    return (mMuteAffectedStreams & (1 << streamType)) != 0;
}

void AudioService::EnsureValidDirection(
    /* [in] */ Int32 direction)
{
    if (direction < AudioManager::ADJUST_LOWER || direction > AudioManager::ADJUST_RAISE) {
        //throw new IllegalArgumentException("Bad direction " + direction);
    }
}

void AudioService::EnsureValidStreamType(
    /* [in] */ Int32 streamType)
{
    if (streamType < 0 || streamType >= mStreamStates->GetLength()) {
        //throw new IllegalArgumentException("Bad stream type " + streamType);
    }
}

Int32 AudioService::GetActiveStreamType(
    /* [in] */ Int32 suggestedStreamType) 
{
    Boolean isOffhook = FALSE;
    /*try {
        ITelephony phone = ITelephony.Stub.asInterface(ServiceManager.checkService("phone"));
        if (phone != NULL) isOffhook = phone.isOffhook();
    } catch (RemoteException e) {
        Log.w(TAG, "Couldn't connect to phone service", e);
    }*/

    if (AudioSystem::GetForceUse(AudioSystem::FOR_COMMUNICATION) == AudioSystem::FORCE_BT_SCO) {
        // Log.v(TAG, "getActiveStreamType: Forcing STREAM_BLUETOOTH_SCO...");
        return AudioSystem::STREAM_BLUETOOTH_SCO;
    } else if (isOffhook || AudioSystem::IsStreamActive(AudioSystem::STREAM_VOICE_CALL)) {
        // Log.v(TAG, "getActiveStreamType: Forcing STREAM_VOICE_CALL...");
        return AudioSystem::STREAM_VOICE_CALL;
    } else if (AudioSystem::IsStreamActive(AudioSystem::STREAM_MUSIC)) {
        // Log.v(TAG, "getActiveStreamType: Forcing STREAM_MUSIC...");
        return AudioSystem::STREAM_MUSIC;
    } else if (suggestedStreamType == AudioManager::USE_DEFAULT_STREAM_TYPE) {
        // Log.v(TAG, "getActiveStreamType: Forcing STREAM_RING...");
        return AudioSystem::STREAM_RING;
    } else {
        // Log.v(TAG, "getActiveStreamType: Returning suggested type " + suggestedStreamType);
        return suggestedStreamType;
    }
}

void AudioService::BroadcastRingerMode()
{
    // Send sticky broadcast
    /*Intent broadcast = new Intent(AudioManager::RINGER_MODE_CHANGED_ACTION);
    broadcast.putExtra(AudioManager::EXTRA_RINGER_MODE, mRingerMode);
    broadcast.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT
            | Intent.FLAG_RECEIVER_REPLACE_PENDING);
    long origCallerIdentityToken = Binder.clearCallingIdentity();
    mContext.sendStickyBroadcast(broadcast);
    Binder.restoreCallingIdentity(origCallerIdentityToken);*/
}

void AudioService::BroadcastVibrateSetting(
    /* [in] */ Int32 vibrateType)
{
    // Send broadcast
    /*if (ActivityManagerNative.isSystemReady()) {
        Intent broadcast = new Intent(AudioManager::VIBRATE_SETTING_CHANGED_ACTION);
        broadcast.putExtra(AudioManager::EXTRA_VIBRATE_TYPE, vibrateType);
        broadcast.putExtra(AudioManager::EXTRA_VIBRATE_SETTING, getVibrateSetting(vibrateType));
        mContext.sendBroadcast(broadcast);
    }*/
}

// Message helper methods
Int32 AudioService::GetMsg(
    /* [in] */ Int32 baseMsg, 
    /* [in] */ Int32 streamType) 
{
    return (baseMsg & 0xffff) | streamType << 16;
}

Int32 AudioService::GetMsgBase(
    /* [in] */ Int32 msg)
{
    return msg & 0xffff;
}

void AudioService::SendMsg(
    /* [in] */ IHandler* handler, 
    /* [in] */ Int32 baseMsg, 
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 existingMsgPolicy, 
    /* [in] */ Int32 arg1, 
    /* [in] */ Int32 arg2, 
    /* [in] */ IInterface* obj, 
    /* [in] */ Int32 delay)
{
    Int32 msg = (streamType == SHARED_MSG) ? baseMsg : GetMsg(baseMsg, streamType);

    Boolean hasMessages;

    /*if (existingMsgPolicy == SENDMSG_REPLACE) {
        handler->RemoveMessages(msg);
    } else if (existingMsgPolicy == SENDMSG_NOOP && (handler->HasMessages(msg, &hasMessages), hasMessages)) {
        return;
    }

    handler->SendMessageDelayed(handler->ObtainMessage(msg, arg1, arg2, obj), delay);*/
}

Boolean AudioService::CheckAudioSettingsPermission(
    /* [in] */ String method)
{
    /*if (mContext->CheckCallingOrSelfPermission("android.permission.MODIFY_AUDIO_SETTINGS")
            == PackageManager.PERMISSION_GRANTED) {
        return TRUE;
    }
    String msg = "Audio Settings Permission Denial: " + method + " from pid="
            + Binder.getCallingPid()
            + ", uid=" + Binder.getCallingUid();
    Log.w(TAG, msg);*/
    return FALSE;
}


///////////////////////////////////////////////////////////////////////////
// Inner classes
///////////////////////////////////////////////////////////////////////////

AudioService::VolumeStreamState::VolumeStreamState(
    /* [in] */ String settingName, 
    /* [in] */ Int32 streamType)
{

    SetVolumeIndexSettingName(settingName);

    mStreamType = streamType;

    //AutoPtr<IContentResolver> cr = mContentResolver;
    //mIndexMax = (*MAX_STREAM_VOLUME)[streamType];
    /*mIndex = Settings.System.getInt(cr,
                                    mVolumeIndexSettingName,
                                    AudioManager::DEFAULT_STREAM_VOLUME[streamType]);
    mLastAudibleIndex = Settings.System.getInt(cr,
                                               mLastAudibleVolumeIndexSettingName,
                                               (mIndex > 0) ? mIndex : AudioManager::DEFAULT_STREAM_VOLUME[streamType]);*/
    AudioSystem::InitStreamVolume(streamType, 0, mIndexMax);
    mIndexMax *= 10;
    mIndex = GetValidIndex(10 * mIndex);
    mLastAudibleIndex = GetValidIndex(10 * mLastAudibleIndex);
    //SetStreamVolumeIndex(streamType, mIndex);
    //mDeathHandlers = new ArrayList<VolumeDeathHandler>();
}

void AudioService::VolumeStreamState::SetVolumeIndexSettingName(
    /* [in] */ String settingName)
{
    mVolumeIndexSettingName = settingName;
    
    //mLastAudibleVolumeIndexSettingName = settingName + System.APPEND_FOR_LAST_AUDIBLE;

    //return NOERROR;
}

Boolean AudioService::VolumeStreamState::AdjustIndex(
    /* [in] */ Int32 deltaIndex) 
{
    return SetIndex(mIndex + deltaIndex * 10, TRUE);
}

Boolean AudioService::VolumeStreamState::SetIndex(
    /* [in] */ Int32 index, 
    /* [in] */ Boolean lastAudible)
{
    Int32 oldIndex = mIndex;
    mIndex = GetValidIndex(index);

    if (oldIndex != mIndex) {
        if (lastAudible) {
            mLastAudibleIndex = mIndex;
        }
        // Apply change to all streams using this one as alias
        Int32 numStreamTypes = AudioSystem::GetNumStreamTypes();
        for (Int32 streamType = numStreamTypes - 1; streamType >= 0; streamType--) {
            /*if (streamType != mStreamType && (*STREAM_VOLUME_ALIAS)[streamType] == mStreamType) {
                (*mStreamStates)[streamType]->SetIndex(RescaleIndex(mIndex, mStreamType, streamType), lastAudible);
            }*/
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

void AudioService::VolumeStreamState::SetLastAudibleIndex(
    /* [in] */ Int32 index)
{
    mLastAudibleIndex = GetValidIndex(index);

    //return NOERROR;
}

void AudioService::VolumeStreamState::AdjustLastAudibleIndex(
    /* [in] */ Int32 deltaIndex)
{
    SetLastAudibleIndex(mLastAudibleIndex + deltaIndex * 10);

    //return NOERROR;
}

Int32 AudioService::VolumeStreamState::GetMaxIndex()
{
    return mIndexMax;
}

void AudioService::VolumeStreamState::Mute(
    /* [in] */ IBinder* cb, 
    /* [in] */ Boolean state)
{
    VolumeDeathHandler* handler = GetDeathHandler(cb, state);
    if (handler == NULL) {
        //Log.e(TAG, "Could not get client death handler for stream: "+mStreamType);
        //return NOERROR;
    }
    handler->Mute(state);

    //return NOERROR;
}

Int32 AudioService::VolumeStreamState::GetValidIndex(
    /* [in] */ Int32 index)
{
    if (index < 0) {
        return 0;
    } else if (index > mIndexMax) {
        return mIndexMax;
    }

    return index;
}


AudioService::VolumeStreamState::VolumeDeathHandler::VolumeDeathHandler(
    /* [in] */ IBinder* cb) 
{
    mICallback = cb;
}

void AudioService::VolumeStreamState::VolumeDeathHandler::Mute(
    /* [in] */ Boolean state)
{
    //synchronized(mDeathHandlers) {
        //if (state) {
        //    if (mMuteCount == 0) {
        //        // Register for client death notification
        //        //try {
        //            // mICallback can be 0 if muted by AudioService
        //            if (mICallback != NULL) {
        //                mICallback->LinkToDeath(this, 0);
        //            }
        //            mDeathHandlers->Add(this);
        //            // If the stream is not yet muted by any client, set lvel to 0
        //            if (muteCount() == 0) {
        //                SetIndex(0, FALSE);
        //                SendMsg(mAudioHandler, MSG_SET_SYSTEM_VOLUME, mStreamType, SENDMSG_NOOP, 0, 0,
        //                        VolumeStreamState.this, 0);
        //            }
        //        //} catch (RemoteException e) {
        //        //    // Client has died!
        //        //    binderDied();
        //        //    mDeathHandlers.notify();
        //        //    return;
        //        //}
        //    } else {
        //        Log.w(TAG, "stream: "+mStreamType+" was already muted by this client");
        //    }
        //    mMuteCount++;
        //} else {
        //    if (mMuteCount == 0) {
        //        Log.e(TAG, "unexpected unmute for stream: "+mStreamType);
        //    } else {
        //        mMuteCount--;
        //        if (mMuteCount == 0) {
        //            // Unregistr from client death notification
        //            mDeathHandlers.remove(this);
        //            // mICallback can be 0 if muted by AudioService
        //            if (mICallback != NULL) {
        //                mICallback.unlinkToDeath(this, 0);
        //            }
        //            if (muteCount() == 0) {
        //                // If the stream is not muted any more, restore it's volume if
        //                // ringer mode allows it
        //                if (!isStreamAffectedByRingerMode(mStreamType) || mRingerMode == AudioManager::RINGER_MODE_NORMAL) {
        //                    setIndex(mLastAudibleIndex, FALSE);
        //                    sendMsg(mAudioHandler, MSG_SET_SYSTEM_VOLUME, mStreamType, SENDMSG_NOOP, 0, 0,
        //                            VolumeStreamState.this, 0);
        //                }
        //            }
        //        }
        //    }
        //}
        //mDeathHandlers.notify();
    //}

    //return NOERROR;
}

ECode AudioService::VolumeStreamState::VolumeDeathHandler::BinderDied()
{
    //Log.w(TAG, "Volume service client died for stream: "+mStreamType);
    if (mMuteCount != 0) {
        // Reset all active mute requests from this client.
        mMuteCount = 1;
        Mute(FALSE);
    }

    return NOERROR;
}

PInterface AudioService::VolumeStreamState::VolumeDeathHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::VolumeStreamState::VolumeDeathHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::VolumeStreamState::VolumeDeathHandler::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::VolumeStreamState::VolumeDeathHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

Int32 AudioService::VolumeStreamState::MuteCount()
{
    Int32 count = 0;
    Int32 size = mDeathHandlers->GetLength();
    for (Int32 i = 0; i < size; i++) {
        //count += (*mDeathHandlers)[i]->mMuteCount;
    }
    return count;
}

AudioService::VolumeStreamState::VolumeDeathHandler* AudioService::VolumeStreamState::GetDeathHandler(
    /* [in] */ IBinder* cb, 
    /* [in] */ Boolean state) 
{
    //synchronized(mDeathHandlers) {
        VolumeDeathHandler* handler;
        Int32 size = mDeathHandlers->GetLength();
        for (Int32 i = 0; i < size; i++) {
            handler = (*mDeathHandlers)[i];
            if (cb == handler->mICallback) {
                return handler;
            }
        }
        // If this is the first mute request for this client, create a new
        // client death handler. Otherwise, it is an out of sequence unmute request.
        if (state) {
            handler = new VolumeDeathHandler(cb);
        } else {
            //Log.w(TAG, "stream was not muted by this client");
            handler = NULL;
        }
        return handler;
    //}
}


/** Thread that handles native AudioSystem control. */
AudioService::AudioSystemThread::AudioSystemThread() 
{
    //super("AudioService");
}

ECode AudioService::AudioSystemThread::Run() 
{
    // Set this thread up so the handler will work on it
    //Looper.prepare();

    //synchronized(AudioService.this) {
    //    mAudioHandler = new AudioHandler();

    //    // Notify that the handler has been created
    //    AudioService.this.notify();
    //}

    //// Listen for volume change requests that are set by VolumePanel
    //Looper.loop();

    return NOERROR;
}

PInterface AudioService::AudioSystemThread::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::AudioSystemThread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::AudioSystemThread::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::AudioSystemThread::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}


/** Handles internal volume messages in separate volume thread. */
void AudioService::AudioHandler::SetSystemVolume(
    /* [in] */ VolumeStreamState* streamState) 
{

    // Adjust volume
    //SetStreamVolumeIndex(streamState->mStreamType, streamState->mIndex);

    // Apply change to all streams using this one as alias
    Int32 numStreamTypes = AudioSystem::GetNumStreamTypes();
    for (Int32 streamType = numStreamTypes - 1; streamType >= 0; streamType--) {
        /*if (streamType != streamState->mStreamType &&
            (*STREAM_VOLUME_ALIAS)[streamType] == streamState->mStreamType) {
            SetStreamVolumeIndex(streamType, (*mStreamStates)[streamType]->mIndex);
        }*/
    }

    // Post a persist volume msg
    /*SendMsg(mAudioHandler, MSG_PERSIST_VOLUME, streamState->mStreamType,
            SENDMSG_REPLACE, 1, 1, (IInterface*)streamState, PERSIST_DELAY);*/

    //return NOERROR;
}

void AudioService::AudioHandler::PersistVolume(
    /* [in] */ VolumeStreamState* streamState, 
    /* [in] */ Boolean current, 
    /* [in] */ Boolean lastAudible)
{
    /*if (current) {
        System.putInt(mContentResolver, streamState.mVolumeIndexSettingName,
                      (streamState.mIndex + 5)/ 10);
    }
    if (lastAudible) {
        System.putInt(mContentResolver, streamState.mLastAudibleVolumeIndexSettingName,
            (streamState.mLastAudibleIndex + 5) / 10);
    }*/

    //return NOERROR;
}

void AudioService::AudioHandler::PersistRingerMode() 
{
    //System.putInt(mContentResolver, System.MODE_RINGER, mRingerMode);
}

void AudioService::AudioHandler::PersistVibrateSetting()
{
    //System.putInt(mContentResolver, System.VIBRATE_ON, mVibrateSetting);
}

void AudioService::AudioHandler::PlaySoundEffect(
    /* [in] */ Int32 effectType, 
    /* [in] */ Int32 volume) 
{
    //synchronized (mSoundEffectsLock) {
    //    if (mSoundPool == NULL) {
    //        return;
    //    }
    //    Float volFloat;
    //    // use STREAM_MUSIC volume attenuated by 3 dB if volume is not specified by caller
    //    if (volume < 0) {
    //        // Same linear to log conversion as in native AudioSystem::linearToLog() (AudioSystem::cpp)
    //        Float dBPerStep = (Float)((0.5 * 100) / MAX_STREAM_VOLUME[AudioSystem::STREAM_MUSIC]);
    //        Int32 musicVolIndex = (mStreamStates[AudioSystem::STREAM_MUSIC].mIndex + 5) / 10;
    //        Float musicVoldB = dBPerStep * (musicVolIndex - MAX_STREAM_VOLUME[AudioSystem::STREAM_MUSIC]);
    //        volFloat = (Float)Math.pow(10, (musicVoldB - 3)/20);
    //    } else {
    //        volFloat = (Float) volume / 1000.0f;
    //    }

    //    if (SOUND_EFFECT_FILES_MAP[effectType][1] > 0) {
    //        mSoundPool.play(SOUND_EFFECT_FILES_MAP[effectType][1], volFloat, volFloat, 0, 0, 1.0f);
    //    } else {
    //        MediaPlayer mediaPlayer = new MediaPlayer();
    //        if (mediaPlayer != NULL) {
    //            try {
    //                String filePath = Environment.getRootDirectory() + SOUND_EFFECTS_PATH + SOUND_EFFECT_FILES[SOUND_EFFECT_FILES_MAP[effectType][0]];
    //                mediaPlayer.setDataSource(filePath);
    //                mediaPlayer.setAudioStreamType(AudioSystem::STREAM_SYSTEM);
    //                mediaPlayer.prepare();
    //                mediaPlayer.setVolume(volFloat, volFloat);
    //                mediaPlayer.setOnCompletionListener(new OnCompletionListener() {
    //                    public void onCompletion(MediaPlayer mp) {
    //                        cleanupPlayer(mp);
    //                    }
    //                });
    //                mediaPlayer.setOnErrorListener(new OnErrorListener() {
    //                    public Boolean onError(MediaPlayer mp, Int32 what, Int32 extra) {
    //                        cleanupPlayer(mp);
    //                        return TRUE;
    //                    }
    //                });
    //                mediaPlayer.start();
    //            } catch (IOException ex) {
    //                Log.w(TAG, "MediaPlayer IOException: "+ex);
    //            } catch (IllegalArgumentException ex) {
    //                Log.w(TAG, "MediaPlayer IllegalArgumentException: "+ex);
    //            } catch (IllegalStateException ex) {
    //                Log.w(TAG, "MediaPlayer IllegalStateException: "+ex);
    //            }
    //        }
    //    }
    //}
}

//void AudioService::AudioHandler::CleanupPlayer(
//    /* [in] */ IMediaPlayer* mp)
//{
//    if (mp != NULL) {
//        try {
//            mp.stop();
//            mp.release();
//        } catch (IllegalStateException ex) {
//            Log.w(TAG, "MediaPlayer IllegalStateException: "+ex);
//        }
//    }
//}

//ECode AudioService::AudioHandler::HandleMessage(
//    /* [in] */ IMessage* msg)
//{
    //Int32 baseMsgWhat = GetMsgBase(msg->what);

    //switch (baseMsgWhat) {

    //    case MSG_SET_SYSTEM_VOLUME:
    //        setSystemVolume((VolumeStreamState) msg.obj);
    //        break;

    //    case MSG_PERSIST_VOLUME:
    //        persistVolume((VolumeStreamState) msg.obj, (msg.arg1 != 0), (msg.arg2 != 0));
    //        break;

    //    case MSG_PERSIST_RINGER_MODE:
    //        persistRingerMode();
    //        break;

    //    case MSG_PERSIST_VIBRATE_SETTING:
    //        persistVibrateSetting();
    //        break;

    //    case MSG_MEDIA_SERVER_DIED:
    //        // Force creation of new IAudioflinger interface
    //        if (!mMediaServerOk) {
    //            Log.e(TAG, "Media server died.");
    //            AudioSystem::isStreamActive(AudioSystem::STREAM_MUSIC);
    //            sendMsg(mAudioHandler, MSG_MEDIA_SERVER_DIED, SHARED_MSG, SENDMSG_NOOP, 0, 0,
    //                    NULL, 500);
    //        }
    //        break;

    //    case MSG_MEDIA_SERVER_STARTED:
    //        Log.e(TAG, "Media server started.");
    //        // indicate to audio HAL that we start the reconfiguration phase after a media
    //        // server crash
    //        // Note that MSG_MEDIA_SERVER_STARTED message is only received when the media server
    //        // process restarts after a crash, not the first time it is started.
    //        AudioSystem::setParameters("restarting=TRUE");

    //        // Restore device connection states
    //        Set set = mConnectedDevices.entrySet();
    //        Iterator i = set.iterator();
    //        while(i.hasNext()){
    //            Map.Entry device = (Map.Entry)i.next();
    //            AudioSystem::setDeviceConnectionState(((Integer)device.getKey()).intValue(),
    //                                                 AudioSystem::DEVICE_STATE_AVAILABLE,
    //                                                 (String)device.getValue());
    //        }

    //        // Restore call state
    //        AudioSystem::setPhoneState(mMode);

    //        // Restore forced usage for communcations and record
    //        AudioSystem::setForceUse(AudioSystem::FOR_COMMUNICATION, mForcedUseForComm);
    //        AudioSystem::setForceUse(AudioSystem::FOR_RECORD, mForcedUseForComm);

    //        // Restore stream volumes
    //        Int32 numStreamTypes = AudioSystem::getNumStreamTypes();
    //        for (Int32 streamType = numStreamTypes - 1; streamType >= 0; streamType--) {
    //            Int32 index;
    //            VolumeStreamState streamState = mStreamStates[streamType];
    //            AudioSystem::initStreamVolume(streamType, 0, (streamState.mIndexMax + 5) / 10);
    //            if (streamState.muteCount() == 0) {
    //                index = streamState.mIndex;
    //            } else {
    //                index = 0;
    //            }
    //            setStreamVolumeIndex(streamType, index);
    //        }

    //        // Restore ringer mode
    //        setRingerModeInt(getRingerMode(), FALSE);

    //        // indicate the end of reconfiguration phase to audio HAL
    //        AudioSystem::setParameters("restarting=FALSE");
    //        break;

    //    case MSG_PLAY_SOUND_EFFECT:
    //        playSoundEffect(msg.arg1, msg.arg2);
    //        break;

    //    case MSG_BTA2DP_DOCK_TIMEOUT:
    //        // msg.obj  == address of BTA2DP device
    //        makeA2dpDeviceUnavailableNow( (String) msg.obj );
    //        break;
    //}

    //return NOERROR;
//}

PInterface AudioService::AudioHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::AudioHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::AudioHandler::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::AudioHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

AudioService::SettingsObserver::SettingsObserver() 
{
    /*super(new Handler());
    mContentResolver.registerContentObserver(Settings.System.getUriFor(
        Settings.System.MODE_RINGER_STREAMS_AFFECTED), FALSE, this);
    mContentResolver.registerContentObserver(Settings.System.getUriFor(
            Settings.System.NOTIFICATIONS_USE_RING_VOLUME), FALSE, this);*/
}

ECode AudioService::SettingsObserver::OnChange(
    /* [in] */ Boolean selfChange) 
{
    //super.onChange(selfChange);
    //synchronized (mSettingsLock) {
    //    Int32 ringerModeAffectedStreams = Settings.System.getInt(mContentResolver,
    //            Settings.System.MODE_RINGER_STREAMS_AFFECTED,
    //            0);
    //    if (ringerModeAffectedStreams != mRingerModeAffectedStreams) {
    //        /*
    //         * Ensure all stream types that should be affected by ringer mode
    //         * are in the proper state.
    //         */
    //        mRingerModeAffectedStreams = ringerModeAffectedStreams;
    //        setRingerModeInt(getRingerMode(), FALSE);
    //    }

    //    Int32 notificationsUseRingVolume = Settings.System.getInt(mContentResolver,
    //            Settings.System.NOTIFICATIONS_USE_RING_VOLUME,
    //            1);
    //    if (notificationsUseRingVolume != mNotificationsUseRingVolume) {
    //        mNotificationsUseRingVolume = notificationsUseRingVolume;
    //        if (mNotificationsUseRingVolume == 1) {
    //            STREAM_VOLUME_ALIAS[AudioSystem::STREAM_NOTIFICATION] = AudioSystem::STREAM_RING;
    //            mStreamStates[AudioSystem::STREAM_NOTIFICATION].setVolumeIndexSettingName(
    //                    System.VOLUME_SETTINGS[AudioSystem::STREAM_RING]);
    //        } else {
    //            STREAM_VOLUME_ALIAS[AudioSystem::STREAM_NOTIFICATION] = AudioSystem::STREAM_NOTIFICATION;
    //            mStreamStates[AudioSystem::STREAM_NOTIFICATION].setVolumeIndexSettingName(
    //                    System.VOLUME_SETTINGS[AudioSystem::STREAM_NOTIFICATION]);
    //            // Persist notification volume volume as it was not persisted while aliased to ring volume
    //            //  and persist with no delay as there might be registered observers of the persisted
    //            //  notification volume.
    //            sendMsg(mAudioHandler, MSG_PERSIST_VOLUME, AudioSystem::STREAM_NOTIFICATION,
    //                    SENDMSG_REPLACE, 1, 1, mStreamStates[AudioSystem::STREAM_NOTIFICATION], 0);
    //        }
    //    }
    //}

    return NOERROR;
}

PInterface AudioService::SettingsObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::SettingsObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::SettingsObserver::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::SettingsObserver::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

void AudioService::MakeA2dpDeviceAvailable(
    /* [in] */ String address) 
{
    AudioSystem::SetDeviceConnectionState(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP,
            AudioSystem::DEVICE_STATE_AVAILABLE,
            address);
    // Reset A2DP suspend state each time a new sink is connected
    AudioSystem::SetParameters(String("A2dpSuspended=FALSE"));
    /*mConnectedDevices.put( new Integer(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP),
            address);*/
}

void AudioService::MakeA2dpDeviceUnavailableNow(
    /* [in] */ String address) 
{
    AutoPtr<IIntent> noisyIntent;
    CIntent::New(AudioManager::ACTION_AUDIO_BECOMING_NOISY, (IIntent**)&noisyIntent);
    mContext->SendBroadcast(noisyIntent);
    AudioSystem::SetDeviceConnectionState(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP,
            AudioSystem::DEVICE_STATE_UNAVAILABLE,
            address);
    //mConnectedDevices.remove(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP);
}

void AudioService::MakeA2dpDeviceUnavailableLater(
    /* [in] */ String address)
{
    // prevent any activity on the A2DP audio output to avoid unwanted
    // reconnection of the sink.
    AudioSystem::SetParameters(String("A2dpSuspended=TRUE"));
    // the device will be made unavailable later, so consider it disconnected right away
    //mConnectedDevices->Remove(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP);
    // send the delayed message to make the device unavailable later
    /*AutoPtr<IMessage> msg = mAudioHandler->ObtainMessage(MSG_BTA2DP_DOCK_TIMEOUT, address);
    mAudioHandler->SendMessageDelayed(msg, BTA2DP_DOCK_TIMEOUT_MILLIS);*/

}

void AudioService::CancelA2dpDeviceTimeout() 
{
    //mAudioHandler->RemoveMessages(MSG_BTA2DP_DOCK_TIMEOUT);
}

Boolean AudioService::HasScheduledA2dpDockTimeout()
{
    //return mAudioHandler->HasMessages(MSG_BTA2DP_DOCK_TIMEOUT);
    return FALSE;
}

/**
 * Receiver for misc intent broadcasts the Phone app cares about.
 */
ECode AudioService::AudioServiceBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context, 
    /* [in] */ IIntent* intent)
{
    //String action;
    //intent->GetAction(&action);

    //if (action.Equals(Intent_ACTION_DOCK_EVENT)) {
    //    Int32 dockState = intent->GetIntExtra(Intent_EXTRA_DOCK_STATE,
    //            Intent_EXTRA_DOCK_STATE_UNDOCKED);
    //    Int32 config;
    //    switch (dockState) {
    //        case Intent_EXTRA_DOCK_STATE_DESK:
    //            config = AudioSystem::FORCE_BT_DESK_DOCK;
    //            break;
    //        case Intent_EXTRA_DOCK_STATE_CAR:
    //            config = AudioSystem::FORCE_BT_CAR_DOCK;
    //            break;
    //        case Intent_EXTRA_DOCK_STATE_UNDOCKED:
    //        default:
    //            config = AudioSystem::FORCE_NONE;
    //    }
    //    AudioSystem::SetForceUse(AudioSystem::FOR_DOCK, config);
    //} else if (action.equals(BluetoothA2dp.ACTION_SINK_STATE_CHANGED)) {
    //    Int32 state = intent.getIntExtra(BluetoothA2dp.EXTRA_SINK_STATE,
    //                                   BluetoothA2dp.STATE_DISCONNECTED);
    //    BluetoothDevice btDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
    //    String address = btDevice.getAddress();
    //    Boolean isConnected = (mConnectedDevices.containsKey(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP) &&
    //                           ((String)mConnectedDevices.get(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP)).equals(address));

    //    if (isConnected &&
    //        state != BluetoothA2dp.STATE_CONNECTED && state != BluetoothA2dp.STATE_PLAYING) {
    //        if (btDevice.isBluetoothDock()) {
    //            if (state == BluetoothA2dp.STATE_DISCONNECTED) {
    //                // introduction of a delay for transient disconnections of docks when
    //                // power is rapidly turned off/on, this message will be canceled if
    //                // we reconnect the dock under a preset delay
    //                makeA2dpDeviceUnavailableLater(address);
    //                // the next time isConnected is evaluated, it will be FALSE for the dock
    //            }
    //        } else {
    //            makeA2dpDeviceUnavailableNow(address);
    //        }
    //    } else if (!isConnected &&
    //                 (state == BluetoothA2dp.STATE_CONNECTED ||
    //                  state == BluetoothA2dp.STATE_PLAYING)) {
    //        if (btDevice.isBluetoothDock()) {
    //            // this could be a reconnection after a transient disconnection
    //            cancelA2dpDeviceTimeout();
    //            mDockAddress = address;
    //        } else {
    //            // this could be a connection of another A2DP device before the timeout of
    //            // a dock: cancel the dock timeout, and make the dock unavailable now
    //            if(hasScheduledA2dpDockTimeout()) {
    //                cancelA2dpDeviceTimeout();
    //                makeA2dpDeviceUnavailableNow(mDockAddress);
    //            }
    //        }
    //        makeA2dpDeviceAvailable(address);
    //    }
    //} else if (action.equals(BluetoothHeadset.ACTION_STATE_CHANGED)) {
    //    Int32 state = intent.getIntExtra(BluetoothHeadset.EXTRA_STATE,
    //                                   BluetoothHeadset.STATE_ERROR);
    //    Int32 device = AudioSystem::DEVICE_OUT_BLUETOOTH_SCO;
    //    BluetoothDevice btDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
    //    String address = NULL;
    //    if (btDevice != NULL) {
    //        address = btDevice.getAddress();
    //        BluetoothClass btClass = btDevice.getBluetoothClass();
    //        if (btClass != NULL) {
    //            switch (btClass.getDeviceClass()) {
    //            case BluetoothClass.Device.AUDIO_VIDEO_WEARABLE_HEADSET:
    //            case BluetoothClass.Device.AUDIO_VIDEO_HANDSFREE:
    //                device = AudioSystem::DEVICE_OUT_BLUETOOTH_SCO_HEADSET;
    //                break;
    //            case BluetoothClass.Device.AUDIO_VIDEO_CAR_AUDIO:
    //                device = AudioSystem::DEVICE_OUT_BLUETOOTH_SCO_CARKIT;
    //                break;
    //            }
    //        }
    //    }

    //    Boolean isConnected = (mConnectedDevices.containsKey(device) &&
    //                           ((String)mConnectedDevices.get(device)).equals(address));

    //    if (isConnected && state != BluetoothHeadset.STATE_CONNECTED) {
    //        AudioSystem::setDeviceConnectionState(device,
    //                                             AudioSystem::DEVICE_STATE_UNAVAILABLE,
    //                                             address);
    //        mConnectedDevices.remove(device);
    //        mBluetoothHeadsetConnected = FALSE;
    //        clearAllScoClients();
    //    } else if (!isConnected && state == BluetoothHeadset.STATE_CONNECTED) {
    //        AudioSystem::setDeviceConnectionState(device,
    //                                             AudioSystem::DEVICE_STATE_AVAILABLE,
    //                                             address);
    //        mConnectedDevices.put(new Integer(device), address);
    //        mBluetoothHeadsetConnected = TRUE;
    //    }
    //} else if (action.equals(Intent.ACTION_HEADSET_PLUG)) {
    //    Int32 state = intent.getIntExtra("state", 0);
    //    Int32 microphone = intent.getIntExtra("microphone", 0);

    //    if (microphone != 0) {
    //        Boolean isConnected = mConnectedDevices.containsKey(AudioSystem::DEVICE_OUT_WIRED_HEADSET);
    //        if (state == 0 && isConnected) {
    //            AudioSystem::setDeviceConnectionState(AudioSystem::DEVICE_OUT_WIRED_HEADSET,
    //                    AudioSystem::DEVICE_STATE_UNAVAILABLE,
    //                    "");
    //            mConnectedDevices.remove(AudioSystem::DEVICE_OUT_WIRED_HEADSET);
    //        } else if (state == 1 && !isConnected)  {
    //            AudioSystem::setDeviceConnectionState(AudioSystem::DEVICE_OUT_WIRED_HEADSET,
    //                    AudioSystem::DEVICE_STATE_AVAILABLE,
    //                    "");
    //            mConnectedDevices.put( new Integer(AudioSystem::DEVICE_OUT_WIRED_HEADSET), "");
    //        }
    //    } else {
    //        Boolean isConnected = mConnectedDevices.containsKey(AudioSystem::DEVICE_OUT_WIRED_HEADPHONE);
    //        if (state == 0 && isConnected) {
    //            AudioSystem::setDeviceConnectionState(AudioSystem::DEVICE_OUT_WIRED_HEADPHONE,
    //                    AudioSystem::DEVICE_STATE_UNAVAILABLE,
    //                    "");
    //            mConnectedDevices.remove(AudioSystem::DEVICE_OUT_WIRED_HEADPHONE);
    //        } else if (state == 1 && !isConnected)  {
    //            AudioSystem::setDeviceConnectionState(AudioSystem::DEVICE_OUT_WIRED_HEADPHONE,
    //                    AudioSystem::DEVICE_STATE_AVAILABLE,
    //                    "");
    //            mConnectedDevices.put( new Integer(AudioSystem::DEVICE_OUT_WIRED_HEADPHONE), "");
    //        }
    //    }
    //} else if (action.equals(BluetoothHeadset.ACTION_AUDIO_STATE_CHANGED)) {
    //    Int32 state = intent.getIntExtra(BluetoothHeadset.EXTRA_AUDIO_STATE,
    //                                   BluetoothHeadset.STATE_ERROR);
    //    synchronized (mScoClients) {
    //        if (!mScoClients.isEmpty()) {
    //            switch (state) {
    //            case BluetoothHeadset.AUDIO_STATE_CONNECTED:
    //                state = AudioManager::SCO_AUDIO_STATE_CONNECTED;
    //                break;
    //            case BluetoothHeadset.AUDIO_STATE_DISCONNECTED:
    //                state = AudioManager::SCO_AUDIO_STATE_DISCONNECTED;
    //                break;
    //            default:
    //                state = AudioManager::SCO_AUDIO_STATE_ERROR;
    //                break;
    //            }
    //            if (state != AudioManager::SCO_AUDIO_STATE_ERROR) {
    //                Intent newIntent = new Intent(AudioManager::ACTION_SCO_AUDIO_STATE_CHANGED);
    //                newIntent.putExtra(AudioManager::EXTRA_SCO_AUDIO_STATE, state);
    //                mContext.sendStickyBroadcast(newIntent);
    //            }
    //        }
    //    }
    //}

    return NOERROR;
}

PInterface AudioService::AudioServiceBroadcastReceiver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::AudioServiceBroadcastReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::AudioServiceBroadcastReceiver::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::AudioServiceBroadcastReceiver::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

//==========================================================================================
// AudioFocus
//==========================================================================================

void AudioService::NotifyTopOfAudioFocusStack()
{
    // notify the top of the stack it gained focus
    /*if (!mFocusStack.empty() && (mFocusStack.peek().mFocusDispatcher != NULL)) {
        if (canReassignAudioFocus()) {
            try {
                mFocusStack.peek().mFocusDispatcher.dispatchAudioFocusChange(
                        AudioManager::AUDIOFOCUS_GAIN, mFocusStack.peek().mClientId);
            } catch (RemoteException e) {
                Log.e(TAG, "Failure to signal gain of audio control focus due to "+ e);
                e.printStackTrace();
            }
        }
    }*/
}


AudioService::FocusStackEntry::FocusStackEntry() 
{
}

AudioService::FocusStackEntry::FocusStackEntry(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 duration, 
    /* [in] */ Boolean isTransportControlReceiver,
    /* [in] */ IAudioFocusDispatcher* afl, 
    /* [in] */ IBinder* source, String id)
{
    mStreamType = streamType;
    mIsTransportControlReceiver = isTransportControlReceiver;
    mFocusDispatcher = afl;
    mSourceRef = source;
    mClientId = id;
    mFocusChangeType = duration;
}


/**
 * Helper function:
 * Display in the log the current entries in the audio focus stack
 */
//void AudioService::DumpFocusStack(
//    /* [in] */ PrintWriter pw)
//{
//    /*pw.println("\nAudio Focus stack entries:");
//    synchronized(mAudioFocusLock) {
//        Iterator<FocusStackEntry> stackIterator = mFocusStack.iterator();
//        while(stackIterator.hasNext()) {
//            FocusStackEntry fse = stackIterator.next();
//            pw.println("     source:" + fse.mSourceRef + " -- client: " + fse.mClientId
//                    + " -- duration: " +fse.mFocusChangeType);
//        }
//    }*/
//}

/**
 * Helper function:
 * Remove a focus listener from the focus stack.
 * @param focusListenerToRemove the focus listener
 * @param signal if TRUE and the listener was at the top of the focus stack, i.e. it was holding
 *   focus, notify the next item in the stack it gained focus.
 */
void AudioService::RemoveFocusStackEntry(
    /* [in] */ String clientToRemove, 
    /* [in] */ Boolean signal)
{
    // is the current top of the focus stack abandoning focus? (because of death or request)
    //if (!mFocusStack.empty() && mFocusStack.peek().mClientId.equals(clientToRemove))
    //{
    //    //Log.i(TAG, "   removeFocusStackEntry() removing top of stack");
    //    mFocusStack.pop();
    //    if (signal) {
    //        // notify the new top of the stack it gained focus
    //        notifyTopOfAudioFocusStack();
    //    }
    //} else {
    //    // focus is abandoned by a client that's not at the top of the stack,
    //    // no need to update focus.
    //    Iterator<FocusStackEntry> stackIterator = mFocusStack.iterator();
    //    while(stackIterator.hasNext()) {
    //        FocusStackEntry fse = (FocusStackEntry)stackIterator.next();
    //        if(fse.mClientId.equals(clientToRemove)) {
    //            Log.i(TAG, " AudioFocus  abandonAudioFocus(): removing entry for "
    //                    + fse.mClientId);
    //            mFocusStack.remove(fse);
    //        }
    //    }
    //}
}

/**
 * Helper function:
 * Remove focus listeners from the focus stack for a particular client.
 */
void AudioService::RemoveFocusStackEntryForClient(
    /* [in] */ IBinder* cb)
{
    // is the owner of the audio focus part of the client to remove?
    //Boolean isTopOfStackForClientToRemove = !mFocusStack.isEmpty() &&
    //        mFocusStack.peek().mSourceRef.equals(cb);
    //Iterator<FocusStackEntry> stackIterator = mFocusStack.iterator();
    //while(stackIterator.hasNext()) {
    //    FocusStackEntry fse = (FocusStackEntry)stackIterator.next();
    //    if(fse.mSourceRef.equals(cb)) {
    //        Log.i(TAG, " AudioFocus  abandonAudioFocus(): removing entry for "
    //                + fse.mClientId);
    //        mFocusStack.remove(fse);
    //    }
    //}
    //if (isTopOfStackForClientToRemove) {
    //    // we removed an entry at the top of the stack:
    //    //  notify the new top of the stack it gained focus.
    //    notifyTopOfAudioFocusStack();
    //}
}

/**
 * Helper function:
 * Returns TRUE if the system is in a state where the focus can be reevaluated, FALSE otherwise.
 */
Boolean AudioService::CanReassignAudioFocus()
{
    // focus requests are rejected during a phone call or when the phone is ringing
    // this is equivalent to IN_VOICE_COMM_FOCUS_ID having the focus
    /*if (!mFocusStack.isEmpty() && IN_VOICE_COMM_FOCUS_ID.equals(mFocusStack.peek().mClientId)) {
        return FALSE;
    }*/
    return TRUE;
}

/**
 * Inner class to monitor audio focus client deaths, and remove them from the audio focus
 * stack if necessary.
 */

AudioService::AudioFocusDeathHandler::AudioFocusDeathHandler(
    /* [in] */ IBinder* cb)
{
    mCb = cb;
}

ECode AudioService::AudioFocusDeathHandler::BinderDied()
{
    /*synchronized(mAudioFocusLock) {
        Log.w(TAG, "  AudioFocus   audio focus client died");
        removeFocusStackEntryForClient(mCb);
    }*/

    return NOERROR;
}

AutoPtr<IBinder> AudioService::AudioFocusDeathHandler::GetBinder() 
{
    return mCb;
}

PInterface AudioService::AudioFocusDeathHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::AudioFocusDeathHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::AudioFocusDeathHandler::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::AudioFocusDeathHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

/** @see AudioManager#requestAudioFocus(IAudioFocusDispatcher, Int32, Int32) */
Int32 AudioService::RequestAudioFocus(
    /* [in] */ Int32 mainStreamType, 
    /* [in] */ Int32 focusChangeHint, 
    /* [in] */ IBinder* cb,
    /* [in] */ IAudioFocusDispatcher* fd, 
    /* [in] */ String clientId) 
{
    //Log.i(TAG, " AudioFocus  requestAudioFocus() from " + clientId);
    //// the main stream type for the audio focus request is currently not used. It may
    //// potentially be used to handle multiple stream type-dependent audio focuses.

    //// we need a valid binder callback for clients other than the AudioService's phone
    //// state listener
    //if (!IN_VOICE_COMM_FOCUS_ID.equals(clientId) && ((cb == NULL) || !cb.pingBinder())) {
    //    Log.i(TAG, " AudioFocus  DOA client for requestAudioFocus(), exiting");
    //    return AudioManager::AUDIOFOCUS_REQUEST_FAILED;
    //}

    //synchronized(mAudioFocusLock) {
    //    if (!canReassignAudioFocus()) {
    //        return AudioManager::AUDIOFOCUS_REQUEST_FAILED;
    //    }

    //    if (!mFocusStack.empty() && mFocusStack.peek().mClientId.equals(clientId)) {
    //        // if focus is already owned by this client and the reason for acquiring the focus
    //        // hasn't changed, don't do anything
    //        if (mFocusStack.peek().mFocusChangeType == focusChangeHint) {
    //            return AudioManager::AUDIOFOCUS_REQUEST_GRANTED;
    //        }
    //        // the reason for the audio focus request has changed: remove the current top of
    //        // stack and respond as if we had a new focus owner
    //        mFocusStack.pop();
    //    }

    //    // notify current top of stack it is losing focus
    //    if (!mFocusStack.empty() && (mFocusStack.peek().mFocusDispatcher != NULL)) {
    //        try {
    //            mFocusStack.peek().mFocusDispatcher.dispatchAudioFocusChange(
    //                    -1 * focusChangeHint, // loss and gain codes are inverse of each other
    //                    mFocusStack.peek().mClientId);
    //        } catch (RemoteException e) {
    //            Log.e(TAG, " Failure to signal loss of focus due to "+ e);
    //            e.printStackTrace();
    //        }
    //    }

    //    // focus requester might already be somewhere below in the stack, remove it
    //    removeFocusStackEntry(clientId, FALSE);

    //    // push focus requester at the top of the audio focus stack
    //    mFocusStack.push(new FocusStackEntry(mainStreamType, focusChangeHint, FALSE, fd, cb,
    //            clientId));
    //}//synchronized(mAudioFocusLock)

    //// handle the potential premature death of the new holder of the focus
    //// (premature death == death before abandoning focus) for a client which is not the
    //// AudioService's phone state listener
    //if (!IN_VOICE_COMM_FOCUS_ID.equals(clientId)) {
    //    // Register for client death notification
    //    AudioFocusDeathHandler afdh = new AudioFocusDeathHandler(cb);
    //    try {
    //        cb.linkToDeath(afdh, 0);
    //    } catch (RemoteException e) {
    //        // client has already died!
    //        Log.w(TAG, "AudioFocus  requestAudioFocus() could not link to "+cb+" binder death");
    //    }
    //}

    return AudioManager::AUDIOFOCUS_REQUEST_GRANTED;
}

/** @see AudioManager#abandonAudioFocus(IAudioFocusDispatcher) */
Int32 AudioService::AbandonAudioFocus(
    /* [in] */ IAudioFocusDispatcher* fl, 
    /* [in] */ String clientId)
{
    //Log.i(TAG, " AudioFocus  abandonAudioFocus() from " + clientId);
    //try {
    //    // this will take care of notifying the new focus owner if needed
    //    synchronized(mAudioFocusLock) {
    //        removeFocusStackEntry(clientId, TRUE);
    //    }
    //} catch (java.util.ConcurrentModificationException cme) {
    //    // Catching this exception here is temporary. It is here just to prevent
    //    // a crash seen when the "Silent" notification is played. This is believed to be fixed
    //    // but this try catch block is left just to be safe.
    //    Log.e(TAG, "FATAL EXCEPTION AudioFocus  abandonAudioFocus() caused " + cme);
    //    cme.printStackTrace();
    //}

    return AudioManager::AUDIOFOCUS_REQUEST_GRANTED;
}

ECode AudioService::UnregisterAudioFocusClient(
    /* [in] */ String clientId)
{
    /*synchronized(mAudioFocusLock) {
        removeFocusStackEntry(clientId, FALSE);
    }*/

    return NOERROR;
}


//==========================================================================================
// RemoteControl
//==========================================================================================
/**
 * Receiver for media button intents. Handles the dispatching of the media button event
 * to one of the registered listeners, or if there was none, resumes the intent broadcast
 * to the rest of the system.
 */

ECode AudioService::MediaButtonBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context, 
    /* [in] */ IIntent* intent) 
{
    String action;
    intent->GetAction(&action);
    /*if (!Intent_ACTION_MEDIA_BUTTON.equals(action)) {
        return;
    }*/
    //KeyEvent event = (KeyEvent) intent.getParcelableExtra(Intent.EXTRA_KEY_EVENT);
    //if (event != NULL) {
    //    // if in a call or ringing, do not break the current phone app behavior
    //    // TODO modify this to let the phone app specifically get the RC focus
    //    //      add modify the phone app to take advantage of the new API
    //    synchronized(mRingingLock) {
    //        if (mIsRinging || (getMode() == AudioSystem::MODE_IN_CALL) ||
    //                (getMode() == AudioSystem::MODE_IN_COMMUNICATION) ||
    //                (getMode() == AudioSystem::MODE_RINGTONE) ) {
    //            return;
    //        }
    //    }
    //    synchronized(mRCStack) {
    //        if (!mRCStack.empty()) {
    //            // create a new intent specifically aimed at the current registered listener
    //            Intent targetedIntent = new Intent(Intent.ACTION_MEDIA_BUTTON);
    //            targetedIntent.putExtras(intent.getExtras());
    //            targetedIntent.setComponent(mRCStack.peek().mReceiverComponent);
    //            // trap the current broadcast
    //            abortBroadcast();
    //            //Log.v(TAG, " Sending intent" + targetedIntent);
    //            context.sendBroadcast(targetedIntent, NULL);
    //        }
    //    }
    //}

    return NOERROR;
}

PInterface AudioService::MediaButtonBroadcastReceiver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioService::MediaButtonBroadcastReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioService::MediaButtonBroadcastReceiver::Release()
{
    return ElRefBase::Release();
}

ECode AudioService::MediaButtonBroadcastReceiver::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

AudioService::RemoteControlStackEntry::RemoteControlStackEntry()
{
}

AudioService::RemoteControlStackEntry::RemoteControlStackEntry(
    /* [in] */ IComponentName* r) 
{
    mReceiverComponent = r;
}

/**
 * Helper function:
 * Display in the log the current entries in the remote control focus stack
 */
//void AudioService::DumpRCStack(
//    /* [in] */ PrintWriter pw) 
//{
//    /*pw.println("\nRemote Control stack entries:");
//    synchronized(mRCStack) {
//        Iterator<RemoteControlStackEntry> stackIterator = mRCStack.iterator();
//        while(stackIterator.hasNext()) {
//            RemoteControlStackEntry fse = stackIterator.next();
//            pw.println("     receiver:" + fse.mReceiverComponent);
//        }
//    }*/
//}

/**
 * Helper function:
 * Set the new remote control receiver at the top of the RC focus stack
 */
void AudioService::PushMediaButtonReceiver(
    /* [in] */ IComponentName* newReceiver)
{
    // already at top of stack?
    /*if (!mRCStack.empty() && mRCStack.peek().mReceiverComponent.equals(newReceiver)) {
        return;
    }
    Iterator<RemoteControlStackEntry> stackIterator = mRCStack.iterator();
    while(stackIterator.hasNext()) {
        RemoteControlStackEntry rcse = (RemoteControlStackEntry)stackIterator.next();
        if(rcse.mReceiverComponent.equals(newReceiver)) {
            mRCStack.remove(rcse);
            break;
        }
    }
    mRCStack.push(new RemoteControlStackEntry(newReceiver));*/
}

/**
 * Helper function:
 * Remove the remote control receiver from the RC focus stack
 */
void AudioService::RemoveMediaButtonReceiver(
    /* [in] */ IComponentName* newReceiver)
{
    /*Iterator<RemoteControlStackEntry> stackIterator = mRCStack.iterator();
    while(stackIterator.hasNext()) {
        RemoteControlStackEntry rcse = (RemoteControlStackEntry)stackIterator.next();
        if(rcse.mReceiverComponent.equals(newReceiver)) {
            mRCStack.remove(rcse);
            break;
        }
    }*/
}


/** see AudioManager::registerMediaButtonEventReceiver(ComponentName eventReceiver) */
ECode AudioService::RegisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    //Log.i(TAG, "  Remote Control   registerMediaButtonEventReceiver() for " + eventReceiver);

    //synchronized(mRCStack) {
        PushMediaButtonReceiver(eventReceiver);
    //}

        return NOERROR;
}

/** see AudioManager::unregisterMediaButtonEventReceiver(ComponentName eventReceiver) */
ECode AudioService::UnregisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver) 
{
    //Log.i(TAG, "  Remote Control   unregisterMediaButtonEventReceiver() for " + eventReceiver);

    //synchronized(mRCStack) {
        RemoveMediaButtonReceiver(eventReceiver);
    //}

        return NOERROR;
}


//void AudioService::Dump(
//    /* [in] */ FileDescriptor fd, 
//    /* [in] */ PrintWriter pw, 
//    /* [in] */ String[] args) 
//{
//    // TODO probably a lot more to do here than just the audio focus and remote control stacks
//    DumpFocusStack(pw);
//    DumpRCStack(pw);
//}

