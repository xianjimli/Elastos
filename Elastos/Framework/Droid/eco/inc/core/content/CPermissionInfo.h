
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

    CARAPI LoadDescription(
        /* [in] */ ILocalCapsuleManager* cm,
        /* [out] */ ICharSequence** des);

    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI GetGroup(
        /* [out] */ String* group);

    CARAPI SetGroup(
        /* [in] */ const String& group);

    CARAPI GetDescriptionRes(
        /* [out] */ Int32* desRes);

    CARAPI SetDescriptionRes(
        /* [in] */ Int32 desRes);

    CARAPI GetNonLocalizedDescription(
        /* [out] */ ICharSequence** des);

    CARAPI SetNonLocalizedDescription(
        /* [in] */ ICharSequence* des);

    CARAPI GetProtectionLevel(
        /* [out] */ Int32* level);

    CARAPI SetProtectionLevel(
        /* [in] */ Int32 level);

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
    AutoPtr<ICharSequence> mNonLocalizedDescription;

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
