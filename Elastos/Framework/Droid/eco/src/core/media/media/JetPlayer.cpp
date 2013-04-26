
#include "media/JetPlayer.h"
#include "media/CJetPlayer.h"

Int32 JetPlayer::MAXTRACKS;
AutoPtr<IJetPlayer> JetPlayer::singletonRef;

//--------------------------------------------
// Constructor, finalize
//------------------------
/**
 * Factory method for the JetPlayer class.
 * @return the singleton JetPlayer instance
 */
AutoPtr<IJetPlayer> JetPlayer::GetJetPlayer()
{
    if (singletonRef == NULL) {
        //singletonRef = new JetPlayer();
        CJetPlayer::New((IJetPlayer**)&singletonRef);
    }
    return singletonRef;
}

/**
 * Cloning a JetPlayer instance is not supported. Calling clone() will generate an exception.
 */
AutoPtr<IInterface> JetPlayer::Clone()
{
    // JetPlayer is a singleton class,
    // so you can't clone a JetPlayer instance
    //throw new CloneNotSupportedException();

    return NULL;
}


JetPlayer::JetPlayer()
{

    // remember which looper is associated with the JetPlayer instanciation
    //if ((mInitializationLooper = Looper.myLooper()) == NULL) {
    //    mInitializationLooper = Looper.getMainLooper();
    //}
    //
    //Int32 buffSizeInBytes = AudioTrack.getMinBufferSize(JET_OUTPUT_RATE,
    //        JET_OUTPUT_CHANNEL_CONFIG, AudioFormat.ENCODING_PCM_16BIT);
    //
    //if ((buffSizeInBytes != AudioTrack.ERROR)
    //        && (buffSizeInBytes != AudioTrack.ERROR_BAD_VALUE)) {
    //
    //    native_setup(new WeakReference<JetPlayer>(this),
    //            JetPlayer.getMaxTracks(),
    //            // bytes to frame conversion: sample format is ENCODING_PCM_16BIT, 2 channels
    //            // 1200 == minimum buffer size in frames on generation 1 hardware
    //            Math.max(1200, buffSizeInBytes / 4));
    //}
}


void JetPlayer::Finalize()
{
    native_finalize();
}


/**
 * Stops the current JET playback, and releases all associated native resources.
 * The object can no longer be used and the reference should be set to NULL
 * after a call to release().
 */
ECode JetPlayer::ReleaseEx()
{
    native_release();
    singletonRef = NULL;

    return NOERROR;
}


//--------------------------------------------
// Getters
//------------------------
/**
 * Returns the maximum number of simultaneous MIDI tracks supported by JetPlayer
 */
Int32 JetPlayer::GetMaxTracks()
{
    return JetPlayer::MAXTRACKS;
}


//--------------------------------------------
// Jet functionality
//------------------------
/**
 * Loads a .jet file from a given path.
 * @param path the path to the .jet file, for instance "/sdcard/mygame/music.jet".
 * @return true if loading the .jet file was successful, false if loading failed.
 */
Boolean JetPlayer::LoadJetFile(
    /* [in] */ String path)
{
    return native_loadJetFromFile(path);
}


/**
 * Loads a .jet file from an asset file descriptor.
 * @param afd the asset file descriptor.
 * @return true if loading the .jet file was successful, false if loading failed.
 */
Boolean JetPlayer::LoadJetFile(
    /* [in] */ IAssetFileDescriptor* afd)
{
    //Int64 len;
    //afd->GetLength(&len);
    //if (len < 0) {
    //    //throw new AndroidRuntimeException("no length for fd");
    //}

    //AutoPtr<IFileDescriptor> fd;
    //afd->GetFileDescriptor((IFileDescriptor**)&fd);
    //
    //Int64 offset;
    //afd->GetStartOffset(&offset);

    //return native_loadJetFromFileD(
    //        fd, offset, len);

    return NOERROR;
}

/**
 * Closes the resource containing the JET content.
 * @return true if successfully closed, false otherwise.
 */
Boolean JetPlayer::CloseJetFile()
{
    return native_closeJetFile();
}


/**
 * Starts playing the JET segment queue.
 * @return true if rendering and playback is successfully started, false otherwise.
 */
Boolean JetPlayer::Play()
{
    return native_playJet();
}


/**
 * Pauses the playback of the JET segment queue.
 * @return true if rendering and playback is successfully paused, false otherwise.
 */
Boolean JetPlayer::Pause()
{
    return native_pauseJet();
}


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
Boolean JetPlayer::QueueJetSegment(
    /* [in] */ Int32 segmentNum,
    /* [in] */ Int32 libNum,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 transpose,
    /* [in] */ Int32 muteFlags,
    /* [in] */ Byte userID)
{
    return native_queueJetSegment(segmentNum, libNum, repeatCount,
            transpose, muteFlags, userID);
}

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
Boolean JetPlayer::QueueJetSegmentMuteArray(
    /* [in] */ Int32 segmentNum,
    /* [in] */ Int32 libNum,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 transpose,
    /* [in] */ ArrayOf<Boolean>* muteArray,
    /* [in] */ Byte userID)
{
    if (muteArray->GetLength() != JetPlayer::GetMaxTracks()) {
        return FALSE;
    }
    return native_queueJetSegmentMuteArray(segmentNum, libNum, repeatCount,
            transpose, muteArray, userID);
}


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
Boolean JetPlayer::SetMuteFlags(
    /* [in] */ Int32 muteFlags,
    /* [in] */ Boolean sync)
{
    return native_setMuteFlags(muteFlags, sync);
}


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
Boolean JetPlayer::SetMuteArray(
    /* [in] */ ArrayOf<Boolean>* muteArray,
    /* [in] */ Boolean sync)
{
    if(muteArray->GetLength() != JetPlayer::GetMaxTracks())
        return FALSE;
    return native_setMuteArray(muteArray, sync);
}


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
Boolean JetPlayer::SetMuteFlag(
    /* [in] */ Int32 trackId,
    /* [in] */ Boolean muteFlag,
    /* [in] */ Boolean sync)
{
    return native_setMuteFlag(trackId, muteFlag, sync);
}


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
Boolean JetPlayer::TriggerClip(
    /* [in] */ Int32 clipId)
{
    return native_triggerClip(clipId);
}


/**
 * Empties the segment queue, and clears all clips that are scheduled for playback.
 * @return true if the queue was successfully cleared, false otherwise.
 */
Boolean JetPlayer::ClearQueue()
{
    return native_clearQueue();
}

//JetPlayer::NativeEventHandler::NativeEventHandler(
//    /* [in] */ JetPlayer* jet,
//    /* [in] */ Looper looper)
//{
//    super(looper);
//    mJet = jet;
//}

ECode JetPlayer::NativeEventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    //OnJetEventListener listener = NULL;
    //synchronized (mEventListenerLock) {
    //    listener = mJet->mJetEventListener;
    //}
    //switch(msg.what) {
    //case JET_EVENT:
    //    if (listener != NULL) {
    //        // call the appropriate listener after decoding the event parameters
    //        // encoded in msg.arg1
    //        mJetEventListener.onJetEvent(
    //                mJet,
    //                (Int16)((msg.arg1 & JET_EVENT_SEG_MASK)   >> JET_EVENT_SEG_SHIFT),
    //                (Byte) ((msg.arg1 & JET_EVENT_TRACK_MASK) >> JET_EVENT_TRACK_SHIFT),
    //                // JETCreator channel numbers start at 1, but the index starts at 0
    //                // in the .jet files
    //                (Byte)(((msg.arg1 & JET_EVENT_CHAN_MASK)  >> JET_EVENT_CHAN_SHIFT) + 1),
    //                (Byte) ((msg.arg1 & JET_EVENT_CTRL_MASK)  >> JET_EVENT_CTRL_SHIFT),
    //                (Byte)  (msg.arg1 & JET_EVENT_VAL_MASK) );
    //    }
    //    return;
    //case JET_USERID_UPDATE:
    //    if (listener != NULL) {
    //        listener.onJetUserIdUpdate(mJet, msg.arg1, msg.arg2);
    //    }
    //    return;
    //case JET_NUMQUEUEDSEGMENT_UPDATE:
    //    if (listener != NULL) {
    //        listener.onJetNumQueuedSegmentUpdate(mJet, msg.arg1);
    //    }
    //    return;
    //case JET_PAUSE_UPDATE:
    //    if (listener != NULL)
    //        listener.onJetPauseUpdate(mJet, msg.arg1);
    //    return;

    //default:
    //    loge("Unknown message type " + msg.what);
    //    return;
    //}

    return NOERROR;
}

PInterface JetPlayer::NativeEventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 JetPlayer::NativeEventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 JetPlayer::NativeEventHandler::Release()
{
    return ElRefBase::Release();
}

ECode JetPlayer::NativeEventHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

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
ECode JetPlayer::SetEventListener(
    /* [in] */ IOnJetEventListener* listener)
{
    SetEventListener(listener, NULL);

    return NOERROR;
}

/**
 * Sets the listener JetPlayer notifies when a JET event is generated by the rendering and
 * playback engine.
 * Use this method to receive JET events in the Handler associated with another
 * thread than the one in which you created the JetPlayer instance.
 * @param listener
 * @param handler the Handler that will receive the event notification messages.
 */
ECode JetPlayer::SetEventListener(
    /* [in] */ IOnJetEventListener* listener,
    /* [in] */ IHandler* handler)
{
    //synchronized(mEventListenerLock) {

        mJetEventListener = listener;

        //if (listener != NULL) {
        //    if (handler != NULL) {
        //        mEventHandler = new NativeEventHandler(this, handler.getLooper());
        //    } else {
        //        // no given handler, use the looper the AudioTrack was created in
        //        mEventHandler = new NativeEventHandler(this, mInitializationLooper);
        //    }
        //} else {
        //    mEventHandler = NULL;
        //}

    //}

        return NOERROR;
}


//--------------------------------------------
// Native methods
//------------------------
Boolean JetPlayer::native_setup(
    /* [in] */ IInterface* Jet_this,
    /* [in] */ Int32 maxTracks,
    /* [in] */ Int32 trackBufferSize)
{
    return FALSE;
}

void JetPlayer::native_finalize()
{

}

void JetPlayer::native_release()
{

}

Boolean JetPlayer::native_loadJetFromFile(
    /* [in] */ String pathToJetFile)
{
    return FALSE;
}

Boolean JetPlayer::native_loadJetFromFileD(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 len)
{
    return FALSE;
}

Boolean JetPlayer::native_closeJetFile()
{
    return FALSE;
}

Boolean JetPlayer::native_playJet()
{
    return FALSE;
}

Boolean JetPlayer::native_pauseJet()
{
    return FALSE;
}

Boolean JetPlayer::native_queueJetSegment(
    /* [in] */ Int32 segmentNum,
    /* [in] */ Int32 libNum,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 transpose,
    /* [in] */ Int32 muteFlags,
    /* [in] */ Byte userID)
{
    return FALSE;
}

Boolean JetPlayer::native_queueJetSegmentMuteArray(
    /* [in] */ Int32 segmentNum,
    /* [in] */ Int32 libNum,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 transpose,
    /* [in] */ ArrayOf<Boolean>* muteArray,
    /* [in] */ Byte userID)
{
    return FALSE;
}

Boolean JetPlayer::native_setMuteFlags(
    /* [in] */ Int32 muteFlags,
    /* [in] */ Boolean sync)
{
    return FALSE;
}

Boolean JetPlayer::native_setMuteArray(
    /* [in] */ ArrayOf<Boolean>* muteArray,
    /* [in] */ Boolean sync)
{
    return FALSE;
}

Boolean JetPlayer::native_setMuteFlag(
    /* [in] */ Int32 trackId,
    /* [in] */ Boolean muteFlag,
    /* [in] */ Boolean sync)
{
    return FALSE;
}

Boolean JetPlayer::native_triggerClip(
    /* [in] */ Int32 clipId)
{
    return FALSE;
}

Boolean JetPlayer::native_clearQueue()
{
    return FALSE;
}

//---------------------------------------------------------
// Called exclusively by native code
//--------------------
void JetPlayer::PostEventFromNative(
    /* [in] */ IInterface* jetplayer_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    //logd("Event posted from the native side: event="+ what + " args="+ arg1+" "+arg2);
    /*JetPlayer jet = (JetPlayer)((WeakReference)jetplayer_ref).get();

    if ((jet != NULL) && (jet.mEventHandler != NULL)) {
        Message m =
            jet.mEventHandler.obtainMessage(what, arg1, arg2, NULL);
        jet.mEventHandler.sendMessage(m);
    }*/

}

void JetPlayer::Logd(
    /* [in] */ String msg)
{
    //Log.d(TAG, "[ android.media.JetPlayer ] " + msg);
}

void JetPlayer::Loge(
    /* [in] */ String msg)
{
    //Log.e(TAG, "[ android.media.JetPlayer ] " + msg);
}

