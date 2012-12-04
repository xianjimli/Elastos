
#ifndef __IOUTILS_H__
#define __IOUTILS_H__

#include "Elastos.IO_server.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class IoUtils
{
public:
    /**
     * Calls close(2) on 'fd'. Also resets the internal int to -1.
     */
    static CARAPI Close(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Closes 'closeable', ignoring any exceptions. Does nothing if 'closeable' is null.
     */
    static CARAPI CloseQuietly(
        /* [in] */ ICloseable* closeable);

    /**
     * Returns the int file descriptor from within the given FileDescriptor 'fd'.
     */
    static CARAPI_(Int32) GetFd(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Returns a new FileDescriptor whose internal integer is set to 'fd'.
     */
    static CARAPI_(AutoPtr<IFileDescriptor>) NewFileDescriptor(
        /* [in] */ Int32 fd);

     /**
     * Creates a pipe by calling pipe(2), returning the two file descriptors in
     * elements 0 and 1 of the array 'fds'. fds[0] is the read end of the pipe.
     * fds[1] is the write end of the pipe. Throws an appropriate IOException on failure.
     */
    static CARAPI Pipe(
        /* [in] */ const ArrayOf<Int32>& fds);

    /**
     * Sets the int file descriptor within the given FileDescriptor 'fd' to 'newValue'.
     */
    static CARAPI SetFd(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 newValue);

    /**
     * Sets 'fd' to be blocking or non-blocking, according to the state of 'blocking'.
     */
    static CARAPI SetBlocking(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean blocking);

private:
    IoUtils();

    static CARAPI_(Boolean) SetBlocking(
        /* [in] */ Int32 fd,
        /* [in] */ Boolean blocking);
};


#endif //__IOUTILS_H__
