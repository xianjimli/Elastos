
#include "cmdef.h"
#include "Int16ToByteBufferAdapter.h"
#include "ReadWriteDirectByteBuffer.h"

PInterface Int16ToByteBufferAdapter::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IInt16Buffer) {
        return (IInt16Buffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_Int16Buffer) {
        return reinterpret_cast<PInterface>((Int16Buffer*)this);
    }

    return NULL;
}


UInt32 Int16ToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Int16ToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode Int16ToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Int16ToByteBufferAdapter::Wrap(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> buf;
    byteBuffer->Slice((IByteBuffer**)&buf);
    Int32 cap;
    buf->Capacity(&cap);
    Int16ToByteBufferAdapter* adapter = new Int16ToByteBufferAdapter(cap, buf);
    if (adapter->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)adapter->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

Int16ToByteBufferAdapter::Int16ToByteBufferAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ IByteBuffer* byteBuffer)
    : Int16Buffer(capacity >> 1)
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
}

Int32 Int16ToByteBufferAdapter::GetByteCapacity()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).getByteCapacity();
    // }
    // assert false : byteBuffer;
    return -1;
}

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

Boolean Int16ToByteBufferAdapter::IsAddressValid()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).isAddressValid();
    // }
    // assert false : byteBuffer;
    return FALSE;
}

void Int16ToByteBufferAdapter::AddressValidityCheck()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).addressValidityCheck();
    // } else {
    //     assert false : byteBuffer;
    // }
}

void Int16ToByteBufferAdapter::Free()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).free();
    // } else {
    //     assert false : byteBuffer;
    // }
}

ECode Int16ToByteBufferAdapter::Array(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return Int16Buffer::Array(array);
}

ECode Int16ToByteBufferAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int16Buffer::ArrayOffset(offset);
}

ECode Int16ToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int16ToByteBufferAdapter* buf = new Int16ToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode Int16ToByteBufferAdapter::Compact()
{
    Boolean isReadOnly;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
    mByteBuffer->SetLimit(mLimit << 1);
    mByteBuffer->SetPosition(mPosition << 1);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;

    return NOERROR;
}

ECode Int16ToByteBufferAdapter::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int16Buffer::CompareTo(otherBuffer, result);
}

ECode Int16ToByteBufferAdapter::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Duplicate((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int16ToByteBufferAdapter* buf = new Int16ToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode Int16ToByteBufferAdapter::GetInt16(
    /* [out] */ Int16* value)
{
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    return mByteBuffer->GetInt16Ex(mPosition++ << 1, value);
}

ECode Int16ToByteBufferAdapter::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->GetInt16Ex(index << 1, value);
}

ECode Int16ToByteBufferAdapter::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16Buffer::GetInt16s(dst);
}

ECode Int16ToByteBufferAdapter::GetInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16Buffer::GetInt16sEx(off, len, dst);
}

ECode Int16ToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode Int16ToByteBufferAdapter::PutInt16(
    /* [in] */ Int16 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    return mByteBuffer->PutInt16Ex(mPosition++ << 1, c);
}

ECode Int16ToByteBufferAdapter::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->PutInt16Ex(index << 1, c);
}

ECode Int16ToByteBufferAdapter::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src)
{
    return Int16Buffer::PutInt16s(src);
}

ECode Int16ToByteBufferAdapter::PutInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int16>& src)
{
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL) {
        mByteBuffer->SetLimit(mLimit << 1);
        mByteBuffer->SetPosition(mPosition << 1);
        ReadWriteDirectByteBuffer* buf =
               (ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer);
        buf->PutInt16s(off, len, src);
        mPosition += len;
        return NOERROR;
    }
    else {
        return Int16Buffer::PutInt16sEx(off, len, src);
    }
}

ECode Int16ToByteBufferAdapter::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    return Int16Buffer::PutInt16Buffer(src);
}

ECode Int16ToByteBufferAdapter::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mByteBuffer->SetLimit(mLimit << 1);
    mByteBuffer->SetPosition(mPosition << 1);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Slice((IByteBuffer**)&byteBuf);
    assert(byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int16ToByteBufferAdapter* result = new Int16ToByteBufferAdapter(cap, byteBuf);
    if (result->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)result->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }
    mByteBuffer->Clear();

    return NOERROR;
}

ECode Int16ToByteBufferAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return Int16Buffer::Capacity(cap);
}

ECode Int16ToByteBufferAdapter::Clear()
{
    return Int16Buffer::Clear();
}

ECode Int16ToByteBufferAdapter::Flip()
{
    return Int16Buffer::Flip();
}

ECode Int16ToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int16Buffer::HasArray(hasArray);
}

ECode Int16ToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int16Buffer::HasRemaining(hasRemaining);
}

ECode Int16ToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode Int16ToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI Int16ToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int16ToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int16ToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int16Buffer::GetLimit(limit);
}

ECode Int16ToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int16Buffer::SetLimit(newLimit);
}

ECode Int16ToByteBufferAdapter::Mark()
{
    return Int16Buffer::Mark();
}

ECode Int16ToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int16Buffer::GetPosition(pos);
}

ECode Int16ToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int16Buffer::SetPosition(newPosition);
}

ECode Int16ToByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int16Buffer::Remaining(remaining);
}

ECode Int16ToByteBufferAdapter::Reset()
{
    return Int16Buffer::Reset();
}

ECode Int16ToByteBufferAdapter::Rewind()
{
    return Int16Buffer::Rewind();
}
