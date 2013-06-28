#ifndef __SSLSOCKETFACTORY_H__
#define __SSLSOCKETFACTORY_H__


class SSLSocketFactory : public SocketFactory
{
public:
	static ECode GetDefault(
		/* [out] */ ISocketFactory** factory);
	
	SSLSocketFactory();
	
	virtual ECode GetDefaultCipherSuites(
		/* [out, callee] */ ArrayOf<String>** array) = 0;

    /**
     * Returns the names of the cipher suites that are supported and could be
     * enabled for an SSL connection.
     *
     * @return the names of the cipher suites that are supported.
     */
    virtual ECode GetSupportedCipherSuites(
    	/* [out, callee] */ ArrayOf<String>** array) = 0;

    /**
     * Creates an {@code SSLSocket} over the specified socket that is connected
     * to the specified host at the specified port.
     *
     * @param s
     *            the socket.
     * @param host
     *            the host.
     * @param port
     *            the port number.
     * @param autoClose
     *            {@code true} if socket {@code s} should be closed when the
     *            created socket is closed, {@code false} if the socket
     *            {@code s} should be left open.
     * @return the creates ssl socket.
     * @throws IOException
     *             if creating the socket fails.
     * @throws java.net.UnknownHostException
     *             if the host is unknown.
     */
    virtual ECode CreateSocket(
    	/* [in] */ ISocket* s, 
    	/* [in] */ const String& host, 
    	/* [in] */ Int32 port, 
    	/* [in] */ Boolean autoClose,
    	/* [in] */ ISocket** ppSocket) = 0;

private:
	static ISocketFactory* mDefaultSocketFactory;
	
	static String mDefaultName;
};

#endif //__SSLSOCKETFACTORY_H__