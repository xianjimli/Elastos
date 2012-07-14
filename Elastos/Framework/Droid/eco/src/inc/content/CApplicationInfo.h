
#ifndef __CAPPLICATIONINFO_H__
#define __CAPPLICATIONINFO_H__

#include "_CApplicationInfo.h"
#include "content/CapsuleItemInfo.h"
#include <elastos/AutoFree.h>

CarClass(CApplicationInfo), public CapsuleItemInfo
{
public:
    /**
     * Value for {@link #flags}: if set, this application is installed in the
     * device's system image.
     */
    static const Int32 FLAG_SYSTEM = 1<<0;

    /**
     * Value for {@link #flags}: set to true if this application would like to
     * allow debugging of its
     * code, even when installed on a non-development system.  Comes
     * from {@link android.R.styleable#AndroidManifestApplication_debuggable
     * android:debuggable} of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_DEBUGGABLE = 1<<1;

    /**
     * Value for {@link #flags}: set to true if this application has code
     * associated with it.  Comes
     * from {@link android.R.styleable#AndroidManifestApplication_hasCode
     * android:hasCode} of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_HAS_CODE = 1<<2;

    /**
     * Value for {@link #flags}: set to true if this application is persistent.
     * Comes from {@link android.R.styleable#AndroidManifestApplication_persistent
     * android:persistent} of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_PERSISTENT = 1<<3;

    /**
     * Value for {@link #flags}: set to true if this application holds the
     * {@link android.Manifest.permission#FACTORY_TEST} permission and the
     * device is running in factory test mode.
     */
    static const Int32 FLAG_FACTORY_TEST = 1<<4;

    /**
     * Value for {@link #flags}: default value for the corresponding ActivityInfo flag.
     * Comes from {@link android.R.styleable#AndroidManifestApplication_allowTaskReparenting
     * android:allowTaskReparenting} of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_ALLOW_TASK_REPARENTING = 1<<5;

    /**
     * Value for {@link #flags}: default value for the corresponding ActivityInfo flag.
     * Comes from {@link android.R.styleable#AndroidManifestApplication_allowClearUserData
     * android:allowClearUserData} of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_ALLOW_CLEAR_USER_DATA = 1<<6;

    /**
     * Value for {@link #flags}: this is set if this application has been
     * install as an update to a built-in system application.
     */
    static const Int32 FLAG_UPDATED_SYSTEM_APP = 1<<7;

    /**
     * Value for {@link #flags}: this is set of the application has specified
     * {@link android.R.styleable#AndroidManifestApplication_testOnly
     * android:testOnly} to be true.
     */
    static const Int32 FLAG_TEST_ONLY = 1<<8;

    /**
     * Value for {@link #flags}: true when the application's window can be
     * reduced in size for smaller screens.  Corresponds to
     * {@link android.R.styleable#AndroidManifestSupportsScreens_smallScreens
     * android:smallScreens}.
     */
    static const Int32 FLAG_SUPPORTS_SMALL_SCREENS = 1<<9;

    /**
     * Value for {@link #flags}: true when the application's window can be
     * displayed on normal screens.  Corresponds to
     * {@link android.R.styleable#AndroidManifestSupportsScreens_normalScreens
     * android:normalScreens}.
     */
    static const Int32 FLAG_SUPPORTS_NORMAL_SCREENS = 1<<10;

    /**
     * Value for {@link #flags}: true when the application's window can be
     * increased in size for larger screens.  Corresponds to
     * {@link android.R.styleable#AndroidManifestSupportsScreens_largeScreens
     * android:largeScreens}.
     */
    static const Int32 FLAG_SUPPORTS_LARGE_SCREENS = 1<<11;

    /**
     * Value for {@link #flags}: true when the application knows how to adjust
     * its UI for different screen sizes.  Corresponds to
     * {@link android.R.styleable#AndroidManifestSupportsScreens_resizeable
     * android:resizeable}.
     */
    static const Int32 FLAG_RESIZEABLE_FOR_SCREENS = 1<<12;

    /**
     * Value for {@link #flags}: true when the application knows how to
     * accomodate different screen densities.  Corresponds to
     * {@link android.R.styleable#AndroidManifestSupportsScreens_anyDensity
     * android:anyDensity}.
     */
    static const Int32 FLAG_SUPPORTS_SCREEN_DENSITIES = 1<<13;

    /**
     * Value for {@link #flags}: set to true if this application would like to
     * request the VM to operate under the safe mode. Comes from
     * {@link android.R.styleable#AndroidManifestApplication_vmSafeMode
     * android:vmSafeMode} of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_VM_SAFE_MODE = 1<<14;

    /**
     * Value for {@link #flags}: set to <code>false</code> if the application does not wish
     * to permit any OS-driven backups of its data; <code>true</code> otherwise.
     *
     * <p>Comes from the
     * {@link android.R.styleable#AndroidManifestApplication_allowBackup android:allowBackup}
     * attribute of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_ALLOW_BACKUP = 1<<15;

    /**
     * Value for {@link #flags}: set to <code>false</code> if the application must be kept
     * in memory following a full-system restore operation; <code>true</code> otherwise.
     * Ordinarily, during a full system restore operation each application is shut down
     * following execution of its agent's onRestore() method.  Setting this attribute to
     * <code>false</code> prevents this.  Most applications will not need to set this attribute.
     *
     * <p>If
     * {@link android.R.styleable#AndroidManifestApplication_allowBackup android:allowBackup}
     * is set to <code>false</code> or no
     * {@link android.R.styleable#AndroidManifestApplication_backupAgent android:backupAgent}
     * is specified, this flag will be ignored.
     *
     * <p>Comes from the
     * {@link android.R.styleable#AndroidManifestApplication_killAfterRestore android:killAfterRestore}
     * attribute of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_KILL_AFTER_RESTORE = 1<<16;

    /**
     * Value for {@link #flags}: Set to <code>true</code> if the application's backup
     * agent claims to be able to handle restore data even "from the future,"
     * i.e. from versions of the application with a versionCode greater than
     * the one currently installed on the device.  <i>Use with caution!</i>  By default
     * this attribute is <code>false</code> and the Backup Manager will ensure that data
     * from "future" versions of the application are never supplied during a restore operation.
     *
     * <p>If
     * {@link android.R.styleable#AndroidManifestApplication_allowBackup android:allowBackup}
     * is set to <code>false</code> or no
     * {@link android.R.styleable#AndroidManifestApplication_backupAgent android:backupAgent}
     * is specified, this flag will be ignored.
     *
     * <p>Comes from the
     * {@link android.R.styleable#AndroidManifestApplication_restoreAnyVersion android:restoreAnyVersion}
     * attribute of the &lt;application&gt; tag.
     */
    static const Int32 FLAG_RESTORE_ANY_VERSION = 1<<17;

    /**
     * Value for {@link #flags}: Set to true if the application is
     * currently installed on external/removable/unprotected storage.  Such
     * applications may not be available if their storage is not currently
     * mounted.  When the storage it is on is not available, it will look like
     * the application has been uninstalled (its .apk is no longer available)
     * but its persistent data is not removed.
     */
    static const Int32 FLAG_EXTERNAL_STORAGE = 1<<18;

    /**
     * Value for {@link #flags}: true when the application's window can be
     * increased in size for extra large screens.  Corresponds to
     * {@link android.R.styleable#AndroidManifestSupportsScreens_xlargeScreens
     * android:xlargeScreens}.
     * @hide
     */
    static const Int32 FLAG_SUPPORTS_XLARGE_SCREENS = 1<<19;

    /**
     * Value for {@link #flags}: this is true if the application has set
     * its android:neverEncrypt to true, false otherwise. It is used to specify
     * that this package specifically "opts-out" of a secured file system solution,
     * and will always store its data in-the-clear.
     *
     * {@hide}
     */
    static const Int32 FLAG_NEVER_ENCRYPT = 1<<30;

    /**
     * Value for {@link #flags}: Set to true if the application has been
     * installed using the forward lock option.
     *
     * {@hide}
     */
    static const Int32 FLAG_FORWARD_LOCK = 1<<29;

    /**
     * Value for {@link #flags}: set to <code>true</code> if the application
     * has reported that it is heavy-weight, and thus can not participate in
     * the normal application lifecycle.
     *
     * <p>Comes from the
     * {@link android.R.styleable#AndroidManifestApplication_cantSaveState android:cantSaveState}
     * attribute of the &lt;application&gt; tag.
     *
     * {@hide}
     */
    static const Int32 FLAG_CANT_SAVE_STATE = 1<<27;

public:
    CApplicationInfo();

    ~CApplicationInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IApplicationInfo* orig);

    CARAPI GetSourceDir(
        /* [out] */ String* sourceDir);

    CARAPI SetSourceDir(
        /* [in] */ const String& sourceDir);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    /**
     * Disable compatibility mode
     *
     * @hide
     */
    CARAPI_(void) DisableCompatibilityMode();

public:
    /**
     * Default task affinity of all activities in this application. See
     * {@link ActivityInfo#taskAffinity} for more information.  This comes
     * from the "taskAffinity" attribute.
     */
    String mTaskAffinity;

    /**
     * Optional name of a permission required to be able to access this
     * application's components.  From the "permission" attribute.
     */
    String mPermission;

    /**
     * The name of the process this application should run in.  From the
     * "process" attribute or, if not set, the same as
     * <var>packageName</var>.
     */
    String mProcessName;

    /**
     * Class implementing the Application object.  From the "class"
     * attribute.
     */
    String mClassName;

    /**
     * A style resource identifier (in the package's resources) of the
     * description of an application.  From the "description" attribute
     * or, if not set, 0.
     */
    Int32 mDescriptionRes;

    /**
     * A style resource identifier (in the package's resources) of the
     * default visual theme of the application.  From the "theme" attribute
     * or, if not set, 0.
     */
    Int32 mTheme;

    /**
     * Class implementing the Application's manage space
     * functionality.  From the "manageSpaceActivity"
     * attribute. This is an optional attribute and will be null if
     * applications don't specify it in their manifest
     */
    String mManageSpaceActivityName;

    /**
     * Class implementing the Application's backup functionality.  From
     * the "backupAgent" attribute.  This is an optional attribute and
     * will be null if the application does not specify it in its manifest.
     *
     * <p>If android:allowBackup is set to false, this attribute is ignored.
     */
    String mBackupAgentName;

    /**
     * Flags associated with the application.  Any combination of
     * {@link #FLAG_SYSTEM}, {@link #FLAG_DEBUGGABLE}, {@link #FLAG_HAS_CODE},
     * {@link #FLAG_PERSISTENT}, {@link #FLAG_FACTORY_TEST}, and
     * {@link #FLAG_ALLOW_TASK_REPARENTING}
     * {@link #FLAG_ALLOW_CLEAR_USER_DATA}, {@link #FLAG_UPDATED_SYSTEM_APP},
     * {@link #FLAG_TEST_ONLY}, {@link #FLAG_SUPPORTS_SMALL_SCREENS},
     * {@link #FLAG_SUPPORTS_NORMAL_SCREENS},
     * {@link #FLAG_SUPPORTS_LARGE_SCREENS},
     * {@link #FLAG_RESIZEABLE_FOR_SCREENS},
     * {@link #FLAG_SUPPORTS_SCREEN_DENSITIES}, {@link #FLAG_VM_SAFE_MODE}
     */
    Int32 mFlags;

    /**
     * Full path to the location of this package.
     */
    String mSourceDir;

    /**
     * Full path to the location of the publicly available parts of this package (i.e. the resources
     * and manifest).  For non-forward-locked apps this will be the same as {@link #sourceDir).
     */
    String mPublicSourceDir;

    /**
     * Full paths to the locations of extra resource packages this application
     * uses. This field is only used if there are extra resource packages,
     * otherwise it is null.
     *
     * {@hide}
     */
    AutoFree<ArrayOf<String> > mResourceDirs;

    /**
     * Paths to all shared libraries this application is linked against.  This
     * field is only set if the {@link PackageManager#GET_SHARED_LIBRARY_FILES
     * PackageManager.GET_SHARED_LIBRARY_FILES} flag was used when retrieving
     * the structure.
     */
    AutoFree<ArrayOf<String> > mSharedLibraryFiles;

    /**
     * Full path to a directory assigned to the package for its persistent
     * data.
     */
    String mDataDir;

    /**
     * Full path to the directory where native JNI libraries are stored.
     */
    String mNativeLibraryDir;

    /**
     * The kernel user-ID that has been assigned to this application;
     * currently this is not a unique ID (multiple applications can have
     * the same uid).
     */
    Int32 mUid;

   /**
     * The minimum SDK version this application targets.  It may run on earlier
     * versions, but it knows how to work with any new behavior added at this
     * version.  Will be {@link android.os.Build.VERSION_CODES#CUR_DEVELOPMENT}
     * if this is a development build and the app is targeting that.  You should
     * compare that this number is >= the SDK version number at which your
     * behavior was introduced.
     */
    Int32 mTargetSdkVersion;

    /**
     * When false, indicates that all components within this application are
     * considered disabled, regardless of their individually set enabled status.
     */
    Boolean mEnabled;

    /**
     * For convenient access to package's install location.
     * @hide
     */
    Int32 mInstallLocation;
};

#endif // __CAPPLICATIONINFO_H__
