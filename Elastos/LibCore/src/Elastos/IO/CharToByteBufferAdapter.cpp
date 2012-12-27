
#include "cmdef.h"
#include "CharToByteBufferAdapter.h"

PInterface CharToByteBufferAdapter::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICharBuffer*)this;
    }
    else if (riid == EIID_ICharBuffer) {
        return (ICharBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_CharBuffer) {
        return reinterpret_cast<PInterface>((CharBuffer*)this);
    }

    return NULL;
}

UInt32 CharToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CharToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode CharToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CharToByteBufferAdapter::Wrap(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> buf;
    byteBuffer->Slice((IByteBuffer**)&buf);
    Int32 cap;
    buf->Capacity(&cap);
    CharToByteBufferAdapter* adapter = new CharToByteBufferAdapter(cap, buf);
    if (adapter->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)adapter->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

CharToByteBufferAdapter::CharToByteBufferAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ IByteBuffer* byteBuffer)
    : CharBuffer(capacity >> 2)
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
}

Int32 CharToByteBufferAdapter::GetByteCapacity()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).getByteCapacity();
    // }
    // assert false : byteBuffer;
    return -1;
}

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

Boolean CharToByteBufferAdapter::IsAddressValid()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).isAddressValid();
    // }
    // assert false : byteBuffer;
    return FALSE;
}

void CharToByteBufferAdapter::AddressValidityCheck()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).addressValidityCheck();
    // } else {
    //     assert false : byteBuffer;
    // }
}

void CharToByteBufferAdapter::Free()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).free();
    // } else {
    //     assert false : byteBuffer;
    // }
}

ECode CharToByteBufferAdapter::Array(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharBuffer::Array(array);
}

ECode CharToByteBufferAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharBuffer::ArrayOffset(offset);
}

ECode CharToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    CharToByteBufferAdapter* buf = new CharToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode CharToByteBufferAdapter::Compact()
{
    Boolean isReadOnly;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
    mByteBuffer->SetLimit(mLimit << 2);
    mByteBuffer->SetPosition(mPosition << 2);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;

    return NOERROR;
}

ECode CharToByteBufferAdapter::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharBuffer::CompareTo(otherBuffer, result);
}

ECode CharToByteBufferAdapter::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Duplicate((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    CharToByteBufferAdapter* buf = new CharToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode CharToByteBufferAdapter::GetChar(
    /* [out] */ Char32* value)
{
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    return mByteBuffer->GetCharEx(mPosition++ << 2, value);
}

ECode CharToByteBufferAdapter::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->GetCharEx(index << 2, value);
}

ECode CharToByteBufferAdapter::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharBuffer::GetChars(dst);
}

ECode CharToByteBufferAdapter::GetCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharBuffer::GetCharsEx(off, len, dst);
}

ECode CharToByteBufferAdapter::GetLength(
    /* [out] */ Int32* number)
{
    return CharBuffer::GetLength(number);
}

ECode CharToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode CharToByteBufferAdapter::PutChar(
    /* [in] */ Char32 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    return mByteBuffer->PutCharEx(mPosition++ << 2, c);
}

ECode CharToByteBufferAdapter::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->PutCharEx(index << 2, c);
}

ECode CharToByteBufferAdapter::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharBuffer::PutChars(src);
}

ECode CharToByteBufferAdapter::PutCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharBuffer::PutCharsEx(off, len, src);
}

ECode CharToByteBufferAdapter::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    return CharBuffer::PutCharBuffer(src);
}

ECode CharToByteBufferAdapter::PutString(
    /* [in] */ const String& str)
{
    return CharBuffer::PutString(str);
}

ECode CharToByteBufferAdapter::PutStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharBuffer::PutStringEx(str, start, end);
}

ECode CharToByteBufferAdapter::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mByteBuffer->SetLimit(mLimit << 2);
    mByteBuffer->SetPosition(mPosition << 2);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Slice((IByteBuffer**)&byteBuf);
    assert(byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    CharToByteBufferAdapter* result = new CharToByteBufferAdapter(cap, byteBuf);
    if (result->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)result->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }
    mByteBuffer->Clear();

    return NOERROR;
}

ECode CharToByteBufferAdapter::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharBuffer::GetCharAt(index, c);
}

ECode CharToByteBufferAdapter::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    Int32 remaining;
    Remaining(&remaining);
    if (start < 0 || end < start || end > remaining) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<ICharBuffer> result;
    Duplicate((ICharBuffer**)&result);
    result->SetLimit(mPosition + end);
    result->SetPosition(mPosition + start);
    if (result->Probe(EIID_ICharSequence) != NULL) {
        *csq = (ICharSequence*)result->Probe(EIID_ICharSequence);
    }
    else {
        *csq = NULL;
    }

    return NOERROR;
}

ECode CharToByteBufferAdapter::AppendChar(
    /* [in] */ Char32 c)
{
    return CharBuffer::AppendChar(c);
}

ECode CharToByteBufferAdapter::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    return CharBuffer::AppendChars(csq);
}

ECode CharToByteBufferAdapter::AppendCharsEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharBuffer::AppendCharsEx(csq, start, end);
}

ECode CharToByteBufferAdapter::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    return CharBuffer::Read(target, number);
}

ECode CharToByteBufferAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return CharBuffer::Capacity(cap);
}

ECode CharToByteBufferAdapter::Clear()
{
    return CharBuffer::Clear();
}

ECode CharToByteBufferAdapter::Flip()
{
    return CharBuffer::Flip();
}

ECode CharToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return CharBuffer::HasArray(hasArray);
}

ECode CharToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return CharBuffer::HasRemaining(hasRemaining);
}

ECode CharToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode CharToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

ECode CharToByteBufferAdapter::CharToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

ECode CharToByteBufferAdapter::CharToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

ECode CharToByteBufferAdapter::CharToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CharToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return CharBuffer::GetLimit(limit);
}

ECode CharToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return CharBuffer::SetLimit(newLimit);
}

ECode CharToByteBufferAdapter::Mark()
{
    return CharBuffer::Mark();
}

ECode CharToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return CharBuffer::GetPosition(pos);
}

ECode CharToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return CharBuffer::SetPosition(newPosition);
}

ECode CharToByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return CharBuffer::Remaining(remaining);
}

ECode CharToByteBufferAdapter::Reset()
{
    return CharBuffer::Reset();
}

ECode CharToByteBufferAdapter::Rewind()
{
    return CharBuffer::Rewind();
}

ECode CharToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    return CharBuffer::ToString(str);
}
