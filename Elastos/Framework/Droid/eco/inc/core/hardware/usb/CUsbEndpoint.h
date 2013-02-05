
#ifndef __CUSBENDPOINT_H__
#define __CUSBENDPOINT_H__

#include "ext/frameworkext.h"
#include "_CUsbEndpoint.h"

/**
 * A class representing an endpoint on a {@link UsbInterface}.
 * Endpoints are the channels for sending and receiving data over USB.
 * Typically bulk endpoints are used for sending non-trivial amounts of data.
 * Interrupt endpoints are used for sending small amounts of data, typically events,
 * separately from the main data streams.
 * The endpoint zero is a special endpoint for control messages sent from the host
 * to device.
 * Isochronous endpoints are currently unsupported.
 */
CarClass(CUsbEndpoint)
{
public:
    /**
     * UsbEndpoint should only be instantiated by UsbService implementation
     * @hide
     */
    CARAPI constructor(
        /* [in] */ Int32 address,
        /* [in] */ Int32 attributes,
        /* [in] */ Int32 size,
        /* [in] */ Int32 interval);

    /**
     * Returns the endpoint's address field.
     * The address is a bitfield containing both the endpoint number
     * as well as the data direction of the endpoint.
     * the endpoint number and direction can also be accessed via
     * {@link #getEndpointNumber} and {@link #getDirection}.
     *
     * @return the endpoint's address
     */
    CARAPI GetAddress(
        /* [out] */ Int32* address);

    /**
     * Extracts the endpoint's endpoint number from its address
     *
     * @return the endpoint's endpoint number
     */
    CARAPI GetEndpointNumber(
        /* [out] */ Int32* number);

    /**
     * Returns the endpoint's direction.
     * Returns {@link UsbConstants#USB_DIR_OUT}
     * if the direction is host to device, and
     * {@link UsbConstants#USB_DIR_IN} if the
     * direction is device to host.
     * @see {@link UsbConstants#USB_DIR_IN}
     * @see {@link UsbConstants#USB_DIR_OUT}
     *
     * @return the endpoint's direction
     */
    CARAPI GetDirection(
        /* [out] */ Int32* direction);

    /**
     * Returns the endpoint's attributes field.
     *
     * @return the endpoint's attributes
     */
    CARAPI GetAttributes(
        /* [out] */ Int32* attributes);

    /**
     * Returns the endpoint's type.
     * Possible results are:
     * <ul>
     * <li>{@link UsbConstants#USB_ENDPOINT_XFER_CONTROL} (endpoint zero)
     * <li>{@link UsbConstants#USB_ENDPOINT_XFER_ISOC} (isochronous endpoint)
     * <li>{@link UsbConstants#USB_ENDPOINT_XFER_BULK} (bulk endpoint)
     * <li>{@link UsbConstants#USB_ENDPOINT_XFER_INT} (interrupt endpoint)
     * </ul>
     *
     * @return the endpoint's type
     */
    CARAPI GetType(
        /* [out] */ Int32* type);

    /**
     * Returns the endpoint's maximum packet size.
     *
     * @return the endpoint's maximum packet size
     */
    CARAPI GetMaxPacketSize(
        /* [out] */ Int32* size);

    /**
     * Returns the endpoint's interval field.
     *
     * @return the endpoint's interval
     */
    CARAPI GetInterval(
        /* [out] */ Int32* interval);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI DescribeContents (
        /* [out] */ Int32* contents);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [out] */ IParcel* dest);

private:
    Int32 mAddress;
    Int32 mAttributes;
    Int32 mMaxPacketSize;
    Int32 mInterval;
};

#endif // __CUSBENDPOINT_H__
