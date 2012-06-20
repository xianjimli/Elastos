
#ifndef __CINPUTSTREAMREADER_H__
#define __CINPUTSTREAMREADER_H__

#include "_CInputStreamReader.h"

CarClass(CInputStreamReader)
{
public:
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ String enc);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Byte* value);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* skipped);
};

#endif //__CINPUTSTREAMREADER_H__
