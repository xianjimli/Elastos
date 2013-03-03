
#include "graphics/utils/CBoundaryPatch.h"
#include "graphics/CPaint.h"
#include "graphics/CBitmapShader.h"
#include <skia/core/SkTypes.h>
#include <skia/utils/SkBoundaryPatch.h>
#include <skia/utils/SkMeshUtils.h>


CBoundaryPatch::CBoundaryPatch()
    : mRows(2)
    , mCols(2)
    , mCubicPoints(ArrayOf<Float>::Alloc(24))
    , mDirty(TRUE)
{
    ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPaint));
    mPaint->SetDither(TRUE);
    mPaint->SetFilterBitmap(TRUE);
}

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
ECode CBoundaryPatch::SetCubicBoundary(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 rows,
    /* [in] */ Int32 cols)
{
    if (rows < 2 || cols < 2) {
        // throw new RuntimeException("rows and cols must be >= 2");
        return E_RUNTIME_EXCEPTION;
    }

    memcpy(mCubicPoints->GetPayload(), pts.GetPayload() + offset, sizeof(Float) * 24);
    if (mRows != rows || mCols != cols) {
        mRows = rows;
        mCols = cols;
    }
    mDirty = TRUE;
    return NOERROR;
}

/**
* Reference a bitmap texture to be mapped onto the patch.
*/
ECode CBoundaryPatch::SetTexture(
    /* [in] */ IBitmap* texture)
{
    if (mTexture.Get() != texture) {
        if (mTexture == NULL) {
            // need to recompute texture coordinates
            mDirty = TRUE;
        }
        else {
            assert(texture != NULL);
            Int32 width1, width2;
            Int32 height1, height2;
            mTexture->GetWidth(&width1);
            mTexture->GetHeight(&height1);
            texture->GetWidth(&width2);
            texture->GetHeight(&height2);
            if (width1 != width2 || height1 != height2) {
                // need to recompute pTexture coordinates
                mDirty = TRUE;
            }
        }
        mTexture = texture;
        AutoPtr<IShader> shader;
        FAIL_RETURN(CBitmapShader::New(texture,
                            ShaderTileMode_CLAMP,
                            ShaderTileMode_CLAMP,
                            (IShader**)&shader));
        mPaint->SetShader(shader);
    }
    return NOERROR;
}

/**
* @param pFlags the paint flags for the patch
*/
ECode CBoundaryPatch::GetPaintFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);

    return mPaint->GetFlags(flags);
}

/**
* Set the paint flags for the patch
*/
ECode CBoundaryPatch::SetPaintFlags(
    /* [in] */ Int32 flags)
{
    return mPaint->SetFlags(flags);
}

/**
* Set the xfermode for the patch
*/
ECode CBoundaryPatch::SetXfermode(
    /* [in] */ IXfermode* mode)
{
    return mPaint->SetXfermode(mode);
}

/**
* Set the alpha for the patch
*/
ECode CBoundaryPatch::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mPaint->SetAlpha(alpha);
}

/**
* Draw the patch onto the canvas.
*
* setCubicBoundary() and setTexture() must be called before drawing.
*/
ECode CBoundaryPatch::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mDirty) {
        BuildCache();
        mDirty = FALSE;
    }

    // cut the count in half, since mVerts.length is really the length of
    // the verts[] and tex[] arrays combined
    // (tex[] are stored after verts[])
    Int32 vertCount = mVerts->GetLength() >> 1;
    return canvas->DrawVertices(CanvasVertexMode_TRIANGLES, vertCount,
                    *mVerts, 0, mVerts, vertCount, NULL, 0,
                    mIndices, 0, mIndices->GetLength(),
                    mPaint);
}

void CBoundaryPatch::BuildCache()
{
    // we need mRows * mCols points, for verts and another set for textures
    // so *2 for going from points -> floats, and *2 for verts and textures
    Int32 vertCount = mRows * mCols * 4;
    if (mVerts == NULL || mVerts->GetLength() != vertCount) {
        mVerts = ArrayOf<Float>::Alloc(vertCount);
    }

    Int32 indexCount = (mRows - 1) * (mCols - 1) * 6;
    if (mIndices == NULL || mIndices->GetLength() != indexCount) {
        mIndices = ArrayOf<Int16>::Alloc(indexCount);
    }

    Int32 width;
    Int32 height;
    mTexture->GetWidth(&width);
    mTexture->GetHeight(&height);

    NativeComputeCubicPatch(mCubicPoints,
            width, height, mRows, mCols, mVerts, mIndices);
}

void CBoundaryPatch::NativeComputeCubicPatch(
    /* [in] */ ArrayOf<Float>* cubicPoints,
    /* [in] */ Int32 texW,
    /* [in] */ Int32 texH,
    /* [in] */ Int32 rows,
    /* [in] */ Int32 cols,
    /* [in] */ ArrayOf<Float>* _verts,
    /* [in] */ ArrayOf<Int16>* indices)
{
    if (cubicPoints != NULL) {
        if (cubicPoints->GetLength() < 24) {
            sk_throw();
        }
    }

    Int32 vertCount = rows * cols;
    if (_verts != NULL) {
        if (_verts->GetLength() < vertCount) {
            sk_throw();
        }
    }
    SkPoint* verts = (SkPoint*)_verts->GetPayload();
    SkPoint* texs = verts + vertCount;

    Int32 idxCount = (rows - 1) * (cols - 1) * 6;
    if (indices != NULL) {
        if (indices->GetLength() < idxCount) {
            sk_throw();
        }
    }
    uint16_t* idx = (uint16_t*)indices->GetPayload();

    SkCubicBoundary cubic;
    memcpy(cubic.fPts, cubicPoints->GetPayload(), 12 * sizeof(SkPoint));

    SkBoundaryPatch patch;
    patch.setBoundary(&cubic);
    // generate our verts
    patch.evalPatch(verts, rows, cols);

    SkMeshIndices mesh;
    // generate our texs and idx
    mesh.init(texs, idx, texW, texH, rows, cols);
}
