
#ifndef __CLOCALSOCKET_H__
#define __CLOCALSOCKET_H__

#include "_CLocalSocket.h"

CarClass(CLocalSocket)
{
public:
    /**
     * Connects this socket to an endpoint. May only be called on an instance
     * that has not yet been connected.
     *
     * @param endpoint endpoint address
     * @throws IOException if socket is in invalid state or the address does
     * not exist.
     */
    CARAPI Connect(
        /* [in] */ ILocalSocketAddress* endpoint);

    /**
     * Binds this socket to an endpoint name. May only be called on an instance
     * that has not yet been bound.
     *
     * @param bindpoint endpoint address
     * @throws IOException
     */
    CARAPI Bind(
        /* [in] */ ILocalSocketAddress* bindpoint);

    /**
     * Retrieves the name that this socket is bound to, if any.
     *
     * @return Local address or null if anonymous
     */
    CARAPI GetLocalSocketAddress(
        /* [out] */ ILocalSocketAddress** bindpoint);

    /**
     * Retrieves the input stream for this instance.
     *
     * @return input stream
     * @throws IOException if socket has been closed or cannot be created.
     */
    CARAPI GetInputStream(
        /* [out] */ IInputStream** inStream);

    /**
     * Retrieves the output stream for this instance.
     *
     * @return output stream
     * @throws IOException if socket has been closed or cannot be created.
     */
    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** outStream);

    /**
     * Closes the socket.
     *
     * @throws IOException
     */
    CARAPI Close();

    /**
     * Shuts down the input side of the socket.
     *
     * @throws IOException
     */
    CARAPI ShutdownInput();

    /**
     * Shuts down the output side of the socket.
     *
     * @throws IOException
     */
    CARAPI ShutdownOutput();

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 n);

    CARAPI GetSoTimeout(
        /* [out] */ Int32* n);

    CARAPI SetSendBufferSize(
        /* [in] */ Int32 n);

    CARAPI GetSendBufferSize(
        /* [out] */ Int32* n);

    CARAPI GetRemoteSocketAddress(
        /* [out] */ ILocalSocketAddress** bindpoint);

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    CARAPI IsOutputShutdown(
        /* [out] */ Boolean* isOutputShutdown);

    CARAPI IsInputShutdown(
        /* [out] */ Boolean* isInputShutdown);

    CARAPI ConnectEx(
        /* [in] */ ILocalSocketAddress* endpoint,
        /* [in] */ Int32 timeout);

    /**
     * Enqueues a set of file descriptors to send to the peer. The queue
     * is one deep. The file descriptors will be sent with the next write
     * of normal data, and will be delivered in a single ancillary message.
     * See "man 7 unix" SCM_RIGHTS on a desktop Linux machine.
     *
     * @param fds non-null; file descriptors to send.
     */
    CARAPI SetFileDescriptorsForSend(
        /* [in] */ const ArrayOf<IFileDescriptor*>& fds);

    /**
     * Retrieves a set of file descriptors that a peer has sent through
     * an ancillary message. This method retrieves the most recent set sent,
     * and then returns null until a new set arrives.
     * File descriptors may only be passed along with regular data, so this
     * method can only return a non-null after a read operation.
     *
     * @return null or file descriptor array
     * @throws IOException
     */
    CARAPI GetAncillaryFileDescriptors(
        /* [out, callee] */ ArrayOf<IFileDescriptor*>** fds);

    /**
     * Retrieves the credentials of this socket's peer. Only valid on
     * connected sockets.
     *
     * @return non-null; peer credentials
     * @throws IOException
     */
    CARAPI GetPeerCredentials(
        /* [out] */ ICredentials** creds);

    /**
     * Returns file descriptor or null if not yet open/already closed
     *
     * @return fd or null
     */
    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fd);

    /**
     * Creates a AF_LOCAL/UNIX domain stream socket.
     */
    CARAPI constructor();

    /**
     * for use with AndroidServerSocket
     * @param impl a SocketImpl
     *
     * package
     */
    CARAPI constructor(
        /* [in] */ Handle32 socketImpl);
};

#endif //__CLOCALSOCKET_H__
