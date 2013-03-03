
#ifndef __CCOMPOSESHADER_H__
#define __CCOMPOSESHADER_H__

#include "_CComposeShader.h"
#include "graphics/Shader.h"


CarClass(CComposeShader), public Shader
{
public:
    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI GetLocalMatrix(
        /* [in] */ IMatrix* localM,
        /* [out] */ Boolean* result);

    CARAPI SetLocalMatrix(
        /* [in] */ IMatrix* localM);

    CARAPI constructor(
        /* [in] */ IShader* shaderA,
        /* [in] */ IShader* shaderB,
        /* [in] */ IXfermode* mode);

    CARAPI constructor(
        /* [in] */ IShader* shaderA,
        /* [in] */ IShader* shaderB,
        /* [in] */ PorterDuffMode mode);

private:
    static CARAPI_(SkShader*) NativeCreate1(
        /* [in] */ SkShader* nativeShaderA,
        /* [in] */ SkShader* nativeShaderB,
        /* [in] */ SkXfermode* nativeMode);

    static CARAPI_(SkShader*) NativeCreate2(
        /* [in] */ SkShader* nativeShaderA,
        /* [in] */ SkShader* nativeShaderB,
        /* [in] */ Int32 porterDuffMode);
};

#endif // __CCOMPOSESHADER_H__
