
#ifndef __HH_ENVIRONMENT_H
#define __HH_ENVIRONMENT_H

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

/**
 * Provides access to environment variables.
 */
class Environment
{
public:
    /**
     * Gets the Android root directory.
     */
    static CARAPI_(AutoPtr<IFile>) GetRootDirectory();

    /**
     * Gets the system directory available for secure storage.
     * If Encrypted File system is enabled, it returns an encrypted directory (/data/secure/system).
     * Otherwise, it returns the unencrypted /data/system directory.
     * @return File object representing the secure storage system directory.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetSystemSecureDirectory();

    /**
     * Gets the data directory for secure storage.
     * If Encrypted File system is enabled, it returns an encrypted directory (/data/secure).
     * Otherwise, it returns the unencrypted /data directory.
     * @return File object representing the data directory for secure storage.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetSecureDataDirectory();

    static CARAPI_(Boolean) IsEncryptedFilesystemEnabled();

    /**
     * Gets the Android data directory.
     */
    static CARAPI_(AutoPtr<IFile>) GetDataDirectory();

    /**
     * Gets the Android external storage directory.  This directory may not
     * currently be accessible if it has been mounted by the user on their
     * computer, has been removed from the device, or some other problem has
     * happened.  You can determine its current state with
     * {@link #getExternalStorageState()}.
     *
     * <p><em>Note: don't be confused by the word "external" here.  This
     * directory can better be thought as media/shared storage.  It is a
     * filesystem that can hold a relatively large amount of data and that
     * is shared across all applications (does not enforce permissions).
     * Traditionally this is an SD card, but it may also be implemented as
     * built-in storage in a device that is distinct from the protected
     * internal storage and can be mounted as a filesystem on a computer.</em></p>
     *
     * <p>In devices with multiple "external" storage directories (such as
     * both secure app storage and mountable shared storage), this directory
     * represents the "primary" external storage that the user will interact
     * with.</p>
     *
     * <p>Applications should not directly use this top-level directory, in
     * order to avoid polluting the user's root namespace.  Any files that are
     * private to the application should be placed in a directory returned
     * by {@link android.content.Context#getExternalFilesDir
     * Context.getExternalFilesDir}, which the system will take care of deleting
     * if the application is uninstalled.  Other shared files should be placed
     * in one of the directories returned by
     * {@link #getExternalStoragePublicDirectory}.
     *
     * <p>Here is an example of typical code to monitor the state of
     * external storage:</p>
     *
     * {@sample development/samples/ApiDemos/src/com/example/android/apis/content/ExternalStorage.java
     * monitor_storage}
     *
     * @see #getExternalStorageState()
     * @see #isExternalStorageRemovable()
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageDirectory();

    /**
     * Get a top-level public external storage directory for placing files of
     * a particular type.  This is where the user will typically place and
     * manage their own files, so you should be careful about what you put here
     * to ensure you don't erase their files or get in the way of their own
     * organization.
     *
     * <p>Here is an example of typical code to manipulate a picture on
     * the public external storage:</p>
     *
     * {@sample development/samples/ApiDemos/src/com/example/android/apis/content/ExternalStorage.java
     * public_picture}
     *
     * @param type The type of storage directory to return.  Should be one of
     * {@link #DIRECTORY_MUSIC}, {@link #DIRECTORY_PODCASTS},
     * {@link #DIRECTORY_RINGTONES}, {@link #DIRECTORY_ALARMS},
     * {@link #DIRECTORY_NOTIFICATIONS}, {@link #DIRECTORY_PICTURES},
     * {@link #DIRECTORY_MOVIES}, {@link #DIRECTORY_DOWNLOADS}, or
     * {@link #DIRECTORY_DCIM}.  May not be null.
     *
     * @return Returns the File path for the directory.  Note that this
     * directory may not yet exist, so you must make sure it exists before
     * using it such as with {@link File#mkdirs File.mkdirs()}.
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStoragePublicDirectory(
        /* [in] */ const String& type);

    /**
     * Returns the path for android-specific data on the SD card.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageElastosDataDir();

    /**
     * Generates the raw path to an application's data
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppDataDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Generates the raw path to an application's media
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppMediaDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Generates the path to an application's files.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppFilesDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Generates the path to an application's cache.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppCacheDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Gets the Android Download/Cache content directory.
     */
    static CARAPI_(AutoPtr<IFile>) GetDownloadCacheDirectory();

     /**
     * Gets the current state of the primary "external" storage device.
     *
     * <p>See {@link #getExternalStorageDirectory()} for more information.
     */
    static CARAPI_(String) GetExternalStorageState();

    /**
     * Returns whether the primary "external" storage device is removable.
     * If true is returned, this device is for example an SD card that the
     * user can remove.  If false is returned, the storage is built into
     * the device and can not be physically removed.
     *
     * <p>See {@link #getExternalStorageDirectory()} for more information.
     */
    static CARAPI_(Boolean) IsExternalStorageRemovable();

private:
    static CARAPI_(AutoPtr<IFile>) GetDirectory(
        /* [in] */ const String& variableName,
        /* [in] */ const String& defaultPath);

    static CARAPI_(void) Init();

public:
    /**
     * Standard directory in which to place any audio files that should be
     * in the regular list of music for the user.
     * This may be combined with
     * {@link #DIRECTORY_PODCASTS}, {@link #DIRECTORY_NOTIFICATIONS},
     * {@link #DIRECTORY_ALARMS}, and {@link #DIRECTORY_RINGTONES} as a series
     * of directories to categories a particular audio file as more than one
     * type.
     */
    static const String DIRECTORY_MUSIC;

    /**
     * Standard directory in which to place any audio files that should be
     * in the list of podcasts that the user can select (not as regular
     * music).
     * This may be combined with {@link #DIRECTORY_MUSIC},
     * {@link #DIRECTORY_NOTIFICATIONS},
     * {@link #DIRECTORY_ALARMS}, and {@link #DIRECTORY_RINGTONES} as a series
     * of directories to categories a particular audio file as more than one
     * type.
     */
    static const String DIRECTORY_PODCASTS;

    /**
     * Standard directory in which to place any audio files that should be
     * in the list of ringtones that the user can select (not as regular
     * music).
     * This may be combined with {@link #DIRECTORY_MUSIC},
     * {@link #DIRECTORY_PODCASTS}, {@link #DIRECTORY_NOTIFICATIONS}, and
     * {@link #DIRECTORY_ALARMS} as a series
     * of directories to categories a particular audio file as more than one
     * type.
     */
    static const String DIRECTORY_RINGTONES;

    /**
     * Standard directory in which to place any audio files that should be
     * in the list of alarms that the user can select (not as regular
     * music).
     * This may be combined with {@link #DIRECTORY_MUSIC},
     * {@link #DIRECTORY_PODCASTS}, {@link #DIRECTORY_NOTIFICATIONS},
     * and {@link #DIRECTORY_RINGTONES} as a series
     * of directories to categories a particular audio file as more than one
     * type.
     */
    static const String DIRECTORY_ALARMS;

    /**
     * Standard directory in which to place any audio files that should be
     * in the list of notifications that the user can select (not as regular
     * music).
     * This may be combined with {@link #DIRECTORY_MUSIC},
     * {@link #DIRECTORY_PODCASTS},
     * {@link #DIRECTORY_ALARMS}, and {@link #DIRECTORY_RINGTONES} as a series
     * of directories to categories a particular audio file as more than one
     * type.
     */
    static const String DIRECTORY_NOTIFICATIONS;

    /**
     * Standard directory in which to place pictures that are available to
     * the user.  Note that this is primarily a convention for the top-level
     * public directory, as the media scanner will find and collect pictures
     * in any directory.
     */
    static const String DIRECTORY_PICTURES;

    /**
     * Standard directory in which to place movies that are available to
     * the user.  Note that this is primarily a convention for the top-level
     * public directory, as the media scanner will find and collect movies
     * in any directory.
     */
    static const String DIRECTORY_MOVIES;

    /**
     * Standard directory in which to place files that have been downloaded by
     * the user.  Note that this is primarily a convention for the top-level
     * public directory, you are free to download files anywhere in your own
     * private directories.  Also note that though the constant here is
     * named DIRECTORY_DOWNLOADS (plural), the actual file name is non-plural for
     * backwards compatibility reasons.
     */
    static const String DIRECTORY_DOWNLOADS;

    /**
     * The traditional location for pictures and videos when mounting the
     * device as a camera.  Note that this is primarily a convention for the
     * top-level public directory, as this convention makes no sense elsewhere.
     */
    static const String DIRECTORY_DCIM;

    /**
     * getExternalStorageState() returns MEDIA_REMOVED if the media is not present.
     */
    static const String MEDIA_REMOVED;

    /**
     * getExternalStorageState() returns MEDIA_UNMOUNTED if the media is present
     * but not mounted.
     */
    static const String MEDIA_UNMOUNTED;

    /**
     * getExternalStorageState() returns MEDIA_CHECKING if the media is present
     * and being disk-checked
     */
    static const String MEDIA_CHECKING;

    /**
     * getExternalStorageState() returns MEDIA_NOFS if the media is present
     * but is blank or is using an unsupported filesystem
     */
    static const String MEDIA_NOFS;

    /**
     * getExternalStorageState() returns MEDIA_MOUNTED if the media is present
     * and mounted at its mount point with read/write access.
     */
    static const String MEDIA_MOUNTED;

    /**
     * getExternalStorageState() returns MEDIA_MOUNTED_READ_ONLY if the media is present
     * and mounted at its mount point with read only access.
     */
    static const String MEDIA_MOUNTED_READ_ONLY;

    /**
     * getExternalStorageState() returns MEDIA_SHARED if the media is present
     * not mounted, and shared via USB mass storage.
     */
    static const String MEDIA_SHARED;

    /**
     * getExternalStorageState() returns MEDIA_BAD_REMOVAL if the media was
     * removed before it was unmounted.
     */
    static const String MEDIA_BAD_REMOVAL;

    /**
     * getExternalStorageState() returns MEDIA_UNMOUNTABLE if the media is present
     * but cannot be mounted.  Typically this happens if the file system on the
     * media is corrupted.
     */
    static const String MEDIA_UNMOUNTABLE;


private:
    static AutoPtr<IFile> ROOT_DIRECTORY;

    static const CString SYSTEM_PROPERTY_EFS_ENABLED;

    static AutoPtr<IMountService> mMntSvc;

    static AutoPtr<IFile> DATA_DIRECTORY;

    /**
     * @hide
     */
    static AutoPtr<IFile> SECURE_DATA_DIRECTORY;

    static AutoPtr<IFile> EXTERNAL_STORAGE_DIRECTORY;

    static AutoPtr<IFile> EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY;

    static AutoPtr<IFile> EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY;

    static AutoPtr<IFile> DOWNLOAD_CACHE_DIRECTORY;

    static Boolean sIsInitilized;
};

#endif // __HH_ENVIRONMENT_H
