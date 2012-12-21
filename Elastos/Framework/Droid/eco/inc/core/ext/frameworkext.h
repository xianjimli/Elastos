
#ifndef __FRAMEWORKEXT_H__
#define __FRAMEWORKEXT_H__

#include "ext/frameworkdef.h"
#include "ext/frameworkerr.h"
#ifdef _FRAMEWORK_CORE
#include "Elastos.Framework.Core_server.h"
#elif defined(_FRAMEWORK_SERVER)
#include "Elastos.Framework.Server_server.h"
#elif defined(_FRAMEWORK_MEDIA)
#include "Elastos.Framework.Media_server.h"
#endif
#include "_frameworkext.h"

#endif //__FRAMEWORKEXT_H__
