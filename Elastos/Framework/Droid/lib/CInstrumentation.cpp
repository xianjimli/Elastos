
#include "CInstrumentation.h"

ECode CInstrumentation::NewApplication(
    /* [in] */ String moduleName,
    /* [in] */ String className, 
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

