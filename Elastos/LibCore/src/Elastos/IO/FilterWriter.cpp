#include "cmdef.h"
#include "FilterWriter.h"

FilterWriter::FilterWriter()
{
}

FilterWriter::~FilterWriter()
{
}

ECode FilterWriter::Init(
    /* [in] */ IWriter* out)
{
    Writer::Init(GetSelfLock());
    mOut = out;

    return NOERROR;
}

ECode FilterWriter::Close()
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return CloseLocked();
}

ECode FilterWriter::CloseLocked()
{
    assert(mLock != NULL);

    return mOut->Close();
}

ECode FilterWriter::Flush()
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return FlushLocked();
}

ECode FilterWriter::FlushLocked()
{
    assert(mLock != NULL);

    return mOut->Flush();
}

ECode FilterWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return WriteBufferExLocked(offset, count, buffer);
}

ECode FilterWriter::WriteBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    assert(mLock != NULL);

    return mOut->WriteBufferEx(offset, count, buffer);
}

ECode FilterWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return WriteLocked(oneChar32);
}

ECode FilterWriter::WriteLocked(
    /* [in] */ Int32 oneChar32)
{
    assert(mLock != NULL);

    return mOut->Write(oneChar32);
}

ECode FilterWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return WriteStringExLocked(offset, count, str);
}

ECode FilterWriter::WriteStringExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    assert(mLock != NULL);

    return mOut->WriteStringEx(offset, count, str);
}