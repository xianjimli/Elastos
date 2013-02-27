
#ifndef __CIMAGEFORMAT_H__
#define __CIMAGEFORMAT_H__

#include "_CImageFormat.h"

CarClass(CImageFormat)
{
public:
    CARAPI GetBitsPerPixel(
        /* [in] */ Int32 format,
        /* [out] */ Int32 * pBits);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CIMAGEFORMAT_H__
