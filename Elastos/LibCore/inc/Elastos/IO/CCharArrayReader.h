#ifndef __CCHARARRAYREADER_H__
#define __CCHARARRAYREADER_H__

#include "_CCharArrayReader.h"
#include "CharArrayReader.h"

CarClass(CCharArrayReader), public CharArrayReader
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
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Char8>* buffer,
        /* [out] */ Int32* number);

    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Char8>& buf);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Char8>& buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

private:
    CARAPI_(Mutex*) GetSelfLock();
};
#endif //__CCHARARRAYREADER_H__