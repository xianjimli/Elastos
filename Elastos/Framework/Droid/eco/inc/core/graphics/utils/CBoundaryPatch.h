
#ifndef __CBOUNDARYPATCH_H__
#define __CBOUNDARYPATCH_H__

#include "_CBoundaryPatch.h"
#include <elastos/AutoFree.h>
#include <elastos/AutoPtr.h>

CarClass(CBoundaryPatch)
{
public:
    CBoundaryPatch();

    CARAPI SetCubicBoundary(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 rows,
        /* [in] */ Int32 cols);

    CARAPI SetTexture(
        /* [in] */ IBitmap* texture);

    CARAPI GetPaintFlags(
        /* [out] */ Int32* flags);

    CARAPI SetPaintFlags(
        /* [in] */ Int32 flags);

    CARAPI SetXfermode(
        /* [in] */ IXfermode* mode);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

private:
    CARAPI_(void) BuildCache();

    static CARAPI_(void) NativeComputeCubicPatch(
        /* [in] */ ArrayOf<Float>* cubicPoints,
        /* [in] */ Int32 texW,
        /* [in] */ Int32 texH,
        /* [in] */ Int32 rows,
        /* [in] */ Int32 cols,
        /* [in] */ ArrayOf<Float>* verts,
        /* [in] */ ArrayOf<Int16>* indices);

private:
    AutoPtr<IPaint> mPaint;
    AutoPtr<IBitmap> mTexture;
    Int32 mRows;
    Int32 mCols;
    AutoFree< ArrayOf<Float> > mCubicPoints;
    Boolean mDirty;
    // these are the computed output of the native code
    AutoFree< ArrayOf<Float> > mVerts;
    AutoFree< ArrayOf<Int16> > mIndices;
};

#endif // __CBOUNDARYPATCH_H__
