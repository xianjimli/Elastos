
#include "graphics/CRadialGradient.h"
#include <SkGradientShader.h>
#include <SkTemplates.h>


ECode CRadialGradient::GetLocalMatrix(
    /* [in ,out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CRadialGradient::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

ECode CRadialGradient::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    if (radius <= 0) {
//        throw new IllegalArgumentException("radius must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (colors.GetLength() < 2) {
//        throw new IllegalArgumentException("needs >= 2 number of colors");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (positions != NULL && colors.GetLength() != positions->GetLength()) {
//        throw new IllegalArgumentException("color and position arrays must be of equal length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNative = NativeCreate1(x, y, radius, colors, positions, tile);
    return NOERROR;
}

ECode CRadialGradient::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tile)
{
    if (radius <= 0) {
//        throw new IllegalArgumentException("radius must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNative = NativeCreate2(x, y, radius, color0, color1, tile);
    return NOERROR;
}

SkShader* CRadialGradient::NativeCreate1(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    SkPoint center;
    center.set(SkFloatToScalar(x), SkFloatToScalar(y));

    size_t count = (size_t)colors.GetLength();
    const Int32* colorValues = colors.GetPayload();

    SkAutoSTMalloc<8, SkScalar> storage(positions ? count : 0);
    SkScalar* pos = NULL;

    if (positions) {
        const Float* posValues = positions->GetPayload();
        pos = (SkScalar*)storage.get();
        for (size_t i = 0; i < count; i++)
            pos[i] = SkFloatToScalar(posValues[i]);
    }

    SkShader* shader = SkGradientShader::CreateRadial(center,
            SkFloatToScalar(radius),
            reinterpret_cast<const SkColor*>(colorValues),
            pos, count,
            static_cast<SkShader::TileMode>(tile));

    assert(shader != NULL);
    return shader;
}

SkShader* CRadialGradient::NativeCreate2(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tile)
{
    SkPoint center;
    center.set(SkFloatToScalar(x), SkFloatToScalar(y));

    SkColor colors[2];
    colors[0] = color0;
    colors[1] = color1;

    SkShader* s = SkGradientShader::CreateRadial(center, SkFloatToScalar(radius), colors, NULL,
            2, (SkShader::TileMode)tile);
    assert(s != NULL);
    return s;
}
