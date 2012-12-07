
#include "CNetPermission.h"

const Int64 CNetPermission::sSerialVersionUID = -8343910153355041693L;

ECode CNetPermission::constructor(
    /* [in] */ const String& name)
{
    return BasicPermission::Init(name);
}

ECode CNetPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    return BasicPermission::Init(name, action);
}
