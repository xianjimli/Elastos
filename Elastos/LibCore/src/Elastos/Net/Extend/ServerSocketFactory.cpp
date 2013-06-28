#include "ServerSocketFactory.h"

AutoPtr<IServerSocketFactory> ServerSocketFactory::mDefaultFactory;


ServerSocketFactory::ServerSocketFactory()
{
}
	
ECode ServerSocketFactory::GetDefault(
		/* [out] */ IServerSocketFactory** ppServerSocketFactory)
{
	if (mDefaultFactory == NULL) {
		CDefaultServerSocketFactory *sfac;
		CDefaultServerSocketFactory::NewByFriend(&sfac);
		mDefaultFactory = (IServerSocketFactory*)sfac;
	}
	
	*ppServerSocketFactory = mDefaultFactory;
	return NOERROR;	
}	 

ECode ServerSocketFactory::CreateServerSocket()
{
	return E_SOCKET_EXCEPTION;
}       