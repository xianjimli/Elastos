#include "HttpConnectionPool.h"


HttpConnectionPool* HttpConnectionPool::INSTANCE;

ECode HttpConnectionPool::Get(
    /* [in] */ HttpConnection::Address* address,
    /* [in] */ Int32 timeout,
    /* [out] */ HttpConnection** connection)
{
    return NOERROR;
}

ECode HttpConnectionPool::Recycle(
    /* [in] */ HttpConnection* connection)
{
    return NOERROR;
}