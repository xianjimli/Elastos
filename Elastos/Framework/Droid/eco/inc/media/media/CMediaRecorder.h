
#ifndef __CMEDIARECORDER_H__
#define __CMEDIARECORDER_H__

#include "_CMediaRecorder.h"

#include "media/MediaRecorder.h"

CarClass(CMediaRecorder), public MediaRecorder
{
public:
    /**
     * Sets a Camera to use for recording. Use this function to switch
     * quickly between preview and capture mode without a teardown of
     * the camera object. Must call before prepare().
     *
     * @param c the Camera to use for recording
     */
    CARAPI SetCamera(
        /* [in] */ ICamera* c);

    CARAPI SetPreviewDisplay(
        /* [in] */ ISurface * pSv);

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
    CARAPI SetAudioSource(
        /* [in] */ Int32 audioSource);

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
    CARAPI SetVideoSource(
        /* [in] */ Int32 videoSource);

    CARAPI GetAudioSourceMax(
        /* [out] */ Int32 * pMaximun);

    CARAPI SetProfile(
        /* [in] */ ICamcorderProfile * pProfile);

    CARAPI SetOrientationHint(
        /* [in] */ Int32 degrees);

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
    CARAPI SetOutputFormat(
        /* [in] */ Int32 outputFormat);

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
    CARAPI SetVideoSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

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
    CARAPI SetVideoFrameRate(
        /* [in] */ Int32 rate);

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
    CARAPI SetMaxDuration(
        /* [in] */ Int32 maxDurationMs);

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
    CARAPI SetMaxFileSize(
        /* [in] */ Int64 maxFilesizeBytes);

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
    CARAPI SetAudioEncoder(
        /* [in] */ Int32 audioEncoder);

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
    CARAPI SetVideoEncoder(
        /* [in] */ Int32 videoEncoder);

    CARAPI SetAudioSamplingRate(
        /* [in] */ Int32 samplingRate);

    CARAPI SetAudioChannels(
        /* [in] */ Int32 numChannels);

    CARAPI SetAudioEncodingBitRate(
        /* [in] */ Int32 bitRate);

    CARAPI SetVideoEncodingBitRate(
        /* [in] */ Int32 bitRate);

    CARAPI SetOutputFile(
        /* [in] */ IFileDescriptor * pFd);

    CARAPI SetOutputFileEx(
        /* [in] */ const String& path);

    CARAPI Prepare();

    /**
     * Begins capturing and encoding data to the file specified with
     * setOutputFile(). Call this after prepare().
     *
     * @throws IllegalStateException if it is called before
     * prepare().
     */
    CARAPI Start();

    /**
     * Stops recording. Call this after start(). Once recording is stopped,
     * you will have to configure it again as if it has just been constructed.
     *
     * @throws IllegalStateException if it is called before start()
     */
    CARAPI Stop();

    CARAPI Reset();

    /**
     * Returns the maximum absolute amplitude that was sampled since the last
     * call to this method. Call this only after the setAudioSource().
     *
     * @return the maximum absolute amplitude measured since the last call, or
     * 0 when called for the first time
     * @throws IllegalStateException if it is called before
     * the audio source has been set.
     */
    CARAPI GetMaxAmplitude(
        /* [out] */ Int32* maxAmplitude);

    CARAPI SetOnErrorListener(
        /* [in] */ IMediaRecorderOnErrorListener * pL);

    CARAPI SetOnInfoListener(
        /* [in] */ IMediaRecorderOnInfoListener * pListener);

    /**
     * Releases resources associated with this MediaRecorder object.
     * It is good practice to call this method when you're done
     * using the MediaRecorder.
     */
    CARAPI ReleaseResources();

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CMEDIARECORDER_H__
