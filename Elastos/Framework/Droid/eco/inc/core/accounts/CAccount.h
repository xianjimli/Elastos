
#ifndef __CACCOUNT_H__
#define __CACCOUNT_H__

#include "_CAccount.h"

CarClass(CAccount)
{
public:
    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetType(
        /* [out] */ String* type);

    CARAPI SetType(
        /* [in] */ const String& type);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& type);

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    String mName;
    String mType;
};

#endif //__CACCOUNT_H__
