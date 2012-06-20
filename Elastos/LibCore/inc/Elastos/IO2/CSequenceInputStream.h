#ifndef __CSEQUENCEINPUTSTREAM_H__
#define __CSEQUENCEINPUTSTREAM_H__

#include "_CSequenceInputStream.h"
#include "SequenceInputStream.h"

using namespace Elastos;
using namespace Elastos::System::Threading;

CarClass(CSequenceInputStream), public SequenceInputStream
{
public:
    CSequenceInputStream();

    virtual ~CSequenceInputStream();

    CARAPI constructor(
        /* [in] */ IInputStream* s1,
        /* [in] */ IInputStream* s2);

    CARAPI constructor(
        /* [in] */ IObjectEnumerator* e);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte>* buffer,
        /* [out] */ Int32* number);
//    CARAPI Read(
//        /* [out] */ byte[] buffer,
//        /* [in] */ Int32 offset,
//        /* [in] */ Int32 count,
//        /* [out] */ Int32 results);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI Reset();

private:
    CARAPI_(Mutex*) GetSelfLock();
};


#endif //__CSEQUENCEINPUTSTREAM_H__