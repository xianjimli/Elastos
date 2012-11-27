
#include "cmdef.h"
#include "DeflaterOutputStream.h"
#include <stdio.h>


const Int32 DeflaterOutputStream::BUF_SIZE;

DeflaterOutputStream::DeflaterOutputStream()
    : mDone(FALSE)
    , mBuf(NULL)
    , mSyncFlush(FALSE)
{}

DeflaterOutputStream::~DeflaterOutputStream()
{
    if (mBuf != NULL) ArrayOf<Byte>::Free(mBuf);
}

/**
 * Compress the data in the input buffer and write it to the underlying
 * stream.
 *
 * @throws IOException
 *             If an error occurs during deflation.
 */
ECode DeflaterOutputStream::Deflate()
{
    Int32 byteCount;
    ECode ec = NOERROR;
    ec = mDef->Deflate(mBuf, &byteCount);
    while (byteCount != 0) {
        //FAIL_RETURN(mOut->WriteBufferEx(0, byteCount, *mBuf));
        FAIL_RETURN(mOut->WriteBufferEx(0, byteCount, *mBuf));
        mOut->Flush();
        mDef->Deflate(mBuf, &byteCount);
    }
    mOut->Flush();
    return NOERROR;
}

/**
 * Writes any unwritten compressed data to the underlying stream, the closes
 * all underlying streams. This stream can no longer be used after close()
 * has been called.
 *
 * @throws IOException
 *             If an error occurs while closing the data compression
 *             process.
 */
//@Override
ECode DeflaterOutputStream::Close()
{
    Boolean finished;
    mDef->Finished(&finished);
    if (!finished) {
        Finish();
    }
    mDef->End();
    return mOut->Close();
}

/**
 * Writes any unwritten data to the underlying stream. Does not close the
 * stream.
 *
 * @throws IOException
 *             If an error occurs.
 */
ECode DeflaterOutputStream::Finish()
{
    if (mDone) {
        return NOERROR;
    }
    mDef->Finish();
    Boolean finished;
    while (mDef->Finished(&finished), !finished) {
        Boolean need;
        mDef->NeedsInput(&need);
        if (need) {
            mDef->SetInputEx(*mBuf, 0, 0);
        }
        Int32 byteCount;
        FAIL_RETURN(mDef->Deflate(mBuf, &byteCount));
        FAIL_RETURN(mOut->WriteBufferEx(0, byteCount, *mBuf));
    }
   // mOut->Flush();
    mDone = TRUE;
    return NOERROR;
}

//@Override
ECode DeflaterOutputStream::Write(
    /* [in] */ Int32 i)
{
    ArrayOf_<Byte, 1> b;
    b[0] = (Byte)i;
    return WriteBufferEx(0, 1, b);
}

/**
 * Compresses {@code nbytes} of data from {@code buf} starting at
 * {@code off} and writes it to the underlying stream.
 *
 * @param buffer
 *            the buffer of data to compress.
 * @param off
 *            offset in buffer to extract data from.
 * @param nbytes
 *            the number of bytes of data to read from the buffer.
 * @throws IOException
 *             If an error occurs during writing.
 */
//@Override
ECode DeflaterOutputStream::WriteBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    if (mDone) {
//        throw new IOException("attempt to write after finish");
        return E_IO_EXCEPTION;
    }
    // avoid int overflow, check null buf
    if (off <= buffer.GetLength() && nbytes >= 0 && off >= 0
            && buffer.GetLength() - off >= nbytes) {
        Boolean need;
        mDef->NeedsInput(&need);
        if (!need) {
//            throw new IOException();
            return E_IO_EXCEPTION;
        }
        mDef->SetInputEx(buffer, off, nbytes);
        return Deflate();
    }
    else {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

/**
 * Flushes the underlying stream. This flushes only the bytes that can be
 * compressed at the highest level.
 *
 * <p>For deflater output streams constructed with Java 7's
 * {@code syncFlush} parameter set to true (not yet available on Android),
 * this first flushes all outstanding data so that it may be immediately
 * read by its recipient. Doing so may degrade compression.
 */
//@Override
ECode DeflaterOutputStream::Flush()
{
    if (mSyncFlush) {
        Int32 byteCount;
        while (mDef->DeflateEx2(0, mBuf->GetLength(),
            CDeflater::SYNC_FLUSH, mBuf, &byteCount), byteCount != 0) {
            FAIL_RETURN(mOut->WriteBufferEx(0, byteCount, *mBuf));
        }
    }
    return mOut->Flush();
}

/**
 * This constructor lets you pass the {@code Deflater} specifying the
 * compression algorithm.
 *
 * @param os
 *            is the {@code OutputStream} where to write the compressed data
 *            to.
 * @param def
 *            is the specific {@code Deflater} that is used to compress
 *            data.
 */
ECode DeflaterOutputStream::Init(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def)
{
    return Init(os, def, BUF_SIZE, FALSE);
}

/**
 * This is the most basic constructor. You only need to pass the {@code
 * OutputStream} to which the compressed data shall be written to. The
 * default settings for the {@code Deflater} and internal buffer are used.
 * In particular the {@code Deflater} produces a ZLIB header in the output
 * stream.
 *
 * @param os
 *            is the OutputStream where to write the compressed data to.
 */
ECode DeflaterOutputStream::Init(
    /* [in] */ IOutputStream* os)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend((CDeflater**)&deflater);
    return Init(os, (IDeflater*)deflater.Get(), BUF_SIZE, FALSE);
}

/**
 * This constructor lets you specify both the compression algorithm as well
 * as the internal buffer size to be used.
 *
 * @param os
 *            is the {@code OutputStream} where to write the compressed data
 *            to.
 * @param def
 *            is the specific {@code Deflater} that will be used to compress
 *            data.
 * @param bsize
 *            is the size to be used for the internal buffer.
 */
ECode DeflaterOutputStream::Init(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Int32 bsize)
{
    return Init(os, def, bsize, FALSE);
}

/**
 * @hide
 * @since 1.7
 */
ECode DeflaterOutputStream::Init(
    /* [in] */ IOutputStream* os,
    /* [in] */ Boolean syncFlush)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend((CDeflater**)&deflater);
    return Init(os, (IDeflater*)deflater.Get(), BUF_SIZE, syncFlush);
}

/**
 * @hide
 * @since 1.7
 */
ECode DeflaterOutputStream::Init(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Boolean syncFlush)
{
    return Init(os, def, BUF_SIZE, syncFlush);
}

/**
 * @hide
 * @since 1.7
 */
ECode DeflaterOutputStream::Init(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Int32 bsize,
    /* [in] */ Boolean syncFlush)
{
    mOut = os;
    if (os == NULL || def == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bsize <= 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDef = (CDeflater*)def;
    mSyncFlush = syncFlush;
    mBuf = ArrayOf<Byte>::Alloc(bsize);
    return NOERROR;
}

ECode DeflaterOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}
