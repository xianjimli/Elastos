
#ifndef __CIMAGEFORMAT_H__
#define __CIMAGEFORMAT_H__

#include "_CImageFormat.h"

CarClass(CImageFormat)
{
public:
    CARAPI GetBitsPerPixel(
        /* [in] */ Int32 format,
        /* [out] */ Int32* bits);
};

#endif // __CIMAGEFORMAT_H__
