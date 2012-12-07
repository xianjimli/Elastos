
#include "cmdef.h"
#include "CDatagramPacket.h"
#include "CInetSocketAddress.h"

CDatagramPacket::CDatagramPacket()
    : mLength(0)
    , mCapacity(0)
    , mPort(-1)
    , mOffset(0)
{
}

ECode CDatagramPacket::GetAddress(
    /* [out] */ IInetAddress** address)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(address);
    *address = mAddress;

    return NOERROR;
}

ECode CDatagramPacket::GetData(
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(data);
    *data = mData;

    return NOERROR;
}

ECode CDatagramPacket::GetLength(
    /* [out] */ Int32* length)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(length);
    *length = mLength;

    return NOERROR;
}

ECode CDatagramPacket::GetOffset(
    /* [out] */ Int32* offset)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;

    return NOERROR;
}

ECode CDatagramPacket::GetPort(
    /* [out] */ Int32* port)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(port);
    *port = mPort;

    return NOERROR;
}

ECode CDatagramPacket::SetAddress(
    /* [in] */ IInetAddress* addr)
{
    Mutex::Autolock lock(mLock);
    mAddress = addr;
    return NOERROR;
}

ECode CDatagramPacket::SetData(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    Mutex::Autolock lock(mLock);

    mLength = buf.GetLength(); // This will check for null
    mCapacity = buf.GetLength();
    mData->Copy(&buf);
    mOffset = 0;

    return NOERROR;
}

Int32 CDatagramPacket::GetCapacity()
{
    Mutex::Autolock lock(mLock);

    return mCapacity;
}

ECode CDatagramPacket::SetDataEx(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 anOffset,
    /* [in] */ Int32 aLength)
{
    Mutex::Autolock lock(mLock);

    if (0 > anOffset || anOffset > buf.GetLength() || 0 > aLength
            || aLength > buf.GetLength() - anOffset) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    mData->Copy(&buf);
    mOffset = anOffset;
    mLength = aLength;
    mCapacity = aLength;

    return NOERROR;
}

ECode CDatagramPacket::SetLength(
    /* [in] */ Int32 len)
{
    Mutex::Autolock lock(mLock);

    if (0 > len || mOffset + len > mData->GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    mLength = len;
    mCapacity = len;

    return NOERROR;
}

ECode CDatagramPacket::SetLengthOnly(
    /* [in] */ Int32 len)
{
    Mutex::Autolock lock(mLock);

    if (0 > len || mOffset + len > mData->GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    mLength = len;

    return NOERROR;
}

ECode CDatagramPacket::SetPort(
    /* [in] */ Int32 aPort)
{
    Mutex::Autolock lock(mLock);

    if (aPort < 0 || aPort > 65535) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Port out of range: " + aPort);
    }
    mPort = aPort;

    return NOERROR;
}

ECode CDatagramPacket::GetSocketAddress(
    /* [out] */ ISocketAddress** sockAddr)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(sockAddr);

    AutoPtr<IInetAddress> addr;
    GetAddress((IInetAddress**)&addr);
    Int32 port;
    GetPort(&port);
    AutoPtr<IInetSocketAddress> sa;
    ASSERT_SUCCEEDED(CInetSocketAddress::New(addr, port, (IInetSocketAddress**)&sa));
    if (sa != NULL && sa->Probe(EIID_IInetSocketAddress) != NULL) {
        *sockAddr = (ISocketAddress*)sa->Probe(EIID_ISocketAddress);
    }
    else {
        *sockAddr = NULL;
    }

    return NOERROR;
}

ECode CDatagramPacket::SetSocketAddress(
    /* [in] */ ISocketAddress* sockAddr)
{
    Mutex::Autolock lock(mLock);

    assert(sockAddr);
    if (sockAddr->Probe(EIID_IInetSocketAddress) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Socket address not an InetSocketAddress: " +
//                (sockAddr == null ? null : sockAddr.getClass()));
    }
    AutoPtr<IInetSocketAddress> inetAddr =
            (IInetSocketAddress*)sockAddr->Probe(EIID_IInetSocketAddress);
    inetAddr->GetPort(&mPort);
    inetAddr->GetAddress((IInetAddress**)&mAddress);

    return NOERROR;
}

ECode CDatagramPacket::constructor(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 length)
{
    return constructor(data, 0, length);;
}

ECode CDatagramPacket::constructor(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    //super();
    return SetDataEx(data, offset, length);
}

ECode CDatagramPacket::constructor(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 aPort)
{
    constructor(data, offset, length);
    SetPort(aPort);
    mAddress = host;

    return NOERROR;
}

ECode CDatagramPacket::constructor(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 length,
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 port)
{
    return constructor(data, 0, length, host, port);
}

ECode CDatagramPacket::constructor(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 length,
    /* [in] */ ISocketAddress* sockAddr)
{
    constructor(data, 0, length);
    return SetSocketAddress(sockAddr);
}

ECode CDatagramPacket::constructor(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ ISocketAddress* sockAddr)
{
    constructor(data, offset, length);
    return SetSocketAddress(sockAddr);
}
