
#include "graphics/CBoundaryPatch.h"
#include "graphics/CPaint.h"
#include "graphics/CBitmapShader.h"
#include <skia/core/SkTypes.h>
#include <skia/utils/SkBoundaryPatch.h>
#include <skia/utils/SkMeshUtils.h>

/**
* Set the boundary to be 4 cubics. This takes a single array of floats,
* and picks up the 12 pairs starting at offset, and treats them as
* the x,y coordinates of the cubic control points. The points wrap around
* a patch, as follows. For documentation purposes, pts[i] will mean the
* x,y pair of floats, as if pts[] were an array of "points".
*
* Top: pts[0..3]
* Right: pts[3..6]
* Bottom: pts[6..9]
* Right: pts[9..11], pts[0]
*
* The coordinates are copied from the input array, so subsequent changes
* to pts[] will not be reflected in the boundary.
*
* @param pts The src array of x,y pairs for the boundary cubics
* @param offset The index into pts of the first pair
* @param rows The number of points across to approximate the boundary.
*             Must be >= 2, though very large values may slow down drawing
* @param cols The number of points down to approximate the boundary.
*             Must be >= 2, though very large values may slow down drawing
*/
ECode CBoundaryPatch::setCubicBoundary(
    /* [in] */ const ArrayOf<Float> & pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 rows,
    /* [in] */ Int32 cols)
{
    if (rows < 2 || cols < 2) {
        return E_RUNTIME_EXCEPTION;
    }

    memcpy(mCubicPoints, &pts, sizeof(Float) * 24);

    if (mRows != rows || mCols != cols) {
        mRows = rows;
        mCols = cols;
    }

    mDirty = true;
    return NOERROR;
}


/**
* Reference a bitmap texture to be mapped onto the patch.
*/
ECode CBoundaryPatch::setTexture(
    /* [in] */ IBitmap * pTexture)
{
    if (mTexture != pTexture) {

        Int32 mtwidth;
        Int32 mtheight;
        Int32 ptwidth;
        Int32 ptheight;

        mTexture->GetWidth(&mtwidth);
        mTexture->GetHeight(&mtheight);

        pTexture->GetWidth(&ptwidth);
        pTexture->GetHeight(&ptheight);

        if (mTexture == NULL ||
            mtwidth != ptwidth ||
            mtheight != ptheight) {
        // need to recompute pTexture coordinates
            mDirty = true;
        }

        mTexture = pTexture;
        IShader* pIs = NULL;
        CBitmapShader::New(pTexture,
                            ShaderTileMode_CLAMP,
                            ShaderTileMode_CLAMP,
                            (IShader**) &pIs);
        mPaint->SetShader(pIs);
    }
    return NOERROR;
}


/**
* @param pFlags the paint flags for the patch
*/
ECode CBoundaryPatch::getPaintFlags(
    /* [out] */ Int32 * pFlags)
{
    mPaint->GetFlags(pFlags);
    return NOERROR;
}

/**
* Set the paint flags for the patch
*/
ECode CBoundaryPatch::setPaintFlags(
    /* [in] */ Int32 flags)
{
    mPaint->SetFlags(flags);
    return NOERROR;
}

/**
* Set the xfermode for the patch
*/
ECode CBoundaryPatch::setXfermode(
    /* [in] */ IXfermode * pMode)
{
    mPaint->SetXfermode(pMode);
    return NOERROR;
}


/**
* Set the alpha for the patch
*/
ECode CBoundaryPatch::setAlpha(
    /* [in] */ Int32 alpha)
{
    mPaint->SetAlpha(alpha);
    return NOERROR;
}


/**
* Draw the patch onto the canvas.
*
* setCubicBoundary() and setTexture() must be called before drawing.
*/
ECode CBoundaryPatch::draw(
    /* [in] */ ICanvas * pCanvas)
{
    if (mDirty) {
        buildCache();
        mDirty = false;
    }

    // cut the count in half, since mVerts.length is really the length of
    // the verts[] and tex[] arrays combined
    // (tex[] are stored after verts[])
    Int32 mvlength = (*mVerts).GetLength();
    Int32 milength = (*mIndices).GetLength();
    Int32 vertCount = mvlength >> 1;
    pCanvas->DrawVertices(CanvasVertexMode_TRIANGLES, vertCount,
                    *mVerts, 0, mVerts, vertCount, NULL, 0,
                    mIndices, 0, milength,
                    mPaint);
    return NOERROR;
}


ECode CBoundaryPatch::buildCache()
{
    // we need mRows * mCols points, for verts and another set for textures
    // so *2 for going from points -> floats, and *2 for verts and textures
    Int32 vertCount = mRows * mCols * 4;
    Int32 mvlength = (*mVerts).GetLength();

    if (mVerts == NULL || mvlength != vertCount) {
        mVerts = ArrayOf<Float>::Alloc(vertCount);
    }

    Int32 indexCount = (mRows - 1) * (mCols - 1) * 6;
    Int32 milength = (*mIndices).GetLength();
    if (mIndices == NULL || milength != indexCount) {
        mIndices = ArrayOf<Int16>::Alloc(indexCount);
    }

    Int32 mtwidth;
    Int32 mtheight;
    mTexture->GetWidth(&mtwidth);
    mTexture->GetHeight(&mtheight);

    nativeComputeCubicPatch(mCubicPoints,
                        mtwidth, mtheight,
                        mRows, mCols, mVerts, mIndices);
    return NOERROR;
}

ECode CBoundaryPatch::constructor(
    /* [in] */ Int32 decoder)
{
    mRows = mCols = 2;  // default minimum
    mCubicPoints = ArrayOf<Float>::Alloc(24);
    CPaint::New((IPaint**)&mPaint);
    if (mPaint) {
        //((CPaint*)mPaint)->setDither(true);
        //((CPaint*)mPaint)->setFilterBitmap(true);
    }

    mDirty = true;
    return NOERROR;
}

void CBoundaryPatch::nativeComputeCubicPatch(
                                        AutoFree< ArrayOf<Float> > cubicPoints,
                                        Int32 texW,
                                        Int32 texH,
                                        Int32 rows,
                                        Int32 cols,
                                        AutoFree< ArrayOf<Float> > verts,
                                        AutoFree< ArrayOf<Int16> > indices)
{
    if (*cubicPoints) {
        Int32 mclength = (*cubicPoints).GetLength();
        if (mclength < 24) {
            SkASSERT(0);
        }
    }

    Int32 vertCount = rows * cols;

    if (*verts) {
        Int32 mvlength = (*verts).GetLength();
        if (mvlength < vertCount) {
            SkASSERT(0);
        }
    }

    SkPoint* pVerts = (SkPoint*)((*verts).GetPayload());
    SkPoint* ptexs = pVerts + vertCount;

    Int32 idxCount = (rows - 1) * (cols - 1) * 6;

    if (*indices) {
        Int32 milength = (*indices).GetLength();
        if (milength < idxCount) {
            SkASSERT(0);
        }
    }

    UInt16* idx = (UInt16*)((*indices).GetPayload());

    SkCubicBoundary cubic;
    memcpy(cubic.fPts, *cubicPoints, 12 * sizeof(SkPoint));

    SkBoundaryPatch patch;
    patch.setBoundary(&cubic);
    // generate our verts
    patch.evalPatch(pVerts, rows, cols);

    SkMeshIndices mesh;
    // generate our texs and idx
    mesh.init(ptexs, idx, texW, texH, rows, cols);
}