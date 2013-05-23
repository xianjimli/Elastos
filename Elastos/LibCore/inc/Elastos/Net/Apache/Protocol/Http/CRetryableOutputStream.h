
#ifndef __CRETRYABLEOUTPUTSTREAM_H__
#define __CRETRYABLEOUTPUTSTREAM_H__

#include "_CRetryableOutputStream.h"

CarClass(CRetryableOutputStream)
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

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 limit);

private:
    // TODO: Add your private member variables here.
};

#endif // __CRETRYABLEOUTPUTSTREAM_H__
