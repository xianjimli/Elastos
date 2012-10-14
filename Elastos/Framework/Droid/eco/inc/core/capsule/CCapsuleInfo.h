
#ifndef __CCAPSULEINFO_H__
#define __CCAPSULEINFO_H__

#include "_CCapsuleInfo.h"
#include "ext/frameworkdef.h"
#include "utils/AutoStringArray.h"
#include "content/Signature.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <elastos/List.h>

CarClass(CCapsuleInfo)
{
public:
    CCapsuleInfo()
        : mVersionCode(0)
        , mSharedUserLabel(0)
        , mFirstInstallTime(0)
        , mLastUpdateTime(0)
        , mInstallLocation(CapsuleInfo_INSTALL_LOCATION_INTERNAL_ONLY)
    {}

    ~CCapsuleInfo() {}

public:
    CARAPI GetCapsuleName(
        /* [out] */ String* name);

    CARAPI SetCapsuleName(
        /* [in] */ const String& name);

    CARAPI GetVersionCode(
        /* [out] */ Int32* versionCode);

    CARAPI SetVersionCode(
        /* [in] */ Int32 versionCode);

    CARAPI GetVersionName(
        /* [out] */ String* versionName);

    CARAPI SetVersionName(
        /* [in] */ const String& versionName);

    CARAPI GetSharedUserId(
        /* [out] */ String* id);

    CARAPI SetSharedUserId(
        /* [in] */ const String& id);

    CARAPI GetSharedUserLabel(
        /* [out] */ Int32* label);

    CARAPI SetSharedUserLabel(
        /* [in] */ Int32 label);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI SetApplicationInfo(
        /* [in] */ IApplicationInfo* info);

    CARAPI GetFirstInstallTime(
        /* [out] */ Int64* time);

    CARAPI SetFirstInstallTime(
        /* [in] */ Int64 time);

    CARAPI GetLastUpdateTime(
        /* [out] */ Int64* time);

    CARAPI SetLastUpdateTime(
        /* [in] */ Int64 time);

    CARAPI GetGids(
        /* [out, callee] */ ArrayOf<Int32>** gids);

    CARAPI SetGids(
        /* [in] */ ArrayOf<Int32>* gids);

    CARAPI AddActivity(
        /* [in] */ IActivityInfo* activity);

    CARAPI AddReceiver(
        /* [in] */ IActivityInfo* receiver);

    CARAPI AddService(
        /* [in] */ IServiceInfo* service);

    CARAPI AddContentProvider(
        /* [in] */ IContentProviderInfo* provider);

    CARAPI AddInstrumentation(
        /* [in] */ IInstrumentationInfo* instumentation);

    CARAPI AddPermission(
        /* [in] */ IPermissionInfo* permission);

    CARAPI AddRequestedPermission(
        /* [in] */ const String& permission);

    CARAPI AddSignature(
        /* [in] */ ISignature* signature);

    CARAPI AddConfigPreference(
        /* [in] */ IConfigurationInfo* config);

    CARAPI AddReqFeature(
        /* [in] */ IFeatureInfo* feature);

    CARAPI GetInstallLocation(
        /* [out] */ Int32* location);

    CARAPI SetInstallLocation(
        /* [in] */ Int32 location);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    /**
     * The name of this package.  From the &lt;manifest&gt; tag's "name"
     * attribute.
     */
    String mCapsuleName;

    /**
     * The version number of this package, as specified by the &lt;manifest&gt;
     * tag's {@link android.R.styleable#AndroidManifest_versionCode versionCode}
     * attribute.
     */
    Int32 mVersionCode;

    /**
     * The version name of this package, as specified by the &lt;manifest&gt;
     * tag's {@link android.R.styleable#AndroidManifest_versionName versionName}
     * attribute.
     */
    String mVersionName;

    /**
     * The shared user ID name of this package, as specified by the &lt;manifest&gt;
     * tag's {@link android.R.styleable#AndroidManifest_sharedUserId sharedUserId}
     * attribute.
     */
    String mSharedUserId;

    /**
     * The shared user ID label of this package, as specified by the &lt;manifest&gt;
     * tag's {@link android.R.styleable#AndroidManifest_sharedUserLabel sharedUserLabel}
     * attribute.
     */
    Int32 mSharedUserLabel;

    /**
     * Information collected from the &lt;application&gt; tag, or null if
     * there was none.
     */
    AutoPtr<IApplicationInfo> mApplicationInfo;

    /**
     * The time at which the app was first installed.  Units are as
     * per {@link System#currentTimeMillis()}.
     */
    Int64 mFirstInstallTime;

    /**
     * The time at which the app was last updated.  Units are as
     * per {@link System#currentTimeMillis()}.
     */
    Int64 mLastUpdateTime;

    /**
     * All kernel group-IDs that have been assigned to this package.
     * This is only filled in if the flag {@link PackageManager#GET_GIDS} was set.
     */
    AutoFree< ArrayOf<Int32> > mGids;

    /**
     * Array of all {@link android.R.styleable#AndroidManifestActivity
     * &lt;activity&gt;} tags included under &lt;application&gt;,
     * or null if there were none.  This is only filled in if the flag
     * {@link PackageManager#GET_ACTIVITIES} was set.
     */
    List< AutoPtr<IActivityInfo> > mActivities;

    /**
     * Array of all {@link android.R.styleable#AndroidManifestReceiver
     * &lt;receiver&gt;} tags included under &lt;application&gt;,
     * or null if there were none.  This is only filled in if the flag
     * {@link PackageManager#GET_RECEIVERS} was set.
     */
    List< AutoPtr<IActivityInfo> > mReceivers;

    /**
     * Array of all {@link android.R.styleable#AndroidManifestService
     * &lt;service&gt;} tags included under &lt;application&gt;,
     * or null if there were none.  This is only filled in if the flag
     * {@link PackageManager#GET_SERVICES} was set.
     */
    List< AutoPtr<IServiceInfo> > mServices;

    /**
     * Array of all {@link android.R.styleable#AndroidManifestProvider
     * &lt;provider&gt;} tags included under &lt;application&gt;,
     * or null if there were none.  This is only filled in if the flag
     * {@link PackageManager#GET_PROVIDERS} was set.
     */
    List< AutoPtr<IContentProviderInfo> > mContentProviders;

    /**
     * Array of all {@link android.R.styleable#AndroidManifestInstrumentation
     * &lt;instrumentation&gt;} tags included under &lt;manifest&gt;,
     * or null if there were none.  This is only filled in if the flag
     * {@link PackageManager#GET_INSTRUMENTATION} was set.
     */
    List< AutoPtr<IInstrumentationInfo> > mInstrumentations;

    /**
     * Array of all {@link android.R.styleable#AndroidManifestPermission
     * &lt;permission&gt;} tags included under &lt;manifest&gt;,
     * or null if there were none.  This is only filled in if the flag
     * {@link PackageManager#GET_PERMISSIONS} was set.
     */
    List< AutoPtr<IPermissionInfo> > mPermissions;

    /**
     * Array of all {@link android.R.styleable#AndroidManifestUsesPermission
     * &lt;uses-permission&gt;} tags included under &lt;manifest&gt;,
     * or null if there were none.  This is only filled in if the flag
     * {@link PackageManager#GET_PERMISSIONS} was set.  This list includes
     * all permissions requested, even those that were not granted or known
     * by the system at install time.
     */
    List<String> mRequestedPermissions;

    /**
     * Array of all signatures read from the package file.  This is only filled
     * in if the flag {@link PackageManager#GET_SIGNATURES} was set.
     */
    List< AutoPtr<ISignature> > mSignatures;

    /**
     * Application specified preferred configuration
     * {@link android.R.styleable#AndroidManifestUsesConfiguration
     * &lt;uses-configuration&gt;} tags included under &lt;manifest&gt;,
     * or null if there were none. This is only filled in if the flag
     * {@link PackageManager#GET_CONFIGURATIONS} was set.
     */
    List< AutoPtr<IConfigurationInfo> > mConfigPreferences;

    /**
     * The features that this application has said it requires.
     */
    List< AutoPtr<IFeatureInfo> > mReqFeatures;

    /**
     * The install location requested by the activity.  From the
     * {@link android.R.attr#installLocation} attribute, one of
     * {@link #INSTALL_LOCATION_AUTO},
     * {@link #INSTALL_LOCATION_INTERNAL_ONLY},
     * {@link #INSTALL_LOCATION_PREFER_EXTERNAL}
     * @hide
     */
    Int32 mInstallLocation;
};

#endif //__CCAPSULEINFO_H__
