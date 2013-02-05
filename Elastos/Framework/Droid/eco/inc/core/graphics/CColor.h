
#ifndef __CCOLOR_H__
#define __CCOLOR_H__

#include "_CColor.h"

CarClass(CColor)
{
public:
    CARAPI alpha(
        /* [in] */ Int32 color,
        /* [out] */ Int32 * pAlpha);

    CARAPI red(
        /* [in] */ Int32 color,
        /* [out] */ Int32 * pRed);

    CARAPI green(
        /* [in] */ Int32 color,
        /* [out] */ Int32 * pGreen);

    CARAPI blue(
        /* [in] */ Int32 color,
        /* [out] */ Int32 * pBlue);

    CARAPI rgb(
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [out] */ Int32 * pRgb);

    CARAPI argb(
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [out] */ Int32 * pArgb);

    CARAPI hue(
        /* [in] */ Int32 color,
        /* [out] */ Float * pH);

    CARAPI saturation(
        /* [in] */ Int32 color,
        /* [out] */ Float * pS);

    CARAPI brightness(
        /* [in] */ Int32 color,
        /* [out] */ Float * pB);

    CARAPI parseColor(
        /* [in] */ Int32 colorString,
        /* [out] */ Int32 * pColor);

    CARAPI HSBtoColor(
        /* [in] */ const ArrayOf<Float> & hsb,
        /* [out] */ Int32 * pColor);

    CARAPI HSBtoColorEx(
        /* [in] */ Float h,
        /* [in] */ Float s,
        /* [in] */ Float b,
        /* [out] */ Int32 * pColor);

    CARAPI RGBToHSV(
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [out, callee] */ ArrayOf<Float> ** ppHsv);

    CARAPI colorToHSV(
        /* [in] */ Int32 color,
        /* [out, callee] */ ArrayOf<Float> ** ppHsv);

    CARAPI HSVToColor(
        /* [in] */ const ArrayOf<Float> & hsv,
        /* [out] */ Int32 * pColor);

    CARAPI HSVToColorEx(
        /* [in] */ Int32 alpha,
        /* [in] */ const ArrayOf<Float> & hsv,
        /* [out] */ Int32 * pColor);

    CARAPI constructor();

private:
    static CARAPI_(void) nativeRGBToHSV(Int32 red, Int32 green, Int32 blue, ArrayOf<Float> ** hsvArray);
    static CARAPI_(Int32) nativeHSVToColor(Int32 alpha, const ArrayOf<Float> & hsvArray);

    // HashMap<String, Integer> sColorNameMap;

    // static {
    //     sColorNameMap = new HashMap<String, Integer>();
    //     sColorNameMap.put("black", BLACK);
    //     sColorNameMap.put("darkgray", DKGRAY);
    //     sColorNameMap.put("gray", GRAY);
    //     sColorNameMap.put("lightgray", LTGRAY);
    //     sColorNameMap.put("white", WHITE);
    //     sColorNameMap.put("red", RED);
    //     sColorNameMap.put("green", GREEN);
    //     sColorNameMap.put("blue", BLUE);
    //     sColorNameMap.put("yellow", YELLOW);
    //     sColorNameMap.put("cyan", CYAN);
    //     sColorNameMap.put("magenta", MAGENTA);
    // }
};

#endif // __CCOLOR_H__