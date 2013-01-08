#include "OutputStreamWriter.h"
#include "ByteBuffer.h"
#include "CharBuffer.h"
#include "cmdef.h"

OutputStreamWriter::OutputStreamWriter()
{
}

ECode OutputStreamWriter::Close()
{
    //Mutex::Autolock lock(mLock);
    // if (mEncoder != NULL) {
    //     mEncoder->Flush(mBuf);
    //     Flush();
    //     mOut->Flush();
    //     mOut->Close();
    //     mEncoder = NULL;
    //     mBytes = NULL;
    // }

    return NOERROR;
}

ECode OutputStreamWriter::Flush()
{
    Mutex::Autolock lock(mLock);
    FAIL_RETURN(CheckStatus());
    Int32 position;
    mBytes->GetPosition(&position);
    if (position > 0) {
        mBytes->Flip();
        ArrayOf<Byte>* buf;
        mBytes->Array(&buf);
        mOut->WriteBufferEx(0, position, *buf);
        mBytes->Clear();
    }
    return mOut->Flush();
}

ECode OutputStreamWriter::CheckStatus()
{
//     if (mEncoder == NULL) {
//         return E_IO_EXCEPTION;
// //        throw new IOException("OutputStreamWriter is closed");
//     }
    return NOERROR;
}

ECode OutputStreamWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    Mutex::Autolock lock(mLock);
    FAIL_RETURN(CheckStatus());
    ArrayOf<Char32>* c = ArrayOf<Char32>::Alloc(1);
    (*c)[0] = (Char32)oneChar32;
    AutoPtr<ICharBuffer> chars;
    CharBuffer::Wrap(c, (ICharBuffer**)&chars);
    return Convert(chars);
}

ECode OutputStreamWriter::Convert(
    /* [in] */ ICharBuffer* chars)
{
//    CoderResult result = encoder.encode(chars, bytes, true);
    // while (TRUE) {
    //     if (result.isError()) {
    //         throw new IOException(result.toString());
    //     } else if (result.isOverflow()) {
    //         // flush the output buffer
    //         flush();
    //         result = encoder.encode(chars, bytes, true);
    //         continue;
    //     }
    //     break;
    // }
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8> &buffer)
{
    Mutex::Autolock lock(mLock);
    FAIL_RETURN(CheckStatus());
    if (offset < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    if (&buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("buffer == null");
    }
    if (count < 0 || offset > buffer.GetLength() - count) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(String(buffer.GetPayload()), (ICharSequence**)&charSeq);
    AutoPtr<ICharBuffer> chars;
    CharBuffer::WrapEx3(charSeq, offset, count, (ICharBuffer**)&chars);//char8? or char32?
    return Convert(chars);
}

ECode OutputStreamWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    Mutex::Autolock lock(mLock);
    if (count < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new StringIndexOutOfBoundsException();
    }
    if (str.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("str == null");
    }
    if (offset < 0 || offset > str.GetLength() - count) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new StringIndexOutOfBoundsException();
    }
    FAIL_RETURN(CheckStatus());
    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(String(str), (ICharSequence**)&charSeq);
    AutoPtr<ICharBuffer> chars;
    CharBuffer::WrapEx3(charSeq, offset, count + offset, (ICharBuffer**)&chars);
    return Convert(chars);
}

ECode OutputStreamWriter::GetEncoding(
    /* [out] */ String *encoding)
{
    VALIDATE_NOT_NULL(encoding);
    // if (mEncoder == NULL) {
    //     *encoding = String(NULL);
    //     return NOERROR;
    // }
    // return HistoricalNamesUtil.getHistoricalName(encoder.charset().name());
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::Init(
    /* [in] */ IOutputStream *os)
{
    ASSERT_SUCCEEDED(ByteBuffer::Allocate(8192, (IByteBuffer**)&mBytes));
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::Init(
    /* [in] */ IOutputStream *os,
    /* [in] */ const String &enc)
{
    ASSERT_SUCCEEDED(ByteBuffer::Allocate(8192, (IByteBuffer**)&mBytes));
    return E_NOT_IMPLEMENTED;
}



