
#ifndef __CPARCELEDLISTSLICE_H__
#define __CPARCELEDLISTSLICE_H__

#include "_CParceledListSlice.h"
#include <elastos/AutoPtr.h>

CarClass(CParceledListSlice)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* p,
        /* [in] */ Int32 numItems,
        /* [in] */ Boolean lastSlice);

    CARAPI Append(
        /* [in] */ IParcelable* item,
        /* [out] */ Boolean* isFull);

    CARAPI PopulateList(
        /* [in, out] */ IObjectContainer* list,
        /* [in] */ const ClassID& clsid,
        /* [out] */ IInterface** obj);

    CARAPI SetLastSlice(
        /* [in] */ Boolean lastSlice);

    CARAPI IsLastSlice(
        /* [out] */ Boolean* isLast);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    /*
     * TODO get this number from somewhere else. For now set it to a quarter of
     * the 1MB limit.
     */
    static const Int32 MAX_IPC_SIZE = 256 * 1024;
    AutoPtr<IParcel> mParcel;
    Int32 mNumItems;
    Boolean mIsLastSlice;
};

#endif // __CPARCELEDLISTSLICE_H__

