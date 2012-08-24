
#ifndef __CPERMISSIONGROUPINFO_H__
#define __CPERMISSIONGROUPINFO_H__

#include "_CPermissionGroupInfo.h"
#include "content/CapsuleItemInfo.h"

CarClass(CPermissionGroupInfo), public CapsuleItemInfo
{
public:
    CPermissionGroupInfo()
        : mDescriptionRes(0) {}

    ~CPermissionGroupInfo() {}

public:
    CARAPI LoadDescription(
        /* [in] */ ICapsuleManager* cm,
        /* [out] */ ICharSequence** des);

    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPermissionGroupInfo* orig);

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
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
    AutoPtr<ICharSequence> mNonLocalizedDescription;
};

#endif // __CPERMISSIONGROUPINFO_H__
