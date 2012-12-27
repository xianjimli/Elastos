
#include "cmdef.h"
#include "Int16Buffer.h"
#include "BufferFactory.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

//  {AE6AA98E-18B9-4de2-A2DA-C030CEF90746}
extern "C" const InterfaceID EIID_Int16Buffer =
     { 0xae6aa98e, 0x18b9, 0x4de2, { 0xa2, 0xda, 0xc0, 0x30, 0xce, 0xf9, 0x7, 0x46 } };

ECode Int16Buffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt16Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewInt16Buffer(capacity, buf);
}

ECode Int16Buffer::Wrap(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ IInt16Buffer** buf)
{
    return WrapEx(array, 0, array->GetLength(), buf);
}

ECode Int16Buffer::WrapEx(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ IInt16Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (array == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException();
    }

    if ((start < 0) || (len < 0) || (Int16)start + (Int16)len > array->GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<IInt16Buffer> Int16Buf;
    BufferFactory::NewInt16BufferEx(array, (IInt16Buffer**)&Int16Buf);
    Int16Buffer* bufCls = (Int16Buffer*)Int16Buf->Probe(EIID_Int16Buffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = Int16Buf;
    return NOERROR;
}

Int16Buffer::Int16Buffer(
    /* [in] */ Int32 capacity)
    : Buffer(capacity)
{
    mElementSizeShift = 1;
}

ECode Int16Buffer::Array(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return ProtectedArray(array);
}

ECode Int16Buffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode Int16Buffer::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
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
    Int16 thisInt16, otherInt16;
    while (compareRemaining > 0) {
        GetInt16Ex(thisPos, &thisInt16);
        otherBuffer->GetInt16Ex(otherPos, &otherInt16);
        // checks for Int16 and NaN inequality
        if (thisInt16 != otherInt16) {
            *result = thisInt16 < otherInt16 ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode Int16Buffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return GetInt16sEx(0, dst->GetLength(), dst);
}

ECode Int16Buffer::GetInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int16>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || ((Int16)off + (Int16)len > length)) {
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
        GetInt16(&(*dst)[i]);
    }

    return NOERROR;
}

ECode Int16Buffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode Int16Buffer::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src)
{
    return PutInt16sEx(0, src.GetLength(), src);
}

ECode Int16Buffer::PutInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int16>& src)
{
    Int32 length = src.GetLength();
    if ((off < 0) || (len < 0) || (Int16) off + (Int16) len > length) {
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
        PutInt16(src[i]);
    }
    return NOERROR;
}

ECode Int16Buffer::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    if (src == (IInt16Buffer*)this->Probe(EIID_IInt16Buffer)) {
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

    ArrayOf<Int16>* Int16s = ArrayOf<Int16>::Alloc(srcRemaining);
    src->GetInt16s(Int16s);
    PutInt16s(*Int16s);
    return NOERROR;
}

String Int16Buffer::ToString()
{
    StringBuffer buf;
//    buf.append(getClass().getName());
    buf += "Int16Buffer";
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
