
#ifndef __CCONTENTPROVIDERINFO_H__
#define __CCONTENTPROVIDERINFO_H__

#include "_CContentProviderInfo.h"

#include "ext/frameworkdef.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include "capsule/ComponentInfo.h"
#include "content/CPathPermission.h"
#include "os/CPatternMatcher.h"

CarClass(CContentProviderInfo), public ComponentInfo
{
public:
    CContentProviderInfo();

    ~CContentProviderInfo();

    CARAPI constructor();

    CARAPI constructor(
            /* [in] */ IContentProviderInfo* orig);

    CARAPI LoadLabel(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ ICharSequence** label);

    CARAPI LoadIcon(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadLogo(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadXmlMetaData(
        /* [in] */ ICapsuleManager* pm,
        /* [in] */ const String& name,
        /* [out] */ IXmlResourceParser** resource);

    CARAPI GetComponentName(
        /* [out] */ String * pCapsuleName,
        /* [out] */ String * pClassName);

    CARAPI SetComponentName(
        /* [in] */ const String& capsuleName,
        /* [in] */ const String& className);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI SetApplicationInfo(
        /* [in] */ IApplicationInfo* info);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    /** The name provider is published under content:// */
    String mAuthority;

    /** Optional permission required for read-only access this content
     * provider. */
    String mReadPermission;

    /** Optional permission required for read/write access this content
     * provider. */
    String mWritePermission;

    /** If true, additional permissions to specific Uris in this content
     * provider can be granted, as per the
     * {@link android.R.styleable#AndroidManifestProvider_grantUriPermissions
     * grantUriPermissions} attribute.
     */
    Boolean mGrantUriPermissions;

    /**
     * If non-null, these are the patterns that are allowed for granting URI
     * permissions.  Any URI that does not match one of these patterns will not
     * allowed to be granted.  If null, all URIs are allowed.  The
     * {@link PackageManager#GET_URI_PERMISSION_PATTERNS
     * PackageManager.GET_URI_PERMISSION_PATTERNS} flag must be specified for
     * this field to be filled in.
     */
    List<AutoPtr<CPatternMatcher> >* mUriPermissionPatterns;

    /**
     * If non-null, these are path-specific permissions that are allowed for
     * accessing the provider.  Any permissions listed here will allow a
     * holding client to access the provider, and the provider will check
     * the URI it provides when making calls against the patterns here.
     */
    List<AutoPtr<CPathPermission> >* mPathPermissions;

    /** If true, this content provider allows multiple instances of itself
     *  to run in different process.  If false, a single instances is always
     *  run in {@link #processName}. */
    Boolean mMultiprocess;

    /** Used to control initialization order of single-process providers
     *  running in the same process.  Higher goes first. */
    Int32 mInitOrder;

    /**
     * Whether or not this provider is syncable.
     * The current way to make a provider
     * syncable is to provide a SyncAdapter service for a given provider/account type.
     */
    Boolean mIsSyncable;
};

#endif // __CCONTENTPROVIDERINFO_H__
