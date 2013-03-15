
#ifndef __RINGTONEMANAGER_H__
#define __RINGTONEMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/List.h>

/**
 * RingtoneManager provides access to ringtones, notification, and other types
 * of sounds. It manages querying the different media providers and combines the
 * results into a single cursor. It also provides a {@link Ringtone} for each
 * ringtone. We generically call these sounds ringtones, however the
 * {@link #TYPE_RINGTONE} refers to the type of sounds that are suitable for the
 * phone ringer.
 * <p>
 * To show a ringtone picker to the user, use the
 * {@link #ACTION_RINGTONE_PICKER} intent to launch the picker as a subactivity.
 * 
 * @see Ringtone
 */
class RingtoneManager
{
public:
    RingtoneManager();

    /**
     * Constructs a RingtoneManager. This constructor is recommended as its
     * constructed instance manages cursor(s).
     * 
     * @param activity The activity used to get a managed cursor.
     */
    RingtoneManager(
        /* [in] */ IActivity* activity);

    /**
     * Constructs a RingtoneManager. The instance constructed by this
     * constructor will not manage the cursor(s), so the client should handle
     * this itself.
     * 
     * @param context The context to used to get a cursor.
     */
    RingtoneManager(
        /* [in] */ IContext* context);

    /**
     * Sets which type(s) of ringtones will be listed by this.
     * 
     * @param type The type(s), one or more of {@link #TYPE_RINGTONE},
     *            {@link #TYPE_NOTIFICATION}, {@link #TYPE_ALARM},
     *            {@link #TYPE_ALL}.
     * @see #EXTRA_RINGTONE_TYPE           
     */
    virtual CARAPI SetType(
        /* [in] */ Int32 type);

    /**
     * Infers the playback stream type based on what type of ringtones this
     * manager is returning.
     * 
     * @return The stream type.
     */
    virtual CARAPI_(Int32) InferStreamType();
    
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
    virtual CARAPI SetStopPreviousRingtone(
        /* [in] */ Boolean stopPreviousRingtone);

    /**
     * @see #setStopPreviousRingtone(Boolean)
     */
    virtual CARAPI_(Boolean) GetStopPreviousRingtone();

    /**
     * Stops playing the last {@link Ringtone} retrieved from this.
     */
    virtual CARAPI StopPreviousRingtone();
    
    /**
     * Returns whether DRM ringtones will be included.
     * 
     * @return Whether DRM ringtones will be included.
     * @see #setIncludeDrm(Boolean)
     */
    virtual CARAPI_(Boolean) GetIncludeDrm();

    /**
     * Sets whether to include DRM ringtones.
     * 
     * @param includeDrm Whether to include DRM ringtones.
     */
    virtual CARAPI SetIncludeDrm(
        /* [in] */ Boolean includeDrm);

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
    virtual CARAPI_(AutoPtr<ICursor>) GetCursor();

    /**
     * Gets a {@link Ringtone} for the ringtone at the given position in the
     * {@link Cursor}.
     * 
     * @param position The position (in the {@link Cursor}) of the ringtone.
     * @return A {@link Ringtone} pointing to the ringtone.
     */
    virtual CARAPI_(AutoPtr<IRingtone>) GetRingtone(
        /* [in] */ Int32 position);

    /**
     * Gets a {@link Uri} for the ringtone at the given position in the {@link Cursor}.
     * 
     * @param position The position (in the {@link Cursor}) of the ringtone.
     * @return A {@link Uri} pointing to the ringtone.
     */
    virtual CARAPI_(AutoPtr<IUri>) GetRingtoneUri(
        /* [in] */ Int32 position);
    
    /**
     * Gets the position of a {@link Uri} within this {@link RingtoneManager}.
     * 
     * @param ringtoneUri The {@link Uri} to retreive the position of.
     * @return The position of the {@link Uri}, or -1 if it cannot be found.
     */
    virtual CARAPI_(Int32) GetRingtonePosition(
        /* [in] */ IUri* ringtoneUri);

    /**
     * Returns a valid ringtone URI. No guarantees on which it returns. If it
     * cannot find one, returns NULL.
     * 
     * @param context The context to use for querying.
     * @return A ringtone URI, or NULL if one cannot be found.
     */
    static CARAPI_(AutoPtr<IUri>) GetValidRingtoneUri(
        /* [in] */ IContext* context);  
    
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
    static CARAPI_(AutoPtr<IRingtone>) GetRingtone(
        /* [in] */ IContext* context, 
        /* [in] */ IUri* ringtoneUri);
    
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
    static CARAPI_(AutoPtr<IUri>) GetActualDefaultRingtoneUri(
        /* [in] */ IContext* context, 
        /* [in] */ Int32 type);
    
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
    static CARAPI SetActualDefaultRingtoneUri(
        /* [in] */ IContext* context, 
        /* [in] */ Int32 type, 
        /* [in] */ IUri* ringtoneUri);
    
    /**
     * Returns whether the given {@link Uri} is one of the default ringtones.
     * 
     * @param ringtoneUri The ringtone {@link Uri} to be checked.
     * @return Whether the {@link Uri} is a default.
     */
    static CARAPI_(Boolean) IsDefault(
        /* [in] */ IUri* ringtoneUri);
    
    /**
     * Returns the type of a default {@link Uri}.
     * 
     * @param defaultRingtoneUri The default {@link Uri}. For example,
     *            {@link System#DEFAULT_RINGTONE_URI},
     *            {@link System#DEFAULT_NOTIFICATION_URI}, or
     *            {@link System#DEFAULT_ALARM_ALERT_URI}.
     * @return The type of the defaultRingtoneUri, or -1.
     */
    static CARAPI_(Int32) GetDefaultType(
        /* [in] */ IUri* defaultRingtoneUri);
 
    /**
     * Returns the {@link Uri} for the default ringtone of a particular type.
     * Rather than returning the actual ringtone's sound {@link Uri}, this will
     * return the symbolic {@link Uri} which will resolved to the actual sound
     * when played.
     * 
     * @param type The ringtone type whose default should be returned.
     * @return The {@link Uri} of the default ringtone for the given type.
     */
    static CARAPI_(AutoPtr<IUri>) GetDefaultUri(
        /* [in] */ Int32 type);

private:
    static CARAPI_(AutoPtr<IUri>) GetUriFromCursor(
        /* [in] */ ICursor* cursor);

    static CARAPI_(AutoPtr<IUri>) GetValidRingtoneUriFromCursorAndClose(
        /* [in] */ IContext* context, 
        /* [in] */ ICursor* cursor);

    CARAPI_(AutoPtr<ICursor>) GetInternalRingtones();
    
    CARAPI_(AutoPtr<ICursor>) GetDrmRingtones();

    CARAPI_(AutoPtr<ICursor>) GetMediaRingtones();
    
    CARAPI_(void) SetFilterColumnsList(
        /* [in] */ Int32 type);
    
    /**
     * Constructs a where clause that consists of at least one column being 1
     * (TRUE). This is used to find all matching sounds for the given sound
     * types (ringtone, notifications, etc.)
     * 
     * @param columns The columns that must be TRUE.
     * @return The where clause.
     */
    static CARAPI_(String) ConstructBooleanTrueWhereClause(
        /* [in] */ List<String>* columns);
    
    CARAPI_(AutoPtr<ICursor>) Query(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String>* projection,
        /* [in] */ String selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ String sortOrder);

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
    static CARAPI_(AutoPtr<IRingtone>) GetRingtone(
        /* [in] */ IContext* context, 
        /* [in] */ IUri* ringtoneUri, 
        /* [in] */ Int32 streamType);

    static CARAPI_(String) GetSettingForType(
        /* [in] */ Int32 type);

public:
    // Make sure these are in sync with attrs.xml:
    // <attr name="ringtoneType">
    
    /**
     * Type that refers to sounds that are used for the phone ringer.
     */
    static const Int32 TYPE_RINGTONE = 1;
    
    /**
     * Type that refers to sounds that are used for notifications.
     */
    static const Int32 TYPE_NOTIFICATION = 2;
    
    /**
     * Type that refers to sounds that are used for the alarm.
     */
    static const Int32 TYPE_ALARM = 4;
    
    /**
     * All types of sounds.
     */
    static const Int32 TYPE_ALL = TYPE_RINGTONE | TYPE_NOTIFICATION | TYPE_ALARM;
    
    // </attr>
    
    /**
     * Activity Action: Shows a ringtone picker.
     * <p>
     * Input: {@link #EXTRA_RINGTONE_EXISTING_URI},
     * {@link #EXTRA_RINGTONE_SHOW_DEFAULT},
     * {@link #EXTRA_RINGTONE_SHOW_SILENT}, {@link #EXTRA_RINGTONE_TYPE},
     * {@link #EXTRA_RINGTONE_DEFAULT_URI}, {@link #EXTRA_RINGTONE_TITLE},
     * {@link #EXTRA_RINGTONE_INCLUDE_DRM}.
     * <p>
     * Output: {@link #EXTRA_RINGTONE_PICKED_URI}.
     */
    static String ACTION_RINGTONE_PICKER;// = "android.intent.action.RINGTONE_PICKER";

    /**
     * Given to the ringtone picker as a Boolean. Whether to show an item for
     * "Default".
     * 
     * @see #ACTION_RINGTONE_PICKER
     */
    static String EXTRA_RINGTONE_SHOW_DEFAULT;/* =
            "android.intent.extra.ringtone.SHOW_DEFAULT";*/
    
    /**
     * Given to the ringtone picker as a Boolean. Whether to show an item for
     * "Silent". If the "Silent" item is picked,
     * {@link #EXTRA_RINGTONE_PICKED_URI} will be NULL.
     * 
     * @see #ACTION_RINGTONE_PICKER
     */
    static const String EXTRA_RINGTONE_SHOW_SILENT;/* =
            "android.intent.extra.ringtone.SHOW_SILENT";*/

    /**
     * Given to the ringtone picker as a Boolean. Whether to include DRM ringtones.
     */
    static String EXTRA_RINGTONE_INCLUDE_DRM;/* =
            "android.intent.extra.ringtone.INCLUDE_DRM";*/
    
    /**
     * Given to the ringtone picker as a {@link Uri}. The {@link Uri} of the
     * current ringtone, which will be used to show a checkmark next to the item
     * for this {@link Uri}. If showing an item for "Default" (@see
     * {@link #EXTRA_RINGTONE_SHOW_DEFAULT}), this can also be one of
     * {@link System#DEFAULT_RINGTONE_URI},
     * {@link System#DEFAULT_NOTIFICATION_URI}, or
     * {@link System#DEFAULT_ALARM_ALERT_URI} to have the "Default" item
     * checked.
     * 
     * @see #ACTION_RINGTONE_PICKER
     */
    static String EXTRA_RINGTONE_EXISTING_URI;/* =
            "android.intent.extra.ringtone.EXISTING_URI";*/
    
    /**
     * Given to the ringtone picker as a {@link Uri}. The {@link Uri} of the
     * ringtone to play when the user attempts to preview the "Default"
     * ringtone. This can be one of {@link System#DEFAULT_RINGTONE_URI},
     * {@link System#DEFAULT_NOTIFICATION_URI}, or
     * {@link System#DEFAULT_ALARM_ALERT_URI} to have the "Default" point to
     * the current sound for the given default sound type. If you are showing a
     * ringtone picker for some other type of sound, you are free to provide any
     * {@link Uri} here.
     */
    static String EXTRA_RINGTONE_DEFAULT_URI;/* =
            "android.intent.extra.ringtone.DEFAULT_URI";*/
    
    /**
     * Given to the ringtone picker as an Int32. Specifies which ringtone type(s) should be
     * shown in the picker. One or more of {@link #TYPE_RINGTONE},
     * {@link #TYPE_NOTIFICATION}, {@link #TYPE_ALARM}, or {@link #TYPE_ALL}
     * (bitwise-ored together).
     */
    static String EXTRA_RINGTONE_TYPE;// = "android.intent.extra.ringtone.TYPE";

    /**
     * Given to the ringtone picker as a {@link CharSequence}. The title to
     * show for the ringtone picker. This has a default value that is suitable
     * in most cases.
     */
    static String EXTRA_RINGTONE_TITLE;// = "android.intent.extra.ringtone.TITLE";
    
    /**
     * Returned from the ringtone picker as a {@link Uri}.
     * <p>
     * It will be one of:
     * <li> the picked ringtone,
     * <li> a {@link Uri} that equals {@link System#DEFAULT_RINGTONE_URI},
     * {@link System#DEFAULT_NOTIFICATION_URI}, or
     * {@link System#DEFAULT_ALARM_ALERT_URI} if the default was chosen,
     * <li> NULL if the "Silent" item was picked.
     * 
     * @see #ACTION_RINGTONE_PICKER
     */
    static String EXTRA_RINGTONE_PICKED_URI;/* =
            "android.intent.extra.ringtone.PICKED_URI";*/

    /**
     * The column index (in the cursor returned by {@link #getCursor()} for the
     * row ID.
     */
    static const Int32 ID_COLUMN_INDEX = 0;

    /**
     * The column index (in the cursor returned by {@link #getCursor()} for the
     * title.
     */
    static const Int32 TITLE_COLUMN_INDEX = 1;

    /**
     * The column index (in the cursor returned by {@link #getCursor()} for the
     * media provider's URI.
     */
    static const Int32 URI_COLUMN_INDEX = 2;

private:

    static String TAG;// = "RingtoneManager";
    
    // Make sure the column ordering and then ..._COLUMN_INDEX are in sync
    
    static ArrayOf<String>* INTERNAL_COLUMNS;/* = new String[] {
        MediaStore.Audio.Media._ID, MediaStore.Audio.Media.TITLE,
        "\"" + MediaStore.Audio.Media.INTERNAL_CONTENT_URI + "\"",
        MediaStore.Audio.Media.TITLE_KEY
    };*/

    static ArrayOf<String>* DRM_COLUMNS;/* = new String[] {
        DrmStore.Audio._ID, DrmStore.Audio.TITLE,
        "\"" + DrmStore.Audio.CONTENT_URI + "\"",
        DrmStore.Audio.TITLE + " AS " + MediaStore.Audio.Media.TITLE_KEY
    };*/

    static ArrayOf<String> MEDIA_COLUMNS;/* = new String[] {
        MediaStore.Audio.Media._ID, MediaStore.Audio.Media.TITLE,
        "\"" + MediaStore.Audio.Media.EXTERNAL_CONTENT_URI + "\"",
        MediaStore.Audio.Media.TITLE_KEY
    };*/

    AutoPtr<IActivity> mActivity;
    AutoPtr<IContext> mContext;
    
    AutoPtr<ICursor> mCursor;

    Int32 mType;// = TYPE_RINGTONE;
    
    /**
     * If a column (item from this list) exists in the Cursor, its value must
     * be TRUE (value of 1) for the row to be returned.
     */
    List<String>* mFilterColumns;// = new ArrayList<String>();
    
    Boolean mStopPreviousRingtone;// = TRUE;
    AutoPtr<IRingtone> mPreviousRingtone;

    Boolean mIncludeDrm;  
};

#endif
