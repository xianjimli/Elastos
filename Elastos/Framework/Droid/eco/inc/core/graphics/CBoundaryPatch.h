
#ifndef __CBOUNDARYPATCH_H__
#define __CBOUNDARYPATCH_H__

#include "_CBoundaryPatch.h"
#include <elastos/AutoFree.h>

CarClass(CBoundaryPatch)
{
public:
    CARAPI setCubicBoundary(
        /* [in] */ const ArrayOf<Float> & pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 rows,
        /* [in] */ Int32 cols);

    CARAPI setTexture(
        /* [in] */ IBitmap * pTexture);

    CARAPI getPaintFlags(
        /* [out] */ Int32 * pFlags);

    CARAPI setPaintFlags(
        /* [in] */ Int32 flags);

    CARAPI setXfermode(
        /* [in] */ IXfermode * pMode);

    CARAPI setAlpha(
        /* [in] */ Int32 alpha);

    CARAPI draw(
        /* [in] */ ICanvas * pCanvas);

    CARAPI buildCache();

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

    static void nativeComputeCubicPatch(
                                        AutoFree< ArrayOf<Float> > cubicPoints,
                                        Int32 texW,
                                        Int32 texH,
                                        Int32 rows,
                                        Int32 cols,
                                        AutoFree< ArrayOf<Float> > verts,
                                        AutoFree< ArrayOf<Int16> > indices);
};

#endif // __CBOUNDARYPATCH_H__
