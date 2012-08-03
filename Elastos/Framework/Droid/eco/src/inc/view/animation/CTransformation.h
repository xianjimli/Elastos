
#ifndef __CTRANSFORMATION_H__
#define __CTRANSFORMATION_H__

#include "_CTransformation.h"
#include <elastos/AutoPtr.h>

CarClass(CTransformation)
{
public:
    CARAPI constructor();

    CARAPI Clear();

    CARAPI GetTransformationType(
        /* [out] */ Int32* type);

    CARAPI SetTransformationType(
        /* [in] */ Int32 transformationType);

    CARAPI Set(
        /* [in] */ ITransformation* t);

    CARAPI Compose(
        /* [in] */ ITransformation* t);

    CARAPI GetMatrix(
        /* [out] */ IMatrix** matrix);

    CARAPI SetAlpha(
        /* [in] */ Float alpha);

    CARAPI GetAlpha(
        /* [out] */ Float* pAlpha);

protected:
    AutoPtr<IMatrix> mMatrix;
    Float mAlpha;
    Int32 mTransformationType;
};

#endif // __CTRANSFORMATION_H__
