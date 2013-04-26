
#include "media/CAudioTrack.h"
#include "media/ElAudioSystem.h"

using namespace Elastos::Core::Threading;

CAudioTrack::CAudioTrack()
    :mState(AudioTrack_STATE_UNINITIALIZED)
    ,mPlayState(AudioTrack_PLAYSTATE_STOPPED)
    ,mNativeBufferSizeInBytes(0)
    ,mSampleRate(22050)
    ,mChannelCount(1)
    ,mChannels(AudioFormat_CHANNEL_OUT_MONO)
    ,mStreamType(AudioManager_STREAM_MUSIC)
    ,mDataLoadMode(AudioTrack_MODE_STREAM)
    ,mChannelConfiguration(AudioFormat_CHANNEL_OUT_MONO)
    ,mAudioFormat(AudioFormat_ENCODING_PCM_16BIT)
    ,mSessionId(0)
{

}

CAudioTrack::~CAudioTrack()
{
    NativeFinalize();
}

ECode CAudioTrack::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode)
{
    Init(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode, 0);
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
  Init(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode, sessionId);
  return NOERROR;
}

ECode CAudioTrack::ReleaseResources()
{
    // even though native_release() stops the native AudioTrack, we need to stop
    // AudioTrack subclasses too.
    Stop();

    NativeRelease();
    mState = AudioTrack_STATE_UNINITIALIZED;

    return NOERROR;
}

ECode CAudioTrack::GetMinVolume(
    /* [out] */ Float* minVolume)
{
    VALIDATE_NOT_NULL(minVolume);
    *minVolume = VOLUME_MIN;
    return NOERROR;
}

ECode CAudioTrack::GetMaxVolume(
    /* [out] */ Float* maxVolume)
{
    VALIDATE_NOT_NULL(maxVolume);
    *maxVolume = VOLUME_MAX;
    return NOERROR;
}

ECode CAudioTrack::GetSampleRate(
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = mSampleRate;
    return NOERROR;
}

ECode CAudioTrack::GetPlaybackRate(
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = NativeGetPlaybackRate();
    return NOERROR;
}

ECode CAudioTrack::GetAudioFormat(
    /* [out] */ Int32* format)
{
    VALIDATE_NOT_NULL(format);
    *format = mAudioFormat;
    return NOERROR;
}

ECode CAudioTrack::GetStreamType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mStreamType;
    return NOERROR;
}

ECode CAudioTrack::GetChannelConfiguration(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChannelConfiguration;
    return NOERROR;
}

ECode CAudioTrack::GetChannelCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mChannelCount;
    return NOERROR;
}

ECode CAudioTrack::GetState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;
    return NOERROR;
}

ECode CAudioTrack::GetPlayState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mPlayState;
    return NOERROR;
}

Int32 CAudioTrack::GetNativeFrameCount()
{
    return NativeGetNativeFrameCount();
}

ECode CAudioTrack::GetNotificationMarkerPosition(
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    *pos = NativeGetMarkerPos();
    return NOERROR;
}

ECode CAudioTrack::GetPositionNotificationPeriod(
    /* [out] */ Int32* period)
{
    VALIDATE_NOT_NULL(period);
    *period = NativeGetPosUpdatePeriod();
    return NOERROR;
}

ECode CAudioTrack::GetPlaybackHeadPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = NativeGetPosition();
    return NOERROR;
}

ECode CAudioTrack::GetNativeOutputSampleRate(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32 * rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = NativeGetOutputSampleRate(streamType);
    return NOERROR;
}

ECode CAudioTrack::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
     Int32 channelCount = 0;
     switch(channelConfig) {
     case AudioFormat_CHANNEL_OUT_MONO:
     case AudioFormat_CHANNEL_CONFIGURATION_MONO:
         channelCount = 1;
         break;
     case AudioFormat_CHANNEL_OUT_STEREO:
     case AudioFormat_CHANNEL_CONFIGURATION_STEREO:
         channelCount = 2;
         break;
     default:
         //loge("getMinBufferSize(): Invalid channel configuration.");
         *size = AudioTrack_ERROR_BAD_VALUE;
         return NOERROR;
     }

     if ((audioFormat != AudioFormat_ENCODING_PCM_16BIT)
         && (audioFormat != AudioFormat_ENCODING_PCM_8BIT)) {
         //loge("getMinBufferSize(): Invalid audio format.");
         *size = AudioTrack_ERROR_BAD_VALUE;
         return NOERROR;
     }

     if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
         //loge("getMinBufferSize(): " + sampleRateInHz +"Hz is not a supported sample rate.");
         *size = AudioTrack_ERROR_BAD_VALUE;
         return NOERROR;
     }

     Int32 BufferSize = NativeGetMinBuffSize(sampleRateInHz, channelCount, audioFormat);
     if ((BufferSize == -1) || (BufferSize == 0)) {
         //loge("getMinBufferSize(): error querying hardware");
         *size = AudioTrack_ERROR;
         return NOERROR;
     }
     else {
        *size = BufferSize;
     }
    return NOERROR;
}

ECode CAudioTrack::GetAudioSessionId(
    /* [out] */ Int32 * id)
{
    VALIDATE_NOT_NULL(id);
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

    {
        //synchronized (mPositionListenerLock)
        Mutex::Autolock lock(&mPositionListenerLock);
        mPositionListener = listener;
    }

     if (listener != NULL) {
        //mEventHandlerDelegate = new NativeEventHandlerDelegate(this, handler);
     }
    return NOERROR;
}

ECode CAudioTrack::SetStereoVolume(
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != AudioTrack_STATE_INITIALIZED) {
         *result = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
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

    *result = AudioTrack_SUCCESS;
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz,
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    if (mState != AudioTrack_STATE_INITIALIZED) {
        *rate = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    if (sampleRateInHz <= 0) {
        *rate = AudioTrack_ERROR_BAD_VALUE;
        return NOERROR;
    }
    *rate = NativeSetPlaybackRate(sampleRateInHz);
    return NOERROR;
}

ECode CAudioTrack::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != AudioTrack_STATE_INITIALIZED) {
        *result = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeSetMarkerPos(markerInFrames);
    return NOERROR;
}

ECode CAudioTrack::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != AudioTrack_STATE_INITIALIZED) {
        *result = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeSetPosUpdatePeriod(periodInFrames);
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackHeadPosition(
    /* [in] */ Int32 positionInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Mutex::Autolock lock(&mPositionListenerLock);
    if ((mPlayState == AudioTrack_PLAYSTATE_STOPPED) || (mPlayState == AudioTrack_PLAYSTATE_PAUSED)) {
        *result = NativeSetPosition(positionInFrames);
    }
    else {
        *result = AudioTrack_ERROR_INVALID_OPERATION;
    }
    return NOERROR;
}

ECode CAudioTrack::SetLoopPoints(
    /* [in] */ Int32 startInFrames,
    /* [in] */ Int32 endInFrames,
    /* [in] */ Int32 loopCount,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mDataLoadMode == AudioTrack_MODE_STREAM) {
        *result = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeSetLoop(startInFrames, endInFrames, loopCount);
    return NOERROR;
}

ECode CAudioTrack::Play()
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        //throw(new IllegalStateException("play() called on uninitialized AudioTrack."));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Mutex::Autolock lock(&mPositionListenerLock);
    NativeStart();
    mPlayState = AudioTrack_PLAYSTATE_PLAYING;
    return NOERROR;
}

ECode CAudioTrack::Stop()
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
         //throw(new IllegalStateException("stop() called on uninitialized AudioTrack."));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // stop playing
    Mutex::Autolock lock(&mPositionListenerLock);
    NativeStop();
    mPlayState = AudioTrack_PLAYSTATE_STOPPED;
    return NOERROR;
}

ECode CAudioTrack::Pause()
{
    if (mState != AudioTrack_STATE_INITIALIZED) {
        //throw(new IllegalStateException("pause() called on uninitialized AudioTrack."));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //logd("pause()");
    // pause playback
    Mutex::Autolock lock(&mPositionListenerLock);
    NativePause();
    mPlayState = AudioTrack_PLAYSTATE_PAUSED;

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
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    if ((mDataLoadMode == AudioTrack_MODE_STATIC) && (mState == AudioTrack_STATE_NO_STATIC_DATA) && (sizeInBytes > 0)) {
    mState = AudioTrack_STATE_INITIALIZED;
    }

    if (mState != AudioTrack_STATE_INITIALIZED) {
        *num = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0)
            || (offsetInBytes + sizeInBytes > audioData->GetLength() )) {
        *num = AudioTrack_ERROR_BAD_VALUE;
        return NOERROR;
    }
    *num = NativeWriteByte(audioData, offsetInBytes, sizeInBytes, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::WriteEx(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    if ((mDataLoadMode == AudioTrack_MODE_STATIC)
            && (mState == AudioTrack_STATE_NO_STATIC_DATA)
            && (sizeInShorts > 0)) {
        mState = AudioTrack_STATE_INITIALIZED;
    }

    if (mState != AudioTrack_STATE_INITIALIZED) {
        *num = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioData == NULL) || (offsetInShorts < 0 ) || (sizeInShorts < 0)
            || (offsetInShorts + sizeInShorts > audioData->GetLength())) {
        *num = AudioTrack_ERROR_BAD_VALUE;
        return NOERROR;
    }
    *num = NativeWriteInt16(audioData, offsetInShorts, sizeInShorts, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::ReloadStaticData(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mDataLoadMode == AudioTrack_MODE_STREAM) {
        *result = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeReloadStatic();
    return NOERROR;
}

ECode CAudioTrack::AttachAuxEffect(
    /* [in] */ Int32 effectId,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != AudioTrack_STATE_INITIALIZED) {
        *result = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeAttachAuxEffect(effectId);
    return NOERROR;
}

ECode CAudioTrack::SetAuxEffectSendLevel(
    /* [in] */ Float level,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != AudioTrack_STATE_INITIALIZED) {
        *result = AudioTrack_ERROR_INVALID_OPERATION;
        return NOERROR;
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
    *result = AudioTrack_SUCCESS;
    return NOERROR;
}


//Native method
#define AUDIOTRACK_SUCCESS                         0
#define AUDIOTRACK_ERROR                           -1
#define AUDIOTRACK_ERROR_BAD_VALUE                 -2
#define AUDIOTRACK_ERROR_INVALID_OPERATION         -3
#define AUDIOTRACK_ERROR_SETUP_AUDIOSYSTEM         -16
#define AUDIOTRACK_ERROR_SETUP_INVALIDCHANNELMASK -17
#define AUDIOTRACK_ERROR_SETUP_INVALIDFORMAT       -18
#define AUDIOTRACK_ERROR_SETUP_INVALIDSTREAMTYPE   -19
#define AUDIOTRACK_ERROR_SETUP_NATIVEINITFAILED    -20

struct fields_t {
// these fields provide access from C++ to the...
Int32       PCM16;                 //...  format constants
Int32       PCM8;                  //...  format constants
Int32       STREAM_VOICE_CALL;     //...  stream type constants
Int32       STREAM_SYSTEM;         //...  stream type constants
Int32       STREAM_RING;           //...  stream type constants
Int32       STREAM_MUSIC;          //...  stream type constants
Int32       STREAM_ALARM;          //...  stream type constants
Int32       STREAM_NOTIFICATION;   //...  stream type constants
Int32       STREAM_BLUETOOTH_SCO;  //...  stream type constants
Int32       STREAM_DTMF;           //...  stream type constants
Int32       MODE_STREAM;           //...  memory mode
Int32       MODE_STATIC;           //...  memory mode
};

Int32 CAudioTrack::AndroidMediaTranslateErrorCode(
    /* [in] */ Int32 code)
{
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

ECode CAudioTrack::Init(
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

    FAIL_RETURN(AudioParamCheck(streamType, sampleRateInHz, channelConfig, audioFormat, mode));

    FAIL_RETURN(AudioBuffSizeCheck(bufferSizeInBytes));

    if (sessionId < 0) {
        //throw (new IllegalArgumentException("Invalid audio session ID: "+sessionId));
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32* session = new Int32[1];
    session[0] = sessionId;
    // native initialization
    Int32 initResult = NativeSetup(mStreamType, mSampleRate, mChannels, mAudioFormat, mNativeBufferSizeInBytes, mDataLoadMode, session);
    if (initResult != AudioTrack_SUCCESS) {
        //loge("Error code "+initResult+" when initializing AudioTrack.");
        return NOERROR; // with mState == STATE_UNINITIALIZED
    }

    mSessionId = session[0];

    if (mDataLoadMode == AudioTrack_MODE_STATIC) {
        mState = AudioTrack_STATE_NO_STATIC_DATA;
    } else {
        mState = AudioTrack_STATE_INITIALIZED;
    }
    return NOERROR;
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

ECode CAudioTrack::AudioParamCheck(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 mode)
{
    //--------------
    // stream type
    if( (streamType != AudioManager_STREAM_ALARM) && (streamType != AudioManager_STREAM_MUSIC)
       && (streamType != AudioManager_STREAM_RING) && (streamType != AudioManager_STREAM_SYSTEM)
       && (streamType != AudioManager_STREAM_VOICE_CALL)
       && (streamType != AudioManager_STREAM_NOTIFICATION)
       && (streamType != AudioManager_STREAM_BLUETOOTH_SCO)
       && (streamType != AudioManager_STREAM_DTMF)) {
       //throw (new IllegalArgumentException("Invalid stream type."));
       return E_ILLEGAL_STATE_EXCEPTION;
    }
    else {
        mStreamType = streamType;
    }

    //--------------
    // sample rate
    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        /*throw (new IllegalArgumentException(sampleRateInHz + "Hz is not a supported sample rate."));*/
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else {
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
        return E_ILLEGAL_STATE_EXCEPTION;
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
        /*throw(new IllegalArgumentException("Unsupported sample encoding." + " Should be ENCODING_PCM_8BIT or ENCODING_PCM_16BIT."));*/
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    //--------------
    // audio load mode
    if ( (mode != AudioTrack_MODE_STREAM) && (mode != AudioTrack_MODE_STATIC) ) {
        //throw(new IllegalArgumentException("Invalid mode."));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else {
        mDataLoadMode = mode;
    }
    return NOERROR;
}

// Convenience method for the contructor's audio buffer size check.
// preconditions:
//    mChannelCount is valid
//    mAudioFormat is valid
// postcondition:
//    mNativeBufferSizeInBytes is valid (multiple of frame size, positive)
ECode CAudioTrack::AudioBuffSizeCheck(
    /* [in] */ Int32 audioBufferSize)
{
    // NB: this section is only valid with PCM data.
    //     To update when supporting compressed formats
    Int32 frameSizeInBytes = mChannelCount * (mAudioFormat == AudioFormat_ENCODING_PCM_8BIT ? 1 : 2);
    if ((audioBufferSize % frameSizeInBytes != 0) || (audioBufferSize < 1)) {
        //throw (new IllegalArgumentException("Invalid audio buffer size."));
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mNativeBufferSizeInBytes = audioBufferSize;
    return NOERROR;
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

Int32 CAudioTrack::NativeSetup(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHertz,
    /* [in] */ Int32 channels,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 buffSizeInBytes,
    /* [in] */ Int32 memoryMode,
    /* [in] */ Int32* sessionId)
{/*
    fields_t javaAudioTrackFields;
    //LOGV("sampleRate=%d, audioFormat(from Java)=%d, channels=%x, buffSize=%d",sampleRateInHertz, audioFormat, channels, buffSizeInBytes);
    Int32 afSampleRate;
    Int32 afFrameCount;

    if (android::AudioSystem::getOutputFrameCount(&afFrameCount, streamType) != android::NO_ERROR) {
        //LOGE("Error creating AudioTrack: Could not get AudioSystem frame count.");
        return AUDIOTRACK_ERROR_SETUP_AUDIOSYSTEM;
    }
    if (android::AudioSystem::getOutputSamplingRate(&afSampleRate, streamType) != android::NO_ERROR) {
        //LOGE("Error creating AudioTrack: Could not get AudioSystem sampling rate.");
        return AUDIOTRACK_ERROR_SETUP_AUDIOSYSTEM;
    }

    if (!android::AudioSystem::isOutputChannel(channels)) {
        //LOGE("Error creating AudioTrack: invalid channel mask.");
        return AUDIOTRACK_ERROR_SETUP_INVALIDCHANNELMASK;
    }
    Int32 nbChannels = android::AudioSystem::popCount(channels);

    // check the stream type
    android::AudioSystem::stream_type atStreamType;
    if (streamType == javaAudioTrackFields.STREAM_VOICE_CALL) {
        atStreamType = android::AudioSystem::VOICE_CALL;
    } else if (streamType == javaAudioTrackFields.STREAM_SYSTEM) {
        atStreamType = android::AudioSystem::SYSTEM;
    } else if (streamType == javaAudioTrackFields.STREAM_RING) {
        atStreamType = android::AudioSystem::RING;
    } else if (streamType == javaAudioTrackFields.STREAM_MUSIC) {
        atStreamType = android::AudioSystem::MUSIC;
    } else if (streamType == javaAudioTrackFields.STREAM_ALARM) {
        atStreamType = android::AudioSystem::ALARM;
    } else if (streamType == javaAudioTrackFields.STREAM_NOTIFICATION) {
        atStreamType = android::AudioSystem::NOTIFICATION;
    } else if (streamType == javaAudioTrackFields.STREAM_BLUETOOTH_SCO) {
        atStreamType = android::AudioSystem::BLUETOOTH_SCO;
    } else if (streamType == javaAudioTrackFields.STREAM_DTMF) {
        atStreamType = android::AudioSystem::DTMF;
    } else {
        //LOGE("Error creating AudioTrack: unknown stream type.");
        return AUDIOTRACK_ERROR_SETUP_INVALIDSTREAMTYPE;
    }

    // check the format.
    // This function was called from Java, so we compare the format against the Java constants
    if ((audioFormat != javaAudioTrackFields.PCM16) && (audioFormat != javaAudioTrackFields.PCM8)) {
        //LOGE("Error creating AudioTrack: unsupported audio format.");
        return AUDIOTRACK_ERROR_SETUP_INVALIDFORMAT;
    }

    // for the moment 8bitPCM in MODE_STATIC is not supported natively in the AudioTrack C++ class
    // so we declare everything as 16bitPCM, the 8->16bit conversion for MODE_STATIC will be handled
    // in android_media_AudioTrack_native_write()
    if ((audioFormat == javaAudioTrackFields.PCM8)
        && (memoryMode == javaAudioTrackFields.MODE_STATIC)) {
        //LOGV("android_media_AudioTrack_native_setup(): requesting MODE_STATIC for 8bit \ buff size of %dbytes, switching to 16bit, buff size of %dbytes",buffSizeInBytes, 2*buffSizeInBytes);
        audioFormat = javaAudioTrackFields.PCM16;
        // we will need twice the memory to store the data
        buffSizeInBytes *= 2;
    }

    // compute the frame count
    Int32 bytesPerSample = audioFormat == javaAudioTrackFields.PCM16 ? 2 : 1;
    Int32 format = audioFormat == javaAudioTrackFields.PCM16 ? android::AudioSystem::PCM_16_BIT : android::AudioSystem::PCM_8_BIT;
    Int32 frameCount = buffSizeInBytes / (nbChannels * bytesPerSample);

    AudioTrackJniStorage* lpJniStorage = new AudioTrackJniStorage();

    // initialize the callback information:
    // this data will be passed with every AudioTrack callback
    //jclass clazz = env->GetObjectClass(thiz);
    //if (clazz == NULL) {
    //    LOGE("Can't find %s when setting up callback.", kClassPathName);
        delete lpJniStorage;
        return AUDIOTRACK_ERROR_SETUP_NATIVEINITFAILED;
    //}
    //lpJniStorage->mCallbackData.audioTrack_class = (jclass)env->NewGlobalRef(clazz);
    // we use a weak reference so the AudioTrack object can be garbage collected.
    //lpJniStorage->mCallbackData.audioTrack_ref = env->NewGlobalRef(weak_this);

    lpJniStorage->mStreamType = atStreamType;

    //if (jSession == NULL) {
    //    LOGE("Error creating AudioTrack: invalid session ID pointer");
    //    delete lpJniStorage;
    //    return AUDIOTRACK_ERROR;
    //}

    Int32* nSession[1];
    nSession[0] = (Int32*)sessionId;
    if (nSession == NULL) {
    //    LOGE("Error creating AudioTrack: Error retrieving session id pointer");
        delete lpJniStorage;
        return AUDIOTRACK_ERROR;
    }
    Int32 sessionId = &nSession[0];
    //env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
    nSession = NULL;

    // create the native AudioTrack object
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    if (lpTrack == NULL) {
    //    LOGE("Error creating uninitialized AudioTrack");
        goto native_track_failure;
    }

    // initialize the native AudioTrack object
    if (memoryMode == javaAudioTrackFields.MODE_STREAM) {

        lpTrack->set(
            atStreamType,// stream type
            sampleRateInHertz,
            format,// word length, PCM
            channels,
            frameCount,
            0,// flags
            audioCallback, &(lpJniStorage->mCallbackData),//callback, callback data (user)
            0,// notificationFrames == 0 since not using EVENT_MORE_DATA to feed the AudioTrack
            0,// shared mem
            true,// thread can call Java
            sessionId);// audio session ID
    }
    else if (memoryMode == javaAudioTrackFields.MODE_STATIC) {
        // AudioTrack is using shared memory

        if (!lpJniStorage->allocSharedMem(buffSizeInBytes)) {
        //    LOGE("Error creating AudioTrack in static mode: error creating mem heap base");
            goto native_init_failure;
        }

        lpTrack->set(
            atStreamType,// stream type
            sampleRateInHertz,
            format,// word length, PCM
            channels,
            frameCount,
            0,// flags
            audioCallback, &(lpJniStorage->mCallbackData),//callback, callback data (user));
            0,// notificationFrames == 0 since not using EVENT_MORE_DATA to feed the AudioTrack
            lpJniStorage->mMemBase,// shared mem
            true,// thread can call Java
            sessionId);// audio session ID
    }

    if (lpTrack->initCheck() != android::NO_ERROR) {
        //LOGE("Error initializing AudioTrack");
        goto native_init_failure;
    }

    nSession = (Int32*) sessionId;
    if (nSession == NULL) {
        //LOGE("Error creating AudioTrack: Error retrieving session id pointer");
        goto native_init_failure;
    }
    // read the audio session ID back from AudioTrack in case we create a new session
    nSession[0] = lpTrack->getSessionId();
    //env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
    nSession = NULL;

    // save our newly created C++ AudioTrack in the "nativeTrackInJavaObj" field
    // of the Java object (in mNativeTrackInJavaObj)
    //env->SetIntField(thiz, javaAudioTrackFields.nativeTrackInJavaObj, (int)lpTrack);

    // save the JNI resources so we can free them later
    //LOGV("storing lpJniStorage: %x\n", (int)lpJniStorage);
    //env->SetIntField(thiz, javaAudioTrackFields.jniData, (int)lpJniStorage);

    return AUDIOTRACK_SUCCESS;

    // failures:
native_init_failure:
    delete lpTrack;
    //env->SetIntField(thiz, javaAudioTrackFields.nativeTrackInJavaObj, 0);

native_track_failure:
    //if (nSession != NULL) {
    //    env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
    //}
    //env->DeleteGlobalRef(lpJniStorage->mCallbackData.audioTrack_class);
    //env->DeleteGlobalRef(lpJniStorage->mCallbackData.audioTrack_ref);
    delete lpJniStorage;
    //env->SetIntField(thiz, javaAudioTrackFields.jniData, 0);
    return AUDIOTRACK_ERROR_SETUP_NATIVEINITFAILED;
    */
    return 0;
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

Int32 CAudioTrack::NativeWriteInt16(
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

Int32 CAudioTrack::NativeGetNativeFrameCount()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    if (lpTrack) {
        return lpTrack->frameCount();
    }
    else {
        //jniThrowException(env, "java/lang/IllegalStateException","Unable to retrieve AudioTrack pointer for frameCount()");
        return AUDIOTRACK_ERROR;
    }
}

void CAudioTrack::NativeSetVolume(
    /* [in] */ Float left,
    /* [in] */ Float right)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);
    lpTrack->setVolume(left, right);
}

Int32 CAudioTrack::NativeSetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->setSampleRate(sampleRateInHz));
}

Int32 CAudioTrack::NativeGetPlaybackRate()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return (Int32) lpTrack->getSampleRate();
}


Int32 CAudioTrack::NativeSetMarkerPos(
    /* [in] */ Int32 markerPos)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode( lpTrack->setMarkerPosition(markerPos) );
}

Int32 CAudioTrack::NativeGetMarkerPos()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getMarkerPosition(&position);
    return (Int32)position;
}


Int32 CAudioTrack::NativeSetPosUpdatePeriod(
    /* [in] */ Int32 periodInFrames)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode( lpTrack->setPositionUpdatePeriod(periodInFrames) );
}


Int32 CAudioTrack::NativeGetPosUpdatePeriod()
{
    uint32_t period = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPositionUpdatePeriod(&period);
    return (Int32)period;
}

Int32 CAudioTrack::NativeSetPosition(
    /* [in] */ Int32 positionInFrames)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode( lpTrack->setPosition(positionInFrames) );
}

Int32 CAudioTrack::NativeGetPosition()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPosition(&position);
    return (Int32)position;
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

#define DEFAULT_OUTPUT_SAMPLE_RATE 44100;

Int32 CAudioTrack::NativeGetOutputSampleRate(
   /* [in] */ Int32 javaStreamType)
{
    fields_t javaAudioTrackFields;
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

Int32 CAudioTrack::NativeGetSessionId()
{
    return 0;
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

