
#include "text/CAllCaps.h"
#include "text/TextUtils.h"
#include "text/CSpannableString.h"
#include "ext/frameworkdef.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

ECode CAllCaps::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);

    for (Int32 i = start; i < end; i++) {
        Char32 c;
        source->GetCharAt(i, &c);
        if (Character::IsLowerCase(c)) {
            ArrayOf<Char8>* v = ArrayOf<Char8>::Alloc(end - start);
            TextUtils::GetChars(source, start, end, v, 0);
            String s(v->GetPayload());
            s.ToUpperCase();

            AutoPtr<ICharSequence> charS;
            FAIL_RETURN(CStringWrapper::New(s, (ICharSequence**)&charS));
            if (source->Probe(EIID_ISpanned) != NULL) {
                AutoPtr<ISpannable> sp;
                FAIL_RETURN(CSpannableString::New(charS, (ISpannable**)&sp));
                TextUtils::CopySpansFrom((ISpanned*)source->Probe(EIID_ISpanned),
                        start, end, EIID_IInterface, sp, 0);
                *cs = (ICharSequence*)sp->Probe(EIID_ICharSequence);
                if (*cs != NULL) {
                    (*cs)->AddRef();
                }
                return NOERROR;
            }
            else {
                *cs = charS;
                if (*cs != NULL) {
                    (*cs)->AddRef();
                }
                return NOERROR;
            }
        }
    }
}
