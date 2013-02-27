
#ifndef __CCAMERA_H__
#define __CCAMERA_H__

#include "_CCamera.h"
#include <skia/utils/SkCamera.h>


CarClass(CCamera)
{
public:
    CARAPI Save();

    CARAPI Restore();

    CARAPI Translate(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float z);

    CARAPI RotateX(
        /* [in] */ Float deg);

    CARAPI RotateY(
        /* [in] */ Float deg);

    CARAPI RotateZ(
        /* [in] */ Float deg);

    CARAPI GetMatrix(
        /* [in] */ IMatrix * pMatrix);

    CARAPI ApplyToCanvas(
        /* [in] */ ICanvas * pCanvas);

    CARAPI DotWithNormal(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ Float dz,
        /* [out] */ Float * pResult);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
    Int32 native_instance;

    Sk3DView*  mView;

    CARAPI NativeConstructor();

    CARAPI NativeDestructor();

    CARAPI NativeGetMatrix(
        /* [in] */ Int32 native_matrix);

    CARAPI NativeApplyToCanvas(
        /* [in] */ Int32 native_canvas);
};

#endif // __CCAMERA_H__
