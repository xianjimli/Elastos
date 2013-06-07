
#include "CBidiHelper.h"
#include "NativeBidi.h"
#include "CBidi.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>

ECode CBidiHelper::ReorderVisually(
    /* [in] */ ArrayOf<Byte>* levels,
    /* [in] */ Int32 levelStart,
    /* [in] */ ArrayOf<IInterface*>* objects,
    /* [in] */ Int32 objectStart,
    /* [in] */ Int32 count)
{
    if (count < 0 || levelStart < 0 || objectStart < 0
            || count > levels->GetLength() - levelStart
            || count > objects->GetLength() - objectStart) {
        //throw new IllegalArgumentException("Invalid ranges (levels=" + levels.length +
        //        ", levelStart=" + levelStart + ", objects=" + objects.length +
        //        ", objectStart=" + objectStart + ", count=" + count + ")");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoFree< ArrayOf<Byte> > realLevels = ArrayOf<Byte>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        (*realLevels)[i] = (*levels)[levelStart + i];
    }

    AutoFree< ArrayOf<Int32> > indices = NativeBidi::Ubidi_reorderVisual(realLevels, count);

    AutoFree< ArrayOf<AutoPtr<IInterface> > > result =
            ArrayOf<AutoPtr<IInterface> >::Alloc(count);
    Int32 i = 0;
    for (; i < count; i++) {
        (*result)[i] = (*objects)[objectStart + (*indices)[i]];
    }

    for (i = 0; i < count; i++) {
        (*objects)[objectStart + i] = (*result)[i];
    }

    return NOERROR;
}

ECode CBidiHelper::RequiresBidi(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [out] */ Boolean * result)
{
    if (limit < 0 || start < 0 || start > limit || limit > text->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IBidi> bidi;
    CBidi::New(text, start, NULL, 0, limit - start, 0, (IBidi**)&bidi);
    Boolean isLefttoRight;
    bidi->IsLeftToRight(&isLefttoRight);
    *result = !isLefttoRight;
    return NOERROR;
}
