
#include "media/CMediaRecorder.h"


ECode CMediaRecorder::SetCamera(
        /* [in] */ ICamera* c)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetPreviewDisplay(
    /* [in] */ ISurface * pSv)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

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
ECode CMediaRecorder::SetAudioSource(
    /* [in] */ Int32 audioSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaRecorder::SetVideoSource(
    /* [in] */ Int32 videoSource)
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

ECode CMediaRecorder::SetProfile(
    /* [in] */ ICamcorderProfile * pProfile)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetOrientationHint(
    /* [in] */ Int32 degrees)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetAudioChannels(
    /* [in] */ Int32 numChannels)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetAudioEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetVideoEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetOutputFile(
    /* [in] */ IFileDescriptor * pFd)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetOutputFileEx(
    /* [in] */ const String& path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::Prepare()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::GetMaxAmplitude(
    /* [out] */ Int32* maxAmplitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetOnErrorListener(
    /* [in] */ IMediaRecorderOnErrorListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::SetOnInfoListener(
    /* [in] */ IMediaRecorderOnInfoListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::ReleaseResources()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaRecorder::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

