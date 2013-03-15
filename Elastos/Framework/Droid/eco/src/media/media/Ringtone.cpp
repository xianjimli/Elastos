
#include "media/Ringtone.h"
#include "media/CMediaPlayer.h"

Ringtone::Ringtone()
{

}

Ringtone::Ringtone(
    /* [in] */ IContext* context)
{
    mContext = context;
    mContext->GetSystemService(Context_AUDIO_SERVICE, (IInterface**)&mAudioManager);
}

/**
 * Sets the stream type where this ringtone will be played.
 * 
 * @param streamType The stream, see {@link AudioManager}.
 */
ECode Ringtone::SetStreamType(
    /* [in] */ Int32 streamType)
{
    mStreamType = streamType;
    
    if (mAudio != NULL) {
        /*
         * The stream type has to be set before the media player is
         * prepared. Re-initialize it.
         */
        //try {
            OpenMediaPlayer();
        /*} catch (IOException e) {
            Log.w(TAG, "Couldn't set the stream type", e);
        }*/
    }

    return NOERROR;
}

/**
 * Gets the stream type where this ringtone will be played.
 * 
 * @return The stream type, see {@link AudioManager}.
 */
Int32 Ringtone::GetStreamType()
{
    return mStreamType;
}

/**
 * Returns a human-presentable title for ringtone. Looks in media and DRM
 * content providers. If not in either, uses the filename
 * 
 * @param context A context used for querying. 
 */
String Ringtone::GetTitle(
    /* [in] */ IContext* context)
{
    if (mTitle != NULL) return mTitle;
    return mTitle = GetTitle(context, mUri, TRUE);
}

String Ringtone::GetTitle(
    /* [in] */ IContext* context, 
    /* [in] */ IUri* uri, 
    /* [in] */ Boolean followSettingsUri)
{
    AutoPtr<ICursor> cursor = NULL;
    AutoPtr<IContentResolver> res;
    context->GetContentResolver((IContentResolver**)&res);
    
    String title;// = NULL;

    /*if (uri != NULL) {
        String authority;
        uri->GetAuthority(&authority);

        if (Settings.AUTHORITY.equals(authority)) {
            if (followSettingsUri) {
                Uri actualUri = RingtoneManager.getActualDefaultRingtoneUri(context,
                        RingtoneManager.getDefaultType(uri));
                String actualTitle = getTitle(context, actualUri, FALSE);
                title = context
                        .getString(com.android.internal.R.string.ringtone_default_with_actual,
                                actualTitle);
            }
        } else {
            
            if (DrmStore.AUTHORITY.equals(authority)) {
                cursor = res.query(uri, DRM_COLUMNS, NULL, NULL, NULL);
            } else if (MediaStore.AUTHORITY.equals(authority)) {
                cursor = res.query(uri, MEDIA_COLUMNS, NULL, NULL, NULL);
            }
            
            try {
                if (cursor != NULL && cursor.getCount() == 1) {
                    cursor.moveToFirst();
                    return cursor.getString(2);
                } else {
                    title = uri.getLastPathSegment();
                }
            } finally {
                if (cursor != NULL) {
                    cursor.close();
                }
            }
        }
    }

    if (title == NULL) {
        title = context.getString(com.android.internal.R.string.ringtone_unknown);
        
        if (title == NULL) {
            title = "";
        }
    }*/
    
    return title;
}

void Ringtone::OpenMediaPlayer() 
{
    CMediaPlayer::New((IMediaPlayer**)&mAudio);
    if (mUri != NULL) {
        mAudio->SetDataSource(mContext, mUri);
    } else if (mFileDescriptor != NULL) {
        //mAudio->SetDataSource(mFileDescriptor);
    } else if (mAssetFileDescriptor != NULL) {
        // Note: using getDeclaredLength so that our behavior is the same
        // as previous versions when the content provider is returning
        // a full file.
        /*if (mAssetFileDescriptor.getDeclaredLength() < 0) {
            mAudio->SetDataSource(mAssetFileDescriptor.getFileDescriptor());
        } else {
            mAudio.setDataSource(mAssetFileDescriptor.getFileDescriptor(),
                    mAssetFileDescriptor.getStartOffset(),
                    mAssetFileDescriptor.getDeclaredLength());
        }*/
    } else {
        //throw new IOException("No data source set.");
    }
    /*mAudio->SetAudioStreamType(mStreamType);
    mAudio->Prepare();*/
}

void Ringtone::Open(
    /* [in] */ IFileDescriptor* fd)
{
    mFileDescriptor = fd;
    OpenMediaPlayer();
}

void Ringtone::Open(
    /* [in] */ IAssetFileDescriptor* fd)
{
    mAssetFileDescriptor = fd;
    OpenMediaPlayer();
}

void Ringtone::Open(
    /* [in] */ IUri* uri)
{
    mUri = uri;
    OpenMediaPlayer();
}

/**
 * Plays the ringtone.
 */
ECode Ringtone::Play()
{
    if (mAudio == NULL) {
        //try {
            OpenMediaPlayer();
        /*} catch (Exception ex) {
            Log.e(TAG, "play() caught ", ex);
            mAudio = NULL;
        }*/
    }
    if (mAudio != NULL) {
        // do not ringtones if stream volume is 0
        // (typically because ringer mode is silent).
        Int32 vol;
        mAudioManager->GetStreamVolume(mStreamType, &vol);
        if (vol != 0) {
            mAudio->Start();
        }
    }

    return NOERROR;
}

/**
 * Stops a playing ringtone.
 */
ECode Ringtone::Stop()
{
    if (mAudio != NULL) {
        mAudio->Reset();
        mAudio->Release();
        mAudio = NULL;
    }

    return NOERROR;
}

/**
 * Whether this ringtone is currently playing.
 * 
 * @return True if playing, FALSE otherwise.
 */
Boolean Ringtone::IsPlaying()
{
    return mAudio != NULL && ((CMediaPlayer*)mAudio.Get())->IsPlaying();
}

void Ringtone::SetTitle(
    /* [in] */ String title) 
{
    mTitle = title;
}
