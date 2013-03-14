
#include "app/backup/RestoreObserver.h"

RestoreObserver::RestoreObserver()
{

}

RestoreObserver::~RestoreObserver()
{

}

void RestoreObserver::RestoreSetsAvailable(
    /* [in] */ ArrayOf<IRestoreSet*>* result)
{
}

void RestoreObserver::RestoreStarting(
    /* [in] */ Int32 numPackages)
{
}

void RestoreObserver::OnUpdate(
    /* [in] */ Int32 nowBeingRestored,
    /* [in] */ const String& currentPackage)
{
}

void RestoreObserver::RestoreFinished(
    /* [in] */ Int32 error)
{
}
