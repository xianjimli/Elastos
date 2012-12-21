
#include "media/AudioTrack.h"
#include "media/AudioFormat.h"

AudioTrack::AudioTrack()
{

}

AudioTrack::AudioTrack(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 sampleRateInHz, 
    /* [in] */ Int32 channelConfig, 
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes, 
    /* [in] */ Int32 mode)
{
    Init(streamType, sampleRateInHz, channelConfig, audioFormat,
            bufferSizeInBytes, mode, 0);
}

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
AudioTrack::AudioTrack(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 sampleRateInHz, 
    /* [in] */ Int32 channelConfig, 
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes, 
    /* [in] */ Int32 mode, 
    /* [in] */ Int32 sessionId)
{
    Init(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode, sessionId);
}

void AudioTrack::Init(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 sampleRateInHz, 
    /* [in] */ Int32 channelConfig, 
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes, 
    /* [in] */ Int32 mode, 
    /* [in] */ Int32 sessionId)
{
    mState = STATE_UNINITIALIZED;

    // remember which looper is associated with the AudioTrack instanciation
    /*if ((mInitializationLooper = Looper.myLooper()) == NULL) {
        mInitializationLooper = Looper.getMainLooper();
    }*/

    AudioParamCheck(streamType, sampleRateInHz, channelConfig, audioFormat, mode);

    AudioBuffSizeCheck(bufferSizeInBytes);

    if (sessionId < 0) {
        //throw (new IllegalArgumentException("Invalid audio session ID: "+sessionId));
    }

    Int32* session = new Int32[1];
    session[0] = sessionId;
    // native initialization
    Int32 initResult; /*= NativeSetup(new WeakReference<AudioTrack>(this),
        mStreamType, mSampleRate, mChannels, mAudioFormat,
        mNativeBufferSizeInBytes, mDataLoadMode, session);*/
    if (initResult != SUCCESS) {
        //loge("Error code "+initResult+" when initializing AudioTrack.");
        return; // with mState == STATE_UNINITIALIZED
    }

    mSessionId = session[0];

    if (mDataLoadMode == MODE_STATIC) {
        mState = STATE_NO_STATIC_DATA;
    } else {
        mState = STATE_INITIALIZED;
    }
}
// Convenience method for the constructor's parameter checks.
// This is where constructor IllegalArgumentException-s are thrown
// postconditions:
//    mStreamType is valid
//    mChannelCount is valid
//    mChannels is valid
//    mAudioFormat is valid
//    mSampleRate is valid
//    mDataLoadMode is valid
void AudioTrack::AudioParamCheck(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig, 
    /* [in] */ Int32 audioFormat, 
    /* [in] */ Int32 mode)
{
    //--------------
    // stream type
    //if( (streamType != AudioManager.STREAM_ALARM) && (streamType != AudioManager.STREAM_MUSIC)
    //   && (streamType != AudioManager.STREAM_RING) && (streamType != AudioManager.STREAM_SYSTEM)
    //   && (streamType != AudioManager.STREAM_VOICE_CALL)
    //   && (streamType != AudioManager.STREAM_NOTIFICATION)
    //   && (streamType != AudioManager.STREAM_BLUETOOTH_SCO)
    //   && (streamType != AudioManager.STREAM_DTMF)) {
    //    //throw (new IllegalArgumentException("Invalid stream type."));
    //} else {
    //    mStreamType = streamType;
    //}

    //--------------
    // sample rate
    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        /*throw (new IllegalArgumentException(sampleRateInHz
                + "Hz is not a supported sample rate."));*/
    } else {
        mSampleRate = sampleRateInHz;
    }

    //--------------
    // channel config
    mChannelConfiguration = channelConfig;

    switch (channelConfig) {
    case AudioFormat::CHANNEL_OUT_DEFAULT: //AudioFormat.CHANNEL_CONFIGURATION_DEFAULT
    case AudioFormat::CHANNEL_OUT_MONO:
    case AudioFormat::CHANNEL_CONFIGURATION_MONO:
        mChannelCount = 1;
        mChannels = AudioFormat::CHANNEL_OUT_MONO;
        break;
    case AudioFormat::CHANNEL_OUT_STEREO:
    case AudioFormat::CHANNEL_CONFIGURATION_STEREO:
        mChannelCount = 2;
        mChannels = AudioFormat::CHANNEL_OUT_STEREO;
        break;
    default:
        mChannelCount = 0;
        mChannels = AudioFormat::CHANNEL_INVALID;
        mChannelConfiguration = AudioFormat::CHANNEL_CONFIGURATION_INVALID;
        //throw(new IllegalArgumentException("Unsupported channel configuration."));
    }

    //--------------
    // audio format
    switch (audioFormat) {
    case AudioFormat::ENCODING_DEFAULT:
        mAudioFormat = AudioFormat::ENCODING_PCM_16BIT;
        break;
    case AudioFormat::ENCODING_PCM_16BIT:
    case AudioFormat::ENCODING_PCM_8BIT:
        mAudioFormat = audioFormat;
        break;
    default:
        mAudioFormat = AudioFormat::ENCODING_INVALID;
        /*throw(new IllegalArgumentException("Unsupported sample encoding."
            + " Should be ENCODING_PCM_8BIT or ENCODING_PCM_16BIT."));*/
    }

    //--------------
    // audio load mode
    if ( (mode != MODE_STREAM) && (mode != MODE_STATIC) ) {
        //throw(new IllegalArgumentException("Invalid mode."));
    } else {
        mDataLoadMode = mode;
    }
}


// Convenience method for the contructor's audio buffer size check.
// preconditions:
//    mChannelCount is valid
//    mAudioFormat is valid
// postcondition:
//    mNativeBufferSizeInBytes is valid (multiple of frame size, positive)
void AudioTrack::AudioBuffSizeCheck(
    /* [in] */ Int32 audioBufferSize) 
{
    // NB: this section is only valid with PCM data.
    //     To update when supporting compressed formats
    Int32 frameSizeInBytes = mChannelCount
            * (mAudioFormat == AudioFormat::ENCODING_PCM_8BIT ? 1 : 2);
    if ((audioBufferSize % frameSizeInBytes != 0) || (audioBufferSize < 1)) {
        //throw (new IllegalArgumentException("Invalid audio buffer size."));
    }

    mNativeBufferSizeInBytes = audioBufferSize;
}


/**
 * Releases the native AudioTrack resources.
 */
ECode AudioTrack::Release() 
{
    // even though native_release() stops the native AudioTrack, we need to stop
    // AudioTrack subclasses too.
    //try {
        Stop();
    //} catch(IllegalStateException ise) { 
        // don't raise an exception, we're releasing the resources.
    //}
    NativeRelease();
    mState = STATE_UNINITIALIZED;

    return NOERROR;
}

void AudioTrack::Finalize()
{
    NativeFinalize();
}

//--------------------------------------------------------------------------
// Getters
//--------------------
/**
 * Returns the minimum valid volume value. Volume values set under this one will
 * be clamped at this value.
 * @return the minimum volume expressed as a linear attenuation.
 */
Float AudioTrack::GetMinVolume()
{
    return VOLUME_MIN;
}

/**
 * Returns the maximum valid volume value. Volume values set above this one will
 * be clamped at this value.
 * @return the maximum volume expressed as a linear attenuation.
 */
Float AudioTrack::GetMaxVolume()
{
    return VOLUME_MAX;
}

/**
 * Returns the configured audio data sample rate in Hz
 */
Int32 AudioTrack::GetSampleRate()
{
    return mSampleRate;
}

/**
 * Returns the current playback rate in Hz.
 */
Int32 AudioTrack::GetPlaybackRate()
{
    return NativeGetPlaybackRate();
}

/**
 * Returns the configured audio data format. See {@link AudioFormat#ENCODING_PCM_16BIT}
 * and {@link AudioFormat#ENCODING_PCM_8BIT}.
 */
Int32 AudioTrack::GetAudioFormat() 
{
    return mAudioFormat;
}

/**
 * Returns the type of audio stream this AudioTrack is configured for.
 * Compare the result against {@link AudioManager#STREAM_VOICE_CALL},
 * {@link AudioManager#STREAM_SYSTEM}, {@link AudioManager#STREAM_RING},
 * {@link AudioManager#STREAM_MUSIC} or {@link AudioManager#STREAM_ALARM}
 */
Int32 AudioTrack::GetStreamType()
{
    return mStreamType;
}

/**
 * Returns the configured channel configuration.

 * See {@link AudioFormat#CHANNEL_OUT_MONO}
 * and {@link AudioFormat#CHANNEL_OUT_STEREO}.
 */
Int32 AudioTrack::GetChannelConfiguration()
{
    return mChannelConfiguration;
}

/**
 * Returns the configured number of channels.
 */
Int32 AudioTrack::GetChannelCount()
{
    return mChannelCount;
}

/**
 * Returns the state of the AudioTrack instance. This is useful after the
 * AudioTrack instance has been created to check if it was initialized
 * properly. This ensures that the appropriate hardware resources have been
 * acquired.
 * @see #STATE_INITIALIZED
 * @see #STATE_NO_STATIC_DATA
 * @see #STATE_UNINITIALIZED
 */
Int32 AudioTrack::GetState()
{
    return mState;
}

/**
 * Returns the playback state of the AudioTrack instance.
 * @see #PLAYSTATE_STOPPED
 * @see #PLAYSTATE_PAUSED
 * @see #PLAYSTATE_PLAYING
 */
Int32 AudioTrack::GetPlayState()
{
    return mPlayState;
}

/**
 *  Returns the native frame count used by the hardware.
 */
Int32 AudioTrack::GetNativeFrameCount()
{
    return NativeGetNativeFrameCount();
}

/**
 * Returns marker position expressed in frames.
 */
Int32 AudioTrack::GetNotificationMarkerPosition()
{
    return NativeGetMarkerPos();
}

/**
 * Returns the notification update period expressed in frames.
 */
Int32 AudioTrack::GetPositionNotificationPeriod() 
{
    return NativeGetPosUpdatePeriod();
}

/**
 * Returns the playback head position expressed in frames
 */
Int32 AudioTrack::GetPlaybackHeadPosition() 
{
    return NativeGetPosition();
}

/**
 *  Returns the hardware output sample rate
 */
Int32 AudioTrack::GetNativeOutputSampleRate(
    /* [in] */ Int32 streamType)
{
    return NativeGetOutputSampleRate(streamType);
}

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
Int32 AudioTrack::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz, 
    /* [in] */ Int32 channelConfig, 
    /* [in] */ Int32 audioFormat)
{
    Int32 channelCount = 0;
    switch(channelConfig) {
    case AudioFormat::CHANNEL_OUT_MONO:
    case AudioFormat::CHANNEL_CONFIGURATION_MONO:
        channelCount = 1;
        break;
    case AudioFormat::CHANNEL_OUT_STEREO:
    case AudioFormat::CHANNEL_CONFIGURATION_STEREO:
        channelCount = 2;
        break;
    default:
        Loge(String("getMinBufferSize(): Invalid channel configuration."));
        return AudioTrack::ERROR_BAD_VALUE;
    }
    
    if ((audioFormat != AudioFormat::ENCODING_PCM_16BIT) 
        && (audioFormat != AudioFormat::ENCODING_PCM_8BIT)) {
        Loge(String("getMinBufferSize(): Invalid audio format."));
        return AudioTrack::ERROR_BAD_VALUE;
    }
    
    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        //Loge(String("getMinBufferSize(): ") + sampleRateInHz + String("Hz is not a supported sample rate."));
        return AudioTrack::ERROR_BAD_VALUE;
    }
    
    Int32 size = NativeGetMinBuffSize(sampleRateInHz, channelCount, audioFormat);
    if ((size == -1) || (size == 0)) {
        Loge(String("getMinBufferSize(): error querying hardware"));
        return AudioTrack::ERROR;
    }
    else {
        return size;
    }
}

/**
 * Returns the audio session ID.
 *
 * @return the ID of the audio session this AudioTrack belongs to.
 */
Int32 AudioTrack::GetAudioSessionId() 
{
    return mSessionId;
}

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
ECode AudioTrack::SetPlaybackPositionUpdateListener(
    /* [in] */ IOnPlaybackPositionUpdateListener* listener) 
{
    SetPlaybackPositionUpdateListener(listener, NULL);

    return NOERROR;
}

/**
 * Sets the listener the AudioTrack notifies when a previously set marker is reached or
 * for each periodic playback head position update.
 * Use this method to receive AudioTrack events in the Handler associated with another
 * thread than the one in which you created the AudioTrack instance.
 * @param listener
 * @param handler the Handler that will receive the event notification messages.
 */
ECode AudioTrack::SetPlaybackPositionUpdateListener(
    /* [in] */ IOnPlaybackPositionUpdateListener* listener, 
    /* [in] */ IHandler* handler)
{
    //synchronized (mPositionListenerLock) {
        mPositionListener = listener;
    //}
    if (listener != NULL) {
        mEventHandlerDelegate = new NativeEventHandlerDelegate(this, handler);
    }
    return NOERROR;
}

 /**
 * Sets the specified left/right output volume values on the AudioTrack. Values are clamped
 * to the ({@link #getMinVolume()}, {@link #getMaxVolume()}) interval if outside this range.
 * @param leftVolume output attenuation for the left channel. A value of 0.0f is silence,
 *      a value of 1.0f is no attenuation.
 * @param rightVolume output attenuation for the right channel
 * @return error code or success, see {@link #SUCCESS},
 *    {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioTrack::SetStereoVolume(
    /* [in] */ Float leftVolume, 
    /* [in] */ Float rightVolume) 
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    // clamp the volumes
    if (leftVolume < GetMinVolume()) {
        leftVolume = GetMinVolume();
    }
    if (leftVolume > GetMaxVolume()) {
        leftVolume = GetMaxVolume();
    }
    if (rightVolume < GetMinVolume()) {
        rightVolume = GetMinVolume();
    }
    if (rightVolume > GetMaxVolume()) {
        rightVolume = GetMaxVolume();
    }

    NativeSetVolume(leftVolume, rightVolume);

    return SUCCESS;
}


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
Int32 AudioTrack::SetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    if (sampleRateInHz <= 0) {
        return ERROR_BAD_VALUE;
    }
    return NativeSetPlaybackRate(sampleRateInHz);
}


/**
 * Sets the position of the notification marker.
 * @param markerInFrames marker in frames
 * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
 *  {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioTrack::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    return NativeSetMarkerPos(markerInFrames);
}


/**
 * Sets the period for the periodic notification event.
 * @param periodInFrames update period expressed in frames
 * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioTrack::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    return NativeSetPosUpdatePeriod(periodInFrames);
}


/**
 * Sets the playback head position. The track must be stopped for the position to be changed.
 * @param positionInFrames playback head position expressed in frames
 * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
 *    {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioTrack::SetPlaybackHeadPosition(
    /* [in] */ Int32 positionInFrames)
{
    //synchronized(mPlayStateLock) {
        if ((mPlayState == PLAYSTATE_STOPPED) || (mPlayState == PLAYSTATE_PAUSED)) {
            return NativeSetPosition(positionInFrames);
        } else {
            return ERROR_INVALID_OPERATION;
        }
    //}
}

/**
 * Sets the loop points and the loop count. The loop can be infinite.
 * @param startInFrames loop start marker expressed in frames
 * @param endInFrames loop end marker expressed in frames
 * @param loopCount the number of times the loop is looped.
 *    A value of -1 means infinite looping.
 * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
 *    {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioTrack::SetLoopPoints(
    /* [in] */ Int32 startInFrames, 
    /* [in] */ Int32 endInFrames, 
    /* [in] */ Int32 loopCount)
{
    if (mDataLoadMode == MODE_STREAM) {
        return ERROR_INVALID_OPERATION;
    }
    return NativeSetLoop(startInFrames, endInFrames, loopCount);
}

/**
 * Sets the initialization state of the instance. To be used in an AudioTrack subclass
 * constructor to set a subclass-specific post-initialization state.
 * @param state the state of the AudioTrack instance
 */
void AudioTrack::SetState(
    /* [in] */ Int32 state)
{
    mState = state;
}


//---------------------------------------------------------
// Transport control methods
//--------------------
/**
 * Starts playing an AudioTrack.
 * @throws IllegalStateException
 */
ECode AudioTrack::Play()
{
    if (mState != STATE_INITIALIZED) {
        //throw(new IllegalStateException("play() called on uninitialized AudioTrack."));
    }

    //synchronized(mPlayStateLock) {
        NativeStart();
        mPlayState = PLAYSTATE_PLAYING;
    //}

    return NOERROR;
}

/**
 * Stops playing the audio data.
 * @throws IllegalStateException
 */
ECode AudioTrack::Stop()
{
    if (mState != STATE_INITIALIZED) {
        //throw(new IllegalStateException("stop() called on uninitialized AudioTrack."));
    }

    // stop playing
    //synchronized(mPlayStateLock) {
        NativeStop();
        mPlayState = PLAYSTATE_STOPPED;
    //}

    return NOERROR;
}

/**
 * Pauses the playback of the audio data.
 * @throws IllegalStateException
 */
ECode AudioTrack::Pause()
{
    if (mState != STATE_INITIALIZED) {
        //throw(new IllegalStateException("pause() called on uninitialized AudioTrack."));
    }
    Logd(String("pause()"));

    // pause playback
    //synchronized(mPlayStateLock) {
        NativePause();
        mPlayState = PLAYSTATE_PAUSED;
    //}

    return NOERROR;
}


//---------------------------------------------------------
// Audio data supply
//--------------------

/**
 * Flushes the audio data currently queued for playback.
 */

ECode AudioTrack::Flush()
{
    if (mState == STATE_INITIALIZED) {
        // flush the data in native layer
        NativeFlush();
    }
    return NOERROR;
}

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

Int32 AudioTrack::Write(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes, 
    /* [in] */ Int32 sizeInBytes) 
{
    if ((mDataLoadMode == MODE_STATIC)
            && (mState == STATE_NO_STATIC_DATA)
            && (sizeInBytes > 0)) {
        mState = STATE_INITIALIZED;
    }

    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    if ( (audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0) 
            || (offsetInBytes + sizeInBytes > audioData->GetLength())) {
        return ERROR_BAD_VALUE;
    }

    return NativeWriteByte(audioData, offsetInBytes, sizeInBytes, mAudioFormat);
}


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

Int32 AudioTrack::Write(
    /* [in] */ ArrayOf<Int16>* audioData, 
    /* [in] */ Int32 offsetInShorts, 
    /* [in] */ Int32 sizeInShorts)
{
    if ((mDataLoadMode == MODE_STATIC)
            && (mState == STATE_NO_STATIC_DATA)
            && (sizeInShorts > 0)) {
        mState = STATE_INITIALIZED;
    }
    
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    if ( (audioData == NULL) || (offsetInShorts < 0 ) || (sizeInShorts < 0) 
            || (offsetInShorts + sizeInShorts > audioData->GetLength())) {
        return ERROR_BAD_VALUE;
    }

    return NativeWriteInt16(audioData, offsetInShorts, sizeInShorts, mAudioFormat);
}


/**
 * Notifies the native resource to reuse the audio data already loaded in the native
 * layer. This call is only valid with AudioTrack instances that don't use the streaming
 * model.
 * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
 *  {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioTrack::ReloadStaticData()
{
    if (mDataLoadMode == MODE_STREAM) {
        return ERROR_INVALID_OPERATION;
    }
    return NativeReloadStatic();
}

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
 * NULL effect id.
 *
 * @param effectId system wide unique id of the effect to attach
 * @return error code or success, see {@link #SUCCESS},
 *    {@link #ERROR_INVALID_OPERATION}, {@link #ERROR_BAD_VALUE}
 */
Int32 AudioTrack::AttachAuxEffect(
    /* [in] */ Int32 effectId) 
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    return NativeAttachAuxEffect(effectId);
}

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
Int32 AudioTrack::SetAuxEffectSendLevel(
    /* [in] */ Float level)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    // clamp the level
    if (level < GetMinVolume()) {
        level = GetMinVolume();
    }
    if (level > GetMaxVolume()) {
        level = GetMaxVolume();
    }
    NativeSetAuxEffectSendLevel(level);
    return SUCCESS;
}


//---------------------------------------------------------
// Inner classes
//--------------------
/**
 * Helper class to handle the forwarding of native events to the appropriate listener
 * (potentially) handled in a different thread
 */    
AudioTrack::NativeEventHandlerDelegate::NativeEventHandlerDelegate(
    /* [in] */ AudioTrack* track, 
    /* [in] */ IHandler* handler)
{
    mAudioTrack = track;
    // find the looper for our new event handler
    //Looper looper;
    //if (handler != NULL) {
    //    looper = handler.getLooper();
    //} else {
    //    // no given handler, use the looper the AudioTrack was created in
    //    looper = mInitializationLooper;
    //}
    //
    //// construct the event handler with this looper
    //if (looper != NULL) {
    //    // implement the event handler delegate
    //    mHandler = new Handler(looper) {
    //        @Override
    //        public void handleMessage(Message msg) {
    //            if (mAudioTrack == NULL) {
    //                return;
    //            }
    //            OnPlaybackPositionUpdateListener listener = NULL;
    //            synchronized (mPositionListenerLock) {
    //                listener = mAudioTrack.mPositionListener;
    //            }
    //            switch(msg.what) {
    //            case NATIVE_EVENT_MARKER:
    //                if (listener != NULL) {
    //                    listener.onMarkerReached(mAudioTrack);
    //                }
    //                break;
    //            case NATIVE_EVENT_NEW_POS:
    //                if (listener != NULL) {
    //                    listener.onPeriodicNotification(mAudioTrack);
    //                }
    //                break;
    //            default:
    //                Log.e(TAG, "[ android.media.AudioTrack.NativeEventHandler ] " +
    //                        "Unknown event type: " + msg.what);
    //                break;
    //            }
    //        }
    //    };
    //} else {
    //    mHandler = NULL;
    //} 
}

AutoPtr<IHandler> AudioTrack::NativeEventHandlerDelegate::GetHandler()
{
    return mHandler;
}

//---------------------------------------------------------
// Java methods called from the native side
//--------------------
void AudioTrack::PostEventFromNative(
    /* [in] */ IInterface* audiotrack_ref,
    /* [in] */ Int32 what, 
    /* [in] */ Int32 arg1, 
    /* [in] */ Int32 arg2, 
    /* [in] */ IInterface* obj)
{
    //logd("Event posted from the native side: event="+ what + " args="+ arg1+" "+arg2);
    /*AudioTrack track = (AudioTrack)((WeakReference)audiotrack_ref).get();
    if (track == NULL) {
    return;
    }

    if (track.mEventHandlerDelegate != NULL) {
    Message m = 
    track.mEventHandlerDelegate.getHandler().obtainMessage(what, arg1, arg2, obj);
    track.mEventHandlerDelegate.getHandler().sendMessage(m);
    }*/

}

Int32 AudioTrack::NativeSetup(
    /* [in] */ IInterface* audiotrack_this,
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 sampleRate, 
    /* [in] */ Int32 nbChannels, 
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 buffSizeInBytes, 
    /* [in] */ Int32 mode, 
    /* [in] */ Int32* sessionId)
{
    return 0;
}

void AudioTrack::NativeFinalize()
{

}

void AudioTrack::NativeRelease()
{

}

void AudioTrack::NativeStart()
{

}

void AudioTrack::NativeStop()
{

}

void AudioTrack::NativePause()
{

}

void AudioTrack::NativeFlush()
{

}

Int32 AudioTrack::NativeWriteByte(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes, 
    /* [in] */ Int32 sizeInBytes, 
    /* [in] */ Int32 format)
{
    return 0;
}

Int32 AudioTrack::NativeWriteInt16(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts, 
    /* [in] */ Int32 sizeInShorts, 
    /* [in] */ Int32 format)
{
    return 0;
}

Int32 AudioTrack::NativeReloadStatic()
{
    return 0;
}

Int32 AudioTrack::NativeGetNativeFrameCount()
{
    return 0;
}

void AudioTrack::NativeSetVolume(
    /* [in] */ Float leftVolume, 
    /* [in] */ Float rightVolume)
{

}

Int32 AudioTrack::NativeSetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz)
{
    return 0;
}

Int32 AudioTrack::NativeGetPlaybackRate()
{
    return 0;
}

Int32 AudioTrack::NativeSetMarkerPos(
    /* [in] */ Int32 marker)
{
    return 0;
}

Int32 AudioTrack::NativeGetMarkerPos()
{
    return 0;
}

Int32 AudioTrack::NativeSetPosUpdatePeriod(
    /* [in] */ Int32 updatePeriod)
{
    return 0;
}

Int32 AudioTrack::NativeGetPosUpdatePeriod()
{
    return 0;
}

Int32 AudioTrack::NativeSetPosition(
    /* [in] */ Int32 position)
{
    return 0;
}

Int32 AudioTrack::NativeGetPosition()
{
    return 0;
}

Int32 AudioTrack::NativeSetLoop(
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end, 
    /* [in] */ Int32 loopCount)
{
    return 0;
}

Int32 AudioTrack::NativeGetOutputSampleRate(
    /* [in] */ Int32 streamType)
{
    return 0;
}

Int32 AudioTrack::NativeGetMinBuffSize(
    /* [in] */ Int32 sampleRateInHz, 
    /* [in] */ Int32 channelConfig, 
    /* [in] */ Int32 audioFormat)
{
    return 0;
}

Int32 AudioTrack::NativeGetSessionId()
{
    return 0;
}

Int32 AudioTrack::NativeAttachAuxEffect(
    /* [in] */ Int32 effectId)
{
    return 0;
}

void AudioTrack::NativeSetAuxEffectSendLevel(
    /* [in] */ Float level)
{

}

#if 0

// ----------------------------------------------------------------------------
//Int32 AudioTrack::NativeSetup(
//    /* [in] */ JNIEnv *env, 
//    /* [in] */ jobject thiz, 
//    /* [in] */ jobject weak_this,
//    /* [in] */ jint streamType, 
//    /* [in] */ jint sampleRateInHertz, 
//    /* [in] */ jint channels,
//    /* [in] */ jint audioFormat, 
//    /* [in] */ jint buffSizeInBytes, 
//    /* [in] */ jint memoryMode, 
//    /* [in] */ jintArray jSession)
//{
//    LOGV("sampleRate=%d, audioFormat(from Java)=%d, channels=%x, buffSize=%d",
//        sampleRateInHertz, audioFormat, channels, buffSizeInBytes);
//    Int32 afSampleRate;
//    Int32 afFrameCount;
//
//    if (AudioSystem::getOutputFrameCount(&afFrameCount, streamType) != NO_ERROR) {
//        LOGE("Error creating AudioTrack: Could not get AudioSystem frame count.");
//        return AUDIOTRACK_ERROR_SETUP_AUDIOSYSTEM;
//    }
//    if (AudioSystem::getOutputSamplingRate(&afSampleRate, streamType) != NO_ERROR) {
//        LOGE("Error creating AudioTrack: Could not get AudioSystem sampling rate.");
//        return AUDIOTRACK_ERROR_SETUP_AUDIOSYSTEM;
//    }
//
//    if (!AudioSystem::isOutputChannel(channels)) {
//        LOGE("Error creating AudioTrack: invalid channel mask.");
//        return AUDIOTRACK_ERROR_SETUP_INVALIDCHANNELMASK;
//    }
//    Int32 nbChannels = AudioSystem::popCount(channels);
//    
//    // check the stream type
//    AudioSystem::stream_type atStreamType;
//    if (streamType == javaAudioTrackFields.STREAM_VOICE_CALL) {
//        atStreamType = AudioSystem::VOICE_CALL;
//    } else if (streamType == javaAudioTrackFields.STREAM_SYSTEM) {
//        atStreamType = AudioSystem::SYSTEM;
//    } else if (streamType == javaAudioTrackFields.STREAM_RING) {
//        atStreamType = AudioSystem::RING;
//    } else if (streamType == javaAudioTrackFields.STREAM_MUSIC) {
//        atStreamType = AudioSystem::MUSIC;
//    } else if (streamType == javaAudioTrackFields.STREAM_ALARM) {
//        atStreamType = AudioSystem::ALARM;
//    } else if (streamType == javaAudioTrackFields.STREAM_NOTIFICATION) {
//        atStreamType = AudioSystem::NOTIFICATION;
//    } else if (streamType == javaAudioTrackFields.STREAM_BLUETOOTH_SCO) {
//        atStreamType = AudioSystem::BLUETOOTH_SCO;
//    } else if (streamType == javaAudioTrackFields.STREAM_DTMF) {
//        atStreamType = AudioSystem::DTMF;
//    } else {
//        LOGE("Error creating AudioTrack: unknown stream type.");
//        return AUDIOTRACK_ERROR_SETUP_INVALIDSTREAMTYPE;
//    }
//
//    // check the format.
//    // This function was called from Java, so we compare the format against the Java constants
//    if ((audioFormat != javaAudioTrackFields.PCM16) && (audioFormat != javaAudioTrackFields.PCM8)) {
//        LOGE("Error creating AudioTrack: unsupported audio format.");
//        return AUDIOTRACK_ERROR_SETUP_INVALIDFORMAT;
//    }
//
//    // for the moment 8bitPCM in MODE_STATIC is not supported natively in the AudioTrack C++ class
//    // so we declare everything as 16bitPCM, the 8->16bit conversion for MODE_STATIC will be handled
//    // in android_media_AudioTrack_native_write()
//    if ((audioFormat == javaAudioTrackFields.PCM8) 
//        && (memoryMode == javaAudioTrackFields.MODE_STATIC)) {
//        LOGV("android_media_AudioTrack_native_setup(): requesting MODE_STATIC for 8bit \
//            buff size of %dbytes, switching to 16bit, buff size of %dbytes",
//            buffSizeInBytes, 2*buffSizeInBytes);
//        audioFormat = javaAudioTrackFields.PCM16;
//        // we will need twice the memory to store the data
//        buffSizeInBytes *= 2;
//    }
//
//    // compute the frame count
//    Int32 bytesPerSample = audioFormat == javaAudioTrackFields.PCM16 ? 2 : 1;
//    Int32 format = audioFormat == javaAudioTrackFields.PCM16 ? 
//            AudioSystem::PCM_16_BIT : AudioSystem::PCM_8_BIT;
//    Int32 frameCount = buffSizeInBytes / (nbChannels * bytesPerSample);
//    
//    AudioTrackJniStorage* lpJniStorage = new AudioTrackJniStorage();
//    
//    // initialize the callback information:
//    // this data will be passed with every AudioTrack callback
//    jclass clazz = env->GetObjectClass(thiz);
//    if (clazz == NULL) {
//        LOGE("Can't find %s when setting up callback.", kClassPathName);
//        delete lpJniStorage;
//        return AUDIOTRACK_ERROR_SETUP_NATIVEINITFAILED;
//    }
//    lpJniStorage->mCallbackData.audioTrack_class = (jclass)env->NewGlobalRef(clazz);
//    // we use a weak reference so the AudioTrack object can be garbage collected.
//    lpJniStorage->mCallbackData.audioTrack_ref = env->NewGlobalRef(weak_this);
//    
//    lpJniStorage->mStreamType = atStreamType;
//
//    if (jSession == NULL) {
//        LOGE("Error creating AudioTrack: invalid session ID pointer");
//        delete lpJniStorage;
//        return AUDIOTRACK_ERROR;
//    }
//
//    jint* nSession = (jint *) env->GetPrimitiveArrayCritical(jSession, NULL);
//    if (nSession == NULL) {
//        LOGE("Error creating AudioTrack: Error retrieving session id pointer");
//        delete lpJniStorage;
//        return AUDIOTRACK_ERROR;
//    }
//    Int32 sessionId = nSession[0];
//    env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
//    nSession = NULL;
//
//    // create the native AudioTrack object
//    AudioTrack* lpTrack = new AudioTrack();
//    if (lpTrack == NULL) {
//        LOGE("Error creating uninitialized AudioTrack");
//        goto native_track_failure;
//    }
//    
//    // initialize the native AudioTrack object
//    if (memoryMode == javaAudioTrackFields.MODE_STREAM) {
//
//        lpTrack->set(
//            atStreamType,// stream type
//            sampleRateInHertz,
//            format,// word length, PCM
//            channels,
//            frameCount,
//            0,// flags
//            audioCallback, &(lpJniStorage->mCallbackData),//callback, callback data (user)
//            0,// notificationFrames == 0 since not using EVENT_MORE_DATA to feed the AudioTrack
//            0,// shared mem
//            true,// thread can call Java
//            sessionId);// audio session ID
//            
//    } else if (memoryMode == javaAudioTrackFields.MODE_STATIC) {
//        // AudioTrack is using shared memory
//        
//        if (!lpJniStorage->allocSharedMem(buffSizeInBytes)) {
//            LOGE("Error creating AudioTrack in static mode: error creating mem heap base");
//            goto native_init_failure;
//        }
//        
//        lpTrack->set(
//            atStreamType,// stream type
//            sampleRateInHertz,
//            format,// word length, PCM
//            channels,
//            frameCount,
//            0,// flags
//            audioCallback, &(lpJniStorage->mCallbackData),//callback, callback data (user));
//            0,// notificationFrames == 0 since not using EVENT_MORE_DATA to feed the AudioTrack 
//            lpJniStorage->mMemBase,// shared mem
//            true,// thread can call Java
//            sessionId);// audio session ID
//    }
//
//    if (lpTrack->initCheck() != NO_ERROR) {
//        LOGE("Error initializing AudioTrack");
//        goto native_init_failure;
//    }
//
//    nSession = (jint *) env->GetPrimitiveArrayCritical(jSession, NULL);
//    if (nSession == NULL) {
//        LOGE("Error creating AudioTrack: Error retrieving session id pointer");
//        goto native_init_failure;
//    }
//    // read the audio session ID back from AudioTrack in case we create a new session
//    nSession[0] = lpTrack->getSessionId();
//    env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
//    nSession = NULL;
//
//    // save our newly created C++ AudioTrack in the "nativeTrackInJavaObj" field 
//    // of the Java object (in mNativeTrackInJavaObj)
//    env->SetIntField(thiz, javaAudioTrackFields.nativeTrackInJavaObj, (Int32)lpTrack);
//    
//    // save the JNI resources so we can free them later
//    //LOGV("storing lpJniStorage: %x\n", (Int32)lpJniStorage);
//    env->SetIntField(thiz, javaAudioTrackFields.jniData, (Int32)lpJniStorage);
//
//    return AUDIOTRACK_SUCCESS;
//    
//    // failures:
//native_init_failure:
//    delete lpTrack;
//    env->SetIntField(thiz, javaAudioTrackFields.nativeTrackInJavaObj, 0);
//    
//native_track_failure:
//    if (nSession != NULL) {
//        env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
//    }
//    env->DeleteGlobalRef(lpJniStorage->mCallbackData.audioTrack_class);
//    env->DeleteGlobalRef(lpJniStorage->mCallbackData.audioTrack_ref);
//    delete lpJniStorage;
//    env->SetIntField(thiz, javaAudioTrackFields.jniData, 0);
//    return AUDIOTRACK_ERROR_SETUP_NATIVEINITFAILED;
    
//}


// ----------------------------------------------------------------------------
void AudioTrack::NativeStart(JNIEnv *env, jobject thiz)
{
    /*AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for start()");
        return;
    }

    lpTrack->start();*/
}


// ----------------------------------------------------------------------------
static void
android_media_AudioTrack_stop(JNIEnv *env, jobject thiz)
{
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for stop()");
        return;
    }

    lpTrack->stop();
}


// ----------------------------------------------------------------------------
static void
android_media_AudioTrack_pause(JNIEnv *env, jobject thiz)
{
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for pause()");
        return;
    }

    lpTrack->pause();
}


// ----------------------------------------------------------------------------
static void
android_media_AudioTrack_flush(JNIEnv *env, jobject thiz)
{
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for flush()");
        return;
    }

    lpTrack->flush();
}

// ----------------------------------------------------------------------------
static void
android_media_AudioTrack_set_volume(JNIEnv *env, jobject thiz, jfloat leftVol, jfloat rightVol )
{
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for setVolume()");
        return;
    }

    lpTrack->setVolume(leftVol, rightVol);
}

// ----------------------------------------------------------------------------
static void android_media_AudioTrack_native_finalize(JNIEnv *env,  jobject thiz) {
    //LOGV("android_media_AudioTrack_native_finalize jobject: %x\n", (Int32)thiz);
       
    // delete the AudioTrack object
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack) {
        //LOGV("deleting lpTrack: %x\n", (Int32)lpTrack);
        lpTrack->stop();
        delete lpTrack;
    }
    
    // delete the JNI data
    AudioTrackJniStorage* pJniStorage = (AudioTrackJniStorage *)env->GetIntField(
        thiz, javaAudioTrackFields.jniData);
    if (pJniStorage) {
        // delete global refs created in native_setup
        env->DeleteGlobalRef(pJniStorage->mCallbackData.audioTrack_class);
        env->DeleteGlobalRef(pJniStorage->mCallbackData.audioTrack_ref);
        //LOGV("deleting pJniStorage: %x\n", (Int32)pJniStorage);
        delete pJniStorage;
    }
}

// ----------------------------------------------------------------------------
static void android_media_AudioTrack_native_release(JNIEnv *env,  jobject thiz) {
       
    // do everything a call to finalize would
    android_media_AudioTrack_native_finalize(env, thiz);
    // + reset the native resources in the Java object so any attempt to access
    // them after a call to release fails.
    env->SetIntField(thiz, javaAudioTrackFields.nativeTrackInJavaObj, 0);
    env->SetIntField(thiz, javaAudioTrackFields.jniData, 0);
}


// ----------------------------------------------------------------------------
jint writeToTrack(AudioTrack* pTrack, jint audioFormat, jbyte* data,
                  jint offsetInBytes, jint sizeInBytes) {
    // give the data to the native AudioTrack object (the data starts at the offset)
    ssize_t written = 0;
    // regular write() or copy the data to the AudioTrack's shared memory?
    if (pTrack->sharedBuffer() == 0) {
        written = pTrack->write(data + offsetInBytes, sizeInBytes);
    } else {
        if (audioFormat == javaAudioTrackFields.PCM16) {
            // writing to shared memory, check for capacity
            if ((size_t)sizeInBytes > pTrack->sharedBuffer()->size()) {
                sizeInBytes = pTrack->sharedBuffer()->size();
            }
            memcpy(pTrack->sharedBuffer()->pointer(), data + offsetInBytes, sizeInBytes);
            written = sizeInBytes;
        } else if (audioFormat == javaAudioTrackFields.PCM8) {
            // data contains 8bit data we need to expand to 16bit before copying
            // to the shared memory
            // writing to shared memory, check for capacity,
            // note that input data will occupy 2X the input space due to 8 to 16bit conversion
            if (((size_t)sizeInBytes)*2 > pTrack->sharedBuffer()->size()) {
                sizeInBytes = pTrack->sharedBuffer()->size() / 2;
            }
            Int32 count = sizeInBytes;
            int16_t *dst = (int16_t *)pTrack->sharedBuffer()->pointer();
            const int8_t *src = (const int8_t *)(data + offsetInBytes);
            while(count--) {
                *dst++ = (int16_t)(*src++^0x80) << 8;
            }
            // even though we wrote 2*sizeInBytes, we only report sizeInBytes as written to hide
            // the 8bit mixer restriction from the user of this function
            written = sizeInBytes;
        }
    }
    return written;

}

// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_native_write(JNIEnv *env,  jobject thiz,
                                                  jbyteArray javaAudioData,
                                                  jint offsetInBytes, jint sizeInBytes,
                                                  jint javaAudioFormat) {
    jbyte* cAudioData = NULL;
    AudioTrack *lpTrack = NULL;
    //LOGV("android_media_AudioTrack_native_write(offset=%d, sizeInBytes=%d) called",
    //    offsetInBytes, sizeInBytes);
    
    // get the audio track to load with samples
    lpTrack = (AudioTrack *)env->GetIntField(thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for write()");
        return 0;
    }

    // get the pointer for the audio data from the java array
    if (javaAudioData) {
        cAudioData = (jbyte *)env->GetPrimitiveArrayCritical(javaAudioData, NULL);
        if (cAudioData == NULL) {
            LOGE("Error retrieving source of audio data to play, can't play");
            return 0; // out of memory or no data to load
        }
    } else {
        LOGE("NULL java array of audio data to play, can't play");
        return 0;
    }

    jint written = writeToTrack(lpTrack, javaAudioFormat, cAudioData, offsetInBytes, sizeInBytes);

    env->ReleasePrimitiveArrayCritical(javaAudioData, cAudioData, 0);

    //LOGV("write wrote %d (tried %d) bytes in the native AudioTrack with offset %d",
    //     (Int32)written, (Int32)(sizeInBytes), (Int32)offsetInBytes);
    return written;
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_native_write_short(JNIEnv *env,  jobject thiz,
                                                  jshortArray javaAudioData,
                                                  jint offsetInShorts, jint sizeInShorts,
                                                  jint javaAudioFormat) {
    return (android_media_AudioTrack_native_write(env, thiz,
                                                 (jbyteArray) javaAudioData,
                                                 offsetInShorts*2, sizeInShorts*2,
                                                 javaAudioFormat)
            / 2);
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_get_native_frame_count(JNIEnv *env,  jobject thiz) {
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);

    if (lpTrack) {
        return lpTrack->frameCount();
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for frameCount()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_set_playback_rate(JNIEnv *env,  jobject thiz,
        jint sampleRateInHz) {
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);

    if (lpTrack) {
        return android_media_translateErrorCode(lpTrack->setSampleRate(sampleRateInHz));
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for setSampleRate()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_get_playback_rate(JNIEnv *env,  jobject thiz) {
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);

    if (lpTrack) {
        return (jint) lpTrack->getSampleRate();   
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for getSampleRate()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_set_marker_pos(JNIEnv *env,  jobject thiz, 
        jint markerPos) {
            
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
                
    if (lpTrack) {
        return android_media_translateErrorCode( lpTrack->setMarkerPosition(markerPos) );   
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for setMarkerPosition()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_get_marker_pos(JNIEnv *env,  jobject thiz) {
    
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    uint32_t markerPos = 0;
                
    if (lpTrack) {
        lpTrack->getMarkerPosition(&markerPos);
        return (jint)markerPos;
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for getMarkerPosition()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_set_pos_update_period(JNIEnv *env,  jobject thiz,
        jint period) {
            
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
                
    if (lpTrack) {
        return android_media_translateErrorCode( lpTrack->setPositionUpdatePeriod(period) );   
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for setPositionUpdatePeriod()");
        return AUDIOTRACK_ERROR;
    }            
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_get_pos_update_period(JNIEnv *env,  jobject thiz) {
    
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    uint32_t period = 0;
                
    if (lpTrack) {
        lpTrack->getPositionUpdatePeriod(&period);
        return (jint)period;
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for getPositionUpdatePeriod()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_set_position(JNIEnv *env,  jobject thiz, 
        jint position) {
            
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
                
    if (lpTrack) {
        return android_media_translateErrorCode( lpTrack->setPosition(position) );
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for setPosition()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_get_position(JNIEnv *env,  jobject thiz) {
    
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    uint32_t position = 0;
                
    if (lpTrack) {
        lpTrack->getPosition(&position);
        return (jint)position;
    }  else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for getPosition()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_set_loop(JNIEnv *env,  jobject thiz,
        jint loopStart, jint loopEnd, jint loopCount) {

     AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
     if (lpTrack) {
        return android_media_translateErrorCode( lpTrack->setLoop(loopStart, loopEnd, loopCount) );
     }  else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for setLoop()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_reload(JNIEnv *env,  jobject thiz) {

     AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);
     if (lpTrack) {
        return android_media_translateErrorCode( lpTrack->reload() );
     } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for reload()");
        return AUDIOTRACK_ERROR;
    }
}


// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_get_output_sample_rate(JNIEnv *env,  jobject thiz,
        jint javaStreamType) {
    Int32 afSamplingRate;
    // convert the stream type from Java to native value
    // FIXME: code duplication with android_media_AudioTrack_native_setup()
    AudioSystem::stream_type nativeStreamType;
    if (javaStreamType == javaAudioTrackFields.STREAM_VOICE_CALL) {
        nativeStreamType = AudioSystem::VOICE_CALL;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_SYSTEM) {
        nativeStreamType = AudioSystem::SYSTEM;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_RING) {
        nativeStreamType = AudioSystem::RING;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_MUSIC) {
        nativeStreamType = AudioSystem::MUSIC;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_ALARM) {
        nativeStreamType = AudioSystem::ALARM;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_NOTIFICATION) {
        nativeStreamType = AudioSystem::NOTIFICATION;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_BLUETOOTH_SCO) {
        nativeStreamType = AudioSystem::BLUETOOTH_SCO;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_DTMF) {
        nativeStreamType = AudioSystem::DTMF;
    } else {
        nativeStreamType = AudioSystem::DEFAULT;
    }

    if (AudioSystem::getOutputSamplingRate(&afSamplingRate, nativeStreamType) != NO_ERROR) {
        LOGE("AudioSystem::getOutputSamplingRate() for stream type %d failed in AudioTrack JNI",
            nativeStreamType);
        return DEFAULT_OUTPUT_SAMPLE_RATE;
    } else {
        return afSamplingRate;
    }
}


// ----------------------------------------------------------------------------
// returns the minimum required size for the successful creation of a streaming AudioTrack
// returns -1 if there was an error querying the hardware.
static jint android_media_AudioTrack_get_min_buff_size(JNIEnv *env,  jobject thiz,
    jint sampleRateInHertz, jint nbChannels, jint audioFormat) {

    Int32 frameCount = 0;
    if (AudioTrack::getMinFrameCount(&frameCount, AudioSystem::DEFAULT,
            sampleRateInHertz) != NO_ERROR) {
        return -1;
    }
    return frameCount * nbChannels * (audioFormat == javaAudioTrackFields.PCM16 ? 2 : 1);
}

// ----------------------------------------------------------------------------
static void
android_media_AudioTrack_setAuxEffectSendLevel(JNIEnv *env, jobject thiz, jfloat level )
{
    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
        thiz, javaAudioTrackFields.nativeTrackInJavaObj);
    if (lpTrack == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for setAuxEffectSendLevel()");
        return;
    }

    lpTrack->setAuxEffectSendLevel(level);
}

// ----------------------------------------------------------------------------
static jint android_media_AudioTrack_attachAuxEffect(JNIEnv *env,  jobject thiz,
        jint effectId) {

    AudioTrack *lpTrack = (AudioTrack *)env->GetIntField(
                thiz, javaAudioTrackFields.nativeTrackInJavaObj);

    if (lpTrack) {
        return android_media_translateErrorCode( lpTrack->attachAuxEffect(effectId) );
    } else {
        jniThrowException(env, "java/lang/IllegalStateException",
            "Unable to retrieve AudioTrack pointer for attachAuxEffect()");
        return AUDIOTRACK_ERROR;
    }
}
//---------------------------------------------------------
// Native methods called from the Java side
//--------------------

private native final Int32 native_setup(Object audiotrack_this,
        Int32 streamType, Int32 sampleRate, Int32 nbChannels, Int32 audioFormat,
        Int32 buffSizeInBytes, Int32 mode, Int32[] sessionId);

private native final void native_finalize();

private native final void native_release();

private native final void native_start();

private native final void native_stop();

private native final void native_pause();

private native final void native_flush();

private native final Int32 native_write_byte(byte[] audioData,
                                           Int32 offsetInBytes, Int32 sizeInBytes, Int32 format);

private native final Int32 native_write_short(short[] audioData,
                                            Int32 offsetInShorts, Int32 sizeInShorts, Int32 format);

private native final Int32 native_reload_static();

private native final Int32 native_get_native_frame_count();

private native final void native_setVolume(Float leftVolume, Float rightVolume);

private native final Int32 native_set_playback_rate(Int32 sampleRateInHz);
private native final Int32 native_get_playback_rate();

private native final Int32 native_set_marker_pos(Int32 marker);
private native final Int32 native_get_marker_pos();

private native final Int32 native_set_pos_update_period(Int32 updatePeriod);
private native final Int32 native_get_pos_update_period();

private native final Int32 native_set_position(Int32 position);
private native final Int32 native_get_position();

private native final Int32 native_set_loop(Int32 start, Int32 end, Int32 loopCount);

static private native final Int32 native_get_output_sample_rate(Int32 streamType);
static private native final Int32 native_get_min_buff_size(
        Int32 sampleRateInHz, Int32 channelConfig, Int32 audioFormat);

private native final Int32 native_get_session_id();

private native final Int32 native_attachAuxEffect(Int32 effectId);
private native final void native_setAuxEffectSendLevel(Float level);

#endif
//---------------------------------------------------------
// Utility methods
//------------------

void AudioTrack::Logd(
    /* [in] */ String msg)
{
    //Log.d(TAG, "[ android.media.AudioTrack ] " + msg);
}

void AudioTrack::Loge(
    /* [in] */ String msg)
{
    //Log.e(TAG, "[ android.media.AudioTrack ] " + msg);
}

