#include "cmdef.h"
#include "CharArrayWriter.h"
#include <elastos/Math.h>
#include <elastos/Character.h>

using namespace Elastos::System;

CharArrayWriter::CharArrayWriter()
    : mBuf(NULL)
    , mCount(0)
{
}

CharArrayWriter::~CharArrayWriter()
{
    if(mBuf != NULL)
    {
        BufferOf<Byte>::Free(mBuf);
    }
}

ECode CharArrayWriter::Init()
{
    Writer::Init(GetSelfLock());
    mBuf = BufferOf<Byte>::Alloc(32);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    mLock = &mBufLock;
    return NOERROR;
}

ECode CharArrayWriter::Init(
    /*[in]*/ Int32 initialSize)
{
    Writer::Init(GetSelfLock());
    if (initialSize < 0) {
//      throw new IllegalArgumentException("size < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = BufferOf<Byte>::Alloc(initialSize);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    mLock = &mBufLock;
    return NOERROR;
}

ECode CharArrayWriter::Close()
{
    return NOERROR;
}

ECode CharArrayWriter::Expand(
    /*[in]*/ Int32 i)
{
    Int32 used = mBuf->GetUsed();
    Int32 size = mBuf->GetCapacity();
    /* Can the buffer handle @i more chars, if not expand it */
    if (used + i <= size) {
        return NOERROR;
    }

    Int32 newLen = Math::Max(2 * size, used + i);
    BufferOf<Byte>* newbuf = BufferOf<Byte>::Alloc(newLen);
    for(Int32 i = 0; i < used; i ++){
        (*newbuf)[i] = (*mBuf)[i];
    }
    BufferOf<Byte>::Free(mBuf);
    mBuf = newbuf;
    return NOERROR;
}

ECode CharArrayWriter::Flush()
{
    return NOERROR;
}

ECode CharArrayWriter::Reset()
{
    Mutex::Autolock lock(mLock);
    mCount = 0;
    return NOERROR;
}

ECode CharArrayWriter::GetSize(
    /*[out]*/Int32* size)
{
    Mutex::Autolock lock(mLock);
    *size = mCount;
    return NOERROR;
}

ECode CharArrayWriter::ToCharArray(
    /* [out] */ BufferOf<Byte>** str)
{
    assert(str != NULL);

    Mutex::Autolock lock(mLock);
    *str = mBuf->Clone();
    if (*str == NULL) return E_OUT_OF_MEMORY_ERROR;
    return NOERROR;
}


ECode CharArrayWriter::ToString(
    /*[out]*/ String* result)
{
    assert(result != NULL);

    Mutex::Autolock lock(mLock);
    *result = String::Duplicate(String((char*)mBuf->GetPayload()));
    return NOERROR;
}

ECode CharArrayWriter::Write(
    /*[in]*/ Int32 oneChar32)
{
    Mutex::Autolock lock(mLock);

    Int32 len = Character::GetByteCount(oneChar32);
    Expand(len);
    Int32 number;
    Character::ToChars(oneChar32, *mBuf, mCount, &number);
    mCount += number;
    return NOERROR;
}

ECode CharArrayWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const BufferOf<Byte>& buffer)
{
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // made implicit null check explicit,
    // removed redundant check,
    // added null check, used (offset | len) < 0 instead of
    // (offset < 0) || (len < 0) to safe one operation
    Int32 number;
    Character::GetCharCount(buffer, offset, buffer.GetCapacity(), &number);
    if ((offset | count) < 0 || count > number) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    Mutex::Autolock lock(mLock);

    Int32 endOffset;
    Character::GetOffsetByChars(buffer, 0,
            buffer.GetCapacity(), offset, count, &endOffset);
    Expand(endOffset - offset);
    for(Int32 i = 0; i < count; i++) {
        Char32 oneChar32;
        Character::GetCharAt(buffer, offset, &oneChar32, &offset);
        Character::ToChars(oneChar32, *mBuf, mCount, &number);
        mCount += number;
    }
    return NOERROR;
}

ECode CharArrayWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ String str)
{
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, used (offset | len) < 0
    // instead of (offset < 0) || (len < 0) to safe one operation
    Int32 number;
    Character::GetCharCount(str, offset, str.GetLength(), &number);
    if ((offset | count) < 0 || count > number) {
//      throw new StringIndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    Mutex::Autolock lock(mLock);

    Int32 endOffset;
    Character::GetOffsetByChars(str, offset, count, &endOffset);
    Expand(endOffset - offset);
    for(Int32 i = 0; i < count; i++) {
        Int32 number;
        Char32 oneChar32;
        Character::GetCharAt(str, offset, &oneChar32, &offset);
        Character::ToChars(oneChar32, *mBuf, mCount, &number);
        mCount += number;
    }
    return NOERROR;
}

ECode CharArrayWriter::WriteTo(
    /*[in]*/ IWriter* out)
{
    Mutex::Autolock lock(mLock);
    return out->WriteBufferEx(0, mCount, *mBuf);
}

ECode CharArrayWriter::Append(
        /* [in] */ Char32 c)
{
    return Write(c);
}

ECode CharArrayWriter::AppendEx(
    /* [in] */ ICharSequence* csq)
{
    if (NULL == csq) {
        //todo: we need StringWrapper here
//        append(TOKEN_NULL, 0, TOKEN_NULL.length());
        return E_NOT_IMPLEMENTED;
    } else {
        Int32 number;
        FAIL_RETURN(csq->GetLength(&number));
        return AppendEx2(csq, 0, number);
    }
}


ECode CharArrayWriter::AppendEx2(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    //todo: we need StringWrapper here
//    if (NULL == csq) {
//        csq = TOKEN_NULL;
//    }
//    String output = csq.subSequence(start, end).toString();
//    WriteStringEx(output, 0, output.length());
    return E_NOT_IMPLEMENTED;
}
