
#include "CInstrumentation.h"

ECode CInstrumentation::NewApplication(
    /* [in] */ const String& moduleName,
    /* [in] */ const String& className, 
    /* [in] */ IContext* ctx, 
    /* [out] */ IApplication** app)
{
    if (app == NULL) return E_INVALID_ARGUMENT;
    
    *app = NULL;
    return NOERROR;
//    Application app = (Application)clazz.newInstance();
//    app.attach(context);
//    return app;
}

ECode CInstrumentation::CallActivityOnNewIntent(
    /* [in] */ IActivity *activity,
    /* [in] */ IIntent *intent)
{
    return activity->DispatchNewIntent(intent);
}

