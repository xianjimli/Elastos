
#include "view/InputEvent.h"
#include "view/CInputDevice.h"

const Int32 InputEvent::PARCEL_TOKEN_MOTION_EVENT;
const Int32 InputEvent::PARCEL_TOKEN_KEY_EVENT;

InputEvent::InputEvent()
{
}

/**
 * Gets the id for the device that this event came from.  An id of
 * zero indicates that the event didn't come from a physical device
 * and maps to the default keymap.  The other numbers are arbitrary and
 * you shouldn't depend on the values.
 *
 * @return The device id.
 * @see InputDevice#getDevice
 */
Int32 InputEvent::GetDeviceId()
{
    return mDeviceId;
}

/**
 * Gets the device that this event came from.
 *
 * @return The device, or null if unknown.
 */
CInputDevice* InputEvent::GetDevice()
{
    return CInputDevice::GetDevice(mDeviceId);
}

/**
 * Gets the source of the event.
 *
 * @return The event source or {@link InputDevice#SOURCE_UNKNOWN} if unknown.
 * @see InputDevice#getSourceInfo
 */
Int32 InputEvent::GetSource()
{
    return mSource;
}

/**
 * Modifies the source of the event.
 * @param source The source.
 *
 * @hide
 */
void InputEvent::SetSource(
    /* [in] */ Int32 source)
{
    mSource = source;
}

Int32 InputEvent::DescribeContents()
{
    return 0;
}

/** @hide */
void InputEvent::ReadBaseFromParcel(
    /* [in] */ IParcel* in)
{
    in->ReadInt32(&mDeviceId);
    in->ReadInt32(&mSource);
}

/** @hide */
void InputEvent::WriteBaseToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt32(mDeviceId);
    out->WriteInt32(mSource);
}
