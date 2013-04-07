
#include "media/ElAudioSystem.h"
#include <media/AudioSystem.h>

/* The audio stream for phone calls */
const Int32 AudioSystem::STREAM_VOICE_CALL;
/* The audio stream for system sounds */
const Int32 AudioSystem::STREAM_SYSTEM;
/* The audio stream for the phone ring and message alerts */
const Int32 AudioSystem::STREAM_RING;
/* The audio stream for music playback */
const Int32 AudioSystem::STREAM_MUSIC;
/* The audio stream for alarms */
const Int32 AudioSystem::STREAM_ALARM;
/* The audio stream for notifications */
const Int32 AudioSystem::STREAM_NOTIFICATION;
/* @hide The audio stream for phone calls when connected on bluetooth */
const Int32 AudioSystem::STREAM_BLUETOOTH_SCO;
/* @hide The audio stream for enforced system sounds in certain countries (e.g camera in Japan) */
const Int32 AudioSystem::STREAM_SYSTEM_ENFORCED;
/* @hide The audio stream for DTMF tones */
const Int32 AudioSystem::STREAM_DTMF;
/* @hide The audio stream for text to speech (TTS) */
const Int32 AudioSystem::STREAM_TTS;
/**
 * @deprecated Use {@link #numStreamTypes() instead}
 */
const Int32 AudioSystem::NUM_STREAMS;

/* modes for setPhoneState */
const Int32 AudioSystem::MODE_INVALID;
const Int32 AudioSystem::MODE_CURRENT;
const Int32 AudioSystem::MODE_NORMAL;
const Int32 AudioSystem::MODE_RINGTONE;
const Int32 AudioSystem::MODE_IN_CALL;
const Int32 AudioSystem::MODE_IN_COMMUNICATION;
const Int32 AudioSystem::NUM_MODES;

/* Routing bits for setRouting/getRouting API */
/** @deprecated */
const Int32 AudioSystem::ROUTE_EARPIECE;

/** @deprecated */
const Int32 AudioSystem::ROUTE_SPEAKER;

/** @deprecated use {@link #ROUTE_BLUETOOTH_SCO} */
const Int32 AudioSystem::ROUTE_BLUETOOTH;

/** @deprecated */
const Int32 AudioSystem::ROUTE_BLUETOOTH_SCO;

/** @deprecated */
const Int32 AudioSystem::ROUTE_HEADSET;

/** @deprecated */
const Int32 AudioSystem::ROUTE_BLUETOOTH_A2DP;

/** @deprecated */
const Int32 AudioSystem::ROUTE_ALL;

// These match the enum in libs/android_runtime/android_media_AudioSystem.cpp
/* Command sucessful or Media server restarted. see ErrorCallback */
const Int32 AudioSystem::AUDIO_STATUS_OK;

/* Command failed or unspecified audio error.  see ErrorCallback */
const Int32 AudioSystem::AUDIO_STATUS_ERROR;

/* Media server died. see ErrorCallback */
const Int32 AudioSystem::AUDIO_STATUS_SERVER_DIED;

// output devices
const Int32 AudioSystem::DEVICE_OUT_EARPIECE;
const Int32 AudioSystem::DEVICE_OUT_SPEAKER;
const Int32 AudioSystem::DEVICE_OUT_WIRED_HEADSET;
const Int32 AudioSystem::DEVICE_OUT_WIRED_HEADPHONE;
const Int32 AudioSystem::DEVICE_OUT_BLUETOOTH_SCO;
const Int32 AudioSystem::DEVICE_OUT_BLUETOOTH_SCO_HEADSET;
const Int32 AudioSystem::DEVICE_OUT_BLUETOOTH_SCO_CARKIT;
const Int32 AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP;
const Int32 AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES;
const Int32 AudioSystem::DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER;
const Int32 AudioSystem::DEVICE_OUT_AUX_DIGITAL;
const Int32 AudioSystem::DEVICE_OUT_DEFAULT;
// input devices
const Int32 AudioSystem::DEVICE_IN_COMMUNICATION;
const Int32 AudioSystem::DEVICE_IN_AMBIENT;
const Int32 AudioSystem::DEVICE_IN_BUILTIN_MIC1;
const Int32 AudioSystem::DEVICE_IN_BUILTIN_MIC2;
const Int32 AudioSystem::DEVICE_IN_MIC_ARRAY;
const Int32 AudioSystem::DEVICE_IN_BLUETOOTH_SCO_HEADSET;
const Int32 AudioSystem::DEVICE_IN_WIRED_HEADSET;
const Int32 AudioSystem::DEVICE_IN_AUX_DIGITAL;
const Int32 AudioSystem::DEVICE_IN_DEFAULT;

// device states
const Int32 AudioSystem::DEVICE_STATE_UNAVAILABLE;
const Int32 AudioSystem::DEVICE_STATE_AVAILABLE;

// phone state
const Int32 AudioSystem::PHONE_STATE_OFFCALL;
const Int32 AudioSystem::PHONE_STATE_RINGING;
const Int32 AudioSystem::PHONE_STATE_INCALL;

// config for setForceUse
const Int32 AudioSystem::FORCE_NONE;
const Int32 AudioSystem::FORCE_SPEAKER;
const Int32 AudioSystem::FORCE_HEADPHONES;
const Int32 AudioSystem::FORCE_BT_SCO;
const Int32 AudioSystem::FORCE_BT_A2DP;
const Int32 AudioSystem::FORCE_WIRED_ACCESSORY;
const Int32 AudioSystem::FORCE_BT_CAR_DOCK;
const Int32 AudioSystem::FORCE_BT_DESK_DOCK;
const Int32 AudioSystem::FORCE_DEFAULT;

// usage for serForceUse
const Int32 AudioSystem::FOR_COMMUNICATION;
const Int32 AudioSystem::FOR_MEDIA;
const Int32 AudioSystem::FOR_RECORD;
const Int32 AudioSystem::FOR_DOCK;

const Int32 AudioSystem::NUM_STREAM_TYPES;
AutoPtr<IAudioSystemErrorCallback> AudioSystem::mErrorCallback;


Int32 AudioSystem::GetNumStreamTypes()
{
    return NUM_STREAM_TYPES;
}

enum AudioError {
    kAudioStatusOk = 0,
    kAudioStatusError = 1,
    kAudioStatusMediaServerDied = 100
};

static Int32 check_AudioSystem_Command(android::status_t status)
{
    if (status == android::NO_ERROR) {
        return kAudioStatusOk;
    }
    else {
        return kAudioStatusError;
    }
}

/*
 * Sets the microphone mute on or off.
 *
 * param on set <var>true</var> to mute the microphone;
 *           <var>false</var> to turn mute off
 * return command completion status see AUDIO_STATUS_OK, see AUDIO_STATUS_ERROR
 */
Int32 AudioSystem::MuteMicrophone(
    /* [in] */ Boolean on)
{
    return check_AudioSystem_Command(android::AudioSystem::muteMicrophone(on));
}

/*
 * Checks whether the microphone mute is on or off.
 *
 * return true if microphone is muted, false if it's not
 */
Boolean AudioSystem::IsMicrophoneMuted()
{
    bool state = false;
    android::AudioSystem::isMicrophoneMuted(&state);
    return (Boolean)state;
}

/*
 * Sets the audio mode.
 *
 * param mode  the requested audio mode (NORMAL, RINGTONE, or IN_CALL).
 *              Informs the HAL about the current audio state so that
 *              it can route the audio appropriately.
 * return command completion status see AUDIO_STATUS_OK, see AUDIO_STATUS_ERROR
 */
/** @deprecated use {@link #setPhoneState(Int32)} */
Int32 AudioSystem::SetMode(
    /* [in] */ Int32 mode)
{
    return AUDIO_STATUS_ERROR;
}
/*
 * Returns the current audio mode.
 *
 * return      the current audio mode (NORMAL, RINGTONE, or IN_CALL).
 *              Returns the current current audio state from the HAL.
 *
 */
/** @deprecated Do not use. */
Int32 AudioSystem::GetMode()
{
    return MODE_INVALID;
}

/*
 * Sets the audio routing for a specified mode
 *
 * param mode   audio mode to change route. E.g., MODE_RINGTONE.
 * param routes bit vector of routes requested, created from one or
 *               more of ROUTE_xxx types. Set bits indicate that route should be on
 * param mask   bit vector of routes to change, created from one or more of
 * ROUTE_xxx types. Unset bits indicate the route should be left unchanged
 * return command completion status see AUDIO_STATUS_OK, see AUDIO_STATUS_ERROR
 */
/** @deprecated use {@link #setDeviceConnectionState(Int32,Int32,String)} */
Int32 AudioSystem::SetRouting(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 routes,
    /* [in] */ Int32 mask)
{
    return AUDIO_STATUS_ERROR;
}

/*
 * Returns the current audio routing bit vector for a specified mode.
 *
 * param mode audio mode to change route (e.g., MODE_RINGTONE)
 * return an audio route bit vector that can be compared with ROUTE_xxx
 * bits
 */
/** @deprecated use {@link #getDeviceConnectionState(Int32,String)} */
Int32 AudioSystem::GetRouting(
    /* [in] */ Int32 mode)
{
    return 0;
}

/*
 * Checks whether the specified stream type is active.
 *
 * return true if any track playing on this stream is active.
 */
Boolean AudioSystem::IsStreamActive(
    /* [in] */ Int32 stream)
{
    bool state = false;
    android::AudioSystem::isStreamActive(stream, &state);
    return (Boolean)state;
}

/*
 * Sets a group generic audio configuration parameters. The use of these parameters
 * are platform dependant, see libaudio
 *
 * param keyValuePairs  list of parameters key value pairs in the form:
 *    key1=value1;key2=value2;...
 */
Int32 AudioSystem::SetParameters(
    /* [in] */ const String& keyValuePairs)
{
    android::String8 c_keyValuePairs8(keyValuePairs.string());
    int status = check_AudioSystem_Command(android::AudioSystem::setParameters(0, c_keyValuePairs8));
    return status;
}

/*
 * Gets a group generic audio configuration parameters. The use of these parameters
 * are platform dependant, see libaudio
 *
 * param keys  list of parameters
 * return value: list of parameters key value pairs in the form:
 *    key1=value1;key2=value2;...
 */
String AudioSystem::GetParameters(
    /* [in] */ const String& keys)
{
    android::String8 c_keys8(keys.string());

    return String(android::AudioSystem::getParameters(0, c_keys8).string());
}

/*
 * Registers a callback to be invoked when an error occurs.
 * param cb the callback to run
 */
void AudioSystem::SetErrorCallback(
    /* [in] */ IAudioSystemErrorCallback* cb)
{
    mErrorCallback = cb;
}

void AudioSystem::ErrorCallbackFromNative(
    /* [in] */ Int32 error)
{
    if (mErrorCallback != NULL) {
        mErrorCallback->OnError(error);
    }
}

Int32 AudioSystem::SetDeviceConnectionState(
    /* [in] */ Int32 device,
    /* [in] */ Int32 state,
    /* [in] */ const String& device_address)
{
    const char *c_address = device_address.string();
    int status = check_AudioSystem_Command(
            android::AudioSystem::setDeviceConnectionState(static_cast<android::AudioSystem::audio_devices>(device),
                    static_cast<android::AudioSystem::device_connection_state>(state), c_address));
    return status;
}

Int32 AudioSystem::GetDeviceConnectionState(
    /* [in] */ Int32 device,
    /* [in] */ const String& device_address)
{
    const char *c_address = device_address.string();
    int state = static_cast<int>(android::AudioSystem::getDeviceConnectionState(
            static_cast<android::AudioSystem::audio_devices>(device), c_address));
    return state;
}

Int32 AudioSystem::SetPhoneState(
    /* [in] */ Int32 state)
{
    return check_AudioSystem_Command(android::AudioSystem::setPhoneState(state));
}

Int32 AudioSystem::SetRingerMode(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 mask)
{
    return check_AudioSystem_Command(android::AudioSystem::setRingerMode(mode, mask));
}

Int32 AudioSystem::SetForceUse(
    /* [in] */ Int32 usage,
    /* [in] */ Int32 config)
{
    return check_AudioSystem_Command(android::AudioSystem::setForceUse(
            static_cast<android::AudioSystem::force_use>(usage),
            static_cast<android::AudioSystem::forced_config>(config)));
}

Int32 AudioSystem::GetForceUse(
    /* [in] */ Int32 usage)
{
    return static_cast<int>(android::AudioSystem::getForceUse(
            static_cast<android::AudioSystem::force_use>(usage)));
}

Int32 AudioSystem::InitStreamVolume(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 indexMin,
    /* [in] */ Int32 indexMax)
{
    return check_AudioSystem_Command(android::AudioSystem::initStreamVolume(
            static_cast<android::AudioSystem::stream_type>(stream), indexMin, indexMax));
}

Int32 AudioSystem::SetStreamVolumeIndex(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 index)
{
    return check_AudioSystem_Command(android::AudioSystem::setStreamVolumeIndex(
            static_cast<android::AudioSystem::stream_type>(stream), index));
}

Int32 AudioSystem::GetStreamVolumeIndex(
    /* [in] */ Int32 stream)
{
    int index;
    if (android::AudioSystem::getStreamVolumeIndex(
            static_cast<android::AudioSystem::stream_type>(stream), &index) != android::NO_ERROR) {
        index = -1;
    }
    return index;
}
