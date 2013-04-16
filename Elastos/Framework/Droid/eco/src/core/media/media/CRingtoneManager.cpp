
#include "media/CRingtoneManager.h"
#include "media/CAudioManager.h"
#include "media/CRingtone.h"

#include <elastos/List.h>
const CString CRingtoneManager::TAG = "RingtoneManager";

CRingtoneManager::CRingtoneManager()
{

}

CRingtoneManager::~CRingtoneManager()
{

}

ECode CRingtoneManager::constructor(
    /* [in] */ IActivity* activity)
{
    mContext = mActivity = activity;
    SetType(mType);
    return NOERROR;
}

ECode CRingtoneManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    SetType(mType);
    return NOERROR;
}

ECode CRingtoneManager::SetType(
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

ECode CRingtoneManager::InferStreamType(
    /* [out] */ Int32 * type)
{
    switch (mType) {

        case RingtoneManager_TYPE_ALARM:
            return AudioManager_STREAM_ALARM;

        case RingtoneManager_TYPE_NOTIFICATION:
            return AudioManager_STREAM_NOTIFICATION;

        default:
            return AudioManager_STREAM_RING;
    }
}

ECode CRingtoneManager::SetStopPreviousRingtone(
    /* [in] */ Boolean stopPreviousRingtone)
{
    mStopPreviousRingtone = stopPreviousRingtone;

    return NOERROR;
}

ECode CRingtoneManager::GetStopPreviousRingtone(
    /* [out] */ Boolean* stopPreviousRingtone)
{
    VALIDATE_NOT_NULL(stopPreviousRingtone);
    *stopPreviousRingtone = mStopPreviousRingtone;
    return NOERROR;
}

ECode CRingtoneManager::StopPreviousRingtone()
{
    if (mPreviousRingtone != NULL) {
        mPreviousRingtone->Stop();
    }

    return NOERROR;
}

ECode CRingtoneManager::GetIncludeDrm(
    /* [out] */ Boolean* included)
{
    VALIDATE_NOT_NULL(included);
    *included = mIncludeDrm;
    return NOERROR;
}

ECode CRingtoneManager::SetIncludeDrm(
    /* [in] */ Boolean includeDrm)
{
    mIncludeDrm = includeDrm;

    return NOERROR;
}

ECode CRingtoneManager::GetCursor(
    /* [out] */ ICursor** curse)
{
    Boolean r;

    mCursor->Requery(&r);
    if (mCursor != NULL && r) {
        *curse = mCursor;
        return NOERROR;
    }

    AutoPtr<ICursor> internalCursor = GetInternalRingtones();
    AutoPtr<ICursor> drmCursor = mIncludeDrm ? GetDrmRingtones() : NULL;
    AutoPtr<ICursor> mediaCursor = GetMediaRingtones();

    /*return mCursor = new SortCursor(new Cursor[] { internalCursor, drmCursor, mediaCursor },
            MediaStore.Audio.Media.DEFAULT_SORT_ORDER);*/

    return NULL;
}

ECode CRingtoneManager::GetRingtone(
    /* [in] */ Int32 position,
    /* [out] */ IRingtone** ringtone)
{
    if (mStopPreviousRingtone && mPreviousRingtone != NULL) {
        mPreviousRingtone->Stop();
    }

    AutoPtr<IUri> tempUri;
    GetRingtoneUri(position,(IUri**) (&tempUri));
    Int32 tempInt32;
    InferStreamType(&tempInt32);
    mPreviousRingtone = GetRingtone((IContext*) (&mContext), (IUri*) (&tempUri), tempInt32);
    *ringtone = mPreviousRingtone;
    return NOERROR;
}

ECode CRingtoneManager::GetRingtoneUri(
    /* [in] */ Int32 position,
    /* [out] */ IUri** ppUri)
{
    AutoPtr<ICursor> cursor;
    GetCursor((ICursor**) (&cursor));

    Boolean res;
    cursor->MoveToPosition(position, &res);
    if (!res) {
        return NULL;
    }

    *ppUri = GetUriFromCursor(cursor);
    return NOERROR;
}

AutoPtr<IUri> CRingtoneManager::GetUriFromCursor(
    /* [in] */ ICursor* cursor)
{
    /*return ContentUris.withAppendedId(Uri.parse(cursor.getString(URI_COLUMN_INDEX)), cursor
            .getLong(ID_COLUMN_INDEX));*/

    return NULL;
}

ECode CRingtoneManager::GetRingtonePosition(
    /* [in] */ IUri* ringtoneUri,
    /* [out] */ Int32* position)
{

    if (ringtoneUri == NULL) return -1;

    AutoPtr<ICursor> cursor;
    GetCursor((ICursor**) (&cursor));
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
        cursor->GetString(RingtoneManager_URI_COLUMN_INDEX, &uriString);
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

AutoPtr<IUri> CRingtoneManager::GetValidRingtoneUri(
    /* [in] */ IContext* context)
{
    CRingtoneManager* rm;

    AutoPtr<IUri> uri = GetValidRingtoneUriFromCursorAndClose(context, rm->GetInternalRingtones());

    if (uri == NULL) {
        uri = GetValidRingtoneUriFromCursorAndClose(context, rm->GetMediaRingtones());
    }

    if (uri == NULL) {
        uri = GetValidRingtoneUriFromCursorAndClose(context, rm->GetDrmRingtones());
    }

    return uri;
}

AutoPtr<IUri> CRingtoneManager::GetValidRingtoneUriFromCursorAndClose(
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

AutoPtr<ICursor> CRingtoneManager::GetInternalRingtones()
{
    /*return query(
            MediaStore.Audio.Media.INTERNAL_CONTENT_URI, INTERNAL_COLUMNS,
            constructBooleanTrueWhereClause(mFilterColumns),
            NULL, MediaStore.Audio.Media.DEFAULT_SORT_ORDER);*/

    return NULL;
}

AutoPtr<ICursor> CRingtoneManager::GetDrmRingtones()
{
    // DRM store does not have any columns to use for filtering
    /*return query(
            DrmStore.Audio.CONTENT_URI, DRM_COLUMNS,
            NULL, NULL, DrmStore.Audio.TITLE);*/

    return NULL;
}

AutoPtr<ICursor> CRingtoneManager::GetMediaRingtones()
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

void CRingtoneManager::SetFilterColumnsList(
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


String CRingtoneManager::ConstructBooleanTrueWhereClause(
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

AutoPtr<ICursor> CRingtoneManager::Query(
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

ECode CRingtoneManager::GetRingtone(
    /* [in] */ IContext* context,
    /* [in] */ IUri* ringtoneUri,
    /* [out] */ IRingtone** ringtone)
{
    // Don't set the stream type
    *ringtone = GetRingtone(context, ringtoneUri, -1);
    return NOERROR;
}

AutoPtr<IRingtone> CRingtoneManager::GetRingtone(
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

AutoPtr<IUri> CRingtoneManager::GetActualDefaultRingtoneUri(
    /* [in] */ IContext* context,
    /* [in] */ Int32 type)
{
    /*String setting = getSettingForType(type);
    if (setting == NULL) return NULL;
    final String uriString = Settings.System.getString(context.getContentResolver(), setting);
    return uriString != NULL ? Uri.parse(uriString) : NULL;*/

    return NULL;
}

ECode CRingtoneManager::SetActualDefaultRingtoneUri(
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

String CRingtoneManager::GetSettingForType(
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

ECode CRingtoneManager::IsDefault(
    /* [in] */ IUri* ringtoneUri,
    /* [out] */ Boolean* pDefault)
{
    *pDefault = (GetDefaultType(ringtoneUri) != -1);
    return NOERROR;
}

Int32 CRingtoneManager::GetDefaultType(
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

ECode CRingtoneManager::GetDefaultUri(
    /* [in] */ Int32 type,
    /* [out] */ IUri ** ppUri)
{
    VALIDATE_NOT_NULL(ppUri);
    /*if ((type & TYPE_RINGTONE) != 0) {
        return Settings.System.DEFAULT_RINGTONE_URI;
    } else if ((type & TYPE_NOTIFICATION) != 0) {
        return Settings.System.DEFAULT_NOTIFICATION_URI;
    } else if ((type & TYPE_ALARM) != 0) {
        return Settings.System.DEFAULT_ALARM_ALERT_URI;
    } else {
        return NULL;
    }*/

    return E_NOT_IMPLEMENTED;
}
