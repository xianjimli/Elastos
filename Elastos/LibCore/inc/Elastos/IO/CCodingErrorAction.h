
#ifndef __CCODINGERRORACTION_H__
#define __CCODINGERRORACTION_H__

#include "_CCodingErrorAction.h"
#include <elastos/AutoPtr.h>

CarClass(CCodingErrorAction) {
public:
    CARAPI GetIGNORE(
        /* [out] */ ICodingErrorAction** ignore);

    CARAPI GetREPLACE(
        /* [out] */ ICodingErrorAction** replace);

    CARAPI GetREPORT(
        /* [out] */ ICodingErrorAction** report);

    CARAPI ToString(
        /* [out] */ String* toString);

private:

    friend AutoPtr<ICodingErrorAction> CreateAction(
        /* [in] */ String action);

private:
    static const AutoPtr<ICodingErrorAction> IGNORE;

    static const AutoPtr<ICodingErrorAction> REPLACE;

    static const AutoPtr<ICodingErrorAction> REPORT;

private:
    String mAction;
};

#endif // __CCODINGERRORACTION_H__
