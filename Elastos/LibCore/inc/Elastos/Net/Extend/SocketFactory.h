#ifndef __SOCKETFACTORY_H__
#define __SOCKETFACTORY_H__

#include <elastos.h>
#include "CDefaultSocketFactory.h"
#include <elastos/AutoPtr.h>
//#include "Elastos.Net.h"

using namespace Elastos;

class SocketFactory
{
public:
	static ECode GetDefault(
		/* [out] */ ISocketFactory** factory);


	ECode CreateSocket(
		/* [out] */ ISocket** ppSocket);

	virtual ECode CreateSocket(
		/* [in] */ const String& host, 
		/* [in] */ Int32 port,
		/* [out] */ ISocket** ppSocket)  = 0;		
	
	virtual ECode CreateSocket(
		/* [in] */ const String& host, 
		/* [in] */ Int32 port, 
		/* [in] */ IInetAddress* localHost, 
		/* [in] */ Int32 localPort,
		/* [out] */ ISocket** ppSocket)	= 0;
	
	virtual ECode createSocket(
		/* [in] */ IInetAddress* host, 
		/* [in] */ Int32 port,
		/* [out] */ ISocket** ppSocket) = 0;
	
	virtual ECode CreateSocket(
		/* [in] */ IInetAddress* address, 
		/* [in] */ Int32 port, 
		/* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort,
        /* [out] */ ISocket** ppSocket) = 0;	
	  		
protected:
	SocketFactory();

private:
	static AutoPtr<ISocketFactory> mDefaultFactory;	
	
};

#endif //__SSLSOCKETFACTORY_H__