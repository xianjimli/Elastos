
#ifndef __CCAMERA_H__
#define __CCAMERA_H__

#include "_CCamera.h"
#include <skia/utils/SkCamera.h>


CarClass(CCamera)
{
public:
    CCamera();

    ~CCamera();

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
        /* [in] */ IMatrix* matrix);

    CARAPI ApplyToCanvas(
        /* [in] */ ICanvas* canvas);

    CARAPI DotWithNormal(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ Float dz,
        /* [out] */ Float* result);

private:
    CARAPI_(void) NativeConstructor();

    CARAPI_(void) NativeDestructor();

    CARAPI_(void) NativeGetMatrix(
        /* [in] */ SkMatrix* nativeMatrix);

    CARAPI_(void) NativeApplyToCanvas(
        /* [in] */ SkCanvas* nativeCanvas);

private:
    Sk3DView* mNativeInstance;
};

#endif // __CCAMERA_H__
