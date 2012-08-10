#include "cmdef.h"
#include "FilterReader.h"

FilterReader::FilterReader()
{
}

FilterReader::~FilterReader()
{
}

ECode FilterReader::Init(
    /* [in] */ IReader* in)
{
    Reader::Init(GetSelfLock());
    mIn = in;

    return NOERROR;
}

ECode FilterReader::Close()
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return CloseLocked();
}

ECode FilterReader::CloseLocked()
{
    assert(mLock != NULL);

    return mIn->Close();
}

ECode FilterReader::Mark(
    /* [in] */ Int32 readLimit)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return MarkLocked(readLimit);
}

ECode FilterReader::MarkLocked(
    /* [in] */ Int32 readLimit)
{
    assert(mLock != NULL);

    return mIn->Mark(readLimit);
}

ECode FilterReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return IsMarkSupportedLocked(supported);
}

ECode FilterReader::IsMarkSupportedLocked(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    assert(mLock != NULL);

    return mIn->IsMarkSupported(supported);
}

ECode FilterReader::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadLocked(value);
}

ECode FilterReader::ReadLocked(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);

    return mIn->Read(value);
}

ECode FilterReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadBufferExLocked(offset, count, buffer, number);
}

ECode FilterReader::ReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);

    return mIn->ReadBufferEx(offset, count, buffer, number);
}

ECode FilterReader::IsReady(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return IsReadyLocked(ready);
}

ECode FilterReader::IsReadyLocked(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    assert(mLock != NULL);

    return mIn->IsReady(ready);
}

ECode FilterReader::Reset()
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ResetLocked();
}

ECode FilterReader::ResetLocked()
{
    assert(mLock != NULL);

    return mIn->Reset();
}

ECode FilterReader:: Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return SkipLocked(count, number);
}

ECode FilterReader:: SkipLocked(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    assert(mLock != NULL);

    return mIn->Skip(count, number);
}