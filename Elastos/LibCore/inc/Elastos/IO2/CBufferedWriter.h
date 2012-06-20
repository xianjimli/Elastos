
#ifndef __CBUFFEREDWRITER_H__
#define __CBUFFEREDWRITER_H__

#include "_CBufferedWriter.h"
#include "BufferedWriter.h"

CarClass(CBufferedWriter),public BufferedWriter
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteBuffer(
        /* [in] */ const BufferOf<Byte>& buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const BufferOf<Byte>& buffer);

    CARAPI WriteString(
        /* [in] */ String str);

    CARAPI WriteStringEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ String str);

    CARAPI Append(
        /* [in] */ Char32 c);

    CARAPI AppendEx(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendEx2(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI NewLine();

    CARAPI constructor(
        /* [in] */ IWriter* wout);

    CARAPI constructor(
        /* [in] */ IWriter* wout,
        /* [in] */ Int32 size);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CBUFFEREDWRITER_H__
