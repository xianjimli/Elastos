#include "TextAttribute.h"

const AutoPtr<ITextAttribute> TextAttribute::BACKGROUND;// = new TextAttribute("background");

const AutoPtr<ITextAttribute> TextAttribute::BIDI_EMBEDDING;// = new TextAttribute("bidi_embedding");

const AutoPtr<ITextAttribute> TextAttribute::CHAR_REPLACEMENT;// = new TextAttribute("char_replacement");

const AutoPtr<ITextAttribute> TextAttribute::FAMILY;// = new TextAttribute("family");

const AutoPtr<ITextAttribute> TextAttribute::FONT;// = new TextAttribute("font");

const AutoPtr<ITextAttribute> TextAttribute::FOREGROUND;// = new TextAttribute("foreground");

const AutoPtr<ITextAttribute> TextAttribute::INPUT_METHOD_HIGHLIGHT;// = new TextAttribute(
        //"input method highlight");

const AutoPtr<ITextAttribute> TextAttribute::INPUT_METHOD_UNDERLINE;// = new TextAttribute(
        //"input method underline");

const AutoPtr<ITextAttribute> TextAttribute::JUSTIFICATION;// = new TextAttribute("justification");

const Float TextAttribute::JUSTIFICATION_FULL = 1.0F;

const Float TextAttribute::JUSTIFICATION_NONE = 0.0F;

const AutoPtr<ITextAttribute> TextAttribute::KERNING;// = new TextAttribute("kerning")

const AutoPtr<ITextAttribute> TextAttribute::LIGATURES;// = new TextAttribute("ligatures");

const AutoPtr<ITextAttribute> TextAttribute::NUMERIC_SHAPING;// = new TextAttribute("numeric_shaping");

const AutoPtr<ITextAttribute> TextAttribute::POSTURE;// = new TextAttribute("posture");

const Float TextAttribute::POSTURE_REGULAR = 0.0F;

const Float TextAttribute::POSTURE_OBLIQUE = 0.20F;

const AutoPtr<ITextAttribute> TextAttribute::RUN_DIRECTION;// = new TextAttribute("run_direction");

const Boolean TextAttribute::RUN_DIRECTION_LTR = FALSE;

const Boolean TextAttribute::RUN_DIRECTION_RTL = TRUE;

const AutoPtr<ITextAttribute> TextAttribute::SIZE;// = new TextAttribute("size");

const AutoPtr<ITextAttribute> TextAttribute::STRIKETHROUGH;// = new TextAttribute("strikethrough");

const Boolean TextAttribute::STRIKETHROUGH_ON = TRUE;

const AutoPtr<ITextAttribute> TextAttribute::SUPERSCRIPT;// = new TextAttribute("superscript");

const AutoPtr<ITextAttribute> TextAttribute::SWAP_COLORS;// = new TextAttribute("swap_colors");

const Boolean TextAttribute::SWAP_COLORS_ON = TRUE;

const AutoPtr<ITextAttribute> TextAttribute::TRACKING;// = new TextAttribute("tracking");

const Float TextAttribute::TRACKING_LOOSE = -1.0F;

const Float TextAttribute::TRACKING_TIGHT = -1.0F;

const AutoPtr<ITextAttribute> TextAttribute::TRANSFORM;// = new TextAttribute("transform");

const AutoPtr<ITextAttribute> TextAttribute::UNDERLINE;// = new TextAttribute("underline");

const AutoPtr<ITextAttribute> TextAttribute::WEIGHT;// = new TextAttribute("weight");

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

const AutoPtr<ITextAttribute> TextAttribute::WIDTH;// = new TextAttribute("width");

const Float TextAttribute::WIDTH_CONDENSED = 0.75F;

const Float TextAttribute::WIDTH_SEMI_CONDENSED = 0.875F;

const Float TextAttribute::WIDTH_REGULAR = 1.0F;

const Float TextAttribute::WIDTH_SEMI_EXTENDED = 1.25F;

const Float TextAttribute::WIDTH_EXTENDED = 1.5F;

ECode TextAttribute::Init(
        /* [in] */ String name)
{
    AttributedCharacterIteratorAttribute::Init(name);
    //Pair<String, IAutoPtr<ITextAttribute> > value(name, this);
    //attrMap->Insert(value);
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