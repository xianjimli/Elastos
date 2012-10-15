
#ifndef __CEDITABLEFACTORY_H__
#define __CEDITABLEFACTORY_H__

#include "_CEditableFactory.h"

CarClass(CEditableFactory)
{
public:
    CARAPI NewEditable(
        /* [in] */ ICharSequence* source,
        /* [out] */ IEditable** editable);
};

#endif //__CEDITABLEFACTORY_H__