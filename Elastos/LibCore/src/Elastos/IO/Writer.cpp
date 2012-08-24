#include "cmdef.h"
#include "Writer.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

const CString Writer::TOKEN_NULL = "NULL";

Writer::Writer()
{}

Writer::Writer(
    /* [in] */ Mutex* lock)
{
    assert(lock != NULL);
    mLock = lock;
}

Writer::~Writer()
{}

ECode Writer::Write(
    /* [in] */ Int32 oneChar32)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return WriteLocked(oneChar32);
}

ECode Writer::WriteLocked(
    /* [in] */ Int32 oneChar32)
{
    assert(mLock != NULL);

    ArrayOf_<Char8, 5> buf;
    Int32 len;
    Character::ToChars(oneChar32, buf, 0, &len);
    buf[len] = '\0';
    return WriteBuffer(buf);
}

ECode Writer::WriteBuffer(
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    Int32 count;
    FAIL_RETURN(Character::GetCharCount(buffer, 0, buffer.GetLength(), &count));
    return WriteBufferEx(0, count, buffer);
}

ECode Writer::WriteString(
    /* [in] */ CString str)
{
    return WriteStringEx(0, str.GetCharCount(), str);
}

ECode Writer::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
     if (count < 0) {
//      throw new StringIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return WriteStringExLocked(offset, count, str);
}

ECode Writer::WriteStringExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
     if (count < 0) {
//      throw new StringIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    ArrayOf<Char8> buf(const_cast<char*>((const char*)str), str.GetLength());

    return WriteBufferEx(offset, count, buf);
}

ECode Writer::Append(
    /* [in] */ Char32 c)
{
    return Write(c);
}

ECode Writer::AppendEx(
    /* [in] */ ICharSequence* csq)
{
    if (NULL == csq) {
        return WriteString(TOKEN_NULL);
    }
    else {
        String tmp;
        csq->ToString(&tmp);
        ECode ec = WriteString(tmp);
        return ec;
    }
}

ECode Writer::AppendEx2(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (NULL == csq) {
        String tmp = String(TOKEN_NULL).Substring(start, end);
        return WriteString(tmp);
    }
    else {
        AutoPtr<ICharSequence> subcsq;
        csq->SubSequence(start, end, (ICharSequence**)&subcsq);
        String tmp;
        subcsq->ToString(&tmp);
        ECode ec = WriteString(tmp);
        return ec;
    }
}

ECode Writer::CheckError(
    /* [out] */ Boolean* hasError)
{
    assert(hasError != NULL);

    *hasError = FALSE;
    return NOERROR;
}

ECode Writer::Init(
    /* [in] */ Mutex* lock)
{
    assert(lock != NULL);
    mLock = lock;
    return NOERROR;
}
