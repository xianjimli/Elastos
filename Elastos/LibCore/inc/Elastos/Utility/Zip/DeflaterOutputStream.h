
#ifndef __DEFLATEROUTPUTSTREAM_H__
#define __DEFLATEROUTPUTSTREAM_H__

#include "Elastos.Utility.Zip_server.h"
#include "CDeflater.h"
#include <elastos/AutoPtr.h>

/**
 * This class provides an implementation of {@code FilterOutputStream} that
 * compresses data using the <i>DEFLATE</i> algorithm. Basically it wraps the
 * {@code Deflater} class and takes care of the buffering.
 *
 * @see Deflater
 */
class DeflaterOutputStream
{
public:
    DeflaterOutputStream();

    virtual ~DeflaterOutputStream();

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
    virtual CARAPI Close();

    /**
     * Writes any unwritten data to the underlying stream. Does not close the
     * stream.
     *
     * @throws IOException
     *             If an error occurs.
     */
    virtual CARAPI Finish();

    //@Override
    virtual CARAPI Write(
        /* [in] */ Int32 i);

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
    virtual CARAPI WriteBufferEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [in] */ const ArrayOf<Byte>& buffer);

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
    virtual CARAPI Flush();

    //inherit frome super class
    virtual CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

protected:
    /**
     * Compress the data in the input buffer and write it to the underlying
     * stream.
     *
     * @throws IOException
     *             If an error occurs during deflation.
     */
    virtual CARAPI Deflate();

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
    CARAPI Init(
        /* [in] */ IOutputStream* os,
        /* [in] */ IDeflater* def);

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
    CARAPI Init(
        /* [in] */ IOutputStream* os);

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
    CARAPI Init(
        /* [in] */ IOutputStream* os,
        /* [in] */ IDeflater* def,
        /* [in] */ Int32 bsize);

    /**
     * @hide
     * @since 1.7
     */
    CARAPI Init(
        /* [in] */ IOutputStream* os,
        /* [in] */ Boolean syncFlush);

    /**
     * @hide
     * @since 1.7
     */
    CARAPI Init(
        /* [in] */ IOutputStream* os,
        /* [in] */ IDeflater* def,
        /* [in] */ Boolean syncFlush);

    /**
     * @hide
     * @since 1.7
     */
    CARAPI Init(
        /* [in] */ IOutputStream* os,
        /* [in] */ IDeflater* def,
        /* [in] */ Int32 bsize,
        /* [in] */ Boolean syncFlush);


public:
    static const Int32 BUF_SIZE = 512;

    Boolean mDone;

protected:
    /**
     * The target output stream for this filter stream.
     */
    AutoPtr<IOutputStream> mOut;

    /**
     * The buffer for the data to be written to.
     */
    ArrayOf<Byte>* mBuf;

    /**
     * The deflater used.
     */
    AutoPtr<CDeflater> mDef;

private:
    Boolean mSyncFlush;
};


#endif //__DEFLATEROUTPUTSTREAM_H__
