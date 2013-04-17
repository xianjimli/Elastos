
#include "media/CAudioTrack.h"
#include "media/ElAudioSystem.h"
#include <utils/Errors.h>
#include <utils/threads.h>

Int32 CAudioTrack::Native_get_playback_rate()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return (Int32) lpTrack->getSampleRate();   
}

Int32 CAudioTrack::Native_get_marker_pos()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getMarkerPosition(&position);
    return (Int32)position; 
}

Int32 CAudioTrack::Native_get_pos_update_period()
{
    uint32_t period = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPositionUpdatePeriod(&period);
    return (Int32)period; 
}

Int32 CAudioTrack::Native_get_position()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPosition(&position);
    return (Int32)position; 
}

Int32 CAudioTrack::Native_get_output_sample_rate(
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
Int32 CAudioTrack::Native_get_min_buff_size(
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

void CAudioTrack::Native_setVolume(
    /* [in] */ Float left, 
    /* [in] */ Float right)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    lpTrack->setVolume(left, right);
}

Int32 CAudioTrack::Android_media_translateErrorCode(
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

Int32 CAudioTrack::Native_set_playback_rate(
    /* [in] */ Int32 sampleRateInHz)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return Android_media_translateErrorCode(lpTrack->setSampleRate(sampleRateInHz));
}

Int32 CAudioTrack::Native_set_marker_pos(
    /* [in] */ Int32 markerPos)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    
    return Android_media_translateErrorCode( lpTrack->setMarkerPosition(markerPos) );   
}

Int32 CAudioTrack::Native_set_pos_update_period(
    /* [in] */ Int32 periodInFrames)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return Android_media_translateErrorCode( lpTrack->setPositionUpdatePeriod(periodInFrames) );   
}

Int32 CAudioTrack::Native_set_position(
    /* [in] */ Int32 positionInFrames)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return Android_media_translateErrorCode( lpTrack->setPosition(positionInFrames) );
}

Int32 CAudioTrack::Native_set_loop(
    /* [in] */ Int32 loopStart, 
    /* [in] */ Int32 loopEnd, 
    /* [in] */ Int32 loopCount)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    return Android_media_translateErrorCode( lpTrack->setLoop(loopStart, loopEnd, loopCount) );
}

void CAudioTrack::Native_Start()
{    
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->start();
}

void CAudioTrack::Native_Stop()
{    
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->stop();
}

void CAudioTrack::Native_Pause()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->pause();
}

void CAudioTrack::Native_Flush()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->flush();
}


Int32 CAudioTrack::writeToTrack(
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

Int32 CAudioTrack::Native_write_byte(    
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
    

    Int32 written = writeToTrack(lpTrack, javaAudioFormat, cAudioData, offsetInBytes, sizeInBytes);
    return written;
}

Int32 CAudioTrack::Native_write_short(
    /* [in] */ ArrayOf<Int16>* javaAudioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [in] */ Int32 javaAudioFormat) 
{
    return (Native_write_byte((ArrayOf<Byte>*) javaAudioData, offsetInShorts*2, sizeInShorts*2, javaAudioFormat)/ 2);
}

Int32 CAudioTrack::Native_reload_static()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return Android_media_translateErrorCode( lpTrack->reload()  );   
}

Int32 CAudioTrack::Native_attachAuxEffect(Int32 effectId)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return Android_media_translateErrorCode( lpTrack->attachAuxEffect(effectId)  );  
}

void CAudioTrack::Native_setAuxEffectSendLevel(
    /* [in] */Float level)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->setAuxEffectSendLevel(level);
}





ECode CAudioTrack::ReleaseResources()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    *rate = Native_get_playback_rate();
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
    *pos = Native_get_marker_pos();

    return NOERROR;
}

ECode CAudioTrack::GetPositionNotificationPeriod(
    /* [out] */ Int32* period)
{
     *period = Native_get_pos_update_period();
    return NOERROR;
}

ECode CAudioTrack::GetPlaybackHeadPosition(
    /* [out] */ Int32* position)
{
    *position = Native_get_position();
    return NOERROR;
}

ECode CAudioTrack::GetNativeOutputSampleRate(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32 * rate)
{
    *rate = Native_get_output_sample_rate(streamType);
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
     case CHANNEL_OUT_MONO:
     case CHANNEL_CONFIGURATION_MONO:
         channelCount = 1;
         break;
     case CHANNEL_OUT_STEREO:
     case CHANNEL_CONFIGURATION_STEREO:
         channelCount = 2;
         break;
     default:
         //loge("getMinBufferSize(): Invalid channel configuration.");
         return ERROR_BAD_VALUE;
     }
        
     if ((audioFormat != ENCODING_PCM_16BIT) 
         && (audioFormat != ENCODING_PCM_8BIT)) {
         //loge("getMinBufferSize(): Invalid audio format.");
         return ERROR_BAD_VALUE;
     }
        
     if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
         //loge("getMinBufferSize(): " + sampleRateInHz +"Hz is not a supported sample rate.");
         return ERROR_BAD_VALUE;
     }
        
     Int32 BufferSize = Native_get_min_buff_size(sampleRateInHz, channelCount, audioFormat);
     if ((BufferSize == -1) || (BufferSize == 0)) {
         //loge("getMinBufferSize(): error querying hardware");
         return ERROR;
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
     if (mState != STATE_INITIALIZED) {
         *code = ERROR_INVALID_OPERATION;
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

     Native_setVolume(leftVolume, rightVolume);

     *code = SUCCESS;
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz,
    /* [out] */ Int32 * rate)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    if (sampleRateInHz <= 0) {
        return ERROR_BAD_VALUE;
    }

     *rate = Native_set_playback_rate(sampleRateInHz);
    return NOERROR;
}

ECode CAudioTrack::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32 * code)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    *code = Native_set_marker_pos(markerInFrames); 
    return NOERROR;
}

ECode CAudioTrack::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32 * code)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    *code = Native_set_pos_update_period(periodInFrames);
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackHeadPosition(
    /* [in] */ Int32 positionInFrames,
    /* [out] */ Int32 * code)
{
    //synchronized(mPlayStateLock) {
         if ((mPlayState == PLAYSTATE_STOPPED) || (mPlayState == PLAYSTATE_PAUSED)) {
             *code = Native_set_position(positionInFrames);
         } else {
             *code = ERROR_INVALID_OPERATION;
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
    if (mDataLoadMode == MODE_STREAM) {
        return ERROR_INVALID_OPERATION;
    }

    *code = Native_set_loop(startInFrames, endInFrames, loopCount);
    return NOERROR;
}

ECode CAudioTrack::Play()
{
    if (mState != STATE_INITIALIZED) {
        //throw(new IllegalStateException("play() called on uninitialized AudioTrack."));
    }

    //synchronized(mPlayStateLock) {
        Native_Start();
        mPlayState = PLAYSTATE_PLAYING;
    //}
    return NOERROR;
}

ECode CAudioTrack::Stop()
{
    if (mState != STATE_INITIALIZED) {
         //throw(new IllegalStateException("stop() called on uninitialized AudioTrack."));
    }

    // stop playing
    //synchronized(mPlayStateLock) {
        Native_Stop();
        mPlayState = PLAYSTATE_STOPPED;
    //}
    return NOERROR;
}

ECode CAudioTrack::Pause()
{
    if (mState != STATE_INITIALIZED) {
        //throw(new IllegalStateException("pause() called on uninitialized AudioTrack."));
    }
    //logd("pause()");

    // pause playback
    //synchronized(mPlayStateLock) {
        Native_Pause();
        mPlayState = PLAYSTATE_PAUSED;
    //}
    return NOERROR;
}

ECode CAudioTrack::Flush()
{
    if (mState == STATE_INITIALIZED) {
         // flush the data in native layer
         Native_Flush();
     }
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::Write(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32 * num)
{

    if ((mDataLoadMode == MODE_STATIC) && (mState == STATE_NO_STATIC_DATA) && (sizeInBytes > 0)) {
    mState = STATE_INITIALIZED;
    }

    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }

    if ( (audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0) 
            || (offsetInBytes + sizeInBytes > audioData->GetLength() )) {
        return ERROR_BAD_VALUE;
    }

    *num = Native_write_byte(audioData, offsetInBytes, sizeInBytes, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::WriteEx(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32 * num)
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

    *num = Native_write_short(audioData, offsetInShorts, sizeInShorts, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::ReloadStaticData(
    /* [out] */ Int32 * code)
{
    if (mDataLoadMode == MODE_STREAM) {
        return ERROR_INVALID_OPERATION;
    }
    *code =  Native_reload_static();
    return NOERROR;
}

ECode CAudioTrack::AttachAuxEffect(
    /* [in] */ Int32 effectId,
    /* [out] */ Int32 * code)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    *code = Native_attachAuxEffect(effectId);
    return NOERROR;
}

ECode CAudioTrack::SetAuxEffectSendLevel(
    /* [in] */ Float level,
    /* [out] */ Int32 * code)
{
    if (mState != STATE_INITIALIZED) {
        return ERROR_INVALID_OPERATION;
    }
    // clamp the level
    Float mV;
    if (level < GetMinVolume(&mV)) {
        level = GetMinVolume(&mV);
    }
    if (level > GetMaxVolume(&mV)) {
        level = GetMaxVolume(&mV);
    }
    Native_setAuxEffectSendLevel(level);
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
    // TODO: Add your code here
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
    // TODO: Add your code here
    return NOERROR;
}
