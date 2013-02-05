
#include "graphics/CCamera.h"
#include "graphics/CMatrix.h"
#include "graphics/CCanvas.h"


ECode CCamera::save()
{
   //Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);
    mView->save();
	 
    return NOERROR;
}

ECode CCamera::restore()
{
    // Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);
    mView->restore();
		
    return NOERROR;
}

ECode CCamera::translate(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
	//Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

	mView->translate(SkFloatToScalar(x), SkFloatToScalar(y), SkFloatToScalar(z));

    return NOERROR;
}

ECode CCamera::rotateX(
    /* [in] */ Float deg)
{
	//Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);
	
	mView->rotateX(SkFloatToScalar(deg));

    return NOERROR;
}

ECode CCamera::rotateY(
    /* [in] */ Float deg)
{
	//Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);
	
	mView->rotateY(SkFloatToScalar(deg));

    return NOERROR;
}

ECode CCamera::rotateZ(
    /* [in] */ Float deg)
{
	//Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

	mView->rotateZ(SkFloatToScalar(deg));

    return NOERROR;
}

ECode CCamera::getMatrix(
    /* [in] */ IMatrix * pMatrix)
{
    nativeGetMatrix((Int32)((CMatrix *)pMatrix)->Ni());
    return NOERROR;
}

ECode CCamera::applyToCanvas(
    /* [in] */ ICanvas * pCanvas)
{
    nativeApplyToCanvas((Int32)((CCanvas*)pCanvas)->mNativeCanvas);
    return NOERROR;
}

ECode CCamera::dotWithNormal(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Float dz,
    /* [out] */ Float * pResult)
{
	//Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);
	
	SkScalar dot = mView->dotWithNormal(SkFloatToScalar(dx), SkFloatToScalar(dy),
																	SkFloatToScalar(dz));
	*pResult = SkScalarToFloat(dot);

    return NOERROR;
}

ECode CCamera::nativeConstructor()
{
	mView = new Sk3DView;
	//env->SetIntField(obj, gNativeInstanceFieldID, (int)view);

    return NOERROR;
}

ECode CCamera::nativeDestructor()
{
	delete mView;
	
    return NOERROR;
}

ECode CCamera::nativeGetMatrix(
    /* [in] */ Int32 native_matrix)
{
	//Sk3DView* v = (Sk3DView*)env->GetIntField(obj, gNativeInstanceFieldID);

	mView->getMatrix((SkMatrix*)native_matrix);

    return NOERROR;
}

ECode CCamera::nativeApplyToCanvas(
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
	
    nativeConstructor();
    return NOERROR;
}

