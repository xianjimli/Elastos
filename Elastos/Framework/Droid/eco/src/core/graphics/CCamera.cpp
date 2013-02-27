
#include "graphics/CCamera.h"
#include "graphics/CMatrix.h"
#include "graphics/CCanvas.h"


ECode CCamera::Save()
{
   //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);
    mView->save();

    return NOERROR;
}

ECode CCamera::Restore()
{
    // Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);
    mView->restore();

    return NOERROR;
}

ECode CCamera::Translate(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

    mView->translate(SkFloatToScalar(x), SkFloatToScalar(y), SkFloatToScalar(z));

    return NOERROR;
}

ECode CCamera::RotateX(
    /* [in] */ Float deg)
{
    //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

    mView->rotateX(SkFloatToScalar(deg));

    return NOERROR;
}

ECode CCamera::RotateY(
    /* [in] */ Float deg)
{
    //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

    mView->rotateY(SkFloatToScalar(deg));

    return NOERROR;
}

ECode CCamera::RotateZ(
    /* [in] */ Float deg)
{
    //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

    mView->rotateZ(SkFloatToScalar(deg));

    return NOERROR;
}

ECode CCamera::GetMatrix(
    /* [in] */ IMatrix * pMatrix)
{
    NativeGetMatrix((Int32)((CMatrix *)pMatrix)->Ni());
    return NOERROR;
}

ECode CCamera::ApplyToCanvas(
    /* [in] */ ICanvas * pCanvas)
{
    NativeApplyToCanvas((Int32)((CCanvas*)pCanvas)->mNativeCanvas);
    return NOERROR;
}

ECode CCamera::DotWithNormal(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Float dz,
    /* [out] */ Float * pResult)
{
    //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

    SkScalar dot = mView->dotWithNormal(
                            SkFloatToScalar(dx),
                            SkFloatToScalar(dy),
                            SkFloatToScalar(dz));

    *pResult = SkScalarToFloat(dot);

    return NOERROR;
}

ECode CCamera::NativeConstructor()
{
    mView = new Sk3DView;
    //env->SetIntField(obj, gNativeInstanceFieldID, (int)view);

    return NOERROR;
}

ECode CCamera::NativeDestructor()
{
    delete mView;

    return NOERROR;
}

ECode CCamera::NativeGetMatrix(
    /* [in] */ Int32 native_matrix)
{
    //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

    mView->getMatrix((SkMatrix*)native_matrix);

    return NOERROR;
}

ECode CCamera::NativeApplyToCanvas(
    /* [in] */ Int32 native_canvas)
{
    //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

    mView->applyToCanvas((SkCanvas*)native_canvas);

    return NOERROR;
}

ECode CCamera::constructor()
{
    // TODO: Add your code here
    native_instance = 0;
    mView = NULL;

    NativeConstructor();
    return NOERROR;
}

