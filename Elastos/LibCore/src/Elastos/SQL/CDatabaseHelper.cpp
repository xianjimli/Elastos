
#include "CDatabaseHelper.h"
#include "Database.h"
#include "cmdef.h"

ECode CDatabaseHelper::Complete(
    /* [in] */ const String& sql,
    /* [out] */ Boolean* result)
{
	VALIDATE_NOT_NULL(result);

    return Database::Complete(sql, result);
}