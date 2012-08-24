#ifndef __READER_H__
#define __READER_H__

#include <elastos.h>
#include <Elastos.Core.h>
#include "Elastos.IO_server.h"
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class Reader
{
public:
    /**
     * Closes this reader. Implementations of this method should free any
     * resources associated with the reader.
     *
     * @throws IOException
     *             if an error occurs while closing this reader.
     */
    virtual CARAPI Close() = 0;

    /**
     * Sets a mark position in this reader. The parameter {@code readLimit}
     * indicates how many characters can be read before the mark is invalidated.
     * Calling {@code reset()} will reposition the reader back to the marked
     * position if {@code readLimit} has not been surpassed.
     * <p>
     * This default implementation simply throws an {@code IOException};
     * subclasses must provide their own implementation.
     *
     * @param readLimit
     *            the number of characters that can be read before the mark is
     *            invalidated.
     * @throws IllegalArgumentException
     *             if {@code readLimit < 0}.
     * @throws IOException
     *             if an error occurs while setting a mark in this reader.
     * @see #markSupported()
     * @see #reset()
     */
    virtual CARAPI Mark(
        /* [in] */ Int32 readLimit);

    /**
     * Indicates whether this reader supports the {@code mark()} and
     * {@code reset()} methods. This default implementation returns
     * {@code false}.
     *
     * @return always {@code false}.
     */
    virtual CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    /**
     * Reads a single character from this reader and returns it as an integer
     * with the two higher-order bytes set to 0. Returns -1 if the end of the
     * reader has been reached.
     *
     * @return the character read or -1 if the end of the reader has been
     *         reached.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    virtual CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads characters from this reader and stores them in the character array
     * {@code buf} starting at offset 0. Returns the number of characters
     * actually read or -1 if the end of the reader has been reached.
     *
     * @param buf
     *            character array to store the characters read.
     * @return the number of characters read or -1 if the end of the reader has
     *         been reached.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    virtual CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Char8>* buffer,
        /* [out] */ Int32* number);

    /**
     * Reads at most {@code count} characters from this reader and stores them
     * at {@code offset} in the character array {@code buf}. Returns the number
     * of characters actually read or -1 if the end of the reader has been
     * reached.
     *
     * @param buf
     *            the character array to store the characters read.
     * @param offset
     *            the initial position in {@code buffer} to store the characters
     *            read from this reader.
     * @param count
     *            the maximum number of characters to read.
     * @return the number of characters read or -1 if the end of the reader has
     *         been reached.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    virtual CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Char8>* buffer,
        /* [out] */ Int32* number) = 0;

    /**
     * Indicates whether this reader is ready to be read without blocking.
     * Returns {@code true} if this reader will not block when {@code read} is
     * called, {@code false} if unknown or blocking will occur. This default
     * implementation always returns {@code false}.
     *
     * @return always {@code false}.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     * @see #read()
     * @see #read(char[])
     * @see #read(char[], int, int)
     */
    virtual CARAPI IsReady(
        /* [out] */ Boolean* ready);

    /**
     * Resets this reader's position to the last {@code mark()} location.
     * Invocations of {@code read()} and {@code skip()} will occur from this new
     * location. If this reader has not been marked, the behavior of
     * {@code reset()} is implementation specific. This default
     * implementation throws an {@code IOException}.
     *
     * @throws IOException
     *             always thrown in this default implementation.
     * @see #mark(int)
     * @see #markSupported()
     */
    virtual CARAPI Reset();

    /**
     * Skips {@code amount} characters in this reader. Subsequent calls of
     * {@code read} methods will not return these characters unless {@code
     * reset()} is used. This method may perform multiple reads to read {@code
     * count} characters.
     *
     * @param count
     *            the maximum number of characters to skip.
     * @return the number of characters actually skipped.
     * @throws IllegalArgumentException
     *             if {@code amount < 0}.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     * @see #mark(int)
     * @see #markSupported()
     * @see #reset()
     */
    virtual CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);
protected:
    /**
     * Constructs a new {@code Reader} with {@code this} as the object used to
     * synchronize critical sections.
     */
    Reader();

    /**
     * Constructs a new {@code Reader} with {@code lock} used to synchronize
     * critical sections.
     *
     * @param lock
     *            the {@code Object} used to synchronize critical sections.
     * @throws NullPointerException
     *             if {@code lock} is {@code null}.
     */
    Reader(
        /* [in] */ Mutex* lock);

    CARAPI Init(
        /* [in] */ Mutex* lock);

    virtual ~Reader();

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

    virtual ReadLocked(
        /* [out] */ Int32* value);

    virtual CARAPI SkipLocked(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);
protected:
    Mutex* mLock;
};

#endif //__READER_H__