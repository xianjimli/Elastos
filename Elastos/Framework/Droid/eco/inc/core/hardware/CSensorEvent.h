
#ifndef __CSENSOREVENT_H__
#define __CSENSOREVENT_H__

#include "_CSensorEvent.h"
#include <elastos/AutoPtr.h>

CarClass(CSensorEvent)
{
public:
    CSensorEvent();

    virtual ~CSensorEvent();

    CARAPI GetValues(
        /* [out, callee] */ ArrayOf<Float>** values);

    CARAPI SetValues(
        /* [in] */ const ArrayOf<Float>& values);

    CARAPI GetSensor(
        /* [out] */ ISensor** sensor);

    CARAPI SetSensor(
        /* [in] */ ISensor* sensor);

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI SetAccuracy(
        /* [in] */ Int32 accuracy);

    CARAPI GetTimeStamp(
        /* [out] */ Int64* timestamp);

    CARAPI SetTimeStamp(
        /* [in] */ Int64 timestamp);

    CARAPI constructor(
        /* [in] */ Int32 size);

private:
    ArrayOf<Float>* mValues;
    AutoPtr<ISensor> mSensor;
    Int32 mAccuracy;
    Int64 mTimestamp;
};

#endif // __CSENSOREVENT_H__
