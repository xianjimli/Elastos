
#include "provider/MediaStore.h" 
#include "content/ContentUris.h"
#include "media/media/MiniThumbFile.h"
#include "net/Uri.h"
#include "media/media/ThumbnailUtils.h"
#include "database/DatabaseUtils.h"
#include "os/Environment.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CMatrix.h"
#include "content/CContentValues.h"

const AutoPtr<IUri> MediaStore::Images::Media::INTERNAL_CONTENT_URI;
const AutoPtr<IUri> MediaStore::Images::Media::EXTERNAL_CONTENT_URI;
const CString MediaStore::Images::Media::CONTENT_TYPE = "vnd.android.cursor.dir/image";
const CString MediaStore::Images::Media::DEFAULT_SORT_ORDER = ImageColumns_BUCKET_DISPLAY_NAME;
const AutoPtr<IUri> MediaStore::Images::Thumbnails::INTERNAL_CONTENT_URI;
const AutoPtr<IUri> MediaStore::Images::Thumbnails::EXTERNAL_CONTENT_URI;
const CString MediaStore::Images::Thumbnails::DEFAULT_SORT_ORDER = "image_id ASC";
const CString MediaStore::Images::Thumbnails::DATA = "_data";
const CString MediaStore::Images::Thumbnails::IMAGE_ID = "image_id";

const CString MediaStore::Images::Thumbnails::KIND = "kind";

const Int32 MediaStore::Images::Thumbnails::MINI_KIND;// = 1;
const Int32 MediaStore::Images::Thumbnails::FULL_SCREEN_KIND;// = 2;
const Int32 MediaStore::Images::Thumbnails::MICRO_KIND;// = 3;
/**
 * The blob raw data of thumbnail
 * <P>Type: DATA STREAM</P>
 */
const CString MediaStore::Images::Thumbnails::THUMB_DATA = "thumb_data";

/**
 * The width of the thumbnal
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Images::Thumbnails::WIDTH = "width";

/**
 * The height of the thumbnail
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Images::Thumbnails::HEIGHT = "height";

/**
 * The content:// style URI for the internal storage.
 */
const AutoPtr<IUri> MediaStore::Audio::Media::INTERNAL_CONTENT_URI = GetContentUri(String("internal"));

/**
 * The content:// style URI for the "primary" external storage
 * volume.
 */
const AutoPtr<IUri> MediaStore::Audio::Media::EXTERNAL_CONTENT_URI = GetContentUri(String("external"));

/**
 * The MIME type for this table.
 */
const CString MediaStore::Audio::Media::CONTENT_TYPE = "vnd.android.cursor.dir/audio";

/**
 * The default sort order for this table
 */
const CString MediaStore::Audio::Media::DEFAULT_SORT_ORDER = AudioColumns_TITLE_KEY;

/**
 * Activity Action: Start SoundRecorder application.
 * <p>Input: nothing.
 * <p>Output: An uri to the recorded sound stored in the Media Library
 * if the recording was successful.
 * May also contain the extra EXTRA_MAX_BYTES.
 * @see #EXTRA_MAX_BYTES
 */
const CString MediaStore::Audio::Media::RECORD_SOUND_ACTION = "android.provider.MediaStore.RECORD_SOUND";

/**
 * The name of the Intent-extra used to define a maximum file size for
 * a recording made by the SoundRecorder application.
 *
 * @see #RECORD_SOUND_ACTION
 */
const CString MediaStore::Audio::Media::EXTRA_MAX_BYTES = "android.provider.MediaStore.extra.MAX_BYTES";


/**
 * The content:// style URI for the internal storage.
 */
const AutoPtr<IUri> MediaStore::Audio::Genres::INTERNAL_CONTENT_URI = GetContentUri(String("internal"));

/**
 * The content:// style URI for the "primary" external storage
 * volume.
 */
const AutoPtr<IUri> MediaStore::Audio::Genres::EXTERNAL_CONTENT_URI = GetContentUri(String("external"));

/**
 * The MIME type for this table.
 */
const CString MediaStore::Audio::Genres::CONTENT_TYPE = "vnd.android.cursor.dir/genre";

/**
 * The MIME type for entries in this table.
 */
const CString MediaStore::Audio::Genres::ENTRY_CONTENT_TYPE = "vnd.android.cursor.item/genre";

/**
 * The default sort order for this table
 */
const CString MediaStore::Audio::Genres::DEFAULT_SORT_ORDER = GenresColumns_NAME;



/**
 * A subdirectory of each genre containing all member audio files.
 */
const CString MediaStore::Audio::Genres::Members::CONTENT_DIRECTORY = "members";

/**
 * The default sort order for this table
 */
const CString MediaStore::Audio::Genres::Members::DEFAULT_SORT_ORDER = AudioColumns_TITLE_KEY;

/**
 * The ID of the audio file
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Audio::Genres::Members::AUDIO_ID = "audio_id";

/**
 * The ID of the genre
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Audio::Genres::Members::GENRE_ID = "genre_id";

/**
 * The content:// style URI for the internal storage.
 */
const AutoPtr<IUri> MediaStore::Audio::Playlists::INTERNAL_CONTENT_URI = GetContentUri(String("internal"));

/**
 * The content:// style URI for the "primary" external storage
 * volume.
 */
const AutoPtr<IUri> MediaStore::Audio::Playlists::EXTERNAL_CONTENT_URI = GetContentUri(String("external"));

/**
 * The MIME type for this table.
 */
const CString MediaStore::Audio::Playlists::CONTENT_TYPE = "vnd.android.cursor.dir/playlist";

/**
 * The MIME type for entries in this table.
 */
const CString MediaStore::Audio::Playlists::ENTRY_CONTENT_TYPE = "vnd.android.cursor.item/playlist";

/**
 * The default sort order for this table
 */
const CString MediaStore::Audio::Playlists::DEFAULT_SORT_ORDER = PlaylistsColumns_NAME;

 
/**
 * The ID within the playlist.
 */
const CString MediaStore::Audio::Playlists::Members::_ID = "_id";

/**
 * A subdirectory of each playlist containing all member audio
 * files.
 */
const CString MediaStore::Audio::Playlists::Members::CONTENT_DIRECTORY = "members";

/**
 * The ID of the audio file
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Audio::Playlists::Members::AUDIO_ID = "audio_id";

/**
 * The ID of the playlist
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Audio::Playlists::Members::PLAYLIST_ID = "playlist_id";

/**
 * The order of the songs in the playlist
 * <P>Type: INTEGER (Int64)></P>
 */
const CString MediaStore::Audio::Playlists::Members::PLAY_ORDER = "play_order";

/**
 * The default sort order for this table
 */
const CString MediaStore::Audio::Playlists::Members::DEFAULT_SORT_ORDER = PLAY_ORDER;

/**
 * The content:// style URI for the internal storage.
 */
const AutoPtr<IUri> MediaStore::Audio::Artists::INTERNAL_CONTENT_URI = GetContentUri(String("internal"));

/**
 * The content:// style URI for the "primary" external storage
 * volume.
 */
const AutoPtr<IUri> MediaStore::Audio::Artists::EXTERNAL_CONTENT_URI = GetContentUri(String("external"));

/**
 * The MIME type for this table.
 */
const CString MediaStore::Audio::Artists::CONTENT_TYPE = "vnd.android.cursor.dir/artists";

/**
 * The MIME type for entries in this table.
 */
const CString MediaStore::Audio::Artists::ENTRY_CONTENT_TYPE = "vnd.android.cursor.item/artist";

/**
 * The default sort order for this table
 */
const CString MediaStore::Audio::Artists::DEFAULT_SORT_ORDER = ArtistColumns_ARTIST_KEY;


/**
 * The content:// style URI for the internal storage.
 */
const AutoPtr<IUri> MediaStore::Audio::Albums::INTERNAL_CONTENT_URI = GetContentUri(String("internal"));

/**
 * The content:// style URI for the "primary" external storage
 * volume.
 */
const AutoPtr<IUri> MediaStore::Audio::Albums::EXTERNAL_CONTENT_URI = GetContentUri(String("external"));

/**
 * The MIME type for this table.
 */
const CString MediaStore::Audio::Albums::CONTENT_TYPE = "vnd.android.cursor.dir/albums";

/**
 * The MIME type for entries in this table.
 */
const CString MediaStore::Audio::Albums::ENTRY_CONTENT_TYPE = "vnd.android.cursor.item/album";

/**
 * The default sort order for this table
 */
const CString MediaStore::Audio::Albums::DEFAULT_SORT_ORDER = AlbumColumns_ALBUM_KEY;

const CString MediaStore::Video::DEFAULT_SORT_ORDER = MediaColumns_DISPLAY_NAME;


/**
 * The content:// style URI for the internal storage.
 */
const AutoPtr<IUri> MediaStore::Video::Media::INTERNAL_CONTENT_URI = GetContentUri(String("internal"));

/**
 * The content:// style URI for the "primary" external storage
 * volume.
 */
const AutoPtr<IUri> MediaStore::Video::Media::EXTERNAL_CONTENT_URI = GetContentUri(String("external"));

/**
 * The MIME type for this table.
 */
const CString MediaStore::Video::Media::CONTENT_TYPE = "vnd.android.cursor.dir/video";

/**
 * The default sort order for this table
 */
const CString MediaStore::Video::Media::DEFAULT_SORT_ORDER = MediaColumns_TITLE;


/**
 * The content:// style URI for the internal storage.
 */
const AutoPtr<IUri> MediaStore::Video::Thumbnails::INTERNAL_CONTENT_URI = GetContentUri(String("internal"));

/**
 * The content:// style URI for the "primary" external storage
 * volume.
 */
const AutoPtr<IUri> MediaStore::Video::Thumbnails::EXTERNAL_CONTENT_URI = GetContentUri(String("external"));

/**
 * The default sort order for this table
 */
const CString MediaStore::Video::Thumbnails::DEFAULT_SORT_ORDER = "video_id ASC";

/**
 * The data stream for the thumbnail
 * <P>Type: DATA STREAM</P>
 */
const CString MediaStore::Video::Thumbnails::DATA = "_data";

/**
 * The original image for the thumbnal
 * <P>Type: INTEGER (ID from Video table)</P>
 */
const CString MediaStore::Video::Thumbnails::VIDEO_ID = "video_id";

/**
 * The kind of the thumbnail
 * <P>Type: INTEGER (One of the values below)</P>
 */
const CString MediaStore::Video::Thumbnails::KIND = "kind";

const Int32 MediaStore::Video::Thumbnails::MINI_KIND;// = 1;
const Int32 MediaStore::Video::Thumbnails::FULL_SCREEN_KIND;// = 2;
const Int32 MediaStore::Video::Thumbnails::MICRO_KIND;// = 3;

/**
 * The width of the thumbnal
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Video::Thumbnails::WIDTH = "width";

/**
 * The height of the thumbnail
 * <P>Type: INTEGER (Int64)</P>
 */
const CString MediaStore::Video::Thumbnails::HEIGHT = "height";


const Int32 MediaStore::InternalThumbnails::DEFAULT_GROUP_ID;// = 0;

const Int32 MediaStore::InternalThumbnails::MINI_KIND;// = 1;
const Int32 MediaStore::InternalThumbnails::FULL_SCREEN_KIND;// = 2;
const Int32 MediaStore::InternalThumbnails::MICRO_KIND;// = 3;
//const ArrayOf_<CString, 2> MediaStore::InternalThumbnails::PROJECTION[2];// = {"_id", "_data"};


const CString MediaStore::AUTHORITY = "media";
    
/**
 * Activity Action: Launch a music player.
 * The activity should be able to play, browse, or manipulate music files stored on the device.
 */
//@SdkConstant(SdkConstantType.ACTIVITY_INTENT_ACTION)
const CString MediaStore::INTENT_ACTION_MUSIC_PLAYER = "android.intent.action.MUSIC_PLAYER";

/**
 * Activity Action: Perform a search for media.
 * Contains at least the {@link android.app.SearchManager#QUERY} extra.
 * May also contain any combination of the following extras:
 * EXTRA_MEDIA_ARTIST, EXTRA_MEDIA_ALBUM, EXTRA_MEDIA_TITLE, EXTRA_MEDIA_FOCUS
 *
 * @see android.provider.MediaStore#EXTRA_MEDIA_ARTIST
 * @see android.provider.MediaStore#EXTRA_MEDIA_ALBUM
 * @see android.provider.MediaStore#EXTRA_MEDIA_TITLE
 * @see android.provider.MediaStore#EXTRA_MEDIA_FOCUS
 */
//@SdkConstant(SdkConstantType.ACTIVITY_INTENT_ACTION)
const CString MediaStore::INTENT_ACTION_MEDIA_SEARCH = "android.intent.action.MEDIA_SEARCH";

/**
 * An intent to perform a search for music media and automatically play content from the
 * result when possible. This can be fired, for example, by the result of a voice recognition
 * command to listen to music.
 * <p>
 * Contains the {@link android.app.SearchManager#QUERY} extra, which is a string
 * that can contain any type of unstructured music search, like the name of an artist,
 * an album, a song, a genre, or any combination of these.
 * <p>
 * Because this intent includes an open-ended unstructured search string, it makes the most
 * sense for apps that can support large-scale search of music, such as services connected
 * to an online database of music which can be streamed and played on the device.
 */
const CString MediaStore::INTENT_ACTION_MEDIA_PLAY_FROM_SEARCH =
        "android.media.action.MEDIA_PLAY_FROM_SEARCH";

/**
 * The name of the Intent-extra used to define the artist
 */
const CString MediaStore::EXTRA_MEDIA_ARTIST = "android.intent.extra.artist";

/**
 * The name of the Intent-extra used to define the album
 */
const CString MediaStore::EXTRA_MEDIA_ALBUM = "android.intent.extra.album";

/**
 * The name of the Intent-extra used to define the song title
 */
const CString MediaStore::EXTRA_MEDIA_TITLE = "android.intent.extra.title";

/**
 * The name of the Intent-extra used to define the search focus. The search focus
 * indicates whether the search should be for things related to the artist, album
 * or song that is identified by the other extras.
 */
const CString MediaStore::EXTRA_MEDIA_FOCUS = "android.intent.extra.focus";

/**
 * The name of the Intent-extra used to control the orientation of a ViewImage or a MovieView.
 * This is an Int32 property that overrides the activity's requestedOrientation.
 * @see android.content.pm.ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED
 */
const CString MediaStore::EXTRA_SCREEN_ORIENTATION = "android.intent.extra.screenOrientation";

/**
 * The name of an Intent-extra used to control the UI of a ViewImage.
 * This is a boolean property that overrides the activity's default fullscreen state.
 */
const CString MediaStore::EXTRA_FULL_SCREEN = "android.intent.extra.fullScreen";

/**
 * The name of an Intent-extra used to control the UI of a ViewImage.
 * This is a boolean property that specifies whether or not to show action icons.
 */
const CString MediaStore::EXTRA_SHOW_ACTION_ICONS = "android.intent.extra.showActionIcons";

/**
 * The name of the Intent-extra used to control the onCompletion behavior of a MovieView.
 * This is a boolean property that specifies whether or not to finish the MovieView activity
 * when the movie completes playing. The default value is true, which means to automatically
 * exit the movie player activity when the movie completes playing.
 */
const CString MediaStore::EXTRA_FINISH_ON_COMPLETION = "android.intent.extra.finishOnCompletion";

/**
 * The name of the Intent action used to launch a camera in still image mode.
 */
const CString MediaStore::INTENT_ACTION_STILL_IMAGE_CAMERA = "android.media.action.STILL_IMAGE_CAMERA";

/**
 * The name of the Intent action used to launch a camera in video mode.
 */
const CString MediaStore::INTENT_ACTION_VIDEO_CAMERA = "android.media.action.VIDEO_CAMERA";

/**
 * Standard Intent action that can be sent to have the camera application
 * capture an image and return it.
 * <p>
 * The caller may pass an extra EXTRA_OUTPUT to control where this image will be written.
 * If the EXTRA_OUTPUT is not present, then a small sized image is returned as a Bitmap
 * object in the extra field. This is useful for applications that only need a small image.
 * If the EXTRA_OUTPUT is present, then the full-sized image will be written to the Uri
 * value of EXTRA_OUTPUT.
 * @see #EXTRA_OUTPUT
 * @see #EXTRA_VIDEO_QUALITY
 */
const CString MediaStore::ACTION_IMAGE_CAPTURE = "android.media.action.IMAGE_CAPTURE";

/**
 * Standard Intent action that can be sent to have the camera application
 * capture an video and return it.
 * <p>
 * The caller may pass in an extra EXTRA_VIDEO_QUALITY to control the video quality.
 * <p>
 * The caller may pass in an extra EXTRA_OUTPUT to control
 * where the video is written. If EXTRA_OUTPUT is not present the video will be
 * written to the standard location for videos, and the Uri of that location will be
 * returned in the data field of the Uri.
 * @see #EXTRA_OUTPUT
 */
const CString MediaStore::ACTION_VIDEO_CAPTURE = "android.media.action.VIDEO_CAPTURE";

/**
 * The name of the Intent-extra used to control the quality of a recorded video. This is an
 * integer property. Currently value 0 means low quality, suitable for MMS messages, and
 * value 1 means high quality. In the future other quality levels may be added.
 */
const CString MediaStore::EXTRA_VIDEO_QUALITY = "android.intent.extra.videoQuality";

/**
 * Specify the maximum allowed size.
 */
const CString MediaStore::EXTRA_SIZE_LIMIT = "android.intent.extra.sizeLimit";

/**
 * Specify the maximum allowed recording duration in seconds.
 */
const CString MediaStore::EXTRA_DURATION_LIMIT = "android.intent.extra.durationLimit";

/**
 * The name of the Intent-extra used to indicate a content resolver Uri to be used to
 * store the requested image or video.
 */
const CString MediaStore::EXTRA_OUTPUT = "output";

/**
  * The string that is used when a media attribute is not known. For example,
  * if an audio file does not have any meta data, the artist and album columns
  * will be set to this value.
  */
const CString MediaStore::UNKNOWN_STRING = "<unknown>";

/**
 * Name of current volume being scanned by the media scanner.
 */
const CString MediaStore::MEDIA_SCANNER_VOLUME = "volume";

/**
 * Name of the file signaling the media scanner to ignore media in the containing directory
 * and its subdirectories. Developers should use this to avoid application graphics showing
 * up in the Gallery and likewise prevent application sounds and music from showing up in
 * the Music app.
 */
const CString MediaStore::MEDIA_IGNORE_FILENAME = ".nomedia";

const CString MediaStore::TAG = "MediaStore";

const String MediaStore::CONTENT_AUTHORITY_SLASH = String("content://") + AUTHORITY + String("/");


AutoPtr<IBitmap> MediaStore::InternalThumbnails::GetMiniThumbFromFile(
    /* [in] */ ICursor* c, 
    /* [in] */ IUri* baseUri, 
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IBitmapFactoryOptions* options)
{
    AutoPtr<IBitmap> bitmap = NULL;
    AutoPtr<IUri> thumbUri = NULL;
    //try {
        Int64 thumbId;
        c->GetInt64(0, &thumbId);

        String filePath;
        c->GetString(1, &filePath);

        thumbUri = ContentUris::WithAppendedId(baseUri, thumbId);

        AutoPtr<IParcelFileDescriptor> pfdInput;
        cr->OpenFileDescriptor(thumbUri, String("r"), (IParcelFileDescriptor**)&pfdInput);

        AutoPtr<IBitmapFactory> factory;
        ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
            (IBitmapFactory**)&factory));

        AutoPtr<IFileDescriptor> fd;
        pfdInput->GetFileDescriptor((IFileDescriptor**)&fd);

        factory->DecodeFileDescriptor(
                fd, NULL, options, (IBitmap**)&bitmap);

        pfdInput->Close();

    /*} catch (FileNotFoundException ex) {
        Log.e(TAG, "couldn't open thumbnail " + thumbUri + "; " + ex);
    } catch (IOException ex) {
        Log.e(TAG, "couldn't open thumbnail " + thumbUri + "; " + ex);
    } catch (OutOfMemoryError ex) {
        Log.e(TAG, "failed to allocate memory for thumbnail "
                + thumbUri + "; " + ex);
    }*/

    return bitmap;
}

/**
 * This method cancels the thumbnail request so clients waiting for getThumbnail will be
 * interrupted and return immediately. Only the original process which made the getThumbnail
 * requests can cancel their own requests.
 *
 * @param cr ContentResolver
 * @param origId original image or video id. use -1 to cancel all requests.
 * @param groupId the same groupId used in getThumbnail
 * @param baseUri the base URI of requested thumbnails
 */
void MediaStore::InternalThumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ IUri* baseUri,
    /* [in] */ Int64 groupId)
{
    AutoPtr<IUri> cancelUri;

    AutoPtr<IUriBuilder> ub;
    baseUri->BuildUpon((IUriBuilder**)&ub);
    
    ub->AppendQueryParameter(String("cancel"), String("1"));
    ub->AppendQueryParameter(String("orig_id"), String::FromInt64(origId));
    ub->AppendQueryParameter(String("group_id"), String::FromInt64(groupId));
    ub->Build((IUri**)&cancelUri);
    
    AutoPtr<ICursor> c = NULL;
    //try {

        cr->Query(cancelUri, *PROJECTION, String(""), *PROJECTION, String(""), (ICursor**)&c);
    //}
    //finally {
        if (c != NULL) c->Close();
    //}
}

/**
 * This method ensure thumbnails associated with origId are generated and decode the byte
 * stream from database (MICRO_KIND) or file (MINI_KIND).
 *
 * Special optimization has been done to avoid further IPC communication for MICRO_KIND
 * thumbnails.
 *
 * @param cr ContentResolver
 * @param origId original image or video id
 * @param kind could be MINI_KIND or MICRO_KIND
 * @param options this is only used for MINI_KIND when decoding the Bitmap
 * @param baseUri the base URI of requested thumbnails
 * @param groupId the id of group to which this request belongs
 * @return Bitmap bitmap of specified thumbnail kind
 */
AutoPtr<IBitmap> MediaStore::InternalThumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int64 groupId, 
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options, 
    /* [in] */ IUri* baseUri, 
    /* [in] */ Boolean isVideo)
{
    AutoPtr<IBitmap> bitmap = NULL;
    String filePath;
    // Log.v(TAG, "getThumbnail: origId="+origId+", kind="+kind+", isVideo="+isVideo);
    // If the magic is non-zero, we simply return thumbnail if it does exist.
    // querying MediaProvider and simply return thumbnail.
    AutoPtr<IMiniThumbFile> thumbFile;
    //CMiniThumbFile::New(baseUri, (IMiniThumbFile**)&thumbFile);
    Int64 magic;
    thumbFile->GetMagic(origId, &magic);
    if (magic != 0) {
        if (kind == MICRO_KIND) {
            //synchronized (sThumbBufLock) {
                if (sThumbBuf == NULL) {
                    sThumbBuf = ArrayOf<Byte>::Alloc(MiniThumbFile::BYTES_PER_MINTHUMB);
                }
                ArrayOf<Byte>* miniThumb;
                thumbFile->GetMiniThumbFromFile(origId, *sThumbBuf, &miniThumb);
                if (miniThumb != NULL) {

                    AutoPtr<IBitmapFactory> factory;
                    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
                        (IBitmapFactory**)&factory));

                    factory->DecodeByteArrayEx(*sThumbBuf, 0, sThumbBuf->GetLength(), (IBitmap**)&bitmap);
                    if (bitmap == NULL) {
                        //Log.w(TAG, "couldn't decode byte array.");
                    }
                }
            //}
            return bitmap;
        } else if (kind == MINI_KIND) {
            String column = isVideo ? String("video_id=") : String("image_id=");
            AutoPtr<ICursor> c = NULL;
            //try {
                //cr->Query(baseUri, PROJECTION, column + origId, NULL, NULL, (ICursor**)&c);

                Boolean bSucceeded;
                if (c != NULL && (c->MoveToFirst(&bSucceeded), bSucceeded)) {
                    bitmap = GetMiniThumbFromFile(c, baseUri, cr, options);
                    if (bitmap != NULL) {
                        return bitmap;
                    }
                }
            //} finally {
                if (c != NULL) c->Close();
            //}
        }
    }

    AutoPtr<ICursor> c = NULL;
    //try {
        AutoPtr<IUri> blockingUri;

        AutoPtr<IUriBuilder> ub;
        baseUri->BuildUpon((IUriBuilder**)&ub);

        ub->AppendQueryParameter(String("blocking"), String("1"));
        ub->AppendQueryParameter(String("orig_id"), String::FromInt64(origId));
        ub->AppendQueryParameter(String("group_id"), String::FromInt64(groupId));
        ub->Build((IUri**)&blockingUri);


        //cr->Query(blockingUri, PROJECTION, NULL, NULL, NULL, (ICursor**)&c);
        // This happens when original image/video doesn't exist.
        if (c == NULL) return NULL;

        // Assuming thumbnail has been generated, at least original image exists.
        if (kind == MICRO_KIND) {
            //synchronized (sThumbBufLock) {
                if (sThumbBuf == NULL) {
                    sThumbBuf = ArrayOf<Byte>::Alloc(MiniThumbFile::BYTES_PER_MINTHUMB);
                }

                ArrayOf<Byte>* miniThumb;
                thumbFile->GetMiniThumbFromFile(origId, *sThumbBuf, &miniThumb);

                if (miniThumb != NULL) {

                    AutoPtr<IBitmapFactory> factory;
                    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
                        (IBitmapFactory**)&factory));

                    factory->DecodeByteArrayEx(*sThumbBuf, 0, sThumbBuf->GetLength(), (IBitmap**)&bitmap);

                    if (bitmap == NULL) {
                        //Log.w(TAG, "couldn't decode byte array.");
                    }
                }
            //}
        } else if (kind == MINI_KIND) {
            Boolean bSucceeded;
            if ((c->MoveToFirst(&bSucceeded), bSucceeded)) {
                bitmap = GetMiniThumbFromFile(c, baseUri, cr, options);
            }
        } else {
            //throw new IllegalArgumentException("Unsupported kind: " + kind);
        }

        // We probably run out of space, so create the thumbnail in memory.
        if (bitmap == NULL) {
            /*Log.v(TAG, "Create the thumbnail in memory: origId=" + origId
                    + ", kind=" + kind + ", isVideo="+isVideo);*/
            AutoPtr<IUri> uri;
            
            AutoPtr<IUriBuilder> ub;
            baseUri->BuildUpon((IUriBuilder**)&ub);
            
            ub->AppendPath(String::FromInt64(origId));

            String str;
            //ub->ToString(&str);

            //str.ReplaceFirst("thumbnails", "media");
            Uri::Parse(str, (IUri**)&uri);
            if (filePath == NULL) {
                if (c != NULL) c->Close();
                //cr->Query(uri, PROJECTION, NULL, NULL, NULL, (ICursor**)&c);

                Boolean bSucceeded;
                if (c == NULL || !(c->MoveToFirst(&bSucceeded), bSucceeded)) {
                    return NULL;
                }
                c->GetString(1, &filePath);
            }
            if (isVideo) {
                bitmap = ThumbnailUtils::CreateVideoThumbnail(filePath, kind);
            } else {
                bitmap = ThumbnailUtils::CreateImageThumbnail(filePath, kind);
            }
        }
    //} catch (SQLiteException ex) {
        //Log.w(TAG, ex);
    //} finally {
        if (c != NULL) c->Close();
    //}
    return bitmap;
}
 











AutoPtr<ICursor> MediaStore::Images::Media::Query(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IUri* uri, 
    /* [in] */ ArrayOf<String>* projection)
{
    AutoPtr<ICursor> c;
    //cr->Query(uri, projection, NULL, NULL, DEFAULT_SORT_ORDER, (ICursor**)&c);

    return c;
}

AutoPtr<ICursor> MediaStore::Images::Media::Query(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IUri* uri, 
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ String where, 
    /* [in] */ String orderBy) 
{
    AutoPtr<ICursor> c;
    /*cr->Query(uri, projection, where,
        NULL, orderBy == NULL ? DEFAULT_SORT_ORDER : orderBy, (ICursor**)&c);*/

    return c;
}

AutoPtr<ICursor> MediaStore::Images::Media::Query(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IUri* uri, 
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ String selection, 
    /* [in] */ ArrayOf<String>* selectionArgs, 
    /* [in] */ String orderBy)
{
    AutoPtr<ICursor> c;
    /*cr->Query(uri, projection, selection,
            selectionArgs, orderBy == NULL ? DEFAULT_SORT_ORDER : orderBy, (ICursor**)&c);*/

    return c;
}

/**
 * Retrieves an image for the given url as a {@link Bitmap}.
 *
 * @param cr The content resolver to use
 * @param url The url of the image
 * @throws FileNotFoundException
 * @throws IOException
 */
AutoPtr<IBitmap> MediaStore::Images::Media::GetBitmap(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IUri* url)
{
    AutoPtr<IInputStream> input;
    cr->OpenInputStream(url, (IInputStream**)&input);

    AutoPtr<IBitmapFactory> factory;
    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
        (IBitmapFactory**)&factory));

    AutoPtr<IBitmap> bitmap;
    factory->DecodeStreamEx(input, (IBitmap**)&bitmap);
    
    input->Close();
    return bitmap;
}

/**
 * Insert an image and create a thumbnail for it.
 *
 * @param cr The content resolver to use
 * @param imagePath The path to the image to insert
 * @param name The name of the image
 * @param description The description of the image
 * @return The URL to the newly created image
 * @throws FileNotFoundException
 */
String MediaStore::Images::Media::InsertImage(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ String imagePath,
    /* [in] */ String name, 
    /* [in] */ String description)
{
    // Check if file exists with a FileInputStream
    AutoPtr<IFileInputStream> stream;
    CFileInputStream::New(imagePath, (IFileInputStream**)&stream);
    //try {

        AutoPtr<IBitmapFactory> factory;
        ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
            (IBitmapFactory**)&factory));
        AutoPtr<IBitmap> bm;
        factory->DecodeFileEx(imagePath, (IBitmap**)&bm);

        String ret = InsertImage(cr, bm, name, description);
        bm->Recycle();
        
    //} finally {
        //try {
            stream->Close();
        /*} catch (IOException e) {
        }*/
    //}

            return ret;
}

AutoPtr<IBitmap> MediaStore::Images::Media::StoreThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IBitmap* source,
    /* [in] */ Int64 id,
    /* [in] */ Float width, 
    /* [in] */ Float height,
    /* [in] */ Int32 kind)
{
    // create the matrix to scale it
    AutoPtr<IMatrix> matrix;
    CMatrix::New((IMatrix**)&matrix);
    

    Int32 w, h;
    source->GetWidth(&w);
    source->GetHeight(&h);

    Float scaleX = width / w;
    Float scaleY = height / h;

    matrix->SetScaleEx(scaleX, scaleY);

    AutoPtr<IBitmapFactory> factory;
    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
        (IBitmapFactory**)&factory));

    AutoPtr<IBitmap> thumb;
    factory->CreateBitmapEx2(source, 0, 0,
                                       w,
                                       h, matrix,
                                       TRUE, (IBitmap**)&thumb);

    AutoPtr<IContentValues> values;
    CContentValues::New(/*4, */(IContentValues**)&values);
    values->PutInt32(String(Images::Thumbnails::KIND),     kind);
    values->PutInt32(String(Images::Thumbnails::IMAGE_ID), (Int32)id);
    values->PutInt32(String(Images::Thumbnails::HEIGHT),   (thumb->GetHeight(&h), h));
    values->PutInt32(String(Images::Thumbnails::WIDTH),    (thumb->GetWidth(&w), w));

    AutoPtr<IUri> url;
    cr->Insert(Images::Thumbnails::EXTERNAL_CONTENT_URI, values, (IUri**)&url);

    //try {
        AutoPtr<IOutputStream> thumbOut;
        cr->OpenOutputStream(url, (IOutputStream**)&thumbOut);

        Boolean res;
        thumb->Compress(BitmapCompressFormat_JPEG, 100, thumbOut, &res);
        thumbOut->Close();
        return thumb;
    /*}
    catch (FileNotFoundException ex) {
        return NULL;
    }
    catch (IOException ex) {
        return NULL;
    }*/
}

/**
 * Insert an image and create a thumbnail for it.
 *
 * @param cr The content resolver to use
 * @param source The stream to use for the image
 * @param title The name of the image
 * @param description The description of the image
 * @return The URL to the newly created image, or <code>NULL</code> if the image failed to be stored
 *              for any reason.
 */
String MediaStore::Images::Media::InsertImage(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IBitmap* source,
    /* [in] */ String title, 
    /* [in] */ String description)
{
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    values->PutString(String(MediaColumns_TITLE), title);
    values->PutString(String(ImageColumns_DESCRIPTION), description);
    values->PutString(String(MediaColumns_MIME_TYPE), String("image/jpeg"));

    AutoPtr<IUri> url = NULL;
    String stringUrl;    /* value to be returned */

    //try {
        cr->Insert(EXTERNAL_CONTENT_URI, values, (IUri**)&url);

        if (source != NULL) {
            AutoPtr<IOutputStream> imageOut;
            cr->OpenOutputStream(url, (IOutputStream**)&imageOut);
            //try {
                Boolean res;
                source->Compress(BitmapCompressFormat_JPEG, 50, imageOut, &res);
            //} finally {
                imageOut->Close();
            //}

                Int64 id = ContentUris::ParseId(url);
            // Wait until MINI_KIND thumbnail is generated.
                AutoPtr<IBitmap> miniThumb = Images::Thumbnails::GetThumbnail(cr, id,
                Images::Thumbnails::MINI_KIND, NULL);
            // This is for backward compatibility.
            AutoPtr<IBitmap> microThumb = StoreThumbnail(cr, miniThumb, id, 50.0, 50.0,
                Images::Thumbnails::MICRO_KIND);
        } else {
            //Log.e(TAG, "Failed to create thumbnail, removing original");
            //cr->Delete(url, NULL, NULL);
            url = NULL;
        }
    /*} catch (Exception e) {
        Log.e(TAG, "Failed to insert image", e);
        if (url != NULL) {
            cr.delete(url, NULL, NULL);
            url = NULL;
        }
    }*/

    if (url != NULL) {
        url->ToString(&stringUrl);
    }

    return stringUrl;
}

/**
 * Get the content:// style URI for the image media table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the image media table on the given volume
 */
AutoPtr<IUri> MediaStore::Images::Media::GetContentUri(
    /* [in] */ String volumeName)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/images/media", (IUri**)&uri);

    return uri;
}




AutoPtr<ICursor> MediaStore::Images::Thumbnails::Query(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IUri* uri, 
    /* [in] */ ArrayOf<String>* projection)
{
    AutoPtr<ICursor> c; 
    //cr->Query(uri, projection, NULL, NULL, DEFAULT_SORT_ORDER, (ICursor**)&c);
    return c;
}

AutoPtr<ICursor> MediaStore::Images::Thumbnails::QueryMiniThumbnails(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IUri* uri, 
    /* [in] */ Int32 kind,
    /* [in] */ ArrayOf<String>* projection) 
{
    AutoPtr<ICursor> c;
    //cr->Query(uri, projection, "kind = " + kind, NULL, DEFAULT_SORT_ORDER, (ICursor**)&c);

    return c;
}

AutoPtr<ICursor> MediaStore::Images::Thumbnails::QueryMiniThumbnail(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int32 kind,
    /* [in] */ ArrayOf<String>* projection)
{
    AutoPtr<ICursor> c;

    /*cr->Query(EXTERNAL_CONTENT_URI, projection,
            IMAGE_ID + " = " + origId + " AND " + KIND + " = " +
            kind, NULL, NULL, (ICursor**)&c);*/

    return c;
}

/**
 * This method cancels the thumbnail request so clients waiting for getThumbnail will be
 * interrupted and return immediately. Only the original process which made the getThumbnail
 * requests can cancel their own requests.
 *
 * @param cr ContentResolver
 * @param origId original image id
 */
void MediaStore::Images::Thumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId)
{
    InternalThumbnails::CancelThumbnailRequest(cr, origId, EXTERNAL_CONTENT_URI,
        InternalThumbnails::DEFAULT_GROUP_ID);
}

/**
 * This method checks if the thumbnails of the specified image (origId) has been created.
 * It will be blocked until the thumbnails are generated.
 *
 * @param cr ContentResolver used to dispatch queries to MediaProvider.
 * @param origId Original image id associated with thumbnail of interest.
 * @param kind The type of thumbnail to fetch. Should be either MINI_KIND or MICRO_KIND.
 * @param options this is only used for MINI_KIND when decoding the Bitmap
 * @return A Bitmap instance. It could be NULL if the original image
 *         associated with origId doesn't exist or memory is not enough.
 */
AutoPtr<IBitmap> MediaStore::Images::Thumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options)
{
    return InternalThumbnails::GetThumbnail(cr, origId,
        InternalThumbnails::DEFAULT_GROUP_ID, kind, options,
            EXTERNAL_CONTENT_URI, FALSE);
}

/**
 * This method cancels the thumbnail request so clients waiting for getThumbnail will be
 * interrupted and return immediately. Only the original process which made the getThumbnail
 * requests can cancel their own requests.
 *
 * @param cr ContentResolver
 * @param origId original image id
 * @param groupId the same groupId used in getThumbnail.
 */
void MediaStore::Images::Thumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int64 groupId)
{
    InternalThumbnails::CancelThumbnailRequest(cr, origId, EXTERNAL_CONTENT_URI, groupId);
}

/**
 * This method checks if the thumbnails of the specified image (origId) has been created.
 * It will be blocked until the thumbnails are generated.
 *
 * @param cr ContentResolver used to dispatch queries to MediaProvider.
 * @param origId Original image id associated with thumbnail of interest.
 * @param groupId the id of group to which this request belongs
 * @param kind The type of thumbnail to fetch. Should be either MINI_KIND or MICRO_KIND.
 * @param options this is only used for MINI_KIND when decoding the Bitmap
 * @return A Bitmap instance. It could be NULL if the original image
 *         associated with origId doesn't exist or memory is not enough.
 */
AutoPtr<IBitmap> MediaStore::Images::Thumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int64 groupId,
    /* [in] */ Int32 kind, 
    /* [in] */ IBitmapFactoryOptions* options)
{
    return InternalThumbnails::GetThumbnail(cr, origId, groupId, kind, options,
            EXTERNAL_CONTENT_URI, FALSE);
}

/**
 * Get the content:// style URI for the image media table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the image media table on the given volume
 */
AutoPtr<IUri> MediaStore::Images::Thumbnails::GetContentUri(
    /* [in] */ String volumeName)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/images/thumbnails", (IUri**)&uri);

    return uri;
}

            



  

/**
 * Converts a name to a "key" that can be used for grouping, sorting
 * and searching.
 * The rules that govern this conversion are:
 * - remove 'special' characters like ()[]'!?.,
 * - remove leading/trailing spaces
 * - convert everything to lowercase
 * - remove leading "the ", "an " and "a "
 * - remove trailing ", the|an|a"
 * - remove accents. This step leaves us with CollationKey data,
 *   which is not human readable
 *
 * @param name The artist or album name to convert
 * @return The "key" for the given name.
 */
String MediaStore::Audio::KeyFor(
    /* [in] */ String name)
{
    if (name != NULL)  {
        Boolean sortfirst = FALSE;
        if (!name.Equals(UNKNOWN_STRING)) {
            return String("\001");
        }
        // Check if the first character is \001. We use this to
        // force sorting of certain special files, like the silent ringtone.
        if (name.StartWith("\001")) {
            sortfirst = TRUE;
        }
        name.Trim().ToLowerCase();
        if (name.StartWith("the ")) {
            name = name.Substring(4);
        }
        if (name.StartWith("an ")) {
            name = name.Substring(3);
        }
        if (name.StartWith("a ")) {
            name = name.Substring(2);
        }
        if (name.EndWith(", the") || name.EndWith(",the") ||
            name.EndWith(", an") || name.EndWith(",an") ||
            name.EndWith(", a") || name.EndWith(",a")) {
            name = name.Substring(0, name.LastIndexOf(','));
        }
        name = name.Replace("[\\[\\]\\(\\)\"'.,?!]", "").Trim();
        if (name.GetLength() > 0) {
            // Insert a separator between the characters to avoid
            // matches on a partial character. If we ever change
            // to start-of-word-only matches, this can be removed.
            StringBuffer b;// = new StringBuilder();
            b += '.';
            Int32 nl = name.GetLength();
            for (Int32 i = 0; i < nl; i++) {
                b += name[i];
                b += '.';
            }
            name = b;
            String key = DatabaseUtils::GetCollationKey(name);
            if (sortfirst) {
                key = "\001" + key;
            }
            return key;
       } else {
            return String("");
        }
    }

    String str;
    return str;
}


/**
 * Get the content:// style URI for the audio media table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the audio media table on the given volume
 */
AutoPtr<IUri> MediaStore::Audio::Media::GetContentUri(
    /* [in] */ String volumeName)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/audio/media", (IUri**)&uri);

    return uri;
}

AutoPtr<IUri> MediaStore::Audio::Media::GetContentUriForPath(
    /* [in] */ String path)
{

    String str;
    AutoPtr<IFile> file = Environment::GetExternalStorageDirectory();
    file->GetPath(&str);

    return (str.StartWith(path) ?
            EXTERNAL_CONTENT_URI : INTERNAL_CONTENT_URI);
}

    



/**
 * Get the content:// style URI for the audio genres table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the audio genres table on the given volume
 */
AutoPtr<IUri> MediaStore::Audio::Genres::GetContentUri(
    /* [in] */ String volumeName)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/audio/genres", (IUri**)&uri);

    return uri;
}




AutoPtr<IUri> MediaStore::Audio::Genres::Members::GetContentUri(
    /* [in] */ String volumeName,
    /* [in] */ Int64 genreId)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName
        + "/audio/genres/" + String::FromInt64(genreId) + "/members", (IUri**)&uri);

    return uri;
}


/**
 * Get the content:// style URI for the audio playlists table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the audio playlists table on the given volume
 */
AutoPtr<IUri> MediaStore::Audio::Playlists::GetContentUri(
    /* [in] */ String volumeName) 
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/audio/playlists", (IUri**)&uri);

    return uri;
}

    
AutoPtr<IUri> MediaStore::Audio::Playlists::Members::GetContentUri(
    /* [in] */ String volumeName,
    /* [in] */ Int64 playlistId)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName
            + "/audio/playlists/" + String::FromInt64(playlistId) + "/members", (IUri**)&uri);

    return uri;
}

/**
 * Convenience method to move a playlist item to a new location
 * @param res The content resolver to use
 * @param playlistId The numeric id of the playlist
 * @param from The position of the item to move
 * @param to The position to move the item to
 * @return TRUE on success
 */
Boolean MediaStore::Audio::Playlists::Members::MoveItem(
    /* [in] */ IContentResolver* res,
    /* [in] */ Int64 playlistId, 
    /* [in] */ Int32 from, 
    /* [in] */ Int32 to) 
{
    AutoPtr<IUri> uri = MediaStore::Audio::Playlists::Members::GetContentUri(String("external"),
            playlistId);

    AutoPtr<IUriBuilder> ub;

            
    uri->BuildUpon((IUriBuilder**)&ub);
    ub->AppendEncodedPath(String::FromInt32(from));
    ub->AppendQueryParameter(String("move"), String("TRUE"));
    ub->Build((IUri**)&uri);
    
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    values->PutInt32(String(MediaStore::Audio::Playlists::Members::PLAY_ORDER), to);
    
    Int32 result;
    //res->Update(uri, values, NULL, NULL, &result);

    return result != 0;
}



/**
 * Get the content:// style URI for the artists table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the audio artists table on the given volume
 */
AutoPtr<IUri> MediaStore::Audio::Artists::GetContentUri(
    /* [in] */ String volumeName)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/audio/artists", (IUri**)&uri);    

    return uri;
}


AutoPtr<IUri> MediaStore::Audio::Artists::Albums::GetContentUri(
    /* [in] */ String volumeName,
    /* [in] */ Int64 artistId) 
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName
        + "/audio/artists/" + String::FromInt64(artistId) + "/albums", (IUri**)&uri);

    return uri;
}


/**
 * Get the content:// style URI for the albums table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the audio albums table on the given volume
 */
AutoPtr<IUri> MediaStore::Audio::Albums::GetContentUri(
    /* [in] */ String volumeName) 
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/audio/albums", (IUri**)&uri);

    return uri;
}




        
AutoPtr<ICursor> MediaStore::Video::Query(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ IUri* uri, 
    /* [in] */ ArrayOf<String>* projection) 
{
    AutoPtr<ICursor> c;
    
    //cr->Query(uri, projection, NULL, NULL, DEFAULT_SORT_ORDER, (ICursor**)&c);

    return c;
}


/**
 * Get the content:// style URI for the video media table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the video media table on the given volume
 */
AutoPtr<IUri> MediaStore::Video::Media::GetContentUri(
    /* [in] */ String volumeName)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/video/media", (IUri**)&uri);

    return uri;
}

    
/**
 * This method cancels the thumbnail request so clients waiting for getThumbnail will be
 * interrupted and return immediately. Only the original process which made the getThumbnail
 * requests can cancel their own requests.
 *
 * @param cr ContentResolver
 * @param origId original video id
 */
void MediaStore::Video::Thumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId)
{
    InternalThumbnails::CancelThumbnailRequest(cr, origId, EXTERNAL_CONTENT_URI,
        InternalThumbnails::DEFAULT_GROUP_ID);
}

/**
 * This method checks if the thumbnails of the specified image (origId) has been created.
 * It will be blocked until the thumbnails are generated.
 *
 * @param cr ContentResolver used to dispatch queries to MediaProvider.
 * @param origId Original image id associated with thumbnail of interest.
 * @param kind The type of thumbnail to fetch. Should be either MINI_KIND or MICRO_KIND.
 * @param options this is only used for MINI_KIND when decoding the Bitmap
 * @return A Bitmap instance. It could be NULL if the original image
 *         associated with origId doesn't exist or memory is not enough.
 */
AutoPtr<IBitmap> MediaStore::Video::Thumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options) 
{
    return InternalThumbnails::GetThumbnail(cr, origId,
        InternalThumbnails::DEFAULT_GROUP_ID, kind, options,
            EXTERNAL_CONTENT_URI, TRUE);
}

/**
 * This method checks if the thumbnails of the specified image (origId) has been created.
 * It will be blocked until the thumbnails are generated.
 *
 * @param cr ContentResolver used to dispatch queries to MediaProvider.
 * @param origId Original image id associated with thumbnail of interest.
 * @param groupId the id of group to which this request belongs
 * @param kind The type of thumbnail to fetch. Should be either MINI_KIND or MICRO_KIND
 * @param options this is only used for MINI_KIND when decoding the Bitmap
 * @return A Bitmap instance. It could be NULL if the original image associated with
 *         origId doesn't exist or memory is not enough.
 */
AutoPtr<IBitmap> MediaStore::Video::Thumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int64 groupId,
    /* [in] */ Int32 kind, 
    /* [in] */ IBitmapFactoryOptions* options) 
{
    return InternalThumbnails::GetThumbnail(cr, origId, groupId, kind, options,
            EXTERNAL_CONTENT_URI, TRUE);
}

/**
 * This method cancels the thumbnail request so clients waiting for getThumbnail will be
 * interrupted and return immediately. Only the original process which made the getThumbnail
 * requests can cancel their own requests.
 *
 * @param cr ContentResolver
 * @param origId original video id
 * @param groupId the same groupId used in getThumbnail.
 */
void MediaStore::Video::Thumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr, 
    /* [in] */ Int64 origId, 
    /* [in] */ Int64 groupId)
{
    InternalThumbnails::CancelThumbnailRequest(cr, origId, EXTERNAL_CONTENT_URI, groupId);
}

/**
 * Get the content:// style URI for the image media table on the
 * given volume.
 *
 * @param volumeName the name of the volume to get the URI for
 * @return the URI to the image media table on the given volume
 */
AutoPtr<IUri> MediaStore::Video::Thumbnails::GetContentUri(
    /* [in] */ String volumeName)
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + volumeName +
            "/video/thumbnails", (IUri**)&uri);

    return uri;
}
    

/**
 * Uri for querying the state of the media scanner.
 */
AutoPtr<IUri> MediaStore::GetMediaScannerUri()
{
    AutoPtr<IUri> uri;

    Uri::Parse(CONTENT_AUTHORITY_SLASH + "none/media_scanner", (IUri**)&uri);

    return uri;
}
