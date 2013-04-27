
#include "content/CEntity.h"
#include "content/CEntityNamedContentValues.h"
#include "ext/frameworkext.h"
#include "utils/CParcelableObjectContainer.h"

CEntity::CEntity()
    : mSubValues(NULL)
{}

CEntity::~CEntity()
{
    if(NULL != mSubValues){
        mSubValues->Clear();
        delete mSubValues;
    }
}

ECode CEntity::GetEntityValues(
    /* [out] */ IContentValues** contentValues)
{
    VALIDATE_NOT_NULL(contentValues);
    *contentValues = mValues;
    return NOERROR;
}

ECode CEntity::GetSubValues(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(CParcelableObjectContainer::New(result));

    if (NULL != mSubValues) {
        List<AutoPtr<IEntityNamedContentValues> >::Iterator iter = mSubValues->Begin();
        for (; iter != mSubValues->End(); ++iter) {
            AutoPtr<IEntityNamedContentValues> namedContentValues = *iter;
            (*result)->Add(namedContentValues.Get());
        }
    }

    return NOERROR;
}

ECode CEntity::AddSubValue(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* contentValues)
{
    AutoPtr<IEntityNamedContentValues> namedContentValues;
    FAIL_RETURN(CEntityNamedContentValues::New(uri, contentValues, (IEntityNamedContentValues**)&namedContentValues));
    mSubValues->PushBack(namedContentValues);
    return NOERROR;
}

ECode CEntity::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    String str("Entity: ");
    String strValues("null");

    if (NULL != mValues) {
        FAIL_RETURN(mValues->ToString(&strValues));
    }

    str.Append(strValues);

    if (NULL != mSubValues) {
        List<AutoPtr<IEntityNamedContentValues> >::Iterator iter = mSubValues->Begin();

        for (; iter != mSubValues->End(); ++iter) {
            AutoPtr<IEntityNamedContentValues> namedContentValues = *iter;
            String strUri("null");
            String strContentValues("null");
            str.Append("\n  ");
            AutoPtr<IUri> uri;
            AutoPtr<IContentValues> contentValues;
            FAIL_RETURN(namedContentValues->GetUri((IUri**)&uri));
            FAIL_RETURN(namedContentValues->GetValues((IContentValues**)&contentValues));

            if (NULL != uri) {
                FAIL_RETURN(uri->ToString(&strUri));
            }

            if (NULL != contentValues) {
                FAIL_RETURN(contentValues->ToString(&strContentValues));
            }

            str.Append(strUri);
            str.Append("\n  -> ");
            str.Append(strContentValues);
        }

    }

    *result = str;
    return NOERROR;
}

ECode CEntity::constructor(
    /* [in] */ IContentValues* values)
{
    mValues = values;
    mSubValues = new List<AutoPtr<IEntityNamedContentValues> >();
    return NOERROR;
}

