
#include "database/sqlite/CSQLiteClosable.h"
ECode CSQLiteClosable::OnAllReferencesReleased()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteClosable::OnAllReferencesReleasedFromContainer()
{
    return SQLiteClosable::OnAllReferencesReleasedFromContainer();
}

ECode CSQLiteClosable::AcquireReference()
{
    return SQLiteClosable::AcquireReference();
}

ECode CSQLiteClosable::ReleaseReference()
{
    return SQLiteClosable::ReleaseReference();
}

ECode CSQLiteClosable::ReleaseReferenceFromContainer()
{
    return SQLiteClosable::ReleaseReferenceFromContainer();
}

