
#ifndef __CPUSHBACKREADER_H__
#define __CPUSHBACKREADER_H__

#include "_CPushbackReader.h"
#include "PushbackReader.h"

CarClass(CPushbackReader), public PushbackReader
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

    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI UnRead(
        /* [in] */ Int32 oneChar);

    CARAPI UnReadBuffer(
        /* [in] */ const ArrayOf<Char8>& buffer);

    CARAPI UnReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ const ArrayOf<Char8>& buffer);

    CARAPI constructor(
        /* [in] */ IReader* ir);

    CARAPI constructor(
        /* [in] */ IReader* ir,
        /* [in] */ Int32 size);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CPUSHBACKREADER_H__
