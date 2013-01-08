
#ifndef __CFILEWRITER_H__
#define __CFILEWRITER_H__

#include "_CFileWriter.h"
#include "FileWriter.h"

CarClass(CFileWriter), public FileWriter
{
public:
    CARAPI constructor(
        /* [in] */ IFile* file);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ Boolean append);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI constructor(
        /* [in] */ const String& filename);

    CARAPI constructor(
        /* [in] */ const String& filename,
        /* [in] */ Boolean append);

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Char8>& buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Char8>& buffer);

    CARAPI WriteString(
        /* [in] */ CString str);

    CARAPI WriteStringEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ CString str);

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

    CARAPI GetEncoding(
        /* [out] */ String* encoding);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif //__CFILEWRITER_H__
