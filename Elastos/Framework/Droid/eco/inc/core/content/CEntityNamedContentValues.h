
#ifndef __CENTITYNAMEDCONTENTVALUES_H__
#define __CENTITYNAMEDCONTENTVALUES_H__

#include "_CEntityNamedContentValues.h"
#include <elastos/AutoPtr.h>

CarClass(CEntityNamedContentValues)
{
public:
	CEntityNamedContentValues();

	~CEntityNamedContentValues();
	
    CARAPI GetUri(
        /* [out] */ IUri** uri);

    CARAPI GetValues(
        /* [out] */ IContentValues** values);

    CARAPI constructor(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values);

private:
	AutoPtr<IUri> mUri;
	AutoPtr<IContentValues> mValues;
    
};

#endif // __CENTITYNAMEDCONTENTVALUES_H__
