
#include "cmdef.h"
#include "FloatBuffer.h"
#include "BufferFactory.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

//  {F4FEE9E7-CAA5-4d2e-9171-A96EB3CCB046}
extern "C" const InterfaceID EIID_FloatBuffer =
    { 0xf4fee9e7, 0xcaa5, 0x4d2e, { 0x91, 0x71, 0xa9, 0x6e, 0xb3, 0xcc, 0xb0, 0x46 } };

ECode FloatBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewFloatBufferEx(capacity, buf);
}

ECode FloatBuffer::Wrap(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ IFloatBuffer** buf)
{
    return WrapEx(array, 0, array->GetLength(), buf);
}

ECode FloatBuffer::WrapEx(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    Int32 length = array->GetLength();
    if ((start < 0) || (len < 0) || (Int64) start + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<IFloatBuffer> FloatBuf;
    BufferFactory::NewFloatBuffer(array, (IFloatBuffer**)&FloatBuf);
    FloatBuffer* bufCls = (FloatBuffer*)FloatBuf->Probe(EIID_FloatBuffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = FloatBuf;
    return NOERROR;
}

FloatBuffer::FloatBuffer(
    /* [in] */ Int32 capacity)
    : Buffer(capacity)
{
    mElementSizeShift = 2;
}

ECode FloatBuffer::Array(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return ProtectedArray(array);
}

ECode FloatBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode FloatBuffer::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
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
    Float thisFloat, otherFloat;
    while (compareRemaining > 0) {
        GetFloatEx(thisPos, &thisFloat);
        otherBuffer->GetFloatEx(otherPos, &otherFloat);
        // checks for Float and NaN inequality
        if ((thisFloat != otherFloat)
                && ((thisFloat == thisFloat) || (otherFloat == otherFloat))) {
            *result = thisFloat < otherFloat ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode FloatBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return GetFloatsEx(0, dst->GetLength(), dst);
}

ECode FloatBuffer::GetFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Float>* dst)
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
        GetFloat(&(*dst)[i]);
    }

    return NOERROR;
}

ECode FloatBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode FloatBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return PutFloatsEx(0, src.GetLength(), src);
}

ECode FloatBuffer::PutFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Float>& src)
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
        PutFloat(src[i]);
    }
    return NOERROR;
}

ECode FloatBuffer::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    if (src == (IFloatBuffer*)this->Probe(EIID_IFloatBuffer)) {
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

    ArrayOf<Float>* Floats = ArrayOf<Float>::Alloc(srcRemaining);
    src->GetFloats(Floats);
    PutFloats(*Floats);
    return NOERROR;
}

String FloatBuffer::ToString()
{
    StringBuffer buf;
//    buf.append(getClass().getName());
    buf += "FloatBuffer";
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
