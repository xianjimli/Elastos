
#ifndef __HH_MEMORYFILE_H
#define __HH_MEMORYFILE_H

#include "ext/frameworkext.h"
#include "elastos/AutoPtr.h"

using namespace Elastos;

/**
 * MemoryFile is a wrapper for the Linux ashmem driver.
 * MemoryFiles are backed by shared memory, which can be optionally
 * set to be purgeable.
 * Purgeable files may have their contents reclaimed by the kernel
 * in low memory conditions (only if allowPurging is set to true).
 * After a file is purged, attempts to read or write the file will
 * cause an IOException to be thrown.
 */
class MemoryFile
{
public:
    /**
     * Allocates a new ashmem region. The region is initially not purgable.
     *
     * @param name optional name for the file (can be null).
     * @param length of the memory file in bytes.
     * @throws IOException if the memory file could not be created.
     */
    MemoryFile(
        /* [in] */ const String& name,
        /* [in] */ Int32 length);

    /**
     * Creates a reference to an existing memory file. Changes to the original file
     * will be available through this reference.
     * Calls to {@link #allowPurging(boolean)} on the returned MemoryFile will fail.
     *
     * @param fd File descriptor for an existing memory file, as returned by
     *        {@link #getFileDescriptor()}. This file descriptor will be closed
     *        by {@link #close()}.
     * @param length Length of the memory file in bytes.
     * @param mode File mode. Currently only "r" for read-only access is supported.
     * @throws NullPointerException if <code>fd</code> is null.
     * @throws IOException If <code>fd</code> does not refer to an existing memory file,
     *         or if the file mode of the existing memory file is more restrictive
     *         than <code>mode</code>.
     *
     * @hide
     */
    MemoryFile(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 length,
        /* [in] */ const char* mode);

    /**
     * Closes the memory file. If there are no other open references to the memory
     * file, it will be deleted.
     */
    CARAPI_(void) Close();

    /**
     * Unmaps the memory file from the process's memory space, but does not close it.
     * After this method has been called, read and write operations through this object
     * will fail, but {@link #getFileDescriptor()} will still return a valid file descriptor.
     *
     * @hide
     */
    CARAPI_(void) Deactivate();

public:
    CARAPI_(void) Finalize();

    /**
     * Returns the length of the memory file.
     *
     * @return file length.
     */
    CARAPI_(Int32) Length();

    /**
     * Is memory file purging enabled?
     *
     * @return true if the file may be purged.
     */
    CARAPI_(Boolean) IsPurgingAllowed();

    /**
     * Enables or disables purging of the memory file.
     *
     * @param allowPurging true if the operating system can purge the contents
     * of the file in low memory situations
     * @return previous value of allowPurging
     */
    CARAPI_(Boolean) AllowPurging(
        /* [in] */ Boolean allowPurging);

    /**
     * Creates a new InputStream for reading from the memory file.
     *
     @return InputStream
     */
    CARAPI GetInputStream(
        /* [out] */ IInputStream** stream);

    /**
     * Creates a new OutputStream for writing to the memory file.
     *
     @return OutputStream
     */
    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** stream);

    /**
     * Reads bytes from the memory file.
     * Will throw an IOException if the file has been purged.
     *
     * @param buffer byte array to read bytes into.
     * @param srcOffset offset into the memory file to read from.
     * @param destOffset offset into the byte array buffer to read into.
     * @param count number of bytes to read.
     * @return number of bytes read.
     * @throws IOException if the memory file has been purged or deactivated.
     */
    CARAPI_(Int32) ReadBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 destOffset,
        /* [in] */ Int32 count);

    /**
     * Write bytes to the memory file.
     * Will throw an IOException if the file has been purged.
     *
     * @param buffer byte array to write bytes from.
     * @param srcOffset offset into the byte array buffer to write from.
     * @param destOffset offset  into the memory file to write to.
     * @param count number of bytes to write.
     * @throws IOException if the memory file has been purged or deactivated.
     */
    CARAPI_(void) WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 destOffset,
        /* [in] */ Int32 count);

    /**
     * Gets a ParcelFileDescriptor for the memory file. See {@link #getFileDescriptor()}
     * for caveats. This must be here to allow classes outside <code>android.os</code< to
     * make ParcelFileDescriptors from MemoryFiles, as
     * {@link ParcelFileDescriptor#ParcelFileDescriptor(FileDescriptor)} is package private.
     *
     *
     * @return The file descriptor owned by this memory file object.
     *         The file descriptor is not duplicated.
     * @throws IOException If the memory file has been closed.
     *
     * @hide
     */
    CARAPI GetParcelFileDescriptor(
        /* [out] */ IParcelFileDescriptor** des);

    /**
     * Gets a FileDescriptor for the memory file. Note that this file descriptor
     * is only safe to pass to {@link #MemoryFile(FileDescriptor,int)}). It
     * should not be used with file descriptor operations that expect a file descriptor
     * for a normal file.
     *
     * The returned file descriptor is not duplicated.
     *
     * @throws IOException If the memory file has been closed.
     *
     * @hide
     */
    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** des);

    /**
     * Checks whether the given file descriptor refers to a memory file.
     *
     * @throws IOException If <code>fd</code> is not a valid file descriptor.
     *
     * @hide
     */
    static CARAPI_(Boolean) IsMemoryFile(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Returns the size of the memory file that the file descriptor refers to,
     * or -1 if the file descriptor does not refer to a memory file.
     *
     * @throws IOException If <code>fd</code> is not a valid file descriptor.
     *
     * @hide
     */
    static CARAPI_(Int32) GetSize(
        /* [in] */ IFileDescriptor* fd);

private:
    /**
     * Checks whether the memory file has been deactivated.
     */
    CARAPI_(Boolean) IsDeactivated();

    /**
     * Checks whether the memory file has been closed.
     */
    CARAPI_(Boolean) IsClosed();

    /**
     * Converts a file mode string to a <code>prot</code> value as expected by
     * native_mmap().
     *
     * @throws IllegalArgumentException if the file mode is invalid.
     */
    static CARAPI_(Int32) ModeToProt(
        /* [in] */ const String& mode);

private:
    static const char* TAG;

    // mmap(2) protection flags from <sys/mman.h>
    static const Int32 PROT_READ = 0x1;
    static const Int32 PROT_WRITE = 0x2;

    AutoPtr<IFileDescriptor> mFD;   // ashmem file descriptor
    Int32 mAddress;                 // address of ashmem memory
    Int32 mLength;                  // total length of our ashmem region
    Boolean mAllowPurging;          // true if our ashmem region is unpinned
    Boolean mOwnsRegion;            // false if this is a ref to an existing ashmem region
};

#endif // __HH_MEMORYFILE_H

