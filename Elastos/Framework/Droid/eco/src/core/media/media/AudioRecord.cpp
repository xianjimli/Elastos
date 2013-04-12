
#include "media/AudioRecord.h"
#include "media/CMediaRecorder.h"

AudioRecord::AudioRecord()
{

}

//---------------------------------------------------------
// Constructor, Finalize
//--------------------
/**
 * Class constructor.
 * @param audioSource the recording source. See {@link MediaRecorder.AudioSource} for
 *    recording source definitions.
 * @param sampleRateInHz the sample rate expressed in Hertz. Examples of rates are (but
 *   not limited to) 44100, 22050 and 11025.
 * @param channelConfig describes the configuration of the audio channels.
 *   See {@link AudioFormat#CHANNEL_IN_MONO} and
 *   {@link AudioFormat#CHANNEL_IN_STEREO}
 * @param audioFormat the format in which the audio data is represented.
 *   See {@link AudioFormat#ENCODING_PCM_16BIT} and
 *   {@link AudioFormat#ENCODING_PCM_8BIT}
 * @param bufferSizeInBytes the total size (in bytes) of the buffer where audio data is written
 *   to during the recording. New audio data can be read from this buffer in smaller chunks
 *   than this size. See {@link #getMinBufferSize(Int32, Int32, Int32)} to determine the minimum
 *   required buffer size for the successful creation of an AudioRecord instance. Using values
 *   smaller than getMinBufferSize() will result in an initialization failure.
 * @throws java.lang.IllegalArgumentException
 */
AudioRecord::AudioRecord(
    /* [in] */ Int32 audioSource,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes)
{
    mState = STATE_UNINITIALIZED;
    mRecordingState = RECORDSTATE_STOPPED;

    // remember which looper is associated with the AudioRecord instanciation
    /*if ((mInitializationLooper = Looper.myLooper()) == NULL) {
        mInitializationLooper = Looper.getMainLooper();
    }*/

    AudioParamCheck(audioSource, sampleRateInHz, channelConfig, audioFormat);

    AudioBuffSizeCheck(bufferSizeInBytes);

    // native initialization
    //TODO: update native initialization when information about hardware init failure
    //      due to capture device already open is available.
    Int32 initResult;/* = native_setup(new WeakReference<AudioRecord>(this),
            mRecordSource, mSampleRate, mChannels, mAudioFormat, mNativeBufferSizeInBytes);*/
    if (initResult != SUCCESS) {
        //loge("Error code "+initResult+" when initializing native AudioRecord object.");
        return; // with mState == STATE_UNINITIALIZED
    }

    mState = STATE_INITIALIZED;
}


// Convenience method for the constructor's parameter checks.
// This is where constructor IllegalArgumentException-s are thrown
// postconditions:
//    mRecordSource is valid
//    mChannelCount is valid
//    mChannels is valid
//    mAudioFormat is valid
//    mSampleRate is valid
void AudioRecord::AudioParamCheck(
    /* [in] */ Int32 audioSource,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat)
{

    //--------------
    // audio source
//    if ( (audioSource < MediaRecorder::AudioSource::DEFAULT) ||
//        (audioSource > MediaRecorder::AudioSource::VOICE_COMMUNICATION) )  {
//            //(audioSource > MediaRecorder.getAudioSourceMax()) )  {
//        //throw (new IllegalArgumentException("Invalid audio source."));
//    } else {
//        mRecordSource = audioSource;
//    }

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
    case AudioFormat_CHANNEL_IN_DEFAULT: // AudioFormat.CHANNEL_CONFIGURATION_DEFAULT
    case AudioFormat_CHANNEL_IN_MONO:
    case AudioFormat_CHANNEL_CONFIGURATION_MONO:
        mChannelCount = 1;
        mChannels = AudioFormat_CHANNEL_IN_MONO;
        break;
    case AudioFormat_CHANNEL_IN_STEREO:
    case AudioFormat_CHANNEL_CONFIGURATION_STEREO:
        mChannelCount = 2;
        mChannels = AudioFormat_CHANNEL_IN_STEREO;
        break;
    default:
        mChannelCount = 0;
        mChannels = AudioFormat_CHANNEL_INVALID;
        mChannelConfiguration = AudioFormat_CHANNEL_INVALID;
        //throw (new IllegalArgumentException("Unsupported channel configuration."));
    }

    //--------------
    // audio format
    switch (audioFormat) {
    case AudioFormat_ENCODING_DEFAULT:
        mAudioFormat = AudioFormat_ENCODING_PCM_16BIT;
        break;
    case AudioFormat_ENCODING_PCM_16BIT:
    case AudioFormat_ENCODING_PCM_8BIT:
        mAudioFormat = audioFormat;
        break;
    default:
        mAudioFormat = AudioFormat_ENCODING_INVALID;
    /*throw (new IllegalArgumentException("Unsupported sample encoding."
            + " Should be ENCODING_PCM_8BIT or ENCODING_PCM_16BIT."));*/
    }
}


// Convenience method for the contructor's audio buffer size check.
// preconditions:
//    mChannelCount is valid
//    mAudioFormat is AudioFormat.ENCODING_PCM_8BIT OR AudioFormat.ENCODING_PCM_16BIT
// postcondition:
//    mNativeBufferSizeInBytes is valid (multiple of frame size, positive)
void AudioRecord::AudioBuffSizeCheck(
    /* [in] */ Int32 audioBufferSize)
{
    // NB: this section is only valid with PCM data.
    // To update when supporting compressed formats
    Int32 frameSizeInBytes = mChannelCount
        * (mAudioFormat == AudioFormat_ENCODING_PCM_8BIT ? 1 : 2);
    if ((audioBufferSize % frameSizeInBytes != 0) || (audioBufferSize < 1)) {
        //throw (new IllegalArgumentException("Invalid audio buffer size."));
    }

    mNativeBufferSizeInBytes = audioBufferSize;
}

/**
 * Releases the native AudioRecord resources.
 * The object can no longer be used and the reference should be set to NULL
 * after a call to release()
 */
ECode AudioRecord::ReleaseEx()
{
    //try {
        Stop();
    //} catch(IllegalStateException ise) {
        // don't raise an exception, we're releasing the resources.
    //}
    native_release();
    mState = STATE_UNINITIALIZED;

    return NOERROR;
}


void AudioRecord::Finalize()
{
    native_finalize();
}


//--------------------------------------------------------------------------
// Getters
//--------------------
/**
 * Returns the configured audio data sample rate in Hz
 */
Int32 AudioRecord::GetSampleRate()
{
    return mSampleRate;
}

/**
 * Returns the audio recording source.
 * @see MediaRecorder.AudioSource
 */
Int32 AudioRecord::GetAudioSource()
{
    return mRecordSource;
}

/**
 * Returns the configured audio data format. See {@link AudioFormat#ENCODING_PCM_16BIT}
 * and {@link AudioFormat#ENCODING_PCM_8BIT}.
 */
Int32 AudioRecord::GetAudioFormat()
{
    return mAudioFormat;
}

/**
 * Returns the configured channel configuration.
 * See {@link AudioFormat#CHANNEL_IN_MONO}
 * and {@link AudioFormat#CHANNEL_IN_STEREO}.
 */
Int32 AudioRecord::GetChannelConfiguration()
{
    return mChannelConfiguration;
}

/**
 * Returns the configured number of channels.
 */
Int32 AudioRecord::GetChannelCount()
{
    return mChannelCount;
}

/**
 * Returns the state of the AudioRecord instance. This is useful after the
 * AudioRecord instance has been created to check if it was initialized
 * properly. This ensures that the appropriate hardware resources have been
 * acquired.
 * @see AudioRecord#STATE_INITIALIZED
 * @see AudioRecord#STATE_UNINITIALIZED
 */
Int32 AudioRecord::GetState()
{
    return mState;
}

/**
 * Returns the recording state of the AudioRecord instance.
 * @see AudioRecord#RECORDSTATE_STOPPED
 * @see AudioRecord#RECORDSTATE_RECORDING
 */
Int32 AudioRecord::GetRecordingState()
{
    return mRecordingState;
}

/**
 * Returns the notification marker position expressed in frames.
 */
Int32 AudioRecord::GetNotificationMarkerPosition()
{
    return native_get_marker_pos();
}

/**
 * Returns the notification update period expressed in frames.
 */
Int32 AudioRecord::GetPositionNotificationPeriod()
{
    return native_get_pos_update_period();
}

/**
 * Returns the minimum buffer size required for the successful creation of an AudioRecord
 * object.
 * Note that this size doesn't guarantee a smooth recording under load, and higher values
 * should be chosen according to the expected frequency at which the AudioRecord instance
 * will be polled for new data.
 * @param sampleRateInHz the sample rate expressed in Hertz.
 * @param channelConfig describes the configuration of the audio channels.
 *   See {@link AudioFormat#CHANNEL_IN_MONO} and
 *   {@link AudioFormat#CHANNEL_IN_STEREO}
 * @param audioFormat the format in which the audio data is represented.
 *   See {@link AudioFormat#ENCODING_PCM_16BIT}.
 * @return {@link #ERROR_BAD_VALUE} if the recording parameters are not supported by the
 *  hardware, or an invalid parameter was passed,
 *  or {@link #ERROR} if the implementation was unable to query the hardware for its
 *  output properties,
 *   or the minimum buffer size expressed in bytes.
 */
Int32 AudioRecord::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat)
{
    Int32 channelCount = 0;
    switch(channelConfig) {
    case AudioFormat_CHANNEL_IN_DEFAULT: // AudioFormat.CHANNEL_CONFIGURATION_DEFAULT
    case AudioFormat_CHANNEL_IN_MONO:
    case AudioFormat_CHANNEL_CONFIGURATION_MONO:
        channelCount = 1;
        break;
    case AudioFormat_CHANNEL_IN_STEREO:
    case AudioFormat_CHANNEL_CONFIGURATION_STEREO:
        channelCount = 2;
        break;
    case AudioFormat_CHANNEL_INVALID:
    default:
        //loge("getMinBufferSize(): Invalid channel configuration.");
        return AudioRecord::ERROR_BAD_VALUE;
    }

    // PCM_8BIT is not supported at the moment
    if (audioFormat != AudioFormat_ENCODING_PCM_16BIT) {
        //loge("getMinBufferSize(): Invalid audio format.");
        return AudioRecord::ERROR_BAD_VALUE;
    }

    Int32 size = native_get_min_buff_size(sampleRateInHz, channelCount, audioFormat);
    if (size == 0) {
        return AudioRecord::ERROR_BAD_VALUE;
    }
    else if (size == -1) {
        return AudioRecord::ERROR;
    }
    else {
        return size;
    }
}


//---------------------------------------------------------
// Transport control methods
//--------------------
/**
 * Starts recording from the AudioRecord instance.
 * @throws IllegalStateException
 */
ECode AudioRecord::StartRecording()
{
    if (mState != STATE_INITIALIZED) {
        /*throw(new IllegalStateException("startRecording() called on an "
                +"uninitialized AudioRecord."));*/
    }

    // start recording
    //synchronized(mRecordingStateLock) {
        if (native_start() == SUCCESS) {
            mRecordingState = RECORDSTATE_RECORDING;
        }
    //}

    return NOERROR;
}



/**
 * Stops recording.
 * @throws IllegalStateException
 */
ECode AudioRecord::Stop()
{
    if (mState != STATE_INITIALIZED) {
        //throw(new IllegalStateException("stop() called on an uninitialized AudioRecord."));
    }

    // stop recording
    //synchronized(mRecordingStateLock) {
        native_stop();
        mRecordingState = RECORDSTATE_STOPPED;
    //}

    return NOERROR;
}


//---------------------------------------------------------
// Audio data supply
//--------------------
/**
 * Reads audio data from the audio hardware for recording into a buffer.
 * @param audioData the array to which the recorded audio data is written.
 * @param offsetInBytes index in audioData from which the data is written expressed in bytes.
 * @param sizeInBytes the number of requested bytes.
 * @return the number of bytes that were read or or {@link #ERROR_INVALID_OPERATION}
 *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
 *    the parameters don't resolve to valid data and indexes.
 *    The number of bytes will not exceed sizeInBytes.
 */
Int32 AudioRecord::Read(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    if ( (audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0)
            || (offsetInBytes + sizeInBytes > audioData->GetLength())) {
        return ERROR_BAD_VALUE;
    }

    return native_read_in_byte_array(audioData, offsetInBytes, sizeInBytes);
}


/**
 * Reads audio data from the audio hardware for recording into a buffer.
 * @param audioData the array to which the recorded audio data is written.
 * @param offsetInShorts index in audioData from which the data is written expressed in shorts.
 * @param sizeInShorts the number of requested shorts.
 * @return the number of shorts that were read or or {@link #ERROR_INVALID_OPERATION}
 *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
 *    the parameters don't resolve to valid data and indexes.
 *    The number of shorts will not exceed sizeInShorts.
 */
Int32 AudioRecord::Read(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    if ( (audioData == NULL) || (offsetInShorts < 0 ) || (sizeInShorts < 0)
            || (offsetInShorts + sizeInShorts > audioData->GetLength())) {
        return ERROR_BAD_VALUE;
    }

    return native_read_in_short_array(audioData, offsetInShorts, sizeInShorts);
}


/**
 * Reads audio data from the audio hardware for recording into a direct buffer. If this buffer
 * is not a direct buffer, this method will always return 0.
 * @param audioBuffer the direct buffer to which the recorded audio data is written.
 * @param sizeInBytes the number of requested bytes.
 * @return the number of bytes that were read or or {@link #ERROR_INVALID_OPERATION}
 *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
 *    the parameters don't resolve to valid data and indexes.
 *    The number of bytes will not exceed sizeInBytes.
 */
//Int32 AudioRecord::Read(
//    /* [in] */ ByteBuffer* audioBuffer,
//    /* [in] */ Int32 sizeInBytes)
//{
//    if (mState != STATE_INITIALIZED) {
//        return ERROR_INVALID_OPERATION;
//    }
//
//    if ( (audioBuffer == NULL) || (sizeInBytes < 0) ) {
//        return ERROR_BAD_VALUE;
//    }
//
//    return native_read_in_direct_buffer(audioBuffer, sizeInBytes);
//}


//--------------------------------------------------------------------------
// Initialization / configuration
//--------------------
/**
 * Sets the listener the AudioRecord notifies when a previously set marker is reached or
 * for each periodic record head position update.
 * @param listener
 */
ECode AudioRecord::SetRecordPositionUpdateListener(
    /* [in] */ IOnRecordPositionUpdateListener* listener)
{
    SetRecordPositionUpdateListener(listener, NULL);

    return NOERROR;
}

/**
 * Sets the listener the AudioRecord notifies when a previously set marker is reached or
 * for each periodic record head position update.
 * Use this method to receive AudioRecord events in the Handler associated with another
 * thread than the one in which you created the AudioTrack instance.
 * @param listener
 * @param handler the Handler that will receive the event notification messages.
 */
ECode AudioRecord::SetRecordPositionUpdateListener(
    /* [in] */ IOnRecordPositionUpdateListener* listener,
    /* [in] */ IHandler* handler)
{
    //synchronized (mPositionListenerLock) {

        mPositionListener = listener;

        //if (listener != NULL) {
        //    if (handler != NULL) {
        //        mEventHandler = new NativeEventHandler(this, handler.getLooper());
        //    } else {
        //        // no given handler, use the looper the AudioRecord was created in
        //        mEventHandler = new NativeEventHandler(this, mInitializationLooper);
        //    }
        //} else {
        //    mEventHandler = NULL;
        //}
    //}

    return NOERROR;

}


/**
 * Sets the marker position at which the listener is called, if set with
 * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)} or
 * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)}.
 * @param markerInFrames marker position expressed in frames
 * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
 *  {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioRecord::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames)
{
    return native_set_marker_pos(markerInFrames);
}


/**
 * Sets the period at which the listener is called, if set with
 * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)} or
 * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)}.
 * @param periodInFrames update period expressed in frames
 * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_INVALID_OPERATION}
 */
Int32 AudioRecord::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames)
{
    return native_set_pos_update_period(periodInFrames);
}

//AudioRecord::NativeEventHandler::NativeEventHandler(
//    /* [in] */ AudioRecord* recorder,
//    /* [in] */ ILooper* looper)
//{
//    super(looper);
//    mAudioRecord = recorder;
//}

ECode AudioRecord::NativeEventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IOnRecordPositionUpdateListener> listener = NULL;
    //synchronized (mPositionListenerLock) {
        listener = mAudioRecord->mPositionListener;
    //}

    //switch(msg.what) {
    //case NATIVE_EVENT_MARKER:
    //    if (listener != NULL) {
    //        listener->OnMarkerReached(mAudioRecord);
    //    }
    //    break;
    //case NATIVE_EVENT_NEW_POS:
    //    if (listener != NULL) {
    //        listener->OnPeriodicNotification(mAudioRecord);
    //    }
    //    break;
    //default:
    //    /*Log.e(TAG, "[ android.media.AudioRecord.NativeEventHandler ] " +
    //            "Unknown event type: " + msg.what);*/
    //break;
    //}

    return NOERROR;
}

PInterface AudioRecord::NativeEventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AudioRecord::NativeEventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioRecord::NativeEventHandler::Release()
{
    return ElRefBase::Release();
}

ECode AudioRecord::NativeEventHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

//---------------------------------------------------------
// Java methods called from the native side
//--------------------
void AudioRecord::PostEventFromNative(
    /* [in] */ IInterface* audiorecord_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    //logd("Event posted from the native side: event="+ what + " args="+ arg1+" "+arg2);
    /*AudioRecord recorder = (AudioRecord)((WeakReference)audiorecord_ref).get();
    if (recorder == NULL) {
        return;
    }

    if (recorder.mEventHandler != NULL) {
        Message m =
            recorder.mEventHandler.obtainMessage(what, arg1, arg2, obj);
        recorder.mEventHandler.sendMessage(m);
    }*/

}


//---------------------------------------------------------
// Native methods called from the Java side
//--------------------

Int32 AudioRecord::native_setup(
    /* [in] */ IInterface* audiorecord_this,
    /* [in] */ Int32 recordSource,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 nbChannels,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 buffSizeInBytes)
{
    return 0;
}

void AudioRecord::native_finalize()
{

}

void AudioRecord::native_release()
{

}

Int32 AudioRecord::native_start()
{
    return 0;
}

void AudioRecord::native_stop()
{

}

Int32 AudioRecord::native_read_in_byte_array(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes)
{
    return 0;
}

Int32 AudioRecord::native_read_in_short_array(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts)
{
    return 0;
}

Int32 AudioRecord::native_read_in_direct_buffer(
    /* [in] */ IInterface* jBuffer,
    /* [in] */ Int32 sizeInBytes)
{
    return 0;
}

Int32 AudioRecord::native_set_marker_pos(
    /* [in] */ Int32 marker)
{
    return 0;
}

Int32 AudioRecord::native_get_marker_pos()
{
    return 0;
}

Int32 AudioRecord::native_set_pos_update_period(
    /* [in] */ Int32 updatePeriod)
{
    return 0;
}

Int32 AudioRecord::native_get_pos_update_period()
{
    return 0;
}

Int32 AudioRecord::native_get_min_buff_size(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelCount,
    /* [in] */ Int32 audioFormat)
{
    return 0;
}


//---------------------------------------------------------
// Utility methods
//------------------

void AudioRecord::Logd(
    /* [in] */ String msg)
{
    //Log.d(TAG, "[ android.media.AudioRecord ] " + msg);
}

void AudioRecord::Loge(
    /* [in] */ String msg)
{
    //Log.e(TAG, "[ android.media.AudioRecord ] " + msg);
}

