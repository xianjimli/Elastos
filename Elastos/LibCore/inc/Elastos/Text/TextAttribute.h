#ifndef __TEXTATTRIBUTE_H__
#define __TEXTATTRIBUTE_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include "AttributedCharacterIteratorAttribute.h"
#include <elastos/Map.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class TextAttribute : public AttributedCharacterIteratorAttribute
{
protected:
    /**
     * Instantiates a new TextAttribute with the specified name.
     *
     * @param name the name of TextAttribute.
     */
    CARAPI Init(
        /* [in] */ String name);

    /**
     * Resolves the instance being deserialized.
     *
     * @return the Object.
     *
     * @throws InvalidObjectException the InvalidObjectException.
     */
    //@Override
    CARAPI ReadResolve(
        /* [out] */ IInterface** resolve);
public:
    /**
     * The BACKGROUND text attribute.
     */
    static const AutoPtr<ITextAttribute> BACKGROUND;// = new TextAttribute("background");

    /** The BIDI_EMBEDDING text attribute key. */
    static const AutoPtr<ITextAttribute> BIDI_EMBEDDING;// = new TextAttribute("bidi_embedding");

    /** The CHAR_REPLACEMENT text attribute key. */
    static const AutoPtr<ITextAttribute> CHAR_REPLACEMENT;// = new TextAttribute("char_replacement");

    /** The FAMILY text attribute key. */
    static const AutoPtr<ITextAttribute> FAMILY;// = new TextAttribute("family");

    /** The FONT text attribute key. */
    static const AutoPtr<ITextAttribute> FONT;// = new TextAttribute("font");

    /** The FOREGROUND text attribute key. */
    static const AutoPtr<ITextAttribute> FOREGROUND;// = new TextAttribute("foreground");

    /** The INPUT_METHOD_HIGHLIGHT text attribute key. */
    static const AutoPtr<ITextAttribute> INPUT_METHOD_HIGHLIGHT;// = new TextAttribute(
            //"input method highlight");

    /** The INPUT_METHOD_UNDERLINE text attribute key. */
    static const AutoPtr<ITextAttribute> INPUT_METHOD_UNDERLINE;// = new TextAttribute(
            //"input method underline");

    /** The JUSTIFICATION text attribute key. */
    static const AutoPtr<ITextAttribute> JUSTIFICATION;// = new TextAttribute("justification");

    /** The Constant JUSTIFICATION_indicates the full requested width. */
    static const Float JUSTIFICATION_FULL;

    /**
     * The Constant JUSTIFICATION_NONE indicates that line is not
     * allowed for justification.
     */
    static const Float JUSTIFICATION_NONE;

    /**
     * The KERNING text attribute key.
     *
     * @since 1.6
     */
    static const AutoPtr<ITextAttribute> KERNING;// = new TextAttribute("kerning");

    /** @since 1.6 */
    static const Int32 KERNING_ON = 1;

    /**
     * The LIGATURES text attribute key.
     *
     * @since 1.6
     */
    static const AutoPtr<ITextAttribute> LIGATURES;// = new TextAttribute("ligatures");

    /** @since 1.6 */
    static const Int32 LIGATURES_ON = 1;

    /** The NUMERIC_SHAPING text attribute. */
    static const AutoPtr<ITextAttribute> NUMERIC_SHAPING;// = new TextAttribute("numeric_shaping");

    /** The POSTURE text attribute. */
    static const AutoPtr<ITextAttribute> POSTURE;// = new TextAttribute("posture");

    /** The Constant POSTURE_REGULAR indicates regular posture. */
    static const Float POSTURE_REGULAR;// = 0.0F;

    /** The Constant POSTURE_OBLIQUE indicates italic posture. */
    static const Float POSTURE_OBLIQUE;// = 0.20F;

    /** The RUN_DIRECTION text attribute. */
    static const AutoPtr<ITextAttribute> RUN_DIRECTION;// = new TextAttribute("run_direction");

    /**
     * The Constant RUN_DIRECTION_LTR indicates left-to-right run
     * direction.
     */
    static const Boolean RUN_DIRECTION_LTR;

    /**
     * The Constant RUN_DIRECTION_RTL indicates right-to-left run
     * direction.
     */
    static const Boolean RUN_DIRECTION_RTL;

    /** The SIZE text attribute. */
    static const AutoPtr<ITextAttribute> SIZE;// = new TextAttribute("size");

    /** The STRIKETHROUGH text attribute. */
    static const AutoPtr<ITextAttribute> STRIKETHROUGH;// = new TextAttribute("strikethrough");

    /** The Constant STRIKETHROUGH_ON indicates a single strikethrough. */
    static const Boolean STRIKETHROUGH_ON;

    /** The SUPERSCRIPT text attribute. */
    static const AutoPtr<ITextAttribute> SUPERSCRIPT;// = new TextAttribute("superscript");

    /** The Constant SUPERSCRIPT_SUB indicates a standard subscript. */
    static const Int32 SUPERSCRIPT_SUB = -1;

    /** The Constant SUPERSCRIPT_SUPER indicates a standard superscript. */
    static const Int32 SUPERSCRIPT_SUPER = 1;

    /** The SWAP_COLORS text attribute. */
    static const AutoPtr<ITextAttribute> SWAP_COLORS;// = new TextAttribute("swap_colors");

    /**
     * The Constant SWAP_COLORS_ON indicates a swap of foreground
     * and background.
     */
    static const Boolean SWAP_COLORS_ON;

    /**
     * The TRACKING text attribute key.
     *
     * @since 1.6
     */
    static const AutoPtr<ITextAttribute> TRACKING;// = new TextAttribute("tracking");

    /** @since 1.6 */
    static const Float TRACKING_LOOSE;
    /** @since 1.6 */
    static const Float TRACKING_TIGHT;

    /** The TRANSFORM text attribute. */
    static const AutoPtr<ITextAttribute> TRANSFORM;// = new TextAttribute("transform");

    /** The Constant UNDERLINE text attribute. */
    static const AutoPtr<ITextAttribute> UNDERLINE;// = new TextAttribute("underline");

    /**
     * The Constant UNDERLINE_ON indicates a standard underline
     * at the roman baseline for roman text.
     */
    static const Int32 UNDERLINE_ON = 0;

    /**
     * The Constant UNDERLINE_LOW_ONE_PIXEL indicates a single
     * pixel solid low underline.
     */
    static const Int32 UNDERLINE_LOW_ONE_PIXEL = 1;

    /**
     * The Constant UNDERLINE_LOW_TWO_PIXEL indicates a double
     * pixel solid low underline.
     */
    static const Int32 UNDERLINE_LOW_TWO_PIXEL = 2;

    /**
     * The Constant UNDERLINE_LOW_DOTTED indicates a
     * single pixel dotted low underline.
     */
    static const Int32 UNDERLINE_LOW_DOTTED = 3;

    /**
     * The Constant UNDERLINE_LOW_GRAY indicates double pixel
     * gray low underline.
     */
    static const Int32 UNDERLINE_LOW_GRAY = 4;

    /**
     * The Constant UNDERLINE_LOW_DASHED indicates single pixel dashed
     * low underline.
     */
    static const Int32 UNDERLINE_LOW_DASHED = 5;

    /** The WEIGHT text attribute. */
    static const AutoPtr<ITextAttribute> WEIGHT;// = new TextAttribute("weight");

    /**
     * The Constant WEIGHT_EXTRA_LIGHT indicates the lightest
     * predefined weight.
     */
    static const Float WEIGHT_EXTRA_LIGHT;

    /**
     * The Constant WEIGHT_LIGHT indicates the standard light weight.
     */
    static const Float WEIGHT_LIGHT;

    /**
     * The Constant WEIGHT_DEMILIGHT indicates an intermediate weight
     * between LIGHT and STANDARD.
     */
    static const Float WEIGHT_DEMILIGHT;

    /**
     * The Constant WEIGHT_REGULAR indicates the standart weight.
     */
    static const Float WEIGHT_REGULAR;

    /**
     * The Constant WEIGHT_SEMIBOLD indicates a semi weight
     * of REGULAR.
     */
    static const Float WEIGHT_SEMIBOLD;

    /** The Constant WEIGHT_MEDIUM indicates average weight
     * between the REGULAR and BOLD. */
    static const Float WEIGHT_MEDIUM;

    /** The Constant WEIGHT_DEMIBOLD indicates
     * a lighter weight than BOLD. */
    static const Float WEIGHT_DEMIBOLD;

    /** The Constant WEIGHT_BOLD indicates the standard bold weight. */
    static const Float WEIGHT_BOLD;

    /** The Constant WEIGHT_HEAVY indicates a heavier weight than BOLD. */
    static const Float WEIGHT_HEAVY;

    /** The Constant WEIGHT_EXTRABOLD indicates an extra heavy weight. */
    static const Float WEIGHT_EXTRABOLD;

    /**
     * The Constant WEIGHT_ULTRABOLD indicates the heaviest predefined
     * weight.
     */
    static const Float WEIGHT_ULTRABOLD;

    /** The WIDTH text attribute. */
    static const AutoPtr<ITextAttribute> WIDTH;// = new TextAttribute("width");

    /**
     * The Constant WIDTH_CONDENSED indicates the most condensed
     * predefined width.
     */
    static const Float WIDTH_CONDENSED;

    /**
     * The Constant WIDTH_SEMI_CONDENSED indicates
     * a semi condensed width.
     */
    static const Float WIDTH_SEMI_CONDENSED;

    /**
     * The Constant WIDTH_REGULAR indicates the standard width.
     */
    static const Float WIDTH_REGULAR;

    /**
     * The Constant WIDTH_SEMI_EXTENDED indicates semi extended width.
     */
    static const Float WIDTH_SEMI_EXTENDED;

    /**
     * The Constant WIDTH_EXTENDED indicates extended width.
     */
    static const Float WIDTH_EXTENDED;

private:
    /** The Constant serialVersionUID. */
    //private static final long serialVersionUID = 7744112784117861702L;

    // set of available text attributes
    /** The Constant attrMap. */
    Map<String, ITextAttribute* >* attrMap;// = new HashMap<String, TextAttribute>();
};
#endif //__TEXTATTRIBUTE_H__