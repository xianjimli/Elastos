
#ifndef __CFILEDESCRIPTOR_H__
#define __CFILEDESCRIPTOR_H__

#include "_CFileDescriptor.h"
#include <Elastos.Core.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CFileDescriptor)
{
public:
    /**
     * The FileDescriptor representing standard input.
     */
    static const AutoPtr<IFileDescriptor> in;

    /**
     * FileDescriptor representing standard out.
     */
    static const AutoPtr<IFileDescriptor> out;

    /**
     * FileDescriptor representing standard error.
     */
    static const AutoPtr<IFileDescriptor> err;

public:
    /**
     * Constructs a new FileDescriptor containing an invalid handle. The
     * contained handle is usually modified by native code at a later point.
     */
    CFileDescriptor();

    /**
     * Ensures that data which is buffered within the underlying implementation
     * is written out to the appropriate device before returning.
     *
     * @throws SyncFailedException
     *             when the operation fails.
     */
    CARAPI Sync();

    /**
     * Indicates whether this FileDescriptor is valid.
     *
     * @return {@code true} if this FileDescriptor is valid, {@code false}
     *         otherwise.
     */
    CARAPI Valid(
        /* [out] */ Boolean* isValid);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetDescriptor(
        /* [out] */ Int32* des);

    CARAPI SetDescriptor(
        /* [in] */ Int32 fd);

private:
    CARAPI SyncImpl();

private:
    friend class FileInputStream;
    friend class FileOutputStream;
    friend class CRandomAccessFile;
    friend class IoUtils;
    friend AutoPtr<IFileDescriptor> CreateFD(Int32 descriptor);

    // BEGIN android-changed
    // file descriptors are only int on android
    /**
     * Represents a link to any underlying OS resources for this FileDescriptor.
     * A value of -1 indicates that this FileDescriptor is invalid.
     */
    Int32 mDescriptor;
    // END android-changed

    Boolean mReadOnly;
};

#endif // __CFILEDESCRIPTOR_H__
