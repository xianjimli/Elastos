#include "cmdef.h"
#include "InputStreamReader.h"
#include "ByteBuffer.h"
#include "CharBuffer.h"
#include "CCharsetHelper.h"
#include "CCharBufferHelper.h"
#include <stdio.h>
InputStreamReader::InputStreamReader()
    : mEndOfInput(FALSE)
{
    ASSERT_SUCCEEDED(ByteBuffer::Allocate(8192, (IByteBuffer**)&mBytes));
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream *in)
{
    AutoPtr<ICharset> ch;
    AutoPtr<ICharsetHelper> helper;

    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);
    helper->DefaultCharset((ICharset**)&ch);

    return Init(in, (ICharset*)ch);
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream *in,
    /* [in] */ const String &enc)
{
    Reader::Init(&mInLock);
//    super(in);
    if (enc.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException();
    }
    mIn = in;
    // try {
    //     decoder = Charset.forName(enc).newDecoder().onMalformedInput(
    //             CodingErrorAction.REPLACE).onUnmappableCharacter(
    //             CodingErrorAction.REPLACE);
    // } catch (IllegalArgumentException e) {
    //     throw (UnsupportedEncodingException)
    //             new UnsupportedEncodingException(enc).initCause(e);
    // }
    return mBytes->SetLimit(0);
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ ICharset* charset)
{
    Reader::Init(&mInLock);
    //super(in);
    mIn = in;
    /*decoder = charset.newDecoder().onMalformedInput(
            CodingErrorAction.REPLACE).onUnmappableCharacter(
            CodingErrorAction.REPLACE);*/
    return mBytes->SetLimit(0);
}

ECode InputStreamReader::Close()
{
    Mutex::Autolock lock(mLock);
    // BEGIN android-added
    // if (mdecoder != NULL) {
    //     decoder.reset();
    // }
    // END android-added
    //mDecoder = NULL;
    if (mIn != NULL) {
        FAIL_RETURN(mIn->Close());
        mIn = NULL;
    }
    return NOERROR;
}

ECode InputStreamReader::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    if (!IsOpen()) {
        *encoding = String(NULL);
        return NOERROR;
    }
 //   return HistoricalNamesUtil.getHistoricalName(decoder.charset().name());
    return E_NOT_IMPLEMENTED;
}

ECode InputStreamReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Mutex::Autolock lock(mLock);
    if (!IsOpen()) {
        return E_IO_EXCEPTION;
//        throw new IOException("InputStreamReader is closed");
    }
    ArrayOf<Char8>* buf = ArrayOf<Char8>::Alloc(1);
    Int32 number;
    FAIL_RETURN(ReadBufferEx(0, 1, buf, &number));
    *value = number != -1 ? (*buf)[0] : -1;
    return NOERROR;
}

ECode InputStreamReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    Mutex::Autolock lock(mLock);

    if (!IsOpen()) {
        return E_IO_EXCEPTION;
//        throw new IOException("InputStreamReader is closed");
    }
    // RI exception compatibility so we can run more tests.
    if (offset < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    if (buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("buffer == null");
    }
    if (count < 0 || offset > buffer->GetLength() - count) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    if (count == 0) {
        *number = 0;
        return NOERROR;
    }
#if 0
    AutoPtr<ICharSequence> charSeq;
    ECode ec = CStringWrapper::New(String(buffer->GetPayload()), (ICharSequence**)&charSeq);
    printf("ec = %d\n", ec);
    AutoPtr<ICharBuffer> out;
    printf("InputStreamReader charseq = %d, offset = %d, count = %d\n", charSeq.Get(), offset, count);
    ec = CharBuffer::WrapEx3(charSeq, 0, 2, (ICharBuffer**)&out);
#else
    AutoPtr<ICharBufferHelper> charBufferHelper;
    CCharBufferHelper::AcquireSingleton((ICharBufferHelper**)&charBufferHelper);
    AutoPtr<ICharBuffer> out;
    charBufferHelper->WrapArrayEx((ArrayOf<Char32>* )buffer, offset, count/sizeof(Char32), (ICharBuffer**)&out);
#endif
    //CoderResult result = CoderResult.UNDERFLOW;
    // bytes.remaining() indicates number of bytes in buffer
    // when 1-st time entered, it'll be equal to zero
    Boolean hasRemaining;
    mBytes->HasRemaining(&hasRemaining);
    Boolean needInput = !hasRemaining;

    //if end of document, do not need read again
    if (mEndOfInput) {
        *number = -1;
        return NOERROR;
    }

    Int32 size;
    Int32 was_red;
    while(out->HasRemaining(&hasRemaining), hasRemaining) {
        // fill the buffer if needed
        if (needInput) {
            //try {
            Int32 num, position;
            mIn->Available(&num);
            out->GetPosition(&position);
            if (num == 0 && position > offset) {
                // we could return the result without blocking read
                break;
            }
            // } catch (IOException e) {
            //     // available didn't work so just try the read
            // }

            Int32 capacity, limit, offset;
            mBytes->Capacity(&capacity);
            mBytes->GetLimit(&limit);
            mBytes->ArrayOffset(&offset);
            Int32 to_read = capacity - limit;
            Int32 off = offset + limit;
            ArrayOf<Byte>* buf;
            mBytes->Array(&buf);
            // printf("mIn read off = %d, to_read = %d\n", off, to_read);
            mIn->ReadBufferEx(off, to_read, buf, &was_red);
            // printf("mIn was_red = %d\n", was_red);
            if (was_red == -1) {
                mEndOfInput = TRUE;
                break;
            }
            else if (was_red == 0) {
                break;
            }
            mBytes->SetLimit(limit + was_red);
            //needInput = FALSE;
        }

        // decode bytes
        //ECode ec = mDecoder->Decode(mBytes.Get(), (ICharBuffer**)&out);

        //if (result.isUnderflow()) {
            // compact the buffer if no space left
        Int32 limit, capacity;
        mBytes->Capacity(&capacity);
        mBytes->GetLimit(&limit);
        if (limit == capacity) {
            mBytes->Compact();
            Int32 pos;
            mBytes->GetPosition(&pos);
            mBytes->SetLimit(pos);
            mBytes->SetPosition(0);
        }
            //needInput = TRUE;
        //}
        //else {
        //    break;
       // }
    }
#if 1
    mBytes->GetLimit(&size);
    // printf("mBytes limited = %d\n", size);

    if (mEndOfInput) {
        Char32 ch;
        for (Int32 i = 0; i < size; i++) {
            mBytes->GetCharEx(i, &ch);
            // printf("ch = %c -- %x, %d/%d\n", ch, ch, i, size);
            out->PutCharEx(i, ch);
        }
    }
#endif

    // if (result == CoderResult.UNDERFLOW && endOfInput) {
    //     result = decoder.decode(bytes, out, true);
    //     decoder.flush(out);
    //     decoder.reset();
    // }
    // if (result.isMalformed()) {
    //     throw new MalformedInputException(result.length());
    // } else if (result.isUnmappable()) {
    //     throw new UnmappableCharacterException(result.length());
    // }

    Int32 charBufPos;
    Int32 outLimit;
    out->GetLimit(&outLimit);
    out->GetPosition(&charBufPos);
    //*number = charBufPos == 0 ? -1 : charBufPos - offset;
    *number = size;
    return NOERROR;
}

Boolean InputStreamReader::IsOpen()
{
    return mIn != NULL;
}

ECode InputStreamReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);
    Mutex::Autolock lock(mLock);
    if (mIn == NULL) {
        return E_IO_EXCEPTION;
//        throw new IOException("InputStreamReader is closed");
    }
    //try {
    Boolean hasRemaining;
    mBytes->HasRemaining(&hasRemaining);
    Int32 number;
    mIn->Available(&number);
    *ready = hasRemaining || number > 0;
    return NOERROR;
    // } catch (IOException e) {
    //     return false;
    // }
}

