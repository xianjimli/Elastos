
#ifndef __MEDIAMETADATARETRIEVER_H__
#define __MEDIAMETADATARETRIEVER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * MediaMetadataRetriever class provides a unified interface for retrieving
 * frame and meta data from an input media file.
 */
class MediaMetadataRetriever
{
public:
    /*static {
        System.loadLibrary("media_jni");
        native_init();
    }*/

    MediaMetadataRetriever();

    /**
     * Sets the data source (file pathname) to use. Call this
     * method before the rest of the methods in this class. This method may be
     * time-consuming.
     * 
     * @param path The path of the input media file.
     * @throws IllegalArgumentException If the path is invalid.
     */
    virtual CARAPI_(void) SetDataSource(
        /* [in] */ String path);
    
    /**
     * Sets the data source (FileDescriptor) to use.  It is the caller's
     * responsibility to close the file descriptor. It is safe to do so as soon
     * as this call returns. Call this method before the rest of the methods in
     * this class. This method may be time-consuming.
     * 
     * @param fd the FileDescriptor for the file you want to play
     * @param offset the offset into the file where the data to be played starts,
     * in bytes. It must be non-negative
     * @param length the length in bytes of the data to be played. It must be
     * non-negative.
     * @throws IllegalArgumentException if the arguments are invalid
     */
    virtual CARAPI_(void) SetDataSource(
        /* [in] */ IFileDescriptor* fd, 
        /* [in] */ Int64 offset, 
        /* [in] */ Int64 length);
    
    /**
     * Sets the data source (FileDescriptor) to use. It is the caller's
     * responsibility to close the file descriptor. It is safe to do so as soon
     * as this call returns. Call this method before the rest of the methods in
     * this class. This method may be time-consuming.
     * 
     * @param fd the FileDescriptor for the file you want to play
     * @throws IllegalArgumentException if the FileDescriptor is invalid
     */
    virtual CARAPI SetDataSource(
        /* [in] */ IFileDescriptor* fd);
    
    /**
     * Sets the data source as a content Uri. Call this method before 
     * the rest of the methods in this class. This method may be time-consuming.
     * 
     * @param context the Context to use when resolving the Uri
     * @param uri the Content URI of the data you want to play
     * @throws IllegalArgumentException if the Uri is invalid
     * @throws SecurityException if the Uri cannot be used due to lack of
     * permission.
     */
    virtual CARAPI SetDataSource(
        /* [in] */ IContext* context, 
        /* [in] */ IUri* uri);

    /**
     * Call this method after setDataSource(). This method retrieves the 
     * meta data value associated with the keyCode.
     * 
     * The keyCode currently supported is listed below as METADATA_XXX
     * constants. With any other value, it returns a NULL pointer.
     * 
     * @param keyCode One of the constants listed below at the end of the class.
     * @return The meta data value associate with the given keyCode on success; 
     * NULL on failure.
     */
    virtual CARAPI_(String) ExtractMetadata(
        /* [in] */ Int32 keyCode);

    /**
     * Call this method after setDataSource(). This method finds a
     * representative frame close to the given time position by considering
     * the given option if possible, and returns it as a bitmap. This is
     * useful for generating a thumbnail for an input data source or just
     * obtain and display a frame at the given time position.
     *
     * @param timeUs The time position where the frame will be retrieved.
     * When retrieving the frame at the given time position, there is no
     * guarantee that the data source has a frame located at the position.
     * When this happens, a frame nearby will be returned. If timeUs is
     * negative, time position and option will ignored, and any frame
     * that the implementation considers as representative may be returned.
     *
     * @param option a hint on how the frame is found. Use
     * {@link #OPTION_PREVIOUS_SYNC} if one wants to retrieve a sync frame
     * that has a timestamp earlier than or the same as timeUs. Use
     * {@link #OPTION_NEXT_SYNC} if one wants to retrieve a sync frame
     * that has a timestamp later than or the same as timeUs. Use
     * {@link #OPTION_CLOSEST_SYNC} if one wants to retrieve a sync frame
     * that has a timestamp closest to or the same as timeUs. Use
     * {@link #OPTION_CLOSEST} if one wants to retrieve a frame that may
     * or may not be a sync frame but is closest to or the same as timeUs.
     * {@link #OPTION_CLOSEST} often has larger performance overhead compared
     * to the other options if there is no sync frame located at timeUs.
     *
     * @return A Bitmap containing a representative video frame, which 
     *         can be NULL, if such a frame cannot be retrieved.
     */
    virtual CARAPI_(AutoPtr<IBitmap>) GetFrameAtTime(
        /* [in] */ Int64 timeUs, 
        /* [in] */ Int32 option);

    /**
     * Call this method after setDataSource(). This method finds a
     * representative frame close to the given time position if possible,
     * and returns it as a bitmap. This is useful for generating a thumbnail
     * for an input data source. Call this method if one does not care
     * how the frame is found as Int64 as it is close to the given time;
     * otherwise, please call {@link #getFrameAtTime(Int64, Int32)}.
     *
     * @param timeUs The time position where the frame will be retrieved.
     * When retrieving the frame at the given time position, there is no
     * guarentee that the data source has a frame located at the position.
     * When this happens, a frame nearby will be returned. If timeUs is
     * negative, time position and option will ignored, and any frame
     * that the implementation considers as representative may be returned.
     *
     * @return A Bitmap containing a representative video frame, which
     *         can be NULL, if such a frame cannot be retrieved.
     *
     * @see #getFrameAtTime(Int64, Int32)
     */
    virtual CARAPI_(AutoPtr<IBitmap>) GetFrameAtTime(
        /* [in] */ Int64 timeUs);

    /**
     * Call this method after setDataSource(). This method finds a
     * representative frame at any time position if possible,
     * and returns it as a bitmap. This is useful for generating a thumbnail
     * for an input data source. Call this method if one does not
     * care about where the frame is located; otherwise, please call
     * {@link #getFrameAtTime(Int64)} or {@link #getFrameAtTime(Int64, Int32)}
     *
     * @return A Bitmap containing a representative video frame, which
     *         can be NULL, if such a frame cannot be retrieved.
     *
     * @see #getFrameAtTime(Int64)
     * @see #getFrameAtTime(Int64, Int32)
     */
    virtual CARAPI_(AutoPtr<IBitmap>) GetFrameAtTime();
    
    /**
     * Call this method after setDataSource(). This method finds the optional
     * graphic or album art associated associated with the data source. If
     * there are more than one pictures, (any) one of them is returned.
     * 
     * @return NULL if no such graphic is found.
     */
    virtual CARAPI_(ArrayOf<Byte>*) GetEmbeddedPicture();

    /**
     * Call it when one is done with the object. This method releases the memory
     * allocated internally.
     */
    virtual CARAPI_(void) ReleaseEx();

protected:
    virtual CARAPI_(void) Finalize();

private:
    CARAPI_(AutoPtr<IBitmap>) _getFrameAtTime(
        /* [in] */ Int64 timeUs, 
        /* [in] */ Int32 option);

    CARAPI_(ArrayOf<Byte>*) GetEmbeddedPicture(
        /* [in] */ Int32 pictureType);

    CARAPI_(void) native_setup();

    static CARAPI_(void) native_init();

    CARAPI_(void) native_finalize();

public:
    /**
     * Option used in method {@link #getFrameAtTime(Int64, Int32)} to get a
     * frame at a specified location.
     *
     * @see #getFrameAtTime(Int64, Int32)
     */
    /* Do not change these option values without updating their counterparts
     * in include/media/stagefright/MediaSource.h!
     */
    /**
     * This option is used with {@link #getFrameAtTime(Int64, Int32)} to retrieve
     * a sync (or key) frame associated with a data source that is located
     * right before or at the given time.
     *
     * @see #getFrameAtTime(Int64, Int32)
     */
    static const Int32 OPTION_PREVIOUS_SYNC    = 0x00;
    /**
     * This option is used with {@link #getFrameAtTime(Int64, Int32)} to retrieve
     * a sync (or key) frame associated with a data source that is located
     * right after or at the given time.
     *
     * @see #getFrameAtTime(Int64, Int32)
     */
    static const Int32 OPTION_NEXT_SYNC        = 0x01;
    /**
     * This option is used with {@link #getFrameAtTime(Int64, Int32)} to retrieve
     * a sync (or key) frame associated with a data source that is located
     * closest to (in time) or at the given time.
     *
     * @see #getFrameAtTime(Int64, Int32)
     */
    static const Int32 OPTION_CLOSEST_SYNC     = 0x02;
    /**
     * This option is used with {@link #getFrameAtTime(Int64, Int32)} to retrieve
     * a frame (not necessarily a key frame) associated with a data source that
     * is located closest to or at the given time.
     *
     * @see #getFrameAtTime(Int64, Int32)
     */
    static const Int32 OPTION_CLOSEST          = 0x03;

    /*
     * Do not change these metadata key values without updating their
     * counterparts in include/media/mediametadataretriever.h!
     */
    /**
     * The metadata key to retrieve the numberic string describing the
     * order of the audio data source on its original recording.
     */
    static const Int32 METADATA_KEY_CD_TRACK_NUMBER = 0;
    /**
     * The metadata key to retrieve the information about the album title
     * of the data source.
     */
    static const Int32 METADATA_KEY_ALBUM           = 1;
    /**
     * The metadata key to retrieve the information about the artist of
     * the data source.
     */
    static const Int32 METADATA_KEY_ARTIST          = 2;
    /**
     * The metadata key to retrieve the information about the author of
     * the data source.
     */
    static const Int32 METADATA_KEY_AUTHOR          = 3;
    /**
     * The metadata key to retrieve the information about the composer of
     * the data source.
     */
    static const Int32 METADATA_KEY_COMPOSER        = 4;
    /**
     * The metadata key to retrieve the date when the data source was created
     * or modified.
     */
    static const Int32 METADATA_KEY_DATE            = 5;
    /**
     * The metadata key to retrieve the content type or genre of the data
     * source.
     */
    static const Int32 METADATA_KEY_GENRE           = 6;
    /**
     * The metadata key to retrieve the data source title.
     */
    static const Int32 METADATA_KEY_TITLE           = 7;
    /**
     * The metadata key to retrieve the year when the data source was created
     * or modified.
     */
    static const Int32 METADATA_KEY_YEAR            = 8;
    /**
     * The metadata key to retrieve the playback duration of the data source.
     */
    static const Int32 METADATA_KEY_DURATION        = 9;
    /**
     * The metadata key to retrieve the number of tracks, such as audio, video,
     * text, in the data source, such as a mp4 or 3gpp file.
     */
    static const Int32 METADATA_KEY_NUM_TRACKS      = 10;
    /**
     * The metadata key to retrieve the information of the writer (such as
     * lyricist) of the data source.
     */
    static const Int32 METADATA_KEY_WRITER          = 11;
    /**
     * The metadata key to retrieve the mime type of the data source. Some
     * example mime types include: "video/mp4", "audio/mp4", "audio/amr-wb",
     * etc.
     */
    static const Int32 METADATA_KEY_MIMETYPE        = 12;
    /**
     * The metadata key to retrieve the information about the performers or
     * artist associated with the data source.
     */
    static const Int32 METADATA_KEY_ALBUMARTIST     = 13;
    /**
     * The metadata key to retrieve the numberic string that describes which
     * part of a set the audio data source comes from.
     */
    static const Int32 METADATA_KEY_DISC_NUMBER     = 14;
    /**
     * The metadata key to retrieve the music album compilation status.
     */
    static const Int32 METADATA_KEY_COMPILATION     = 15;
    // Add more here...

private:
    // The field below is accessed by native methods
    //@SuppressWarnings("unused")
    Int32 mNativeContext;

    static const Int32 EMBEDDED_PICTURE_TYPE_ANY = 0xFFFF;
};

#endif
