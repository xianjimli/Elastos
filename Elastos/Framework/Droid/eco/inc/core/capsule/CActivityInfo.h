
#ifndef __CACTIVITYINFO_H__
#define __CACTIVITYINFO_H__

#include "_CActivityInfo.h"
#include "capsule/ComponentInfo.h"

CarClass(CActivityInfo), public ComponentInfo
{
public:
    CActivityInfo();

    ~CActivityInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IActivityInfo* orig);

    CARAPI LoadLabel(
        /* [in] */ ILocalCapsuleManager* pm,
        /* [out] */ ICharSequence** label);

    CARAPI LoadIcon(
        /* [in] */ ILocalCapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadLogo(
        /* [in] */ ILocalCapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadXmlMetaData(
        /* [in] */ ILocalCapsuleManager* pm,
        /* [in] */ const String& name,
        /* [out] */ IXmlResourceParser** resource);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetCapsuleName(
        /* [out] */ String* name);

    CARAPI SetCapsuleName(
        /* [in] */ const String& name);

    CARAPI GetLabelRes(
        /* [out] */ Int32* labelRes);

    CARAPI SetLabelRes(
        /* [in] */ Int32 labelRes);

    CARAPI GetNonLocalizedLabel(
        /* [out] */ ICharSequence** label);

    CARAPI SetNonLocalizedLabel(
        /* [in] */ ICharSequence* label);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetLogo(
        /* [out] */ Int32* logo);

    CARAPI SetLogo(
        /* [in] */ Int32 logo);

    CARAPI GetMetaData(
        /* [out] */ IBundle** metaData);

    CARAPI SetMetaData(
        /* [in] */ IBundle* metaData);

    CARAPI GetComponentName(
        /* [out] */ String *capsuleName,
        /* [out] */ String *name);

    CARAPI SetComponentName(
        /* [in] */ const String& capsuleName,
        /* [in] */ const String& name);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI SetApplicationInfo(
        /* [in] */ IApplicationInfo* info);

    CARAPI GetProcessName(
        /* [out] */ String* processName);

    CARAPI SetProcessName(
        /* [in] */ const String& processName);

    CARAPI GetDescriptionRes(
        /* [out] */ Int32* desRes);

    CARAPI SetDescriptionRes(
        /* [in] */ Int32 desRes);

    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsExported(
        /* [out] */ Boolean* exported);

    CARAPI SetExported(
        /* [in] */ Boolean exported);

    CARAPI GetTheme(
        /* [out] */ Int32* theme);

    CARAPI SetTheme(
        /* [in] */ Int32 theme);

    CARAPI GetLaunchMode(
        /* [out] */ Int32 *mode);

    CARAPI SetLaunchMode(
        /* [in] */ Int32 mode);

    CARAPI GetPermission(
        /* [out] */ String* permission);

    CARAPI SetPermission(
        /* [in] */ const String& permission);

    CARAPI GetTaskAffinity(
        /* [out] */ String* task);

    CARAPI SetTaskAffinity(
        /* [in] */ const String& task);

    CARAPI GetTargetActivity(
        /* [out] */ String* target);

    CARAPI SetTargetActivity(
        /* [in] */ const String& target);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI GetScreenOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetScreenOrientation(
        /* [in] */ Int32 orientation);

    CARAPI GetConfigChanges(
        /* [out] */ Int32* changes);

    CARAPI GetSoftInputMode(
        /* [out] */ Int32* mode);

    CARAPI SetSoftInputMode(
        /* [in] */ Int32 mode);

    CARAPI SetConfigChanges(
        /* [in] */ Int32 changes);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetIconResource(
        /* [out] */ Int32* icon);

    /**
    * Return the theme resource identifier to use for this activity.  If
    * the activity defines a theme, that is used; else, the application
    * theme is used.
    *
    * @return The theme associated with this activity.
    */
    CARAPI GetThemeResource(
        /* [out] */ Int32* theme);

public:
    /**
     * A style resource identifier (in the package's resources) of this
     * activity's theme.  From the "theme" attribute or, if not set, 0.
     */
    Int32 mTheme;

    /**
     * The launch mode style requested by the activity.  From the
     * {@link android.R.attr#launchMode} attribute, one of
     * {@link #LAUNCH_MULTIPLE},
     * {@link #LAUNCH_SINGLE_TOP}, {@link #LAUNCH_SINGLE_TASK}, or
     * {@link #LAUNCH_SINGLE_INSTANCE}.
     */
    Int32 mLaunchMode;

    /**
     * Optional name of a permission required to be able to access this
     * Activity.  From the "permission" attribute.
     */
    String mPermission;

    /**
     * The affinity this activity has for another task in the system.  The
     * string here is the name of the task, often the package name of the
     * overall package.  If null, the activity has no affinity.  Set from the
     * {@link android.R.attr#taskAffinity} attribute.
     */
    String mTaskAffinity;

    /**
     * If this is an activity alias, this is the real activity class to run
     * for it.  Otherwise, this is null.
     */
    String mTargetActivity;

    /**
     * Options that have been set in the activity declaration in the
     * manifest: {@link #FLAG_MULTIPROCESS},
     * {@link #FLAG_FINISH_ON_TASK_LAUNCH}, {@link #FLAG_CLEAR_TASK_ON_LAUNCH},
     * {@link #FLAG_ALWAYS_RETAIN_TASK_STATE},
     * {@link #FLAG_STATE_NOT_NEEDED}, {@link #FLAG_EXCLUDE_FROM_RECENTS},
     * {@link #FLAG_ALLOW_TASK_REPARENTING}, {@link #FLAG_NO_HISTORY},
     * {@link #FLAG_FINISH_ON_CLOSE_SYSTEM_DIALOGS}.
     */
    Int32 mFlags;

    /**
     * The preferred screen orientation this activity would like to run in.
     * From the {@link android.R.attr#screenOrientation} attribute, one of
     * {@link #SCREEN_ORIENTATION_UNSPECIFIED},
     * {@link #SCREEN_ORIENTATION_LANDSCAPE},
     * {@link #SCREEN_ORIENTATION_PORTRAIT},
     * {@link #SCREEN_ORIENTATION_USER},
     * {@link #SCREEN_ORIENTATION_BEHIND},
     * {@link #SCREEN_ORIENTATION_SENSOR},
     * {@link #SCREEN_ORIENTATION_NOSENSOR},
     * {@link #SCREEN_ORIENTATION_SENSOR_LANDSCAPE},
     * {@link #SCREEN_ORIENTATION_SENSOR_PORTRAIT},
     * {@link #SCREEN_ORIENTATION_REVERSE_LANDSCAPE},
     * {@link #SCREEN_ORIENTATION_REVERSE_PORTRAIT},
     * {@link #SCREEN_ORIENTATION_FULL_SENSOR}.
     */
    Int32 mScreenOrientation;

    /**
     * Bit mask of kinds of configuration changes that this activity
     * can handle itself (without being restarted by the system).
     * Contains any combination of {@link #CONFIG_FONT_SCALE},
     * {@link #CONFIG_MCC}, {@link #CONFIG_MNC},
     * {@link #CONFIG_LOCALE}, {@link #CONFIG_TOUCHSCREEN},
     * {@link #CONFIG_KEYBOARD}, {@link #CONFIG_NAVIGATION},
     * {@link #CONFIG_ORIENTATION}, and {@link #CONFIG_SCREEN_LAYOUT}.  Set from the
     * {@link android.R.attr#configChanges} attribute.
     */
    Int32 mConfigChanges;

    /**
     * The desired soft input mode for this activity's main window.
     * Set from the {@link android.R.attr#windowSoftInputMode} attribute
     * in the activity's manifest.  May be any of the same values allowed
     * for {@link android.view.WindowManager.LayoutParams#softInputMode
     * WindowManager.LayoutParams.softInputMode}.  If 0 (unspecified),
     * the mode from the theme will be used.
     */
    Int32 mSoftInputMode;

//	    public static final Parcelable.Creator<ActivityInfo> CREATOR
//	            = new Parcelable.Creator<ActivityInfo>() {
};

#endif // __CACTIVITYINFO_H__
