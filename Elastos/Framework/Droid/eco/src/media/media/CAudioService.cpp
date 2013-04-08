
#include "media/CAudioService.h"
#include "media/ElAudioSystem.h"
#include "media/CAudioManager.h"
#include "media/CSoundPool.h"
#include "os/SystemProperties.h"


CAudioService::VolumeStreamState::VolumeDeathHandler::VolumeDeathHandler(
    /* [in] */ IBinder* cb,
    /* [in] */ CAudioService::VolumeStreamState* host)
    : mICallback(cb)
    , mHost(host)
{}

PInterface CAudioService::VolumeStreamState::VolumeDeathHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBinderDeathRecipient*)this;
    }
    else if (riid == EIID_IBinderDeathRecipient) {
        return (IBinderDeathRecipient*)this;
    }

    return NULL;
}

UInt32 CAudioService::VolumeStreamState::VolumeDeathHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::VolumeStreamState::VolumeDeathHandler::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::VolumeStreamState::VolumeDeathHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IBinderDeathRecipient*)this) {
        *pIID = EIID_IBinderDeathRecipient;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

void CAudioService::VolumeStreamState::VolumeDeathHandler::Mute(
    /* [in] */ Boolean state)
{
    Mutex::Autolock lock(&mHost->mDeathHandlersLock);

    //if (state) {
    //    if (mMuteCount == 0) {
    //        // Register for client death notification
    //        //try {
    //            // mICallback can be 0 if muted by CAudioService
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
    //            // mICallback can be 0 if muted by CAudioService
    //            if (mICallback != NULL) {
    //                mICallback.unlinkToDeath(this, 0);
    //            }
    //            if (muteCount() == 0) {
    //                // If the stream is not muted any more, restore it's volume if
    //                // ringer mode allows it
    //                if (!isStreamAffectedByRingerMode(mStreamType) || mRingerMode == AudioManager_RINGER_MODE_NORMAL) {
    //                    setIndex(mLastAudibleIndex, FALSE);
    //                    sendMsg(mAudioHandler, MSG_SET_SYSTEM_VOLUME, mStreamType, SENDMSG_NOOP, 0, 0,
    //                            VolumeStreamState.this, 0);
    //                }
    //            }
    //        }
    //    }
    //}
    //mDeathHandlers.notify();
}

ECode CAudioService::VolumeStreamState::VolumeDeathHandler::BinderDied()
{
    //Log.w(TAG, "Volume service client died for stream: "+mStreamType);
    if (mMuteCount != 0) {
        // Reset all active mute requests from this client.
        mMuteCount = 1;
        Mute(FALSE);
    }

    return NOERROR;
}


CAudioService::VolumeStreamState::VolumeStreamState(
    /* [in] */ const String& settingName,
    /* [in] */ Int32 streamType,
    /* [in] */ CAudioService* host)
    : mHost(host)
{
    SetVolumeIndexSettingName(settingName);

    mStreamType = streamType;

    //AutoPtr<IContentResolver> cr = mContentResolver;
    //mIndexMax = (*MAX_STREAM_VOLUME)[streamType];
    /*mIndex = Settings.System.getInt(cr,
                                    mVolumeIndexSettingName,
                                    AudioManager_DEFAULT_STREAM_VOLUME[streamType]);
    mLastAudibleIndex = Settings.System.getInt(cr,
                                               mLastAudibleVolumeIndexSettingName,
                                               (mIndex > 0) ? mIndex : AudioManager_DEFAULT_STREAM_VOLUME[streamType]);*/
    AudioSystem::InitStreamVolume(streamType, 0, mIndexMax);
    mIndexMax *= 10;
    mIndex = GetValidIndex(10 * mIndex);
    mLastAudibleIndex = GetValidIndex(10 * mLastAudibleIndex);
    mHost->SetStreamVolumeIndex(streamType, mIndex);
}

void CAudioService::VolumeStreamState::SetVolumeIndexSettingName(
    /* [in] */ const String& settingName)
{
    mVolumeIndexSettingName = settingName;
    //mLastAudibleVolumeIndexSettingName = settingName + System.APPEND_FOR_LAST_AUDIBLE;
}

Boolean CAudioService::VolumeStreamState::AdjustIndex(
    /* [in] */ Int32 deltaIndex)
{
    return SetIndex(mIndex + deltaIndex * 10, TRUE);
}

Boolean CAudioService::VolumeStreamState::SetIndex(
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
    }
    else {
        return FALSE;
    }
}

void CAudioService::VolumeStreamState::SetLastAudibleIndex(
    /* [in] */ Int32 index)
{
    mLastAudibleIndex = GetValidIndex(index);
}

void CAudioService::VolumeStreamState::AdjustLastAudibleIndex(
    /* [in] */ Int32 deltaIndex)
{
    SetLastAudibleIndex(mLastAudibleIndex + deltaIndex * 10);
}

Int32 CAudioService::VolumeStreamState::GetMaxIndex()
{
    return mIndexMax;
}

void CAudioService::VolumeStreamState::Mute(
    /* [in] */ IBinder* cb,
    /* [in] */ Boolean state)
{
    AutoPtr<VolumeDeathHandler> handler = GetDeathHandler(cb, state);
    if (handler == NULL) {
        //Log.e(TAG, "Could not get client death handler for stream: "+mStreamType);
        //return NOERROR;
    }
    handler->Mute(state);
}

Int32 CAudioService::VolumeStreamState::GetValidIndex(
    /* [in] */ Int32 index)
{
    if (index < 0) {
        return 0;
    }
    else if (index > mIndexMax) {
        return mIndexMax;
    }

    return index;
}

Int32 CAudioService::VolumeStreamState::MuteCount()
{
    Int32 count = 0;
    List< AutoPtr<VolumeDeathHandler> >::Iterator it;
    for (it = mDeathHandlers.Begin(); it != mDeathHandlers.End(); ++it) {
        //count += (*mDeathHandlers)[i]->mMuteCount;
    }
    return count;
}

AutoPtr<CAudioService::VolumeStreamState::VolumeDeathHandler> CAudioService::VolumeStreamState::GetDeathHandler(
    /* [in] */ IBinder* cb,
    /* [in] */ Boolean state)
{
    Mutex::Autolock lock(&mDeathHandlersLock);

    AutoPtr<VolumeDeathHandler> handler;
    List< AutoPtr<VolumeDeathHandler> >::Iterator it;
    for (it = mDeathHandlers.Begin(); it != mDeathHandlers.End(); ++it) {
        handler = *it;
        if (cb == handler->mICallback) {
            return handler;
        }
    }
    // If this is the first mute request for this client, create a new
    // client death handler. Otherwise, it is an out of sequence unmute request.
    if (state) {
        handler = new VolumeDeathHandler(cb, this);
    }
    else {
        //Log.w(TAG, "stream was not muted by this client");
        handler = NULL;
    }
    return handler;
}


CAudioService::SetModeDeathHandler::SetModeDeathHandler(
    /* [in] */ IBinder* cb,
    /* [in] */ CAudioService* host)
    : mCb(cb)
    , mMode(AudioSystem::MODE_NORMAL) // Current mode set by this client
    , mHost(host)
{}

PInterface CAudioService::SetModeDeathHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBinderDeathRecipient*)this;
    }
    else if (riid == EIID_IBinderDeathRecipient) {
        return (IBinderDeathRecipient*)this;
    }

    return NULL;
}

UInt32 CAudioService::SetModeDeathHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::SetModeDeathHandler::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::SetModeDeathHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IBinderDeathRecipient*)this) {
        *pIID = EIID_IBinderDeathRecipient;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::SetModeDeathHandler::BinderDied()
{
    Mutex::Autolock lock(&mHost->mSetModeDeathHandlersLock);

    //Log.w(TAG, "setMode() client died");
    List< AutoPtr<SetModeDeathHandler> >::Iterator it =
            Find(mHost->mSetModeDeathHandlers.Begin(), mHost->mSetModeDeathHandlers.End(),
            AutoPtr<SetModeDeathHandler>(this));
    if (it == mHost->mSetModeDeathHandlers.End()) {
        //Log.w(TAG, "unregistered setMode() client died");
    }
    else if (it == mHost->mSetModeDeathHandlers.Begin()) {
        mHost->mSetModeDeathHandlers.Erase(it);
        // If dead client was a the top of client list,
        // apply next mode in the stack
        // mSetModeDeathHandlers is never empty as the initial entry
        // created when CAudioService starts is never removed
        Int32 mode = (*mHost->mSetModeDeathHandlers.Begin())->GetMode();
        if (mHost->mMode != mode) {
            if (AudioSystem::SetPhoneState(mode) == AudioSystem::AUDIO_STATUS_OK) {
                mHost->mMode = mode;
            }
        }
    }
    else {
        mHost->mSetModeDeathHandlers.Erase(it);
    }

    return NOERROR;
}

void CAudioService::SetModeDeathHandler::SetMode(
    /* [in] */ Int32 mode)
{
    mMode = mode;
}

Int32 CAudioService::SetModeDeathHandler::GetMode()
{
    return mMode;
}

AutoPtr<IBinder> CAudioService::SetModeDeathHandler::GetBinder()
{
    return mCb;
}


CAudioService::ScoClient::ScoClient(
    /* [in] */ IBinder* cb,
    /* [in] */ CAudioService* host)
    : mCb(cb)
    , mStartcount(0)
    , mHost(host)
{}

PInterface CAudioService::ScoClient::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBinderDeathRecipient*)this;
    }
    else if (riid == EIID_IBinderDeathRecipient) {
        return (IBinderDeathRecipient*)this;
    }

    return NULL;
}

UInt32 CAudioService::ScoClient::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::ScoClient::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::ScoClient::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IBinderDeathRecipient*)this) {
        *pIID = EIID_IBinderDeathRecipient;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::ScoClient::BinderDied()
{
    Mutex::Autolock lock(&mHost->mScoClientsLock);

    // Log.w(TAG, "SCO client died");
    List< AutoPtr<ScoClient> >::Iterator it =
            Find(mHost->mScoClients.Begin(), mHost->mScoClients.End(), this);
    if (it == mHost->mScoClients.End()) {
        // Log.w(TAG, "unregistered SCO client died");
    }
    else {
        ClearCount(TRUE);
        mHost->mScoClients.Erase(it);
    }

    return NOERROR;
}

void CAudioService::ScoClient::IncCount()
{
    Mutex::Autolock lock(&mHost->mScoClientsLock);

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
}

void CAudioService::ScoClient::DecCount()
{
    Mutex::Autolock lock(&mHost->mScoClientsLock);

    // if (mStartcount == 0) {
    //     Log.w(TAG, "ScoClient.decCount() already 0");
    // } else {
    //     mStartcount--;
    //     if (mStartcount == 0) {
    //         try {
    //             mCb.unlinkToDeath(this, 0);
    //         } catch (NoSuchElementException e) {
    //             Log.w(TAG, "decCount() going to 0 but not registered to binder");
    //         }
    //     }
    //     requestScoState(BluetoothHeadset.AUDIO_STATE_DISCONNECTED);
    // }
}

void CAudioService::ScoClient::ClearCount(
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
}

Int32 CAudioService::ScoClient::GetCount()
{
    return mStartcount;
}

AutoPtr<IBinder> CAudioService::ScoClient::GetBinder()
{
    return mCb;
}

Int32 CAudioService::ScoClient::TotalCount()
{
    Mutex::Autolock lock(&mHost->mScoClientsLock);

    Int32 count = 0;
    List< AutoPtr<ScoClient> >::Iterator it;
    for (it = mHost->mScoClients.Begin(); it != mHost->mScoClients.End(); ++it) {
        count += (*it)->GetCount();
    }
    return count;
}

void CAudioService::ScoClient::RequestScoState(
    /* [in] */ Int32 state)
{
    assert(0);
    /*if (totalCount() == 0 &&
        mBluetoothHeadsetConnected &&
        CAudioService.this.mMode == AudioSystem::MODE_NORMAL) {
        if (state == BluetoothHeadset.AUDIO_STATE_CONNECTED) {
            mBluetoothHeadset.startVoiceRecognition();
        } else {
            mBluetoothHeadset.stopVoiceRecognition();
        }
    }*/
}

/** Thread that handles native AudioSystem control. */
CAudioService::AudioSystemThread::AudioSystemThread()
{
    //super("AudioService");
}

PInterface CAudioService::AudioSystemThread::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IThread*)this;
    }
    else if (riid == EIID_IThread) {
        return (IThread*)this;
    }

    return NULL;
}

UInt32 CAudioService::AudioSystemThread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::AudioSystemThread::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::AudioSystemThread::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IThread*)this) {
        *pIID = EIID_IThread;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::AudioSystemThread::Run()
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


PInterface CAudioService::AudioHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IApartment*)this;
    }
    else if (riid == EIID_IApartment) {
        return (IApartment*)this;
    }

    return NULL;
}

UInt32 CAudioService::AudioHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::AudioHandler::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::AudioHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IApartment*)this) {
        *pIID = EIID_IApartment;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

/** Handles internal volume messages in separate volume thread. */
void CAudioService::AudioHandler::SetSystemVolume(
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

void CAudioService::AudioHandler::PersistVolume(
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

void CAudioService::AudioHandler::PersistRingerMode()
{
    //System.putInt(mContentResolver, System.MODE_RINGER, mRingerMode);
}

void CAudioService::AudioHandler::PersistVibrateSetting()
{
    //System.putInt(mContentResolver, System.VIBRATE_ON, mVibrateSetting);
}

void CAudioService::AudioHandler::PlaySoundEffect(
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


CAudioService::SettingsObserver::SettingsObserver()
{
    /*super(new Handler());
    mContentResolver.registerContentObserver(Settings.System.getUriFor(
        Settings.System.MODE_RINGER_STREAMS_AFFECTED), FALSE, this);
    mContentResolver.registerContentObserver(Settings.System.getUriFor(
            Settings.System.NOTIFICATIONS_USE_RING_VOLUME), FALSE, this);*/
}

PInterface CAudioService::SettingsObserver::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IContentObserver*)this;
    }
    else if (riid == EIID_IContentObserver) {
        return (IContentObserver*)this;
    }

    return NULL;
}

UInt32 CAudioService::SettingsObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::SettingsObserver::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::SettingsObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IContentObserver*)this) {
        *pIID = EIID_IContentObserver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::SettingsObserver::OnChange(
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


PInterface CAudioService::AudioServiceBroadcastReceiver::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBroadcastReceiver*)this;
    }
    else if (riid == EIID_IBroadcastReceiver) {
        return (IBroadcastReceiver*)this;
    }

    return NULL;
}

UInt32 CAudioService::AudioServiceBroadcastReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::AudioServiceBroadcastReceiver::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::AudioServiceBroadcastReceiver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IBroadcastReceiver*)this) {
        *pIID = EIID_IBroadcastReceiver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::AudioServiceBroadcastReceiver::OnReceive(
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
    //                state = AudioManager_SCO_AUDIO_STATE_CONNECTED;
    //                break;
    //            case BluetoothHeadset.AUDIO_STATE_DISCONNECTED:
    //                state = AudioManager_SCO_AUDIO_STATE_DISCONNECTED;
    //                break;
    //            default:
    //                state = AudioManager_SCO_AUDIO_STATE_ERROR;
    //                break;
    //            }
    //            if (state != AudioManager_SCO_AUDIO_STATE_ERROR) {
    //                Intent newIntent = new Intent(AudioManager_ACTION_SCO_AUDIO_STATE_CHANGED);
    //                newIntent.putExtra(AudioManager_EXTRA_SCO_AUDIO_STATE, state);
    //                mContext.sendStickyBroadcast(newIntent);
    //            }
    //        }
    //    }
    //}

    return NOERROR;
}


CAudioService::FocusStackEntry::FocusStackEntry()
    : mStreamType(-1)
    , mIsTransportControlReceiver(FALSE)
{}

CAudioService::FocusStackEntry::FocusStackEntry(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 duration,
    /* [in] */ Boolean isTransportControlReceiver,
    /* [in] */ IAudioFocusDispatcher* afl,
    /* [in] */ IBinder* source,
    /* [in] */ const String& id)
    : mStreamType(streamType)
    , mIsTransportControlReceiver(isTransportControlReceiver)
    , mFocusDispatcher(afl)
    , mSourceRef(source)
    , mClientId(id)
    , mFocusChangeType(duration)
{}


CAudioService::AudioFocusDeathHandler::AudioFocusDeathHandler(
    /* [in] */ IBinder* cb,
    /* [in] */ CAudioService* host)
    : mCb(cb)
    , mHost(host)
{}

PInterface CAudioService::AudioFocusDeathHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBinderDeathRecipient*)this;
    }
    else if (riid == EIID_IBinderDeathRecipient) {
        return (IBinderDeathRecipient*)this;
    }

    return NULL;
}

UInt32 CAudioService::AudioFocusDeathHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::AudioFocusDeathHandler::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::AudioFocusDeathHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IBinderDeathRecipient*)this) {
        *pIID = EIID_IBinderDeathRecipient;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::AudioFocusDeathHandler::BinderDied()
{
    Mutex::Autolock lock(&mHost->mAudioFocusLock);

    // Log.w(TAG, "  AudioFocus   audio focus client died");
    mHost->RemoveFocusStackEntryForClient(mCb);

    return NOERROR;
}

AutoPtr<IBinder> CAudioService::AudioFocusDeathHandler::GetBinder()
{
    return mCb;
}


PInterface CAudioService::MediaButtonBroadcastReceiver::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBroadcastReceiver*)this;
    }
    else if (riid == EIID_IBroadcastReceiver) {
        return (IBroadcastReceiver*)this;
    }

    return NULL;
}

UInt32 CAudioService::MediaButtonBroadcastReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::MediaButtonBroadcastReceiver::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::MediaButtonBroadcastReceiver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IBroadcastReceiver*)this) {
        *pIID = EIID_IBroadcastReceiver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::MediaButtonBroadcastReceiver::OnReceive(
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


CAudioService::RemoteControlStackEntry::RemoteControlStackEntry()
{}

CAudioService::RemoteControlStackEntry::RemoteControlStackEntry(
    /* [in] */ IComponentName* r)
    : mReceiverComponent(r)
{}


PInterface CAudioService::AudioSystemErrorCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAudioSystemErrorCallback*)this;
    }
    else if (riid == EIID_IAudioSystemErrorCallback) {
        return (IAudioSystemErrorCallback*)this;
    }

    return NULL;
}

UInt32 CAudioService::AudioSystemErrorCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioService::AudioSystemErrorCallback::Release()
{
    return ElRefBase::Release();
}

ECode CAudioService::AudioSystemErrorCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IAudioSystemErrorCallback*)this) {
        *pIID = EIID_IAudioSystemErrorCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CAudioService::AudioSystemErrorCallback::OnError(
    /* [in] */ Int32 error)
{
    // switch (error) {
    // case AudioSystem.AUDIO_STATUS_SERVER_DIED:
    //     if (mMediaServerOk) {
    //         sendMsg(mAudioHandler, MSG_MEDIA_SERVER_DIED, SHARED_MSG, SENDMSG_NOOP, 0, 0,
    //                 NULL, 1500);
    //         mMediaServerOk = FALSE;
    //     }
    //     break;
    // case AudioSystem.AUDIO_STATUS_OK:
    //     if (!mMediaServerOk) {
    //         sendMsg(mAudioHandler, MSG_MEDIA_SERVER_STARTED, SHARED_MSG, SENDMSG_NOOP, 0, 0,
    //                 NULL, 0);
    //         mMediaServerOk = TRUE;
    //     }
    //     break;
    // default:
    //     break;
    // }
    return E_NOT_IMPLEMENTED;
}


CString CAudioService::TAG = "CAudioService";

/** How long to delay before persisting a change in volume/ringer mode. */
const Int32 CAudioService::PERSIST_DELAY;
// sendMsg() flags
/** Used when a message should be shared across all stream types. */
const Int32 CAudioService::SHARED_MSG;
/** If the msg is already queued, replace it with this one. */
const Int32 CAudioService::SENDMSG_REPLACE;
/** If the msg is already queued, ignore this one and leave the old. */
const Int32 CAudioService::SENDMSG_NOOP;
/** If the msg is already queued, queue this one and leave the old. */
const Int32 CAudioService::SENDMSG_QUEUE;

// AudioHandler message.whats
const Int32 CAudioService::MSG_SET_SYSTEM_VOLUME;
const Int32 CAudioService::MSG_PERSIST_VOLUME;
const Int32 CAudioService::MSG_PERSIST_RINGER_MODE;
const Int32 CAudioService::MSG_PERSIST_VIBRATE_SETTING;
const Int32 CAudioService::MSG_MEDIA_SERVER_DIED;
const Int32 CAudioService::MSG_MEDIA_SERVER_STARTED;
const Int32 CAudioService::MSG_PLAY_SOUND_EFFECT;
const Int32 CAudioService::MSG_BTA2DP_DOCK_TIMEOUT;

const Int32 CAudioService::BTA2DP_DOCK_TIMEOUT_MILLIS;

const Int32 CAudioService::NUM_SOUNDPOOL_CHANNELS;
const Int32 CAudioService::SOUND_EFFECT_VOLUME;

/* Sound effect file names  */
CString CAudioService::SOUND_EFFECTS_PATH = "/media/audio/ui/";
CString CAudioService::SOUND_EFFECT_FILES[] = {
    CString("Effect_Tick.ogg"),
    CString("KeypressStandard.ogg"),
    CString("KeypressSpacebar.ogg"),
    CString("KeypressDelete.ogg"),
    CString("KeypressReturn.ogg")
};
const Int32 CAudioService::SOUND_EFFECT_FILES_LENGTH;

CString CAudioService::IN_VOICE_COMM_FOCUS_ID = "AudioFocus_For_Phone_Ring_And_Calls";

Mutex CAudioService::mAudioFocusLock;

Mutex CAudioService::mRingingLock;

CAudioService::CAudioService()
    : mStreamStates(NULL)
    , mMediaServerOk(FALSE)
    , mIsRinging(FALSE)
    , mConnectedDevices(7)
    , mBluetoothHeadsetConnected(FALSE)
{
    SOUND_EFFECT_FILES_MAP[0][0] = 0; SOUND_EFFECT_FILES_MAP[0][1] = -1; // FX_KEY_CLICK
    SOUND_EFFECT_FILES_MAP[1][0] = 0; SOUND_EFFECT_FILES_MAP[1][1] = -1; // FX_FOCUS_NAVIGATION_UP
    SOUND_EFFECT_FILES_MAP[2][0] = 0; SOUND_EFFECT_FILES_MAP[2][1] = -1; // FX_FOCUS_NAVIGATION_DOWN
    SOUND_EFFECT_FILES_MAP[3][0] = 0; SOUND_EFFECT_FILES_MAP[3][1] = -1; // FX_FOCUS_NAVIGATION_LEFT
    SOUND_EFFECT_FILES_MAP[4][0] = 0; SOUND_EFFECT_FILES_MAP[4][1] = -1; // FX_FOCUS_NAVIGATION_RIGHT
    SOUND_EFFECT_FILES_MAP[5][0] = 1; SOUND_EFFECT_FILES_MAP[5][1] = -1; // FX_KEYPRESS_STANDARD
    SOUND_EFFECT_FILES_MAP[6][0] = 2; SOUND_EFFECT_FILES_MAP[6][1] = -1; // FX_KEYPRESS_SPACEBAR
    SOUND_EFFECT_FILES_MAP[7][0] = 3; SOUND_EFFECT_FILES_MAP[7][1] = -1; // FX_FOCUS_DELETE
    SOUND_EFFECT_FILES_MAP[8][0] = 4; SOUND_EFFECT_FILES_MAP[8][1] = -1; // FX_FOCUS_RETURN

    MAX_STREAM_VOLUME[0] = 5; // STREAM_VOICE_CALL
    MAX_STREAM_VOLUME[1] = 7; // STREAM_SYSTEM
    MAX_STREAM_VOLUME[2] = 7; // STREAM_RING
    MAX_STREAM_VOLUME[3] = 15; // STREAM_MUSIC
    MAX_STREAM_VOLUME[4] = 7; // STREAM_ALARM
    MAX_STREAM_VOLUME[5] = 7; // STREAM_NOTIFICATION
    MAX_STREAM_VOLUME[6] = 15; // STREAM_BLUETOOTH_SCO
    MAX_STREAM_VOLUME[7] = 7; // STREAM_SYSTEM_ENFORCED
    MAX_STREAM_VOLUME[8] = 15; // STREAM_DTMF
    MAX_STREAM_VOLUME[9] = 15; // STREAM_TTS

    STREAM_VOLUME_ALIAS[0] = AudioSystem::STREAM_VOICE_CALL;  // STREAM_VOICE_CALL
    STREAM_VOLUME_ALIAS[1] = AudioSystem::STREAM_SYSTEM;  // STREAM_SYSTEM
    STREAM_VOLUME_ALIAS[2] = AudioSystem::STREAM_RING;  // STREAM_RING
    STREAM_VOLUME_ALIAS[3] = AudioSystem::STREAM_MUSIC; // STREAM_MUSIC
    STREAM_VOLUME_ALIAS[4] = AudioSystem::STREAM_ALARM;  // STREAM_ALARM
    STREAM_VOLUME_ALIAS[5] = AudioSystem::STREAM_NOTIFICATION;  // STREAM_NOTIFICATION
    STREAM_VOLUME_ALIAS[6] = AudioSystem::STREAM_BLUETOOTH_SCO; // STREAM_BLUETOOTH_SCO
    STREAM_VOLUME_ALIAS[7] = AudioSystem::STREAM_SYSTEM;  // STREAM_SYSTEM_ENFORCED
    STREAM_VOLUME_ALIAS[8] = AudioSystem::STREAM_VOICE_CALL; // STREAM_DTMF
    STREAM_VOLUME_ALIAS[9] = AudioSystem::STREAM_MUSIC;  // STREAM_TTS

    mAudioSystemCallback = new AudioSystemErrorCallback();

    pthread_cond_init(&mWaitCond, NULL);
}

CAudioService::~CAudioService()
{
    if (mStreamStates != NULL) {
        for (Int32 i = 0; i < mStreamStates->GetLength(); ++i) {
            (*mStreamStates)[i] = NULL;
        }
        ArrayOf< AutoPtr<VolumeStreamState> >::Free(mStreamStates);
    }

    pthread_cond_destroy(&mWaitCond);
}

ECode CAudioService::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    context->GetContentResolver((IContentResolver**)&mContentResolver);

    // Intialized volume
    // MAX_STREAM_VOLUME[AudioSystem::STREAM_VOICE_CALL] = SystemProperties::GetInt32(
    //     "ro.config.vc_call_vol_steps",
    //     MAX_STREAM_VOLUME[AudioSystem::STREAM_VOICE_CALL]);

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
    Int32 mode;
    GetRingerMode(&mode);
    SetRingerModeInt(mode, FALSE);

    AudioSystem::SetErrorCallback(mAudioSystemCallback);
    Boolean succeeded;
    LoadSoundEffects(&succeeded);

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
    return NOERROR;
}

void CAudioService::CreateAudioSystemThread()
{
    //mAudioSystemThread = new AudioSystemThread();
    // mAudioSystemThread->Start();
    WaitForAudioHandlerCreation();
}

/** Waits for the volume handler to be created by the other thread. */
void CAudioService::WaitForAudioHandlerCreation()
{
    Mutex::Autolock lock(&_m_syncLock);

    while (mAudioHandler == NULL) {
        //try {
            // Wait for mAudioHandler to be set by the other thread
        Wait();
        /*} catch (InterruptedException e) {
            Log.e(TAG, "Interrupted while waiting on volume handler.");
        }*/
    }
}

void CAudioService::CreateStreamStates()
{
    Int32 numStreamTypes = AudioSystem::GetNumStreamTypes();
    ArrayOf< AutoPtr<VolumeStreamState> >* streams = mStreamStates =
            ArrayOf< AutoPtr<VolumeStreamState> >::Alloc(numStreamTypes);

    for (Int32 i = 0; i < numStreamTypes; i++) {
        // streams[i] = new VolumeStreamState(System.VOLUME_SETTINGS[STREAM_VOLUME_ALIAS[i]], i, this);
    }

    // Correct stream index values for streams with aliases
    for (Int32 i = 0; i < numStreamTypes; i++) {
        if (STREAM_VOLUME_ALIAS[i] != i) {
            Int32 index = RescaleIndex((*streams)[i]->mIndex, STREAM_VOLUME_ALIAS[i], i);
            (*streams)[i]->mIndex = (*streams)[i]->GetValidIndex(index);
            SetStreamVolumeIndex(i, index);
            index = RescaleIndex((*streams)[i]->mLastAudibleIndex, STREAM_VOLUME_ALIAS[i], i);
            (*streams)[i]->mLastAudibleIndex = (*streams)[i]->GetValidIndex(index);
        }
    }
}

void CAudioService::ReadPersistedSettings()
{
    AutoPtr<IContentResolver> cr = mContentResolver;

    AutoPtr<ISettingsSystem> system;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&system);

    system->GetInt32(cr, String(SettingsSystem_MODE_RINGER), AudioManager_RINGER_MODE_NORMAL, &mRingerMode);

    system->GetInt32(cr, String(SettingsSystem_VIBRATE_ON), 0, &mVibrateSetting);

    system->GetInt32(cr,
            String(SettingsSystem_MODE_RINGER_STREAMS_AFFECTED),
            ((1 << AudioSystem::STREAM_RING)|(1 << AudioSystem::STREAM_NOTIFICATION)|
             (1 << AudioSystem::STREAM_SYSTEM)|(1 << AudioSystem::STREAM_SYSTEM_ENFORCED)),
            &mRingerModeAffectedStreams);

    system->GetInt32(cr,
            String(SettingsSystem_MUTE_STREAMS_AFFECTED),
            ((1 << AudioSystem::STREAM_MUSIC)|(1 << AudioSystem::STREAM_RING)|(1 << AudioSystem::STREAM_SYSTEM)),
            &mMuteAffectedStreams);

    system->GetInt32(cr,
            String(SettingsSystem_NOTIFICATIONS_USE_RING_VOLUME), 1, &mNotificationsUseRingVolume);

    if (mNotificationsUseRingVolume == 1) {
        STREAM_VOLUME_ALIAS[AudioSystem::STREAM_NOTIFICATION] = AudioSystem::STREAM_RING;
    }
    // Each stream will read its own persisted settings

    // Broadcast the sticky intent
    BroadcastRingerMode();

    // Broadcast vibrate settings
    BroadcastVibrateSetting(AudioManager_VIBRATE_TYPE_RINGER);
    BroadcastVibrateSetting(AudioManager_VIBRATE_TYPE_NOTIFICATION);
}

void CAudioService::SetStreamVolumeIndex(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 index)
{
    AudioSystem::SetStreamVolumeIndex(stream, (index + 5)/10);
}

Int32 CAudioService::RescaleIndex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 srcStream,
    /* [in] */ Int32 dstStream)
{
    return (index * (*mStreamStates)[dstStream]->GetMaxIndex()
            + (*mStreamStates)[srcStream]->GetMaxIndex() / 2)
            / (*mStreamStates)[srcStream]->GetMaxIndex();
}

///////////////////////////////////////////////////////////////////////////
// IPC methods
///////////////////////////////////////////////////////////////////////////

/** @see AudioManager#adjustVolume(Int32, Int32) */
ECode CAudioService::AdjustVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    return AdjustSuggestedStreamVolume(direction, AudioManager_USE_DEFAULT_STREAM_TYPE, flags);
}

/** @see AudioManager#adjustVolume(Int32, Int32, Int32) */
ECode CAudioService::AdjustSuggestedStreamVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 suggestedStreamType,
    /* [in] */ Int32 flags)
{
    Int32 streamType = GetActiveStreamType(suggestedStreamType);

    // Don't play sound on other streams
    if (streamType != AudioSystem::STREAM_RING && (flags & AudioManager_FLAG_PLAY_SOUND) != 0) {
        flags &= ~AudioManager_FLAG_PLAY_SOUND;
    }

    return AdjustStreamVolume(streamType, direction, flags);
}

/** @see AudioManager#adjustStreamVolume(Int32, Int32, Int32) */
ECode CAudioService::AdjustStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    EnsureValidDirection(direction);
    EnsureValidStreamType(streamType);

    AutoPtr<VolumeStreamState> streamState = (*mStreamStates)[STREAM_VOLUME_ALIAS[streamType]];
    Int32 oldIndex = (streamState->MuteCount() != 0) ? streamState->mLastAudibleIndex : streamState->mIndex;
    Boolean adjustVolume = TRUE;

    // If either the client forces allowing ringer modes for this adjustment,
    // or the stream type is one that is affected by ringer modes
    if ((flags & AudioManager_FLAG_ALLOW_RINGER_MODES) != 0
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
            // SendMsg(mAudioHandler, MSG_PERSIST_VOLUME, streamType,
            //         SENDMSG_REPLACE, 0, 1, (IInterface*)streamState, PERSIST_DELAY);
        }
        index = streamState->mLastAudibleIndex;
    }
    else {
        if (adjustVolume && streamState->AdjustIndex(direction)) {
            // Post message to set system volume (it in turn will post a message
            // to persist). Do not change volume if stream is muted.
            // SendMsg(mAudioHandler, MSG_SET_SYSTEM_VOLUME, STREAM_VOLUME_ALIAS[streamType], SENDMSG_NOOP, 0, 0,
            //         (IInterface*)streamState, 0);
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
ECode CAudioService::SetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 index,
    /* [in] */ Int32 flags)
{
    EnsureValidStreamType(streamType);
    AutoPtr<VolumeStreamState> streamState = (*mStreamStates)[STREAM_VOLUME_ALIAS[streamType]];

    Int32 oldIndex = (streamState->MuteCount() != 0) ? streamState->mLastAudibleIndex : streamState->mIndex;

    index = RescaleIndex(index * 10, streamType, STREAM_VOLUME_ALIAS[streamType]);
    SetStreamVolumeInt(STREAM_VOLUME_ALIAS[streamType], index, FALSE, TRUE);

    index = (streamState->MuteCount() != 0) ? streamState->mLastAudibleIndex : streamState->mIndex;

    // UI, etc.
    //mVolumePanel.postVolumeChanged(streamType, flags);
    // Broadcast Intent
    SendVolumeUpdate(streamType, oldIndex, index);

    return NOERROR;
}

void CAudioService::SendVolumeUpdate(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 oldIndex,
    /* [in] */ Int32 index)
{
    oldIndex = (oldIndex + 5) / 10;
    index = (index + 5) / 10;

    AutoPtr<IIntent> intent;
    CIntent::New(String(AudioManager_VOLUME_CHANGED_ACTION), (IIntent**)&intent);
    intent->PutInt32Extra(String(AudioManager_EXTRA_VOLUME_STREAM_TYPE), streamType);
    intent->PutInt32Extra(String(AudioManager_EXTRA_VOLUME_STREAM_VALUE), index);
    intent->PutInt32Extra(String(AudioManager_EXTRA_PREV_VOLUME_STREAM_VALUE), oldIndex);

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
void CAudioService::SetStreamVolumeInt(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 index,
    /* [in] */ Boolean force,
    /* [in] */ Boolean lastAudible)
{
    AutoPtr<VolumeStreamState> streamState = (*mStreamStates)[streamType];

    // If stream is muted, set last audible index only
    if (streamState->MuteCount() != 0) {
        // Do not allow last audible index to be 0
        if (index != 0) {
            streamState->SetLastAudibleIndex(index);
            // Post a persist volume msg
            // SendMsg(mAudioHandler, MSG_PERSIST_VOLUME, streamType,
            //         SENDMSG_REPLACE, 0, 1, (IInterface*)streamState, PERSIST_DELAY);
        }
    } else {
        if (streamState->SetIndex(index, lastAudible) || force) {
            // Post message to set system volume (it in turn will post a message
            // to persist).
            // SendMsg(mAudioHandler, MSG_SET_SYSTEM_VOLUME, streamType, SENDMSG_NOOP, 0, 0,
            //         (IInterface*)streamState, 0);
        }
    }
}

/** @see AudioManager#setStreamSolo(Int32, Boolean) */
ECode CAudioService::SetStreamSolo(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state,
    /* [in] */ IBinder* cb)
{
    for (Int32 stream = 0; stream < mStreamStates->GetLength(); stream++) {
        Boolean result;
        if ((IsStreamAffectedByMute(stream, &result), !result) || stream == streamType) continue;
        // Bring back last audible volume
        (*mStreamStates)[stream]->Mute(cb, state);
     }

    return NOERROR;
}

/** @see AudioManager#setStreamMute(Int32, Boolean) */
ECode CAudioService::SetStreamMute(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state,
    /* [in] */ IBinder* cb)
{
    Boolean result;
    if (IsStreamAffectedByMute(streamType, &result), result) {
        (*mStreamStates)[streamType]->Mute(cb, state);
    }

    return NOERROR;
}

/** @see AudioManager#getStreamVolume(Int32) */
ECode CAudioService::GetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* volume)
{
    VALIDATE_NOT_NULL(volume);

    FAIL_RETURN(EnsureValidStreamType(streamType));
    *volume = ((*mStreamStates)[streamType]->mIndex + 5) / 10;
    return NOERROR;
}



/** @see AudioManager#getStreamMaxVolume(Int32) */
ECode CAudioService::GetStreamMaxVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* volume)
{
    VALIDATE_NOT_NULL(volume);

    FAIL_RETURN(EnsureValidStreamType(streamType));
    *volume = ((*mStreamStates)[streamType]->GetMaxIndex() + 5) / 10;
    return NOERROR;
}

/** @see AudioManager#getRingerMode() */
ECode CAudioService::GetRingerMode(
    /* [out] */ Int32* mode)
{
    *mode = mRingerMode;
    return NOERROR;
}

/** @see AudioManager#setRingerMode(Int32) */
ECode CAudioService::SetRingerMode(
    /* [in] */ Int32 ringerMode)
{
    Mutex::Autolock lock(&mSettingsLock);

    if (ringerMode != mRingerMode) {
        SetRingerModeInt(ringerMode, TRUE);
        // Send sticky broadcast
        BroadcastRingerMode();
    }

    return NOERROR;
}

void CAudioService::SetRingerModeInt(
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
        Boolean result;
        if (IsStreamMutedByRingerMode(streamType)) {
            if ((IsStreamAffectedByRingerMode(streamType, &result), !result) ||
                mRingerMode == AudioManager_RINGER_MODE_NORMAL) {
                (*mStreamStates)[streamType]->Mute(NULL, FALSE);
                mRingerModeMutedStreams &= ~(1 << streamType);
            }
        }
        else {
            if ((IsStreamAffectedByRingerMode(streamType, &result), result) &&
                mRingerMode != AudioManager_RINGER_MODE_NORMAL) {
               (*mStreamStates)[streamType]->Mute(NULL, TRUE);
               mRingerModeMutedStreams |= (1 << streamType);
           }
        }
    }

    // Post a persist ringer mode msg
    if (persist) {
        // SendMsg(mAudioHandler, MSG_PERSIST_RINGER_MODE, SHARED_MSG,
        //         SENDMSG_REPLACE, 0, 0, NULL, PERSIST_DELAY);
    }
}

/** @see AudioManager#shouldVibrate(Int32) */
ECode CAudioService::ShouldVibrate(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 setting;
    GetVibrateSetting(vibrateType, &setting);
    switch (setting) {
        case AudioManager_VIBRATE_SETTING_ON:
            *result = mRingerMode != AudioManager_RINGER_MODE_SILENT;
            return NOERROR;

        case AudioManager_VIBRATE_SETTING_ONLY_SILENT:
            *result = mRingerMode == AudioManager_RINGER_MODE_VIBRATE;
            return NOERROR;

        case AudioManager_VIBRATE_SETTING_OFF:
            // return FALSE, even for incoming calls
            *result = FALSE;
            return NOERROR;

        default:
            *result = FALSE;
            return NOERROR;
    }
}

/** @see AudioManager#getVibrateSetting(Int32) */
ECode CAudioService::GetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(setting);

    *setting = (mVibrateSetting >> (vibrateType * 2)) & 3;
    return NOERROR;
}

/** @see AudioManager#setVibrateSetting(Int32, Int32) */
ECode CAudioService::SetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [in] */ Int32 vibrateSetting)
{
    mVibrateSetting = GetValueForVibrateSetting(mVibrateSetting, vibrateType, vibrateSetting);

    // Broadcast change
    BroadcastVibrateSetting(vibrateType);

    // Post message to set ringer mode (it in turn will post a message
    // to persist)
    // SendMsg(mAudioHandler, MSG_PERSIST_VIBRATE_SETTING, SHARED_MSG, SENDMSG_NOOP, 0, 0,
    //         NULL, 0);

    return NOERROR;
}

/**
 * @see #setVibrateSetting(Int32, Int32)
 */
Int32 CAudioService::GetValueForVibrateSetting(
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

/** @see AudioManager#setMode(Int32) */
ECode CAudioService::SetMode(
    /* [in] */ Int32 mode,
    /* [in] */ IBinder* cb)
{
    if (!CheckAudioSettingsPermission(String("setMode()"))) {
        return NOERROR;
    }

    if (mode < AudioSystem::MODE_CURRENT || mode >= AudioSystem::NUM_MODES) {
        return NOERROR;
    }

    Mutex::Autolock lock(&mSettingsLock);

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
            //        // cb is NULL when setMode() is called by CAudioService constructor
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
    Int32 streamType = GetActiveStreamType(AudioManager_USE_DEFAULT_STREAM_TYPE);
    Int32 index = (*mStreamStates)[STREAM_VOLUME_ALIAS[streamType]]->mIndex;
    SetStreamVolumeInt(STREAM_VOLUME_ALIAS[streamType], index, TRUE, FALSE);

    return NOERROR;
}

/** @see AudioManager#getMode() */
ECode CAudioService::GetMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);

    *mode = mMode;
    return NOERROR;
}

/** @see AudioManager#playSoundEffect(Int32) */
ECode CAudioService::PlaySoundEffect(
    /* [in] */ Int32 effectType)
{
    // return SendMsg(mAudioHandler, MSG_PLAY_SOUND_EFFECT, SHARED_MSG, SENDMSG_NOOP,
    //         effectType, -1, NULL, 0);
    return E_NOT_IMPLEMENTED;
}

/** @see AudioManager#playSoundEffect(Int32, Float) */
ECode CAudioService::PlaySoundEffectVolume(
    /* [in] */ Int32 effectType,
    /* [in] */ Float volume)
{
    Boolean succeeded;
    LoadSoundEffects(&succeeded);
    // return SendMsg(mAudioHandler, MSG_PLAY_SOUND_EFFECT, SHARED_MSG, SENDMSG_NOOP,
    //         effectType, (Int32) (volume * 1000), NULL, 0);
    return E_NOT_IMPLEMENTED;
}

/**
 * Loads samples into the soundpool.
 * This method must be called at when sound effects are enabled
 */
ECode CAudioService::LoadSoundEffects(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    {
        Mutex::Autolock lock(&mSoundEffectsLock);

        if (mSoundPool != NULL) {
            *succeeded = TRUE;
            return NOERROR;
        }
        CSoundPool::New(NUM_SOUNDPOOL_CHANNELS, AudioSystem::STREAM_SYSTEM, 0, (ISoundPool**)&mSoundPool);
        if (mSoundPool == NULL) {
            *succeeded = FALSE;
            return NOERROR;
        }
        /*
        * poolId table: The value -1 in this table indicates that corresponding
        * file (same index in SOUND_EFFECT_FILES[] has not been loaded.
        * Once loaded, the value in poolId is the sample ID and the same
        * sample can be reused for another effect using the same file.
        */
        Int32 poolId[SOUND_EFFECT_FILES_LENGTH];
        for (Int32 fileIdx = 0; fileIdx < SOUND_EFFECT_FILES_LENGTH; fileIdx++) {
            poolId[fileIdx] = -1;
        }
        /*
        * Effects whose value in SOUND_EFFECT_FILES_MAP[effect][1] is -1 must be loaded.
        * If load succeeds, value in SOUND_EFFECT_FILES_MAP[effect][1] is > 0:
        * this indicates we have a valid sample loaded for this effect.
        */
        for (Int32 effect = 0; effect < AudioManager_NUM_SOUND_EFFECTS; effect++) {
            // Do not load sample if this effect uses the MediaPlayer
            if (SOUND_EFFECT_FILES_MAP[effect][1] == 0) {
                continue;
            }
            if (poolId[SOUND_EFFECT_FILES_MAP[effect][0]] == -1) {
                String filePath;// = Environment.getRootDirectory() + SOUND_EFFECTS_PATH + SOUND_EFFECT_FILES[SOUND_EFFECT_FILES_MAP[effect][0]];
                Int32 sampleId;
                mSoundPool->Load(filePath, 0, &sampleId);
                SOUND_EFFECT_FILES_MAP[effect][1] = sampleId;
                poolId[SOUND_EFFECT_FILES_MAP[effect][0]] = sampleId;
                if (sampleId <= 0) {
                    // Log.w(TAG, "Soundpool could not load file: "+filePath);
                }
            }
            else {
                SOUND_EFFECT_FILES_MAP[effect][1] = poolId[SOUND_EFFECT_FILES_MAP[effect][0]];
            }
        }
    }

    *succeeded = TRUE;
    return NOERROR;
}

/**
 *  Unloads samples from the sound pool.
 *  This method can be called to free some memory when
 *  sound effects are disabled.
 */
ECode CAudioService::UnloadSoundEffects()
{
    Mutex::Autolock lock(&mSoundEffectsLock);

    if (mSoundPool == NULL) {
        return NOERROR;
    }
    Int32 poolId[SOUND_EFFECT_FILES_LENGTH];
    for (Int32 fileIdx = 0; fileIdx < SOUND_EFFECT_FILES_LENGTH; fileIdx++) {
        poolId[fileIdx] = 0;
    }

    for (Int32 effect = 0; effect < AudioManager_NUM_SOUND_EFFECTS; effect++) {
        if (SOUND_EFFECT_FILES_MAP[effect][1] <= 0) {
            continue;
        }
        if (poolId[SOUND_EFFECT_FILES_MAP[effect][0]] == 0) {
            Boolean succeeded;
            mSoundPool->Unload(SOUND_EFFECT_FILES_MAP[effect][1], &succeeded);
            SOUND_EFFECT_FILES_MAP[effect][1] = -1;
            poolId[SOUND_EFFECT_FILES_MAP[effect][0]] = -1;
        }
    }
    mSoundPool = NULL;

    return NOERROR;
}

/** @see AudioManager#reloadAudioSettings() */
ECode CAudioService::ReloadAudioSettings()
{
    // restore ringer mode, ringer mode affected streams, mute affected streams and vibrate settings
    ReadPersistedSettings();

    // restore volume settings
    Int32 numStreamTypes = AudioSystem::GetNumStreamTypes();
    AutoPtr<ISettingsSystem> system;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&system);
    for (Int32 streamType = 0; streamType < numStreamTypes; streamType++) {
        AutoPtr<VolumeStreamState> streamState = (*mStreamStates)[streamType];

        String settingName;// = System.VOLUME_SETTINGS[STREAM_VOLUME_ALIAS[streamType]];
        String lastAudibleSettingName = settingName + SettingsSystem_APPEND_FOR_LAST_AUDIBLE;
        Int32 index;
        system->GetInt32(mContentResolver,
                           settingName,
                           CAudioManager::DEFAULT_STREAM_VOLUME[streamType], &index);
        if (STREAM_VOLUME_ALIAS[streamType] != streamType) {
            index = RescaleIndex(index * 10, STREAM_VOLUME_ALIAS[streamType], streamType);
        }
        else {
            index *= 10;
        }
        streamState->mIndex = streamState->GetValidIndex(index);

        index = (index + 5) / 10;
        system->GetInt32(mContentResolver,
                        lastAudibleSettingName,
                        (index > 0) ? index : CAudioManager::DEFAULT_STREAM_VOLUME[streamType], &index);
        if (STREAM_VOLUME_ALIAS[streamType] != streamType) {
            index = RescaleIndex(index * 10, STREAM_VOLUME_ALIAS[streamType], streamType);
        }
        else {
            index *= 10;
        }
        streamState->mLastAudibleIndex = streamState->GetValidIndex(index);

        // unmute stream that was muted but is not affect by mute anymore
        Boolean result;
        if (streamState->MuteCount() != 0 && (IsStreamAffectedByMute(streamType, &result), !result)) {
            List< AutoPtr<VolumeStreamState::VolumeDeathHandler> >::Iterator it = streamState->mDeathHandlers.Begin();
            for (; it != streamState->mDeathHandlers.End(); ++it) {
                (*it)->mMuteCount = 1;
                (*it)->Mute(FALSE);
            }
        }
        // apply stream volume
        if (streamState->MuteCount() == 0) {
            SetStreamVolumeIndex(streamType, streamState->mIndex);
        }
    }

    // apply new ringer mode
    Int32 mode;
    GetRingerMode(&mode);
    SetRingerModeInt(mode, FALSE);

    return NOERROR;
}

/** @see AudioManager#setSpeakerphoneOn() */
ECode CAudioService::SetSpeakerphoneOn(
    /* [in] */ Boolean on)
{
    if (!CheckAudioSettingsPermission(String("setSpeakerphoneOn()"))) {
        return NOERROR;
    }
    if (on) {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_SPEAKER);
        mForcedUseForComm = AudioSystem::FORCE_SPEAKER;
    }
    else {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_NONE);
        mForcedUseForComm = AudioSystem::FORCE_NONE;
    }

    return NOERROR;
}

/** @see AudioManager#isSpeakerphoneOn() */
ECode CAudioService::IsSpeakerphoneOn(
    /* [out] */ Boolean* isOn)
{
    VALIDATE_NOT_NULL(isOn);

    if (mForcedUseForComm == AudioSystem::FORCE_SPEAKER) {
        *isOn = TRUE;
        return NOERROR;
    }
    else {
        *isOn = FALSE;
        return NOERROR;
    }
}

/** @see AudioManager#setBluetoothScoOn() */
ECode CAudioService::SetBluetoothScoOn(
    /* [in] */ Boolean on)
{
    if (!CheckAudioSettingsPermission(String("setBluetoothScoOn()"))) {
        return NOERROR;
    }
    if (on) {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_BT_SCO);
        AudioSystem::SetForceUse(AudioSystem::FOR_RECORD, AudioSystem::FORCE_BT_SCO);
        mForcedUseForComm = AudioSystem::FORCE_BT_SCO;
    }
    else {
        AudioSystem::SetForceUse(AudioSystem::FOR_COMMUNICATION, AudioSystem::FORCE_NONE);
        AudioSystem::SetForceUse(AudioSystem::FOR_RECORD, AudioSystem::FORCE_NONE);
        mForcedUseForComm = AudioSystem::FORCE_NONE;
    }

    return NOERROR;
}

/** @see AudioManager#isBluetoothScoOn() */
ECode CAudioService::IsBluetoothScoOn(
    /* [out] */ Boolean* isOn)
{
    VALIDATE_NOT_NULL(isOn);

    if (mForcedUseForComm == AudioSystem::FORCE_BT_SCO) {
        *isOn = TRUE;
        return NOERROR;
    }
    else {
        *isOn = FALSE;
        return NOERROR;
    }
}

/** @see AudioManager#startBluetoothSco() */
ECode CAudioService::StartBluetoothSco(
    /* [in] */ IBinder* cb)
{
    if (!CheckAudioSettingsPermission(String("startBluetoothSco()"))) {
        return NOERROR;
    }
    AutoPtr<ScoClient> client = GetScoClient(cb);
    client->IncCount();

    return NOERROR;
}

/** @see AudioManager#stopBluetoothSco() */
ECode CAudioService::StopBluetoothSco(
    /* [in] */ IBinder* cb)
{
    if (!CheckAudioSettingsPermission(String("stopBluetoothSco()"))) {
        return NOERROR;
    }
    AutoPtr<ScoClient> client = GetScoClient(cb);
    client->DecCount();

    return NOERROR;
}

AutoPtr<CAudioService::ScoClient> CAudioService::GetScoClient(
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

ECode CAudioService::ClearAllScoClients()
{
    Mutex::Autolock lock(&mScoClientsLock);

    List< AutoPtr<ScoClient> >::Iterator it;
    for (it = mScoClients.Begin(); it != mScoClients.End(); ++it) {
        (*it)->ClearCount(FALSE);
    }

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
Boolean CAudioService::CheckForRingerModeChange(
    /* [in] */ Int32 oldIndex,
    /* [in] */ Int32 direction)
{
    Boolean adjustVolumeIndex = TRUE;
    Int32 newRingerMode = mRingerMode;

    if (mRingerMode == AudioManager_RINGER_MODE_NORMAL) {
        // audible mode, at the bottom of the scale
        if (direction == AudioManager_ADJUST_LOWER
                && (oldIndex + 5) / 10 == 1) {
            // "silent mode", but which one?
            AutoPtr<ISettingsSystem> system;
            CSettingsSystem::AcquireSingleton((ISettingsSystem**)&system);
            Int32 value;
            system->GetInt32(mContentResolver, String(SettingsSystem_VIBRATE_IN_SILENT), 1, &value);
            newRingerMode = value == 1
                ? AudioManager_RINGER_MODE_VIBRATE
                : AudioManager_RINGER_MODE_SILENT;
        }
    }
    else {
        if (direction == AudioManager_ADJUST_RAISE) {
            // exiting silent mode
            newRingerMode = AudioManager_RINGER_MODE_NORMAL;
        }
        else {
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

ECode CAudioService::IsStreamAffectedByRingerMode(
    /* [in] */ Int32 streamType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (mRingerModeAffectedStreams & (1 << streamType)) != 0;
    return NOERROR;
}

Boolean CAudioService::IsStreamMutedByRingerMode(
    /* [in] */ Int32 streamType)
{
    return (mRingerModeMutedStreams & (1 << streamType)) != 0;
}

ECode CAudioService::IsStreamAffectedByMute(
    /* [in] */ Int32 streamType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (mMuteAffectedStreams & (1 << streamType)) != 0;
    return NOERROR;
}

ECode CAudioService::EnsureValidDirection(
    /* [in] */ Int32 direction)
{
    if (direction < AudioManager_ADJUST_LOWER || direction > AudioManager_ADJUST_RAISE) {
        //throw new IllegalArgumentException("Bad direction " + direction);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioService::EnsureValidStreamType(
    /* [in] */ Int32 streamType)
{
    if (streamType < 0 || streamType >= mStreamStates->GetLength()) {
        //throw new IllegalArgumentException("Bad stream type " + streamType);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

Int32 CAudioService::GetActiveStreamType(
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
    }
    else if (isOffhook || AudioSystem::IsStreamActive(AudioSystem::STREAM_VOICE_CALL)) {
        // Log.v(TAG, "getActiveStreamType: Forcing STREAM_VOICE_CALL...");
        return AudioSystem::STREAM_VOICE_CALL;
    }
    else if (AudioSystem::IsStreamActive(AudioSystem::STREAM_MUSIC)) {
        // Log.v(TAG, "getActiveStreamType: Forcing STREAM_MUSIC...");
        return AudioSystem::STREAM_MUSIC;
    }
    else if (suggestedStreamType == AudioManager_USE_DEFAULT_STREAM_TYPE) {
        // Log.v(TAG, "getActiveStreamType: Forcing STREAM_RING...");
        return AudioSystem::STREAM_RING;
    }
    else {
        // Log.v(TAG, "getActiveStreamType: Returning suggested type " + suggestedStreamType);
        return suggestedStreamType;
    }
}

void CAudioService::BroadcastRingerMode()
{
    // Send sticky broadcast
    /*Intent broadcast = new Intent(AudioManager_RINGER_MODE_CHANGED_ACTION);
    broadcast.putExtra(AudioManager_EXTRA_RINGER_MODE, mRingerMode);
    broadcast.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT
            | Intent.FLAG_RECEIVER_REPLACE_PENDING);
    long origCallerIdentityToken = Binder.clearCallingIdentity();
    mContext.sendStickyBroadcast(broadcast);
    Binder.restoreCallingIdentity(origCallerIdentityToken);*/
}

void CAudioService::BroadcastVibrateSetting(
    /* [in] */ Int32 vibrateType)
{
    // Send broadcast
    /*if (ActivityManagerNative.isSystemReady()) {
        Intent broadcast = new Intent(AudioManager_VIBRATE_SETTING_CHANGED_ACTION);
        broadcast.putExtra(AudioManager_EXTRA_VIBRATE_TYPE, vibrateType);
        broadcast.putExtra(AudioManager_EXTRA_VIBRATE_SETTING, getVibrateSetting(vibrateType));
        mContext.sendBroadcast(broadcast);
    }*/
}

// Message helper methods
Int32 CAudioService::GetMsg(
    /* [in] */ Int32 baseMsg,
    /* [in] */ Int32 streamType)
{
    return (baseMsg & 0xffff) | streamType << 16;
}

Int32 CAudioService::GetMsgBase(
    /* [in] */ Int32 msg)
{
    return msg & 0xffff;
}

ECode CAudioService::SendMsg(
    /* [in] */ IApartment* handler,
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
    return E_NOT_IMPLEMENTED;
}

Boolean CAudioService::CheckAudioSettingsPermission(
    /* [in] */ const String& method)
{
    /*if (mContext->CheckCallingOrSelfPermission("android.permission.MODIFY_AUDIO_SETTINGS")
            == PackageManager.PERMISSION_GRANTED) {
        return TRUE;
    }
    String msg = "Audio Settings Permission Denial: " + method + " from pid="
            + Binder.getCallingPid()
            + ", uid=" + Binder.getCallingUid();
    Log.w(TAG, msg);*/
    return TRUE;
}

void CAudioService::MakeA2dpDeviceAvailable(
    /* [in] */ const String& address)
{
    AudioSystem::SetDeviceConnectionState(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP,
            AudioSystem::DEVICE_STATE_AVAILABLE,
            address);
    // Reset A2DP suspend state each time a new sink is connected
    AudioSystem::SetParameters(String("A2dpSuspended=FALSE"));
    mConnectedDevices[AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP] = address;
}

void CAudioService::MakeA2dpDeviceUnavailableNow(
    /* [in] */ const String& address)
{
    AutoPtr<IIntent> noisyIntent;
    CIntent::New(String(AudioManager_ACTION_AUDIO_BECOMING_NOISY), (IIntent**)&noisyIntent);
    mContext->SendBroadcast(noisyIntent);
    AudioSystem::SetDeviceConnectionState(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP,
            AudioSystem::DEVICE_STATE_UNAVAILABLE,
            address);
    mConnectedDevices.Erase(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP);
}

void CAudioService::MakeA2dpDeviceUnavailableLater(
    /* [in] */ const String& address)
{
    // prevent any activity on the A2DP audio output to avoid unwanted
    // reconnection of the sink.
    AudioSystem::SetParameters(String("A2dpSuspended=TRUE"));
    // the device will be made unavailable later, so consider it disconnected right away
    mConnectedDevices.Erase(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP);
    // send the delayed message to make the device unavailable later
    // AutoPtr<IMessage> msg = mAudioHandler->ObtainMessage(MSG_BTA2DP_DOCK_TIMEOUT, address);
    // mAudioHandler->SendMessageDelayed(msg, BTA2DP_DOCK_TIMEOUT_MILLIS);
}

void CAudioService::CancelA2dpDeviceTimeout()
{
    // mAudioHandler->RemoveMessages(MSG_BTA2DP_DOCK_TIMEOUT);
}

Boolean CAudioService::HasScheduledA2dpDockTimeout()
{
    // return mAudioHandler->HasMessages(MSG_BTA2DP_DOCK_TIMEOUT);
    return FALSE;
}

void CAudioService::NotifyTopOfAudioFocusStack()
{
    // notify the top of the stack it gained focus
    // if (!mFocusStack.empty() && (mFocusStack.peek().mFocusDispatcher != NULL)) {
    //     if (canReassignAudioFocus()) {
    //         try {
    //             mFocusStack.peek().mFocusDispatcher.dispatchAudioFocusChange(
    //                     AudioManager_AUDIOFOCUS_GAIN, mFocusStack.peek().mClientId);
    //         } catch (RemoteException e) {
    //             Log.e(TAG, "Failure to signal gain of audio control focus due to "+ e);
    //             e.printStackTrace();
    //         }
    //     }
    // }
}

/**
 * Helper function:
 * Display in the log the current entries in the audio focus stack
 */
//void CAudioService::DumpFocusStack(
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
void CAudioService::RemoveFocusStackEntry(
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
void CAudioService::RemoveFocusStackEntryForClient(
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
Boolean CAudioService::CanReassignAudioFocus()
{
    // focus requests are rejected during a phone call or when the phone is ringing
    // this is equivalent to IN_VOICE_COMM_FOCUS_ID having the focus
    /*if (!mFocusStack.isEmpty() && IN_VOICE_COMM_FOCUS_ID.equals(mFocusStack.peek().mClientId)) {
        return FALSE;
    }*/
    return TRUE;
}

/** @see AudioManager#requestAudioFocus(IAudioFocusDispatcher, Int32, Int32) */
ECode CAudioService::RequestAudioFocus(
    /* [in] */ Int32 mainStreamType,
    /* [in] */ Int32 focusChangeHint,
    /* [in] */ IBinder* cb,
    /* [in] */ IAudioFocusDispatcher* fd,
    /* [in] */ const String& clientId,
    /* [out] */ Int32* result)
{
    //Log.i(TAG, " AudioFocus  requestAudioFocus() from " + clientId);
    //// the main stream type for the audio focus request is currently not used. It may
    //// potentially be used to handle multiple stream type-dependent audio focuses.

    //// we need a valid binder callback for clients other than the CAudioService's phone
    //// state listener
    //if (!IN_VOICE_COMM_FOCUS_ID.equals(clientId) && ((cb == NULL) || !cb.pingBinder())) {
    //    Log.i(TAG, " AudioFocus  DOA client for requestAudioFocus(), exiting");
    //    return AudioManager_AUDIOFOCUS_REQUEST_FAILED;
    //}

    //synchronized(mAudioFocusLock) {
    //    if (!canReassignAudioFocus()) {
    //        return AudioManager_AUDIOFOCUS_REQUEST_FAILED;
    //    }

    //    if (!mFocusStack.empty() && mFocusStack.peek().mClientId.equals(clientId)) {
    //        // if focus is already owned by this client and the reason for acquiring the focus
    //        // hasn't changed, don't do anything
    //        if (mFocusStack.peek().mFocusChangeType == focusChangeHint) {
    //            return AudioManager_AUDIOFOCUS_REQUEST_GRANTED;
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
    //// CAudioService's phone state listener
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

    return AudioManager_AUDIOFOCUS_REQUEST_GRANTED;
}

/** @see AudioManager#abandonAudioFocus(IAudioFocusDispatcher) */
ECode CAudioService::AbandonAudioFocus(
    /* [in] */ IAudioFocusDispatcher* fl,
    /* [in] */ const String& clientId,
    /* [out] */ Int32* result)
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

    return AudioManager_AUDIOFOCUS_REQUEST_GRANTED;
}

ECode CAudioService::UnregisterAudioFocusClient(
    /* [in] */ const String& clientId)
{
    /*synchronized(mAudioFocusLock) {
        removeFocusStackEntry(clientId, FALSE);
    }*/

    return NOERROR;
}

/**
 * Helper function:
 * Display in the log the current entries in the remote control focus stack
 */
//void CAudioService::DumpRCStack(
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
void CAudioService::PushMediaButtonReceiver(
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
void CAudioService::RemoveMediaButtonReceiver(
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


/** see AudioManager_registerMediaButtonEventReceiver(ComponentName eventReceiver) */
ECode CAudioService::RegisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    //Log.i(TAG, "  Remote Control   registerMediaButtonEventReceiver() for " + eventReceiver);

    //synchronized(mRCStack) {
        PushMediaButtonReceiver(eventReceiver);
    //}

        return NOERROR;
}

/** see AudioManager_unregisterMediaButtonEventReceiver(ComponentName eventReceiver) */
ECode CAudioService::UnregisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    //Log.i(TAG, "  Remote Control   unregisterMediaButtonEventReceiver() for " + eventReceiver);

    //synchronized(mRCStack) {
        RemoveMediaButtonReceiver(eventReceiver);
    //}

        return NOERROR;
}


//void CAudioService::Dump(
//    /* [in] */ FileDescriptor fd,
//    /* [in] */ PrintWriter pw,
//    /* [in] */ String[] args)
//{
//    // TODO probably a lot more to do here than just the audio focus and remote control stacks
//    DumpFocusStack(pw);
//    DumpRCStack(pw);
//}

void CAudioService::Wait()
{
    pthread_cond_wait(&mWaitCond, &_m_syncLock.mMutex);
}

void CAudioService::NotifyAll()
{
    pthread_cond_broadcast(&mWaitCond);
}

