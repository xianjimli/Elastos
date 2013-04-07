
#ifndef __ELAUDIOSYSTEM_H__
#define __ELAUDIOSYSTEM_H__

/* IF YOU CHANGE ANY OF THE CONSTANTS IN THIS FILE, DO NOT FORGET
 * TO UPDATE THE CORRESPONDING NATIVE GLUE.  THANK YOU FOR YOUR COOPERATION
 */

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * @hide
 */
class AudioSystem
{
public:
    static CARAPI_(Int32) GetNumStreamTypes();

    /*
     * Sets the microphone mute on or off.
     *
     * param on set <var>true</var> to mute the microphone;
     *           <var>false</var> to turn mute off
     * return command completion status see AUDIO_STATUS_OK, see AUDIO_STATUS_ERROR
     */
    static CARAPI_(Int32) MuteMicrophone(
        /* [in] */ Boolean on);

    /*
     * Checks whether the microphone mute is on or off.
     *
     * return true if microphone is muted, false if it's not
     */
    static CARAPI_(Boolean) IsMicrophoneMuted();

    /*
     * Sets the audio mode.
     *
     * param mode  the requested audio mode (NORMAL, RINGTONE, or IN_CALL).
     *              Informs the HAL about the current audio state so that
     *              it can route the audio appropriately.
     * return command completion status see AUDIO_STATUS_OK, see AUDIO_STATUS_ERROR
     */
    /** @deprecated use {@link #setPhoneState(Int32)} */
    static CARAPI_(Int32) SetMode(
        /* [in] */ Int32 mode);

    /*
     * Returns the current audio mode.
     *
     * return      the current audio mode (NORMAL, RINGTONE, or IN_CALL).
     *              Returns the current current audio state from the HAL.
     *
     */
    /** @deprecated Do not use. */
    static CARAPI_(Int32) GetMode();

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
    static CARAPI_(Int32) SetRouting(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 routes,
        /* [in] */ Int32 mask);

    /*
     * Returns the current audio routing bit vector for a specified mode.
     *
     * param mode audio mode to change route (e.g., MODE_RINGTONE)
     * return an audio route bit vector that can be compared with ROUTE_xxx
     * bits
     */
    /** @deprecated use {@link #getDeviceConnectionState(Int32,String)} */
    static CARAPI_(Int32) GetRouting(
        /* [in] */ Int32 mode);

    /*
     * Checks whether the specified stream type is active.
     *
     * return true if any track playing on this stream is active.
     */
    static CARAPI_(Boolean) IsStreamActive(
        /* [in] */ Int32 stream);

    /*
     * Sets a group generic audio configuration parameters. The use of these parameters
     * are platform dependant, see libaudio
     *
     * param keyValuePairs  list of parameters key value pairs in the form:
     *    key1=value1;key2=value2;...
     */
    static CARAPI_(Int32) SetParameters(
        /* [in] */ const String& keyValuePairs);

    /*
     * Gets a group generic audio configuration parameters. The use of these parameters
     * are platform dependant, see libaudio
     *
     * param keys  list of parameters
     * return value: list of parameters key value pairs in the form:
     *    key1=value1;key2=value2;...
     */
    static CARAPI_(String) GetParameters(
        /* [in] */ const String& keys);

    /*
     * Registers a callback to be invoked when an error occurs.
     * param cb the callback to run
     */
    static CARAPI_(void) SetErrorCallback(
        /* [in] */ IAudioSystemErrorCallback* cb);

    /*
     * AudioPolicyService methods
     */
    static CARAPI_(Int32) SetDeviceConnectionState(
        /* [in] */ Int32 device,
        /* [in] */ Int32 state,
        /* [in] */ const String& device_address);

    static CARAPI_(Int32) GetDeviceConnectionState(
        /* [in] */ Int32 device,
        /* [in] */ const String& device_address);

    static CARAPI_(Int32) SetPhoneState(
        /* [in] */ Int32 state);

    static CARAPI_(Int32) SetRingerMode(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 mask);

    static CARAPI_(Int32) SetForceUse(
        /* [in] */ Int32 usage,
        /* [in] */ Int32 config);

    static CARAPI_(Int32) GetForceUse(
        /* [in] */ Int32 usage);

    static CARAPI_(Int32) InitStreamVolume(
        /* [in] */ Int32 stream,
        /* [in] */ Int32 indexMin,
        /* [in] */ Int32 indexMax);

    static CARAPI_(Int32) SetStreamVolumeIndex(
        /* [in] */ Int32 stream,
        /* [in] */ Int32 index);

    static CARAPI_(Int32) GetStreamVolumeIndex(
        /* [in] */ Int32 stream);

private:
    static CARAPI_(void) ErrorCallbackFromNative(
        /* [in] */ Int32 error);

public:
    /* FIXME: Need to finalize this and correlate with native layer */
    /*
     * If these are modified, please also update Settings.System.VOLUME_SETTINGS
     * and attrs.xml
     */
    /* The audio stream for phone calls */
    static const Int32 STREAM_VOICE_CALL = 0;
    /* The audio stream for system sounds */
    static const Int32 STREAM_SYSTEM = 1;
    /* The audio stream for the phone ring and message alerts */
    static const Int32 STREAM_RING = 2;
    /* The audio stream for music playback */
    static const Int32 STREAM_MUSIC = 3;
    /* The audio stream for alarms */
    static const Int32 STREAM_ALARM = 4;
    /* The audio stream for notifications */
    static const Int32 STREAM_NOTIFICATION = 5;
    /* @hide The audio stream for phone calls when connected on bluetooth */
    static const Int32 STREAM_BLUETOOTH_SCO = 6;
    /* @hide The audio stream for enforced system sounds in certain countries (e.g camera in Japan) */
    static const Int32 STREAM_SYSTEM_ENFORCED = 7;
    /* @hide The audio stream for DTMF tones */
    static const Int32 STREAM_DTMF = 8;
    /* @hide The audio stream for text to speech (TTS) */
    static const Int32 STREAM_TTS = 9;
    /**
     * @deprecated Use {@link #numStreamTypes() instead}
     */
    static const Int32 NUM_STREAMS = 5;

    /* modes for setPhoneState */
    static const Int32 MODE_INVALID            = -2;
    static const Int32 MODE_CURRENT            = -1;
    static const Int32 MODE_NORMAL             = 0;
    static const Int32 MODE_RINGTONE           = 1;
    static const Int32 MODE_IN_CALL            = 2;
    static const Int32 MODE_IN_COMMUNICATION   = 3;
    static const Int32 NUM_MODES               = 4;


    /* Routing bits for setRouting/getRouting API */
    /** @deprecated */
    static const Int32 ROUTE_EARPIECE          = (1 << 0);

    /** @deprecated */
    static const Int32 ROUTE_SPEAKER           = (1 << 1);

    /** @deprecated use {@link #ROUTE_BLUETOOTH_SCO} */
    static const Int32 ROUTE_BLUETOOTH = (1 << 2);

    /** @deprecated */
    static const Int32 ROUTE_BLUETOOTH_SCO     = (1 << 2);

    /** @deprecated */
    static const Int32 ROUTE_HEADSET           = (1 << 3);

    /** @deprecated */
    static const Int32 ROUTE_BLUETOOTH_A2DP    = (1 << 4);

    /** @deprecated */
    static const Int32 ROUTE_ALL               = 0xFFFFFFFF;

    // These match the enum in libs/android_runtime/android_media_AudioSystem.cpp
    /* Command sucessful or Media server restarted. see ErrorCallback */
    static const Int32 AUDIO_STATUS_OK = 0;

    /* Command failed or unspecified audio error.  see ErrorCallback */
    static const Int32 AUDIO_STATUS_ERROR = 1;

    /* Media server died. see ErrorCallback */
    static const Int32 AUDIO_STATUS_SERVER_DIED = 100;

    // output devices
    static const Int32 DEVICE_OUT_EARPIECE = 0x1;
    static const Int32 DEVICE_OUT_SPEAKER = 0x2;
    static const Int32 DEVICE_OUT_WIRED_HEADSET = 0x4;
    static const Int32 DEVICE_OUT_WIRED_HEADPHONE = 0x8;
    static const Int32 DEVICE_OUT_BLUETOOTH_SCO = 0x10;
    static const Int32 DEVICE_OUT_BLUETOOTH_SCO_HEADSET = 0x20;
    static const Int32 DEVICE_OUT_BLUETOOTH_SCO_CARKIT = 0x40;
    static const Int32 DEVICE_OUT_BLUETOOTH_A2DP = 0x80;
    static const Int32 DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES = 0x100;
    static const Int32 DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER = 0x200;
    static const Int32 DEVICE_OUT_AUX_DIGITAL = 0x400;
    static const Int32 DEVICE_OUT_DEFAULT = 0x8000;
    // input devices
    static const Int32 DEVICE_IN_COMMUNICATION = 0x10000;
    static const Int32 DEVICE_IN_AMBIENT = 0x20000;
    static const Int32 DEVICE_IN_BUILTIN_MIC1 = 0x40000;
    static const Int32 DEVICE_IN_BUILTIN_MIC2 = 0x80000;
    static const Int32 DEVICE_IN_MIC_ARRAY = 0x100000;
    static const Int32 DEVICE_IN_BLUETOOTH_SCO_HEADSET = 0x200000;
    static const Int32 DEVICE_IN_WIRED_HEADSET = 0x400000;
    static const Int32 DEVICE_IN_AUX_DIGITAL = 0x800000;
    static const Int32 DEVICE_IN_DEFAULT = 0x80000000;

    // device states
    static const Int32 DEVICE_STATE_UNAVAILABLE = 0;
    static const Int32 DEVICE_STATE_AVAILABLE = 1;

    // phone state
    static const Int32 PHONE_STATE_OFFCALL = 0;
    static const Int32 PHONE_STATE_RINGING = 1;
    static const Int32 PHONE_STATE_INCALL = 2;

    // config for setForceUse
    static const Int32 FORCE_NONE = 0;
    static const Int32 FORCE_SPEAKER = 1;
    static const Int32 FORCE_HEADPHONES = 2;
    static const Int32 FORCE_BT_SCO = 3;
    static const Int32 FORCE_BT_A2DP = 4;
    static const Int32 FORCE_WIRED_ACCESSORY = 5;
    static const Int32 FORCE_BT_CAR_DOCK = 6;
    static const Int32 FORCE_BT_DESK_DOCK = 7;
    static const Int32 FORCE_DEFAULT = FORCE_NONE;

    // usage for serForceUse
    static const Int32 FOR_COMMUNICATION = 0;
    static const Int32 FOR_MEDIA = 1;
    static const Int32 FOR_RECORD = 2;
    static const Int32 FOR_DOCK = 3;

private:
    // Expose only the getter method publicly so we can change it in the future
    static const Int32 NUM_STREAM_TYPES = 10;

    /*
    private const static String TAG = "audio";

    private void log(String msg) {
        Log.d(TAG, "[AudioSystem] " + msg);
    }
    */

    static AutoPtr<IAudioSystemErrorCallback> mErrorCallback;
};

#endif //__ELAUDIOSYSTEM_H__
