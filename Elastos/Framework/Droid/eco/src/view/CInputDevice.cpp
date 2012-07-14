
#include "view/CInputDevice.h"
#include "view/ElKeyCharacterMap.h"
#include <Slogger.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;

const Int32 CInputDevice::SOURCE_CLASS_MASK;
const Int32 CInputDevice::SOURCE_CLASS_BUTTON;
const Int32 CInputDevice::SOURCE_CLASS_POINTER;
const Int32 CInputDevice::SOURCE_CLASS_TRACKBALL;
const Int32 CInputDevice::SOURCE_CLASS_POSITION;
const Int32 CInputDevice::SOURCE_UNKNOWN;
const Int32 CInputDevice::SOURCE_KEYBOARD;
const Int32 CInputDevice::SOURCE_DPAD;
const Int32 CInputDevice::SOURCE_TOUCHSCREEN;
const Int32 CInputDevice::SOURCE_MOUSE;
const Int32 CInputDevice::SOURCE_TRACKBALL;
const Int32 CInputDevice::SOURCE_TOUCHPAD;
const Int32 CInputDevice::SOURCE_ANY;
const Int32 CInputDevice::MOTION_RANGE_X;
const Int32 CInputDevice::MOTION_RANGE_Y;
const Int32 CInputDevice::MOTION_RANGE_PRESSURE;
const Int32 CInputDevice::MOTION_RANGE_SIZE;
const Int32 CInputDevice::MOTION_RANGE_TOUCH_MAJOR;
const Int32 CInputDevice::MOTION_RANGE_TOUCH_MINOR;
const Int32 CInputDevice::MOTION_RANGE_TOOL_MAJOR;
const Int32 CInputDevice::MOTION_RANGE_TOOL_MINOR;
const Int32 CInputDevice::MOTION_RANGE_ORIENTATION;
const Int32 CInputDevice::KEYBOARD_TYPE_NONE;
const Int32 CInputDevice::KEYBOARD_TYPE_NON_ALPHABETIC;
const Int32 CInputDevice::KEYBOARD_TYPE_ALPHABETIC;
const Int32 CInputDevice::MOTION_RANGE_LAST;

CInputDevice::MotionRange::MotionRange(
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float flat,
    /* [in] */ Float fuzz) :
    mMin(min),
    mMax(max),
    mFlat(flat),
    mFuzz(fuzz)
{
}

PInterface CInputDevice::MotionRange::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == EIID_IMotionRange) {
        return (IMotionRange*)this;
    }
    return NULL;
}

UInt32 CInputDevice::MotionRange::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CInputDevice::MotionRange::Release()
{
    return ElRefBase::Release();
}

ECode CInputDevice::MotionRange::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

/**
* Gets the minimum value for the coordinate.
* @return The minimum value.
*/
ECode CInputDevice::MotionRange::GetMin(
    /* [out] */ Float* minimum)
{
    VALIDATE_NOT_NULL(minimum);
    *minimum = mMin;

    return NOERROR;
}

/**
* Gets the maximum value for the coordinate.
* @return The minimum value.
*/
ECode CInputDevice::MotionRange::GetMax(
    /* [out] */ Float* maximum)
{
    VALIDATE_NOT_NULL(maximum);
    *maximum = mMax;

    return NOERROR;
}

/**
* Gets the range of the coordinate (difference between maximum and minimum).
* @return The range of values.
*/
ECode CInputDevice::MotionRange::GetRange(
    /* [out] */ Float* range)
{
    VALIDATE_NOT_NULL(range);
    *range = mMax - mMin;

    return NOERROR;
}

/**
* Gets the extent of the center flat position with respect to this coordinate.
* For example, a flat value of 8 means that the center position is between -8 and +8.
* This value is mainly useful for calibrating self-centering devices.
* @return The extent of the center flat position.
*/
ECode CInputDevice::MotionRange::GetFlat(
    /* [out] */ Float* flat)
{
    VALIDATE_NOT_NULL(flat);
    *flat = mFlat;

    return NOERROR;
}

/**
* Gets the error tolerance for input device measurements with respect to this coordinate.
* For example, a value of 2 indicates that the measured value may be up to +/- 2 units
* away from the actual value due to noise and device sensitivity limitations.
* @return The error tolerance.
*/
ECode CInputDevice::MotionRange::GetFuzz(
    /* [out] */ Float* fuzz)
{
    VALIDATE_NOT_NULL(fuzz);
    *fuzz = mFuzz;

    return NOERROR;
}

CInputDevice::CInputDevice()
{
    memset(mMotionRanges, 0, sizeof(MotionRange*) * (MOTION_RANGE_LAST + 1));
}

CInputDevice::~CInputDevice()
{
    Int32 size = MOTION_RANGE_LAST + 1;
    for (Int32 i=0; i<size; ++i) {
        delete mMotionRanges[i];
    }
}

/**
* Gets information about the input device with the specified id.
* @param id The device id.
* @return The input device or NULL if not found.
*/
CInputDevice* CInputDevice::GetDevice(
    /* [in] */ Int32 id)
{
    AutoPtr<IServiceManager> sm;
    GetServiceManager((IServiceManager**)&sm);

    AutoPtr<IWindowManagerEx> wm;
    sm->GetService(String("window"), (IInterface**)(IWindowManagerEx**)&wm);

    AutoPtr<CInputDevice> inputDevice;
    if (FAILED(wm->GetInputDevice(id, (IInputDevice**)&inputDevice))) {
        Slogger::E("", "Could not get input device information from Window Manager.");
    }

    return inputDevice;
}

/**
* Gets the ids of all input devices in the system.
* @return The input device ids.
*/
//Int32[] InputDevice::GetDeviceIds()
//{
//    IWindowManager wm = IWindowManager.Stub.asInterface(ServiceManager.getService("window"));
//    try {
//        return wm.getInputDeviceIds();
//    } catch (RemoteException ex) {
//        throw new RuntimeException(
//            "Could not get input device ids from Window Manager.", ex);
//    }
//}

/**
* Gets the input device id.
* @return The input device id.
*/
ECode CInputDevice::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;

    return NOERROR;
}

/**
* Gets the name of this input device.
* @return The input device name.
*/
ECode CInputDevice::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;

    return NOERROR;
}

/**
* Gets the input sources supported by this input device as a combined bitfield.
* @return The supported input sources.
*/
ECode CInputDevice::GetSources(
    /* [out] */ Int32* sources)
{
    VALIDATE_NOT_NULL(sources);
    *sources = mSources;

    return NOERROR;
}

/**
* Gets the keyboard type.
* @return The keyboard type.
*/
ECode CInputDevice::GetKeyboardType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mKeyboardType;

    return NOERROR;
}

/**
* Gets the key character map associated with this input device.
* @return The key character map.
*/
ElKeyCharacterMap* CInputDevice::GetKeyCharacterMap()
{
    return ElKeyCharacterMap::Load(mId);
}

/**
* Gets information about the range of values for a particular {@link MotionEvent}
* coordinate.
* @param rangeType The motion range constant.
* @return The range of values, or NULL if the requested coordinate is not
* supported by the device.
*/
ECode CInputDevice::GetMotionRange(
    /* [in] */ Int32 rangeType,
    /* [out] */ IMotionRange** montionRange)
{
    VALIDATE_NOT_NULL(montionRange);

    if (rangeType < 0 || rangeType > MOTION_RANGE_LAST) {
        //throw new IllegalArgumentException("Requested range is out of bounds.");
        Slogger::E("", "Requested range is out of bounds.");
        *montionRange = NULL;
        return NOERROR;
    }

    *montionRange = mMotionRanges[rangeType];
    if (*montionRange) (*montionRange)->AddRef();

    return NOERROR;
}

void CInputDevice::AddMotionRange(
    /* [in] */ Int32 rangeType,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float flat,
    /* [in] */ Float fuzz)
{
    if (rangeType >= 0 && rangeType <= MOTION_RANGE_LAST) {
        MotionRange* range = new MotionRange(min, max, flat, fuzz);
        mMotionRanges[rangeType] = range;
    }
}

ECode CInputDevice::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    in->ReadInt32(&mId);
    in->ReadString(&mName);
    in->ReadInt32(&mSources);
    in->ReadInt32(&mKeyboardType);

    for (;;) {
        Int32 rangeType;
        in->ReadInt32(&rangeType);
        if (rangeType < 0) {
            break;
        }
        Float min, max, flat, fuzz ;
        in->ReadFloat(&min);
        in->ReadFloat(&max);
        in->ReadFloat(&flat);
        in->ReadFloat(&fuzz);
        AddMotionRange(rangeType, min, max, flat, fuzz);
    }

    return NOERROR;
}

ECode CInputDevice::WriteToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt32(mId);
    out->WriteString(mName);
    out->WriteInt32(mSources);
    out->WriteInt32(mKeyboardType);

    for (Int32 i = 0; i <= MOTION_RANGE_LAST; i++) {
        MotionRange* range = mMotionRanges[i];
        if (range != NULL) {
            out->WriteInt32(i);
            out->WriteFloat(range->mMin);
            out->WriteFloat(range->mMax);
            out->WriteFloat(range->mFlat);
            out->WriteFloat(range->mFuzz);
        }
    }
    out->WriteInt32(-1);

    return NOERROR;
}

ECode CInputDevice::DescribeContents(
    /* [out] */ Int32* describe)
{
    VALIDATE_NOT_NULL(describe);
    *describe = 0;

    return NOERROR;
}
