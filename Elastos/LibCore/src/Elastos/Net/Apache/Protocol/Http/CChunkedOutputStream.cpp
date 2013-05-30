
#include "CChunkedOutputStream.h"
#include <elastos/Math.h>

ArrayOf<Byte>* CChunkedOutputStream::CRLF;// = { '\r', '\n' };
ArrayOf<Byte>* CChunkedOutputStream::HEX_DIGITS;// = {
    //    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    //};
ArrayOf<Byte>* CChunkedOutputStream::FINAL_CHUNK;// = { '0', '\r', '\n', '\r', '\n' };


ECode CChunkedOutputStream::Close()
{
    // TODO: Add your code here
    if (mClosed) {
        return E_IO_EXCEPTION;
    }
    mClosed = TRUE;
    WriteBufferedChunkToSocket();
    mSocketOut->WriteBuffer(*FINAL_CHUNK);
}

ECode CChunkedOutputStream::Flush()
{
    // TODO: Add your code here
    if (mClosed) {
        return E_IO_EXCEPTION; // don't throw; this stream might have been closed on the caller's behalf
    }
    WriteBufferedChunkToSocket();
    mSocketOut->Flush();;
}

ECode CChunkedOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    CheckNotClosed();
    CheckBounds(buffer, offset, count);

    while (count > 0) {
        Int32 numBytesWritten;
        Int32 size;
        mBufferedChunk->GetSize(&size);
        if (size > 0 || count < mMaxChunkLength) {
            // fill the buffered chunk and then maybe write that to the stream
            numBytesWritten = Math::Min(count, mMaxChunkLength - size);
            // TODO: skip unnecessary copies from buffer->bufferedChunk?
            mBufferedChunk->WriteBufferEx(offset, numBytesWritten, buffer);
            if (size == mMaxChunkLength) {
                WriteBufferedChunkToSocket();
            }

        } else {
            // write a single chunk of size maxChunkLength to the stream
            numBytesWritten = mMaxChunkLength;
            WriteHex(numBytesWritten);
            mSocketOut->WriteBufferEx(offset, numBytesWritten, buffer);
            mSocketOut->WriteBuffer(*CRLF);
        }

        offset += numBytesWritten;
        count -= numBytesWritten;
    };
    return NOERROR;
}

ECode CChunkedOutputStream::CheckError(
    /* [out] */ Boolean * pHasError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedOutputStream::constructor(
    /* [in] */ IOutputStream * pSocketOut,
    /* [in] */ Int32 maxChunkLength)
{
    // TODO: Add your code here
    mSocketOut = pSocketOut;
    mMaxChunkLength = Math::Max(1, DataLength(mMaxChunkLength));
    CByteArrayOutputStream::New(maxChunkLength, (IByteArrayOutputStream**) &mBufferedChunk);
}

Int32 CChunkedOutputStream::DataLength(
    /* [in] */ Int32 dataPlusHeaderLength)
{
}

ECode CChunkedOutputStream::WriteHex(
    /* [in] */ Int32 i)
{
    Int32 cursor = 8;
    do {
        (*mHex)[--cursor] = (*HEX_DIGITS)[i & 0xf];
    } while ((i >> 4) != 0);
    mSocketOut->WriteBufferEx(cursor, mHex->GetLength() - cursor, *mHex);
}

ECode CChunkedOutputStream::WriteBufferedChunkToSocket()
{
    Int32 size;
    mBufferedChunk->GetSize(&size);
    if (size <= 0) {
        return E_IO_EXCEPTION;
    }

    WriteHex(size);
    mBufferedChunk->WriteTo(mSocketOut);
    mBufferedChunk->Reset();
    mSocketOut->WriteBuffer(*CRLF);
}