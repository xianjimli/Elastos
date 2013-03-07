
#ifndef __MEDIASCANNER_H__
#define __MEDIASCANNER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>

/**
 * Internal service helper that no-one should use directly.
 *
 * The way the scan currently works is:
 * - The Java MediaScannerService creates a MediaScanner (this class), and calls
 *   MediaScanner.scanDirectories on it.
 * - scanDirectories() calls the native processDirectory() for each of the specified directories.
 * - the processDirectory() JNI method wraps the provided mediascanner client in a native
 *   'MyMediaScannerClient' class, then calls processDirectory() on the native MediaScanner
 *   object (which got created when the Java MediaScanner was created).
 * - native MediaScanner.processDirectory() (currently part of opencore) calls
 *   doProcessDirectory(), which recurses over the folder, and calls
 *   native MyMediaScannerClient.scanFile() for every file whose extension matches.
 * - native MyMediaScannerClient.scanFile() calls back on Java MediaScannerClient.scanFile,
 *   which calls doScanFile, which after some setup calls back down to native code, calling
 *   MediaScanner.processFile().
 * - MediaScanner.processFile() calls one of several methods, depending on the type of the
 *   file: parseMP3, parseMP4, parseMidi, parseOgg or parseWMA.
 * - each of these methods gets metadata key/value pairs from the file, and repeatedly
 *   calls native MyMediaScannerClient.handleStringTag, which calls back up to its Java
 *   counterparts in this file.
 * - Java handleStringTag() gathers the key/value pairs that it's interested in.
 * - once processFile returns and we're back in Java code in doScanFile(), it calls
 *   Java MyMediaScannerClient.endFile(), which takes all the data that's been
 *   gathered and inserts an entry in to the database.
 *
 * In summary:
 * Java MediaScannerService calls
 * Java MediaScanner scanDirectories, which calls
 * Java MediaScanner processDirectory (native method), which calls
 * native MediaScanner processDirectory, which calls
 * native MyMediaScannerClient scanFile, which calls
 * Java MyMediaScannerClient scanFile, which calls
 * Java MediaScannerClient doScanFile, which calls
 * Java MediaScanner processFile (native method), which calls
 * native MediaScanner processFile, which calls
 * native parseMP3, parseMP4, parseMidi, parseOgg or parseWMA, which calls
 * native MyMediaScanner handleStringTag, which calls
 * Java MyMediaScanner handleStringTag.
 * Once MediaScanner processFile returns, an entry is inserted in to the database.
 *
 * {@hide}
 */

class MediaScanner
{
public:
    MediaScanner();

    MediaScanner(
        /* [in] */ IContext* c);

    virtual CARAPI ScanDirectories(
        /* [in] */ ArrayOf<String>* directories, 
        /* [in] */ String volumeName);

    // this function is used to scan a single file
    virtual CARAPI_(AutoPtr<IUri>) ScanSingleFile(
        /* [in] */ String path, 
        /* [in] */ String volumeName, 
        /* [in] */ String mimeType);

    /* native */ virtual CARAPI_(void) SetLocale(
        /* [in] */ String locale);

    /* native */ virtual CARAPI_(ArrayOf<Byte>*) ExtractAlbumArt(
        /* [in] */ IFileDescriptor* fd);

    class FileCacheEntry
    {
    public:
        FileCacheEntry(
            /* [in] */ IUri* tableUri, 
            /* [in] */ Int64 rowId, 
            /* [in] */ String path, 
            /* [in] */ Int64 lastModified);

        CARAPI_(String) ToString();

        AutoPtr<IUri> mTableUri;
        Int64 mRowId;
        String mPath;
        Int64 mLastModified;
        Boolean mSeenInFileSystem;
        Boolean mLastModifiedChanged;
    };

protected:
    virtual CARAPI_(void) Finalize();

private:
    CARAPI_(void) SetDefaultRingtoneFileNames();


    class MyMediaScannerClient : public IMediaScannerClient, public ElRefBase
    {
    public:
        virtual CARAPI_(FileCacheEntry*) BeginFile(
            /* [in] */ String path, 
            /* [in] */ String mimeType, 
            /* [in] */ Int64 lastModified, 
            /* [in] */ Int64 fileSize);
        
        virtual CARAPI ScanFile(
            /* [in] */ String path, 
            /* [in] */ Int64 lastModified, 
            /* [in] */ Int64 fileSize);
        
        virtual CARAPI ScanFileEx(
            /* [in] */ String path, 
            /* [in] */ String mimeType, 
            /* [in] */ Int64 lastModified, 
            /* [in] */ Int64 fileSize);
        
        virtual CARAPI_(AutoPtr<IUri>) DoScanFile(
            /* [in] */ String path, 
            /* [in] */ String mimeType, 
            /* [in] */ Int64 lastModified, 
            /* [in] */ Int64 fileSize, 
            /* [in] */ Boolean scanAlways);
        
        virtual CARAPI HandleStringTag(
            /* [in] */ String name, 
            /* [in] */ String value);

        virtual CARAPI SetMimeType(
            /* [in] */ String mimeType);

        virtual CARAPI AddNoMediaFolder(
            /* [in] */ String path);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        CARAPI_(Int32) ParseSubstring(
            /* [in] */ String s, 
            /* [in] */ Int32 start, 
            /* [in] */ Int32 defaultValue);

        /**
         * Formats the data into a values array suitable for use with the Media
         * Content Provider.
         *
         * @return a map of values
         */
        CARAPI_(AutoPtr<IContentValues>) ToValues();

        CARAPI_(AutoPtr<IUri>) EndFile(
            /* [in] */ FileCacheEntry* entry, 
            /* [in] */ Boolean ringtones, 
            /* [in] */ Boolean notifications,
            /* [in] */ Boolean alarms, 
            /* [in] */ Boolean music, 
            /* [in] */ Boolean podcasts);

        CARAPI_(Boolean) DoesPathHaveFilename(
            /* [in] */ String path, 
            /* [in] */ String filename);

        CARAPI_(void) SetSettingIfNotSet(
            /* [in] */ String settingName, 
            /* [in] */ IUri* uri, 
            /* [in] */ Int64 rowId);

    private:
        String mArtist;
        String mAlbumArtist;    // use this if mArtist is missing
        String mAlbum;
        String mTitle;
        String mComposer;
        String mGenre;
        String mMimeType;
        Int32 mFileType;
        Int32 mTrack;
        Int32 mYear;
        Int32 mDuration;
        String mPath;
        Int64 mLastModified;
        Int64 mFileSize;
        String mWriter;
        Int32 mCompilation;

    }; // end of anonymous MediaScannerClient instance

    CARAPI_(void) Prescan(
        /* [in] */ String filePath);

    CARAPI_(Boolean) InScanDirectory(
        /* [in] */ String path, 
        /* [in] */ ArrayOf<String>* directories);

    CARAPI_(void) PruneDeadThumbnailFiles();

    CARAPI_(void) Postscan(
        /* [in] */ ArrayOf<String>* directories);

    CARAPI_(void) Initialize(
        /* [in] */ String volumeName);

    // returns the number of matching file/directory names, starting from the right
    CARAPI_(Int32) MatchPaths(
        /* [in] */ String path1, 
        /* [in] */ String path2);

    CARAPI_(Boolean) AddPlayListEntry(
        /* [in] */ String entry, 
        /* [in] */ String playListDirectory,
        /* [in] */ IUri* uri, 
        /* [in] */ IContentValues* values, 
        /* [in] */ Int32 index);

    CARAPI_(void) ProcessM3uPlayList(
        /* [in] */ String path, 
        /* [in] */ String playListDirectory, 
        /* [in] */ IUri* uri, 
        /* [in] */ IContentValues* values);

    CARAPI_(void) ProcessPlsPlayList(
        /* [in] */ String path, 
        /* [in] */ String playListDirectory, 
        /* [in] */ IUri* uri, 
        /* [in] */ IContentValues* values);

    class WplHandler /*implements ElementListener*/
    {
    public:
        WplHandler(
            /* [in] */ String playListDirectory, 
            /* [in] */ IUri* uri);

        //virtual CARAPI_(void) Start(
        //    /* [in] */ IAttributes* attributes);

        virtual CARAPI_(void) End();

        //virtual CARAPI_(AutoPtr<IContentHandler>) GetContentHandler();

        //AutoPtr<IContentHandler> handler;
        String playListDirectory;
        AutoPtr<IUri> uri;
        AutoPtr<IContentValues> values;// = new ContentValues();
        Int32 index;// = 0;
    };

    CARAPI_(void) ProcessWplPlayList(
        /* [in] */ String path, 
        /* [in] */ String playListDirectory, 
        /* [in] */ IUri* uri);

    CARAPI_(void) ProcessPlayLists();

    /* native */ CARAPI_(void) ProcessDirectory(
        /* [in] */ String path, 
        /* [in] */ String extensions, 
        /* [in] */ IMediaScannerClient* client);
    
    /* native */ CARAPI_(void) ProcessFile(
        /* [in] */ String path, 
        /* [in] */ String mimeType, 
        /* [in] */ IMediaScannerClient* client);


    static /* native */ CARAPI_(void) native_init();
    /* native */ CARAPI_(void) native_setup();
    /* native */ CARAPI_(void) native_finalize();

    AutoPtr<MyMediaScannerClient> mClient;// = new MyMediaScannerClient();

    /*static {
        System.loadLibrary("media_jni");
        native_init();
    }*/

    static String TAG;// = "MediaScanner";

    static String* AUDIO_PROJECTION;// = new String[] {
    //        Audio.Media._ID, // 0
    //        Audio.Media.DATA, // 1
    //        Audio.Media.DATE_MODIFIED, // 2
    //};

    static const Int32 ID_AUDIO_COLUMN_INDEX = 0;
    static const Int32 PATH_AUDIO_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_AUDIO_COLUMN_INDEX = 2;

    static String* VIDEO_PROJECTION;// = new String[] {
    //        Video.Media._ID, // 0
    //        Video.Media.DATA, // 1
    //        Video.Media.DATE_MODIFIED, // 2
    //};

    static const Int32 ID_VIDEO_COLUMN_INDEX = 0;
    static const Int32 PATH_VIDEO_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_VIDEO_COLUMN_INDEX = 2;

    static String* IMAGES_PROJECTION;// = new String[] {
    //        Images.Media._ID, // 0
    //        Images.Media.DATA, // 1
    //        Images.Media.DATE_MODIFIED, // 2
    //};

    static const Int32 ID_IMAGES_COLUMN_INDEX = 0;
    static const Int32 PATH_IMAGES_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_IMAGES_COLUMN_INDEX = 2;

    static String* PLAYLISTS_PROJECTION;// = new String[] {
    //        Audio.Playlists._ID, // 0
    //        Audio.Playlists.DATA, // 1
    //        Audio.Playlists.DATE_MODIFIED, // 2
    //};

    static String* PLAYLIST_MEMBERS_PROJECTION;// = new String[] {
     //       Audio.Playlists.Members.PLAYLIST_ID, // 0
     //};

    static const Int32 ID_PLAYLISTS_COLUMN_INDEX = 0;
    static const Int32 PATH_PLAYLISTS_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_PLAYLISTS_COLUMN_INDEX = 2;

    static String* GENRE_LOOKUP_PROJECTION;// = new String[] {
    //        Audio.Genres._ID, // 0
    //        Audio.Genres.NAME, // 1
    //};

    static String RINGTONES_DIR;// = "/ringtones/";
    static String NOTIFICATIONS_DIR;// = "/notifications/";
    static String ALARMS_DIR;// = "/alarms/";
    static String MUSIC_DIR;// = "/music/";
    static String PODCAST_DIR;// = "/podcasts/";

    static String* ID3_GENRES;// = {
        // ID3v1 Genres
    //    "Blues",
    //    "Classic Rock",
    //    "Country",
    //    "Dance",
    //    "Disco",
    //    "Funk",
    //    "Grunge",
    //    "Hip-Hop",
    //    "Jazz",
    //    "Metal",
    //    "New Age",
    //    "Oldies",
    //    "Other",
    //    "Pop",
    //    "R&B",
    //    "Rap",
    //    "Reggae",
    //    "Rock",
    //    "Techno",
    //    "Industrial",
    //    "Alternative",
    //    "Ska",
    //    "Death Metal",
    //    "Pranks",
    //    "Soundtrack",
    //    "Euro-Techno",
    //    "Ambient",
    //    "Trip-Hop",
    //    "Vocal",
    //    "Jazz+Funk",
    //    "Fusion",
    //    "Trance",
    //    "Classical",
    //    "Instrumental",
    //    "Acid",
    //    "House",
    //    "Game",
    //    "Sound Clip",
    //    "Gospel",
    //    "Noise",
    //    "AlternRock",
    //    "Bass",
    //    "Soul",
    //    "Punk",
    //    "Space",
    //    "Meditative",
    //    "Instrumental Pop",
    //    "Instrumental Rock",
    //    "Ethnic",
    //    "Gothic",
    //    "Darkwave",
    //    "Techno-Industrial",
    //    "Electronic",
    //    "Pop-Folk",
    //    "Eurodance",
    //    "Dream",
    //    "Southern Rock",
    //    "Comedy",
    //    "Cult",
    //    "Gangsta",
    //    "Top 40",
    //    "Christian Rap",
    //    "Pop/Funk",
    //    "Jungle",
    //    "Native American",
    //    "Cabaret",
    //    "New Wave",
    //    "Psychadelic",
    //    "Rave",
    //    "Showtunes",
    //    "Trailer",
    //    "Lo-Fi",
    //    "Tribal",
    //    "Acid Punk",
    //    "Acid Jazz",
    //    "Polka",
    //    "Retro",
    //    "Musical",
    //    "Rock & Roll",
    //    "Hard Rock",
    //    // The following genres are Winamp extensions
    //    "Folk",
    //    "Folk-Rock",
    //    "National Folk",
    //    "Swing",
    //    "Fast Fusion",
    //    "Bebob",
    //    "Latin",
    //    "Revival",
    //    "Celtic",
    //    "Bluegrass",
    //    "Avantgarde",
    //    "Gothic Rock",
    //    "Progressive Rock",
    //    "Psychedelic Rock",
    //    "Symphonic Rock",
    //    "Slow Rock",
    //    "Big Band",
    //    "Chorus",
    //    "Easy Listening",
    //    "Acoustic",
    //    "Humour",
    //    "Speech",
    //    "Chanson",
    //    "Opera",
    //    "Chamber Music",
    //    "Sonata",
    //    "Symphony",
    //    "Booty Bass",
    //    "Primus",
    //    "Porn Groove",
    //    "Satire",
    //    "Slow Jam",
    //    "Club",
    //    "Tango",
    //    "Samba",
    //    "Folklore",
    //    "Ballad",
    //    "Power Ballad",
    //    "Rhythmic Soul",
    //    "Freestyle",
    //    "Duet",
    //    "Punk Rock",
    //    "Drum Solo",
    //    "A capella",
    //    "Euro-House",
    //    "Dance Hall"
    //};

    Int32 mNativeContext;
    AutoPtr<IContext> mContext;
    AutoPtr<IContentProvider> mMediaProvider;
    AutoPtr<IUri> mAudioUri;
    AutoPtr<IUri> mVideoUri;
    AutoPtr<IUri> mImagesUri;
    AutoPtr<IUri> mThumbsUri;
    AutoPtr<IUri> mGenresUri;
    AutoPtr<IUri> mPlaylistsUri;
    Boolean mProcessPlaylists, mProcessGenres;

    // used when scanning the image database so we know whether we have to prune
    // old thumbnail files
    Int32 mOriginalCount;
    /** Whether the scanner has set a default sound for the ringer ringtone. */
    Boolean mDefaultRingtoneSet;
    /** Whether the scanner has set a default sound for the notification ringtone. */
    Boolean mDefaultNotificationSet;
    /** Whether the scanner has set a default sound for the alarm ringtone. */
    Boolean mDefaultAlarmSet;
    /** The filename for the default sound for the ringer ringtone. */
    String mDefaultRingtoneFilename;
    /** The filename for the default sound for the notification ringtone. */
    String mDefaultNotificationFilename;
    /** The filename for the default sound for the alarm ringtone. */
    String mDefaultAlarmAlertFilename;
    /**
     * The prefix for system properties that define the default sound for
     * ringtones. Concatenate the name of the setting from Settings
     * to get the full system property.
     */
    static String DEFAULT_RINGTONE_PROPERTY_PREFIX;// = "ro.config.";

    // set to true if file path comparisons should be case insensitive.
    // this should be set when scanning files on a case insensitive file system.
    Boolean mCaseInsensitivePaths;

    CBitmapFactoryOptions* mBitmapOptions;// = new BitmapFactory.Options();

    

    // hashes file path to FileCacheEntry.
    // path should be lower case if mCaseInsensitivePaths is true
    HashMap<String, FileCacheEntry*> mFileCache;

    ArrayOf<FileCacheEntry*>* mPlayLists;
    HashMap<String, AutoPtr<IUri> > mGenreCache;
};

#endif
