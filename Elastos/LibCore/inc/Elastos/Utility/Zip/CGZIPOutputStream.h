
#ifndef __CGZIPOUTPUTSTREAM_H__
#define __CGZIPOUTPUTSTREAM_H__

#include "_CGZIPOutputStream.h"
#include "DeflaterOutputStream.h"
#include "CCRC32.h"
#include <elastos/AutoPtr.h>

/**
 * The {@code GZIPOutputStream} class is used to write data to a stream in the
 * GZIP storage format.
 */
CarClass(CGZIPOutputStream), public DeflaterOutputStream
{
public:
    CGZIPOutputStream();

    /**
     * Indicates to the stream that all data has been written out, and any GZIP
     * terminal data can now be written.
     *
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    //@Override
    CARAPI Finish();

    /**
     * Write up to nbytes of data from the given buffer, starting at offset off,
     * to the underlying stream in GZIP format.
     */
    //@Override
    CARAPI WriteBufferEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 val);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    /**
     * Construct a new {@code GZIPOutputStream} to write data in GZIP format to
     * the underlying stream.
     *
     * @param os
     *            the {@code OutputStream} to write data to.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* os);

    /**
     * Construct a new {@code GZIPOutputStream} to write data in GZIP format to
     * the underlying stream. Set the internal compression buffer to size
     * {@code size}.
     *
     * @param os
     *            the {@code OutputStream} to write to.
     * @param size
     *            the internal buffer size.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* os,
        /* [in] */ Int32 size);

private:
    CARAPI_(Int64) WriteInt64(
        /* [in] */ Int64 i);

    CARAPI_(Int32) WriteInt16(
        /* [in] */ Int32 i);

protected:
    /**
     * The checksum algorithm used when treating uncompressed data.
     */
    AutoPtr<CCRC32> mCrc;
};


#endif //__CGZIPOUTPUTSTREAM_H__
