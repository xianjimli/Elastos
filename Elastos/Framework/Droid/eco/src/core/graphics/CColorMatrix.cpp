
#include "graphics/CColorMatrix.h"
#include <elastos/Math.h>
#include <elastos/AutoFree.h>


using namespace Elastos::Core;

CColorMatrix::CColorMatrix()
{
    mArray = ArrayOf<Float>::Alloc(20);
}

CColorMatrix::~CColorMatrix()
{
    ArrayOf<Float>::Free(mArray);
}

/**
* @Parma ppPArray :the array of floats representing this colormatrix.
*/
ECode CColorMatrix::GetArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    *array = mArray->Clone();
    return NOERROR;
}

/**
* Set this colormatrix to identity:
* [ 1 0 0 0 0   - red vector
*   0 1 0 0 0   - green vector
*   0 0 1 0 0   - blue vector
*   0 0 0 1 0 ] - alpha vector
*/
ECode CColorMatrix::Reset()
{
    for (Int32 i = 19; i > 0; --i) {
        (*mArray)[i] = 0;
    }
    (*mArray)[0] = (*mArray)[6] = (*mArray)[12] = (*mArray)[18] = 1;
    return NOERROR;
}

/**
* Assign the src colormatrix into this matrix, copying all of its values.
*/
ECode CColorMatrix::Set(
    /* [in] */ IColorMatrix* src)
{
    memcpy(mArray->GetPayload(), ((CColorMatrix*)src)->mArray->GetPayload(), sizeof(Float) * 20);
    return NOERROR;
}

/**
* Assign the array of floats into this matrix, copying all of its values.
*/
ECode CColorMatrix::SetEx(
    /* [in] */ const ArrayOf<Float>& src)
{
    memcpy(mArray->GetPayload(), src.GetPayload(), sizeof(Float) * 20);
    return NOERROR;
}

/**
* Set this colormatrix to scale by the specified values.
*/
ECode CColorMatrix::SetScale(
    /* [in] */ Float rScale,
    /* [in] */ Float gScale,
    /* [in] */ Float bScale,
    /* [in] */ Float aScale)
{
    for (Int32 i = 19; i > 0; --i) {
        (*mArray)[i] = 0;
    }
    (*mArray)[0] = rScale;
    (*mArray)[6] = gScale;
    (*mArray)[12] = bScale;
    (*mArray)[18] = aScale;
    return NOERROR;
}

/**
* Set the rotation on a color axis by the specified values.
* axis=0 correspond to a rotation around the RED color
* axis=1 correspond to a rotation around the GREEN color
* axis=2 correspond to a rotation around the BLUE color
*/
ECode CColorMatrix::SetRotate(
    /* [in] */ Int32 axis,
    /* [in] */ Float degrees)
{
    Reset();
    Float radians = degrees * (Float)Math::DOUBLE_PI / 180;
    Float cosine = (Float)Math::Cos((Double)radians);
    Float sine = (Float)Math::Sin((Double)radians);
    switch (axis) {
    // Rotation around the red color
        case 0:
        (*mArray)[6] = (*mArray)[12] = cosine;
        (*mArray)[7] = sine;
        (*mArray)[11] = -sine;
        break;
        // Rotation around the green color
        case 1:
        (*mArray)[0] = (*mArray)[12] = cosine;
        (*mArray)[2] = -sine;
        (*mArray)[10] = sine;
        break;
        // Rotation around the blue color
        case 2:
        (*mArray)[0] = (*mArray)[6] = cosine;
        (*mArray)[1] = sine;
        (*mArray)[5] = -sine;
        break;
        default:
        // throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

/**
* Set this colormatrix to the concatenation of the two specified
* colormatrices, such that the resulting colormatrix has the same effect
* as applying matB and then applying matA. It is legal for either matA or
* matB to be the same colormatrix as this.
*/
ECode CColorMatrix::SetConcat(
    /* [in] */ IColorMatrix* matA,
    /* [in] */ IColorMatrix* matB)
{
    ArrayOf<Float>* tmp = NULL;

    if (matA == (IColorMatrix*)this || matB == (IColorMatrix*)this) {
        tmp = ArrayOf<Float>::Alloc(20);
    }
    else {
        tmp = mArray;
    }

    ArrayOf<Float>* a = ((CColorMatrix*)matA)->mArray;
    ArrayOf<Float>* b = ((CColorMatrix*)matB)->mArray;
    Int32 index = 0;
    for (Int32 j = 0; j < 20; j += 5) {
        for (Int32 i = 0; i < 4; i++) {
            (*tmp)[index++] = (*a)[j + 0] * (*b)[i + 0] +  (*a)[j + 1] * (*b)[i + 5] +
                           (*a)[j + 2] * (*b)[i + 10] + (*a)[j + 3] * (*b)[i + 15];
        }
        (*tmp)[index++] = (*a)[j + 0] * (*b)[4] +  (*a)[j + 1] * (*b)[9] +
                        (*a)[j + 2] * (*b)[14] + (*a)[j + 3] * (*b)[19] +
                        (*a)[j + 4];
    }

    if (tmp != mArray) {
        memcpy(mArray->GetPayload(), tmp->GetPayload(), sizeof(Float) * 20);
        ArrayOf<Float>::Free(tmp);
    }
    return NOERROR;
}

/**
* Concat this colormatrix with the specified prematrix. This is logically
* the same as calling setConcat(this, prematrix);
*/
ECode CColorMatrix::PreConcat(
    /* [in] */ IColorMatrix* prematrix)
{
    return SetConcat((IColorMatrix*)this, prematrix);
}

/**
* Concat this colormatrix with the specified postmatrix. This is logically
* the same as calling setConcat(postmatrix, this);
*/
ECode CColorMatrix::PostConcat(
    /* [in] */ IColorMatrix* postmatrix)
{
    return SetConcat(postmatrix, (IColorMatrix*)this);
}

/**
* Set the matrix to affect the saturation of colors. A value of 0 maps the
* color to gray-scale. 1 is identity.
*/
ECode CColorMatrix::SetSaturation(
    /* [in] */ Float sat)
{
    Reset();
    ArrayOf<Float>* m = mArray;

    Float invSat = 1 - sat;
    Float R = 0.213f * invSat;
    Float G = 0.715f * invSat;
    Float B = 0.072f * invSat;

    (*m)[0] = R + sat;      (*m)[1] = G;    (*m)[2] = B;
    (*m)[5] = R;    (*m)[6] = G + sat;  (*m)[7] = B;
    (*m)[10] = R;   (*m)[11] = G;       (*m)[12] = B + sat;
    return NOERROR;
}


/**
* Set the matrix to convert RGB to YUV
*/
ECode CColorMatrix::SetRGB2YUV()
{
    Reset();
    ArrayOf<Float>* m = mArray;
    // these coefficients match those in libjpeg
    (*m)[0]  = 0.299f;    (*m)[1]  = 0.587f;    (*m)[2]  = 0.114f;
    (*m)[5]  = -0.16874f; (*m)[6]  = -0.33126f; (*m)[7]  = 0.5f;
    (*m)[10] = 0.5f;      (*m)[11] = -0.41869f; (*m)[12] = -0.08131f;
    return NOERROR;
}

// /**
// * Set the matrix to convert from YUV to RGB
// */
ECode CColorMatrix::SetYUV2RGB()
{
    Reset();
    ArrayOf<Float>* m = mArray;
    // these coefficients match those in libjpeg
    (*m)[2] = 1.402f;
    (*m)[5] = 1;   (*m)[6] = -0.34414f;   (*m)[7] = -0.71414f;
    (*m)[10] = 1;  (*m)[11] = 1.772f;     (*m)[12] = 0;
    return NOERROR;
}

/**
*  5x4 matrix for transforming the color+alpha components of a Bitmap.
*  The matrix is stored in a single array, and its treated as follows:
*  [ a, b, c, d, e,
*    f, g, h, i, j,
*    k, l, m, n, o,
*    p, q, r, s, t ]
*
* When applied to a color [r, g, b, a], the resulting color is computed as
* (after clamping)
*   R' = a*R + b*G + c*B + d*A + e;
*   G' = f*R + g*G + h*B + i*A + j;
*   B' = k*R + l*G + m*B + n*A + o;
*   A' = p*R + q*G + r*B + s*A + t;
*/
ECode CColorMatrix::constructor()
{
    Reset();
    return NOERROR;
}

ECode CColorMatrix::constructor(
    /* [in] */ const ArrayOf<Float> & src)
{
    memcpy(mArray->GetPayload(), src.GetPayload(), sizeof(Float) * 20);
    return NOERROR;
}

ECode CColorMatrix::constructor(
    /* [in] */ IColorMatrix* array)
{
    memcpy(mArray->GetPayload(), ((CColorMatrix*)array)->mArray->GetPayload(), sizeof(Float) * 20);
    return NOERROR;
}
