#ifndef __INPUTEVENT_H__
#define __INPUTEVENT_H__

#include "ext/frameworkext.h"

class CInputDevice;

class InputEvent
{
protected:
    /** @hide */
    Int32 mDeviceId;
    /** @hide */
    Int32 mSource;

    /** @hide */
    static const Int32 PARCEL_TOKEN_MOTION_EVENT = 1;
    /** @hide */
    static const Int32 PARCEL_TOKEN_KEY_EVENT = 2;

protected:
    InputEvent();

public:
    /**
     * Gets the id for the device that this event came from.  An id of
     * zero indicates that the event didn't come from a physical device
     * and maps to the default keymap.  The other numbers are arbitrary and
     * you shouldn't depend on the values.
     *
     * @return The device id.
     * @see InputDevice#getDevice
     */
    CARAPI_(Int32) GetDeviceId();

    /**
     * Gets the device that this event came from.
     *
     * @return The device, or null if unknown.
     */
    CARAPI_(CInputDevice*) GetDevice();

    /**
     * Gets the source of the event.
     *
     * @return The event source or {@link InputDevice#SOURCE_UNKNOWN} if unknown.
     * @see InputDevice#getSourceInfo
     */
    CARAPI_(Int32) GetSource();

    /**
     * Modifies the source of the event.
     * @param source The source.
     *
     * @hide
     */
    CARAPI_(void) SetSource(
        /* [in] */ Int32 source);

    CARAPI_(Int32) DescribeContents();

protected:
    /** @hide */
    CARAPI_(void) ReadBaseFromParcel(
        /* [in] */ IParcel* in);

    /** @hide */
    CARAPI_(void) WriteBaseToParcel(
        /* [in] */ IParcel* out);

    //public static final Parcelable.Creator<InputEvent> CREATOR
    //    = new Parcelable.Creator<InputEvent>() {
    //        public InputEvent createFromParcel(Parcel in) {
    //            Int32 token = in.readInt();
    //            if (token == PARCEL_TOKEN_KEY_EVENT) {
    //                return KeyEvent.createFromParcelBody(in);
    //            } else if (token == PARCEL_TOKEN_MOTION_EVENT) {
    //                return MotionEvent.createFromParcelBody(in);
    //            } else {
    //                throw new IllegalStateException("Unexpected input event type token in parcel.");
    //            }
    //        }

    //        public InputEvent[] newArray(Int32 size) {
    //            return new InputEvent[size];
    //        }
    //};
};
#endif //__INPUTEVENT_H__
