
#ifndef __CRETRYABLEOUTPUTSTREAM_H__
#define __CRETRYABLEOUTPUTSTREAM_H__

#include "_CRetryableOutputStream.h"
#include "Elastos.IO.h"
#include <elastos/AutoPtr.h>
#include "AbstractHttpOutputStream.h"
CarClass(CRetryableOutputStream), public AbstractHttpOutputStream
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

    CARAPI ContentLength(
        /* [out] */ Int32* pSize);

    CARAPI WriteToSocket(
        /* [in] */ IOutputStream* pSocketOut);
private:
    // TODO: Add your private member variables here.
    Int32 mLimit;
    AutoPtr<IByteArrayOutputStream> mContent;
};

#endif // __CRETRYABLEOUTPUTSTREAM_H__
