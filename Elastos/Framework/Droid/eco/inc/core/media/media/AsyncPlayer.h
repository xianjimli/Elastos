
#ifndef __ASYNCPLAYER_H__
#define __ASYNCPLAYER_H__


#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>


/**
 * Plays a series of audio URIs, but does all the hard work on another thread
 * so that any slowness with preparing or loading doesn't block the calling thread.
 */
class AsyncPlayer
{
public:
    AsyncPlayer();

    /**
     * Construct an AsyncPlayer object.
     *
     * @param tag a string to use for debugging
     */
    AsyncPlayer(
        /* [in] */ String tag);

    /**
     * Start playing the sound.  It will actually start playing at some
     * point in the future.  There are no guarantees about latency here.
     * Calling this before another audio file is done playing will stop
     * that one and start the new one.
     *
     * @param context Your application's context.
     * @param uri The URI to play.  (see {@link MediaPlayer#setDataSource(Context, Uri)})
     * @param looping Whether the audio should loop forever.  
     *          (see {@link MediaPlayer#setLooping(Boolean)})
     * @param stream the AudioStream to use.
     *          (see {@link MediaPlayer#setAudioStreamType(Int32)})
     */
    virtual CARAPI Play(
        /* [in] */ IContext* context, 
        /* [in] */ IUri* uri, 
        /* [in] */ Boolean looping, 
        /* [in] */ Int32 stream);
    
    /**
     * Stop a previously played sound.  It can't be played again or unpaused
     * at this point.  Calling this multiple times has no ill effects.
     */
    virtual CARAPI Stop();

    /**
     * We want to hold a wake lock while we do the prepare and play.  The stop probably is
     * optional, but it won't hurt to have it too.  The problem is that if you start a sound
     * while you're holding a wake lock (e.g. an alarm starting a notification), you want the
     * sound to play, but if the CPU turns off before mThread gets to work, it won't.  The
     * simplest way to deal with this is to make it so there is a wake lock held while the
     * thread is starting or running.  You're going to need the WAKE_LOCK permission if you're
     * going to call this.
     *
     * This must be called before the first time play is called.
     *
     * @hide
     */
    virtual CARAPI SetUsesWakeLock(
        /* [in] */ IContext* context);

private:
    class Command
    {
    public:
        CARAPI_(String) ToString();

        Int32 code;
        AutoPtr<IContext> context;
        AutoPtr<IUri> uri;
        Boolean looping;
        Int32 stream;
        Millisecond64 requestTime;
    };

    CARAPI_(void) EnqueueLocked(
        /* [in] */ Command* cmd);

    CARAPI_(void) AcquireWakeLock();

    CARAPI_(void) ReleaseWakeLock();

    static const Int32 PLAY = 1;
    static const Int32 STOP = 2;
    static const Boolean mDebug = FALSE;

    //LinkedList<Command> mCmdQueue = new LinkedList();

    CARAPI_(void) StartSound(
        /* [in] */ Command* cmd);

    class Thread : public IThread, public ElRefBase
    {
    public:
        Thread();

        virtual CARAPI Run();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        AsyncPlayer* mOwner;
    };

    String mTag;
    AutoPtr<Thread> mThread;
    AutoPtr<IMediaPlayer> mPlayer;
    //PowerManager.WakeLock mWakeLock;

    // The current state according to the caller.  Reality lags behind
    // because of the asynchronous nature of this class.
    Int32 mState;// = STOP;
};

#endif
