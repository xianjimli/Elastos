
#ifndef __CDEFAULTSOCKETFACTORY_H__
#define __CDEFAULTSOCKETFACTORY_H__

#include "_CDefaultSocketFactory.h"

CarClass(CDefaultSocketFactory)
{
public:
    CARAPI CreateSocket(
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx2(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress * pLocalHost,
        /* [in] */ Int32 localPort,
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx3(
        /* [in] */ IInetAddress * pHost,
        /* [in] */ Int32 port,
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx4(
        /* [in] */ IInetAddress * pAddress,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress * pLocalAddress,
        /* [in] */ Int32 localPort,
        /* [out] */ ISocket ** ppSocket);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CDEFAULTSOCKETFACTORY_H__
