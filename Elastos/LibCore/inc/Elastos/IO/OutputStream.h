#ifndef __OUTPUTSTREAM_H__
#define __OUTPUTSTREAM_H__

#include <elastos.h>
#include <Elastos.Core.h>
#include "Elastos.IO_server.h"
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class OutputStream
{
public:
    OutputStream();

    virtual ~OutputStream();

    /**
     * Closes this stream. Implementations of this method should free any
     * resources used by the stream. This implementation does nothing.
     *
     * @throws IOException
     *             if an error occurs while closing this stream.
     */
    virtual CARAPI Close();

    /**
     * Flushes this stream. Implementations of this method should ensure that
     * any buffered data is written out. This implementation does nothing.
     *
     * @throws IOException
     *             if an error occurs while flushing this stream.
     */
    virtual CARAPI Flush();

    /**
     * Writes the entire contents of the byte array {@code buffer} to this
     * stream.
     *
     * @param buffer
     *            the buffer to be written.
     * @throws IOException
     *             if an error occurs while writing to this stream.
     */

    /**
     * Writes a single byte to this stream. Only the least significant byte of
     * the integer {@code oneByte} is written to the stream.
     *
     * @param oneByte
     *            the byte to be written.
     * @throws IOException
     *             if an error occurs while writing to this stream.
     */
    virtual CARAPI Write(
        /* [in] */ Int32 oneByte) = 0;

    virtual CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

    /**
     * Writes {@code count} bytes from the byte array {@code buffer} starting at
     * position {@code offset} to this stream.
     *
     * @param buffer
     *            the buffer to be written.
     * @param offset
     *            the start position in {@code buffer} from where to get bytes.
     * @param count
     *            the number of bytes from {@code buffer} to write to this
     *            stream.
     * @throws IOException
     *             if an error occurs while writing to this stream.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if
     *             {@code offset + count} is bigger than the length of
     *             {@code buffer}.
     */
    virtual CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

    /**
     * Returns true if this writer has encountered and suppressed an error. Used
     * by PrintStreams as an alternative to checked exceptions.
     */
    virtual CARAPI CheckError(
        /* [out] */ Boolean* hasError);

protected:
    virtual CARAPI_(Mutex*) GetSelfLock() = 0;
};

#endif //__OUTPUTSTREAM_H__