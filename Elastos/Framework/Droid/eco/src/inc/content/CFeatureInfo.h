
#ifndef __CFEATUREINFO_H__
#define __CFEATUREINFO_H__

#include "_CFeatureInfo.h"

CarClass(CFeatureInfo)
{
public:
    CFeatureInfo()
        : mReqGlEsVersion(0)
        , mFlags(0) {}

    ~CFeatureInfo() {}

public:
    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI GetGlEsVersion(
        /* [out] */ String* ver);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IFeatureInfo* orig);

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    /**
     * The name of this feature, for example "android.hardware.camera".  If
     * this is null, then this is an OpenGL ES version feature as described
     * in {@link #reqGlEsVersion}.
     */
    String mName;

    /**
     * The GLES version used by an application. The upper order 16 bits represent the
     * major version and the lower order 16 bits the minor version.  Only valid
     * if {@link #name} is null.
     */
    Int32 mReqGlEsVersion;

    /**
     * Additional flags.  May be zero or more of {@link #FLAG_REQUIRED}.
     */
    Int32 mFlags;
};

#endif // __CFEATUREINFO_H__
