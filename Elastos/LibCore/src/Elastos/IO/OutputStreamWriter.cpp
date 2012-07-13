#include "OutputStreamWriter.h"
#include "cmdef.h"

OutputStreamWriter::OutputStreamWriter()
{
}

OutputStreamWriter::~OutputStreamWriter()
{
    ArrayOf<Byte>::Free(mBuf);
}

ECode OutputStreamWriter::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::Flush()
{
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8> &buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::GetEncoding(
    /* [out] */ String *encoding)
{
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::Init(
    /* [in] */ IOutputStream *os)
{
    mBuf = ArrayOf<Byte>::Alloc(8192);
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::Init(
    /* [in] */ IOutputStream *os,
    /* [in] */ const String &enc)
{
    mBuf = ArrayOf<Byte>::Alloc(8192);
    return E_NOT_IMPLEMENTED;
}



