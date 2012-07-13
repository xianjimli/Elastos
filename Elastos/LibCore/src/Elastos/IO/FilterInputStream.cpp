
#include "FilterInputStream.h"

FilterInputStream::FilterInputStream()
{
}

FilterInputStream::~FilterInputStream()
{
}

ECode FilterInputStream::Init(
    /* [in] */ IInputStream* in)
{
    mIn = in;
    return NOERROR;
}

ECode FilterInputStream::Available(
    /* [out] */ Int32* number)
{
    return mIn->Available(number);
}

ECode FilterInputStream::Close()
{
    return mIn->Close();
}

ECode FilterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return mIn->Mark(readLimit);
}

ECode FilterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return mIn->IsMarkSupported(supported);
}

ECode FilterInputStream::Read(
    /* [out] */ Int32* value)
{
    return mIn->Read(value);
}

ECode FilterInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode FilterInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return mIn->ReadBufferEx(offset, length, buffer, number);
}

ECode FilterInputStream::Reset()
{
    return mIn->Reset();
}

ECode FilterInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    return mIn->Skip(count, number);
}
