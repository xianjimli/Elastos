
#include "cmdef.h"
#include "BaseByteBuffer.h"
#include "CharToByteBufferAdapter.h"
#include "DoubleToByteBufferAdapter.h"
#include "FloatToByteBufferAdapter.h"
#include "Int16ToByteBufferAdapter.h"
#include "Int32ToByteBufferAdapter.h"
#include "Int64ToByteBufferAdapter.h"

BaseByteBuffer::BaseByteBuffer(
    /* [in] */ Int32 capacity)
    : ByteBuffer(capacity)
{}

ECode BaseByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    if (this->Probe(EIID_IByteBuffer) != NULL) {
        byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    }

    return CharToByteBufferAdapter::Wrap(byteBuf, buffer);
}

ECode BaseByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    if (this->Probe(EIID_IByteBuffer) != NULL) {
        byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    }

    return DoubleToByteBufferAdapter::Wrap(byteBuf, buffer);
}

ECode BaseByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    if (this->Probe(EIID_IByteBuffer) != NULL) {
        byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    }

    return FloatToByteBufferAdapter::Wrap(byteBuf, buffer);
}

ECode BaseByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    if (this->Probe(EIID_IByteBuffer) != NULL) {
        byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    }

    return Int16ToByteBufferAdapter::Wrap(byteBuf, buffer);
}

ECode BaseByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    if (this->Probe(EIID_IByteBuffer) != NULL) {
        byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    }

    return Int32ToByteBufferAdapter::Wrap(byteBuf, buffer);
}

ECode BaseByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    if (this->Probe(EIID_IByteBuffer) != NULL) {
        byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    }

    return Int64ToByteBufferAdapter::Wrap(byteBuf, buffer);
}

ECode BaseByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 temp;
    GetInt32(&temp);
    *value = (Char32)temp;
    return NOERROR;
}

ECode BaseByteBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 temp;
    GetInt32Ex(index, &temp);
    *value = (Char32)temp;
    return NOERROR;
}

ECode BaseByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return PutInt32((Int32)value);
}

ECode BaseByteBuffer::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return PutInt32Ex(index, (Int32)value);
}
