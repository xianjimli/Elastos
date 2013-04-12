
#include "media/CMediaRecorder.h"
#include "media/CCamcorderProfile.h"
const CString CMediaRecorder::TAG = "MediaRecorder";

CMediaRecorder::CMediaRecorder()
{

}

CMediaRecorder::~CMediaRecorder()
{
    native_finalize();
}

ECode CMediaRecorder::constructor()
{
    /*Looper looper;
    if ((looper = Looper.myLooper()) != NULL) {
        mEventHandler = new EventHandler(this, looper);
    } else if ((looper = Looper.getMainLooper()) != NULL) {
        mEventHandler = new EventHandler(this, looper);
    } else {
        mEventHandler = NULL;
    }*/

    /* Native setup requires a weak reference to our object.
     * It's easier to create it here than in C++.
     */
    //native_setup(new WeakReference<MediaRecorder>(this));
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetCamera(
    /* [in] */ ICamera* c)
{
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetPreviewDisplay(
    /* [in] */ ISurface* sv)
{
    mSurface = sv;

    return NOERROR;
}

PInterface CMediaRecorder::AudioSource::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContextMenuInfo) {
        return (IContextMenuInfo*)this;
    }

    return NULL;
}

UInt32 CMediaRecorder::AudioSource::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaRecorder::AudioSource::Release()
{
    return ElRefBase::Release();
}

ECode CMediaRecorder::AudioSource::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IContextMenuInfo*)this) {
        *pIID = EIID_IContextMenuInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CMediaRecorder::AudioSource::AudioSource()
    :DEFAULT(0)
    ,MIC(1)
    ,VOICE_UPLINK(2)
    ,VOICE_DOWNLINK(3)
    ,VOICE_CALL(4)
    ,CAMCORDER(5)
    ,VOICE_RECOGNITION(6)
    ,VOICE_COMMUNICATION(7)
{
}

PInterface CMediaRecorder::VideoSource::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContextMenuInfo) {
        return (IContextMenuInfo*)this;
    }

    return NULL;
}

UInt32 CMediaRecorder::VideoSource::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaRecorder::VideoSource::Release()
{
    return ElRefBase::Release();
}

ECode CMediaRecorder::VideoSource::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IContextMenuInfo*)this) {
        *pIID = EIID_IContextMenuInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CMediaRecorder::VideoSource::VideoSource()
    :DEFAULT(0)
    ,CAMERA(1)
{
}

PInterface CMediaRecorder::OutputFormat::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContextMenuInfo) {
        return (IContextMenuInfo*)this;
    }

    return NULL;
}

UInt32 CMediaRecorder::OutputFormat::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaRecorder::OutputFormat::Release()
{
    return ElRefBase::Release();
}

ECode CMediaRecorder::OutputFormat::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IContextMenuInfo*)this) {
        *pIID = EIID_IContextMenuInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CMediaRecorder::OutputFormat::OutputFormat()
        :DEFAULT(0)
        ,THREE_GPP(1)
        ,MPEG_4(2)
        ,RAW_AMR(3)
        ,AMR_NB(3)
        ,AMR_WB(4)
        ,AAC_ADIF(5)
        ,AAC_ADTS(6)
        ,OUTPUT_FORMAT_RTP_AVP(7)
        ,OUTPUT_FORMAT_MPEG2TS(8)
{
}

PInterface CMediaRecorder::AudioEncoder::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContextMenuInfo) {
        return (IContextMenuInfo*)this;
    }

    return NULL;
}

UInt32 CMediaRecorder::AudioEncoder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaRecorder::AudioEncoder::Release()
{
    return ElRefBase::Release();
}

ECode CMediaRecorder::AudioEncoder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IContextMenuInfo*)this) {
        *pIID = EIID_IContextMenuInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CMediaRecorder::AudioEncoder::AudioEncoder()
        :DEFAULT(0)
        ,AMR_NB(1)
        ,AMR_WB(2)
        ,AAC(3)
        ,AAC_PLUS(4)
        ,EAAC_PLUS(4)
{
}

PInterface CMediaRecorder::VideoEncoder::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContextMenuInfo) {
        return (IContextMenuInfo*)this;
    }

    return NULL;
}

UInt32 CMediaRecorder::VideoEncoder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaRecorder::VideoEncoder::Release()
{
    return ElRefBase::Release();
}

ECode CMediaRecorder::VideoEncoder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IContextMenuInfo*)this) {
        *pIID = EIID_IContextMenuInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CMediaRecorder::VideoEncoder::VideoEncoder()
        :DEFAULT(0)
        ,H263(1)
        ,H264(2)
        ,MPEG_4_SP(3)
{
}

ECode CMediaRecorder::SetAudioSource(
    /* [in] */ Int32 audioSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::GetAudioSourceMax(
    /* [out] */ Int32 * pMaximun)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetVideoSource(
    /* [in] */ Int32 videoSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}


ECode CMediaRecorder::SetProfile(
    /* [in] */ ICamcorderProfile* profile)
{
    SetOutputFormat(((CCamcorderProfile*)profile)->fileFormat);
    SetVideoFrameRate(((CCamcorderProfile*)profile)->videoFrameRate);
    SetVideoSize(((CCamcorderProfile*)profile)->videoFrameWidth, ((CCamcorderProfile*)profile)->videoFrameHeight);
    SetVideoEncodingBitRate(((CCamcorderProfile*)profile)->videoBitRate);
    SetAudioEncodingBitRate(((CCamcorderProfile*)profile)->audioBitRate);
    SetAudioChannels(((CCamcorderProfile*)profile)->audioChannels);
    SetAudioSamplingRate(((CCamcorderProfile*)profile)->audioSampleRate);
    SetVideoEncoder(((CCamcorderProfile*)profile)->videoCodec);
    SetAudioEncoder(((CCamcorderProfile*)profile)->audioCodec);

    return NOERROR;
}

ECode CMediaRecorder::SetOrientationHint(
    /* [in] */ Int32 degrees)
{
    if (degrees != 0   &&
        degrees != 90  &&
        degrees != 180 &&
        degrees != 270) {
        //throw new IllegalArgumentException("Unsupported angle: " + degrees);
    }
    //SetParameter(String.format("video-param-rotation-angle-degrees=%d", degrees));

    return NOERROR;
}

/**
 * Sets the format of the output file produced during recording. Call this
 * after setAudioSource()/setVideoSource() but before prepare().
 *
 * <p>It is recommended to always use 3GP format when using the H.263
 * video encoder and AMR audio encoder. Using an MPEG-4 container format
 * may confuse some desktop players.</p>
 *
 * @param output_format the output format to use. The output format
 * needs to be specified before setting recording-parameters or encoders.
 * @throws IllegalStateException if it is called after prepare() or before
 * setAudioSource()/setVideoSource().
 * @see android.media.MediaRecorder.OutputFormat
 */
ECode CMediaRecorder::SetOutputFormat(
    /* [in] */ Int32 outputFormat)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the width and height of the video to be captured.  Must be called
 * after setVideoSource(). Call this after setOutFormat() but before
 * prepare().
 *
 * @param width the width of the video to be captured
 * @param height the height of the video to be captured
 * @throws IllegalStateException if it is called after
 * prepare() or before setOutputFormat()
 */
ECode CMediaRecorder::SetVideoSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the frame rate of the video to be captured.  Must be called
 * after setVideoSource(). Call this after setOutFormat() but before
 * prepare().
 *
 * @param rate the number of frames per second of video to capture
 * @throws IllegalStateException if it is called after
 * prepare() or before setOutputFormat().
 *
 * NOTE: On some devices that have auto-frame rate, this sets the
 * maximum frame rate, not a constant frame rate. Actual frame rate
 * will vary according to lighting conditions.
 */
ECode CMediaRecorder::SetVideoFrameRate(
    /* [in] */ Int32 rate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the maximum duration (in ms) of the recording session.
 * Call this after setOutFormat() but before prepare().
 * After recording reaches the specified duration, a notification
 * will be sent to the {@link android.media.MediaRecorder.OnInfoListener}
 * with a "what" code of {@link #MEDIA_RECORDER_INFO_MAX_DURATION_REACHED}
 * and recording will be stopped. Stopping happens asynchronously, there
 * is no guarantee that the recorder will have stopped by the time the
 * listener is notified.
 *
 * @param max_duration_ms the maximum duration in ms (if zero or negative, disables the duration limit)
 *
 */
ECode CMediaRecorder::SetMaxDuration(
    /* [in] */ Int32 maxDurationMs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the maximum filesize (in bytes) of the recording session.
 * Call this after setOutFormat() but before prepare().
 * After recording reaches the specified filesize, a notification
 * will be sent to the {@link android.media.MediaRecorder.OnInfoListener}
 * with a "what" code of {@link #MEDIA_RECORDER_INFO_MAX_FILESIZE_REACHED}
 * and recording will be stopped. Stopping happens asynchronously, there
 * is no guarantee that the recorder will have stopped by the time the
 * listener is notified.
 *
 * @param max_filesize_bytes the maximum filesize in bytes (if zero or negative, disables the limit)
 *
 */
ECode CMediaRecorder::SetMaxFileSize(
    /* [in] */ Int64 maxFilesizeBytes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the audio encoder to be used for recording. If this method is not
 * called, the output file will not contain an audio track. Call this after
 * setOutputFormat() but before prepare().
 *
 * @param audio_encoder the audio encoder to use.
 * @throws IllegalStateException if it is called before
 * setOutputFormat() or after prepare().
 * @see android.media.MediaRecorder.AudioEncoder
 */
ECode CMediaRecorder::SetAudioEncoder(
    /* [in] */ Int32 audioEncoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the video encoder to be used for recording. If this method is not
 * called, the output file will not contain an video track. Call this after
 * setOutputFormat() and before prepare().
 *
 * @param video_encoder the video encoder to use.
 * @throws IllegalStateException if it is called before
 * setOutputFormat() or after prepare()
 * @see android.media.MediaRecorder.VideoEncoder
 */
ECode CMediaRecorder::SetVideoEncoder(
    /* [in] */ Int32 videoEncoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetAudioSamplingRate(
    /* [in] */ Int32 samplingRate)
{
    if (samplingRate <= 0) {
        //throw new IllegalArgumentException("Audio sampling rate is not positive");
    }
    //setParameter(String.format("audio-param-sampling-rate=%d", samplingRate));

    return NOERROR;
}

ECode CMediaRecorder::SetAudioChannels(
    /* [in] */ Int32 numChannels)
{
    if (numChannels <= 0) {
        //throw new IllegalArgumentException("Number of channels is not positive");
    }
    //setParameter(String.format("audio-param-number-of-channels=%d", numChannels));
    return NOERROR;
}

ECode CMediaRecorder::SetAudioEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    if (bitRate <= 0) {
        //throw new IllegalArgumentException("Audio encoding bit rate is not positive");
    }
    //setParameter(String.format("audio-param-encoding-bitrate=%d", bitRate));

    return NOERROR;
}

ECode CMediaRecorder::SetVideoEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    if (bitRate <= 0) {
        //throw new IllegalArgumentException("Video encoding bit rate is not positive");
    }
    //setParameter(String.format("video-param-encoding-bitrate=%d", bitRate));

    return NOERROR;
}

ECode CMediaRecorder::SetOutputFile(
    /* [in] */ IFileDescriptor* fd)
{
    mPath = NULL;
    mFd = fd;

    return NOERROR;
}

ECode CMediaRecorder::SetOutputFileEx(
    /* [in] */ const String& path)
{
    mFd = NULL;
    mPath = path;

    return NOERROR;
}

// native implementation
void CMediaRecorder::_setOutputFile(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{

}

void CMediaRecorder::_prepare()
{

}
ECode CMediaRecorder::Prepare()
{
    if (mPath != NULL) {
        //FileOutputStream fos = new FileOutputStream(mPath);
        //try {
           // _setOutputFile(fos.getFD(), 0, 0);
        /*} finally {
            fos.close();
        }*/
    } else if (mFd != NULL) {
        _setOutputFile(mFd, 0, 0);
    } else {
        //throw new IOException("No valid output file");
    }
    _prepare();

    return NOERROR;
}

/**
 * Begins capturing and encoding data to the file specified with
 * setOutputFile(). Call this after prepare().
 *
 * @throws IllegalStateException if it is called before
 * prepare().
 */
ECode CMediaRecorder::Start()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Stops recording. Call this after start(). Once recording is stopped,
 * you will have to configure it again as if it has just been constructed.
 *
 * @throws IllegalStateException if it is called before start()
 */
ECode CMediaRecorder::Stop()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::Reset()
{
    native_reset();

    // make sure none of the listeners get called anymore
    //mEventHandler->RemoveCallbacksAndMessages(NULL);

    return NOERROR;
}

void CMediaRecorder::native_reset()
{

}

ECode CMediaRecorder::GetMaxAmplitude(
    /* [out] */ Int32* maxAmplitude)
{
    return NOERROR;
}

ECode CMediaRecorder::SetOnErrorListener(
    /* [in] */ IMediaRecorderOnErrorListener* l)
{
    mOnErrorListener = l;

    return NOERROR;
}

ECode CMediaRecorder::SetOnInfoListener(
    /* [in] */ IMediaRecorderOnInfoListener* listener)
{
    mOnInfoListener = listener;

    return NOERROR;
}

//CMediaRecorder::EventHandler::EventHandler(
//    /* [in] */ MediaRecorder* mr,
//    /* [in] */ Looper looper)
//{
//    super(looper);
//    mMediaRecorder = mr;
//}

ECode CMediaRecorder::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (mMediaRecorder->mNativeContext == 0) {
        //Log.w(TAG, "mediarecorder went away with unhandled events");
        return NOERROR;
    }
    //switch(((CMessage*)msg)->what) {
    //case MEDIA_RECORDER_EVENT_ERROR:
    //    if (mMediaRecorder->mOnErrorListener != NULL)
    //        //mOnErrorListener.onError(mMediaRecorder, msg.arg1, msg.arg2);

    //    return NOERROR;

    //case MEDIA_RECORDER_EVENT_INFO:
    //    if (mMediaRecorder->mOnInfoListener != NULL)
    //        //mOnInfoListener.onInfo(mMediaRecorder, msg.arg1, msg.arg2);

    //    return NOERROR;

    //default:
    //    //Log.e(TAG, "Unknown message type " + msg.what);
    //    return NOERROR;
    //}
}

PInterface CMediaRecorder::EventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CMediaRecorder::EventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaRecorder::EventHandler::Release()
{
    return ElRefBase::Release();
}

ECode CMediaRecorder::EventHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

void CMediaRecorder::PostEventFromNative(
    /* [in] */ IInterface* mediarecorder_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    /*MediaRecorder mr = (MediaRecorder)((WeakReference)mediarecorder_ref).get();
    if (mr == NULL) {
        return;
    }

    if (mr.mEventHandler != NULL) {
        Message m = mr.mEventHandler.obtainMessage(what, arg1, arg2, obj);
        mr.mEventHandler.sendMessage(m);
    }*/
}

ECode CMediaRecorder::ReleaseResources()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}


void CMediaRecorder::native_setup(
    /* [in] */ IInterface* mediarecorder_this)
{

}

void CMediaRecorder::native_finalize()
{

}

void CMediaRecorder::SetParameter(
    /* [in] */ String nameValuePair)
{

}