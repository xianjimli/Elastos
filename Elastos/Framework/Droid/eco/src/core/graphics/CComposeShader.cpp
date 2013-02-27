
#include "graphics/CComposeShader.h"
#include "graphics/Xfermode.h"
#include <skia/core/SkComposeShader.h>
#include <skia/effects/SkPorterDuff.h>

ECode CComposeShader::GetLocalMatrix(
    /* [in] */ IMatrix * pLocalM,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    VALIDATE_NOT_NULL(pResult);

    *pResult = Shader::GetLocalMatrix(pLocalM);

    return NOERROR;
}

ECode CComposeShader::SetLocalMatrix(
    /* [in] */ IMatrix * pLocalM)
{
    // TODO: Add your code here
    Shader::SetLocalMatrix(pLocalM);

    return NOERROR;
}

ECode CComposeShader::constructor(
    /* [in] */ IShader * pShaderA,
    /* [in] */ IShader * pShaderB,
    /* [in] */ IXfermode * pMode)
{
    // TODO: Add your code here
    mNative = NativeCreate1(
                ((Shader*)pShaderA)->Ni(),
                ((Shader*)pShaderB)->Ni(),
                (pMode != NULL) ? ((Xfermode*)pMode)->mNativeInstance : 0);

    return NOERROR;
}

ECode CComposeShader::constructor(
    /* [in] */ IShader * pShaderA,
    /* [in] */ IShader * pShaderB,
    /* [in] */ PorterDuffMode mode)
{
    // TODO: Add your code here

    mNative = NativeCreate2(
                ((Shader*)pShaderA)->Ni(),
                ((Shader*)pShaderB)->Ni(),
                mode);

    return NOERROR;
}

SkShader* CComposeShader::NativeCreate1(
    /* [in] */ SkShader * native_shaderA,
    /* [in] */ SkShader * native_shaderB,
    /* [in] */ SkXfermode * native_mode)
{
    return new SkComposeShader(
                native_shaderA,
                native_shaderB,
                native_mode);
}

SkShader* CComposeShader::NativeCreate2(
    /* [in] */ SkShader * native_shaderA,
    /* [in] */ SkShader * native_shaderB,
    /* [in] */ Int32 porterDuffMode)
{
    SkAutoUnref au(SkPorterDuff::CreateXfermode((SkPorterDuff::Mode)porterDuffMode));

    return new SkComposeShader(
                native_shaderA,
                native_shaderB,
                (SkXfermode*)au.get());
}

