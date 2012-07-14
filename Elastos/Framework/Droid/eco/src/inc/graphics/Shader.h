
#ifndef __SHADER_H__
#define __SHADER_H__

#include <sys/atomics.h>
#include <SkShader.h>
#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>

extern "C" const InterfaceID EIID_Shader;

class Shader
{
    friend class Paint;

public:
    ~Shader();

    /**
     * Return true if the shader has a non-identity local matrix.
     * @param localM If not null, it is set to the shader's local matrix.
     * @return true if the shader has a non-identity local matrix
     */
    CARAPI_(Boolean) GetLocalMatrix(
        /* [in, out] */ IMatrix* localM);

    /**
     * Set the shader's local matrix. Passing null will reset the shader's
     * matrix to identity
     * @param localM The shader's new local matrix, or null to specify identity
     */
    CARAPI_(void) SetLocalMatrix(
        /* [in] */ IMatrix* localM);

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkShader* shader);

    static CARAPI_(Boolean) NativeGetLocalMatrix(
        /* [in] */ SkShader* shader,
        /* [in] */ SkMatrix* matrix);

    static CARAPI_(void) NativeSetLocalMatrix(
        /* [in] */ SkShader* shader,
        /* [in] */ const SkMatrix* matrix);

protected:
    SkShader* mNative;
};

#endif //__SHADER_H__
