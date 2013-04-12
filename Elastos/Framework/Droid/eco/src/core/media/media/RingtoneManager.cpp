
#include "media/RingtoneManager.h"
#include "media/CAudioManager.h"
#include "media/CRingtone.h"

#include <elastos/List.h>

RingtoneManager::RingtoneManager()
{

}

/**
 * Constructs a RingtoneManager. This constructor is recommended as its
 * constructed instance manages cursor(s).
 *
 * @param activity The activity used to get a managed cursor.
 */
RingtoneManager::RingtoneManager(
    /* [in] */ IActivity* activity)
{
    mContext = mActivity = activity;
    SetType(mType);
}

/**
 * Constructs a RingtoneManager. The instance constructed by this
 * constructor will not manage the cursor(s), so the client should handle
 * this itself.
 *
 * @param context The context to used to get a cursor.
 */
RingtoneManager::RingtoneManager(
    /* [in] */ IContext* context)
{
    mContext = context;
    SetType(mType);
}

/**
 * Sets which type(s) of ringtones will be listed by this.
 *
 * @param type The type(s), one or more of {@link #TYPE_RINGTONE},
 *            {@link #TYPE_NOTIFICATION}, {@link #TYPE_ALARM},
 *            {@link #TYPE_ALL}.
 * @see #EXTRA_RINGTONE_TYPE
 */
ECode RingtoneManager::SetType(
    /* [in] */ Int32 type)
{

    if (mCursor != NULL) {
        /*throw new IllegalStateException(
                "Setting filter columns should be done before querying for ringtones.");*/
    }

    mType = type;
    SetFilterColumnsList(type);

    return NOERROR;
}

/**
 * Infers the playback stream type based on what type of ringtones this
 * manager is returning.
 *
 * @return The stream type.
 */
Int32 RingtoneManager::InferStreamType()
{
    switch (mType) {

        case TYPE_ALARM:
            return AudioManager_STREAM_ALARM;

        case TYPE_NOTIFICATION:
            return AudioManager_STREAM_NOTIFICATION;

        default:
            return AudioManager_STREAM_RING;
    }
}

/**
 * Whether retrieving another {@link Ringtone} will stop playing the
 * previously retrieved {@link Ringtone}.
 * <p>
 * If this is false, make sure to {@link Ringtone#stop()} any previous
 * ringtones to free resources.
 *
 * @param stopPreviousRingtone If TRUE, the previously retrieved
 *            {@link Ringtone} will be stopped.
 */
ECode RingtoneManager::SetStopPreviousRingtone(
    /* [in] */ Boolean stopPreviousRingtone)
{
    mStopPreviousRingtone = stopPreviousRingtone;

    return NOERROR;
}

/**
 * @see #setStopPreviousRingtone(Boolean)
 */
Boolean RingtoneManager::GetStopPreviousRingtone()
{
    return mStopPreviousRingtone;
}

/**
 * Stops playing the last {@link Ringtone} retrieved from this.
 */
ECode RingtoneManager::StopPreviousRingtone()
{
    if (mPreviousRingtone != NULL) {
        mPreviousRingtone->Stop();
    }

    return NOERROR;
}

/**
 * Returns whether DRM ringtones will be included.
 *
 * @return Whether DRM ringtones will be included.
 * @see #setIncludeDrm(Boolean)
 */
Boolean RingtoneManager::GetIncludeDrm()
{
    return mIncludeDrm;
}

/**
 * Sets whether to include DRM ringtones.
 *
 * @param includeDrm Whether to include DRM ringtones.
 */
ECode RingtoneManager::SetIncludeDrm(
    /* [in] */ Boolean includeDrm)
{
    mIncludeDrm = includeDrm;

    return NOERROR;
}

/**
 * Returns a {@link Cursor} of all the ringtones available. The returned
 * cursor will be the same cursor returned each time this method is called,
 * so do not {@link Cursor#close()} the cursor. The cursor can be
 * {@link Cursor#deactivate()} safely.
 * <p>
 * If {@link RingtoneManager#RingtoneManager(Activity)} was not used, the
 * caller should manage the returned cursor through its activity's life
 * cycle to prevent leaking the cursor.
 *
 * @return A {@link Cursor} of all the ringtones available.
 * @see #ID_COLUMN_INDEX
 * @see #TITLE_COLUMN_INDEX
 * @see #URI_COLUMN_INDEX
 */
AutoPtr<ICursor> RingtoneManager::GetCursor()
{
    Boolean r;

    mCursor->Requery(&r);
    if (mCursor != NULL && r) {
        return mCursor;
    }

    AutoPtr<ICursor> internalCursor = GetInternalRingtones();
    AutoPtr<ICursor> drmCursor = mIncludeDrm ? GetDrmRingtones() : NULL;
    AutoPtr<ICursor> mediaCursor = GetMediaRingtones();

    /*return mCursor = new SortCursor(new Cursor[] { internalCursor, drmCursor, mediaCursor },
            MediaStore.Audio.Media.DEFAULT_SORT_ORDER);*/

    return NULL;
}

/**
 * Gets a {@link Ringtone} for the ringtone at the given position in the
 * {@link Cursor}.
 *
 * @param position The position (in the {@link Cursor}) of the ringtone.
 * @return A {@link Ringtone} pointing to the ringtone.
 */
AutoPtr<IRingtone> RingtoneManager::GetRingtone(
    /* [in] */ Int32 position)
{
    if (mStopPreviousRingtone && mPreviousRingtone != NULL) {
        mPreviousRingtone->Stop();
    }

    mPreviousRingtone = GetRingtone(mContext, GetRingtoneUri(position), InferStreamType());
    return mPreviousRingtone;
}

/**
 * Gets a {@link Uri} for the ringtone at the given position in the {@link Cursor}.
 *
 * @param position The position (in the {@link Cursor}) of the ringtone.
 * @return A {@link Uri} pointing to the ringtone.
 */
AutoPtr<IUri> RingtoneManager::GetRingtoneUri(
    /* [in] */ Int32 position)
{
    AutoPtr<ICursor> cursor = GetCursor();

    Boolean res;
    cursor->MoveToPosition(position, &res);
    if (!res) {
        return NULL;
    }

    return GetUriFromCursor(cursor);
}

AutoPtr<IUri> RingtoneManager::GetUriFromCursor(
    /* [in] */ ICursor* cursor)
{
    /*return ContentUris.withAppendedId(Uri.parse(cursor.getString(URI_COLUMN_INDEX)), cursor
            .getLong(ID_COLUMN_INDEX));*/

    return NULL;
}

/**
 * Gets the position of a {@link Uri} within this {@link RingtoneManager}.
 *
 * @param ringtoneUri The {@link Uri} to retreive the position of.
 * @return The position of the {@link Uri}, or -1 if it cannot be found.
 */
Int32 RingtoneManager::GetRingtonePosition(
    /* [in] */ IUri* ringtoneUri)
{

    if (ringtoneUri == NULL) return -1;

    AutoPtr<ICursor> cursor = GetCursor();
    Int32 cursorCount;
    cursor->GetCount(&cursorCount);

    Boolean res;
    cursor->MoveToFirst(&res);
    if (!res) {
        return -1;
    }

    // Only create Uri objects when the actual URI changes
    AutoPtr<IUri> currentUri = NULL;
    String previousUriString;// = NULL;
    for (Int32 i = 0; i < cursorCount; i++) {
        String uriString;
        cursor->GetString(URI_COLUMN_INDEX, &uriString);
        if (currentUri == NULL || !uriString.Equals(previousUriString)) {
            //Uri->Parse(uriString, &currentUri);
        }

        /*if (ringtoneUri.equals(ContentUris.withAppendedId(currentUri, cursor
                .getLong(ID_COLUMN_INDEX)))) {
            return i;
        }*/

        Boolean res;
        cursor->Move(1, &res);

        previousUriString = uriString;
    }

    return -1;
}

/**
 * Returns a valid ringtone URI. No guarantees on which it returns. If it
 * cannot find one, returns NULL.
 *
 * @param context The context to use for querying.
 * @return A ringtone URI, or NULL if one cannot be found.
 */
AutoPtr<IUri> RingtoneManager::GetValidRingtoneUri(
    /* [in] */ IContext* context)
{
    RingtoneManager* rm = new RingtoneManager(context);

    AutoPtr<IUri> uri = GetValidRingtoneUriFromCursorAndClose(context, rm->GetInternalRingtones());

    if (uri == NULL) {
        uri = GetValidRingtoneUriFromCursorAndClose(context, rm->GetMediaRingtones());
    }

    if (uri == NULL) {
        uri = GetValidRingtoneUriFromCursorAndClose(context, rm->GetDrmRingtones());
    }

    return uri;
}

AutoPtr<IUri> RingtoneManager::GetValidRingtoneUriFromCursorAndClose(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor)
{
    if (cursor != NULL) {
        AutoPtr<IUri> uri = NULL;

        Boolean res;
        cursor->MoveToFirst(&res);
        if (res) {
            uri = GetUriFromCursor(cursor);
        }
        cursor->Close();

        return uri;
    } else {
        return NULL;
    }
}

AutoPtr<ICursor> RingtoneManager::GetInternalRingtones()
{
    /*return query(
            MediaStore.Audio.Media.INTERNAL_CONTENT_URI, INTERNAL_COLUMNS,
            constructBooleanTrueWhereClause(mFilterColumns),
            NULL, MediaStore.Audio.Media.DEFAULT_SORT_ORDER);*/

    return NULL;
}

AutoPtr<ICursor> RingtoneManager::GetDrmRingtones()
{
    // DRM store does not have any columns to use for filtering
    /*return query(
            DrmStore.Audio.CONTENT_URI, DRM_COLUMNS,
            NULL, NULL, DrmStore.Audio.TITLE);*/

    return NULL;
}

AutoPtr<ICursor> RingtoneManager::GetMediaRingtones()
{
     // Get the external media cursor. First check to see if it is mounted.
    /*final String status = Environment.getExternalStorageState();

    return (status.equals(Environment.MEDIA_MOUNTED) ||
                status.equals(Environment.MEDIA_MOUNTED_READ_ONLY))
            ? query(
                MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, MEDIA_COLUMNS,
                constructBooleanTrueWhereClause(mFilterColumns), NULL,
                MediaStore.Audio.Media.DEFAULT_SORT_ORDER)
            : NULL;*/

    return NULL;
}

void RingtoneManager::SetFilterColumnsList(
    /* [in] */ Int32 type)
{
    /*List<String> columns = mFilterColumns;
    columns.clear();

    if ((type & TYPE_RINGTONE) != 0) {
        columns.add(MediaStore.Audio.AudioColumns.IS_RINGTONE);
    }

    if ((type & TYPE_NOTIFICATION) != 0) {
        columns.add(MediaStore.Audio.AudioColumns.IS_NOTIFICATION);
    }

    if ((type & TYPE_ALARM) != 0) {
        columns.add(MediaStore.Audio.AudioColumns.IS_ALARM);
    }*/
}

/**
 * Constructs a where clause that consists of at least one column being 1
 * (TRUE). This is used to find all matching sounds for the given sound
 * types (ringtone, notifications, etc.)
 *
 * @param columns The columns that must be TRUE.
 * @return The where clause.
 */
String RingtoneManager::ConstructBooleanTrueWhereClause(
    /* [in] */ List<String>* columns)
{

    //if (columns == NULL) return NULL;
    //
    //StringBuilder sb = new StringBuilder();
    //for (Int32 i = columns.size() - 1; i >= 0; i--) {
    //    sb.append(columns.get(i)).append("=1 or ");
    //}
    //
    //if (columns.size() > 0) {
    //    // Remove last ' or '
    //    sb.setLength(sb.length() - 4);
    //}
    //
    //return sb.toString();

    return String("");
}

AutoPtr<ICursor> RingtoneManager::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ String selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ String sortOrder)
{
    /*if (mActivity != NULL) {
        return mActivity.managedQuery(uri, projection, selection, selectionArgs, sortOrder);
    } else {
        return mContext.getContentResolver().query(uri, projection, selection, selectionArgs,
                sortOrder);
    }*/

    return NULL;
}

/**
 * Returns a {@link Ringtone} for a given sound URI.
 * <p>
 * If the given URI cannot be opened for any reason, this method will
 * attempt to fallback on another sound. If it cannot find any, it will
 * return NULL.
 *
 * @param context A context used to query.
 * @param ringtoneUri The {@link Uri} of a sound or ringtone.
 * @return A {@link Ringtone} for the given URI, or NULL.
 */
AutoPtr<IRingtone> RingtoneManager::GetRingtone(
    /* [in] */ IContext* context,
    /* [in] */ IUri* ringtoneUri)
{
    // Don't set the stream type
    return GetRingtone(context, ringtoneUri, -1);
}

/**
 * Returns a {@link Ringtone} for a given sound URI on the given stream
 * type. Normally, if you change the stream type on the returned
 * {@link Ringtone}, it will re-create the {@link MediaPlayer}. This is just
 * an optimized route to avoid that.
 *
 * @param streamType The stream type for the ringtone, or -1 if it should
 *            not be set (and the default used instead).
 * @see #getRingtone(Context, Uri)
 */
AutoPtr<IRingtone> RingtoneManager::GetRingtone(
    /* [in] */ IContext* context,
    /* [in] */ IUri* ringtoneUri,
    /* [in] */ Int32 streamType)
{

    //try {
        AutoPtr<IRingtone> r;
        CRingtone::New(context, (IRingtone**)&r);
        if (streamType >= 0) {
            r->SetStreamType(streamType);
        }
        ((CRingtone*)r.Get())->Open(ringtoneUri);
        return r;
    /*} catch (Exception ex) {
        Log.e(TAG, "Failed to open ringtone " + ringtoneUri);
    }*/

    return NULL;
}

/**
 * Gets the current default sound's {@link Uri}. This will give the actual
 * sound {@link Uri}, instead of using this, most clients can use
 * {@link System#DEFAULT_RINGTONE_URI}.
 *
 * @param context A context used for querying.
 * @param type The type whose default sound should be returned. One of
 *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION}, or
 *            {@link #TYPE_ALARM}.
 * @return A {@link Uri} pointing to the default sound for the sound type.
 * @see #setActualDefaultRingtoneUri(Context, Int32, Uri)
 */
AutoPtr<IUri> RingtoneManager::GetActualDefaultRingtoneUri(
    /* [in] */ IContext* context,
    /* [in] */ Int32 type)
{
    /*String setting = getSettingForType(type);
    if (setting == NULL) return NULL;
    final String uriString = Settings.System.getString(context.getContentResolver(), setting);
    return uriString != NULL ? Uri.parse(uriString) : NULL;*/

    return NULL;
}

/**
 * Sets the {@link Uri} of the default sound for a given sound type.
 *
 * @param context A context used for querying.
 * @param type The type whose default sound should be set. One of
 *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION}, or
 *            {@link #TYPE_ALARM}.
 * @param ringtoneUri A {@link Uri} pointing to the default sound to set.
 * @see #getActualDefaultRingtoneUri(Context, Int32)
 */
ECode RingtoneManager::SetActualDefaultRingtoneUri(
    /* [in] */ IContext* context,
    /* [in] */ Int32 type,
    /* [in] */ IUri* ringtoneUri)
{
    /*String setting = getSettingForType(type);
    if (setting == NULL) return;
    Settings.System.putString(context.getContentResolver(), setting,
            ringtoneUri != NULL ? ringtoneUri.toString() : NULL);*/

    return NOERROR;
}

String RingtoneManager::GetSettingForType(
    /* [in] */ Int32 type)
{
    /*if ((type & TYPE_RINGTONE) != 0) {
        return Settings.System.RINGTONE;
    } else if ((type & TYPE_NOTIFICATION) != 0) {
        return Settings.System.NOTIFICATION_SOUND;
    } else if ((type & TYPE_ALARM) != 0) {
        return Settings.System.ALARM_ALERT;
    } else {
        return NULL;
    }*/

    return String("");
}

/**
 * Returns whether the given {@link Uri} is one of the default ringtones.
 *
 * @param ringtoneUri The ringtone {@link Uri} to be checked.
 * @return Whether the {@link Uri} is a default.
 */
Boolean RingtoneManager::IsDefault(
    /* [in] */ IUri* ringtoneUri)
{
    return GetDefaultType(ringtoneUri) != -1;
}

/**
 * Returns the type of a default {@link Uri}.
 *
 * @param defaultRingtoneUri The default {@link Uri}. For example,
 *            {@link System#DEFAULT_RINGTONE_URI},
 *            {@link System#DEFAULT_NOTIFICATION_URI}, or
 *            {@link System#DEFAULT_ALARM_ALERT_URI}.
 * @return The type of the defaultRingtoneUri, or -1.
 */
Int32 RingtoneManager::GetDefaultType(
    /* [in] */ IUri* defaultRingtoneUri)
{
    /*if (defaultRingtoneUri == NULL) {
        return -1;
    } else if (defaultRingtoneUri.equals(Settings.System.DEFAULT_RINGTONE_URI)) {
        return TYPE_RINGTONE;
    } else if (defaultRingtoneUri.equals(Settings.System.DEFAULT_NOTIFICATION_URI)) {
        return TYPE_NOTIFICATION;
    } else if (defaultRingtoneUri.equals(Settings.System.DEFAULT_ALARM_ALERT_URI)) {
        return TYPE_ALARM;
    } else {
        return -1;
    }*/

    return 0;
}

/**
 * Returns the {@link Uri} for the default ringtone of a particular type.
 * Rather than returning the actual ringtone's sound {@link Uri}, this will
 * return the symbolic {@link Uri} which will resolved to the actual sound
 * when played.
 *
 * @param type The ringtone type whose default should be returned.
 * @return The {@link Uri} of the default ringtone for the given type.
 */
AutoPtr<IUri> RingtoneManager::GetDefaultUri(
    /* [in] */ Int32 type)
{
    /*if ((type & TYPE_RINGTONE) != 0) {
        return Settings.System.DEFAULT_RINGTONE_URI;
    } else if ((type & TYPE_NOTIFICATION) != 0) {
        return Settings.System.DEFAULT_NOTIFICATION_URI;
    } else if ((type & TYPE_ALARM) != 0) {
        return Settings.System.DEFAULT_ALARM_ALERT_URI;
    } else {
        return NULL;
    }*/

    return NULL;
}

