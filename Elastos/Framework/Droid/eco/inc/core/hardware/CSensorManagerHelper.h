
#ifndef __CSENSORMANAGERHELPER_H__
#define __CSENSORMANAGERHELPER_H__

#include "_CSensorManagerHelper.h"

CarClass(CSensorManagerHelper)
{
public:
    friend class CSensorManagerLegacyListener;

public:
    CARAPI GetRotationMatrix(
        /* [in] */ ArrayOf<Float> * pR,
        /* [in] */ ArrayOf<Float> * pI,
        /* [in] */ ArrayOf<Float> * pGravity,
        /* [in] */ ArrayOf<Float> * pGeomagnetic,
        /* [out] */ Boolean * pResult);

    CARAPI GetInclination(
        /* [in] */ const ArrayOf<Float> & I,
        /* [out] */ Float * pInclination);

    CARAPI RemapCoordinateSystem(
        /* [in] */ ArrayOf<Float> * pInR,
        /* [in] */ Int32 X,
        /* [in] */ Int32 Y,
        /* [in] */ ArrayOf<Float> * pOutR,
        /* [out] */ Boolean * pResult);

    CARAPI GetOrientation(
        /* [in] */ const ArrayOf<Float> & R,
        /* [in] */ ArrayOf<Float> * pValues);

    CARAPI GetAltitude(
        /* [in] */ Float p0,
        /* [in] */ Float p,
        /* [out] */ Float * pAltitude);

    CARAPI GetAngleChange(
        /* [in] */ ArrayOf<Float> * pAngleChange,
        /* [in] */ const ArrayOf<Float> & R,
        /* [in] */ const ArrayOf<Float> & prevR);

    CARAPI GetRotationMatrixFromVector(
        /* [in] */ ArrayOf<Float> * pR,
        /* [in] */ const ArrayOf<Float> & rotationVector);

    CARAPI GetQuaternionFromVector(
        /* [in] */ ArrayOf<Float> * pQ,
        /* [in] */ const ArrayOf<Float> & rv);

private:
    CARAPI GetRotation(
        /* [out] */ Int32* rotation);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSENSORMANAGERHELPER_H__
