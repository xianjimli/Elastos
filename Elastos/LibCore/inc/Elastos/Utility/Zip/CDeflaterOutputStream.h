
#ifndef __CDEFLATEROUTPUTSTREAM_H__
#define __CDEFLATEROUTPUTSTREAM_H__


#include "_CDeflaterOutputStream.h"
#include "DeflaterOutputStream.h"


CarClass(CDeflaterOutputStream), public DeflaterOutputStream
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI Finish();

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
    CARAPI constructor(
        /* [in] */ IOutputStream* os);

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
    CARAPI constructor(
        /* [in] */ IOutputStream* os,
        /* [in] */ IDeflater* def);

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
    CARAPI constructor(
        /* [in] */ IOutputStream* os,
        /* [in] */ IDeflater* def,
        /* [in] */ Int32 bsize);
};

#endif //__CDEFLATEROUTPUTSTREAM_H__
