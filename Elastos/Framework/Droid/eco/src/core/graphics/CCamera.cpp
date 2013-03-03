
#include "graphics/CCamera.h"
#include "graphics/CMatrix.h"
#include "graphics/CCanvas.h"


CCamera::CCamera()
{
    NativeConstructor();
}

CCamera::~CCamera()
{
    NativeDestructor();
}

ECode CCamera::Save()
{
    mNativeInstance->save();
    return NOERROR;
}

ECode CCamera::Restore()
{
    mNativeInstance->restore();
    return NOERROR;
}

ECode CCamera::Translate(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    mNativeInstance->translate(SkFloatToScalar(x), SkFloatToScalar(y), SkFloatToScalar(z));
    return NOERROR;
}

ECode CCamera::RotateX(
    /* [in] */ Float deg)
{
    mNativeInstance->rotateX(SkFloatToScalar(deg));
    return NOERROR;
}

ECode CCamera::RotateY(
    /* [in] */ Float deg)
{
    mNativeInstance->rotateY(SkFloatToScalar(deg));
    return NOERROR;
}

ECode CCamera::RotateZ(
    /* [in] */ Float deg)
{
    mNativeInstance->rotateZ(SkFloatToScalar(deg));
    return NOERROR;
}

ECode CCamera::GetMatrix(
    /* [in] */ IMatrix* matrix)
{
    NativeGetMatrix(((CMatrix*)matrix)->Ni());
    return NOERROR;
}

ECode CCamera::ApplyToCanvas(
    /* [in] */ ICanvas* canvas)
{
    NativeApplyToCanvas(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas);
    return NOERROR;
}

ECode CCamera::DotWithNormal(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Float dz,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);

    SkScalar dot = mNativeInstance->dotWithNormal(
                            SkFloatToScalar(dx),
                            SkFloatToScalar(dy),
                            SkFloatToScalar(dz));
    *result = SkScalarToFloat(dot);
    return NOERROR;
}

void CCamera::NativeConstructor()
{
    mNativeInstance = new Sk3DView;
}

void CCamera::NativeDestructor()
{
    delete mNativeInstance;
}

void CCamera::NativeGetMatrix(
    /* [in] */ SkMatrix* nativeMatrix)
{
    mNativeInstance->getMatrix(nativeMatrix);
}

void CCamera::NativeApplyToCanvas(
    /* [in] */ SkCanvas* nativeCanvas)
{
    mNativeInstance->applyToCanvas(nativeCanvas);
}
