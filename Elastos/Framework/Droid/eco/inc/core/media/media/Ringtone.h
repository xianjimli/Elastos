
#ifndef __RINGTONE_H__
#define __RINGTONE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * Ringtone provides a quick method for playing a ringtone, notification, or
 * other similar types of sounds.
 * <p>
 * For ways of retrieving {@link Ringtone} objects or to show a ringtone
 * picker, see {@link RingtoneManager}.
 * 
 * @see RingtoneManager
 */
class Ringtone 
{
public:
    Ringtone();

    Ringtone(
        /* [in] */ IContext* context);

    /**
     * Sets the stream type where this ringtone will be played.
     * 
     * @param streamType The stream, see {@link AudioManager}.
     */
    virtual CARAPI SetStreamType(
        /* [in] */ Int32 streamType);

    /**
     * Gets the stream type where this ringtone will be played.
     * 
     * @return The stream type, see {@link AudioManager}.
     */
    virtual CARAPI_(Int32) GetStreamType();

    /**
     * Returns a human-presentable title for ringtone. Looks in media and DRM
     * content providers. If not in either, uses the filename
     * 
     * @param context A context used for querying. 
     */
    virtual CARAPI_(String) GetTitle(
        /* [in] */ IContext* context);

    CARAPI_(void) Open(
        /* [in] */ IFileDescriptor* fd);

    CARAPI_(void) Open(
        /* [in] */ IAssetFileDescriptor* fd);

    CARAPI_(void) Open(
        /* [in] */ IUri* uri);
    
    /**
     * Plays the ringtone.
     */
    virtual CARAPI Play();

    /**
     * Stops a playing ringtone.
     */
    virtual CARAPI Stop();

    /**
     * Whether this ringtone is currently playing.
     * 
     * @return True if playing, FALSE otherwise.
     */
    virtual CARAPI_(Boolean) IsPlaying();

    CARAPI_(void) SetTitle(
        /* [in] */ String title);

private:
    static CARAPI_(String) GetTitle(
        /* [in] */ IContext* context, 
        /* [in] */ IUri* uri, 
        /* [in] */ Boolean followSettingsUri);

    CARAPI_(void) OpenMediaPlayer();

private:
    static String TAG;// = "Ringtone";

    static ArrayOf<String>* MEDIA_COLUMNS;/* = new String[] {
        MediaStore.Audio.Media._ID,
            MediaStore.Audio.Media.DATA,
            MediaStore.Audio.Media.TITLE
    };*/

    static ArrayOf<String>* DRM_COLUMNS;/* = new String[] {
        DrmStore.Audio._ID,
            DrmStore.Audio.DATA,
            DrmStore.Audio.TITLE
    };*/

    AutoPtr<IMediaPlayer> mAudio;

    AutoPtr<IUri> mUri;
    String mTitle;
    AutoPtr<IFileDescriptor> mFileDescriptor;
    AutoPtr<IAssetFileDescriptor> mAssetFileDescriptor;

    Int32 mStreamType;// = AudioManager.STREAM_RING;
    AutoPtr<IAudioManager> mAudioManager;

    AutoPtr<IContext> mContext;
};

#endif
