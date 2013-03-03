
#include "graphics/CComposeShader.h"
#include "graphics/Xfermode.h"
#include <skia/core/SkComposeShader.h>
#include <skia/effects/SkPorterDuff.h>


PInterface CComposeShader::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    return _CComposeShader::Probe(riid);
}

ECode CComposeShader::GetLocalMatrix(
    /* [in] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CComposeShader::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

ECode CComposeShader::constructor(
    /* [in] */ IShader* shaderA,
    /* [in] */ IShader* shaderB,
    /* [in] */ IXfermode* mode)
{
    mNative = NativeCreate1(
                ((Shader*)shaderA->Probe(EIID_Shader))->Ni(),
                ((Shader*)shaderB->Probe(EIID_Shader))->Ni(),
                (mode != NULL) ? ((Xfermode*)mode->Probe(EIID_Xfermode))->mNativeInstance : 0);
    return NOERROR;
}

ECode CComposeShader::constructor(
    /* [in] */ IShader* shaderA,
    /* [in] */ IShader* shaderB,
    /* [in] */ PorterDuffMode mode)
{
    mNative = NativeCreate2(
                ((Shader*)shaderA->Probe(EIID_Shader))->Ni(),
                ((Shader*)shaderB->Probe(EIID_Shader))->Ni(),
                mode);
    return NOERROR;
}

SkShader* CComposeShader::NativeCreate1(
    /* [in] */ SkShader* nativeShaderA,
    /* [in] */ SkShader * nativeShaderB,
    /* [in] */ SkXfermode * nativeMode)
{
    return new SkComposeShader(
                nativeShaderA,
                nativeShaderB,
                nativeMode);
}

SkShader* CComposeShader::NativeCreate2(
    /* [in] */ SkShader* nativeShaderA,
    /* [in] */ SkShader* nativeShaderB,
    /* [in] */ Int32 porterDuffMode)
{
    SkAutoUnref au(SkPorterDuff::CreateXfermode((SkPorterDuff::Mode)porterDuffMode));
    return new SkComposeShader(
                nativeShaderA,
                nativeShaderB,
                (SkXfermode*)au.get());
}

