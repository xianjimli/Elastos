
#ifndef __COUTPUTSTREAMWRITER_H__
#define __COUTPUTSTREAMWRITER_H__

#include "_COutputStreamWriter.h"
#include "OutputStreamWriter.h"

CarClass(COutputStreamWriter), public OutputStreamWriter
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

    CARAPI GetEncoding(
        /* [out] */ String *encoding);

    CARAPI constructor(
        /* [in] */ IOutputStream *os);

    CARAPI constructor(
        /* [in] */ IOutputStream *os,
        /* [in] */ const String &enc);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __COUTPUTSTREAMWRITER_H__
