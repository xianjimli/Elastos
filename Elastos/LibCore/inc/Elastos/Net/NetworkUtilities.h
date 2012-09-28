
#ifndef __NETWORKUTILITIES_H__
#define __NETWORKUTILITIES_H__

#include "Elastos.Net_server.h"
#include <arpa/inet.h>

using namespace Elastos;

ELAPI ByteArrayToSocketAddress(
    /* [in] */ const ArrayOf<Byte>& byteArray,
    /* [in]*/ Int32 port,
    /* [out] */ sockaddr_storage* ss);


ELAPI SocketAddressToByteArray(
    /* [in] */ sockaddr_storage* ss,
    /* [out] */ ArrayOf<Byte>* ipAddress);


#endif //__NETWORKUTILITIES_H__

