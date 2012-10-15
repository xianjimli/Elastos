
#include "text/CEditableFactory.h"
#include "text/CSpannableStringBuilder.h"

ECode CEditableFactory::NewEditable(
        /* [in] */ ICharSequence* source,
        /* [out] */ IEditable** editable)
{
    return CSpannableStringBuilder::New(
        source, (ISpannableStringBuilder**)editable);
}
