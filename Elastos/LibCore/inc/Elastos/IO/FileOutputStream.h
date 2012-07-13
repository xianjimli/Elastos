#ifndef __FILEOUTPUTSTREAM_H__
#define __FILEOUTPUTSTREAM_H__

#include "OutputStream.h"
#include "CFileDescriptor.h"
#include "Com.Kortide.Platform.h"
#include <elastos/AutoPtr.h>

/**
 * A specialized {@link OutputStream} that writes to a file in the file system.
 * All write requests made by calling methods in this class are directly
 * forwarded to the equivalent function of the underlying operating system.
 * Since this may induce some performance penalty, in particular if many small
 * write requests are made, a FileOutputStream is often wrapped by a
 * BufferedOutputStream.
 *
 * @see BufferedOutputStream
 * @see FileInputStream
 */
class FileOutputStream : public OutputStream
{
protected:
    FileOutputStream();

    /**
     * Frees any resources allocated for this stream before it is garbage
     * collected. This method is called from the Java Virtual Machine.
     *
     * @throws IOException
     *             if an error occurs attempting to finalize this stream.
     */
    virtual ~FileOutputStream();

    /**
     * Constructs a new FileOutputStream on the File {@code file}. If the file
     * exists, it is overwritten.
     *
     * @param file
     *            the file to which this stream writes.
     * @throws FileNotFoundException
     *             if {@code file} cannot be opened for writing.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             write request.
     * @see java.lang.SecurityManager#checkWrite(FileDescriptor)
     */
    CARAPI Init(
        /* [in] */ IFile* file);

    /**
     * Constructs a new FileOutputStream on the File {@code file}. The
     * parameter {@code append} determines whether or not the file is opened and
     * appended to or just opened and overwritten.
     *
     * @param file
     *            the file to which this stream writes.
     * @param append
     *            indicates whether or not to append to an existing file.
     * @throws FileNotFoundException
     *             if the {@code file} cannot be opened for writing.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             write request.
     * @see java.lang.SecurityManager#checkWrite(FileDescriptor)
     * @see java.lang.SecurityManager#checkWrite(String)
     */
    CARAPI Init(
        /* [in] */ IFile* file,
        /* [in] */ Boolean append);

    /**
     * Constructs a new FileOutputStream on the FileDescriptor {@code fd}. The
     * file must already be open, therefore no {@code FileNotFoundException}
     * will be thrown.
     *
     * @param fd
     *            the FileDescriptor to which this stream writes.
     * @throws NullPointerException
     *             if {@code fd} is {@code null}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             write request.
     * @see java.lang.SecurityManager#checkWrite(FileDescriptor)
     */
    CARAPI Init(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Constructs a new FileOutputStream on the file named {@code filename}. If
     * the file exists, it is overwritten. The {@code filename} may be absolute
     * or relative to the system property {@code "user.dir"}.
     *
     * @param filename
     *            the name of the file to which this stream writes.
     * @throws FileNotFoundException
     *             if the file cannot be opened for writing.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             write request.
     */
    CARAPI Init(
        /* [in] */ const String& fileName);

    /**
     * Constructs a new FileOutputStream on the file named {@code filename}.
     * The parameter {@code append} determines whether or not the file is opened
     * and appended to or just opened and overwritten. The {@code filename} may
     * be absolute or relative to the system property {@code "user.dir"}.
     *
     * @param filename
     *            the name of the file to which this stream writes.
     * @param append
     *            indicates whether or not to append to an existing file.
     * @throws FileNotFoundException
     *             if the file cannot be opened for writing.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             write request.
     */
    CARAPI Init(
        /* [in] */ const String& fileName,
        /* [in] */ Boolean append);

public:
    /**
     * Closes this stream. This implementation closes the underlying operating
     * system resources allocated to represent this stream.
     *
     * @throws IOException
     *             if an error occurs attempting to close this stream.
     */
    CARAPI Close();

    /**
     * Returns the FileChannel equivalent to this output stream.
     * <p>
     * The file channel is write-only and has an initial position within the
     * file that is the same as the current position of this stream within the
     * file. All changes made to the underlying file descriptor state via the
     * channel are visible by the output stream and vice versa.
     *
     * @return the file channel representation for this stream.
     */
    virtual CARAPI GetChannel(
        /* [out] */ IFileChannel** channel);

    /**
     * Returns a FileDescriptor which represents the lowest level representation
     * of an operating system stream resource.
     *
     * @return a FileDescriptor representing this stream.
     * @throws IOException
     *             if an error occurs attempting to get the FileDescriptor of
     *             this stream.
     */
    virtual CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    /**
     * Writes the specified byte {@code oneByte} to this stream. Only the low
     * order byte of the integer {@code oneByte} is written.
     *
     * @param oneByte
     *            the byte to be written.
     * @throws IOException
     *             if this stream is closed an error occurs attempting to write
     *             to this stream.
     */
    CARAPI Write(
        /* [in] */ Int32 oneByte);

    /**
     * Writes the entire contents of the byte array {@code buffer} to this
     * stream.
     *
     * @param buffer
     *            the buffer to be written to the file.
     * @throws IOException
     *             if this stream is closed or an error occurs attempting to
     *             write to this stream.
     */
    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

    /**
     * Writes {@code count} bytes from the byte array {@code buffer} starting at
     * {@code offset} to this stream.
     *
     * @param buffer
     *            the buffer to write to this stream.
     * @param offset
     *            the index of the first byte in {@code buffer} to write.
     * @param count
     *            the number of bytes from {@code buffer} to write.
     * @throws IndexOutOfBoundsException
     *             if {@code count < 0} or {@code offset < 0}, or if
     *             {@code count + offset} is greater than the length of
     *             {@code buffer}.
     * @throws IOException
     *             if this stream is closed or an error occurs attempting to
     *             write to this stream.
     * @throws NullPointerException
     *             if {@code buffer} is {@code null}.
     */
    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

private:
    CARAPI OpenCheck();

private:
    AutoPtr<CFileDescriptor> mFd;

    Boolean mInnerFD;

    // The unique file channel associated with this FileInputStream (lazily
    // initialized).
    AutoPtr<IFileChannel> mChannel;

    AutoPtr<IFileSystem> mFileSystem;
};


#endif // __FILEOUTPUTSTREAM_H__
