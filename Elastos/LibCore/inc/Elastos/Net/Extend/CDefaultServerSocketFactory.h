
#ifndef __CDEFAULTSERVERSOCKETFACTORY_H__
#define __CDEFAULTSERVERSOCKETFACTORY_H__

#include "_CDefaultServerSocketFactory.h"

CarClass(CDefaultServerSocketFactory)
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

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CDEFAULTSERVERSOCKETFACTORY_H__
