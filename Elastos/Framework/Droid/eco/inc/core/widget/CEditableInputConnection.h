
#ifndef __CEDITABLEINPUTCONNECTION_H__
#define __CEDITABLEINPUTCONNECTION_H__

#include "ext/frameworkext.h"
#include "_CEditableInputConnection.h"
#include "view/inputmethod/BaseInputConnection.h"
#include "view/inputmethod/BaseInputConnectionMacro.h"
#include "widget/EditableInputConnection.h"


CarClass(CEditableInputConnection), public EditableInputConnection
{
public:
    BASEINPUTCONNECTION_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ ITextView* textview);
};

#endif //__CEDITABLEINPUTCONNECTION_H__
