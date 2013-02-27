
#ifndef __CBOUNDARYPATCH_H__
#define __CBOUNDARYPATCH_H__

#include "_CBoundaryPatch.h"
#include <elastos/AutoFree.h>

CarClass(CBoundaryPatch)
{
public:
    CARAPI SetCubicBoundary(
        /* [in] */ const ArrayOf<Float> & pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 rows,
        /* [in] */ Int32 cols);

    CARAPI SetTexture(
        /* [in] */ IBitmap * pTexture);

    CARAPI GetPaintFlags(
        /* [out] */ Int32 * pFlags);

    CARAPI SetPaintFlags(
        /* [in] */ Int32 flags);

    CARAPI SetXfermode(
        /* [in] */ IXfermode * pMode);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI Draw(
        /* [in] */ ICanvas * pCanvas);

    CARAPI BuildCache();

    CARAPI constructor(
        /* [in] */ Int32 decoder);

private:
    IPaint*   mPaint;
    IBitmap*  mTexture;
    Int32     mRows;
    Int32     mCols;
    AutoFree< ArrayOf<Float> > mCubicPoints;
    Boolean mDirty;
    // these are the computed output of the native code
    AutoFree< ArrayOf<Float> > mVerts;
    AutoFree< ArrayOf<Int16> > mIndices;

    static void NativeComputeCubicPatch(
        /* [in] */ AutoFree< ArrayOf<Float> > cubicPoints,
        /* [in] */ Int32 texW,
        /* [in] */ Int32 texH,
        /* [in] */ Int32 rows,
        /* [in] */ Int32 cols,
        /* [in] */ AutoFree< ArrayOf<Float> > verts,
        /* [in] */ AutoFree< ArrayOf<Int16> > indices);
};

#endif // __CBOUNDARYPATCH_H__
