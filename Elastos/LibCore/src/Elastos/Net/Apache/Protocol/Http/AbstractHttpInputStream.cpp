#include "AbstractHttpInputStream.h"

AbstractHttpInputStream::AbstractHttpInputStream(
    /* [in] */ IInputStream* in,
    /* [in] */ IHttpURLConnectionImpl* httpURLConnection,
    /* [in] */ ICacheRequest* cacheRequest)
{
}


ECode AbstractHttpInputStream::Read(
    /* [out] */ Int32* value)
{
}

ECode AbstractHttpInputStream::Skip(
    /* [in] */ Int64 n,
    /* [in] */ Int64* value)
{

}

ECode AbstractHttpInputStream::CheckBounds(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
}

ECode AbstractHttpInputStream::CheckNotClosed()
{
}

ECode AbstractHttpInputStream::CacheWrite(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
}

ECode AbstractHttpInputStream::EndOfInput(
    /* [in] */ Boolean reuseSocket)
{
}

ECode AbstractHttpInputStream::UnexpectedEndOfInput()
{
}