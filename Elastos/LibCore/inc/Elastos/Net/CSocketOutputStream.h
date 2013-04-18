
#ifndef __CSOCKETOUTPUTSTREAM_H__
#define __CSOCKETOUTPUTSTREAM_H__

#include "_CSocketOutputStream.h"
#include "PlainSocketImpl.h"

CarClass(CSocketOutputStream)
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

    CARAPI constructor(
        /* [in] */ ISocketImpl * pImpl);

private:
    // TODO: Add your private member variables here.
    PlainSocketImpl* mSocket;
};

#endif // __CSOCKETOUTPUTSTREAM_H__
