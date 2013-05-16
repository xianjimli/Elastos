
#ifndef __CDEFAULTSSLSERVERSOCKETFACTORY_H__
#define __CDEFAULTSSLSERVERSOCKETFACTORY_H__

#include "_CDefaultSSLServerSocketFactory.h"

CarClass(CDefaultSSLServerSocketFactory)
{
public:
    CARAPI CreateServerSocket(
        /* [out] */ IServerSocket ** ppServerSocket);

    CARAPI CreateServerSocketEx(
        /* [in] */ Int32 port,
        /* [out] */ IServerSocket ** ppServerSocket);

    CARAPI CreateServerSocketEx2(
        /* [in] */ Int32 port,
        /* [in] */ Int32 backlog,
        /* [out] */ IServerSocket ** ppServerSocket);

    CARAPI CreateServerSocketEx3(
        /* [in] */ Int32 port,
        /* [in] */ Int32 backlog,
        /* [in] */ IInetAddress * pIAddress,
        /* [out] */ IServerSocket ** ppServerSocket);

    CARAPI constructor(
        /* [in] */ const String& mes);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDEFAULTSSLSERVERSOCKETFACTORY_H__
