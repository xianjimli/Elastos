
#ifndef __CPATHPERMISSION_H__
#define __CPATHPERMISSION_H__

#include "_CPathPermission.h"
#include "os/PatternMatcher.h"

CarClass(CPathPermission), public PatternMatcher
{
public:
    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetType(
        /* [out] */ Int32* path);

    CARAPI Match(
        /* [in] */ const String& str,
        /* [out] */ Boolean* isMatch);

    CARAPI GetDescription(
    /* [out] */ String* description);

    CARAPI DescribeContents(
        /* [out] */ Int32* count);

    CARAPI GetReadPermission(
        /* [out] */ String* permission);

    CARAPI GetWritePermission(
        /* [out] */ String* permission);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ Int32 type,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission);

    CARAPI constructor(
        /* [in] */ IParcel* src);

private:
    String mReadPermission;
    String mWritePermission;
};

#endif //__CPATHPERMISSION_H__
