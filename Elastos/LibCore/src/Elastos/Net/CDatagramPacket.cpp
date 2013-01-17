
#include "cmdef.h"
#include "CDatagramPacket.h"
#include "CInetSocketAddress.h"


CDatagramPacket::CDatagramPacket()
    : mData(NULL)
    , mLength(0)
    , mCapacity(0)
    , mPort(-1)
    , mOffset(0)
{}

CDatagramPacket::~CDatagramPacket()
{
    if (mData != NULL) ArrayOf<Byte>::Free(mData);
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length)
{
    return constructor(data, 0, length);;
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return SetDataEx(data, offset, length);
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 aPort)
{
    FAIL_RETURN(constructor(data, offset, length));
    SetPort(aPort);
    mAddress = host;

    return NOERROR;
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length,
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 port)
{
    return constructor(data, 0, length, host, port);
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length,
    /* [in] */ ISocketAddress* sockAddr)
{
    FAIL_RETURN(constructor(data, 0, length));
    return SetSocketAddress(sockAddr);
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ ISocketAddress* sockAddr)
{
    FAIL_RETURN(constructor(data, offset, length));
    return SetSocketAddress(sockAddr);
}

ECode CDatagramPacket::GetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Mutex::Autolock lock(_m_syncLock);

    *address = mAddress;
    if (*address != NULL) (*address)->AddRef();

    return NOERROR;
}

ECode CDatagramPacket::GetData(
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);

    Mutex::Autolock lock(_m_syncLock);

    *data = mData == NULL ? NULL : mData->Clone();

    return NOERROR;
}

ECode CDatagramPacket::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);

    Mutex::Autolock lock(_m_syncLock);

    *length = mLength;

    return NOERROR;
}

ECode CDatagramPacket::GetOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    Mutex::Autolock lock(_m_syncLock);

    *offset = mOffset;

    return NOERROR;
}

ECode CDatagramPacket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    Mutex::Autolock lock(_m_syncLock);

    *port = mPort;

    return NOERROR;
}

ECode CDatagramPacket::SetAddress(
    /* [in] */ IInetAddress* addr)
{
    Mutex::Autolock lock(_m_syncLock);
    mAddress = addr;

    return NOERROR;
}

ECode CDatagramPacket::SetDataEx(
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 anOffset,
    /* [in] */ Int32 aLength)
{
    Mutex::Autolock lock(_m_syncLock);

    if (0 > anOffset || anOffset > buf->GetLength() || 0 > aLength
            || aLength > buf->GetLength() - anOffset) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mData != NULL) ArrayOf<Byte>::Free(mData);
    mData = buf;
    mOffset = anOffset;
    mLength = aLength;
    mCapacity = aLength;

    return NOERROR;
}

ECode CDatagramPacket::SetData(
    /* [in] */ ArrayOf<Byte>* buf)
{
    Mutex::Autolock lock(_m_syncLock);

    mLength = buf->GetLength(); // This will check for null
    mCapacity = buf->GetLength();
    if (mData != NULL) ArrayOf<Byte>::Free(mData);
    mData = buf;
    mOffset = 0;

    return NOERROR;
}

Int32 CDatagramPacket::GetCapacity()
{
    Mutex::Autolock lock(_m_syncLock);

    return mCapacity;
}

ECode CDatagramPacket::SetLength(
    /* [in] */ Int32 len)
{
    Mutex::Autolock lock(_m_syncLock);

    if (0 > len || mOffset + len > mData->GetLength()) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mLength = len;
    mCapacity = len;

    return NOERROR;
}

ECode CDatagramPacket::SetLengthOnly(
    /* [in] */ Int32 len)
{
    Mutex::Autolock lock(_m_syncLock);

    if (0 > len || mOffset + len > mData->GetLength()) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mLength = len;

    return NOERROR;
}

ECode CDatagramPacket::SetPort(
    /* [in] */ Int32 aPort)
{
    Mutex::Autolock lock(_m_syncLock);

    if (aPort < 0 || aPort > 65535) {
//        throw new IllegalArgumentException("Port out of range: " + aPort);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mPort = aPort;

    return NOERROR;
}

ECode CDatagramPacket::GetSocketAddress(
    /* [out] */ ISocketAddress** sockAddr)
{
    VALIDATE_NOT_NULL(sockAddr);

    Mutex::Autolock lock(_m_syncLock);

    AutoPtr<IInetAddress> addr;
    GetAddress((IInetAddress**)&addr);
    Int32 port;
    GetPort(&port);
    AutoPtr<IInetSocketAddress> sa;
    FAIL_RETURN(CInetSocketAddress::New(addr, port, (IInetSocketAddress**)&sa));
    *sockAddr = ISocketAddress::Probe(sa);
    (*sockAddr)->AddRef();

    return NOERROR;
}

ECode CDatagramPacket::SetSocketAddress(
    /* [in] */ ISocketAddress* sockAddr)
{
    Mutex::Autolock lock(_m_syncLock);

    if (sockAddr == NULL || IInetSocketAddress::Probe(sockAddr) == NULL) {
//        throw new IllegalArgumentException("Socket address not an InetSocketAddress: " +
//                (sockAddr == null ? null : sockAddr.getClass()));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    IInetSocketAddress* inetAddr = IInetSocketAddress::Probe(sockAddr);
    inetAddr->GetPort(&mPort);
    mAddress = NULL;
    inetAddr->GetAddress((IInetAddress**)&mAddress);

    return NOERROR;
}
