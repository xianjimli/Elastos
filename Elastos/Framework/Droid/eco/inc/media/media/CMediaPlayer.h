
#ifndef __CMEDIAPLAYER_H__
#define __CMEDIAPLAYER_H__

#include "_CMediaPlayer.h"
#include "media/MediaPlayer.h"

CarClass(CMediaPlayer), public MediaPlayer
{
public:
    CARAPI NewRequest(
        /* [out] */ IParcel ** ppParcel);

    CARAPI Invoke(
        /* [in] */ IParcel * pRequest,
        /* [in] */ IParcel * pReply,
        /* [out] */ Int32 * pStatus);

    CARAPI SetDisplay(
        /* [in] */ ISurfaceHolder * pSh);

    CARAPI Create(
        /* [in] */ IContext * pContext,
        /* [in] */ IUri * pUri,
        /* [out] */ IMediaPlayer ** ppPlayer);

    CARAPI CreateEx(
        /* [in] */ IContext * pContext,
        /* [in] */ IUri * pUri,
        /* [in] */ ISurfaceHolder * pHolder,
        /* [out] */ IMediaPlayer ** ppPlayer);

    CARAPI CreateEx2(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 resid,
        /* [out] */ IMediaPlayer ** ppPlayer);

    /**
     * Sets the data source as a content Uri.
     *
     * @param context the Context to use when resolving the Uri
     * @param uri the Content URI of the data you want to play
     * @throws IllegalStateException if it is called in an invalid state
     */
    CARAPI SetDataSource(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri);

    /**
     * Sets the data source as a content Uri.
     *
     * @param context the Context to use when resolving the Uri
     * @param uri the Content URI of the data you want to play
     * @param headers the headers to be sent together with the request for the data
     * @throws IllegalStateException if it is called in an invalid state
     * @hide pending API council
     */
    CARAPI SetDataSourceEx(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [in] */ IObjectStringMap* headers);

    /**
     * Sets the data source (file-path or http/rtsp URL) to use.
     *
     * @param path the path of the file, or the http/rtsp URL of the stream you want to play
     * @throws IllegalStateException if it is called in an invalid state
     */
    CARAPI SetDataSourceEx2(
        /* [in] */ const String& path);

    /**
     * Sets the data source (file-path or http/rtsp URL) to use.
     *
     * @param path the path of the file, or the http/rtsp URL of the stream you want to play
     * @param headers the headers associated with the http request for the stream you want to play
     * @throws IllegalStateException if it is called in an invalid state
     * @hide pending API council
     */
    CARAPI SetDataSourceEx3(
        /* [in] */ const String& path,
        /* [in] */ IObjectStringMap* headers);

    /**
     * Sets the data source (FileDescriptor) to use. It is the caller's responsibility
     * to close the file descriptor. It is safe to do so as soon as this call returns.
     *
     * @param fd the FileDescriptor for the file you want to play
     * @throws IllegalStateException if it is called in an invalid state
     */
    CARAPI SetDataSourceEx4(
        /* [in] */ IFileDescriptor* fd);

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
    CARAPI SetDataSourceEx5(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 length);

    /**
     * Prepares the player for playback, synchronously.
     *
     * After setting the datasource and the display surface, you need to either
     * call prepare() or prepareAsync(). For files, it is OK to call prepare(),
     * which blocks until MediaPlayer is ready for playback.
     *
     * @throws IllegalStateException if it is called in an invalid state
     */
    CARAPI Prepare();

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
    CARAPI PrepareAsync();

    CARAPI Start();

    CARAPI Stop();

    CARAPI Pause();

    CARAPI SetWakeMode(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 mode);

    CARAPI SetScreenOnWhilePlaying(
        /* [in] */ Boolean screenOn);

    /**
     * Returns the width of the video.
     *
     * @return the width of the video, or 0 if there is no video,
     * no display surface was set, or the width has not been determined
     * yet. The OnVideoSizeChangedListener can be registered via
     * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}
     * to provide a notification when the width is available.
     */
    CARAPI GetVideoWidth(
        /* [out] */ Int32* width);

    /**
     * Returns the height of the video.
     *
     * @return the height of the video, or 0 if there is no video,
     * no display surface was set, or the height has not been determined
     * yet. The OnVideoSizeChangedListener can be registered via
     * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}
     * to provide a notification when the height is available.
     */
    CARAPI GetVideoHeight(
        /* [out] */ Int32* height);

    /**
     * Checks whether the MediaPlayer is playing.
     *
     * @return true if currently playing, false otherwise
     */
    CARAPI IsPlaying(
        /* [out] */ Boolean* isPlaying);

    /**
     * Seeks to specified time position.
     *
     * @param msec the offset in milliseconds from the start to seek to
     * @throws IllegalStateException if the internal player engine has not been
     * initialized
     */
    CARAPI SeekTo(
        /* [in] */ Int32 msec);

    /**
     * Gets the current playback position.
     *
     * @return the current position in milliseconds
     */
    CARAPI GetCurrentPosition(
        /* [out] */ Int32* position);

    /**
     * Gets the duration of the file.
     *
     * @return the duration in milliseconds
     */
    CARAPI GetDuration(
        /* [out] */ Int32* duration);

    CARAPI GetMetadata(
        /* [in] */ Boolean update_only,
        /* [in] */ Boolean apply_filter,
        /* [out] */ IMetadata ** ppMetadata);

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
    CARAPI SetMetadataFilter(
        /* [in] */ IObjectContainer* allow,
        /* [in] */ IObjectContainer* block,
        /* [out] */ Int32* result);

    CARAPI ReleaseResources();

    CARAPI Reset();

    CARAPI Suspend(
        /* [out] */ Boolean * pRes);

    CARAPI Resume(
        /* [out] */ Boolean * pRes);

    /**
     * Sets the audio stream type for this MediaPlayer. See {@link AudioManager}
     * for a list of stream types. Must call this method before prepare() or
     * prepareAsync() in order for the target stream type to become effective
     * thereafter.
     *
     * @param streamtype the audio stream type
     * @see android.media.AudioManager
     */
    CARAPI SetAudioStreamType(
        /* [in] */ Int32 streamtype);

    /**
     * Sets the player to be looping or non-looping.
     *
     * @param looping whether to loop or not
     */
    CARAPI SetLooping(
        /* [in] */ Boolean looping);

    /**
     * Checks whether the MediaPlayer is looping or non-looping.
     *
     * @return true if the MediaPlayer is currently looping, false otherwise
     */
    CARAPI IsLooping(
        /* [out] */ Boolean* isLooping);

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
    CARAPI SetVolume(
        /* [in] */ Float leftVolume,
        /* [in] */ Float rightVolume);

    /**
     * Currently not implemented, returns null.
     * @deprecated
     * @hide
     */
    CARAPI GetFrameAt(
        /* [in] */ Int32 msec,
        /* [out] */ IBitmap** frame);

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
    CARAPI SetAudioSessionId(
        /* [in] */ Int32 sessionId);

    /**
     * Returns the audio session ID.
     *
     * @return the audio session ID. {@see #setAudioSessionId(int)}
     * Note that the audio session ID is 0 only if a problem occured when the MediaPlayer was contructed.
     */
    CARAPI GetAudioSessionId(
        /* [out] */ Int32* sessionID);

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
    CARAPI AttachAuxEffect(
        /* [in] */ Int32 effectId);

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
    CARAPI SetAuxEffectSendLevel(
        /* [in] */ Float level);

    CARAPI SetOnPreparedListener(
        /* [in] */ IMediaPlayerOnPreparedListener * pListener);

    CARAPI SetOnCompletionListener(
        /* [in] */ IMediaPlayerOnCompletionListener * pListener);

    CARAPI SetOnBufferingUpdateListener(
        /* [in] */ IMediaPlayerOnBufferingUpdateListener * pListener);

    CARAPI SetOnSeekCompleteListener(
        /* [in] */ IMediaPlayerOnSeekCompleteListener * pListener);

    CARAPI SetOnVideoSizeChangedListener(
        /* [in] */ IMediaPlayerOnVideoSizeChangedListener * pListener);

    CARAPI SetOnErrorListener(
        /* [in] */ IMediaPlayerOnErrorListener * pListener);

    CARAPI SetOnInfoListener(
        /* [in] */ IMediaPlayerOnInfoListener * pListener);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CMEDIAPLAYER_H__
