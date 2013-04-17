
#ifndef __CAUDIOTRACK_H__
#define __CAUDIOTRACK_H__

#include "_CAudioTrack.h"
#include <elastos/AutoPtr.h>

#include <utils/RefBase.h>

#include <media/AudioSystem.h>
#include <media/AudioTrack.h>
#include "media/ElAudioTrack.h"

CarClass(CAudioTrack)//, public AudioTrack
{
/*
private:
    class NativeEventHandlerDelegate
    {
        android::AudioTrack mAudioTrack;
        IApartment* mHandler;
        
        NativeEventHandlerDelegate(android::AudioTrack track, IApartment* handler) {
            mAudioTrack = track;
            // find the looper for our new event handler
            Looper looper;
            if (handler != null ){
                looper = handler.getLooper();
            } else {
                // no given handler, use the looper the AudioTrack was created in
                looper = mInitializationLooper;
            }
            
            // construct the event handler with this looper
            if (looper != null) {
                // implement the event handler delegate
                mHandler = new Handler(looper) {
                    //@Override
                    public void handleMessage(Message msg) {
                        if (mAudioTrack == null) {
                            return;
                        }
                        OnPlaybackPositionUpdateListener listener = null;
                        synchronized (mPositionListenerLock) {
                            listener = mAudioTrack.mPositionListener;
                        }
                        switch(msg.what) {
                        case NATIVE_EVENT_MARKER:
                            if (listener != null) {
                                listener.onMarkerReached(mAudioTrack);
                            }
                            break;
                        case NATIVE_EVENT_NEW_POS:
                            if (listener != null) {
                                listener.onPeriodicNotification(mAudioTrack);
                            }
                            break;
                        default:
                            //Log.e(TAG, "[ android.media.AudioTrack.NativeEventHandler ] " + "Unknown event type: " + msg.what);
                            break;
                        }
                    }
                };
            } else {
                mHandler = null;
            } 
        }
        
        Handler getHandler() {
            return mHandler;
        }
    };
*/
public:
    CARAPI ReleaseResources();

    CARAPI GetMinVolume(
        /* [out] */ Float* minVolume);

    CARAPI GetMaxVolume(
        /* [out] */ Float* maxVolume);

    CARAPI GetSampleRate(
        /* [out] */ Int32* rate);

    CARAPI GetPlaybackRate(
        /* [out] */ Int32* rate);

    CARAPI GetAudioFormat(
        /* [out] */ Int32* format);

    CARAPI GetStreamType(
        /* [out] */ Int32* type);

    CARAPI GetChannelConfiguration(
        /* [out] */ Int32* config);

    CARAPI GetChannelCount(
        /* [out] */ Int32* count);

    CARAPI GetState(
        /* [out] */ Int32* state);

    CARAPI GetPlayState(
        /* [out] */ Int32* state);

    CARAPI GetNotificationMarkerPosition(
        /* [out] */ Int32* pos);

    CARAPI GetPositionNotificationPeriod(
        /* [out] */ Int32* period);

    CARAPI GetPlaybackHeadPosition(
        /* [out] */ Int32* position);

    CARAPI GetNativeOutputSampleRate(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* rate);

    CARAPI GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [out] */ Int32* size);

    CARAPI GetAudioSessionId(
        /* [out] */ Int32* id);

    CARAPI SetPlaybackPositionUpdateListener(
        /* [in] */ IOnPlaybackPositionUpdateListener* listener);

    CARAPI SetPlaybackPositionUpdateListenerEx(
        /* [in] */ IOnPlaybackPositionUpdateListener* listener,
        /* [in] */ IApartment* handler);

    CARAPI SetStereoVolume(
        /* [in] */ Float leftVolume,
        /* [in] */ Float rightVolume,
        /* [out] */ Int32* code);

    CARAPI SetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz,
        /* [out] */ Int32* rate);

    CARAPI SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames,
        /* [out] */ Int32* code);

    CARAPI SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames,
        /* [out] */ Int32* code);

    CARAPI SetPlaybackHeadPosition(
        /* [in] */ Int32 positionInFrames,
        /* [out] */ Int32* code);

    CARAPI SetLoopPoints(
        /* [in] */ Int32 startInFrames,
        /* [in] */ Int32 endInFrames,
        /* [in] */ Int32 loopCount,
        /* [out] */ Int32* code);

    CARAPI Play();

    CARAPI Stop();

    CARAPI Pause();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ ArrayOf<Byte> *audioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [out] */ Int32* num);

    CARAPI WriteEx(
        /* [in] */ ArrayOf<Int16> *audioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [out] */ Int32* num);

    CARAPI ReloadStaticData(
        /* [out] */ Int32* code);

    CARAPI AttachAuxEffect(
        /* [in] */ Int32 effectId,
        /* [out] */ Int32* code);

    CARAPI SetAuxEffectSendLevel(
        /* [in] */ Float level,
        /* [out] */ Int32* code);

    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode);

    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 sessionId);

private:
    CARAPI_(Int32) GetMinFrameCount(
      /* [in] */ Int32* frameCount,
      /* [in] */ Int32 streamType,
      /* [in] */ uint32_t sampleRate);

    CARAPI_(Int32) Native_get_playback_rate();

    CARAPI_(Int32) Native_get_marker_pos();

    CARAPI_(Int32) Native_get_pos_update_period();

    CARAPI_(Int32) Native_get_position();

    CARAPI_(Int32) Native_get_output_sample_rate(
      /* [in] */ Int32 javaStreamType);

    CARAPI_(Int32) Native_get_min_buff_size(
      /* [in] */ Int32 sampleRateInHertz, 
      /* [in] */ Int32 nbChannels,
      /* [in] */ Int32 audioFormat);

    CARAPI_(void) Native_setVolume(
      /* [in] */ Float left, 
      /* [in] */ Float right);

    CARAPI_(Int32) Android_media_translateErrorCode(
      /* [in] */ Int32 code);

    CARAPI_(Int32) Native_set_playback_rate(
      /* [in] */ Int32 sampleRateInHz);

    CARAPI_(Int32) Native_set_marker_pos(
      /* [in] */ Int32 markerPos);

    CARAPI_(Int32) Native_set_pos_update_period(
      /* [in] */ Int32 periodInFrames);

    CARAPI_(Int32) Native_set_position(
      /* [in] */ Int32 positionInFrames);

    CARAPI_(Int32) Native_set_loop(
      /* [in] */ Int32 loopStart, 
      /* [in] */ Int32 loopEnd, 
      /* [in] */ Int32 loopCount);

    CARAPI_(void) Native_Start();
    CARAPI_(void) Native_Stop();
    CARAPI_(void) Native_Pause();
    CARAPI_(void) Native_Flush();

    CARAPI_(Int32) writeToTrack(
      /* [in] */ android::AudioTrack* pTrack, 
      /* [in] */ Int32 audioFormat, 
      /* [in] */ Byte* data,
      /* [in] */ Int32 offsetInBytes, 
      /* [in] */ Int32 sizeInBytes) ;

    CARAPI_(Int32) Native_write_byte(    
      /* [in] */ ArrayOf<Byte> *javaAudioData,
      /* [in] */ Int32 offsetInBytes,
      /* [in] */ Int32 sizeInBytes,
      /* [in] */ Int32 javaAudioFormat); 

    CARAPI_(Int32) Native_write_short(
      /* [in] */ ArrayOf<Int16> *javaAudioData,
      /* [in] */ Int32 offsetInShorts,
      /* [in] */ Int32 sizeInShorts,
      /* [in] */ Int32 javaAudioFormat);

    CARAPI_(Int32) Native_reload_static();

    CARAPI_(Int32) Native_attachAuxEffect(
      /* [in] */ Int32 effectId);

    CARAPI_(void) Native_setAuxEffectSendLevel(
      /* [in] */ Float level);

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

public:
    /** indicates AudioTrack state is stopped */
    static const Int32 PLAYSTATE_STOPPED = 1;  // matches SL_PLAYSTATE_STOPPED
    /** indicates AudioTrack state is paused */
    static const Int32 PLAYSTATE_PAUSED  = 2;  // matches SL_PLAYSTATE_PAUSED
    /** indicates AudioTrack state is playing */
    static const Int32 PLAYSTATE_PLAYING = 3;  // matches SL_PLAYSTATE_PLAYING
    /**
     * Creation mode where audio data is transferred from Java to the native layer
     * only once before the audio starts playing.
     */
    static const Int32 MODE_STATIC = 0;
    /**
     * Creation mode where audio data is streamed from Java to the native layer
     * as the audio is playing.
     */
    static const Int32 MODE_STREAM = 1;

    /**
     * State of an AudioTrack that was not successfully initialized upon creation.
     */
    static const Int32 STATE_UNINITIALIZED = 0;
    /**
     * State of an AudioTrack that is ready to be used.
     */
    static const Int32 STATE_INITIALIZED   = 1;
    /**
     * State of a successfully initialized AudioTrack that uses static data,
     * but that hasn't received that data yet.
     */
    static const Int32 STATE_NO_STATIC_DATA = 2;

    // Error codes:
    // to keep in sync with frameworks/base/core/jni/android_media_AudioTrack.cpp
    /**
     * Denotes a successful operation.
     */
    static const Int32 SUCCESS                               = 0;
    /**
     * Denotes a generic operation failure.
     */
    static const Int32 ERROR                                 = -1;
    /**
     * Denotes a failure due to the use of an invalid value.
     */
    static const Int32 ERROR_BAD_VALUE                       = -2;
    
    /**
     * Denotes a failure due to the improper use of a method.
     */
    static const Int32 ERROR_INVALID_OPERATION               = -3;


    //from AudioFormat.h
        /** Invalid audio data format */
    static const Int32 ENCODING_INVALID = 0;
    /** Default audio data format */
    static const Int32 ENCODING_DEFAULT = 1;
    /** Audio data format: PCM 16 bit per sample */
    static const Int32 ENCODING_PCM_16BIT = 2; // accessed by native code
    /** Audio data format: PCM 8 bit per sample */
    static const Int32 ENCODING_PCM_8BIT = 3;  // accessed by native code
    /** Invalid audio channel configuration */
    /** @deprecated use CHANNEL_INVALID instead  */
    static const Int32 CHANNEL_CONFIGURATION_INVALID   = 0;
    /** Default audio channel configuration */
    /** @deprecated use CHANNEL_OUT_DEFAULT or CHANNEL_IN_DEFAULT instead  */
    static const Int32 CHANNEL_CONFIGURATION_DEFAULT   = 1;
    /** Mono audio configuration */
    /** @deprecated use CHANNEL_OUT_MONO or CHANNEL_IN_MONO instead  */
    static const Int32 CHANNEL_CONFIGURATION_MONO      = 2;
    /** Stereo (2 channel) audio configuration */
    /** @deprecated use CHANNEL_OUT_STEREO or CHANNEL_IN_STEREO instead  */
    static const Int32 CHANNEL_CONFIGURATION_STEREO    = 3;
    // Channel mask definitions must be kept in sync with native values in include/media/AudioSystem.h
    static const Int32 CHANNEL_OUT_FRONT_LEFT = 0x4;
    static const Int32 CHANNEL_OUT_FRONT_RIGHT = 0x8;
    static const Int32 CHANNEL_OUT_FRONT_CENTER = 0x10;
    static const Int32 CHANNEL_OUT_LOW_FREQUENCY = 0x20;
    static const Int32 CHANNEL_OUT_BACK_LEFT = 0x40;
    static const Int32 CHANNEL_OUT_BACK_RIGHT = 0x80;
    static const Int32 CHANNEL_OUT_FRONT_LEFT_OF_CENTER = 0x100;
    static const Int32 CHANNEL_OUT_FRONT_RIGHT_OF_CENTER = 0x200;
    static const Int32 CHANNEL_OUT_BACK_CENTER = 0x400;
    static const Int32 CHANNEL_OUT_MONO = CHANNEL_OUT_FRONT_LEFT;
    static const Int32 CHANNEL_OUT_STEREO = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT);
    static const Int32 CHANNEL_OUT_QUAD = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_BACK_LEFT | CHANNEL_OUT_BACK_RIGHT);
    static const Int32 CHANNEL_OUT_SURROUND = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_FRONT_CENTER | CHANNEL_OUT_BACK_CENTER);
    static const Int32 CHANNEL_OUT_5POINT1 = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_FRONT_CENTER | CHANNEL_OUT_LOW_FREQUENCY | CHANNEL_OUT_BACK_LEFT | CHANNEL_OUT_BACK_RIGHT);
    static const Int32 CHANNEL_OUT_7POINT1 = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_FRONT_CENTER | CHANNEL_OUT_LOW_FREQUENCY | CHANNEL_OUT_BACK_LEFT | CHANNEL_OUT_BACK_RIGHT |
            CHANNEL_OUT_FRONT_LEFT_OF_CENTER | CHANNEL_OUT_FRONT_RIGHT_OF_CENTER);



private:  
    /** Minimum value for a channel volume */
    static const Float VOLUME_MIN = 0.0f;

    /** Maximum value for a channel volume */
    static const Float VOLUME_MAX = 1.0f;

    static const Int32 ERROR_NATIVESETUP_AUDIOSYSTEM         = -16;
    static const Int32 ERROR_NATIVESETUP_INVALIDCHANNELMASK  = -17;
    static const Int32 ERROR_NATIVESETUP_INVALIDFORMAT       = -18;
    static const Int32 ERROR_NATIVESETUP_INVALIDSTREAMTYPE   = -19;
    static const Int32 ERROR_NATIVESETUP_NATIVEINITFAILED    = -20;
    // Events:
    // to keep in sync with frameworks/base/include/media/AudioTrack.h
    /**
     * Event id denotes when playback head has reached a previously set marker.
     */
    static const Int32 NATIVE_EVENT_MARKER  = 3;
    /**
     * Event id denotes when previously set update period has elapsed during playback.
     */
    static const Int32 NATIVE_EVENT_NEW_POS = 4;
    /**
     * Indicates the state of the AudioTrack instance.
     */
    Int32 mState ;//= STATE_UNINITIALIZED;

    /**
     * Indicates the play state of the AudioTrack instance.
     */
    Int32 mPlayState ;

    /**
     * The audio data sampling rate in Hz.
     */
    static const Int32 mSampleRate  = 22050;

    /**
     * The way audio is consumed by the hardware, streaming or static.
     */
    static const Int32 mDataLoadMode = MODE_STREAM;

    /**
     * The number of audio output channels (1 is mono, 2 is stereo).
     */
    static const Int32 mChannelCount = 1;

     /**
     * The encoding of the audio samples.
     * @see AudioFormat#ENCODING_PCM_8BIT
     * @see AudioFormat#ENCODING_PCM_16BIT
     */
    static const Int32 mAudioFormat = ENCODING_PCM_16BIT;

     /**
     * The current audio channel configuration.
     */
    static const Int32 mChannelConfiguration = CHANNEL_OUT_MONO;

    static const Int32 mSessionId = 0;

    static const Int32 mStreamType = 3;//:AudioManager.STREAM_MUSIC;


    /**
     * Lock to make sure mPlayState updates are reflecting the actual state of the object.
     */
    AutoPtr<IInterface> mPlayStateLock;// = new Object();

    /**
     * The listener the AudioTrack notifies when the playback position reaches a marker
     * or for periodic updates during the progression of the playback head.
     *  @see #setPlaybackPositionUpdateListener(OnPlaybackPositionUpdateListener)
     */
    AutoPtr<IOnPlaybackPositionUpdateListener> mPositionListener;// = null;
    /**
     * Lock to protect event listener updates against event notifications.
     */
    AutoPtr<IInterface> mPositionListenerLock;// = new Object();

     /**
     * Handler for marker events coming from the native code.
     */


    Int32 mNativeTrack;
};

#endif // __CAUDIOTRACK_H__
