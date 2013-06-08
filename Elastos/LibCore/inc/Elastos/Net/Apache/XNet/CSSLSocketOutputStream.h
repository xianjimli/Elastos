
#ifndef __CSSLSOCKETOUTPUTSTREAM_H__
#define __CSSLSOCKETOUTPUTSTREAM_H__

#include "_CSSLSocketOutputStream.h"

CarClass(CSSLSocketOutputStream)
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI CheckError(
        /* [out] */ Boolean * pHasError);

    CARAPI constructor(
        /* [in] */ ISSLSocketImpl * pOwner);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLSOCKETOUTPUTSTREAM_H__
