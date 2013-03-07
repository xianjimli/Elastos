
#include "media/MediaScanner.h"
#include "media/MediaFile.h"

MediaScanner::MediaScanner()
{

}

MediaScanner::FileCacheEntry::FileCacheEntry(
    /* [in] */ IUri* tableUri, 
    /* [in] */ Int64 rowId, 
    /* [in] */ String path, 
    /* [in] */ Int64 lastModified)
{
    mTableUri = tableUri;
    mRowId = rowId;
    mPath = path;
    mLastModified = lastModified;
    mSeenInFileSystem = FALSE;
    mLastModifiedChanged = FALSE;
}

String MediaScanner::FileCacheEntry::ToString()
{
    return mPath;
}

MediaScanner::MediaScanner(
    /* [in] */ IContext* c)
{
    native_setup();
    mContext = c;
    /*mBitmapOptions.inSampleSize = 1;
    mBitmapOptions.inJustDecodeBounds = TRUE;*/

    SetDefaultRingtoneFileNames();
}

void MediaScanner::SetDefaultRingtoneFileNames()
{
    /*mDefaultRingtoneFilename = SystemProperties.get(DEFAULT_RINGTONE_PROPERTY_PREFIX
            + Settings.System.RINGTONE);
    mDefaultNotificationFilename = SystemProperties.get(DEFAULT_RINGTONE_PROPERTY_PREFIX
            + Settings.System.NOTIFICATION_SOUND);
    mDefaultAlarmAlertFilename = SystemProperties.get(DEFAULT_RINGTONE_PROPERTY_PREFIX
            + Settings.System.ALARM_ALERT);*/
}


MediaScanner::FileCacheEntry* MediaScanner::MyMediaScannerClient::BeginFile(
    /* [in] */ String path, 
    /* [in] */ String mimeType, 
    /* [in] */ Int64 lastModified, 
    /* [in] */ Int64 fileSize)
{

    // special case certain file names
    // I use regionMatches() instead of substring() below
    // to avoid memory allocation
    //Int32 lastSlash = path.LastIndexOf('/');
    //if (lastSlash >= 0 && lastSlash + 2 < path.length()) {
    //    // ignore those ._* files created by MacOS
    //    if (path.RegionMatches(lastSlash + 1, "._", 0, 2)) {
    //        return NULL;
    //    }

    //    // ignore album art files created by Windows Media Player:
    //    // Folder.jpg, AlbumArtSmall.jpg, AlbumArt_{...}_Large.jpg and AlbumArt_{...}_Small.jpg
    //    if (path.RegionMatches(TRUE, path.length() - 4, ".jpg", 0, 4)) {
    //        if (path.regionMatches(TRUE, lastSlash + 1, "AlbumArt_{", 0, 10) ||
    //                path.regionMatches(TRUE, lastSlash + 1, "AlbumArt.", 0, 9)) {
    //            return NULL;
    //        }
    //        Int32 length = path.length() - lastSlash - 1;
    //        if ((length == 17 && path.regionMatches(TRUE, lastSlash + 1, "AlbumArtSmall", 0, 13)) ||
    //                (length == 10 && path.regionMatches(TRUE, lastSlash + 1, "Folder", 0, 6))) {
    //            return NULL;
    //        }
    //    }
    //}

    //mMimeType = NULL;
    //// try mimeType first, if it is specified
    //if (mimeType != NULL) {
    //    mFileType = MediaFile.getFileTypeForMimeType(mimeType);
    //    if (mFileType != 0) {
    //        mMimeType = mimeType;
    //    }
    //}
    //mFileSize = fileSize;

    //// if mimeType was not specified, compute file type based on file extension.
    //if (mMimeType == NULL) {
    //    MediaFile.MediaFileType mediaFileType = MediaFile.getFileType(path);
    //    if (mediaFileType != NULL) {
    //        mFileType = mediaFileType.fileType;
    //        mMimeType = mediaFileType.mimeType;
    //    }
    //}

    //String key = path;
    //if (mCaseInsensitivePaths) {
    //    key = path.toLowerCase();
    //}
    //FileCacheEntry entry = mFileCache.get(key);
    //if (entry == NULL) {
    //    entry = new FileCacheEntry(NULL, 0, path, 0);
    //    mFileCache.put(key, entry);
    //}
    //entry.mSeenInFileSystem = TRUE;

    //// add some slack to avoid a rounding error
    //Int64 delta = lastModified - entry.mLastModified;
    //if (delta > 1 || delta < -1) {
    //    entry.mLastModified = lastModified;
    //    entry.mLastModifiedChanged = TRUE;
    //}

    //if (mProcessPlaylists && MediaFile.isPlayListFileType(mFileType)) {
    //    mPlayLists.add(entry);
    //    // we don't process playlists in the main scan, so return NULL
    //    return NULL;
    //}

    //// clear all the metadata
    //mArtist = NULL;
    //mAlbumArtist = NULL;
    //mAlbum = NULL;
    //mTitle = NULL;
    //mComposer = NULL;
    //mGenre = NULL;
    //mTrack = 0;
    //mYear = 0;
    //mDuration = 0;
    //mPath = path;
    //mLastModified = lastModified;
    //mWriter = NULL;
    //mCompilation = 0;

    //return entry;

    return NULL;
}

ECode MediaScanner::MyMediaScannerClient::ScanFile(
    /* [in] */ String path, 
    /* [in] */ Int64 lastModified, 
    /* [in] */ Int64 fileSize)
{
    // This is the callback funtion from native codes.
    // Log.v(TAG, "scanFile: "+path);
    DoScanFile(path, String(NULL), lastModified, fileSize, FALSE);

    return NOERROR;
}

ECode MediaScanner::MyMediaScannerClient::ScanFileEx(
    /* [in] */ String path, 
    /* [in] */ String mimeType, 
    /* [in] */ Int64 lastModified, 
    /* [in] */ Int64 fileSize) 
{
    DoScanFile(path, mimeType, lastModified, fileSize, FALSE);

    return NOERROR;
}

AutoPtr<IUri> MediaScanner::MyMediaScannerClient::DoScanFile(
    /* [in] */ String path, 
    /* [in] */ String mimeType, 
    /* [in] */ Int64 lastModified, 
    /* [in] */ Int64 fileSize, 
    /* [in] */ Boolean scanAlways) 
{
    AutoPtr<IUri> result = NULL;
//            Int64 t1 = System.currentTimeMillis();
    //try {
        //FileCacheEntry entry = BeginFile(path, mimeType, lastModified, fileSize);
        //// rescan for metadata if file was modified since last scan
        //if (entry != NULL && (entry.mLastModifiedChanged || scanAlways)) {
        //    String lowpath = path.toLowerCase();
        //    Boolean ringtones = (lowpath.indexOf(RINGTONES_DIR) > 0);
        //    Boolean notifications = (lowpath.indexOf(NOTIFICATIONS_DIR) > 0);
        //    Boolean alarms = (lowpath.indexOf(ALARMS_DIR) > 0);
        //    Boolean podcasts = (lowpath.indexOf(PODCAST_DIR) > 0);
        //    Boolean music = (lowpath.indexOf(MUSIC_DIR) > 0) ||
        //        (!ringtones && !notifications && !alarms && !podcasts);

        //    if (!MediaFile.isImageFileType(mFileType)) {
        //        ProcessFile(path, mimeType, this);
        //    }

        //    result = EndFile(entry, ringtones, notifications, alarms, music, podcasts);
        //}
    /*} catch (RemoteException e) {
        Log.e(TAG, "RemoteException in MediaScanner.scanFile()", e);
    }*/
//            Int64 t2 = System.currentTimeMillis();
//            Log.v(TAG, "scanFile: " + path + " took " + (t2-t1));
    return result;
}

Int32 MediaScanner::MyMediaScannerClient::ParseSubstring(
    /* [in] */ String s, 
    /* [in] */ Int32 start, 
    /* [in] */ Int32 defaultValue) 
{
    //Int32 length = s.Size();
    //if (start == length) return defaultValue;

    //Char32 ch = s.CharAt(start++);
    //// return defaultValue if we have no integer at all
    //if (ch < '0' || ch > '9') return defaultValue;

    //Int32 result = ch - '0';
    //while (start < length) {
    //    ch = s.CharAt(start++);
    //    if (ch < '0' || ch > '9') return result;
    //    result = result * 10 + (ch - '0');
    //}

    //return result;

    return 0;
}

ECode MediaScanner::MyMediaScannerClient::HandleStringTag(
    /* [in] */ String name, 
    /* [in] */ String value) 
{
    //if (name.equalsIgnoreCase("title") || name.startsWith("title;")) {
    //    // Don't trim() here, to preserve the special \001 character
    //    // used to force sorting. The media provider will trim() before
    //    // inserting the title in to the database.
    //    mTitle = value;
    //} else if (name.equalsIgnoreCase("artist") || name.startsWith("artist;")) {
    //    mArtist = value.trim();
    //} else if (name.equalsIgnoreCase("albumartist") || name.startsWith("albumartist;")) {
    //    mAlbumArtist = value.trim();
    //} else if (name.equalsIgnoreCase("album") || name.startsWith("album;")) {
    //    mAlbum = value.trim();
    //} else if (name.equalsIgnoreCase("composer") || name.startsWith("composer;")) {
    //    mComposer = value.trim();
    //} else if (name.equalsIgnoreCase("genre") || name.startsWith("genre;")) {
    //    // handle numeric genres, which PV sometimes encodes like "(20)"
    //    if (value.length() > 0) {
    //        Int32 genreCode = -1;
    //        char ch = value.charAt(0);
    //        if (ch == '(') {
    //            genreCode = parseSubstring(value, 1, -1);
    //        } else if (ch >= '0' && ch <= '9') {
    //            genreCode = parseSubstring(value, 0, -1);
    //        }
    //        if (genreCode >= 0 && genreCode < ID3_GENRES.length) {
    //            value = ID3_GENRES[genreCode];
    //        } else if (genreCode == 255) {
    //            // 255 is defined to be unknown
    //            value = NULL;
    //        }
    //    }
    //    mGenre = value;
    //} else if (name.equalsIgnoreCase("year") || name.startsWith("year;")) {
    //    mYear = parseSubstring(value, 0, 0);
    //} else if (name.equalsIgnoreCase("tracknumber") || name.startsWith("tracknumber;")) {
    //    // track number might be of the form "2/12"
    //    // we just read the number before the slash
    //    Int32 num = parseSubstring(value, 0, 0);
    //    mTrack = (mTrack / 1000) * 1000 + num;
    //} else if (name.equalsIgnoreCase("discnumber") ||
    //        name.equals("set") || name.startsWith("set;")) {
    //    // set number might be of the form "1/3"
    //    // we just read the number before the slash
    //    Int32 num = parseSubstring(value, 0, 0);
    //    mTrack = (num * 1000) + (mTrack % 1000);
    //} else if (name.equalsIgnoreCase("duration")) {
    //    mDuration = parseSubstring(value, 0, 0);
    //} else if (name.equalsIgnoreCase("writer") || name.startsWith("writer;")) {
    //    mWriter = value.trim();
    //} else if (name.equalsIgnoreCase("compilation")) {
    //    mCompilation = parseSubstring(value, 0, 0);
    //}

    return NOERROR;
}

ECode MediaScanner::MyMediaScannerClient::SetMimeType(
    /* [in] */ String mimeType)
{
    //if ("audio/mp4".equals(mMimeType) &&
    //        mimeType.startsWith("video")) {
    //    // for feature parity with Donut, we force m4a files to keep the
    //    // audio/mp4 mimetype, even if they are really "enhanced podcasts"
    //    // with a video track
    //    return;
    //}
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
AutoPtr<IContentValues> MediaScanner::MyMediaScannerClient::ToValues()
{
    //ContentValues map = new ContentValues();

    //map.put(MediaStore.MediaColumns.DATA, mPath);
    //map.put(MediaStore.MediaColumns.TITLE, mTitle);
    //map.put(MediaStore.MediaColumns.DATE_MODIFIED, mLastModified);
    //map.put(MediaStore.MediaColumns.SIZE, mFileSize);
    //map.put(MediaStore.MediaColumns.MIME_TYPE, mMimeType);

    //if (MediaFile.isVideoFileType(mFileType)) {
    //    map.put(Video.Media.ARTIST, (mArtist != NULL && mArtist.length() > 0 ? mArtist : MediaStore.UNKNOWN_STRING));
    //    map.put(Video.Media.ALBUM, (mAlbum != NULL && mAlbum.length() > 0 ? mAlbum : MediaStore.UNKNOWN_STRING));
    //    map.put(Video.Media.DURATION, mDuration);
    //    // FIXME - add RESOLUTION
    //} else if (MediaFile.isImageFileType(mFileType)) {
    //    // FIXME - add DESCRIPTION
    //} else if (MediaFile.isAudioFileType(mFileType)) {
    //    map.put(Audio.Media.ARTIST, (mArtist != NULL && mArtist.length() > 0) ?
    //            mArtist : MediaStore.UNKNOWN_STRING);
    //    map.put(Audio.Media.ALBUM_ARTIST, (mAlbumArtist != NULL &&
    //            mAlbumArtist.length() > 0) ? mAlbumArtist : NULL);
    //    map.put(Audio.Media.ALBUM, (mAlbum != NULL && mAlbum.length() > 0) ?
    //            mAlbum : MediaStore.UNKNOWN_STRING);
    //    map.put(Audio.Media.COMPOSER, mComposer);
    //    if (mYear != 0) {
    //        map.put(Audio.Media.YEAR, mYear);
    //    }
    //    map.put(Audio.Media.TRACK, mTrack);
    //    map.put(Audio.Media.DURATION, mDuration);
    //    map.put(Audio.Media.COMPILATION, mCompilation);
    //}
    //return map;

    return NULL;
}

AutoPtr<IUri> MediaScanner::MyMediaScannerClient::EndFile(
    /* [in] */ FileCacheEntry* entry, 
    /* [in] */ Boolean ringtones, 
    /* [in] */ Boolean notifications,
    /* [in] */ Boolean alarms, 
    /* [in] */ Boolean music, 
    /* [in] */ Boolean podcasts)
{
#if 0
    // update database
    Uri tableUri;
    Boolean isAudio = MediaFile.isAudioFileType(mFileType);
    Boolean isVideo = MediaFile.isVideoFileType(mFileType);
    Boolean isImage = MediaFile.isImageFileType(mFileType);
    if (isVideo) {
        tableUri = mVideoUri;
    } else if (isImage) {
        tableUri = mImagesUri;
    } else if (isAudio) {
        tableUri = mAudioUri;
    } else {
        // don't add file to database if not audio, video or image
        return NULL;
    }
    entry.mTableUri = tableUri;

     // use album artist if artist is missing
    if (mArtist == NULL || mArtist.length() == 0) {
        mArtist = mAlbumArtist;
    }

    ContentValues values = toValues();
    String title = values.getAsString(MediaStore.MediaColumns.TITLE);
    if (title == NULL || TextUtils.isEmpty(title.trim())) {
        title = values.getAsString(MediaStore.MediaColumns.DATA);
        // extract file name after last slash
        Int32 lastSlash = title.lastIndexOf('/');
        if (lastSlash >= 0) {
            lastSlash++;
            if (lastSlash < title.length()) {
                title = title.substring(lastSlash);
            }
        }
        // truncate the file extension (if any)
        Int32 lastDot = title.lastIndexOf('.');
        if (lastDot > 0) {
            title = title.substring(0, lastDot);
        }
        values.put(MediaStore.MediaColumns.TITLE, title);
    }
    String album = values.getAsString(Audio.Media.ALBUM);
    if (MediaStore.UNKNOWN_STRING.equals(album)) {
        album = values.getAsString(MediaStore.MediaColumns.DATA);
        // extract last path segment before file name
        Int32 lastSlash = album.lastIndexOf('/');
        if (lastSlash >= 0) {
            Int32 previousSlash = 0;
            while (TRUE) {
                Int32 idx = album.indexOf('/', previousSlash + 1);
                if (idx < 0 || idx >= lastSlash) {
                    break;
                }
                previousSlash = idx;
            }
            if (previousSlash != 0) {
                album = album.substring(previousSlash + 1, lastSlash);
                values.put(Audio.Media.ALBUM, album);
            }
        }
    }
    Int64 rowId = entry.mRowId;
    if (isAudio && rowId == 0) {
        // Only set these for new entries. For existing entries, they
        // may have been modified later, and we want to keep the current
        // values so that custom ringtones still show up in the ringtone
        // picker.
        values.put(Audio.Media.IS_RINGTONE, ringtones);
        values.put(Audio.Media.IS_NOTIFICATION, notifications);
        values.put(Audio.Media.IS_ALARM, alarms);
        values.put(Audio.Media.IS_MUSIC, music);
        values.put(Audio.Media.IS_PODCAST, podcasts);
    } else if (mFileType == MediaFile.FILE_TYPE_JPEG) {
        ExifInterface exif = NULL;
        try {
            exif = new ExifInterface(entry.mPath);
        } catch (IOException ex) {
            // exif is NULL
        }
        if (exif != NULL) {
            float[] latlng = new float[2];
            if (exif.getLatLong(latlng)) {
                values.put(Images.Media.LATITUDE, latlng[0]);
                values.put(Images.Media.LONGITUDE, latlng[1]);
            }

            Int64 time = exif.getGpsDateTime();
            if (time != -1) {
                values.put(Images.Media.DATE_TAKEN, time);
            }

            Int32 orientation = exif.getAttributeInt(
                ExifInterface.TAG_ORIENTATION, -1);
            if (orientation != -1) {
                // We only recognize a subset of orientation tag values.
                Int32 degree;
                switch(orientation) {
                    case ExifInterface.ORIENTATION_ROTATE_90:
                        degree = 90;
                        break;
                    case ExifInterface.ORIENTATION_ROTATE_180:
                        degree = 180;
                        break;
                    case ExifInterface.ORIENTATION_ROTATE_270:
                        degree = 270;
                        break;
                    default:
                        degree = 0;
                        break;
                }
                values.put(Images.Media.ORIENTATION, degree);
            }
        }
    }

    Uri result = NULL;
    if (rowId == 0) {
        // new file, insert it
        result = mMediaProvider.insert(tableUri, values);
        if (result != NULL) {
            rowId = ContentUris.parseId(result);
            entry.mRowId = rowId;
        }
    } else {
        // updated file
        result = ContentUris.withAppendedId(tableUri, rowId);
        mMediaProvider.update(result, values, NULL, NULL);
    }
    if (mProcessGenres && mGenre != NULL) {
        String genre = mGenre;
        Uri uri = mGenreCache.get(genre);
        if (uri == NULL) {
            Cursor cursor = NULL;
            try {
                // see if the genre already exists
                cursor = mMediaProvider.query(
                        mGenresUri,
                        GENRE_LOOKUP_PROJECTION, MediaStore.Audio.Genres.NAME + "=?",
                                new String[] { genre }, NULL);
                if (cursor == NULL || cursor.getCount() == 0) {
                    // genre does not exist, so create the genre in the genre table
                    values.clear();
                    values.put(MediaStore.Audio.Genres.NAME, genre);
                    uri = mMediaProvider.insert(mGenresUri, values);
                } else {
                    // genre already exists, so compute its Uri
                    cursor.moveToNext();
                    uri = ContentUris.withAppendedId(mGenresUri, cursor.getLong(0));
                }
                if (uri != NULL) {
                    uri = Uri.withAppendedPath(uri, Genres.Members.CONTENT_DIRECTORY);
                    mGenreCache.put(genre, uri);
                }
            } finally {
                // release the cursor if it exists
                if (cursor != NULL) {
                    cursor.close();
                }
            }
        }

        if (uri != NULL) {
            // add entry to audio_genre_map
            values.clear();
            values.put(MediaStore.Audio.Genres.Members.AUDIO_ID, Long.valueOf(rowId));
            mMediaProvider.insert(uri, values);
        }
    }

    if (notifications && !mDefaultNotificationSet) {
        if (TextUtils.isEmpty(mDefaultNotificationFilename) ||
                doesPathHaveFilename(entry.mPath, mDefaultNotificationFilename)) {
            setSettingIfNotSet(Settings.System.NOTIFICATION_SOUND, tableUri, rowId);
            mDefaultNotificationSet = TRUE;
        }
    } else if (ringtones && !mDefaultRingtoneSet) {
        if (TextUtils::IsEmpty(mDefaultRingtoneFilename) ||
                DoesPathHaveFilename(entry.mPath, mDefaultRingtoneFilename)) {
            SetSettingIfNotSet(Settings.System.RINGTONE, tableUri, rowId);
            mDefaultRingtoneSet = TRUE;
        }
    } else if (alarms && !mDefaultAlarmSet) {
        if (TextUtils::IsEmpty(mDefaultAlarmAlertFilename) ||
                doesPathHaveFilename(entry.mPath, mDefaultAlarmAlertFilename)) {
            SetSettingIfNotSet(Settings.System.ALARM_ALERT, tableUri, rowId);
            mDefaultAlarmSet = TRUE;
        }
    }

    return result;
#endif

    return NULL;
}

Boolean MediaScanner::MyMediaScannerClient::DoesPathHaveFilename(
    /* [in] */ String path, 
    /* [in] */ String filename)
{
    /*Int32 pathFilenameStart = path.lastIndexOf(File.separatorChar) + 1;
    Int32 filenameLength = filename.length();
    return path.regionMatches(pathFilenameStart, filename, 0, filenameLength) &&
            pathFilenameStart + filenameLength == path.length();*/

    return FALSE;
}

void MediaScanner::MyMediaScannerClient::SetSettingIfNotSet(
    /* [in] */ String settingName, 
    /* [in] */ IUri* uri, 
    /* [in] */ Int64 rowId)
{
    //String existingSettingValue = Settings.System.getString(mContext.getContentResolver(),
    //        settingName);

    //if (TextUtils::IsEmpty(existingSettingValue)) {
    //    // Set the setting to the given URI
    //    Settings.System.putString(mContext.getContentResolver(), settingName,
    //            ContentUris.withAppendedId(uri, rowId).toString());
    //}
}

ECode MediaScanner::MyMediaScannerClient::AddNoMediaFolder(
    /* [in] */ String path)
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

IInterface* MediaScanner::MyMediaScannerClient::Probe(
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

UInt32 MediaScanner::MyMediaScannerClient::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MediaScanner::MyMediaScannerClient::Release()
{
    return ElRefBase::Release();
}

ECode MediaScanner::MyMediaScannerClient::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}



void MediaScanner::Prescan(
    /* [in] */ String filePath)
{
#if 0
    Cursor c = NULL;
    String where = NULL;
    String[] selectionArgs = NULL;

    if (mFileCache == NULL) {
        mFileCache = new HashMap<String, FileCacheEntry>();
    } else {
        mFileCache.clear();
    }
    if (mPlayLists == NULL) {
        mPlayLists = new ArrayList<FileCacheEntry>();
    } else {
        mPlayLists.clear();
    }

    // Build the list of files from the content provider
    try {
        // Read existing files from the audio table
        if (filePath != NULL) {
            where = MediaStore.Audio.Media.DATA + "=?";
            selectionArgs = new String[] { filePath };
        }
        c = mMediaProvider.query(mAudioUri, AUDIO_PROJECTION, where, selectionArgs, NULL);

        if (c != NULL) {
            try {
                while (c.moveToNext()) {
                    Int64 rowId = c.getLong(ID_AUDIO_COLUMN_INDEX);
                    String path = c.getString(PATH_AUDIO_COLUMN_INDEX);
                    Int64 lastModified = c.getLong(DATE_MODIFIED_AUDIO_COLUMN_INDEX);

                    // Only consider entries with absolute path names.
                    // This allows storing URIs in the database without the
                    // media scanner removing them.
                    if (path.startsWith("/")) {
                        String key = path;
                        if (mCaseInsensitivePaths) {
                            key = path.toLowerCase();
                        }
                        mFileCache.put(key, new FileCacheEntry(mAudioUri, rowId, path,
                                lastModified));
                    }
                }
            } finally {
                c.close();
                c = NULL;
            }
        }

        // Read existing files from the video table
        if (filePath != NULL) {
            where = MediaStore.Video.Media.DATA + "=?";
        } else {
            where = NULL;
        }
        c = mMediaProvider.query(mVideoUri, VIDEO_PROJECTION, where, selectionArgs, NULL);

        if (c != NULL) {
            try {
                while (c.moveToNext()) {
                    Int64 rowId = c.getLong(ID_VIDEO_COLUMN_INDEX);
                    String path = c.getString(PATH_VIDEO_COLUMN_INDEX);
                    Int64 lastModified = c.getLong(DATE_MODIFIED_VIDEO_COLUMN_INDEX);

                    // Only consider entries with absolute path names.
                    // This allows storing URIs in the database without the
                    // media scanner removing them.
                    if (path.startsWith("/")) {
                        String key = path;
                        if (mCaseInsensitivePaths) {
                            key = path.toLowerCase();
                        }
                        mFileCache.put(key, new FileCacheEntry(mVideoUri, rowId, path,
                                lastModified));
                    }
                }
            } finally {
                c.close();
                c = NULL;
            }
        }

        // Read existing files from the images table
        if (filePath != NULL) {
            where = MediaStore.Images.Media.DATA + "=?";
        } else {
            where = NULL;
        }
        mOriginalCount = 0;
        c = mMediaProvider.query(mImagesUri, IMAGES_PROJECTION, where, selectionArgs, NULL);

        if (c != NULL) {
            try {
                mOriginalCount = c.getCount();
                while (c.moveToNext()) {
                    Int64 rowId = c.getLong(ID_IMAGES_COLUMN_INDEX);
                    String path = c.getString(PATH_IMAGES_COLUMN_INDEX);
                   Int64 lastModified = c.getLong(DATE_MODIFIED_IMAGES_COLUMN_INDEX);

                   // Only consider entries with absolute path names.
                   // This allows storing URIs in the database without the
                   // media scanner removing them.
                   if (path.startsWith("/")) {
                       String key = path;
                       if (mCaseInsensitivePaths) {
                           key = path.toLowerCase();
                       }
                       mFileCache.put(key, new FileCacheEntry(mImagesUri, rowId, path,
                               lastModified));
                   }
                }
            } finally {
                c.close();
                c = NULL;
            }
        }

        if (mProcessPlaylists) {
            // Read existing files from the playlists table
            if (filePath != NULL) {
                where = MediaStore.Audio.Playlists.DATA + "=?";
            } else {
                where = NULL;
            }
            c = mMediaProvider.query(mPlaylistsUri, PLAYLISTS_PROJECTION, where, selectionArgs, NULL);

            if (c != NULL) {
                try {
                    while (c.moveToNext()) {
                        String path = c.getString(PATH_PLAYLISTS_COLUMN_INDEX);

                        if (path != NULL && path.length() > 0) {
                            Int64 rowId = c.getLong(ID_PLAYLISTS_COLUMN_INDEX);
                            Int64 lastModified = c.getLong(DATE_MODIFIED_PLAYLISTS_COLUMN_INDEX);

                            String key = path;
                            if (mCaseInsensitivePaths) {
                                key = path.toLowerCase();
                            }
                            mFileCache.put(key, new FileCacheEntry(mPlaylistsUri, rowId, path,
                                    lastModified));
                        }
                    }
                } finally {
                    c.close();
                    c = NULL;
                }
            }
        }
    }
    finally {
        if (c != NULL) {
            c.close();
        }
    }
#endif
}

Boolean MediaScanner::InScanDirectory(
    /* [in] */ String path, 
    /* [in] */ ArrayOf<String>* directories)
{
    /*for (Int32 i = 0; i < directories.length; i++) {
        if (path.startsWith(directories[i])) {
            return TRUE;
        }
    }*/
    return FALSE;
}

void MediaScanner::PruneDeadThumbnailFiles() 
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

void MediaScanner::Postscan(
    /* [in] */ ArrayOf<String>* directories)
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
}

void MediaScanner::Initialize(
    /* [in] */ String volumeName)
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

ECode MediaScanner::ScanDirectories(
    /* [in] */ ArrayOf<String>* directories, 
    /* [in] */ String volumeName)
{
    //try {
        /*Int64 start = System.currentTimeMillis();
        initialize(volumeName);
        prescan(NULL);
        Int64 prescan = System.currentTimeMillis();

        for (Int32 i = 0; i < directories.length; i++) {
            processDirectory(directories[i], MediaFile.sFileExtensions, mClient);
        }
        Int64 scan = System.currentTimeMillis();
        postscan(directories);
        Int64 end = System.currentTimeMillis();

        if (Config.LOGD) {
            Log.d(TAG, " prescan time: " + (prescan - start) + "ms\n");
            Log.d(TAG, "    scan time: " + (scan - prescan) + "ms\n");
            Log.d(TAG, "postscan time: " + (end - scan) + "ms\n");
            Log.d(TAG, "   total time: " + (end - start) + "ms\n");
        }*/
    //} catch (SQLException e) {
    //    // this might happen if the SD card is removed while the media scanner is running
    //    Log.e(TAG, "SQLException in MediaScanner.scan()", e);
    //} catch (UnsupportedOperationException e) {
    //    // this might happen if the SD card is removed while the media scanner is running
    //    Log.e(TAG, "UnsupportedOperationException in MediaScanner.scan()", e);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "RemoteException in MediaScanner.scan()", e);
    //}

    return NOERROR;
}

// this function is used to scan a single file
AutoPtr<IUri> MediaScanner::ScanSingleFile(
    /* [in] */ String path, 
    /* [in] */ String volumeName, 
    /* [in] */ String mimeType)
{
    //try {
        //initialize(volumeName);
        //prescan(path);

        //File file = new File(path);
        //// always scan the file, so we can return the content://media Uri for existing files
        //return mClient.doScanFile(path, mimeType, file.lastModified(), file.length(), TRUE);
    /*} catch (RemoteException e) {
        Log.e(TAG, "RemoteException in MediaScanner.scanFile()", e);
        return NULL;
    }*/


    return NULL;
}

// returns the number of matching file/directory names, starting from the right
Int32 MediaScanner::MatchPaths(
    /* [in] */ String path1, 
    /* [in] */ String path2) 
{
    Int32 result = 0;
    /*Int32 end1 = path1.length();
    Int32 end2 = path2.length();

    while (end1 > 0 && end2 > 0) {
        Int32 slash1 = path1.lastIndexOf('/', end1 - 1);
        Int32 slash2 = path2.lastIndexOf('/', end2 - 1);
        Int32 backSlash1 = path1.lastIndexOf('\\', end1 - 1);
        Int32 backSlash2 = path2.lastIndexOf('\\', end2 - 1);
        Int32 start1 = (slash1 > backSlash1 ? slash1 : backSlash1);
        Int32 start2 = (slash2 > backSlash2 ? slash2 : backSlash2);
        if (start1 < 0) start1 = 0; else start1++;
        if (start2 < 0) start2 = 0; else start2++;
        Int32 length = end1 - start1;
        if (end2 - start2 != length) break;
        if (path1.regionMatches(TRUE, start1, path2, start2, length)) {
            result++;
            end1 = start1 - 1;
            end2 = start2 - 1;
        } else break;
    }*/

    return result;
}

Boolean MediaScanner::AddPlayListEntry(
    /* [in] */ String entry, 
    /* [in] */ String playListDirectory,
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
    //    Log.e(TAG, "RemoteException in MediaScanner.addPlayListEntry()", e);
    //    return FALSE;
    //}*/

    return TRUE;
}

void MediaScanner::ProcessM3uPlayList(
    /* [in] */ String path, 
    /* [in] */ String playListDirectory, 
    /* [in] */ IUri* uri, 
    /* [in] */ IContentValues* values)
{
    //BufferedReader reader = NULL;
    ////try {
    //    File f = new File(path);
    //    if (f.exists()) {
    //        reader = new BufferedReader(
    //                new InputStreamReader(new FileInputStream(f)), 8192);
    //        String line = reader.readLine();
    //        Int32 index = 0;
    //        while (line != NULL) {
    //            // ignore comment lines, which begin with '#'
    //            if (line.length() > 0 && line.charAt(0) != '#') {
    //                values.clear();
    //                if (addPlayListEntry(line, playListDirectory, uri, values, index))
    //                    index++;
    //            }
    //            line = reader.readLine();
    //        }
    //    }
    /*} catch (IOException e) {
        Log.e(TAG, "IOException in MediaScanner.processM3uPlayList()", e);
    } finally {
        try {
            if (reader != NULL)
                reader.close();
        } catch (IOException e) {
            Log.e(TAG, "IOException in MediaScanner.processM3uPlayList()", e);
        }
    }*/
}

void MediaScanner::ProcessPlsPlayList(
    /* [in] */ String path, 
    /* [in] */ String playListDirectory, 
    /* [in] */ IUri* uri, 
    /* [in] */ IContentValues* values)
{
    //BufferedReader reader = NULL;
    ////try {
    //    File f = new File(path);
    //    if (f.exists()) {
    //        reader = new BufferedReader(
    //                new InputStreamReader(new FileInputStream(f)), 8192);
    //        String line = reader.readLine();
    //        Int32 index = 0;
    //        while (line != NULL) {
    //            // ignore comment lines, which begin with '#'
    //            if (line.startsWith("File")) {
    //                Int32 equals = line.indexOf('=');
    //                if (equals > 0) {
    //                    values.clear();
    //                    if (addPlayListEntry(line.substring(equals + 1), playListDirectory, uri, values, index))
    //                        index++;
    //                }
    //            }
    //            line = reader.readLine();
    //        }
    //    }
    /*} catch (IOException e) {
        Log.e(TAG, "IOException in MediaScanner.processPlsPlayList()", e);
    } finally {
        try {
            if (reader != NULL)
                reader.close();
        } catch (IOException e) {
            Log.e(TAG, "IOException in MediaScanner.processPlsPlayList()", e);
        }
    }*/
}


MediaScanner::WplHandler::WplHandler(
    /* [in] */ String playListDirectory, 
    /* [in] */ IUri* uri)
{
    /*this.playListDirectory = playListDirectory;
    this.uri = uri;

    RootElement root = new RootElement("smil");
    Element body = root.getChild("body");
    Element seq = body.getChild("seq");
    Element media = seq.getChild("media");
    media.setElementListener(this);

    this.handler = root.getContentHandler();*/
}

//void MediaScanner::WplHandler::Start(
//    /* [in] */ IAttributes* attributes)
//{
//    /*String path = attributes.getValue("", "src");
//    if (path != NULL) {
//        values.clear();
//        if (addPlayListEntry(path, playListDirectory, uri, values, index)) {
//            index++;
//        }
//    }*/
//}

void MediaScanner::WplHandler::End()
{
}

//AutoPtr<IContentHandler> MediaScanner::WplHandler::GetContentHandler()
//{
//    return handler;
//}


void MediaScanner::ProcessWplPlayList(
    /* [in] */ String path, 
    /* [in] */ String playListDirectory, 
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
            Log.e(TAG, "IOException in MediaScanner.processWplPlayList()", e);
        }
    }*/
}

void MediaScanner::ProcessPlayLists()
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
}

void MediaScanner::ProcessDirectory(
    /* [in] */ String path, 
    /* [in] */ String extensions, 
    /* [in] */ IMediaScannerClient* client)
{

}

void MediaScanner::ProcessFile(
    /* [in] */ String path, 
    /* [in] */ String mimeType, 
    /* [in] */ IMediaScannerClient* client)
{

}

void MediaScanner::SetLocale(
    /* [in] */ String locale)
{

}

ArrayOf<Byte>* MediaScanner::ExtractAlbumArt(
    /* [in] */ IFileDescriptor* fd)
{
    return NULL;
}

void MediaScanner::native_init()
{

}

void MediaScanner::native_setup()
{

}

void MediaScanner::native_finalize()
{

}

void MediaScanner::Finalize() 
{
    //mContext.getContentResolver().releaseProvider(mMediaProvider);
    native_finalize();
}

