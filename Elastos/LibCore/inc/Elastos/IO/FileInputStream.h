#ifndef __FILEINPUTSTREAM_H__
#define __FILEINPUTSTREAM_H__

#include "InputStream.h"
#include "CFileDescriptor.h"
#include "Com.Kortide.Platform.h"
#include <elastos/AutoPtr.h>

/**
 * A specialized {@link InputStream} that reads from a file in the file system.
 * All read requests made by calling methods in this class are directly
 * forwarded to the equivalent function of the underlying operating system.
 * Since this may induce some performance penalty, in particular if many small
 * read requests are made, a FileInputStream is often wrapped by a
 * BufferedInputStream.
 *
 * @see BufferedInputStream
 * @see FileOutputStream
 */
class FileInputStream : public InputStream
{
protected:
    FileInputStream();

    /**
     * Ensures that all resources for this stream are released when it is about
     * to be garbage collected.
     *
     * @throws IOException
     *             if an error occurs attempting to finalize this stream.
     */
    virtual ~FileInputStream();

    /**
     * Constructs a new {@code FileInputStream} based on {@code file}.
     *
     * @param file
     *            the file from which this stream reads.
     * @throws FileNotFoundException
     *             if {@code file} does not exist.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             read request.
     */
    CARAPI Init(
        /* [in] */ IFile* file);

    /**
     * Constructs a new {@code FileInputStream} on the {@link FileDescriptor}
     * {@code fd}. The file must already be open, therefore no
     * {@code FileNotFoundException} will be thrown.
     *
     * @param fd
     *            the FileDescriptor from which this stream reads.
     * @throws NullPointerException
     *             if {@code fd} is {@code null}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             read request.
     */
    CARAPI Init(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Constructs a new {@code FileInputStream} on the file named
     * {@code fileName}. The path of {@code fileName} may be absolute or
     * relative to the system property {@code "user.dir"}.
     *
     * @param fileName
     *            the path and name of the file from which this stream reads.
     * @throws FileNotFoundException
     *             if there is no file named {@code fileName}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             read request.
     */
    CARAPI Init(
        /* [in] */ const String& fileName);

public:
    CARAPI Available(
        /* [out] */ Int32* avail);

    /**
     * Closes this stream.
     *
     * @throws IOException
     *             if an error occurs attempting to close this stream.
     */
    CARAPI Close();

    /**
     * Returns the {@link FileChannel} equivalent to this input stream.
     * <p>
     * The file channel is read-only and has an initial position within the file
     * that is the same as the current position of this stream within the file.
     * All changes made to the underlying file descriptor state via the channel
     * are visible by the input stream and vice versa.
     *
     * @return the file channel for this stream.
     */
    virtual CARAPI GetChannel(
        /* [out] */ IFileChannel** channel);

    /**
     * Returns the {@link FileDescriptor} representing the operating system
     * resource for this stream.
     *
     * @return the {@code FileDescriptor} for this stream.
     * @throws IOException
     *             if an error occurs while getting this stream's
     *             {@code FileDescriptor}.
     */
    virtual CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    /**
     * Reads a single byte from this stream and returns it as an integer in the
     * range from 0 to 255. Returns -1 if the end of this stream has been
     * reached.
     *
     * @return the byte read or -1 if the end of this stream has been reached.
     * @throws IOException
     *             if this stream is closed or another I/O error occurs.
     */
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads bytes from this stream and stores them in the byte array
     * {@code buffer}.
     *
     * @param buffer
     *            the byte array in which to store the bytes read.
     * @return the number of bytes actually read or -1 if the end of the stream
     *         has been reached.
     * @throws IOException
     *             if this stream is closed or another I/O error occurs.
     */
    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Reads at most {@code count} bytes from this stream and stores them in the
     * byte array {@code buffer} starting at {@code offset}.
     *
     * @param buffer
     *            the byte array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code buffer} to store the bytes read
     *            from this stream.
     * @param count
     *            the maximum number of bytes to store in {@code buffer}.
     * @return the number of bytes actually read or -1 if the end of the stream
     *         has been reached.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if
     *             {@code offset + count} is greater than the size of
     *             {@code buffer}.
     * @throws IOException
     *             if the stream is closed or another IOException occurs.
     */
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Skips {@code count} number of bytes in this stream. Subsequent
     * {@code read()}s will not return these bytes unless {@code reset()} is
     * used. If the underlying stream is unseekable, an IOException is thrown.
     *
     * @param count
     *            the number of bytes to skip.
     * @return the number of bytes actually skipped.
     * @throws IOException
     *             if {@code count < 0}, this stream is closed or unseekable,
     *             or another IOException occurs.
     */
    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);
protected:
    CARAPI CloseLocked();

    virtual CARAPI GetChannelLocked(
        /* [out] */ IFileChannel** channel);

    CARAPI ReadBufferExLocked(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI SkipLocked(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);
private:
    CARAPI OpenCheck();

    CARAPI OpenCheckLocked();

private:
    /**
     * The {@link FileDescriptor} representing this {@code FileInputStream}.
     */
    AutoPtr<CFileDescriptor> mFd;

    // The unique file channel associated with this FileInputStream (lazily
    // initialized).
    AutoPtr<IFileChannel> mChannel;

    Boolean mInnerFD;

    AutoPtr<IFileSystem> mFileSystem;

    Mutex repositioningLock;
};
#endif //__FILEINPUTSTREAM_H__