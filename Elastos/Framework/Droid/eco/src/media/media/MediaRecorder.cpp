
#include "media/MediaRecorder.h"
#include "media/CCamcorderProfile.h"

/**
 * Default constructor.
 */
MediaRecorder::MediaRecorder()
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
}

/**
 * Sets a Camera to use for recording. Use this function to switch
 * quickly between preview and capture mode without a teardown of
 * the camera object. Must call before prepare().
 *
 * @param c the Camera to use for recording
 */
void MediaRecorder::SetCamera(
    /* [in] */ ICamera* c)
{

}

/**
 * Sets a Surface to show a preview of recorded media (video). Calls this
 * before prepare() to make sure that the desirable preview display is
 * set.
 *
 * @param sv the Surface to use for the preview
 */
ECode MediaRecorder::SetPreviewDisplay(
    /* [in] */ ISurface* sv)
{
    mSurface = sv;

    return NOERROR;
}

/**
 * Defines the audio source. These constants are used with
 * {@link MediaRecorder#setAudioSource(Int32)}.
 */

/* Do not change these values without updating their counterparts
 * in include/media/mediarecorder.h!
 */
MediaRecorder::AudioSource::AudioSource() 
{}


/**
 * Defines the video source. These constants are used with
 * {@link MediaRecorder#setVideoSource(Int32)}.
 */
/* Do not change these values without updating their counterparts
 * in include/media/mediarecorder.h!
 */
MediaRecorder::VideoSource::VideoSource()
{}


/**
 * Defines the output format. These constants are used with
 * {@link MediaRecorder#setOutputFormat(Int32)}.
 */

/* Do not change these values without updating their counterparts
 * in include/media/mediarecorder.h!
 */
MediaRecorder::OutputFormat::OutputFormat() {}

/**
 * Defines the audio encoding. These constants are used with
 * {@link MediaRecorder#setAudioEncoder(Int32)}.
 */

/* Do not change these values without updating their counterparts
 * in include/media/mediarecorder.h!
 */
MediaRecorder::AudioEncoder::AudioEncoder() {}


/**
 * Defines the video encoding. These constants are used with
 * {@link MediaRecorder#setVideoEncoder(Int32)}.
 */

/* Do not change these values without updating their counterparts
 * in include/media/mediarecorder.h!
 */
MediaRecorder::VideoEncoder::VideoEncoder() {}


/**
 * Sets the audio source to be used for recording. If this method is not
 * called, the output file will not contain an audio track. The source needs
 * to be specified before setting recording-parameters or encoders. Call
 * this only before setOutputFormat().
 *
 * @param audio_source the audio source to use
 * @throws IllegalStateException if it is called after setOutputFormat()
 * @see android.media.MediaRecorder.AudioSource
 */
void MediaRecorder::SetAudioSource(
    /* [in] */ Int32 audio_source)
{

}

/**
 * Gets the maximum value for audio sources.
 * @see android.media.MediaRecorder.AudioSource
 */
Int32 MediaRecorder::GetAudioSourceMax()
{ 
    return AudioSource::VOICE_RECOGNITION;
}

/**
 * Sets the video source to be used for recording. If this method is not
 * called, the output file will not contain an video track. The source needs
 * to be specified before setting recording-parameters or encoders. Call
 * this only before setOutputFormat().
 *
 * @param video_source the video source to use
 * @throws IllegalStateException if it is called after setOutputFormat()
 * @see android.media.MediaRecorder.VideoSource
 */
void MediaRecorder::SetVideoSource(
    /* [in] */ Int32 video_source)
{

}

/**
 * Uses the settings from a CamcorderProfile object for recording. This method should
 * be called after the video AND audio sources are set, and before setOutputFile().
 *
 * @param profile the CamcorderProfile to use
 * @see android.media.CamcorderProfile
 */
ECode MediaRecorder::SetProfile(
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

/**
 * Sets the orientation hint for output video playback.
 * This method should be called before prepare(). This method will not
 * trigger the source video frame to rotate during video recording, but to
 * add a composition matrix containing the rotation angle in the output
 * video if the output format is OutputFormat.THREE_GPP or
 * OutputFormat.MPEG_4 so that a video player can choose the proper
 * orientation for playback. Note that some video players may choose
 * to ignore the compostion matrix in a video during playback.
 *
 * @param degrees the angle to be rotated clockwise in degrees.
 * The supported angles are 0, 90, 180, and 270 degrees.
 * @throws IllegalArgumentException if the angle is not supported.
 *
 */
ECode MediaRecorder::SetOrientationHint(
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
void MediaRecorder::SetOutputFormat(
    /* [in] */ Int32 output_format)
{

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
void MediaRecorder::SetVideoSize(
    /* [in] */ Int32 width, 
    /* [in] */ Int32 height)
{

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
void MediaRecorder::SetVideoFrameRate(
    /* [in] */ Int32 rate)
{

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
void MediaRecorder::SetMaxDuration(
    /* [in] */ Int32 max_duration_ms)
{

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
void MediaRecorder::SetMaxFileSize(
    /* [in] */ Int64 max_filesize_bytes)
{

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
void MediaRecorder::SetAudioEncoder(
    /* [in] */ Int32 audio_encoder)
{

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
void MediaRecorder::SetVideoEncoder(
    /* [in] */ Int32 video_encoder)
{

}

/**
 * Sets the audio sampling rate for recording. Call this method before prepare().
 * Prepare() may perform additional checks on the parameter to make sure whether
 * the specified audio sampling rate is applicable. The sampling rate really depends
 * on the format for the audio recording, as well as the capabilities of the platform.
 * For instance, the sampling rate supported by AAC audio coding standard ranges
 * from 8 to 96 kHz. Please consult with the related audio coding standard for the
 * supported audio sampling rate.
 *
 * @param samplingRate the sampling rate for audio in samples per second.
 */
ECode MediaRecorder::SetAudioSamplingRate(
    /* [in] */ Int32 samplingRate)
{
    if (samplingRate <= 0) {
        //throw new IllegalArgumentException("Audio sampling rate is not positive");
    }
    //setParameter(String.format("audio-param-sampling-rate=%d", samplingRate));

    return NOERROR;
}

/**
 * Sets the number of audio channels for recording. Call this method before prepare().
 * Prepare() may perform additional checks on the parameter to make sure whether the
 * specified number of audio channels are applicable.
 *
 * @param numChannels the number of audio channels. Usually it is either 1 (mono) or 2
 * (stereo).
 */
ECode MediaRecorder::SetAudioChannels(
    /* [in] */ Int32 numChannels)
{
    if (numChannels <= 0) {
        //throw new IllegalArgumentException("Number of channels is not positive");
    }
    //setParameter(String.format("audio-param-number-of-channels=%d", numChannels));
    return NOERROR;
}

/**
 * Sets the audio encoding bit rate for recording. Call this method before prepare().
 * Prepare() may perform additional checks on the parameter to make sure whether the
 * specified bit rate is applicable, and sometimes the passed bitRate will be clipped
 * internally to ensure the audio recording can proceed smoothly based on the
 * capabilities of the platform.
 *
 * @param bitRate the audio encoding bit rate in bits per second.
 */
ECode MediaRecorder::SetAudioEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    if (bitRate <= 0) {
        //throw new IllegalArgumentException("Audio encoding bit rate is not positive");
    }
    //setParameter(String.format("audio-param-encoding-bitrate=%d", bitRate));

    return NOERROR;
}

/**
 * Sets the video encoding bit rate for recording. Call this method before prepare().
 * Prepare() may perform additional checks on the parameter to make sure whether the
 * specified bit rate is applicable, and sometimes the passed bitRate will be
 * clipped internally to ensure the video recording can proceed smoothly based on
 * the capabilities of the platform.
 *
 * @param bitRate the video encoding bit rate in bits per second.
 */
ECode MediaRecorder::SetVideoEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    if (bitRate <= 0) {
        //throw new IllegalArgumentException("Video encoding bit rate is not positive");
    }
    //setParameter(String.format("video-param-encoding-bitrate=%d", bitRate));

    return NOERROR;
}

/**
 * Pass in the file descriptor of the file to be written. Call this after
 * setOutputFormat() but before prepare().
 *
 * @param fd an open file descriptor to be written into.
 * @throws IllegalStateException if it is called before
 * setOutputFormat() or after prepare()
 */
ECode MediaRecorder::SetOutputFile(
    /* [in] */ IFileDescriptor* fd)
{
    mPath = NULL;
    mFd = fd;

    return NOERROR;
}

/**
 * Sets the path of the output file to be produced. Call this after
 * setOutputFormat() but before prepare().
 *
 * @param path The pathname to use.
 * @throws IllegalStateException if it is called before
 * setOutputFormat() or after prepare()
 */
ECode MediaRecorder::SetOutputFile(
    /* [in] */ String path)
{
    mFd = NULL;
    mPath = path;

    return NOERROR;
}

// native implementation
void MediaRecorder::_setOutputFile(
    /* [in] */ IFileDescriptor* fd, 
    /* [in] */ Int64 offset, 
    /* [in] */ Int64 length)
{

}

void MediaRecorder::_prepare()
{

}

/**
 * Prepares the recorder to begin capturing and encoding data. This method
 * must be called after setting up the desired audio and video sources,
 * encoders, file format, etc., but before start().
 *
 * @throws IllegalStateException if it is called after
 * start() or before setOutputFormat().
 * @throws IOException if prepare fails otherwise.
 */
ECode MediaRecorder::Prepare()
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
void MediaRecorder::Start()
{

}

/**
 * Stops recording. Call this after start(). Once recording is stopped,
 * you will have to configure it again as if it has just been constructed.
 *
 * @throws IllegalStateException if it is called before start()
 */
void MediaRecorder::Stop()
{

}

/**
 * Restarts the MediaRecorder to its idle state. After calling
 * this method, you will have to configure it again as if it had just been
 * constructed.
 */
ECode MediaRecorder::Reset()
{
    native_reset();

    // make sure none of the listeners get called anymore
    //mEventHandler->RemoveCallbacksAndMessages(NULL);

    return NOERROR;
}

void MediaRecorder::native_reset()
{

}

/**
 * Returns the maximum absolute amplitude that was sampled since the last
 * call to this method. Call this only after the setAudioSource().
 *
 * @return the maximum absolute amplitude measured since the last call, or
 * 0 when called for the first time
 * @throws IllegalStateException if it is called before
 * the audio source has been set.
 */
Int32 MediaRecorder::GetMaxAmplitude()
{
    return 0;
}

/**
 * Register a callback to be invoked when an error occurs while
 * recording.
 *
 * @param l the callback that will be run
 */
ECode MediaRecorder::SetOnErrorListener(
    /* [in] */ IOnErrorListener* l)
{
    mOnErrorListener = l;

    return 0;
}

/**
 * Register a callback to be invoked when an informational event occurs while
 * recording.
 *
 * @param listener the callback that will be run
 */
ECode MediaRecorder::SetOnInfoListener(
    /* [in] */ IOnInfoListener* listener)
{
    mOnInfoListener = listener;

    return NOERROR;
}

//MediaRecorder::EventHandler::EventHandler(
//    /* [in] */ MediaRecorder* mr, 
//    /* [in] */ Looper looper) 
//{
//    super(looper);
//    mMediaRecorder = mr;
//}

ECode MediaRecorder::EventHandler::HandleMessage(
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

PInterface MediaRecorder::EventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 MediaRecorder::EventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MediaRecorder::EventHandler::Release()
{
    return ElRefBase::Release();
}

ECode MediaRecorder::EventHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Called from native code when an interesting event happens.  This method
 * just uses the EventHandler system to post the event back to the main app thread.
 * We use a weak reference to the original MediaRecorder object so that the native
 * code is safe from the object disappearing from underneath it.  (This is
 * the cookie passed to native_setup().)
 */
void MediaRecorder::PostEventFromNative(
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

/**
 * Releases resources associated with this MediaRecorder object.
 * It is good practice to call this method when you're done
 * using the MediaRecorder.
 */
void MediaRecorder::Release()
{

}

void MediaRecorder::native_init()
{

}

void MediaRecorder::native_setup(
    /* [in] */ IInterface* mediarecorder_this)
{

}

void MediaRecorder::native_finalize()
{

}

void MediaRecorder::SetParameter(
    /* [in] */ String nameValuePair)
{

}

void MediaRecorder::Finalize() 
{ 
    native_finalize(); 
}
