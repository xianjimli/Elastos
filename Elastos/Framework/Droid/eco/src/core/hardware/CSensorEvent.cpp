
#include "ext/frameworkext.h"
#include "hardware/CSensorEvent.h"

CSensorEvent::CSensorEvent()
    : mValues(NULL)
    , mAccuracy(0)
    , mTimestamp(0)
{
}

CSensorEvent::~CSensorEvent()
{
    if (mValues) ArrayOf<Float>::Free(mValues);
}

ECode CSensorEvent::GetValues(
    /* [out, callee] */ ArrayOf<Float>** values)
{
    VALIDATE_NOT_NULL(values);

    if (!mValues) {
        *values = NULL;
        return NOERROR;
    }

    ArrayOf<Float>* array = ArrayOf<Float>::Alloc(mValues->GetLength());
    if (!array) return E_OUT_OF_MEMORY;

    for (Int32 i = 0; i < mValues->GetLength(); ++i) {
        (*array)[i] = (*mValues)[i];
    }

    *values = array;

    return NOERROR;
}

ECode CSensorEvent::SetValues(
    /* [in] */ const ArrayOf<Float>& values)
{
    if (mValues) ArrayOf<Float>::Free(mValues);

    mValues = ArrayOf<Float>::Alloc(values.GetLength());
    if (!mValues) return E_OUT_OF_MEMORY;

    for (Int32 i = 0; i < values.GetLength(); ++i) {
        (*mValues)[i] = values[i];
    }

    return NOERROR;
}

ECode CSensorEvent::GetSensor(
    /* [out] */ ISensor** sensor)
{
    VALIDATE_NOT_NULL(sensor);

    *sensor = mSensor;
    (*sensor)->AddRef();

    return NOERROR;
}

ECode CSensorEvent::SetSensor(
    /* [in] */ ISensor* sensor)
{
    mSensor = sensor;

    return NOERROR;
}

ECode CSensorEvent::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);

    *accuracy = mAccuracy;

    return NOERROR;
}

ECode CSensorEvent::SetAccuracy(
    /* [in] */ Int32 accuracy)
{
    mAccuracy = accuracy;

    return NOERROR;
}

ECode CSensorEvent::GetTimeStamp(
    /* [out] */ Int64* timestamp)
{
    VALIDATE_NOT_NULL(timestamp);

    *timestamp = mTimestamp;

    return NOERROR;
}

ECode CSensorEvent::SetTimeStamp(
    /* [in] */ Int64 timestamp)
{
    mTimestamp = timestamp;

    return NOERROR;
}

ECode CSensorEvent::constructor(
    /* [in] */ Int32 size)
{
    mValues = ArrayOf<Float>::Alloc(size);
    if (!mValues) return E_OUT_OF_MEMORY;

    return NOERROR;
}

