
#ifndef __CMEDIASCANNER_H__
#define __CMEDIASCANNER_H__

#include "_CMediaScanner.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
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
CarClass(CMediaScanner)
{
public:
    class WplHandler /*implements ElementListener*/
    {
    public:
        WplHandler(
            /* [in] */ const String& playListDirectory,
            /* [in] */ IUri* uri);

        CARAPI Start(
            /* [in] */ IAttributeSet* attributes);

        CARAPI End();

        //virtual CARAPI_(AutoPtr<IContentHandler>) GetContentHandler();

        //AutoPtr<IContentHandler> handler;
        String mPlayListDirectory;
        AutoPtr<IUri> mUri;
        AutoPtr<IContentValues> mValues;// = new ContentValues();
        Int32 mIndex;// = 0;

        CMediaScanner* mOwner;
    };

private:
    class FileCacheEntry : public ElRefBase
    {
    public:
        FileCacheEntry(
            /* [in] */ IUri* tableUri,
            /* [in] */ Int64 rowId,
            /* [in] */ const String& path,
            /* [in] */ Int64 lastModified);

        CARAPI_(String) ToString();

    public:
        AutoPtr<IUri> mTableUri;
        Int64 mRowId;
        String mPath;
        Int64 mLastModified;
        Boolean mSeenInFileSystem;
        Boolean mLastModifiedChanged;
    };

    class MyMediaScannerClient
        : public IMediaScannerClient
        , public ElRefBase
    {
    public:
        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(AutoPtr<FileCacheEntry>) BeginFile(
            /* [in] */ const String& path,
            /* [in] */ const String& mimeType,
            /* [in] */ Int64 lastModified,
            /* [in] */ Int64 fileSize);

        CARAPI ScanFile(
            /* [in] */ const String& path,
            /* [in] */ Int64 lastModified,
            /* [in] */ Int64 fileSize);

        CARAPI ScanFileEx(
            /* [in] */ const String& path,
            /* [in] */ const String& mimeType,
            /* [in] */ Int64 lastModified,
            /* [in] */ Int64 fileSize);

        CARAPI_(AutoPtr<IUri>) DoScanFile(
            /* [in] */ const String& path,
            /* [in] */ const String& mimeType,
            /* [in] */ Int64 lastModified,
            /* [in] */ Int64 fileSize,
            /* [in] */ Boolean scanAlways);

        CARAPI HandleStringTag(
            /* [in] */ const String& name,
            /* [in] */ const String& value);

        CARAPI SetMimeType(
            /* [in] */ const String& mimeType);

        CARAPI AddNoMediaFolder(
            /* [in] */ const String& path);

    private:
        CARAPI_(Int32) ParseSubstring(
            /* [in] */ const String& s,
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
            /* [in] */ const String& path,
            /* [in] */ const String& filename);

        CARAPI_(void) SetSettingIfNotSet(
            /* [in] */ const String& settingName,
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

        CMediaScanner* mOwner;
    }; // end of anonymous MediaScannerClient instance

public:
    CMediaScanner();

    ~CMediaScanner();

    CARAPI constructor(
        /* [in] */ IContext * c);

    CARAPI ScanDirectories(
        /* [in] */ const ArrayOf<String>& directories,
        /* [in] */ const String& volumeName);

    CARAPI ScanSingleFile(
        /* [in] */ const String& path,
        /* [in] */ const String& volumeName,
        /* [in] */ const String& mimeType,
        /* [out] */ IUri ** uri);

    CARAPI SetLocale(
        /* [in] */ const String& locale);

    CARAPI ExtractAlbumArt(
        /* [in] */ IFileDescriptor* fd,
        /* [out, callee] */ ArrayOf<Byte>** albumArts);

private:
    CARAPI_(void) SetDefaultRingtoneFileNames();

    CARAPI Prescan(
        /* [in] */ const String& filePath);

    CARAPI_(Boolean) InScanDirectory(
        /* [in] */ const String& path,
        /* [in] */ const ArrayOf<String>& directories);

    CARAPI_(void) PruneDeadThumbnailFiles();

    CARAPI Postscan(
        /* [in] */ const ArrayOf<String>& directories);

    CARAPI_(void) Initialize(
        /* [in] */ const String& volumeName);

    // returns the number of matching file/directory names, starting from the right
    CARAPI_(Int32) MatchPaths(
        /* [in] */ const String& path1,
        /* [in] */ const String& path2);

    CARAPI_(Boolean) AddPlayListEntry(
        /* [in] */ const String& entry,
        /* [in] */ const String& playListDirectory,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ Int32 index);

    CARAPI_(void) ProcessM3uPlayList(
        /* [in] */ const String& path,
        /* [in] */ const String& playListDirectory,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values);

    CARAPI_(void) ProcessPlsPlayList(
        /* [in] */ const String& path,
        /* [in] */ const String& playListDirectory,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values);

    CARAPI_(void) ProcessWplPlayList(
        /* [in] */ const String& path,
        /* [in] */ const String& playListDirectory,
        /* [in] */ IUri* uri);

    CARAPI ProcessPlayLists();

    CARAPI_(void) ProcessDirectory(
        /* [in] */ const String& path,
        /* [in] */ const String& extensions,
        /* [in] */ IMediaScannerClient* client);

    CARAPI_(void) ProcessFile(
        /* [in] */ const String& path,
        /* [in] */ const String& mimeType,
        /* [in] */ IMediaScannerClient* client);

    static CARAPI_(void) NativeInit();
    CARAPI_(void) NativeSetup();
    CARAPI_(void) NativeFinalize();

private:
    static CString TAG;// = "MediaScanner";

    static CString AUDIO_PROJECTION[];

    static const Int32 ID_AUDIO_COLUMN_INDEX = 0;
    static const Int32 PATH_AUDIO_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_AUDIO_COLUMN_INDEX = 2;

    static CString VIDEO_PROJECTION[];

    static const Int32 ID_VIDEO_COLUMN_INDEX = 0;
    static const Int32 PATH_VIDEO_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_VIDEO_COLUMN_INDEX = 2;

    static CString IMAGES_PROJECTION[];

    static const Int32 ID_IMAGES_COLUMN_INDEX = 0;
    static const Int32 PATH_IMAGES_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_IMAGES_COLUMN_INDEX = 2;

    static CString PLAYLISTS_PROJECTION[];

    static CString PLAYLIST_MEMBERS_PROJECTION[];

    static const Int32 ID_PLAYLISTS_COLUMN_INDEX = 0;
    static const Int32 PATH_PLAYLISTS_COLUMN_INDEX = 1;
    static const Int32 DATE_MODIFIED_PLAYLISTS_COLUMN_INDEX = 2;

    static CString GENRE_LOOKUP_PROJECTION[];

    static CString RINGTONES_DIR;
    static CString NOTIFICATIONS_DIR;
    static CString ALARMS_DIR;
    static CString MUSIC_DIR;
    static CString PODCAST_DIR;

    static CString ID3_GENRES[];
    static const Int32 ID3_GENRES_LENGTH;

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
    static CString DEFAULT_RINGTONE_PROPERTY_PREFIX;

    // set to true if file path comparisons should be case insensitive.
    // this should be set when scanning files on a case insensitive file system.
    Boolean mCaseInsensitivePaths;

    AutoPtr<IBitmapFactoryOptions> mBitmapOptions;

    // hashes file path to FileCacheEntry.
    // path should be lower case if mCaseInsensitivePaths is true
    HashMap<String, AutoPtr<FileCacheEntry> >* mFileCache;

    List< AutoPtr<FileCacheEntry> >* mPlayLists;
    HashMap<String, AutoPtr<IUri> >* mGenreCache;

    AutoPtr<MyMediaScannerClient> mClient;
};

#endif // __CMEDIASCANNER_H__
