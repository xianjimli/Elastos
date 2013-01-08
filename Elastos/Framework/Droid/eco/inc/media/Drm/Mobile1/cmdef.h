
#ifndef __CMDEF_H__
#define __CMDEF_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <Elastos.Core.h>

#define FAIL_RETURN(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return ec; \
    } while(0);

#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));
#define VALIDATE_NOT_NULL(x) if (!x) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }

#endif //__CMDEF_H__
