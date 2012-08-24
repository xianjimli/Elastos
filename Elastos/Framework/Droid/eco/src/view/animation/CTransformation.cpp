
#include "view/animation/CTransformation.h"
#include "ext/frameworkdef.h"
#include "graphics/CMatrix.h"

ECode CTransformation::constructor()
{
    return Clear();
}

ECode CTransformation::Clear()
{
    if (mMatrix == NULL) {
        AutoPtr<CMatrix> matrix;
        CMatrix::NewByFriend((CMatrix**)&matrix);
        mMatrix = (IMatrix*)matrix.Get();
    }
    else {
        mMatrix->Reset();
    }
    mAlpha = 1.0f;
    mTransformationType = Transformation_TYPE_BOTH;

    return NOERROR;
}

ECode CTransformation::GetTransformationType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mTransformationType;

    return NOERROR;
}

ECode CTransformation::SetTransformationType(
    /* [in] */ Int32 transformationType)
{
    mTransformationType = transformationType;

    return NOERROR;
}

ECode CTransformation::Set(
    /* [in] */ ITransformation* t)
{
    t->GetAlpha(&mAlpha);
    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    mMatrix->Set(matrix);
    t->GetTransformationType(&mTransformationType);

    return NOERROR;
}

ECode CTransformation::Compose(
    /* [in] */ ITransformation* t)
{
    Float alpha;
    t->GetAlpha(&alpha);
    mAlpha *= alpha;

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    Boolean res;
    mMatrix->PreConcat(matrix, &res);

    return NOERROR;
}

ECode CTransformation::GetMatrix(
    /* [out] */ IMatrix** matrix)
{
    VALIDATE_NOT_NULL(matrix)
    *matrix = mMatrix;
    if (*matrix) {
        (*matrix)->AddRef();
    }

    return NOERROR;
}

ECode CTransformation::SetAlpha(
    /* [in] */ Float alpha)
{
    mAlpha = alpha;
    return NOERROR;
}

ECode CTransformation::GetAlpha(
    /* [out] */ Float* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = mAlpha;

    return NOERROR;
}
