
#ifndef __CPATHMEASURE_H__
#define __CPATHMEASURE_H__

#include "_CPathMeasure.h"
#include "graphics/CPath.h"
#include "graphics/CMatrix.h"
#include <skia/core/SkPathMeasure.h>

struct PathMeasurePair {
    PathMeasurePair() {}
    PathMeasurePair(const SkPath& path, Boolean forceClosed)
        : fPath(path), fMeasure(fPath, forceClosed) {}

    SkPath          fPath;      // copy of the user's path
    SkPathMeasure   fMeasure;   // this guy points to fPath
};

CarClass(CPathMeasure)
{
public:
    CARAPI SetPath(
        /* [in] */ IPath * pPath,
        /* [in] */ Boolean forceClosed);

    CARAPI GetLength(
        /* [out] */ Float * pLength);

    CARAPI GetPosTan(
        /* [in] */ Float distance,
        /* [out, callee] */ ArrayOf<Float> ** ppPos,
        /* [out, callee] */ ArrayOf<Float> ** ppTan,
        /* [out] */ Boolean * pResult);

    CARAPI GetMatrix(
        /* [in] */ Float distance,
        /* [in] */ IMatrix * pMatrix,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean * pResult);

    CARAPI GetSegment(
        /* [in] */ Float startD,
        /* [in] */ Float stopD,
        /* [in] */ IPath * pDst,
        /* [in] */ Boolean startWithMoveTo,
        /* [out] */ Boolean * pResult);

    CARAPI IsClosed(
        /* [out] */ Boolean * pResult);

    CARAPI NextContour(
        /* [out] */ Boolean * pResult);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPath * pPath,
        /* [in] */ Boolean forceClosed);

private:
    static CARAPI_(PathMeasurePair*) Native_create(
        /* [in] */ SkPath* native_path,
        /* [in] */ Boolean forceClosed);

    static CARAPI_(void) Native_setPath(
        /* [in] */ PathMeasurePair* native_instance,
        /* [in] */ SkPath* native_path,
        /* [in] */ Boolean forceClosed);

    static CARAPI_(Float) Native_getLength(
        /* [in] */ PathMeasurePair* native_instance);

    static CARAPI_(void) ConvertTwoElemFloatArray(
        /* [out] */ ArrayOf<Float> * array,
        /* [in] */ SkScalar* src);

    static CARAPI_(Boolean) Native_getPosTan(
        /* [in] */ PathMeasurePair* native_instance,
        /* [in] */ Float distance,
        /* [out] */ ArrayOf<Float> ** ppPos,
        /* [out] */ ArrayOf<Float> ** ppTan);

    static CARAPI_(Boolean) Native_getMatrix(
        /* [in] */ PathMeasurePair* native_instance,
        /* [in] */ Float distance,
        /* [out] */ SkMatrix* native_matrix,
        /* [in] */ Int32 flags);

    static CARAPI_(Boolean) Native_getSegment(
        /* [in] */ PathMeasurePair* native_instance,
        /* [in] */ Float startF,
        /* [in] */ Float stopF,
        /* [out] */ SkPath* native_path,
        /* [in] */ Boolean startWithMoveTo);

    static CARAPI_(Boolean) Native_isClosed(
        /* [in] */ PathMeasurePair* native_instance);

    static CARAPI_(Boolean) Native_nextContour(
        /* [in] */ PathMeasurePair* native_instance);

    static CARAPI_(void) Native_destroy(
        /* [in] */ PathMeasurePair* native_instance);

    PathMeasurePair* native_instance;
};


#endif // __CPATHMEASURE_H__
