
#ifndef __CPERMISSIONINFO_H__
#define __CPERMISSIONINFO_H__

#include "_CPermissionInfo.h"
#include "content/CapsuleItemInfo.h"
#include <ext/frameworkext.h>
#include <elastos/AutoFree.h>
#include <elastos/AutoPtr.h>

/**
 * Information you can retrieve about a particular security permission
 * known to the system.  This corresponds to information collected from the
 * AndroidManifest.xml's &lt;permission&gt; tags.
 */
CarClass(CPermissionInfo), public CapsuleItemInfo
{
public:
    CPermissionInfo();

    ~CPermissionInfo();

    CARAPI LoadDescription(
        /* [in] */ ICapsuleManager* cm,
        /* [out, callee] */ ArrayOf<Char8>** des);

    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPermissionInfo* orig);

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    /**
     * The group this permission is a part of, as per
     * {@link android.R.attr#permissionGroup}.
     */
    String mGroup;

    /**
     * A string resource identifier (in the package's resources) of this
     * permission's description.  From the "description" attribute or,
     * if not set, 0.
     */
    Int32 mDescriptionRes;

    /**
     * The description string provided in the AndroidManifest file, if any.  You
     * probably don't want to use this, since it will be null if the description
     * is in a resource.  You probably want
     * {@link PermissionInfo#loadDescription} instead.
     */
    AutoFree<ArrayOf<Char8> > mNonLocalizedDescription;

    /**
     * The level of access this permission is protecting, as per
     * {@link android.R.attr#protectionLevel}.  Values may be
     * {@link #PROTECTION_NORMAL}, {@link #PROTECTION_DANGEROUS}, or
     * {@link #PROTECTION_SIGNATURE}.
     */
    Int32 mProtectionLevel;

//	    public static final Creator<PermissionInfo> CREATOR =
//	        new Creator<PermissionInfo>() {
};

#endif // __CPERMISSIONINFO_H__
