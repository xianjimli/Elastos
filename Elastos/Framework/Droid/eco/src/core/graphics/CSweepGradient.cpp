
#include "graphics/CSweepGradient.h"
#include <SkGradientShader.h>
#include <SkTemplates.h>


ECode CSweepGradient::GetLocalMatrix(
    /* [in ,out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CSweepGradient::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

/**
 * A subclass of Shader that draws a sweep gradient around a center point.
 *
 * @param cx       The x-coordinate of the center
 * @param cy       The y-coordinate of the center
 * @param colors   The colors to be distributed between around the center.
 *                 There must be at least 2 colors in the array.
 * @param positions May be NULL. The relative position of
 *                 each corresponding color in the colors array, beginning
 *                 with 0 and ending with 1.0. If the values are not
 *                 monotonic, the drawing may produce unexpected results.
 *                 If positions is NULL, then the colors are automatically
 *                 spaced evenly.
 */
ECode CSweepGradient::constructor(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions)
{
    if (colors.GetLength() < 2) {
//        throw new IllegalArgumentException("needs >= 2 number of colors");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (positions != NULL && colors.GetLength() != positions->GetLength()) {
//        throw new IllegalArgumentException(
//                    "color and position arrays must be of equal length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNative = NativeCreate1(cx, cy, colors, positions);
    return NOERROR;
}

/**
 * A subclass of Shader that draws a sweep gradient around a center point.
 *
 * @param cx       The x-coordinate of the center
 * @param cy       The y-coordinate of the center
 * @param color0   The color to use at the start of the sweep
 * @param color1   The color to use at the end of the sweep
 */
ECode CSweepGradient::constructor(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1)
{
    mNative = NativeCreate2(cx, cy, color0, color1);
    return NOERROR;
}

SkShader* CSweepGradient::NativeCreate1(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions)
{
    size_t count = (size_t)colors.GetLength();
    const Int32* colValues = colors.GetPayload();

    SkAutoSTMalloc<8, SkScalar> storage(positions ? count : 0);
    SkScalar* pos = NULL;

    if (NULL != positions) {
        const Float* posValues = positions->GetPayload();
        pos = (SkScalar*)storage.get();
        for (size_t i = 0; i < count; i++) {
            pos[i] = SkFloatToScalar(posValues[i]);
        }
    }

    SkShader* shader = SkGradientShader::CreateSweep(SkFloatToScalar(cx),
            SkFloatToScalar(cy),
            reinterpret_cast<const SkColor*>(colValues),
            pos, count);
    assert(shader != NULL);
    return shader;
}

SkShader* CSweepGradient::NativeCreate2(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1)
{
    SkColor colors[2];
    colors[0] = color0;
    colors[1] = color1;
    SkShader* s = SkGradientShader::CreateSweep(
            SkFloatToScalar(cx), SkFloatToScalar(cy), colors, NULL, 2);
    assert(s != NULL);
    return s;
}
