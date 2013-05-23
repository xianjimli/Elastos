#ifndef __HTTPCONNECTIONPOOL_H__
#define __HTTPCONNECTIONPOOL_H__

#include "HttpConnection.h"

class HttpConnectionPool
{
public:
    ECode Get(
        /* [in] */ HttpConnection::Address* address,
        /* [in] */ Int32 timeout,
        /* [out] */ HttpConnection** connection);

    ECode Recycle(
        /* [in] */ HttpConnection* connection);

public:
    static HttpConnectionPool* INSTANCE;


};

#endif //__HTTPCONNECTIONPOOL_H__