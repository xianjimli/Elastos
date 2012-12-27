
#include "cmdef.h"
#include "DoubleBuffer.h"
#include "BufferFactory.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

// {6548CB4D-07DE-41e9-9AF8-8625E6DF3F08}
extern "C" const InterfaceID EIID_DoubleBuffer =
    { 0x6548cb4d, 0x7de, 0x41e9, { 0x9a, 0xf8, 0x86, 0x25, 0xe6, 0xdf, 0x3f, 0x8 } };

ECode DoubleBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IDoubleBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewDoubleBufferEx(capacity, buf);
}

ECode DoubleBuffer::Wrap(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ IDoubleBuffer** buf)
{
    return WrapEx(array, 0, array->GetLength(), buf);
}

ECode DoubleBuffer::WrapEx(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ IDoubleBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    Int32 length = array->GetLength();
    if ((start < 0) || (len < 0) || (Int64) start + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<IDoubleBuffer> doubleBuf;
    BufferFactory::NewDoubleBuffer(array, (IDoubleBuffer**)&doubleBuf);
    DoubleBuffer* bufCls = (DoubleBuffer*)doubleBuf->Probe(EIID_DoubleBuffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = doubleBuf;
    return NOERROR;
}

DoubleBuffer::DoubleBuffer(
    /* [in] */ Int32 capacity)
    : Buffer(capacity)
{
    mElementSizeShift = 3;
}

ECode DoubleBuffer::Array(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return ProtectedArray(array);
}

ECode DoubleBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode DoubleBuffer::CompareTo(
    /* [in] */ IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 remaining, otherRemaining;
    Remaining(&remaining);
    otherBuffer->Remaining(&otherRemaining);
    Int32 compareRemaining = (remaining < otherRemaining) ? remaining : otherRemaining;
    Int32 thisPos = mPosition;
    Int32 otherPos;
    otherBuffer->GetPosition(&otherPos);
    Double thisDouble, otherDouble;
    while (compareRemaining > 0) {
        GetDoubleEx(thisPos, &thisDouble);
        otherBuffer->GetDoubleEx(otherPos, &otherDouble);
        // checks for double and NaN inequality
        if ((thisDouble != otherDouble)
                && ((thisDouble == thisDouble) || (otherDouble == otherDouble))) {
            *result = thisDouble < otherDouble ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode DoubleBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return GetDoublesEx(0, dst->GetLength(), dst);
}

ECode DoubleBuffer::GetDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Double>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || ((Int64) off + (Int64) len > length)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    for (Int32 i = off; i < off + len; i++) {
        GetDouble(&(*dst)[i]);
    }

    return NOERROR;
}

ECode DoubleBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode DoubleBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return PutDoublesEx(0, src.GetLength(), src);
}

ECode DoubleBuffer::PutDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Double>& src)
{
    Int32 length = src.GetLength();
    if ((off < 0) || (len < 0) || (Int64) off + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    for (Int32 i = off; i < off + len; i++) {
        PutDouble(src[i]);
    }
    return NOERROR;
}

ECode DoubleBuffer::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    if (src == (IDoubleBuffer*)this->Probe(EIID_IDoubleBuffer)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    Int32 srcRemaining, remaining;
    src->Remaining(&srcRemaining);
    Remaining(&remaining);
    if (srcRemaining > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }

    ArrayOf<Double>* doubles = ArrayOf<Double>::Alloc(srcRemaining);
    src->GetDoubles(doubles);
    PutDoubles(*doubles);
    return NOERROR;
}

String DoubleBuffer::ToString()
{
    StringBuffer buf;
//    buf.append(getClass().getName());
    buf += "DoubleBuffer";
    Int32 cap, pos, limit;
    Capacity(&cap);
    GetPosition(&pos);
    GetLimit(&limit);
    buf += ", status: capacity=";

    buf += cap;
    buf += " position=";
    buf += pos;
    buf += " limit=";
    buf += limit;
    return String(buf);
}
