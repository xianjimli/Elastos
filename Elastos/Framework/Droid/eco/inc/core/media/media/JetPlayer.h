
#ifndef __JETPLAYER_H__
#define __JETPLAYER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * JetPlayer provides access to JET content playback and control.
 *
 * <p>Please refer to the JET Creator User Manual for a presentation of the JET interactive
 * music concept and how to use the JetCreator tool to create content to be player by JetPlayer.
 *
 * <p>Use of the JetPlayer class is based around the playback of a number of JET segments
 * sequentially added to a playback FIFO queue. The rendering of the MIDI content stored in each
 * segment can be dynamically affected by two mechanisms:
 * <ul>
 * <li>tracks in a segment can be muted or unmuted at any moment, individually or through
 *    a mask (to change the mute state of multiple tracks at once)</li>
 * <li>parts of tracks in a segment can be played at predefined points in the segment, in order
 *    to maintain synchronization with the other tracks in the segment. This is achieved through
 *    the notion of "clips", which can be triggered at any time, but that will play only at the
 *    right time, as authored in the corresponding JET file.</li>
 * </ul>
 * As a result of the rendering and playback of the JET segments, the user of the JetPlayer instance
 * can receive notifications from the JET engine relative to:
 * <ul>
 * <li>the playback state,</li>
 * <li>the number of segments left to play in the queue,</li>
 * <li>application controller events (CC80-83) to mark points in the MIDI segments.</li>
 * </ul>
 *  Use {@link #getJetPlayer()} to construct a JetPlayer instance. JetPlayer is a singleton class.
 *
 */
class JetPlayer
{
public:

    JetPlayer();

    //--------------------------------------------
    // Constructor, finalize
    //------------------------
    /**
     * Factory method for the JetPlayer class.
     * @return the singleton JetPlayer instance
     */
    static CARAPI_(AutoPtr<IJetPlayer>) GetJetPlayer();

    /**
     * Cloning a JetPlayer instance is not supported. Calling clone() will generate an exception.
     */
    virtual CARAPI_(AutoPtr<IInterface>) Clone();

    /**
     * Stops the current JET playback, and releases all associated native resources.
     * The object can no longer be used and the reference should be set to NULL
     * after a call to release().
     */
    virtual CARAPI ReleaseEx();


    //--------------------------------------------
    // Getters
    //------------------------
    /**
     * Returns the maximum number of simultaneous MIDI tracks supported by JetPlayer
     */
    static CARAPI_(Int32) GetMaxTracks();


    //--------------------------------------------
    // Jet functionality
    //------------------------
    /**
     * Loads a .jet file from a given path.
     * @param path the path to the .jet file, for instance "/sdcard/mygame/music.jet".
     * @return true if loading the .jet file was successful, false if loading failed.
     */
    virtual CARAPI_(Boolean) LoadJetFile(
        /* [in] */ String path);


    /**
     * Loads a .jet file from an asset file descriptor.
     * @param afd the asset file descriptor.
     * @return true if loading the .jet file was successful, false if loading failed.
     */
    virtual CARAPI_(Boolean) LoadJetFile(
        /* [in] */ IAssetFileDescriptor* afd);

    /**
     * Closes the resource containing the JET content.
     * @return true if successfully closed, false otherwise.
     */
    virtual CARAPI_(Boolean) CloseJetFile();


    /**
     * Starts playing the JET segment queue.
     * @return true if rendering and playback is successfully started, false otherwise.
     */
    virtual CARAPI_(Boolean) Play();


    /**
     * Pauses the playback of the JET segment queue.
     * @return true if rendering and playback is successfully paused, false otherwise.
     */
    virtual CARAPI_(Boolean) Pause();


    /**
     * Queues the specified segment in the JET queue.
     * @param segmentNum the identifier of the segment.
     * @param libNum the index of the sound bank associated with the segment. Use -1 to indicate
     *    that no sound bank (DLS file) is associated with this segment, in which case JET will use
     *    the General MIDI library.
     * @param repeatCount the number of times the segment will be repeated. 0 means the segment will
     *    only play once. -1 means the segment will repeat indefinitely.
     * @param transpose the amount of pitch transposition. Set to 0 for normal playback.
     *    Range is -12 to +12.
     * @param muteFlags a bitmask to specify which MIDI tracks will be muted during playback. Bit 0
     *    affects track 0, bit 1 affects track 1 etc.
     * @param userID a value specified by the application that uniquely identifies the segment.
     *    this value is received in the
     *    {@link OnJetEventListener#onJetUserIdUpdate(JetPlayer, Int32, Int32)} event listener method.
     *    Normally, the application will keep a Byte value that is incremented each time a new
     *    segment is queued up. This can be used to look up any special characteristics of that
     *    track including trigger clips and mute flags.
     * @return true if the segment was successfully queued, false if the queue is full or if the
     *    parameters are invalid.
     */
    virtual CARAPI_(Boolean) QueueJetSegment(
        /* [in] */ Int32 segmentNum,
        /* [in] */ Int32 libNum,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 transpose,
        /* [in] */ Int32 muteFlags,
        /* [in] */ Byte userID);

    /**
     * Queues the specified segment in the JET queue.
     * @param segmentNum the identifier of the segment.
     * @param libNum the index of the soundbank associated with the segment. Use -1 to indicate that
     *    no sound bank (DLS file) is associated with this segment, in which case JET will use
     *    the General MIDI library.
     * @param repeatCount the number of times the segment will be repeated. 0 means the segment will
     *    only play once. -1 means the segment will repeat indefinitely.
     * @param transpose the amount of pitch transposition. Set to 0 for normal playback.
     *    Range is -12 to +12.
     * @param muteArray an array of booleans to specify which MIDI tracks will be muted during
     *    playback. The value at index 0 affects track 0, value at index 1 affects track 1 etc.
     *    The length of the array must be {@link #getMaxTracks()} for the call to succeed.
     * @param userID a value specified by the application that uniquely identifies the segment.
     *    this value is received in the
     *    {@link OnJetEventListener#onJetUserIdUpdate(JetPlayer, Int32, Int32)} event listener method.
     *    Normally, the application will keep a Byte value that is incremented each time a new
     *    segment is queued up. This can be used to look up any special characteristics of that
     *    track including trigger clips and mute flags.
     * @return true if the segment was successfully queued, false if the queue is full or if the
     *    parameters are invalid.
     */
    virtual CARAPI_(Boolean) QueueJetSegmentMuteArray(
        /* [in] */ Int32 segmentNum,
        /* [in] */ Int32 libNum,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 transpose,
        /* [in] */ ArrayOf<Boolean>* muteArray,
        /* [in] */ Byte userID);


    /**
     * Modifies the mute flags.
     * @param muteFlags a bitmask to specify which MIDI tracks are muted. Bit 0 affects track 0,
     *    bit 1 affects track 1 etc.
     * @param sync if false, the new mute flags will be applied as soon as possible by the JET
     *    render and playback engine. If true, the mute flags will be updated at the start of the
     *    next segment. If the segment is repeated, the flags will take effect the next time
     *    segment is repeated.
     * @return true if the mute flags were successfully updated, false otherwise.
     */
    virtual CARAPI_(Boolean) SetMuteFlags(
        /* [in] */ Int32 muteFlags,
        /* [in] */ Boolean sync);


    /**
     * Modifies the mute flags for the current active segment.
     * @param muteArray an array of booleans to specify which MIDI tracks are muted. The value at
     *    index 0 affects track 0, value at index 1 affects track 1 etc.
     *    The length of the array must be {@link #getMaxTracks()} for the call to succeed.
     * @param sync if false, the new mute flags will be applied as soon as possible by the JET
     *    render and playback engine. If true, the mute flags will be updated at the start of the
     *    next segment. If the segment is repeated, the flags will take effect the next time
     *    segment is repeated.
     * @return true if the mute flags were successfully updated, false otherwise.
     */
    virtual CARAPI_(Boolean) SetMuteArray(
        /* [in] */ ArrayOf<Boolean>* muteArray,
        /* [in] */ Boolean sync);


    /**
     * Mutes or unmutes a single track.
     * @param trackId the index of the track to mute.
     * @param muteFlag set to true to mute, false to unmute.
     * @param sync if false, the new mute flags will be applied as soon as possible by the JET
     *    render and playback engine. If true, the mute flag will be updated at the start of the
     *    next segment. If the segment is repeated, the flag will take effect the next time
     *    segment is repeated.
     * @return true if the mute flag was successfully updated, false otherwise.
     */
    virtual CARAPI_(Boolean) SetMuteFlag(
        /* [in] */ Int32 trackId,
        /* [in] */ Boolean muteFlag,
        /* [in] */ Boolean sync);

    /**
     * Schedules the playback of a clip.
     * This will automatically update the mute flags in sync with the JET Clip Marker (controller
     * 103). The parameter clipID must be in the range of 0-63. After the call to triggerClip, when
     * JET next encounters a controller event 103 with bits 0-5 of the value equal to clipID and
     * bit 6 set to 1, it will automatically unmute the track containing the controller event.
     * When JET encounters the complementary controller event 103 with bits 0-5 of the value equal
     * to clipID and bit 6 set to 0, it will mute the track again.
     * @param clipId the identifier of the clip to trigger.
     * @return true if the clip was successfully triggered, false otherwise.
     */
    virtual CARAPI_(Boolean) TriggerClip(
        /* [in] */ Int32 clipId);


    /**
     * Empties the segment queue, and clears all clips that are scheduled for playback.
     * @return true if the queue was successfully cleared, false otherwise.
     */
    virtual CARAPI_(Boolean) ClearQueue();

    //--------------------------------------------
    // Jet event listener
    //------------------------
    /**
     * Sets the listener JetPlayer notifies when a JET event is generated by the rendering and
     * playback engine.
     * Notifications will be received in the same thread as the one in which the JetPlayer
     * instance was created.
     * @param listener
     */
    virtual CARAPI SetEventListener(
        /* [in] */ IOnJetEventListener* listener);

    /**
     * Sets the listener JetPlayer notifies when a JET event is generated by the rendering and
     * playback engine.
     * Use this method to receive JET events in the Handler associated with another
     * thread than the one in which you created the JetPlayer instance.
     * @param listener
     * @param handler the Handler that will receive the event notification messages.
     */
    virtual CARAPI SetEventListener(
        /* [in] */ IOnJetEventListener* listener,
        /* [in] */ IHandler* handler);

protected:
    virtual CARAPI_(void) Finalize();

private:
    //---------------------------------------------------------
    // Internal class to handle events posted from native code
    //------------------------
    class NativeEventHandler : public IHandler, public ElRefBase
    {
    public:
        //NativeEventHandler(
        //    /* [in] */ JetPlayer jet,
        //    /* [in] */ Looper looper);

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
        JetPlayer* mJet;
    };

    //--------------------------------------------
    // Native methods
    //------------------------
    CARAPI_(Boolean) native_setup(
        /* [in] */ IInterface* Jet_this,
        /* [in] */ Int32 maxTracks,
        /* [in] */ Int32 trackBufferSize);

    CARAPI_(void) native_finalize();

    CARAPI_(void) native_release();

    CARAPI_(Boolean) native_loadJetFromFile(
        /* [in] */ String pathToJetFile);

    CARAPI_(Boolean) native_loadJetFromFileD(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 len);

    CARAPI_(Boolean) native_closeJetFile();

    CARAPI_(Boolean) native_playJet();

    CARAPI_(Boolean) native_pauseJet();

    CARAPI_(Boolean) native_queueJetSegment(
        /* [in] */ Int32 segmentNum,
        /* [in] */ Int32 libNum,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 transpose,
        /* [in] */ Int32 muteFlags,
        /* [in] */ Byte userID);

    CARAPI_(Boolean) native_queueJetSegmentMuteArray(
        /* [in] */ Int32 segmentNum,
        /* [in] */ Int32 libNum,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 transpose,
        /* [in] */ ArrayOf<Boolean>* muteArray,
        /* [in] */ Byte userID);

    CARAPI_(Boolean) native_setMuteFlags(
        /* [in] */ Int32 muteFlags,
        /* [in] */ Boolean sync);

    CARAPI_(Boolean) native_setMuteArray(
        /* [in] */ ArrayOf<Boolean>* muteArray,
        /* [in] */ Boolean sync);

    CARAPI_(Boolean) native_setMuteFlag(
        /* [in] */ Int32 trackId,
        /* [in] */ Boolean muteFlag,
        /* [in] */ Boolean sync);

    CARAPI_(Boolean) native_triggerClip(
        /* [in] */ Int32 clipId);

    CARAPI_(Boolean) native_clearQueue();

    //---------------------------------------------------------
    // Called exclusively by native code
    //--------------------
    static CARAPI_(void) PostEventFromNative(
        /* [in] */ IInterface* jetplayer_ref,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    static CARAPI_(void) Logd(
        /* [in] */ String msg);

    static CARAPI_(void) Loge(
        /* [in] */ String msg);

    //---------------------------------------------------------
    // Utils
    //--------------------
    static String TAG;// = "JetPlayer-J";

    //--------------------------------------------
    // Constants
    //------------------------
    /**
     * The maximum number of simultaneous tracks. Use {@link #getMaxTracks()} to
     * access this value.
     */
    static Int32 MAXTRACKS;// = 32;

    // to keep in sync with the JetPlayer class constants
    // defined in frameworks/base/include/media/JetPlayer.h
    static const Int32 JET_EVENT                   = 1;
    static const Int32 JET_USERID_UPDATE           = 2;
    static const Int32 JET_NUMQUEUEDSEGMENT_UPDATE = 3;
    static const Int32 JET_PAUSE_UPDATE            = 4;

    // to keep in sync with external/sonivox/arm-wt-22k/lib_src/jet_data.h
    // Encoding of event information on 32 bits
    static const Int32 JET_EVENT_VAL_MASK    = 0x0000007f; // mask for value
    static const Int32 JET_EVENT_CTRL_MASK   = 0x00003f80; // mask for controller
    static const Int32 JET_EVENT_CHAN_MASK   = 0x0003c000; // mask for channel
    static const Int32 JET_EVENT_TRACK_MASK  = 0x00fc0000; // mask for track number
    static const Int32 JET_EVENT_SEG_MASK    = 0xff000000; // mask for segment ID
    static const Int32 JET_EVENT_CTRL_SHIFT  = 7;  // shift to get controller number to bit 0
    static const Int32 JET_EVENT_CHAN_SHIFT  = 14; // shift to get MIDI channel to bit 0
    static const Int32 JET_EVENT_TRACK_SHIFT = 18; // shift to get track ID to bit 0
    static const Int32 JET_EVENT_SEG_SHIFT   = 24; // shift to get segment ID to bit 0

    // to keep in sync with values used in external/sonivox/arm-wt-22k/Android.mk
    // Jet rendering audio parameters
    static const Int32 JET_OUTPUT_RATE = 22050; // _SAMPLE_RATE_22050 in Android.mk
    static const Int32 JET_OUTPUT_CHANNEL_CONFIG =
        AudioFormat_CHANNEL_OUT_STEREO; // NUM_OUTPUT_CHANNELS=2 in Android.mk


    //--------------------------------------------
    // Member variables
    //------------------------
    /**
     * Handler for jet events and status updates coming from the native code
     */
    NativeEventHandler* mEventHandler;// = NULL;

    /**
     * Looper associated with the thread that creates the AudioTrack instance
     */
    //private Looper mInitializationLooper = NULL;

    /**
     * Lock to protect the event listener updates against event notifications
     */
    //private final Object mEventListenerLock = new Object();

    AutoPtr<IOnJetEventListener> mJetEventListener;// = NULL;

    static AutoPtr<IJetPlayer> singletonRef;


    //--------------------------------
    // Used exclusively by native code
    //--------------------
    /**
     * Accessed by native methods: provides access to C++ JetPlayer object
     */
    Int32 mNativePlayerInJavaObj;
};

#endif
