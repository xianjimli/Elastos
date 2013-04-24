
#ifndef __CAUDIOTRACK_H__
#define __CAUDIOTRACK_H__

#include "_CAudioTrack.h"
#include <elastos/AutoPtr.h>
#include <utils/RefBase.h>

#include <media/AudioTrack.h>

#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>

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
    struct audiotrack_callback_cookie {
    //jclass      audioTrack_class;
    //jobject     audioTrack_ref;
    };

    class AudioTrackJniStorage {
        public:
            android::sp<android::MemoryHeapBase>         mMemHeap;
            android::sp<android::MemoryBase>             mMemBase;
            //audiotrack_callback_cookie mCallbackData;
            Int32                        mStreamType;

        AudioTrackJniStorage() {
            //mCallbackData.audioTrack_class = 0;
            //mCallbackData.audioTrack_ref = 0;
            mStreamType = android::AudioSystem::DEFAULT;
        }

        ~AudioTrackJniStorage() {
            mMemBase.clear();
            mMemHeap.clear();
        }

        bool allocSharedMem(Int32 sizeInBytes) {
            mMemHeap = new android::MemoryHeapBase(sizeInBytes, 0, "AudioTrack Heap Base");
            if (mMemHeap->getHeapID() < 0) {
                return false;
            }
            mMemBase = new android::MemoryBase(mMemHeap, 0, sizeInBytes);
            return true;
        }
    };

public:
    CAudioTrack();

    ~CAudioTrack();

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
        /* [out] */ Int32* result);

    CARAPI SetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz,
        /* [out] */ Int32* rate);

    CARAPI SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames,
        /* [out] */ Int32* result);

    CARAPI SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames,
        /* [out] */ Int32* result);

    CARAPI SetPlaybackHeadPosition(
        /* [in] */ Int32 positionInFrames,
        /* [out] */ Int32* result);

    CARAPI SetLoopPoints(
        /* [in] */ Int32 startInFrames,
        /* [in] */ Int32 endInFrames,
        /* [in] */ Int32 loopCount,
        /* [out] */ Int32* result);

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
        /* [out] */ Int32* result);

    CARAPI AttachAuxEffect(
        /* [in] */ Int32 effectId,
        /* [out] */ Int32* result);

    CARAPI SetAuxEffectSendLevel(
        /* [in] */ Float level,
        /* [out] */ Int32* result);


private:
    CARAPI_(Int32) AndroidMediaTranslateErrorCode(
        /* [in] */ Int32 code);

    CARAPI AudioParamCheck(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 mode);

    CARAPI AudioBuffSizeCheck(
        /* [in] */ Int32 audioBufferSize);

    CARAPI Init(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 sessionId);

    CARAPI_(Int32)GetNativeFrameCount(); 

    CARAPI_(void) SetState(
        /* [in] */ Int32 state);

    CARAPI_(Int32) GetMinFrameCount(
        /* [in] */ Int32* frameCount,
        /* [in] */ Int32 streamType,
        /* [in] */ uint32_t sampleRate);

    CARAPI_(Int32) WriteToTrack(
        /* [in] */ android::AudioTrack* pTrack, 
        /* [in] */ Int32 audioFormat, 
        /* [in] */ Byte* data,
        /* [in] */ Int32 offsetInBytes, 
        /* [in] */ Int32 sizeInBytes) ;

    CARAPI_(void) PostEventFromNative(
    /* [in] */ IInterface* audiotrack_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj);

    CARAPI_(Int32) NativeSetup(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 nbChannels,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 buffSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32* sessionId);

    CARAPI_(void) NativeFinalize();
    CARAPI_(void) NativeRelease();
    CARAPI_(void) NativeStart();
    CARAPI_(void) NativeStop();
    CARAPI_(void) NativePause();
    CARAPI_(void) NativeFlush();

    CARAPI_(Int32) NativeWriteByte(    
        /* [in] */ ArrayOf<Byte> *javaAudioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [in] */ Int32 javaAudioFormat); 

    CARAPI_(Int32) NativeWriteInt16(
        /* [in] */ ArrayOf<Int16> *javaAudioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [in] */ Int32 javaAudioFormat);

    CARAPI_(Int32) NativeReloadStatic();

    CARAPI_(Int32) NativeGetNativeFrameCount();

    CARAPI_(void) NativeSetVolume(
        /* [in] */ Float left, 
        /* [in] */ Float right);

    CARAPI_(Int32) NativeSetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz);
    CARAPI_(Int32) NativeGetPlaybackRate();

    CARAPI_(Int32) NativeSetMarkerPos(
        /* [in] */ Int32 markerPos);
    CARAPI_(Int32) NativeGetMarkerPos();

    CARAPI_(Int32) NativeSetPosUpdatePeriod(
        /* [in] */ Int32 periodInFrames);
    CARAPI_(Int32) NativeGetPosUpdatePeriod();

    CARAPI_(Int32) NativeSetPosition(
        /* [in] */ Int32 positionInFrames);
    CARAPI_(Int32) NativeGetPosition();

    CARAPI_(Int32) NativeSetLoop(
        /* [in] */ Int32 loopStart, 
        /* [in] */ Int32 loopEnd, 
        /* [in] */ Int32 loopCount);

    CARAPI_(Int32) NativeGetOutputSampleRate(
        /* [in] */ Int32 javaStreamType);

    CARAPI_(Int32) NativeGetMinBuffSize(
        /* [in] */ Int32 sampleRateInHertz, 
        /* [in] */ Int32 nbChannels,
        /* [in] */ Int32 audioFormat);

    CARAPI_(Int32) NativeGetSessionId();


    CARAPI_(Int32) NativeAttachAuxEffect(
        /* [in] */ Int32 effectId);

    CARAPI_(void) NativeSetAuxEffectSendLevel(
        /* [in] */ Float level);

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
    static const CString TAG;// = "AudioTrack-Java";

    /**
     * Indicates the state of the AudioTrack instance.
     */
    Int32 mState;

    /**
     * Indicates the play state of the AudioTrack instance.
     */
    Int32 mPlayState;

    /**
     * Lock to make sure mPlayState updates are reflecting the actual state of the object.
     */
    Mutex mPlayStateLock;// = new Object();

    /**
     * The listener the AudioTrack notifies when the playback position reaches a marker
     * or for periodic updates during the progression of the playback head.
     *  @see #setPlaybackPositionUpdateListener(OnPlaybackPositionUpdateListener)
     */
    AutoPtr<IOnPlaybackPositionUpdateListener> mPositionListener;// = null;
    /**
     * Lock to protect event listener updates against event notifications.
     */
    Mutex mPositionListenerLock;// = new Object();

    /**
     * Size of the native audio buffer.
     */
    Int32 mNativeBufferSizeInBytes;
    /**
     * Handler for marker events coming from the native code.
     */
    //NativeEventHandlerDelegate mEventHandlerDelegate = null;
    /**
     * Looper associated with the thread that creates the AudioTrack instance.
     */
    //Looper mInitializationLooper = null;

    /**
     * The audio data sampling rate in Hz.
     */
    Int32 mSampleRate;

    /**
     * The number of audio output channels (1 is mono, 2 is stereo).
     */
    Int32 mChannelCount;

    Int32 mChannels;

    /**
     * The type of the audio stream to play. See
     *   {@link AudioManager#STREAM_VOICE_CALL}, {@link AudioManager#STREAM_SYSTEM},
     *   {@link AudioManager#STREAM_RING}, {@link AudioManager#STREAM_MUSIC} and
     *   {@link AudioManager#STREAM_ALARM}
     */
    Int32 mStreamType;

    /**
     * The way audio is consumed by the hardware, streaming or static.
     */
    Int32 mDataLoadMode;

    /**
     * The current audio channel configuration.
     */
    Int32 mChannelConfiguration;

     /**
     * The encoding of the audio samples.
     * @see AudioFormat#ENCODING_PCM_8BIT
     * @see AudioFormat#ENCODING_PCM_16BIT
     */
    Int32 mAudioFormat;


    Int32 mSessionId;

    Int32 mNativeTrack;
};

#endif // __CAUDIOTRACK_H__
