
#ifndef __CFILEDESCRIPTOR_H__
#define __CFILEDESCRIPTOR_H__

#include "_CFileDescriptor.h"

CarClass(CFileDescriptor)
{
public:
    CARAPI Sync();

    CARAPI Valid(
        /* [out] */ Boolean* isValid);

    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI GetDescriptor(
        /* [out] */ Int32* des);

    CARAPI SetDescriptor(
        /* [in] */ Int32 fd);

    CARAPI GetNative(
        /* [out] */ Int32* native);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CFILEDESCRIPTOR_H__
