
#ifndef __CCAPSULEINFOLITE_H__
#define __CCAPSULEINFOLITE_H__

#include "_CCapsuleInfoLite.h"

CarClass(CCapsuleInfoLite)
{
public:
    CCapsuleInfoLite();

    CARAPI ToString(
        /* [out] */ String* des);

    CARAPI DescribeContents(
        /* [out] */ Int32* ret);

    CARAPI GetCapsuleName(
        /* [out] */ String* name);

    CARAPI SetCapsuleName(
        /* [in] */ const String& name);

    CARAPI GetRecommendedInstallLocation(
        /* [out] */ Int32* location);

    CARAPI SetRecommendedInstallLocation(
        /* [in] */ Int32 location);

    CARAPI GetInstallLocation(
        /* [out] */ Int32* location);

    CARAPI SetInstallLocation(
        /* [in] */ Int32 location);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    /**
     * The name of this package.  From the &lt;manifest&gt; tag's "name"
     * attribute.
     */
    String mCapsuleName;

    /**
     * Specifies the recommended install location. Can be one of
     * {@link #PackageHelper.RECOMMEND_INSTALL_INTERNAL} to install on internal storage
     * {@link #PackageHelper.RECOMMEND_INSTALL_EXTERNAL} to install on external media
     * {@link PackageHelper.RECOMMEND_FAILED_INSUFFICIENT_STORAGE} for storage errors
     * {@link PackageHelper.RECOMMEND_FAILED_INVALID_APK} for parse errors.
     */
    Int32 mRecommendedInstallLocation;
    Int32 mInstallLocation;
};

#endif // __CCAPSULEINFOLITE_H__
