
#include "cmdef.h"
#include "CArray.h"


CArray::CArray()
    : mElements(NULL)
{}

CArray::~CArray()
{
    if (mElements != NULL) {
        for (Int32 i = 0; i < mElements->GetLength(); ++i) {
            (*mElements)[i] = NULL;
        }
        ArrayOf< AutoPtr<IInterface> >::Free(mElements);
    }
}

ECode CArray::constructor(
    /* [in] */ Int32 size)
{
    if (size < 0) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    mElements = ArrayOf< AutoPtr<IInterface> >::Alloc(size);
    return NOERROR;
}

ECode CArray::GetLength(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    *size = mElements == NULL ? -1 : mElements->GetLength();
    return NOERROR;
}

ECode CArray::Get(
    /* [in] */ Int32 index,
    /* [out] */ IInterface** element)
{
    VALIDATE_NOT_NULL(element);

    if (index < 0 || mElements == NULL || index >= mElements->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *element = (*mElements)[index];
    if (*element != NULL) (*element)->AddRef();
    return NOERROR;
}

ECode CArray::Put(
    /* [in] */ Int32 index,
    /* [in] */ IInterface* element)
{
    if (index < 0 || mElements == NULL || index >= mElements->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    (*mElements)[index] = element;
    return NOERROR;
}
