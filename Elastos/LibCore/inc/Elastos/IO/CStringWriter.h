#ifndef __CSTRINGWRITER_H__
#define __CSTRINGWRITER_H__

#include "_CStringWriter.h"
#include "StringWriter.h"

CarClass(CStringWriter), public StringWriter
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Char8> &buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Char8> &buffer);

    CARAPI WriteString(
        /* [in] */ CString str);

    CARAPI WriteStringEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ CString str);

    CARAPI Append(
        /* [in] */ Char32 c);

    CARAPI AppendEx(
        /* [in] */ ICharSequence *csq);

    CARAPI AppendEx2(
        /* [in] */ ICharSequence *csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckError(
        /* [out] */ Boolean *hasError);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 initialSize);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CSTRINGWRITER_H__

