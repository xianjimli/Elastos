
#ifndef __CMDEF_H__
#define __CMDEF_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#define FAIL_RETURN(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return ec; \
    } while(0);

#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));
#define VALIDATE_NOT_NULL(x) if (!x) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }

#define FREE_ARRAY_OF_STRING(var) \
    if (var != NULL) { \
        for (Int32 i = 0; i < var->GetLength(); ++i) { \
            (*var)[i] = NULL; \
        } \
        ArrayOf<String>::Free(var); \
        var = NULL; \
    }

#endif //__CMDEF_H__
