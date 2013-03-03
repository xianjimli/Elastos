
#include "graphics/CColor.h"
#include <elastos/Math.h>
#include <skia/core/SkScalar.h>
#include <skia/core/SkColor.h>

using namespace Elastos::Core;


static HashMap<String, Int32>* InitColorNameMap()
{
    HashMap<String, Int32>* map = new HashMap<String, Int32>(11);
    assert(map != NULL);
    (*map)[String("black")] = COLOR_BLACK;
    (*map)[String("darkgray")] = COLOR_DKGRAY;
    (*map)[String("gray")] = COLOR_GRAY;
    (*map)[String("lightgray")] = COLOR_LTGRAY;
    (*map)[String("white")] = COLOR_WHITE;
    (*map)[String("red")] = COLOR_RED;
    (*map)[String("green")] = COLOR_GREEN;
    (*map)[String("blue")] = COLOR_BLUE;
    (*map)[String("yellow")] = COLOR_YELLOW;
    (*map)[String("cyan")] = COLOR_CYAN;
    (*map)[String("magenta")] = COLOR_MAGENTA;
    return map;
}

HashMap<String, Int32>* CColor::sColorNameMap = InitColorNameMap();

/**
* @param pAlpha: the alpha component of a color int. This is the same as saying
* color >>> 24
*/
ECode CColor::Alpha(
    /* [in] */ Int32 color,
    /* [out] */ Int32* alpha)
{
    *alpha = color >> 24;
    return NOERROR;
}

/**
* @param pRed: the red component of a color int. This is the same as saying
* (color >> 16) & 0xFF
*/
ECode CColor::Red(
    /* [in] */ Int32 color,
    /* [out] */ Int32* red)
{
    *red = (color >> 16) & 0xFF;
    return NOERROR;
}

/**
* @param pGreen: the green component of a color int. This is the same as saying
* (color >> 8) & 0xFF
*/
ECode CColor::Green(
    /* [in] */ Int32 color,
    /* [out] */ Int32* green)
{
    *green = (color >> 8) & 0xFF;
    return NOERROR;
}

/**
* Return the blue component of a color int. This is the same as saying
* color & 0xFF
*/
ECode CColor::Blue(
    /* [in] */ Int32 color,
    /* [out] */ Int32* blue)
{
    *blue = color & 0xFF;
    return NOERROR;
}

/**
* Return a color-int from red, green, blue components.
* The alpha component is implicity 255 (fully opaque).
* These component values should be [0..255], but there is no
* range check performed, so if they are out of range, the
* returned color is undefined.
* @param red  Red component [0..255] of the color
* @param green Green component [0..255] of the color
* @param blue  Blue component [0..255] of the color
* @param pRgb  the rgb value
*/
ECode CColor::Rgb(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ Int32* rgb)
{
    *rgb = (0xFF << 24) | (red << 16) | (green << 8) | blue;
    return NOERROR;
}

/**
* Return a color-int from alpha, red, green, blue components.
* These component values should be [0..255], but there is no
* range check performed, so if they are out of range, the
* returned color is undefined.
* @param alpha Alpha component [0..255] of the color
* @param red   Red component [0..255] of the color
* @param green Green component [0..255] of the color
* @param blue  Blue component [0..255] of the color
* @param pArgb  the argb value
*/
ECode CColor::Argb(
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ Int32* argb)
{
    *argb = (alpha << 24) | (red << 16) | (green << 8) | blue;
    return NOERROR;
}

/**
* Returns the hue component of a color int.
*
* @param pH: A value between 0.0f and 1.0f
*
* @hide Pending API council
*/
ECode CColor::Hue(
    /* [in] */ Int32 color,
    /* [out] */ Float* hue)
{
    Int32 r = (color >> 16) & 0xFF;
    Int32 g = (color >> 8) & 0xFF;
    Int32 b = color & 0xFF;

    Int32 V = Math::Max(b, Math::Max(r, g));
    Int32 temp = Math::Min(b, Math::Min(r, g));

    Float H;

    if (V == temp) {
        H = 0;
    }
    else {
        const Float vtemp = (Float) (V - temp);
        const Float cr = (V - r) / vtemp;
        const Float cg = (V - g) / vtemp;
        const Float cb = (V - b) / vtemp;

        if (r == V) {
            H = cb - cg;
        }
        else if (g == V) {
            H = 2 + cr - cb;
        }
        else {
            H = 4 + cg - cr;
        }

        H /= 6.f;
        if (H < 0) {
            H++;
        }
    }

    *hue = H;
    return NOERROR;
}

/**
* Returns the saturation component of a color int.
*
* @Param pS: value between 0.0f and 1.0f
*
* @hide Pending API council
*/
ECode CColor::Saturation(
    /* [in] */ Int32 color,
    /* [out] */ Float* saturation)
{
    Int32 r = (color >> 16) & 0xFF;
    Int32 g = (color >> 8) & 0xFF;
    Int32 b = color & 0xFF;


    Int32 V = Math::Max(b, Math::Max(r, g));
    Int32 temp = Math::Min(b, Math::Min(r, g));

    Float S;

    if (V == temp) {
        S = 0;
    }
    else {
        S = (V - temp) / (Float) V;
    }

    *saturation = S;
    return NOERROR;
}

/**
* Returns the brightness component of a color int.
*
* @Param pB: A value between 0.0f and 1.0f
*
* @hide Pending API council
*/
ECode CColor::Brightness(
    /* [in] */ Int32 color,
    /* [out] */ Float* brightness)
{
    Int32 r = (color >> 16) & 0xFF;
    Int32 g = (color >> 8) & 0xFF;
    Int32 b = color & 0xFF;

    Int32 V = Math::Max(b, Math::Max(r, g));

    *brightness = (V / 255.f);
    return NOERROR;
}

/**
* Parse the color string, and return the corresponding color-int.
* If the string cannot be parsed, throws an IllegalArgumentException
* exception. Supported formats are:
* #RRGGBB
* #AARRGGBB
* 'red', 'blue', 'green', 'black', 'white', 'gray', 'cyan', 'magenta',
* 'yellow', 'lightgray', 'darkgray'
*/
ECode CColor::ParseColor(
    /* [in] */ const String& colorString,
    /* [out] */ Int32* color)
{
    // if (colorString.charAt(0) == '#') {
    //     // Use a long to avoid rollovers on #ffXXXXXX
    //     long color = Long.parseLong(colorString.substring(1), 16);
    //     if (colorString.length() == 7) {
    //         // Set the alpha value
    //         color |= 0x00000000ff000000;
    //     } else if (colorString.length() != 9) {
    //         throw new IllegalArgumentException("Unknown color");
    //     }
    //     return (int)color;
    // } else {
    //     Integer color = sColorNameMap.get(colorString.toLowerCase(Locale.US));
    //     if (color != null) {
    //         return color;
    //     }
    // }
    // throw new IllegalArgumentException("Unknown color");
    return E_NOT_IMPLEMENTED;
}

/**
* Convert HSB components to an ARGB color. Alpha set to 0xFF.
*     hsv[0] is Hue [0 .. 1)
*     hsv[1] is Saturation [0...1]
*     hsv[2] is Value [0...1]
* If hsv values are out of range, they are pinned.
* @param hsb  3 element array which holds the input HSB components.
* @param pColor : the resulting argb color
*
* @hide Pending API council
*/
ECode CColor::HSBtoColor(
    /* [in] */ const ArrayOf<Float>& hsb,
    /* [out] */ Int32* color)
{
    return HSBtoColorEx(hsb[0], hsb[1], hsb[2], color);
}

/**
* Convert HSB components to an ARGB color. Alpha set to 0xFF.
*     hsv[0] is Hue [0 .. 1)
*     hsv[1] is Saturation [0...1]
*     hsv[2] is Value [0...1]
* If hsv values are out of range, they are pinned.
* @param h Hue component
* @param s Saturation component
* @param b Brightness component
* @param pColor the resulting argb color
*
* @hide Pending API council
*/
ECode CColor::HSBtoColorEx(
    /* [in] */ Float h,
    /* [in] */ Float s,
    /* [in] */ Float b,
    /* [out] */ Int32* color)
{
    //TODO:
    // h = MathUtils.constrain(h, 0.0f, 1.0f);
    // s = MathUtils.constrain(s, 0.0f, 1.0f);
    // b = MathUtils.constrain(b, 0.0f, 1.0f);

    Float red = 0.0f;
    Float green = 0.0f;
    Float blue = 0.0f;

    Float hf = (h - (Int32) h) * 6.0f;
    Int32 ihf = (Int32) hf;
    Float f = hf - ihf;
    Float pv = b * (1.0f - s);
    Float qv = b * (1.0f - s * f);
    Float tv = b * (1.0f - s * (1.0f - f));

    switch (ihf) {
        case 0:         // Red is the dominant color
            red = b;
            green = tv;
            blue = pv;
            break;
        case 1:         // Green is the dominant color
            red = qv;
            green = b;
            blue = pv;
            break;
        case 2:
            red = pv;
            green = b;
            blue = tv;
            break;
        case 3:         // Blue is the dominant color
            red = pv;
            green = qv;
            blue = b;
            break;
        case 4:
            red = tv;
            green = pv;
            blue = b;
            break;
        case 5:         // Red is the dominant color
            red = b;
            green = pv;
            blue = qv;
            break;
    }

    *color = 0xFF000000 | (((Int32) (red * 255.0f)) << 16) |
            (((Int32) (green * 255.0f)) << 8) | ((Int32) (blue * 255.0f));
    return NOERROR;
}

/**
* Convert RGB components to HSV.
*     hsv[0] is Hue [0 .. 360)
*     hsv[1] is Saturation [0...1]
*     hsv[2] is Value [0...1]
* @param red  red component value [0..255]
* @param green  green component value [0..255]
* @param blue  blue component value [0..255]
* @param hsv  3 element array which holds the resulting HSV components.
*/
ECode CColor::RGBToHSV(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ ArrayOf<Float>* hsv)
{
    if (hsv->GetLength() < 3) {
        // throw new RuntimeException("3 components required for hsv");
        return E_RUNTIME_EXCEPTION;
    }

    NativeRGBToHSV(red, green, blue, hsv);
    return NOERROR;
}

/**
* Convert the argb color to its HSV components.
*     hsv[0] is Hue [0 .. 360)
*     hsv[1] is Saturation [0...1]
*     hsv[2] is Value [0...1]
* @param color the argb color to convert. The alpha component is ignored.
* @param hsv  3 element array which holds the resulting HSV components.
*/
ECode CColor::ColorToHSV(
    /* [in] */ Int32 color,
    /* [out] */ ArrayOf<Float>* hsv)
{
    return RGBToHSV((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, hsv);
}

/**
* Convert HSV components to an ARGB color. Alpha set to 0xFF.
*     hsv[0] is Hue [0 .. 360)
*     hsv[1] is Saturation [0...1]
*     hsv[2] is Value [0...1]
* If hsv values are out of range, they are pinned.
* @param hsv  3 element array which holds the input HSV components.
* @return the resulting argb color
*/
ECode CColor::HSVToColor(
    /* [in] */ const ArrayOf<Float>& hsv,
    /* [out] */ Int32* color)
{
    return HSVToColorEx(0xFF, hsv, color);
}

/**
* Convert HSV components to an ARGB color. The alpha component is passed
* through unchanged.
*     hsv[0] is Hue [0 .. 360)
*     hsv[1] is Saturation [0...1]
*     hsv[2] is Value [0...1]
* If hsv values are out of range, they are pinned.
* @param alpha the alpha component of the returned argb color.
* @param hsv  3 element array which holds the input HSV components.
* @param pColor :the resulting argb color
*/
ECode CColor::HSVToColorEx(
    /* [in] */ Int32 alpha,
    /* [in] */ const ArrayOf<Float>& hsv,
    /* [out] */ Int32* color)
{
    if (hsv.GetLength() < 3) {
        // throw new RuntimeException("3 components required for hsv");
        return E_RUNTIME_EXCEPTION;
    }
    *color = NativeHSVToColor(alpha, hsv);
    return NOERROR;
}

void CColor::NativeRGBToHSV(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ ArrayOf<Float>* hsvArray)
{
    SkScalar hsv[3];
    SkRGBToHSV(red, green, blue, hsv);

    SkASSERT(hsvArray);
    if (hsvArray->GetLength() < 3) {
        sk_throw();
    }

    Float* values = hsvArray->GetPayload();
    for (Int32 i = 0; i < 3; i++) {
        values[i] = SkScalarToFloat(hsv[i]);
    }
}

Int32 CColor::NativeHSVToColor(
    /* [in] */ Int32 alpha,
    /* [out] */ const ArrayOf<Float>& hsvArray)
{
    if (hsvArray.GetLength() < 3) {
        sk_throw();
        return 0;
    }

    Float* values = hsvArray.GetPayload();
    SkScalar hsv[3];

    for (Int32 i = 0; i < 3; i++) {
        hsv[i] = SkFloatToScalar(values[i]);
    }

    return SkHSVToColor(alpha, hsv);
}
