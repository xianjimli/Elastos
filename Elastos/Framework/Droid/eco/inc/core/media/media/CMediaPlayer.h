
#ifndef __CMEDIAPLAYER_H__
#define __CMEDIAPLAYER_H__

#include "_CMediaPlayer.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * MediaPlayer class can be used to control playback
 * of audio/video files and streams. An example on how to use the methods in
 * this class can be found in {@link android.widget.VideoView}.
 * Please see <a href="{@docRoot}guide/topics/media/index.html">Audio and Video</a>
 * for additional help using MediaPlayer.
 *
 * <p>Topics covered here are:
 * <ol>
 * <li><a href="#StateDiagram">State Diagram</a>
 * <li><a href="#Valid_and_Invalid_States">Valid and Invalid States</a>
 * <li><a href="#Permissions">Permissions</a>
 * </ol>
 *
 * <a name="StateDiagram"></a>
 * <h3>State Diagram</h3>
 *
 * <p>Playback control of audio/video files and streams is managed as a state
 * machine. The following diagram shows the life cycle and the states of a
 * MediaPlayer object driven by the supported playback control operations.
 * The ovals represent the states a MediaPlayer object may reside
 * in. The arcs represent the playback control operations that drive the object
 * state transition. There are two types of arcs. The arcs with a single arrow
 * head represent synchronous method calls, while those with
 * a double arrow head represent asynchronous method calls.</p>
 *
 * <p><img src="../../../images/mediaplayer_state_diagram.gif"
 *         alt="MediaPlayer State diagram"
 *         border="0" /></p>
 *
 * <p>From this state diagram, one can see that a MediaPlayer object has the
 *    following states:</p>
 * <ul>
 *     <li>When a MediaPlayer object is just created using <code>new</code> or
 *         after {@link #reset()} is called, it is in the <em>Idle</em> state; and after
 *         {@link #release()} is called, it is in the <em>End</em> state. Between these
 *         two states is the life cycle of the MediaPlayer object.
 *         <ul>
 *         <li>There is a subtle but important difference between a newly constructed
 *         MediaPlayer object and the MediaPlayer object after {@link #reset()}
 *         is called. It is a programming error to invoke methods such
 *         as {@link #getCurrentPosition()},
 *         {@link #getDuration()}, {@link #getVideoHeight()},
 *         {@link #getVideoWidth()}, {@link #setAudioStreamType(int)},
 *         {@link #setLooping(boolean)},
 *         {@link #setVolume(float, float)}, {@link #pause()}, {@link #start()},
 *         {@link #stop()}, {@link #seekTo(int)}, {@link #prepare()} or
 *         {@link #prepareAsync()} in the <em>Idle</em> state for both cases. If any of these
 *         methods is called right after a MediaPlayer object is constructed,
 *         the user supplied callback method OnErrorListener.onError() won't be
 *         called by the internal player engine and the object state remains
 *         unchanged; but if these methods are called right after {@link #reset()},
 *         the user supplied callback method OnErrorListener.onError() will be
 *         invoked by the internal player engine and the object will be
 *         transfered to the <em>Error</em> state. </li>
 *         <li>It is also recommended that once
 *         a MediaPlayer object is no longer being used, call {@link #release()} immediately
 *         so that resources used by the internal player engine associated with the
 *         MediaPlayer object can be released immediately. Resource may include
 *         singleton resources such as hardware acceleration components and
 *         failure to call {@link #release()} may cause subsequent instances of
 *         MediaPlayer objects to fallback to software implementations or fail
 *         altogether. Once the MediaPlayer
 *         object is in the <em>End</em> state, it can no longer be used and
 *         there is no way to bring it back to any other state. </li>
 *         <li>Furthermore,
 *         the MediaPlayer objects created using <code>new</code> is in the
 *         <em>Idle</em> state, while those created with one
 *         of the overloaded convenient <code>create</code> methods are <em>NOT</em>
 *         in the <em>Idle</em> state. In fact, the objects are in the <em>Prepared</em>
 *         state if the creation using <code>create</code> method is successful.
 *         </li>
 *         </ul>
 *         </li>
 *     <li>In general, some playback control operation may fail due to various
 *         reasons, such as unsupported audio/video format, poorly interleaved
 *         audio/video, resolution too high, streaming timeout, and the like.
 *         Thus, error reporting and recovery is an important concern under
 *         these circumstances. Sometimes, due to programming errors, invoking a playback
 *         control operation in an invalid state may also occur. Under all these
 *         error conditions, the internal player engine invokes a user supplied
 *         OnErrorListener.onError() method if an OnErrorListener has been
 *         registered beforehand via
 *         {@link #setOnErrorListener(android.media.MediaPlayer.OnErrorListener)}.
 *         <ul>
 *         <li>It is important to note that once an error occurs, the
 *         MediaPlayer object enters the <em>Error</em> state (except as noted
 *         above), even if an error listener has not been registered by the application.</li>
 *         <li>In order to reuse a MediaPlayer object that is in the <em>
 *         Error</em> state and recover from the error,
 *         {@link #reset()} can be called to restore the object to its <em>Idle</em>
 *         state.</li>
 *         <li>It is good programming practice to have your application
 *         register a OnErrorListener to look out for error notifications from
 *         the internal player engine.</li>
 *         <li>IllegalStateException is
 *         thrown to prevent programming errors such as calling {@link #prepare()},
 *         {@link #prepareAsync()}, or one of the overloaded <code>setDataSource
 *         </code> methods in an invalid state. </li>
 *         </ul>
 *         </li>
 *     <li>Calling
 *         {@link #setDataSource(FileDescriptor)}, or
 *         {@link #setDataSource(String)}, or
 *         {@link #setDataSource(Context, Uri)}, or
 *         {@link #setDataSource(FileDescriptor, Int64, Int64)} transfers a
 *         MediaPlayer object in the <em>Idle</em> state to the
 *         <em>Initialized</em> state.
 *         <ul>
 *         <li>An IllegalStateException is thrown if
 *         setDataSource() is called in any other state.</li>
 *         <li>It is good programming
 *         practice to always look out for <code>IllegalArgumentException</code>
 *         and <code>IOException</code> that may be thrown from the overloaded
 *         <code>setDataSource</code> methods.</li>
 *         </ul>
 *         </li>
 *     <li>A MediaPlayer object must first enter the <em>Prepared</em> state
 *         before playback can be started.
 *         <ul>
 *         <li>There are two ways (synchronous vs.
 *         asynchronous) that the <em>Prepared</em> state can be reached:
 *         either a call to {@link #prepare()} (synchronous) which
 *         transfers the object to the <em>Prepared</em> state once the method call
 *         returns, or a call to {@link #prepareAsync()} (asynchronous) which
 *         first transfers the object to the <em>Preparing</em> state after the
 *         call returns (which occurs almost right way) while the internal
 *         player engine continues working on the rest of preparation work
 *         until the preparation work completes. When the preparation completes or when {@link #prepare()} call returns,
 *         the internal player engine then calls a user supplied callback method,
 *         onPrepared() of the OnPreparedListener interface, if an
 *         OnPreparedListener is registered beforehand via {@link
 *         #setOnPreparedListener(android.media.MediaPlayer.OnPreparedListener)}.</li>
 *         <li>It is important to note that
 *         the <em>Preparing</em> state is a transient state, and the behavior
 *         of calling any method with side effect while a MediaPlayer object is
 *         in the <em>Preparing</em> state is undefined.</li>
 *         <li>An IllegalStateException is
 *         thrown if {@link #prepare()} or {@link #prepareAsync()} is called in
 *         any other state.</li>
 *         <li>While in the <em>Prepared</em> state, properties
 *         such as audio/sound volume, screenOnWhilePlaying, looping can be
 *         adjusted by invoking the corresponding set methods.</li>
 *         </ul>
 *         </li>
 *     <li>To start the playback, {@link #start()} must be called. After
 *         {@link #start()} returns successfully, the MediaPlayer object is in the
 *         <em>Started</em> state. {@link #isPlaying()} can be called to test
 *         whether the MediaPlayer object is in the <em>Started</em> state.
 *         <ul>
 *         <li>While in the <em>Started</em> state, the internal player engine calls
 *         a user supplied OnBufferingUpdateListener.onBufferingUpdate() callback
 *         method if a OnBufferingUpdateListener has been registered beforehand
 *         via {@link #setOnBufferingUpdateListener(OnBufferingUpdateListener)}.
 *         This callback allows applications to keep track of the buffering status
 *         while streaming audio/video.</li>
 *         <li>Calling {@link #start()} has not effect
 *         on a MediaPlayer object that is already in the <em>Started</em> state.</li>
 *         </ul>
 *         </li>
 *     <li>Playback can be paused and stopped, and the current playback position
 *         can be adjusted. Playback can be paused via {@link #pause()}. When the call to
 *         {@link #pause()} returns, the MediaPlayer object enters the
 *         <em>Paused</em> state. Note that the transition from the <em>Started</em>
 *         state to the <em>Paused</em> state and vice versa happens
 *         asynchronously in the player engine. It may take some time before
 *         the state is updated in calls to {@link #isPlaying()}, and it can be
 *         a number of seconds in the case of streamed content.
 *         <ul>
 *         <li>Calling {@link #start()} to resume playback for a paused
 *         MediaPlayer object, and the resumed playback
 *         position is the same as where it was paused. When the call to
 *         {@link #start()} returns, the paused MediaPlayer object goes back to
 *         the <em>Started</em> state.</li>
 *         <li>Calling {@link #pause()} has no effect on
 *         a MediaPlayer object that is already in the <em>Paused</em> state.</li>
 *         </ul>
 *         </li>
 *     <li>Calling  {@link #stop()} stops playback and causes a
 *         MediaPlayer in the <em>Started</em>, <em>Paused</em>, <em>Prepared
 *         </em> or <em>PlaybackCompleted</em> state to enter the
 *         <em>Stopped</em> state.
 *         <ul>
 *         <li>Once in the <em>Stopped</em> state, playback cannot be started
 *         until {@link #prepare()} or {@link #prepareAsync()} are called to set
 *         the MediaPlayer object to the <em>Prepared</em> state again.</li>
 *         <li>Calling {@link #stop()} has no effect on a MediaPlayer
 *         object that is already in the <em>Stopped</em> state.</li>
 *         </ul>
 *         </li>
 *     <li>The playback position can be adjusted with a call to
 *         {@link #seekTo(int)}.
 *         <ul>
 *         <li>Although the asynchronuous {@link #seekTo(int)}
 *         call returns right way, the actual seek operation may take a while to
 *         finish, especially for audio/video being streamed. When the actual
 *         seek operation completes, the internal player engine calls a user
 *         supplied OnSeekComplete.onSeekComplete() if an OnSeekCompleteListener
 *         has been registered beforehand via
 *         {@link #setOnSeekCompleteListener(OnSeekCompleteListener)}.</li>
 *         <li>Please
 *         note that {@link #seekTo(int)} can also be called in the other states,
 *         such as <em>Prepared</em>, <em>Paused</em> and <em>PlaybackCompleted
 *         </em> state.</li>
 *         <li>Furthermore, the actual current playback position
 *         can be retrieved with a call to {@link #getCurrentPosition()}, which
 *         is helpful for applications such as a Music player that need to keep
 *         track of the playback progress.</li>
 *         </ul>
 *         </li>
 *     <li>When the playback reaches the end of stream, the playback completes.
 *         <ul>
 *         <li>If the looping mode was being set to <var>true</var>with
 *         {@link #setLooping(boolean)}, the MediaPlayer object shall remain in
 *         the <em>Started</em> state.</li>
 *         <li>If the looping mode was set to <var>false
 *         </var>, the player engine calls a user supplied callback method,
 *         OnCompletion.onCompletion(), if a OnCompletionListener is registered
 *         beforehand via {@link #setOnCompletionListener(OnCompletionListener)}.
 *         The invoke of the callback signals that the object is now in the <em>
 *         PlaybackCompleted</em> state.</li>
 *         <li>While in the <em>PlaybackCompleted</em>
 *         state, calling {@link #start()} can restart the playback from the
 *         beginning of the audio/video source.</li>
 * </ul>
 *
 *
 * <a name="Valid_and_Invalid_States"></a>
 * <h3>Valid and invalid states</h3>
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr><td>Method Name </p></td>
 *     <td>Valid Sates </p></td>
 *     <td>Invalid States </p></td>
 *     <td>Comments </p></td></tr>
 * <tr><td>attachAuxEffect </p></td>
 *     <td>{Initialized, Prepared, Started, Paused, Stopped, PlaybackCompleted} </p></td>
 *     <td>{Idle, Error} </p></td>
 *     <td>This method must be called after setDataSource.
 *     Calling it does not change the object state. </p></td></tr>
 * <tr><td>getAudioSessionId </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>getCurrentPosition </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted} </p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change the
 *         state. Calling this method in an invalid state transfers the object
 *         to the <em>Error</em> state. </p></td></tr>
 * <tr><td>getDuration </p></td>
 *     <td>{Prepared, Started, Paused, Stopped, PlaybackCompleted} </p></td>
 *     <td>{Idle, Initialized, Error} </p></td>
 *     <td>Successful invoke of this method in a valid state does not change the
 *         state. Calling this method in an invalid state transfers the object
 *         to the <em>Error</em> state. </p></td></tr>
 * <tr><td>getVideoHeight </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change the
 *         state. Calling this method in an invalid state transfers the object
 *         to the <em>Error</em> state.  </p></td></tr>
 * <tr><td>getVideoWidth </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an invalid state transfers the
 *         object to the <em>Error</em> state. </p></td></tr>
 * <tr><td>isPlaying </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *          PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an invalid state transfers the
 *         object to the <em>Error</em> state. </p></td></tr>
 * <tr><td>pause </p></td>
 *     <td>{Started, Paused}</p></td>
 *     <td>{Idle, Initialized, Prepared, Stopped, PlaybackCompleted, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Paused</em> state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 * <tr><td>prepare </p></td>
 *     <td>{Initialized, Stopped} </p></td>
 *     <td>{Idle, Prepared, Started, Paused, PlaybackCompleted, Error} </p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Prepared</em> state. Calling this method in an
 *         invalid state throws an IllegalStateException.</p></td></tr>
 * <tr><td>prepareAsync </p></td>
 *     <td>{Initialized, Stopped} </p></td>
 *     <td>{Idle, Prepared, Started, Paused, PlaybackCompleted, Error} </p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Preparing</em> state. Calling this method in an
 *         invalid state throws an IllegalStateException.</p></td></tr>
 * <tr><td>release </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>After {@link #release()}, the object is no longer available. </p></td></tr>
 * <tr><td>reset </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted, Error}</p></td>
 *     <td>{}</p></td>
 *     <td>After {@link #reset()}, the object is like being just created.</p></td></tr>
 * <tr><td>seekTo </p></td>
 *     <td>{Prepared, Started, Paused, PlaybackCompleted} </p></td>
 *     <td>{Idle, Initialized, Stopped, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an invalid state transfers the
 *         object to the <em>Error</em> state. </p></td></tr>
 * <tr><td>setAudioSessionId </p></td>
 *     <td>{Idle} </p></td>
 *     <td>{Initialized, Prepared, Started, Paused, Stopped, PlaybackCompleted,
 *          Error} </p></td>
 *     <td>This method must be called in idle state as the audio session ID must be known before
 *         calling setDataSource. Calling it does not change the object state. </p></td></tr>
 * <tr><td>setAudioStreamType </p></td>
 *     <td>{Idle, Initialized, Stopped, Prepared, Started, Paused,
 *          PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method does not change the state. In order for the
 *         target audio stream type to become effective, this method must be called before
 *         prepare() or prepareAsync().</p></td></tr>
 * <tr><td>setAuxEffectSendLevel </p></td>
 *     <td>any</p></td>
 *     <td>{} </p></td>
 *     <td>Calling this method does not change the object state. </p></td></tr>
 * <tr><td>setDataSource </p></td>
 *     <td>{Idle} </p></td>
 *     <td>{Initialized, Prepared, Started, Paused, Stopped, PlaybackCompleted,
 *          Error} </p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Initialized</em> state. Calling this method in an
 *         invalid state throws an IllegalStateException.</p></td></tr>
 * <tr><td>setDisplay </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setLooping </p></td>
 *     <td>{Idle, Initialized, Stopped, Prepared, Started, Paused,
 *         PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 * <tr><td>isLooping </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnBufferingUpdateListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnCompletionListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnErrorListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnPreparedListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnSeekCompleteListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setScreenOnWhilePlaying</></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state.  </p></td></tr>
 * <tr><td>setVolume </p></td>
 *     <td>{Idle, Initialized, Stopped, Prepared, Started, Paused,
 *          PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method does not change the state.
 * <tr><td>setWakeMode </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state.</p></td></tr>
 * <tr><td>start </p></td>
 *     <td>{Prepared, Started, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Stopped, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Started</em> state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 * <tr><td>stop </p></td>
 *     <td>{Prepared, Started, Stopped, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Stopped</em> state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 *
 * </table>
 *
 * <a name="Permissions"></a>
 * <h3>Permissions</h3>
 * <p>One may need to declare a corresponding WAKE_LOCK permission {@link
 * android.R.styleable#AndroidManifestUsesPermission &lt;uses-permission&gt;}
 * element.
 *
 */
CarClass(CMediaPlayer)
{
private:
    class EventHandler
        : public ElRefBase
        , public IApartment
    {
    public:
        //EventHandler(
        //    /* [in] */ IMediaPlayer* mp,
        //    /* [in] */ ILooper* looper);

        //virtual CARAPI HandleMessage(
        //    /* [in] */ IMessage* msg);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        IMediaPlayer* mMediaPlayer;
    };

public:
    /**
     * Default constructor. Consider using one of the create() methods for
     * synchronously instantiating a MediaPlayer from a Uri or resource.
     * <p>When done with the MediaPlayer, you should call  {@link #release()},
     * to free the resources. If not released, too many MediaPlayer instances may
     * result in an exception.</p>
     */
    CMediaPlayer();

    ~CMediaPlayer();

    /**
     * Create a request parcel which can be routed to the  native  media
     * player using {@link #invoke(Parcel, Parcel)}. The Parcel
     * returned has the proper InterfaceToken set. The caller should
     * not overwrite that token, i.e it can only append data to the
     * Parcel.
     *
     * @return A parcel suitable to hold a request for the  native
     * player.
     * {@hide}
     */
    CARAPI NewRequest(
        /* [out] */ IParcel** parcel);

    /**
     * Invoke a generic method on the  native  player using opaque
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
    CARAPI Invoke(
        /* [in] */ IParcel* request,
        /* [in] */ IParcel* reply,
        /* [out] */ Int32* status);

    /**
     * Sets the SurfaceHolder to use for displaying the video portion of the media.
     * This call is optional. Not calling it when playing back a video will
     * result in only the audio track being played.
     *
     * @param sh the SurfaceHolder to use for video display
     */
    CARAPI SetDisplay(
        /* [in] */ ISurfaceHolder* sh);

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
    static CARAPI Create(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [out] */ IMediaPlayer** player);

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
    static CARAPI Create(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [in] */ ISurfaceHolder* holder,
        /* [out] */ IMediaPlayer ** player);

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
    static CARAPI Create(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resid,
        /* [out] */ IMediaPlayer** player);

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

    /**
     * Starts or resumes playback. If playback had previously been paused,
     * playback will continue from where it was paused. If playback had
     * been stopped, or never started before, playback will start at the
     * beginning.
     *
     * @throws IllegalStateException if it is called in an invalid state
     */
    CARAPI Start();

    /**
     * Stops playback after playback has been stopped or paused.
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     */
    CARAPI Stop();

    /**
     * Pauses playback. Call start() to resume.
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     */
    CARAPI Pause();

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
    CARAPI SetWakeMode(
        /* [in] */ IContext* context,
        /* [in] */ Int32 mode);

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
    CARAPI GetMetadata(
        /* [in] */ Boolean update_only,
        /* [in] */ Boolean apply_filter,
        /* [out] */ IMetadata** metadata);

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

    /**
     * Releases resources associated with this MediaPlayer object.
     * It is considered good practice to call this method when you're
     * done using the MediaPlayer.
     */
    CARAPI ReleaseResources();

    /**
     * Resets the MediaPlayer to its uninitialized state. After calling
     * this method, you will have to initialize it again by setting the
     * data source and calling prepare().
     */
    CARAPI Reset();

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
    CARAPI Suspend(
        /* [out] */ Boolean* succeeded);

    /**
     * Resumes the MediaPlayer. Only to be called after a previous (successful)
     * call to {@link #suspend()}.
     * MediaPlayer will return to a state close to what it was in before
     * suspension.
     * @hide
     */
    CARAPI Resume(
        /* [out] */ Boolean* succeeded);

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

    /**
     * Register a callback to be invoked when the media source is ready
     * for playback.
     *
     * @param listener the callback that will be run
     */
    CARAPI SetOnPreparedListener(
        /* [in] */ IMediaPlayerOnPreparedListener* listener);

    /**
     * Register a callback to be invoked when the end of a media source
     * has been reached during playback.
     *
     * @param listener the callback that will be run
     */
    CARAPI SetOnCompletionListener(
        /* [in] */ IMediaPlayerOnCompletionListener* listener);

    /**
     * Register a callback to be invoked when the status of a network
     * stream's buffer has changed.
     *
     * @param listener the callback that will be run.
     */
    CARAPI SetOnBufferingUpdateListener(
        /* [in] */ IMediaPlayerOnBufferingUpdateListener* listener);

    /**
     * Register a callback to be invoked when a seek operation has been
     * completed.
     *
     * @param listener the callback that will be run
     */
    CARAPI SetOnSeekCompleteListener(
        /* [in] */ IMediaPlayerOnSeekCompleteListener* listener);

    /**
     * Register a callback to be invoked when the video size is
     * known or updated.
     *
     * @param listener the callback that will be run
     */
    CARAPI SetOnVideoSizeChangedListener(
        /* [in] */ IMediaPlayerOnVideoSizeChangedListener* listener);

    /**
     * Register a callback to be invoked when an info/warning is available.
     *
     * @param listener the callback that will be run
     */
    CARAPI SetOnInfoListener(
        /* [in] */ IMediaPlayerOnInfoListener* listener);

    /**
     * Register a callback to be invoked when an error has happened
     * during an asynchronous operation.
     *
     * @param listener the callback that will be run
     */
    CARAPI SetOnErrorListener(
        /* [in] */ IMediaPlayerOnErrorListener* listener);

    /**
     * Called from  native  code when an interesting event happens.  This method
     * just uses the EventHandler system to post the event back to the main app thread.
     * We use a weak reference to the original MediaPlayer object so that the native
     * code is safe from the object disappearing from underneath it.  (This is
     * the cookie passed to native_setup().)
     */
    static CARAPI_(void) PostEventFromNative(
        /* [in] */ IInterface* mediaplayer_ref,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

private:
    /*
     * Update the MediaPlayer ISurface. Call after updating mSurface.
     */
    CARAPI NativeSetVideoSurface();

    CARAPI NativeStart();

    CARAPI NativeStop();

    CARAPI NativePause();

    CARAPI_(void) StayAwake(
        /* [in] */ Boolean awake);

    CARAPI_(void) UpdateSurfaceScreenOn();

    CARAPI_(void) NativeRelease();

    CARAPI NativeReset();

    /**
     * @hide
     */
    CARAPI NativeSuspendResume(
        /* [in] */ Boolean isSuspend,
        /* [out] */ Int32* result);

    /**
     * @param request Parcel destinated to the media player. The
     *                Interface token must be set to the IMediaPlayer
     *                one to be routed correctly through the system.
     * @param reply[out] Parcel that will contain the reply.
     * @return The status code.
     */
    CARAPI NativeInvoke(
        /* [in] */ IParcel* request,
        /* [in] */ IParcel* reply,
        /* [out] */ Int32* result);

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
    CARAPI NativeGetMetadata(
        /* [in] */ Boolean update_only,
        /* [in] */ Boolean apply_filter,
        /* [in] */ IParcel* reply,
        /* [out] */ Boolean* succeeded);

    /**
     * @param request Parcel with the 2 serialized lists of allowed
     *                metadata types followed by the one to be
     *                dropped. Each list starts with an integer
     *                indicating the number of metadata type elements.
     * @return The status code.
     */
    CARAPI NativeSetMetadataFilter(
        /* [in] */ IParcel* request,
        /* [out] */ Int32* result);

    CARAPI NativeSetup(
        /* [in] */ CMediaPlayer* mediaplayer);

    CARAPI_(void) NativeFinalize();

public:
    Int32 mNativeContext; // accessed by native methods
    AutoPtr<ISurface> mSurface; // accessed by native methods

private:
    static CString TAG;
    // Name of the remote interface for the media player. Must be kept
    // in sync with the 2nd parameter of the IMPLEMENT_META_INTERFACE
    // macro invocation in IMediaPlayer.cpp
    static CString IMEDIA_PLAYER;

    Int32 mListenerContext; // accessed by native methods

    AutoPtr<ISurfaceHolder> mSurfaceHolder;
    AutoPtr<IApartment> mEventHandler;
    //PowerManager.WakeLock mWakeLock = NULL;
    Boolean mScreenOnWhilePlaying;
    Boolean mStayAwake;

    /* Do not change these values without updating their counterparts
     * in include/media/mediaplayer.h!
     */
    static const Int32 MEDIA_NOP = 0; // interface test message
    static const Int32 MEDIA_PREPARED = 1;
    static const Int32 MEDIA_PLAYBACK_COMPLETE = 2;
    static const Int32 MEDIA_BUFFERING_UPDATE = 3;
    static const Int32 MEDIA_SEEK_COMPLETE = 4;
    static const Int32 MEDIA_SET_VIDEO_SIZE = 5;
    static const Int32 MEDIA_ERROR = 100;
    static const Int32 MEDIA_INFO = 200;

    AutoPtr<IMediaPlayerOnPreparedListener> mOnPreparedListener;

    AutoPtr<IMediaPlayerOnCompletionListener> mOnCompletionListener;

    AutoPtr<IMediaPlayerOnBufferingUpdateListener> mOnBufferingUpdateListener;

    AutoPtr<IMediaPlayerOnSeekCompleteListener> mOnSeekCompleteListener;

    AutoPtr<IMediaPlayerOnVideoSizeChangedListener> mOnVideoSizeChangedListener;

    AutoPtr<IMediaPlayerOnErrorListener> mOnErrorListener;

    AutoPtr<IMediaPlayerOnInfoListener> mOnInfoListener;
};

#endif // __CMEDIAPLAYER_H__
