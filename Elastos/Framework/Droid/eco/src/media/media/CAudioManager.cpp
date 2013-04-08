
#include "media/CAudioManager.h"
#include "media/ElAudioSystem.h"


/**
 * Helper class to handle the forwarding of audio focus events to the appropriate listener
 */
CAudioManager::FocusEventHandlerDelegate::FocusEventHandlerDelegate()
{
    //Looper looper;
    //if ((looper = Looper.myLooper()) == NULL) {
    //    looper = Looper.getMainLooper();
    //}

    //if (looper != NULL) {
    //    // implement the event handler delegate to receive audio focus events
    //    mHandler = new Handler(looper) {
    //        @Override
    //        public void handleMessage(Message msg) {
    //            OnAudioFocusChangeListener listener = NULL;
    //            synchronized(mFocusListenerLock) {
    //                listener = findFocusListener((String)msg.obj);
    //            }
    //            if (listener != NULL) {
    //                listener.onAudioFocusChange(msg.what);
    //            }
    //        }
    //    };
    //} else {
    //    mHandler = NULL;
    //}
}

AutoPtr<IApartment> CAudioManager::FocusEventHandlerDelegate::GetHandler()
{
    return mHandler;
}

const Int32 CAudioManager::DEFAULT_STREAM_VOLUME[] = {
    4,  // STREAM_VOICE_CALL
    7,  // STREAM_SYSTEM
    5,  // STREAM_RING
    11, // STREAM_MUSIC
    6,  // STREAM_ALARM
    5,  // STREAM_NOTIFICATION
    7,  // STREAM_BLUETOOTH_SCO
    7,  // STREAM_SYSTEM_ENFORCED
    11, // STREAM_DTMF
    11  // STREAM_TTS
};

CString CAudioManager::TAG = "CAudioManager";
Boolean CAudioManager::DEBUG = FALSE;
Boolean CAudioManager::localLOGV = DEBUG;// || android.util.Config.LOGV;

AutoPtr<IAudioService> CAudioManager::sService;

ECode CAudioManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    //mHandler = new Handler(context.getMainLooper());
    return NOERROR;
}

AutoPtr<IAudioService> CAudioManager::GetService()
{
    if (sService != NULL) {
        return sService;
    }
    //AutoPtr<IBinder> b = ServiceManager::GetService(String(Context_AUDIO_SERVICE));
    //sService = AutoPtr<IAudioService>(b);
    return sService;
}

ECode CAudioManager::AdjustStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->AdjustStreamVolume(streamType, direction, flags);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in adjustStreamVolume", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in adjustStreamVolume", e);
    // }
    return NOERROR;
}

ECode CAudioManager::AdjustVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
    ECode ec = service->AdjustVolume(direction, flags);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in adjustVolume", e);
    }
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Dead object in adjustVolume", e);
    //}
    return NOERROR;
}

ECode CAudioManager::AdjustSuggestedStreamVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 suggestedStreamType,
    /* [in] */ Int32 flags)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->AdjustSuggestedStreamVolume(direction, suggestedStreamType, flags);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in adjustVolume", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in adjustVolume", e);
    // }
    return NOERROR;
}

ECode CAudioManager::GetRingerMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->GetRingerMode(mode);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in getRingerMode", e);
        *mode = AudioManager_RINGER_MODE_NORMAL;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in getRingerMode", e);
    //     return RINGER_MODE_NORMAL;
    // }
    return NOERROR;
}

ECode CAudioManager::GetStreamMaxVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* volume)
{
    VALIDATE_NOT_NULL(volume);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->GetStreamMaxVolume(streamType, volume);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in getStreamMaxVolume", e);
        *volume = 0;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in getStreamMaxVolume", e);
    //     return 0;
    // }
    return NOERROR;
}

ECode CAudioManager::GetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* volume)
{
    VALIDATE_NOT_NULL(volume);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->GetStreamVolume(streamType, volume);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in getStreamVolume", e);
        *volume = 0;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in getStreamVolume", e);
    //     return 0;
    // }
    return NOERROR;
}

ECode CAudioManager::SetRingerMode(
    /* [in] */ Int32 ringerMode)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetRingerMode(ringerMode);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setRingerMode", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setRingerMode", e);
    // }
    return NOERROR;
}

ECode CAudioManager::SetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 index,
    /* [in] */ Int32 flags)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetStreamVolume(streamType, index, flags);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setStreamVolume", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setStreamVolume", e);
    // }
    return NOERROR;
}

ECode CAudioManager::SetStreamSolo(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetStreamSolo(streamType, state, mICallBack);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setStreamSolo", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setStreamSolo", e);
    // }
    return NOERROR;
}

ECode CAudioManager::SetStreamMute(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetStreamMute(streamType, state, mICallBack);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setStreamMute", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setStreamMute", e);
    // }
    return NOERROR;
}

ECode CAudioManager::ShouldVibrate(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->ShouldVibrate(vibrateType, result);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in shouldVibrate", e);
        *result = FALSE;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in shouldVibrate", e);
    //     return FALSE;
    // }
    return NOERROR;
}

ECode CAudioManager::GetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(setting);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->GetVibrateSetting(vibrateType, setting);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in getVibrateSetting", e);
        *setting = AudioManager_VIBRATE_SETTING_OFF;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in getVibrateSetting", e);
    //     return VIBRATE_SETTING_OFF;
    // }
    return NOERROR;
}

ECode CAudioManager::SetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [in] */ Int32 vibrateSetting)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetVibrateSetting(vibrateType, vibrateSetting);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setVibrateSetting", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setVibrateSetting", e);
    // }
    return NOERROR;
}

ECode CAudioManager::SetSpeakerphoneOn(
    /* [in] */ Boolean on)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetSpeakerphoneOn(on);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setSpeakerphoneOn", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setSpeakerphoneOn", e);
    // }
    return NOERROR;
}

ECode CAudioManager::IsSpeakerphoneOn(
    /* [out] */ Boolean* isOn)
{
    VALIDATE_NOT_NULL(isOn);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->IsSpeakerphoneOn(isOn);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setSpeakerphoneOn", e);
        *isOn = FALSE;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in isSpeakerphoneOn", e);
    //     return FALSE;
    // }
    return NOERROR;
}

static const Int32 R_Bool_Config_bluetooth_sco_off_call = 0x010d0011;

ECode CAudioManager::IsBluetoothScoAvailableOffCall(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IResources> res;
    FAIL_RETURN(mContext->GetResources((IResources**)&res));
    return res->GetBoolean(R_Bool_Config_bluetooth_sco_off_call, /*com.android.internal.R.bool.config_bluetooth_sco_off_call*/
                              result);
}

ECode CAudioManager::StartBluetoothSco()
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->StartBluetoothSco(mICallBack);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in startBluetoothSco", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in startBluetoothSco", e);
    // }
    return NOERROR;
}

ECode CAudioManager::StopBluetoothSco()
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->StopBluetoothSco(mICallBack);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in stopBluetoothSco", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in stopBluetoothSco", e);
    // }
    return NOERROR;
}

ECode CAudioManager::SetBluetoothScoOn(
    /* [in] */ Boolean on)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetBluetoothScoOn(on);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in stopBluetoothSco", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setBluetoothScoOn", e);
    // }
    return NOERROR;
}

ECode CAudioManager::IsBluetoothScoOn(
    /* [out] */ Boolean* isOn)
{
    VALIDATE_NOT_NULL(isOn);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->IsBluetoothScoOn(isOn);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in isBluetoothScoOn", e);
        *isOn = FALSE;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in isBluetoothScoOn", e);
    //     return FALSE;
    // }
    return NOERROR;
}

ECode CAudioManager::SetBluetoothA2dpOn(
    /* [in] */ Boolean on)
{
    return NOERROR;
}

ECode CAudioManager::IsBluetoothA2dpOn(
    /* [out] */ Boolean* isOn)
{
    VALIDATE_NOT_NULL(isOn);

    if (AudioSystem::GetDeviceConnectionState(
            AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP, String(""))
            == AudioSystem::DEVICE_STATE_UNAVAILABLE) {
        *isOn = FALSE;
        return NOERROR;
    }
    else {
        *isOn = TRUE;
        return NOERROR;
    }
}

ECode CAudioManager::SetWiredHeadsetOn(
    /* [in] */ Boolean on)
{
    return NOERROR;
}

ECode CAudioManager::IsWiredHeadsetOn(
    /* [out] */ Boolean* isOn)
{
    VALIDATE_NOT_NULL(isOn);

    if (AudioSystem::GetDeviceConnectionState(
            AudioSystem::DEVICE_OUT_WIRED_HEADSET, String(""))
            == AudioSystem::DEVICE_STATE_UNAVAILABLE &&
            AudioSystem::GetDeviceConnectionState(
            AudioSystem::DEVICE_OUT_WIRED_HEADPHONE, String(""))
            == AudioSystem::DEVICE_STATE_UNAVAILABLE) {
        *isOn = FALSE;
        return NOERROR;
    }
    else {
        *isOn = TRUE;
        return NOERROR;
    }
}

ECode CAudioManager::SetMicrophoneMute(
    /* [in] */ Boolean on)
{
    AudioSystem::MuteMicrophone(on);
    return NOERROR;
}

ECode CAudioManager::IsMicrophoneMute(
    /* [out] */ Boolean* isMute)
{
    VALIDATE_NOT_NULL(isMute);

    *isMute = AudioSystem::IsMicrophoneMuted();
    return NOERROR;
}

ECode CAudioManager::SetMode(
    /* [in] */ Int32 mode)
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->SetMode(mode, mICallBack);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in setMode", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in setMode", e);
    // }
    return NOERROR;
}

ECode CAudioManager::GetMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->GetMode(mode);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in getMode", e);
        *mode = AudioManager_MODE_INVALID;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in getMode", e);
    //     return MODE_INVALID;
    // }
    return NOERROR;
}

ECode CAudioManager::SetRouting(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 routes,
    /* [in] */ Int32 mask)
{
    return NOERROR;
}

ECode CAudioManager::GetRouting(
    /* [in] */ Int32 mode,
    /* [out] */ Int32* routing)
{
    VALIDATE_NOT_NULL(routing);

    *routing = -1;
    return NOERROR;
}

ECode CAudioManager::IsMusicActive(
    /* [out] */ Boolean* isActive)
{
    VALIDATE_NOT_NULL(isActive);

    *isActive = AudioSystem::IsStreamActive(
            AudioManager_STREAM_MUSIC);
    return NOERROR;
}

ECode CAudioManager::SetParameter(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    return SetParameters(key + "=" + value);
}

ECode CAudioManager::SetParameters(
    /* [in] */ const String& keyValuePairs)
{
    AudioSystem::SetParameters(keyValuePairs);
    return NOERROR;
}

ECode CAudioManager::GetParameters(
    /* [in] */ const String& keys,
    /* [out] */ String* parameters)
{
    VALIDATE_NOT_NULL(parameters);

    *parameters = AudioSystem::GetParameters(keys);
    return NOERROR;
}

ECode CAudioManager::PlaySoundEffect(
    /* [in] */ Int32 effectType)
{
    if (effectType < 0 || effectType >= AudioManager_NUM_SOUND_EFFECTS) {
        return NOERROR;
    }

    if (!QuerySoundEffectsEnabled()) {
        return NOERROR;
    }

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->PlaySoundEffect(effectType);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in playSoundEffect", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in playSoundEffect"+e);
    // }
    return NOERROR;
}

ECode CAudioManager::PlaySoundEffectEx(
    /* [in] */ Int32 effectType,
    /* [in] */ Float volume)
{
    if (effectType < 0 || effectType >= AudioManager_NUM_SOUND_EFFECTS) {
        return NOERROR;
    }

    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->PlaySoundEffectVolume(effectType, volume);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in playSoundEffect", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in playSoundEffect"+e);
    // }
    return NOERROR;
}

/**
 * Settings has an in memory cache, so this is fast.
 */
Boolean CAudioManager::QuerySoundEffectsEnabled()
{
    //return Settings.System.getInt(mContext.getContentResolver(), Settings.System.SOUND_EFFECTS_ENABLED, 0) != 0;
    return FALSE;
}

ECode CAudioManager::LoadSoundEffects()
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    Boolean res;
    ECode ec = service->LoadSoundEffects(&res);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in loadSoundEffects", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in loadSoundEffects"+e);
    // }
    return NOERROR;
}

ECode CAudioManager::UnloadSoundEffects()
{
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->UnloadSoundEffects();
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in unloadSoundEffects", e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Dead object in unloadSoundEffects"+e);
    // }
    return NOERROR;
}

AutoPtr<IOnAudioFocusChangeListener> CAudioManager::FindFocusListener(
    /* [in] */ const String& id)
{
    HashMap<String, AutoPtr<IOnAudioFocusChangeListener> >::Iterator it =
            mAudioFocusIdListenerMap.Find(id);
    if (it != mAudioFocusIdListenerMap.End()) {
        return it->mSecond;
    }
    return NULL;
}

String CAudioManager::GetIdForAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    /*if (l == NULL) {
        return new String(this.toString());
    } else {
        return new String(this.toString() + l.toString());
    }*/
    return String("");
}

ECode CAudioManager::RegisterAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    //synchronized(mFocusListenerLock) {
        /*if (mAudioFocusIdListenerMap.ContainsKey(GetIdForAudioFocusListener(l))) {
            return NOERROR;
        }
        mAudioFocusIdListenerMap.Put(GetIdForAudioFocusListener(l), l);*/
    //}

    return NOERROR;
}

ECode CAudioManager::UnregisterAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    // remove locally
    //synchronized(mFocusListenerLock) {
        //mAudioFocusIdListenerMap.Remove(GetIdForAudioFocusListener(l));
    //}

    return NOERROR;
}

ECode CAudioManager::RequestAudioFocus(
    /* [in] */ IOnAudioFocusChangeListener* l,
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 durationHint,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 status = AudioManager_AUDIOFOCUS_REQUEST_FAILED;
    if ((durationHint < AudioManager_AUDIOFOCUS_GAIN) || (durationHint > AudioManager_AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK))
    {
        //Log.e(TAG, "Invalid duration hint, audio focus request denied");
        *result = status;
        return NOERROR;
    }
    RegisterAudioFocusListener(l);
    //TODO protect request by permission check?
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->RequestAudioFocus(streamType, durationHint, mICallBack,
                mAudioFocusDispatcher, GetIdForAudioFocusListener(l), result);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Can't call requestAudioFocus() from AudioService due to "+e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Can't call requestAudioFocus() from AudioService due to "+e);
    // }
    return NOERROR;
}

ECode CAudioManager::AbandonAudioFocus(
    /* [in] */ IOnAudioFocusChangeListener* l,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 status = AudioManager_AUDIOFOCUS_REQUEST_FAILED;
    UnregisterAudioFocusListener(l);
    AutoPtr<IAudioService> service = GetService();
    // try {
    ECode ec = service->AbandonAudioFocus(mAudioFocusDispatcher,
                GetIdForAudioFocusListener(l), result);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Can't call abandonAudioFocus() from AudioService due to "+e);
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Can't call abandonAudioFocus() from AudioService due to "+e);
    // }
    return NOERROR;
}

ECode CAudioManager::RegisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    //TODO enforce the rule about the receiver being declared in the manifest
    AutoPtr<IAudioService> service = GetService();
    //try {
    ECode ec = service->RegisterMediaButtonEventReceiver(eventReceiver);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in registerMediaButtonEventReceiver"+e);
    }
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in registerMediaButtonEventReceiver"+e);
    }*/
    return NOERROR;
}

ECode CAudioManager::UnregisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
    ECode ec = service->UnregisterMediaButtonEventReceiver(eventReceiver);
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in unregisterMediaButtonEventReceiver"+e);
    }
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in unregisterMediaButtonEventReceiver"+e);
    }*/
    return NOERROR;
}

ECode CAudioManager::ReloadAudioSettings()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
    ECode ec = service->ReloadAudioSettings();
    if (ec == E_REMOTE_EXCEPTION) {
        // Log.e(TAG, "Dead object in reloadAudioSettings"+e);
    }
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in reloadAudioSettings"+e);
    }*/
    return NOERROR;
}
