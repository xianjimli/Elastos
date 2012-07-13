
#ifndef __CFILTEROUTPUTSTREAM_H__
#define __CFILTEROUTPUTSTREAM_H__

#include "_CFilterOutputStream.h"
#include "FilterOutputStream.h"

CarClass(CFilterOutputStream), public FilterOutputStream
{
public:
    CFilterOutputStream();

    ~CFilterOutputStream();

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI constructor(
        /* [in] */ IOutputStream* out);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CFILTEROUTPUTSTREAM_H__
