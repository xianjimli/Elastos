
#ifndef __CCOLOR_H__
#define __CCOLOR_H__

#include "ext/frameworkext.h"
#include "_CColor.h"
#include <elastos/HashMap.h>

/*
* The Color class defines methods for creating and converting color ints.
* Colors are represented as packed ints, made up of 4 bytes: alpha, red,
* green, blue. The values are unpremultiplied, meaning any transparency is
* stored solely in the alpha component, and not in the color components. The
* components are stored as follows (alpha << 24) | (red << 16) |
* (green << 8) | blue. Each component ranges between 0..255 with 0
* meaning no contribution for that component, and 255 meaning 100%
* contribution. Thus opaque-black would be 0xFF000000 (100% opaque but
* no contributions from red, green, or blue), and opaque-white would be
* 0xFFFFFFFF
*/
CarClass(CColor)
{
public:
    CARAPI Alpha(
        /* [in] */ Int32 color,
        /* [out] */ Int32* alpha);

    CARAPI Red(
        /* [in] */ Int32 color,
        /* [out] */ Int32* red);

    CARAPI Green(
        /* [in] */ Int32 color,
        /* [out] */ Int32* green);

    CARAPI Blue(
        /* [in] */ Int32 color,
        /* [out] */ Int32* blue);

    CARAPI Rgb(
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [out] */ Int32* rgb);

    CARAPI Argb(
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [out] */ Int32* argb);

    CARAPI Hue(
        /* [in] */ Int32 color,
        /* [out] */ Float* hue);

    CARAPI Saturation(
        /* [in] */ Int32 color,
        /* [out] */ Float* saturation);

    CARAPI Brightness(
        /* [in] */ Int32 color,
        /* [out] */ Float* brightness);

    CARAPI ParseColor(
        /* [in] */ const String& colorString,
        /* [out] */ Int32* color);

    CARAPI HSBtoColor(
        /* [in] */ const ArrayOf<Float>& hsb,
        /* [out] */ Int32* color);

    CARAPI HSBtoColorEx(
        /* [in] */ Float h,
        /* [in] */ Float s,
        /* [in] */ Float b,
        /* [out] */ Int32* color);

    CARAPI RGBToHSV(
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [out] */ ArrayOf<Float>* hsv);

    CARAPI ColorToHSV(
        /* [in] */ Int32 color,
        /* [out] */ ArrayOf<Float>* hsv);

    CARAPI HSVToColor(
        /* [in] */ const ArrayOf<Float>& hsv,
        /* [out] */ Int32* color);

    CARAPI HSVToColorEx(
        /* [in] */ Int32 alpha,
        /* [in] */ const ArrayOf<Float>& hsv,
        /* [out] */ Int32* color);

private:
    static CARAPI_(void) NativeRGBToHSV(
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [out] */ ArrayOf<Float>* hsv);

    static CARAPI_(Int32) NativeHSVToColor(
        /* [in] */ Int32 alpha,
        /* [in] */ const ArrayOf<Float>& hsv);

    CARAPI_(Float) Constrain(
        /* [in] */ Float amount,
        /* [in] */ Float low,
        /* [in] */ Float high);

private:
    static HashMap<String, Int32>* sColorNameMap;
};

#endif // __CCOLOR_H__
