
#ifndef __CINPUTDEVICE_H__
#define __CINPUTDEVICE_H__

#include "_CInputDevice.h"
#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>

class ElKeyCharacterMap;

CarClass(CInputDevice)
{
private:
    static const Int32 MOTION_RANGE_LAST = InputDevice_MOTION_RANGE_ORIENTATION;

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
