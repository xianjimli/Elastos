
#ifndef __SOCKETIMPL_H__
#define __SOCKETIMPL_H__

#include "Elastos.Net_server.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>


/**
 * This class is the base of all streaming socket implementation classes.
 * Streaming sockets are wrapped by two classes, {@code ServerSocket} and
 * {@code Socket} at the server and client end of a connection. At the server,
 * there are two types of sockets engaged in communication, the {@code
 * ServerSocket} on a well known port (referred to as listener) used to
 * establish a connection and the resulting {@code Socket} (referred to as
 * host).
 */
class SocketImpl
{
public:
    SocketImpl();

    /**
     * Waits for an incoming request and blocks until the connection is opened
     * on the given socket.
     *
     * @param newSocket
     *            the socket to accept connections on.
     * @throws IOException
     *             if an error occurs while accepting a new connection.
     */
    virtual CARAPI Accept(
        /* [in] */ ISocketImpl* newSocket) = 0;

    /**
     * Returns the available number of bytes which are readable from this socket
     * without blocking.
     *
     * @return the number of bytes that may be read without blocking.
     * @throws IOException
     *             if an error occurs while reading the number of bytes.
     */
    virtual CARAPI Available(
        /* [out] */ Int32* num) = 0;

    /**
     * Binds this socket to the specified local host address and port number.
     *
     * @param address
     *            the local machine address to bind this socket to.
     * @param port
     *            the port on the local machine to bind this socket to.
     * @throws IOException
     *             if an error occurs while binding this socket.
     */
    virtual CARAPI Bind(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port) = 0;

    /**
     * Closes this socket. This makes later access invalid.
     *
     * @throws IOException
     *             if an error occurs while closing this socket.
     */
    virtual CARAPI Close() = 0;

    /**
     * Connects this socket to the specified remote host and port number.
     *
     * @param host
     *            the remote host this socket has to be connected to.
     * @param port
     *            the remote port on which this socket has to be connected.
     * @throws IOException
     *             if an error occurs while connecting to the remote host.
     */
    virtual CARAPI Connect(
        /* [in] */ const String& host,
        /* [in] */ Int32 port) = 0;

    /**
     * Connects this socket to the specified remote host address and port
     * number.
     *
     * @param address
     *            the remote host address this socket has to be connected to.
     * @param port
     *            the remote port on which this socket has to be connected.
     * @throws IOException
     *             if an error occurs while connecting to the remote host.
     */
    virtual CARAPI ConnectEx(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port) = 0;

    /**
     * Creates a new unconnected socket. The argument {@code isStreaming}
     * defines whether the new socket is a streaming or a datagram socket.
     *
     * @param isStreaming
     *            defines whether the type of the new socket is streaming or
     *            datagram.
     * @throws IOException
     *             if an error occurs while creating the socket.
     */
    virtual CARAPI Create(
        /* [in] */ Boolean isStreaming) = 0;

    /**
     * Gets the file descriptor of this socket.
     *
     * @return the file descriptor of this socket.
     */
    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fileDescriptor);

    /**
     * Gets the remote address this socket is connected to.
     *
     * @return the remote address of this socket.
     */
    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** netAddress);

    /**
     * Gets the input stream of this socket.
     *
     * @return the input stream of this socket.
     * @throws IOException
     *             if an error occurs while accessing the input stream.
     */
    virtual CARAPI GetInputStream(
        /* [out] */ IInputStream** inputStream) = 0;

    /**
     * Gets the local port number of this socket. The field is initialized to
     * {@code -1} and upon demand will go to the IP stack to get the bound
     * value. See the class comment for the context of the local port.
     *
     * @return the local port number this socket is bound to.
     */
    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* num);

    /**
     * Gets the output stream of this socket.
     *
     * @return the output stream of this socket.
     * @throws IOException
     *             if an error occurs while accessing the output stream.
     */
    virtual CARAPI GetOutputStream(
        /* [out] */ IOutputStream** outputStream) = 0;

    /**
     * Gets the remote port number of this socket. This value is not meaningful
     * when this instance is wrapped by a {@code ServerSocket}.
     *
     * @return the remote port this socket is connected to.
     */
    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

    /**
     * Listens for connection requests on this streaming socket. Incoming
     * connection requests are queued up to the limit specified by {@code
     * backlog}. Additional requests are rejected. The method {@code listen()}
     * may only be invoked on streaming sockets.
     *
     * @param backlog
     *            the maximum number of outstanding connection requests.
     * @throws IOException
     *             if an error occurs while listening.
     */
    virtual CARAPI Listen(
        /* [in] */ Int32 backlog) = 0;

    /**
     * In the IP stack, write at most {@code count} bytes on the socket
     * from the {@code buffer}, from the {@code offset}.
     *
     * @param buffer
     *            the buffer to read into
     * @param offset
     *            the offset into the buffer
     * @param count
     *            the number of bytes to write
     * @return int the actual number of bytes written
     * @throws IOException
     *                thrown if an error occurs while writing
     */
    /* package */ virtual CARAPI Write(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    /**
     * Closes the input channel of this socket.
     * <p>
     * This default implementation always throws an {@link IOException} to
     * indicate that the subclass should have overridden this method.
     *
     * @throws IOException
     *             always because this method should be overridden.
     */
    virtual CARAPI ShutdownInput();

    /**
     * Closes the output channel of this socket.
     * <p>
     * This default implementation always throws an {@link IOException} to
     * indicate that the subclass should have overridden this method.
     *
     * @throws IOException
     *             always because this method should be overridden.
     */
    virtual CARAPI ShutdownOutput();

    /**
     * Connects this socket to the remote host address and port number specified
     * by the {@code SocketAddress} object with the given timeout. This method
     * will block indefinitely if the timeout is set to zero.
     *
     * @param remoteAddr
     *            the remote host address and port number to connect to.
     * @param timeout
     *            the timeout value in milliseconds.
     * @throws IOException
     *             if an error occurs while connecting.
     */
    virtual CARAPI ConnectEx2(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout) = 0;

    /**
     * Returns whether the socket supports urgent data or not. Subclasses should
     * override this method.
     *
     * @return {@code false} because subclasses must override this method.
     */
    virtual CARAPI SupportsUrgentData(
        /* [out] */ Boolean* isSupports);

    /**
     * Sends the single byte of urgent data on the socket.
     *
     * @param value
     *            the byte of urgent data.
     * @throws IOException
     *             if an error occurs sending urgent data.
     */
    virtual CARAPI SendUrgentData(
        /* [in] */ Int32 value) = 0;

    /**
     * Sets performance preference for connection time, latency and bandwidth.
     * Does nothing by default.
     *
     * @param connectionTime
     *            the importance of connect time.
     * @param latency
     *            the importance of latency.
     * @param bandwidth
     *            the importance of bandwidth.
     */
    virtual CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth) = 0;

protected:
    /**
     * The remote address this socket is connected to.
     */
    AutoPtr<IInetAddress> mAddress;

    /**
     * The remote port this socket is connected to.
     */
    Int32 mPort;

    /**
     * The file descriptor of this socket.
     */
    AutoPtr<IFileDescriptor> mFd;

    /**
     * The local port this socket is connected to.
     */
    Int32 mLocalport;

public:
    AutoPtr<INetworkSystem> mNetImpl;

    Boolean mStreaming;
};

#endif //__SOCKETIMPL_H__
