
#ifndef __CTYPEFACE_H__
#define __CTYPEFACE_H__

#include "_CTypeface.h"
#include "graphics/Typeface.h"

using namespace Elastos;

/**
 * The Typeface class specifies the typeface and intrinsic style of a font.
 * This is used in the paint, along with optionally Paint settings like
 * textSize, textSkewX, textScaleX to specify
 * how text appears when drawn (and measured).
 */
CarClass(CTypeface), public Typeface
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetStyle(
        /* [out] */ Int32* style);

    CARAPI IsBold(
        /* [out] */ Boolean* isBold);

    CARAPI IsItalic(
        /* [out] */ Boolean* isItalic);

    CARAPI GetStyle(
        /* [in] */ Int32 native_instance,
        /* [out] */ Int32 *nStyle);
};

#endif //__CTYPEFACE_H__
