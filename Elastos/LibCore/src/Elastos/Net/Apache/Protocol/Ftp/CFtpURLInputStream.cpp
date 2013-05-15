
#include "CFtpURLInputStream.h"
ECode CFtpURLInputStream::Available(
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return mIs->Available(pNumber);
}

ECode CFtpURLInputStream::Close()
{
    // TODO: Add your code here
    ECode ec = NOERROR;
    ec = mIs->Close();
    if (FAILED(ec)) {
        return ec;
    }

    return mControlSocket->Close();
}

ECode CFtpURLInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    // TODO: Add your code here
    return mIs->Mark(readLimit);;
}

ECode CFtpURLInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    // TODO: Add your code here
    return mIs->IsMarkSupported(pSupported);
}

ECode CFtpURLInputStream::Read(
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return mIs->Read(pValue);;
}

ECode CFtpURLInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return mIs->ReadBufferEx(offset, length, pBuffer, pNumber);
}

ECode CFtpURLInputStream::Reset()
{
    // TODO: Add your code here
    return mIs->Reset();;
}

ECode CFtpURLInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * pNumber)
{
    // TODO: Add your code here
    return mIs->Skip(count, pNumber);
}

ECode CFtpURLInputStream::constructor(
    /* [in] */ IInputStream * pIs,
    /* [in] */ ISocket * pControlSocket)
{
    // TODO: Add your code here
    mIs = pIs;
    mControlSocket = pControlSocket;
}

