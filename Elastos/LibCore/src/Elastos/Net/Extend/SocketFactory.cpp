#include "SocketFactory.h"

ECode SocketFactory::GetDefault(
	/* [out] */ ISocketFactory** factory)
{
    if (mDefaultFactory == NULL) {
    	CDefaultSocketFactory *fac;
    	CDefaultSocketFactory::NewByFriend(&fac);
        mDefaultFactory = (ISocketFactory*)fac;
    }
    
    *factory = mDefaultFactory;
    return NOERROR;	
}

SocketFactory::SocketFactory()
{
}

ECode SocketFactory::CreateSocket(
	/* [out] */ ISocket** ppSocket)
{
	return E_SOCKET_EXCEPTION;
}