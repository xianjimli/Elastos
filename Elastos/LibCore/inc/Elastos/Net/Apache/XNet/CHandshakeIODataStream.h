
#ifndef __CHANDSHAKEIODATASTREAM_H__
#define __CHANDSHAKEIODATASTREAM_H__

#include "_CHandshakeIODataStream.h"

CarClass(CHandshakeIODataStream)
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

    CARAPI Append(
        /* [in] */ const ArrayOf<Byte> & src);

    CARAPI HasData(
        /* [out] */ Boolean * pHasData);

    CARAPI GetData(
        /* [in] */ Int32 length,
        /* [out, callee] */ ArrayOf<Byte> ** ppArray);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CHANDSHAKEIODATASTREAM_H__
