
#include "ext/frameworkdef.h"
#include "graphics/CBitmapShader.h"
#include "graphics/CBitmap.h"

ECode CBitmapShader::GetLocalMatrix(
    /* [in ,out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CBitmapShader::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

ECode CBitmapShader::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ShaderTileMode tileX,
    /* [in] */ ShaderTileMode tileY)
{
    mBitmap = bitmap;
    mNative = NativeCreate(((CBitmap*)bitmap)->Ni(), tileX, tileY);
    return NOERROR;
}

SkShader* CBitmapShader::NativeCreate(
    /* [in] */ SkBitmap* nBitmap,
    /* [in] */ Int32 tileModeX,
    /* [in] */ Int32 tileModeY)
{
    SkShader* s = SkShader::CreateBitmapShader(*nBitmap,
            (SkShader::TileMode)tileModeX,
            (SkShader::TileMode)tileModeY);
    return s;
}
