
#include "media/MediaPlayer.h"
#include "media/CMediaPlayer.h"
#include "media/CMetadata.h"

/**
 * Default constructor. Consider using one of the create() methods for
 * synchronously instantiating a MediaPlayer from a Uri or resource.
 * <p>When done with the MediaPlayer, you should call  {@link #release()},
 * to free the resources. If not released, too many MediaPlayer instances may
 * result in an exception.</p>
 */
MediaPlayer::MediaPlayer() 
{

    //Looper looper;
    //if ((looper = Looper.myLooper()) != NULL) {
    //    mEventHandler = new EventHandler(this, looper);
    //} else if ((looper = Looper.getMainLooper()) != NULL) {
    //    mEventHandler = new EventHandler(this, looper);
    //} else {
    //    mEventHandler = NULL;
    //}

    ///* Native setup requires a weak reference to our object.
    // * It's easier to create it here than in C++.
    // */
    //Native_setup(new WeakReference<MediaPlayer>(this));
}

/*
 * Update the MediaPlayer ISurface. Call after updating mSurface.
 */
void MediaPlayer::_setVideoSurface()
{

}

/**
 * Create a request parcel which can be routed to the native media
 * player using {@link #invoke(Parcel, Parcel)}. The Parcel
 * returned has the proper InterfaceToken set. The caller should
 * not overwrite that token, i.e it can only append data to the
 * Parcel.
 *
 * @return A parcel suitable to hold a request for the native
 * player.
 * {@hide}
 */
AutoPtr<IParcel> MediaPlayer::NewRequest()
{
    AutoPtr<IParcel> parcel;// = Parcel.obtain();
    //parcel->WriteInterfaceToken(IMEDIA_PLAYER);
    return parcel;
}

/**
 * Invoke a generic method on the native player using opaque
 * parcels for the request and reply. Both payloads' format is a
 * convention between the java caller and the native player.
 * Must be called after setDataSource to make sure a native player
 * exists.
 *
 * @param request Parcel with the data for the extension. The
 * caller must use {@link #newRequest()} to get one.
 *
 * @param reply Output parcel with the data returned by the
 * native player.
 *
 * @return The status code see utils/Errors.h
 * {@hide}
 */
Int32 MediaPlayer::Invoke(
    /* [in] */ IParcel* request, 
    /* [in] */ IParcel* reply) 
{
    Int32 retcode = Native_invoke(request, reply);
    //reply->SetDataPosition(0);
    return retcode;
}

/**
 * Sets the SurfaceHolder to use for displaying the video portion of the media.
 * This call is optional. Not calling it when playing back a video will
 * result in only the audio track being played.
 *
 * @param sh the SurfaceHolder to use for video display
 */
ECode MediaPlayer::SetDisplay(
    /* [in] */ ISurfaceHolder* sh)
{
    mSurfaceHolder = sh;
    if (sh != NULL) {
        sh->GetSurface((ISurface**)&mSurface);
    } else {
        mSurface = NULL;
    }
    _setVideoSurface();
    UpdateSurfaceScreenOn();

    return NOERROR;
}

/**
 * Convenience method to create a MediaPlayer for a given Uri.
 * On success, {@link #prepare()} will already have been called and must not be called again.
 * <p>When done with the MediaPlayer, you should call  {@link #release()},
 * to free the resources. If not released, too many MediaPlayer instances will
 * result in an exception.</p>
 *
 * @param context the Context to use
 * @param uri the Uri from which to get the datasource
 * @return a MediaPlayer object, or NULL if creation failed
 */
AutoPtr<IMediaPlayer> MediaPlayer::Create(
    /* [in] */ IContext* context, 
    /* [in] */ IUri* uri)
{
    return Create(context, uri, NULL);
}

/**
 * Convenience method to create a MediaPlayer for a given Uri.
 * On success, {@link #prepare()} will already have been called and must not be called again.
 * <p>When done with the MediaPlayer, you should call  {@link #release()},
 * to free the resources. If not released, too many MediaPlayer instances will
 * result in an exception.</p>
 *
 * @param context the Context to use
 * @param uri the Uri from which to get the datasource
 * @param holder the SurfaceHolder to use for displaying the video
 * @return a MediaPlayer object, or NULL if creation failed
 */
AutoPtr<IMediaPlayer> MediaPlayer::Create(
    /* [in] */ IContext* context, 
    /* [in] */ IUri* uri, 
    /* [in] */ ISurfaceHolder* holder)
{
    //try {
        AutoPtr<IMediaPlayer> mp;
        CMediaPlayer::New((IMediaPlayer**)&mp);
        mp->SetDataSource(context, uri);
        if (holder != NULL) {
            mp->SetDisplay(holder);
        }
        ((CMediaPlayer*)mp.Get())->Prepare();
        return mp;
    //} catch (IOException ex) {
    //    Log.d(TAG, "create failed:", ex);
    //    // fall through
    //} catch (IllegalArgumentException ex) {
    //    Log.d(TAG, "create failed:", ex);
    //    // fall through
    //} catch (SecurityException ex) {
    //    Log.d(TAG, "create failed:", ex);
    //    // fall through
    //}

    //return NULL;
}

/**
 * Convenience method to create a MediaPlayer for a given resource id.
 * On success, {@link #prepare()} will already have been called and must not be called again.
 * <p>When done with the MediaPlayer, you should call  {@link #release()},
 * to free the resources. If not released, too many MediaPlayer instances will
 * result in an exception.</p>
 *
 * @param context the Context to use
 * @param resid the raw resource id (<var>R.raw.&lt;something></var>) for
 *              the resource to use as the datasource
 * @return a MediaPlayer object, or NULL if creation failed
 */
AutoPtr<IMediaPlayer> MediaPlayer::Create(
    /* [in] */ IContext* context, 
    /* [in] */ Int32 resid)
{
    //try {
    //    AssetFileDescriptor afd = context.getResources().openRawResourceFd(resid);
    //    if (afd == NULL) return NULL;

    //    MediaPlayer mp = new MediaPlayer();
    //    mp.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
    //    afd.close();
    //    mp.prepare();
    //    return mp;
    //} catch (IOException ex) {
    //    Log.d(TAG, "create failed:", ex);
    //    // fall through
    //} catch (IllegalArgumentException ex) {
    //    Log.d(TAG, "create failed:", ex);
    //   // fall through
    //} catch (SecurityException ex) {
    //    Log.d(TAG, "create failed:", ex);
    //    // fall through
    //}
    return NULL;
}

/**
 * Sets the data source as a content Uri.
 *
 * @param context the Context to use when resolving the Uri
 * @param uri the Content URI of the data you want to play
 * @throws IllegalStateException if it is called in an invalid state
 */
ECode MediaPlayer::SetDataSource(
    /* [in] */ IContext* context, 
    /* [in] */ IUri* uri)
{
    return SetDataSource(context, uri, NULL);
}

/**
 * Sets the data source as a content Uri.
 *
 * @param context the Context to use when resolving the Uri
 * @param uri the Content URI of the data you want to play
 * @param headers the headers to be sent together with the request for the data
 * @throws IllegalStateException if it is called in an invalid state
 * @hide pending API council
 */
ECode MediaPlayer::SetDataSource(
    /* [in] */ IContext* context, 
    /* [in] */ IUri* uri, 
    /* [in] */ Map<String, String>* headers)
{
    //String scheme = uri.getScheme();
    //if(scheme == NULL || scheme.equals("file")) {
    //    setDataSource(uri.getPath());
    //    return;
    //}

    //AssetFileDescriptor fd = NULL;
    //try {
    //    ContentResolver resolver = context.getContentResolver();
    //    fd = resolver.openAssetFileDescriptor(uri, "r");
    //    if (fd == NULL) {
    //        return;
    //    }
    //    // Note: using getDeclaredLength so that our behavior is the same
    //    // as previous versions when the content provider is returning
    //    // a full file.
    //    if (fd.getDeclaredLength() < 0) {
    //        setDataSource(fd.getFileDescriptor());
    //    } else {
    //        setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getDeclaredLength());
    //    }
    //    return;
    //} catch (SecurityException ex) {
    //} catch (IOException ex) {
    //} finally {
    //    if (fd != NULL) {
    //        fd.close();
    //    }
    //}
    //Log.d(TAG, "Couldn't open file on client side, trying server side");
    //SetDataSource(uri.toString(), headers);
    return NOERROR;
}

/**
 * Sets the data source (file-path or http/rtsp URL) to use.
 *
 * @param path the path of the file, or the http/rtsp URL of the stream you want to play
 * @throws IllegalStateException if it is called in an invalid state
 */
void MediaPlayer::SetDataSource(
    /* [in] */ String path)
{

}

/**
 * Sets the data source (file-path or http/rtsp URL) to use.
 *
 * @param path the path of the file, or the http/rtsp URL of the stream you want to play
 * @param headers the headers associated with the http request for the stream you want to play
 * @throws IllegalStateException if it is called in an invalid state
 * @hide pending API council
 */
void MediaPlayer::SetDataSource(
    /* [in] */ String path,  
    /* [in] */ Map<String, String>* headers)
{

}

/**
 * Sets the data source (FileDescriptor) to use. It is the caller's responsibility
 * to close the file descriptor. It is safe to do so as soon as this call returns.
 *
 * @param fd the FileDescriptor for the file you want to play
 * @throws IllegalStateException if it is called in an invalid state
 */
ECode MediaPlayer::SetDataSource(
    /* [in] */ IFileDescriptor* fd)
{
    // intentionally less than LONG_MAX
    SetDataSource(fd, 0, 0x7ffffffffffffffL);

    return NOERROR;
}

/**
 * Sets the data source (FileDescriptor) to use.  The FileDescriptor must be
 * seekable (N.B. a LocalSocket is not seekable). It is the caller's responsibility
 * to close the file descriptor. It is safe to do so as soon as this call returns.
 *
 * @param fd the FileDescriptor for the file you want to play
 * @param offset the offset into the file where the data to be played starts, in bytes
 * @param length the length in bytes of the data to be played
 * @throws IllegalStateException if it is called in an invalid state
 */
void MediaPlayer::SetDataSource(
    /* [in] */ IFileDescriptor* fd, 
    /* [in] */ Int64 offset, 
    /* [in] */ Int64 length)
{

}

/**
 * Prepares the player for playback, synchronously.
 *
 * After setting the datasource and the display surface, you need to either
 * call prepare() or prepareAsync(). For files, it is OK to call prepare(),
 * which blocks until MediaPlayer is ready for playback.
 *
 * @throws IllegalStateException if it is called in an invalid state
 */
void MediaPlayer::Prepare()
{

}

/**
 * Prepares the player for playback, asynchronously.
 *
 * After setting the datasource and the display surface, you need to either
 * call prepare() or prepareAsync(). For streams, you should call prepareAsync(),
 * which returns immediately, rather than blocking until enough data has been
 * buffered.
 *
 * @throws IllegalStateException if it is called in an invalid state
 */
void MediaPlayer::PrepareAsync()
{

}

/**
 * Starts or resumes playback. If playback had previously been paused,
 * playback will continue from where it was paused. If playback had
 * been stopped, or never started before, playback will start at the
 * beginning.
 *
 * @throws IllegalStateException if it is called in an invalid state
 */
ECode MediaPlayer::Start()
{
    StayAwake(TRUE);
    _start();

    return NOERROR;
}

void MediaPlayer::_start()
{

}

/**
 * Stops playback after playback has been stopped or paused.
 *
 * @throws IllegalStateException if the internal player engine has not been
 * initialized.
 */
ECode MediaPlayer::Stop()
{
    StayAwake(FALSE);
    _stop();

    return NOERROR;
}

void MediaPlayer::_stop()
{

}

/**
 * Pauses playback. Call start() to resume.
 *
 * @throws IllegalStateException if the internal player engine has not been
 * initialized.
 */
ECode MediaPlayer::Pause()
{
    StayAwake(FALSE);
    _pause();

    return NOERROR;
}

void MediaPlayer::_pause()
{

}

/**
 * Set the low-level power management behavior for this MediaPlayer.  This
 * can be used when the MediaPlayer is not playing through a SurfaceHolder
 * set with {@link #setDisplay(SurfaceHolder)} and thus can use the
 * high-level {@link #setScreenOnWhilePlaying(Boolean)} feature.
 *
 * <p>This function has the MediaPlayer access the low-level power manager
 * service to control the device's power usage while playing is occurring.
 * The parameter is a combination of {@link android.os.PowerManager} wake flags.
 * Use of this method requires {@link android.Manifest.permission#WAKE_LOCK}
 * permission.
 * By default, no attempt is made to keep the device awake during playback.
 *
 * @param context the Context to use
 * @param mode    the power/wake mode to set
 * @see android.os.PowerManager
 */
ECode MediaPlayer::SetWakeMode(
    /* [in] */ IContext* context, 
    /* [in] */ Int32 mode)
{
    Boolean washeld = FALSE;
    /*if (mWakeLock != NULL) {
        if (mWakeLock.isHeld()) {
            washeld = TRUE;
            mWakeLock.release();
        }
        mWakeLock = NULL;
    }

    PowerManager pm = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
    mWakeLock = pm.newWakeLock(mode|PowerManager.ON_AFTER_RELEASE, MediaPlayer.class.getName());
    mWakeLock.setReferenceCounted(FALSE);
    if (washeld) {
        mWakeLock.acquire();
    }*/

    return NOERROR;
}

/**
 * Control whether we should use the attached SurfaceHolder to keep the
 * screen on while video playback is occurring.  This is the preferred
 * method over {@link #setWakeMode} where possible, since it doesn't
 * require that the application have permission for low-level wake lock
 * access.
 *
 * @param screenOn Supply TRUE to keep the screen on, FALSE to allow it
 * to turn off.
 */
ECode MediaPlayer::SetScreenOnWhilePlaying(
    /* [in] */ Boolean screenOn)
{
    if (mScreenOnWhilePlaying != screenOn) {
        mScreenOnWhilePlaying = screenOn;
        UpdateSurfaceScreenOn();
    }

    return NOERROR;
}

void MediaPlayer::StayAwake(
    /* [in] */ Boolean awake)
{
    /*if (mWakeLock != NULL) {
        if (awake && !mWakeLock.isHeld()) {
            mWakeLock.acquire();
        } else if (!awake && mWakeLock.isHeld()) {
            mWakeLock.release();
        }
    }*/
    mStayAwake = awake;
    UpdateSurfaceScreenOn();
}

void MediaPlayer::UpdateSurfaceScreenOn()
{
    if (mSurfaceHolder != NULL) {
        mSurfaceHolder->SetKeepScreenOn(mScreenOnWhilePlaying && mStayAwake);
    }
}

/**
 * Returns the width of the video.
 *
 * @return the width of the video, or 0 if there is no video,
 * no display surface was set, or the width has not been determined
 * yet. The OnVideoSizeChangedListener can be registered via
 * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}
 * to provide a notification when the width is available.
 */
Int32 MediaPlayer::GetVideoWidth()
{
    return 0;
}

/**
 * Returns the height of the video.
 *
 * @return the height of the video, or 0 if there is no video,
 * no display surface was set, or the height has not been determined
 * yet. The OnVideoSizeChangedListener can be registered via
 * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}
 * to provide a notification when the height is available.
 */
Int32 MediaPlayer::GetVideoHeight()
{
    return 0;
}

/**
 * Checks whether the MediaPlayer is playing.
 *
 * @return TRUE if currently playing, FALSE otherwise
 */
Boolean MediaPlayer::IsPlaying()
{
    return FALSE;
}

/**
 * Seeks to specified time position.
 *
 * @param msec the offset in milliseconds from the start to seek to
 * @throws IllegalStateException if the internal player engine has not been
 * initialized
 */
void MediaPlayer::SeekTo(
    /* [in] */ Int32 msec)
{

}

/**
 * Gets the current playback position.
 *
 * @return the current position in milliseconds
 */
Int32 MediaPlayer::GetCurrentPosition()
{
    return 0;
}

/**
 * Gets the duration of the file.
 *
 * @return the duration in milliseconds
 */
Int32 MediaPlayer::GetDuration()
{
    return 0;
}

/**
 * Gets the media metadata.
 *
 * @param update_only controls whether the full set of available
 * metadata is returned or just the set that changed since the
 * last call. See {@see #METADATA_UPDATE_ONLY} and {@see
 * #METADATA_ALL}.
 *
 * @param apply_filter if TRUE only metadata that matches the
 * filter is returned. See {@see #APPLY_METADATA_FILTER} and {@see
 * #BYPASS_METADATA_FILTER}.
 *
 * @return The metadata, possibly empty. NULL if an error occured.
 // FIXME: unhide.
 * {@hide}
 */
AutoPtr<IMetadata> MediaPlayer::GetMetadata(
    /* [in] */ Boolean update_only,
    /* [in] */ Boolean apply_filter)
{
    AutoPtr<IParcel> reply;// = Parcel.obtain();
    AutoPtr<IMetadata> data;
    CMetadata::New((IMetadata**)&data);

    if (!Native_getMetadata(update_only, apply_filter, reply)) {
        //reply->Recycle();
        return NULL;
    }

    // Metadata takes over the parcel, don't recycle it unless
    // there is an error.
    Boolean res;
    data->Parse(reply, &res);
    if (!res) {
        //reply->Recycle();
        return NULL;
    }
    return data;
}

/**
 * Set a filter for the metadata update notification and update
 * retrieval. The caller provides 2 set of metadata keys, allowed
 * and blocked. The blocked set always takes precedence over the
 * allowed one.
 * Metadata.MATCH_ALL and Metadata.MATCH_NONE are 2 sets available as
 * shorthands to allow/block all or no metadata.
 *
 * By default, there is no filter set.
 *
 * @param allow Is the set of metadata the client is interested
 *              in receiving new notifications for.
 * @param block Is the set of metadata the client is not interested
 *              in receiving new notifications for.
 * @return The call status code.
 *
 // FIXME: unhide.
 * {@hide}
 */
Int32 MediaPlayer::SetMetadataFilter(
    /* [in] */ Set<Int32>* allow, 
    /* [in] */ Set<Int32>* block)
{
    // Do our serialization manually instead of calling
    // Parcel.writeArray since the sets are made of the same type
    // we avoid paying the price of calling writeValue (used by
    // writeArray) which burns an extra Int32 per element to encode
    // the type.
    AutoPtr<IParcel> request =  NewRequest();

    // The parcel starts already with an interface token. There
    // are 2 filters. Each one starts with a 4bytes number to
    // store the len followed by a number of Int32 (4 bytes as well)
    // representing the metadata type.
    Int32 capacity;
    //request->DataSize(&capacity);
    //capacity += 4 * (1 + allow->Size() + 1 + block->Size());

    Int32 cap;
    //request->DataCapacity(&cap);
    if (cap < capacity) {
        //request->SetDataCapacity(capacity);
    }

    //request->WriteInt32(allow->size());
    /*for(Integer t: allow) {
        request->WriteInt32(t);
    }*/
    //request->WriteInt32(block->Size());
    /*for(Integer t: block) {
        request->WriteInt32(t);
    }*/
    return Native_setMetadataFilter(request);
}

/**
 * Releases resources associated with this MediaPlayer object.
 * It is considered good practice to call this method when you're
 * done using the MediaPlayer.
 */
ECode MediaPlayer::ReleaseEx()
{
    StayAwake(FALSE);
    UpdateSurfaceScreenOn();
    mOnPreparedListener = NULL;
    mOnBufferingUpdateListener = NULL;
    mOnCompletionListener = NULL;
    mOnSeekCompleteListener = NULL;
    mOnErrorListener = NULL;
    mOnInfoListener = NULL;
    mOnVideoSizeChangedListener = NULL;
    _release();

    return NOERROR;
}

void MediaPlayer::_release()
{

}

/**
 * Resets the MediaPlayer to its uninitialized state. After calling
 * this method, you will have to initialize it again by setting the
 * data source and calling prepare().
 */
ECode MediaPlayer::Reset()
{
    StayAwake(FALSE);
    _reset();
    // make sure none of the listeners get called anymore
    //mEventHandler->RemoveCallbacksAndMessages(NULL);
}

void MediaPlayer::_reset()
{

}

/**
 * Suspends the MediaPlayer. The only methods that may be called while
 * suspended are {@link #reset()}, {@link #release()} and {@link #resume()}.
 * MediaPlayer will release its hardware resources as far as
 * possible and reasonable. A successfully suspended MediaPlayer will
 * cease sending events.
 * If suspension is successful, this method returns TRUE, otherwise
 * FALSE is returned and the player's state is not affected.
 * @hide
 */
Boolean MediaPlayer::Suspend()
{
    if (Native_suspend_resume(TRUE) < 0) {
        return FALSE;
    }

    StayAwake(FALSE);

    // make sure none of the listeners get called anymore
    //mEventHandler->RemoveCallbacksAndMessages(NULL);

    return TRUE;
}

/**
 * Resumes the MediaPlayer. Only to be called after a previous (successful)
 * call to {@link #suspend()}.
 * MediaPlayer will return to a state close to what it was in before
 * suspension.
 * @hide
 */
Boolean MediaPlayer::Resume()
{
    if (Native_suspend_resume(FALSE) < 0) {
        return FALSE;
    }

    if (IsPlaying()) {
        StayAwake(TRUE);
    }

    return TRUE;
}

/**
 * @hide
 */
Int32 MediaPlayer::Native_suspend_resume(
    /* [in] */ Boolean isSuspend)
{
    return 0;
}

/**
 * Sets the audio stream type for this MediaPlayer. See {@link AudioManager}
 * for a list of stream types. Must call this method before prepare() or
 * prepareAsync() in order for the target stream type to become effective
 * thereafter.
 *
 * @param streamtype the audio stream type
 * @see android.media.AudioManager
 */
void MediaPlayer::SetAudioStreamType(
    /* [in] */ Int32 streamtype)
{

}

/**
 * Sets the player to be looping or non-looping.
 *
 * @param looping whether to loop or not
 */
void MediaPlayer::SetLooping(
    /* [in] */ Boolean looping)
{

}

/**
 * Checks whether the MediaPlayer is looping or non-looping.
 *
 * @return TRUE if the MediaPlayer is currently looping, FALSE otherwise
 */
Boolean MediaPlayer::IsLooping()
{
    return FALSE;
}

/**
 * Sets the volume on this player.
 * This API is recommended for balancing the output of audio streams
 * within an application. Unless you are writing an application to
 * control user settings, this API should be used in preference to
 * {@link AudioManager#setStreamVolume(Int32, Int32, Int32)} which sets the volume of ALL streams of
 * a particular type. Note that the passed volume values are raw scalars.
 * UI controls should be scaled logarithmically.
 *
 * @param leftVolume left volume scalar
 * @param rightVolume right volume scalar
 */
void MediaPlayer::SetVolume(
    /* [in] */ Float leftVolume, 
    /* [in] */ Float rightVolume)
{
    
}

/**
 * Currently not implemented, returns NULL.
 * @deprecated
 * @hide
 */
AutoPtr<IBitmap> MediaPlayer::GetFrameAt(
    /* [in] */ Int32 msec)
{
    return NULL;
}

/**
 * Sets the audio session ID.
 *
 * @param sessionId the audio session ID.
 * The audio session ID is a system wide unique identifier for the audio stream played by
 * this MediaPlayer instance.
 * The primary use of the audio session ID  is to associate audio effects to a particular
 * instance of MediaPlayer: if an audio session ID is provided when creating an audio effect,
 * this effect will be applied only to the audio content of media players within the same
 * audio session and not to the output mix.
 * When created, a MediaPlayer instance automatically generates its own audio session ID.
 * However, it is possible to force this player to be part of an already existing audio session
 * by calling this method.
 * This method must be called before one of the overloaded <code> setDataSource </code> methods.
 * @throws IllegalStateException if it is called in an invalid state
 */
void MediaPlayer::SetAudioSessionId(
    /* [in] */ Int32 sessionId)
{

}

/**
 * Returns the audio session ID.
 *
 * @return the audio session ID. {@see #setAudioSessionId(Int32)}
 * Note that the audio session ID is 0 only if a problem occured when the MediaPlayer was contructed.
 */
Int32 MediaPlayer::GetAudioSessionId()
{
    return 0;
}

/**
 * Attaches an auxiliary effect to the player. A typical auxiliary effect is a reverberation
 * effect which can be applied on any sound source that directs a certain amount of its
 * energy to this effect. This amount is defined by setAuxEffectSendLevel().
 * {@see #setAuxEffectSendLevel(float)}.
 * <p>After creating an auxiliary effect (e.g.
 * {@link android.media.audiofx.EnvironmentalReverb}), retrieve its ID with
 * {@link android.media.audiofx.AudioEffect#getId()} and use it when calling this method
 * to attach the player to the effect.
 * <p>To detach the effect from the player, call this method with a NULL effect id.
 * <p>This method must be called after one of the overloaded <code> setDataSource </code>
 * methods.
 * @param effectId system wide unique id of the effect to attach
 */
void MediaPlayer::AttachAuxEffect(
    /* [in] */ Int32 effectId)
{

}

/**
 * Sets the send level of the player to the attached auxiliary effect
 * {@see #attachAuxEffect(Int32)}. The level value range is 0 to 1.0.
 * <p>By default the send level is 0, so even if an effect is attached to the player
 * this method must be called for the effect to be applied.
 * <p>Note that the passed level value is a raw scalar. UI controls should be scaled
 * logarithmically: the gain applied by audio framework ranges from -72dB to 0dB,
 * so an appropriate conversion from linear UI input x to level is:
 * x == 0 -> level = 0
 * 0 < x <= R -> level = 10^(72*(x-R)/20/R)
 * @param level send level scalar
 */
void MediaPlayer::SetAuxEffectSendLevel(
    /* [in] */ Float level)
{

}

/**
 * @param request Parcel destinated to the media player. The
 *                Interface token must be set to the IMediaPlayer
 *                one to be routed correctly through the system.
 * @param reply[out] Parcel that will contain the reply.
 * @return The status code.
 */
Int32 MediaPlayer::Native_invoke(
    /* [in] */ IParcel* request, 
    /* [in] */ IParcel* reply)
{
    return 0;
}


/**
 * @param update_only If TRUE fetch only the set of metadata that have
 *                    changed since the last invocation of getMetadata.
 *                    The set is built using the unfiltered
 *                    notifications the native player sent to the
 *                    MediaPlayerService during that period of
 *                    time. If FALSE, all the metadatas are considered.
 * @param apply_filter  If TRUE, once the metadata set has been built based on
 *                     the value update_only, the current filter is applied.
 * @param reply[out] On return contains the serialized
 *                   metadata. Valid only if the call was successful.
 * @return The status code.
 */
Boolean MediaPlayer::Native_getMetadata(
    /* [in] */ Boolean update_only,
    /* [in] */ Boolean apply_filter,
    /* [in] */ IParcel* reply)
{
    return FALSE;
}

/**
 * @param request Parcel with the 2 serialized lists of allowed
 *                metadata types followed by the one to be
 *                dropped. Each list starts with an integer
 *                indicating the number of metadata type elements.
 * @return The status code.
 */
Int32 MediaPlayer::Native_setMetadataFilter(
    /* [in] */ IParcel* request)
{
    return 0;
}

void MediaPlayer::Native_init()
{

}

void MediaPlayer::Native_setup(
    /* [in] */ IInterface* mediaplayer_this)
{

}

void MediaPlayer::Native_finalize()
{

}

void MediaPlayer::Finalize()
{ 
    Native_finalize(); 
}


//MediaPlayer::EventHandler::EventHandler(
//    /* [in] */ IMediaPlayer* mp, 
//    /* [in] */ Looper looper)
//{
//    super(looper);
//    mMediaPlayer = mp;
//}

//ECode MediaPlayer::EventHandler::HandleMessage(
//    /* [in] */ IMessage* msg)
//{
//    if (((CMediaPlayer*)mMediaPlayer)->mNativeContext == 0) {
//        //Log.w(TAG, "mediaplayer went away with unhandled events");
//        return NOERROR;
//    }
//    switch((CMessage*)msg->what) {
//    case MEDIA_PREPARED:
//        if (mOnPreparedListener != NULL)
//            mOnPreparedListener->OnPrepared(mMediaPlayer);
//        return NOERROR;
//
//    case MEDIA_PLAYBACK_COMPLETE:
//        if (mOnCompletionListener != NULL)
//            mOnCompletionListener->OnCompletion(mMediaPlayer);
//        StayAwake(FALSE);
//        return NOERROR;
//
//    case MEDIA_BUFFERING_UPDATE:
//        if (mOnBufferingUpdateListener != NULL)
//            mOnBufferingUpdateListener->OnBufferingUpdate(mMediaPlayer, ((CMessage*)msg)->arg1);
//        return NOERROR;
//
//    case MEDIA_SEEK_COMPLETE:
//      if (mOnSeekCompleteListener != NULL)
//          mOnSeekCompleteListener->OnSeekComplete(mMediaPlayer);
//      return NOERROR;
//
//    case MEDIA_SET_VIDEO_SIZE:
//      if (mOnVideoSizeChangedListener != NULL)
//          mOnVideoSizeChangedListener->OnVideoSizeChanged(mMediaPlayer, ((CMessage*)msg)->arg1, ((CMessage*)msg)->arg2);
//      return NOERROR;
//
//    case MEDIA_ERROR:
//        // For PV specific error values (msg.arg2) look in
//        // opencore/pvmi/pvmf/include/pvmf_return_codes.h
//        //Log.e(TAG, "Error (" + msg.arg1 + "," + msg.arg2 + ")");
//        Boolean error_was_handled = FALSE;
//        if (mOnErrorListener != NULL) {
//            error_was_handled = mOnErrorListener->OnError(mMediaPlayer, ((CMessage*)msg)->arg1, ((CMessage*)msg)->arg2);
//        }
//        if (mOnCompletionListener != NULL && ! error_was_handled) {
//            mOnCompletionListener->OnCompletion(mMediaPlayer);
//        }
//        StayAwake(FALSE);
//        return NOERROR;
//
//    case MEDIA_INFO:
//        // For PV specific code values (msg.arg2) look in
//        // opencore/pvmi/pvmf/include/pvmf_return_codes.h
//        //Log.i(TAG, "Info (" + msg.arg1 + "," + msg.arg2 + ")");
//        if (mOnInfoListener != NULL) {
//            mOnInfoListener->OnInfo(mMediaPlayer, ((CMessage*)msg)->arg1, ((CMessage*)msg)->arg2);
//        }
//        // No real default action so far.
//        return NOERROR;
//
//    case MEDIA_NOP: // interface test message - ignore
//        break;
//
//    default:
//        //Log.e(TAG, "Unknown message type " + msg.what);
//        return NOERROR;
//    }
//}

IInterface* MediaPlayer::EventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IObject*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    return NULL;
}

UInt32 MediaPlayer::EventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MediaPlayer::EventHandler::Release()
{
    return ElRefBase::Release();
}

ECode MediaPlayer::EventHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}


/**
 * Called from native code when an interesting event happens.  This method
 * just uses the EventHandler system to post the event back to the main app thread.
 * We use a weak reference to the original MediaPlayer object so that the native
 * code is safe from the object disappearing from underneath it.  (This is
 * the cookie passed to native_setup().)
 */
void MediaPlayer::PostEventFromNative(
    /* [in] */ IInterface* mediaplayer_ref,
    /* [in] */ Int32 what, 
    /* [in] */ Int32 arg1, 
    /* [in] */ Int32 arg2, 
    /* [in] */ IInterface* obj)
{
    /*MediaPlayer mp = (MediaPlayer)((WeakReference)mediaplayer_ref).get();
    if (mp == NULL) {
        return;
    }

    if (mp.mEventHandler != NULL) {
        Message m = mp.mEventHandler.obtainMessage(what, arg1, arg2, obj);
        mp.mEventHandler.sendMessage(m);
    }*/
}

/**
 * Register a callback to be invoked when the media source is ready
 * for playback.
 *
 * @param listener the callback that will be run
 */
ECode MediaPlayer::SetOnPreparedListener(
    /* [in] */ IOnPreparedListener* listener)
{
    mOnPreparedListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when the end of a media source
 * has been reached during playback.
 *
 * @param listener the callback that will be run
 */
ECode MediaPlayer::SetOnCompletionListener(
    /* [in] */ IOnCompletionListener* listener)
{
    mOnCompletionListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when the status of a network
 * stream's buffer has changed.
 *
 * @param listener the callback that will be run.
 */
ECode MediaPlayer::SetOnBufferingUpdateListener(
    /* [in] */ IOnBufferingUpdateListener* listener)
{
    mOnBufferingUpdateListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when a seek operation has been
 * completed.
 *
 * @param listener the callback that will be run
 */
ECode MediaPlayer::SetOnSeekCompleteListener(
    /* [in] */ IOnSeekCompleteListener* listener)
{
    mOnSeekCompleteListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when the video size is
 * known or updated.
 *
 * @param listener the callback that will be run
 */
ECode MediaPlayer::SetOnVideoSizeChangedListener(
    /* [in] */ IOnVideoSizeChangedListener* listener)
{
    mOnVideoSizeChangedListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when an error has happened
 * during an asynchronous operation.
 *
 * @param listener the callback that will be run
 */
ECode MediaPlayer::SetOnErrorListener(
    /* [in] */ IOnErrorListener* listener)
{
    mOnErrorListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when an info/warning is available.
 *
 * @param listener the callback that will be run
 */
ECode MediaPlayer::SetOnInfoListener(
    /* [in] */ IOnInfoListener* listener)
{
    mOnInfoListener = listener;

    return NOERROR;
}

