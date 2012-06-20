
#ifndef __CASSETFILEDESCRIPTOR_H__
#define __CASSETFILEDESCRIPTOR_H__

#include "_CAssetFileDescriptor.h"

CarClass(CAssetFileDescriptor)
{
public:
    CARAPI constructor(
        /* [in] */ IParcelFileDescriptor* fd,
        /* [in] */ Int64 startOffset,
        /* [in] */ Int64 length);

    CARAPI constructor(
        /* [in] */ IParcel* src);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    // TODO: Add your private member variables here.
};

#endif // __CASSETFILEDESCRIPTOR_H__
