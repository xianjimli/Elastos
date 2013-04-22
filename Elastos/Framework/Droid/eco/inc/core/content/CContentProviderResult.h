
#ifndef __CCONTENTPROVIDERRESULT_H__
#define __CCONTENTPROVIDERRESULT_H__

#include "_CContentProviderResult.h"
#include <elastos/AutoPtr.h>

CarClass(CContentProviderResult)
{
public:
    CContentProviderResult();

    ~CContentProviderResult();

    CARAPI GetUri(
        /* [out] */ IUri** uri);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IUri* uri);

    CARAPI constructor(
        /* [in] */ Int32 count);

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    AutoPtr<IUri> mUri;
    Int32 mCount;
};

#endif // __CCONTENTPROVIDERRESULT_H__
