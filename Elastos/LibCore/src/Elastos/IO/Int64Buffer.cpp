
#include "cmdef.h"
#include "Int64Buffer.h"
#include "BufferFactory.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

//  {286E2FAC-3118-4904-8D87-AD0C30BF47C7}
extern "C" const InterfaceID EIID_Int64Buffer =
     { 0x286e2fac, 0x3118, 0x4904, { 0x8d, 0x87, 0xad, 0xc, 0x30, 0xbf, 0x47, 0xc7 } };

ECode Int64Buffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt64Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewInt64Buffer(capacity, buf);
}

ECode Int64Buffer::Wrap(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ IInt64Buffer** buf)
{
    return WrapEx(array, 0, array->GetLength(), buf);
}

ECode Int64Buffer::WrapEx(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ IInt64Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (array == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException();
    }

    if ((start < 0) || (len < 0) || (Int64)start + (Int64)len > array->GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<IInt64Buffer> Int64Buf;
    BufferFactory::NewInt64BufferEx(array, (IInt64Buffer**)&Int64Buf);
    Int64Buffer* bufCls = (Int64Buffer*)Int64Buf->Probe(EIID_Int64Buffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = Int64Buf;
    return NOERROR;
}

Int64Buffer::Int64Buffer(
    /* [in] */ Int32 capacity)
    : Buffer(capacity)
{
    mElementSizeShift = 3;
}

ECode Int64Buffer::Array(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return ProtectedArray(array);
}

ECode Int64Buffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode Int64Buffer::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
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
    Int64 thisInt64, otherInt64;
    while (compareRemaining > 0) {
        GetInt64Ex(thisPos, &thisInt64);
        otherBuffer->GetInt64Ex(otherPos, &otherInt64);
        // checks for Int64 and NaN inequality
        if (thisInt64 != otherInt64) {
            *result = thisInt64 < otherInt64 ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode Int64Buffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return GetInt64sEx(0, dst->GetLength(), dst);
}

ECode Int64Buffer::GetInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int64>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || ((Int64)off + (Int64)len > length)) {
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
        GetInt64(&(*dst)[i]);
    }

    return NOERROR;
}

ECode Int64Buffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode Int64Buffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return PutInt64sEx(0, src.GetLength(), src);
}

ECode Int64Buffer::PutInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int64>& src)
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
        PutInt64(src[i]);
    }
    return NOERROR;
}

ECode Int64Buffer::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    if (src == (IInt64Buffer*)this->Probe(EIID_IInt64Buffer)) {
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

    ArrayOf<Int64>* Int64s = ArrayOf<Int64>::Alloc(srcRemaining);
    src->GetInt64s(Int64s);
    PutInt64s(*Int64s);
    return NOERROR;
}

String Int64Buffer::ToString()
{
    StringBuffer buf;
//    buf.append(getClass().getName());
    buf += "Int64Buffer";
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
