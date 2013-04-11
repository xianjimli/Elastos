
#ifndef __AUDIOTRACK_H__
#define __AUDIOTRACK_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * The AudioTrack class manages and plays a single audio resource for Java applications.
 * It allows to stream PCM audio buffers to the audio hardware for playback. This is
 * achieved by "pushing" the data to the AudioTrack object using one of the
 *  {@link #write(byte[], Int32, Int32)} and {@link #write(short[], Int32, Int32)} methods.
 *  
 * <p>An AudioTrack instance can operate under two modes: static or streaming.<br>
 * In Streaming mode, the application writes a continuous stream of data to the AudioTrack, using
 * one of the write() methods. These are blocking and return when the data has been transferred
 * from the Java layer to the native layer and queued for playback. The streaming mode
 *  is most useful when playing blocks of audio data that for instance are:
 * <ul>
 *   <li>too big to fit in memory because of the duration of the sound to play,</li>
 *   <li>too big to fit in memory because of the characteristics of the audio data
 *         (high sampling rate, bits per sample ...)</li>
 *   <li>received or generated while previously queued audio is playing.</li>
 * </ul>
 * The static mode is to be chosen when dealing with short sounds that fit in memory and
 * that need to be played with the smallest latency possible. AudioTrack instances in static mode
 * can play the sound without the need to transfer the audio data from Java to native layer
 * each time the sound is to be played. The static mode will therefore be preferred for UI and
 * game sounds that are played often, and with the smallest overhead possible.
 * 
 * <p>Upon creation, an AudioTrack object initializes its associated audio buffer.
 * The size of this buffer, specified during the construction, determines how long an AudioTrack
 * can play before running out of data.<br>
 * For an AudioTrack using the static mode, this size is the maximum size of the sound that can
 * be played from it.<br>
 * For the streaming mode, data will be written to the hardware in chunks of
 * sizes inferior to the total buffer size.
 */
class AudioTrack
{
public:
    AudioTrack();

    //--------------------------------------------------------------------------
    // Constructor, Finalize
    //--------------------
    /**
     * Class constructor.
     * @param streamType the type of the audio stream. See
     *   {@link AudioManager#STREAM_VOICE_CALL}, {@link AudioManager#STREAM_SYSTEM},
     *   {@link AudioManager#STREAM_RING}, {@link AudioManager#STREAM_MUSIC} and
     *   {@link AudioManager#STREAM_ALARM}
     * @param sampleRateInHz the sample rate expressed in Hertz. Examples of rates are (but
     *   not limited to) 44100, 22050 and 11025.
     * @param channelConfig describes the configuration of the audio channels.
     *   See {@link AudioFormat#CHANNEL_OUT_MONO} and
     *   {@link AudioFormat#CHANNEL_OUT_STEREO}
     * @param audioFormat the format in which the audio data is represented.
     *   See {@link AudioFormat#ENCODING_PCM_16BIT} and
     *   {@link AudioFormat#ENCODING_PCM_8BIT}
     * @param bufferSizeInBytes the total size (in bytes) of the buffer where audio data is read
     *   from for playback. If using the AudioTrack in streaming mode, you can write data into
     *   this buffer in smaller chunks than this size. If using the AudioTrack in static mode,
     *   this is the maximum size of the sound that will be played for this instance.
     *   See {@link #getMinBufferSize(Int32, Int32, Int32)} to determine the minimum required buffer size
     *   for the successful creation of an AudioTrack instance in streaming mode. Using values
     *   smaller than getMinBufferSize() will result in an initialization failure.
     * @param mode streaming or static buffer. See {@link #MODE_STATIC} and {@link #MODE_STREAM}
     * @throws java.lang.IllegalArgumentException
     */
    AudioTrack(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes, 
        /* [in] */ Int32 mode);

    /**
     * Class constructor with audio session. Use this constructor when the AudioTrack must be
     * attached to a particular audio session. The primary use of the audio session ID is to
     * associate audio effects to a particular instance of AudioTrack: if an audio session ID
     * is provided when creating an AudioEffect, this effect will be applied only to audio tracks
     * and media players in the same session and not to the output mix.
     * When an AudioTrack is created without specifying a session, it will create its own session
     * which can be retreived by calling the {@link #getAudioSessionId()} method.
     * If a session ID is provided, this AudioTrack will share effects attached to this session
     * with all other media players or audio tracks in the same session.
     * @param streamType the type of the audio stream. See
     *   {@link AudioManager#STREAM_VOICE_CALL}, {@link AudioManager#STREAM_SYSTEM},
     *   {@link AudioManager#STREAM_RING}, {@link AudioManager#STREAM_MUSIC} and
     *   {@link AudioManager#STREAM_ALARM}
     * @param sampleRateInHz the sample rate expressed in Hertz. Examples of rates are (but
     *   not limited to) 44100, 22050 and 11025.
     * @param channelConfig describes the configuration of the audio channels.
     *   See {@link AudioFormat#CHANNEL_OUT_MONO} and
     *   {@link AudioFormat#CHANNEL_OUT_STEREO}
     * @param audioFormat the format in which the audio data is represented.
     *   See {@link AudioFormat#ENCODING_PCM_16BIT} and
     *   {@link AudioFormat#ENCODING_PCM_8BIT}
     * @param bufferSizeInBytes the total size (in bytes) of the buffer where audio data is read
     *   from for playback. If using the AudioTrack in streaming mode, you can write data into
     *   this buffer in smaller chunks than this size. If using the AudioTrack in static mode,
     *   this is the maximum size of the sound that will be played for this instance.
     *   See {@link #getMinBufferSize(Int32, Int32, Int32)} to determine the minimum required buffer size
     *   for the successful creation of an AudioTrack instance in streaming mode. Using values
     *   smaller than getMinBufferSize() will result in an initialization failure.
     * @param mode streaming or static buffer. See {@link #MODE_STATIC} and {@link #MODE_STREAM}
     * @param sessionId Id of audio session the AudioTrack must be attached to
     * @throws java.lang.IllegalArgumentException
     */
    AudioTrack(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes, 
        /* [in] */ Int32 mode, 
        /* [in] */ Int32 sessionId);

    /**
     * Releases the native AudioTrack resources.
     */
    virtual CARAPI Release();

    //--------------------------------------------------------------------------
    // Getters
    //--------------------
    /**
     * Returns the minimum valid volume value. Volume values set under this one will
     * be clamped at this value.
     * @return the minimum volume expressed as a linear attenuation.
     */
    static CARAPI_(Float) GetMinVolume();

    /**
     * Returns the maximum valid volume value. Volume values set above this one will
     * be clamped at this value.
     * @return the maximum volume expressed as a linear attenuation.
     */
    static CARAPI_(Float) GetMaxVolume();

    /**
     * Returns the configured audio data sample rate in Hz
     */
    virtual CARAPI_(Int32) GetSampleRate();
    
    /**
     * Returns the current playback rate in Hz.
     */
    virtual CARAPI_(Int32) GetPlaybackRate();

    /**
     * Returns the configured audio data format. See {@link AudioFormat#ENCODING_PCM_16BIT}
     * and {@link AudioFormat#ENCODING_PCM_8BIT}.
     */
    virtual CARAPI_(Int32) GetAudioFormat();

    /**
     * Returns the type of audio stream this AudioTrack is configured for.
     * Compare the result against {@link AudioManager#STREAM_VOICE_CALL},
     * {@link AudioManager#STREAM_SYSTEM}, {@link AudioManager#STREAM_RING},
     * {@link AudioManager#STREAM_MUSIC} or {@link AudioManager#STREAM_ALARM}
     */
    virtual CARAPI_(Int32) GetStreamType();

    /**
     * Returns the configured channel configuration.

     * See {@link AudioFormat#CHANNEL_OUT_MONO}
     * and {@link AudioFormat#CHANNEL_OUT_STEREO}.
     */
    virtual CARAPI_(Int32) GetChannelConfiguration();

    /**
     * Returns the configured number of channels.
     */
    virtual CARAPI_(Int32) GetChannelCount();

    /**
     * Returns the state of the AudioTrack instance. This is useful after the
     * AudioTrack instance has been created to check if it was initialized
     * properly. This ensures that the appropriate hardware resources have been
     * acquired.
     * @see #STATE_INITIALIZED
     * @see #STATE_NO_STATIC_DATA
     * @see #STATE_UNINITIALIZED
     */
    virtual CARAPI_(Int32) GetState();

    /**
     * Returns the playback state of the AudioTrack instance.
     * @see #PLAYSTATE_STOPPED
     * @see #PLAYSTATE_PAUSED
     * @see #PLAYSTATE_PLAYING
     */
    virtual CARAPI_(Int32) GetPlayState();

    /**
     * Returns marker position expressed in frames.
     */
    virtual CARAPI_(Int32) GetNotificationMarkerPosition();

    /**
     * Returns the notification update period expressed in frames.
     */
    virtual CARAPI_(Int32) GetPositionNotificationPeriod();

    /**
     * Returns the playback head position expressed in frames
     */
    virtual CARAPI_(Int32) GetPlaybackHeadPosition();

    /**
     *  Returns the hardware output sample rate
     */
    static CARAPI_(Int32) GetNativeOutputSampleRate(
        /* [in] */ Int32 streamType);
    
    /**
     * Returns the minimum buffer size required for the successful creation of an AudioTrack
     * object to be created in the {@link #MODE_STREAM} mode. Note that this size doesn't
     * guarantee a smooth playback under load, and higher values should be chosen according to
     * the expected frequency at which the buffer will be refilled with additional data to play. 
     * @param sampleRateInHz the sample rate expressed in Hertz.
     * @param channelConfig describes the configuration of the audio channels. 
     *   See {@link AudioFormat#CHANNEL_OUT_MONO} and
     *   {@link AudioFormat#CHANNEL_OUT_STEREO}
     * @param audioFormat the format in which the audio data is represented. 
     *   See {@link AudioFormat#ENCODING_PCM_16BIT} and 
     *   {@link AudioFormat#ENCODING_PCM_8BIT}
     * @return {@link #ERROR_BAD_VALUE} if an invalid parameter was passed,
     *   or {@link #ERROR} if the implementation was unable to query the hardware for its output 
     *     properties, 
     *   or the minimum buffer size expressed in bytes.
     */
    static CARAPI_(Int32) GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat);

    /**
     * Returns the audio session ID.
     *
     * @return the ID of the audio session this AudioTrack belongs to.
     */
    virtual CARAPI_(Int32) GetAudioSessionId();

    //--------------------------------------------------------------------------
    // Initialization / configuration
    //--------------------
    /**
     * Sets the listener the AudioTrack notifies when a previously set marker is reached or
     * for each periodic playback head position update.
     * Notifications will be received in the same thread as the one in which the AudioTrack
     * instance was created.
     * @param listener
     */
    virtual CARAPI SetPlaybackPositionUpdateListener(
        /* [in] */ IOnPlaybackPositionUpdateListener* listener);
    
    /**
     * Sets the listener the AudioTrack notifies when a previously set marker is reached or
     * for each periodic playback head position update.
     * Use this method to receive AudioTrack events in the Handler associated with another
     * thread than the one in which you created the AudioTrack instance.
     * @param listener
     * @param handler the Handler that will receive the event notification messages.
     */
    virtual CARAPI SetPlaybackPositionUpdateListener(
        /* [in] */ IOnPlaybackPositionUpdateListener* listener, 
        /* [in] */ IHandler* handler);

     /**
     * Sets the specified left/right output volume values on the AudioTrack. Values are clamped
     * to the ({@link #getMinVolume()}, {@link #getMaxVolume()}) interval if outside this range.
     * @param leftVolume output attenuation for the left channel. A value of 0.0f is silence,
     *      a value of 1.0f is no attenuation.
     * @param rightVolume output attenuation for the right channel
     * @return error code or success, see {@link #SUCCESS},
     *    {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetStereoVolume(
        /* [in] */ Float leftVolume, 
        /* [in] */ Float rightVolume);

    /**
     * Sets the playback sample rate for this track. This sets the sampling rate at which
     * the audio data will be consumed and played back, not the original sampling rate of the
     * content. Setting it to half the sample rate of the content will cause the playback to
     * last twice as long, but will also result in a negative pitch shift.
     * The valid sample rate range if from 1Hz to twice the value returned by
     * {@link #getNativeOutputSampleRate(Int32)}.
     * @param sampleRateInHz the sample rate expressed in Hz
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
     *    {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz);

    /**
     * Sets the position of the notification marker.
     * @param markerInFrames marker in frames
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
     *  {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames);

    /**
     * Sets the period for the periodic notification event.
     * @param periodInFrames update period expressed in frames
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames);

    /**
     * Sets the playback head position. The track must be stopped for the position to be changed.
     * @param positionInFrames playback head position expressed in frames
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
     *    {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetPlaybackHeadPosition(
        /* [in] */ Int32 positionInFrames);

    /**
     * Sets the loop points and the loop count. The loop can be infinite.
     * @param startInFrames loop start marker expressed in frames
     * @param endInFrames loop end marker expressed in frames
     * @param loopCount the number of times the loop is looped.
     *    A value of -1 means infinite looping.
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
     *    {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetLoopPoints(
        /* [in] */ Int32 startInFrames, 
        /* [in] */ Int32 endInFrames, 
        /* [in] */ Int32 loopCount);

    //---------------------------------------------------------
    // Transport control methods
    //--------------------
    /**
     * Starts playing an AudioTrack.
     * @throws IllegalStateException
     */
    virtual CARAPI Play();

    /**
     * Stops playing the audio data.
     * @throws IllegalStateException
     */
    virtual CARAPI Stop();

    /**
     * Pauses the playback of the audio data.
     * @throws IllegalStateException
     */
    virtual CARAPI Pause();


    //---------------------------------------------------------
    // Audio data supply
    //--------------------

    /**
     * Flushes the audio data currently queued for playback.
     */

    virtual CARAPI Flush();

    /**
     * Writes the audio data to the audio hardware for playback.
     * @param audioData the array that holds the data to play.
     * @param offsetInBytes the offset expressed in bytes in audioData where the data to play 
     *    starts.
     * @param sizeInBytes the number of bytes to read in audioData after the offset.
     * @return the number of bytes that were written or {@link #ERROR_INVALID_OPERATION}
     *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
     *    the parameters don't resolve to valid data and indexes.
     */

    virtual CARAPI_(Int32) Write(
        /* [in] */ ArrayOf<Byte>* audioData,
        /* [in] */ Int32 offsetInBytes, 
        /* [in] */ Int32 sizeInBytes);


    /**
     * Writes the audio data to the audio hardware for playback.
     * @param audioData the array that holds the data to play.
     * @param offsetInShorts the offset expressed in shorts in audioData where the data to play
     *     starts.
     * @param sizeInShorts the number of bytes to read in audioData after the offset.
     * @return the number of shorts that were written or {@link #ERROR_INVALID_OPERATION}
      *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
      *    the parameters don't resolve to valid data and indexes.
     */

    virtual CARAPI_(Int32) Write(
        /* [in] */ ArrayOf<Int16>* audioData, 
        /* [in] */ Int32 offsetInShorts, 
        /* [in] */ Int32 sizeInShorts);


    /**
     * Notifies the native resource to reuse the audio data already loaded in the native
     * layer. This call is only valid with AudioTrack instances that don't use the streaming
     * model.
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
     *  {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) ReloadStaticData();

    //--------------------------------------------------------------------------
    // Audio effects management
    //--------------------

    /**
     * Attaches an auxiliary effect to the audio track. A typical auxiliary
     * effect is a reverberation effect which can be applied on any sound source
     * that directs a certain amount of its energy to this effect. This amount
     * is defined by setAuxEffectSendLevel().
     * {@see #setAuxEffectSendLevel(Float)}.
     * <p>After creating an auxiliary effect (e.g.
     * {@link android.media.audiofx.EnvironmentalReverb}), retrieve its ID with
     * {@link android.media.audiofx.AudioEffect#getId()} and use it when calling
     * this method to attach the audio track to the effect.
     * <p>To detach the effect from the audio track, call this method with a
     * null effect id.
     *
     * @param effectId system wide unique id of the effect to attach
     * @return error code or success, see {@link #SUCCESS},
     *    {@link #ERROR_INVALID_OPERATION}, {@link #ERROR_BAD_VALUE}
     */
    virtual CARAPI_(Int32) AttachAuxEffect(
        /* [in] */ Int32 effectId);

    /**
     * Sets the send level of the audio track to the attached auxiliary effect
     * {@see #attachAuxEffect(Int32)}. The level value range is 0 to 1.0.
     * <p>By default the send level is 0, so even if an effect is attached to the player
     * this method must be called for the effect to be applied.
     * <p>Note that the passed level value is a raw scalar. UI controls should be scaled
     * logarithmically: the gain applied by audio framework ranges from -72dB to 0dB,
     * so an appropriate conversion from linear UI input x to level is:
     * x == 0 -> level = 0
     * 0 < x <= R -> level = 10^(72*(x-R)/20/R)
     *
     * @param level send level scalar
     * @return error code or success, see {@link #SUCCESS},
     *    {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetAuxEffectSendLevel(
        /* [in] */ Float level);

protected:
    virtual CARAPI_(void) Finalize(); 

    /**
     *  Returns the native frame count used by the hardware.
     */
    virtual CARAPI_(Int32) GetNativeFrameCount();

    /**
     * Sets the initialization state of the instance. To be used in an AudioTrack subclass
     * constructor to set a subclass-specific post-initialization state.
     * @param state the state of the AudioTrack instance
     */
    virtual CARAPI_(void) SetState(
        /* [in] */ Int32 state);

private:

    CARAPI_(void) Init(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes, 
        /* [in] */ Int32 mode, 
        /* [in] */ Int32 sessionId);

    // Convenience method for the constructor's parameter checks.
    // This is where constructor IllegalArgumentException-s are thrown
    // postconditions:
    //    mStreamType is valid
    //    mChannelCount is valid
    //    mChannels is valid
    //    mAudioFormat is valid
    //    mSampleRate is valid
    //    mDataLoadMode is valid
    CARAPI_(void) AudioParamCheck(
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat, 
        /* [in] */ Int32 mode);

    // Convenience method for the contructor's audio buffer size check.
    // preconditions:
    //    mChannelCount is valid
    //    mAudioFormat is valid
    // postcondition:
    //    mNativeBufferSizeInBytes is valid (multiple of frame size, positive)
    CARAPI_(void) AudioBuffSizeCheck(
        /* [in] */ Int32 audioBufferSize);    

    //---------------------------------------------------------
    // Inner classes
    //--------------------
    /**
     * Helper class to handle the forwarding of native events to the appropriate listener
     * (potentially) handled in a different thread
     */  
    class NativeEventHandlerDelegate
    {
    public:
        NativeEventHandlerDelegate(
            /* [in] */ AudioTrack* track, 
            /* [in] */ IHandler* handler);

        CARAPI_(AutoPtr<IHandler>) GetHandler();

    private:
        AudioTrack* mAudioTrack;
        AutoPtr<IHandler> mHandler;
    };


    //---------------------------------------------------------
    // Java methods called from the native side
    //--------------------
    static CARAPI_(void) PostEventFromNative(
        /* [in] */ IInterface* audiotrack_ref,
        /* [in] */ Int32 what, 
        /* [in] */ Int32 arg1, 
        /* [in] */ Int32 arg2, 
        /* [in] */ IInterface* obj);


    //---------------------------------------------------------
    // Native methods called from the Java side
    //--------------------

    CARAPI_(Int32) NativeSetup(
        /* [in] */ IInterface* audiotrack_this,
        /* [in] */ Int32 streamType, 
        /* [in] */ Int32 sampleRate, 
        /* [in] */ Int32 nbChannels, 
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 buffSizeInBytes, 
        /* [in] */ Int32 mode, 
        /* [in] */ Int32* sessionId);

    CARAPI_(void) NativeFinalize();

    CARAPI_(void) NativeRelease();

    CARAPI_(void) NativeStart();

    CARAPI_(void) NativeStop();

    CARAPI_(void) NativePause();

    CARAPI_(void) NativeFlush();

    CARAPI_(Int32) NativeWriteByte(
        /* [in] */ ArrayOf<Byte>* audioData,
        /* [in] */ Int32 offsetInBytes, 
        /* [in] */ Int32 sizeInBytes, 
        /* [in] */ Int32 format);

    CARAPI_(Int32) NativeWriteInt16(
        /* [in] */ ArrayOf<Int16>* audioData,
        /* [in] */ Int32 offsetInShorts, 
        /* [in] */ Int32 sizeInShorts, 
        /* [in] */ Int32 format);

    CARAPI_(Int32) NativeReloadStatic();

    CARAPI_(Int32) NativeGetNativeFrameCount();

    CARAPI_(void) NativeSetVolume(
        /* [in] */ Float leftVolume, 
        /* [in] */ Float rightVolume);

    CARAPI_(Int32) NativeSetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz);
    
    CARAPI_(Int32) NativeGetPlaybackRate();

    CARAPI_(Int32) NativeSetMarkerPos(
        /* [in] */ Int32 marker);
    
    CARAPI_(Int32) NativeGetMarkerPos();

    CARAPI_(Int32) NativeSetPosUpdatePeriod(
        /* [in] */ Int32 updatePeriod);
    
    CARAPI_(Int32) NativeGetPosUpdatePeriod();

    CARAPI_(Int32) NativeSetPosition(
        /* [in] */ Int32 position);
    
    CARAPI_(Int32) NativeGetPosition();

    CARAPI_(Int32) NativeSetLoop(
        /* [in] */ Int32 start, 
        /* [in] */ Int32 end, 
        /* [in] */ Int32 loopCount);

    static CARAPI_(Int32) NativeGetOutputSampleRate(
        /* [in] */ Int32 streamType);
    
    static CARAPI_(Int32) NativeGetMinBuffSize(
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat);

    CARAPI_(Int32) NativeGetSessionId();

    CARAPI_(Int32) NativeAttachAuxEffect(
        /* [in] */ Int32 effectId);

    CARAPI_(void) NativeSetAuxEffectSendLevel(
        /* [in] */ Float level);

    //---------------------------------------------------------
    // Utility methods
    //------------------

    static CARAPI_(void) Logd(
        /* [in] */ String msg);

    static CARAPI_(void) Loge(
        /* [in] */ String msg);

private:
    //---------------------------------------------------------
    // Constants
    //--------------------
    /** Minimum value for a channel volume */
    static const Float VOLUME_MIN = 0.0f;
    /** Maximum value for a channel volume */
    static const Float VOLUME_MAX = 1.0f;

public:
    /** indicates AudioTrack state is stopped */
    static const Int32 PLAYSTATE_STOPPED = 1;  // matches SL_PLAYSTATE_STOPPED
    /** indicates AudioTrack state is paused */
    static const Int32 PLAYSTATE_PAUSED  = 2;  // matches SL_PLAYSTATE_PAUSED
    /** indicates AudioTrack state is playing */
    static const Int32 PLAYSTATE_PLAYING = 3;  // matches SL_PLAYSTATE_PLAYING

    /**
     * Creation mode where audio data is transferred from Java to the native layer
     * only once before the audio starts playing.
     */
    static const Int32 MODE_STATIC = 0;
    /**
     * Creation mode where audio data is streamed from Java to the native layer
     * as the audio is playing.
     */
    static const Int32 MODE_STREAM = 1;

    /**
     * State of an AudioTrack that was not successfully initialized upon creation.
     */
    static const Int32 STATE_UNINITIALIZED = 0;
    /**
     * State of an AudioTrack that is ready to be used.
     */
    static const Int32 STATE_INITIALIZED   = 1;
    /**
     * State of a successfully initialized AudioTrack that uses static data,
     * but that hasn't received that data yet.
     */
    static const Int32 STATE_NO_STATIC_DATA = 2;

    // Error codes:
    // to keep in sync with frameworks/base/core/jni/android_media_AudioTrack.cpp
    /**
     * Denotes a successful operation.
     */
    static const Int32 SUCCESS                               = 0;
    /**
     * Denotes a generic operation failure.
     */
    static const Int32 ERROR                                 = -1;
    /**
     * Denotes a failure due to the use of an invalid value.
     */
    static const Int32 ERROR_BAD_VALUE                       = -2;
    
    /**
     * Denotes a failure due to the improper use of a method.
     */
    static const Int32 ERROR_INVALID_OPERATION               = -3;

private:
    static const Int32 ERROR_NATIVESETUP_AUDIOSYSTEM         = -16;
    static const Int32 ERROR_NATIVESETUP_INVALIDCHANNELMASK  = -17;
    static const Int32 ERROR_NATIVESETUP_INVALIDFORMAT       = -18;
    static const Int32 ERROR_NATIVESETUP_INVALIDSTREAMTYPE   = -19;
    static const Int32 ERROR_NATIVESETUP_NATIVEINITFAILED    = -20;

    // Events:
    // to keep in sync with frameworks/base/include/media/AudioTrack.h
    /**
     * Event id denotes when playback head has reached a previously set marker.
     */
    static const Int32 NATIVE_EVENT_MARKER  = 3;
    /**
     * Event id denotes when previously set update period has elapsed during playback.
     */
    static const Int32 NATIVE_EVENT_NEW_POS = 4;

    static const String TAG;// = String("AudioTrack-Java");


    //--------------------------------------------------------------------------
    // Member variables
    //--------------------
    /**
     * Indicates the state of the AudioTrack instance.
     */
    Int32 mState;// = STATE_UNINITIALIZED;

    /**
     * Indicates the play state of the AudioTrack instance.
     */
    Int32 mPlayState;// = PLAYSTATE_STOPPED;

    /**
     * Lock to make sure mPlayState updates are reflecting the actual state of the object.
     */
    AutoPtr<IInterface> mPlayStateLock;// = new Object();

    /**
     * The listener the AudioTrack notifies when the playback position reaches a marker
     * or for periodic updates during the progression of the playback head.
     *  @see #setPlaybackPositionUpdateListener(OnPlaybackPositionUpdateListener)
     */
    AutoPtr<IOnPlaybackPositionUpdateListener> mPositionListener;// = null;
    /**
     * Lock to protect event listener updates against event notifications.
     */
    AutoPtr<IInterface> mPositionListenerLock;// = new Object();
    /**
     * Size of the native audio buffer.
     */
    Int32 mNativeBufferSizeInBytes;// = 0;
    
    /**
     * Handler for marker events coming from the native code.
     */
    NativeEventHandlerDelegate* mEventHandlerDelegate;// = null;
    
    /**
     * Looper associated with the thread that creates the AudioTrack instance.
     */
    //private Looper mInitializationLooper = null;
    
    /**
     * The audio data sampling rate in Hz.
     */
    Int32 mSampleRate;// = 22050;
    
    /**
     * The number of audio output channels (1 is mono, 2 is stereo).
     */
    Int32 mChannelCount;// = 1;
    
    /**
     * The audio channel mask.
     */
    Int32 mChannels;// = AudioFormat.CHANNEL_OUT_MONO;

    /**
     * The type of the audio stream to play. See
     *   {@link AudioManager#STREAM_VOICE_CALL}, {@link AudioManager#STREAM_SYSTEM},
     *   {@link AudioManager#STREAM_RING}, {@link AudioManager#STREAM_MUSIC} and
     *   {@link AudioManager#STREAM_ALARM}
     */
    Int32 mStreamType;// = AudioManager.STREAM_MUSIC;

    /**
     * The way audio is consumed by the hardware, streaming or static.
     */
    Int32 mDataLoadMode;// = MODE_STREAM;

    /**
     * The current audio channel configuration.
     */
    Int32 mChannelConfiguration;// = AudioFormat.CHANNEL_OUT_MONO;

    /**
     * The encoding of the audio samples.
     * @see AudioFormat#ENCODING_PCM_8BIT
     * @see AudioFormat#ENCODING_PCM_16BIT
     */
    Int32 mAudioFormat;// = AudioFormat.ENCODING_PCM_16BIT;

    /**
     * Audio session ID
     */
    Int32 mSessionId;// = 0;


    //--------------------------------
    // Used exclusively by native code
    //--------------------
    /**
     * Accessed by native methods: provides access to C++ AudioTrack object.
     */
    Int32 mNativeTrackInJavaObj;
    /**
     * Accessed by native methods: provides access to the JNI data (i.e. resources used by
     * the native AudioTrack object, but not stored in it).
     */
    Int32 mJniData;

};

#endif
