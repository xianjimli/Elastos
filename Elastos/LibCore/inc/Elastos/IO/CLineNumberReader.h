
#ifndef __CLINENUMBERREADER_H__
#define __CLINENUMBERREADER_H__

#include "_CLineNumberReader.h"
#include "LineNumberReader.h"

CarClass(CLineNumberReader), public LineNumberReader
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
        /* [out] */ ArrayOf<Char8>* puffer,
        /* [out] */ Int32* number);

    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI ReadLine(
        /* [out] */ String * contents);

    CARAPI GetLineNumber(
        /* [out] */ Int32* lineNumber);

    CARAPI SetLineNumber(
        /* [in] */ Int32 lineNumber);

    CARAPI constructor(
        /* [in] */ IReader* ir);

    CARAPI constructor(
        /* [in] */ IReader* ir,
        /* [in] */ Int32 size);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CLINENUMBERREADER_H__
