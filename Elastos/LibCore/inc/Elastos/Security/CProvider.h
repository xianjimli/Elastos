
#ifndef __CPROVIDER_H__
#define __CPROVIDER_H__

#include "_CProvider.h"

CarClass(CProvider)
{
public:
    CProvider();

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetVersion(
        /* [out] */ Double* versionNumber);

    CARAPI GetInfo(
        /* [out] */ String* info);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& pversion,
        /* [in] */ const String& info);

private:

};

#endif //__CPROVIDER_H__
