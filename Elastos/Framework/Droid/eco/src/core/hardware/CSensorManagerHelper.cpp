
#include "ext/frameworkext.h"
#include "hardware/CSensorManagerHelper.h"

ECode CSensorManagerHelper::GetRotationMatrix(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ ArrayOf<Float>* I,
    /* [in] */ ArrayOf<Float>* gravity,
    /* [in] */ ArrayOf<Float>* geomagnetic,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManagerHelper::GetInclination(
    /* [in] */ const ArrayOf<Float>& I,
    /* [out] */ Float* inclination)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManagerHelper::RemapCoordinateSystem(
    /* [in] */ ArrayOf<Float>* inR,
    /* [in] */ Int32 X,
    /* [in] */ Int32 Y,
    /* [in] */ ArrayOf<Float>* outR,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManagerHelper::GetOrientation(
    /* [in] */ const ArrayOf<Float>& R,
    /* [in] */ ArrayOf<Float>* values)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManagerHelper::GetAltitude(
    /* [in] */ Float p0,
    /* [in] */ Float p,
    /* [out] */ Float* altitude)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManagerHelper::GetAngleChange(
    /* [in] */ ArrayOf<Float>* angleChange,
    /* [in] */ const ArrayOf<Float>& R,
    /* [in] */ const ArrayOf<Float>& prevR)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManagerHelper::GetRotationMatrixFromVector(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ const ArrayOf<Float>& rotationVector)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManagerHelper::GetQuaternionFromVector(
    /* [in] */ ArrayOf<Float> * Q,
    /* [in] */ const ArrayOf<Float>& rv)
{
    return E_NOT_IMPLEMENTED;
}

