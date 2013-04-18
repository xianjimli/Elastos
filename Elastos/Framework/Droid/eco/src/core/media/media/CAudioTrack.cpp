
#include "media/CAudioTrack.h"
#include "media/ElAudioSystem.h"

Int32 CAudioTrack::NativeSetup(
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


Int32 CAudioTrack::NativeGetPlaybackRate()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return (Int32) lpTrack->getSampleRate();   
}

Int32 CAudioTrack::NativeGetMarkerPos()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getMarkerPosition(&position);
    return (Int32)position; 
}

Int32 CAudioTrack::NativeGetPosUpdatePeriod()
{
    uint32_t period = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPositionUpdatePeriod(&period);
    return (Int32)period; 
}

Int32 CAudioTrack::NativeGetPosition()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPosition(&position);
    return (Int32)position; 
}

Int32 CAudioTrack::NativeGetOutputSampleRate(
   /* [in] */ Int32 javaStreamType)
{
    fields_t javaAudioTrackFields;
    Int32 DEFAULT_OUTPUT_SAMPLE_RATE = 44100;

    Int32 afSamplingRate;
    // convert the stream type from Java to native value
    // FIXME: code duplication with android_media_AudioTrack_native_setup()
    android::AudioSystem::stream_type nativeStreamType;
    if (javaStreamType == javaAudioTrackFields.STREAM_VOICE_CALL) {
        nativeStreamType = android::AudioSystem::VOICE_CALL;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_SYSTEM) {
        nativeStreamType = android::AudioSystem::SYSTEM;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_RING) {
        nativeStreamType = android::AudioSystem::RING;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_MUSIC) {
        nativeStreamType = android::AudioSystem::MUSIC;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_ALARM) {
        nativeStreamType = android::AudioSystem::ALARM;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_NOTIFICATION) {
        nativeStreamType = android::AudioSystem::NOTIFICATION;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_BLUETOOTH_SCO) {
        nativeStreamType = android::AudioSystem::BLUETOOTH_SCO;
    } else if (javaStreamType == javaAudioTrackFields.STREAM_DTMF) {
        nativeStreamType = android::AudioSystem::DTMF;
    } else {
        nativeStreamType = android::AudioSystem::DEFAULT;
    }

    if (android::AudioSystem::getOutputSamplingRate(&afSamplingRate, nativeStreamType) != android::NO_ERROR) {
        //LOGE("AudioSystem::getOutputSamplingRate() for stream type %d failed in AudioTrack JNI",nativeStreamType);
        return DEFAULT_OUTPUT_SAMPLE_RATE;
    } else {
        return afSamplingRate;
    }
}

// returns the minimum required size for the successful creation of a streaming AudioTrack
// returns -1 if there was an error querying the hardware.
Int32 CAudioTrack::NativeGetMinBuffSize(
    /* [in] */ Int32 sampleRateInHertz, 
    /* [in] */ Int32 nbChannels,
    /* [in] */ Int32 audioFormat) 
{
    fields_t javaAudioTrackFields;
    Int32 frameCount = 0;
    if (android::AudioTrack::getMinFrameCount(&frameCount, android::AudioSystem::DEFAULT,sampleRateInHertz) != android::NO_ERROR) {
        return -1;
    }
    return frameCount * nbChannels * (audioFormat == javaAudioTrackFields.PCM16 ? 2 : 1);
}

void CAudioTrack::NativeSetVolume(
    /* [in] */ Float left, 
    /* [in] */ Float right)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    lpTrack->setVolume(left, right);
}

Int32 CAudioTrack::AndroidMediaTranslateErrorCode(
    /* [in] */ Int32 code) 
{
    Int32 AUDIOTRACK_SUCCESS                        = 0;
    Int32 AUDIOTRACK_ERROR                           =-1;
    Int32 AUDIOTRACK_ERROR_BAD_VALUE                 =-2;
    Int32 AUDIOTRACK_ERROR_INVALID_OPERATION         =-3;

    switch(code) {
    case 0://case NO_ERROR
        return AUDIOTRACK_SUCCESS;
    case -22://case BAD_VALUE
        return AUDIOTRACK_ERROR_BAD_VALUE;
    case -38://case INVALID_OPERATION
        return AUDIOTRACK_ERROR_INVALID_OPERATION;
    default:
        return AUDIOTRACK_ERROR;
    }   
}

Int32 CAudioTrack::NativeSetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->setSampleRate(sampleRateInHz));
}

Int32 CAudioTrack::NativeSetMarkerPos(
    /* [in] */ Int32 markerPos)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    
    return AndroidMediaTranslateErrorCode( lpTrack->setMarkerPosition(markerPos) );   
}

Int32 CAudioTrack::NativeSetPosUpdatePeriod(
    /* [in] */ Int32 periodInFrames)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode( lpTrack->setPositionUpdatePeriod(periodInFrames) );   
}

Int32 CAudioTrack::NativeSetPosition(
    /* [in] */ Int32 positionInFrames)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode( lpTrack->setPosition(positionInFrames) );
}

Int32 CAudioTrack::NativeSetLoop(
    /* [in] */ Int32 loopStart, 
    /* [in] */ Int32 loopEnd, 
    /* [in] */ Int32 loopCount)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    return AndroidMediaTranslateErrorCode( lpTrack->setLoop(loopStart, loopEnd, loopCount) );
}

void CAudioTrack::NativeStart()
{    
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->start();
}

void CAudioTrack::NativeStop()
{    
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->stop();
}

void CAudioTrack::NativePause()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->pause();
}

void CAudioTrack::NativeFlush()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->flush();
}


Int32 CAudioTrack::WriteToTrack(
    /* [in] */ android::AudioTrack* pTrack, 
    /* [in] */ Int32 audioFormat, 
    /* [in] */ Byte* data,
    /* [in] */ Int32 offsetInBytes, 
    /* [in] */ Int32 sizeInBytes) 
{
    fields_t javaAudioTrackFields;
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

Int32 CAudioTrack::NativeWriteByte(    
        /* [in] */ ArrayOf<Byte>* javaAudioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [in] */ Int32 javaAudioFormat) 
{
    
    Byte* cAudioData = NULL;

    android::AudioTrack *lpTrack = NULL;

    //LOGV("android_media_AudioTrack_native_write(offset=%d, sizeInBytes=%d) called",offsetInBytes, sizeInBytes);
    // get the pointer for the audio data from the java array
    
    if (javaAudioData) {
         cAudioData = javaAudioData->GetPayload();
        if (cAudioData == NULL) {
            //LOGE("Error retrieving source of audio data to play, can't play");
            return 0; // out of memory or no data to load
        }
    } else {
        //LOGE("NULL java array of audio data to play, can't play");
        return 0;
    }
    

    Int32 written = WriteToTrack(lpTrack, javaAudioFormat, cAudioData, offsetInBytes, sizeInBytes);
    return written;
}

Int32 CAudioTrack::NativeWriteShort(
    /* [in] */ ArrayOf<Int16>* javaAudioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [in] */ Int32 javaAudioFormat) 
{
    return (NativeWriteByte((ArrayOf<Byte>*) javaAudioData, offsetInShorts*2, sizeInShorts*2, javaAudioFormat)/ 2);
}

Int32 CAudioTrack::NativeReloadStatic()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode( lpTrack->reload()  );   
}

Int32 CAudioTrack::NativeAttachAuxEffect(Int32 effectId)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode( lpTrack->attachAuxEffect(effectId)  );  
}

void CAudioTrack::NativeSetAuxEffectSendLevel(
    /* [in] */Float level)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->setAuxEffectSendLevel(level);
}

void CAudioTrack::NativeFinalize()
{
    //LOGV("android_media_AudioTrack_native_finalize jobject: %x\n", (int)thiz);
       
    // delete the AudioTrack object
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    //LOGV("deleting lpTrack: %x\n", (int)lpTrack);
    lpTrack->stop();
    delete lpTrack;
}

void CAudioTrack::NativeRelease()
{
    // do everything a call to finalize would
    NativeFinalize();
}


void CAudioTrack::Init(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 sessionId)
{
    mState = AudioTrack_STATE_UNINITIALIZED;

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
    if (initResult != AudioTrack_SUCCESS) {
        //loge("Error code "+initResult+" when initializing AudioTrack.");
        return; // with mState == STATE_UNINITIALIZED
    }

    mSessionId = session[0];

    if (mDataLoadMode == AudioTrack_MODE_STATIC) {
        mState = AudioTrack_STATE_NO_STATIC_DATA;
    } else {
        mState = AudioTrack_STATE_INITIALIZED;
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
void CAudioTrack::AudioParamCheck(
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
    case AudioFormat_CHANNEL_OUT_DEFAULT: //AudioFormat.CHANNEL_CONFIGURATION_DEFAULT
    case AudioFormat_CHANNEL_OUT_MONO:
    case AudioFormat_CHANNEL_CONFIGURATION_MONO:
        mChannelCount = 1;
        mChannels = AudioFormat_CHANNEL_OUT_MONO;
        break;
    case AudioFormat_CHANNEL_OUT_STEREO:
    case AudioFormat_CHANNEL_CONFIGURATION_STEREO:
        mChannelCount = 2;
        mChannels = AudioFormat_CHANNEL_OUT_STEREO;
        break;
    default:
        mChannelCount = 0;
        mChannels = AudioFormat_CHANNEL_INVALID;
        mChannelConfiguration = AudioFormat_CHANNEL_CONFIGURATION_INVALID;
        //throw(new IllegalArgumentException("Unsupported channel configuration."));
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
        /*throw(new IllegalArgumentException("Unsupported sample encoding."
            + " Should be ENCODING_PCM_8BIT or ENCODING_PCM_16BIT."));*/
    }

    //--------------
    // audio load mode
    if ( (mode != AudioTrack_MODE_STREAM) && (mode != AudioTrack_MODE_STATIC) ) {
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
void CAudioTrack::AudioBuffSizeCheck(
    /* [in] */ Int32 audioBufferSize)
{
    // NB: this section is only valid with PCM data.
    //     To update when supporting compressed formats
    Int32 frameSizeInBytes = mChannelCount
            * (mAudioFormat == AudioFormat_ENCODING_PCM_8BIT ? 1 : 2);
    if ((audioBufferSize % frameSizeInBytes != 0) || (audioBufferSize < 1)) {
        //throw (new IllegalArgumentException("Invalid audio buffer size."));
    }

    mNativeBufferSizeInBytes = audioBufferSize;
}

void CAudioTrack::Finalize()
{
    NativeFinalize();
}

/**
 * Sets the initialization state of the instance. To be used in an AudioTrack subclass
 * constructor to set a subclass-specific post-initialization state.
 * @param state the state of the AudioTrack instance
 */
void CAudioTrack::SetState(
    /* [in] */ Int32 state)
{
    mState = state;
}

//---------------------------------------------------------
// Java methods called from the native side
//--------------------
void CAudioTrack::PostEventFromNative(
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


ECode CAudioTrack::ReleaseResources()
{
    // even though native_release() stops the native AudioTrack, we need to stop
    // AudioTrack subclasses too.
    //try {
        Stop();
    //} catch(IllegalStateException ise) {
        // don't raise an exception, we're releasing the resources.
    //}
    NativeRelease();
    mState = AudioTrack_STATE_UNINITIALIZED;

    return NOERROR;
}

ECode CAudioTrack::GetMinVolume(
    /* [out] */ Float* minVolume)
{
    *minVolume = VOLUME_MIN;
    return NOERROR;
}

ECode CAudioTrack::GetMaxVolume(
    /* [out] */ Float* maxVolume)
{
    *maxVolume = VOLUME_MAX;
    return NOERROR;
}

ECode CAudioTrack::GetSampleRate(
    /* [out] */ Int32* rate)
{
    *rate = mSampleRate;
    return NOERROR;
}

ECode CAudioTrack::GetPlaybackRate(
    /* [out] */ Int32* rate)
{
    *rate = NativeGetPlaybackRate();
    return NOERROR;
}

ECode CAudioTrack::GetAudioFormat(
    /* [out] */ Int32* format)
{
    *format = mAudioFormat;
    return NOERROR;
}

ECode CAudioTrack::GetStreamType(
    /* [out] */ Int32* type)
{
    *type = mStreamType;
    return NOERROR;
}

ECode CAudioTrack::GetChannelConfiguration(
    /* [out] */ Int32* config)
{
    *config = mChannelConfiguration;
    return NOERROR;
}

ECode CAudioTrack::GetChannelCount(
    /* [out] */ Int32* count)
{
    *count = mChannelCount;
    return NOERROR;
}

ECode CAudioTrack::GetState(
    /* [out] */ Int32* state)
{
    *state = mState;
    return NOERROR;
}

ECode CAudioTrack::GetPlayState(
    /* [out] */ Int32* state)
{
    *state = mPlayState;
    return NOERROR;
}

ECode CAudioTrack::GetNotificationMarkerPosition(
    /* [out] */ Int32* pos)
{
    *pos = NativeGetMarkerPos();

    return NOERROR;
}

ECode CAudioTrack::GetPositionNotificationPeriod(
    /* [out] */ Int32* period)
{
     *period = NativeGetPosUpdatePeriod();
    return NOERROR;
}

ECode CAudioTrack::GetPlaybackHeadPosition(
    /* [out] */ Int32* position)
{
    *position = NativeGetPosition();
    return NOERROR;
}

ECode CAudioTrack::GetNativeOutputSampleRate(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32 * rate)
{
    *rate = NativeGetOutputSampleRate(streamType);
    return NOERROR;
}

ECode CAudioTrack::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [out] */ Int32 * size)
{
     Int32 channelCount = 0;
     switch(channelConfig) {
     case AudioTrack_CHANNEL_OUT_MONO:
     case AudioTrack_CHANNEL_CONFIGURATION_MONO:
         channelCount = 1;
         break;
     case AudioTrack_CHANNEL_OUT_STEREO:
     case AudioTrack_CHANNEL_CONFIGURATION_STEREO:
         channelCount = 2;
         break;
     default:
         //loge("getMinBufferSize(): Invalid channel configuration.");
         return AudioTrack_ERROR_BAD_VALUE;
     }
        
     if ((audioFormat != AudioTrack_ENCODING_PCM_16BIT) 
         && (audioFormat != AudioTrack_ENCODING_PCM_8BIT)) {
         //loge("getMinBufferSize(): Invalid audio format.");
         return AudioTrack_ERROR_BAD_VALUE;
     }
        
     if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
         //loge("getMinBufferSize(): " + sampleRateInHz +"Hz is not a supported sample rate.");
         return AudioTrack_ERROR_BAD_VALUE;
     }
        
     Int32 BufferSize = NativeGetMinBuffSize(sampleRateInHz, channelCount, audioFormat);
     if ((BufferSize == -1) || (BufferSize == 0)) {
         //loge("getMinBufferSize(): error querying hardware");
         return AudioTrack_ERROR;
     }
     else {
        *size = BufferSize;
     }
    return NOERROR;
}

ECode CAudioTrack::GetAudioSessionId(
    /* [out] */ Int32 * id)
{
    *id = mSessionId;
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackPositionUpdateListener(
    /* [in] */ IOnPlaybackPositionUpdateListener *listener)
{
    SetPlaybackPositionUpdateListenerEx(listener, NULL);
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackPositionUpdateListenerEx(
    /* [in] */ IOnPlaybackPositionUpdateListener *listener,
    /* [in] */ IApartment *handler)
{
    //NativeEventHandlerDelegate* mEventHandlerDelegate = NULL;
    
    //synchronized (mPositionListenerLock) {
         mPositionListener = listener;
    //    }

         if (listener != NULL) {
            //mEventHandlerDelegate = new NativeEventHandlerDelegate(this, handler);  
         }
    
    return NOERROR;
}

ECode CAudioTrack::SetStereoVolume(
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume,
    /* [out] */ Int32 *code)
{
     if (mState != AudioTrack_STATE_INITIALIZED) {
         *code = AudioTrack_ERROR_INVALID_OPERATION;
     }

     // clamp the volumes
     Float mV; // mV is for output parameter
     if (leftVolume < GetMinVolume(&mV)) {
         leftVolume = GetMinVolume(&mV);
     }
     if (leftVolume > GetMaxVolume(&mV)) {
         leftVolume = GetMaxVolume(&mV);
     }
     if (rightVolume < GetMinVolume(&mV)) {
         rightVolume = GetMinVolume(&mV);
     }
     if (rightVolume > GetMaxVolume(&mV)) {
         rightVolume = GetMaxVolume(&mV);
     }

     NativeSetVolume(leftVolume, rightVolume);

     *code = AudioTrack_SUCCESS;
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz,
    /* [out] */ Int32 * rate)
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }
    if (sampleRateInHz <= 0) {
        return AudioTrack_ERROR_BAD_VALUE;
    }

     *rate = NativeSetPlaybackRate(sampleRateInHz);
    return NOERROR;
}

ECode CAudioTrack::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32 * code)
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }

    *code = NativeSetMarkerPos(markerInFrames); 
    return NOERROR;
}

ECode CAudioTrack::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32 * code)
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }

    *code = NativeSetPosUpdatePeriod(periodInFrames);
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackHeadPosition(
    /* [in] */ Int32 positionInFrames,
    /* [out] */ Int32 * code)
{
    //synchronized(mPlayStateLock) {
         if ((mPlayState == AudioTrack_PLAYSTATE_STOPPED) || (mPlayState == AudioTrack_PLAYSTATE_PAUSED)) {
             *code = NativeSetPosition(positionInFrames);
         } else {
             *code = AudioTrack_ERROR_INVALID_OPERATION;
         }
    //}
    return NOERROR;
}

ECode CAudioTrack::SetLoopPoints(
    /* [in] */ Int32 startInFrames,
    /* [in] */ Int32 endInFrames,
    /* [in] */ Int32 loopCount,
    /* [out] */ Int32 * code)
{
    if (mDataLoadMode == AudioTrack_MODE_STREAM) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }

    *code = NativeSetLoop(startInFrames, endInFrames, loopCount);
    return NOERROR;
}

ECode CAudioTrack::Play()
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        //throw(new IllegalStateException("play() called on uninitialized AudioTrack."));
    }

    //synchronized(mPlayStateLock) {
        NativeStart();
        mPlayState = AudioTrack_PLAYSTATE_PLAYING;
    //}
    return NOERROR;
}

ECode CAudioTrack::Stop()
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
         //throw(new IllegalStateException("stop() called on uninitialized AudioTrack."));
    }

    // stop playing
    //synchronized(mPlayStateLock) {
        NativeStop();
        mPlayState = AudioTrack_PLAYSTATE_STOPPED;
    //}
    return NOERROR;
}

ECode CAudioTrack::Pause()
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        //throw(new IllegalStateException("pause() called on uninitialized AudioTrack."));
    }
    //logd("pause()");

    // pause playback
    //synchronized(mPlayStateLock) {
        NativePause();
        mPlayState = AudioTrack_PLAYSTATE_PAUSED;
    //}
    return NOERROR;
}

ECode CAudioTrack::Flush()
{
    if (mState == AudioTrack_STATE_INITIALIZED) {
         // flush the data in native layer
         NativeFlush();
     }
    return NOERROR;
}

ECode CAudioTrack::Write(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32 * num)
{

    if ((mDataLoadMode == AudioTrack_MODE_STATIC) && (mState == AudioTrack_STATE_NO_STATIC_DATA) && (sizeInBytes > 0)) {
    mState = AudioTrack_STATE_INITIALIZED;
    }

    if (mState != AudioTrack_STATE_INITIALIZED) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }

    if ( (audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0) 
            || (offsetInBytes + sizeInBytes > audioData->GetLength() )) {
        return AudioTrack_ERROR_BAD_VALUE;
    }

    *num = NativeWriteByte(audioData, offsetInBytes, sizeInBytes, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::WriteEx(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32 * num)
{
    if ((mDataLoadMode == AudioTrack_MODE_STATIC)
            && (mState == AudioTrack_STATE_NO_STATIC_DATA)
            && (sizeInShorts > 0)) {
        mState = AudioTrack_STATE_INITIALIZED;
    }
        
    if (mState != AudioTrack_STATE_INITIALIZED) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }

    if ( (audioData == NULL) || (offsetInShorts < 0 ) || (sizeInShorts < 0) 
            || (offsetInShorts + sizeInShorts > audioData->GetLength())) {
        return AudioTrack_ERROR_BAD_VALUE;
    }

    *num = NativeWriteShort(audioData, offsetInShorts, sizeInShorts, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::ReloadStaticData(
    /* [out] */ Int32 * code)
{
    if (mDataLoadMode == AudioTrack_MODE_STREAM) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }
    *code =  NativeReloadStatic();
    return NOERROR;
}

ECode CAudioTrack::AttachAuxEffect(
    /* [in] */ Int32 effectId,
    /* [out] */ Int32 * code)
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }
    *code = NativeAttachAuxEffect(effectId);
    return NOERROR;
}

ECode CAudioTrack::SetAuxEffectSendLevel(
    /* [in] */ Float level,
    /* [out] */ Int32 * code)
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        return AudioTrack_ERROR_INVALID_OPERATION;
    }
    // clamp the level
    Float mV;
    if (level < GetMinVolume(&mV)) {
        level = GetMinVolume(&mV);
    }
    if (level > GetMaxVolume(&mV)) {
        level = GetMaxVolume(&mV);
    }
    NativeSetAuxEffectSendLevel(level);
    //return SUCCESS;
    return NOERROR;
}

ECode CAudioTrack::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode)
{
    //Init(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode, 0);
    return NOERROR;
}

ECode CAudioTrack::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 sessionId)
{
  //Init(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode, sessionId);
  return NOERROR;
}
