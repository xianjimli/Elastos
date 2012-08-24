
#ifndef __CBITMAPSHADER_H__
#define __CBITMAPSHADER_H__

#include "_CBitmapShader.h"
#include "graphics/Shader.h"
#include <elastos/AutoPtr.h>
#include <SkBitmap.h>

CarClass(CBitmapShader), public Shader
{
public:
    /**
     * Return true if the shader has a non-identity local matrix.
     * @param localM If not null, it is set to the shader's local matrix.
     * @return true if the shader has a non-identity local matrix
     */
    CARAPI GetLocalMatrix(
        /* [in ,out] */ IMatrix* localM,
        /* [out] */ Boolean* result);

    /**
     * Set the shader's local matrix. Passing null will reset the shader's
     * matrix to identity
     * @param localM The shader's new local matrix, or null to specify identity
     */
    CARAPI SetLocalMatrix(
        /* [in] */ IMatrix* localM);

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ShaderTileMode tileX,
        /* [in] */ ShaderTileMode tileY);

private:
    static CARAPI_(SkShader*) NativeCreate(
        /* [in] */ SkBitmap* nBitmap,
        /* [in] */ Int32 tileModeX,
        /* [in] */ Int32 tileModeY);

private:
    // we hold on just for the GC, since our native counterpart is using it
    AutoPtr<IBitmap> mBitmap;
};

#endif //__CBITMAPSHADER_H__
