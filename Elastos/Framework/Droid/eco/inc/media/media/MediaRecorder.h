
#ifndef __MEDIARECORDER_H__
#define __MEDIARECORDER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * Used to record audio and video. The recording control is based on a
 * simple state machine (see below).
 *
 * <p><img src="{@docRoot}images/mediarecorder_state_diagram.gif" border="0" />
 * </p>
 *
 * <p>A common case of using MediaRecorder to record audio works as follows:
 *
 * <pre>MediaRecorder recorder = new MediaRecorder();
 * recorder.setAudioSource(MediaRecorder.AudioSource.MIC);
 * recorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
 * recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
 * recorder.setOutputFile(PATH_NAME);
 * recorder.prepare();
 * recorder.start();   // Recording is now started
 * ...
 * recorder.stop();
 * recorder.reset();   // You can reuse the object by going back to setAudioSource() step
 * recorder.release(); // Now the object cannot be reused
 * </pre>
 *
 * <p>See the <a href="{@docRoot}guide/topics/media/index.html">Audio and Video</a>
 * documentation for additional help with using MediaRecorder.
 * <p>Note: Currently, MediaRecorder does not work on the emulator.
 */
class MediaRecorder
{
public:
    /*static {
        System.loadLibrary("media_jni");
        native_init();
    }*/


    /**
     * Default constructor.
     */
    MediaRecorder();

    /**
     * Sets a Camera to use for recording. Use this function to switch
     * quickly between preview and capture mode without a teardown of
     * the camera object. Must call before prepare().
     *
     * @param c the Camera to use for recording
     */
    virtual CARAPI SetCamera(
        /* [in] */ ICamera* c);

    /**
     * Sets a Surface to show a preview of recorded media (video). Calls this
     * before prepare() to make sure that the desirable preview display is
     * set.
     *
     * @param sv the Surface to use for the preview
     */
    virtual CARAPI SetPreviewDisplay(
        /* [in] */ ISurface* sv);

    /**
     * Defines the audio source. These constants are used with
     * {@link MediaRecorder#setAudioSource(Int32)}.
     */
    class AudioSource
    {
    public:
        static const Int32 DEFAULT = 0;

        /** Microphone audio source */
        static const Int32 MIC = 1;

        /** Voice call uplink (Tx) audio source */
        static const Int32 VOICE_UPLINK = 2;

        /** Voice call downlink (Rx) audio source */
        static const Int32 VOICE_DOWNLINK = 3;

        /** Voice call uplink + downlink audio source */
        static const Int32 VOICE_CALL = 4;

        /** Microphone audio source with same orientation as camera if available, the main
         *  device microphone otherwise */
        static const Int32 CAMCORDER = 5;

        /** Microphone audio source tuned for voice recognition if available, behaves like
         *  {@link #DEFAULT} otherwise. */
        static const Int32 VOICE_RECOGNITION = 6;

        /**
         * @hide
         * Microphone audio source tuned for voice communications such as VoIP. It
         * will for instance take advantage of echo cancellation or automatic gain control
         * if available. It otherwise behaves like {@link #DEFAULT} if no voice processing
         * is available.
         */
        static const Int32 VOICE_COMMUNICATION = 7;

    private:
        /* Do not change these values without updating their counterparts
         * in include/media/mediarecorder.h!
         */
        AudioSource();
    };

    /**
     * Defines the video source. These constants are used with
     * {@link MediaRecorder#setVideoSource(Int32)}.
     */
    class VideoSource
    {
    public:
        static const Int32 DEFAULT = 0;

        /** Camera video source */
        static const Int32 CAMERA = 1;

    private:
        /* Do not change these values without updating their counterparts
         * in include/media/mediarecorder.h!
         */
        VideoSource();
    };

    /**
     * Defines the output format. These constants are used with
     * {@link MediaRecorder#setOutputFormat(Int32)}.
     */
    class OutputFormat
    {
    public:
        static const Int32 DEFAULT = 0;

        /** 3GPP media file format*/
        static const Int32 THREE_GPP = 1;

        /** MPEG4 media file format*/
        static const Int32 MPEG_4 = 2;

        /** The following formats are audio only .aac or .amr formats **/
        /** @deprecated  Deprecated in favor of AMR_NB */
        /** Deprecated in favor of MediaRecorder.OutputFormat.AMR_NB */
        /** AMR NB file format */
        static const Int32 RAW_AMR = 3;

        /** AMR NB file format */
        static const Int32 AMR_NB = 3;

        /** AMR WB file format */
        static const Int32 AMR_WB = 4;

        /** @hide AAC ADIF file format */
        static const Int32 AAC_ADIF = 5;

        /** @hide AAC ADTS file format */
        static const Int32 AAC_ADTS = 6;

        /** @hide Stream over a socket, limited to a single stream */
        static const Int32 OUTPUT_FORMAT_RTP_AVP = 7;

        /** @hide H.264/AAC data encapsulated in MPEG2/TS */
        static const Int32 OUTPUT_FORMAT_MPEG2TS = 8;

    private:
        /* Do not change these values without updating their counterparts
         * in include/media/mediarecorder.h!
         */
        OutputFormat();
    };

    /**
     * Defines the audio encoding. These constants are used with
     * {@link MediaRecorder#setAudioEncoder(Int32)}.
     */
    class AudioEncoder
    {
    public:
        static const Int32 DEFAULT = 0;

        /** AMR (Narrowband) audio codec */
        static const Int32 AMR_NB = 1;

        /** AMR (Wideband) audio codec */
        static const Int32 AMR_WB = 2;

        /** AAC audio codec */
        static const Int32 AAC = 3;

        /** @hide enhanced AAC audio codec */
        static const Int32 AAC_PLUS = 4;

        /** @hide enhanced AAC plus audio codec */
        static const Int32 EAAC_PLUS = 5;

    private:
        /* Do not change these values without updating their counterparts
         * in include/media/mediarecorder.h!
         */
        AudioEncoder();
    };

    /**
     * Defines the video encoding. These constants are used with
     * {@link MediaRecorder#setVideoEncoder(Int32)}.
     */
    class VideoEncoder
    {
    public:
        static const Int32 DEFAULT = 0;
        static const Int32 H263 = 1;
        static const Int32 H264 = 2;
        static const Int32 MPEG_4_SP = 3;

    private:
        /* Do not change these values without updating their counterparts
         * in include/media/mediarecorder.h!
         */
        VideoEncoder();
    };

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
    CARAPI_(void) SetAudioSource(
        /* [in] */ Int32 audio_source);

    /**
     * Gets the maximum value for audio sources.
     * @see android.media.MediaRecorder.AudioSource
     */
    CARAPI_(Int32) GetAudioSourceMax();

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
    CARAPI_(void) SetVideoSource(
        /* [in] */ Int32 video_source);

    /**
     * Uses the settings from a CamcorderProfile object for recording. This method should
     * be called after the video AND audio sources are set, and before setOutputFile().
     *
     * @param profile the CamcorderProfile to use
     * @see android.media.CamcorderProfile
     */
    virtual CARAPI SetProfile(
        /* [in] */ ICamcorderProfile* profile);

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
    virtual CARAPI SetOrientationHint(
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
    CARAPI_(void) SetOutputFormat(
        /* [in] */ Int32 output_format);

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
    CARAPI_(void) SetVideoSize(
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
    CARAPI_(void) SetVideoFrameRate(
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
    CARAPI_(void) SetMaxDuration(
        /* [in] */ Int32 max_duration_ms);

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
    CARAPI_(void) SetMaxFileSize(
        /* [in] */ Int64 max_filesize_bytes);

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
    CARAPI_(void) SetAudioEncoder(
        /* [in] */ Int32 audio_encoder);

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
    CARAPI_(void) SetVideoEncoder(
        /* [in] */ Int32 video_encoder);

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
    virtual CARAPI SetAudioSamplingRate(
        /* [in] */ Int32 samplingRate);

    /**
     * Sets the number of audio channels for recording. Call this method before prepare().
     * Prepare() may perform additional checks on the parameter to make sure whether the
     * specified number of audio channels are applicable.
     *
     * @param numChannels the number of audio channels. Usually it is either 1 (mono) or 2
     * (stereo).
     */
    virtual CARAPI SetAudioChannels(
        /* [in] */ Int32 numChannels);

    /**
     * Sets the audio encoding bit rate for recording. Call this method before prepare().
     * Prepare() may perform additional checks on the parameter to make sure whether the
     * specified bit rate is applicable, and sometimes the passed bitRate will be clipped
     * internally to ensure the audio recording can proceed smoothly based on the
     * capabilities of the platform.
     *
     * @param bitRate the audio encoding bit rate in bits per second.
     */
    virtual CARAPI SetAudioEncodingBitRate(
        /* [in] */ Int32 bitRate);

    /**
     * Sets the video encoding bit rate for recording. Call this method before prepare().
     * Prepare() may perform additional checks on the parameter to make sure whether the
     * specified bit rate is applicable, and sometimes the passed bitRate will be
     * clipped internally to ensure the video recording can proceed smoothly based on
     * the capabilities of the platform.
     *
     * @param bitRate the video encoding bit rate in bits per second.
     */
    virtual CARAPI SetVideoEncodingBitRate(
        /* [in] */ Int32 bitRate);

    /**
     * Pass in the file descriptor of the file to be written. Call this after
     * setOutputFormat() but before prepare().
     *
     * @param fd an open file descriptor to be written into.
     * @throws IllegalStateException if it is called before
     * setOutputFormat() or after prepare()
     */
    virtual CARAPI SetOutputFile(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Sets the path of the output file to be produced. Call this after
     * setOutputFormat() but before prepare().
     *
     * @param path The pathname to use.
     * @throws IllegalStateException if it is called before
     * setOutputFormat() or after prepare()
     */
    virtual CARAPI SetOutputFile(
        /* [in] */ String path);

    // native implementation
    CARAPI_(void) _setOutputFile(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 length);

    CARAPI_(void) _prepare();

    /**
     * Prepares the recorder to begin capturing and encoding data. This method
     * must be called after setting up the desired audio and video sources,
     * encoders, file format, etc., but before start().
     *
     * @throws IllegalStateException if it is called after
     * start() or before setOutputFormat().
     * @throws IOException if prepare fails otherwise.
     */
    virtual CARAPI Prepare();

    /**
     * Begins capturing and encoding data to the file specified with
     * setOutputFile(). Call this after prepare().
     *
     * @throws IllegalStateException if it is called before
     * prepare().
     */
    CARAPI_(void) Start();

    /**
     * Stops recording. Call this after start(). Once recording is stopped,
     * you will have to configure it again as if it has just been constructed.
     *
     * @throws IllegalStateException if it is called before start()
     */
    CARAPI_(void) Stop();

    /**
     * Restarts the MediaRecorder to its idle state. After calling
     * this method, you will have to configure it again as if it had just been
     * constructed.
     */
    virtual CARAPI Reset();

    CARAPI_(void) native_reset();

    /**
     * Returns the maximum absolute amplitude that was sampled since the last
     * call to this method. Call this only after the setAudioSource().
     *
     * @return the maximum absolute amplitude measured since the last call, or
     * 0 when called for the first time
     * @throws IllegalStateException if it is called before
     * the audio source has been set.
     */
    CARAPI_(Int32) GetMaxAmplitude();

    /**
     * Register a callback to be invoked when an error occurs while
     * recording.
     *
     * @param l the callback that will be run
     */
    virtual CARAPI SetOnErrorListener(
        /* [in] */ IMediaRecorderOnErrorListener* l);

    /**
     * Register a callback to be invoked when an informational event occurs while
     * recording.
     *
     * @param listener the callback that will be run
     */
    virtual CARAPI SetOnInfoListener(
        /* [in] */ IMediaRecorderOnInfoListener* listener);

    class EventHandler : public IHandler, public ElRefBase
    {
    public:
        //EventHandler(
        //    /* [in] */ MediaRecorder* mr,
        //    /* [in] */ ILooper* looper);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        MediaRecorder* mMediaRecorder;

        /* Do not change these values without updating their counterparts
         * in include/media/mediarecorder.h!
         */
        static const Int32 MEDIA_RECORDER_EVENT_ERROR = 1;
        static const Int32 MEDIA_RECORDER_EVENT_INFO  = 2;
    };

    /**
     * Called from native code when an interesting event happens.  This method
     * just uses the EventHandler system to post the event back to the main app thread.
     * We use a weak reference to the original MediaRecorder object so that the native
     * code is safe from the object disappearing from underneath it.  (This is
     * the cookie passed to native_setup().)
     */
    static CARAPI_(void) PostEventFromNative(
        /* [in] */ IInterface* mediarecorder_ref,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    /**
     * Releases resources associated with this MediaRecorder object.
     * It is good practice to call this method when you're done
     * using the MediaRecorder.
     */
    CARAPI_(void) Release();

    CARAPI_(void) native_init();

    CARAPI_(void) native_setup(
        /* [in] */ IInterface* mediarecorder_this);

    CARAPI_(void) native_finalize();

    CARAPI_(void) SetParameter(
        /* [in] */ String nameValuePair);

    CARAPI_(void) Finalize();

public:
    /* Do not change this value without updating its counterpart
     * in include/media/mediarecorder.h!
     */
    /** Unspecified media recorder error.
     * @see android.media.MediaRecorder.OnErrorListener
     */
    static const Int32 MEDIA_RECORDER_ERROR_UNKNOWN = 1;

    /* Do not change these values without updating their counterparts
     * in include/media/mediarecorder.h!
     */
    /** Unspecified media recorder error.
     * @see android.media.MediaRecorder.OnInfoListener
     */
    static const Int32 MEDIA_RECORDER_INFO_UNKNOWN              = 1;

    /** A maximum duration had been setup and has now been reached.
     * @see android.media.MediaRecorder.OnInfoListener
     */
    static const Int32 MEDIA_RECORDER_INFO_MAX_DURATION_REACHED = 800;

    /** A maximum filesize had been setup and has now been reached.
     * @see android.media.MediaRecorder.OnInfoListener
     */
    static const Int32 MEDIA_RECORDER_INFO_MAX_FILESIZE_REACHED = 801;

private:
    static const String TAG;// = "MediaRecorder";

    // The two fields below are accessed by native methods
    Int32 mNativeContext;

    AutoPtr<ISurface> mSurface;

    String mPath;
    AutoPtr<IFileDescriptor> mFd;
    AutoPtr<EventHandler> mEventHandler;
    AutoPtr<IMediaRecorderOnErrorListener> mOnErrorListener;
    AutoPtr<IMediaRecorderOnInfoListener> mOnInfoListener;
};

#endif
