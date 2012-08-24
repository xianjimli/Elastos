
#ifndef __CSTRINGREADER_H__
#define __CSTRINGREADER_H__

#include "_CStringReader.h"
#include "StringReader.h"

CarClass(CStringReader), public StringReader
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

    CARAPI constructor(
        /* [in] */ String str);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CSTRINGREADER_H__
