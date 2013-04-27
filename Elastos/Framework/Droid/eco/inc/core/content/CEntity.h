
#ifndef __CENTITY_H__
#define __CENTITY_H__
#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CEntity.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

CarClass(CEntity)
{
public:
    CEntity();

    ~CEntity();

    CARAPI GetEntityValues(
        /* [out] */ IContentValues** contentValues);

    CARAPI GetSubValues(
        /* [out] */ IObjectContainer** result);

    CARAPI AddSubValue(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* contentValues);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI constructor(
        /* [in] */ IContentValues* values);

private:
    AutoPtr<IContentValues> mValues;
    List<AutoPtr<IEntityNamedContentValues> >* mSubValues;
};

#endif // __CENTITY_H__
