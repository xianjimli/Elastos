#include "TextAttribute.h"
#include "CTextAttribute.h"

static AutoPtr<ITextAttribute> sInit(
    /* [in] */ const String& name)
{
    // AutoPtr<CTextAttribute> attribute;
    // CTextAttribute::NewByFriend(name, (CTextAttribute**)&attribute);
    // return attribute.Get();
    return NULL;
}

const AutoPtr<ITextAttribute> TextAttribute::BACKGROUND = sInit(String("background") );

const AutoPtr<ITextAttribute> TextAttribute::BIDI_EMBEDDING = sInit(String("bidi_embedding") );

const AutoPtr<ITextAttribute> TextAttribute::CHAR_REPLACEMENT = sInit(String("char_replacement") );

const AutoPtr<ITextAttribute> TextAttribute::FAMILY = sInit(String("family") );

const AutoPtr<ITextAttribute> TextAttribute::FONT = sInit(String("font") );

const AutoPtr<ITextAttribute> TextAttribute::FOREGROUND = sInit(String("foreground") );

const AutoPtr<ITextAttribute> TextAttribute::INPUT_METHOD_HIGHLIGHT = sInit(String("input method highlight") );

const AutoPtr<ITextAttribute> TextAttribute::INPUT_METHOD_UNDERLINE = sInit(String("input method underline") );

const AutoPtr<ITextAttribute> TextAttribute::JUSTIFICATION = sInit(String("justification") );

const Float TextAttribute::JUSTIFICATION_FULL = 1.0F;

const Float TextAttribute::JUSTIFICATION_NONE = 0.0F;

const AutoPtr<ITextAttribute> TextAttribute::KERNING = sInit(String("kerning") );

const AutoPtr<ITextAttribute> TextAttribute::LIGATURES = sInit(String("ligatures") );

const AutoPtr<ITextAttribute> TextAttribute::NUMERIC_SHAPING = sInit(String("numeric_shaping") );

const AutoPtr<ITextAttribute> TextAttribute::POSTURE = sInit(String("posture") );

const Float TextAttribute::POSTURE_REGULAR = 0.0F;

const Float TextAttribute::POSTURE_OBLIQUE = 0.20F;

const AutoPtr<ITextAttribute> TextAttribute::RUN_DIRECTION = sInit(String("run_direction") );

const Boolean TextAttribute::RUN_DIRECTION_LTR = FALSE;

const Boolean TextAttribute::RUN_DIRECTION_RTL = TRUE;

const AutoPtr<ITextAttribute> TextAttribute::SIZE = sInit(String("size") );

const AutoPtr<ITextAttribute> TextAttribute::STRIKETHROUGH = sInit(String("strikethrough") );

const Boolean TextAttribute::STRIKETHROUGH_ON = TRUE;

const AutoPtr<ITextAttribute> TextAttribute::SUPERSCRIPT = sInit(String("superscript") );

const AutoPtr<ITextAttribute> TextAttribute::SWAP_COLORS = sInit(String("swap_colors") );

const Boolean TextAttribute::SWAP_COLORS_ON = TRUE;

const AutoPtr<ITextAttribute> TextAttribute::TRACKING = sInit(String("tracking") );

const Float TextAttribute::TRACKING_LOOSE = -1.0F;

const Float TextAttribute::TRACKING_TIGHT = -1.0F;

const AutoPtr<ITextAttribute> TextAttribute::TRANSFORM = sInit(String("transform") );

const AutoPtr<ITextAttribute> TextAttribute::UNDERLINE = sInit(String("underline") );

const AutoPtr<ITextAttribute> TextAttribute::WEIGHT = sInit(String("weight") );

const Float TextAttribute::WEIGHT_EXTRA_LIGHT = 0.5F;

const Float TextAttribute::WEIGHT_LIGHT = 0.75F;

const Float TextAttribute::WEIGHT_DEMILIGHT = 0.875F;

const Float TextAttribute::WEIGHT_REGULAR = 1.0F;

const Float TextAttribute::WEIGHT_SEMIBOLD = 1.25F;

const Float TextAttribute::WEIGHT_MEDIUM = 1.5F;

const Float TextAttribute::WEIGHT_DEMIBOLD = 1.75F;

const Float TextAttribute::WEIGHT_BOLD = 2.0F;

const Float TextAttribute::WEIGHT_HEAVY = 2.25F;

const Float TextAttribute::WEIGHT_EXTRABOLD = 2.5F;

const Float TextAttribute::WEIGHT_ULTRABOLD = 2.75F;

const AutoPtr<ITextAttribute> TextAttribute::WIDTH = sInit(String("width") );

const Float TextAttribute::WIDTH_CONDENSED = 0.75F;

const Float TextAttribute::WIDTH_SEMI_CONDENSED = 0.875F;

const Float TextAttribute::WIDTH_REGULAR = 1.0F;

const Float TextAttribute::WIDTH_SEMI_EXTENDED = 1.25F;

const Float TextAttribute::WIDTH_EXTENDED = 1.5F;

PInterface TextAttribute::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

ECode TextAttribute::Init(
    /* [in] */ String name)
{
    AttributedCharacterIteratorAttribute::Init(name);
    AutoPtr<ITextAttribute> attribute = reinterpret_cast<ITextAttribute * >(this->Probe(EIID_ITextAttribute));
    Pair<String, AutoPtr<ITextAttribute> > value(name, attribute);
    attrMap->Insert(value);
    return NOERROR;
}

ECode TextAttribute::ReadResolve(
    /* [out] */ IInterface** resolve)
{
    String name = String("");
    GetName(&name);
    Map<String, ITextAttribute*>::Iterator it = attrMap->Find(name);
    AutoPtr<ITextAttribute> result;
    if (it != attrMap->End()) {
        result = it->mSecond;
    }
    if (result != NULL) {
        *resolve = (IInterface*)result;
        return NOERROR;
    }
    //throw new InvalidObjectException("Unknown attribute name");
    return NOERROR;
}