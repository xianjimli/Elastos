
#include "media/AudioManager.h"
#include "os/ServiceManager.h"

AutoPtr<IAudioService> AudioManager::sService;

static const Int32 R_Bool_Config_bluetooth_sco_off_call=0x010d0011;

AudioManager::AudioManager()
{

}

/**
 * @hide
 */
AudioManager::AudioManager(
    /* [in] */ IContext* context)
{
    mContext = context;
    //mHandler = new Handler(context.getMainLooper());
}

AutoPtr<IAudioService> AudioManager::GetService()
{
    if (sService != NULL) {
        return sService;
    }
    //AutoPtr<IBinder> b = ServiceManager::GetService(String(Context_AUDIO_SERVICE));
    //sService = AutoPtr<IAudioService>(b);
    return sService;
}

/**
 * Adjusts the volume of a particular stream by one step in a direction.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param streamType The stream type to adjust. One of {@link #STREAM_VOICE_CALL},
 * {@link #STREAM_SYSTEM}, {@link #STREAM_RING}, {@link #STREAM_MUSIC} or
 * {@link #STREAM_ALARM}
 * @param direction The direction to adjust the volume. One of
 *            {@link #ADJUST_LOWER}, {@link #ADJUST_RAISE}, or
 *            {@link #ADJUST_SAME}.
 * @param flags One or more flags.
 * @see #adjustVolume(Int32, Int32)
 * @see #setStreamVolume(Int32, Int32, Int32)
 */
ECode AudioManager::AdjustStreamVolume(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 direction, 
    /* [in] */ Int32 flags)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->AdjustStreamVolume(streamType, direction, flags);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in adjustStreamVolume", e);
    }*/

    return NOERROR;
}

/**
 * Adjusts the volume of the most relevant stream. For example, if a call is
 * active, it will have the highest priority regardless of if the in-call
 * screen is showing. Another example, if music is playing in the background
 * and a call is not active, the music stream will be adjusted.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param direction The direction to adjust the volume. One of
 *            {@link #ADJUST_LOWER}, {@link #ADJUST_RAISE}, or
 *            {@link #ADJUST_SAME}.
 * @param flags One or more flags.
 * @see #adjustSuggestedStreamVolume(Int32, Int32, Int32)
 * @see #adjustStreamVolume(Int32, Int32, Int32)
 * @see #setStreamVolume(Int32, Int32, Int32)
 */
ECode AudioManager::AdjustVolume(
    /* [in] */ Int32 direction, 
    /* [in] */ Int32 flags) 
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->AdjustVolume(direction, flags);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Dead object in adjustVolume", e);
    //}

    return NOERROR;
}

/**
 * Adjusts the volume of the most relevant stream, or the given fallback
 * stream.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param direction The direction to adjust the volume. One of
 *            {@link #ADJUST_LOWER}, {@link #ADJUST_RAISE}, or
 *            {@link #ADJUST_SAME}.
 * @param suggestedStreamType The stream type that will be used if there
 *            isn't a relevant stream. {@link #USE_DEFAULT_STREAM_TYPE} is valid here.
 * @param flags One or more flags.
 * @see #adjustVolume(Int32, Int32)
 * @see #adjustStreamVolume(Int32, Int32, Int32)
 * @see #setStreamVolume(Int32, Int32, Int32)
 */
ECode AudioManager::AdjustSuggestedStreamVolume(
    /* [in] */ Int32 direction, 
    /* [in] */ Int32 suggestedStreamType, 
    /* [in] */ Int32 flags)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->AdjustSuggestedStreamVolume(direction, suggestedStreamType, flags);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in adjustVolume", e);
    }*/

    return NOERROR;
}

/**
 * Returns the current ringtone mode.
 *
 * @return The current ringtone mode, one of {@link #RINGER_MODE_NORMAL},
 *         {@link #RINGER_MODE_SILENT}, or {@link #RINGER_MODE_VIBRATE}.
 * @see #setRingerMode(Int32)
 */
Int32 AudioManager::GetRingerMode()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Int32 mode;
        service->GetRingerMode(&mode);

        return mode;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in getRingerMode", e);
        return RINGER_MODE_NORMAL;
    }*/
}

/**
 * Returns the maximum volume index for a particular stream.
 *
 * @param streamType The stream type whose maximum volume index is returned.
 * @return The maximum valid volume index for the stream.
 * @see #getStreamVolume(Int32)
 */
Int32 AudioManager::GetStreamMaxVolume(
    /* [in] */ Int32 streamType)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Int32 volume;
        service->GetStreamMaxVolume(streamType, &volume);
        return volume;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in getStreamMaxVolume", e);
        return 0;
    }*/
}

/**
 * Returns the current volume index for a particular stream.
 *
 * @param streamType The stream type whose volume index is returned.
 * @return The current volume index for the stream.
 * @see #getStreamMaxVolume(Int32)
 * @see #setStreamVolume(Int32, Int32, Int32)
 */
Int32 AudioManager::GetStreamVolume(
    /* [in] */ Int32 streamType)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Int32 volume;
        service->GetStreamVolume(streamType, &volume);
        return volume;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in getStreamVolume", e);
        return 0;
    }*/
}

/**
 * Sets the ringer mode.
 * <p>
 * Silent mode will mute the volume and will not vibrate. Vibrate mode will
 * mute the volume and vibrate. Normal mode will be audible and may vibrate
 * according to user settings.
 *
 * @param ringerMode The ringer mode, one of {@link #RINGER_MODE_NORMAL},
 *            {@link #RINGER_MODE_SILENT}, or {@link #RINGER_MODE_VIBRATE}.
 * @see #getRingerMode()
 */
ECode AudioManager::SetRingerMode(
    /* [in] */ Int32 ringerMode)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->SetRingerMode(ringerMode);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setRingerMode", e);
    }*/

    return NOERROR;
}

/**
 * Sets the volume index for a particular stream.
 *
 * @param streamType The stream whose volume index should be set.
 * @param index The volume index to set. See
 *            {@link #getStreamMaxVolume(Int32)} for the largest valid value.
 * @param flags One or more flags.
 * @see #getStreamMaxVolume(Int32)
 * @see #getStreamVolume(Int32)
 */
ECode AudioManager::SetStreamVolume(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 index, 
    /* [in] */ Int32 flags)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->SetStreamVolume(streamType, index, flags);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setStreamVolume", e);
    }*/

        return NOERROR;
}

/**
 * Solo or unsolo a particular stream. All other streams are muted.
 * <p>
 * The solo command is protected against client process death: if a process
 * with an active solo request on a stream dies, all streams that were muted
 * because of this request will be unmuted automatically.
 * <p>
 * The solo requests for a given stream are cumulative: the AudioManager
 * can receive several solo requests from one or more clients and the stream
 * will be unsoloed only when the same number of unsolo requests are received.
 * <p>
 * For a better user experience, applications MUST unsolo a soloed stream
 * in onPause() and solo is again in onResume() if appropriate.
 *
 * @param streamType The stream to be soloed/unsoloed.
 * @param state The required solo state: TRUE for solo ON, FALSE for solo OFF
 */
ECode AudioManager::SetStreamSolo(
    /* [in] */ Int32 streamType, 
    /* [in] */ Boolean state)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->SetStreamSolo(streamType, state, mICallBack);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setStreamSolo", e);
    }*/

    return NOERROR;
}

/**
 * Mute or unmute an audio stream.
 * <p>
 * The mute command is protected against client process death: if a process
 * with an active mute request on a stream dies, this stream will be unmuted
 * automatically.
 * <p>
 * The mute requests for a given stream are cumulative: the AudioManager
 * can receive several mute requests from one or more clients and the stream
 * will be unmuted only when the same number of unmute requests are received.
 * <p>
 * For a better user experience, applications MUST unmute a muted stream
 * in onPause() and mute is again in onResume() if appropriate.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param streamType The stream to be muted/unmuted.
 * @param state The required mute state: TRUE for mute ON, FALSE for mute OFF
 */
ECode AudioManager::SetStreamMute(
    /* [in] */ Int32 streamType, 
    /* [in] */ Boolean state)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->SetStreamMute(streamType, state, mICallBack);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setStreamMute", e);
    }*/

    return NOERROR;
}

/**
 * Returns whether a particular type should vibrate according to user
 * settings and the current ringer mode.
 * <p>
 * This shouldn't be needed by most clients that use notifications to
 * vibrate. The notification manager will not vibrate if the policy doesn't
 * allow it, so the client should always set a vibrate pattern and let the
 * notification manager control whether or not to actually vibrate.
 *
 * @param vibrateType The type of vibrate. One of
 *            {@link #VIBRATE_TYPE_NOTIFICATION} or
 *            {@link #VIBRATE_TYPE_RINGER}.
 * @return Whether the type should vibrate at the instant this method is
 *         called.
 * @see #setVibrateSetting(Int32, Int32)
 * @see #getVibrateSetting(Int32)
 */
Boolean AudioManager::ShouldVibrate(
    /* [in] */ Int32 vibrateType)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Boolean vibrate;
        service->ShouldVibrate(vibrateType, &vibrate);
        return vibrate;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in shouldVibrate", e);
        return FALSE;
    }*/
}

/**
 * Returns whether the user's vibrate setting for a vibrate type.
 * <p>
 * This shouldn't be needed by most clients that want to vibrate, instead
 * see {@link #shouldVibrate(Int32)}.
 *
 * @param vibrateType The type of vibrate. One of
 *            {@link #VIBRATE_TYPE_NOTIFICATION} or
 *            {@link #VIBRATE_TYPE_RINGER}.
 * @return The vibrate setting, one of {@link #VIBRATE_SETTING_ON},
 *         {@link #VIBRATE_SETTING_OFF}, or
 *         {@link #VIBRATE_SETTING_ONLY_SILENT}.
 * @see #setVibrateSetting(Int32, Int32)
 * @see #shouldVibrate(Int32)
 */
Int32 AudioManager::GetVibrateSetting(
    /* [in] */ Int32 vibrateType)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Int32 setting;
        service->GetVibrateSetting(vibrateType, &setting);

        return setting;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in getVibrateSetting", e);
        return VIBRATE_SETTING_OFF;
    }*/
}

/**
 * Sets the setting for when the vibrate type should vibrate.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param vibrateType The type of vibrate. One of
 *            {@link #VIBRATE_TYPE_NOTIFICATION} or
 *            {@link #VIBRATE_TYPE_RINGER}.
 * @param vibrateSetting The vibrate setting, one of
 *            {@link #VIBRATE_SETTING_ON},
 *            {@link #VIBRATE_SETTING_OFF}, or
 *            {@link #VIBRATE_SETTING_ONLY_SILENT}.
 * @see #getVibrateSetting(Int32)
 * @see #shouldVibrate(Int32)
 */
ECode AudioManager::SetVibrateSetting(
    /* [in] */ Int32 vibrateType, 
    /* [in] */ Int32 vibrateSetting) 
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->SetVibrateSetting(vibrateType, vibrateSetting);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setVibrateSetting", e);
    }*/

        return NOERROR;
}

/**
 * Sets the speakerphone on or off.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param on set <var>TRUE</var> to turn on speakerphone;
 *           <var>FALSE</var> to turn it off
 */
ECode AudioManager::SetSpeakerphoneOn(
    /* [in] */ Boolean on)
{
    IAudioService* service = GetService();
    //try {
        service->SetSpeakerphoneOn(on);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setSpeakerphoneOn", e);
    }*/

        return NOERROR;
}

/**
 * Checks whether the speakerphone is on or off.
 *
 * @return TRUE if speakerphone is on, FALSE if it's off
 */
Boolean AudioManager::IsSpeakerphoneOn()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Boolean on;
        service->IsSpeakerphoneOn(&on);
        return on;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in isSpeakerphoneOn", e);
        return FALSE;
    }*/
}

/**
 * Indicates if current platform supports use of SCO for off call use cases.
 * Application wanted to use bluetooth SCO audio when the phone is not in call
 * must first call thsi method to make sure that the platform supports this
 * feature.
 * @return TRUE if bluetooth SCO can be used for audio when not in call
 *         FALSE otherwise
 * @see #startBluetoothSco()
*/
Boolean AudioManager::IsBluetoothScoAvailableOffCall()
{
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    Boolean result;
    res->GetBoolean(R_Bool_Config_bluetooth_sco_off_call, &result);
    return result;
}

/**
 * Start bluetooth SCO audio connection.
 * <p>Requires Permission:
 *   {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}.
 * <p>This method can be used by applications wanting to send and received audio
 * to/from a bluetooth SCO headset while the phone is not in call.
 * <p>As the SCO connection establishment can take several seconds,
 * applications should not rely on the connection to be available when the method
 * returns but instead register to receive the intent {@link #ACTION_SCO_AUDIO_STATE_CHANGED}
 * and wait for the state to be {@link #SCO_AUDIO_STATE_CONNECTED}.
 * <p>As the connection is not guaranteed to succeed, applications must wait for this intent with
 * a timeout.
 * <p>When finished with the SCO connection or if the establishment times out,
 * the application must call {@link #stopBluetoothSco()} to clear the request and turn
 * down the bluetooth connection.
 * <p>Even if a SCO connection is established, the following restrictions apply on audio
 * output streams so that they can be routed to SCO headset:
 * - the stream type must be {@link #STREAM_VOICE_CALL}
 * - the format must be mono
 * - the sampling must be 16kHz or 8kHz
 * <p>The following restrictions apply on input streams:
 * - the format must be mono
 * - the sampling must be 8kHz
 *
 * <p>Note that the phone application always has the priority on the usage of the SCO
 * connection for telephony. If this method is called while the phone is in call
 * it will be ignored. Similarly, if a call is received or sent while an application
 * is using the SCO connection, the connection will be lost for the application and NOT
 * returned automatically when the call ends.
 * @see #stopBluetoothSco()
 * @see #ACTION_SCO_AUDIO_STATE_CHANGED
 */
ECode AudioManager::StartBluetoothSco()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->StartBluetoothSco(mICallBack);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in startBluetoothSco", e);
    }*/

    return NOERROR;
}

/**
 * Stop bluetooth SCO audio connection.
 * <p>Requires Permission:
 *   {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}.
 * <p>This method must be called by applications having requested the use of
 * bluetooth SCO audio with {@link #startBluetoothSco()}
 * when finished with the SCO connection or if the establishment times out.
 * @see #startBluetoothSco()
 */
ECode AudioManager::StopBluetoothSco()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->StopBluetoothSco(mICallBack);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in stopBluetoothSco", e);
    }*/

    return NOERROR;
}

/**
 * Request use of Bluetooth SCO headset for communications.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param on set <var>TRUE</var> to use bluetooth SCO for communications;
 *               <var>FALSE</var> to not use bluetooth SCO for communications
 */
ECode AudioManager::SetBluetoothScoOn(
    /* [in] */ Boolean on)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->SetBluetoothScoOn(on);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setBluetoothScoOn", e);
    }*/

    return NOERROR;
}

/**
 * Checks whether communications use Bluetooth SCO.
 *
 * @return TRUE if SCO is used for communications;
 *         FALSE if otherwise
 */
Boolean AudioManager::IsBluetoothScoOn() 
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Boolean on;
        service->IsBluetoothScoOn(&on);
        return on;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in isBluetoothScoOn", e);
        return FALSE;
    }*/
}

/**
 * @param on set <var>TRUE</var> to route A2DP audio to/from Bluetooth
 *           headset; <var>FALSE</var> disable A2DP audio
 * @deprecated Do not use.
 */
ECode AudioManager::SetBluetoothA2dpOn(
    /* [in] */ Boolean on)
{
    return NOERROR;
}

/**
 * Checks whether A2DP audio routing to the Bluetooth headset is on or off.
 *
 * @return TRUE if A2DP audio is being routed to/from Bluetooth headset;
 *         FALSE if otherwise
 */
Boolean AudioManager::IsBluetoothA2dpOn()
{
    if (AudioSystem::GetDeviceConnectionState(AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP,String(""))
        == AudioSystem::DEVICE_STATE_UNAVAILABLE) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * Sets audio routing to the wired headset on or off.
 *
 * @param on set <var>TRUE</var> to route audio to/from wired
 *           headset; <var>FALSE</var> disable wired headset audio
 * @deprecated Do not use.
 */
ECode AudioManager::SetWiredHeadsetOn(
    /* [in] */ Boolean on)
{
    return NOERROR;
}

/**
 * Checks whether audio routing to the wired headset is on or off.
 *
 * @return TRUE if audio is being routed to/from wired headset;
 *         FALSE if otherwise
 */
Boolean AudioManager::IsWiredHeadsetOn() 
{
    if (AudioSystem::GetDeviceConnectionState(AudioSystem::DEVICE_OUT_WIRED_HEADSET,String(""))
        == AudioSystem::DEVICE_STATE_UNAVAILABLE &&
            AudioSystem::GetDeviceConnectionState(AudioSystem::DEVICE_OUT_WIRED_HEADPHONE,String(""))
            == AudioSystem::DEVICE_STATE_UNAVAILABLE) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * Sets the microphone mute on or off.
 * <p>
 * This method should only be used by applications that replace the platform-wide
 * management of audio settings or the main telephony application.
 *
 * @param on set <var>TRUE</var> to mute the microphone;
 *           <var>FALSE</var> to turn mute off
 */
ECode AudioManager::SetMicrophoneMute(
    /* [in] */ Boolean on)
{
    AudioSystem::MuteMicrophone(on);

    return NOERROR;
}

/**
 * Checks whether the microphone mute is on or off.
 *
 * @return TRUE if microphone is muted, FALSE if it's not
 */
Boolean AudioManager::IsMicrophoneMute() 
{
    return AudioSystem::IsMicrophoneMuted();
}

/**
 * Sets the audio mode.
 * <p>
 * The audio mode encompasses audio routing AND the behavior of
 * the telephony layer. Therefore this method should only be used by applications that
 * replace the platform-wide management of audio settings or the main telephony application.
 * In particular, the {@link #MODE_IN_CALL} mode should only be used by the telephony
 * application when it places a phone call, as it will cause signals from the radio layer
 * to feed the platform mixer.
 *
 * @param mode  the requested audio mode (NORMAL, RINGTONE, or IN_CALL).
 *              Informs the HAL about the current audio state so that
 *              it can route the audio appropriately.
 */
ECode AudioManager::SetMode(
    /* [in] */ Int32 mode)
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->SetMode(mode, mICallBack);

        return NOERROR;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in setMode", e);
    }*/
}

/**
 * Returns the current audio mode.
 *
 * @return      the current audio mode (NORMAL, RINGTONE, or IN_CALL).
 *              Returns the current current audio state from the HAL.
 */
Int32 AudioManager::GetMode()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Int32 mode;
        service->GetMode(&mode);
        return mode;
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in getMode", e);
        return MODE_INVALID;
    }*/
}

/**
 * Sets the audio routing for a specified mode
 *
 * @param mode   audio mode to change route. E.g., MODE_RINGTONE.
 * @param routes bit vector of routes requested, created from one or
 *               more of ROUTE_xxx types. Set bits indicate that route should be on
 * @param mask   bit vector of routes to change, created from one or more of
 * ROUTE_xxx types. Unset bits indicate the route should be left unchanged
 *
 * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
 * setBluetoothScoOn() methods instead.
 */
ECode AudioManager::SetRouting(
    /* [in] */ Int32 mode, 
    /* [in] */ Int32 routes, 
    /* [in] */ Int32 mask)
{
    return NOERROR;
}

/**
 * Returns the current audio routing bit vector for a specified mode.
 *
 * @param mode audio mode to get route (e.g., MODE_RINGTONE)
 * @return an audio route bit vector that can be compared with ROUTE_xxx
 * bits
 * @deprecated   Do not query audio routing directly, use isSpeakerphoneOn(),
 * isBluetoothScoOn(), isBluetoothA2dpOn() and isWiredHeadsetOn() methods instead.
 */
Int32 AudioManager::GetRouting(
    /* [in] */ Int32 mode)
{
    return -1;
}

/**
 * Checks whether any music is active.
 *
 * @return TRUE if any music tracks are active.
 */
Boolean AudioManager::IsMusicActive()
{
    return AudioSystem::IsStreamActive(STREAM_MUSIC);
}

/*
 * Sets a generic audio configuration parameter. The use of these parameters
 * are platform dependant, see libaudio
 *
 * ** Temporary interface - DO NOT USE
 *
 * TODO: Replace with a more generic key:value get/set mechanism
 *
 * param key   name of parameter to set. Must not be NULL.
 * param value value of parameter. Must not be NULL.
 */
/**
 * @hide
 * @deprecated Use {@link #setPrameters(String)} instead
 */
ECode AudioManager::SetParameter(
    /* [in] */ String key, 
    /* [in] */ String value)
{
    SetParameters(key+"="+value);

    return NOERROR;
}

/**
 * Sets a variable number of parameter values to audio hardware.
 *
 * @param keyValuePairs list of parameters key value pairs in the form:
 *    key1=value1;key2=value2;...
 *
 */
ECode AudioManager::SetParameters(
    /* [in] */ String keyValuePairs)
{
    AudioSystem::SetParameters(keyValuePairs);

    return NOERROR;
}

/**
 * Sets a varaible number of parameter values to audio hardware.
 *
 * @param keys list of parameters
 * @return list of parameters key value pairs in the form:
 *    key1=value1;key2=value2;...
 */
String AudioManager::GetParameters(
    /* [in] */ String keys)
{
    return AudioSystem::GetParameters(keys);
}

/**
 * Plays a sound effect (Key clicks, lid open/close...)
 * @param effectType The type of sound effect. One of
 *            {@link #FX_KEY_CLICK},
 *            {@link #FX_FOCUS_NAVIGATION_UP},
 *            {@link #FX_FOCUS_NAVIGATION_DOWN},
 *            {@link #FX_FOCUS_NAVIGATION_LEFT},
 *            {@link #FX_FOCUS_NAVIGATION_RIGHT},
 *            {@link #FX_KEYPRESS_STANDARD},
 *            {@link #FX_KEYPRESS_SPACEBAR},
 *            {@link #FX_KEYPRESS_DELETE},
 *            {@link #FX_KEYPRESS_RETURN},
 * NOTE: This version uses the UI settings to determine
 * whether sounds are heard or not.
 */
ECode AudioManager::PlaySoundEffect(
    /* [in] */ Int32 effectType) 
{
    if (effectType < 0 || effectType >= NUM_SOUND_EFFECTS) {
        return E_INVALID_ARGUMENT;
    }

    if (!QuerySoundEffectsEnabled()) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IAudioService> service = GetService();
    //try {
        service->PlaySoundEffect(effectType);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in playSoundEffect"+e);
    }*/

    return NOERROR;
}

/**
 * Plays a sound effect (Key clicks, lid open/close...)
 * @param effectType The type of sound effect. One of
 *            {@link #FX_KEY_CLICK},
 *            {@link #FX_FOCUS_NAVIGATION_UP},
 *            {@link #FX_FOCUS_NAVIGATION_DOWN},
 *            {@link #FX_FOCUS_NAVIGATION_LEFT},
 *            {@link #FX_FOCUS_NAVIGATION_RIGHT},
 *            {@link #FX_KEYPRESS_STANDARD},
 *            {@link #FX_KEYPRESS_SPACEBAR},
 *            {@link #FX_KEYPRESS_DELETE},
 *            {@link #FX_KEYPRESS_RETURN},
 * @param volume Sound effect volume.
 * The volume value is a raw scalar so UI controls should be scaled logarithmically.
 * If a volume of -1 is specified, the AudioManager.STREAM_MUSIC stream volume minus 3dB will be used.
 * NOTE: This version is for applications that have their own
 * settings panel for enabling and controlling volume.
 */
ECode AudioManager::PlaySoundEffect(
    /* [in] */ Int32 effectType, 
    /* [in] */ Float volume)
{
    if (effectType < 0 || effectType >= NUM_SOUND_EFFECTS) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IAudioService> service = GetService();
    //try {
        service->PlaySoundEffectVolume(effectType, volume);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in playSoundEffect"+e);
    }*/

    return NOERROR;
}

/**
 * Settings has an in memory cache, so this is fast.
 */
Boolean AudioManager::QuerySoundEffectsEnabled()
{
    //return Settings.System.getInt(mContext.getContentResolver(), Settings.System.SOUND_EFFECTS_ENABLED, 0) != 0;

    return FALSE;
}


/**
 *  Load Sound effects.
 *  This method must be called when sound effects are enabled.
 */
ECode AudioManager::LoadSoundEffects() 
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        Boolean res;
        service->LoadSoundEffects(&res);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in loadSoundEffects"+e);
    }*/

    return NOERROR;
}

/**
 *  Unload Sound effects.
 *  This method can be called to free some memory when
 *  sound effects are disabled.
 */
ECode AudioManager::UnloadSoundEffects()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->UnloadSoundEffects();
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in unloadSoundEffects"+e);
    }*/

    return NOERROR;
}

AutoPtr<IOnAudioFocusChangeListener> AudioManager::FindFocusListener(
    /* [in] */ String id)
{
    //return mAudioFocusIdListenerMap.Get(id);

    return NULL;
}

/**
 * Helper class to handle the forwarding of audio focus events to the appropriate listener
 */
AudioManager::FocusEventHandlerDelegate::FocusEventHandlerDelegate()
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

AutoPtr<IHandler> AudioManager::FocusEventHandlerDelegate::GetHandler()
{
    return mHandler;
}

String AudioManager::GetIdForAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l) 
{
    /*if (l == NULL) {
        return new String(this.toString());
    } else {
        return new String(this.toString() + l.toString());
    }*/

    return String("");
}

/**
 * @hide
 * Registers a listener to be called when audio focus changes. Calling this method is optional
 * before calling {@link #requestAudioFocus(OnAudioFocusChangeListener, Int32, Int32)}, as it
 * will register the listener as well if it wasn't registered already.
 * @param l the listener to be notified of audio focus changes.
 */
ECode AudioManager::RegisterAudioFocusListener(
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

/**
 * @hide
 * Causes the specified listener to not be called anymore when focus is gained or lost.
 * @param l the listener to unregister.
 */
ECode AudioManager::UnregisterAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    // remove locally
    //synchronized(mFocusListenerLock) {
        //mAudioFocusIdListenerMap.Remove(GetIdForAudioFocusListener(l));
    //}

    return NOERROR;
}

/**
 *  Request audio focus.
 *  Send a request to obtain the audio focus
 *  @param l the listener to be notified of audio focus changes
 *  @param streamType the main audio stream type affected by the focus request
 *  @param durationHint use {@link #AUDIOFOCUS_GAIN_TRANSIENT} to indicate this focus request
 *      is temporary, and focus will be abandonned shortly. Examples of transient requests are
 *      for the playback of driving directions, or notifications sounds.
 *      Use {@link #AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK} to indicate also that it's ok for
 *      the previous focus owner to keep playing if it ducks its audio output.
 *      Use {@link #AUDIOFOCUS_GAIN} for a focus request of unknown duration such
 *      as the playback of a song or a video.
 *  @return {@link #AUDIOFOCUS_REQUEST_FAILED} or {@link #AUDIOFOCUS_REQUEST_GRANTED}
 */
Int32 AudioManager::RequestAudioFocus(
    /* [in] */ IOnAudioFocusChangeListener* l, 
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 durationHint)
{
    Int32 status = AUDIOFOCUS_REQUEST_FAILED;
    if ((durationHint < AUDIOFOCUS_GAIN) || (durationHint > AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK))
    {
        //Log.e(TAG, "Invalid duration hint, audio focus request denied");
        return status;
    }
    RegisterAudioFocusListener(l);
    //TODO protect request by permission check?
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->RequestAudioFocus(streamType, durationHint, mICallBack,
                mAudioFocusDispatcher, GetIdForAudioFocusListener(l), &status);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Can't call requestAudioFocus() from AudioService due to "+e);
    }*/
    return status;
}


/**
 *  Abandon audio focus. Causes the previous focus owner, if any, to receive focus.
 *  @param l the listener with which focus was requested.
 *  @return {@link #AUDIOFOCUS_REQUEST_FAILED} or {@link #AUDIOFOCUS_REQUEST_GRANTED}
 */
Int32 AudioManager::AbandonAudioFocus(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    Int32 status = AUDIOFOCUS_REQUEST_FAILED;
    UnregisterAudioFocusListener(l);
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->AbandonAudioFocus(mAudioFocusDispatcher,
                GetIdForAudioFocusListener(l), &status);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Can't call abandonAudioFocus() from AudioService due to "+e);
    }*/
    return status;
}


//====================================================================
// Remote Control
/**
 * Register a component to be the sole receiver of MEDIA_BUTTON intents.
 * @param eventReceiver identifier of a {@link android.content.BroadcastReceiver}
 *      that will receive the media button intent. This broadcast receiver must be declared
 *      in the application manifest.
 */
ECode AudioManager::RegisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    //TODO enforce the rule about the receiver being declared in the manifest
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->RegisterMediaButtonEventReceiver(eventReceiver);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in registerMediaButtonEventReceiver"+e);
    }*/

    return NOERROR;
}

/**
 * Unregister the receiver of MEDIA_BUTTON intents.
 * @param eventReceiver identifier of a {@link android.content.BroadcastReceiver}
 *      that was registered with {@link #registerMediaButtonEventReceiver(ComponentName)}.
 */
ECode AudioManager::UnregisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver) 
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->UnregisterMediaButtonEventReceiver(eventReceiver);
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in unregisterMediaButtonEventReceiver"+e);
    }*/

    return NOERROR;
}

/**
 *  @hide
 *  Reload audio settings. This method is called by Settings backup
 *  agent when audio settings are restored and causes the AudioService
 *  to read and apply restored settings.
 */
ECode AudioManager::ReloadAudioSettings()
{
    AutoPtr<IAudioService> service = GetService();
    //try {
        service->ReloadAudioSettings();
    /*} catch (RemoteException e) {
        Log.e(TAG, "Dead object in reloadAudioSettings"+e);
    }*/

    return NOERROR;
}
