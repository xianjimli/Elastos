
#ifndef __CBUFFEREDREADER_H__
#define __CBUFFEREDREADER_H__

#include "_CBufferedReader.h"
#include "BufferedReader.h"

CarClass(CBufferedReader), public BufferedReader
{
public:
    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Char8>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ ArrayOf<Char8>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadLine(
        /* [out] */ String* contents);

    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI constructor(
        /* [in] */ IReader* rin);

    CARAPI constructor(
        /* [in] */ IReader* rin,
        /* [in] */ Int32 size);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CBUFFEREDREADER_H__
