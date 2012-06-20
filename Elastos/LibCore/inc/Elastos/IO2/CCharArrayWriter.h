
#ifndef __CCHARARRAYWRITER_H__
#define __CCHARARRAYWRITER_H__

#include "_CCharArrayWriter.h"
#include "CharArrayWriter.h"

CarClass(CCharArrayWriter), public CharArrayWriter
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

    CARAPI Reset();

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI ToCharArray(
        /* [out, callee] */ BufferOf<Byte>** buffer);

    CARAPI WriteTo(
        /* [in] */ IWriter* ot);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 initialSize);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CCHARARRAYWRITER_H__
