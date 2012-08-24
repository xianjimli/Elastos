
#include "graphics/CRegion.h"

ECode CRegion::SetEmpty()
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::Set(
    /* [in] */ IRegion* src,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::SetEx(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::SetEx2(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::SetPath(
    /* [in] */ IPath* path,
    /* [in] */ IRegion* clip,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::IsRect(
    /* [out] */ Boolean* isRect)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::IsComplex(
    /* [out] */ Boolean* isComplex)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::GetBounds(
    /* [out] */ IRect** bounds)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::GetBoundsEx(
    /* [in, out] */ IRect* r,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Return the boundary of the region as a new Path. If the region is empty,
 * the path will also be empty.
 */
ECode CRegion::GetBoundaryPath(
    /* [out] */ IPath** path)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Set the path to the boundary of the region. If the region is empty, the
 * path will also be empty.
 */
ECode CRegion::GetBoundaryPathEx(
    /* [in, out] */ IPath* path,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::Contains(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::QuickContains(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::QuickContainsEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::QuickReject(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::QuickRejectEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::QuickRejectEx2(
    /* [in] */ IRegion* rgn,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::Translate(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::TranslateEx(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ IRegion* dst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::Scale(
    /* [in] */ Float scale)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::ScaleEx(
    /* [in] */ Float scale,
    /* [in] */ IRegion* dst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::Union(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::Op(
    /* [in] */ IRect* r,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::OpEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::OpEx2(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::OpEx3(
    /* [in] */ IRect* rect,
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::OpEx4(
    /* [in] */ IRegion* region1,
    /* [in] */ IRegion* region2,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::Equals(
    /* [in] */ IRegion* r,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

SkRegion* CRegion::Ni()
{
    return mNativeRegion;
}

ECode CRegion::constructor()
{
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ IRegion* region)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::constructor(
    /* [in] */ IRect* r)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::constructor(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::constructor(
    /* [in] */ Int32 ni)
{
    return E_NOT_IMPLEMENTED;
}
