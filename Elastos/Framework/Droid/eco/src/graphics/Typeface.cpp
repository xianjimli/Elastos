
#include "graphics/Typeface.h"
#include "graphics/CTypeface.h"
#include "content/CAssetManager.h"
#include "utils/AutoString.h"
#include <SkStream.h>
#include <utils/AssetManager.h>

// {CCCF6009-21EE-411A-BC20-EA156193742D}
extern "C" const InterfaceID EIID_Typeface =
{ 0xcccf6009, 0x21ee, 0x411a, { 0xbc, 0x20, 0xea, 0x15, 0x61, 0x93, 0x74, 0x2d } };

AutoPtr<ITypeface>* SInit()
{
    Typeface::Create((String)NULL, 0, (ITypeface**)&Typeface::DEFAULT);
    Typeface::Create((String)NULL, Typeface_BOLD, (ITypeface**)&Typeface::DEFAULT_BOLD);
    Typeface::Create("sans-serif", 0, (ITypeface**)&Typeface::SANS_SERIF);
    Typeface::Create("serif", 0, (ITypeface**)&Typeface::SERIF);
    Typeface::Create("monospace", 0, (ITypeface**)&Typeface::MONOSPACE);

    AutoPtr<ITypeface> italic, boldItalic;
    Typeface::Create((String)NULL, Typeface_ITALIC, (ITypeface**)&italic);
    Typeface::Create((String)NULL, Typeface_BOLD_ITALIC, (ITypeface**)&boldItalic);

    AutoPtr<ITypeface>* typefaces = new AutoPtr<ITypeface>[4];
    typefaces[0] = Typeface::DEFAULT;
    typefaces[1] = Typeface::DEFAULT_BOLD;
    typefaces[2] = italic;
    typefaces[3] = boldItalic;
    return typefaces;
}

AutoPtr<ITypeface> Typeface::DEFAULT;
AutoPtr<ITypeface> Typeface::DEFAULT_BOLD;
AutoPtr<ITypeface> Typeface::SANS_SERIF;
AutoPtr<ITypeface> Typeface::SERIF;
AutoPtr<ITypeface> Typeface::MONOSPACE;
AutoPtr<ITypeface>* Typeface::sDefaults = SInit();

Typeface::~Typeface()
{
    NativeUnref(mNativeInstance);
}

Int32 Typeface::GetStyle()
{
    return NativeGetStyle(mNativeInstance);
}

Boolean Typeface::IsBold()
{
    return (GetStyle() & Typeface_BOLD) != 0;
}

Boolean Typeface::IsItalic()
{
    return (GetStyle() & Typeface_ITALIC) != 0;
}

ECode Typeface::Create(
    /* [in] */ String familyName,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    SkTypeface* nObj = Typeface::NativeCreate(familyName, style);
    AutoPtr<CTypeface> obj;
    FAIL_RETURN(CTypeface::NewByFriend((CTypeface**)&obj));
    obj->Init(nObj);
    *typeface = (ITypeface*)obj.Get();
    (*typeface)->AddRef();
    return NOERROR;
}

ECode Typeface::Create(
    /* [in] */ ITypeface* family,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    SkTypeface* ni = NULL;
    if (family != NULL) {
        ni = ((Typeface*)family->Probe(EIID_Typeface))->mNativeInstance;
    }
    SkTypeface* nObj = Typeface::NativeCreateFromTypeface(ni, style);
    AutoPtr<CTypeface> obj;
    FAIL_RETURN(CTypeface::NewByFriend((CTypeface**)&obj));
    obj->Init(nObj);
    *typeface = (ITypeface*)obj.Get();
    (*typeface)->AddRef();
    return NOERROR;
}

ECode Typeface::DefaultFromStyle(
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    *typeface = sDefaults[style];
    if (*typeface) (*typeface)->AddRef();
    return NOERROR;
}

ECode Typeface::CreateFromAsset(
    /* [in] */ IAssetManager* mgr,
    /* [in] */ String path,
    /* [out] */ ITypeface** typeface)
{
    SkTypeface* nObj = Typeface::NativeCreateFromAsset(mgr, path);
    AutoPtr<CTypeface> obj;
    FAIL_RETURN(CTypeface::NewByFriend((CTypeface**)&obj));
    obj->Init(nObj);
    *typeface = (ITypeface*)obj.Get();
    (*typeface)->AddRef();
    return NOERROR;
}

ECode Typeface::CreateFromFile(
    /* [in] */ IFile* path,
    /* [out] */ ITypeface** typeface)
{
    AutoString absPath;
    path->GetAbsolutePath(&absPath);
    SkTypeface* nObj = Typeface::NativeCreateFromFile(absPath);
    AutoPtr<CTypeface> obj;
    FAIL_RETURN(CTypeface::NewByFriend((CTypeface**)&obj));
    obj->Init(nObj);
    *typeface = (ITypeface*)obj.Get();
    (*typeface)->AddRef();
    return NOERROR;
}

ECode Typeface::CreateFromFile(
    /* [in]*/ String path,
    /* [out] */ ITypeface** typeface)
{
    SkTypeface* nObj = Typeface::NativeCreateFromFile(path);
    AutoPtr<CTypeface> obj;
    FAIL_RETURN(CTypeface::NewByFriend((CTypeface**)&obj));
    obj->Init(nObj);
    *typeface = (ITypeface*)obj.Get();
    (*typeface)->AddRef();
    return NOERROR;
}

ECode Typeface::Init(
    /* [in] */ SkTypeface* ni)
{
    if (NULL == ni) {
//        throw new RuntimeException("native typeface cannot be made");
        return E_RUNTIME_EXCEPTION;
    }
    mNativeInstance = ni;
    return NOERROR;
}

SkTypeface* Typeface::NativeCreate(
    /* [in] */ String familyName,
    /* [in] */ Int32 style)
{
    SkTypeface* face;

    if (familyName.IsNull()) {
        face = SkTypeface::CreateFromName(NULL, (SkTypeface::Style)style);
    }
    else {
        face = SkTypeface::CreateFromName(familyName, (SkTypeface::Style)style);
    }
    return face;
}

SkTypeface* Typeface::NativeCreateFromTypeface(
    /* [in] */ SkTypeface* nOther,
    /* [in] */ Int32 style)
{
    return SkTypeface::CreateFromTypeface(nOther, (SkTypeface::Style)style);
}

void Typeface::NativeUnref(
    /* [in] */ SkTypeface* nObj)
{
    SkSafeUnref(nObj);
}

Int32 Typeface::NativeGetStyle(
    /* [in] */ SkTypeface* nObj)
{
    return nObj->style();
}

class AssetStream : public SkStream
{
public:
    AssetStream(android::Asset* asset, bool hasMemoryBase) : fAsset(asset)
    {
        fMemoryBase = hasMemoryBase ? fAsset->getBuffer(false) : NULL;
    }

    virtual ~AssetStream()
    {
        delete fAsset;
    }

    virtual const void* getMemoryBase()
    {
        return fMemoryBase;
    }

    virtual bool rewind()
    {
        off_t pos = fAsset->seek(0, SEEK_SET);
        return pos != (off_t)-1;
    }

    virtual size_t read(void* buffer, size_t size)
    {
        ssize_t amount;

        if (NULL == buffer)
        {
            if (0 == size)  // caller is asking us for our total length
                return fAsset->getLength();

            // asset->seek returns new total offset
            // we want to return amount that was skipped

            off_t oldOffset = fAsset->seek(0, SEEK_CUR);
            if (-1 == oldOffset)
                return 0;
            off_t newOffset = fAsset->seek(size, SEEK_CUR);
            if (-1 == newOffset)
                return 0;

            amount = newOffset - oldOffset;
        }
        else
        {
            amount = fAsset->read(buffer, size);
        }

        if (amount < 0)
            amount = 0;
        return amount;
    }

private:
    android::Asset* fAsset;
    const void* fMemoryBase;
};

SkTypeface* Typeface::NativeCreateFromAsset(
    /* [in] */ IAssetManager* mgr,
    /* [in] */ String path)
{
    if (mgr == NULL || path.IsNull()) return NULL;

    android::AssetManager* amgr = ((CAssetManager*)mgr)->mNative;
    if (NULL == mgr) {
        return NULL;
    }

    android::Asset* asset = amgr->open(
            (const char*)path, android::Asset::ACCESS_BUFFER);
    if (NULL == asset) {
        return NULL;
    }

    return SkTypeface::CreateFromStream(new AssetStream(asset, false));
}

SkTypeface* Typeface::NativeCreateFromFile(
    /* [in] */ String path)
{
    if (path.IsNull()) return NULL;

    return SkTypeface::CreateFromFile((const char*)path);
}

#define MIN_GAMMA   (0.1f)
#define MAX_GAMMA   (10.0f)
static float pinGamma(float gamma)
{
    if (gamma < MIN_GAMMA) {
        gamma = MIN_GAMMA;
    }
    else if (gamma > MAX_GAMMA) {
        gamma = MAX_GAMMA;
    }
    return gamma;
}

extern void skia_set_text_gamma(float, float);

ECode Typeface::SetGammaForText(
        /* [in] */ Float blackGamma,
        /* [in] */ Float whiteGamma)
{
    // Comment this out for release builds. This is only used during development
    skia_set_text_gamma(pinGamma(blackGamma), pinGamma(whiteGamma));
    return NOERROR;
}
