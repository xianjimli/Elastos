
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

    ~CDatagramPacket();

    /**
     * Constructs a new {@code DatagramPacket} object to receive data up to
     * {@code length} bytes.
     *
     * @param data
     *            a byte array to store the read characters.
     * @param length
     *            the length of the data buffer.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length);

    /**
     * Constructs a new {@code DatagramPacket} object to receive data up to
     * {@code length} bytes with a specified buffer offset.
     *
     * @param data
     *            a byte array to store the read characters.
     * @param offset
     *            the offset of the byte array where the bytes is written.
     * @param length
     *            the length of the data.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    /**
     * Constructs a new {@code DatagramPacket} object to send data to the port
     * {@code aPort} of the address {@code host}. The {@code length} must be
     * lesser than or equal to the size of {@code data}. The first {@code
     * length} bytes from the byte array position {@code offset} are sent.
     *
     * @param data
     *            a byte array which stores the characters to be sent.
     * @param offset
     *            the offset of {@code data} where to read from.
     * @param length
     *            the length of data.
     * @param host
     *            the address of the target host.
     * @param aPort
     *            the port of the target host.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ IInetAddress* host,
        /* [in] */ Int32 aPort);

    /**
     * Constructs a new {@code DatagramPacket} object to send data to the port
     * {@code aPort} of the address {@code host}. The {@code length} must be
     * lesser than or equal to the size of {@code data}. The first {@code
     * length} bytes are sent.
     *
     * @param data
     *            a byte array which stores the characters to be sent.
     * @param length
     *            the length of data.
     * @param host
     *            the address of the target host.
     * @param port
     *            the port of the target host.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length,
        /* [in] */ IInetAddress* host,
        /* [in] */ Int32 port);

    /**
     * Constructs a new {@code DatagramPacket} object to send data to the
     * address {@code sockAddr}. The {@code length} must be lesser than or equal
     * to the size of {@code data}. The first {@code length} bytes of the data
     * are sent.
     *
     * @param data
     *            the byte array to store the data.
     * @param length
     *            the length of the data.
     * @param sockAddr
     *            the target host address and port.
     * @throws SocketException
     *             if an error in the underlying protocol occurs.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length,
        /* [in] */ ISocketAddress* sockAddr);

    /**
     * Constructs a new {@code DatagramPacket} object to send data to the
     * address {@code sockAddr}. The {@code length} must be lesser than or equal
     * to the size of {@code data}. The first {@code length} bytes of the data
     * are sent.
     *
     * @param data
     *            the byte array to store the data.
     * @param offset
     *            the offset of the data.
     * @param length
     *            the length of the data.
     * @param sockAddr
     *            the target host address and port.
     * @throws SocketException
     *             if an error in the underlying protocol occurs.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ ISocketAddress* sockAddr);

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
        /* [in] */ ArrayOf<Byte>* buf);

    /**
     * Gets the current capacity value.
     *
     * @return the current capacity value
     */
    CARAPI_(Int32) GetCapacity();

    CARAPI SetDataEx(
        /* [in] */ ArrayOf<Byte>* buf,
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
};

#endif //__CDATAGRAMPACKET_H__
