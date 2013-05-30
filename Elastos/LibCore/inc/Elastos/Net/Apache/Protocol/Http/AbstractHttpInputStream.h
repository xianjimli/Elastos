#ifndef __ABSTRACTHTTPINPUTSTREAM_H__
#define __ABSTRACTHTTPINPUTSTREAM_H__

#include "Elastos.IO.h"
#include "CHttpURLConnectionImpl.h"

//using namespace Elastos;
class AbstractHttpInputStream
{
public:
    AbstractHttpInputStream(
        /* [in] */ IInputStream* in,
        /* [in] */ IHttpURLConnectionImpl* httpURLConnection,
        /* [in] */ ICacheRequest* cacheRequest);

    AbstractHttpInputStream(){};

    ECode Read(
        /* [out] */ Int32* value);

    ECode Skip(
        /* [in] */ Int64 n,
        /* [in] */ Int64* value);

protected:
    ECode CheckBounds(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    ECode CheckNotClosed();

    ECode CacheWrite(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    ECode EndOfInput(
        /* [in] */ Boolean reuseSocket);

    ECode UnexpectedEndOfInput();

protected:
    AutoPtr<IInputStream> mIn;
    AutoPtr<IHttpURLConnectionImpl> mHttpURLConnection;
    AutoPtr<ICacheRequest> mCacheRequest;
    AutoPtr<IOutputStream> cacheOut;
    Boolean mClosed;

private:
    ArrayOf<Byte>* mSkipBuffer;
};


#endif //__ABSTRACTHTTPINPUTSTREAM_H__