
#include "graphics/drawable/Drawable.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CPorterDuffColorFilter.h"
#include "graphics/ElPixelFormat.h"
#include "graphics/NinePatch.h"
#include "graphics/drawable/CBitmapDrawable.h"
#include "graphics/drawable/CColorDrawable.h"
#include "graphics/drawable/CNinePatchDrawable.h"
#include "graphics/drawable/CStateListDrawable.h"
#include "graphics/drawable/CAnimatedRotateDrawable.h"
#include "graphics/drawable/CLayerDrawable.h"
#include "graphics/drawable/CGradientDrawable.h"
#include "graphics/drawable/CClipDrawable.h"
#include "graphics/drawable/CAnimationDrawable.h"
#include "utils/StateSet.h"
#include "utils/CDisplayMetrics.h"
#include "utils/Xml.h"
#include <elastos/AutoFree.h>
#include <elastos/AutoPtr.h>

static AutoPtr<IRect> ZERO_BOUNDS_RECT;

void StaticInit()
{
    if (ZERO_BOUNDS_RECT == NULL) {
        ASSERT_SUCCEEDED(CRect::New((IRect**)&ZERO_BOUNDS_RECT));
    }
}

Drawable::Drawable()
    : mLevel(0)
    , mChangingConfigurations(0)
    , mVisible(TRUE)
{
    StaticInit();
    mStateSet = StateSet::WILD_CARD->Clone();
    mBounds = ZERO_BOUNDS_RECT;
}

Drawable::~Drawable()
{
    ArrayOf<Int32>::Free(mStateSet);
}

ECode Drawable::SetBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IRect> _oldBounds = mBounds;

    if (_oldBounds == ZERO_BOUNDS_RECT) {
        FAIL_RETURN(CRect::New((IRect**)&mBounds));
        _oldBounds = mBounds;
    }

    CRect* oldBounds = (CRect*)_oldBounds.Get();
    if (oldBounds->mLeft != left || oldBounds->mTop != top ||
            oldBounds->mRight != right || oldBounds->mBottom != bottom) {
        mBounds->Set(left, top, right, bottom);
        OnBoundsChange(mBounds);
    }
    return NOERROR;
}

ECode Drawable::SetBounds(
    /* [in] */ IRect* _bounds)
{
    assert(_bounds);
    CRect* bounds = (CRect*)_bounds;
    return SetBounds(bounds->mLeft, bounds->mTop, bounds->mRight, bounds->mBottom);
}

void Drawable::CopyBounds(
    /* [in] */ IRect* bounds)
{
    assert(bounds != NULL);
    bounds->SetEx(mBounds.Get());
}

AutoPtr<IRect> Drawable::CopyBounds()
{
    AutoPtr<IRect> r;
    ASSERT_SUCCEEDED(CRect::New(mBounds.Get(), (IRect**)&r));
    return r;
}

AutoPtr<IRect> Drawable::GetBounds()
{
    if (mBounds == ZERO_BOUNDS_RECT) {
        ASSERT_SUCCEEDED(CRect::New((IRect**)&mBounds));
    }

    return mBounds;
}

ECode Drawable::SetChangingConfigurations(
    /* [in] */ Int32 configs)
{
    mChangingConfigurations = configs;
    return NOERROR;
}

Int32 Drawable::GetChangingConfigurations()
{
    return mChangingConfigurations;
}

ECode Drawable::SetDither(
    /* [in] */ Boolean dither)
{
    return NOERROR;
}

ECode Drawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return NOERROR;
}

ECode Drawable::SetCallback(
    /* [in] */ IDrawableCallback* cb)
{
    mCallback = cb;
    return NOERROR;
}

ECode Drawable::InvalidateSelf()
{
    if (mCallback != NULL) {
        mCallback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode Drawable::ScheduleSelf(
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    if (mCallback != NULL) {
        mCallback->ScheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable),
                what, when);
    }
    return NOERROR;
}

ECode Drawable::UnscheduleSelf(
    /* [in] */ IRunnable* what)
{
    if (mCallback != NULL) {
        mCallback->UnscheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable),
                what);
    }
    return NOERROR;
}

ECode Drawable::SetColorFilter(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    AutoPtr<IColorFilter> filter;
    CPorterDuffColorFilter::New(color, mode, (IColorFilter**)&filter);
    return SetColorFilter(filter);
}

ECode Drawable::ClearColorFilter()
{
    return SetColorFilter(NULL);
}

Boolean Drawable::IsStateful()
{
    return FALSE;
}

Boolean Drawable::SetState(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    Boolean isEqual = TRUE;

    if (mStateSet ==  stateSet) {
        isEqual = TRUE;
    }
    else if (mStateSet == NULL || stateSet == NULL
            || mStateSet->GetLength() != stateSet->GetLength()) {
        isEqual = FALSE;
    }
    else {
        Int32 len = mStateSet->GetLength();
        for (Int32 i = 0; i < len; i++) {
            if ((*mStateSet)[i] != (*stateSet)[i]) {
                isEqual = FALSE;
            }
        }
    }

    if (!isEqual) {
        ArrayOf<Int32>::Free(mStateSet);
        mStateSet = stateSet->Clone();
        return OnStateChange(stateSet);
    }
    return FALSE;
}

ArrayOf<Int32>* Drawable::GetState()
{
    return mStateSet;
}

AutoPtr<IDrawable> Drawable::GetCurrent()
{
    return AutoPtr<IDrawable>((IDrawable*)this->Probe(EIID_IDrawable));
}

Boolean Drawable::SetLevel(
    /* [in] */ Int32 level)
{
    if (mLevel != level) {
        mLevel = level;
        return OnLevelChange(level);
    }
    return FALSE;
}

Int32 Drawable::GetLevel()
{
    return mLevel;
}

Boolean Drawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = mVisible != visible;
    mVisible = visible;
    return changed;
}

Boolean Drawable::IsVisible()
{
    return mVisible;
}

Int32 Drawable::ResolveOpacity(
    /* [in] */ Int32 op1,
    /* [in] */ Int32 op2)
{
    if (op1 == op2) {
        return op1;
    }
    if (op1 == ElPixelFormat::UNKNOWN || op2 == ElPixelFormat::UNKNOWN) {
        return ElPixelFormat::UNKNOWN;
    }
    if (op1 == ElPixelFormat::TRANSLUCENT || op2 == ElPixelFormat::TRANSLUCENT) {
        return ElPixelFormat::TRANSLUCENT;
    }
    if (op1 == ElPixelFormat::TRANSPARENT || op2 == ElPixelFormat::TRANSPARENT) {
        return ElPixelFormat::TRANSPARENT;
    }
    return ElPixelFormat::OPAQUE;
}

AutoPtr<IRegion> Drawable::GetTransparentRegion()
{
    return AutoPtr<IRegion>(NULL);
}

Boolean Drawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    return FALSE;
}

Boolean Drawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    return FALSE;
}

void Drawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
}

Int32 Drawable::GetIntrinsicWidth()
{
    return -1;
}

Int32 Drawable::GetIntrinsicHeight()
{
    return -1;
}

Int32 Drawable::GetMinimumWidth()
{
    Int32 intrinsicWidth = GetIntrinsicWidth();
    return intrinsicWidth > 0 ? intrinsicWidth : 0;
}

Int32 Drawable::GetMinimumHeight()
{
    Int32 intrinsicHeight = GetIntrinsicHeight();
    return intrinsicHeight > 0 ? intrinsicHeight : 0;
}

Boolean Drawable::GetPadding(
    /* [in, out] */ IRect* padding)
{
    assert(padding != NULL);
    padding->Set(0, 0, 0, 0);
    return FALSE;
}

AutoPtr<IDrawable> Drawable::Mutate()
{
    return AutoPtr<IDrawable>((IDrawable*)this->Probe(EIID_IDrawable));
}

ECode Drawable::CreateFromStream(
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** drawable)
{
    return CreateFromResourceStream(NULL, NULL, is, srcName, NULL, drawable);
}

ECode Drawable::CreateFromResourceStream(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** drawable)
{
    return CreateFromResourceStream(res, value, is, srcName, NULL, drawable);
}

ECode Drawable::CreateFromResourceStream(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [in] */ IBitmapFactoryOptions* _opts,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    if (is == NULL) {
        *drawable = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    /*  ugh. The decodeStream contract is that we have already allocated
        the pad rect, but if the bitmap does not had a ninepatch chunk,
        then the pad will be ignored. If we could change this to lazily
        alloc/assign the rect, we could avoid the GC churn of making new
        Rects only to drop them on the floor.
    */
    AutoPtr<IRect> pad;
    CRect::New((IRect**)&pad);

    // Special stuff for compatibility mode: if the target density is not
    // the same as the display density, but the resource -is- the same as
    // the display density, then don't scale it down to the target density.
    // This allows us to load the system's density-correct resources into
    // an application in compatibility mode, without scaling those down
    // to the compatibility density only to have them scaled back up when
    // drawn to the screen.
    AutoPtr<CBitmapFactoryOptions> opts = (CBitmapFactoryOptions*)_opts;
    if (opts == NULL) {
        CBitmapFactoryOptions::NewByFriend((CBitmapFactoryOptions**)&opts);
    }
    opts->mInScreenDensity = CDisplayMetrics::DENSITY_DEVICE;

    AutoPtr<IBitmap> bm;
    AutoPtr<IBitmapFactory> factory;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    FAIL_RETURN(factory->DecodeResourceStream(
            res, value, is, pad.Get(), opts.Get(), (IBitmap**)&bm));
    if (bm != NULL) {
        AutoFree< ArrayOf<Byte> > np;
        FAIL_RETURN(bm->GetNinePatchChunk((ArrayOf<Byte>**)&np));
        if (np == NULL || !NinePatch::IsNinePatchChunk(*np)) {
            np = NULL;
            pad = NULL;
        }
        return DrawableFromBitmap(res, bm.Get(), np.Get(), pad.Get(), srcName, drawable);
    }
    *drawable = NULL;
    return E_RUNTIME_EXCEPTION;
}

ECode Drawable::CreateFromXml(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IDrawable** drawable)
{
    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type;
    FAIL_RETURN(parser->Next(&type));
    while (type != IXmlPullParser_START_TAG &&
            type != IXmlPullParser_END_DOCUMENT) {
        // Empty loop
        FAIL_RETURN(parser->Next(&type));
    }

    if (type != IXmlPullParser_START_TAG) {
//        throw new XmlPullParserException("No start tag found");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    FAIL_RETURN(CreateFromXmlInner(r, parser, attrs, drawable));

    if (*drawable == NULL) {
//        throw new RuntimeException("Unknown initial tag: " + parser.getName());
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

ECode Drawable::CreateFromXmlInner(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IDrawable** drawable)
{
    String name;
    FAIL_RETURN(parser->GetName((String*)&name));

    printf("Drawable::CreateFromXmlInner name = %s\n", (const char*)name);

    //printf("Drawable::CreateFromXmlInner, name = %s\n", (const char*)name);
    if (name.Equals("selector")) {
          FAIL_RETURN(CStateListDrawable::New((IStateListDrawable**)drawable));
    }
    else if (name.Equals("level-list")) {
//        drawable = new LevelListDrawable();
        assert(0);
    }
    else if (name.Equals("layer-list")) {
        FAIL_RETURN(CLayerDrawable::New((ILayerDrawable**)drawable));
    }
    else if (name.Equals("transition")) {
//        drawable = new TransitionDrawable();
//        assert(0);
    }
    else if (name.Equals("color")) {
        FAIL_RETURN(CColorDrawable::New((IColorDrawable**)drawable));
    }
    else if (name.Equals("shape")) {
        FAIL_RETURN(CGradientDrawable::New((IGradientDrawable**)drawable));
    }
    else if (name.Equals("scale")) {
//        drawable = new ScaleDrawable();
        assert(0);
    }
    else if (name.Equals("clip")) {
        FAIL_RETURN(CClipDrawable::New((IClipDrawable**)drawable));
    }
    else if (name.Equals("rotate")) {
//        drawable = new RotateDrawable();
        assert(0);
    }
    else if (name.Equals("animated-rotate")) {
        FAIL_RETURN(CAnimatedRotateDrawable::New((IAnimatedRotateDrawable**)drawable));
    }
    else if (name.Equals("animation-list")) {
        FAIL_RETURN(CAnimationDrawable::New((IAnimationDrawable**)drawable));
    }
    else if (name.Equals("inset")) {
//        drawable = new InsetDrawable();
        assert(0);
    }
    else if (name.Equals("bitmap")) {
        FAIL_RETURN(CBitmapDrawable::New((IBitmapDrawable**)drawable));
        if (r != NULL) {
//           ((BitmapDrawable)drawable).setTargetDensity(r.getDisplayMetrics());
        }
    }
    else if (name.Equals("nine-patch")) {
        FAIL_RETURN(CNinePatchDrawable::New((INinePatchDrawable**)drawable));
        if (r != NULL) {
//            ((NinePatchDrawable) drawable)->SetTargetDensity(r.GetDisplayMetrics());
        }
    } else {
//        throw new XmlPullParserException(parser.getPositionDescription() +
//                ": invalid drawable tag " + name);
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    //TODO:
    if (*drawable == NULL) {
        return NOERROR;
    }

    return (*drawable)->Inflate(r, parser, attrs);
}

ECode Drawable::CreateFromPath(
    /* [in] */ const String& pathName,
    /* [out] */ IDrawable** drawable)
{
    if (pathName.IsNull()) {
        *drawable = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IBitmapFactory> factory;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    AutoPtr<IBitmap> bm;
    FAIL_RETURN(factory->DecodeFileEx(pathName, (IBitmap**)&bm));
    if (bm != NULL) {
        return DrawableFromBitmap(NULL, bm, NULL, NULL, pathName, drawable);
    }

    *drawable = NULL;
    return E_RUNTIME_EXCEPTION;
}

static Int32 R_Styleable_Drawable[] = {
    0x01010194
};

ECode Drawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs, ArrayOf<Int32>(R_Styleable_Drawable, 1)/*com.android.internal.R.styleable.Drawable*/,
            (ITypedArray**)&a));
    FAIL_RETURN(InflateWithAttributes(r, parser, a, 0/*com.android.internal.R.styleable.Drawable_visible*/));
    return a->Recycle();
}

ECode Drawable::InflateWithAttributes(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ ITypedArray* attrs,
    /* [in] */ Int32 visibleAttr)
{
    return attrs->GetBoolean(visibleAttr, mVisible, &mVisible);
}

AutoPtr<IDrawableConstantState> Drawable::GetConstantState()
{
    return AutoPtr<IDrawableConstantState>(NULL);
}

/////////////////////////////////////////////////////////////////////

ECode Drawable::DrawableFromBitmap(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bm,
    /* [in] */ ArrayOf<Byte>* np,
    /* [in] */ IRect* pad,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** drawable)
{
    assert(drawable != NULL);

    if (np != NULL) {
        return CNinePatchDrawable::New(
            res, bm, *np, pad, srcName, (INinePatchDrawable**)drawable);
    }

    return CBitmapDrawable::New(res, bm, (IBitmapDrawable**)drawable);
}

