
#include "text/CDirections.h"
#include "ext/frameworkdef.h"

CDirections::~CDirections()
{
    ArrayOf<Int16>::Free(mDirections);
}

ECode CDirections::GetDirections(
    /* [out] */ ArrayOf<Int16>** directions)
{
    VALIDATE_NOT_NULL(directions);

    *directions = mDirections != NULL? mDirections->Clone() : NULL;
    return NOERROR;
}

ECode CDirections::constructor(
    /* [in] */ const ArrayOf<Int16>& dirs)
{
    mDirections = dirs.Clone();
    return NOERROR;
}
