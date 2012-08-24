
#ifndef __CRECTF_H__
#define __CRECTF_H__

#include "_CRectF.h"

CarClass(CRectF)
{
public:
    CRectF()
        : mLeft(0.0f)
        , mTop(0.0f)
        , mRight(0.0f)
        , mBottom(0.0f)
    {}

public:
    CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty);

    CARAPI Width(
        /* [out] */ Float* width);

    CARAPI Height(
        /* [out] */ Float* height);

    CARAPI CenterX(
        /* [out] */ Float* x);

    CARAPI CenterY(
        /* [out] */ Float* y);

    CARAPI SetEmpty();

    CARAPI Set(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom);

    CARAPI SetEx(
        /* [in] */ IRectF* src);

    CARAPI SetEx2(
        /* [in] */ IRect* src);

    CARAPI Offset(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI OffsetTo(
        /* [in] */ Float newLeft,
        /* [in] */ Float newTop);

    CARAPI Inset(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI Contains(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Boolean* isContained);

    CARAPI ContainsEx(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ Boolean* isContained);

    CARAPI ContainsEx2(
        /* [in] */ IRectF* r,
        /* [out] */ Boolean* isContained);

    CARAPI Intersect(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ Boolean* isIntersected);

    CARAPI IntersectEx(
        /* [in] */ IRectF* r,
        /* [out] */ Boolean* isIntersected);

    CARAPI SetIntersect(
        /* [in] */ IRectF* a,
        /* [in] */ IRectF* b,
        /* [out] */ Boolean* isIntersected);

    CARAPI Intersects(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ Boolean* isIntersected);

    CARAPI Round(
        /* [in] */ IRect* dst);

    CARAPI RoundOut(
        /* [in] */ IRect* dst);

    CARAPI Union(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom);

    CARAPI UnionEx(
        /* [in] */ IRectF* r);

    CARAPI UnionEx2(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI Sort();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom);

    CARAPI constructor(
        /* [in] */ IRectF* rf);

    CARAPI constructor(
        /* [in] */ IRect* r);

    CARAPI_(Float) GetWidth();

    CARAPI_(Float) GetHeight();

public:
    Float mLeft;
    Float mTop;
    Float mRight;
    Float mBottom;
};

#endif //__CRECTF_H__
