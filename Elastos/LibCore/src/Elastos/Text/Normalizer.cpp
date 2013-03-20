#include "Normalizer.h"

Boolean Normalizer::IsNormalized(
        /* [in] */ ICharSequence* src,
        /* [in] */ Form form)
{
//    return NativeNormalizer.isNormalized(src, form);
    String str = String("");
    src->ToString(&str);
    return Native_isNormalizedImpl(str, ToUNormalizationMode(form));
}

String Normalizer::Normalize(
        /* [in] */ ICharSequence* src,
        /* [in] */ Form form)
{
//    return NativeNormalizer.normalize(src, form);
    String str = String("");
    src->ToString(&str);
    return Native_normalizeImpl(str, ToUNormalizationMode(form));
}

Normalizer::Normalizer(){}

Int32 Normalizer::ToUNormalizationMode(
    /* [in] */ Form form)
{
    //Translate Java enum constants to ICU int constants.
    //See UNormalizationMode in "unicode.unorm.h".stable API since ICU 2.0.
    switch(form) {
        case NFC: return 4;
        case NFD: return 2;
        case NFKC: return 5;
        case NFKD: return 3;
    }
    //throw new AssertionError("unknown Normalizer.Form " + form);
}

String Normalizer::Native_normalizeImpl(
    /* [in] */ String src,
    /* [in] */ Int32 form)
{
    return String("");
}

Boolean Normalizer::Native_isNormalizedImpl(
    /* [in] */ String src,
    /* [in] */ Int32 form)
{
    return FALSE;
}