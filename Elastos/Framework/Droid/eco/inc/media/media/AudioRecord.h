
#ifndef __AUDIORECORD_H__
#define __AUDIORECORD_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * The AudioRecord class manages the audio resources for Java applications
 * to record audio from the audio input hardware of the platform. This is
 * achieved by "pulling" (reading) the data from the AudioRecord object. The
 * application is responsible for polling the AudioRecord object in time using one of 
 * the following three methods:  {@link #read(byte[],Int32, Int32)}, {@link #read(Int16[], Int32, Int32)}
 * or {@link #read(ByteBuffer, Int32)}. The choice of which method to use will be based 
 * on the audio data storage format that is the most convenient for the user of AudioRecord.
 * <p>Upon creation, an AudioRecord object initializes its associated audio buffer that it will
 * fill with the new audio data. The size of this buffer, specified during the construction, 
 * determines how long an AudioRecord can record before "over-running" data that has not
 * been read yet. Data should be read from the audio hardware in chunks of sizes inferior to
 * the total recording buffer size.
 */
class AudioRecord
{
public:
    AudioRecord();

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
    AudioRecord(
        /* [in] */ Int32 audioSource, 
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat, 
        /* [in] */ Int32 bufferSizeInBytes);

    /**
     * Releases the native AudioRecord resources.
     * The object can no longer be used and the reference should be set to NULL
     * after a call to release()
     */
    virtual CARAPI ReleaseEx();

    //--------------------------------------------------------------------------
    // Getters
    //--------------------
    /**
     * Returns the configured audio data sample rate in Hz
     */
    virtual CARAPI_(Int32) GetSampleRate();
    
    /**
     * Returns the audio recording source. 
     * @see MediaRecorder.AudioSource
     */
    virtual CARAPI_(Int32) GetAudioSource();

    /**
     * Returns the configured audio data format. See {@link AudioFormat#ENCODING_PCM_16BIT}
     * and {@link AudioFormat#ENCODING_PCM_8BIT}.
     */
    virtual CARAPI_(Int32) GetAudioFormat();

    /**
     * Returns the configured channel configuration. 
     * See {@link AudioFormat#CHANNEL_IN_MONO}
     * and {@link AudioFormat#CHANNEL_IN_STEREO}.
     */
    virtual CARAPI_(Int32) GetChannelConfiguration();

    /**
     * Returns the configured number of channels.
     */
    virtual CARAPI_(Int32) GetChannelCount();

    /**
     * Returns the state of the AudioRecord instance. This is useful after the
     * AudioRecord instance has been created to check if it was initialized 
     * properly. This ensures that the appropriate hardware resources have been
     * acquired.
     * @see AudioRecord#STATE_INITIALIZED
     * @see AudioRecord#STATE_UNINITIALIZED
     */
    virtual CARAPI_(Int32) GetState();

    /**
     * Returns the recording state of the AudioRecord instance.
     * @see AudioRecord#RECORDSTATE_STOPPED
     * @see AudioRecord#RECORDSTATE_RECORDING
     */
    virtual CARAPI_(Int32) GetRecordingState();

    /**
     * Returns the notification marker position expressed in frames.
     */
    virtual CARAPI_(Int32) GetNotificationMarkerPosition();

    /**
     * Returns the notification update period expressed in frames.
     */
    virtual CARAPI_(Int32) GetPositionNotificationPeriod();

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
    static CARAPI_(Int32) GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat);

    //---------------------------------------------------------
    // Transport control methods
    //--------------------
    /**
     * Starts recording from the AudioRecord instance. 
     * @throws IllegalStateException
     */
    virtual CARAPI StartRecording();

    /**
     * Stops recording.
     * @throws IllegalStateException
     */
    virtual CARAPI Stop();


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
    virtual CARAPI_(Int32) Read(
        /* [in] */ ArrayOf<Byte>* audioData, 
        /* [in] */ Int32 offsetInBytes, 
        /* [in] */ Int32 sizeInBytes);


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
    virtual CARAPI_(Int32) Read(
        /* [in] */ ArrayOf<Int16>* audioData, 
        /* [in] */ Int32 offsetInShorts, 
        /* [in] */ Int32 sizeInShorts);

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
    //virtual CARAPI_(Int32) Read(
    //    /* [in] */ ByteBuffer audioBuffer, 
    //    /* [in] */ Int32 sizeInBytes);

    //--------------------------------------------------------------------------
    // Initialization / configuration
    //--------------------  
    /**
     * Sets the listener the AudioRecord notifies when a previously set marker is reached or
     * for each periodic record head position update.
     * @param listener
     */
    virtual CARAPI SetRecordPositionUpdateListener(
        /* [in] */ IOnRecordPositionUpdateListener* listener);

    /**
     * Sets the listener the AudioRecord notifies when a previously set marker is reached or
     * for each periodic record head position update.
     * Use this method to receive AudioRecord events in the Handler associated with another
     * thread than the one in which you created the AudioTrack instance.
     * @param listener
     * @param handler the Handler that will receive the event notification messages.
     */
    virtual CARAPI SetRecordPositionUpdateListener(
        /* [in] */ IOnRecordPositionUpdateListener* listener, 
        /* [in] */ IHandler* handler);

    /**
     * Sets the marker position at which the listener is called, if set with 
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)} or 
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)}.
     * @param markerInFrames marker position expressed in frames
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
     *  {@link #ERROR_INVALID_OPERATION} 
     */
    virtual CARAPI_(Int32) SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames);


    /**
     * Sets the period at which the listener is called, if set with
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)} or 
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)}.
     * @param periodInFrames update period expressed in frames
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_INVALID_OPERATION}
     */
    virtual CARAPI_(Int32) SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames);


protected:
    virtual CARAPI_(void) Finalize(); 

private:
    // Convenience method for the constructor's parameter checks.
    // This is where constructor IllegalArgumentException-s are thrown
    // postconditions:
    //    mRecordSource is valid
    //    mChannelCount is valid
    //    mChannels is valid
    //    mAudioFormat is valid
    //    mSampleRate is valid
    CARAPI_(void) AudioParamCheck(
        /* [in] */ Int32 audioSource, 
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelConfig, 
        /* [in] */ Int32 audioFormat);

    // Convenience method for the contructor's audio buffer size check.
    // preconditions:
    //    mChannelCount is valid
    //    mAudioFormat is AudioFormat.ENCODING_PCM_8BIT OR AudioFormat.ENCODING_PCM_16BIT
    // postcondition:
    //    mNativeBufferSizeInBytes is valid (multiple of frame size, positive)
    CARAPI_(void) AudioBuffSizeCheck(
        /* [in] */ Int32 audioBufferSize);


    //---------------------------------------------------------
    // Inner classes
    //--------------------
      
    /**
     * Helper class to handle the forwarding of native events to the appropriate listener
     * (potentially) handled in a different thread
     */  
    class NativeEventHandler : public IHandler, public ElRefBase
    {
    public:
        //NativeEventHandler(
        //    /* [in] */ AudioRecord* recorder, 
        //    /* [in] */ ILooper* looper);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
      
    private:
        AudioRecord* mAudioRecord;
    };
    
    
    //---------------------------------------------------------
    // Java methods called from the native side
    //--------------------
    static CARAPI_(void) PostEventFromNative(
        /* [in] */ IInterface* audiorecord_ref,
        /* [in] */ Int32 what, 
        /* [in] */ Int32 arg1, 
        /* [in] */ Int32 arg2, 
        /* [in] */ IInterface* obj);
    

    //---------------------------------------------------------
    // Native methods called from the Java side
    //--------------------

    CARAPI_(Int32) native_setup(
        /* [in] */ IInterface* audiorecord_this, 
        /* [in] */ Int32 recordSource, 
        /* [in] */ Int32 sampleRate, 
        /* [in] */ Int32 nbChannels, 
        /* [in] */ Int32 audioFormat, 
        /* [in] */ Int32 buffSizeInBytes);

    CARAPI_(void) native_finalize();
    
    CARAPI_(void) native_release();

    CARAPI_(Int32) native_start();

    CARAPI_(void) native_stop();

    CARAPI_(Int32) native_read_in_byte_array(
        /* [in] */ ArrayOf<Byte>* audioData, 
        /* [in] */ Int32 offsetInBytes, 
        /* [in] */ Int32 sizeInBytes);

    CARAPI_(Int32) native_read_in_short_array(
        /* [in] */ ArrayOf<Int16>* audioData, 
        /* [in] */ Int32 offsetInShorts, 
        /* [in] */ Int32 sizeInShorts);

    CARAPI_(Int32) native_read_in_direct_buffer(
        /* [in] */ IInterface* jBuffer, 
        /* [in] */ Int32 sizeInBytes);
    
    CARAPI_(Int32) native_set_marker_pos(
        /* [in] */ Int32 marker);

    CARAPI_(Int32) native_get_marker_pos();
    
    CARAPI_(Int32) native_set_pos_update_period(
        /* [in] */ Int32 updatePeriod);
    
    CARAPI_(Int32) native_get_pos_update_period();
    
    static CARAPI_(Int32) native_get_min_buff_size(
        /* [in] */ Int32 sampleRateInHz, 
        /* [in] */ Int32 channelCount, 
        /* [in] */ Int32 audioFormat);

    
    //---------------------------------------------------------
    // Utility methods
    //------------------

    static CARAPI_(void) Logd(
        /* [in] */ String msg);

    static CARAPI_(void) Loge(
        /* [in] */ String msg);


public:
    //---------------------------------------------------------
    // Constants
    //--------------------
    /**
     *  indicates AudioRecord state is not successfully initialized. 
     */
    static const Int32 STATE_UNINITIALIZED = 0;

    /**
     *  indicates AudioRecord state is ready to be used 
     */
    static const Int32 STATE_INITIALIZED   = 1;


    /**
     * indicates AudioRecord recording state is not recording 
     */
    static const Int32 RECORDSTATE_STOPPED = 1;  // matches SL_RECORDSTATE_STOPPED

    /**
     * indicates AudioRecord recording state is recording 
     */
    static const Int32 RECORDSTATE_RECORDING = 3;// matches SL_RECORDSTATE_RECORDING

    // Error codes:
    // to keep in sync with frameworks/base/core/jni/android_media_AudioRecord.cpp
    /**
     * Denotes a successful operation.
     */
    static const Int32 SUCCESS                 = 0;

    /**
     * Denotes a generic operation failure.
     */
    static const Int32 ERROR                   = -1;

    /**
     * Denotes a failure due to the use of an invalid value.
     */
    static const Int32 ERROR_BAD_VALUE         = -2;

    /**
     * Denotes a failure due to the improper use of a method.
     */
    static const Int32 ERROR_INVALID_OPERATION = -3;

private:
    static const Int32 AUDIORECORD_ERROR_SETUP_ZEROFRAMECOUNT      = -16;
    static const Int32 AUDIORECORD_ERROR_SETUP_INVALIDCHANNELMASK  = -17;
    static const Int32 AUDIORECORD_ERROR_SETUP_INVALIDFORMAT       = -18;
    static const Int32 AUDIORECORD_ERROR_SETUP_INVALIDSOURCE       = -19;
    static const Int32 AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED    = -20;
    
    // Events:
    // to keep in sync with frameworks/base/include/media/AudioRecord.h 
    /**
     * Event id denotes when record head has reached a previously set marker.
     */
    static const Int32 NATIVE_EVENT_MARKER  = 2;
    /**
     * Event id denotes when previously set update period has elapsed during recording.
     */
    static const Int32 NATIVE_EVENT_NEW_POS = 3;
    
    static const String TAG;// = "AudioRecord-Java";


    //---------------------------------------------------------
    // Used exclusively by native code
    //--------------------
    /** 
     * Accessed by native methods: provides access to C++ AudioRecord object 
     */
    Int32 mNativeRecorderInJavaObj;

    /** 
     * Accessed by native methods: provides access to the callback data.
     */
    Int32 mNativeCallbackCookie;
    

    //---------------------------------------------------------
    // Member variables
    //--------------------    
    /**
     * The audio data sampling rate in Hz.
     */
    Int32 mSampleRate;// = 22050;

    /**
     * The number of input audio channels (1 is mono, 2 is stereo)
     */
    Int32 mChannelCount;// = 1;

    /**
     * The audio channel mask
     */
    Int32 mChannels;// = AudioFormat.CHANNEL_IN_MONO;

    /**
     * The current audio channel configuration
     */
    Int32 mChannelConfiguration;// = AudioFormat.CHANNEL_IN_MONO;

    /**
     * The encoding of the audio samples.
     * @see AudioFormat#ENCODING_PCM_8BIT
     * @see AudioFormat#ENCODING_PCM_16BIT
     */
    Int32 mAudioFormat;// = AudioFormat.ENCODING_PCM_16BIT;

    /**
     * Where the audio data is recorded from.
     */
    Int32 mRecordSource;// = MediaRecorder.AudioSource.DEFAULT;

    /**
     * Indicates the state of the AudioRecord instance.
     */
    Int32 mState;// = STATE_UNINITIALIZED;
    
    /**
     * Indicates the recording state of the AudioRecord instance.
     */
    Int32 mRecordingState;// = RECORDSTATE_STOPPED;
    
    /**
     * Lock to make sure mRecordingState updates are reflecting the actual state of the object.
     */
    AutoPtr<IInterface> mRecordingStateLock;// = new Object();
    
    /**
     * The listener the AudioRecord notifies when the record position reaches a marker
     * or for periodic updates during the progression of the record head.
     *  @see #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)
     *  @see #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)
     */
    AutoPtr<IOnRecordPositionUpdateListener> mPositionListener;// = NULL;
    
    /**
     * Lock to protect position listener updates against event notifications
     */
    //const Object mPositionListenerLock = new Object();
    
    /**
     * Handler for marker events coming from the native code
     */
    NativeEventHandler* mEventHandler;// = NULL;
    
    /**
     * Looper associated with the thread that creates the AudioRecord instance
     */
    //AutoPtr<ILooper> mInitializationLooper;// = NULL;
    
    /**
     * Size of the native audio buffer.
     */
    Int32 mNativeBufferSizeInBytes;// = 0;
};

#endif
