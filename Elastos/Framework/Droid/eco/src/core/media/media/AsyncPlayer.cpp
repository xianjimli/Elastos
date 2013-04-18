
#include "media/AsyncPlayer.h"
#include "media/CMediaPlayer.h"
#include "os/SystemClock.h"

String AsyncPlayer::Command::ToString()
{
    /*return "{ code=" + code + " looping=" + looping + " stream=" + stream
            + " uri=" + uri + " }";*/
}

void AsyncPlayer::StartSound(
    /* [in] */ Command* cmd)
{
    // Preparing can be slow, so if there is something else
    // is playing, let it continue until we're done, so there
    // is less of a glitch.
    //try {
        //if (mDebug) Log.d(mTag, "Starting playback");
        AutoPtr<IMediaPlayer> player;
        CMediaPlayer::New((IMediaPlayer**)&player);
        ((CMediaPlayer*)player.Get())->SetAudioStreamType(cmd->stream);
        player->SetDataSource(cmd->context, cmd->uri);
        ((CMediaPlayer*)player.Get())->SetLooping(cmd->looping);
        ((CMediaPlayer*)player.Get())->Prepare();
        player->Start();
        if (mPlayer != NULL) {
            mPlayer->Release();
        }
        mPlayer = player;
        Int64 delay;// = SystemClock::GetUptimeMillis() - cmd->requestTime;
        if (delay > 1000) {
            //Log.w(mTag, "Notification sound delayed by " + delay + "msecs");
        }
    //}
    /*catch (Exception e) {
        Log.w(mTag, "error loading sound for " + cmd.uri, e);
    }*/
}


AsyncPlayer::Thread::Thread()
{
    //super("AsyncPlayer-" + mTag);
}

ECode AsyncPlayer::Thread::Run()
{
    while (TRUE) {
        Command* cmd = NULL;

        //synchronized (mCmdQueue) {
            //if (mDebug) Log.d(mTag, "RemoveFirst");
            //cmd = mCmdQueue.removeFirst();
        //}

        switch (cmd->code) {
        case PLAY:
            //if (mDebug) Log.d(mTag, "PLAY");
            mOwner->StartSound(cmd);
            break;
        case STOP:
            //if (mDebug) Log.d(mTag, "STOP");
            if (mOwner->mPlayer != NULL) {
                Millisecond64 delay;// = SystemClock::GetUptimeMillis() - cmd->requestTime;
                if (delay > 1000) {
                    //Log.w(mTag, "Notification stop delayed by " + delay + "msecs");
                }
                mOwner->mPlayer->Stop();
                mOwner->mPlayer->Release();
                mOwner->mPlayer = NULL;
            } else {
                //Log.w(mTag, "STOP command without a player");
            }
            break;
        }

        //synchronized (mCmdQueue) {
            //if (mCmdQueue.size() == 0) {
            //    // nothing left to do, quit
            //    // doing this check after we're done prevents the case where they
            //    // added it during the operation from spawning two threads and
            //    // trying to do them in parallel.
            //    mThread = NULL;
            //    releaseWakeLock();
            //    return;
            //}
        //}
    }

    return NOERROR;
}

PInterface AsyncPlayer::Thread::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 AsyncPlayer::Thread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AsyncPlayer::Thread::Release()
{
    return ElRefBase::Release();
}

ECode AsyncPlayer::Thread::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}


AsyncPlayer::AsyncPlayer()
{

}

/**
 * Construct an AsyncPlayer object.
 *
 * @param tag a string to use for debugging
 */
AsyncPlayer::AsyncPlayer(
    /* [in] */ String tag) 
{
    if (tag != NULL) {
        mTag = tag;
    } else {
        mTag = "AsyncPlayer";
    }
}

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
ECode AsyncPlayer::Play(
    /* [in] */ IContext* context, 
    /* [in] */ IUri* uri, 
    /* [in] */ Boolean looping, 
    /* [in] */ Int32 stream)
{
    Command* cmd = new Command();
    //cmd->requestTime = SystemClock::GetUptimeMillis();
    cmd->code = PLAY;
    cmd->context = context;
    cmd->uri = uri;
    cmd->looping = looping;
    cmd->stream = stream;
    //synchronized (mCmdQueue) {
        EnqueueLocked(cmd);
        mState = PLAY;
    //}

    return NOERROR;
}

/**
 * Stop a previously played sound.  It can't be played again or unpaused
 * at this point.  Calling this multiple times has no ill effects.
 */
ECode AsyncPlayer::Stop()
{
    //synchronized (mCmdQueue) {
        // This check allows stop to be called multiple times without starting
        // a thread that ends up doing nothing.
        if (mState != STOP) {
            Command* cmd = new Command();
            //cmd->requestTime = SystemClock::GetUptimeMillis();
            cmd->code = STOP;
            EnqueueLocked(cmd);
            mState = STOP;
        }
    //}
}

void AsyncPlayer::EnqueueLocked(
    /* [in] */ Command* cmd)
{
    //mCmdQueue.add(cmd);
    if (mThread == NULL) {
        AcquireWakeLock();
        //mThread = new Thread();
        mThread->Start();
    }
}

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
ECode AsyncPlayer::SetUsesWakeLock(
    /* [in] */ IContext* context)
{
    //if (mWakeLock != NULL || mThread != NULL) {
    //    // if either of these has happened, we've already played something.
    //    // and our releases will be out of sync.
    //    throw new RuntimeException("assertion failed mWakeLock=" + mWakeLock
    //            + " mThread=" + mThread);
    //}
    //PowerManager pm = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
    //mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, mTag);

    return NOERROR;
}

void AsyncPlayer::AcquireWakeLock()
{
    /*if (mWakeLock != NULL) {
        mWakeLock.acquire();
    }*/
}

void AsyncPlayer::ReleaseWakeLock()
{
    /*if (mWakeLock != NULL) {
        mWakeLock.release();
    }*/
}

