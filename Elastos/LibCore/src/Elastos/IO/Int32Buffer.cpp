
#include "cmdef.h"
#include "Int32Buffer.h"
#include "BufferFactory.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

//  {AE6AA98E-18B9-4de2-A2DA-C030CEF90746}
extern "C" const InterfaceID EIID_Int32Buffer =
     { 0xae6aa98e, 0x18b9, 0x4de2, { 0xa2, 0xda, 0xc0, 0x30, 0xce, 0xf9, 0x7, 0x46 } };

ECode Int32Buffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt32Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewInt32Buffer(capacity, buf);
}

ECode Int32Buffer::Wrap(
    /* [in] */ ArrayOf<Int32>* array,
    /* [out] */ IInt32Buffer** buf)
{
    return WrapEx(array, 0, array->GetLength(), buf);
}

ECode Int32Buffer::WrapEx(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ IInt32Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (array == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException();
    }

    if ((start < 0) || (len < 0) || (Int32)start + (Int32)len > array->GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<IInt32Buffer> Int32Buf;
    BufferFactory::NewInt32BufferEx(array, (IInt32Buffer**)&Int32Buf);
    Int32Buffer* bufCls = (Int32Buffer*)Int32Buf->Probe(EIID_Int32Buffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = Int32Buf;
    return NOERROR;
}

Int32Buffer::Int32Buffer(
    /* [in] */ Int32 capacity)
    : Buffer(capacity)
{
    mElementSizeShift = 2;
}

ECode Int32Buffer::Array(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return ProtectedArray(array);
}

ECode Int32Buffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode Int32Buffer::CompareTo(
    /* [in] */ IInt32Buffer* otherBuffer,
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
    Int32 thisInt32, otherInt32;
    while (compareRemaining > 0) {
        GetInt32Ex(thisPos, &thisInt32);
        otherBuffer->GetInt32Ex(otherPos, &otherInt32);
        // checks for Int32 and NaN inequality
        if (thisInt32 != otherInt32) {
            *result = thisInt32 < otherInt32 ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode Int32Buffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst)
{
    return GetInt32sEx(0, dst->GetLength(), dst);
}

ECode Int32Buffer::GetInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int32>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || ((Int32)off + (Int32)len > length)) {
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
        GetInt32(&(*dst)[i]);
    }

    return NOERROR;
}

ECode Int32Buffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode Int32Buffer::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src)
{
    return PutInt32sEx(0, src.GetLength(), src);
}

ECode Int32Buffer::PutInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int32>& src)
{
    Int32 length = src.GetLength();
    if ((off < 0) || (len < 0) || (Int32) off + (Int32) len > length) {
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
        PutInt32(src[i]);
    }
    return NOERROR;
}

ECode Int32Buffer::PutInt32Buffer(
    /* [in] */ IInt32Buffer* src)
{
    if (src == (IInt32Buffer*)this->Probe(EIID_IInt32Buffer)) {
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

    ArrayOf<Int32>* Int32s = ArrayOf<Int32>::Alloc(srcRemaining);
    src->GetInt32s(Int32s);
    PutInt32s(*Int32s);
    return NOERROR;
}

String Int32Buffer::ToString()
{
    StringBuffer buf;
//    buf.append(getClass().getName());
    buf += "Int32Buffer";
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
