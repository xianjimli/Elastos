
#include "graphics/CRegion.h"
#include "graphics/Graphics.h"
#include "graphics/CRect.h"
#include "graphics/CPath.h"
#include "ext/frameworkdef.h"

ECode CRegion::constructor()
{
    mNativeRegion = NativeConstructor();
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ IRegion* region)
{
    mNativeRegion = NativeConstructor();
    NativeSetRegion(mNativeRegion, ((CRegion*)region)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ IRect* r)
{
    CRect* rect = (CRect*)r;

    mNativeRegion = NativeConstructor();
    NativeSetRect(mNativeRegion, rect->mLeft, rect->mTop, rect->mRight, rect->mBottom);
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mNativeRegion = NativeConstructor();
    NativeSetRect(mNativeRegion, left, top, right, bottom);
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ Int32 ni)
{
    if (ni == NULL) {
        //throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }

    mNativeRegion = (SkRegion*)ni;
    return NOERROR;
}

CRegion::~CRegion()
{
    NativeDestructor(mNativeRegion);
}

ECode CRegion::SetEmpty()
{
    NativeSetRect(mNativeRegion, 0, 0, 0, 0);
    return NOERROR;
}

ECode CRegion::Set(
    /* [in] */ IRegion* src,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetRegion(mNativeRegion, ((CRegion*)src)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::SetEx(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* rect = (CRect*)r;

    *result = NativeSetRect(mNativeRegion, rect->mLeft, rect->mTop, rect->mRight, rect->mBottom);
    return NOERROR;
}

ECode CRegion::SetEx2(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetRect(mNativeRegion, left, top, right, bottom);
    return NOERROR;
}

ECode CRegion::SetPath(
    /* [in] */ IPath* path,
    /* [in] */ IRegion* clip,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetPath(mNativeRegion, ((CPath*)path)->Ni(), ((CRegion*)clip)->mNativeRegion);
    return NOERROR;
}

// SkRegion* GetSkRegion(SkRegion* regionObject)
// {
//     SkASSERT(regionObject->mNativeRegion != NULL);
//     return regionObject->mNativeRegion;
// }

ECode CRegion::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);

    *isEmpty = mNativeRegion->isEmpty();
    return NOERROR;
}

ECode CRegion::IsRect(
    /* [out] */ Boolean* isRect)
{
    VALIDATE_NOT_NULL(isRect);

    *isRect = mNativeRegion->isRect();
    return NOERROR;
}

ECode CRegion::IsComplex(
    /* [out] */ Boolean* isComplex)
{
    VALIDATE_NOT_NULL(isComplex);

    *isComplex = mNativeRegion->isComplex();
    return NOERROR;
}

ECode CRegion::GetBounds(
    /* [out] */ IRect** bounds)
{
    CRect::New(bounds);
    NativeGetBounds(mNativeRegion, *bounds);
    return NOERROR;
}

ECode CRegion::GetBoundsEx(
    /* [in, out] */ IRect* r,
    /* [out] */ Boolean* result)
{
    if (r == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    *result = NativeGetBounds(mNativeRegion, r);
    return NOERROR;
}

/**
 * Return the boundary of the region as a new Path. If the region is empty,
 * the path will also be empty.
 */
ECode CRegion::GetBoundaryPath(
    /* [out] */ IPath** path)
{
    CPath::New(path);

    NativeGetBoundaryPath(mNativeRegion, ((CPath*)*path)->Ni());
    return NOERROR;
}

/**
 * Set the path to the boundary of the region. If the region is empty, the
 * path will also be empty.
 */
ECode CRegion::GetBoundaryPathEx(
    /* [in, out] */ IPath* path,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeGetBoundaryPath(mNativeRegion, ((CPath*)path)->Ni());
    return NOERROR;
}

ECode CRegion::Contains(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    mNativeRegion->contains(x, y);
    return NOERROR;
}

ECode CRegion::QuickContains(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    CRect* rect = (CRect*)r;

    return QuickContainsEx(rect->mLeft, rect->mTop, rect->mRight, rect->mBottom, result);
}

ECode CRegion::QuickContainsEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mNativeRegion->quickContains(left, top, right, bottom);
    return NOERROR;
}

ECode CRegion::QuickReject(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    CRect* rect = (CRect*)r;

    return QuickRejectEx(rect->mLeft, rect->mTop, rect->mRight, rect->mBottom, result);
}

ECode CRegion::QuickRejectEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    SkIRect ir;

    ir.set(left, top, right, bottom);
    *result = mNativeRegion->quickReject(ir);
    return NOERROR;
}

ECode CRegion::QuickRejectEx2(
    /* [in] */ IRegion* rgn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    mNativeRegion->quickReject(*((CRegion*)rgn)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::Translate(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return TranslateEx(dx, dy, NULL);
}

ECode CRegion::TranslateEx(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ IRegion* dst)
{
    if (dst) {
        mNativeRegion->translate(dx, dy, ((CRegion*)dst)->mNativeRegion);
    }
    else {
        mNativeRegion->translate(dx, dy);
    }

    return NOERROR;
}

ECode CRegion::Scale(
    /* [in] */ Float scale)
{
    return ScaleEx(scale, NULL);
}

// Scale the rectangle by given scale and set the reuslt to the dst.
void CRegion::ScaleRect(
    /* [in] */ SkIRect* dst,
    /* [in] */ const SkIRect& src,
    /* [in] */ Float scale)
{
   dst->fLeft = (Int32)::roundf(src.fLeft * scale);
   dst->fTop = (Int32)::roundf(src.fTop * scale);
   dst->fRight = (Int32)::roundf(src.fRight * scale);
   dst->fBottom = (Int32)::roundf(src.fBottom * scale);
}

// Scale the region by given scale and set the reuslt to the dst.
// dest and src can be the same region instance.
void CRegion::ScaleRgn(
    /* [in] */ SkRegion* dst,
    /* [in] */ const SkRegion& src,
    /* [in] */ Float scale)
{
   SkRegion tmp;
   SkRegion::Iterator iter(src);

   for (; !iter.done(); iter.next()) {
       SkIRect r;
       ScaleRect(&r, iter.rect(), scale);
       tmp.op(r, SkRegion::kUnion_Op);
   }
   dst->swap(tmp);
}

ECode CRegion::ScaleEx(
    /* [in] */ Float scale,
    /* [in] */ IRegion* dst)
{
    if (dst)
        ScaleRgn(((CRegion*)dst)->mNativeRegion, *mNativeRegion, scale);
    else
        ScaleRgn(mNativeRegion, *mNativeRegion, scale);

    return NOERROR;
}

ECode CRegion::Union(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    return Op(r, RegionOp_UNION, result);
}

ECode CRegion::Op(
    /* [in] */ IRect* r,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CRect* rect = (CRect*)r;

    *result = NativeOp(mNativeRegion, rect->mLeft, rect->mTop, rect->mRight, rect->mBottom, op);
    return NOERROR;
}

ECode CRegion::OpEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeOp(mNativeRegion, left, top, right, bottom, op);
    return NOERROR;
}

ECode CRegion::OpEx2(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    return OpEx4((IRegion*)Probe(EIID_IRegion), region, op, result);
}

ECode CRegion::OpEx3(
    /* [in] */ IRect* rect,
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeOp(mNativeRegion, rect, ((CRegion*)region)->mNativeRegion, op);
    return NOERROR;
}

ECode CRegion::OpEx4(
    /* [in] */ IRegion* region1,
    /* [in] */ IRegion* region2,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeOp(
                mNativeRegion,
                ((CRegion*)region1)->mNativeRegion,
                ((CRegion*)region2)->mNativeRegion,
                op);

    return NOERROR;
}

ECode CRegion::Equals(
    /* [in] */ IRegion* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if ((r == NULL) || (IRegion::Probe(r) == NULL)) {
        *result = FALSE;
        return NOERROR;
    }

    *result = NativeEquals(mNativeRegion, ((CRegion*)r)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRegion::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (!NativeWriteToParcel(mNativeRegion, dest)) {
        //throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

SkRegion* CRegion::Ni()
{
    return mNativeRegion;
}

SkRegion* CRegion::NativeConstructor()
{
    return new SkRegion;
}

void CRegion::NativeDestructor(
    /* [in] */ SkRegion* region)
{
    SkASSERT(region);
    delete region;
}

Boolean CRegion::NativeSetRegion(
    /* [out] */ SkRegion* dst,
    /* [in] */ SkRegion* src)
{
    SkASSERT(dst && src);
    *dst = *src;

    return TRUE;
}

Boolean CRegion::NativeSetRect(
    /* [in] */ SkRegion* dst,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return dst->setRect(left, top, right, bottom);
}

Boolean CRegion::NativeSetPath(
    /* [in] */ SkRegion* dst,
    /* [in] */ SkPath* path,
    /* [in] */ SkRegion* clip)
{
    SkASSERT(dst && path && clip);
    return dst->setPath(*path, *clip);
}

Boolean CRegion::NativeGetBounds(
    /* [in] */ SkRegion* region,
    /* [out] */ IRect* rect)
{
    Graphics::SkIRect2IRect(region->getBounds(), rect);
    return !region->isEmpty();
}

Boolean CRegion::NativeGetBoundaryPath(
    /* [in] */ SkRegion* region,
    /* [out] */ SkPath* path)
{
    return region->getBoundaryPath(path);
}

Boolean CRegion::NativeOp(
    /* [in] */ SkRegion* dst,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ RegionOp op)
{
    SkIRect ir;

    ir.set(left, top, right, bottom);
    return dst->op(ir, (SkRegion::Op)op);
}

Boolean CRegion::NativeOp(
    /* [in] */ SkRegion* dst,
    /* [in] */ IRect* rect,
    /* [in] */ SkRegion* region,
    /* [in] */ RegionOp op)
{
    SkIRect ir;

    Graphics::IRect2SkIRect(rect, &ir);
    return dst->op(ir, *region, (SkRegion::Op)op);
}

Boolean CRegion::NativeOp(
    /* [in] */ SkRegion* dst,
    /* [in] */ SkRegion* region1,
    /* [in] */ SkRegion* region2,
    /* [in] */ RegionOp op)
{
    return dst->op(*region1, *region2, (SkRegion::Op)op);
}

SkRegion* CRegion::NativeCreateFromParcel(
    /* [in] */ IParcel* parcel)
{
    //TODO:
    return NULL;
}

Boolean CRegion::NativeWriteToParcel(
    /* [in] */ SkRegion* region,
    /* [out] */ IParcel* parcel)
{
    //TODO:
    //VALIDATE_NOT_NULL(parcel);

    return FALSE;
}

Boolean CRegion::NativeEquals(
    /* [in] */ SkRegion* region1,
    /* [in] */ SkRegion* region2)
{
    return *region1 == *region2;
}
