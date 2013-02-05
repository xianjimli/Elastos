
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
    CARAPI setPath(
        /* [in] */ IPath * pPath,
        /* [in] */ Boolean forceClosed);

    CARAPI getLength(
        /* [out] */ Float * pLength);

    CARAPI getPosTan(
        /* [in] */ Float distance,
        /* [out, callee] */ ArrayOf<Float> ** ppPos,
        /* [out, callee] */ ArrayOf<Float> ** ppTan,
        /* [out] */ Boolean * pResult);

    CARAPI getMatrix(
        /* [in] */ Float distance,
        /* [in] */ IMatrix * pMatrix,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean * pResult);

    CARAPI getSegment(
        /* [in] */ Float startD,
        /* [in] */ Float stopD,
        /* [in] */ IPath * pDst,
        /* [in] */ Boolean startWithMoveTo,
        /* [out] */ Boolean * pResult);

    CARAPI isClosed(
        /* [out] */ Boolean * pResult);

    CARAPI nextContour(
        /* [out] */ Boolean * pResult);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPath * pPath,
        /* [in] */ Boolean forceClosed);

private:
    static CARAPI_(PathMeasurePair*) native_create(SkPath* native_path, Boolean forceClosed);
    static CARAPI_(void) native_setPath(PathMeasurePair* native_instance, SkPath* native_path, Boolean forceClosed);
    static CARAPI_(Float) native_getLength(PathMeasurePair* native_instance);
	static CARAPI_(void) convertTwoElemFloatArray(ArrayOf<Float> *  array, SkScalar* src);
    static CARAPI_(Boolean) native_getPosTan(PathMeasurePair* native_instance, Float distance,
            ArrayOf<Float> ** ppPos, ArrayOf<Float> ** ppTan);
    static CARAPI_(Boolean) native_getMatrix(PathMeasurePair* native_instance, Float distance, SkMatrix* native_matrix,
            Int32 flags);
    static CARAPI_(Boolean) native_getSegment(PathMeasurePair* native_instance, Float startF,
            Float stopF, SkPath* native_path, Boolean startWithMoveTo);
    static CARAPI_(Boolean) native_isClosed(PathMeasurePair* native_instance);
    static CARAPI_(Boolean) native_nextContour(PathMeasurePair* native_instance);
    static CARAPI_(void) native_destroy(PathMeasurePair* native_instance);

    PathMeasurePair* native_instance;
};


#endif // __CPATHMEASURE_H__
