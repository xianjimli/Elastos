
#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "media/AudioSystem.h"
#include <elastos/Math.h>
#include <elastos/HashMap.h>

using namespace Elastos::Core;

/**
 * AudioManager provides access to volume and ringer mode control.
 * <p>
 * Use <code>Context.getSystemService(Context.AUDIO_SERVICE)</code> to get
 * an instance of this class.
 */
class AudioManager 
{
public:
    AudioManager();

    /**
     * @hide
     */
    AudioManager(
        /* [in] */ IContext* context);

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
    virtual CARAPI AdjustStreamVolume(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 direction, 
        /* [in] */ Int32 flags);

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
    virtual CARAPI AdjustVolume(
        /* [in] */ Int32 direction, 
        /* [in] */ Int32 flags);

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
    virtual CARAPI AdjustSuggestedStreamVolume(
        /* [in] */ Int32 direction, 
        /* [in] */ Int32 suggestedStreamType, 
        /* [in] */ Int32 flags);

    /**
     * Returns the current ringtone mode.
     *
     * @return The current ringtone mode, one of {@link #RINGER_MODE_NORMAL},
     *         {@link #RINGER_MODE_SILENT}, or {@link #RINGER_MODE_VIBRATE}.
     * @see #setRingerMode(Int32)
     */
    virtual CARAPI_(Int32) GetRingerMode();

    /**
     * Returns the maximum volume index for a particular stream.
     *
     * @param streamType The stream type whose maximum volume index is returned.
     * @return The maximum valid volume index for the stream.
     * @see #getStreamVolume(Int32)
     */
    virtual CARAPI_(Int32) GetStreamMaxVolume(
        /* [in] */ Int32 streamType);

    /**
     * Returns the current volume index for a particular stream.
     *
     * @param streamType The stream type whose volume index is returned.
     * @return The current volume index for the stream.
     * @see #getStreamMaxVolume(Int32)
     * @see #setStreamVolume(Int32, Int32, Int32)
     */
    virtual CARAPI_(Int32) GetStreamVolume(
        /* [in] */ Int32 streamType);

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
    virtual CARAPI SetRingerMode(
        /* [in] */ Int32 ringerMode);

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
    virtual CARAPI SetStreamVolume(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 index, 
        /* [in] */ Int32 flags);

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
    virtual CARAPI SetStreamSolo(
        /* [in] */ Int32 streamType, 
        /* [in] */ Boolean state);

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
    virtual CARAPI SetStreamMute(
        /* [in] */ Int32 streamType, 
        /* [in] */ Boolean state);

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
    virtual CARAPI_(Boolean) ShouldVibrate(
        /* [in] */ Int32 vibrateType);

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
    virtual CARAPI_(Int32) GetVibrateSetting(
        /* [in] */ Int32 vibrateType);

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
    virtual CARAPI SetVibrateSetting(
        /* [in] */ Int32 vibrateType, 
        /* [in] */ Int32 vibrateSetting);

    /**
     * Sets the speakerphone on or off.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param on set <var>TRUE</var> to turn on speakerphone;
     *           <var>FALSE</var> to turn it off
     */
    virtual CARAPI SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether the speakerphone is on or off.
     *
     * @return TRUE if speakerphone is on, FALSE if it's off
     */
    virtual CARAPI_(Boolean) IsSpeakerphoneOn();

    //====================================================================
    // Bluetooth SCO control
    /**
     * Sticky broadcast intent action indicating that the bluetoooth SCO audio
     * connection state has changed. The intent contains on extra {@link #EXTRA_SCO_AUDIO_STATE}
     * indicating the new state which is either {@link #SCO_AUDIO_STATE_DISCONNECTED}
     * or {@link #SCO_AUDIO_STATE_CONNECTED}
     *
     * @see #startBluetoothSco()
     */
    static const String ACTION_SCO_AUDIO_STATE_CHANGED/* =
            "android.media.SCO_AUDIO_STATE_CHANGED"*/;
    /**
     * Extra for intent {@link #ACTION_SCO_AUDIO_STATE_CHANGED} containing the new
     * bluetooth SCO connection state.
     */
    static const String EXTRA_SCO_AUDIO_STATE/* =
            "android.media.extra.SCO_AUDIO_STATE"*/;

    /**
     * Value for extra {@link #EXTRA_SCO_AUDIO_STATE} indicating that the
     * SCO audio channel is not established
     */
    static const Int32 SCO_AUDIO_STATE_DISCONNECTED = 0;

    /**
     * Value for extra {@link #EXTRA_SCO_AUDIO_STATE} indicating that the
     * SCO audio channel is established
     */
    static const Int32 SCO_AUDIO_STATE_CONNECTED = 1;

    /**
     * Value for extra {@link #EXTRA_SCO_AUDIO_STATE} indicating that
     * there was an error trying to obtain the state
     */
    static const Int32 SCO_AUDIO_STATE_ERROR = -1;


    /**
     * Indicates if current platform supports use of SCO for off call use cases.
     * Application wanted to use bluetooth SCO audio when the phone is not in call
     * must first call thsi method to make sure that the platform supports this
     * feature.
     * @return TRUE if bluetooth SCO can be used for audio when not in call
     *         FALSE otherwise
     * @see #startBluetoothSco()
     */
    virtual CARAPI_(Boolean) IsBluetoothScoAvailableOffCall();

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
    virtual CARAPI StartBluetoothSco();

    /**
     * Stop bluetooth SCO audio connection.
     * <p>Requires Permission:
     *   {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}.
     * <p>This method must be called by applications having requested the use of
     * bluetooth SCO audio with {@link #startBluetoothSco()}
     * when finished with the SCO connection or if the establishment times out.
     * @see #startBluetoothSco()
     */
    virtual CARAPI StopBluetoothSco();

    /**
     * Request use of Bluetooth SCO headset for communications.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param on set <var>TRUE</var> to use bluetooth SCO for communications;
     *               <var>FALSE</var> to not use bluetooth SCO for communications
     */
    virtual CARAPI SetBluetoothScoOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether communications use Bluetooth SCO.
     *
     * @return TRUE if SCO is used for communications;
     *         FALSE if otherwise
     */
    virtual CARAPI_(Boolean) IsBluetoothScoOn();

    /**
     * @param on set <var>TRUE</var> to route A2DP audio to/from Bluetooth
     *           headset; <var>FALSE</var> disable A2DP audio
     * @deprecated Do not use.
     */
    virtual CARAPI SetBluetoothA2dpOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether A2DP audio routing to the Bluetooth headset is on or off.
     *
     * @return TRUE if A2DP audio is being routed to/from Bluetooth headset;
     *         FALSE if otherwise
     */
    virtual CARAPI_(Boolean) IsBluetoothA2dpOn();

    /**
     * Sets audio routing to the wired headset on or off.
     *
     * @param on set <var>TRUE</var> to route audio to/from wired
     *           headset; <var>FALSE</var> disable wired headset audio
     * @deprecated Do not use.
     */
    virtual CARAPI SetWiredHeadsetOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether audio routing to the wired headset is on or off.
     *
     * @return TRUE if audio is being routed to/from wired headset;
     *         FALSE if otherwise
     */
    virtual CARAPI_(Boolean) IsWiredHeadsetOn();

    /**
     * Sets the microphone mute on or off.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param on set <var>TRUE</var> to mute the microphone;
     *           <var>FALSE</var> to turn mute off
     */
    virtual CARAPI SetMicrophoneMute(
        /* [in] */ Boolean on);

    /**
     * Checks whether the microphone mute is on or off.
     *
     * @return TRUE if microphone is muted, FALSE if it's not
     */
    virtual CARAPI_(Boolean) IsMicrophoneMute();

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
    virtual CARAPI SetMode(
        /* [in] */ Int32 mode);

    /**
     * Returns the current audio mode.
     *
     * @return      the current audio mode (NORMAL, RINGTONE, or IN_CALL).
     *              Returns the current current audio state from the HAL.
     */
    virtual CARAPI_(Int32) GetMode();

    /* modes for setMode/getMode/setRoute/getRoute */
    /**
     * Audio harware modes.
     */
    /**
     * Invalid audio mode.
     */
    static const Int32 MODE_INVALID            = AudioSystem::MODE_INVALID;

    /**
     * Current audio mode. Used to apply audio routing to current mode.
     */
    static const Int32 MODE_CURRENT            = AudioSystem::MODE_CURRENT;

    /**
     * Normal audio mode: not ringing and no call established.
     */
    static const Int32 MODE_NORMAL             = AudioSystem::MODE_NORMAL;

    /**
     * Ringing audio mode. An incoming is being signaled.
     */
    static const Int32 MODE_RINGTONE           = AudioSystem::MODE_RINGTONE;

    /**
     * In call audio mode. A telephony call is established.
     */
    static const Int32 MODE_IN_CALL            = AudioSystem::MODE_IN_CALL;

    /**
     * @hide
     * In communication audio mode. An audio/video chat or VoIP call is established.
     */
    static const Int32 MODE_IN_COMMUNICATION   = AudioSystem::MODE_IN_COMMUNICATION;

    /* Routing bits for setRouting/getRouting API */
    /**
     * Routing audio output to earpiece
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    static const Int32 ROUTE_EARPIECE          = AudioSystem::ROUTE_EARPIECE;

    /**
     * Routing audio output to speaker
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    static const Int32 ROUTE_SPEAKER           = AudioSystem::ROUTE_SPEAKER;

    /**
     * @deprecated use {@link #ROUTE_BLUETOOTH_SCO}
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    static const Int32 ROUTE_BLUETOOTH = AudioSystem::ROUTE_BLUETOOTH_SCO;

    /**
     * Routing audio output to bluetooth SCO
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    static const Int32 ROUTE_BLUETOOTH_SCO     = AudioSystem::ROUTE_BLUETOOTH_SCO;

    /**
     * Routing audio output to headset
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    static const Int32 ROUTE_HEADSET           = AudioSystem::ROUTE_HEADSET;

    /**
     * Routing audio output to bluetooth A2DP
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    static const Int32 ROUTE_BLUETOOTH_A2DP    = AudioSystem::ROUTE_BLUETOOTH_A2DP;

    /**
     * Used for mask parameter of {@link #setRouting(Int32,Int32,Int32)}.
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    static const Int32 ROUTE_ALL               = AudioSystem::ROUTE_ALL;

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
    virtual CARAPI SetRouting(
        /* [in] */ Int32 mode, 
        /* [in] */ Int32 routes, 
        /* [in] */ Int32 mask);

    /**
     * Returns the current audio routing bit vector for a specified mode.
     *
     * @param mode audio mode to get route (e.g., MODE_RINGTONE)
     * @return an audio route bit vector that can be compared with ROUTE_xxx
     * bits
     * @deprecated   Do not query audio routing directly, use isSpeakerphoneOn(),
     * isBluetoothScoOn(), isBluetoothA2dpOn() and isWiredHeadsetOn() methods instead.
     */
    virtual CARAPI_(Int32) GetRouting(
        /* [in] */ Int32 mode);

    /**
     * Checks whether any music is active.
     *
     * @return TRUE if any music tracks are active.
     */
    virtual CARAPI_(Boolean) IsMusicActive();

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
    virtual CARAPI SetParameter(
        /* [in] */ String key, 
        /* [in] */ String value);

    /**
     * Sets a variable number of parameter values to audio hardware.
     *
     * @param keyValuePairs list of parameters key value pairs in the form:
     *    key1=value1;key2=value2;...
     *
     */
    virtual CARAPI SetParameters(
        /* [in] */ String keyValuePairs);

    /**
     * Sets a varaible number of parameter values to audio hardware.
     *
     * @param keys list of parameters
     * @return list of parameters key value pairs in the form:
     *    key1=value1;key2=value2;...
     */
    virtual CARAPI_(String) GetParameters(
        /* [in] */ String keys);

    /* Sound effect identifiers */
    /**
     * Keyboard and direction pad click sound
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_KEY_CLICK = 0;

    /**
     * Focus has moved up
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_FOCUS_NAVIGATION_UP = 1;

    /**
     * Focus has moved down
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_FOCUS_NAVIGATION_DOWN = 2;

    /**
     * Focus has moved left
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_FOCUS_NAVIGATION_LEFT = 3;

    /**
     * Focus has moved right
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_FOCUS_NAVIGATION_RIGHT = 4;

    /**
     * IME standard keypress sound
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_KEYPRESS_STANDARD = 5;

    /**
     * IME spacebar keypress sound
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_KEYPRESS_SPACEBAR = 6;

    /**
     * IME delete keypress sound
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_KEYPRESS_DELETE = 7;

    /**
     * IME return_keypress sound
     * @see #playSoundEffect(Int32)
     */
    static const Int32 FX_KEYPRESS_RETURN = 8;

    /**
     * @hide Number of sound effects
     */
    static const Int32 NUM_SOUND_EFFECTS = 9;

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
    virtual CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType);

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
    virtual CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType, 
        /* [in] */ Float volume);


    /**
     *  Load Sound effects.
     *  This method must be called when sound effects are enabled.
     */
    virtual CARAPI LoadSoundEffects();

    /**
     *  Unload Sound effects.
     *  This method can be called to free some memory when
     *  sound effects are disabled.
     */
    virtual CARAPI UnloadSoundEffects();

    /**
     * Used to indicate a gain of audio focus, or a request of audio focus, of unknown duration.
     * @see OnAudioFocusChangeListener#onAudioFocusChange(Int32)
     * @see #requestAudioFocus(OnAudioFocusChangeListener, Int32, Int32)
     */
    static const Int32 AUDIOFOCUS_GAIN = 1;
    
    /**
     * Used to indicate a temporary gain or request of audio focus, anticipated to last a short
     * amount of time. Examples of temporary changes are the playback of driving directions, or an
     * event notification.
     * @see OnAudioFocusChangeListener#onAudioFocusChange(Int32)
     * @see #requestAudioFocus(OnAudioFocusChangeListener, Int32, Int32)
     */
    static const Int32 AUDIOFOCUS_GAIN_TRANSIENT = 2;
    
    /**
     * Used to indicate a temporary request of audio focus, anticipated to last a short
     * amount of time, and where it is acceptable for other audio applications to keep playing
     * after having lowered their output level (also referred to as "ducking").
     * Examples of temporary changes are the playback of driving directions where playback of music
     * in the background is acceptable.
     * @see OnAudioFocusChangeListener#onAudioFocusChange(Int32)
     * @see #requestAudioFocus(OnAudioFocusChangeListener, Int32, Int32)
     */
    static const Int32 AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK = 3;
    
    /**
     * Used to indicate a loss of audio focus of unknown duration.
     * @see OnAudioFocusChangeListener#onAudioFocusChange(Int32)
     */
    static const Int32 AUDIOFOCUS_LOSS = -1 * AUDIOFOCUS_GAIN;
    
    /**
     * Used to indicate a transient loss of audio focus.
     * @see OnAudioFocusChangeListener#onAudioFocusChange(Int32)
     */
    static const Int32 AUDIOFOCUS_LOSS_TRANSIENT = -1 * AUDIOFOCUS_GAIN_TRANSIENT;
    
    /**
     * Used to indicate a transient loss of audio focus where the loser of the audio focus can
     * lower its output volume if it wants to continue playing (also referred to as "ducking"), as
     * the new focus owner doesn't require others to be silent.
     * @see OnAudioFocusChangeListener#onAudioFocusChange(Int32)
     */
    static const Int32 AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK =
            -1 * AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK;

private:
    /**
     * Map to convert focus event listener IDs, as used in the AudioService audio focus stack,
     * to actual listener objects.
     */
    HashMap<String, AutoPtr<IOnAudioFocusChangeListener> > mAudioFocusIdListenerMap;/* =
            new HashMap<String, OnAudioFocusChangeListener>();*/
    /**
     * Lock to prevent concurrent changes to the list of focus listeners for this AudioManager
     * instance.
     */
    AutoPtr<IInterface> mFocusListenerLock;// = new Object();

    AutoPtr<IOnAudioFocusChangeListener> FindFocusListener(
        /* [in] */ String id);

    /**
     * Helper class to handle the forwarding of audio focus events to the appropriate listener
     */
    class FocusEventHandlerDelegate 
    {
    public:
        FocusEventHandlerDelegate();

        AutoPtr<IHandler> GetHandler();

    private:
        AutoPtr<IHandler> mHandler;
    };

    /**
     * Handler for audio focus events coming from the audio service.
     */
    FocusEventHandlerDelegate* mAudioFocusEventHandlerDelegate;/* =
            new FocusEventHandlerDelegate();*/


    AutoPtr<IAudioFocusDispatcher> mAudioFocusDispatcher;/* = new IAudioFocusDispatcher.Stub() {

        public void dispatchAudioFocusChange(Int32 focusChange, String id) {
            Message m = mAudioFocusEventHandlerDelegate.getHandler().obtainMessage(focusChange, id);
            mAudioFocusEventHandlerDelegate.getHandler().sendMessage(m);
        }

    };*/

    CARAPI_(String) GetIdForAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener* l);

public:
    /**
     * @hide
     * Registers a listener to be called when audio focus changes. Calling this method is optional
     * before calling {@link #requestAudioFocus(OnAudioFocusChangeListener, Int32, Int32)}, as it
     * will register the listener as well if it wasn't registered already.
     * @param l the listener to be notified of audio focus changes.
     */
    virtual CARAPI RegisterAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener* l);

    /**
     * @hide
     * Causes the specified listener to not be called anymore when focus is gained or lost.
     * @param l the listener to unregister.
     */
    virtual CARAPI UnregisterAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener* l);

    /**
     * A failed focus change request.
     */
    static const Int32 AUDIOFOCUS_REQUEST_FAILED = 0;

    /**
     * A successful focus change request.
     */
    static const Int32 AUDIOFOCUS_REQUEST_GRANTED = 1;


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
    virtual CARAPI_(Int32) RequestAudioFocus(
        /* [in] */ IOnAudioFocusChangeListener* l, 
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 durationHint);


    /**
     *  Abandon audio focus. Causes the previous focus owner, if any, to receive focus.
     *  @param l the listener with which focus was requested.
     *  @return {@link #AUDIOFOCUS_REQUEST_FAILED} or {@link #AUDIOFOCUS_REQUEST_GRANTED}
     */
    virtual CARAPI_(Int32) AbandonAudioFocus(
        /* [in] */ IOnAudioFocusChangeListener* l);


    //====================================================================
    // Remote Control
    /**
     * Register a component to be the sole receiver of MEDIA_BUTTON intents.
     * @param eventReceiver identifier of a {@link android.content.BroadcastReceiver}
     *      that will receive the media button intent. This broadcast receiver must be declared
     *      in the application manifest.
     */
    virtual CARAPI RegisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

    /**
     * Unregister the receiver of MEDIA_BUTTON intents.
     * @param eventReceiver identifier of a {@link android.content.BroadcastReceiver}
     *      that was registered with {@link #registerMediaButtonEventReceiver(ComponentName)}.
     */
    virtual CARAPI UnregisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

    /**
     *  @hide
     *  Reload audio settings. This method is called by Settings backup
     *  agent when audio settings are restored and causes the AudioService
     *  to read and apply restored settings.
     */
    virtual CARAPI ReloadAudioSettings();
    
private:
    static CARAPI_(AutoPtr<IAudioService>) GetService();

    /**
     * Settings has an in memory cache, so this is fast.
     */
    CARAPI_(Boolean) QuerySoundEffectsEnabled();

    /**
     * {@hide}
     */
    AutoPtr<IBinder> mICallBack;/// = new Binder();

    AutoPtr<IContext> mContext;
    AutoPtr<IHandler> mHandler;

    static String TAG;// = "AudioManager";
    static Boolean DEBUG;// = FALSE;
    static Boolean localLOGV;// = DEBUG;// || android.util.Config.LOGV;

    static AutoPtr<IAudioService> sService;

public:
    /**
     * Broadcast intent, a hint for applications that audio is about to become
     * 'noisy' due to a change in audio outputs. For example, this intent may
     * be sent when a wired headset is unplugged, or when an A2DP audio
     * sink is disconnected, and the audio system is about to automatically
     * switch audio route to the speaker. Applications that are controlling
     * audio streams may consider pausing, reducing volume or some other action
     * on receipt of this intent so as not to surprise the user with audio
     * from the speaker.
     */
    static const String ACTION_AUDIO_BECOMING_NOISY/* = "android.media.AUDIO_BECOMING_NOISY"*/;

    /**
     * Sticky broadcast intent action indicating that the ringer mode has
     * changed. Includes the new ringer mode.
     *
     * @see #EXTRA_RINGER_MODE
     */
    static const String RINGER_MODE_CHANGED_ACTION/* = "android.media.RINGER_MODE_CHANGED"*/;

    /**
     * The new ringer mode.
     *
     * @see #RINGER_MODE_CHANGED_ACTION
     * @see #RINGER_MODE_NORMAL
     * @see #RINGER_MODE_SILENT
     * @see #RINGER_MODE_VIBRATE
     */
    static const String EXTRA_RINGER_MODE/* = "android.media.EXTRA_RINGER_MODE"*/;

    /**
     * Broadcast intent action indicating that the vibrate setting has
     * changed. Includes the vibrate type and its new setting.
     *
     * @see #EXTRA_VIBRATE_TYPE
     * @see #EXTRA_VIBRATE_SETTING
     */
    static const String VIBRATE_SETTING_CHANGED_ACTION/* = "android.media.VIBRATE_SETTING_CHANGED"*/;

    /**
     * @hide Broadcast intent when the volume for a particular stream type changes.
     * Includes the stream, the new volume and previous volumes
     *
     * @see #EXTRA_VOLUME_STREAM_TYPE
     * @see #EXTRA_VOLUME_STREAM_VALUE
     * @see #EXTRA_PREV_VOLUME_STREAM_VALUE
     */
    static const String VOLUME_CHANGED_ACTION/* = "android.media.VOLUME_CHANGED_ACTION"*/;

    /**
     * The new vibrate setting for a particular type.
     *
     * @see #VIBRATE_SETTING_CHANGED_ACTION
     * @see #EXTRA_VIBRATE_TYPE
     * @see #VIBRATE_SETTING_ON
     * @see #VIBRATE_SETTING_OFF
     * @see #VIBRATE_SETTING_ONLY_SILENT
     */
    static const String EXTRA_VIBRATE_SETTING/* = "android.media.EXTRA_VIBRATE_SETTING"*/;

    /**
     * The vibrate type whose setting has changed.
     *
     * @see #VIBRATE_SETTING_CHANGED_ACTION
     * @see #VIBRATE_TYPE_NOTIFICATION
     * @see #VIBRATE_TYPE_RINGER
     */
    static const String EXTRA_VIBRATE_TYPE/* = "android.media.EXTRA_VIBRATE_TYPE"*/;

    /**
     * @hide The stream type for the volume changed intent.
     */
    static const String EXTRA_VOLUME_STREAM_TYPE/* = "android.media.EXTRA_VOLUME_STREAM_TYPE"*/;

    /**
     * @hide The volume associated with the stream for the volume changed intent.
     */
    static const String EXTRA_VOLUME_STREAM_VALUE/* =
        "android.media.EXTRA_VOLUME_STREAM_VALUE"*/;

    /**
     * @hide The previous volume associated with the stream for the volume changed intent.
     */
    static const String EXTRA_PREV_VOLUME_STREAM_VALUE/* =
        "android.media.EXTRA_PREV_VOLUME_STREAM_VALUE"*/;

    /** The audio stream for phone calls */
    static const Int32 STREAM_VOICE_CALL = AudioSystem::STREAM_VOICE_CALL;

    /** The audio stream for system sounds */
    static const Int32 STREAM_SYSTEM = AudioSystem::STREAM_SYSTEM;

    /** The audio stream for the phone ring */
    static const Int32 STREAM_RING = AudioSystem::STREAM_RING;

    /** The audio stream for music playback */
    static const Int32 STREAM_MUSIC = AudioSystem::STREAM_MUSIC;

    /** The audio stream for alarms */
    static const Int32 STREAM_ALARM = AudioSystem::STREAM_ALARM;

    /** The audio stream for notifications */
    static const Int32 STREAM_NOTIFICATION = AudioSystem::STREAM_NOTIFICATION;

    /** @hide The audio stream for phone calls when connected to bluetooth */
    static const Int32 STREAM_BLUETOOTH_SCO = AudioSystem::STREAM_BLUETOOTH_SCO;

    /** @hide The audio stream for enforced system sounds in certain countries (e.g camera in Japan) */
    static const Int32 STREAM_SYSTEM_ENFORCED = AudioSystem::STREAM_SYSTEM_ENFORCED;

    /** The audio stream for DTMF Tones */
    static const Int32 STREAM_DTMF = AudioSystem::STREAM_DTMF;

    /** @hide The audio stream for text to speech (TTS) */
    static const Int32 STREAM_TTS = AudioSystem::STREAM_TTS;
    
    /** Number of audio streams */
    /**
     * @deprecated Use AudioSystem::getNumStreamTypes() instead
     */
    static const Int32 NUM_STREAMS = AudioSystem::NUM_STREAMS;


    /**  @hide Default volume index values for audio streams */
    static const Int32 DEFAULT_STREAM_VOLUME[];
    //=  {
    //    4,  // STREAM_VOICE_CALL
    //    7,  // STREAM_SYSTEM
    //    5,  // STREAM_RING
    //    11, // STREAM_MUSIC
    //    6,  // STREAM_ALARM
    //    5,  // STREAM_NOTIFICATION
    //    7,  // STREAM_BLUETOOTH_SCO
    //    7,  // STREAM_SYSTEM_ENFORCED
    //    11, // STREAM_DTMF
    //    11  // STREAM_TTS
    //};

    /**
     * Increase the ringer volume.
     *
     * @see #adjustVolume(Int32, Int32)
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     */
    static const Int32 ADJUST_RAISE = 1;

    /**
     * Decrease the ringer volume.
     *
     * @see #adjustVolume(Int32, Int32)
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     */
    static const Int32 ADJUST_LOWER = -1;

    /**
     * Maintain the previous ringer volume. This may be useful when needing to
     * show the volume toast without actually modifying the volume.
     *
     * @see #adjustVolume(Int32, Int32)
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     */
    static const Int32 ADJUST_SAME = 0;

    // Flags should be powers of 2!

    /**
     * Show a toast containing the current volume.
     *
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     * @see #adjustVolume(Int32, Int32)
     * @see #setStreamVolume(Int32, Int32, Int32)
     * @see #setRingerMode(Int32)
     */
    static const Int32 FLAG_SHOW_UI = 1 << 0;

    /**
     * Whether to include ringer modes as possible options when changing volume.
     * For example, if TRUE and volume level is 0 and the volume is adjusted
     * with {@link #ADJUST_LOWER}, then the ringer mode may switch the silent or
     * vibrate mode.
     * <p>
     * By default this is on for the ring stream. If this flag is included,
     * this behavior will be present regardless of the stream type being
     * affected by the ringer mode.
     *
     * @see #adjustVolume(Int32, Int32)
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     */
    static const Int32 FLAG_ALLOW_RINGER_MODES = 1 << 1;

    /**
     * Whether to play a sound when changing the volume.
     * <p>
     * If this is given to {@link #adjustVolume(Int32, Int32)} or
     * {@link #adjustSuggestedStreamVolume(Int32, Int32, Int32)}, it may be ignored
     * in some cases (for example, the decided stream type is not
     * {@link AudioManager#STREAM_RING}, or the volume is being adjusted
     * downward).
     *
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     * @see #adjustVolume(Int32, Int32)
     * @see #setStreamVolume(Int32, Int32, Int32)
     */
    static const Int32 FLAG_PLAY_SOUND = 1 << 2;

    /**
     * Removes any sounds/vibrate that may be in the queue, or are playing (related to
     * changing volume).
     */
    static const Int32 FLAG_REMOVE_SOUND_AND_VIBRATE = 1 << 3;

    /**
     * Whether to vibrate if going into the vibrate ringer mode.
     */
    static const Int32 FLAG_VIBRATE = 1 << 4;

    /**
     * Ringer mode that will be silent and will not vibrate. (This overrides the
     * vibrate setting.)
     *
     * @see #setRingerMode(Int32)
     * @see #getRingerMode()
     */
    static const Int32 RINGER_MODE_SILENT = 0;

    /**
     * Ringer mode that will be silent and will vibrate. (This will cause the
     * phone ringer to always vibrate, but the notification vibrate to only
     * vibrate if set.)
     *
     * @see #setRingerMode(Int32)
     * @see #getRingerMode()
     */
    static const Int32 RINGER_MODE_VIBRATE = 1;

    /**
     * Ringer mode that may be audible and may vibrate. It will be audible if
     * the volume before changing out of this mode was audible. It will vibrate
     * if the vibrate setting is on.
     *
     * @see #setRingerMode(Int32)
     * @see #getRingerMode()
     */
    static const Int32 RINGER_MODE_NORMAL = 2;

    /**
     * Vibrate type that corresponds to the ringer.
     *
     * @see #setVibrateSetting(Int32, Int32)
     * @see #getVibrateSetting(Int32)
     * @see #shouldVibrate(Int32)
     */
    static const Int32 VIBRATE_TYPE_RINGER = 0;

    /**
     * Vibrate type that corresponds to notifications.
     *
     * @see #setVibrateSetting(Int32, Int32)
     * @see #getVibrateSetting(Int32)
     * @see #shouldVibrate(Int32)
     */
    static const Int32 VIBRATE_TYPE_NOTIFICATION = 1;

    /**
     * Vibrate setting that suggests to never vibrate.
     *
     * @see #setVibrateSetting(Int32, Int32)
     * @see #getVibrateSetting(Int32)
     */
    static const Int32 VIBRATE_SETTING_OFF = 0;

    /**
     * Vibrate setting that suggests to vibrate when possible.
     *
     * @see #setVibrateSetting(Int32, Int32)
     * @see #getVibrateSetting(Int32)
     */
    static const Int32 VIBRATE_SETTING_ON = 1;

    /**
     * Vibrate setting that suggests to only vibrate when in the vibrate ringer
     * mode.
     *
     * @see #setVibrateSetting(Int32, Int32)
     * @see #getVibrateSetting(Int32)
     */
    static const Int32 VIBRATE_SETTING_ONLY_SILENT = 2;

    /**
     * Suggests using the default stream type. This may not be used in all
     * places a stream type is needed.
     */
    static const Int32 USE_DEFAULT_STREAM_TYPE = 0x80000000;//Math::INT32_MIN_VALUE;
};

#endif
