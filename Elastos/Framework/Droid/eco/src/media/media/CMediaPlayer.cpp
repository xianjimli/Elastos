
#include "media/CMediaPlayer.h"

ECode CMediaPlayer::NewRequest(
    /* [out] */ IParcel ** ppParcel)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Invoke(
    /* [in] */ IParcel * pRequest,
    /* [in] */ IParcel * pReply,
    /* [out] */ Int32 * pStatus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetDisplay(
    /* [in] */ ISurfaceHolder * pSh)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Create(
    /* [in] */ IContext * pContext,
    /* [in] */ IUri * pUri,
    /* [out] */ IMediaPlayer ** ppPlayer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::CreateEx(
    /* [in] */ IContext * pContext,
    /* [in] */ IUri * pUri,
    /* [in] */ ISurfaceHolder * pHolder,
    /* [out] */ IMediaPlayer ** ppPlayer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::CreateEx2(
    /* [in] */ IContext * pContext,
    /* [in] */ Int32 resid,
    /* [out] */ IMediaPlayer ** ppPlayer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the data source as a content Uri.
 *
 * @param context the Context to use when resolving the Uri
 * @param uri the Content URI of the data you want to play
 * @throws IllegalStateException if it is called in an invalid state
 */
ECode CMediaPlayer::SetDataSource(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::SetDataSourceEx(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ IObjectStringMap* headers)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the data source (file-path or http/rtsp URL) to use.
 *
 * @param path the path of the file, or the http/rtsp URL of the stream you want to play
 * @throws IllegalStateException if it is called in an invalid state
 */
ECode CMediaPlayer::SetDataSourceEx2(
    /* [in] */ const String& path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the data source (file-path or http/rtsp URL) to use.
 *
 * @param path the path of the file, or the http/rtsp URL of the stream you want to play
 * @param headers the headers associated with the http request for the stream you want to play
 * @throws IllegalStateException if it is called in an invalid state
 * @hide pending API council
 */
ECode CMediaPlayer::SetDataSourceEx3(
    /* [in] */ const String& path,
    /* [in] */ IObjectStringMap* headers)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the data source (FileDescriptor) to use. It is the caller's responsibility
 * to close the file descriptor. It is safe to do so as soon as this call returns.
 *
 * @param fd the FileDescriptor for the file you want to play
 * @throws IllegalStateException if it is called in an invalid state
 */
ECode CMediaPlayer::SetDataSourceEx4(
    /* [in] */ IFileDescriptor* fd)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::SetDataSourceEx5(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::Prepare()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::PrepareAsync()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Start()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Stop()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Pause()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetWakeMode(
    /* [in] */ IContext * pContext,
    /* [in] */ Int32 mode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetScreenOnWhilePlaying(
    /* [in] */ Boolean screenOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::GetVideoWidth(
    /* [out] */ Int32* width)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::GetVideoHeight(
    /* [out] */ Int32* height)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Checks whether the MediaPlayer is playing.
 *
 * @return true if currently playing, false otherwise
 */
ECode CMediaPlayer::IsPlaying(
    /* [out] */ Boolean* isPlaying)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Seeks to specified time position.
 *
 * @param msec the offset in milliseconds from the start to seek to
 * @throws IllegalStateException if the internal player engine has not been
 * initialized
 */
ECode CMediaPlayer::SeekTo(
    /* [in] */ Int32 msec)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Gets the current playback position.
 *
 * @return the current position in milliseconds
 */
ECode CMediaPlayer::GetCurrentPosition(
    /* [out] */ Int32* position)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::GetDuration(
    /* [out] */ Int32* duration)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::GetMetadata(
    /* [in] */ Boolean update_only,
    /* [in] */ Boolean apply_filter,
    /* [out] */ IMetadata ** ppMetadata)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetMetadataFilter(
    /* [in] */ IObjectContainer* allow,
    /* [in] */ IObjectContainer* block,
    /* [out] */ Int32* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::ReleaseResources()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Reset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Suspend(
    /* [out] */ Boolean * pRes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Resume(
    /* [out] */ Boolean * pRes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::SetAudioStreamType(
    /* [in] */ Int32 streamtype)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the player to be looping or non-looping.
 *
 * @param looping whether to loop or not
 */
ECode CMediaPlayer::SetLooping(
    /* [in] */ Boolean looping)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Checks whether the MediaPlayer is looping or non-looping.
 *
 * @return true if the MediaPlayer is currently looping, false otherwise
 */
ECode CMediaPlayer::IsLooping(
    /* [out] */ Boolean* isLooping)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the volume on this player.
 * This API is recommended for balancing the output of audio streams
 * within an application. Unless you are writing an application to
 * control user settings, this API should be used in preference to
 * {@link AudioManager#setStreamVolume(int, int, int)} which sets the volume of ALL streams of
 * a particular type. Note that the passed volume values are raw scalars.
 * UI controls should be scaled logarithmically.
 *
 * @param leftVolume left volume scalar
 * @param rightVolume right volume scalar
 */
ECode CMediaPlayer::SetVolume(
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Currently not implemented, returns null.
 * @deprecated
 * @hide
 */
ECode CMediaPlayer::GetFrameAt(
    /* [in] */ Int32 msec,
    /* [out] */ IBitmap** frame)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
ECode CMediaPlayer::SetAudioSessionId(
    /* [in] */ Int32 sessionId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the audio session ID.
 *
 * @return the audio session ID. {@see #setAudioSessionId(int)}
 * Note that the audio session ID is 0 only if a problem occured when the MediaPlayer was contructed.
 */
ECode CMediaPlayer::GetAudioSessionId(
    /* [out] */ Int32* sessionID)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
 * <p>To detach the effect from the player, call this method with a null effect id.
 * <p>This method must be called after one of the overloaded <code> setDataSource </code>
 * methods.
 * @param effectId system wide unique id of the effect to attach
 */
ECode CMediaPlayer::AttachAuxEffect(
    /* [in] */ Int32 effectId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the send level of the player to the attached auxiliary effect
 * {@see #attachAuxEffect(int)}. The level value range is 0 to 1.0.
 * <p>By default the send level is 0, so even if an effect is attached to the player
 * this method must be called for the effect to be applied.
 * <p>Note that the passed level value is a raw scalar. UI controls should be scaled
 * logarithmically: the gain applied by audio framework ranges from -72dB to 0dB,
 * so an appropriate conversion from linear UI input x to level is:
 * x == 0 -> level = 0
 * 0 < x <= R -> level = 10^(72*(x-R)/20/R)
 * @param level send level scalar
 */
ECode CMediaPlayer::SetAuxEffectSendLevel(
    /* [in] */ Float level)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetOnPreparedListener(
    /* [in] */ IMediaPlayerOnPreparedListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetOnCompletionListener(
    /* [in] */ IMediaPlayerOnCompletionListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetOnBufferingUpdateListener(
    /* [in] */ IMediaPlayerOnBufferingUpdateListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetOnSeekCompleteListener(
    /* [in] */ IMediaPlayerOnSeekCompleteListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetOnVideoSizeChangedListener(
    /* [in] */ IMediaPlayerOnVideoSizeChangedListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetOnErrorListener(
    /* [in] */ IMediaPlayerOnErrorListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetOnInfoListener(
    /* [in] */ IMediaPlayerOnInfoListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

