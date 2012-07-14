
#ifndef __CSTATUSBARICONLIST_H__
#define __CSTATUSBARICONLIST_H__

#include "_CStatusBarIconList.h"

using namespace Elastos;

CarClass(CStatusBarIconList)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSTATUSBARICONLIST_H__

