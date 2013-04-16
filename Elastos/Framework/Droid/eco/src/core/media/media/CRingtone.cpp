
#include "media/CRingtone.h"
#include "media/CMediaPlayer.h"
const CString CRingtone::TAG = "Ringtone";

CRingtone::CRingtone()
{

}

CRingtone::~CRingtone()
{

}

ECode CRingtone::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    mContext->GetSystemService(Context_AUDIO_SERVICE, (IInterface**)&mAudioManager);
    return NOERROR;
}

ECode CRingtone::SetStreamType(
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

ECode CRingtone::GetStreamType(
    /* [out] */ Int32* type)
{
    *type = mStreamType;
    return NOERROR;
}

ECode CRingtone::GetTitle(
    /* [in] */ IContext* context,
    /* [out] */ String* title)
{
    if (mTitle != NULL) 
    {
        *title = mTitle;
        return NOERROR;
    }
    mTitle = GetTitle(context, mUri, TRUE);
    *title = mTitle;
    return NOERROR;
}

String CRingtone::GetTitle(
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

void CRingtone::OpenMediaPlayer()
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

void CRingtone::Open(
    /* [in] */ IFileDescriptor* fd)
{
    mFileDescriptor = fd;
    OpenMediaPlayer();
}

void CRingtone::Open(
    /* [in] */ IAssetFileDescriptor* fd)
{
    mAssetFileDescriptor = fd;
    OpenMediaPlayer();
}

void CRingtone::Open(
    /* [in] */ IUri* uri)
{
    mUri = uri;
    OpenMediaPlayer();
}

ECode CRingtone::Play()
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

ECode CRingtone::Stop()
{
    if (mAudio != NULL) {
        mAudio->Reset();
        mAudio->Release();
        mAudio = NULL;
    }

    return NOERROR;
}

ECode CRingtone::IsPlaying(
    /* [out] */ Boolean* playing)
{
    VALIDATE_NOT_NULL(playing);
    *playing = mAudio != NULL/* && mAudio->IsPlaying()*/;
    return NOERROR;
}

void CRingtone::SetTitle(
    /* [in] */ String title)
{
    mTitle = title;
}