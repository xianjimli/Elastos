
#include "graphics/Shader.h"
#include "graphics/CMatrix.h"

// {AE795BA0-60FF-4BED-ADF2-9BDE205AB72F}
extern "C" const InterfaceID EIID_Shader =
    { 0xae795ba0, 0x60ff, 0x4bed, { 0xad, 0xf2, 0x9b, 0xde, 0x20, 0x5a, 0xb7, 0x2f } };


Shader::~Shader()
{
    NativeDestructor(mNative);
}

Boolean Shader::GetLocalMatrix(
    /* [in, out] */ IMatrix* localM)
{
    assert(localM != NULL);
    return NativeGetLocalMatrix(mNative, ((CMatrix*)localM)->mNativeInstance);
}

void Shader::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    NativeSetLocalMatrix(mNative,
        localM != NULL ? ((CMatrix*)localM)->mNativeInstance : NULL);
}

void Shader::NativeDestructor(
    /* [in] */ SkShader* shader)
{
    shader->safeUnref();
}

Boolean Shader::NativeGetLocalMatrix(
    /* [in] */ SkShader* shader,
    /* [in] */ SkMatrix* matrix)
{
    return shader ? shader->getLocalMatrix(matrix) : FALSE;
}

void Shader::NativeSetLocalMatrix(
    /* [in] */ SkShader* shader,
    /* [in] */ const SkMatrix* matrix)
{
    if (shader) {
        if (NULL == matrix) {
            shader->resetLocalMatrix();
        }
        else {
            shader->setLocalMatrix(*matrix);
        }
    }
}
