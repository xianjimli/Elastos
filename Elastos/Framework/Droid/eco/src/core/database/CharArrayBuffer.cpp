
#include "database/CharArrayBuffer.h"

CharArrayBuffer::CharArrayBuffer()
    : mData(NULL)
    , mSizeCopied(0)
{}

CharArrayBuffer::~CharArrayBuffer()
{
    if (mData != NULL) {
        ArrayOf<Char32>::Free(mData);
    }
}

ECode CharArrayBuffer::Init(
    /* [in] */ Int32 size)
{
    mData = ArrayOf<Char32>::Alloc(size);
    return NOERROR;
}

ECode CharArrayBuffer::Init(
    /* [in] */ ArrayOf<Char32>* buf)
{
    mData = buf;
    return NOERROR;
}

ECode CharArrayBuffer::GetData(
    /* [out,callee] */ ArrayOf<Char32>** data)
{
    *data = mData->Clone();
    return NOERROR;
}

ECode CharArrayBuffer::SetData(
    /* [in] */ ArrayOf<Char32>* data)
{
    if (mData != NULL) {
        ArrayOf<Char32>::Free(mData);
    }
    mData = data;
    return NOERROR;
}
