
#include "content/CEntityNamedContentValues.h"
#include "ext/frameworkext.h"

CEntityNamedContentValues::CEntityNamedContentValues()
{}

CEntityNamedContentValues::~CEntityNamedContentValues()
{}

ECode CEntityNamedContentValues::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = mUri;
    return NOERROR;
}

ECode CEntityNamedContentValues::GetValues(
    /* [out] */ IContentValues** values)
{
    VALIDATE_NOT_NULL(values);
    *values = mValues;
    return NOERROR;
}

ECode CEntityNamedContentValues::constructor(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values)
{
    mUri = uri;
    mValues = values;
    return NOERROR;
}

