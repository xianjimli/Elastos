
#ifndef __INFLATERINPUTSTREAM_H__
#define __INFLATERINPUTSTREAM_H__

#include "Elastos.Utility.Zip_server.h"
#include "CInflater.h"
#include <elastos/AutoPtr.h>

/**
 * This class provides an implementation of {@code FilterInputStream} that
 * uncompresses data that was compressed using the <i>DEFLATE</i> algorithm
 * (see <a href="http://www.gzip.org/algorithm.txt">specification</a>).
 * Basically it wraps the {@code Inflater} class and takes care of the
 * buffering.
 *
 * @see Inflater
 * @see DeflaterOutputStream
 */
class InflaterInputStream
{
public:
    InflaterInputStream();

    virtual ~InflaterInputStream();

    /**
     * Reads a single byte of decompressed data.
     *
     * @return the byte read.
     * @throws IOException
     *             if an error occurs reading the byte.
     */
    //@Override
    virtual CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads up to {@code nbytes} of decompressed data and stores it in
     * {@code buffer} starting at {@code off}.
     *
     * @param buffer
     *            the buffer to write data to.
     * @param off
     *            offset in buffer to start writing.
     * @param nbytes
     *            number of bytes to read.
     * @return Number of uncompressed bytes read
     * @throws IOException
     *             if an IOException occurs.
     */
    //@Override
    virtual CARAPI ReadBufferEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Skips up to {@code byteCount} bytes of uncompressed data.
     *
     * @param byteCount the number of bytes to skip.
     * @return the number of uncompressed bytes skipped.
     * @throws IOException if an error occurs skipping.
     */
    //@Override
    virtual CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    /**
     * Returns 0 when when this stream has exhausted its input; and 1 otherwise.
     * A result of 1 does not guarantee that further bytes can be returned,
     * with or without blocking.
     *
     * <p>Although consistent with the RI, this behavior is inconsistent with
     * {@link InputStream#available()}, and violates the <a
     * href="http://en.wikipedia.org/wiki/Liskov_substitution_principle">Liskov
     * Substitution Principle</a>. This method should not be used.
     *
     * @return 0 if no further bytes are available. Otherwise returns 1,
     *         which suggests (but does not guarantee) that additional bytes are
     *         available.
     * @throws IOException if this stream is closed or an error occurs
     */
    //@Override
    virtual CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Closes the input stream.
     *
     * @throws IOException
     *             If an error occurs closing the input stream.
     */
    //@Override
    virtual CARAPI Close();

    /**
     * Marks the current position in the stream. This implementation overrides
     * the super type implementation to do nothing at all.
     *
     * @param readlimit
     *            of no use.
     */
    //@Override
    virtual CARAPI Mark(
        /* [in] */ Int32 readLimit);

    /**
     * Reset the position of the stream to the last marked position. This
     * implementation overrides the supertype implementation and always throws
     * an {@link IOException IOException} when called.
     *
     * @throws IOException
     *             if the method is called
     */
    //@Override
    virtual CARAPI Reset();

    /**
     * Returns whether the receiver implements {@code mark} semantics. This type
     * does not support {@code mark()}, so always responds {@code false}.
     *
     * @return false, always
     */
    //@Override
    virtual CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    virtual CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

protected:
    /**
     * This is the most basic constructor. You only need to pass the {@code
     * InputStream} from which the compressed data is to be read from. Default
     * settings for the {@code Inflater} and internal buffer are be used. In
     * particular the Inflater expects a ZLIB header from the input stream.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     */
    CARAPI Init(
        /* [in] */ IInputStream* is);

    /**
     * This constructor lets you pass a specifically initialized Inflater,
     * for example one that expects no ZLIB header.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     * @param inflater
     *            the specific {@code Inflater} for uncompressing data.
     */
    CARAPI Init(
        /* [in] */ IInputStream* is,
        /* [in] */ IInflater* inflater);

    /**
     * This constructor lets you specify both the {@code Inflater} as well as
     * the internal buffer size to be used.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     * @param inflater
     *            the specific {@code Inflater} for uncompressing data.
     * @param bsize
     *            the size to be used for the internal buffer.
     */
    CARAPI Init(
        /* [in] */ IInputStream* is,
        /* [in] */ IInflater* inflater,
        /* [in] */ Int32 bsize);

    /**
     * Fills the input buffer with data to be decompressed.
     *
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    virtual CARAPI Fill();

private:
    CARAPI CheckClosed();

public:
    static const Int32 BUF_SIZE = 512;

    Boolean mClosed;

    /**
     * True if this stream's last byte has been returned to the user. This
     * could be because the underlying stream has been exhausted, or if errors
     * were encountered while inflating that stream.
     */
    Boolean mEof;

    Int32 mNativeEndBufSize; // android-only

protected:
    /**
     * The inflater used for this stream.
     */
    AutoPtr<CInflater> mInf;

    /**
     * The input buffer used for decompression.
     */
    ArrayOf<Byte>* mBuf;

    /**
     * The length of the buffer.
     */
    Int32 mLen;

    AutoPtr<IInputStream> mIn;
};

#endif //__INFLATERINPUTSTREAM_H__
