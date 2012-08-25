
#ifndef __CINPUTDEVICE_H__
#define __CINPUTDEVICE_H__

#include "_CInputDevice.h"
#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>

class ElKeyCharacterMap;

CarClass(CInputDevice)
{
public:
    /**
     * A mask for input source classes.
     *
     * Each distinct input source constant has one or more input source class bits set to
     * specify the desired interpretation for its input events.
     */
    static const Int32 SOURCE_CLASS_MASK = 0x000000ff;

    /**
     * The input source has buttons or keys.
     * Examples: {@link #SOURCE_KEYBOARD}, {@link #SOURCE_DPAD}.
     *
     * A {@link KeyEvent} should be interpreted as a button or key press.
     *
     * Use {@link #getKeyCharacterMap} to query the device's button and key mappings.
     */
    static const Int32 SOURCE_CLASS_BUTTON = 0x00000001;

    /**
     * The input source is a pointing device associated with a display.
     * Examples: {@link #SOURCE_TOUCHSCREEN}, {@link #SOURCE_MOUSE}.
     *
     * A {@link MotionEvent} should be interpreted as absolute coordinates in
     * display units according to the {@link View} hierarchy.  Pointer down/up indicated when
     * the finger touches the display or when the selection button is pressed/released.
     *
     * Use {@link #getMotionRange} to query the range of the pointing device.  Some devices permit
     * touches outside the display area so the effective range may be somewhat smaller or larger
     * than the actual display size.
     */
    static const Int32 SOURCE_CLASS_POINTER = 0x00000002;

    /**
     * The input source is a trackball navigation device.
     * Examples: {@link #SOURCE_TRACKBALL}.
     *
     * A {@link MotionEvent} should be interpreted as relative movements in device-specific
     * units used for navigation purposes.  Pointer down/up indicates when the selection button
     * is pressed/released.
     *
     * Use {@link #getMotionRange} to query the range of motion.
     */
    static const Int32 SOURCE_CLASS_TRACKBALL = 0x00000004;

    /**
     * The input source is an absolute positioning device not associated with a display
     * (unlike {@link #SOURCE_CLASS_POINTER}).
     *
     * A {@link MotionEvent} should be interpreted as absolute coordinates in
     * device-specific surface units.
     *
     * Use {@link #getMotionRange} to query the range of positions.
     */
    static const Int32 SOURCE_CLASS_POSITION = 0x00000008;

    /**
     * The input source is unknown.
     */
    static const Int32 SOURCE_UNKNOWN = 0x00000000;

    /**
     * The input source is a keyboard.
     *
     * @see #SOURCE_CLASS_BUTTON
     */
    static const Int32 SOURCE_KEYBOARD = 0x00000100 | SOURCE_CLASS_BUTTON;

    /**
     * The input source is a DPad.
     *
     * @see #SOURCE_CLASS_BUTTON
     */
    static const Int32 SOURCE_DPAD = 0x00000200 | SOURCE_CLASS_BUTTON;

    /**
     * The input source is a touch screen pointing device.
     *
     * @see #SOURCE_CLASS_POINTER
     */
    static const Int32 SOURCE_TOUCHSCREEN = 0x00001000 | SOURCE_CLASS_POINTER;

    /**
     * The input source is a mouse pointing device.
     * This code is also used for other mouse-like pointing devices such as trackpads
     * and trackpoints.
     *
     * @see #SOURCE_CLASS_POINTER
     */
    static const Int32 SOURCE_MOUSE = 0x00002000 | SOURCE_CLASS_POINTER;

    /**
     * The input source is a trackball.
     *
     * @see #SOURCE_CLASS_TRACKBALL
     */
    static const Int32 SOURCE_TRACKBALL = 0x00010000 | SOURCE_CLASS_TRACKBALL;

    /**
     * The input source is a touch pad or digitizer tablet that is not
     * associated with a display (unlike {@link #SOURCE_TOUCHSCREEN}).
     *
     * @see #SOURCE_CLASS_POSITION
     */
    static const Int32 SOURCE_TOUCHPAD = 0x00100000 | SOURCE_CLASS_POSITION;

    /**
     * A special input source constant that is used when filtering input devices
     * to match devices that provide any type of input source.
     */
    static const Int32 SOURCE_ANY = 0xffffff00;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#x}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_X = 0;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#y}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_Y = 1;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#pressure}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_PRESSURE = 2;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#size}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_SIZE = 3;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#touchMajor}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_TOUCH_MAJOR = 4;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#touchMinor}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_TOUCH_MINOR = 5;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#toolMajor}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_TOOL_MAJOR = 6;

    /**
     * Constant for retrieving the range of values for {@link MotionEvent.PointerCoords#toolMinor}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_TOOL_MINOR = 7;

    /**
     * Constant for retrieving the range of values for
     * {@link MotionEvent.PointerCoords#orientation}.
     *
     * @see #getMotionRange
     */
    static const Int32 MOTION_RANGE_ORIENTATION = 8;

    /**
     * There is no keyboard.
     */
    static const Int32 KEYBOARD_TYPE_NONE = 0;

    /**
     * The keyboard is not fully alphabetic.  It may be a numeric keypad or an assortment
     * of buttons that are not mapped as alphabetic keys suitable for text input.
     */
    static const Int32 KEYBOARD_TYPE_NON_ALPHABETIC = 1;

    /**
     * The keyboard supports a complement of alphabetic keys.
     */
    static const Int32 KEYBOARD_TYPE_ALPHABETIC = 2;

private:
    static const Int32 MOTION_RANGE_LAST = MOTION_RANGE_ORIENTATION;

public:
    /**
     * Provides information about the range of values for a particular {@link MotionEvent}
     * coordinate.
     */
    class MotionRange : public ElRefBase, public IMotionRange
    {
        friend class CInputDevice;
    private:
        MotionRange(
            /* [in] */ Float min,
            /* [in] */ Float max,
            /* [in] */ Float flat,
            /* [in] */ Float fuzz);

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /**
         * Gets the minimum value for the coordinate.
         * @return The minimum value.
         */
        CARAPI GetMin(
            /* [out] */ Float* minimum);

        /**
         * Gets the maximum value for the coordinate.
         * @return The minimum value.
         */
        CARAPI GetMax(
            /* [out] */ Float* maximum);

        /**
         * Gets the range of the coordinate (difference between maximum and minimum).
         * @return The range of values.
         */
        CARAPI GetRange(
            /* [out] */ Float* range);

        /**
         * Gets the extent of the center flat position with respect to this coordinate.
         * For example, a flat value of 8 means that the center position is between -8 and +8.
         * This value is mainly useful for calibrating self-centering devices.
         * @return The extent of the center flat position.
         */
        CARAPI GetFlat(
            /* [out] */ Float* flat);

        /**
         * Gets the error tolerance for input device measurements with respect to this coordinate.
         * For example, a value of 2 indicates that the measured value may be up to +/- 2 units
         * away from the actual value due to noise and device sensitivity limitations.
         * @return The error tolerance.
         */
        CARAPI GetFuzz(
            /* [out] */ Float* fuzz);

    private:
        Float mMin;
        Float mMax;
        Float mFlat;
        Float mFuzz;
    };

public:
    CInputDevice();

    ~CInputDevice();

    /**
     * Gets information about the input device with the specified id.
     * @param id The device id.
     * @return The input device or NULL if not found.
     */
    static CARAPI_(CInputDevice*) GetDevice(
        /* [in] */ Int32 id);

    /**
     * Gets the ids of all input devices in the system.
     * @return The input device ids.
     */
    //static Int32[] GetDeviceIds();

    /**
     * Gets the input device id.
     * @return The input device id.
     */
    CARAPI GetId(
        /* [out] */ Int32* id);

    /**
     * Gets the name of this input device.
     * @return The input device name.
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Gets the input sources supported by this input device as a combined bitfield.
     * @return The supported input sources.
     */
    CARAPI GetSources(
        /* [out] */ Int32* sources);

    /**
     * Gets the keyboard type.
     * @return The keyboard type.
     */
    CARAPI GetKeyboardType(
        /* [out] */ Int32* type);

    /**
     * Gets the key character map associated with this input device.
     * @return The key character map.
     */
    CARAPI_(ElKeyCharacterMap*) GetKeyCharacterMap();

    /**
     * Gets information about the range of values for a particular {@link MotionEvent}
     * coordinate.
     * @param rangeType The motion range constant.
     * @return The range of values, or NULL if the requested coordinate is not
     * supported by the device.
     */
    CARAPI GetMotionRange(
        /* [in] */ Int32 rangeType,
        /* [out] */ IMotionRange** motionRange);


    CARAPI DescribeContents(
        /* [out] */ Int32* describe);

    //String ToString() {
    //    StringBuilder description = new StringBuilder();
    //    description.append("Input Device ").append(mId).append(": ").append(mName).append("\n");

    //    description.append("  Keyboard Type: ");
    //    switch (mKeyboardType) {
    //    case KEYBOARD_TYPE_NONE:
    //        description.append("none");
    //        break;
    //    case KEYBOARD_TYPE_NON_ALPHABETIC:
    //        description.append("non-alphabetic");
    //        break;
    //    case KEYBOARD_TYPE_ALPHABETIC:
    //        description.append("alphabetic");
    //        break;
    //    }
    //    description.append("\n");

    //    description.append("  Sources:");
    //    appendSourceDescriptionIfApplicable(description, SOURCE_KEYBOARD, "keyboard");
    //    appendSourceDescriptionIfApplicable(description, SOURCE_DPAD, "dpad");
    //    appendSourceDescriptionIfApplicable(description, SOURCE_TOUCHSCREEN, "touchscreen");
    //    appendSourceDescriptionIfApplicable(description, SOURCE_MOUSE, "mouse");
    //    appendSourceDescriptionIfApplicable(description, SOURCE_TRACKBALL, "trackball");
    //    appendSourceDescriptionIfApplicable(description, SOURCE_TOUCHPAD, "touchpad");
    //    description.append("\n");

    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_X, "x");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_Y, "y");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_PRESSURE, "pressure");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_SIZE, "size");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_TOUCH_MAJOR, "touchMajor");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_TOUCH_MINOR, "touchMinor");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_TOOL_MAJOR, "toolMajor");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_TOOL_MINOR, "toolMinor");
    //    appendRangeDescriptionIfApplicable(description, MOTION_RANGE_ORIENTATION, "orientation");

    //    return description.toString();
    //}

    //private void appendSourceDescriptionIfApplicable(StringBuilder description, Int32 source,
    //    String sourceName) {
    //        if ((mSources & source) == source) {
    //            description.append(" ");
    //            description.append(sourceName);
    //        }
    //}

    //private void appendRangeDescriptionIfApplicable(StringBuilder description,
    //    Int32 rangeType, String rangeName) {
    //        MotionRange range = mMotionRanges[rangeType];
    //        if (range != NULL) {
    //            description.append("  Range[").append(rangeName);
    //            description.append("]: min=").append(range.mMin);
    //            description.append(" max=").append(range.mMax);
    //            description.append(" flat=").append(range.mFlat);
    //            description.append(" fuzz=").append(range.mFuzz);
    //            description.append("\n");
    //        }
    //}

    //static final Parcelable.Creator<InputDevice> CREATOR
    //    = new Parcelable.Creator<InputDevice>() {
    //        public InputDevice createFromParcel(Parcel in) {
    //            InputDevice result = new InputDevice();
    //            result.readFromParcel(in);
    //            return result;
    //        }

    //        public InputDevice[] newArray(Int32 size) {
    //            return new InputDevice[size];
    //        }
    //};

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    CARAPI_(void) AddMotionRange(
        /* [in] */ Int32 rangeType,
        /* [in] */ Float min,
        /* [in] */ Float max,
        /* [in] */ Float flat,
        /* [in] */ Float fuzz);

private:
    Int32 mId;
    String mName;
    Int32 mSources;
    Int32 mKeyboardType;

    MotionRange* mMotionRanges[MOTION_RANGE_LAST + 1];
};

#endif //__CINPUTDEVICE_H__
