
#ifndef __CPARCELFILEDESCRIPTOR_H__
#define __CPARCELFILEDESCRIPTOR_H__

#include "_CParcelFileDescriptor.h"

CarClass(CParcelFileDescriptor)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IParcelFileDescriptor* descriptor);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* descriptor);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPARCELFILEDESCRIPTOR_H__
