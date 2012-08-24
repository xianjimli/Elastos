
#include "cmdef.h"
#include "BufferedWriter.h"

#include <elastos/Character.h>

using namespace Elastos::Core;

//todo:
#ifdef _linux
const CString BufferedWriter::mLineSeparator = "\n"; // = AccessController.doPrivileged(new PriviAction<String>("line.separator"));
#else
const CString BufferedWriter::mLineSeparator = "\r\n";
#endif

BufferedWriter::BufferedWriter()
{
}

BufferedWriter::~BufferedWriter()
{
    if(mBuf != NULL) {
        ArrayOf<Char8>::Free(mBuf);
    }
}

ECode BufferedWriter::Init(
    /*[in]*/ IWriter* wout)
{
    return Init(wout, 8192);
}

ECode BufferedWriter::Init(
    /*[in]*/ IWriter* wout,
    /*[in]*/ Int32 size)
{
    // super(out);
    Writer::Init(GetSelfLock());
    if (size <= 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mOut = wout;
    mBuf = ArrayOf<Char8>::Alloc(size);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    return NOERROR;
}

ECode BufferedWriter::Close()
{
    Mutex::Autolock lock(mLock);

    return CloseLocked();
}

ECode BufferedWriter::CloseLocked()
{
    if (IsClosed()) {
        return NOERROR;
    }

    ECode ec = FlushInternal();
    ArrayOf<Char8>::Free(mBuf);
    mBuf = NULL;
    ECode ec2 = mOut->Close();
    if(ec2 != NOERROR && ec == NOERROR) ec = ec2;
    mOut = NULL;

    if (ec != NOERROR) return ec;

    return NOERROR;
}

ECode BufferedWriter::Flush()
{
    Mutex::Autolock lock(mLock);

    return FlushLocked();
}

ECode BufferedWriter::FlushLocked()
{
    FAIL_RETURN(CheckNotClosed());
    FAIL_RETURN(FlushInternal());
    FAIL_RETURN(mOut->Flush());

    return NOERROR;
}

ECode BufferedWriter::CheckNotClosed()
{
    if (IsClosed()) {
//      throw new IOException("BufferedWriter is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode BufferedWriter::FlushInternal()
{
    if (mPos > 0) {
        FAIL_RETURN(mOut->WriteBufferEx(0, mPos, *mBuf));
    }
    mPos = 0;

    return NOERROR;
}

Boolean BufferedWriter::IsClosed()
{
    return mOut == NULL;
}

ECode BufferedWriter::NewLine()
{
    FAIL_RETURN(WriteStringEx(0, mLineSeparator.GetLength(), mLineSeparator));
    return NOERROR;
}

ECode BufferedWriter::WriteBufferEx(
    /*[in]*/ Int32 offset,
    /*[in]*/ Int32 count,
    /*[in]*/ const ArrayOf<Char8>& cbuf)
{
    Mutex::Autolock lock(mLock);

    return WriteBufferExLocked(offset, count, cbuf);
}

ECode BufferedWriter::WriteBufferExLocked(
    /*[in]*/ Int32 offset,
    /*[in]*/ Int32 count,
    /*[in]*/ const ArrayOf<Char8>& cbuf)
{
    FAIL_RETURN(CheckNotClosed());
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // made implicit null check explicit, used (offset | count) < 0
    // instead of (offset < 0) || (count < 0) to safe one operation
    //assert(cbuf != NULL);
    Int32 number;
    Character::GetCharCount(cbuf, offset, cbuf.GetLength(), &number);
    if ((offset | count) < 0 || count > number) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    Int32 index;
    Character::GetOffsetByChars(cbuf, 0, cbuf.GetLength(), offset, count, &index);
    Int32 bytes = index - offset;

    Int32 bufferLength = mBuf->GetLength();
    if (mPos == 0 && bytes >= bufferLength) {
        FAIL_RETURN(mOut->WriteBufferEx(offset, count, cbuf));
        return NOERROR;
    }
    Int32 available = bufferLength - mPos;
    if (bytes < available) {
        available = bytes;
    }
    if (available > 0) {
        for(Int32 i = 0; i < available; i++) {
            (*mBuf)[mPos + i] = cbuf[offset + i];
        }
        mPos += available;
    }
    if (mPos == bufferLength) {
        FAIL_RETURN(mOut->WriteBufferEx(0, bufferLength, *mBuf));
        mPos = 0;
        if (bytes > available) {
            offset += available;
            available = bytes - available;
            if (available >= bufferLength) {
                FAIL_RETURN(mOut->WriteBufferEx(offset, available, cbuf));
                return NULL;
            }

            for(Int32 i = 0; i < available; i++) {
                (*mBuf)[mPos + i] = cbuf[offset + i];
            }
            mPos += available;
        }
    }

    return NOERROR;
}

ECode BufferedWriter::Write(
    /*[in]*/ Int32 oneChar)
{
    Mutex::Autolock lock(mLock);

    return WriteLocked(oneChar);
}

ECode BufferedWriter::WriteLocked(
    /*[in]*/ Int32 oneChar)
{
    FAIL_RETURN(CheckNotClosed());

    Int32 bufferLength = mBuf->GetLength();
    if (mPos >= bufferLength) {
        FAIL_RETURN(mOut->WriteBufferEx(0, bufferLength, *mBuf));
        mPos = 0;
    }

    Int32 number;
    Character::ToChars(oneChar, *mBuf, mPos, &number);
    mPos += number;

    return NOERROR;
}

ECode BufferedWriter::WriteStringEx(
    /*[in]*/ Int32 offset,
    /*[in]*/ Int32 count,
    /*[in]*/ CString str)
{
    Mutex::Autolock lock(mLock);

    return WriteStringExLocked(offset, count, str);
}

ECode BufferedWriter::WriteStringExLocked(
    /*[in]*/ Int32 offset,
    /*[in]*/ Int32 count,
    /*[in]*/ CString str)
{
    FAIL_RETURN(CheckNotClosed());
    if (count <= 0) {
        return NOERROR;
    }
    Int32 number;
    Character::GetCharCount(str, offset, str.GetLength(), &number);
    if (count > number || offset < 0) {
        // throw new StringIndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 bufferLength = mBuf->GetLength();
    Int32 index;
    Character::GetOffsetByChars(str, offset, count, &index);
    Int32 bytes = index - offset;
    if (mPos == 0 && bytes >= bufferLength) {
        return mOut->WriteStringEx(offset, count, str);
    }

    Int32 available = bufferLength - mPos;
    if (bytes < available) {
        available = bytes;
    }
    if (available > 0) {
        for(Int32 i = 0; i < available; i++) {
            (*mBuf)[mPos + i] = str[offset + i];
        }
        mPos += available;
    }
    if (mPos == bufferLength) {
        FAIL_RETURN(mOut->WriteBufferEx(0, bufferLength, *mBuf));
        mPos = 0;
        if (bytes > available) {
            offset += available;
            available = bytes - available;
            if (available >= bufferLength) {
                Int32 number;
                Character::GetCharCount(str, offset, str.GetLength(), &number);
                return mOut->WriteStringEx(offset, number, str);
            }
            for(Int32 i = 0; i < available; i++) {
                (*mBuf)[mPos + i] = str[offset + i];
            }
            mPos += available;
        }
    }

    return NOERROR;
}