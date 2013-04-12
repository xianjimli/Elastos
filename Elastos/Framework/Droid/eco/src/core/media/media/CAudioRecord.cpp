
#include <media/AudioSystem.h>
#include <media/mediarecorder.h>
#include <media/AudioTrack.h>
#include <media/AudioRecord.h>
#include "media/CAudioRecord.h"
#include "media/CMediaRecorder.h"


//AudioRecord::NativeEventHandler::NativeEventHandler(
//    /* [in] */ AudioRecord* recorder,
//    /* [in] */ ILooper* looper)
//{
//    super(looper);
//    mAudioRecord = recorder;
//}

PInterface CAudioRecord::NativeEventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CAudioRecord::NativeEventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioRecord::NativeEventHandler::Release()
{
    return ElRefBase::Release();
}

ECode CAudioRecord::NativeEventHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::NativeEventHandler::HandleMessage(
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


const Int32 CAudioRecord::AUDIORECORD_ERROR_SETUP_ZEROFRAMECOUNT;
const Int32 CAudioRecord::AUDIORECORD_ERROR_SETUP_INVALIDCHANNELMASK;
const Int32 CAudioRecord::AUDIORECORD_ERROR_SETUP_INVALIDFORMAT;
const Int32 CAudioRecord::AUDIORECORD_ERROR_SETUP_INVALIDSOURCE;
const Int32 CAudioRecord::AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED;

// Events:
// to keep in sync with frameworks/base/include/media/AudioRecord.h
/**
 * Event id denotes when record head has reached a previously set marker.
 */
const Int32 CAudioRecord::NATIVE_EVENT_MARKER;
/**
 * Event id denotes when previously set update period has elapsed during recording.
 */
const Int32 CAudioRecord::NATIVE_EVENT_NEW_POS;

const CString CAudioRecord::TAG = "CAudioRecord";


CAudioRecord::CAudioRecord()
    : mSampleRate(22050)
    , mChannelCount(1)
    , mChannels(AudioFormat_CHANNEL_IN_MONO)
    , mChannelConfiguration(AudioFormat_CHANNEL_IN_MONO)
    , mAudioFormat(AudioFormat_ENCODING_PCM_16BIT)
    , mRecordSource(CMediaRecorder::AudioSource::DEFAULT)
    , mState(AudioRecord_STATE_UNINITIALIZED)
    , mRecordingState(AudioRecord_RECORDSTATE_STOPPED)
    , mNativeBufferSizeInBytes(0)
{}

CAudioRecord::~CAudioRecord()
{
    NativeFinalize();
}

ECode CAudioRecord::constructor(
    /* [in] */ Int32 audioSource,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes)
{
    mState = AudioRecord_STATE_UNINITIALIZED;
    mRecordingState = AudioRecord_RECORDSTATE_STOPPED;

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
    if (initResult != AudioRecord_SUCCESS) {
        //loge("Error code "+initResult+" when initializing native AudioRecord object.");
        return NOERROR; // with mState == STATE_UNINITIALIZED
    }

    mState = AudioRecord_STATE_INITIALIZED;
    return NOERROR;
}

// Convenience method for the constructor's parameter checks.
// This is where constructor IllegalArgumentException-s are thrown
// postconditions:
//    mRecordSource is valid
//    mChannelCount is valid
//    mChannels is valid
//    mAudioFormat is valid
//    mSampleRate is valid
ECode CAudioRecord::AudioParamCheck(
    /* [in] */ Int32 audioSource,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat)
{
    //--------------
    // audio source
    if ( (audioSource < CMediaRecorder::AudioSource::DEFAULT) ||
        (audioSource > CMediaRecorder::AudioSource::VOICE_COMMUNICATION) )  {
            //(audioSource > MediaRecorder.getAudioSourceMax()) )  {
        //throw (new IllegalArgumentException("Invalid audio source."));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
        mRecordSource = audioSource;
    }

    //--------------
    // sample rate
    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        /*throw (new IllegalArgumentException(sampleRateInHz
                + "Hz is not a supported sample rate."));*/
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
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
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
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
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

// Convenience method for the contructor's audio buffer size check.
// preconditions:
//    mChannelCount is valid
//    mAudioFormat is AudioFormat.ENCODING_PCM_8BIT OR AudioFormat.ENCODING_PCM_16BIT
// postcondition:
//    mNativeBufferSizeInBytes is valid (multiple of frame size, positive)
ECode CAudioRecord::AudioBuffSizeCheck(
    /* [in] */ Int32 audioBufferSize)
{
    // NB: this section is only valid with PCM data.
    // To update when supporting compressed formats
    Int32 frameSizeInBytes = mChannelCount
        * (mAudioFormat == AudioFormat_ENCODING_PCM_8BIT ? 1 : 2);
    if ((audioBufferSize % frameSizeInBytes != 0) || (audioBufferSize < 1)) {
        //throw (new IllegalArgumentException("Invalid audio buffer size."));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mNativeBufferSizeInBytes = audioBufferSize;
    return NOERROR;
}

ECode CAudioRecord::ReleaseResources()
{
    //try {
    Stop();
    //} catch(IllegalStateException ise) {
        // don't raise an exception, we're releasing the resources.
    //}
    NativeRelease();
    mState = AudioRecord_STATE_UNINITIALIZED;

    return NOERROR;
}

ECode CAudioRecord::GetSampleRate(
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);

    *rate = mSampleRate;
    return NOERROR;
}

ECode CAudioRecord::GetAudioSource(
    /* [out] */ Int32* source)
{
    VALIDATE_NOT_NULL(source);

    *source = mRecordSource;
    return NOERROR;
}

ECode CAudioRecord::GetAudioFormat(
    /* [out] */ Int32* format)
{
    VALIDATE_NOT_NULL(format);

    *format = mAudioFormat;
    return NOERROR;
}

ECode CAudioRecord::GetChannelConfiguration(
    /* [out] */ Int32* configuration)
{
    VALIDATE_NOT_NULL(configuration);

    *configuration = mChannelConfiguration;
    return NOERROR;
}

ECode CAudioRecord::GetChannelCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = mChannelCount;
    return NOERROR;
}

ECode CAudioRecord::GetState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    *state = mState;
    return NOERROR;
}

ECode CAudioRecord::GetRecordingState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    *state = mRecordingState;
    return NOERROR;
}

ECode CAudioRecord::GetNotificationMarkerPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);

    *position = NativeGetMarkerPos();
    return NOERROR;
}

ECode CAudioRecord::GetPositionNotificationPeriod(
    /* [out] */ Int32* period)
{
    VALIDATE_NOT_NULL(period);

    *period = NativeGetPosUpdatePeriod();
    return NOERROR;
}

Int32 CAudioRecord::GetMinBufferSize(
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
        return AudioRecord_ERROR_BAD_VALUE;
    }

    // PCM_8BIT is not supported at the moment
    if (audioFormat != AudioFormat_ENCODING_PCM_16BIT) {
        //loge("getMinBufferSize(): Invalid audio format.");
        return AudioRecord_ERROR_BAD_VALUE;
    }

    Int32 size = NativeGetMinBuffSize(sampleRateInHz, channelCount, audioFormat);
    if (size == 0) {
        return AudioRecord_ERROR_BAD_VALUE;
    }
    else if (size == -1) {
        return AudioRecord_ERROR;
    }
    else {
        return size;
    }
}

ECode CAudioRecord::StartRecording()
{
    if (mState != AudioRecord_STATE_INITIALIZED) {
        /*throw(new IllegalStateException("startRecording() called on an "
                +"uninitialized AudioRecord."));*/
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // start recording
    {
        Mutex::Autolock lock(&mRecordingStateLock);

        if (NativeStart() == AudioRecord_SUCCESS) {
            mRecordingState = AudioRecord_RECORDSTATE_RECORDING;
        }
    }

    return NOERROR;
}

ECode CAudioRecord::Stop()
{
    if (mState != AudioRecord_STATE_INITIALIZED) {
        //throw(new IllegalStateException("stop() called on an uninitialized AudioRecord."));
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // stop recording
    {
        Mutex::Autolock lock(&mRecordingStateLock);

        NativeStop();
        mRecordingState = AudioRecord_RECORDSTATE_STOPPED;
    }

    return NOERROR;
}

ECode CAudioRecord::Read(
    /* [out] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (mState != AudioRecord_STATE_INITIALIZED) {
        *number = AudioRecord_ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0)
            || (offsetInBytes + sizeInBytes > audioData->GetLength())) {
        *number = AudioRecord_ERROR_BAD_VALUE;
        return NOERROR;
    }

    *number = NativeReadInByteArray(audioData, offsetInBytes, sizeInBytes);
    return NOERROR;
}

ECode CAudioRecord::ReadEx(
    /* [out] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (mState != AudioRecord_STATE_INITIALIZED) {
        *number = AudioRecord_ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioData == NULL) || (offsetInShorts < 0 ) || (sizeInShorts < 0)
            || (offsetInShorts + sizeInShorts > audioData->GetLength())) {
        *number = AudioRecord_ERROR_BAD_VALUE;
        return NOERROR;
    }

    *number = NativeReadInInt16Array(audioData, offsetInShorts, sizeInShorts);
    return NOERROR;
}

ECode CAudioRecord::ReadEx2(
    /* [in] */ IByteBuffer* audioBuffer,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

//    if (mState != STATE_INITIALIZED) {
//        return ERROR_INVALID_OPERATION;
//    }
//
//    if ( (audioBuffer == NULL) || (sizeInBytes < 0) ) {
//        return ERROR_BAD_VALUE;
//    }
//
//    return native_read_in_direct_buffer(audioBuffer, sizeInBytes);
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::SetRecordPositionUpdateListener(
    /* [in] */ IOnRecordPositionUpdateListener* listener)
{
    return SetRecordPositionUpdateListenerEx(listener, NULL);
}

ECode CAudioRecord::SetRecordPositionUpdateListenerEx(
    /* [in] */ IOnRecordPositionUpdateListener* listener,
    /* [in] */ IApartment* handler)
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

    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetMarkerPos(markerInFrames);
    return NOERROR;
}

ECode CAudioRecord::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetPosUpdatePeriod(periodInFrames);
    return NOERROR;
}

void CAudioRecord::PostEventFromNative(
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

struct audiorecord_callback_cookie
{
    CAudioRecord* mAudioRecordRef;  //weak reference
};

#define AUDIORECORD_SUCCESS                         0
#define AUDIORECORD_ERROR                           -1
#define AUDIORECORD_ERROR_BAD_VALUE                 -2
#define AUDIORECORD_ERROR_INVALID_OPERATION         -3
#define AUDIORECORD_ERROR_SETUP_ZEROFRAMECOUNT      -16
#define AUDIORECORD_ERROR_SETUP_INVALIDCHANNELMASK -17
#define AUDIORECORD_ERROR_SETUP_INVALIDFORMAT       -18
#define AUDIORECORD_ERROR_SETUP_INVALIDSOURCE       -19
#define AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED    -20

Int32 translateRecorderErrorCode(int code)
{
    switch(code) {
    case android::NO_ERROR:
        return AUDIORECORD_SUCCESS;
    case android::BAD_VALUE:
        return AUDIORECORD_ERROR_BAD_VALUE;
    case android::INVALID_OPERATION:
        return AUDIORECORD_ERROR_INVALID_OPERATION;
    default:
        return AUDIORECORD_ERROR;
    }
}

// ----------------------------------------------------------------------------
static void recorderCallback(int event, void* user, void *info)
{
    // if (event == AudioRecord::EVENT_MORE_DATA) {
    //     // set size to 0 to signal we're not using the callback to read more data
    //     AudioRecord::Buffer* pBuff = (AudioRecord::Buffer*)info;
    //     pBuff->size = 0;

    // } else if (event == AudioRecord::EVENT_MARKER) {
    //     audiorecord_callback_cookie *callbackInfo = (audiorecord_callback_cookie *)user;
    //     JNIEnv *env = AndroidRuntime::getJNIEnv();
    //     if (user && env) {
    //         env->CallStaticVoidMethod(
    //             callbackInfo->audioRecord_class,
    //             javaAudioRecordFields.postNativeEventInJava,
    //             callbackInfo->audioRecord_ref, event, 0,0, NULL);
    //         if (env->ExceptionCheck()) {
    //             env->ExceptionDescribe();
    //             env->ExceptionClear();
    //         }
    //     }

    // } else if (event == AudioRecord::EVENT_NEW_POS) {
    //     audiorecord_callback_cookie *callbackInfo = (audiorecord_callback_cookie *)user;
    //     JNIEnv *env = AndroidRuntime::getJNIEnv();
    //     if (user && env) {
    //         env->CallStaticVoidMethod(
    //             callbackInfo->audioRecord_class,
    //             javaAudioRecordFields.postNativeEventInJava,
    //             callbackInfo->audioRecord_ref, event, 0,0, NULL);
    //         if (env->ExceptionCheck()) {
    //             env->ExceptionDescribe();
    //             env->ExceptionClear();
    //         }
    //     }
    // }
}

Int32 CAudioRecord::NativeSetup(
    /* [in] */ Int32 recordSource,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channels,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 buffSizeInBytes)
{
    //LOGV(">> Entering android_media_AudioRecord_setup");
    //LOGV("sampleRate=%d, audioFormat=%d, channels=%x, buffSizeInBytes=%d",
    //     sampleRateInHertz, audioFormat, channels,     buffSizeInBytes);

    if (!android::AudioSystem::isInputChannel(channels)) {
        // LOGE("Error creating AudioRecord: channel count is not 1 or 2.");
        return AUDIORECORD_ERROR_SETUP_INVALIDCHANNELMASK;
    }
    uint32_t nbChannels = android::AudioSystem::popCount(channels);

    // compare the format against the Java constants
    if ((audioFormat != AudioFormat_ENCODING_PCM_16BIT)
        && (audioFormat != AudioFormat_ENCODING_PCM_8BIT)) {
        // LOGE("Error creating AudioRecord: unsupported audio format.");
        return AUDIORECORD_ERROR_SETUP_INVALIDFORMAT;
    }

    int bytesPerSample = audioFormat == AudioFormat_ENCODING_PCM_16BIT ? 2 : 1;
    int format = audioFormat == AudioFormat_ENCODING_PCM_16BIT ?
            android::AudioSystem::PCM_16_BIT : android::AudioSystem::PCM_8_BIT;

    if (buffSizeInBytes == 0) {
         // LOGE("Error creating AudioRecord: frameCount is 0.");
        return AUDIORECORD_ERROR_SETUP_ZEROFRAMECOUNT;
    }
    int frameSize = nbChannels * bytesPerSample;
    size_t frameCount = buffSizeInBytes / frameSize;

    if (recordSource >= android::AUDIO_SOURCE_LIST_END) {
        // LOGE("Error creating AudioRecord: unknown source.");
        return AUDIORECORD_ERROR_SETUP_INVALIDSOURCE;
    }

    audiorecord_callback_cookie *lpCallbackData = NULL;
    android::AudioRecord* lpRecorder = NULL;

    // create an uninitialized AudioRecord object
    lpRecorder = new android::AudioRecord();
        if(lpRecorder == NULL) {
        // LOGE("Error creating AudioRecord instance.");
        return AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED;
    }

    // create the callback information:
    // this data will be passed with every AudioRecord callback
    lpCallbackData = new audiorecord_callback_cookie;
    // we use a weak reference so the AudioRecord object can be garbage collected.
    // lpCallbackData->audioRecord_ref = env->NewGlobalRef(weak_this);
    lpCallbackData->mAudioRecordRef = this;

    lpRecorder->set(recordSource,
        sampleRate,
        format,        // word length, PCM
        channels,
        frameCount,
        0,             // flags
        recorderCallback,// callback_t
        lpCallbackData,// void* user
        0,             // notificationFrames,
        true);         // threadCanCallJava)

    if(lpRecorder->initCheck() != android::NO_ERROR) {
        // LOGE("Error creating AudioRecord instance: initialization check failed.");
        goto native_init_failure;
    }

    // save our newly created C++ AudioRecord in the "nativeRecorderInJavaObj" field
    // of the Java object
    mNativeRecorder = (Int32)lpRecorder;

    // save our newly created callback information in the "nativeCallbackCookie" field
    // of the Java object (in mNativeCallbackCookie) so we can free the memory in finalize()
    mNativeCallbackCookie = (Int32)lpCallbackData;

    return AUDIORECORD_SUCCESS;

    // failure:
native_init_failure:
    delete lpCallbackData;
    delete lpRecorder;

    mNativeRecorder = 0;
    mNativeCallbackCookie = 0;

    return AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED;
}

void CAudioRecord::NativeFinalize()
{
    NativeRelease();
}

void CAudioRecord::NativeRelease()
{
    // serialize access. Ugly, but functional.
    Mutex::Autolock lock(&_m_syncLock);

    android::AudioRecord* lpRecorder = (android::AudioRecord *)mNativeRecorder;
    audiorecord_callback_cookie* lpCookie = (audiorecord_callback_cookie *)mNativeCallbackCookie;

    // reset the native resources in the Java object so any attempt to access
    // them after a call to release fails.
    mNativeRecorder = 0;
    mNativeCallbackCookie = 0;

    // delete the AudioRecord object
    if (lpRecorder) {
        // LOGV("About to delete lpRecorder: %x\n", (int)lpRecorder);
        lpRecorder->stop();
        delete lpRecorder;
    }

    // delete the callback information
    if (lpCookie) {
        // LOGV("deleting lpCookie: %x\n", (int)lpCookie);
        // env->DeleteGlobalRef(lpCookie->audioRecord_class);
        // env->DeleteGlobalRef(lpCookie->audioRecord_ref);
        delete lpCookie;
    }
}

Int32 CAudioRecord::NativeStart()
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorder;
    if (lpRecorder == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return AUDIORECORD_ERROR;
    }

    return translateRecorderErrorCode(lpRecorder->start());
}

void CAudioRecord::NativeStop()
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorder;
    if (lpRecorder == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    lpRecorder->stop();
    //LOGV("Called lpRecorder->stop()");
}

Int32 CAudioRecord::NativeReadInByteArray(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes)
{
    Byte* recordBuff = NULL;
    android::AudioRecord *lpRecorder = NULL;

    // get the audio recorder from which we'll read new audio samples
    lpRecorder = (android::AudioRecord *)mNativeRecorder;
    if (lpRecorder == NULL) {
        // LOGE("Unable to retrieve AudioRecord object, can't record");
        return 0;
    }

    if (!audioData) {
        // LOGE("Invalid Java array to store recorded audio, can't record");
        return 0;
    }

    // get the pointer to where we'll record the audio
    recordBuff = audioData->GetPayload();

    if (recordBuff == NULL) {
        // LOGE("Error retrieving destination for recorded audio data, can't record");
        return 0;
    }

    // read the new audio data from the native AudioRecord object
    ssize_t recorderBuffSize = lpRecorder->frameCount() * lpRecorder->frameSize();
    ssize_t readSize = lpRecorder->read(recordBuff + offsetInBytes,
                                        sizeInBytes > (Int32)recorderBuffSize ?
                                            (Int32)recorderBuffSize : sizeInBytes );

    return (Int32)readSize;
}

Int32 CAudioRecord::NativeReadInInt16Array(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts)
{
    return (NativeReadInByteArray((ArrayOf<Byte>*)audioData,
            offsetInShorts * 2, sizeInShorts * 2) / 2);
}

Int32 CAudioRecord::NativeReadInDirectBuffer(
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 sizeInBytes)
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorder;
    //LOGV("Entering android_media_AudioRecord_readInBuffer");

    // get the audio recorder from which we'll read new audio samples
    if(lpRecorder == NULL) {
        return 0;
    }

    // direct buffer and direct access supported?
//    long capacity = env->GetDirectBufferCapacity(jBuffer);
//    if(capacity == -1) {
//        // buffer direct access is not supported
//        LOGE("Buffer direct access is not supported, can't record");
//        return 0;
//    }
//    //LOGV("capacity = %ld", capacity);
//    jbyte* nativeFromJavaBuf = (jbyte*) env->GetDirectBufferAddress(jBuffer);
//    if(nativeFromJavaBuf==NULL) {
//        LOGE("Buffer direct access is not supported, can't record");
//        return 0;
//    }
//
//    // read new data from the recorder
//    return (jint) lpRecorder->read(nativeFromJavaBuf,
//                                   capacity < sizeInBytes ? capacity : sizeInBytes);
    return 0;
}

Int32 CAudioRecord::NativeSetMarkerPos(
    /* [in] */ Int32 marker)
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorder;

    if (lpRecorder) {
        return translateRecorderErrorCode(lpRecorder->setMarkerPosition(marker));
    }
    else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for setMarkerPosition()");
        return AUDIORECORD_ERROR;
    }
}

Int32 CAudioRecord::NativeGetMarkerPos()
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorder;
    uint32_t markerPos = 0;

    if (lpRecorder) {
        lpRecorder->getMarkerPosition(&markerPos);
        return (Int32)markerPos;
    }
    else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for getMarkerPosition()");
        return AUDIORECORD_ERROR;
    }
}

Int32 CAudioRecord::NativeSetPosUpdatePeriod(
    /* [in] */ Int32 updatePeriod)
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorder;

    if (lpRecorder) {
        return translateRecorderErrorCode(lpRecorder->setPositionUpdatePeriod(updatePeriod));
    }
    else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for setPositionUpdatePeriod()");
        return AUDIORECORD_ERROR;
    }
}

Int32 CAudioRecord::NativeGetPosUpdatePeriod()
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorder;
    uint32_t period = 0;

    if (lpRecorder) {
        lpRecorder->getPositionUpdatePeriod(&period);
        return (Int32)period;
    }
    else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for getPositionUpdatePeriod()");
        return AUDIORECORD_ERROR;
    }
}

Int32 CAudioRecord::NativeGetMinBuffSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelCount,
    /* [in] */ Int32 audioFormat)
{
    // LOGV(">> android_media_AudioRecord_get_min_buff_size(%d, %d, %d)", sampleRateInHertz, nbChannels, audioFormat);

    int frameCount = 0;
    android::status_t result = android::AudioRecord::getMinFrameCount(&frameCount,
            sampleRateInHz,
            (audioFormat == AudioFormat_ENCODING_PCM_16BIT ?
                android::AudioSystem::PCM_16_BIT : android::AudioSystem::PCM_8_BIT),
            channelCount);

    if (result == android::BAD_VALUE) {
        return 0;
    }
    if (result != android::NO_ERROR) {
        return -1;
    }
    return frameCount * channelCount * (audioFormat == AudioFormat_ENCODING_PCM_16BIT ? 2 : 1);
}

void CAudioRecord::Logd(
    /* [in] */ const String& msg)
{
    //Log.d(TAG, "[ android.media.AudioRecord ] " + msg);
}

void CAudioRecord::Loge(
    /* [in] */ const String& msg)
{
    //Log.e(TAG, "[ android.media.AudioRecord ] " + msg);
}
