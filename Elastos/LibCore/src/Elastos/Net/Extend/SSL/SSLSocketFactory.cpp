#include "SSLSocketFactory.h"

ISocketFactory* SSLSocketFactory::mDefaultSocketFactory;
	
String SSLSocketFactory::mDefaultName;
	
ECode SSLSocketFactory::GetDefault(
	/* [out] */ ISocketFactory** factory)
{
}

SSLSocketFactory::SSLSocketFactory()
{
}