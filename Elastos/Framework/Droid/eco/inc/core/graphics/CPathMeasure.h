
#ifndef __CPATHMEASURE_H__
#define __CPATHMEASURE_H__

#include "_CPathMeasure.h"
#include "graphics/CPath.h"
#include "graphics/CMatrix.h"
#include <skia/core/SkPathMeasure.h>


CarClass(CPathMeasure)
{
private:
    struct PathMeasurePair {
        PathMeasurePair() {}
        PathMeasurePair(const SkPath& path, Boolean forceClosed)
            : fPath(path), fMeasure(fPath, forceClosed) {}

        SkPath fPath;      // copy of the user's path
        SkPathMeasure fMeasure;   // this guy points to fPath
    };

public:
    ~CPathMeasure();

    CARAPI SetPath(
        /* [in] */ IPath* path,
        /* [in] */ Boolean forceClosed);

    CARAPI GetLength(
        /* [out] */ Float* length);

    CARAPI GetPosTan(
        /* [in] */ Float distance,
        /* [out] */ ArrayOf<Float>* pos,
        /* [out] */ ArrayOf<Float>* tan,
        /* [out] */ Boolean* result);

    CARAPI GetMatrix(
        /* [in] */ Float distance,
        /* [in] */ IMatrix* matrix,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* result);

    CARAPI GetSegment(
        /* [in] */ Float startD,
        /* [in] */ Float stopD,
        /* [in] */ IPath* dst,
        /* [in] */ Boolean startWithMoveTo,
        /* [out] */ Boolean* result);

    CARAPI IsClosed(
        /* [out] */ Boolean* result);

    CARAPI NextContour(
        /* [out] */ Boolean* result);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPath* path,
        /* [in] */ Boolean forceClosed);

private:
    static CARAPI_(PathMeasurePair*) NativeCreate(
        /* [in] */ SkPath* nativePath,
        /* [in] */ Boolean forceClosed);

    static CARAPI_(void) NativeSetPath(
        /* [in] */ PathMeasurePair* nativeInstance,
        /* [in] */ SkPath* nativePath,
        /* [in] */ Boolean forceClosed);

    static CARAPI_(Float) NativeGetLength(
        /* [in] */ PathMeasurePair* nativeInstance);

    static CARAPI_(void) ConvertTwoElemFloatArray(
        /* [out] */ ArrayOf<Float>* array,
        /* [in] */ SkScalar* src);

    static CARAPI_(Boolean) NativeGetPosTan(
        /* [in] */ PathMeasurePair* nativeInstance,
        /* [in] */ Float distance,
        /* [out] */ ArrayOf<Float>* pos,
        /* [out] */ ArrayOf<Float>* tan);

    static CARAPI_(Boolean) NativeGetMatrix(
        /* [in] */ PathMeasurePair* nativeInstance,
        /* [in] */ Float distance,
        /* [out] */ SkMatrix* nativeMatrix,
        /* [in] */ Int32 flags);

    static CARAPI_(Boolean) NativeGetSegment(
        /* [in] */ PathMeasurePair* nativeInstance,
        /* [in] */ Float startF,
        /* [in] */ Float stopF,
        /* [out] */ SkPath* nativePath,
        /* [in] */ Boolean startWithMoveTo);

    static CARAPI_(Boolean) NativeIsClosed(
        /* [in] */ PathMeasurePair* nativeInstance);

    static CARAPI_(Boolean) NativeNextContour(
        /* [in] */ PathMeasurePair* nativeInstance);

    static CARAPI_(void) NativeDestroy(
        /* [in] */ PathMeasurePair* nativeInstance);

private:
    PathMeasurePair* mNativeInstance;
};


#endif // __CPATHMEASURE_H__
