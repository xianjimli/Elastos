
#include "net/CLocalSocket.h"

/**
 * Connects this socket to an endpoint. May only be called on an instance
 * that has not yet been connected.
 *
 * @param endpoint endpoint address
 * @throws IOException if socket is in invalid state or the address does
 * not exist.
 */
ECode CLocalSocket::Connect(
    /* [in] */ ILocalSocketAddress* endpoint)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Binds this socket to an endpoint name. May only be called on an instance
 * that has not yet been bound.
 *
 * @param bindpoint endpoint address
 * @throws IOException
 */
ECode CLocalSocket::Bind(
    /* [in] */ ILocalSocketAddress* bindpoint)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Retrieves the name that this socket is bound to, if any.
 *
 * @return Local address or null if anonymous
 */
ECode CLocalSocket::GetLocalSocketAddress(
    /* [out] */ ILocalSocketAddress** bindpoint)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Retrieves the input stream for this instance.
 *
 * @return input stream
 * @throws IOException if socket has been closed or cannot be created.
 */
ECode CLocalSocket::GetInputStream(
    /* [out] */ IInputStream** inStream)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Retrieves the output stream for this instance.
 *
 * @return output stream
 * @throws IOException if socket has been closed or cannot be created.
 */
ECode CLocalSocket::GetOutputStream(
    /* [out] */ IOutputStream** outStream)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Closes the socket.
 *
 * @throws IOException
 */
ECode CLocalSocket::Close()
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Shuts down the input side of the socket.
 *
 * @throws IOException
 */
ECode CLocalSocket::ShutdownInput()
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Shuts down the output side of the socket.
 *
 * @throws IOException
 */
ECode CLocalSocket::ShutdownOutput()
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::SetSoTimeout(
    /* [in] */ Int32 n)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::GetSoTimeout(
    /* [out] */ Int32* n)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::SetSendBufferSize(
    /* [in] */ Int32 n)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::GetSendBufferSize(
    /* [out] */ Int32* n)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::GetRemoteSocketAddress(
    /* [out] */ ILocalSocketAddress** bindpoint)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::IsOutputShutdown(
    /* [out] */ Boolean* isOutputShutdown)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::IsInputShutdown(
    /* [out] */ Boolean* isInputShutdown)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalSocket::ConnectEx(
    /* [in] */ ILocalSocketAddress* endpoint,
    /* [in] */ Int32 timeout)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Enqueues a set of file descriptors to send to the peer. The queue
 * is one deep. The file descriptors will be sent with the next write
 * of normal data, and will be delivered in a single ancillary message.
 * See "man 7 unix" SCM_RIGHTS on a desktop Linux machine.
 *
 * @param fds non-null; file descriptors to send.
 */
ECode CLocalSocket::SetFileDescriptorsForSend(
    /* [in] */ const ArrayOf<IFileDescriptor*>& fds)
{
    return E_NOT_IMPLEMENTED;
}

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
ECode CLocalSocket::GetAncillaryFileDescriptors(
    /* [out, callee] */ ArrayOf<IFileDescriptor*>** fds)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Retrieves the credentials of this socket's peer. Only valid on
 * connected sockets.
 *
 * @return non-null; peer credentials
 * @throws IOException
 */
ECode CLocalSocket::GetPeerCredentials(
    /* [out] */ ICredentials** creds)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns file descriptor or null if not yet open/already closed
 *
 * @return fd or null
 */
ECode CLocalSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Creates a AF_LOCAL/UNIX domain stream socket.
 */
ECode CLocalSocket::constructor()
{
    return E_NOT_IMPLEMENTED;
}

/**
 * for use with AndroidServerSocket
 * @param impl a SocketImpl
 *
 * package
 */
ECode CLocalSocket::constructor(
    /* [in] */ Handle32 socketImpl)
{
    return E_NOT_IMPLEMENTED;
}
