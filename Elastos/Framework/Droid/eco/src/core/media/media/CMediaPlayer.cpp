
#include <media/mediaplayer.h>
#include <binder/Parcel.h>
#include <surfaceflinger/Surface.h>
#include <utils/String8.h>
#include "media/CMediaPlayer.h"
#include "media/CMetadata.h"
#include "os/CApartment.h"


//MediaPlayer::EventHandler::EventHandler(
//    /* [in] */ IMediaPlayer* mp,
//    /* [in] */ Looper looper)
//{
//    super(looper);
//    mMediaPlayer = mp;
//}

IInterface* CMediaPlayer::EventHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IApartment*)this;
    }
    else if (riid == EIID_IApartment) {
        return (IApartment*)this;
    }
    return NULL;
}

UInt32 CMediaPlayer::EventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaPlayer::EventHandler::Release()
{
    return ElRefBase::Release();
}

ECode CMediaPlayer::EventHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

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


CString CMediaPlayer::TAG = "MediaPlayer";
// Name of the remote interface for the media player. Must be kept
// in sync with the 2nd parameter of the IMPLEMENT_META_INTERFACE
// macro invocation in IMediaPlayer.cpp
CString CMediaPlayer::IMEDIA_PLAYER = "android.media.IMediaPlayer";
const Int32 CMediaPlayer::MEDIA_NOP;
const Int32 CMediaPlayer::MEDIA_PREPARED;
const Int32 CMediaPlayer::MEDIA_PLAYBACK_COMPLETE;
const Int32 CMediaPlayer::MEDIA_BUFFERING_UPDATE;
const Int32 CMediaPlayer::MEDIA_SEEK_COMPLETE;
const Int32 CMediaPlayer::MEDIA_SET_VIDEO_SIZE;
const Int32 CMediaPlayer::MEDIA_ERROR;
const Int32 CMediaPlayer::MEDIA_INFO;

static Mutex sLock;

CMediaPlayer::CMediaPlayer()
    : mScreenOnWhilePlaying(FALSE)
    , mStayAwake(FALSE)
{
    CApartment::GetMyApartment((IApartment**)&mEventHandler);
    if (mEventHandler == NULL) {
         CApartment::GetMainApartment((IApartment**)&mEventHandler);
    }

    ///* Native setup requires a weak reference to our object.
    // * It's easier to create it here than in C++.
    // */
    //Native_setup(new WeakReference<MediaPlayer>(this));
    ASSERT_SUCCEEDED(NativeSetup(this));
}

CMediaPlayer::~CMediaPlayer()
{
    NativeFinalize();
}

// ----------------------------------------------------------------------------
// ref-counted object for callbacks
class JNIMediaPlayerListener : public android::MediaPlayerListener
{
public:
    JNIMediaPlayerListener(CMediaPlayer* weak_thiz);
    ~JNIMediaPlayerListener();
    void notify(int msg, int ext1, int ext2);
private:
    JNIMediaPlayerListener();
    CMediaPlayer* mObject;    // Weak ref to MediaPlayer Java object to call on
};

JNIMediaPlayerListener::JNIMediaPlayerListener(CMediaPlayer* weak_thiz)
{
    // We use a weak reference so the MediaPlayer object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    mObject  = weak_thiz;
}

JNIMediaPlayerListener::~JNIMediaPlayerListener()
{}

void JNIMediaPlayerListener::notify(int msg, int ext1, int ext2)
{
    // mObject->PostEventFromNative(mObject, msg, ext1, ext2, 0);
}

static android::sp<android::MediaPlayer> getMediaPlayer(CMediaPlayer* thiz)
{
    Mutex::Autolock l(sLock);
    android::MediaPlayer* const p = (android::MediaPlayer*)thiz->mNativeContext;
    return android::sp<android::MediaPlayer>(p);
}

static android::sp<android::MediaPlayer> setMediaPlayer(CMediaPlayer* thiz, const android::sp<android::MediaPlayer>& player)
{
    Mutex::Autolock l(sLock);
    android::sp<android::MediaPlayer> old = (android::MediaPlayer*)thiz->mNativeContext;
    if (player.get()) {
        player->incStrong(thiz);
    }
    if (old != 0) {
        old->decStrong(thiz);
    }
    thiz->mNativeContext = (Int32)player.get();
    return old;
}

// If exception is NULL and opStatus is not OK, this method sends an error
// event to the client application; otherwise, if exception is not NULL and
// opStatus is not OK, this method throws the given exception to the client
// application.
static ECode process_media_player_call(CMediaPlayer* thiz, android::status_t opStatus, ECode ec)
{
    if (ec == NOERROR) {  // Don't throw exception. Instead, send an event.
        if (opStatus != 0) {
            android::sp<android::MediaPlayer> mp = getMediaPlayer(thiz);
            if (mp != 0) mp->notify(android::MEDIA_ERROR, opStatus, 0);
        }
        return NOERROR;
    }
    else {  // Throw exception!
        if (opStatus == (android::status_t)android::INVALID_OPERATION ) {
            // jniThrowException(env, "java/lang/IllegalStateException", NULL);
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        else if (opStatus != 0) {
            // if (strlen(message) > 230) {
            //    // if the message is too long, don't bother displaying the status code
            //    jniThrowException( env, exception, message);
            // }
            // else {
            //    char msg[256];
            //     // append the status code to the message
            //    sprintf(msg, "%s: status=0x%X", message, opStatus);
            //    jniThrowException( env, exception, msg);
            // }
            return ec;
        }
        return NOERROR;
    }
}

static void setVideoSurface(const android::sp<android::MediaPlayer>& mp, CMediaPlayer* thiz)
{
    if (thiz->mSurface != NULL) {
        android::Surface* _surface;
        thiz->mSurface->GetSurface((Handle32*)&_surface);
        android::sp<android::Surface> native_surface = _surface;
        // LOGV("prepare: surface=%p (id=%d)",
        //      native_surface.get(), native_surface->getIdentity());
        mp->setVideoSurface(native_surface);
    }
}

/*
 * Update the MediaPlayer ISurface. Call after updating mSurface.
 */
ECode CMediaPlayer::NativeSetVideoSurface()
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    setVideoSurface(mp, this);
    return NOERROR;
}


ECode CMediaPlayer::NewRequest(
    /* [out] */ IParcel ** parcel)
{
    VALIDATE_NOT_NULL(parcel);

    // Parcel parcel = Parcel.obtain();
    // parcel.writeInterfaceToken(IMEDIA_PLAYER);
    // return parcel;
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Invoke(
    /* [in] */ IParcel* request,
    /* [in] */ IParcel* reply,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    FAIL_RETURN(NativeInvoke(request, reply, status));
    //reply->SetDataPosition(0);
    return NOERROR;
}

ECode CMediaPlayer::SetDisplay(
    /* [in] */ ISurfaceHolder* sh)
{
    mSurfaceHolder = sh;
    if (sh != NULL) {
        sh->GetSurface((ISurface**)&mSurface);
    }
    else {
        mSurface = NULL;
    }
    FAIL_RETURN(NativeSetVideoSurface());
    UpdateSurfaceScreenOn();
    return NOERROR;
}

ECode CMediaPlayer::Create(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [out] */ IMediaPlayer** player)
{
    return Create(context, uri, NULL, player);
}

ECode CMediaPlayer::Create(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ ISurfaceHolder* holder,
    /* [out] */ IMediaPlayer** player)
{
    VALIDATE_NOT_NULL(player);

    //try {
    *player = NULL;
    AutoPtr<IMediaPlayer> mp;
    FAIL_RETURN(CMediaPlayer::New((IMediaPlayer**)&mp));
    FAIL_RETURN(mp->SetDataSource(context, uri));
    if (holder != NULL) {
        FAIL_RETURN(mp->SetDisplay(holder));
    }
    FAIL_RETURN(mp->Prepare());
    *player = mp;
    (*player)->AddRef();
    return NOERROR;
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
}

ECode CMediaPlayer::Create(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resid,
    /* [out] */ IMediaPlayer** player)
{
    VALIDATE_NOT_NULL(player);

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
    // return NULL;
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
    return SetDataSourceEx(context, uri, NULL);
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
    String scheme;
    uri->GetScheme(&scheme);
    if(scheme.IsNull() || scheme.Equals("file")) {
        String path;
        uri->GetPath(&path);
        return SetDataSourceEx2(path);
    }

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
    return SetDataSourceEx3(path, NULL);
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
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (path.IsNull()) {
        // jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    const char *pathStr = path.string();

    // headers is a Map<String, String>.
    // We build a similar KeyedVector out of it.
    android::KeyedVector<android::String8, android::String8> headersVector;
//    if (headers) {
//        // Get the Map's entry Set.
//        jclass mapClass = env->FindClass("java/util/Map");
//
//        jmethodID entrySet =
//            env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
//
//        jobject set = env->CallObjectMethod(headers, entrySet);
//        // Obtain an iterator over the Set
//        jclass setClass = env->FindClass("java/util/Set");
//
//        jmethodID iterator =
//            env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
//
//        jobject iter = env->CallObjectMethod(set, iterator);
//        // Get the Iterator method IDs
//        jclass iteratorClass = env->FindClass("java/util/Iterator");
//        jmethodID hasNext = env->GetMethodID(iteratorClass, "hasNext", "()Z");
//
//        jmethodID next =
//            env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
//
//        // Get the Entry class method IDs
//        jclass entryClass = env->FindClass("java/util/Map$Entry");
//
//        jmethodID getKey =
//            env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
//
//        jmethodID getValue =
//            env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
//
//        // Iterate over the entry Set
//        while (env->CallBooleanMethod(iter, hasNext)) {
//            jobject entry = env->CallObjectMethod(iter, next);
//            jstring key = (jstring) env->CallObjectMethod(entry, getKey);
//            jstring value = (jstring) env->CallObjectMethod(entry, getValue);
//
//            const char* keyStr = env->GetStringUTFChars(key, NULL);
//            if (!keyStr) {  // Out of memory
//                jniThrowException(
//                        env, "java/lang/RuntimeException", "Out of memory");
//                return;
//            }
//
//            const char* valueStr = env->GetStringUTFChars(value, NULL);
//            if (!valueStr) {  // Out of memory
//                jniThrowException(
//                        env, "java/lang/RuntimeException", "Out of memory");
//                return;
//            }
//
//            headersVector.add(String8(keyStr), String8(valueStr));
//
//            env->DeleteLocalRef(entry);
//            env->ReleaseStringUTFChars(key, keyStr);
//            env->DeleteLocalRef(key);
//            env->ReleaseStringUTFChars(value, valueStr);
//            env->DeleteLocalRef(value);
//      }
//
//      env->DeleteLocalRef(entryClass);
//      env->DeleteLocalRef(iteratorClass);
//      env->DeleteLocalRef(iter);
//      env->DeleteLocalRef(setClass);
//      env->DeleteLocalRef(set);
//      env->DeleteLocalRef(mapClass);
//    }
//
    // LOGV("setDataSource: path %s", pathStr);
    android::status_t opStatus =
        mp->setDataSource(
            android::String8(pathStr), headers ? &headersVector : NULL);
    return process_media_player_call(this, opStatus, E_IO_EXCEPTION);
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
    // intentionally less than LONG_MAX
    return SetDataSourceEx5(fd, 0, 0x7ffffffffffffffll);
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
    // android::sp<android::MediaPlayer> mp = getMediaPlayer(env, thiz);
    // if (mp == NULL ) {
    //     jniThrowException(env, "java/lang/IllegalStateException", NULL);
    //     return;
    // }

    // if (fileDescriptor == NULL) {
    //     jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
    //     return;
    // }
    // int fd = getParcelFileDescriptorFD(env, fileDescriptor);
    // LOGV("setDataSourceFD: fd %d", fd);
    // process_media_player_call( env, thiz, mp->setDataSource(fd, offset, length), "java/io/IOException", "setDataSourceFD failed." );
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
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    setVideoSurface(mp, this);
    return process_media_player_call(this, mp->prepare(), E_IO_EXCEPTION);
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
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mSurface != NULL) {
        android::Surface* _surface;
        mSurface->GetSurface((Handle32*)&_surface);
        android::sp<android::Surface> native_surface = _surface;
        // LOGV("prepareAsync: surface=%p (id=%d)",
        //      native_surface.get(), native_surface->getIdentity());
        mp->setVideoSurface(native_surface);
    }
    return process_media_player_call(this, mp->prepareAsync(), E_IO_EXCEPTION);
}

ECode CMediaPlayer::Start()
{
    StayAwake(TRUE);
    return NativeStart();
}

ECode CMediaPlayer::NativeStart()
{
    // LOGV("start");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->start(), NOERROR);
}

ECode CMediaPlayer::Stop()
{
    StayAwake(FALSE);
    return NativeStop();
}

ECode CMediaPlayer::NativeStop()
{
    // LOGV("stop");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->stop(), NOERROR);
}

ECode CMediaPlayer::Pause()
{
    StayAwake(FALSE);
    return NativePause();
}

ECode CMediaPlayer::NativePause()
{
    // LOGV("pause");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->pause(), NOERROR);
}

ECode CMediaPlayer::SetWakeMode(
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

    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetScreenOnWhilePlaying(
    /* [in] */ Boolean screenOn)
{
    if (mScreenOnWhilePlaying != screenOn) {
        mScreenOnWhilePlaying = screenOn;
        UpdateSurfaceScreenOn();
    }

    return NOERROR;
}

void CMediaPlayer::StayAwake(
    /* [in] */ Boolean awake)
{
    // if (mWakeLock != NULL) {
    //     if (awake && !mWakeLock.isHeld()) {
    //         mWakeLock.acquire();
    //     } else if (!awake && mWakeLock.isHeld()) {
    //         mWakeLock.release();
    //     }
    // }
    mStayAwake = awake;
    UpdateSurfaceScreenOn();
}

void CMediaPlayer::UpdateSurfaceScreenOn()
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
ECode CMediaPlayer::GetVideoWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    int w;
    if (0 != mp->getVideoWidth(&w)) {
        // LOGE("getVideoWidth failed");
        w = 0;
    }
    // LOGV("getVideoWidth: %d", w);
    *width = w;
    return NOERROR;
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
    VALIDATE_NOT_NULL(height);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    int h;
    if (0 != mp->getVideoHeight(&h)) {
        // LOGE("getVideoHeight failed");
        h = 0;
    }
    // LOGV("getVideoHeight: %d", h);
    *height = h;
    return NOERROR;
}

/**
 * Checks whether the MediaPlayer is playing.
 *
 * @return true if currently playing, false otherwise
 */
ECode CMediaPlayer::IsPlaying(
    /* [out] */ Boolean* isPlaying)
{
    VALIDATE_NOT_NULL(isPlaying);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        *isPlaying = FALSE;
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *isPlaying = mp->isPlaying();

    // LOGV("isPlaying: %d", is_playing);
    return NOERROR;
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
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // LOGV("seekTo: %d(msec)", msec);
    return process_media_player_call(this, mp->seekTo(msec), NOERROR);
}

/**
 * Gets the current playback position.
 *
 * @return the current position in milliseconds
 */
ECode CMediaPlayer::GetCurrentPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        *position = 0;
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    int msec;
    ECode ec = process_media_player_call(this, mp->getCurrentPosition(&msec), NOERROR);
    // LOGV("getCurrentPosition: %d (msec)", msec);
    *position = msec;
    return ec;
}

ECode CMediaPlayer::GetDuration(
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        *duration = 0;
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    int msec;
    ECode ec = process_media_player_call(this, mp->getDuration(&msec), NOERROR);
    // LOGV("getDuration: %d (msec)", msec);
    *duration = msec;
    return ec;
}

ECode CMediaPlayer::GetMetadata(
    /* [in] */ Boolean update_only,
    /* [in] */ Boolean apply_filter,
    /* [out] */ IMetadata** metadata)
{
    VALIDATE_NOT_NULL(metadata);

    AutoPtr<IParcel> reply;// = Parcel.obtain();
    AutoPtr<IMetadata> data;
    FAIL_RETURN(CMetadata::New((IMetadata**)&data));

    Boolean succeeded;
    FAIL_RETURN(NativeGetMetadata(update_only, apply_filter, reply, &succeeded));
    if (!succeeded) {
        // reply->Recycle();
        *metadata = NULL;
        return NOERROR;
    }

    // Metadata takes over the parcel, don't recycle it unless
    // there is an error.
    Boolean res;
    FAIL_RETURN(data->Parse(reply, &res));
    if (!res) {
        // reply->Recycle();
        *metadata = NULL;
        return NOERROR;
    }
    *metadata = data;
    (*metadata)->AddRef();

    return NOERROR;
}

ECode CMediaPlayer::SetMetadataFilter(
    /* [in] */ IObjectContainer* allow,
    /* [in] */ IObjectContainer* block,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    // Do our serialization manually instead of calling
    // Parcel.writeArray since the sets are made of the same type
    // we avoid paying the price of calling writeValue (used by
    // writeArray) which burns an extra Int32 per element to encode
    // the type.
    AutoPtr<IParcel> request;
    FAIL_RETURN(NewRequest((IParcel**)&request));

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
    return NativeSetMetadataFilter(request, result);
}

ECode CMediaPlayer::ReleaseResources()
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
    NativeRelease();

    return NOERROR;
}

void CMediaPlayer::NativeRelease()
{
    LOGV("release");
    android::sp<android::MediaPlayer> mp = setMediaPlayer(this,
            android::sp<android::MediaPlayer>(NULL));
    if (mp != NULL) {
        // this prevents native callbacks after the object is released
        mp->setListener(0);
        mp->disconnect();
    }
}

ECode CMediaPlayer::Reset()
{
    StayAwake(FALSE);
    FAIL_RETURN(NativeReset());
    // make sure none of the listeners get called anymore
    //mEventHandler->RemoveCallbacksAndMessages(NULL);
    return NOERROR;
}

ECode CMediaPlayer::NativeReset()
{
    // LOGV("reset");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->reset(), NOERROR);
}


ECode CMediaPlayer::Suspend(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Int32 result;
    FAIL_RETURN(NativeSuspendResume(TRUE, &result));
    if (result < 0) {
        *succeeded = FALSE;
        return NOERROR;
    }

    StayAwake(FALSE);

    // make sure none of the listeners get called anymore
    //mEventHandler->RemoveCallbacksAndMessages(NULL);

    *succeeded = TRUE;
    return NOERROR;
}

ECode CMediaPlayer::Resume(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Int32 result;
    FAIL_RETURN(NativeSuspendResume(FALSE, &result));
    if (result < 0) {
        *succeeded = FALSE;
        return NOERROR;
    }

    Boolean isPlaying;
    FAIL_RETURN(IsPlaying(&isPlaying));
    if (isPlaying) {
        StayAwake(TRUE);
    }

    *succeeded = TRUE;
    return NOERROR;
}

/**
 * @hide
 */
ECode CMediaPlayer::NativeSuspendResume(
    /* [in] */ Boolean isSuspend,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    // LOGV("suspend_resume(%d)", isSuspend);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        *result = android::UNKNOWN_ERROR;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *result = isSuspend ? mp->suspend() : mp->resume();
    return NOERROR;
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
    // LOGV("setAudioStreamType: %d", streamtype);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setAudioStreamType(streamtype) , NOERROR);
}

/**
 * Sets the player to be looping or non-looping.
 *
 * @param looping whether to loop or not
 */
ECode CMediaPlayer::SetLooping(
    /* [in] */ Boolean looping)
{
    // LOGV("setLooping: %d", looping);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setLooping(looping), NOERROR);
}

/**
 * Checks whether the MediaPlayer is looping or non-looping.
 *
 * @return true if the MediaPlayer is currently looping, false otherwise
 */
ECode CMediaPlayer::IsLooping(
    /* [out] */ Boolean* isLooping)
{
    VALIDATE_NOT_NULL(isLooping);

    // LOGV("isLooping");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        *isLooping = FALSE;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *isLooping = mp->isLooping();
    return NOERROR;
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
    // LOGV("setVolume: left %f  right %f", leftVolume, rightVolume);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setVolume(leftVolume, rightVolume), NOERROR);
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
    VALIDATE_NOT_NULL(frame);

    *frame = NULL;
    return NOERROR;
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
    // LOGV("set_session_id(): %d", sessionId);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setAudioSessionId(sessionId), NOERROR);
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
    VALIDATE_NOT_NULL(sessionID);

    // LOGV("get_session_id()");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        *sessionID = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *sessionID = mp->getAudioSessionId();
    return NOERROR;
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
    // LOGV("attachAuxEffect(): %d", effectId);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->attachAuxEffect(effectId), NOERROR);
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
    // LOGV("setAuxEffectSendLevel: level %f", level);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setAuxEffectSendLevel(level), NOERROR);
}

ECode CMediaPlayer::NativeInvoke(
    /* [in] */ IParcel* request,
    /* [in] */ IParcel* reply,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    android::sp<android::MediaPlayer> media_player = getMediaPlayer(this);
    if (media_player == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        *result = android::UNKNOWN_ERROR;
        return E_ILLEGAL_STATE_EXCEPTION;
    }


    android::Parcel* _request;
    request->GetElementPayload((Handle32*)&_request);
    android::Parcel* _reply;
    reply->GetElementPayload((Handle32*)&_reply);

    // Don't use process_media_player_call which use the async loop to
    // report errors, instead returns the status.
    *result = media_player->invoke(*_request, _reply);
    return NOERROR;
}

ECode CMediaPlayer::NativeGetMetadata(
    /* [in] */ Boolean update_only,
    /* [in] */ Boolean apply_filter,
    /* [in] */ IParcel* reply,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    android::sp<android::MediaPlayer> media_player = getMediaPlayer(this);
    if (media_player == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        *succeeded = FALSE;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Parcel* metadata;
    reply->GetElementPayload((Handle32*)&metadata);

    if (metadata == NULL ) {
        // jniThrowException(env, "java/lang/RuntimeException", "Reply parcel is null");
        *succeeded = FALSE;
        return E_RUNTIME_EXCEPTION;
    }

    metadata->freeData();
    // On return metadata is positioned at the beginning of the
    // metadata. Note however that the parcel actually starts with the
    // return code so you should not rewind the parcel using
    // setDataPosition(0).
    *succeeded = media_player->getMetadata(update_only, apply_filter, metadata) == 0;
    return NOERROR;
}

ECode CMediaPlayer::NativeSetMetadataFilter(
    /* [in] */ IParcel* request,
    /* [out] */ Int32* result)
{
    android::sp<android::MediaPlayer> media_player = getMediaPlayer(this);
    if (media_player == NULL ) {
        // jniThrowException(env, "java/lang/IllegalStateException", NULL);
        *result = android::UNKNOWN_ERROR;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Parcel* filter;
    request->GetElementPayload((Handle32*)&filter);

    if (filter == NULL ) {
        // jniThrowException(env, "java/lang/RuntimeException", "Filter is null");
        *result = android::UNKNOWN_ERROR;
        return E_RUNTIME_EXCEPTION;
    }

    *result = media_player->setMetadataFilter(*filter);
    return NOERROR;
}

ECode CMediaPlayer::NativeSetup(
    /* [in] */ CMediaPlayer* mediaplayer/*weak_this*/)
{
    // LOGV("native_setup");
    android::sp<android::MediaPlayer> mp = new android::MediaPlayer();
    if (mp == NULL) {
        // jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
        return E_RUNTIME_EXCEPTION;
    }

    // create new listener and give it to MediaPlayer
    android::sp<JNIMediaPlayerListener> listener = new JNIMediaPlayerListener(this);
    mp->setListener(listener);

    // Stow our new C++ MediaPlayer in an opaque field in the Java object.
    setMediaPlayer(this, mp);
    return NOERROR;
}

void CMediaPlayer::NativeFinalize()
{
    // LOGV("native_finalize");
    NativeRelease();
}

void CMediaPlayer::PostEventFromNative(
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
ECode CMediaPlayer::SetOnPreparedListener(
    /* [in] */ IMediaPlayerOnPreparedListener* listener)
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
ECode CMediaPlayer::SetOnCompletionListener(
    /* [in] */ IMediaPlayerOnCompletionListener* listener)
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
ECode CMediaPlayer::SetOnBufferingUpdateListener(
    /* [in] */ IMediaPlayerOnBufferingUpdateListener* listener)
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
ECode CMediaPlayer::SetOnSeekCompleteListener(
    /* [in] */ IMediaPlayerOnSeekCompleteListener* listener)
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
ECode CMediaPlayer::SetOnVideoSizeChangedListener(
    /* [in] */ IMediaPlayerOnVideoSizeChangedListener* listener)
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
ECode CMediaPlayer::SetOnErrorListener(
    /* [in] */ IMediaPlayerOnErrorListener* listener)
{
    mOnErrorListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when an info/warning is available.
 *
 * @param listener the callback that will be run
 */
ECode CMediaPlayer::SetOnInfoListener(
    /* [in] */ IMediaPlayerOnInfoListener* listener)
{
    mOnInfoListener = listener;

    return NOERROR;
}
