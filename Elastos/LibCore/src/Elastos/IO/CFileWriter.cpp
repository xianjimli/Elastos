
#include "CFileWriter.h"
#include "CFileOutputStream.h"
#include "cmdef.h"

ECode CFileWriter::constructor(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(file, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Boolean append)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(file, append, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(fd, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ const String& filename)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(filename, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ const String& filename,
    /* [in] */ Boolean append)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(filename, append, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::Close()
{
    return FileWriter::Close();
}

ECode CFileWriter::Flush()
{
    return FileWriter::Flush();
}

ECode CFileWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return FileWriter::Write(oneChar32);
}

ECode CFileWriter::WriteBuffer(
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return FileWriter::WriteBuffer(buffer);
}

ECode CFileWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return FileWriter::WriteBufferEx(offset, count, buffer);
}

ECode CFileWriter::WriteString(
    /* [in] */ CString str)
{
    return FileWriter::WriteString(str);
}

ECode CFileWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    return FileWriter::WriteStringEx(offset, count, str);
}

ECode CFileWriter::Append(
    /* [in] */ Char32 c)
{
    return FileWriter::Append(c);
}

ECode CFileWriter::AppendEx(
    /* [in] */ ICharSequence* csq)
{
    return FileWriter::AppendEx(csq);
}

ECode CFileWriter::AppendEx2(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return FileWriter::AppendEx2(csq, start, end);
}

ECode CFileWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    return FileWriter::CheckError(hasError);
}

ECode CFileWriter::GetEncoding(
    /* [out] */ String* encoding)
{
    return FileWriter::GetEncoding(encoding);
}

Mutex* CFileWriter::GetSelfLock()
{
    return &_m_syncLock;
}
