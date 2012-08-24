
#ifndef __FBREADERDEF_H__
#define __FBREADERDEF_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }

#define FAIL_RETURN(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return ec; \
    } while(0);

#endif //__FBREADERDEF_H__
