
#include "cmdef.h"
#include "CByteBufferHelper.h"
#include "BufferFactory.h"
#include "ByteBuffer.h"
#include <elastos/AutoPtr.h>

ECode CByteBufferHelper::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewByteBufferEx(capacity, buf);
}

ECode CByteBufferHelper::AllocateDirect(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewDirectByteBuffer(capacity, buf);
}

ECode CByteBufferHelper::Wrap(
    /* [in] */ const ArrayOf<Byte>& array,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    ArrayOf<Byte>* newArray = array.Clone();
    return BufferFactory::NewByteBuffer(newArray, buf);
}

ECode CByteBufferHelper::WrapEx(
    /* [in] */ const ArrayOf<Byte>& array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    Int32 length = array.GetLength();
    if ((start < 0) || (len < 0) || ((Int64) start + (Int64) len > length)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<IByteBuffer> byteBuf;
    ArrayOf<Byte>* newArray = array.Clone();
    BufferFactory::NewByteBuffer(newArray, (IByteBuffer**)&byteBuf);
    ByteBuffer* bufCls = (ByteBuffer*)byteBuf->Probe(EIID_ByteBuffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = byteBuf;
    return NOERROR;
}
