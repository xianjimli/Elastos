
#include <gesture/CGesturePoint.h>

ECode CGesturePoint::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 t)
{
    mX = x;
    mY = y;
    mTimestamp = t;
    return NOERROR;
}

ECode CGesturePoint::Deserialize(
    /* [in] */ IDataInputStream* in,
    /* [out] */ IGesturePoint** obj)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(obj);

    // Read X and Y
    Float x, y;

    IDataInput * din = IDataInput::Probe(in);
    din->ReadFloat(&x);
    din->ReadFloat(&y);

    // Read timestamp
    Int64 timeStamp;
    din->ReadInt64(&timeStamp);
    return CGesturePoint::New(x, y, timeStamp, obj);
}

ECode CGesturePoint::DeserializeEx(
    /* [in] */ IParcel* in,
    /* [out] */ IGesturePoint** obj)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(obj);

    // Read X and Y
    Float x, y;

    in->ReadFloat(&x);
    in->ReadFloat(&y);

    // Read timestamp
    Int64 timeStamp;
    in->ReadInt64(&timeStamp);
    return CGesturePoint::New(x, y, timeStamp, obj);
}

ECode CGesturePoint::Clone(
    /* [out] */ IGesturePoint ** gesturePoint)
{
    VALIDATE_NOT_NULL(gesturePoint);
    return CGesturePoint::New(mX, mY, mTimestamp, gesturePoint);
}

ECode CGesturePoint::GetX(
    /* [out] */ Float *x)
{
    VALIDATE_NOT_NULL(x);
    *x = mX;
    return NOERROR;
}

ECode CGesturePoint::GetY(
    /* [out] */ Float *y)
{
    VALIDATE_NOT_NULL(y);
    *y = mY;
    return NOERROR;
}

ECode CGesturePoint::GetTimestamp(
    /* [out] */ Int64 * timestamp)
{
    VALIDATE_NOT_NULL(timestamp);
    *timestamp = mTimestamp;
    return NOERROR;
}
