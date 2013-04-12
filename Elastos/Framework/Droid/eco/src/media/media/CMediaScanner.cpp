
#include "media/CMediaScanner.h"
#include "media/MediaFile.h"
#include <elastos/System.h>
// #include "os/SystemClock.h"
// #include "os/SystemProperties.h"
// #include "text/TextUtils.h"
// #include "provider/Settings.h"

using namespace Elastos::Core;


CMediaScanner::WplHandler::WplHandler(
    /* [in] */ const String& playListDirectory,
    /* [in] */ IUri* uri)
    : mPlayListDirectory(playListDirectory)
    , mUri(uri)
{
    /*RootElement root = new RootElement("smil");
    Element body = root.getChild("body");
    Element seq = body.getChild("seq");
    Element media = seq.getChild("media");
    media.setElementListener(this);

    this.handler = root.getContentHandler();*/
}

ECode CMediaScanner::WplHandler::Start(
    /* [in] */ IAttributeSet* attributes)
{
    String path;
    attributes->GetAttributeValueEx("", "src", &path);
    if (!path.IsNull()) {
        //values->Clear();
        if (mOwner->AddPlayListEntry(path, mPlayListDirectory, mUri, mValues, mIndex)) {
            mIndex++;
        }
    }
    return NOERROR;
}

ECode CMediaScanner::WplHandler::End()
{
    return NOERROR;
}

//AutoPtr<IContentHandler> CMediaScanner::WplHandler::GetContentHandler()
//{
//    return handler;
//}


CMediaScanner::FileCacheEntry::FileCacheEntry(
    /* [in] */ IUri* tableUri,
    /* [in] */ Int64 rowId,
    /* [in] */ const String& path,
    /* [in] */ Int64 lastModified)
    : mTableUri(tableUri)
    , mRowId(rowId)
    , mPath(path)
    , mLastModified(lastModified)
    , mSeenInFileSystem(FALSE)
    , mLastModifiedChanged(FALSE)
{}

String CMediaScanner::FileCacheEntry::ToString()
{
    return mPath;
}


IInterface* CMediaScanner::MyMediaScannerClient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IObject*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    return NULL;
}

UInt32 CMediaScanner::MyMediaScannerClient::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMediaScanner::MyMediaScannerClient::Release()
{
    return ElRefBase::Release();
}

ECode CMediaScanner::MyMediaScannerClient::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

AutoPtr<CMediaScanner::FileCacheEntry> CMediaScanner::MyMediaScannerClient::BeginFile(
    /* [in] */ const String& path,
    /* [in] */ const String& mimeType,
    /* [in] */ Int64 lastModified,
    /* [in] */ Int64 fileSize)
{
//    // special case certain file names
//    // I use regionMatches() instead of substring() below
//    // to avoid memory allocation
//    Int32 lastSlash = path.LastIndexOf('/');
//    if (lastSlash >= 0 && lastSlash + 2 < path.GetLength()) {
//        // ignore those ._* files created by MacOS
//        if (path.RegionMatches(lastSlash + 1, "._", 0, 2)) {
//            return NULL;
//        }
//
//        // ignore album art files created by Windows Media Player:
//        // Folder.jpg, AlbumArtSmall.jpg, AlbumArt_{...}_Large.jpg and AlbumArt_{...}_Small.jpg
//        if (path.RegionMatches(TRUE, path.Length() - 4, ".jpg", 0, 4)) {
//            if (path.RegionMatches(TRUE, lastSlash + 1, "AlbumArt_{", 0, 10) ||
//                    path.RegionMatches(TRUE, lastSlash + 1, "AlbumArt.", 0, 9)) {
//                return NULL;
//            }
//            Int32 length = path.Length() - lastSlash - 1;
//            if ((length == 17 && path.RegionMatches(TRUE, lastSlash + 1, "AlbumArtSmall", 0, 13)) ||
//                    (length == 10 && path.RegionMatches(TRUE, lastSlash + 1, "Folder", 0, 6))) {
//                return NULL;
//            }
//        }
//    }
//
//    mMimeType = NULL;
//    // try mimeType first, if it is specified
//    if (mimeType != NULL) {
//        mFileType = MediaFile::GetFileTypeForMimeType(mimeType);
//        if (mFileType != 0) {
//            mMimeType = mimeType;
//        }
//    }
//    mFileSize = fileSize;
//
//    // if mimeType was not specified, compute file type based on file extension.
//    if (mMimeType == NULL) {
//        MediaFile::MediaFileType mediaFileType = MediaFile::GetFileType(path);
//        if (mediaFileType != NULL) {
//            mFileType = mediaFileType.fileType;
//            mMimeType = mediaFileType.mimeType;
//        }
//    }
//
//    String key = path;
//    if (mCaseInsensitivePaths) {
//        key = path.ToLowerCase();
//    }
//    FileCacheEntry* entry = mFileCache.get(key);
//    if (entry == NULL) {
//        entry = new FileCacheEntry(NULL, 0, path, 0);
//        mFileCache.put(key, entry);
//    }
//    entry.mSeenInFileSystem = TRUE;
//
//    // add some slack to avoid a rounding error
//    Int64 delta = lastModified - entry.mLastModified;
//    if (delta > 1 || delta < -1) {
//        entry.mLastModified = lastModified;
//        entry.mLastModifiedChanged = TRUE;
//    }
//
//    if (mProcessPlaylists && MediaFile::IsPlayListFileType(mFileType)) {
//        mPlayLists.add(entry);
//        // we don't process playlists in the main scan, so return NULL
//        return NULL;
//    }
//
//    // clear all the metadata
//    mArtist = NULL;
//    mAlbumArtist = NULL;
//    mAlbum = NULL;
//    mTitle = NULL;
//    mComposer = NULL;
//    mGenre = NULL;
//    mTrack = 0;
//    mYear = 0;
//    mDuration = 0;
//    mPath = path;
//    mLastModified = lastModified;
//    mWriter = NULL;
//    mCompilation = 0;
//
//    return entry;
    return NULL;
}

ECode CMediaScanner::MyMediaScannerClient::ScanFile(
    /* [in] */ const String& path,
    /* [in] */ Int64 lastModified,
    /* [in] */ Int64 fileSize)
{
    // This is the callback funtion from native codes.
    // Log.v(TAG, "scanFile: "+path);
    DoScanFile(path, String(NULL), lastModified, fileSize, FALSE);
    return NOERROR;
}

ECode CMediaScanner::MyMediaScannerClient::ScanFileEx(
    /* [in] */ const String& path,
    /* [in] */ const String& mimeType,
    /* [in] */ Int64 lastModified,
    /* [in] */ Int64 fileSize)
{
    DoScanFile(path, mimeType, lastModified, fileSize, FALSE);
    return NOERROR;
}

AutoPtr<IUri> CMediaScanner::MyMediaScannerClient::DoScanFile(
    /* [in] */ const String& path,
    /* [in] */ const String& mimeType,
    /* [in] */ Int64 lastModified,
    /* [in] */ Int64 fileSize,
    /* [in] */ Boolean scanAlways)
{
    AutoPtr<IUri> result;
//    long t1 = System.currentTimeMillis();
    //try {
    AutoPtr<FileCacheEntry> entry = BeginFile(path, mimeType, lastModified, fileSize);
    // rescan for metadata if file was modified since last scan
    if (entry != NULL && (entry->mLastModifiedChanged || scanAlways)) {
        String lowpath = path;
        lowpath.ToLowerCase();
        Boolean ringtones = (lowpath.IndexOf(RINGTONES_DIR) > 0);
        Boolean notifications = (lowpath.IndexOf(NOTIFICATIONS_DIR) > 0);
        Boolean alarms = (lowpath.IndexOf(ALARMS_DIR) > 0);
        Boolean podcasts = (lowpath.IndexOf(PODCAST_DIR) > 0);
        Boolean music = (lowpath.IndexOf(MUSIC_DIR) > 0) ||
            (!ringtones && !notifications && !alarms && !podcasts);

        if (!MediaFile::IsImageFileType(mFileType)) {
            mOwner->ProcessFile(path, mimeType, this);
        }

        result = EndFile(entry, ringtones, notifications, alarms, music, podcasts);
    }
    /*} catch (RemoteException e) {
        Log.e(TAG, "RemoteException in CMediaScanner.scanFile()", e);
    }*/
//            Int64 t2 = System.currentTimeMillis();
//            Log.v(TAG, "scanFile: " + path + " took " + (t2-t1));
    return result;
}

Int32 CMediaScanner::MyMediaScannerClient::ParseSubstring(
    /* [in] */ const String& s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 defaultValue)
{
    Int32 length = s.GetSize();
    if (start == length) return defaultValue;

    Char32 ch = s.GetChar(start++);
    // return defaultValue if we have no integer at all
    if (ch < '0' || ch > '9') return defaultValue;

    Int32 result = ch - '0';
    while (start < length) {
        ch = s.GetChar(start++);
        if (ch < '0' || ch > '9') return result;
        result = result * 10 + (ch - '0');
    }

    return result;
}

ECode CMediaScanner::MyMediaScannerClient::HandleStringTag(
    /* [in] */ const String& name,
    /* [in] */ const String& _value)
{
    String value = _value;
    if (name.EqualsIgnoreCase("title") || name.StartWith("title;")) {
        // Don't trim() here, to preserve the special \001 character
        // used to force sorting. The media provider will trim() before
        // inserting the title in to the database.
        mTitle = value;
    }
    else if (name.EqualsIgnoreCase("artist") || name.StartWith("artist;")) {
        mArtist = value.Trim();
    }
    else if (name.EqualsIgnoreCase("albumartist") || name.StartWith("albumartist;")) {
        mAlbumArtist = value.Trim();
    }
    else if (name.EqualsIgnoreCase("album") || name.StartWith("album;")) {
        mAlbum = value.Trim();
    }
    else if (name.EqualsIgnoreCase("composer") || name.StartWith("composer;")) {
        mComposer = value.Trim();
    }
    else if (name.EqualsIgnoreCase("genre") || name.StartWith("genre;")) {
        // handle numeric genres, which PV sometimes encodes like "(20)"
        if (value.GetLength() > 0) {
            Int32 genreCode = -1;
            Char32 ch = value.GetChar(0);
            if (ch == '(') {
                genreCode = ParseSubstring(value, 1, -1);
            }
            else if (ch >= '0' && ch <= '9') {
                genreCode = ParseSubstring(value, 0, -1);
            }
            if (genreCode >= 0 && genreCode < ID3_GENRES_LENGTH) {
                value = ID3_GENRES[genreCode];
            }
            else if (genreCode == 255) {
                // 255 is defined to be unknown
                value = NULL;
            }
        }
        mGenre = value;
    }
    else if (name.EqualsIgnoreCase("year") || name.StartWith("year;")) {
        mYear = ParseSubstring(value, 0, 0);
    }
    else if (name.EqualsIgnoreCase("tracknumber") || name.StartWith("tracknumber;")) {
        // track number might be of the form "2/12"
        // we just read the number before the slash
        Int32 num = ParseSubstring(value, 0, 0);
        mTrack = (mTrack / 1000) * 1000 + num;
    }
    else if (name.EqualsIgnoreCase("discnumber") ||
            name.Equals("set") || name.StartWith("set;")) {
        // set number might be of the form "1/3"
        // we just read the number before the slash
        Int32 num = ParseSubstring(value, 0, 0);
        mTrack = (num * 1000) + (mTrack % 1000);
    }
    else if (name.EqualsIgnoreCase("duration")) {
        mDuration = ParseSubstring(value, 0, 0);
    }
    else if (name.EqualsIgnoreCase("writer") || name.StartWith("writer;")) {
        mWriter = value.Trim();
    }
    else if (name.EqualsIgnoreCase("compilation")) {
        mCompilation = ParseSubstring(value, 0, 0);
    }

    return NOERROR;
}

ECode CMediaScanner::MyMediaScannerClient::SetMimeType(
    /* [in] */ const String& mimeType)
{
    if (CString("audio/mp4").Equals(mMimeType) &&
           mimeType.StartWith("video")) {
    //    // for feature parity with Donut, we force m4a files to keep the
    //    // audio/mp4 mimetype, even if they are really "enhanced podcasts"
    //    // with a video track
        return NOERROR;
    }
    mMimeType = mimeType;
    mFileType = MediaFile::GetFileTypeForMimeType(mimeType);
    return NOERROR;
}

/**
 * Formats the data into a values array suitable for use with the Media
 * Content Provider.
 *
 * @return a map of values
 */
AutoPtr<IContentValues> CMediaScanner::MyMediaScannerClient::ToValues()
{
    AutoPtr<IContentValues> map;
    CContentValues::New((IContentValues**)&map);

    //map.PutString(MediaStore.MediaColumns.DATA, mPath);
    //map.PutString(MediaStore.MediaColumns.TITLE, mTitle);
    //map.PutString(MediaStore.MediaColumns.DATE_MODIFIED, mLastModified);
    //map.PutString(MediaStore.MediaColumns.SIZE, mFileSize);
    //map.PutString(MediaStore.MediaColumns.MIME_TYPE, mMimeType);

    //if (MediaFile.isVideoFileType(mFileType)) {
    //    map.PutString(Video.Media.ARTIST, (mArtist != NULL && mArtist.length() > 0 ? mArtist : MediaStore.UNKNOWN_STRING));
    //    map.PutString(Video.Media.ALBUM, (mAlbum != NULL && mAlbum.length() > 0 ? mAlbum : MediaStore.UNKNOWN_STRING));
    //    map.PutString(Video.Media.DURATION, mDuration);
    //    // FIXME - add RESOLUTION
    //} else if (MediaFile.isImageFileType(mFileType)) {
    //    // FIXME - add DESCRIPTION
    //} else if (MediaFile.isAudioFileType(mFileType)) {
    //    map.PutString(Audio.Media.ARTIST, (mArtist != NULL && mArtist.length() > 0) ?
    //            mArtist : MediaStore.UNKNOWN_STRING);
    //    map.PutString(Audio.Media.ALBUM_ARTIST, (mAlbumArtist != NULL &&
    //            mAlbumArtist.length() > 0) ? mAlbumArtist : NULL);
    //    map.PutString(Audio.Media.ALBUM, (mAlbum != NULL && mAlbum.length() > 0) ?
    //            mAlbum : MediaStore.UNKNOWN_STRING);
    //    map.PutString(Audio.Media.COMPOSER, mComposer);
    //    if (mYear != 0) {
    //        map.PutString(Audio.Media.YEAR, mYear);
    //    }
    //    map.PutString(Audio.Media.TRACK, mTrack);
    //    map.PutString(Audio.Media.DURATION, mDuration);
    //    map.PutString(Audio.Media.COMPILATION, mCompilation);
    //}
    return map;
}

AutoPtr<IUri> CMediaScanner::MyMediaScannerClient::EndFile(
    /* [in] */ FileCacheEntry* entry,
    /* [in] */ Boolean ringtones,
    /* [in] */ Boolean notifications,
    /* [in] */ Boolean alarms,
    /* [in] */ Boolean music,
    /* [in] */ Boolean podcasts)
{
    // update database
    AutoPtr<IUri> tableUri;
    Boolean isAudio = MediaFile::IsAudioFileType(mFileType);
    Boolean isVideo = MediaFile::IsVideoFileType(mFileType);
    Boolean isImage = MediaFile::IsImageFileType(mFileType);
    if (isVideo) {
        tableUri = mOwner->mVideoUri;
    }
    else if (isImage) {
        tableUri = mOwner->mImagesUri;
    }
    else if (isAudio) {
        tableUri = mOwner->mAudioUri;
    }
    else {
        // don't add file to database if not audio, video or image
        return NULL;
    }
    entry->mTableUri = tableUri;

     // use album artist if artist is missing
    if (mArtist.IsNull() || mArtist.GetLength() == 0) {
        mArtist = mAlbumArtist;
    }

    AutoPtr<IContentValues> values = ToValues();
    String title;// = values->GetAsString(MediaStore.MediaColumns.TITLE);
    if (title.IsNull()/* || TextUtils::IsEmpty(title.Trim())*/) {
        //title = values.getAsString(MediaStore.MediaColumns.DATA);
        // extract file name after last slash
        Int32 lastSlash = title.LastIndexOf('/');
        if (lastSlash >= 0) {
            lastSlash++;
            if (lastSlash < title.GetLength()) {
                title = title.Substring(lastSlash);
            }
        }
        // truncate the file extension (if any)
        Int32 lastDot = title.LastIndexOf('.');
        if (lastDot > 0) {
            title = title.Substring(0, lastDot);
        }
        //values.put(MediaStore.MediaColumns.TITLE, title);
    }
    String album;// = values.getAsString(Audio.Media.ALBUM);
    //if (MediaStore.UNKNOWN_STRING.equals(album)) {
    //    album = values.getAsString(MediaStore.MediaColumns.DATA);
    //    // extract last path segment before file name
    //    Int32 lastSlash = album.lastIndexOf('/');
    //    if (lastSlash >= 0) {
    //        Int32 previousSlash = 0;
    //        while (TRUE) {
    //            Int32 idx = album.indexOf('/', previousSlash + 1);
    //            if (idx < 0 || idx >= lastSlash) {
    //                break;
    //            }
    //            previousSlash = idx;
    //        }
    //        if (previousSlash != 0) {
    //            album = album.substring(previousSlash + 1, lastSlash);
    //            values.put(Audio.Media.ALBUM, album);
    //        }
    //    }
    //}
    Int64 rowId = entry->mRowId;

//    if (isAudio && rowId == 0) {
//        // Only set these for new entries. For existing entries, they
//        // may have been modified later, and we want to keep the current
//        // values so that custom ringtones still show up in the ringtone
//        // picker.
//        values.PutString(Audio.Media.IS_RINGTONE, ringtones);
//        values.PutString(Audio.Media.IS_NOTIFICATION, notifications);
//        values.PutString(Audio.Media.IS_ALARM, alarms);
//        values.PutString(Audio.Media.IS_MUSIC, music);
//        values.PutString(Audio.Media.IS_PODCAST, podcasts);
//    } else if (mFileType == MediaFile::FILE_TYPE_JPEG) {
//        ExifInterface exif = NULL;
//        try {
//            exif = new ExifInterface(entry.mPath);
//        } catch (IOException ex) {
//            // exif is NULL
//        }
//        if (exif != NULL) {
//            float[] latlng = new float[2];
//            if (exif.getLatLong(latlng)) {
//                values.put(Images.Media.LATITUDE, latlng[0]);
//                values.put(Images.Media.LONGITUDE, latlng[1]);
//            }
//
//            Int64 time = exif.getGpsDateTime();
//            if (time != -1) {
//                values.put(Images.Media.DATE_TAKEN, time);
//            }
//
//            Int32 orientation = exif.getAttributeInt(
//                ExifInterface.TAG_ORIENTATION, -1);
//            if (orientation != -1) {
//                // We only recognize a subset of orientation tag values.
//                Int32 degree;
//                switch(orientation) {
//                    case ExifInterface.ORIENTATION_ROTATE_90:
//                        degree = 90;
//                        break;
//                    case ExifInterface.ORIENTATION_ROTATE_180:
//                        degree = 180;
//                        break;
//                    case ExifInterface.ORIENTATION_ROTATE_270:
//                        degree = 270;
//                        break;
//                    default:
//                        degree = 0;
//                        break;
//                }
//                values.put(Images.Media.ORIENTATION, degree);
//            }
//        }
//    }
//
//    AutoPtr<IUri> result = NULL;
//    if (rowId == 0) {
//        // new file, insert it
//        result = mMediaProvider->Insert(tableUri, values);
//        if (result != NULL) {
//            rowId = ContentUris::ParseId(result);
//            entry->mRowId = rowId;
//        }
//    } else {
//        // updated file
//        result = ContentUris.withAppendedId(tableUri, rowId);
//        mMediaProvider.update(result, values, NULL, NULL);
//    }
//    if (mProcessGenres && mGenre != NULL) {
//        String genre = mGenre;
//        Uri uri = mGenreCache.get(genre);
//        if (uri == NULL) {
//            Cursor cursor = NULL;
//            try {
//                // see if the genre already exists
//                cursor = mMediaProvider.query(
//                        mGenresUri,
//                        GENRE_LOOKUP_PROJECTION, MediaStore.Audio.Genres.NAME + "=?",
//                                new String[] { genre }, NULL);
//                if (cursor == NULL || cursor.getCount() == 0) {
//                    // genre does not exist, so create the genre in the genre table
//                    values.clear();
//                    values.put(MediaStore.Audio.Genres.NAME, genre);
//                    uri = mMediaProvider.insert(mGenresUri, values);
//                } else {
//                    // genre already exists, so compute its Uri
//                    cursor.moveToNext();
//                    uri = ContentUris.withAppendedId(mGenresUri, cursor.getLong(0));
//                }
//                if (uri != NULL) {
//                    uri = Uri.withAppendedPath(uri, Genres.Members.CONTENT_DIRECTORY);
//                    mGenreCache.put(genre, uri);
//                }
//            } finally {
//                // release the cursor if it exists
//                if (cursor != NULL) {
//                    cursor.close();
//                }
//            }
//        }
//
//        if (uri != NULL) {
//            // add entry to audio_genre_map
//            values.clear();
//            values.put(MediaStore.Audio.Genres.Members.AUDIO_ID, Long.valueOf(rowId));
//            mMediaProvider.insert(uri, values);
//        }
//    }
//
//    if (notifications && !mDefaultNotificationSet) {
//        if (TextUtils.isEmpty(mDefaultNotificationFilename) ||
//                doesPathHaveFilename(entry.mPath, mDefaultNotificationFilename)) {
//            setSettingIfNotSet(Settings.System.NOTIFICATION_SOUND, tableUri, rowId);
//            mDefaultNotificationSet = TRUE;
//        }
//    } else if (ringtones && !mDefaultRingtoneSet) {
//        if (TextUtils::IsEmpty(mDefaultRingtoneFilename) ||
//                DoesPathHaveFilename(entry.mPath, mDefaultRingtoneFilename)) {
//            SetSettingIfNotSet(Settings.System.RINGTONE, tableUri, rowId);
//            mDefaultRingtoneSet = TRUE;
//        }
//    } else if (alarms && !mDefaultAlarmSet) {
//        if (TextUtils::IsEmpty(mDefaultAlarmAlertFilename) ||
//                doesPathHaveFilename(entry.mPath, mDefaultAlarmAlertFilename)) {
//            SetSettingIfNotSet(Settings.System.ALARM_ALERT, tableUri, rowId);
//            mDefaultAlarmSet = TRUE;
//        }
//    }
//
//    return result;

    return NULL;
}

Boolean CMediaScanner::MyMediaScannerClient::DoesPathHaveFilename(
    /* [in] */ const String& path,
    /* [in] */ const String& filename)
{
    //Int32 pathFilenameStart = path.LastIndexOf(File::separatorChar) + 1;
    Int32 filenameLength = filename.GetLength();
    /*return path.RegionMatches(pathFilenameStart, filename, 0, filenameLength) &&
            pathFilenameStart + filenameLength == path.GetLength();*/

    return FALSE;
}

void CMediaScanner::MyMediaScannerClient::SetSettingIfNotSet(
    /* [in] */ const String& settingName,
    /* [in] */ IUri* uri,
    /* [in] */ Int64 rowId)
{
    AutoPtr<IContentResolver> cr;
    mOwner->mContext->GetContentResolver((IContentResolver**)&cr);

    String existingSettingValue;
    //Settings::System::GetString(cr, settingName, &existingSettingValue);

    //if (TextUtils::IsEmpty(existingSettingValue)) {
    //    // Set the setting to the given URI
    //    mOwner->mContext->GetContentResolver((IContentResolver**)&cr);

    //    Settings::System::PutString(cr, settingName,
    //        ContentUris::WithAppendedId(uri, rowId).ToString());
    //}
}

ECode CMediaScanner::MyMediaScannerClient::AddNoMediaFolder(
    /* [in] */ const String& path)
{
    //ContentValues values = new ContentValues();
    //values.put(MediaStore.Images.ImageColumns.DATA, "");
    //String [] pathSpec = new String[] {path + '%'};
    ////try {
    //    // These tables have DELETE_FILE triggers that delete the file from the
    //    // sd card when deleting the database entry. We don't want to do this in
    //    // this case, since it would cause those files to be removed if a .nomedia
    //    // file was added after the fact, when in that case we only want the database
    //    // entries to be removed.
    //    mMediaProvider.update(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values,
    //            MediaStore.Images.ImageColumns.DATA + " LIKE ?", pathSpec);
    //    mMediaProvider.update(MediaStore.Video.Media.EXTERNAL_CONTENT_URI, values,
    //            MediaStore.Images.ImageColumns.DATA + " LIKE ?", pathSpec);
    ///*} catch (RemoteException e) {
    //    throw new RuntimeException();
    //}*/

    return NOERROR;
}


CString CMediaScanner::TAG = "MediaScanner";

CString CMediaScanner::AUDIO_PROJECTION[] = {
       // Audio.Media._ID, // 0
       // Audio.Media.DATA, // 1
       // Audio.Media.DATE_MODIFIED, // 2
};

const Int32 CMediaScanner::ID_AUDIO_COLUMN_INDEX;
const Int32 CMediaScanner::PATH_AUDIO_COLUMN_INDEX;
const Int32 CMediaScanner::DATE_MODIFIED_AUDIO_COLUMN_INDEX;

CString CMediaScanner::VIDEO_PROJECTION[] = {
       // Video.Media._ID, // 0
       // Video.Media.DATA, // 1
       // Video.Media.DATE_MODIFIED, // 2
};

const Int32 CMediaScanner::ID_VIDEO_COLUMN_INDEX;
const Int32 CMediaScanner::PATH_VIDEO_COLUMN_INDEX;
const Int32 CMediaScanner::DATE_MODIFIED_VIDEO_COLUMN_INDEX;

CString CMediaScanner::IMAGES_PROJECTION[] = {
//        Images.Media._ID, // 0
//        Images.Media.DATA, // 1
//        Images.Media.DATE_MODIFIED, // 2
};

const Int32 CMediaScanner::ID_IMAGES_COLUMN_INDEX;
const Int32 CMediaScanner::PATH_IMAGES_COLUMN_INDEX;
const Int32 CMediaScanner::DATE_MODIFIED_IMAGES_COLUMN_INDEX;

CString CMediaScanner::PLAYLISTS_PROJECTION[] = {
//        Audio.Playlists._ID, // 0
//        Audio.Playlists.DATA, // 1
//        Audio.Playlists.DATE_MODIFIED, // 2
};

CString CMediaScanner::PLAYLIST_MEMBERS_PROJECTION[] = {
//       Audio.Playlists.Members.PLAYLIST_ID, // 0
};

const Int32 CMediaScanner::ID_PLAYLISTS_COLUMN_INDEX;
const Int32 CMediaScanner::PATH_PLAYLISTS_COLUMN_INDEX;
const Int32 CMediaScanner::DATE_MODIFIED_PLAYLISTS_COLUMN_INDEX;

CString CMediaScanner::GENRE_LOOKUP_PROJECTION[] = {
//        Audio.Genres._ID, // 0
//        Audio.Genres.NAME, // 1
};

CString CMediaScanner::RINGTONES_DIR = "/ringtones/";
CString CMediaScanner::NOTIFICATIONS_DIR = "/notifications/";
CString CMediaScanner::ALARMS_DIR = "/alarms/";
CString CMediaScanner::MUSIC_DIR = "/music/";
CString CMediaScanner::PODCAST_DIR = "/podcasts/";

CString CMediaScanner::ID3_GENRES[] = {
    // ID3v1 Genres
    "Blues",
    "Classic Rock",
    "Country",
    "Dance",
    "Disco",
    "Funk",
    "Grunge",
    "Hip-Hop",
    "Jazz",
    "Metal",
    "New Age",
    "Oldies",
    "Other",
    "Pop",
    "R&B",
    "Rap",
    "Reggae",
    "Rock",
    "Techno",
    "Industrial",
    "Alternative",
    "Ska",
    "Death Metal",
    "Pranks",
    "Soundtrack",
    "Euro-Techno",
    "Ambient",
    "Trip-Hop",
    "Vocal",
    "Jazz+Funk",
    "Fusion",
    "Trance",
    "Classical",
    "Instrumental",
    "Acid",
    "House",
    "Game",
    "Sound Clip",
    "Gospel",
    "Noise",
    "AlternRock",
    "Bass",
    "Soul",
    "Punk",
    "Space",
    "Meditative",
    "Instrumental Pop",
    "Instrumental Rock",
    "Ethnic",
    "Gothic",
    "Darkwave",
    "Techno-Industrial",
    "Electronic",
    "Pop-Folk",
    "Eurodance",
    "Dream",
    "Southern Rock",
    "Comedy",
    "Cult",
    "Gangsta",
    "Top 40",
    "Christian Rap",
    "Pop/Funk",
    "Jungle",
    "Native American",
    "Cabaret",
    "New Wave",
    "Psychadelic",
    "Rave",
    "Showtunes",
    "Trailer",
    "Lo-Fi",
    "Tribal",
    "Acid Punk",
    "Acid Jazz",
    "Polka",
    "Retro",
    "Musical",
    "Rock & Roll",
    "Hard Rock",
    // The following genres are Winamp extensions
    "Folk",
    "Folk-Rock",
    "National Folk",
    "Swing",
    "Fast Fusion",
    "Bebob",
    "Latin",
    "Revival",
    "Celtic",
    "Bluegrass",
    "Avantgarde",
    "Gothic Rock",
    "Progressive Rock",
    "Psychedelic Rock",
    "Symphonic Rock",
    "Slow Rock",
    "Big Band",
    "Chorus",
    "Easy Listening",
    "Acoustic",
    "Humour",
    "Speech",
    "Chanson",
    "Opera",
    "Chamber Music",
    "Sonata",
    "Symphony",
    "Booty Bass",
    "Primus",
    "Porn Groove",
    "Satire",
    "Slow Jam",
    "Club",
    "Tango",
    "Samba",
    "Folklore",
    "Ballad",
    "Power Ballad",
    "Rhythmic Soul",
    "Freestyle",
    "Duet",
    "Punk Rock",
    "Drum Solo",
    "A capella",
    "Euro-House",
    "Dance Hall"
};
const Int32 CMediaScanner::ID3_GENRES_LENGTH = 126;

CString CMediaScanner::DEFAULT_RINGTONE_PROPERTY_PREFIX = "ro.config.";

CMediaScanner::CMediaScanner()
    : mProcessPlaylists(FALSE)
    , mProcessGenres(FALSE)
    , mDefaultRingtoneSet(FALSE)
    , mDefaultNotificationSet(FALSE)
    , mDefaultAlarmSet(FALSE)
    , mCaseInsensitivePaths(FALSE)
    , mFileCache(NULL)
    , mPlayLists(NULL)
    , mGenreCache(NULL)
{
    CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&mBitmapOptions);
    mClient = new MyMediaScannerClient();
}

CMediaScanner::~CMediaScanner()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Boolean res;
    cr->ReleaseProvider(mMediaProvider, &res);
    NativeFinalize();
}

ECode CMediaScanner::constructor(
    /* [in] */ IContext * c)
{
    NativeSetup();
    mContext = c;
    //mBitmapOptions.inSampleSize = 1;
    //mBitmapOptions.inJustDecodeBounds = TRUE;

    SetDefaultRingtoneFileNames();
    return NOERROR;
}

void CMediaScanner::SetDefaultRingtoneFileNames()
{
    // mDefaultRingtoneFilename = SystemProperties::Get(DEFAULT_RINGTONE_PROPERTY_PREFIX
    //         + SettingsSystem_RINGTONE);
    // mDefaultNotificationFilename = SystemProperties::Get(DEFAULT_RINGTONE_PROPERTY_PREFIX
    //         + SettingsSystem_NOTIFICATION_SOUND);
    // mDefaultAlarmAlertFilename = SystemProperties::Get(DEFAULT_RINGTONE_PROPERTY_PREFIX
    //         + SettingsSystem_ALARM_ALERT);
}

ECode CMediaScanner::Prescan(
    /* [in] */ const String& filePath)
{
//    Cursor c = NULL;
//    String where = NULL;
//    String[] selectionArgs = NULL;
//
//    if (mFileCache == NULL) {
//        mFileCache = new HashMap<String, FileCacheEntry>();
//    } else {
//        mFileCache.clear();
//    }
//    if (mPlayLists == NULL) {
//        mPlayLists = new ArrayList<FileCacheEntry>();
//    } else {
//        mPlayLists.clear();
//    }
//
//    // Build the list of files from the content provider
//    try {
//        // Read existing files from the audio table
//        if (filePath != NULL) {
//            where = MediaStore.Audio.Media.DATA + "=?";
//            selectionArgs = new String[] { filePath };
//        }
//        c = mMediaProvider.query(mAudioUri, AUDIO_PROJECTION, where, selectionArgs, NULL);
//
//        if (c != NULL) {
//            try {
//                while (c.moveToNext()) {
//                    Int64 rowId = c.getLong(ID_AUDIO_COLUMN_INDEX);
//                    String path = c.getString(PATH_AUDIO_COLUMN_INDEX);
//                    Int64 lastModified = c.getLong(DATE_MODIFIED_AUDIO_COLUMN_INDEX);
//
//                    // Only consider entries with absolute path names.
//                    // This allows storing URIs in the database without the
//                    // media scanner removing them.
//                    if (path.startsWith("/")) {
//                        String key = path;
//                        if (mCaseInsensitivePaths) {
//                            key = path.toLowerCase();
//                        }
//                        mFileCache.put(key, new FileCacheEntry(mAudioUri, rowId, path,
//                                lastModified));
//                    }
//                }
//            } finally {
//                c.close();
//                c = NULL;
//            }
//        }
//
//        // Read existing files from the video table
//        if (filePath != NULL) {
//            where = MediaStore.Video.Media.DATA + "=?";
//        } else {
//            where = NULL;
//        }
//        c = mMediaProvider.query(mVideoUri, VIDEO_PROJECTION, where, selectionArgs, NULL);
//
//        if (c != NULL) {
//            try {
//                while (c.moveToNext()) {
//                    Int64 rowId = c.getLong(ID_VIDEO_COLUMN_INDEX);
//                    String path = c.getString(PATH_VIDEO_COLUMN_INDEX);
//                    Int64 lastModified = c.getLong(DATE_MODIFIED_VIDEO_COLUMN_INDEX);
//
//                    // Only consider entries with absolute path names.
//                    // This allows storing URIs in the database without the
//                    // media scanner removing them.
//                    if (path.startsWith("/")) {
//                        String key = path;
//                        if (mCaseInsensitivePaths) {
//                            key = path.toLowerCase();
//                        }
//                        mFileCache.put(key, new FileCacheEntry(mVideoUri, rowId, path,
//                                lastModified));
//                    }
//                }
//            } finally {
//                c.close();
//                c = NULL;
//            }
//        }
//
//        // Read existing files from the images table
//        if (filePath != NULL) {
//            where = MediaStore.Images.Media.DATA + "=?";
//        } else {
//            where = NULL;
//        }
//        mOriginalCount = 0;
//        c = mMediaProvider.query(mImagesUri, IMAGES_PROJECTION, where, selectionArgs, NULL);
//
//        if (c != NULL) {
//            try {
//                mOriginalCount = c.getCount();
//                while (c.moveToNext()) {
//                    Int64 rowId = c.getLong(ID_IMAGES_COLUMN_INDEX);
//                    String path = c.getString(PATH_IMAGES_COLUMN_INDEX);
//                   Int64 lastModified = c.getLong(DATE_MODIFIED_IMAGES_COLUMN_INDEX);
//
//                   // Only consider entries with absolute path names.
//                   // This allows storing URIs in the database without the
//                   // media scanner removing them.
//                   if (path.startsWith("/")) {
//                       String key = path;
//                       if (mCaseInsensitivePaths) {
//                           key = path.toLowerCase();
//                       }
//                       mFileCache.put(key, new FileCacheEntry(mImagesUri, rowId, path,
//                               lastModified));
//                   }
//                }
//            } finally {
//                c.close();
//                c = NULL;
//            }
//        }
//
//        if (mProcessPlaylists) {
//            // Read existing files from the playlists table
//            if (filePath != NULL) {
//                where = MediaStore.Audio.Playlists.DATA + "=?";
//            } else {
//                where = NULL;
//            }
//            c = mMediaProvider.query(mPlaylistsUri, PLAYLISTS_PROJECTION, where, selectionArgs, NULL);
//
//            if (c != NULL) {
//                try {
//                    while (c.moveToNext()) {
//                        String path = c.getString(PATH_PLAYLISTS_COLUMN_INDEX);
//
//                        if (path != NULL && path.length() > 0) {
//                            Int64 rowId = c.getLong(ID_PLAYLISTS_COLUMN_INDEX);
//                            Int64 lastModified = c.getLong(DATE_MODIFIED_PLAYLISTS_COLUMN_INDEX);
//
//                            String key = path;
//                            if (mCaseInsensitivePaths) {
//                                key = path.toLowerCase();
//                            }
//                            mFileCache.put(key, new FileCacheEntry(mPlaylistsUri, rowId, path,
//                                    lastModified));
//                        }
//                    }
//                } finally {
//                    c.close();
//                    c = NULL;
//                }
//            }
//        }
//    }
//    finally {
//        if (c != NULL) {
//            c.close();
//        }
//    }

    return E_NOT_IMPLEMENTED;
}

Boolean CMediaScanner::InScanDirectory(
    /* [in] */ const String& path,
    /* [in] */ const ArrayOf<String>& directories)
{
    for (Int32 i = 0; i < directories.GetLength(); i++) {
        if (path.StartWith(directories[i])) {
            return TRUE;
        }
    }
}

void CMediaScanner::PruneDeadThumbnailFiles()
{
    //HashSet<String> existingFiles = new HashSet<String>();
    //String directory = "/sdcard/DCIM/.thumbnails";
    //String [] files = (new File(directory)).list();
    //if (files == NULL)
    //    files = new String[0];

    //for (Int32 i = 0; i < files.length; i++) {
    //    String fullPathString = directory + "/" + files[i];
    //    existingFiles.add(fullPathString);
    //}

    ////try {
    //    Cursor c = mMediaProvider.query(
    //            mThumbsUri,
    //            new String [] { "_data" },
    //            NULL,
    //            NULL,
    //            NULL);
    //    Log.v(TAG, "pruneDeadThumbnailFiles... " + c);
    //    if (c != NULL && c.moveToFirst()) {
    //        do {
    //            String fullPathString = c.getString(0);
    //            existingFiles.remove(fullPathString);
    //        } while (c.moveToNext());
    //    }

    //    for (String fileToDelete : existingFiles) {
    //        if (Config.LOGV)
    //            Log.v(TAG, "fileToDelete is " + fileToDelete);
    //        try {
    //            (new File(fileToDelete)).delete();
    //        } catch (SecurityException ex) {
    //        }
    //    }

    //    Log.v(TAG, "/pruneDeadThumbnailFiles... " + c);
    //    if (c != NULL) {
    //        c.close();
    //    }
    ////} catch (RemoteException e) {
    ////    // We will soon be killed...
    ////}
}

ECode CMediaScanner::Postscan(
    /* [in] */ const ArrayOf<String>& directories)
{
    //Iterator<FileCacheEntry> iterator = mFileCache.values().iterator();

    //while (iterator.hasNext()) {
    //    FileCacheEntry entry = iterator.next();
    //    String path = entry.mPath;

    //    // remove database entries for files that no longer exist.
    //    Boolean fileMissing = FALSE;

    //    if (!entry.mSeenInFileSystem) {
    //        if (inScanDirectory(path, directories)) {
    //            // we didn't see this file in the scan directory.
    //            fileMissing = TRUE;
    //        } else {
    //            // the file is outside of our scan directory,
    //            // so we need to check for file existence here.
    //            File testFile = new File(path);
    //            if (!testFile.exists()) {
    //                fileMissing = TRUE;
    //            }
    //        }
    //    }

    //    if (fileMissing) {
    //        // do not delete missing playlists, since they may have been modified by the user.
    //        // the user can delete them in the media player instead.
    //        // instead, clear the path and lastModified fields in the row
    //        MediaFile.MediaFileType mediaFileType = MediaFile.getFileType(path);
    //        Int32 fileType = (mediaFileType == NULL ? 0 : mediaFileType.fileType);

    //        if (MediaFile.isPlayListFileType(fileType)) {
    //            ContentValues values = new ContentValues();
    //            values.put(MediaStore.Audio.Playlists.DATA, "");
    //            values.put(MediaStore.Audio.Playlists.DATE_MODIFIED, 0);
    //            mMediaProvider.update(ContentUris.withAppendedId(mPlaylistsUri, entry.mRowId), values, NULL, NULL);
    //        } else {
    //            mMediaProvider.delete(ContentUris.withAppendedId(entry.mTableUri, entry.mRowId), NULL, NULL);
    //            iterator.remove();
    //        }
    //    }
    //}

    //// handle playlists last, after we know what media files are on the storage.
    //if (mProcessPlaylists) {
    //    processPlayLists();
    //}

    //if (mOriginalCount == 0 && mImagesUri.equals(Images.Media.getContentUri("external")))
    //    pruneDeadThumbnailFiles();

    // allow GC to clean up
    /*mGenreCache = NULL;
    mPlayLists = NULL;
    mFileCache = NULL;
    mMediaProvider = NULL;*/

    return E_NOT_IMPLEMENTED;
}

void CMediaScanner::Initialize(
    /* [in] */ const String& volumeName)
{
    //mMediaProvider = mContext.getContentResolver().acquireProvider("media");

    //mAudioUri = Audio.Media.getContentUri(volumeName);
    //mVideoUri = Video.Media.getContentUri(volumeName);
    //mImagesUri = Images.Media.getContentUri(volumeName);
    //mThumbsUri = Images.Thumbnails.getContentUri(volumeName);

    //if (!volumeName.equals("internal")) {
    //    // we only support playlists on external media
    //    mProcessPlaylists = TRUE;
    //    mProcessGenres = TRUE;
    //    mGenreCache = new HashMap<String, Uri>();
    //    mGenresUri = Genres.getContentUri(volumeName);
    //    mPlaylistsUri = Playlists.getContentUri(volumeName);
    //    // assuming external storage is FAT (case insensitive), except on the simulator.
    //    if ( Process.supportsProcesses()) {
    //        mCaseInsensitivePaths = TRUE;
    //    }
    //}
}

ECode CMediaScanner::ScanDirectories(
    /* [in] */ const ArrayOf<String>& directories,
    /* [in] */ const String& volumeName)
{
    //try {
    Int64 start = System::GetCurrentTimeMillis();
    Initialize(volumeName);
    Prescan(String());
    Int64 prescan = System::GetCurrentTimeMillis();

    for (Int32 i = 0; i < directories.GetLength(); i++) {
        ProcessDirectory(directories[i], MediaFile::sFileExtensions, mClient);
    }
    Int64 scan = System::GetCurrentTimeMillis();
    Postscan(directories);
    Int64 end = System::GetCurrentTimeMillis();

    /*if (Config.LOGD) {
        Log.d(TAG, " prescan time: " + (prescan - start) + "ms\n");
        Log.d(TAG, "    scan time: " + (scan - prescan) + "ms\n");
        Log.d(TAG, "postscan time: " + (end - scan) + "ms\n");
        Log.d(TAG, "   total time: " + (end - start) + "ms\n");
    }*/
    //} catch (SQLException e) {
    //    // this might happen if the SD card is removed while the media scanner is running
    //    Log.e(TAG, "SQLException in CMediaScanner.scan()", e);
    //} catch (UnsupportedOperationException e) {
    //    // this might happen if the SD card is removed while the media scanner is running
    //    Log.e(TAG, "UnsupportedOperationException in CMediaScanner.scan()", e);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "RemoteException in CMediaScanner.scan()", e);
    //}

    return NOERROR;
}

// this function is used to scan a single file
ECode CMediaScanner::ScanSingleFile(
    /* [in] */ const String& path,
    /* [in] */ const String& volumeName,
    /* [in] */ const String& mimeType,
    /* [out] */ IUri** ppUri)
{
    //try {
        Initialize(volumeName);
        Prescan(path);

        //File file = new File(path);
        // always scan the file, so we can return the content://media Uri for existing files
        //return mClient->DoScanFile(path, mimeType, file.lastModified(), file.length(), TRUE);
    /*} catch (RemoteException e) {
        Log.e(TAG, "RemoteException in CMediaScanner.scanFile()", e);
        return NULL;
    }*/

    return NOERROR;
}

// returns the number of matching file/directory names, starting from the right
Int32 CMediaScanner::MatchPaths(
    /* [in] */ const String& path1,
    /* [in] */ const String& path2)
{
    Int32 result = 0;
    Int32 end1 = path1.GetLength();
    Int32 end2 = path2.GetLength();

    while (end1 > 0 && end2 > 0) {
        Int32 slash1 = path1.LastIndexOf('/', end1 - 1);
        Int32 slash2 = path2.LastIndexOf('/', end2 - 1);
        Int32 backSlash1 = path1.LastIndexOf('\\', end1 - 1);
        Int32 backSlash2 = path2.LastIndexOf('\\', end2 - 1);
        Int32 start1 = (slash1 > backSlash1 ? slash1 : backSlash1);
        Int32 start2 = (slash2 > backSlash2 ? slash2 : backSlash2);
        if (start1 < 0) start1 = 0; else start1++;
        if (start2 < 0) start2 = 0; else start2++;
        Int32 length = end1 - start1;
        if (end2 - start2 != length) break;
        // if (path1.RegionMatches(TRUE, start1, path2, start2, length)) {
        //     result++;
        //     end1 = start1 - 1;
        //     end2 = start2 - 1;
        // } else break;
    }

    return result;
}

Boolean CMediaScanner::AddPlayListEntry(
    /* [in] */ const String& entry,
    /* [in] */ const String& playListDirectory,
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ Int32 index)
{
    // watch for trailing whitespace
    //Int32 entryLength = entry.length();
    //while (entryLength > 0 && Character.isWhitespace(entry.charAt(entryLength - 1))) entryLength--;
    //// path should be longer than 3 characters.
    //// avoid index out of bounds errors below by returning here.
    //if (entryLength < 3) return FALSE;
    //if (entryLength < entry.length()) entry = entry.substring(0, entryLength);

    //// does entry appear to be an absolute path?
    //// look for Unix or DOS absolute paths
    //char ch1 = entry.charAt(0);
    //Boolean fullPath = (ch1 == '/' ||
    //        (Character.isLetter(ch1) && entry.charAt(1) == ':' && entry.charAt(2) == '\\'));
    //// if we have a relative path, combine entry with playListDirectory
    //if (!fullPath)
    //    entry = playListDirectory + entry;

    ////FIXME - should we look for "../" within the path?

    //// best matching MediaFile for the play list entry
    //FileCacheEntry bestMatch = NULL;

    //// number of rightmost file/directory names for bestMatch
    //Int32 bestMatchLength = 0;

    //Iterator<FileCacheEntry> iterator = mFileCache.values().iterator();
    //while (iterator.hasNext()) {
    //    FileCacheEntry cacheEntry = iterator.next();
    //    String path = cacheEntry.mPath;

    //    if (path.equalsIgnoreCase(entry)) {
    //        bestMatch = cacheEntry;
    //        break;    // don't bother continuing search
    //    }

    //    Int32 matchLength = matchPaths(path, entry);
    //    if (matchLength > bestMatchLength) {
    //        bestMatch = cacheEntry;
    //        bestMatchLength = matchLength;
    //    }
    //}

    //// if the match is not for an audio file, bail out
    //if (bestMatch == NULL || ! mAudioUri.equals(bestMatch.mTableUri)) {
    //    return FALSE;
    //}

    ////try {
    //// OK, now we need to add this to the database
    //    values.clear();
    //    values.put(MediaStore.Audio.Playlists.Members.PLAY_ORDER, Integer.valueOf(index));
    //    values.put(MediaStore.Audio.Playlists.Members.AUDIO_ID, Long.valueOf(bestMatch.mRowId));
    //    mMediaProvider.insert(uri, values);
    ///*} catch (RemoteException e) {
    //    Log.e(TAG, "RemoteException in CMediaScanner.addPlayListEntry()", e);
    //    return FALSE;
    //}*/

    return TRUE;
}

void CMediaScanner::ProcessM3uPlayList(
    /* [in] */ const String& path,
    /* [in] */ const String& playListDirectory,
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values)
{
    //BufferedReader reader = NULL;
    //try {
    AutoPtr<IFile> f;
    CFile::New(path, (IFile**)&f);
    Boolean isExists;
    if (f->Exists(&isExists), isExists) {
        AutoPtr<IFileInputStream> fs;
        CFileInputStream::New(f, (IFileInputStream**)&fs);
        AutoPtr<IInputStreamReader> ir;
        // CInputStreamReader::New(fs, 8192, (IInputStreamReader**)&ir);
        AutoPtr<IBufferedReader> reader;
        CBufferedReader::New(ir, (IBufferedReader**)&reader);
        String line;
        reader->ReadLine(&line);
        Int32 index = 0;
        while (!line.IsNull()) {
            // ignore comment lines, which begin with '#'
            if (line.GetLength() > 0 && line.GetChar(0) != '#') {
                //values->Clear();
                if (AddPlayListEntry(line, playListDirectory, uri, values, index))
                    index++;
            }
            reader->ReadLine(&line);
        }
    }
    /*} catch (IOException e) {
        Log.e(TAG, "IOException in CMediaScanner.processM3uPlayList()", e);
    } finally {
        try {
            if (reader != NULL)
                reader.close();
        } catch (IOException e) {
            Log.e(TAG, "IOException in CMediaScanner.processM3uPlayList()", e);
        }
    }*/
}

void CMediaScanner::ProcessPlsPlayList(
    /* [in] */ const String& path,
    /* [in] */ const String& playListDirectory,
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values)
{
    ////try {
        AutoPtr<IFile> f;
        CFile::New(path, (IFile**)&f);
        Boolean isExists;
        f->Exists(&isExists);
        if (isExists) {
            AutoPtr<IFileInputStream> fs;
            CFileInputStream::New(f, (IFileInputStream**)&fs);

            AutoPtr<IInputStreamReader> ir;
            //CInputStreamReader::New(fs, 8192, (IInputStreamReader**)&ir);

            AutoPtr<IBufferedReader> reader;
            CBufferedReader::New(ir, (IBufferedReader**)&reader);

            String line;
            reader->ReadLine(&line);
            Int32 index = 0;
            while (line != NULL) {
                // ignore comment lines, which begin with '#'
                if (line.StartWith("File")) {
                    Int32 equals = line.IndexOf('=');
                    if (equals > 0) {
                        //values.clear();
                        if (AddPlayListEntry(line.Substring(equals + 1), playListDirectory, uri, values, index))
                            index++;
                    }
                }
                reader->ReadLine(&line);
            }
        }
    /*} catch (IOException e) {
        Log.e(TAG, "IOException in CMediaScanner.processPlsPlayList()", e);
    } finally {
        try {
            if (reader != NULL)
                reader.close();
        } catch (IOException e) {
            Log.e(TAG, "IOException in CMediaScanner.processPlsPlayList()", e);
        }
    }*/
}

void CMediaScanner::ProcessWplPlayList(
    /* [in] */ const String& path,
    /* [in] */ const String& playListDirectory,
    /* [in] */ IUri* uri)
{
    /*FileInputStream fis = NULL;
    try {
        File f = new File(path);
        if (f.exists()) {
            fis = new FileInputStream(f);

            Xml.parse(fis, Xml.findEncodingByName("UTF-8"), new WplHandler(playListDirectory, uri).getContentHandler());
        }
    } catch (SAXException e) {
        e.printStackTrace();
    } catch (IOException e) {
        e.printStackTrace();
    } finally {
        try {
            if (fis != NULL)
                fis.close();
        } catch (IOException e) {
            Log.e(TAG, "IOException in CMediaScanner.processWplPlayList()", e);
        }
    }*/
}

ECode CMediaScanner::ProcessPlayLists()
{
    //Iterator<FileCacheEntry> iterator = mPlayLists.iterator();
    //while (iterator.hasNext()) {
    //    FileCacheEntry entry = iterator.next();
    //    String path = entry.mPath;

    //    // only process playlist files if they are new or have been modified since the last scan
    //    if (entry.mLastModifiedChanged) {
    //        ContentValues values = new ContentValues();
    //        Int32 lastSlash = path.lastIndexOf('/');
    //        if (lastSlash < 0) throw new IllegalArgumentException("bad path " + path);
    //        Uri uri, membersUri;
    //        Int64 rowId = entry.mRowId;
    //        if (rowId == 0) {
    //            // Create a new playlist

    //            Int32 lastDot = path.lastIndexOf('.');
    //            String name = (lastDot < 0 ? path.substring(lastSlash + 1) : path.substring(lastSlash + 1, lastDot));
    //            values.put(MediaStore.Audio.Playlists.NAME, name);
    //            values.put(MediaStore.Audio.Playlists.DATA, path);
    //            values.put(MediaStore.Audio.Playlists.DATE_MODIFIED, entry.mLastModified);
    //            uri = mMediaProvider.insert(mPlaylistsUri, values);
    //            rowId = ContentUris.parseId(uri);
    //            membersUri = Uri.withAppendedPath(uri, Playlists.Members.CONTENT_DIRECTORY);
    //        } else {
    //            uri = ContentUris.withAppendedId(mPlaylistsUri, rowId);

    //            // update lastModified value of existing playlist
    //            values.put(MediaStore.Audio.Playlists.DATE_MODIFIED, entry.mLastModified);
    //            mMediaProvider.update(uri, values, NULL, NULL);

    //            // delete members of existing playlist
    //            membersUri = Uri.withAppendedPath(uri, Playlists.Members.CONTENT_DIRECTORY);
    //            mMediaProvider.delete(membersUri, NULL, NULL);
    //        }

    //        String playListDirectory = path.substring(0, lastSlash + 1);
    //        MediaFile.MediaFileType mediaFileType = MediaFile.getFileType(path);
    //        Int32 fileType = (mediaFileType == NULL ? 0 : mediaFileType.fileType);

    //        if (fileType == MediaFile.FILE_TYPE_M3U)
    //            processM3uPlayList(path, playListDirectory, membersUri, values);
    //        else if (fileType == MediaFile.FILE_TYPE_PLS)
    //            processPlsPlayList(path, playListDirectory, membersUri, values);
    //        else if (fileType == MediaFile.FILE_TYPE_WPL)
    //            processWplPlayList(path, playListDirectory, membersUri);

    //        Cursor cursor = mMediaProvider.query(membersUri, PLAYLIST_MEMBERS_PROJECTION, NULL,
    //                NULL, NULL);
    //        try {
    //            if (cursor == NULL || cursor.getCount() == 0) {
    //                Log.d(TAG, "playlist is empty - deleting");
    //                mMediaProvider.delete(uri, NULL, NULL);
    //            }
    //        } finally {
    //            if (cursor != NULL) cursor.close();
    //        }
    //    }
    //}
    return E_NOT_IMPLEMENTED;
}

void CMediaScanner::ProcessDirectory(
    /* [in] */ const String& path,
    /* [in] */ const String& extensions,
    /* [in] */ IMediaScannerClient* client)
{
    /*CMediaScanner *mp = (CMediaScanner *)env->GetIntField(thiz, fields.context);


    MyMediaScannerClient myClient(env, client);
    mp->processDirectory(pathStr, extensionsStr, myClient, ExceptionCheck, env);*/

}

void CMediaScanner::ProcessFile(
    /* [in] */ const String& path,
    /* [in] */ const String& mimeType,
    /* [in] */ IMediaScannerClient* client)
{
    //mp->processFile(pathStr, mimeTypeStr, myClient);
}

ECode CMediaScanner::SetLocale(
    /* [in] */ const String& locale)
{
    //    CMediaScanner *mp = (CMediaScanner *)env->GetIntField(thiz, fields.context);
    //
    //    if (locale == NULL) {
    //        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
    //        return;
    //    }
    //    const char *localeStr = env->GetStringUTFChars(locale, NULL);
    //    if (localeStr == NULL) {  // Out of memory
    //        jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
    //        return;
    //    }
    //    mp->setLocale(localeStr);
    //
    //    env->ReleaseStringUTFChars(locale, localeStr);
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScanner::ExtractAlbumArt(
    /* [in] */ IFileDescriptor* fd,
    /* [out, callee] */ ArrayOf<Byte>** albumArts)
{
    //    CMediaScanner *mp = (CMediaScanner *)env->GetIntField(thiz, fields.context);
    //
    //    if (fileDescriptor == NULL) {
    //        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
    //        return NULL;
    //    }
    //
    //    int fd = getParcelFileDescriptorFD(env, fileDescriptor);
    //    char* data = mp->extractAlbumArt(fd);
    //    if (!data) {
    //        return NULL;
    //    }
    //    long len = *((long*)data);
    //
    //    jbyteArray array = env->NewByteArray(len);
    //    if (array != NULL) {
    //        jbyte* bytes = env->GetByteArrayElements(array, NULL);
    //        memcpy(bytes, data + 4, len);
    //        env->ReleaseByteArrayElements(array, bytes, 0);
    //    }
    //
    //done:
    //    free(data);
    //    // if NewByteArray() returned NULL, an out-of-memory
    //    // exception will have been raised. I just want to
    //    // return null in that case.
    //    env->ExceptionClear();
    //    return array;

    return E_NOT_IMPLEMENTED;
}

void CMediaScanner::NativeInit()
{
}

void CMediaScanner::NativeSetup()
{
}

void CMediaScanner::NativeFinalize()
{
}

