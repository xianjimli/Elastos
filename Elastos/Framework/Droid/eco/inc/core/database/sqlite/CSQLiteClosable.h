
#ifndef __CSQLITECLOSABLE_H__
#define __CSQLITECLOSABLE_H__

#include "_CSQLiteClosable.h"

CarClass(CSQLiteClosable)
{
public:
    CARAPI OnAllReferencesReleased();

    CARAPI OnAllReferencesReleasedFromContainer();

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

private:
    // TODO: Add your private member variables here.
};

#endif // __CSQLITECLOSABLE_H__
