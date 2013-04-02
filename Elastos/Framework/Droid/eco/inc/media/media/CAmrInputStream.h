
#ifndef __CAMRINPUTSTREAM_H__
#define __CAMRINPUTSTREAM_H__

#include "_CAmrInputStream.h"
#include "media/AmrInputStream.h"

CarClass(CAmrInputStream), public AmrInputStream
{
public:
    CARAPI Available(
        /* [out] */ Int32 * pNumber);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);

    CARAPI Read(
        /* [out] */ Int32 * pValue);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte> * pBuffer,
        /* [out] */ Int32 * pNumber);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte> * pBuffer,
        /* [out] */ Int32 * pNumber);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64 * pNumber);

    CARAPI constructor(
        /* [in] */ IInputStream * pInputStream);

private:
    // TODO: Add your private member variables here.
};

#endif // __CAMRINPUTSTREAM_H__
