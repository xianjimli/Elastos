
#include "media/CSoundPool.h"
const CString CSoundPool::TAG = "SoundPool"; 

CSoundPool::CSoundPool()
    :DEBUG(FALSE)
    ,SAMPLE_LOADED(1)
{

}

CSoundPool::~CSoundPool()
{
    ReleaseResources();
}

ECode CSoundPool::constructor(
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
    return E_NOT_IMPLEMENTED;
}

ECode CSoundPool::Load(
    /* [in] */ const String& path,
    /* [in] */ Int32 priority,
    /* [out] */ Int32 * pID)
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

    return E_NOT_IMPLEMENTED;
}

ECode CSoundPool::LoadEx(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Int32 priority,
    /* [out] */ Int32 * pID)
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

    return E_NOT_IMPLEMENTED;
}

ECode CSoundPool::LoadEx2(
    /* [in] */ IAssetFileDescriptor* afd,
    /* [in] */ Int32 priority,
    /* [out] */ Int32 * pID)
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

    return E_NOT_IMPLEMENTED;
}

ECode CSoundPool::LoadEx3(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length,
    /* [in] */ Int32 priority,
    /* [out] */ Int32 * pID)
{
    return _load(fd, offset, length, priority);
}

Int32 CSoundPool::_load(
    /* [in] */ String uri,
    /* [in] */ Int32 priority)
{
    return 0;
}

Int32 CSoundPool::_load(
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
 * successfully unloaded, false if the sound was already unloaded.
 *
 * @param soundID a soundID returned by the load() function
 * @return true if just unloaded, false if previously unloaded
 */
ECode CSoundPool::Unload(
    /* [in] */ Int32 soundID,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CSoundPool::Play(
    /* [in] */ Int32 soundID,
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume,
    /* [in] */ Int32 priority,
    /* [in] */ Int32 loop,
    /* [in] */ Float rate,
    /* [out] */ Int32* id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CSoundPool::Pause(
    /* [in] */ Int32 streamID)
{
    // TODO: Add your code here
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
ECode CSoundPool::Resume(
    /* [in] */ Int32 streamID)
{
    // TODO: Add your code here
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
ECode CSoundPool::AutoPause()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Resume all previously active streams.
 *
 * Automatically resumes all streams that were paused in previous
 * calls to autoPause().
 */
ECode CSoundPool::AutoResume()
{
    // TODO: Add your code here
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
ECode CSoundPool::Stop(
    /* [in] */ Int32 streamID)
{
    // TODO: Add your code here
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
ECode CSoundPool::SetVolume(
    /* [in] */ Int32 streamID,
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume)
{
    // TODO: Add your code here
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
ECode CSoundPool::SetPriority(
    /* [in] */ Int32 streamID,
    /* [in] */ Int32 priority)
{
    // TODO: Add your code here
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
ECode CSoundPool::SetLoop(
    /* [in] */ Int32 streamID,
    /* [in] */ Int32 loop)
{
    // TODO: Add your code here
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
ECode CSoundPool::SetRate(
    /* [in] */ Int32 streamID,
    /* [in] */ Float rate)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSoundPool::SetOnLoadCompleteListener(
    /* [in] */ IOnLoadCompleteListener* listener)
{
    //synchronized(mLock) {
        mOnLoadCompleteListener = listener;
    //}
    return E_NOT_IMPLEMENTED;
}

//public EventHandler(SoundPool soundPool, Looper looper) {
//    super(looper);
//    mSoundPool = soundPool;
//}


ECode CSoundPool::EventHandler::HandleMessage(
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


PInterface CSoundPool::EventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CSoundPool::EventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSoundPool::EventHandler::Release()
{
    return ElRefBase::Release();
}

ECode CSoundPool::EventHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

void CSoundPool::PostEventFromNative(
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
 * should be set to null.
 */
ECode CSoundPool::ReleaseResources()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

Int32 CSoundPool::native_setup(
    /* [in] */ IInterface* weakRef,
    /* [in] */ Int32 maxStreams,
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 srcQuality)
{
    return 0;
}