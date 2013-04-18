
#ifndef __CAUDIOTRACK_H__
#define __CAUDIOTRACK_H__

#include "_CAudioTrack.h"
#include <elastos/AutoPtr.h>
#include <utils/RefBase.h>

#include <media/AudioSystem.h>
#include <media/AudioTrack.h>

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
            if (handler != NULL ){
                looper = handler.getLooper();
            } else {
                // no given handler, use the looper the AudioTrack was created in
                looper = mInitializationLooper;
            }
            
            // construct the event handler with this looper
            if (looper != NULL) {
                // implement the event handler delegate
                mHandler = new Handler(looper) {
                    //@Override
                    public void handleMessage(Message msg) {
                        if (mAudioTrack == NULL) {
                            return;
                        }
                        OnPlaybackPositionUpdateListener listener = NULL;
                        synchronized (mPositionListenerLock) {
                            listener = mAudioTrack.mPositionListener;
                        }
                        switch(msg.what) {
                        case NATIVE_EVENT_MARKER:
                            if (listener != NULL) {
                                listener.onMarkerReached(mAudioTrack);
                            }
                            break;
                        case NATIVE_EVENT_NEW_POS:
                            if (listener != NULL) {
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
                mHandler = NULL;
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

    CARAPI_(Int32) NativeSetup(
        /* [in] */ IInterface* audiotrack_this,
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 nbChannels,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 buffSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32* sessionId);

    CARAPI_(Int32) GetMinFrameCount(
        /* [in] */ Int32* frameCount,
        /* [in] */ Int32 streamType,
        /* [in] */ uint32_t sampleRate);

    CARAPI_(Int32) NativeGetPlaybackRate();

    CARAPI_(Int32) NativeGetMarkerPos();

    CARAPI_(Int32) NativeGetPosUpdatePeriod();

    CARAPI_(Int32) NativeGetPosition();

    CARAPI_(Int32) NativeGetOutputSampleRate(
        /* [in] */ Int32 javaStreamType);

    CARAPI_(Int32) NativeGetMinBuffSize(
        /* [in] */ Int32 sampleRateInHertz, 
        /* [in] */ Int32 nbChannels,
        /* [in] */ Int32 audioFormat);

    CARAPI_(void) NativeSetVolume(
        /* [in] */ Float left, 
        /* [in] */ Float right);

    CARAPI_(Int32) AndroidMediaTranslateErrorCode(
        /* [in] */ Int32 code);

    CARAPI_(Int32) NativeSetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz);

    CARAPI_(Int32) NativeSetMarkerPos(
        /* [in] */ Int32 markerPos);

    CARAPI_(Int32) NativeSetPosUpdatePeriod(
        /* [in] */ Int32 periodInFrames);

    CARAPI_(Int32) NativeSetPosition(
        /* [in] */ Int32 positionInFrames);

    CARAPI_(Int32) NativeSetLoop(
        /* [in] */ Int32 loopStart, 
        /* [in] */ Int32 loopEnd, 
        /* [in] */ Int32 loopCount);

    CARAPI_(void) NativeStart();
    CARAPI_(void) NativeStop();
    CARAPI_(void) NativePause();
    CARAPI_(void) NativeFlush();

    CARAPI_(Int32) WriteToTrack(
        /* [in] */ android::AudioTrack* pTrack, 
        /* [in] */ Int32 audioFormat, 
        /* [in] */ Byte* data,
        /* [in] */ Int32 offsetInBytes, 
        /* [in] */ Int32 sizeInBytes) ;

    CARAPI_(Int32) NativeWriteByte(    
        /* [in] */ ArrayOf<Byte> *javaAudioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [in] */ Int32 javaAudioFormat); 

    CARAPI_(Int32) NativeWriteShort(
        /* [in] */ ArrayOf<Int16> *javaAudioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [in] */ Int32 javaAudioFormat);

    CARAPI_(Int32) NativeReloadStatic();

    CARAPI_(Int32) NativeAttachAuxEffect(
        /* [in] */ Int32 effectId);

    CARAPI_(void) NativeSetAuxEffectSendLevel(
        /* [in] */ Float level);

    CARAPI_(void) NativeFinalize();
    
    CARAPI_(void) NativeRelease();

    CARAPI_(void) Init(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 sessionId);

    CARAPI_(void) AudioParamCheck(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 mode);

    CARAPI_(void)AudioBuffSizeCheck(
        /* [in] */ Int32 audioBufferSize);

    CARAPI_(void) SetState(
        /* [in] */ Int32 state);

    CARAPI_(void) Finalize();

    CARAPI_(void) PostEventFromNative(
    /* [in] */ IInterface* audiotrack_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj);

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
     Int32 mSampleRate ;// = 22050;

    /**
     * The way audio is consumed by the hardware, streaming or static.
     */
    Int32 mDataLoadMode ;// = AudioTrack_MODE_STREAM;

    /**
     * The number of audio output channels (1 is mono, 2 is stereo).
     */
     Int32 mChannelCount ;//= 1;

     Int32 mChannels; //= AudioFormat.CHANNEL_OUT_MONO;

     Int32 mNativeBufferSizeInBytes ;//= 0;

     /**
     * The encoding of the audio samples.
     * @see AudioFormat#ENCODING_PCM_8BIT
     * @see AudioFormat#ENCODING_PCM_16BIT
     */
    Int32 mAudioFormat ;//= AudioTrack_ENCODING_PCM_16BIT;

     /**
     * The current audio channel configuration.
     */
    Int32 mChannelConfiguration;// = AudioTrack_CHANNEL_OUT_MONO;

    Int32 mSessionId ;//= 0;

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
