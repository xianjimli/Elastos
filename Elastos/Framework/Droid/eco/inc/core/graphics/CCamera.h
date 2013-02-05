
#ifndef __CCAMERA_H__
#define __CCAMERA_H__

#include "_CCamera.h"
#include <skia/utils/SkCamera.h>


CarClass(CCamera)
{
public:
    CARAPI save();

    CARAPI restore();

    CARAPI translate(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float z);

    CARAPI rotateX(
        /* [in] */ Float deg);

    CARAPI rotateY(
        /* [in] */ Float deg);

    CARAPI rotateZ(
        /* [in] */ Float deg);

    CARAPI getMatrix(
        /* [in] */ IMatrix * pMatrix);

    CARAPI applyToCanvas(
        /* [in] */ ICanvas * pCanvas);

    CARAPI dotWithNormal(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ Float dz,
        /* [out] */ Float * pResult);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
    Int32 native_instance;

	Sk3DView*  mView;
    
    CARAPI nativeConstructor();

    CARAPI nativeDestructor();

    CARAPI nativeGetMatrix(
        /* [in] */ Int32 native_matrix);

    CARAPI nativeApplyToCanvas(
        /* [in] */ Int32 native_canvas);
};

#endif // __CCAMERA_H__
