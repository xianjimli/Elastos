#ifndef __ServerSOCKETFACTORY_H__
#define __ServerSOCKETFACTORY_H__

#include <elastos.h>
#include "CDefaultServerSocketFactory.h"
#include <elastos/AutoPtr.h>

class ServerSocketFactory
{
private:
	static AutoPtr<IServerSocketFactory> mDefaultFactory;

protected:
	ServerSocketFactory();
	
public:
	static ECode GetDefault(
		/* [out] */ IServerSocketFactory** ppServerSocketFactory);
	 
	ECode CreateServerSocket();       


    virtual ECode CreateServerSocket(
    	/* [in] */ Int32 port,
    	/* [out] */ IServerSocket** ppServerSocket) = 0;

    virtual ECode CreateServerSocket(
    	/* [in] */ Int32 port, 
    	/* [in] */ Int32 backlog,
    	/* [out] */ IServerSocket** ppServerSocket) = 0;
    	
    virtual ECode CreateServerSocket(
    	/* [in] */ Int32 port, 
    	/* [in] */ Int32 backlog, 
    	/* [in] */ IInetAddress* iAddress,
    	/* [out] */ IServerSocket** ppServerSocket) = 0;
};

#endif // __ServerSOCKETFACTORY_H__
