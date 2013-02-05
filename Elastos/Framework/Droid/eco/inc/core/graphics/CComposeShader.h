
#ifndef __CCOMPOSESHADER_H__
#define __CCOMPOSESHADER_H__

#include "_CComposeShader.h"
#include "graphics/Shader.h"


CarClass(CComposeShader), public Shader
{
public:
    CARAPI GetLocalMatrix(
        /* [in] */ IMatrix * pLocalM,
        /* [out] */ Boolean * pResult);

    CARAPI SetLocalMatrix(
        /* [in] */ IMatrix * pLocalM);

    CARAPI constructor(
        /* [in] */ IShader * pShaderA,
        /* [in] */ IShader * pShaderB,
        /* [in] */ IXfermode * pMode);

    CARAPI constructor(
        /* [in] */ IShader * pShaderA,
        /* [in] */ IShader * pShaderB,
        /* [in] */ PorterDuffMode mode);

private:
    // TODO: Add your private member variables here.
    static CARAPI_(SkShader*) nativeCreate1(
        /* [in] */ SkShader * native_shaderA,
        /* [in] */ SkShader * native_shaderB,
        /* [in] */ SkXfermode * native_mode);

    static CARAPI_(SkShader*) nativeCreate2(
        /* [in] */ SkShader * native_shaderA,
        /* [in] */ SkShader * native_shaderB,
        /* [in] */ Int32 porterDuffMode);

};

#endif // __CCOMPOSESHADER_H__
