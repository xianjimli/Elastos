
#ifndef __CPATHPERMISSION_H__
#define __CPATHPERMISSION_H__

#include "_CPathPermission.h"
#include "os/_PatternMatcher.h"

CarClass(CPathPermission), public _PatternMatcher
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor(
        /* [in] */ String pattern,
        /* [in] */ Int32 type,
        /* [in] */ String readPermission,
        /* [in] */ String writePermission);

    CARAPI constructor(
        /* [in] */ IParcel* src);

public:
    CARAPI_(String) GetReadPermission();

    CARAPI_(String) GetWritePermission();
};

#endif //__CPATHPERMISSION_H__
