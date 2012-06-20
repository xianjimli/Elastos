
#include "graphics/CLinearGradient.h"
#include <SkGradientShader.h>
#include <SkTemplates.h>

PInterface CLinearGradient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    return _CLinearGradient::Probe(riid);
}

ECode CLinearGradient::GetLocalMatrix(
    /* [in, out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(localM);
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CLinearGradient::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

ECode CLinearGradient::constructor(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ const ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    if (colors.GetLength() < 2) {
//        throw new IllegalArgumentException("needs >= 2 number of colors");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (positions != NULL && colors.GetLength() != positions->GetLength()) {
//        throw new IllegalArgumentException("color and position arrays must be of equal length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeCreate1(x0, y0, x1, y1, colors, positions, tile, &mNative);
}

ECode CLinearGradient::constructor(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tile)
{
    return NativeCreate2(x0, y0, x1, y1, color0, color1, tile, &mNative);
}

ECode CLinearGradient::NativeCreate1(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ const ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tileMode,
    /* [out] */ SkShader** shader)
{
    assert(shader != NULL);

    SkPoint pts[2];
    pts[0].set(SkFloatToScalar(x0), SkFloatToScalar(y0));
    pts[1].set(SkFloatToScalar(x1), SkFloatToScalar(y1));

    size_t count = colors.GetLength();

    SkAutoSTMalloc<8, SkScalar> storage(positions ? count : 0);
    SkScalar* pos = NULL;

    if (positions) {
        assert((size_t)positions->GetLength() >= count);
        const float* posValues = positions->GetPayload();
        pos = (SkScalar*)storage.get();
        for (size_t i = 0; i < count; i++) {
            pos[i] = SkFloatToScalar(posValues[i]);
        }
    }

    *shader = SkGradientShader::CreateLinear(pts,
            reinterpret_cast<const SkColor*>(colors.GetPayload()),
            pos, count,
            static_cast<SkShader::TileMode>(tileMode));
    if (*shader == NULL) {
//        doThrow(env, "java/lang/IllegalArgumentException", msg);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CLinearGradient::NativeCreate2(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tileMode,
    /* [out] */ SkShader** shader)
{
    assert(shader != NULL);

    SkPoint pts[2];
    pts[0].set(SkFloatToScalar(x0), SkFloatToScalar(y0));
    pts[1].set(SkFloatToScalar(x1), SkFloatToScalar(y1));

    SkColor colors[2];
    colors[0] = color0;
    colors[1] = color1;

    *shader = SkGradientShader::CreateLinear(pts, colors, NULL, 2,
            (SkShader::TileMode)tileMode);
    if (*shader == NULL) {
//        doThrow(env, "java/lang/IllegalArgumentException", msg);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}
