
#include "media/SoundPool.h"

SoundPool::SoundPool()
{

}

/**
 * Constructor. Constructs a SoundPool object with the following
 * characteristics:
 *
 * @param maxStreams the maximum number of simultaneous streams for this
 *                   SoundPool object
 * @param streamType the audio stream type as described in AudioManager
 *                   For example, game applications will normally use
 *                   {@link AudioManager#STREAM_MUSIC}.
 * @param srcQuality the sample-rate converter quality. Currently has no
 *                   effect. Use 0 for the default.
 * @return a SoundPool object, or NULL if creation failed
 */
SoundPool::SoundPool(
    /* [in] */ Int32 maxStreams,
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 srcQuality)
{

    // do native setup
    //if (native_setup(new WeakReference(this), maxStreams, streamType, srcQuality) != 0) {
    //    throw new RuntimeException("Native setup failed");
    //}
    //mLock = new Object();

    //// setup message handler
    //Looper looper;
    //if ((looper = Looper.myLooper()) != NULL) {
    //    mEventHandler = new EventHandler(this, looper);
    //} else if ((looper = Looper.getMainLooper()) != NULL) {
    //    mEventHandler = new EventHandler(this, looper);
    //} else {
    //    mEventHandler = NULL;
    //}

}

/**
 * Load the sound from the specified path.
 *
 * @param path the path to the audio file
 * @param priority the priority of the sound. Currently has no effect. Use
 *                 a value of 1 for future compatibility.
 * @return a sound ID. This value can be used to play or unload the sound.
 */
Int32 SoundPool::Load(
    /* [in] */ String path,
    /* [in] */ Int32 priority)
{
    // pass network streams to player
    //if (path.startsWith("http:"))
    //    return _load(path, priority);

    //// try local path
    //Int32 id = 0;
    //try {
    //    File f = new File(path);
    //    if (f != NULL) {
    //        ParcelFileDescriptor fd = ParcelFileDescriptor.open(f, ParcelFileDescriptor.MODE_READ_ONLY);
    //        if (fd != NULL) {
    //            id = _load(fd.getFileDescriptor(), 0, f.length(), priority);
    //            fd.close();
    //        }
    //    }
    //} catch (java.io.IOException e) {
    //    Log.e(TAG, "error loading " + path);
    //}
    //return id;

    return 0;
}

/**
 * Load the sound from the specified APK resource.
 *
 * Note that the extension is dropped. For example, if you want to load
 * a sound from the raw resource file "explosion.mp3", you would specify
 * "R.raw.explosion" as the resource ID. Note that this means you cannot
 * have both an "explosion.wav" and an "explosion.mp3" in the res/raw
 * directory.
 *
 * @param context the application context
 * @param resId the resource ID
 * @param priority the priority of the sound. Currently has no effect. Use
 *                 a value of 1 for future compatibility.
 * @return a sound ID. This value can be used to play or unload the sound.
 */
Int32 SoundPool::Load(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Int32 priority)
{
    //AssetFileDescriptor afd = context.getResources().openRawResourceFd(resId);
    //Int32 id = 0;
    //if (afd != NULL) {
    //    id = _load(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength(), priority);
    //    try {
    //        afd.close();
    //    } catch (java.io.IOException ex) {
    //        //Log.d(TAG, "close failed:", ex);
    //    }
    //}
    //return id;

    return 0;
}

/**
 * Load the sound from an asset file descriptor.
 *
 * @param afd an asset file descriptor
 * @param priority the priority of the sound. Currently has no effect. Use
 *                 a value of 1 for future compatibility.
 * @return a sound ID. This value can be used to play or unload the sound.
 */
Int32 SoundPool::Load(
    /* [in] */ IAssetFileDescriptor* afd,
    /* [in] */ Int32 priority)
{
    //if (afd != NULL) {
    //    Int64 len;
    //    afd->GetLength(&len);
    //    if (len < 0) {
    //        //throw new AndroidRuntimeException("no length for fd");
    //    }

    //    AutoPtr<IFileDescriptor> fd;
    //    afd->GetFileDescriptor((IFileDescriptor**)&fd);

    //    Int64 offset;
    //    afd->GetStartOffset(&offset);

    //    return _load(fd, offset, len, priority);
    //} else {
    //    return 0;
    //}

    return 0;
}

/**
 * Load the sound from a FileDescriptor.
 *
 * This version is useful if you store multiple sounds in a single
 * binary. The offset specifies the offset from the start of the file
 * and the length specifies the length of the sound within the file.
 *
 * @param fd a FileDescriptor object
 * @param offset offset to the start of the sound
 * @param length length of the sound
 * @param priority the priority of the sound. Currently has no effect. Use
 *                 a value of 1 for future compatibility.
 * @return a sound ID. This value can be used to play or unload the sound.
 */
Int32 SoundPool::Load(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length,
    /* [in] */ Int32 priority)
{
    return _load(fd, offset, length, priority);
}

Int32 SoundPool::_load(
    /* [in] */ String uri,
    /* [in] */ Int32 priority)
{
    return 0;
}

Int32 SoundPool::_load(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length,
    /* [in] */ Int32 priority)
{
    return 0;
}

/**
 * Unload a sound from a sound ID.
 *
 * Unloads the sound specified by the soundID. This is the value
 * returned by the load() function. Returns true if the sound is
 * successfully unloaded, FALSE if the sound was already unloaded.
 *
 * @param soundID a soundID returned by the load() function
 * @return true if just unloaded, FALSE if previously unloaded
 */
Boolean SoundPool::Unload(
    /* [in] */ Int32 soundID)
{
    return FALSE;
}

/**
 * Play a sound from a sound ID.
 *
 * Play the sound specified by the soundID. This is the value
 * returned by the load() function. Returns a non-zero streamID
 * if successful, zero if it fails. The streamID can be used to
 * further control playback. Note that calling play() may cause
 * another sound to stop playing if the maximum number of active
 * streams is exceeded. A loop value of -1 means loop forever,
 * a value of 0 means don't loop, other values indicate the
 * number of repeats, e.g. a value of 1 plays the audio twice.
 * The playback rate allows the application to vary the playback
 * rate (pitch) of the sound. A value of 1.0 means play back at
 * the original frequency. A value of 2.0 means play back twice
 * as fast, and a value of 0.5 means playback at half speed.
 *
 * @param soundID a soundID returned by the load() function
 * @param leftVolume left volume value (range = 0.0 to 1.0)
 * @param rightVolume right volume value (range = 0.0 to 1.0)
 * @param priority stream priority (0 = lowest priority)
 * @param loop loop mode (0 = no loop, -1 = loop forever)
 * @param rate playback rate (1.0 = normal playback, range 0.5 to 2.0)
 * @return non-zero streamID if successful, zero if failed
 */
Int32 SoundPool::Play(
    /* [in] */ Int32 soundID,
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume,
    /* [in] */ Int32 priority,
    /* [in] */ Int32 loop,
    /* [in] */ Float rate)
{
    return 0;
}

/**
 * Pause a playback stream.
 *
 * Pause the stream specified by the streamID. This is the
 * value returned by the play() function. If the stream is
 * playing, it will be paused. If the stream is not playing
 * (e.g. is stopped or was previously paused), calling this
 * function will have no effect.
 *
 * @param streamID a streamID returned by the play() function
 */
ECode SoundPool::Pause(
    /* [in] */ Int32 streamID)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Resume a playback stream.
 *
 * Resume the stream specified by the streamID. This
 * is the value returned by the play() function. If the stream
 * is paused, this will resume playback. If the stream was not
 * previously paused, calling this function will have no effect.
 *
 * @param streamID a streamID returned by the play() function
 */
ECode SoundPool::Resume(
    /* [in] */ Int32 streamID)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Pause all active streams.
 *
 * Pause all streams that are currently playing. This function
 * iterates through all the active streams and pauses any that
 * are playing. It also sets a flag so that any streams that
 * are playing can be resumed by calling autoResume().
 */
ECode SoundPool::AutoPause()
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Resume all previously active streams.
 *
 * Automatically resumes all streams that were paused in previous
 * calls to autoPause().
 */
ECode SoundPool::AutoResume()
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Stop a playback stream.
 *
 * Stop the stream specified by the streamID. This
 * is the value returned by the play() function. If the stream
 * is playing, it will be stopped. It also releases any native
 * resources associated with this stream. If the stream is not
 * playing, it will have no effect.
 *
 * @param streamID a streamID returned by the play() function
 */
ECode SoundPool::Stop(
    /* [in] */ Int32 streamID)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Set stream volume.
 *
 * Sets the volume on the stream specified by the streamID.
 * This is the value returned by the play() function. The
 * value must be in the range of 0.0 to 1.0. If the stream does
 * not exist, it will have no effect.
 *
 * @param streamID a streamID returned by the play() function
 * @param leftVolume left volume value (range = 0.0 to 1.0)
 * @param rightVolume right volume value (range = 0.0 to 1.0)
 */
ECode SoundPool::SetVolume(
    /* [in] */ Int32 streamID,
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Change stream priority.
 *
 * Change the priority of the stream specified by the streamID.
 * This is the value returned by the play() function. Affects the
 * order in which streams are re-used to play new sounds. If the
 * stream does not exist, it will have no effect.
 *
 * @param streamID a streamID returned by the play() function
 */
ECode SoundPool::SetPriority(
    /* [in] */ Int32 streamID,
    /* [in] */ Int32 priority)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Set loop mode.
 *
 * Change the loop mode. A loop value of -1 means loop forever,
 * a value of 0 means don't loop, other values indicate the
 * number of repeats, e.g. a value of 1 plays the audio twice.
 * If the stream does not exist, it will have no effect.
 *
 * @param streamID a streamID returned by the play() function
 * @param loop loop mode (0 = no loop, -1 = loop forever)
 */
ECode SoundPool::SetLoop(
    /* [in] */ Int32 streamID,
    /* [in] */ Int32 loop)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Change playback rate.
 *
 * The playback rate allows the application to vary the playback
 * rate (pitch) of the sound. A value of 1.0 means playback at
 * the original frequency. A value of 2.0 means playback twice
 * as fast, and a value of 0.5 means playback at half speed.
 * If the stream does not exist, it will have no effect.
 *
 * @param streamID a streamID returned by the play() function
 * @param rate playback rate (1.0 = normal playback, range 0.5 to 2.0)
 */
ECode SoundPool::SetRate(
    /* [in] */ Int32 streamID,
    /* [in] */ Float rate)
{
    return E_NOT_IMPLEMENTED;
}



/**
 * Sets the callback hook for the OnLoadCompleteListener.
 */
ECode SoundPool::SetOnLoadCompleteListener(
    /* [in] */ IOnLoadCompleteListener* listener)
{
    //synchronized(mLock) {
        mOnLoadCompleteListener = listener;
    //}

        return NOERROR;
}

//public EventHandler(SoundPool soundPool, Looper looper) {
//    super(looper);
//    mSoundPool = soundPool;
//}


ECode SoundPool::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    /*switch(msg.what) {
    case SAMPLE_LOADED:
        if (DEBUG) Log.d(TAG, "Sample " + msg.arg1 + " loaded");
        synchronized(mLock) {
            if (mOnLoadCompleteListener != NULL) {
                mOnLoadCompleteListener.onLoadComplete(mSoundPool, msg.arg1, msg.arg2);
            }
        }
        break;
    default:
        Log.e(TAG, "Unknown message type " + msg.what);
        return;
    }*/

    return NOERROR;
}


PInterface SoundPool::EventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 SoundPool::EventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SoundPool::EventHandler::Release()
{
    return ElRefBase::Release();
}

ECode SoundPool::EventHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

// post event from native code to message handler
void SoundPool::PostEventFromNative(
    /* [in] */ IInterface* weakRef,
    /* [in] */ Int32 msg,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    /*SoundPool soundPool = (SoundPool)((WeakReference)weakRef).get();
    if (soundPool == NULL)
        return;

    if (soundPool.mEventHandler != NULL) {
        Message m = soundPool.mEventHandler.obtainMessage(msg, arg1, arg2, obj);
        soundPool.mEventHandler.sendMessage(m);
    }*/
}

/**
 * Release the SoundPool resources.
 *
 * Release all memory and native resources used by the SoundPool
 * object. The SoundPool can no longer be used and the reference
 * should be set to NULL.
 */
void SoundPool::Release()
{

}

Int32 SoundPool::native_setup(
    /* [in] */ IInterface* weakRef,
    /* [in] */ Int32 maxStreams,
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 srcQuality)
{
    return 0;
}

void SoundPool::Finalize()
{
    Release();
}

