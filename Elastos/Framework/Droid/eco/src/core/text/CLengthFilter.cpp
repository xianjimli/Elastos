
#include "text/CLengthFilter.h"
#include "ext/frameworkdef.h"

using namespace Elastos::Core;

CLengthFilter::CLengthFilter()
    : mMax(0)
{}

ECode CLengthFilter::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);

    Int32 len;
    dest->GetLength(&len);
    Int32 keep = mMax - (len - (dend - dstart));

    if (keep <= 0) {
        return CStringWrapper::New(String(""), cs);
    }
    else if (keep >= end - start) {
        *cs = NULL;
        return NOERROR; // keep original
    }
    else {
        return source->SubSequence(start, start + keep, cs);
    }
}

ECode CLengthFilter::constructor(
    /* [in] */ Int32 max)
{
    mMax = max;
    return NOERROR;
}
