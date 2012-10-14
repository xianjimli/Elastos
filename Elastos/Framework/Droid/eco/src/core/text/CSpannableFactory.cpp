
#include "text/CSpannableFactory.h"
#include "text/CSpannableString.h"

ECode CSpannableFactory::NewSpannable(
        /* [in] */ ICharSequence* source,
        /* [out] */ ISpannable** spannable)
{
    return CSpannableString::New(source, spannable);
}
