
#ifndef __MEDIASTORE_H__
#define __MEDIASTORE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * The Media provider contains meta data for all available media on both internal
 * and external storage devices.
 */
class MediaStore 
{
public:

    /**
     * Contains meta data for all available images.
     */
    class Images
    {
    public:
        class Media
        {
        public:
            static CARAPI_(AutoPtr<ICursor>) Query(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ IUri* uri, 
                /* [in] */ ArrayOf<String>* projection);

            static CARAPI_(AutoPtr<ICursor>) Query(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ IUri* uri, 
                /* [in] */ ArrayOf<String>* projection,
                /* [in] */ String where, 
                /* [in] */ String orderBy);

            static CARAPI_(AutoPtr<ICursor>) Query(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ IUri* uri, 
                /* [in] */ ArrayOf<String>* projection,
                /* [in] */ String selection, 
                /* [in] */ ArrayOf<String>* selectionArgs, 
                /* [in] */ String orderBy);

            /**
             * Retrieves an image for the given url as a {@link Bitmap}.
             *
             * @param cr The content resolver to use
             * @param url The url of the image
             * @throws FileNotFoundException
             * @throws IOException
             */
            static CARAPI_(AutoPtr<IBitmap>) GetBitmap(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ IUri* url);

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
            static CARAPI_(String) InsertImage(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ String imagePath,
                /* [in] */ String name, 
                /* [in] */ String description);

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
            static CARAPI_(String) InsertImage(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ IBitmap* source,
                /* [in] */ String title, 
                /* [in] */ String description);

            /**
             * Get the content:// style URI for the image media table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the image media table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>) GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;

            /**
             * The MIME type of of this directory of
             * images.  Note that each entry in this directory will have a standard
             * image MIME type as appropriate -- for example, image/jpeg.
             */
            static const CString CONTENT_TYPE;// = "vnd.android.cursor.dir/image";

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = ImageColumns_BUCKET_DISPLAY_NAME;

        private:
            static CARAPI_(AutoPtr<IBitmap>) StoreThumbnail(
                /* [in] */ IContentResolver* cr,
                /* [in] */ IBitmap* source,
                /* [in] */ Int64 id,
                /* [in] */ Float width, 
                /* [in] */ Float height,
                /* [in] */ Int32 kind);
        };

        /**
         * This class allows developers to query and get two kinds of thumbnails:
         * MINI_KIND: 512 x 384 thumbnail
         * MICRO_KIND: 96 x 96 thumbnail
         */
        class Thumbnails
        {
        public:
            static CARAPI_(AutoPtr<ICursor>) Query(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ IUri* uri, 
                /* [in] */ ArrayOf<String>* projection);

            static CARAPI_(AutoPtr<ICursor> QueryMiniThumbnails(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ IUri* uri, 
                /* [in] */ Int32 kind,
                /* [in] */ ArrayOf<String>* projection);

            static CARAPI_(AutoPtr<ICursor>) QueryMiniThumbnail(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId, 
                /* [in] */ Int32 kind,
                /* [in] */ ArrayOf<String>* projection);

            /**
             * This method cancels the thumbnail request so clients waiting for getThumbnail will be
             * interrupted and return immediately. Only the original process which made the getThumbnail
             * requests can cancel their own requests.
             *
             * @param cr ContentResolver
             * @param origId original image id
             */
            static CARAPI_(void) CancelThumbnailRequest(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId);

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
            static CARAPI_(AutoPtr<IBitmap>) GetThumbnail(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId, 
                /* [in] */ Int32 kind,
                /* [in] */ IBitmapFactoryOptions* options);

            /**
             * This method cancels the thumbnail request so clients waiting for getThumbnail will be
             * interrupted and return immediately. Only the original process which made the getThumbnail
             * requests can cancel their own requests.
             *
             * @param cr ContentResolver
             * @param origId original image id
             * @param groupId the same groupId used in getThumbnail.
             */
            static CARAPI_(void) CancelThumbnailRequest(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId, 
                /* [in] */ Int64 groupId);

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
            static CARAPI_(AutoPtr<IBitmap>) GetThumbnail(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId, 
                /* [in] */ Int64 groupId,
                /* [in] */ Int32 kind, 
                /* [in] */ IBitmapFactoryOptions* options);

            /**
             * Get the content:// style URI for the image media table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the image media table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>)) GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = "image_id ASC";

            /**
             * The data stream for the thumbnail
             * <P>Type: DATA STREAM</P>
             */
            static const CString DATA;// = "_data";

            /**
             * The original image for the thumbnal
             * <P>Type: INTEGER (ID from Images table)</P>
             */
            static const CString IMAGE_ID;// = "image_id";

            /**
             * The kind of the thumbnail
             * <P>Type: INTEGER (One of the values below)</P>
             */
            static const CString KIND;// = "kind";

            static const Int32 MINI_KIND = 1;
            static const Int32 FULL_SCREEN_KIND = 2;
            static const Int32 MICRO_KIND = 3;
            /**
             * The blob raw data of thumbnail
             * <P>Type: DATA STREAM</P>
             */
            static const CString THUMB_DATA;// = "thumb_data";

            /**
             * The width of the thumbnal
             * <P>Type: INTEGER (Int64)</P>
             */
            static const CString WIDTH;// = "width";

            /**
             * The height of the thumbnail
             * <P>Type: INTEGER (Int64)</P>
             */
            static const CString HEIGHT;// = "height";
        };
    };

    /**
     * Container for all audio content.
     */
    class Audio
    {
    public:
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
        static CARAPI_(String) KeyFor(
            /* [in] */ String name);

        class Media
        {
        public:
            /**
             * Get the content:// style URI for the audio media table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the audio media table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>) GetContentUri(
                /* [in] */ String volumeName);

            static CARAPI_(AutoPtr<IUri>) GetContentUriForPath(
                /* [in] */ String path);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;// = GetContentUri("internal");

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;// = GetContentUri("external");

            /**
             * The MIME type for this table.
             */
            static const CString CONTENT_TYPE;// = "vnd.android.cursor.dir/audio";

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = TITLE_KEY;

            /**
             * Activity Action: Start SoundRecorder application.
             * <p>Input: nothing.
             * <p>Output: An uri to the recorded sound stored in the Media Library
             * if the recording was successful.
             * May also contain the extra EXTRA_MAX_BYTES.
             * @see #EXTRA_MAX_BYTES
             */
            static const CString RECORD_SOUND_ACTION;// = "android.provider.MediaStore.RECORD_SOUND";

            /**
             * The name of the Intent-extra used to define a maximum file size for
             * a recording made by the SoundRecorder application.
             *
             * @see #RECORD_SOUND_ACTION
             */
             static const CString EXTRA_MAX_BYTES;// = "android.provider.MediaStore.extra.MAX_BYTES";
        };


        /**
         * Contains all genres for audio files
         */
        class Genres
        {
        public:
            /**
             * Get the content:// style URI for the audio genres table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the audio genres table on the given volume
             */
            static AutoPtr<IUri> GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;// = GetContentUri("internal");

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;// = GetContentUri("external");

            /**
             * The MIME type for this table.
             */
            static const CString CONTENT_TYPE;// = "vnd.android.cursor.dir/genre";

            /**
             * The MIME type for entries in this table.
             */
            static const CString ENTRY_CONTENT_TYPE;// = "vnd.android.cursor.item/genre";

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = NAME;

            /**
             * Sub-directory of each genre containing all members.
             */
            class Members
            {
            public:
                static CARAPI_(AutoPtr<IUri>) GetContentUri(
                    /* [in] */ String volumeName,
                    /* [in] */ Int64 genreId);

                /**
                 * A subdirectory of each genre containing all member audio files.
                 */
                static const CString CONTENT_DIRECTORY;// = "members";

                /**
                 * The default sort order for this table
                 */
                static const CString DEFAULT_SORT_ORDER;// = TITLE_KEY;

                /**
                 * The ID of the audio file
                 * <P>Type: INTEGER (Int64)</P>
                 */
                static const CString AUDIO_ID;// = "audio_id";

                /**
                 * The ID of the genre
                 * <P>Type: INTEGER (Int64)</P>
                 */
                static const CString GENRE_ID;// = "genre_id";
            };
        };


        /**
         * Contains playlists for audio files
         */
        class Playlists
        {
        public:
            /**
             * Get the content:// style URI for the audio playlists table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the audio playlists table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>) GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;// = GetContentUri("internal");

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;// = GetContentUri("external");

            /**
             * The MIME type for this table.
             */
            static const CString CONTENT_TYPE;// = "vnd.android.cursor.dir/playlist";

            /**
             * The MIME type for entries in this table.
             */
            static const CString ENTRY_CONTENT_TYPE;// = "vnd.android.cursor.item/playlist";

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = NAME;

            /**
             * Sub-directory of each playlist containing all members.
             */
            class Members 
            {
            public:
                static CARAPI_(AutoPtr<IUri>) GetContentUri(
                    /* [in] */ String volumeName,
                    /* [in] */ Int64 playlistId);

                /**
                 * Convenience method to move a playlist item to a new location
                 * @param res The content resolver to use
                 * @param playlistId The numeric id of the playlist
                 * @param from The position of the item to move
                 * @param to The position to move the item to
                 * @return true on success
                 */
                static CARAPI_(Boolean) MoveItem(
                    /* [in] */ IContentResolver* res,
                    /* [in] */ Int64 playlistId, 
                    /* [in] */ Int32 from, 
                    /* [in] */ Int32 to);

                /**
                 * The ID within the playlist.
                 */
                static const CString _ID;// = "_id";

                /**
                 * A subdirectory of each playlist containing all member audio
                 * files.
                 */
                static const CString CONTENT_DIRECTORY;// = "members";

                /**
                 * The ID of the audio file
                 * <P>Type: INTEGER (Int64)</P>
                 */
                static const CString AUDIO_ID;// = "audio_id";

                /**
                 * The ID of the playlist
                 * <P>Type: INTEGER (Int64)</P>
                 */
                static const CString PLAYLIST_ID;// = "playlist_id";

                /**
                 * The order of the songs in the playlist
                 * <P>Type: INTEGER (Int64)></P>
                 */
                static const CString PLAY_ORDER;// = "play_order";

                /**
                 * The default sort order for this table
                 */
                static const CString DEFAULT_SORT_ORDER;// = PLAY_ORDER;
            };
        };

        /**
         * Contains artists for audio files
         */
        class Artists
        {
        public: 
            /**
             * Get the content:// style URI for the artists table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the audio artists table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>) GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;// = GetContentUri("internal");

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;// = GetContentUri("external");

            /**
             * The MIME type for this table.
             */
            static const CString CONTENT_TYPE;// = "vnd.android.cursor.dir/artists";

            /**
             * The MIME type for entries in this table.
             */
            static const CString ENTRY_CONTENT_TYPE;// = "vnd.android.cursor.item/artist";

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = ARTIST_KEY;

            /**
             * Sub-directory of each artist containing all albums on which
             * a song by the artist appears.
             */
            class Albums
            {
            public:
                static CARAPI_(AutoPtr<IUri>) GetContentUri(
                    /* [in] */ String volumeName,
                    /* [in] */ Int64 artistId);
            };
        };

        /**
         * Contains artists for audio files
         */
        class Albums
        {
        public:
            /**
             * Get the content:// style URI for the albums table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the audio albums table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>) GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;// = GetContentUri("internal");

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;// = GetContentUri("external");

            /**
             * The MIME type for this table.
             */
            static const CString CONTENT_TYPE;// = "vnd.android.cursor.dir/albums";

            /**
             * The MIME type for entries in this table.
             */
            static const CString ENTRY_CONTENT_TYPE;// = "vnd.android.cursor.item/album";

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = ALBUM_KEY;
        };
    };

    class Video 
    {
    public:
        /**
         * The default sort order for this table.
         */
        static const CString DEFAULT_SORT_ORDER;// = MediaColumns_DISPLAY_NAME;

        static CARAPI_(AutoPtr<ICursor>) Query(
            /* [in] */ IContentResolver* cr, 
            /* [in] */ IUri* uri, 
            /* [in] */ ArrayOf<String>* projection);


        class Media
        {
        public:
            /**
             * Get the content:// style URI for the video media table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the video media table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>) GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;// = GetContentUri("internal");

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;// = GetContentUri("external");

            /**
             * The MIME type for this table.
             */
            static const CString CONTENT_TYPE;// = "vnd.android.cursor.dir/video";

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = TITLE;
        };

        /**
         * This class allows developers to query and get two kinds of thumbnails:
         * MINI_KIND: 512 x 384 thumbnail
         * MICRO_KIND: 96 x 96 thumbnail
         *
         */
        class Thumbnails
        {
        public:
            /**
             * This method cancels the thumbnail request so clients waiting for getThumbnail will be
             * interrupted and return immediately. Only the original process which made the getThumbnail
             * requests can cancel their own requests.
             *
             * @param cr ContentResolver
             * @param origId original video id
             */
            static CARAPI_(void) CancelThumbnailRequest(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId);

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
            static CARAPI_(AutoPtr<IBitmap>) GetThumbnail(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId, 
                /* [in] */ Int32 kind,
                /* [in] */ IBitmapFactoryOptions* options);

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
            static CARAPI_(AutoPtr<IBitmap>) GetThumbnail(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId, 
                /* [in] */ Int64 groupId,
                /* [in] */ Int32 kind, 
                /* [in] */ IBitmapFactoryOptions* options);

            /**
             * This method cancels the thumbnail request so clients waiting for getThumbnail will be
             * interrupted and return immediately. Only the original process which made the getThumbnail
             * requests can cancel their own requests.
             *
             * @param cr ContentResolver
             * @param origId original video id
             * @param groupId the same groupId used in getThumbnail.
             */
            static CARAPI_(void) CancelThumbnailRequest(
                /* [in] */ IContentResolver* cr, 
                /* [in] */ Int64 origId, 
                /* [in] */ Int64 groupId);

            /**
             * Get the content:// style URI for the image media table on the
             * given volume.
             *
             * @param volumeName the name of the volume to get the URI for
             * @return the URI to the image media table on the given volume
             */
            static CARAPI_(AutoPtr<IUri>) GetContentUri(
                /* [in] */ String volumeName);

            /**
             * The content:// style URI for the internal storage.
             */
            static const AutoPtr<IUri> INTERNAL_CONTENT_URI;// = GetContentUri("internal");

            /**
             * The content:// style URI for the "primary" external storage
             * volume.
             */
            static const AutoPtr<IUri> EXTERNAL_CONTENT_URI;// = GetContentUri("external");

            /**
             * The default sort order for this table
             */
            static const CString DEFAULT_SORT_ORDER;// = "video_id ASC";

            /**
             * The data stream for the thumbnail
             * <P>Type: DATA STREAM</P>
             */
            static const CString DATA;// = "_data";

            /**
             * The original image for the thumbnal
             * <P>Type: INTEGER (ID from Video table)</P>
             */
            static const CString VIDEO_ID;// = "video_id";

            /**
             * The kind of the thumbnail
             * <P>Type: INTEGER (One of the values below)</P>
             */
            static const CString KIND;// = "kind";

            static const Int32 MINI_KIND = 1;
            static const Int32 FULL_SCREEN_KIND = 2;
            static const Int32 MICRO_KIND = 3;

            /**
             * The width of the thumbnal
             * <P>Type: INTEGER (Int64)</P>
             */
            static const CString WIDTH;// = "width";

            /**
             * The height of the thumbnail
             * <P>Type: INTEGER (Int64)</P>
             */
            static const CString HEIGHT;// = "height";
        };
    };

 
private:
    /**
     * This class is used internally by Images.Thumbnails and Video.Thumbnails, it's not intended
     * to be accessed elsewhere.
     */
    class InternalThumbnails
    {
    public:
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
        static CARAPI_(void) CancelThumbnailRequest(
            /* [in] */ IContentResolver* cr, 
            /* [in] */ Int64 origId, 
            /* [in] */ IUri* baseUri,
            /* [in] */ Int64 groupId);

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
        static CARAPI_(AutoPtr<IBitmap>) GetThumbnail(
            /* [in] */ IContentResolver* cr, 
            /* [in] */ Int64 origId, 
            /* [in] */ Int64 groupId, 
            /* [in] */ Int32 kind,
            /* [in] */ IBitmapFactoryOptions* options, 
            /* [in] */ IUri* baseUri, 
            /* [in] */ Boolean isVideo);

        static const Int32 DEFAULT_GROUP_ID = 0;
    
    private:
        static const Int32 MINI_KIND = 1;
        static const Int32 FULL_SCREEN_KIND = 2;
        static const Int32 MICRO_KIND = 3;
        static ArrayOf<String>* PROJECTION;// = {"_id", "_data"};// = new String[] {_ID, MediaColumns.DATA};
     
        //static const Object sThumbBufLock = new Object();
        static ArrayOf<Byte>* sThumbBuf;

        static CARAPI_(AutoPtr<IBitmap>) GetMiniThumbFromFile(
            /* [in] */ ICursor* c, 
            /* [in] */ IUri* baseUri, 
            /* [in] */ IContentResolver* cr, 
            /* [in] */ IBitmapFactoryOptions* options);

    };

public:
    /**
     * Uri for querying the state of the media scanner.
     */
    static CARAPI_(AutoPtr<IUri>) GetMediaScannerUri();

public:
    static const CString AUTHORITY;// = "media";
    
    /**
     * Activity Action: Launch a music player.
     * The activity should be able to play, browse, or manipulate music files stored on the device.
     */
    //@SdkConstant(SdkConstantType.ACTIVITY_INTENT_ACTION)
    static const CString INTENT_ACTION_MUSIC_PLAYER;// = "android.intent.action.MUSIC_PLAYER";

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
    static const CString INTENT_ACTION_MEDIA_SEARCH;// = "android.intent.action.MEDIA_SEARCH";

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
    static const CString INTENT_ACTION_MEDIA_PLAY_FROM_SEARCH;/* =
            "android.media.action.MEDIA_PLAY_FROM_SEARCH";*/
    
    /**
     * The name of the Intent-extra used to define the artist
     */
    static const CString EXTRA_MEDIA_ARTIST;// = "android.intent.extra.artist";

    /**
     * The name of the Intent-extra used to define the album
     */
    static const CString EXTRA_MEDIA_ALBUM;// = "android.intent.extra.album";

    /**
     * The name of the Intent-extra used to define the song title
     */
    static const CString EXTRA_MEDIA_TITLE;// = "android.intent.extra.title";

    /**
     * The name of the Intent-extra used to define the search focus. The search focus
     * indicates whether the search should be for things related to the artist, album
     * or song that is identified by the other extras.
     */
    static const CString EXTRA_MEDIA_FOCUS ;//= "android.intent.extra.focus";

    /**
     * The name of the Intent-extra used to control the orientation of a ViewImage or a MovieView.
     * This is an Int32 property that overrides the activity's requestedOrientation.
     * @see android.content.pm.ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED
     */
    static const CString EXTRA_SCREEN_ORIENTATION ;//= "android.intent.extra.screenOrientation";

    /**
     * The name of an Intent-extra used to control the UI of a ViewImage.
     * This is a boolean property that overrides the activity's default fullscreen state.
     */
    static const CString EXTRA_FULL_SCREEN;// = "android.intent.extra.fullScreen";

    /**
     * The name of an Intent-extra used to control the UI of a ViewImage.
     * This is a boolean property that specifies whether or not to show action icons.
     */
    static const CString EXTRA_SHOW_ACTION_ICONS ;//= "android.intent.extra.showActionIcons";

    /**
     * The name of the Intent-extra used to control the onCompletion behavior of a MovieView.
     * This is a boolean property that specifies whether or not to finish the MovieView activity
     * when the movie completes playing. The default value is true, which means to automatically
     * exit the movie player activity when the movie completes playing.
     */
    static const CString EXTRA_FINISH_ON_COMPLETION;// = "android.intent.extra.finishOnCompletion";

    /**
     * The name of the Intent action used to launch a camera in still image mode.
     */
    static const CString INTENT_ACTION_STILL_IMAGE_CAMERA;// = "android.media.action.STILL_IMAGE_CAMERA";

    /**
     * The name of the Intent action used to launch a camera in video mode.
     */
    static const CString INTENT_ACTION_VIDEO_CAMERA;// = "android.media.action.VIDEO_CAMERA";

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
    static const CString ACTION_IMAGE_CAPTURE ;//= "android.media.action.IMAGE_CAPTURE";

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
    static const CString ACTION_VIDEO_CAPTURE ;//= "android.media.action.VIDEO_CAPTURE";

    /**
     * The name of the Intent-extra used to control the quality of a recorded video. This is an
     * integer property. Currently value 0 means low quality, suitable for MMS messages, and
     * value 1 means high quality. In the future other quality levels may be added.
     */
    static const CString EXTRA_VIDEO_QUALITY;// = "android.intent.extra.videoQuality";

    /**
     * Specify the maximum allowed size.
     */
    static const CString EXTRA_SIZE_LIMIT;// = "android.intent.extra.sizeLimit";

    /**
     * Specify the maximum allowed recording duration in seconds.
     */
    static const CString EXTRA_DURATION_LIMIT;// = "android.intent.extra.durationLimit";

    /**
     * The name of the Intent-extra used to indicate a content resolver Uri to be used to
     * store the requested image or video.
     */
    static const CString EXTRA_OUTPUT;// = "output";

    /**
      * The string that is used when a media attribute is not known. For example,
      * if an audio file does not have any meta data, the artist and album columns
      * will be set to this value.
      */
    static const CString UNKNOWN_STRING;// = "<unknown>";

    /**
     * Name of current volume being scanned by the media scanner.
     */
    static const CString MEDIA_SCANNER_VOLUME;// = "volume";

    /**
     * Name of the file signaling the media scanner to ignore media in the containing directory
     * and its subdirectories. Developers should use this to avoid application graphics showing
     * up in the Gallery and likewise prevent application sounds and music from showing up in
     * the Music app.
     */
    static const CString MEDIA_IGNORE_FILENAME;// = ".nomedia";

private:
    static const CString TAG;// = "MediaStore";

    static const String CONTENT_AUTHORITY_SLASH;// = "content://" + AUTHORITY + "/";
};

#endif
