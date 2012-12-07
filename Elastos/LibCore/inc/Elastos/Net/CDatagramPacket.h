
#ifndef __CDATAGRAMPACKET_H__
#define __CDATAGRAMPACKET_H__

#include "_CDatagramPacket.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

CarClass(CDatagramPacket)
{
public:
    CDatagramPacket();

    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetData(
        /* [out, callee] */ ArrayOf<Byte>** data);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI SetAddress(
        /* [in] */ IInetAddress* addr);

    CARAPI SetData(
        /* [in] */ const ArrayOf<Byte>& buf);

    /**
     * Gets the current capacity value.
     *
     * @return the current capacity value
     */
    CARAPI_(Int32) GetCapacity();

    CARAPI SetDataEx(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 anOffset,
        /* [in] */ Int32 aLength);

    CARAPI SetLength(
        /* [in] */ Int32 len);

    /**
     * An alternative to {@link #setLength(int)}, that doesn't reset the {@link #capacity}
     * field.
     *
     * @param len the length of this datagram packet
     */
    CARAPI SetLengthOnly(
        /* [in] */ Int32 len);

    CARAPI SetPort(
        /* [in] */ Int32 aPort);

    CARAPI GetSocketAddress(
        /* [out] */ ISocketAddress** sockAddr);

    CARAPI SetSocketAddress(
        /* [in] */ ISocketAddress* sockAddr);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 length);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ IInetAddress* host,
        /* [in] */ Int32 aPort);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 length,
        /* [in] */ IInetAddress* host,
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 length,
        /* [in] */ ISocketAddress* sockAddr);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ ISocketAddress* sockAddr);

public:
    ArrayOf<Byte>* mData;

    /**
     * Length of the data to be sent or size of data that was received via
     * DatagramSocket#receive() method call.
     */
    Int32 mLength;

    /**
     * Size of internal buffer that is used to store received data. Should be
     * greater or equal to "length" field.
     */
    Int32 mCapacity;

    AutoPtr<IInetAddress> mAddress;

    Int32 mPort; // The default port number is -1

    Int32 mOffset;

    Mutex* mLock;
};

#endif //__CDATAGRAMPACKET_H__
