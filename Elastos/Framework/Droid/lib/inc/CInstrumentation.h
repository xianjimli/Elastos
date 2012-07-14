
#ifndef __CINSTRUMENTATION_H__
#define __CINSTRUMENTATION_H__

#include <Elastos.Framework.h>
#include "CBaseObject.h"

using namespace Elastos;

class CInstrumentation : public CBaseObject, public IInstrumentation
{
public:
    CARAPI NewApplication(
        /* [in] */ const String& moduleName,
        /* [in] */ const String& className, 
        /* [in] */ IContext* ctx, 
        /* [out] */ IApplication** app);
            
    CARAPI CallActivityOnNewIntent(
        /* [in] */ IActivity *activity,
        /* [in] */ IIntent *intent);

private:
    
};

#endif // __CINSTRUMENTATION_H__
